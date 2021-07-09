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

#ifndef DBUS_GATT_SRC_DBUS_GATT_EXCEPTIONS_H_
#define DBUS_GATT_SRC_DBUS_GATT_EXCEPTIONS_H_

#include <exception>
#include <string>

class DBusGATTException : public std::exception {

};

class MethodNotAllowedWithoutFlags : public DBusGATTException {
public:
    explicit MethodNotAllowedWithoutFlags(std::string method_name)
    : error_msg_("method "+ std::move(method_name) + " not allowed without read flags") {
    }

    [[nodiscard]] const char * what () const throw() override  {
        return error_msg_.c_str();
    }
private:
    const std::string error_msg_;
};

class FlagsWithoutMethodProvided : public DBusGATTException {
public:
    explicit FlagsWithoutMethodProvided(std::string method_name)
        : error_msg_("method "+ std::move(method_name) + " do not provided") {
    }

    [[nodiscard]] const char * what () const throw () override {
        return error_msg_.c_str();
    }
private:
    const std::string error_msg_;
};


class InvalidCharacteristicFlags : public DBusGATTException {
public:
    explicit InvalidCharacteristicFlags(std::string message)
    : what_(std::move(message)) {
    }
    [[nodiscard]] const char * what () const noexcept override {
        return what_.c_str();
    }
private:
    std::string what_;
};

class InvalidUUID : public DBusGATTException {
public:
    InvalidUUID(): what_("Invalid dbus uuid") {

    }
    [[nodiscard]] const char * what () const noexcept override {
        return what_.c_str();
    }
private:
    std::string what_;
};

#endif //DBUS_GATT_SRC_DBUS_GATT_EXCEPTIONS_H_
