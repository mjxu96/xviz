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

#include <xviz/def.h>
#include <xviz/utils/style_utils.h>

#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <variant>

namespace xviz {

using namespace v2;

class MetadataBuilder;

template <typename StreamMetadataBuilderT, typename MetadataBuilderT>
class StreamMetadataBuilderMixin
    : public MetadataBuilderMixin<MetadataBuilderT> {
  using BaseT = MetadataBuilderMixin<MetadataBuilderT>;

 public:
  StreamMetadataBuilderMixin(StreamMetadataBuilderT& builder,
                             MetadataBuilderT& parent_builder)
      : BaseT(parent_builder), builder_(builder) {}

  auto&& Category(StreamMetadata::Category category) {
    return builder_.Category(category);
  }

  auto&& Coordinate(StreamMetadata::CoordinateType coordinate_type) {
    return builder_.Coordinate(coordinate_type);
  }

  auto&& StreamStyle(const StyleType& style) {
    return builder_.StreamStyle(style);
  }

  template <std::same_as<StyleStreamValue> T>
  auto&& StreamStyle(T&& style) {
    return builder_.StreamStyle(std::forward<T>(style));
  }

  auto&& StyleClass(const std::string_view class_name, const StyleType& style) {
    return builder_.StyleClass(class_name, style);
  }

  template <std::same_as<StyleObjectValue> T>
  auto&& StyleClass(const std::string_view class_name, T&& style) {
    return builder_.StyleClass(class_name, std::forward<T>(style));
  }

 private:
  StreamMetadataBuilderT& builder_;
};

template <typename BaseBuilder, typename MetadataBuilderT>
class StreamMetadataCategoryBuilderBase
    : public StreamMetadataBuilderMixin<BaseBuilder, MetadataBuilderT> {
  using BaseT = StreamMetadataBuilderMixin<BaseBuilder, MetadataBuilderT>;

 public:
  using BaseT::BaseT;

 protected:
  template <typename>
  friend class StreamMetadataBuilder;

  template <bool ForceCheck, typename T>
  void SetType(T type) {
    if (static_cast<std::underlying_type_t<T>>(type) == 0) [[unlikely]] {
      // TODO static_assert invalid enum are all 0s
      throw std::runtime_error("TODO");
    }
    if constexpr (std::is_same_v<T, StreamMetadata::PrimitiveType>) {
      if constexpr (ForceCheck) {
        if (data_->category() != StreamMetadata::PRIMITIVE) [[unlikely]] {
          throw std::runtime_error("TODO");
        }
      }
      data_->set_primitive_type(type);
    } else if constexpr (std::is_same_v<T, StreamMetadata::ScalarType>) {
      if constexpr (ForceCheck) {
        if (data_->category() != StreamMetadata::VARIABLE &&
            data_->category() != StreamMetadata::TIME_SERIES) [[unlikely]] {
          throw std::runtime_error("TODO");
        }
      }
      data_->set_scalar_type(type);
    }
  }

  template <bool ForceCheck, typename... Args>
    requires(std::constructible_from<std::string, Args...>)
  void SetUnit(Args&&... args) {
    if constexpr (ForceCheck) {
      if ((data_->category() != StreamMetadata::TIME_SERIES) &&
          (data_->category() != StreamMetadata::VARIABLE)) [[unlikely]] {
        throw std::runtime_error("TODO");
      }
    }
    data_->set_units(std::forward<Args>(args)...);
  }

  void StartCategory(StreamMetadata& data, StreamMetadata::Category category) {
    assert(!this->data_);
    data_ = &data;
    data_->set_category(category);
  }

  void EndCategory() {
    if (!data_) {
      return;
    }
    // TODO do some checks here
    data_ = nullptr;
  }
  StreamMetadata* data_{nullptr};
};

template <typename BaseBuilder, StreamMetadata::Category C,
          typename MetadataBuilderT>
class StreamMetadataCategoryBuilder
    : public StreamMetadataCategoryBuilderBase<BaseBuilder, MetadataBuilderT> {
  using BaseT =
      StreamMetadataCategoryBuilderBase<BaseBuilder, MetadataBuilderT>;

 public:
  using BaseT::BaseT;

  StreamMetadataCategoryBuilder& Type(StreamMetadata::PrimitiveType type)
    requires(C == StreamMetadata::PRIMITIVE)
  {
    BaseT::template SetType<false, StreamMetadata::PrimitiveType>(type);
    return *this;
  }

  template <typename... Args>
    requires(std::constructible_from<std::string, Args...> &&
             (C == StreamMetadata::VARIABLE ||
              C == StreamMetadata::TIME_SERIES))
  StreamMetadataCategoryBuilder& Unit(Args&&... args) {
    BaseT::template SetUnit<false, Args...>(std::forward<Args>(args)...);
    return *this;
  }
};

template <typename BaseBuilder>
class StreamMetadataBuilder : public MetadataBuilderMixin<BaseBuilder> {
  using BaseT = MetadataBuilderMixin<BaseBuilder>;

 public:
  StreamMetadataBuilder(BaseBuilder& parent_builder)
      : BaseT(parent_builder), category_builder_(*this, parent_builder) {}

  StreamMetadataBuilder& Category(StreamMetadata::Category category) {
    category_builder_.EndCategory();
    category_builder_.StartCategory(*data_, category);
    return *this;
  }

  template <StreamMetadata::Category C>
  StreamMetadataCategoryBuilder<StreamMetadataBuilder, C, BaseBuilder>&
  Category()
    requires(C != StreamMetadata::STREAM_METADATA_CATEGORY_INVALID)
  {
    category_builder_.EndCategory();
    auto& builder = reinterpret_cast<
        StreamMetadataCategoryBuilder<StreamMetadataBuilder, C, BaseBuilder>&>(
        category_builder_);
    builder.StartCategory(*data_, C);
    return builder;
  }

  template <typename T>
  StreamMetadataBuilder& Type(T type) {
    category_builder_.template SetType<true, T>(type);
    return *this;
  }

  template <typename... Args>
    requires(std::constructible_from<std::string, Args...>)
  StreamMetadataBuilder& Unit(Args&&... args) {
    category_builder_.template SetUnit<false, Args...>(
        std::forward<Args>(args)...);
    return *this;
  }

  StreamMetadataBuilder& Coordinate(
      StreamMetadata::CoordinateType coordinate_type) {
    data_->set_coordinate(coordinate_type);
    return *this;
  }

  StreamMetadataBuilder& StreamStyle(const StyleType& style) {
    return this->StreamStyle(
        detail::ConvertInternalTypeToProtobufType<StyleStreamValue>(style));
  }

  template <std::same_as<StyleStreamValue> T>
  StreamMetadataBuilder& StreamStyle(T&& style) {
    (*data_->mutable_stream_style()) = std::forward<T>(style);
    return *this;
  }

  StreamMetadataBuilder& StyleClass(const std::string_view class_name,
                                    const StyleType& style) {
    return this->StyleClass(
        class_name,
        detail::ConvertInternalTypeToProtobufType<StyleObjectValue>(style));
  }

  template <std::same_as<StyleObjectValue> T>
  StreamMetadataBuilder& StyleClass(const std::string_view class_name,
                                    T&& style) {
    auto new_style_class = data_->add_style_classes();
    new_style_class->set_name(class_name.data());
    *(new_style_class->mutable_style()) = std::forward<T>(style);
    return *this;
  }

 private:
  friend class MetadataBuilder;
  StreamMetadataBuilder& StartStream(StreamMetadata& data) {
    assert(!data_);
    data_ = &data;
    return *this;
  }
  void EndStream() {
    if (!data_) {
      return;
    }
    // TODO do some checks here
    category_builder_.EndCategory();
    data_ = nullptr;
  }
  StreamMetadata* data_{nullptr};
  StreamMetadataCategoryBuilderBase<StreamMetadataBuilder, BaseBuilder>
      category_builder_;
};

}  // namespace xviz
