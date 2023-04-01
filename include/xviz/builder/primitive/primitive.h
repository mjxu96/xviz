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

#include "circle.h"
#include "image.h"
#include "point.h"
#include "polygon.h"
#include "polyline.h"
#include "text.h"

#include <xviz/builder/builder_mixin.h>
#include <xviz/def.h>

namespace xviz {

template <typename BaseBuilder>
class PrimitiveBuilder : public BuilderMixin<PrimitiveBuilder<BaseBuilder>,
                                             BaseBuilder, PrimitiveState> {
  using BaseT =
      BuilderMixin<PrimitiveBuilder<BaseBuilder>, BaseBuilder, PrimitiveState>;

 public:
  PrimitiveBuilder(BaseBuilder& builder)
      : BaseT(builder),
        polygon_builder_(*this, builder),
        polyline_builder_(*this, builder),
        text_builder_(*this, builder),
        circle_builder_(*this, builder),
        point_builder_(*this, builder),
        image_builder_(*this, builder) {}

  PrimitivePolygonBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>& Polygon(
      const std::vector<std::array<float, 3>>& vertices) {
    polygon_builder_.End();
    auto new_polygon = this->Data().add_polygons();
    for (const auto& points : vertices) {
      for (float point : points) {
        new_polygon->add_vertices(point);
      }
    }
    return polygon_builder_.Start(*new_polygon);
  }

  PrimitivePolylineBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>&
  Polyline(const std::vector<std::array<float, 3>>& vertices) {
    polyline_builder_.End();
    auto new_polyline = this->Data().add_polylines();
    for (const auto& points : vertices) {
      for (float point : points) {
        new_polyline->add_vertices(point);
      }
    }
    return polyline_builder_.Start(*new_polyline);
  }

  PrimitiveCircleBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>& Circle(
      const std::array<float, 3>& center, float radius) {
    circle_builder_.End();
    auto new_circle = this->Data().add_circles();
    new_circle->add_center(center[0]);
    new_circle->add_center(center[1]);
    new_circle->add_center(center[2]);
    new_circle->set_radius(radius);
    return circle_builder_.Start(*new_circle);
  }

  template <typename... Args>
  PrimitiveImageBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>& Image(
      Args&&... args) {
    image_builder_.End();
    auto new_image = this->Data().add_images();
    new_image->set_data(std::forward<Args>(args)...);
    return image_builder_.Start(*new_image);
  }

  template <typename... Args>
  requires(std::constructible_from<std::string, Args...>)
      PrimitiveTextBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>
  &Text(Args&&... args) {
    text_builder_.End();
    auto new_text = this->Data().add_texts();
    new_text->set_text(std::forward<Args>(args)...);
    return text_builder_.Start(*new_text);
  }

 protected:
  friend class Builder;

  void End() {
    polygon_builder_.End();
    polyline_builder_.End();
    text_builder_.End();
    circle_builder_.End();
    point_builder_.End();
    image_builder_.End();

    BaseT::End();
  }
  PrimitivePolygonBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>
      polygon_builder_;
  PrimitivePolylineBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>
      polyline_builder_;
  PrimitiveTextBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>
      text_builder_;
  PrimitiveCircleBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>
      circle_builder_;
  PrimitivePointBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>
      point_builder_;
  PrimitiveImageBuilder<PrimitiveBuilder<BaseBuilder>, BaseBuilder>
      image_builder_;

  // TODO DELETE
  // PrimitiveState* data_;
};

}  // namespace xviz
