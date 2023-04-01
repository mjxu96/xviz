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

#include <type_traits>

#include <optional>
#include <unordered_map>

namespace xviz::tests {

template <typename T, template <typename...> typename Tpl>
struct IsTemplateClass : std::false_type {};

template <template <typename...> typename Tpl, typename... Args>
struct IsTemplateClass<Tpl<Args...>, Tpl> : std::true_type {};

template <typename T, template <typename...> typename Tpl>
constexpr bool IsTemplateClassValue = IsTemplateClass<T, Tpl>::value;

template <typename T>
using IsOptional = IsTemplateClass<std::remove_cvref_t<T>, std::optional>;

template <typename T>
using IsUnorderedMap =
    IsTemplateClass<std::remove_cvref_t<T>, std::unordered_map>;

}  // namespace xviz::tests
