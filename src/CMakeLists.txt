add_subdirectory(dlw-1)

add_executable(dlw1 main.cpp)
target_link_libraries(dlw1 PRIVATE dlw-1)
