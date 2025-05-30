cmake_minimum_required(VERSION 3.18)

project(foxlog-example)

add_executable(foxlog-example-c ${CMAKE_CURRENT_SOURCE_DIR}/example.c)
add_executable(foxlog-example-cpp ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp)

target_include_directories(foxlog-example-c PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(foxlog-example-cpp PRIVATE ${CMAKE_SOURCE_DIR}/include)