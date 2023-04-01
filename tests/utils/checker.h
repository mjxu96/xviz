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

#include "comparators.h"
#include "macros.h"
#include "types.h"

#include <gtest/gtest.h>

#include <optional>
#include <unordered_map>

namespace xviz::tests {

template <typename... CheckTargets>
class TargetChecker;

template <>
class TargetChecker<> {
 public:
  TargetChecker() = default;

  void Check() const {}
};

template <typename Target, typename... OtherCheckTargets>
class TargetChecker<Target, OtherCheckTargets...> {
 public:
  template <typename T, typename... Others>
  TargetChecker(T&& value, Others&&... other_values)
      : expected_value_(std::forward<T>(value)),
        next_(std::forward<Others>(other_values)...) {}

  template <typename T, typename... Others>
  void Check(T&& value, Others&&... others) const {
    // If value is an std::optional<T>
    if constexpr (IsOptional<T>::value) {
      if (expected_value_.has_value()) {
        EXPECT_EQ(value.value(), expected_value_.value());
      } else {
        EXPECT_EQ(value.has_value(), false);
      }
      // else if value is nullopt
    } else if constexpr (std::is_same_v<std::remove_cvref_t<T>,
                                        std::nullopt_t>) {
      EXPECT_EQ(expected_value_.has_value(), false);
    } else {
      // if expected has stored value
      if (expected_value_.has_value()) {
        if constexpr (IsUnorderedMap<T>::value) {
          // for unordered map we will need to iterate all elements
          auto& expected_value_map_ = expected_value_.value();
          EXPECT_EQ(expected_value_map_.size(), value.size());
          for (const auto& [k, v] : value) {
            auto found_itr = expected_value_map_.find(k);
            // we have found the same key
            EXPECT_NE(found_itr, expected_value_map_.end());
            EXPECT_EQ(found_itr->second, v);
          }
        } else {
          EXPECT_EQ(value, expected_value_.value());
        }
      } else {
        // check other situations
        if constexpr (std::is_same_v<std::remove_cvref_t<T>, std::string> ||
                      IsUnorderedMap<T>::value) {
          EXPECT_EQ(value.empty(), true);
        } else {
          FAIL() << "Unacceptable type: " << typeid(T).name() << std::endl
                 << expected_value_.has_value() << std::endl;
        }
      }
    }
    next_.Check(std::forward<Others>(others)...);
  }

 private:
  std::optional<Target> expected_value_;
  TargetChecker<OtherCheckTargets...> next_;
};

}  // namespace xviz::tests
