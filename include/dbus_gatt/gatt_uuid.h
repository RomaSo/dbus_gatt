//
// Created by romanso on 09/07/2021.
//

#ifndef DBUS_GATT_GATT_UUID_H
#define DBUS_GATT_GATT_UUID_H

#include <string>

namespace dbus_gatt {

class GattUUID {
public:
    explicit GattUUID(std::string str_uuid);
    static void validate(const std::string &str_uuid);
    std::string uuid() const;
private:
    std::string str_uuid_;
};

}

#endif //DBUS_GATT_GATT_UUID_H
