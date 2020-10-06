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

#ifndef DBUS_GATT_DBUS_OBJECT_H_
#define DBUS_GATT_DBUS_OBJECT_H_

#include <memory>

#include "dbus_interface.h"

namespace dbus_gatt {

class DBusObject {
public:
    using ChildListT = std::list<std::shared_ptr<DBusObject>>;
    using IFaceListT = std::list<std::shared_ptr<DBusInterface>>;

    class DBusInterfaceProxy {
    public:
        explicit DBusInterfaceProxy(IFaceListT& i_face_lst)
        :i_face_lst_(i_face_lst){
        }
        ~DBusInterfaceProxy() = default;
        IFaceListT::iterator begin() {return i_face_lst_.begin();}
        IFaceListT::iterator end()   {return i_face_lst_.end();}
    private:
        IFaceListT i_face_lst_;
    };

    explicit DBusObject(std::string name, std::string path) : name_(std::move(name)), path_(std::move(path)) {}
    virtual ~DBusObject() = default;
    DBusObject(const DBusObject &) = delete;
    DBusObject(DBusObject &&) = delete;
    DBusObject &operator=(const DBusObject &) = delete;
    DBusObject &operator=(DBusObject &&) = delete;

    [[nodiscard]] virtual std::string path() const {
        return path_.value();
    }

    virtual bool emitSignal(GDBusConnection *pBusConnection, const std::string &interfaceName, const std::string &signalName, GVariant *pParameters);

    virtual std::shared_ptr<DBusObject> createChild(std::string name) {
        auto new_object_path = std::string(path() + "/" + name);
        object_lst_.push_back(std::make_shared<DBusObject>(std::move(name), std::move(new_object_path)));
        return object_lst_.back();
    }

    [[nodiscard]] virtual std::string asXml() const;

    void addInterface(std::shared_ptr<DBusInterface> interface) {
        i_face_lst_.push_back(std::move(interface));
    }

    std::shared_ptr<DBusObject> findChild(const std::string &path);

    std::shared_ptr<DBusInterface> getInterface(const std::string &name);

    ChildListT::iterator begin() {return object_lst_.begin();}
    ChildListT::iterator end()   {return object_lst_.end();}

    DBusInterfaceProxy interfaces() {
        return DBusInterfaceProxy(i_face_lst_);
    }

private:
    std::string name_;
    DBusObjectPath path_;
    IFaceListT i_face_lst_;
    ChildListT object_lst_;

    virtual std::shared_ptr<DBusObject> findChild(const std::shared_ptr<DBusObject> & p_child, const std::string &path);
};

} // namespace dbus_gatt

#endif //DBUS_GATT_DBUS_OBJECT_H_
