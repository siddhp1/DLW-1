enable_testing()

file(GLOB TEST_SOURCES "*.cpp")
add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(unit_tests PRIVATE dlw1_emulator GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
