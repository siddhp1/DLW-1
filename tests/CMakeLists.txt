include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB TEST_SOURCES "*.cpp")
add_executable(dlw-1_tests ${TEST_SOURCES})

target_link_libraries(dlw-1_tests PRIVATE GTest::gtest_main)
target_link_libraries(dlw-1_tests PRIVATE dlw1)

include(GoogleTest)
gtest_discover_tests(dlw-1_tests)
