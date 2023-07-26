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

#include "builder_mixin.h"

#include <string>

namespace xviz {

template <typename BaseBuilder>
class UIPrimitiveBuilder : public BuilderMixin<UIPrimitiveBuilder<BaseBuilder>,
                                               BaseBuilder, UIPrimitiveState> {
  using BaseT = BuilderMixin<UIPrimitiveBuilder<BaseBuilder>, BaseBuilder,
                             UIPrimitiveState>;

 public:
  UIPrimitiveBuilder(BaseBuilder& builder) : BaseT(builder) {}

  UIPrimitiveBuilder& Column(const std::string& display_text,
                             xviz::TreeTableColumn::ColumnType column_type,
                             const std::string& unit = "") {
    auto new_column = this->Data().mutable_treetable()->add_columns();
    new_column->set_display_text(display_text);
    new_column->set_type(column_type);
    new_column->set_unit(unit);
    return *this;
  }

  UIPrimitiveBuilder& Row(
      int id, int parent,
      const std::vector<xviz::util::TreeTableValueVariant>& values) {
    if (values.size() != this->Data().mutable_treetable()->columns_size())
        [[unlikely]] {
      throw std::runtime_error(
          std::format("TODO wrong number of column values"));
    }
    auto new_row = this->Data().mutable_treetable()->add_nodes();
    new_row->set_id(id);
    new_row->set_parent(parent);
    uint32_t idx = 0;
    for (const auto& v : values) {
      auto expected_type =
          this->Data().mutable_treetable()->columns().at(idx).type();
      if (!v.IsSameType(expected_type)) [[unlikely]] {
        throw std::runtime_error(std::format(
            "TODO column {} value type {} at row {} does not match the "
            "required column type {}",
            this->Data().treetable().columns(idx).display_text(),
            xviz::util::TreeTableValueVariant::GetColumnTypeString(
                v.GetHoldType()),
            idx,
            xviz::util::TreeTableValueVariant::GetColumnTypeString(
                expected_type)));
      }
      new_row->add_column_values(v.ToString());
      idx++;
    }
    return *this;
  }

  UIPrimitiveBuilder& Row(
      int id, const std::vector<xviz::util::TreeTableValueVariant>& values) {
    return Row(id, 0, values);
  }
};

}  // namespace xviz