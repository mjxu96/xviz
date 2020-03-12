
cmake_minimum_required(VERSION 3.5.1)

project(libxviz)
SET(CMAKE_CXX_FLAGS "-std=c++17 -pthread -fPIC -O3 -DNDEBUG")


find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()


include_directories(${XVIZ_ROOT_DIR}/include)
include_directories(${PROTOBUF_INCLUDE_DIRS})

add_library(xviz-pb
  ${XVIZ_ROOT_DIR}/src/proto/declarativeui.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/session.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/session.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/core.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/annotation.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/primitives.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/uiprimitives.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/style.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/options.pb.cc
  ${XVIZ_ROOT_DIR}/src/proto/envelope.pb.cc
)

target_link_libraries(xviz-pb ${PROTOBUF_LIBRARIES})

add_library(xviz-utils
  ${XVIZ_ROOT_DIR}/src/utils/utils.cpp
  ${XVIZ_ROOT_DIR}/src/utils/base64.cpp
)

add_library(xviz-ui-builder
  ${XVIZ_ROOT_DIR}/src/builder/declarative_ui/ui_builder.cpp
  ${XVIZ_ROOT_DIR}/src/builder/declarative_ui/container_builder.cpp
  ${XVIZ_ROOT_DIR}/src/builder/declarative_ui/plot_builder.cpp
  ${XVIZ_ROOT_DIR}/src/builder/declarative_ui/table_builder.cpp
  ${XVIZ_ROOT_DIR}/src/builder/declarative_ui/video_builder.cpp
  ${XVIZ_ROOT_DIR}/src/builder/declarative_ui/metric_builder.cpp
  ${XVIZ_ROOT_DIR}/src/builder/declarative_ui/base_ui_builder.cpp
)

target_link_libraries(xviz-ui-builder
  xviz-pb
)

add_library(xviz-builder
  ${XVIZ_ROOT_DIR}/src/builder/metadata.cpp
  ${XVIZ_ROOT_DIR}/src/builder/base_builder.cpp
  ${XVIZ_ROOT_DIR}/src/builder/pose.cpp
  ${XVIZ_ROOT_DIR}/src/builder/primitive.cpp
  ${XVIZ_ROOT_DIR}/src/builder/time_series.cpp
  ${XVIZ_ROOT_DIR}/src/builder/ui_primitive.cpp
  ${XVIZ_ROOT_DIR}/src/builder/xviz_builder.cpp
)

target_link_libraries(xviz-builder xviz-ui-builder)

add_library(xviz-io
  ${XVIZ_ROOT_DIR}/src/io/glb_writer.cpp
)

add_library(xviz-message
  ${XVIZ_ROOT_DIR}/src/message.cpp
)

add_library(xviz-server
  ${XVIZ_ROOT_DIR}/src/server/xviz_server.cpp
  ${XVIZ_ROOT_DIR}/src/server/xviz_handler.cpp
  ${XVIZ_ROOT_DIR}/src/server/xviz_session.cpp
)

target_link_libraries(xviz-server third-party)

add_library(xviz 
  ${XVIZ_ROOT_DIR}/src/xviz.cpp
)

target_link_libraries(xviz xviz-ui-builder xviz-message xviz-builder xviz-io xviz-pb xviz-utils xviz-server third-party)
target_include_directories(xviz PUBLIC ${XVIZ_ROOT_DIR}/include)