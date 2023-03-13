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

#include <xviz/def.h>

#include "builder_mixin.h"

#include <array>

namespace xviz {

template <typename BaseBuilder>
class PoseBuilder
    : public BuilderMixin<PoseBuilder<BaseBuilder>, BaseBuilder, Pose> {
  using BaseT = BuilderMixin<PoseBuilder<BaseBuilder>, BaseBuilder, Pose>;

 public:
  PoseBuilder(BaseBuilder& builder) : BaseT(builder) {}

  PoseBuilder& Timestamp(double timestamp) {
    this->Data().set_timestamp(timestamp);
    return *this;
  }

  PoseBuilder& MapOrigin(double longitude, double latitude, double altitude) {
    this->Data().mutable_map_origin()->set_longitude(longitude);
    this->Data().mutable_map_origin()->set_latitude(latitude);
    this->Data().mutable_map_origin()->set_altitude(altitude);
    return *this;
  }

  PoseBuilder& MapOrigin(const std::array<double, 3>& map_origin) {
    return this->MapOrigin(map_origin[0], map_origin[1], map_origin[2]);
  }

  PoseBuilder& Position(double x, double y, double z) {
    this->Data().clear_position();
    this->Data().add_position(x);
    this->Data().add_position(y);
    this->Data().add_position(z);
    return *this;
  }

  PoseBuilder& Position(const std::array<double, 3>& positions) {
    return this->Position(positions[0], positions[1], positions[2]);
  }

  PoseBuilder& Orientation(double roll, double pitch, double yaw) {
    this->Data().clear_orientation();
    this->Data().add_orientation(roll);
    this->Data().add_orientation(pitch);
    this->Data().add_orientation(yaw);
    return *this;
  }

  PoseBuilder& Orientation(const std::array<double, 3>& orientations) {
    return this->Orientation(orientations[0], orientations[1], orientations[2]);
  }

};

}  // namespace xviz