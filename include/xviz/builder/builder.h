/*
 * Project: libxviz
 * Description: C++ Implementation of XVIZ Protocol
 * Author: Minjun Xu (mjxu96@outlook.com)
 * -----
 * MIT License
 * Copyright (c) 2022 Minjun Xu
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

#include "pose.h"
#include "time_series.h"
#include "ui_primitive.h"

#include <xviz/builder/metadata/metadata.h>
#include <xviz/builder/primitive/primitive.h>

namespace xviz {

class Builder {
 public:
  Builder()
      : pose_builder_(*this),
        primitive_builder_(*this),
        time_series_builder_(*this),
        ui_primitive_builder_(*this) {
    Reset();
  }

  void Reset() {
    data_.Clear();
    data_.set_update_type(StateUpdate::SNAPSHOT);
    data_.add_updates();
  }

  Builder& Timestamp(double timestamp) {
    data_.mutable_updates()->at(0).set_timestamp(timestamp);
    return *this;
  }

  template <xviz::concepts::CanConstructString... Args>
  PoseBuilder<Builder>& Pose(Args&&... args) {
    pose_builder_.End();
    std::string stream_id = std::string(std::forward<Args>(args)...);

    auto poses_itr =
        data_.mutable_updates()->at(0).mutable_poses()->find(stream_id);
    if (poses_itr == data_.mutable_updates()->at(0).mutable_poses()->end()) {
      auto insertion_res =
          data_.mutable_updates()->at(0).mutable_poses()->insert(
              {stream_id, xviz::Pose()});
      if (!insertion_res.second) [[unlikely]] {
        throw std::runtime_error("TODO Cannot insert pose");
      }
      poses_itr = insertion_res.first;
    }
    return pose_builder_.Start(poses_itr->second);
  }

  template <xviz::concepts::CanConstructString... Args>
  PrimitiveBuilder<Builder>& Primitive(Args&&... args) {
    primitive_builder_.End();
    std::string stream_id = std::string(std::forward<Args>(args)...);

    auto primitives_itr =
        data_.mutable_updates()->at(0).mutable_primitives()->find(stream_id);
    if (primitives_itr ==
        data_.mutable_updates()->at(0).mutable_primitives()->end()) {
      auto insertion_res =
          data_.mutable_updates()->at(0).mutable_primitives()->insert(
              {stream_id, PrimitiveState()});
      if (!insertion_res.second) [[unlikely]] {
        throw std::runtime_error("TODO Cannot insert primitive");
      }
      primitives_itr = insertion_res.first;
    }
    return primitive_builder_.Start(primitives_itr->second);
  }

  template <xviz::concepts::CanConstructString... Args>
  TimeSeriesBuilder<Builder>& TimeSeries(Args&&... args) {
    time_series_builder_.End();
    std::string stream_id = std::string(std::forward<Args>(args)...);
    auto new_time_series_ptr = data_.mutable_updates()->at(0).add_time_series();
    new_time_series_ptr->add_streams(stream_id);
    return time_series_builder_.Start(*new_time_series_ptr);
  }

  template <xviz::concepts::CanConstructString... Args>
  UIPrimitiveBuilder<Builder>& UIPrimitive(Args&&... args) {
    ui_primitive_builder_.End();
    std::string stream_id = std::string(std::forward<Args>(args)...);
    auto ui_primitives_itr =
        data_.mutable_updates()->at(0).mutable_ui_primitives()->find(stream_id);
    if (ui_primitives_itr ==
        data_.mutable_updates()->at(0).mutable_ui_primitives()->end()) {
      auto insertion_res =
          data_.mutable_updates()->at(0).mutable_ui_primitives()->insert(
              {stream_id, UIPrimitiveState()});
      if (!insertion_res.second) [[unlikely]] {
        throw std::runtime_error("TODO Cannot insert ui primitive");
      }
      ui_primitives_itr = insertion_res.first;
    }
    return ui_primitive_builder_.Start(ui_primitives_itr->second);
  }

  StateUpdate& GetData() {
    pose_builder_.End();
    primitive_builder_.End();
    time_series_builder_.End();
    ui_primitive_builder_.End();

    return data_;
  }

 private:
  StateUpdate data_;
  PoseBuilder<Builder> pose_builder_;
  PrimitiveBuilder<Builder> primitive_builder_;
  TimeSeriesBuilder<Builder> time_series_builder_;
  UIPrimitiveBuilder<Builder> ui_primitive_builder_;
};

}  // namespace xviz