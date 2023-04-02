cmake_minimum_required(VERSION 3.14)

if(NOT XVIZ_VERSION)
  set(XVIZ_VERSION "l.0.0")
endif()

project(
  xviz
  VERSION ${XVIZ_VERSION}
  DESCRIPTION "C++ Implementation of XVIZ Protocol"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(XVIZ_BUILD_TESTS AND XVIZ_TEST_COVERAGE)
  message(STATUS "Appending coverage flags to compile command")
  include(cmake/CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif()

add_subdirectory("src")

if(XVIZ_BUILD_TESTS)
  enable_testing()
  add_subdirectory("tests")
endif()

if (XVIZ_BUILD_EXAMPLES)
  add_subdirectory("examples")
endif()
