cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

#components
add_subdirectory("code/common")
add_subdirectory("code/common/logger")
add_subdirectory("code/application")
add_subdirectory("code/debug_ui")
add_subdirectory("code/render/drivers/driver_implimentations/driver_opengl")
add_subdirectory("code/render/drivers/driver_interface")
add_subdirectory("code/render/render_system")
add_subdirectory("code/input")
add_subdirectory("code/resource")
add_subdirectory("code/game_system")
add_subdirectory("code/scene")
add_subdirectory("code/image")
add_subdirectory("code/windows")

#TEST_PROJECT
project(core_exe)
set(CMAKE_CXX_STANDARD 20)

add_executable(core_exe main.cpp "code/render/drivers/driver_interface/rnd_buffer_layout.h")
target_link_libraries(core_exe application)
