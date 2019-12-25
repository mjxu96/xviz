/*
 * File: metadata.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Tuesday, 24th December 2019 8:15:24 pm
 */


#include "builder/metadata.h"

using namespace xviz;


XVIZMetadataBuilder::XVIZMetadataBuilder() {
  data_ = std::make_shared<Metadata>();
  // TODO UI BUILDER
  data_->set_version("2.0.0");
  Reset();
}

std::shared_ptr<Metadata> XVIZMetadataBuilder::GetData() {
  Flush();
  // Metadata metadata;
  // metadata.CopyFrom(*data_);

  // TODO UI

  // return metadata;
  return data_;
}

XVIZMessage XVIZMetadataBuilder::GetMessage() {
  return XVIZMessage(GetData());
}

XVIZMetadataBuilder& XVIZMetadataBuilder::Stream(const std::string& stream_id) {
  if (!stream_id_.empty()) {
    Flush();
  }
  stream_id_ = stream_id;
  return *this;
}

XVIZMetadataBuilder& XVIZMetadataBuilder::Category(xviz::Category category) {
  temp_stream_.set_category(category);
  return *this;
}

// TODO determine this type
XVIZMetadataBuilder& XVIZMetadataBuilder::Coordinate(CoordinateType coordinate_type) {
  temp_stream_.set_coordinate(coordinate_type);
  return *this;
}

XVIZMetadataBuilder& XVIZMetadataBuilder::Type(Primitive primitive_type) {
  type_ = (uint32_t)primitive_type;
  return *this;
}

XVIZMetadataBuilder& XVIZMetadataBuilder::Type(ScalarType scalar_type) {
  type_ = (uint32_t)scalar_type;
  return *this;
}

XVIZMetadataBuilder& XVIZMetadataBuilder::StreamStyle(const std::string& style_str) {
  auto stream_style = temp_stream_.mutable_stream_style();
  stream_style->MergeFrom(*JsonStringToStyleStream(style_str));
  return *this;
}

void XVIZMetadataBuilder::Reset() {
  stream_id_ = "";
  temp_stream_ = StreamMetadata();
  // TODO TMP TYPE
}

void XVIZMetadataBuilder::Flush() {
  if (stream_id_.empty()) {
    Reset();
    return;
  }  

  auto category_int = (uint32_t)(temp_stream_.category());
  if (category_int == 1u || category_int == 5u) {
    temp_stream_.set_primitive_type((Primitive)(type_));
  } else if (category_int == 2u || category_int == 3u) {
    temp_stream_.set_scalar_type((ScalarType)type_);
  }

  auto streams = data_->mutable_streams();
  (*streams)[stream_id_].MergeFrom(temp_stream_);

  Reset();
}