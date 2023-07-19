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
#include "utils/checker.h"
#include "utils/cleanup.h"
#include "utils/converters.h"

#include <gtest/gtest.h>

#include <optional>

namespace xviz::tests {
class MetadataBuilderTest : public ::testing::Test {
 public:
  using MetadataBuilderChecker =
      TargetChecker<StreamMetadata::Category, StreamMetadata::ScalarType,
                    StreamMetadata::PrimitiveType,
                    StreamMetadata::CoordinateType, std::string,
                    StyleStreamValue,
                    std::unordered_map<std::string, StyleObjectValue>>;
  void SetUp() override {}

  void TearDown() override {}

  // TODO finish this one when needed
  template <typename T>
  void PrintCheckInformation(std::ostream& os, std::string_view explanation,
                             const std::optional<T>& element) {
    os << explanation << ": ";
    if (element.has_value()) {
    }
  }

  void Check(const xviz::v2::Metadata& msg, const std::string& stream_id,
             const MetadataBuilderChecker& checker) {
    EXPECT_NE(msg.streams().find(stream_id), msg.streams().end());
    const auto& stream = msg.streams().find(stream_id)->second;
    checker.Check(CONVERT_PROTOBUF_ENUM_TO_OPTIONAL(stream, category),
                  CONVERT_PROTOBUF_ENUM_TO_OPTIONAL(stream, scalar_type),
                  CONVERT_PROTOBUF_ENUM_TO_OPTIONAL(stream, primitive_type),
                  CONVERT_PROTOBUF_ENUM_TO_OPTIONAL(stream, coordinate),
                  stream.units(),
                  CONVERT_PROTOBUF_FIELD_TO_OPTIONAL(stream, stream_style),
                  ConvertStyleClassToMap(stream.style_classes()));
  }

  xviz::MetadataBuilder builder_;

  xviz::StyleType to_check_style_stream = {
      {"extruded", true},
      {"fill_color", "#ff66cc"},
      {"height", 3.0f},
      {"radius_min_pixels", 12u},
      {"font_family", "some_font_family"},
      {"point_color_mode", xviz::v2::PointColorMode::ELEVATION},
      {"point_color_domain", {3.0f, 3.1f, 3.2f}},
      {"text_anchor", xviz::v2::TextAnchor::MIDDLE},
      {"text_baseline", xviz::v2::TextAlignmentBaseline::CENTER}};

  std::unordered_map<std::string, xviz::StyleType> to_check_style_class = {
      {"class1",
       {{"fill_color", "#ff66cc"},
        {"height", 3.0f},
        {"text_anchor", xviz::v2::TextAnchor::MIDDLE},
        {"text_baseline", xviz::v2::TextAlignmentBaseline::CENTER}}},

      {"class2",
       {{"fill_color", "#cc66ff"},
        {"height", 0.3f},
        {"text_anchor", xviz::v2::TextAnchor::END},
        {"text_baseline", xviz::v2::TextAlignmentBaseline::BOTTOM}}}};
};

TEST_F(MetadataBuilderTest, StreamExistTest) {
  // clang-format off
  auto msg = builder_
    .Stream("/test/stream1")
    .GetMessage();
  // clang-format on

  EXPECT_EQ(msg.streams_size(), 1);
  EXPECT_NE(msg.streams().find("/test/stream1"), msg.streams().end());
}

TEST_F(MetadataBuilderTest, StreamNotExistAfterResetTest) {
  // clang-format off
  auto msg = builder_
    .Stream("/test/stream1")
    .GetMessage();
  // clang-format on

  EXPECT_EQ(msg.streams_size(), 1);
  EXPECT_NE(msg.streams().find("/test/stream1"), msg.streams().end());

  builder_.Reset();
  msg = builder_.GetMessage();
  EXPECT_EQ(msg.streams_size(), 0);
}

TEST_F(MetadataBuilderTest, PoseCategoryTest) {
  auto msg = builder_.Stream("/test/pose")
                 .Category<xviz::StreamMetadata::POSE>()
                 .GetMessage();
  EXPECT_EQ(msg.streams_size(), 1);

  Check(msg, "/test/pose",
        MetadataBuilderChecker(xviz::StreamMetadata::POSE, std::nullopt,
                               std::nullopt, std::nullopt, std::string(),
                               std::nullopt, std::nullopt));
}

TEST_F(MetadataBuilderTest, PrimitiveCategorySingleStreamTest) {
  // clang-format off
  auto msg = builder_
    .Stream("/test/stream1")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::CIRCLE)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(to_check_style_stream)
        .StyleClass("class1", to_check_style_class["class1"])
        .StyleClass("class2", to_check_style_class["class2"])
    .GetMessage();
  // clang-format on

  EXPECT_EQ(msg.streams_size(), 1);

  Check(msg, "/test/stream1",
        MetadataBuilderChecker(
            xviz::StreamMetadata::PRIMITIVE, std::nullopt,
            xviz::StreamMetadata::CIRCLE, xviz::StreamMetadata::IDENTITY,
            std::string(),
            detail::ConvertInternalTypeToProtobufType<StyleStreamValue>(
                to_check_style_stream),
            ConvertMapToStyleClass(to_check_style_class)));
}

TEST_F(MetadataBuilderTest, PrimitiveCategoryMultipleStreamsTest) {
  // clang-format off
  auto msg = builder_
    .Stream("/test1/stream1")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::CIRCLE)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(to_check_style_stream)
        // .Unit("m/s")  <-- This will throw exception
        .StyleClass("class1", to_check_style_class["class1"])
        .StyleClass("class2", to_check_style_class["class2"])
    .Stream("/test2/stream2")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::POLYGON)
        .Coordinate(xviz::StreamMetadata::DYNAMIC)
        // .Unit("m/s")  <-- This will not be compiled
        // no stream style and style classes for this stream
    .Stream("/test3/stream3")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::POINT)
        .Coordinate(xviz::StreamMetadata::VEHICLE_RELATIVE)
        // no stream style and style classes for this stream
    .GetMessage();
  // clang-format on

  EXPECT_EQ(msg.streams_size(), 3);

  Check(msg, "/test1/stream1",
        MetadataBuilderChecker(
            xviz::StreamMetadata::PRIMITIVE, std::nullopt,
            xviz::StreamMetadata::CIRCLE, xviz::StreamMetadata::IDENTITY,
            std::string(),
            detail::ConvertInternalTypeToProtobufType<StyleStreamValue>(
                to_check_style_stream),
            ConvertMapToStyleClass(to_check_style_class)));

  Check(msg, "/test2/stream2",
        MetadataBuilderChecker(xviz::StreamMetadata::PRIMITIVE, std::nullopt,
                               xviz::StreamMetadata::POLYGON,
                               xviz::StreamMetadata::DYNAMIC, std::string(),
                               std::nullopt, std::nullopt));

  Check(msg, "/test3/stream3",
        MetadataBuilderChecker(xviz::StreamMetadata::PRIMITIVE, std::nullopt,
                               xviz::StreamMetadata::POINT,
                               xviz::StreamMetadata::VEHICLE_RELATIVE,
                               std::string(), std::nullopt, std::nullopt));
}

TEST_F(MetadataBuilderTest, PrimitiveCategoryMultipleStreamsResetTest) {
  // clang-format off
  auto msg = builder_
    .Stream("/test1/stream1")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::CIRCLE)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(to_check_style_stream)
        // .Unit("m/s")  <-- This will throw exception
        .StyleClass("class1", to_check_style_class["class1"])
        .StyleClass("class2", to_check_style_class["class2"])
    .Stream("/test2/stream2")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::POLYGON)
        .Coordinate(xviz::StreamMetadata::DYNAMIC)
        // .Unit("m/s")  <-- This will not be compiled
        // no stream style and style classes for this stream
    .Stream("/test3/stream3")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::POINT)
        .Coordinate(xviz::StreamMetadata::VEHICLE_RELATIVE)
        // no stream style and style classes for this stream
    .GetMessage();
  // clang-format on

  EXPECT_EQ(msg.streams_size(), 3);

  Check(msg, "/test1/stream1",
        MetadataBuilderChecker(
            xviz::StreamMetadata::PRIMITIVE, std::nullopt,
            xviz::StreamMetadata::CIRCLE, xviz::StreamMetadata::IDENTITY,
            std::string(),
            detail::ConvertInternalTypeToProtobufType<StyleStreamValue>(
                to_check_style_stream),
            ConvertMapToStyleClass(to_check_style_class)));

  Check(msg, "/test2/stream2",
        MetadataBuilderChecker(xviz::StreamMetadata::PRIMITIVE, std::nullopt,
                               xviz::StreamMetadata::POLYGON,
                               xviz::StreamMetadata::DYNAMIC, std::string(),
                               std::nullopt, std::nullopt));

  Check(msg, "/test3/stream3",
        MetadataBuilderChecker(xviz::StreamMetadata::PRIMITIVE, std::nullopt,
                               xviz::StreamMetadata::POINT,
                               xviz::StreamMetadata::VEHICLE_RELATIVE,
                               std::string(), std::nullopt, std::nullopt));

  builder_.Reset();

  // clang-format off
  auto msg2 = builder_
    .Stream("/test1/stream1")
      .Category(xviz::StreamMetadata::PRIMITIVE)
        .Type(xviz::StreamMetadata::CIRCLE)
        .Coordinate(xviz::StreamMetadata::IDENTITY)
        .StreamStyle(to_check_style_stream)
        // .Unit("m/s")  <-- This will throw exception
        .StyleClass("class1", to_check_style_class["class1"])
        .StyleClass("class2", to_check_style_class["class2"])
    .Stream("/test2/stream2")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::POLYGON)
        .Coordinate(xviz::StreamMetadata::DYNAMIC)
        // .Unit("m/s")  <-- This will not be compiled
        // no stream style and style classes for this stream
    .Stream("/test3/stream3")
      .Category<xviz::StreamMetadata::PRIMITIVE>()
        .Type(xviz::StreamMetadata::POINT)
        .Coordinate(xviz::StreamMetadata::VEHICLE_RELATIVE)
        // no stream style and style classes for this stream
    .GetMessage();
  // clang-format on

  EXPECT_EQ(msg2.streams_size(), 3);

  Check(msg2, "/test1/stream1",
        MetadataBuilderChecker(
            xviz::StreamMetadata::PRIMITIVE, std::nullopt,
            xviz::StreamMetadata::CIRCLE, xviz::StreamMetadata::IDENTITY,
            std::string(),
            detail::ConvertInternalTypeToProtobufType<StyleStreamValue>(
                to_check_style_stream),
            ConvertMapToStyleClass(to_check_style_class)));

  Check(msg2, "/test2/stream2",
        MetadataBuilderChecker(xviz::StreamMetadata::PRIMITIVE, std::nullopt,
                               xviz::StreamMetadata::POLYGON,
                               xviz::StreamMetadata::DYNAMIC, std::string(),
                               std::nullopt, std::nullopt));

  Check(msg2, "/test3/stream3",
        MetadataBuilderChecker(xviz::StreamMetadata::PRIMITIVE, std::nullopt,
                               xviz::StreamMetadata::POINT,
                               xviz::StreamMetadata::VEHICLE_RELATIVE,
                               std::string(), std::nullopt, std::nullopt));
}

TEST_F(MetadataBuilderTest, TimeseriesCategorySingleStreamTest) {
  // clang-format off
  auto msg = builder_
    .Stream("/test/stream1")
      .Category<xviz::StreamMetadata::TIME_SERIES>()
        .Type(xviz::StreamMetadata::FLOAT)
        .Unit("m/s")
    .GetMessage();
  // clang-format on

  EXPECT_EQ(msg.streams_size(), 1);

  Check(msg, "/test/stream1",
        MetadataBuilderChecker(xviz::StreamMetadata::TIME_SERIES,
                               xviz::StreamMetadata::FLOAT, std::nullopt,
                               std::nullopt, std::string("m/s"), std::nullopt,
                               std::nullopt));
}

TEST_F(MetadataBuilderTest, TimeseriesCategoryMultipleStreamsTest) {
  // clang-format off
  auto msg = builder_
    .Stream("/test/stream1")
      .Category<xviz::StreamMetadata::TIME_SERIES>()
        .Type(xviz::StreamMetadata::FLOAT)
        .Unit("m/s")
    .Stream("/test/stream2")
      .Category<xviz::StreamMetadata::TIME_SERIES>()
        .Type(xviz::StreamMetadata::STRING)
        // no unit for this stream
    .GetMessage();
  // clang-format on

  EXPECT_EQ(msg.streams_size(), 2);

  Check(msg, "/test/stream1",
        MetadataBuilderChecker(xviz::StreamMetadata::TIME_SERIES,
                               xviz::StreamMetadata::FLOAT, std::nullopt,
                               std::nullopt, std::string("m/s"), std::nullopt,
                               std::nullopt));

  Check(msg, "/test/stream2",
        MetadataBuilderChecker(xviz::StreamMetadata::TIME_SERIES,
                               xviz::StreamMetadata::STRING, std::nullopt,
                               std::nullopt, std::string(""), std::nullopt,
                               std::nullopt));
}

// TODO add more tests for UI metadata
// TODO add Reset tests for UI metadata

}  // namespace xviz::tests
