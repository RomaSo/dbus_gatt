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

#include "dbus_gatt/dbus_gatt_exceptions.h"
#include "dbus_gatt/gatt_uuid.h"
#include "gtest/gtest.h"

static constexpr auto kValidGATTUUID{"00002a31-0000-1000-8000-00805f9b34fb"};

TEST(GattUuidTestSuite, InvalidUiidSomeString) {
    EXPECT_THROW(dbus_gatt::GattUUID("fake_uuid"), InvalidUUID);
}

class GattUUIDTestSuiteInvalidCharacter: public testing::TestWithParam<char> {
public:
    void SetUp() override {
        str_uuid_.replace(0, 1, 1, GetParam());
    }
    std::string str_uuid_ = kValidGATTUUID;
};

TEST_P(GattUUIDTestSuiteInvalidCharacter, InvalidUiidSimbols) {
    EXPECT_THROW(dbus_gatt::GattUUID(std::string(str_uuid_)), InvalidUUID);
}

INSTANTIATE_TEST_SUITE_P(InvalidCharacterRange_g_z,
                         GattUUIDTestSuiteInvalidCharacter,
                         testing::Range('g', static_cast<char>('z' + 1)));
INSTANTIATE_TEST_SUITE_P(InvalidCharacterRange_G_Z,
                         GattUUIDTestSuiteInvalidCharacter,
                         testing::Range('G', static_cast<char>('Z' + 1)));

TEST(GattUuidTestSuite, ValidUiid) {
    EXPECT_NO_THROW(dbus_gatt::GattUUID(std::string(kValidGATTUUID)));
}

TEST(GattUuidTestSuite, GetUUID) {
    std::string testing_str_uuid = kValidGATTUUID;
    auto testing_uuid = dbus_gatt::GattUUID(testing_str_uuid);
    EXPECT_EQ(testing_str_uuid, testing_uuid.uuid());
}