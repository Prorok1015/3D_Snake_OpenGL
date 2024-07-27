cmake_minimum_required(VERSION 3.16)
project(build_all)

set(THIRD_PARTY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib3dparty")
set(ENGINE_CORE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/core")

set(SE_PROJECT_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(editor)