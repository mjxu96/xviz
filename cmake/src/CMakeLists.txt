
message(STATUS "libxviz CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})


find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()

execute_process(COMMAND ${Protobuf_PROTOC_EXECUTABLE} --version 
  OUTPUT_VARIABLE PROTOC_VERSION
)
string(STRIP ${PROTOC_VERSION} PROTOC_VERSION)
string(SUBSTRING ${PROTOC_VERSION} 10 -1 PROTOC_VERSION)

message(STATUS "Protobuf version: " ${Protobuf_VERSION})
message(STATUS "Protoc version: " ${PROTOC_VERSION})
if (NOT PROTOC_VERSION STREQUAL Protobuf_VERSION)
  message(WARNING "Please make sure your protobuf version and protoc version match with each other. Maybe it can compile but without any guarantee.")
endif()

# generate protobuf files
file(GLOB PROTO_FILES "${XVIZ_ROOT_DIR}/pb/*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

find_package(websocketpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_path(CPP_BASE64_INCLUDE_DIRS "cpp-base64/base64.cpp")


include_directories(${XVIZ_ROOT_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROTOBUF_INCLUDE_DIRS})

# all headers
install(DIRECTORY ${XVIZ_ROOT_DIR}/include/xviz DESTINATION include)

add_library(libxviz_pb ${PROTO_HDRS} ${PROTO_SRCS})
target_include_directories(libxviz_pb PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# protobuf files
# file(GLOB libxviz_pb_sources
#   "${XVIZ_ROOT_DIR}/src/proto/declarativeui.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/session.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/session.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/core.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/annotation.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/primitives.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/uiprimitives.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/style.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/options.pb.cc"
#   "${XVIZ_ROOT_DIR}/src/proto/envelope.pb.cc"
# )
# set(libxviz_sources "${libxviz_sources};${libxviz_pb_sources}")

# utils sources
file(GLOB libxviz_utils_sources
  "${XVIZ_ROOT_DIR}/src/utils/utils.cpp"
  "${XVIZ_ROOT_DIR}/src/utils/base64.cpp"
)
set(libxviz_sources "${libxviz_sources};${libxviz_utils_sources}")

# builder sources
file(GLOB libxviz_builder_sources
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/ui_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/container_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/plot_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/table_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/video_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/panel_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/metric_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/base_ui_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/declarative_ui/base_ui_component_builder.cpp"

  "${XVIZ_ROOT_DIR}/src/builder/metadata.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/base_builder.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/pose.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/primitive.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/time_series.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/ui_primitive.cpp"
  "${XVIZ_ROOT_DIR}/src/builder/xviz_builder.cpp"
)
set(libxviz_sources "${libxviz_sources};${libxviz_builder_sources}")

# io sources
file(GLOB libxviz_io_sources
  "${XVIZ_ROOT_DIR}/src/io/protobuf_writer.cpp"
)
set(libxviz_sources "${libxviz_sources};${libxviz_io_sources}")

# message sources
file(GLOB libxviz_message_sources
  "${XVIZ_ROOT_DIR}/src/message.cpp"
)
set(libxviz_sources "${libxviz_sources};${libxviz_message_sources}")

# server sources
file(GLOB libxviz_server_sources
  "${XVIZ_ROOT_DIR}/src/server/xviz_server.cpp"
  "${XVIZ_ROOT_DIR}/src/server/xviz_handler.cpp"
  "${XVIZ_ROOT_DIR}/src/server/xviz_session.cpp"
)
set(libxviz_sources "${libxviz_sources};${libxviz_server_sources}")

add_library(cpp-base64
  ${CPP_BASE64_INCLUDE_DIRS}/cpp-base64/base64.h
  ${CPP_BASE64_INCLUDE_DIRS}/cpp-base64/base64.cpp
)

add_library(xviz 
  STATIC
  ${libxviz_sources}
)

target_link_libraries(xviz libxviz_pb)
target_link_libraries(xviz ${PROTOBUF_LIBRARIES})
target_link_libraries(xviz websocketpp::websocketpp)
target_link_libraries(xviz cpp-base64)
target_link_libraries(xviz nlohmann_json nlohmann_json::nlohmann_json)


install(TARGETS xviz DESTINATION lib)

target_include_directories(xviz PUBLIC ${XVIZ_ROOT_DIR}/include)
target_include_directories(xviz PUBLIC ${XVIZ_ROOT_DIR}/include/xviz/proto)