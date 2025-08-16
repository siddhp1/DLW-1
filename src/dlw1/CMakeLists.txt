add_library(dlw1 config.cpp cpu.cpp emulator.cpp instruction.cpp memory.cpp)

target_include_directories(dlw1 PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                       $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_link_libraries(dlw1 PRIVATE logger)
