//
// Created by romanso on 09/07/2021.
//

#include "gtest/gtest.h"
#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "dbus_gatt/gatt_uuid.h"

static constexpr auto kValidGATTUUID{"00002a31-0000-1000-8000-00805f9b34fb"};

TEST(GattUuidTestSuite, InvalidUiidSomeString) {
    EXPECT_THROW(dbus_gatt::GattUUID("fake_uuid"), InvalidUUID);
}

class GattUUIDTestSuiteInvalidCharacter: public testing::TestWithParam<char> {
public:
    void SetUp() override {
        str_uuid_.replace(0,1,1,GetParam());
    }
    std::string str_uuid_ = kValidGATTUUID;
};

TEST_P(GattUUIDTestSuiteInvalidCharacter, InvalidUiidSimbols) {
    EXPECT_THROW(dbus_gatt::GattUUID(std::string(str_uuid_)), InvalidUUID);
}

INSTANTIATE_TEST_SUITE_P(InvalidCharacterRange_g_z,
                         GattUUIDTestSuiteInvalidCharacter,
                         testing::Range('g', static_cast<char>('z'+1)));
INSTANTIATE_TEST_SUITE_P(InvalidCharacterRange_G_Z,
                         GattUUIDTestSuiteInvalidCharacter,
                         testing::Range('G', static_cast<char>('Z'+1)));

TEST(GattUuidTestSuite, ValidUiid) {
    EXPECT_NO_THROW(dbus_gatt::GattUUID(std::string(kValidGATTUUID)));
}

TEST(GattUuidTestSuite, GetUUID) {
    std::string testing_str_uuid = kValidGATTUUID;
    auto testing_uuid = dbus_gatt::GattUUID(testing_str_uuid);
    EXPECT_EQ(testing_str_uuid, testing_uuid.uuid());
}