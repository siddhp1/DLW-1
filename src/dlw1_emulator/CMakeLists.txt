# Core DLW-1 emulator library
add_library(dlw1_emulator STATIC config.cpp cpu.cpp emulator.cpp instruction.cpp memory.cpp)

target_include_directories(dlw1_emulator PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
												$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_link_libraries(dlw1_emulator PRIVATE logger)

# Emulator executable
add_executable(emulator main.cpp)

target_link_libraries(emulator PRIVATE dlw1_emulator logger cxxopts)

target_compile_definitions(emulator PRIVATE PROJECT_VERSION="${PROJECT_VERSION}" APP_NAME="emulator")

add_custom_target(copy_binary_program ALL 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/resources/sample_program.bin"
		"$<TARGET_FILE_DIR:emulator>/sample_program.bin"
	COMMENT "Copying sample_program.bin to output directory"
	VERBATIM
)
add_dependencies(emulator copy_binary_program)
