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

#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace xviz::util {

class TreeTableValueVariant {
 public:
  TreeTableValueVariant(const std::string& v) : storage_(v) {}
  TreeTableValueVariant(std::string&& v) : storage_(std::move(v)) {}
  TreeTableValueVariant(const char* v) : storage_(v) {}
  TreeTableValueVariant(int v) : storage_(v) {}
  TreeTableValueVariant(double v) : storage_(v) {}
  TreeTableValueVariant(bool v) : storage_(v) {}

  bool IsSameType(xviz::TreeTableColumn::ColumnType column_type) const {
    switch (column_type) {
      case xviz::TreeTableColumn::INT32:
        return std::holds_alternative<int>(storage_);
      case xviz::TreeTableColumn::DOUBLE:
        return std::holds_alternative<double>(storage_);
      case xviz::TreeTableColumn::STRING:
        return std::holds_alternative<std::string>(storage_);
      case xviz::TreeTableColumn::BOOLEAN:
        return std::holds_alternative<bool>(storage_);
      default:
        return false;
    }
  }

  xviz::TreeTableColumn::ColumnType GetHoldType() const {
    switch (storage_.index()) {
      case 0:
        return xviz::TreeTableColumn::INT32;
      case 1:
        return xviz::TreeTableColumn::DOUBLE;
      case 2:
        return xviz::TreeTableColumn::STRING;
      case 3:
        return xviz::TreeTableColumn::BOOLEAN;
      default:
        return xviz::TreeTableColumn::TREE_TABLE_COLUMN_COLUMN_TYPE_INVALID;
    }
  }

  std::string ToString() const {
    return std::visit(
        [](const auto& value) -> std::string {
          if constexpr (std::is_same_v<std::remove_cvref_t<decltype(value)>,
                                       std::string>) {
            return value;
          } else {
            return std::to_string(value);
          }
        },
        storage_);
  }

  static std::string GetColumnTypeString(
      xviz::TreeTableColumn::ColumnType type) {
    return TreeTableColumn_ColumnType_Name(type);
  }

 private:
  std::variant<int, double, std::string, bool> storage_;
};

std::vector<uint8_t> GetBytesArrayFromHexString(std::string_view hexstring);

}  // namespace xviz::util
