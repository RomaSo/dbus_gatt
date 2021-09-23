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

#include "dbus_gatt/gatt_uuid.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "regex"

namespace dbus_gatt {

static constexpr auto kGATT_UUID_RegEx{
    "[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}"};

GattUUID::GattUUID(std::string str_uuid) {
    GattUUID::validate(str_uuid);
    str_uuid_ = std::move(str_uuid);
}

void GattUUID::validate(const std::string& str_uuid) {
    if(!std::regex_match(str_uuid, std::regex(kGATT_UUID_RegEx, std::regex::icase))) {
        throw InvalidUUID();
    }
}

std::string GattUUID::uuid() const {
    return str_uuid_;
}

}  // namespace dbus_gatt