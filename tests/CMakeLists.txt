enable_testing()

file(GLOB TEST_SOURCES "*.cpp")
add_executable(dlw1_tests ${TEST_SOURCES})

target_link_libraries(dlw1_tests PRIVATE dlw1 GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(dlw1_tests)
