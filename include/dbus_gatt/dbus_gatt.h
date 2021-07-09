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

#ifndef DBUS_GATT_SRC_DBUS_GATT_H_
#define DBUS_GATT_SRC_DBUS_GATT_H_

#include <memory>
#include <string>
#include <iostream>

#include <gio/gio.h>
#include "dbus_gatt_service.h"
#include "dbus_gatt_characteristic.h"
#include "dbus_gatt_characteristic_read_write_callback.h"
#include "dbus_gatt_characteristic_read_only_value.h"

namespace dbus_gatt {

class DBusGATTImpl;

class DBusGATT {
public:
    DBusGATT() = delete;
    virtual bool start();
    virtual bool stop();
protected:
    DBusGATT(std::string bus_name, std::string app_path);
    virtual ~DBusGATT() = default;

    bool setCharacteristicValue(const DBusGattVariantT & value, const std::string &object_path, bool notify = false);

    void addDevicePropertyChangedCallback(EDeviceProperty property, DevicePropertyChangedCallbackT clbk);

    void searchAlreadyConnectedDevices();

    static void Services() {}

    template<typename... Arguments>
    void Services(std::shared_ptr<DBusGattService> arg, Arguments ... args) {
        addService(std::move(arg));
        Services(args...);
    }

    static void Characteristics(DBusGattService*) {}


    template<typename... Arguments>
    static void Characteristics(DBusGattService * service, std::unique_ptr<DBusGATTCharacteristic> characteristic, Arguments&& ... args) {
        service->addCharacteristic(std::move(characteristic));
        Characteristics(service, std::forward<Arguments>(args)...);
    }

    template<typename... Arguments>
    static std::shared_ptr<DBusGattService> Service(std::string name, const std::string &uuid, Arguments && ... args) {
        auto service = std::make_shared<DBusGattService>(std::move(name), uuid, true);
        Characteristics(service.get(), std::forward<Arguments>(args)...);
        return service;
    }

    static std::unique_ptr<DBusGATTCharacteristic> Characteristic(
        std::string name,
        const std::string &uuid,
        CharacteristicFlag flags,
        CharacteristicOnReadCallbackT r_clbk = nullptr,
        CharacteristicOnWriteCallbackT w_clbk = nullptr) {
        return std::make_unique<CharacteristicCallbackOnReadWrite>(std::move(name), GattUUID(uuid), flags, std::move(r_clbk), std::move(w_clbk));
    }

    static std::unique_ptr<DBusGATTCharacteristic> ReadOnlyValueCharacteristic(
            std::string name,
            const std::string &uuid,
            CharacteristicFlag flags,
            dbus_gatt::DBusGattVariantT value) {
        return std::make_unique<CharacteristicReadOnlyValue>(std::move(name), GattUUID(uuid), flags, std::move(value));
    }

private:
    DBusGATTImpl &p_impl_;
    void addService(std::shared_ptr<DBusGattService> service);
};

} // namespace dbus_gatt

#endif //DBUS_GATT_SRC_DBUS_GATT_H_
