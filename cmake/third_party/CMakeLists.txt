
project(xviz-third-party)
SET(CMAKE_CXX_FLAGS "-std=c++17 -pthread -fPIC -O3 -DNDEBUG")

include_directories (${XVIZ_ROOT_DIR}/third_party)

add_library(third-party
  ${XVIZ_ROOT_DIR}/third_party/third_party.cpp
)

target_include_directories(third-party
  PUBLIC
  ${XVIZ_ROOT_DIR}/third_party
)