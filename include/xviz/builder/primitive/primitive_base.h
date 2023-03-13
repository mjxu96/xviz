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

#include "primitive_mixin.h"

#include <xviz/utils/style_utils.h>

namespace xviz {

template <typename PrimitiveSubBuilderType, typename PrimitiveSubType,
          typename PrimitiveBuilderType, typename BuilderType>
class PrimitiveBaseBuilder
    : public PrimitiveBuilderMixin<PrimitiveBuilderType,
                                   PrimitiveSubBuilderType, BuilderType,
                                   PrimitiveSubType> {
  using BaseType = PrimitiveBuilderMixin<PrimitiveBuilderType, PrimitiveSubBuilderType, BuilderType,
                                         PrimitiveSubType>;

 public:
  using BaseType::BaseType;
  using BaseType::Start;
  using BaseType::End;

  template <xviz::concepts::CanConstructString... Args>
  PrimitiveSubBuilderType& ID(Args&&... args) {
    this->Data().mutable_base()->set_object_id(std::forward<Args>(args)...);
    return static_cast<PrimitiveSubBuilderType&>(*this);
  }

  PrimitiveSubBuilderType& Classes(const std::vector<std::string>& classes) {
    *this->Data().mutable_base()->mutable_classes() = {classes.begin(),
                                                       classes.end()};
    return static_cast<PrimitiveSubBuilderType&>(*this);
  }

  PrimitiveSubBuilderType& Classes(std::vector<std::string>&& classes) {
    *this->Data().mutable_base()->mutable_classes() = {
        std::make_move_iterator(classes.begin()),
        std::make_move_iterator(classes.end())};
    return static_cast<PrimitiveSubBuilderType&>(*this);
  }

  PrimitiveSubBuilderType& Style(const StyleType& style) {
    return this->Style(
        detail::ConvertInternalTypeToProtobufType<StyleObjectValue>(style));
  }

  template <std::same_as<StyleObjectValue> T>
  PrimitiveSubBuilderType& Style(T&& style) {
    (*this->Data().mutable_base()->mutable_style()) = std::forward<T>(style);
    return static_cast<PrimitiveSubBuilderType&>(*this);
  }

 protected:
  // PrimitiveBuilderType& Start(PrimitiveSubType& data) {
  //   assert(!data_);
  //   data_ = &data;
  //   return static_cast<PrimitiveBuilderType&>(*this);
  // }

  // void End() {
  //   if (!data_) {
  //     return;
  //   }
  //   // TODO some checks here;
  //   data_ = nullptr;
  // }
  // PrimitiveSubType* data_{nullptr};
  // Circle* data_;
};

}  // namespace xviz
