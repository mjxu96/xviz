
cmake_minimum_required(VERSION 3.5.1)
SET(CMAKE_C_COMPILER /usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)

SET(XVIZ_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("cmake")


# example
project(libxviz-example)
SET(CMAKE_CXX_FLAGS "-std=c++17 -pthread -fPIC -O3 -DNDEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${XVIZ_ROOT_DIR}/bin)
add_executable(example example.cpp)
target_link_libraries(example xviz)

add_executable(example-server example_server.cpp)
target_link_libraries(example-server third-party)