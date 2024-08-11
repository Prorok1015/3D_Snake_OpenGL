cmake_minimum_required(VERSION 3.16)

project(editor)
set(CMAKE_CXX_STANDARD 20)
add_executable(editor main.cpp)

add_subdirectory(code/editor_system)
target_link_libraries(editor editor_system)

target_compile_definitions(resource_system PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/") 
