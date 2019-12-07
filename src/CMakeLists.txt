
add_subdirectory("proto")

include_directories(${CLIENT_ROOT_FOLDER}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CLIENT_ROOT_FOLDER}/bin)

add_executable(mmm main.cpp)

add_library(xviz)
target_link_libraries(xviz xviz-pb)

target_link_libraries(mmm xviz)