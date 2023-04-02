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
class TimeSeriesBuilder : public BuilderMixin<TimeSeriesBuilder<BaseBuilder>,
                                              BaseBuilder, TimeSeriesState> {
  using BaseT = BuilderMixin<TimeSeriesBuilder<BaseBuilder>, BaseBuilder,
                             TimeSeriesState>;

 public:
  TimeSeriesBuilder(BaseBuilder& builder) : BaseT(builder) {}

  TimeSeriesBuilder& Timestamp(double timestamp) {
    this->Data().set_timestamp(timestamp);
    return *this;
  }

  template <xviz::concepts::TimeSeriesAcceptableType... Args>
  TimeSeriesBuilder& Value(Args&&... args) {
    auto value = this->Data().mutable_values();
    if (value->doubles_size() || value->int32s_size() || value->bools_size() ||
        value->strings_size()) [[unlikely]] {
      throw std::runtime_error(
          std::format("TODO Cannot set multiple values for one stream {}",
                      this->Data().streams().at(0)));
    }

    if constexpr (xviz::concepts::CanConstructString<Args...>) {
      value->add_strings(std::forward<Args>(args)...);
    } else if constexpr (sizeof...(Args) == 1) {
      using ArgType = xviz::concepts::FirstArgType<Args...>;
      if constexpr (std::is_same_v<ArgType, bool>) {
        value->add_bools(args...);
      } else if constexpr (std::is_same_v<ArgType, double>) {
        value->add_doubles(args...);
      } else if constexpr (std::is_same_v<ArgType, int32_t>) {
        value->add_int32s(args...);
      } else {
        throw std::runtime_error(std::format("TODO this is impossible"));
      }
    } else {
      throw std::runtime_error(std::format("TODO this is impossible"));
    }

    return *this;
  }

  template <xviz::concepts::CanConstructString... Args>
  TimeSeriesBuilder& ID(Args&&... args) {
    this->Data().set_object_id(std::forward<Args>(args)...);
    return *this;
  }
};

}  // namespace xviz