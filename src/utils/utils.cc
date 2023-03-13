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

#include <xviz/utils/utils.h>

#include <cstdint>
#include <stdexcept>

std::vector<uint8_t> xviz::util::GetBytesArrayFromHexString(
    std::string_view hexstring) {
  if (hexstring.size() % 2 == 1) [[unlikely]] {
    throw std::runtime_error("TODO hex string should has even numbers of data");
  }
  std::vector<uint8_t> bytearray(hexstring.size() / 2, 0);
  for (size_t i = 0, j = 0; i < (hexstring.size() / 2); i++, j += 2)
    bytearray[i] =
        (hexstring[j] % 32 + 9) % 25 * 16 + (hexstring[j + 1] % 32 + 9) % 25;
  return bytearray;
}
