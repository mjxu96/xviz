
add_subdirectory("src")
add_subdirectory("examples")

if (BUILD_TEST)
    message("---------------------")
    message("Enable building tests")
    message("---------------------")
    add_subdirectory("test")
endif (BUILD_TEST)
