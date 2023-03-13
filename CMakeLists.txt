cmake_minimum_required(VERSION 3.14)

if(NOT XVIZ_VERSION)
  set(XVIZ_VERSION "l.0.0")
endif()

project(
  xviz
  VERSION ${XVIZ_VERSION}
  DESCRIPTION "C++ Implementation of XVIZ Protocol"
  LANGUAGES CXX)

add_subdirectory("src")
if(XVIZ_BUILD_TESTS)
  enable_testing()
  add_subdirectory("tests")
endif()

add_subdirectory("examples")
