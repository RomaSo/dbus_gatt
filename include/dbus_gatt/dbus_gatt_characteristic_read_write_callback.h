//
// Created by romanso on 06/07/2021.
//

#ifndef DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_WRITE_CALLBACK_H
#define DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_WRITE_CALLBACK_H

#include "dbus_gatt_characteristic.h"

namespace dbus_gatt {

class CharacteristicCallbackOnReadWrite : public DBusGATTCharacteristic {
public:
    CharacteristicCallbackOnReadWrite(
            std::string name,
            std::string uuid,
            CharacteristicFlag flags,
            CharacteristicOnReadCallbackT callback_on_read,
            CharacteristicOnWriteCallbackT callback_on_write);

    void validateFlags(CharacteristicFlag flags);
};

} // namespace dbus_gatt

#endif //DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_WRITE_CALLBACK_H
