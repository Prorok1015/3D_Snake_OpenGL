cmake_minimum_required(VERSION 3.16)
project(resource_system)
add_library(resource_system)

file(GLOB_RECURSE SE_RESOURCE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SE_RESOURCE_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} glm common assimp engine_logger image)
