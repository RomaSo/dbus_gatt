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

#include "dbus_gatt/dbus_gatt_characteristic_read_write_callback.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "impl/dbus_gatt_characteristic_impl.h"

namespace dbus_gatt {

CharacteristicCallbackOnReadWrite::CharacteristicCallbackOnReadWrite(
    std::string name,
    GattUUID uuid,
    CharacteristicFlag flags,
    CharacteristicOnReadCallbackT callback_on_read,
    CharacteristicOnWriteCallbackT callback_on_write)
: DBusGATTCharacteristic(std::move(name)) {
    validateFlags(flags);

    if(!callback_on_read) {
        throw std::invalid_argument("callback_on_read is null");
    }

    if(!callback_on_write) {
        throw std::invalid_argument("callback_on_write is null");
    }

    pimpl_ = std::make_shared<DBusGattCharacteristicImpl>(std::move(uuid.uuid()),
                                                          flags,
                                                          std::move(callback_on_read),
                                                          std::move(callback_on_write));
}

void CharacteristicCallbackOnReadWrite::validateFlags(CharacteristicFlag flags) {
    if(!(flags & kCharacteristicFlagRead) && !(flags & kCharacteristicFlagEncryptRead) &&
       !(flags & kCharacteristicFlagEncryptAuthenticatedRead) &&
       !(flags & kCharacteristicFlagSecureRead)) {
        throw InvalidCharacteristicFlags("read flags is not set");
    }

    if(!(flags & kCharacteristicFlagWrite) && !(flags & kCharacteristicFlagWriteWithoutResponse) &&
       !(flags & kCharacteristicFlagAuthenticatedSignedWrites) &&
       !(flags & kCharacteristicFlagReliableWrite) &&
       !(flags & kCharacteristicFlagWritableAuxiliaries) &&
       !(flags & kCharacteristicFlagEncryptWrite) &&
       !(flags & kCharacteristicFlagEncryptAuthenticatedWrite) &&
       !(flags & kCharacteristicFlagSecureWrite)) {
        throw InvalidCharacteristicFlags("write flags is not set");
    }
}

}  // namespace dbus_gatt