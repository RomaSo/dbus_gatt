//
// Created by romanso on 09/07/2021.
//

#include "regex"
#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "dbus_gatt/gatt_uuid.h"

namespace dbus_gatt {

static constexpr auto kGATT_UUID_RegEx{"[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}"};

GattUUID::GattUUID(std::string str_uuid) {
    GattUUID::validate(str_uuid);
    str_uuid_ = std::move(str_uuid);
}

void GattUUID::validate(const std::string &str_uuid) {
    if(!std::regex_match(str_uuid, std::regex(kGATT_UUID_RegEx, std::regex::icase))) {
        throw InvalidUUID();
    }
}

std::string GattUUID::uuid() const {
    return str_uuid_;
}


}