/*
 * File: primitive.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Tuesday, 17th December 2019 10:00:38 pm
 */

#include "builder/primitive.h"

using namespace xviz;
using Json = nlohmann::json;

template<typename T>
void AddVertices(T& vertice_to_add, const std::shared_ptr<std::vector<double>>& vertices) {
  if (vertices == nullptr) {
    LOG_ERROR("Vertice pointer is NULL");
    return;
  }
  for (const auto& v : *vertices) {
    vertice_to_add.add_vertices(v);
    // vertice_to_add.add_vertices(point.y);
    // vertice_to_add.add_vertices(point.z);
  }
}

XVIZPrimitiveBuilder::XVIZPrimitiveBuilder(std::shared_ptr<Metadata> metadata) :
  XVIZBaseBuilder(xviz::StreamMetadata::PRIMITIVE, metadata) {
  primitives_ = std::make_shared<std::unordered_map<std::string, PrimitiveState>>();
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Stream(const std::string& stream_id) {
  if (stream_id_.size() > 0) {
    this->Flush();
  }
  stream_id_ = stream_id;
  return *this;
}

// Polygon
XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Polygon(const std::vector<double>& vertices) {
  auto vertices_ptr = std::make_shared<std::vector<double>>(vertices);
  return Polygon(vertices_ptr);
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Polygon(std::vector<double>&& vertices) {
  auto vertices_ptr = std::make_shared<std::vector<double>>(std::move(vertices));
  return Polygon(vertices_ptr);
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Polygon(const std::shared_ptr<std::vector<double>>& vertices_ptr) {
  if (type_ != nullptr) {
    Flush();
  }
  vertices_ = std::shared_ptr<std::vector<double>>(vertices_ptr);
  type_ = std::make_shared<Primitive>();
  *type_ = Primitive::StreamMetadata_PrimitiveType_POLYGON;
  return *this;
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Polyline(const std::vector<double>& vertices) {
  auto vertices_ptr = std::make_shared<std::vector<double>>(vertices);
  return Polyline(vertices_ptr);
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Polyline(std::vector<double>&& vertices) {
  auto vertices_ptr = std::make_shared<std::vector<double>>(vertices);
  return Polyline(vertices_ptr);
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Polyline(const std::shared_ptr<std::vector<double>>& vertices_ptr) {
  if (type_ != nullptr) {
    Flush();
  }
  vertices_ = std::shared_ptr<std::vector<double>>(vertices_ptr);
  type_ = std::make_shared<Primitive>();
  *type_ = Primitive::StreamMetadata_PrimitiveType_POLYLINE;
  return *this;
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Points(const std::vector<double>& vertices) {
  auto vertices_ptr = std::make_shared<std::vector<double>>(vertices);
  return Points(vertices_ptr);
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Points(std::vector<double>&& vertices) {
  auto vertices_ptr = std::make_shared<std::vector<double>>(vertices);
  return Points(vertices_ptr);
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Points(const std::shared_ptr<std::vector<double>>& vertices_ptr) {
  if (type_ != nullptr) {
    Flush();
  }
  vertices_ = std::shared_ptr<std::vector<double>>(vertices_ptr);
  type_ = std::make_shared<Primitive>();
  *type_ = Primitive::StreamMetadata_PrimitiveType_POINT;
  return *this;
}

// Style
XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Style(const std::string& style_json_str) {
  return Style(JsonStringToStyleObject(style_json_str));
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Style(const Json& style_json) {
  return Style(JsonObjectToStyleObject(style_json));
}

XVIZPrimitiveBuilder& XVIZPrimitiveBuilder::Style(const std::shared_ptr<StyleObjectValue>& style_object) {
  ValidatePrerequisite();
  style_ = style_object;
  return *this;
}

std::shared_ptr<std::unordered_map<std::string, PrimitiveState>> XVIZPrimitiveBuilder::GetData() {
  if (type_ != nullptr) {
    Flush();
  }
  if (primitives_->empty()) {
    return nullptr;
  }
  return primitives_;
}

void XVIZPrimitiveBuilder::Flush() {
  Validate();
  FlushPrimitives();
}

void XVIZPrimitiveBuilder::Validate() {
  // SUPER :: VALIDATE
  // TODO imple this function
}

void XVIZPrimitiveBuilder::ValidatePrerequisite() {
  if (type_ == nullptr) {
    LOG_ERROR("Start from a primitive first, e.g Polygon(), Image(), etc.");
  }
}

void XVIZPrimitiveBuilder::FlushPrimitives() {
  if (primitives_->find(stream_id_) == primitives_->end()) {
    (*primitives_)[stream_id_] = PrimitiveState();
  }
  auto stream_ptr = &(*primitives_)[stream_id_];
  auto base_pair = FlushPrimitiveBase();
  auto has_base = base_pair.first;
  auto base = base_pair.second;

  switch (*type_) {
    case Primitive::StreamMetadata_PrimitiveType_POLYGON:
      {
        auto polygon_ptr = stream_ptr->add_polygons();
        AddVertices<xviz::Polygon>(*polygon_ptr, vertices_);
        if (has_base) {
          auto cur_base_ptr = polygon_ptr->mutable_base();
          cur_base_ptr->MergeFrom(base);
        }
        break;
      }

    case Primitive::StreamMetadata_PrimitiveType_POLYLINE:
      {
        auto polyline_ptr = stream_ptr->add_polylines();
        AddVertices<xviz::Polyline>(*polyline_ptr, vertices_);
        if (has_base) {
          auto cur_base_ptr = polyline_ptr->mutable_base();
          cur_base_ptr->MergeFrom(base);
        }
        break;
      }
    
    case Primitive::StreamMetadata_PrimitiveType_POINT:
      {
        if (vertices_ == nullptr) {
          LOG_ERROR("Vertice pointer is NULL");
          break;
        }
        auto point_ptr = stream_ptr->add_points();
        for (const auto& v : *vertices_) {
          point_ptr->add_points(v);
        }
        if (colors_ != nullptr) {
          auto colors_ptr = point_ptr->mutable_colors();
          *colors_ptr = *colors_;
        }
        if (has_base) {
          auto cur_base_ptr = point_ptr->mutable_base();
          cur_base_ptr->MergeFrom(base);
        }
        break;
      }

    case Primitive::StreamMetadata_PrimitiveType_IMAGE:
      {
        if (vertices_ != nullptr && vertices_->size() > 2) {
          image_->add_position((*vertices_)[0]);
          image_->add_position((*vertices_)[1]);
          image_->add_position((*vertices_)[2]);
        }
        auto image_ptr = stream_ptr->add_images();
        *image_ptr = std::move(*image_);
        if (has_base) {
          auto cur_base_ptr = image_ptr->mutable_base();
          cur_base_ptr->MergeFrom(base);
        }
        break;
      }
    
    // TEXT, CIRCLE, STADIUM,
    default:
      LOG_ERROR("No this type exists %d", *type_);
      return;
  }


  Reset();
}

std::pair<bool, PrimitiveBase> XVIZPrimitiveBuilder::FlushPrimitiveBase() {
  bool has_base = false;
  PrimitiveBase base;

  if (id_ != nullptr) {
    has_base = true;
    auto id_ptr = base.mutable_object_id();
    *id_ptr = *id_;
  }

  if (style_ != nullptr) {
    has_base = true;
    auto style_ptr = base.mutable_style();
    // TODO style?
    style_ptr->MergeFrom(*style_);
    // *style_ptr = *style_;
  }

  if (classes_ != nullptr) {
    has_base = true;
    for (const auto& c : *classes_) {
      auto new_class_ptr = base.add_classes();
      *new_class_ptr = c;
    }
  }


  return {has_base, base};
}

void XVIZPrimitiveBuilder::Reset() {
  type_ = nullptr;

  image_ = nullptr;
  vertices_ = nullptr;
  radius_ = nullptr;
  text_ = nullptr;
  colors_ = nullptr;

  id_ = nullptr;
  style_ = nullptr;
  classes_ = nullptr;
}