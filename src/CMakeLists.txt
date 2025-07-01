add_subdirectory(dlw-1)

add_executable(dlw1-simulator main.cpp)
target_link_libraries(dlw1-simulator PRIVATE dlw-1)
