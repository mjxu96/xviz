add_subdirectory("proto")
add_subdirectory("builder")

include_directories(${XVIZ_ROOT_FOLDER}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${XVIZ_ROOT_FOLDER}/bin)

add_executable(mmm main.cpp)

add_library(xviz-message
  message.cpp
)

add_library(xviz)
target_link_libraries(xviz xviz-message xviz-builder xviz-pb)

target_link_libraries(mmm xviz)