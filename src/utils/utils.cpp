/*
 * File: utils.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Tuesday, 24th December 2019 8:42:41 pm
 */

#include "utils/utils.h"

using Json = nlohmann::json;

std::shared_ptr<xviz::StyleObjectValue> xviz::JsonObjectToStyleObject(const nlohmann::json& json) {
  auto style_object = std::make_shared<xviz::StyleObjectValue>();
  if (json.find("fill_color") != json.end()) {
    auto fill_color = json.value("fill_color", "#FFFFFF");
    style_object->set_fill_color(fill_color.c_str());
  }
  return style_object;
}

std::shared_ptr<xviz::StyleObjectValue> xviz::JsonObjectToStyleObject(nlohmann::json&& json) {
  auto style_object = std::make_shared<xviz::StyleObjectValue>();
  if (json.find("fill_color") != json.end()) {
    auto fill_color = json.value("fill_color", "#FFFFFF");
    style_object->set_fill_color(fill_color.c_str());
  }
  return style_object;
}

std::shared_ptr<xviz::StyleObjectValue> xviz::JsonStringToStyleObject(const std::string& json_str) {
  return JsonObjectToStyleObject(Json::parse(json_str));
}

std::shared_ptr<xviz::StyleObjectValue> xviz::JsonStringToStyleObject(std::string&& json_str) {
  return JsonObjectToStyleObject(Json::parse(std::move(json_str)));
}

std::shared_ptr<xviz::StyleStreamValue> xviz::JsonObjectToStyleStream(const nlohmann::json& json) {
  auto style_stream = std::make_shared<xviz::StyleStreamValue>();
  if (json.find("fill_color") != json.end()) {
    auto fill_color = json.value("fill_color", "#FFFFFF");
    style_stream->set_fill_color(fill_color.c_str());
  }
  return style_stream;
}

std::shared_ptr<xviz::StyleStreamValue> xviz::JsonObjectToStyleStream(nlohmann::json&& json) {
  auto style_stream = std::make_shared<xviz::StyleStreamValue>();
  if (json.find("fill_color") != json.end()) {
    auto fill_color = json.value("fill_color", "#FFFFFF");
    style_stream->set_fill_color(fill_color.c_str());
  }
  return style_stream;
}

std::shared_ptr<xviz::StyleStreamValue> xviz::JsonStringToStyleStream(const std::string& json_str) {
  return JsonObjectToStyleStream(Json::parse(json_str));
}

std::shared_ptr<xviz::StyleStreamValue> xviz::JsonStringToStyleStream(std::string&& json_str) {
  return JsonObjectToStyleStream(Json::parse(std::move(json_str)));
}