find_package(GTest REQUIRED)

function(build_tests)
  foreach(test_file ${ARGV})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} xviz GTest::gtest GTest::gtest_main)
    add_test(${test_name} ${test_name})
    # Any test should exceed the time limit
    set_tests_properties(${test_name} PROPERTIES TIMEOUT 60)
  endforeach(test_file ${ARGV})
endfunction()

file(GLOB test_files ${CMAKE_SOURCE_DIR}/tests/*.cc)
build_tests(${test_files})
