
cmake_minimum_required(VERSION 3.5.1)
SET(CMAKE_C_COMPILER /usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)

project(xviz-all)

SET(XVIZ_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("cmake")
