add_executable(dlw1_app main.cpp)

target_link_libraries(dlw1_app PRIVATE dlw1)

add_custom_target(copy_program ALL 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/resources/program.bin"
		"$<TARGET_FILE_DIR:dlw1_app>/program.bin"
	COMMENT "Copying program.bin to output directory"
	VERBATIM
)
add_dependencies(dlw1_app copy_program)
