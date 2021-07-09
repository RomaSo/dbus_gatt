//
// Created by romanso on 08/07/2021.
//

#include <stdexcept>
#include "dbus_gatt/dbus_gatt_characteristic_read_only_value.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"

#include "impl/dbus_gatt_characteristic_impl.h"

namespace dbus_gatt {

CharacteristicReadOnlyValue::CharacteristicReadOnlyValue(std::string name,
                                                         std::string uuid,
                                                         CharacteristicFlag flags,
                                                         DBusGattVariantT value)
: DBusGATTCharacteristic(std::move(name)) {
    validateFlags(flags);
    //ToDo (RomanSo): validate uuid
    pimpl_ = std::make_shared<DBusGattCharacteristicImpl>(
            std::move(uuid),
            flags,
            std::move(value)
    );
}

void CharacteristicReadOnlyValue::validateFlags(CharacteristicFlag flags) {
    if( !(flags & kCharacteristicFlagRead) &&
        !(flags & kCharacteristicFlagEncryptRead) &&
        !(flags & kCharacteristicFlagEncryptAuthenticatedRead) &&
        !(flags & kCharacteristicFlagSecureRead)) {
        throw InvalidCharacteristicFlags("read flags is required");
    }

    if( (flags & kCharacteristicFlagWrite) &&
        (flags & kCharacteristicFlagWriteWithoutResponse) &&
        (flags & kCharacteristicFlagAuthenticatedSignedWrites) &&
        (flags & kCharacteristicFlagReliableWrite) &&
        (flags & kCharacteristicFlagWritableAuxiliaries) &&
        (flags & kCharacteristicFlagEncryptWrite) &&
        (flags & kCharacteristicFlagEncryptAuthenticatedWrite) &&
        (flags & kCharacteristicFlagSecureWrite)) {
        throw InvalidCharacteristicFlags("write flags is not supported");
    }
}

}