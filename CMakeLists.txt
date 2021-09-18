cmake_minimum_required(VERSION 3.13)
project(dbus_gatt VERSION 0.0.1 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_UNIT_TESTS "Enable unit testing" ON)
option(BUILD_DBUS_GATT_EXAMPLES "Enable unit testing" ON)

find_package(Boost 1.62.0 REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost library not found! Install 'libboost-all-dev' ")
endif ()

find_package(GLIB 2.00.0 REQUIRED gio gobject)
if (NOT GLIB_FOUND)
    message(FATAL_ERROR "GLib not found! Install 'libglib2.0-dev' ")
endif ()

find_package(DBus REQUIRED)
if (NOT GLIB_FOUND)
    message(FATAL_ERROR "gDBus library not found ")
endif ()

include_directories(
    include
    ${GLIB_INCLUDE_DIRS}
    ${DBUS_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

add_subdirectory(src)

add_library(dbus_gatt::dbus_gatt ALIAS dbus_gatt)

target_include_directories(dbus_gatt PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

if (BUILD_DBUS_GATT_EXAMPLES)
    add_subdirectory(example)
endif (BUILD_DBUS_GATT_EXAMPLES)

if (BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(unit_tests)
endif (BUILD_UNIT_TESTS)


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include/${PROJECT_NAME}
)