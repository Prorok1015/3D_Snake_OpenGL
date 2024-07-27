cmake_minimum_required(VERSION 3.16)
project(editor_system)

add_library(editor_system)
file(GLOB_RECURSE SE_EDT_SOURCES CONFIGURE_DEPENDS "*.cpp")
target_sources(editor_system PRIVATE ${SE_EDT_SOURCES})


add_subdirectory(${ENGINE_CORE_PATH} core)
target_link_libraries(editor_system PUBLIC core)

target_link_directories(editor_system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})