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

#include <xviz/xviz.h>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/util/json_util.h>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <lodepng.h>

#include <chrono>
#include <iostream>

using namespace xviz;

websocketpp::server<websocketpp::config::asio> server;
// minimal example of a PNG file
std::vector<unsigned char> image = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x01, 0x03, 0x00, 0x00, 0x00, 0x66, 0xBC, 0x3A, 0x25, 0x00, 0x00, 0x00,
    0x03, 0x50, 0x4C, 0x54, 0x45, 0xB5, 0xD0, 0xD0, 0x63, 0x04, 0x16, 0xEA,
    0x00, 0x00, 0x00, 0x1F, 0x49, 0x44, 0x41, 0x54, 0x68, 0x81, 0xED, 0xC1,
    0x01, 0x0D, 0x00, 0x00, 0x00, 0xC2, 0xA0, 0xF7, 0x4F, 0x6D, 0x0E, 0x37,
    0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0x0D, 0x21,
    0x00, 0x00, 0x01, 0x9A, 0x60, 0xE1, 0xD5, 0x00, 0x00, 0x00, 0x00, 0x49,
    0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};

xviz::Message<xviz::Metadata> GetMetadata() {
  xviz::MetadataBuilder meta_builder;

  // clang-format off
  auto metadata = meta_builder
    .Stream("/vehicle_pose")
      .Category<xviz::StreamMetadata::POSE>()
    .Stream("/object/tracking_point")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::CIRCLE)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(
          {
            {"extruded", true},
            {"height", 2.0f}
          })
    .Stream("/object/shape")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::POLYGON)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(
          {
            {"extruded", true},
            {"fill_color", "#ff66cc"},
            {"height", 3.0f}
          })
    .Stream("/object/lines")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::POLYLINE)
        .Coordinate(xviz::StreamMetadata::IDENTITY)

    .Stream("/sensor/camera/1")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::IMAGE)
    .Stream("/sensor/camera/2")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::IMAGE)

    .Stream("/other/text/1")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::TEXT)

    .Stream("/game/time")
      .Category<xviz::StreamMetadata::UI_PRIMITIVE>()

    .Stream("/sensor/gnss/1")
      .Category<xviz::StreamMetadata::UI_PRIMITIVE>()

    .Stream("/metric/steer")
      .Category<xviz::StreamMetadata::TIME_SERIES>()
        .Type(xviz::StreamMetadata::FLOAT)
        .Unit("rad/s")

    .Stream("/vehicle/acceleration")
      .Category(xviz::StreamMetadata::TIME_SERIES)
        .Unit("m/s^2")
        .Type(xviz::StreamMetadata::FLOAT)

    .UI("Camera")
      .Container("Camera", xviz::LayoutType::HORIZONTAL)
        .Video({"/sensor/camera/1", "/sensor/camera/2"})
      .EndContainer()
    
    .UI("Tables")
      .Container("Tables", xviz::LayoutType::HORIZONTAL)
        .TreeeTable("game time", "game time", "/game/time", false)
        .TreeeTable("gnss", "gnss", "/sensor/gnss/1", false)

    .UI("Metrics")
      .Container("Metrics", xviz::LayoutType::HORIZONTAL)
        .Metric("steer", "steer", {"/metric/steer"})
    .GetMessage();
  // clang-format on

  return xviz::Message<xviz::Metadata>(std::move(metadata));
}

xviz::Message<xviz::StateUpdate> GetUpdate(float x) {
  auto now = std::chrono::duration<double>(
                 std::chrono::high_resolution_clock::now().time_since_epoch())
                 .count();

  xviz::Builder builder;
  // clang-format off
  auto a = builder
    .Timestamp(now)
    .Pose("/vehicle_pose")
      .MapOrigin(0, 0, 0)
      .Orientation(0, 0, 0)
      .Position(x, 0, 0)
    .Primitive("/object/tracing_point")
      .Circle({10, 10, 1}, 20)
        .ID("object-1")
        .Style({
          {"stroke_color", "#ffffff"}
        })
    .Primitive("/object/shape")
      .Polygon({{x, 14, 0}, {7, 10, 0}, {13, 6, 0}})
      .ID("object-1")
    .Primitive("/object/lines")
      .Polyline({{x, 0, 0}, {13, 6, 0}, {13, 6, 10}})
      .Polyline({{x, 30, 0}, {13, 6, 0}, {13, 6, 10}})
    .Primitive("/sensor/camera/1")
      .Image(image.data(), image.size())
    .Primitive("/sensor/camera/2")
      .Image(image.data(), image.size())
    .Primitive("/other/text/1")
      .Text("Hello World!")
      .Position({0, 0, 10})

    .UIPrimitive("/game/time")
      .Column("game time", xviz::TreeTableColumn::DOUBLE)
      .Column("real time", xviz::TreeTableColumn::STRING)
        .Row(0, {now, "test"})

    .TimeSeries("/metric/steer")
      .Timestamp(now)
      .Value(3.0)
    .TimeSeries("/vehicle/acceleration")
      .Timestamp(now)
      .Value(3.0)
    .GetMessage();
  // clang-format on

  return xviz::Message<xviz::StateUpdate>(std::move(a));
}

void UpdatePeriodcally(
    std::shared_ptr<websocketpp::connection<websocketpp::config::asio>> conn) {
  auto metadata = GetMetadata();
  auto metadata_string = metadata.ToProtobufBinary();
  // auto metadata_string = metadata.ToJsonString();
  conn->send(metadata_string.data(), metadata_string.size(),
             websocketpp::frame::opcode::binary);
  std::error_code err;
  float x = 10;
  while (!err) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    x += 10;
    auto update_string = GetUpdate(x).ToProtobufBinary();
    err = conn->send(update_string.data(), update_string.size(),
                     websocketpp::frame::opcode::binary);
  }
  std::cout << "disconnected " << err << std::endl;
}

void LoadPNGImage(char* file_name) {
  lodepng::load_file(image, file_name);
  std::cout << "loaded file size: " << image.size() << std::endl;
}

int main(int argc, char* argv[]) {
  server.init_asio();
  server.set_reuse_addr(true);

  if (argc > 1) {
    std::cout << "loading png file at " << argv[1] << std::endl;
    LoadPNGImage(argv[1]);
  } else {
    std::cout << "no png file provided, will use a default one" << std::endl;
  }

  std::vector<std::thread> threads;

  server.set_open_handler([&](websocketpp::connection_hdl hdl) {
    std::cout << "open!" << std::endl;
    auto conn = server.get_con_from_hdl(hdl);
    threads.emplace_back(std::bind(UpdatePeriodcally, conn));
  });

  server.listen(8081);
  server.start_accept();

  server.run();

  for (auto& th : threads) {
    th.join();
  }

  google::protobuf::ShutdownProtobufLibrary();
}
