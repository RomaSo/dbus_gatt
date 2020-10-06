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

#ifndef DBUS_GATT_SRC_DBUS_GATT_SERVICE_H_
#define DBUS_GATT_SRC_DBUS_GATT_SERVICE_H_

#include <memory>
#include <vector>

#include "dbus_gatt_characteristic.h"

namespace dbus_gatt {

class DBusGattServiceImpl;

class DBusGattService {
public:
    using CharacteristicsT = std::vector<DBusGATTCharacteristic>;

    DBusGattService() = delete;
    ~DBusGattService() = default;
    explicit DBusGattService(std::string name, const std::string &uuid, bool primary = true);

    std::shared_ptr<DBusGattServiceImpl> getImpl() {
        return pimpl_;
    }

    std::string name() const{
        return name_;
    }



    CharacteristicsT::iterator begin() {return characteristics_.begin();}
    CharacteristicsT::iterator end()   {return characteristics_.end();}

    void addCharacteristic(DBusGATTCharacteristic characteristic) {
        characteristics_.push_back(std::move(characteristic));
    }

private:
    std::string name_;
    std::shared_ptr<DBusGattServiceImpl> pimpl_;
    CharacteristicsT characteristics_;
};

} // namespace dbus_gatt

#endif //DBUS_GATT_SRC_DBUS_GATT_SERVICE_H_
