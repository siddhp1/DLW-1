add_library(logger logger.cpp)

target_include_directories(logger PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                         $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_link_libraries(logger PUBLIC spdlog::spdlog)
