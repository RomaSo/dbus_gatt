//
// Created by romanso on 08/07/2021.
//

#ifndef DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_ONLY_VALUE_H
#define DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_ONLY_VALUE_H

#include "dbus_gatt_characteristic.h"
#include "gatt_uuid.h"

namespace dbus_gatt {

class CharacteristicReadOnlyValue : public DBusGATTCharacteristic {
public:
    CharacteristicReadOnlyValue(std::string name,
                                GattUUID uuid,
                                CharacteristicFlag flags,
                                DBusGattVariantT value);

    void validateFlags(CharacteristicFlag flags);
};

} // namespace dbus_gatt

#endif //DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_ONLY_VALUE_H
