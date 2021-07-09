//
// Created by romanso on 08/07/2021.
//

#include "gtest/gtest.h"
#include "dbus_gatt/dbus_gatt_constants.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "dbus_gatt/dbus_gatt_characteristic_read_only_value.h"

class CharacteristicTestSuiteFlags : public testing::TestWithParam<dbus_gatt::CharacteristicFlag> {
};

TEST_P(CharacteristicTestSuiteFlags, ReadOnlyCharacteristicConstructorValidFlags) {
    EXPECT_NO_THROW({
                        auto a = dbus_gatt::CharacteristicReadOnlyValue(
                                "test_read_only_value",
                                "fake_uuid",
                                GetParam(),
                                dbus_gatt::DBusGattVariantT(static_cast<int32_t>(0))
                        );
                    });
}

INSTANTIATE_TEST_SUITE_P(ValidFlags, CharacteristicTestSuiteFlags, testing::Values(
      dbus_gatt::kCharacteristicFlagRead
    , dbus_gatt::kCharacteristicFlagEncryptRead
    , dbus_gatt::kCharacteristicFlagEncryptAuthenticatedRead
    , dbus_gatt::kCharacteristicFlagSecureRead
));


class CharacteristicTestSuiteInvalidFlags : public CharacteristicTestSuiteFlags {

};


TEST_P(CharacteristicTestSuiteInvalidFlags, ReadOnlyCharacteristicConstructorInvalidFlags) {
    EXPECT_THROW({
                        auto a = dbus_gatt::CharacteristicReadOnlyValue(
                                "test_read_only_value",
                                "fake_uuid",
                                GetParam(),
                                dbus_gatt::DBusGattVariantT(static_cast<int32_t>(0))
                        );
                    }, InvalidCharacteristicFlags);
}

INSTANTIATE_TEST_SUITE_P(InvalidFlags, CharacteristicTestSuiteInvalidFlags, testing::Values(
        dbus_gatt::kCharacteristicFlagWrite
        , dbus_gatt::kCharacteristicFlagWriteWithoutResponse
        , dbus_gatt::kCharacteristicFlagAuthenticatedSignedWrites
        , dbus_gatt::kCharacteristicFlagReliableWrite
        , dbus_gatt::kCharacteristicFlagWritableAuxiliaries
        , dbus_gatt::kCharacteristicFlagEncryptWrite
        , dbus_gatt::kCharacteristicFlagEncryptAuthenticatedWrite
        , dbus_gatt::kCharacteristicFlagSecureWrite
));