cmake_minimum_required(VERSION 3.16)
project(core)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME})
file(GLOB_RECURSE SE_CORE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SE_CORE_SOURCES})
#TODO need separate to components
target_include_directories(${PROJECT_NAME} PUBLIC code)

#components
add_subdirectory("code/image")
target_link_libraries(${PROJECT_NAME} image)

add_subdirectory("code/render/drivers/driver_implimentations/driver_opengl")
target_link_libraries(${PROJECT_NAME} drv_opengl)


#glfw TODO: move to window_system component
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(${THIRD_PARTY_PATH}/glfw glfw)
target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE=1) 
target_link_libraries(${PROJECT_NAME} glfw)



#assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${THIRD_PARTY_PATH}/assimp assimp)
target_link_libraries(${PROJECT_NAME} assimp)

#imgui TODO: move to debug_ui component
target_sources(${PROJECT_NAME} PRIVATE 
"${THIRD_PARTY_PATH}/imgui/backends/imgui_impl_glfw.cpp"
"${THIRD_PARTY_PATH}/imgui/backends/imgui_impl_opengl3.cpp"
"${THIRD_PARTY_PATH}/imgui/imgui.cpp" 
"${THIRD_PARTY_PATH}/imgui/imgui_draw.cpp"
"${THIRD_PARTY_PATH}/imgui/imgui_widgets.cpp" 
"${THIRD_PARTY_PATH}/imgui/imgui_tables.cpp"
"${THIRD_PARTY_PATH}/imgui/imgui_demo.cpp"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${THIRD_PARTY_PATH}/imgui)


#TEST_PROJECT
project(core_exe)
set(CMAKE_CXX_STANDARD 20)
add_executable(core_exe main.cpp)
target_link_libraries(core_exe core)
