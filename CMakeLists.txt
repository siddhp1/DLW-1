cmake_minimum_required(VERSION 3.16)
project(DLW1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add subdirectories
add_subdirectory(src)
