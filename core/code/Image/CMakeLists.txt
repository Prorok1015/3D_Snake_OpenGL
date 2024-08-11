cmake_minimum_required(VERSION 3.16)
project(image)
add_library(image)

target_sources(image PRIVATE "image.cpp" "image_loader.cpp")
target_include_directories(image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(${THIRD_PARTY_PATH}/stb_image stb_image)
target_link_libraries(image stb_image common)

project(image_test)
add_executable(image_test "test/Source.cpp")
target_compile_definitions(image_test PRIVATE TESTLIB=1)
target_link_libraries(image_test image)