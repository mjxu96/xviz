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

#pragma once

#include "stream_metadata.h"
#include "ui_metadata.h"

#include <xviz/def.h>

#include <concepts>

namespace xviz {

using namespace v2;

class MetadataBuilder {
 public:
  MetadataBuilder() : stream_builder_(*this), ui_builder_(*this) {
    data_.set_version("2.0.0");
  }

  template <concepts::CanConstructString... Args>
  MetadataBuilder& Version(Args&&... args) {
    data_.set_version(std::forward<Args>(args)...);
    return *this;
  }

  template <typename... Args>
  requires(std::constructible_from<std::string, Args...>)
      StreamMetadataBuilder<MetadataBuilder>
  &Stream(Args&&... args) {
    stream_builder_.EndStream();
    EndAllBuilders();
    std::string stream_id = std::string(std::forward<Args>(args)...);
    auto stream_metadata_itr = data_.mutable_streams()->find(stream_id);
    if (stream_metadata_itr == data_.mutable_streams()->end()) {
      auto insertion_res =
          data_.mutable_streams()->insert({stream_id, StreamMetadata()});
      if (!insertion_res.second) [[unlikely]] {
        throw std::runtime_error("TODO Cannot insert stream meta");
      }
      stream_metadata_itr = insertion_res.first;
    }
    return stream_builder_.StartStream(stream_metadata_itr->second);
  }

  template <typename... Args>
  requires(std::constructible_from<std::string, Args...>)
      UIMetadataBuilder<MetadataBuilder>
  &UI(Args&&... args) {
    EndAllBuilders();
    std::string panel_key = std::string(std::forward<Args>(args)...);
    auto ui_config_itr = data_.mutable_ui_config()->find(panel_key);
    if (ui_config_itr == data_.mutable_ui_config()->end()) {
      auto insertion_res =
          data_.mutable_ui_config()->insert({panel_key, UIPanelInfo()});
      if (!insertion_res.second) [[unlikely]] {
        throw std::runtime_error("TODO Cannot insert ui config meta");
      }
      ui_config_itr = insertion_res.first;
    }
    ui_config_itr->second.set_name(panel_key);
    return ui_builder_.Start(ui_config_itr->second, panel_key);
  }

  Metadata& GetMessage() {
    EndAllBuilders();
    return data_;
  }

 private:
  Metadata data_;
  StreamMetadataBuilder<MetadataBuilder> stream_builder_;
  UIMetadataBuilder<MetadataBuilder> ui_builder_;

  void EndAllBuilders() {
    stream_builder_.EndStream();
    ui_builder_.End();
  }
};

}  // namespace xviz