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

#ifndef DBUS_GATT_SRC_DBUS_GATT_CONSTANTS_H_
#define DBUS_GATT_SRC_DBUS_GATT_CONSTANTS_H_

//
// https://git.kernel.org/pub/scm/bluetooth/bluez.git/plain/doc/gatt-api.txt
//
#include <cstdint>

namespace dbus_gatt {

using CharacteristicFlag = uint16_t;

constexpr CharacteristicFlag kCharacteristicFlagBroadcast{0b0000000000000001};
constexpr CharacteristicFlag kCharacteristicFlagRead{0b0000000000000010};
constexpr CharacteristicFlag kCharacteristicFlagWriteWithoutResponse{0b0000000000000100};
constexpr CharacteristicFlag kCharacteristicFlagWrite{0b0000000000001000};
constexpr CharacteristicFlag kCharacteristicFlagNotify{0b0000000000010000};
constexpr CharacteristicFlag kCharacteristicFlagAuthenticatedSignedWrites{0b0000000000100000};
constexpr CharacteristicFlag kCharacteristicFlagExtendedProperties{0b0000000001000000};
constexpr CharacteristicFlag kCharacteristicFlagReliableWrite{0b0000000010000000};
constexpr CharacteristicFlag kCharacteristicFlagWritableAuxiliaries{0b0000000100000000};
constexpr CharacteristicFlag kCharacteristicFlagEncryptRead{0b0000001000000000};
constexpr CharacteristicFlag kCharacteristicFlagEncryptWrite{0b0000010000000000};
constexpr CharacteristicFlag kCharacteristicFlagEncryptAuthenticatedRead{0b0000100000000000};
constexpr CharacteristicFlag kCharacteristicFlagEncryptAuthenticatedWrite{0b0001000000000000};
constexpr CharacteristicFlag kCharacteristicFlagSecureRead{0b0010000000000000};
constexpr CharacteristicFlag kCharacteristicFlagSecureWrite{0b0100000000000000};
constexpr CharacteristicFlag kCharacteristicFlagAuthorize{0b1000000000000000};

enum class EDeviceProperty : std::size_t {
    Address = 0,
    AddressType,
    Name,
    Alias,
    Paired,
    Trusted,
    Blocked,
    LegacyPairing,
    Connected,
    Adapter,
    ServicesResolved,
    RSSI
};

}  // namespace dbus_gatt

#endif  //DBUS_GATT_SRC_DBUS_GATT_CONSTANTS_H_
