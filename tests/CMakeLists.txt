add_executable(unit_tests
    unit/test_sensor_service.cpp
    unit/test_main.cpp
)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(unit_tests
    PRIVATE
        sensor_core
        GTest::gtest
        GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(unit_tests)


add_executable(integration_tests
    integration/test_sensor_repository.cpp
    unit/test_main.cpp
)

target_include_directories(integration_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(integration_tests
    PRIVATE
        sensor_core
        GTest::gtest
        GTest::gmock
)

gtest_discover_tests(integration_tests)