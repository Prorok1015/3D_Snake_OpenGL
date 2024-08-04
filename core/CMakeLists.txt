cmake_minimum_required(VERSION 3.16)
project(core)
set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

add_library(core)
file(GLOB_RECURSE SE_CORE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(core PRIVATE ${SE_CORE_SOURCES})

add_subdirectory(code/image)
target_link_libraries(core image)

add_subdirectory(${THIRD_PARTY_PATH}/glfw glfw)
add_subdirectory(${THIRD_PARTY_PATH}/glm glm)
#add_subdirectory(${THIRD_PARTY_PATH}/glad glad)
add_subdirectory(${THIRD_PARTY_PATH}/assimp assimp)

target_sources(core PRIVATE 
"${THIRD_PARTY_PATH}/imgui/backends/imgui_impl_glfw.cpp"
"${THIRD_PARTY_PATH}/imgui/backends/imgui_impl_opengl3.cpp"
"${THIRD_PARTY_PATH}/imgui/imgui.cpp" 
"${THIRD_PARTY_PATH}/imgui/imgui_draw.cpp"
"${THIRD_PARTY_PATH}/imgui/imgui_widgets.cpp" 
"${THIRD_PARTY_PATH}/imgui/imgui_tables.cpp"
"${THIRD_PARTY_PATH}/imgui/imgui_demo.cpp"
)

target_include_directories(core PUBLIC ${THIRD_PARTY_PATH}/imgui)

#tmp need separate to components
target_include_directories(core PUBLIC code)

target_link_libraries(core glfw)
#target_link_libraries(core glad)
target_link_libraries(core glm)
target_link_libraries(core assimp)

target_compile_definitions(core PUBLIC GLFW_INCLUDE_NONE=1) 
target_compile_definitions(core PUBLIC GLM_ENABLE_EXPERIMENTAL=1)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/code/render/drivers/driver_implimentations/driver_opengl")
target_link_libraries(core drv_opengl)

project(core_exe)
set(CMAKE_CXX_STANDARD 20)
add_executable(core_exe main.cpp)
target_link_libraries(core_exe core)
