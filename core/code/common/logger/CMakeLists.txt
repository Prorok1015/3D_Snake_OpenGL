cmake_minimum_required(VERSION 3.16)
project(engine_logger)
add_library(engine_logger INTERFACE)
add_library(engine_logger::engine_logger ALIAS engine_logger)

target_include_directories(engine_logger INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
