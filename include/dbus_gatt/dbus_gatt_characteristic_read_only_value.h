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

#ifndef DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_ONLY_VALUE_H
#define DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_ONLY_VALUE_H

#include "dbus_gatt_characteristic.h"
#include "gatt_uuid.h"

namespace dbus_gatt {

class CharacteristicReadOnlyValue: public DBusGATTCharacteristic {
public:
    CharacteristicReadOnlyValue(std::string name,
                                GattUUID uuid,
                                CharacteristicFlag flags,
                                DBusGattVariantT value);

    void validateFlags(CharacteristicFlag flags);
};

}  // namespace dbus_gatt

#endif  //DBUS_GATT_DBUS_GATT_CHARACTERISTIC_READ_ONLY_VALUE_H
