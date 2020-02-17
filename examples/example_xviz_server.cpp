/*
 * File: example_xviz_server.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Monday, 17th February 2020 3:16:09 pm
 */

#include "server/xviz_server.h"
#include "server/xviz_handler.h"
#include "server/xviz_session.h"

#include <iostream>
#include <fstream>
#include "proto/primitives.pb.h"
#include "builder/pose.h"
#include "builder/xviz_builder.h"
#include "builder/metadata.h"

#include "builder/declarative_ui/video_builder.h"
#include "builder/declarative_ui/metric_builder.h"
#include "builder/declarative_ui/container_builder.h"
#include "io/glb_writer.h"

using namespace xviz;

std::unordered_map<std::string, XVIZUIBuilder> GetUIBuilders() {

  std::unordered_map<std::string, XVIZUIBuilder> ui_builders;

  ui_builders["Camera"] = XVIZUIBuilder();
  ui_builders["Metrics"] = XVIZUIBuilder();
  // ui_builders["Plot"] = XVIZUIBuilder();
  // ui_builders["Table"] = XVIZUIBuilder();

  std::vector<std::string> cameras = {"/camera/images0"};
  std::vector<std::string> streams = {"/vehicle/acceleration"};
  std::vector<std::string> dep_vars = {"ddd", "aaa"};
  // auto camera_builder = std::make_shared<XVIZVideoBuilder>(cameras);
  XVIZVideoBuilder camera_builder(cameras);
  
  std::shared_ptr<XVIZBaseUIBuilder> metric_builder1 = std::make_shared<XVIZMetricBuilder>(streams, "acceleration", "acceleration");
  // std::shared_ptr<XVIZBaseUIBuilder> metric_builder2 = std::make_shared<XVIZMetricBuilder>(streams, "acceleration", "acceleration");

  std::shared_ptr<XVIZBaseUIBuilder> container_builder = std::make_shared<XVIZContainerBuilder>("metrics", LayoutType::VERTICAL);
  container_builder->Child(metric_builder1);
  // container_builder->Child(metric_builder2);
  // container_builder->Child(streams, "acceleration", "acceleration");
  ui_builders["Camera"].Child(std::move(camera_builder));
  ui_builders["Metrics"].Child(container_builder);
  return ui_builders;
}

class Scenario {
public:
  Scenario(const std::string& png_file_name) {
    for (int i = 0; i < 10; i++) {
      points_.push_back(0);
      points_.push_back(0);
      points_.push_back(i);
    }
    if (png_file_name.empty()) {
      LOG_INFO("No png is shown");
      return;
    }
    std::ifstream in(png_file_name, std::ios::binary);
    if (!in.is_open()) {
      LOG_INFO("No png is shown");
      return;
    }
    while (in) {
      char c;
      in.get(c);
      if (in) {
        image.push_back(c);
      }
    }
    LOG_INFO("Image length: %ld", image.size());
  }

  std::shared_ptr<XVIZMetadataBuilder> GetMetaBuilder() {
    std::string s = "{\"fill_color\": \"#fff\"}"; 
    std::string s1 = "{\"fill_color\": \"#fff\", \"point_cloud_mode\": \"distance_to_vehicle\"}"; 
    auto metadata_builder = std::make_shared<XVIZMetadataBuilder>();
    metadata_builder->Stream("/vehicle_pose").Category(Category::StreamMetadata_Category_POSE)
      .Stream("/object/shape").Category(Category::StreamMetadata_Category_PRIMITIVE).Type(Primitive::StreamMetadata_PrimitiveType_POINT)
      .StreamStyle(s1)
      .Stream("/object/circles").Category(Category::StreamMetadata_Category_PRIMITIVE).Type(Primitive::StreamMetadata_PrimitiveType_CIRCLE)
      .StreamStyle(s)
      .Stream("/camera/images0").Category(Category::StreamMetadata_Category_PRIMITIVE).Type(Primitive::StreamMetadata_PrimitiveType_IMAGE)
      .Stream("/vehicle/acceleration")
        .Category(Category::StreamMetadata_Category_TIME_SERIES)
        .Unit("m/s^2")
        .Type(ScalarType::StreamMetadata_ScalarType_FLOAT)
      .UI(std::move(GetUIBuilders()));
    metadata_ptr_ = metadata_builder;
    return metadata_builder;
  }

  std::string GetUpdate() {
    points_.push_back(0);
    points_.push_back(0);
    points_.push_back(cnt);
    XVIZBuilder builder(metadata_ptr_->GetData());
    builder.Pose("/vehicle_pose")
      .Timestamp(cnt)
      .MapOrigin(0.00, 0.00, 0.000)
      .Orientation(0, 0, 0);
    builder.Primitive("/object/circles")
      .Circle({1, 2, 3}, 20.0);
    builder.Primitive("/object/shape")
      .Points(points_);
    if (!image.empty()) {
      builder.Primitive("/camera/images0")
        .Image(image);
    }
    builder.TimeSeries("/vehicle/acceleration")
      .Timestamp(cnt)
      .Value(cnt % 10)
      .Id("acceleration");
    cnt++;
    XVIZGLBWriter writer;
    std::string str;
    writer.WriteMessage(str, builder.GetMessage());
    return str;
    // return builder.GetMessage().ToObjectString();
  }
private:
  std::shared_ptr<xviz::XVIZMetadataBuilder> metadata_ptr_{};
  int cnt = 11;
  std::vector<double> points_{};
  std::string image;
};


class LiveSession : public XVIZBaseSession {
public:
  LiveSession(std::shared_ptr<websocketpp::connection<websocketpp::config::asio>> conn_ptr,
    const Scenario& sce) : XVIZBaseSession(conn_ptr), sce_(sce) {}
  void OnConnect() override {
    conn_ptr_->send(sce_.GetMetaBuilder()->GetMessage().ToObjectString());
  }
  void Main() override {
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      auto err_code = conn_ptr_->send(sce_.GetUpdate(), websocketpp::frame::opcode::BINARY);
      if (err_code) {
        throw websocketpp::exception("loss connection");
      }
    }
  }
  void OnDisconnect() override {
    // conn_ptr_->send("byebye");
  }

private:
  Scenario sce_{""};
};

class LiveHandler : public XVIZBaseHandler {
public:
  LiveHandler(const Scenario& sce) : sce_(sce), XVIZBaseHandler() {}
  std::shared_ptr<XVIZBaseSession> GetSession(const std::unordered_map<std::string, std::string>& params,
    std::shared_ptr<websocketpp::connection<websocketpp::config::asio>> conn_ptr) override {
    return std::make_shared<LiveSession>(conn_ptr, sce_);
  }
private:
  Scenario sce_;
};

int main(int argc, char** argv) {
  std::string png_file_name;
  if (argc >= 2) {
    png_file_name = argv[1];
  }
  std::vector<std::shared_ptr<xviz::XVIZBaseHandler>> handlers;
  handlers.push_back(std::make_shared<LiveHandler>(Scenario(png_file_name)));
  xviz::XVIZServer server(handlers);
  server.Serve();
}