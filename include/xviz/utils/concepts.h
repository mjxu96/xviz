/*
 * Project: libxviz
 * Description: C++ Implementation of XVIZ Protocol
 * Author: Minjun Xu (mjxu96@outlook.com)
 * -----
 * MIT License
 * Copyright (c) 2022 Minjun Xu
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

#include <concepts>
#include <cstdint>
#include <string>
#include <type_traits>

namespace xviz::concepts {

template <typename... Args>
using FirstArgType = std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>;

template <typename... Args>
concept CanConstructString = std::constructible_from<std::string, Args...>;

template <typename... ValueArgsT>
concept TimeSeriesAcceptableType = CanConstructString<ValueArgsT...> ||
    ((sizeof...(ValueArgsT) == 1) &&
     (std::same_as<FirstArgType<ValueArgsT...>, bool> ||
      std::same_as<FirstArgType<ValueArgsT...>, double> ||
      std::same_as<FirstArgType<ValueArgsT...>, int32_t>));

}  // namespace xviz::concepts