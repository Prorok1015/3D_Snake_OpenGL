cmake_minimum_required(VERSION 3.16)
project(editor)
set(CMAKE_CXX_STANDARD 20)
set(SE_RESOURCE_ABSOLUTE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res/")

add_executable(${PROJECT_NAME} "main.cpp")

add_subdirectory("code/editor_system")
target_link_libraries(${PROJECT_NAME} editor_system)
