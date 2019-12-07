
find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()


include_directories(${CLIENT_ROOT_FOLDER}/include)
include_directories(${PROTOBUF_INCLUDE_DIRS})

add_library(xviz-pb
  session.pb.cc
  core.pb.cc
  annotation.pb.cc
  primitives.pb.cc
  uiprimitives.pb.cc
  style.pb.cc
  options.pb.cc
  envelope.pb.cc
)

target_link_libraries(xviz-pb ${PROTOBUF_LIBRARIES})
