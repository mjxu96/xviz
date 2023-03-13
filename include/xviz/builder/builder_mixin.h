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

#include <cassert>

namespace xviz {

template <typename BuilderT, typename BaseBuilderT, typename DataT>
class BuilderMixin {
public:
  BuilderMixin(BaseBuilderT& builder) : builder_(builder) {}

  auto&& GetMessage() {
    return builder_.GetMessage();
  }

  template <typename ...Args>
  auto&& Primitive(Args&&... args) {
    return builder_.Primitive(std::forward<Args>(args)...);
  }

  template <typename ...Args>
  auto&& Pose(Args&&... args) {
    return builder_.Pose(std::forward<Args>(args)...);
  }

  template <typename ...Args>
  auto&& TimeSeries(Args&&... args) {
    return builder_.TimeSeries(std::forward<Args>(args)...);
  }

protected:
  friend class Builder;
  DataT* data_{nullptr};

  DataT& Data() {
    assert(data_);
    return *data_;
  }

  BuilderT& Start(DataT& data) {
    assert(!data_);
    data_ = &data;
    return static_cast<BuilderT&>(*this);
  }

  void End() {
    if (!data_) {
      return;
    }
    // TODO do some checks
    data_ = nullptr;
  }
private:
  BaseBuilderT& builder_;
};
  
} // namespace xviz
