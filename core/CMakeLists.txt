cmake_minimum_required(VERSION 3.16)
project(common)
add_library(common INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE "code/common")

project(core)
set(CMAKE_CXX_STANDARD 20)
add_library(core INTERFACE)

#target_link_directories(${PROJECT_NAME} application)

#components
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

#assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${THIRD_PARTY_PATH}/assimp assimp)

#glm
add_subdirectory(${THIRD_PARTY_PATH}/glm glm)
target_compile_definitions(glm INTERFACE GLM_ENABLE_EXPERIMENTAL=1)

#TEST_PROJECT
project(core_exe)
set(CMAKE_CXX_STANDARD 20)
add_executable(core_exe main.cpp)
#target_compile_definitions(resource_system PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/") 
target_link_libraries(core_exe core application)
