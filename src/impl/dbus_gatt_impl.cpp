// MIT License
//
// Copyright (c) 2020 RomanSo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "dbus_gatt_impl.h"

namespace dbus_gatt {

static std::mutex G_D_BUS_GATT_IMPL_MUTEX_;

DBusGATTImpl& DBusGATTImpl::create(std::string app_name, std::string app_path) {
    std::lock_guard<std::mutex> lock(G_D_BUS_GATT_IMPL_MUTEX_);
    static DBusGATTImpl impl = DBusGATTImpl(std::move(app_name), std::move(app_path));
    return impl;
}

DBusGATTImpl::DBusGATTImpl(std::string app_name, std::string app_path)
: app_name_(std::move(app_name)), managed_objects_{app_path, app_path} {
    addDevicePropertyChangedCallback(EDeviceProperty::Connected,
                                     [self = this](const DBusGattVariantT& value) {
                                         try {
                                             auto bool_value = std::get<bool>(value);
                                             if(bool_value) {
                                                 self->deviceConnect();
                                             } else {
                                                 self->deviceDisConnect();
                                             }
                                         } catch(std::bad_variant_access&) {}
                                     });

    init();
}

DBusGATTImpl::~DBusGATTImpl() {
    if(p_adpt_api_proxy_) {
        g_object_unref(p_adpt_api_proxy_);
    }
    if(p_gatt_mgr_proxy_) {
        g_object_unref(p_gatt_mgr_proxy_);
    }
    if(p_objs_mgr_proxy_) {
        g_object_unref(p_objs_mgr_proxy_);
    }
    if(p_object_mgr_) {
        g_object_unref(p_object_mgr_);
    }

    for(const auto id: registered_objects_id_) {
        g_dbus_connection_unregister_object(p_dbus_conn_, id);
    }

    for(const auto id: subscribe_signals_id_) {
        g_dbus_connection_signal_unsubscribe(p_dbus_conn_, id);
    }

    if(p_dbus_conn_) {
        g_object_unref(p_dbus_conn_);
    }
    if(p_main_loop_) {
        g_main_loop_unref(p_main_loop_);
    }
}

std::shared_ptr<DBusObject> DBusGATTImpl::addService(
    std::string name,
    std::shared_ptr<DBusGattServiceImpl> p_service) {
    auto node = managed_objects_.createChild(std::move(name));
    node->addInterface(std::move(p_service));
    return node;
}

bool DBusGATTImpl::setCharacteristicValue(const DBusGattVariantT& value,
                                          const std::string& object_path,
                                          bool notify) {
    //    if(deviceIsConnected()) {
    for(const auto& p_obj: managed_objects_) {
        auto obj = p_obj->findChild(object_path);
        if(obj) {
            auto object_interface = obj->getInterface(kOrgBluezGattCharacteristicInterfaceName);
            g_auto(GVariantBuilder) builder;
            g_variant_builder_init(&builder, G_VARIANT_TYPE_ARRAY);
            GVariant* p_val = GVariantConverter::toGVariantByteArray(value);
            if(object_interface) {
                object_interface->setPropertyValue(kOrgBluezGattPropertyValueName,
                                                   g_variant_ref(p_val));
            }
            g_variant_builder_add(&builder, "{sv}", kOrgBluezGattPropertyValueName, p_val);
            GVariant* pSasv =
                g_variant_new("(sa{sv})", kOrgBluezGattCharacteristicInterfaceName, &builder);
            return obj->emitSignal(p_dbus_conn_,
                                   kDBusPropertiesInterfaceName,
                                   kDBusPropertiesChangedSignal,
                                   pSasv);
        }
    }
    //    }
    return false;
}

void DBusGATTImpl::addDevicePropertyChangedCallback(EDeviceProperty property,
                                                    DevicePropertyChangedCallbackT clbk) {
    switch(property) {
    case EDeviceProperty::Address:
        device_property_callbacks_.emplace(kDevicePropertyAddress, std::move(clbk));
        break;
    case EDeviceProperty::AddressType:
        device_property_callbacks_.emplace(kDevicePropertyAddressType, std::move(clbk));
        break;
    case EDeviceProperty::Name:
        device_property_callbacks_.emplace(kDevicePropertyName, std::move(clbk));
        break;
    case EDeviceProperty::Alias:
        device_property_callbacks_.emplace(kDevicePropertyAlias, std::move(clbk));
        break;
    case EDeviceProperty::Paired:
        device_property_callbacks_.emplace(kDevicePropertyPaired, std::move(clbk));
        break;
    case EDeviceProperty::Trusted:
        device_property_callbacks_.emplace(kDevicePropertyTrusted, std::move(clbk));
        break;
    case EDeviceProperty::Blocked:
        device_property_callbacks_.emplace(kDevicePropertyBlocked, std::move(clbk));
        break;
    case EDeviceProperty::LegacyPairing:
        device_property_callbacks_.emplace(kDevicePropertyLegacyPairing, std::move(clbk));
        break;
    case EDeviceProperty::Connected:
        device_property_callbacks_.emplace(kDevicePropertyConnected, std::move(clbk));
        break;
    case EDeviceProperty::Adapter:
        device_property_callbacks_.emplace(kDevicePropertyAdapter, std::move(clbk));
        break;
    case EDeviceProperty::ServicesResolved:
        device_property_callbacks_.emplace(kDevicePropertyServicesResolved, std::move(clbk));
        break;
    case EDeviceProperty::RSSI:
        device_property_callbacks_.emplace(kDevicePropertyRSSI, std::move(clbk));
        break;
    default:
        break;
    }
}

bool DBusGATTImpl::start() {
    if(!p_main_loop_) {
        p_main_loop_ = g_main_loop_new(nullptr, false);
        if(p_main_loop_ && !g_main_loop_is_running(p_main_loop_)) {
            if(initialized_) {
                subscribeSignals();
                registerDBusObjects();
                setBusName();
                g_main_loop_run(p_main_loop_);
                return true;
            }
        }
    }
    return false;
}

bool DBusGATTImpl::stop() {
    if(p_main_loop_) {
        if(g_main_loop_is_running(p_main_loop_)) {
            g_main_loop_quit(p_main_loop_);
            return true;
        }
    }
    return false;
}

bool DBusGATTImpl::init() {
    if(!initialized_) {
        if(!p_dbus_conn_) {
            GError* p_error = nullptr;
            p_dbus_conn_ = g_bus_get_sync(G_BUS_TYPE_SYSTEM, nullptr, &p_error);
            if(!p_dbus_conn_) {
                if(p_error) {
                    g_error_free(p_error);
                }
                return false;
            }
        }
        initialized_ = initDBusObjects();
    }
    return initialized_;
}

bool DBusGATTImpl::initDBusObjects() {
    GError* p_error = nullptr;
    p_object_mgr_ = g_dbus_object_manager_client_new_sync(p_dbus_conn_,
                                                          G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
                                                          kOrgBluezObjMgrName,
                                                          kObjMgrPath,
                                                          nullptr,
                                                          nullptr,
                                                          nullptr,
                                                          nullptr,
                                                          &p_error);

    if(p_object_mgr_ == nullptr) {
        if(p_error) {
            g_error_free(p_error);
        }
        return false;
    }

    auto getProxyObj = [&](const char* bus_name, const char* iface_path, const char* iface_name) {
        GError* p_error = nullptr;
        auto obj = g_dbus_proxy_new_sync(p_dbus_conn_,
                                         G_DBUS_PROXY_FLAGS_NONE,
                                         nullptr,
                                         bus_name,
                                         iface_path,
                                         iface_name,
                                         nullptr,
                                         &p_error);
        if(!obj) {}
        if(p_error) {
            g_error_free(p_error);
        }
        return obj;
    };

    p_objs_mgr_proxy_ = getProxyObj(kOrgBluezObjMgrName, kObjMgrPath, kObjsMgrIFaceName);
    p_gatt_mgr_proxy_ = getProxyObj(kOrgBluezObjMgrName, kAdapterPath, kGATTMgrIFaceName);
    p_adpt_api_proxy_ = getProxyObj(kOrgBluezObjMgrName, kAdapterPath, kAdapterIFaceName);

    return (p_objs_mgr_proxy_ != nullptr && p_gatt_mgr_proxy_ != nullptr &&
            p_adpt_api_proxy_ != nullptr);
}

bool DBusGATTImpl::subscribeSignals() {
    if(p_dbus_conn_) {
        auto signal_prop_changed_id =
            g_dbus_connection_signal_subscribe(p_dbus_conn_,
                                               kOrgBluezObjMgrName,
                                               kDBusPropertiesInterfaceName,
                                               kDBusPropertiesChangedSignal,
                                               nullptr,
                                               nullptr,
                                               G_DBUS_SIGNAL_FLAGS_NONE,
                                               DBusGATTImpl::onSignalPropertyChanged,
                                               this,
                                               nullptr);

        auto signal_interface_added_id =
            g_dbus_connection_signal_subscribe(p_dbus_conn_,
                                               kOrgBluezObjMgrName,
                                               kObjsMgrIFaceName,
                                               kDBusInterfacesAddedSignal,
                                               nullptr,
                                               nullptr,
                                               G_DBUS_SIGNAL_FLAGS_NONE,
                                               DBusGATTImpl::onSignalInterfaceAdded,
                                               this,
                                               nullptr);

        auto signal_interface_removed_id =
            g_dbus_connection_signal_subscribe(p_dbus_conn_,
                                               kOrgBluezObjMgrName,
                                               kObjsMgrIFaceName,
                                               kDBusInterfacesRemovedSignal,
                                               nullptr,
                                               nullptr,
                                               G_DBUS_SIGNAL_FLAGS_NONE,
                                               DBusGATTImpl::onSignalInterfaceRemoved,
                                               this,
                                               nullptr);

        subscribe_signals_id_.push_back(signal_prop_changed_id);
        subscribe_signals_id_.push_back(signal_interface_added_id);
        subscribe_signals_id_.push_back(signal_interface_removed_id);

        return true;
    }
    return false;
}

bool DBusGATTImpl::registerDBusNodes(GDBusNodeInfo* p_node, const std::string& path) {
    auto b_ret = true;
    GError* p_error = nullptr;
    static GDBusInterfaceVTable interfaceVTable;
    interfaceVTable.method_call = DBusGATTImpl::onMethodCall;
    interfaceVTable.get_property = DBusGATTImpl::onGetProperty;
    interfaceVTable.set_property = DBusGATTImpl::onSetProperty;

    GDBusInterfaceInfo** ppInterface = p_node->interfaces;
    for(; *ppInterface != nullptr && b_ret; ++ppInterface) {
        auto id = g_dbus_connection_register_object(p_dbus_conn_,
                                                    path.c_str(),
                                                    *ppInterface,
                                                    &interfaceVTable,
                                                    this,
                                                    nullptr,
                                                    &p_error);
        if(id != 0) {
            registered_objects_id_.push_back(id);
        } else {
            b_ret = false;
        }
        if(p_error) {
            g_error_free(p_error);
            p_error = nullptr;
        }
    }
    GDBusNodeInfo** ppChild = p_node->nodes;
    for(; *ppChild != nullptr && b_ret; ppChild++) {
        b_ret = registerDBusNodes(*ppChild, path + "/" + (*ppChild)->path);
    }
    return b_ret;
}

bool DBusGATTImpl::registerDBusNodesXml(const std::string& xml) {
    auto ret = false;
    GError* p_error = nullptr;
    GDBusNodeInfo* p_node = g_dbus_node_info_new_for_xml(xml.c_str(), &p_error);
    if(p_node) {
        ret = registerDBusNodes(p_node, p_node->path);
        g_dbus_node_info_unref(p_node);
    }
    if(p_error) {
        g_error_free(p_error);
    }
    return ret;
}

bool DBusGATTImpl::registerDBusObjects() {
    std::string managed_objects_xml{
        "<?xml version='1.0'?><!DOCTYPE node PUBLIC '-//freedesktop//DTD D-BUS Object Introspection 1.0//EN' 'http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd'>" +
        managed_objects_.asXml()};

    if(!registerDBusNodesXml(managed_objects_xml)) {
        return false;
    }

    return registerDBusNodesXml(kObjectManagerNodeXml);
}

bool DBusGATTImpl::setBusName() {
    bus_name_id_ = g_bus_own_name_on_connection(
        p_dbus_conn_,
        app_name_.c_str(),
        G_BUS_NAME_OWNER_FLAGS_NONE,
        [](GDBusConnection*, const gchar*, gpointer user_data) {
            DBusGATTImpl::registerApp((GDBusProxy*)user_data);
        },
        [](GDBusConnection*, const gchar*, gpointer) {},
        p_gatt_mgr_proxy_,
        nullptr);
    return bus_name_id_ != 0;
}

void DBusGATTImpl::registerApp(GDBusProxy* p_bluez_gatt_manager) {
    g_auto(GVariantBuilder) builder;
    g_variant_builder_init(&builder, G_VARIANT_TYPE("a{sv}"));
    GVariant* pParams = g_variant_new("(oa{sv})", "/", &builder);
    g_dbus_proxy_call(
        p_bluez_gatt_manager,
        kGATTMgrRegisterAppNethodName,
        pParams,
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        nullptr,
        [](GObject*, GAsyncResult* pAsyncResult, gpointer p_bluez_gatt_manager) {
            GError* p_error = nullptr;
            GVariant* ret =
                g_dbus_proxy_call_finish((GDBusProxy*)p_bluez_gatt_manager, pAsyncResult, &p_error);
            if(ret == nullptr) {
            } else {
                g_variant_unref(ret);
            }
            if(p_error) {
                g_error_free(p_error);
            }
        },
        p_bluez_gatt_manager);
}

void DBusGATTImpl::packManagedObjects(const std::shared_ptr<DBusObject>& p_obj,
                                      GVariantBuilder* p_managed_objects) {
    for(const auto& p_interface: p_obj->interfaces()) {
        GVariantBuilder* p_property_array_builder = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);
        GVariantBuilder* p_interface_array_builder = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);
        for(const auto& prop: p_interface->props()) {
            g_variant_builder_add(p_property_array_builder,
                                  "{sv}",
                                  prop.name().c_str(),
                                  prop.value());
        }
        g_variant_builder_add(p_interface_array_builder,
                              "{sa{sv}}",
                              p_interface->name().c_str(),
                              p_property_array_builder);
        g_variant_builder_add(p_managed_objects,
                              "{oa{sa{sv}}}",
                              p_obj->path().c_str(),
                              p_interface_array_builder);
    }

    for(const auto& obj: *p_obj) {
        packManagedObjects(obj, p_managed_objects);
    }
}

GVariant* DBusGATTImpl::getManagedObjects() {
    GVariantBuilder* p_managed_objects = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);
    for(const auto& obj: managed_objects_) {
        packManagedObjects(obj, p_managed_objects);
    }
    return g_variant_new("(a{oa{sa{sv}}})", p_managed_objects);
}

void DBusGATTImpl::searchAlreadyConnectedDevices() {
    if(initialized_) {
        auto visitor = [&](GDBusObject* object, GDBusProxy* interface) {
            auto interface_name = g_dbus_proxy_get_interface_name(interface);
            if(strcmp(interface_name, kDeviceIFaceName) == 0) {
                auto object_proxy_name = g_dbus_proxy_get_name(interface);
                auto object_path = g_dbus_object_get_object_path(object);
                auto properties_interface =
                    g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                                  G_DBUS_PROXY_FLAGS_NONE,
                                                  nullptr,
                                                  kOrgBluezObjMgrName,
                                                  object_path,
                                                  kDBusPropertiesInterfaceName,
                                                  nullptr,
                                                  nullptr);

                if(properties_interface) {
                    auto address_prop =
                        getProperty(properties_interface,
                                    g_variant_new("(ss)", interface_name, kDevicePropertyAddress));
                    auto address_type_prop = getProperty(
                        properties_interface,
                        g_variant_new("(ss)", interface_name, kDevicePropertyAddressType));
                    auto connected_prop = getProperty(
                        properties_interface,
                        g_variant_new("(ss)", interface_name, kDevicePropertyConnected));
                    auto rssi_prop =
                        getProperty(properties_interface,
                                    g_variant_new("(ss)", interface_name, kDevicePropertyRSSI));
                    auto paired_prop =
                        getProperty(properties_interface,
                                    g_variant_new("(ss)", interface_name, kDevicePropertyPaired));
                    auto trusted_prop =
                        getProperty(properties_interface,
                                    g_variant_new("(ss)", interface_name, kDevicePropertyTrusted));

                    if(connected_prop && g_variant_get_boolean(connected_prop)) {
                        auto callbacks =
                            device_property_callbacks_.equal_range(kDevicePropertyConnected);
                        for(auto i = callbacks.first; i != callbacks.second; ++i) {
                            i->second(GVariantConverter::toDBusGattVariant(connected_prop));
                        }
                        g_variant_unref(connected_prop);
                    }

                    if(trusted_prop && g_variant_get_boolean(trusted_prop)) {
                        auto callbacks =
                            device_property_callbacks_.equal_range(kDevicePropertyTrusted);
                        for(auto i = callbacks.first; i != callbacks.second; ++i) {
                            i->second(GVariantConverter::toDBusGattVariant(trusted_prop));
                        }
                        g_variant_unref(trusted_prop);
                    }
                    if(paired_prop && g_variant_get_boolean(paired_prop)) {
                        auto callbacks =
                            device_property_callbacks_.equal_range(kDevicePropertyPaired);
                        for(auto i = callbacks.first; i != callbacks.second; ++i) {
                            i->second(GVariantConverter::toDBusGattVariant(paired_prop));
                        }
                        g_variant_unref(paired_prop);
                    }

                    if(rssi_prop && g_variant_get_uint16(rssi_prop)) {
                        auto callbacks =
                            device_property_callbacks_.equal_range(kDevicePropertyPaired);
                        for(auto i = callbacks.first; i != callbacks.second; ++i) {
                            i->second(GVariantConverter::toDBusGattVariant(rssi_prop));
                        }
                        g_variant_unref(rssi_prop);
                    }

                    if(address_type_prop) {
                        g_variant_unref(address_type_prop);
                    }
                    if(address_prop) {
                        if(g_variant_is_of_type(address_prop, G_VARIANT_TYPE_STRING)) {}
                        g_variant_unref(address_prop);
                    }

                    g_object_unref(properties_interface);
                }
            }
        };
        listManagedObjects(visitor);
    }
}

GVariant* DBusGATTImpl::getProperty(GDBusProxy* property_interface, GVariant* property) {
    auto ret = g_dbus_proxy_call_sync(property_interface,
                                      "Get",
                                      property,
                                      G_DBUS_CALL_FLAGS_NONE,
                                      -1,
                                      nullptr,
                                      nullptr);

    if(ret) {
        GVariant* ret_proppery;
        g_variant_get(ret, "(v)", &ret_proppery);
        g_variant_unref(ret);
        return ret_proppery;
    }

    return nullptr;
}

void DBusGATTImpl::listObjectInterfaces(GDBusObject* p_object,
                                        std::function<void(GDBusObject*, GDBusProxy*)> visitor) {
    auto interfaces = g_dbus_object_get_interfaces(p_object);
    if(interfaces) {
        for(guint j = 0; j < g_list_length(interfaces); ++j) {
            auto interface = static_cast<GDBusProxy*>(g_list_nth_data(interfaces, j));
            if(interface) {
                visitor(p_object, interface);
                g_object_unref(interface);
            }
        }
        g_list_free(interfaces);
    }
}

void DBusGATTImpl::listManagedObjects(std::function<void(GDBusObject*, GDBusProxy*)> visitor) {
    auto objects = g_dbus_object_manager_get_objects(p_object_mgr_);
    if(objects) {
        for(guint i = 0; i < g_list_length(objects); ++i) {
            auto object = static_cast<GDBusObject*>(g_list_nth_data(objects, i));
            if(object) {
                listObjectInterfaces(object, visitor);
                g_object_unref(object);
            }
        }
        g_list_free(objects);
    }
}

bool DBusGATTImpl::callMethod(const std::string& p_object_path,
                              const std::string& p_interface_name,
                              const std::string& p_method_name,
                              GVariant* p_parameters,
                              GVariant** p_ret) {
    if(p_method_name == "GetManagedObjects") {
        *p_ret = getManagedObjects();
        return true;
    } else {
        auto p_obj = managed_objects_.findChild(p_object_path);
        if(p_obj) {
            auto p_interface = p_obj->getInterface(p_interface_name);
            if(p_interface) {
                if(p_interface->callMethod(p_method_name, p_parameters, p_ret)) {
                    return true;
                } else {
                }

            } else {
            }
        } else {
        }
    }

    return false;
}

void DBusGATTImpl::onDevicePropertyChanged(const char* object_path,
                                           const char* prop_name,
                                           GVariant* prop_value) {
    // to do add object_path to clbk
    auto callbacks = device_property_callbacks_.equal_range(prop_name);

    for(auto i = callbacks.first; i != callbacks.second; ++i) {
        i->second(GVariantConverter::toDBusGattVariant(prop_value));
    }
}

void DBusGATTImpl::onSignalPropertyChanged(GDBusConnection* c,
                                           const gchar* sender_name,
                                           const gchar* object_path,
                                           const gchar* interface_name,
                                           const gchar* signal_name,
                                           GVariant* parameters,
                                           gpointer user_data) {
    auto p_impl = reinterpret_cast<DBusGATTImpl*>(user_data);
    if(p_impl) {
        GVariant* prop_list;
        GVariantIter* changed_props_iter;
        gchar* ifname;
        if(interface_name == std::string(kDBusPropertiesInterfaceName)) {
            g_variant_get(parameters, "(s*as)", &ifname, &prop_list, &changed_props_iter);

            GVariantIter* iter;
            gchar* prop_name;
            GVariant* prop_value;
            g_variant_get(prop_list, "a{sv}", &iter);
            while(g_variant_iter_loop(iter, "{sv}", &prop_name, &prop_value)) {
                if(strcmp(kDeviceIFaceName, ifname) == 0) {
                    p_impl->onDevicePropertyChanged(object_path, prop_name, prop_value);
                }
            }
        }
    }
}

void DBusGATTImpl::onSignalInterfaceAdded(GDBusConnection* c,
                                          const gchar* sender_name,
                                          const gchar* object_path,
                                          const gchar* interface_name,
                                          const gchar* signal_name,
                                          GVariant* parameters,
                                          gpointer user_data) {
}

void DBusGATTImpl::onSignalInterfaceRemoved(GDBusConnection* c,
                                            const gchar* sender_name,
                                            const gchar* object_path,
                                            const gchar* interface_name,
                                            const gchar* signal_name,
                                            GVariant* parameters,
                                            gpointer user_data) {
}

GVariant* DBusGATTImpl::onGetProperty(GDBusConnection* p_connection,
                                      const gchar* p_sender,
                                      const gchar* p_object_path,
                                      const gchar* p_interface_name,
                                      const gchar* p_property_name,
                                      GError** p_error,
                                      gpointer p_user_data) {
    return nullptr;
}

gboolean DBusGATTImpl::onSetProperty(GDBusConnection* p_connection,
                                     const gchar* p_sender,
                                     const gchar* p_object_path,
                                     const gchar* p_interface_name,
                                     const gchar* p_property_name,
                                     GVariant* p_value,
                                     GError** p_error,
                                     gpointer p_user_data) {
    return 0;
}

void DBusGATTImpl::onMethodCall(GDBusConnection* p_connection,
                                const gchar* p_sender,
                                const gchar* p_object_path,
                                const gchar* p_interface_name,
                                const gchar* p_method_name,
                                GVariant* p_parameters,
                                GDBusMethodInvocation* p_invocation,
                                gpointer p_user_data) {
    auto pimpl = reinterpret_cast<DBusGATTImpl*>(p_user_data);
    GVariant* ret = nullptr;
    if(pimpl->callMethod(p_object_path, p_interface_name, p_method_name, p_parameters, &ret)) {
        if(ret != nullptr) {
            g_dbus_method_invocation_return_value(p_invocation, ret);
        }
    } else {
        g_dbus_method_invocation_return_dbus_error(
            p_invocation,
            ("com." + pimpl->app_name_ + ".NotImplemented").c_str(),
            "This method is not implemented");
    }
}

}  // namespace dbus_gatt