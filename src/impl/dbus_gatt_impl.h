// MIT License
//
// Copyright (c) 2020 RomanSo
//
//     Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
//     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//     copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
//     copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef DBUS_GATT_SRC_DBUS_GATT_IMPL_H_
#define DBUS_GATT_SRC_DBUS_GATT_IMPL_H_

#include <string>
#include <iostream>
#include <variant>
#include <mutex>
#include <map>

#include "dbus_object.h"
#include "dbus_gatt_service_impl.h"
#include "dbus_gatt_characteristic_impl.h"

namespace dbus_gatt {

class DBusGATTImpl {
public:
    static DBusGATTImpl& create(std::string app_name, std::string app_path);
    DBusGATTImpl() = delete;

    DBusGATTImpl(const DBusGATTImpl &) = delete;
    DBusGATTImpl(DBusGATTImpl &&) = delete;

    DBusGATTImpl &operator=(const DBusGATTImpl &) = delete;
    DBusGATTImpl &operator=(DBusGATTImpl &&) = delete;

    ~DBusGATTImpl();

    std::shared_ptr<DBusObject> addService(
        std::string name,
        std::shared_ptr<DBusGattServiceImpl> p_service);

    bool start();
    bool stop();

    bool setCharacteristicValue(const DBusGattVariantT & value, const std::string &object_path, bool notify);
    void addDevicePropertyChangedCallback(EDeviceProperty property, DevicePropertyChangedCallbackT clbk);
    void searchAlreadyConnectedDevices();

protected:
    DBusGATTImpl(std::string app_name, std::string app_path);
private:
    std::string app_name_;
    DBusObject managed_objects_;
    GMainLoop *p_main_loop_{nullptr};
    GDBusConnection *p_dbus_conn_{nullptr};
    GDBusObjectManager *p_object_mgr_{nullptr};
    GDBusProxy *p_objs_mgr_proxy_{nullptr};
    GDBusProxy *p_gatt_mgr_proxy_{nullptr};
    GDBusProxy *p_adpt_api_proxy_{nullptr};

    bool initialized_{false};
    guint bus_name_id_{0};
    std::vector<gulong> subscribe_signals_id_;
    std::vector<guint> registered_objects_id_;

    std::multimap<std::string,DevicePropertyChangedCallbackT> device_property_callbacks_;

    int32_t connected_devices{0};

    void deviceConnect() {
        ++connected_devices;
    }

    void deviceDisConnect() {
        --connected_devices;
    }

    bool deviceIsConnected() {
        return connected_devices > 0;
    }

    bool init();
    bool initDBusObjects();
    bool subscribeSignals();

    bool registerDBusNodes(GDBusNodeInfo *p_node, const std::string &path = "");
    bool registerDBusNodesXml(const std::string &);
    bool registerDBusObjects();

    bool setBusName();

    static void registerApp(GDBusProxy *p_bluez_gatt_manager);

    void packManagedObjects(const std::shared_ptr<DBusObject> &p_obj, GVariantBuilder *p_managed_objects);
    GVariant *getManagedObjects();

    GVariant* getProperty(GDBusProxy *property_interface, GVariant* property);
    void listManagedObjects(std::function<void(GDBusObject*,GDBusProxy*)>);
    void listObjectInterfaces(GDBusObject*p_object, std::function<void(GDBusObject*,GDBusProxy*)>);

    bool callMethod(
        const std::string &p_object_path,
        const std::string &p_interface_name,
        const std::string &p_method_name,
        GVariant *p_parameters,
        GVariant **p_ret);

    void onDevicePropertyChanged(const char* object_path, const char* prop_name, GVariant *prop_value);

    static void onSignalPropertyChanged(
        GDBusConnection *c,
        const gchar *sender_name,
        const gchar *object_path,
        const gchar *interface_name,
        const gchar *signal_name,
        GVariant *parameters,
        gpointer user_data);

    static void onSignalInterfaceAdded(
        GDBusConnection *c,
        const gchar *sender_name,
        const gchar *object_path,
        const gchar *interface_name,
        const gchar *signal_name,
        GVariant *parameters,
        gpointer user_data);

    static void onSignalInterfaceRemoved(
        GDBusConnection *c,
        const gchar *sender_name,
        const gchar *object_path,
        const gchar *interface_name,
        const gchar *signal_name,
        GVariant *parameters,
        gpointer user_data);

    static GVariant *onGetProperty(
        GDBusConnection *p_connection,
        const gchar *p_sender,
        const gchar *p_object_path,
        const gchar *p_interface_name,
        const gchar *p_property_name,
        GError **p_error,
        gpointer p_user_data);

    static gboolean onSetProperty(
        GDBusConnection *p_connection,
        const gchar *p_sender,
        const gchar *p_object_path,
        const gchar *p_interface_name,
        const gchar *p_property_name,
        GVariant *p_value,
        GError **p_error,
        gpointer p_user_data);

    static void onMethodCall(
        GDBusConnection *p_connection,
        const gchar *p_sender,
        const gchar *p_object_path,
        const gchar *p_interface_name,
        const gchar *p_method_name,
        GVariant *p_parameters,
        GDBusMethodInvocation *p_invocation,
        gpointer p_user_data
    );
};

} // namespace dbus_gatt

#endif //DBUS_GATT_SRC_DBUS_GATT_IMPL_H_
