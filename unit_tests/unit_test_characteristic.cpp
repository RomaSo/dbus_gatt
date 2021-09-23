// MIT License
//
// Copyright (c) 2020 RomanSo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "dbus_gatt/dbus_gatt_characteristic_read_only_value.h"
#include "dbus_gatt/dbus_gatt_constants.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "gtest/gtest.h"

class CharacteristicTestSuiteFlags: public testing::TestWithParam<dbus_gatt::CharacteristicFlag> {};

TEST_P(CharacteristicTestSuiteFlags, ReadOnlyCharacteristicConstructorValidFlags) {
    EXPECT_NO_THROW({
        auto a = dbus_gatt::CharacteristicReadOnlyValue(
            "test_read_only_value",
            dbus_gatt::GattUUID("00002a31-0000-1000-8000-00805f9b34fb"),
            GetParam(),
            dbus_gatt::DBusGattVariantT(static_cast<int32_t>(0)));
    });
}

INSTANTIATE_TEST_SUITE_P(ValidFlags,
                         CharacteristicTestSuiteFlags,
                         testing::Values(dbus_gatt::kCharacteristicFlagRead,
                                         dbus_gatt::kCharacteristicFlagEncryptRead,
                                         dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead,
                                         dbus_gatt::kCharacteristicFlagSecureRead));

class CharacteristicTestSuiteInvalidFlags: public CharacteristicTestSuiteFlags {};

TEST_P(CharacteristicTestSuiteInvalidFlags, ReadOnlyCharacteristicConstructorInvalidFlags) {
    EXPECT_THROW(
        {
            auto a = dbus_gatt::CharacteristicReadOnlyValue(
                "test_read_only_value",
                dbus_gatt::GattUUID("00002a31-0000-1000-8000-00805f9b34fb"),
                GetParam(),
                dbus_gatt::DBusGattVariantT(static_cast<int32_t>(0)));
        },
        InvalidCharacteristicFlags);
}

INSTANTIATE_TEST_SUITE_P(InvalidFlags,
                         CharacteristicTestSuiteInvalidFlags,
                         testing::Values(dbus_gatt::kCharacteristicFlagWrite,
                                         dbus_gatt::kCharacteristicFlagWriteWithoutResponse,
                                         dbus_gatt::kCharacteristicFlagAuthenticatedSignedWrites,
                                         dbus_gatt::kCharacteristicFlagReliableWrite,
                                         dbus_gatt::kCharacteristicFlagWritableAuxiliaries,
                                         dbus_gatt::kCharacteristicFlagEncryptWrite,
                                         dbus_gatt::kCharacteristicFlagEncryptAuthenticatedWrite,
                                         dbus_gatt::kCharacteristicFlagSecureWrite));