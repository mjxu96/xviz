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

#include <xviz/utils/utils.h>

#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace xviz {

class StyleValueVariant {
 public:
  StyleValueVariant(const std::string& v) : storage_(v) {}
  StyleValueVariant(const char* v) : storage_(v) {}
  StyleValueVariant(uint32_t v) : storage_(v) {}
  StyleValueVariant(float v) : storage_(v) {}
  StyleValueVariant(bool v) : storage_(v) {}

  StyleValueVariant(TextAnchor v) : storage_(static_cast<int>(v)) {}
  StyleValueVariant(TextAlignmentBaseline v) : storage_(static_cast<int>(v)) {}
  StyleValueVariant(PointColorMode v) : storage_(static_cast<int>(v)) {}
  StyleValueVariant(const std::vector<float>& v) : storage_(v) {}
  StyleValueVariant(std::initializer_list<float> v)
      : storage_(std::vector<float>(v)) {}

  auto&& Variant() { return storage_; }

  template <typename T>
  auto&& GetValue() const {
    return std::get<T>(storage_);
  }

  template <typename T>
  bool CheckType() const {
    return std::holds_alternative<T>(storage_);
  }
  template <typename T>
  void ThrowIfTypeNotMatch(std::string_view style_name,
                           std::string_view type_name) const {
    if (!CheckType<T>()) {
      throw std::runtime_error(
          std::format("TODO {}'s type is not {}", style_name, type_name));
    }
  }

 private:
  std::variant<uint32_t, float, std::string, bool, int, std::vector<float>>
      storage_;
};

using StyleType = std::unordered_map<std::string, StyleValueVariant>;

namespace detail {

template <typename Ret>
Ret ConvertInternalTypeToProtobufType(const xviz::StyleType& style) {
  Ret ret;
  auto refl = ret.GetReflection();

  for (const auto& [style_name, style_value] : style) {
    auto field_desc = ret.GetDescriptor()->FindFieldByName(style_name);
    if (!field_desc) {
      throw std::runtime_error(
          std::format("TODO Field name {} is not found for {}", style_name,
                      ret.GetTypeName()));
    }
    switch (field_desc->cpp_type()) {
      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING: {
        style_value.template ThrowIfTypeNotMatch<std::string>(
            style_name, field_desc->cpp_type_name());
        const std::string& value = style_value.template GetValue<std::string>();
        if (std::strcmp("bytes", field_desc->type_name()) == 0) {
          // This is bytes array
          // It needs special handling for colors
          if (value.at(0) == '#') {
            auto bytes_array = xviz::util::GetBytesArrayFromHexString(
                value.substr(std::size_t(1)));
            refl->SetString(
                &ret, field_desc,
                std::string(bytes_array.begin(), bytes_array.end()));
            break;
          }
        }
        refl->SetString(&ret, field_desc, value);
        break;
      }

      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_BOOL:
        style_value.template ThrowIfTypeNotMatch<bool>(
            style_name, field_desc->cpp_type_name());
        refl->SetBool(&ret, field_desc, style_value.template GetValue<bool>());
        break;

      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_FLOAT:
        if (field_desc->is_repeated()) {
          style_value.template ThrowIfTypeNotMatch<std::vector<float>>(
              style_name, field_desc->cpp_type_name());
          auto& value = style_value.template GetValue<std::vector<float>>();
          for (auto f : value) {
            refl->AddFloat(&ret, field_desc, f);
          }
        } else {
          style_value.template ThrowIfTypeNotMatch<float>(
              style_name, field_desc->cpp_type_name());
          refl->SetFloat(&ret, field_desc,
                         style_value.template GetValue<float>());
        }
        break;

      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_UINT32:
        style_value.template ThrowIfTypeNotMatch<uint32_t>(
            style_name, field_desc->cpp_type_name());
        refl->SetUInt32(&ret, field_desc,
                        style_value.template GetValue<uint32_t>());
        break;

      case google::protobuf::FieldDescriptor::CppType::CPPTYPE_ENUM:
        style_value.template ThrowIfTypeNotMatch<int>(
            style_name, field_desc->cpp_type_name());
        refl->SetEnumValue(&ret, field_desc,
                           style_value.template GetValue<int>());
        break;

      default:
        throw std::runtime_error(
            std::format("TODO {}'s type {} is not supported for now",
                        style_name, field_desc->cpp_type_name()));
        break;
    }
  }
  return ret;
}

}  // namespace detail
}  // namespace xviz
