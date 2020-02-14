cmake_minimum_required(VERSION 3.5.1)

find_package(GTest REQUIRED)
if (GTEST_FOUND)
    message(STATUS "gtest library found")
else()
    message(FATAL_ERROR "gtest library is needed but cant be found. If you don't want to build tests, please set -DBUILD_TEST=OFF.")
endif()

include_directories(${XVIZ_ROOT_FOLDER}/include)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(xviz-test-lib
  test_utils.cpp
)

add_executable(xviz-test test_all.cpp)

target_link_libraries(xviz-test xviz-test-lib xviz ${GTEST_BOTH_LIBRARIES})

gtest_add_tests(TARGET xviz-test)
                # TEST_LIST xvizTests)
# set_tests_properties(${xvizTests} PROPERTIES TIMEOUT 3)