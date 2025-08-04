CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG v1.17.0
  VERSION 1.17.0
  OPTIONS
    "INSTALL_GTEST OFF"
    "gtest_force_shared_crt ON"
)

enable_testing()

file(GLOB TEST_SOURCES "*.cpp")
add_executable(dlw1_tests ${TEST_SOURCES})

target_link_libraries(dlw1_tests PRIVATE GTest::gtest_main)
target_link_libraries(dlw1_tests PRIVATE dlw1)

include(GoogleTest)
gtest_discover_tests(dlw1_tests)
