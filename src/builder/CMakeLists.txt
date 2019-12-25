
include_directories(${XVIZ_ROOT_FOLDER}/include)

add_library(xviz-builder
  metadata.cpp
  base_builder.cpp
  pose.cpp
  primitive.cpp
  xviz_builder.cpp
)

target_link_libraries(xviz-builder)