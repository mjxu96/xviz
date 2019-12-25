/*
 * File: primitive.h
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Tuesday, 17th December 2019 9:57:00 pm
 */

#ifndef XVIZ_PRIMITIVE_BUILDER_H_
#define XVIZ_PRIMITIVE_BUILDER_H_

#include "base_builder.h"
#include "utils/json.hpp"
#include "utils/macrologger.h"
#include "utils/utils.h"
#include "proto/core.pb.h"
#include "proto/primitives.pb.h"

namespace xviz {

class XVIZPrimitiveBuilder : public XVIZBaseBuilder, public std::enable_shared_from_this<XVIZPrimitiveBuilder> {
public:
  XVIZPrimitiveBuilder(std::shared_ptr<Metadata> metadata);

  XVIZPrimitiveBuilder& Stream(const std::string& stream_id);
  std::shared_ptr<std::unordered_map<std::string, PrimitiveState>> GetData();

  XVIZPrimitiveBuilder& Polygon(const std::vector<double>& vertices);
  XVIZPrimitiveBuilder& Polygon(std::vector<double>&& vertices);
  XVIZPrimitiveBuilder& Polygon(const std::shared_ptr<std::vector<double>>& vertices_ptr);

  XVIZPrimitiveBuilder& Polyline(const std::vector<double>& vertices);
  XVIZPrimitiveBuilder& Polyline(std::vector<double>&& vertices);
  XVIZPrimitiveBuilder& Polyline(const std::shared_ptr<std::vector<double>>& vertices_ptr);

  XVIZPrimitiveBuilder& Points(const std::vector<double>& vertices);
  XVIZPrimitiveBuilder& Points(std::vector<double>&& vertices);
  XVIZPrimitiveBuilder& Points(const std::shared_ptr<std::vector<double>>& vertices_ptr);

  XVIZPrimitiveBuilder& Style(const nlohmann::json& style_json);
  // XVIZPrimitiveBuilder& Style(std::shared_ptr<nlohmann::json>& style_json_ptr);
  XVIZPrimitiveBuilder& Style(const std::string& style_json_str);
  // XVIZPrimitiveBuilder& Style(std::shared_ptr<std::string>& style_json_str_ptr);
  XVIZPrimitiveBuilder& Style(const std::shared_ptr<StyleObjectValue>& style_object);
private:
  void Reset();
  void Flush() override;
  void FlushPrimitives();
  std::pair<bool, PrimitiveBase> FlushPrimitiveBase();
  void Validate();
  void ValidatePrerequisite();

  // void SetVertices(const std::shared_ptr<std::vector<double>>& vertices_ptr);
  

  std::shared_ptr<std::unordered_map<std::string, PrimitiveState>> primitives_{nullptr};

  std::shared_ptr<Primitive> type_{nullptr};

  std::shared_ptr<Image> image_{nullptr};
  std::shared_ptr<std::vector<double>> vertices_{nullptr};
  std::shared_ptr<double> radius_{nullptr};
  std::shared_ptr<std::string> text_{nullptr};
  std::shared_ptr<std::string> colors_{nullptr};

  std::shared_ptr<std::string> id_{nullptr};
  std::shared_ptr<StyleObjectValue> style_{nullptr};
  std::shared_ptr<std::vector<std::string>> classes_{nullptr};
  // TODO classes ????
};
  
} // namespace xviz


#endif