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

#ifndef DBUS_GATT_GATT_CHARACTERISTIC_H_
#define DBUS_GATT_GATT_CHARACTERISTIC_H_

#include <memory>
#include <functional>

#include "dbus_interface.h"
#include "dbus_gatt_app_impl_constants.h"
#include "dbus_gatt/dbus_gatt_types.h"
#include "dbus_gatt/dbus_gatt_constants.h"

namespace dbus_gatt {

class DBusGattCharacteristicImpl final : public DBusInterface {
public:
    DBusGattCharacteristicImpl() = delete;
    ~DBusGattCharacteristicImpl() final = default;
    DBusGattCharacteristicImpl(const DBusGattCharacteristicImpl &) = delete;
    DBusGattCharacteristicImpl(DBusGattCharacteristicImpl &&) = delete;
    DBusGattCharacteristicImpl &operator=(const DBusGattCharacteristicImpl &) = delete;
    DBusGattCharacteristicImpl &operator=(DBusGattCharacteristicImpl &&) = delete;

    DBusGattCharacteristicImpl(
        std::string uuid,
        CharacteristicFlag flags,
        DBusGattVariantT value);

    DBusGattCharacteristicImpl(
        std::string uuid,
        CharacteristicFlag flags,
        CharacteristicOnReadCallbackT callback_on_read = nullptr,
        CharacteristicOnWriteCallbackT callback_on_write = nullptr);

    void setPropertyService(const std::string &service_object_path) {
        addProperty(kOrgBluezGattPropertyServiceName, DBusObjectPath(service_object_path));
    }

private:
    void addReadValueMethod(CharacteristicFlag flags, CharacteristicOnReadCallbackT callback_on_read);
    void setReadValueMethodCallback(DBusMethod::CallbackT callback);

    void addWriteValueMethod(CharacteristicFlag flags, CharacteristicOnWriteCallbackT callback_on_write);
    void setWriteValueWithoutResponseCallback(DBusMethod::CallbackT callback);
    void setWriteValueWithResponseCallback(DBusMethod::CallbackT callback);

    void addNotifySupport(CharacteristicFlag flags);
    void addMethodStartNotify();
    void addMethodStopNotify();

    void addPropertyFlags(CharacteristicFlag flags);
    void addPropertyValue(CharacteristicFlag flags, DBusGattVariantT value);
};

} // namespace dbus_gatt

#endif //DBUS_GATT_GATT_CHARACTERISTIC_H_
