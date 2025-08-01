cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
  DLW1
  VERSION 0.0.1
  DESCRIPTION "DLW-1 Emulator"
  LANGUAGES CXX C)

add_subdirectory(src)
add_subdirectory(tests)
