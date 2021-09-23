include(FetchContent)
set(BUILD_GMOCK)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)


file(GLOB TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/unit_test*.cpp)
add_executable(dbus-gatt-unit-test ${TEST_SRC})


target_link_libraries(dbus-gatt-unit-test
        ${PROJECT_NAME}
        ${GLIB_GIO_LIBRARIES}
        ${GLIB_LIBRARIES}
        ${DBUS_LIBRARIES}
        ${GLIB_GOBJECT_LIBRARIES}
        pthread
        gtest_main
        )

add_test(dbus-gatt-unit-test dbus-gatt-unit-test)
