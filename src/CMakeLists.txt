add_subdirectory("proto")
add_subdirectory("utils")
add_subdirectory("builder")

# add_library(xviz-utils
#   utils/utils.cpp
# )

# add_library(xviz-builder
#   builder/metadata.cpp
#   builder/base_builder.cpp
#   builder/pose.cpp
#   builder/primitive.cpp
#   builder/xviz_builder.cpp
# )

# target_link_libraries(xviz-builder xviz-utils)

include_directories(${XVIZ_ROOT_FOLDER}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${XVIZ_ROOT_FOLDER}/bin)

add_executable(mmm main.cpp)

add_library(xviz-message
  message.cpp
)

add_library(xviz)
target_link_libraries(xviz xviz-message xviz-builder xviz-pb xviz-utils)

target_link_libraries(mmm xviz)