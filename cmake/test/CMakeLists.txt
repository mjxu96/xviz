cmake_minimum_required(VERSION 3.5.1)

project(libxviz-test)
SET(CMAKE_CXX_FLAGS "-std=c++17 -pthread -fPIC -O3 -DNDEBUG")

find_package(GTest REQUIRED)
if (GTEST_FOUND)
    message(STATUS "gtest library found")
else()
    message(FATAL_ERROR "gtest library is needed but cant be found. If you don't want to build tests, please set -DBUILD_TEST=OFF.")
endif()

include_directories(${XVIZ_ROOT_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(xviz-test-lib
  ${XVIZ_ROOT_DIR}/test/test_utils.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${XVIZ_ROOT_DIR}/bin)
add_executable(xviz-test 
  ${XVIZ_ROOT_DIR}/test/test_all.cpp
)

target_link_libraries(xviz-test xviz-test-lib xviz ${GTEST_BOTH_LIBRARIES})

gtest_add_tests(TARGET xviz-test)
                # TEST_LIST xvizTests)
# set_tests_properties(${xvizTests} PROPERTIES TIMEOUT 3)