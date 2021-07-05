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

#ifndef DBUS_GATT_SRC_DBUS_GATT_TYPES_H_
#define DBUS_GATT_SRC_DBUS_GATT_TYPES_H_

#include <cstddef>
#include <functional>
#include <variant>

namespace dbus_gatt {

using DBusGattVariantT = std::variant<bool,
                                      int8_t,
                                      uint8_t,
                                      int16_t,
                                      uint16_t,
                                      int32_t,
                                      uint32_t,
                                      int64_t,
                                      uint64_t,
                                      double,
                                      std::string,
                                      std::pair<uint8_t *, size_t>,
                                      std::pair<const uint8_t *, size_t>>;

using CharacteristicOnReadCallbackT =  std::function<DBusGattVariantT()>;
using CharacteristicOnWriteCallbackT = std::function<DBusGattVariantT(const uint8_t *, size_t)>;
using DevicePropertyChangedCallbackT = std::function<void(const DBusGattVariantT &value)>;

} // namespace dbus_gatt

#endif //DBUS_GATT_SRC_DBUS_GATT_TYPES_H_
