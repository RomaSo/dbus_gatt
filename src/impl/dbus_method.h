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

#ifndef DBUS_GATT_DBUS_METHOD_H_
#define DBUS_GATT_DBUS_METHOD_H_

#include <string>
#include <vector>
#include <functional>
#include <iostream>

#include "gio/gio.h"
#include "gvariant_converter.h"

namespace dbus_gatt {

class DBusMethod {
public:
    using CallbackT = std::function<void(const DBusMethod &method, GVariant *p_parameters, GVariant **p_ret)>;

    DBusMethod(std::string name, std::vector<std::string> i_args, std::string o_args, CallbackT callback)
        : _name{std::move(name)},
          _i_args{std::move(i_args)},
          _o_args{std::move(o_args)},
          _callback{std::move(callback)} {
    }

    [[nodiscard]] std::string asXml() const;

    [[nodiscard]] const std::string &name() const {
        return _name;
    }

    void call(GVariant *p_parameters, GVariant **p_ret) const {
        _callback(*this, p_parameters, p_ret);
        if(*p_ret) {
            *p_ret = g_variant_new_tuple(p_ret, 1);
        }
    }

    std::vector<uint8_t> readValue(GVariant *p_parameters) const {
        std::vector<uint8_t> value;
        GVariantConverter::FromGVariantBytesArray(value, g_variant_get_child_value(p_parameters, 0));
        return std::move(value);
    }

private:
    std::string _name;
    std::vector<std::string> _i_args;
    std::string _o_args;
    CallbackT _callback;

};

} // namespace dbus_gatt

#endif //DBUS_GATT_DBUS_METHOD_H_
