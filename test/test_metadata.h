/*
 * File: test_metadata_json.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Monday, 10th February 2020 10:43:37 am
 */

#ifndef XVIZ_TEST_METADATA_H_
#define XVIZ_TEST_METADATA_H_

#include "builder/metadata.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

class XVIZMetadataTest : public ::testing::Test {
 protected:

  xviz::XVIZMetadataBuilder metadata_builder_{};
  std::string expected_str_{};
  nlohmann::json expected_json_{};

  ::testing::AssertionResult IsSameJson(const nlohmann::json& expected_json, const nlohmann::json& given_json) {
    if (expected_json == given_json) {
      return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure() << "\nexpected: \n" <<
      expected_json << "\nbut get:\n" << given_json << "\ndifference: \n" <<
      nlohmann::json::diff(given_json, expected_json) << std::endl;
  }

  ::testing::AssertionResult IsDifferentJson(const nlohmann::json& expected_json, const nlohmann::json& given_json) {
    if (expected_json != given_json) {
      return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure() << "\nexpected different json " <<
      "but get same json: \n" << expected_json << std::endl;
  }

  std::unordered_map<std::string, xviz::XVIZUIBuilder> GetUIBuilders() {
    using namespace xviz;

    std::unordered_map<std::string, XVIZUIBuilder> ui_builders;

    ui_builders["Camera"] = XVIZUIBuilder();
    ui_builders["Metrics"] = XVIZUIBuilder();
    ui_builders["Plot"] = XVIZUIBuilder();
    ui_builders["Table"] = XVIZUIBuilder();

    std::vector<std::string> cameras = {"/camera/images0"};
    std::vector<std::string> streams = {"/object/ts"};
    std::vector<std::string> dep_vars = {"ddd", "aaa"};
    // auto camera_builder = std::make_shared<XVIZVideoBuilder>(cameras);
    XVIZVideoBuilder camera_builder(cameras);
    XVIZPlotBuilder plot_builder("title", "des", "indep_var",
                                 std::move(dep_vars));
    XVIZTableBuilder table_builder("title", "des", "/some_stream/table", true);

    std::shared_ptr<XVIZBaseUIBuilder> metric_builder1 =
        std::make_shared<XVIZMetricBuilder>(streams, "123", "123");
    std::shared_ptr<XVIZBaseUIBuilder> metric_builder2 =
        std::make_shared<XVIZMetricBuilder>(streams, "123", "123");
    std::shared_ptr<XVIZBaseUIBuilder> metric_builder3 =
        std::make_shared<XVIZMetricBuilder>(streams, "123", "123");

    std::shared_ptr<XVIZBaseUIBuilder> container_builder =
        std::make_shared<XVIZContainerBuilder>("metrics", LayoutType::VERTICAL);
    container_builder->Child(metric_builder1);
    container_builder->Child(metric_builder2);
    container_builder->Child(streams, "123", "123");
    ui_builders["Camera"].Child(std::move(camera_builder));
    ui_builders["Metrics"].Child(container_builder);
    ui_builders["Plot"].Child(plot_builder);
    ui_builders["Table"].Child(table_builder);
    return ui_builders;
  }

  void SetUpMetadataBuilder() {
    std::string s = "{\"fill_color\": \"#fff\"}";
    std::string s1 =
        "{\"fill_color\": \"#fff\", \"point_cloud_mode\": "
        "\"distance_to_vehicle\"}";

    metadata_builder_.Stream("/vehicle_pose").Category(xviz::Category::StreamMetadata_Category_POSE)
      .Stream("/object/shape").Category(xviz::Category::StreamMetadata_Category_PRIMITIVE).Type(xviz::Primitive::StreamMetadata_PrimitiveType_POLYGON)
      .Coordinate(xviz::CoordinateType::StreamMetadata_CoordinateType_VEHICLE_RELATIVE)//.Unit("123").Source("123")
      .StreamStyle(s1)
      .Stream("/object/shape2").Category(xviz::Category::StreamMetadata_Category_PRIMITIVE).Type(xviz::Primitive::StreamMetadata_PrimitiveType_POLYGON)
      .Stream("/object/circles").Category(xviz::Category::StreamMetadata_Category_PRIMITIVE).Type(xviz::Primitive::StreamMetadata_PrimitiveType_CIRCLE)
      .Stream("/camera/images0").Category(xviz::Category::StreamMetadata_Category_PRIMITIVE).Type(xviz::Primitive::StreamMetadata_PrimitiveType_IMAGE)
      .Stream("/object/text").Category(xviz::Category::StreamMetadata_Category_PRIMITIVE).Type(xviz::Primitive::StreamMetadata_PrimitiveType_TEXT)
      .Stream("/object/stadium").Category(xviz::Category::StreamMetadata_Category_PRIMITIVE).Type(xviz::Primitive::StreamMetadata_PrimitiveType_STADIUM)
      .Stream("/object/ts").Category(xviz::Category::StreamMetadata_Category_TIME_SERIES)
      .Stream("/object/uptest").Category(xviz::Category::StreamMetadata_Category_UI_PRIMITIVE)
      .UI(std::move(GetUIBuilders()));
    metadata_builder_.StartTime(1000).EndTime(1010);
  }

  void SetUpExpectedValue() {
    expected_str_ =       
      "{\"log_info\":{\"end_time\":1010,\"start_time\":1000},\"streams\":{\"/"
      "camera/"
      "images0\":{\"category\":\"PRIMITIVE\",\"primitive_type\":\"IMAGE\"},\"/"
      "object/"
      "circles\":{\"category\":\"PRIMITIVE\",\"primitive_type\":\"CIRCLE\"},\"/"
      "object/"
      "shape\":{\"category\":\"PRIMITIVE\",\"coordinate\":\"VEHICLE_RELATIVE\","
      "\"primitive_type\":\"POLYGON\",\"stream_style\":{\"fill_color\":\"#"
      "fff\",\"point_cloud_mode\":\"distance_to_vehicle\"}},\"/object/"
      "shape2\":{\"category\":\"PRIMITIVE\",\"primitive_type\":\"POLYGON\"},\"/"
      "object/"
      "stadium\":{\"category\":\"PRIMITIVE\",\"primitive_type\":\"STADIUM\"},"
      "\"/object/"
      "text\":{\"category\":\"PRIMITIVE\",\"primitive_type\":\"TEXT\"},\"/"
      "object/ts\":{\"category\":\"TIME_SERIES\"},\"/object/"
      "uptest\":{\"category\":\"UI_PRIMITIVE\"},\"/"
      "vehicle_pose\":{\"category\":\"POSE\"}},\"ui_config\":{\"Camera\":{"
      "\"children\":[{\"cameras\":[\"/camera/"
      "images0\"],\"type\":\"VIDEO\"}],\"name\":\"Camera\",\"type\":\"panel\"},"
      "\"Metrics\":{\"children\":[{\"children\":[{\"description\":\"123\","
      "\"streams\":[\"/object/"
      "ts\"],\"title\":\"123\",\"type\":\"METRIC\"},{\"description\":\"123\","
      "\"streams\":[\"/object/"
      "ts\"],\"title\":\"123\",\"type\":\"METRIC\"},{\"description\":\"123\","
      "\"streams\":[\"/object/"
      "ts\"],\"title\":\"123\",\"type\":\"METRIC\"}],\"name\":\"metrics\","
      "\"type\":\"CONTAINER\"}],\"name\":\"Metrics\",\"type\":\"panel\"},"
      "\"Plot\":{\"children\":[{\"dependent_variables\":[\"ddd\",\"aaa\"],"
      "\"description\":\"des\",\"independent_variable\":\"indep_var\","
      "\"title\":\"title\",\"type\":\"PLOT\"}],\"name\":\"Plot\",\"type\":"
      "\"panel\"},\"Table\":{\"children\":[{\"description\":\"des\",\"display_"
      "object_id\":true,\"stream\":\"/some_stream/"
      "table\",\"title\":\"title\"}],\"name\":\"Table\",\"type\":\"panel\"}},"
      "\"version\":\"2.0.0\"}";
    expected_json_ = nlohmann::json::parse(expected_str_);
  }

  void SetUp() override {
    SetUpMetadataBuilder();
    SetUpExpectedValue();
  }
};

TEST_F(XVIZMetadataTest, SameJsonTest) {
  auto output = metadata_builder_.GetMessage().ToObject();
  ASSERT_TRUE(IsSameJson(expected_json_, output));
}

TEST_F(XVIZMetadataTest, NotSameJsonTest) {
  auto output = metadata_builder_.GetMessage().ToObject();
  auto tmp_json = expected_json_;
  tmp_json["dummy"] = "dummy";
  ASSERT_TRUE(IsDifferentJson(tmp_json, output));
}

#endif