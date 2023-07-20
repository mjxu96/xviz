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
#include "primitive_base.h"

namespace xviz {

template <typename PrimitiveBuilderType, typename BuilderType>
class PrimitivePointBuilder
    : public PrimitiveBaseBuilder<
          PrimitivePointBuilder<PrimitiveBuilderType, BuilderType>, Point,
          PrimitiveBuilderType, BuilderType> {
  using BaseType = PrimitiveBaseBuilder<
      PrimitivePointBuilder<PrimitiveBuilderType, BuilderType>, Point,
      PrimitiveBuilderType, BuilderType>;

 public:
  using BaseType::BaseType;
  using BaseType::End;
  using BaseType::Start;

  // must be R,G,B,A
  PrimitivePointBuilder& Color(const std::vector<uint8_t>& flatten_colors) {
    assert(flatten_colors.size() % 4 == 0);
    assert(flatten_colors.size() / 4 == this->Data().points_size() / 3);
    this->Data().set_colors(flatten_colors.data(), flatten_colors.size());
    return *this;
  }

  PrimitivePointBuilder& Color(
      const std::vector<std::array<uint8_t, 4>>& colors) {
    assert(colors.size() == this->Data().points_size() / 3);
    std::vector<uint8_t> flatten_colors;
    flatten_colors.reserve(4 * colors.size());
    for (const auto& color : colors) {
      for (uint8_t c : color) {
        flatten_colors.push_back(c);
      }
    }
    return this->Color(flatten_colors);
  }
};

}  // namespace xviz
