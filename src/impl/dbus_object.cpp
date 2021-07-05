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

#include "dbus_object.h"

namespace dbus_gatt {

[[nodiscard]] std::string DBusObject::asXml() const {
    std::string s = "<node name='" + name_ + "'>";
    s += "<annotation name='ObjectPath' value='" + path() + "'/>";
    for (const auto &i: i_face_lst_) {
        s += i->asXml();
    }

    for (const auto &i: object_lst_) {
        s += i->asXml();
    }
    s += "</node>";
    return s;
}

bool DBusObject::emitSignal(
    GDBusConnection *pBusConnection,
    const std::string &interfaceName,
    const std::string &signalName,
    GVariant *pParameters) {

    GError *pError = nullptr;

    auto ret = g_dbus_connection_emit_signal
        (
            pBusConnection,          // GDBusConnection *connection
            nullptr,                    // const gchar *destination_bus_name
            path().c_str(),       // const gchar *object_path
            interfaceName.c_str(),   // const gchar *interface_name
            signalName.c_str(),      // const gchar *signal_name
            pParameters,             // GVariant *parameters
            &pError                  // GError **error
        );
    if(ret==0) {
    }
    return ret;
}

std::shared_ptr<DBusObject> DBusObject::findChild(const std::string &path) {
    std::shared_ptr<DBusObject> obj{nullptr};
    for (const auto &i: object_lst_) {
        if(obj) {
            break;
        }
        if (i->path() == path) {
            return i;
        } else {
            obj = findChild(i, path);
        }
    }
    return obj;
}

std::shared_ptr<DBusObject> DBusObject::findChild(const std::shared_ptr<DBusObject> & p_child, const std::string &path) {
    std::shared_ptr<DBusObject> obj{nullptr};
    for (const auto &i: p_child->object_lst_) {
        if(obj) {
            break;
        }
        if (i->path() == path) {
            return i;
        } else {
            obj = findChild(i, path);
        }
    }
    return obj;
}
std::shared_ptr<DBusInterface> DBusObject::getInterface(const std::string &name) {
    for (const auto &i: i_face_lst_) {
        if (i->name() == name) {
            return i;
        }
    }

    return nullptr;
}

} // namespace dbus_gatt