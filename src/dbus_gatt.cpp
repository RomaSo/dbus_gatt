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

#include "dbus_gatt/dbus_gatt.h"

#include <memory>
#include "impl/dbus_gatt_impl.h"

namespace dbus_gatt {

DBusGATT::DBusGATT(std::string bus_name, std::string app_path)
:p_impl_(DBusGATTImpl::create(std::move(bus_name), std::move(app_path))) {
}

void DBusGATT::addService(std::shared_ptr<DBusGattService> service) {
    auto service_node = p_impl_.addService(service->name(), std::move(service->getImpl()));
    for (const auto &characteristic: *service) {
        auto child = service_node->createChild(std::move(characteristic.name()));
        auto characteristic_impl = characteristic.getImpl();
        characteristic_impl->setPropertyService(service_node->path());
        child->addInterface(std::move(characteristic_impl));
    }
}

bool DBusGATT::start() {
    return p_impl_.start();
}

bool DBusGATT::stop() {
    return p_impl_.stop();
}

void DBusGATT::addDevicePropertyChangedCallback(EDeviceProperty property, DevicePropertyChangedCallbackT clbk) {
    p_impl_.addDevicePropertyChangedCallback(property, std::move(clbk));
}

bool DBusGATT::setCharacteristicValue(const DBusGattVariantT &value,
                                      const std::string &characteristic_path,
                                      bool notify) {
    return p_impl_.setCharacteristicValue(
        value,
        characteristic_path,
        notify
    );
}
void DBusGATT::searchAlreadyConnectedDevices() {
    p_impl_.searchAlreadyConnectedDevices();
}

} // namespace dbus_gatt