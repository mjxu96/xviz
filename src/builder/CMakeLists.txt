
include_directories(${XVIZ_ROOT_FOLDER}/include)

add_library(xviz-builder
  base_builder.cpp
  pose.cpp
)

target_link_libraries(xviz-builder xviz-pb)