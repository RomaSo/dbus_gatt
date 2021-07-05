include(FetchContent)
set(BUILD_GMOCK OFF)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)
add_executable(test_characteristic_impl test_characteristic_impl.cpp)

target_link_libraries(test_characteristic_impl
        ${PROJECT_NAME}
        ${GLIB_GIO_LIBRARIES}
        ${GLIB_LIBRARIES}
        ${DBUS_LIBRARIES}
        ${GLIB_GOBJECT_LIBRARIES}
        pthread
        gtest_main
        )

add_test(test_characteristic_impl test_characteristic_impl)