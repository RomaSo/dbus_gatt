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

#ifndef DBUS_GATT_CONSTANTS_H_
#define DBUS_GATT_CONSTANTS_H_

namespace dbus_gatt {

constexpr auto kObjectManagerNodeXml = R"(<?xml version='1.0'?>
<!DOCTYPE node PUBLIC '-//freedesktop//DTD D-BUS Object Introspection 1.0//EN' 'http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd'>
<node name='/'>
  <interface name='org.freedesktop.DBus.ObjectManager'>
    <method name='GetManagedObjects'>
      <arg type='a{oa{sa{sv}}}' direction='out'>
      </arg>
    </method>
  </interface>
</node>
)";



constexpr auto kStrCharacteristicFlagBroadcast{"broadcast"};
constexpr auto kStrCharacteristicFlagRead{"read"};
constexpr auto kStrCharacteristicFlagWriteWithoutResponse{"write-without-response"};
constexpr auto kStrCharacteristicFlagWrite{"write"};
constexpr auto kStrCharacteristicFlagNotify{"notify"};
constexpr auto kStrCharacteristicFlagIndicate{"indicate"};
constexpr auto kStrCharacteristicFlagAuthenticatedSignedWrites{"authenticated-signed-writes"};
constexpr auto kStrCharacteristicFlagExtendedProperties{"extended-properties"};
constexpr auto kStrCharacteristicFlagReliableWrite{"reliable-write"};
constexpr auto kStrCharacteristicFlagWritableAuxiliaries{"writable-auxiliaries"};
constexpr auto kStrCharacteristicFlagEncryptRead{"encrypt-read"};
constexpr auto kStrCharacteristicFlagEncryptWrite{"encrypt-write"};
constexpr auto kStrCharacteristicFlagEncryptAuthenticatedRead{"encrypt-authenticated-read"};
constexpr auto kStrCharacteristicFlagEncryptAuthenticatedWrite{"encrypt-authenticated-write"};
constexpr auto kStrCharacteristicFlagSecureRead{"secure-read"};
constexpr auto kStrCharacteristicFlagSecureWrite{"secure-write"};
constexpr auto kStrCharacteristicFlagAuthorize{"authorize"};
constexpr auto kOrgBluezObjMgrName{"org.bluez"};
constexpr auto kObjMgrPath{"/"};
constexpr auto kAdapterPath{"/org/bluez/hci0"};
constexpr auto kObjsMgrIFaceName{"org.freedesktop.DBus.ObjectManager"};
constexpr auto kDBusPropertiesInterfaceName{"org.freedesktop.DBus.Properties"};
constexpr auto kDBusPropertiesChangedSignal{"PropertiesChanged"};
constexpr auto kDBusInterfacesAddedSignal{"InterfacesAdded"};
constexpr auto kDBusInterfacesRemovedSignal{"InterfacesRemoved"};
constexpr auto kGATTMgrIFaceName{"org.bluez.GattManager1"};
constexpr auto kAdapterIFaceName{"org.bluez.Adapter1"};
constexpr auto kDeviceIFaceName{"org.bluez.Device1"};
constexpr auto kDevicePropertyAddress{"Address"};
constexpr auto kDevicePropertyAddressType{"AddressType"};
constexpr auto kDevicePropertyName{"Name"};
constexpr auto kDevicePropertyAlias{"Alias"};
constexpr auto kDevicePropertyPaired{"Paired"};
constexpr auto kDevicePropertyTrusted{"Trusted"};
constexpr auto kDevicePropertyBlocked{"Blocked"};
constexpr auto kDevicePropertyLegacyPairing{"LegacyPairing"};
constexpr auto kDevicePropertyConnected{"Connected"};
constexpr auto kDevicePropertyAdapter{"Adapter"};
constexpr auto kDevicePropertyServicesResolved{"ServicesResolved"};
constexpr auto kDevicePropertyRSSI{"RSSI"};
constexpr auto kGATTMgrRegisterAppNethodName{"RegisterApplication"};
constexpr auto kOrgBluezGattCharacteristicInterfaceName{"org.bluez.GattCharacteristic1"};
constexpr auto kOrgBluezGattServiceInterfaceName{"org.bluez.GattService1"};
constexpr auto kOrgBluezGattPropertyUUIDName{"UUID"};
constexpr auto kOrgBluezGattPropertyValueName{"Value"};
constexpr auto kOrgBluezGattPropertyFlagsName{"Flags"};
constexpr auto kOrgBluezGattPropertyServiceName{"Service"};
constexpr auto kOrgBluezGattPropertyNotifyingName{"Notifying"};
constexpr auto kOrgBluezGattCharacteristicReadValueMethodName{"ReadValue"};
constexpr auto kOrgBluezGattCharacteristicWriteValueMethodName{"WriteValue"};
constexpr auto kOrgBluezGattCharacteristicStartNotifyMethodName{"StartNotify"};
constexpr auto kOrgBluezGattCharacteristicStopNotifyMethodName{"StopNotify"};
constexpr auto kOrgBluezGattCharacteristicReadValueOutArgs{"ay"};
constexpr auto kOrgBluezGattCharacteristicReadValueInArgs{"a{sv}"};
constexpr auto kOrgBluezGattCharacteristicWriteValueOutArgs{""};
constexpr auto kOrgBluezGattCharacteristicWriteValueInArg1{"ay"};
constexpr auto kOrgBluezGattCharacteristicWriteValueInArg2{"a{sv}"};

} // namespace dbus_gatt

#endif //DBUS_GATT_CONSTANTS_H_
