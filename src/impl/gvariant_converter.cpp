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

#include <iostream>
#include "gvariant_converter.h"

namespace dbus_gatt {

GVariant* GVariantConverter::toGVariant(bool value) {
    return g_variant_new_boolean(value);
}

GVariant* GVariantConverter::toGVariant(uint8_t value) {
    return g_variant_new_byte(value);
}

GVariant* GVariantConverter::toGVariant(int16_t value) {
    return g_variant_new_int16(value);
}

GVariant* GVariantConverter::toGVariant(uint16_t value) {
    return g_variant_new_uint16(value);
}

GVariant* GVariantConverter::toGVariant(int32_t value) {
    return g_variant_new_int32(value);
}

GVariant* GVariantConverter::toGVariant(uint32_t value) {
    return g_variant_new_uint32(value);
}

GVariant* GVariantConverter::toGVariant(int64_t value) {
    return g_variant_new_int64(value);
}

GVariant* GVariantConverter::toGVariant(uint64_t value) {
    return g_variant_new_uint64(value);
}

GVariant* GVariantConverter::toGVariant(double value) {
    return g_variant_new_double(value);
}

GVariant* GVariantConverter::toGVariant(const std::string &value) {
    return g_variant_new_string(value.c_str());
}

GVariant* GVariantConverter::toGVariant(const char* value) {
    return g_variant_new_string(value);
}

GVariant *GVariantConverter::toGVariant(const DBusObjectPath &path) {
    return g_variant_new_object_path(path.value().c_str());
}

GVariant *GVariantConverter::toGVariant(const std::vector<std::string> &str_vec) {
    if (str_vec.empty()) {
        return g_variant_new("as", nullptr);
    }

    g_auto(GVariantBuilder) builder;
    g_variant_builder_init(&builder, G_VARIANT_TYPE_ARRAY);

    for (const auto &i : str_vec) {
        g_variant_builder_add(&builder, "s", i.c_str());
    }
    return g_variant_builder_end(&builder);
}

GVariant* GVariantConverter::toGVariantByteArray(uint8_t *p_value, size_t value_size) {
    GBytes *p_bytes = g_bytes_new(p_value, value_size);
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, value_size);
}

GVariant *GVariantConverter::toGVariantByteArray(int8_t value) {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}

GVariant* GVariantConverter::toGVariantByteArray(uint8_t value) {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}

GVariant* GVariantConverter::toGVariantByteArray(int16_t value)  {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}

GVariant* GVariantConverter::toGVariantByteArray(uint16_t value) {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}
GVariant* GVariantConverter::toGVariantByteArray(int32_t value) {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}
GVariant* GVariantConverter::toGVariantByteArray(uint32_t value) {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}
GVariant* GVariantConverter::toGVariantByteArray(int64_t value) {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}
GVariant* GVariantConverter::toGVariantByteArray(uint64_t value) {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}
GVariant* GVariantConverter::toGVariantByteArray(double value) {
    GBytes *p_bytes = g_bytes_new(&value, sizeof(value));
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, sizeof(value));
}

GVariant* GVariantConverter::toGVariantByteArray(const std::string &value) {
    GBytes *p_bytes = g_bytes_new(value.c_str(), value.size());
    return g_variant_new_from_bytes(G_VARIANT_TYPE_BYTESTRING, p_bytes, value.size());
}

GVariant *GVariantConverter::toGVariantByteArray(const DBusGattVariantT & value) {
    if (std::holds_alternative<int8_t>(value)) {
        return toGVariantByteArray(std::get<int8_t>(value));
    } else if (std::holds_alternative<uint8_t>(value)) {
        return toGVariantByteArray(std::get<uint8_t>(value));
    } else if (std::holds_alternative<int16_t>(value)) {
        return toGVariantByteArray(std::get<int16_t>(value));
    } else if (std::holds_alternative<uint16_t>(value)) {
        return toGVariantByteArray(std::get<uint16_t>(value));
    } else if (std::holds_alternative<int32_t>(value)) {
        return toGVariantByteArray(std::get<int32_t>(value));
    } else if (std::holds_alternative<uint32_t>(value)) {
        return toGVariantByteArray(std::get<uint32_t>(value));
    } else if (std::holds_alternative<double>(value)) {
        return toGVariantByteArray(std::get<double>(value));
    } else if (std::holds_alternative<std::string>(value)) {
        return toGVariantByteArray(std::get<std::string>(value));
    } else if (std::holds_alternative<std::pair<uint8_t *, size_t>>(value)) {
        auto v = std::get<std::pair<uint8_t *, size_t>>(value);
        return toGVariantByteArray(v.first, v.second);
    }
    return nullptr;
}

std::string GVariantConverter::toString(GVariant *value) {
    if (g_variant_is_of_type(value, G_VARIANT_TYPE_BOOLEAN)) {
        return (g_variant_get_boolean(value) != 0 ? "true" : "false");
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT16)) {
        return std::to_string(g_variant_get_int16(value));
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT16)) {
        return std::to_string(g_variant_get_uint16(value));
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT32)) {
        return std::to_string(g_variant_get_int32(value));
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT32)) {
        return std::to_string(g_variant_get_uint32(value));
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT64)) {
        return std::to_string(g_variant_get_int64(value));
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT64)) {
        return std::to_string(g_variant_get_uint64(value));
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_DOUBLE)) {
        return std::to_string(g_variant_get_double(value));
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING)) {
        return g_variant_get_string(value, nullptr);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_OBJECT_PATH)) {
        return g_variant_get_string(value, nullptr);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING)) {
        return g_variant_get_bytestring(value);
    }
    return std::string("NONE");
}

void GVariantConverter::FromGVariantBytesArray(std::vector<uint8_t> & result, GVariant * gvar ) {
    gsize size;
    auto ptr = reinterpret_cast<const uint8_t*>(g_variant_get_fixed_array(const_cast<GVariant *>(gvar), &size, 1));
    for(int i = 0; i < size; ++i, ptr++) {
        result.push_back(static_cast<uint8_t>(*ptr));
    }
}
DBusGattVariantT GVariantConverter::toDBusGattVariant(GVariant * value) {
    if (g_variant_is_of_type(value, G_VARIANT_TYPE_BOOLEAN)) {
        return static_cast<bool>(g_variant_get_boolean(value));
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT16)) {
        return g_variant_get_int16(value);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT16)) {
        return g_variant_get_uint16(value);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT32)) {
        return g_variant_get_int32(value);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT32)) {
        return g_variant_get_uint32(value);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT64)) {
        return g_variant_get_int64(value);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT64)) {
        return g_variant_get_uint64(value);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_DOUBLE)) {
        return g_variant_get_double(value);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING)) {
        return g_variant_get_string(value, nullptr);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_OBJECT_PATH)) {
        return g_variant_get_string(value, nullptr);
    } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING)) {
        return g_variant_get_bytestring(value);
    }
    return dbus_gatt::DBusGattVariantT();
}

} // namespace dbus_gatt