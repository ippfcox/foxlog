cmake_minimum_required(VERSION 3.16)

project("foxlog")

aux_source_directory("${CMAKE_SOURCE_DIR}/work" SRC)

add_executable(${PROJECT_NAME} ${SRC})
