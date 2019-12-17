/*
 * File: pose.h
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Saturday, 7th December 2019 2:21:32 pm
 */

#ifndef MINJUN_POSE_BUILDER_H_
#define MINJUN_POSE_BUILDER_H_

#include "builder/base_builder.h"
#include "proto/core.pb.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace minjun {

class XVIZPoseBuilder : public XVIZBaseBuilder, public std::enable_shared_from_this<XVIZPoseBuilder> {
public:

  XVIZPoseBuilder(const xviz::Metadata& metadata);
  std::shared_ptr<XVIZPoseBuilder> MapOrigin(double longitude, double latitude, double altitude);
  std::shared_ptr<XVIZPoseBuilder> Position(double x, double y, double z);
  std::shared_ptr<XVIZPoseBuilder> Orientation(double roll, double pitch, double yaw);
  std::shared_ptr<XVIZPoseBuilder> Timestamp(double timestamp);
  std::shared_ptr<XVIZPoseBuilder> Stream(const std::string& stream_id);

  std::shared_ptr<std::unordered_map<std::string, xviz::Pose>> GetData();

private:
  std::shared_ptr<std::unordered_map<std::string, xviz::Pose>> poses_{nullptr};
  xviz::Pose temp_poses_{};

  void Flush() override;
};
  
} // namespace minjun 

#endif