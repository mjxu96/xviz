
add_subdirectory("src")
add_subdirectory("examples")

if (BUILD_TEST)
    message("Enable building tests")
    add_subdirectory("test")
endif (BUILD_TEST)
