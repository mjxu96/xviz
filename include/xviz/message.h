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

#include <xviz/builder/builder.h>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/util/json_util.h>

namespace xviz {

template<typename MessageType>
struct MessageTypeStr;

template<>
struct MessageTypeStr<StateUpdate> {
  constexpr static auto value = "xviz/state_update";
};

template<>
struct MessageTypeStr<Metadata> {
  constexpr static auto value = "xviz/metadata";
};

template <typename MessageType>
class Message {
 public:
  explicit Message(MessageType&& message)
      : message_(std::forward<MessageType>(message)) {
    json_print_option_.preserve_proto_field_names = true;
  }

  Message(const Message&) = default;
  Message& operator=(const Message&) = default;
  Message(Message&&) = default;
  Message& operator=(Message&&) = default;

  std::string ToJsonString() {
    std::string ret;
    Envelope evenlope;
    evenlope.set_type(type_.data());
    evenlope.mutable_data()->PackFrom(message_);
    google::protobuf::util::MessageToJsonString(evenlope, &ret,
                                                json_print_option_);
    return ret;
  }

  std::string ToProtobufBinary() {
    std::string ret = "\x50\x42\x45\x31";
    Envelope evenlope;
    evenlope.set_type(type_.data());
    evenlope.mutable_data()->PackFrom(message_);
    evenlope.AppendToString(&ret);
    return ret;
  }

 private:
  MessageType message_;
  constexpr static std::string_view type_ = MessageTypeStr<MessageType>::value;
  google::protobuf::util::JsonPrintOptions json_print_option_;
};

}  // namespace xviz
