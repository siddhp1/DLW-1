# Core DLW-1 assembler library
add_library(dlw1_assembler STATIC assembler.cpp lexer.cpp linestream.cpp token.cpp)

target_include_directories(dlw1_assembler PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
																								 $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_link_libraries(dlw1_assembler PRIVATE logger)

# Assembler executable
add_executable(assembler main.cpp)

target_link_libraries(assembler PRIVATE dlw1_assembler logger cxxopts)

target_compile_definitions(assembler PRIVATE PROJECT_VERSION="${PROJECT_VERSION}" APP_NAME="assembler")

add_custom_target(copy_assembly_program ALL
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/resources/sample_program.s"
		"$<TARGET_FILE_DIR:assembler>/sample_program.s"
	COMMENT "Copying sample_program.s to output directory"
	VERBATIM
)
add_dependencies(assembler copy_assembly_program)
