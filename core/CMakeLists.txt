cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

#components
add_subdirectory("components/common")
add_subdirectory("components/common/logger")
add_subdirectory("components/application")
add_subdirectory("components/gui/backends/interface")
add_subdirectory("components/gui/backends/opengl")
add_subdirectory("components/gui/gui_system")
add_subdirectory("components/render/drivers/impls/opengl")
add_subdirectory("components/render/drivers/interface")
add_subdirectory("components/render/render_system")
add_subdirectory("components/input")
add_subdirectory("components/resource")
add_subdirectory("components/game_system")
add_subdirectory("components/scene")
add_subdirectory("components/image")
add_subdirectory("components/windows")

#TEST_PROJECT
project(core_exe)
set(CMAKE_CXX_STANDARD 20)

add_executable(core_exe main.cpp)
target_link_libraries(core_exe application)
