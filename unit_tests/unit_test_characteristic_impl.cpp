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
#include "gtest/gtest.h"
#include "dbus_gatt/dbus_gatt_constants.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"

#include "../src/impl/dbus_gatt_characteristic_impl.h"

std::vector<dbus_gatt::CharacteristicFlag> kTestingReadFlagsCombination = {
        dbus_gatt::kCharacteristicFlagRead,
        dbus_gatt::kCharacteristicFlagEncryptRead,
        dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead,
        dbus_gatt::kCharacteristicFlagSecureRead,
        dbus_gatt::kCharacteristicFlagRead | dbus_gatt::kCharacteristicFlagEncryptRead,
        dbus_gatt::kCharacteristicFlagRead | dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead,
        dbus_gatt::kCharacteristicFlagRead | dbus_gatt::kCharacteristicFlagSecureRead,
        dbus_gatt::kCharacteristicFlagRead | dbus_gatt::kCharacteristicFlagEncryptRead |
        dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead,
        dbus_gatt::kCharacteristicFlagRead | dbus_gatt::kCharacteristicFlagEncryptRead |
        dbus_gatt::kCharacteristicFlagSecureRead,
        dbus_gatt::kCharacteristicFlagRead | dbus_gatt::kCharacteristicFlagEncryptRead |
        dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead |
        dbus_gatt::kCharacteristicFlagSecureRead,
        dbus_gatt::kCharacteristicFlagEncryptRead |
        dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead,
        dbus_gatt::kCharacteristicFlagEncryptRead | dbus_gatt::kCharacteristicFlagSecureRead,
        dbus_gatt::kCharacteristicFlagEncryptRead |
        dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead |
        dbus_gatt::kCharacteristicFlagSecureRead,
        dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead |
        dbus_gatt::kCharacteristicFlagSecureRead
};

std::vector<dbus_gatt::CharacteristicFlag> kTestingWriteFlagsCombination = {
        dbus_gatt::kCharacteristicFlagWrite,
        dbus_gatt::kCharacteristicFlagWriteWithoutResponse,
        dbus_gatt::kCharacteristicFlagNotify,
        dbus_gatt::kCharacteristicFlagAuthenticatedSignedWrites,
        dbus_gatt::kCharacteristicFlagExtendedProperties,
        dbus_gatt::kCharacteristicFlagReliableWrite,
        dbus_gatt::kCharacteristicFlagWritableAuxiliaries,
        dbus_gatt::kCharacteristicFlagEncryptWrite,
        dbus_gatt::kCharacteristicFlagEncryptAuthenticatedWrite,
        dbus_gatt::kCharacteristicFlagSecureWrite
};

class CharacteristicTestSuiteCombineFlags
        : public testing::TestWithParam<std::tuple<dbus_gatt::CharacteristicFlag, dbus_gatt::CharacteristicFlag>> {
};

class CharacteristicTestSuiteFlags : public testing::TestWithParam<dbus_gatt::CharacteristicFlag> {
};


TEST_P(CharacteristicTestSuiteCombineFlags, ReadOnlyCharacteristicConstructorThrow) {
    EXPECT_THROW({
                     auto a = dbus_gatt::DBusGattCharacteristicImpl(
                             "fake_uuid",
                             std::get<0>(GetParam()) | std::get<1>(GetParam()),
                             dbus_gatt::DBusGattVariantT(static_cast<int32_t>(0))
                     );
                 }, InvalidCharacteristicFlags);
}

INSTANTIATE_TEST_SUITE_P(
        InvalidFlags,
        CharacteristicTestSuiteCombineFlags,
        testing::Combine(
                testing::ValuesIn(kTestingWriteFlagsCombination),
                testing::ValuesIn(kTestingReadFlagsCombination)
        )
);

TEST_P(CharacteristicTestSuiteFlags, ReadOnlyCharacteristicConstructorNoThrow) {
    EXPECT_NO_THROW({
                        auto a = dbus_gatt::DBusGattCharacteristicImpl(
                                "fake_uuid",
                                GetParam(),
                                dbus_gatt::DBusGattVariantT(static_cast<int32_t>(0))
                        );
                    });
}

INSTANTIATE_TEST_SUITE_P(ValidFlags, CharacteristicTestSuiteFlags, testing::ValuesIn(kTestingReadFlagsCombination));

