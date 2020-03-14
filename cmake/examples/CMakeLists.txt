project(libxviz-examples)
SET(CMAKE_CXX_FLAGS "-std=c++17 -pthread -fPIC -O3 -DNDEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${XVIZ_ROOT_DIR}/bin)

add_executable(example 
  ${XVIZ_ROOT_DIR}/examples/example.cpp
)
target_link_libraries(example xviz)

add_executable(example-server 
  ${XVIZ_ROOT_DIR}/examples/example_xviz_server.cpp
)

target_link_libraries(example-server xviz)