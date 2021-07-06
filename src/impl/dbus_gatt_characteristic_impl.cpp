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

#include "dbus_gatt_characteristic_impl.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "gvariant_converter.h"

namespace dbus_gatt {

DBusGattCharacteristicImpl::DBusGattCharacteristicImpl(
    std::string uuid,
    CharacteristicFlag flags,
    DBusGattVariantT value)
    : DBusInterface(kOrgBluezGattCharacteristicInterfaceName) {
    if((flags & kCharacteristicFlagBroadcast ||
        flags & kCharacteristicFlagWriteWithoutResponse ||
        flags & kCharacteristicFlagWrite ||
        flags & kCharacteristicFlagNotify ||
        flags & kCharacteristicFlagAuthenticatedSignedWrites ||
        flags & kCharacteristicFlagExtendedProperties ||
        flags & kCharacteristicFlagReliableWrite ||
        flags & kCharacteristicFlagWritableAuxiliaries ||
        flags & kCharacteristicFlagEncryptWrite ||
        flags & kCharacteristicFlagEncryptAuthenticatedWrite ||
        flags & kCharacteristicFlagSecureWrite)) {

        throw InvalidCharacteristicFlags("Write flags do not supported for this characteristic");
    }

    if( !(flags & kCharacteristicFlagRead) &&
        !(flags & kCharacteristicFlagEncryptRead) &&
        !(flags & kCharacteristicFlagEncryptAuthenticatedRead) &&
        !(flags & kCharacteristicFlagSecureRead)) {

        throw InvalidCharacteristicFlags("Read flags do not provided");
    }

    setReadValueMethodCallback(
            [p_self = this](
                    const dbus_gatt::DBusMethod &method,
                    GVariant *p_parameters,
                    GVariant **p_ret) {
                *p_ret = p_self->getPropertyValue(kOrgBluezGattPropertyValueName);
                if (*p_ret) {
                    *p_ret = g_variant_new_tuple(p_ret, 1);
                    return true;
                } else {
                    return false;
                }
            }
    );

    addNotifySupport(flags);
    addPropertyFlags(flags);
    addProperty(kOrgBluezGattPropertyUUIDName, std::move(uuid));
    addPropertyValue(flags, std::move(value));
}

DBusGattCharacteristicImpl::DBusGattCharacteristicImpl(
    std::string uuid,
    CharacteristicFlag flags,
    CharacteristicOnReadCallbackT callback_on_read,
    CharacteristicOnWriteCallbackT callback_on_write)
    : DBusInterface(kOrgBluezGattCharacteristicInterfaceName) {

    addReadValueMethod(flags, std::move(callback_on_read));
    addWriteValueMethod(flags, std::move(callback_on_write));
    addNotifySupport(flags);
    addPropertyFlags(flags);
    addProperty(kOrgBluezGattPropertyUUIDName, std::move(uuid));
}

void DBusGattCharacteristicImpl::addReadValueMethod(
    CharacteristicFlag flags,
    CharacteristicOnReadCallbackT callback_on_read) {
    if (flags & kCharacteristicFlagRead ||
        flags & kCharacteristicFlagEncryptRead ||
        flags & kCharacteristicFlagEncryptAuthenticatedRead ||
        flags & kCharacteristicFlagSecureRead) {

        if (callback_on_read) {
            setReadValueMethodCallback(
                    [callback = std::move(callback_on_read)](
                            const dbus_gatt::DBusMethod &method,
                            GVariant *p_parameters,
                            GVariant **p_ret) {
                        *p_ret = GVariantConverter::toGVariantByteArray(callback());
                    }
            );
        } else {
            throw FlagsWithoutMethodProvided(kOrgBluezGattCharacteristicReadValueMethodName);
        }
    } else if (callback_on_read != nullptr) {
        throw MethodNotAllowedWithoutFlags(kOrgBluezGattCharacteristicReadValueMethodName);
    }
}

void DBusGattCharacteristicImpl::setReadValueMethodCallback(DBusMethod::CallbackT callback)  {
    static const std::vector<std::string> i_args{kOrgBluezGattCharacteristicReadValueInArgs};
    DBusInterface::addMethod(
            kOrgBluezGattCharacteristicReadValueMethodName,
            i_args,
            kOrgBluezGattCharacteristicReadValueOutArgs,
            std::move(callback)
    );
}

void DBusGattCharacteristicImpl::addWriteValueMethod(
    CharacteristicFlag flags,
    CharacteristicOnWriteCallbackT callback_on_write) {

    if (flags & kCharacteristicFlagWrite ||
        flags & kCharacteristicFlagEncryptWrite ||
        flags & kCharacteristicFlagEncryptAuthenticatedWrite ||
        flags & kCharacteristicFlagSecureWrite) {

        if (callback_on_write) {
            if(flags & kCharacteristicFlagWriteWithoutResponse) {
                setWriteValueWithoutResponseCallback(
                        [callback = std::move(callback_on_write)](const dbus_gatt::DBusMethod &method,
                                                                  GVariant *p_parameters,
                                                                  GVariant **p_ret) {
                            gsize size;
                            auto p_var = g_variant_get_child_value(p_parameters, 0);
                            auto ptr = reinterpret_cast<const uint8_t *>(g_variant_get_fixed_array(p_var, &size,
                                                                                                   sizeof(uint8_t)));
                            callback(ptr, size);
                        }
                );
            } else {
                setWriteValueWithResponseCallback(
                        [callback = std::move(callback_on_write)](const dbus_gatt::DBusMethod &method,
                                                                  GVariant *p_parameters,
                                                                  GVariant **p_ret) {
                            gsize size;
                            auto p_var = g_variant_get_child_value(p_parameters, 0);
                            auto ptr = reinterpret_cast<const uint8_t *>(g_variant_get_fixed_array(p_var, &size,
                                                                                                   sizeof(uint8_t)));
                            *p_ret = GVariantConverter::toGVariantByteArray(callback(ptr, size));
                        }
                );
            }
        } else {
            throw FlagsWithoutMethodProvided(kOrgBluezGattCharacteristicWriteValueMethodName);
        }

    } else if (callback_on_write != nullptr) {
        throw MethodNotAllowedWithoutFlags(kOrgBluezGattCharacteristicWriteValueMethodName);
    }
}

void DBusGattCharacteristicImpl::setWriteValueWithoutResponseCallback(DBusMethod::CallbackT callback) {
    static const std::vector<std::string> i_args{
            kOrgBluezGattCharacteristicWriteValueInArg1,
            kOrgBluezGattCharacteristicWriteValueInArg2
    };
    DBusInterface::addMethod(
            kOrgBluezGattCharacteristicWriteValueMethodName,
            i_args,
            kOrgBluezGattCharacteristicWriteValueOutArgs,
            std::move(callback)
    );
}

void DBusGattCharacteristicImpl::setWriteValueWithResponseCallback(DBusMethod::CallbackT callback) {
    static const std::vector<std::string> i_args{
            kOrgBluezGattCharacteristicWriteValueInArg1,
            kOrgBluezGattCharacteristicWriteValueInArg2
    };
    DBusInterface::addMethod(
            kOrgBluezGattCharacteristicWriteValueMethodName,
            i_args,
            kOrgBluezGattCharacteristicReadValueOutArgs,
            std::move(callback)
    );
}

void DBusGattCharacteristicImpl::addNotifySupport(CharacteristicFlag flags) {
    if (flags & kCharacteristicFlagNotify) {
        addMethodStartNotify();
        addMethodStopNotify();
        addProperty(kOrgBluezGattPropertyNotifyingName, false);
    }
}

void DBusGattCharacteristicImpl::addMethodStartNotify() {
    DBusInterface::addMethod(
            kOrgBluezGattCharacteristicStartNotifyMethodName,
            std::vector<std::string>{},
            "",
            [self=this](const dbus_gatt::DBusMethod &method,
                        GVariant *p_parameters,
                        GVariant **p_ret) {
                self->setPropertyValue(kOrgBluezGattPropertyNotifyingName, g_variant_new_boolean(true));
            }
    );
}

void DBusGattCharacteristicImpl::addMethodStopNotify() {
    DBusInterface::addMethod(
            kOrgBluezGattCharacteristicStopNotifyMethodName,
            std::vector<std::string>{},
            "",
            [self=this](const dbus_gatt::DBusMethod &method,
                        GVariant *p_parameters,
                        GVariant **p_ret) {
                self->setPropertyValue(kOrgBluezGattPropertyNotifyingName, g_variant_new_boolean(false));
            }
    );
}

void DBusGattCharacteristicImpl::addPropertyFlags(CharacteristicFlag flags) {
    std::vector<std::string> str_flags;
    if (flags & kCharacteristicFlagBroadcast) {
        str_flags.emplace_back(kStrCharacteristicFlagBroadcast);
    }
    if (flags & kCharacteristicFlagRead) {
        str_flags.emplace_back(kStrCharacteristicFlagRead);
    }
    if (flags & kCharacteristicFlagWriteWithoutResponse) {
        str_flags.emplace_back(kStrCharacteristicFlagWriteWithoutResponse);
    }
    if (flags & kCharacteristicFlagWrite) {
        str_flags.emplace_back(kStrCharacteristicFlagWrite);
    }
    if (flags & kCharacteristicFlagNotify) {
        str_flags.emplace_back(kStrCharacteristicFlagNotify);
    }
    if (flags & kCharacteristicFlagAuthenticatedSignedWrites) {
        str_flags.emplace_back(kStrCharacteristicFlagAuthenticatedSignedWrites);
    }
    if (flags & kCharacteristicFlagExtendedProperties) {
        str_flags.emplace_back(kStrCharacteristicFlagExtendedProperties);
    }
    if (flags & kCharacteristicFlagReliableWrite) {
        str_flags.emplace_back(kStrCharacteristicFlagReliableWrite);
    }
    if (flags & kCharacteristicFlagWritableAuxiliaries) {
        str_flags.emplace_back(kStrCharacteristicFlagWritableAuxiliaries);
    }
    if (flags & kCharacteristicFlagEncryptRead) {
        str_flags.emplace_back(kStrCharacteristicFlagEncryptRead);
    }
    if (flags & kCharacteristicFlagEncryptWrite) {
        str_flags.emplace_back(kStrCharacteristicFlagEncryptWrite);
    }
    if (flags & kCharacteristicFlagEncryptAuthenticatedRead) {
        str_flags.emplace_back(kStrCharacteristicFlagEncryptAuthenticatedRead);
    }
    if (flags & kCharacteristicFlagEncryptAuthenticatedWrite) {
        str_flags.emplace_back(kStrCharacteristicFlagEncryptAuthenticatedWrite);
    }
    if (flags & kCharacteristicFlagSecureRead) {
        str_flags.emplace_back(kStrCharacteristicFlagSecureRead);
    }
    if (flags & kCharacteristicFlagSecureWrite) {
        str_flags.emplace_back(kStrCharacteristicFlagSecureWrite);
    }
    if (flags & kCharacteristicFlagAuthorize) {
        str_flags.emplace_back(kStrCharacteristicFlagAuthorize);
    }

    if (!str_flags.empty()) {
        addProperty(kOrgBluezGattPropertyFlagsName, str_flags);
    } else {
        throw InvalidCharacteristicFlags("no flags provided");
    }
}

void DBusGattCharacteristicImpl::addPropertyValue(CharacteristicFlag flags, DBusGattVariantT value) {
    if (flags & kCharacteristicFlagRead ||
        flags & kCharacteristicFlagEncryptRead ||
        flags & kCharacteristicFlagEncryptAuthenticatedRead ||
        flags & kCharacteristicFlagSecureRead ||
        flags & kCharacteristicFlagWrite ||
        flags & kCharacteristicFlagEncryptWrite ||
        flags & kCharacteristicFlagEncryptAuthenticatedWrite ||
        flags & kCharacteristicFlagSecureWrite) {
        addProperty(kOrgBluezGattPropertyValueName, GVariantConverter::toGVariantByteArray(value));
    }
}

} // namespace dbus_gatt