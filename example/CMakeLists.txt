add_executable(example_${PROJECT_NAME} main.cpp)

target_link_libraries(
        example_${PROJECT_NAME}
        ${PROJECT_NAME}
        ${GLIB_GIO_LIBRARIES}
        ${GLIB_LIBRARIES}
        ${DBUS_LIBRARIES}
        ${GLIB_GOBJECT_LIBRARIES}
        pthread
)

install(
        TARGETS
        example_${PROJECT_NAME}
        RUNTIME DESTINATION bin
)
