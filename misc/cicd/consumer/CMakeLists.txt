cmake_minimum_required(VERSION 3.14)

find_package(websocketpp REQUIRED)
find_package(lodepng REQUIRED)
find_package(xviz REQUIRED)

function(build_examples)
  foreach(example_file ${ARGV})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(${example_name} ${example_file})
    target_link_libraries(${example_name} xviz::xviz websocketpp::websocketpp
                          lodepng::lodepng)
  endforeach(example_file ${ARGV})
endfunction()

file(GLOB example_files ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

build_examples(${example_files})
