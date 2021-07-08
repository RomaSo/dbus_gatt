//
// Created by romanso on 06/07/2021.
//

#include "dbus_gatt/dbus_gatt_characteristic_read_write_callback.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "impl/dbus_gatt_characteristic_impl.h"

namespace dbus_gatt {

CharacteristicReadWriteCallback::CharacteristicReadWriteCallback(
        std::string name,
        std::string uuid,
        CharacteristicFlag flags,
        CharacteristicOnReadCallbackT callback_on_read,
        CharacteristicOnWriteCallbackT callback_on_write)
        : DBusGATTCharacteristic(std::move(name)) {

    validateFlags(flags);

    //ToDo (RomanSo): validate uuid

    if(!callback_on_read) {
        throw std::invalid_argument("callback_on_read is null");
    }

    if(!callback_on_write) {
        throw std::invalid_argument("callback_on_write is null");
    }

    pimpl_ = std::make_shared<DBusGattCharacteristicImpl>(
            std::move(uuid),
            flags,
            std::move(callback_on_read),
            std::move(callback_on_write)
    );
}

void CharacteristicReadWriteCallback::validateFlags(CharacteristicFlag flags) {
    if( !(flags & kCharacteristicFlagRead) &&
        !(flags & kCharacteristicFlagEncryptRead) &&
        !(flags & kCharacteristicFlagEncryptAuthenticatedRead) &&
        !(flags & kCharacteristicFlagSecureRead)) {
        throw InvalidCharacteristicFlags("read flags is not set");
    }

    if( !(flags & kCharacteristicFlagWrite) &&
        !(flags & kCharacteristicFlagWriteWithoutResponse) &&
        !(flags & kCharacteristicFlagAuthenticatedSignedWrites) &&
        !(flags & kCharacteristicFlagReliableWrite) &&
        !(flags & kCharacteristicFlagWritableAuxiliaries) &&
        !(flags & kCharacteristicFlagEncryptWrite) &&
        !(flags & kCharacteristicFlagEncryptAuthenticatedWrite) &&
        !(flags & kCharacteristicFlagSecureWrite)) {
        throw InvalidCharacteristicFlags("write flags is not set");
    }
}

} // namespace dbus_gatt