cmake_minimum_required(VERSION 3.16)
project(common)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} INTERFACE glm)

#glm
add_subdirectory(${THIRD_PARTY_PATH}/glm glm)
target_compile_definitions(glm INTERFACE GLM_ENABLE_EXPERIMENTAL=1)
