/*
 * File: base_builder.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Saturday, 7th December 2019 2:44:22 pm
 */

#include "builder/base_builder.h"

using namespace xviz;

XVIZBaseBuilder::XVIZBaseBuilder(Category category, const xviz::Metadata& metadata) {
  category_ = category;
  metadata_ = std::move(metadata);
}


// std::shared_ptr<XVIZBaseBuilder> XVIZBaseBuilder::Stream(std::string stream_id) {
//   if (stream_id_.size() != 0) {
//     this->Flush();
//   }
//   stream_id_ = std::move(stream_id);
//   return shared_from_this();
// }