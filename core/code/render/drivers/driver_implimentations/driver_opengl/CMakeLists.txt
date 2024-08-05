cmake_minimum_required(VERSION 3.16)
project(drv_opengl)
add_library(drv_opengl)

target_compile_definitions(drv_opengl PUBLIC PROJECT_SOURCE_PATH="${SE_PROJECT_SOURCE_PATH}") 

# temporary add from here as public lib
add_subdirectory("${ENGINE_CORE_PATH}/code/common/logger" engine_logger)
target_link_libraries(${PROJECT_NAME} engine_logger)
# end tmp

add_subdirectory(${THIRD_PARTY_PATH}/glad glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(../../driver_interface drv_interface)
target_link_libraries(${PROJECT_NAME} drv_interface)

file(GLOB_RECURSE SE_DRV_GL_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SE_DRV_GL_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(${PROJECT_NAME} PRIVATE "${ENGINE_CORE_PATH}/code/common")