
function(build_examples)
  foreach(example_file ${ARGV})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(${example_name} ${example_file})
    target_link_libraries(${example_name} xviz)
  endforeach(example_file ${ARGV})
endfunction()

file(GLOB example_files ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

build_examples(${example_files})
