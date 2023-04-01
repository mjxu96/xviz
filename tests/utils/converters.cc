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

#include "converters.h"
#include <xviz/utils/style_utils.h>

namespace xviz::tests {

std::unordered_map<std::string, xviz::v2::StyleObjectValue>
ConvertStyleClassToMap(
    const google::protobuf::RepeatedPtrField<xviz::v2::StyleClass>& input) {
  std::unordered_map<std::string, xviz::v2::StyleObjectValue> ret;
  for (const auto& style_class : input) {
    ret[style_class.name()] = style_class.style();
  }
  return ret;
}

std::unordered_map<std::string, xviz::v2::StyleObjectValue>
ConvertMapToStyleClass(
    const std::unordered_map<std::string, xviz::StyleType>& input) {
  std::unordered_map<std::string, xviz::v2::StyleObjectValue> ret;
  for (const auto& [name, style_class] : input) {
    ret[name] =
        detail::ConvertInternalTypeToProtobufType<xviz::v2::StyleObjectValue>(
            style_class);
  }
  return ret;
}

}  // namespace xviz::tests
