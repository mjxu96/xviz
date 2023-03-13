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

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <chrono>
#include <iostream>

using namespace xviz;

websocketpp::server<websocketpp::config::asio> server;

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

int main() {
  server.init_asio();
  server.set_reuse_addr(true);

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
