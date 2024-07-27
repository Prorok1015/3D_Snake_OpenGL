cmake_minimum_required(VERSION 3.13)
project(core)
set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_library(core)
# Define MY_SOURCES to be a list of all the source files for my project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(core PRIVATE ${MY_SOURCES})

add_subdirectory(${THERD_PATRY_PATH}/glfw glfw)
add_subdirectory(${THERD_PATRY_PATH}/glm glm)
add_subdirectory(${THERD_PATRY_PATH}/glad glad)
add_subdirectory(${THERD_PATRY_PATH}/assimp assimp)
add_subdirectory(${THERD_PATRY_PATH}/stb_image stb_image)

target_sources(core PRIVATE 
"${THERD_PATRY_PATH}/imgui/backends/imgui_impl_glfw.cpp"
"${THERD_PATRY_PATH}/imgui/backends/imgui_impl_opengl3.cpp"
"${THERD_PATRY_PATH}/imgui/imgui.cpp" 
"${THERD_PATRY_PATH}/imgui/imgui_draw.cpp"
"${THERD_PATRY_PATH}/imgui/imgui_widgets.cpp" 
"${THERD_PATRY_PATH}/imgui/imgui_tables.cpp"
"${THERD_PATRY_PATH}/imgui/imgui_demo.cpp"
)

target_include_directories(core PUBLIC ${THERD_PATRY_PATH}/imgui)
target_include_directories(core PUBLIC code)

target_link_libraries(core glfw)
target_link_libraries(core glad)
target_link_libraries(core glm)
target_link_libraries(core assimp)
target_link_libraries(core stb_image)

target_compile_definitions(core PUBLIC GLFW_INCLUDE_NONE=1) 
target_compile_definitions(core PUBLIC GLM_ENABLE_EXPERIMENTAL=1) 

project(core_exe)
set(CMAKE_CXX_STANDARD 20)
add_executable(core_exe main.cpp)
target_link_libraries(core_exe core)