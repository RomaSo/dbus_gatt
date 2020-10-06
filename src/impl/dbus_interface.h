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

#ifndef DBUS_GATT_DBUS_INTERFACE_H_
#define DBUS_GATT_DBUS_INTERFACE_H_

#include <string>
#include <list>
#include <vector>

#include <gio/gio.h>

#include "dbus_property.h"
#include "dbus_method.h"
#include "dbus_object_path.h"
#include "gvariant_converter.h"

namespace dbus_gatt {

class DBusInterface {
public:
    explicit DBusInterface(std::string name) : _name(std::move(name)) {}
    virtual ~DBusInterface() = default;

    DBusInterface(const DBusInterface &) = delete;
    DBusInterface(DBusInterface &&) = delete;
    DBusInterface &operator=(const DBusInterface &) = delete;
    DBusInterface &operator=(DBusInterface &&) = delete;

    [[nodiscard]] virtual const std::string &name() const {
        return _name;
    }

    [[nodiscard]] virtual std::string asXml() const {
        std::string s = "<interface name='" + _name + "'>";
        for (const auto &prop: _props) {
            s += prop.asXml();
        }
        for (const auto &method: _methods) {
            s += method.asXml();
        }
        s += "</interface>";
        return s;
    }

    virtual bool callMethod(const std::string &name, GVariant *p_parameters, GVariant **p_ret) {
        for (const auto &i: _methods) {
            if (i.name() == name) {
                i.call(p_parameters, p_ret);
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] virtual const std::list<DBusProperty> & props() const {
        return _props;
    }

    bool setPropertyValue(const std::string & name, GVariant * value) {
        for(auto & prop: _props) {
            if(prop.name() == name) {
                prop.setValue(value);
                return true;
            }
        }
        return false;
    }

    GVariant* getPropertyValue(const std::string & name) const {
        for(auto & prop: _props) {
            if(prop.name() == name) {
                return prop.value();
            }
        }
        return nullptr;
    }

    void addProperty(std::string name, GVariant * value) {
        _props.emplace_back(std::move(name), value);
    }

    template <typename T>
    void addProperty(std::string name, T value) {
        _props.emplace_back(std::move(name), GVariantConverter::toGVariant(value));
    }
protected:
    virtual void addMethod(std::string name,
                           std::vector<std::string> i_args,
                           std::string o_args,
                           DBusMethod::CallbackT callback) {
        _methods.emplace_back(std::move(name), std::move(i_args), std::move(o_args), std::move(callback));
    }
private:
    std::list<DBusProperty> _props;
    std::list<DBusMethod> _methods;
    std::string _name;
};

} // namespace dbus_gatt

#endif //DBUS_GATT_DBUS_INTERFACE_H_
