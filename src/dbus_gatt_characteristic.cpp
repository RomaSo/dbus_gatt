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

#include "dbus_gatt/dbus_gatt_characteristic.h"
#include "impl/dbus_gatt_characteristic_impl.h"

namespace dbus_gatt {

DBusGATTCharacteristic::DBusGATTCharacteristic(
    std::string name,
    std::string uuid,
    CharacteristicFlag flags,
    CharacteristicOnReadCallbackT callback_on_read,
    CharacteristicOnWriteCallbackT callback_on_write)
:name_(std::move(name))
, pimpl_{nullptr} {
    pimpl_ = std::make_shared<DBusGattCharacteristicImpl>( std::move(uuid),
                                                           flags,
                                                           std::move(callback_on_read),
                                                           std::move(callback_on_write)
                                                           );
}

} // namespace dbus_gatt