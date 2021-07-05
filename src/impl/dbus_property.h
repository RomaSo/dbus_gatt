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

#ifndef DBUS_GATT_DBUS_PROPERTY_H_
#define DBUS_GATT_DBUS_PROPERTY_H_

#include <string>
#include <iostream>
#include <gio/gio.h>

#include "gvariant_converter.h"

namespace dbus_gatt {

class DBusProperty {
public:
    DBusProperty(std::string name, GVariant *p_value);
    ~DBusProperty();
    // Disable move and copy
    DBusProperty(const DBusProperty &) = delete;
    DBusProperty(DBusProperty &&) = delete;
    DBusProperty &operator=(const DBusProperty &) = delete;
    DBusProperty &operator=(DBusProperty &&) = delete;

    [[nodiscard]] virtual std::string asXml() const {
        std::string s = "<property name='" + _name;
        if (_p_value != nullptr) {
            s += "' type='" + std::string(g_variant_get_type_string(_p_value)) + "' access='read'>";
            s += "<annotation name='name' value='" + std::string(GVariantConverter::toString(_p_value)) + "'/>";

        } else {
            s += ">";
        }
        s += "</property>";
        return s;
    }

    [[nodiscard]] const std::string &name() const {
        return _name;
    }

    [[nodiscard]] GVariant * value() const {
        return g_variant_ref(_p_value);
    }

    void setValue(GVariant *p_value) {
        if(_p_value) {
            g_variant_unref(_p_value);
        }
        _p_value = p_value;
    }

private:
    std::string _name;
    GVariant *_p_value;
};

} // namespace dbus_gatt

#endif //DBUS_GATT_DBUS_PROPERTY_H_
