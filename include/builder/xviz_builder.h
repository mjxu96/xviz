/*
 * File: xviz_builder.h
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Tuesday, 17th December 2019 2:24:08 am
 */

#ifndef XVIZ_XVIZ_BUILDER_H_
#define XVIZ_XVIZ_BUILDER_H_

#include "base_builder.h"
#include "pose.h"
#include "message.h"

#include <optional>

namespace xviz {

class XVIZBuilder {
public:
  // TODO add "disabled stream var"
  XVIZBuilder(const Metadata& metadata);
  std::shared_ptr<XVIZPoseBuilder> Pose(const std::string& stream_id);
  XVIZFrame GetData();
  XVIZMessage GetMessage();

private:
  void Reset();


  std::shared_ptr<XVIZPoseBuilder> pose_{nullptr};
  // TODO do I need optional?
  std::optional<Metadata> metadata_{std::nullopt};
};
  
} // namespace xviz



#endif