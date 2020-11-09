cmake_minimum_required(VERSION 3.10)

# set the project name
project(libssbase)

include_directories("${CMAKE_SOURCE_DIR}/include")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/lib")

# library
add_subdirectory("${CMAKE_SOURCE_DIR}/src")

# unit test
add_subdirectory("${CMAKE_SOURCE_DIR}/test")