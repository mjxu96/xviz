/*
 * Project: libxviz
 * Description: C++ Implementation of XVIZ Protocol
 * Author: Minjun Xu (mjxu96@outlook.com)
 * -----
 * MIT License
 * Copyright (c) 2023 Minjun Xu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <xviz/builder/builder.h>
#include <xviz/message.h>
#include <xviz/utils/utils.h>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/util/json_util.h>

#include <iostream>

using namespace xviz;

int main() {
  xviz::MetadataBuilder meta_builder;

  // clang-format off
  auto data = meta_builder
    .Stream("/vehicle_pose")
      .Category(xviz::StreamMetadata::POSE)
    .Stream("/game/time")
      .Category(xviz::StreamMetadata::UI_PRIMITIVE)
    .Stream("/object/vehicles")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::POLYGON)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(
          {
            {"extruded", true},
            {"fill_color", "#40E0D0"},
            {"height", 2.0f}
          })
    .Stream("/object/walkers")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::POLYGON)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(
          {
            {"extruded", true},
            {"fill_color", "#FF69B4"},
            {"height", 1.5f}
          })
    .Stream("/vehicle/acceleration")
      .Category(xviz::StreamMetadata::TIME_SERIES)
        .Type(xviz::StreamMetadata::FLOAT)
        .Unit("m/s^2")
    .Stream("/vehicle/velocity")
      .Category(xviz::StreamMetadata::TIME_SERIES)
        .Type(xviz::StreamMetadata::FLOAT)
        .Unit("m/s")
    .Stream("/traffic/stop_signs")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::POLYLINE)
        .StreamStyle(
          {
            {"stroke_width", 0.1f},
            {"stroke_color", "#FFFFFF"},
          })
        .StyleClass("vertical",
          {
            {"stroke_width", 0.2f},
            {"stroke_color", "#FF0000"},
          })
    .Stream("/traffic/traffic_lights")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::POLYGON)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(
          {
            {"extruded", true},
            {"height", 0.1f}
          })
        .StyleClass("red_state", 
          {
            {"fill_color", "#FF0000"},
          })
        .StyleClass("yellow_state", 
          {
            {"fill_color", "#FFFF00"},
          })
        .StyleClass("green_state", 
          {
            {"fill_color", "#00FF00"},
          })
        .StyleClass("unknown", 
          {
            {"fill_color", "#FFFFFF"},
          })
    .Stream("/lidar/points")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::POINT)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(
          {
            {"point_color_mode", xviz::PointColorMode::ELEVATION},
            {"radius_pixels", 2u}
          })
    .Stream("/radar/points")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::POINT)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(
          {
            {"radius_pixels", 2u}
          })
    .Stream("/drawing/polylines")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::POLYLINE)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(
          {
            {"stroke_width", 2.0f},
            {"stroke_color", "#FFD700"},
          })
    .Stream("/drawing/points")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::POLYLINE)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
    .Stream("/drawing/texts")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::TEXT)
        .Coordinate(xviz::StreamMetadata::IDENTITY)

    .Stream("/time_series/demo/1")
      .Category<xviz::StreamMetadata::TIME_SERIES>()
        .Type(xviz::StreamMetadata::FLOAT)
        .Unit("m/s")

    .Stream("/time_series/demo/2")
      .Category<xviz::StreamMetadata::TIME_SERIES>()
        .Type(xviz::StreamMetadata::BOOL)

    .Stream("/time_series/demo/3")
      .Category<xviz::StreamMetadata::TIME_SERIES>()
        .Type(xviz::StreamMetadata::INT32)

    .Stream("/time_series/demo/4")
      .Category<xviz::StreamMetadata::TIME_SERIES>()
        .Type(xviz::StreamMetadata::STRING)

    .Stream("/ui_primitive/demo/1")
      .Category<xviz::StreamMetadata::UI_PRIMITIVE>()
    .Stream("/ui_primitive/demo/2")
      .Category<xviz::StreamMetadata::UI_PRIMITIVE>()

    .UI("Metrics")
      .NeededStream("/ui/stream1")
      .NeededStream("/ui/stream2")
      .Container("Metrics", xviz::LayoutType::HORIZONTAL)
        .Metric("metric1", "hh", {"/ui/metric1", "/ui/metric2"})
        .Container("Sub 1", xviz::LayoutType::HORIZONTAL)
          .Video({"/ui/camera1", "/ui/camera2"})
          .Metric("metric2", "hh", {"/ui/metric1", "/ui/metric2"})
        .EndContainer()
        .Video({"/ui/camera3", "/ui/camera4"})
      .EndContainer()
    .GetMessage();
  // clang-format on

  std::cout << &data << std::endl;

  google::protobuf::util::JsonPrintOptions option;
  option.add_whitespace = true;
  // option.always_print_primitive_fields = true;
  option.preserve_proto_field_names = true;

  std::string str;
  google::protobuf::util::MessageToJsonString(data, &str, option);
  std::cout << str << std::endl;
  // std::cout << data.DebugString() << std::endl;

  xviz::Builder builder;
  // clang-format off
  auto a = builder
    .Pose("/vehicle_pose")
      .Timestamp(1000)
      .MapOrigin(-122.4, 37.8, 0)
      .Orientation(0, 0, 0)
    .Primitive("/object/tracing_point")
      .Circle({10, 10, 1}, 20)
        .ID("object-1")
        .Style({
          // {"stroke_color", "#ffffff"}
        })
    .Primitive("/object/shape")
      .Polygon({{10, 14, 0}, {7, 10, 0}, {13, 6, 0}})
      .ID("object-1")

    .TimeSeries("/time_series/demo/1")
      .Timestamp(1000)
      .Value(1.3)
      .ID("time-series-1")

    .TimeSeries("/time_series/demo/2")
      .Timestamp(1000)
      .Value(true)
      .ID("time-series-2")

    .TimeSeries("/time_series/demo/3")
      .Timestamp(1000)
      .Value(1)
      .ID("time-series-3")

    .TimeSeries("/time_series/demo/4")
      .Timestamp(1000)
      .Value("hello world")
      .ID("time-series-4")

    .UIPrimitive("/ui_primitive/demo/1")
      .Column("object_id", xviz::TreeTableColumn::INT32)
      .Column("value", xviz::TreeTableColumn::DOUBLE)
      .Column("comment", xviz::TreeTableColumn::STRING)
        .Row(0, {1, 0.1, "this is one"})
        .Row(1, {2, 0.2, "this is two"})

    .GetMessage();
  // clang-format on
  // std::cout << a.DebugString() << std::endl;
  str.clear();
  google::protobuf::util::MessageToJsonString(a, &str, option);
  std::cout << str << std::endl;

  xviz::Message<xviz::StateUpdate> msg(std::move(a));

  auto msg2 = msg;

  std::cout << msg.ToJsonString().size() << std::endl;
  std::cout << msg2.ToProtobufBinary().size() << std::endl;

  google::protobuf::ShutdownProtobufLibrary();
}