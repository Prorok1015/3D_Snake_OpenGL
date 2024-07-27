cmake_minimum_required(VERSION 3.16)
project(image)

add_library(image)
target_sources(image PRIVATE Image.cpp)
target_link_directories(image PUBLIC Image.h)

add_subdirectory(${THIRD_PARTY_PATH}/stb_image stb_image)
target_link_libraries(image stb_image)


project(image_test)
add_executable(image_test "Source.cpp")

target_link_libraries(image)