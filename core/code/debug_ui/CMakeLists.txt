cmake_minimum_required(VERSION 3.16)
project(debug_ui)
add_library(debug_ui)
file(GLOB_RECURSE SE_DEBUG_UI_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SE_DEBUG_UI_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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

target_link_libraries(${PROJECT_NAME} common window game input_system application)