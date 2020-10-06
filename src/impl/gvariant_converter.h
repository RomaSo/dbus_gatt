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

#ifndef DBUS_GATT_SRC_GVARIANT_CONVERTER_H_
#define DBUS_GATT_SRC_GVARIANT_CONVERTER_H_

#include <string>
#include <vector>
#include <glib.h>
#include <algorithm>

#include "dbus_object_path.h"
#include "dbus_gatt/dbus_gatt_types.h"

namespace dbus_gatt {

class GVariantConverter {
public:
    // G_VARIANT_TYPE_BOOLEAN
    static GVariant *toGVariant(bool);
    // G_VARIANT_TYPE_BYTE
    static GVariant *toGVariant(uint8_t value);
    // G_VARIANT_TYPE_INT16
    static GVariant *toGVariant(int16_t value);
    // G_VARIANT_TYPE_UINT16
    static GVariant *toGVariant(uint16_t value);
    // G_VARIANT_TYPE_INT32
    static GVariant *toGVariant(int32_t value);
    // G_VARIANT_TYPE_UINT32
    static GVariant *toGVariant(uint32_t value);
    // G_VARIANT_TYPE_INT64
    static GVariant *toGVariant(int64_t value);
    // G_VARIANT_TYPE_UINT64
    static GVariant *toGVariant(uint64_t value);
    // G_VARIANT_TYPE_DOUBLE
    static GVariant *toGVariant(double value);
    // G_VARIANT_TYPE_STRING
    static GVariant *toGVariant(const std::string &value);
    static GVariant *toGVariant(const char *value);
    // G_VARIANT_TYPE_OBJECT_PATH
    static GVariant *toGVariant(const DBusObjectPath &path);
    // G_VARIANT_TYPE_STRING_ARRAY
    static GVariant *toGVariant(const std::vector<std::string> &str_vec);

    static GVariant *toGVariantByteArray(uint8_t *p_value, size_t value_size);

    static GVariant *toGVariantByteArray(int8_t value);
    static GVariant *toGVariantByteArray(uint8_t value);
    static GVariant *toGVariantByteArray(int16_t value);
    static GVariant *toGVariantByteArray(uint16_t value);
    static GVariant *toGVariantByteArray(int32_t value);
    static GVariant *toGVariantByteArray(uint32_t value);
    static GVariant *toGVariantByteArray(int64_t value);
    static GVariant *toGVariantByteArray(uint64_t value);
    static GVariant *toGVariantByteArray(double value);
    static GVariant *toGVariantByteArray(const std::string &value);
    static GVariant *toGVariantByteArray(const DBusGattVariantT &value);

    static DBusGattVariantT toDBusGattVariant(GVariant *);

    static std::string toString(GVariant *value);

    static void FromGVariantBytesArray(std::vector<uint8_t> &result, GVariant *gvar);
};

} // namespace dbus_gatt

#endif //DBUS_GATT_UTILS_H_
