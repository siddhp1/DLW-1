add_library(dlw-1 STATIC
    cpu.cpp
    instruction.cpp
    memory.cpp
    simulator.cpp
)

target_include_directories(dlw-1 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(dlw-1 PUBLIC cxx_std_20)
