
add_subdirectory("third_party")
add_subdirectory("src")

if (BUILD_TEST)
    message("Enable building tests")
    add_subdirectory("test")
endif (BUILD_TEST)
