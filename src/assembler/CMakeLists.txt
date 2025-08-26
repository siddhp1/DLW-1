add_executable(dlw1_assembler main.cpp)

target_link_libraries(dlw1_assembler PRIVATE logger cxxopts)

target_compile_definitions(dlw1_assembler PRIVATE PROJECT_VERSION="${PROJECT_VERSION}" APP_NAME="dlw1_assembler")

add_custom_target(copy_assembly_program ALL 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/resources/sample_program.s"
		"$<TARGET_FILE_DIR:dlw1_assembler>/sample_program.s"
	COMMENT "Copying sample_program.s to output directory"
	VERBATIM
)
add_dependencies(dlw1_assembler copy_assembly_program)
