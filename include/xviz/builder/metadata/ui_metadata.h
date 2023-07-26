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

#include "metadata_mixin.h"

#include <xviz/v2/declarativeui.pb.h>
#include <xviz/v2/session.pb.h>

#include <google/protobuf/util/json_util.h>

#include <vector>

namespace xviz {

template <typename MetadataBuilderT>
class UIContainerMetadataBuilder
    : public MetadataBuilderMixin<MetadataBuilderT> {
  using SelfT = UIContainerMetadataBuilder<MetadataBuilderT>;
  using BaseT = MetadataBuilderMixin<MetadataBuilderT>;

 public:
  UIContainerMetadataBuilder(MetadataBuilderT& metadata_builder_)
      : BaseT(metadata_builder_) {}

  SelfT& Container(const std::string& name, xviz::LayoutType layout_type,
                   [[maybe_unused]] const std::string& iterations_type = "") {
    assert(data_);
    auto new_data = data_->add_children();
    all_data_.push_back(data_);
    data_ = new_data;
    data_->set_name(name);
    data_->set_type(xviz::ComponentType::CONTAINER);
    data_->set_layout(layout_type);
    return *this;
  }

  SelfT& Table(const std::string& title, const std::string& description,
               const std::string& stream, bool display_object_id) {
    auto new_data =
        SetCommonInformation(xviz::ComponentType::TABLE, title, description);
    new_data->set_stream(stream);
    new_data->set_display_object_id(display_object_id);
    return *this;
  }

  SelfT& Metric(const std::string& title, const std::string& description,
                const std::vector<std::string>& streams) {
    auto new_data =
        SetCommonInformation(xviz::ComponentType::METRIC, title, description);
    for (auto& stream : streams) {
      new_data->add_streams(std::forward<decltype(stream)>(stream));
    }
    return *this;
  }

  SelfT* Plot(const std::string& title, const std::string& description,
              const std::string& independent_variable,
              const std::vector<std::string>& dependent_variables) {
    auto new_data =
        SetCommonInformation(xviz::ComponentType::PLOT, title, description);
    new_data->set_independent_variable(independent_variable);
    for (auto& dependent_variable : dependent_variables) {
      new_data->add_dependent_variables(
          std::forward<decltype(dependent_variable)>(dependent_variable));
    }
    return *this;
  }

  SelfT& TreeTable(const std::string& title, const std::string& description,
                   const std::string& stream, bool display_object_id) {
    auto new_data = SetCommonInformation(xviz::ComponentType::TREETABLE, title,
                                         description);
    new_data->set_stream(stream);
    new_data->set_display_object_id(display_object_id);
    return *this;
  }

  SelfT& Video(const std::vector<std::string>& camera_streams) {
    assert(data_);
    auto new_data = data_->add_children();
    new_data->set_type(xviz::ComponentType::VIDEO);
    for (auto& camera : camera_streams) {
      new_data->add_cameras(std::forward<decltype(camera)>(camera));
    }
    return *this;
  }

  SelfT& EndContainer() {
    if (all_data_.empty()) {
      data_ = nullptr;
    } else {
      data_ = all_data_.back();
      all_data_.pop_back();
    }
    return *this;
  }

 protected:
  template <typename>
  friend class UIMetadataBuilder;

  void Reset() {
    data_ = nullptr;
    all_data_.clear();
  }

  SelfT& Start(UIPanel* data) {
    assert(!data_);
    assert(data);
    data_ = data;
    return *this;
  }

  void End() {
    assert(all_data_.empty());
    if (!data_) {
      return;
    }
    // TODO do some check here
    data_ = nullptr;
    return;
  }

  UIPanel* data_{nullptr};
  std::vector<UIPanel*> all_data_;

 private:
  UIPanel* SetCommonInformation(xviz::ComponentType type,
                                const std::string& title,
                                const std::string& description) {
    assert(data_);
    auto new_data = data_->add_children();
    assert(new_data);
    new_data->set_type(type);
    new_data->set_title(title);
    new_data->set_description(description);
    return new_data;
  }
};

template <typename BaseBuilderT>
class UIMetadataBuilder : public MetadataBuilderMixin<BaseBuilderT> {
  using BaseT = MetadataBuilderMixin<BaseBuilderT>;
  using SelfT = UIMetadataBuilder<BaseBuilderT>;

 public:
  UIMetadataBuilder(BaseBuilderT& parent_builder)
      : BaseT(parent_builder), container_builder_(parent_builder) {
    option_.preserve_proto_field_names = true;
  }

  void Reset() {
    data_ = nullptr;
    internal_data_.Clear();
    container_builder_.Reset();
  }

  template <typename... Args>
  requires(std::constructible_from<std::string, Args...>)
      UIMetadataBuilder& NeededStream(Args&&... args) {
    data_->add_needed_streams(std::forward<Args>(args)...);
    return *this;
  }

  UIContainerMetadataBuilder<BaseBuilderT>& Container(
      const std::string& name, xviz::LayoutType layout_type,
      [[maybe_unused]] const std::string& iterations_type = "") {
    container_builder_.End();
    if (!internal_data_.name().empty() && internal_data_.name() != name) {
      throw std::runtime_error(std::format(
          "TODO Root container name should be the same as UI key name {}",
          internal_data_.name()));
    }
    internal_data_.set_layout(layout_type);
    return container_builder_.Start(&internal_data_);
  }

 private:
  friend class MetadataBuilder;
  UIMetadataBuilder& Start(UIPanelInfo& data, const std::string& name) {
    assert(!data_);
    data_ = &data;
    internal_data_.Clear();
    internal_data_.set_name(name);
    return *this;
  }

  void End() {
    if (!data_) {
      return;
    }
    // TODO do some checks here
    // TODO find a better way to convert a pb message
    //      to pb.Struct
    std::string tmp_output;
    google::protobuf::util::MessageToJsonString(internal_data_, &tmp_output,
                                                option_);
    google::protobuf::util::JsonStringToMessage(tmp_output,
                                                data_->mutable_config());
    google::protobuf::Value panel_value;
    panel_value.set_string_value("PANEL");
    data_->mutable_config()->mutable_fields()->insert(
        {"type", std::move(panel_value)});
    data_ = nullptr;
  }
  UIPanelInfo* data_{nullptr};
  UIPanel internal_data_;

  UIContainerMetadataBuilder<BaseBuilderT> container_builder_;

  google::protobuf::util::JsonPrintOptions option_;
};

}  // namespace xviz