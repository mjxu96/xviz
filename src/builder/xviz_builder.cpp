/*
 * File: xviz_builder.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Tuesday, 17th December 2019 2:24:17 am
 */

#include "builder/xviz_builder.h"

using namespace xviz;

std::string primary_pose_stream = "/vehicle_pose";

template<typename K, typename V>
void ConvertFromStdMapToProtoBufMap(google::protobuf::Map<K, V>* map, std::unordered_map<K, V>& m) {
  map->clear();
  for (const auto& [k, v] : m) {
    (*map)[k] = v;
  }
}

XVIZBuilder::XVIZBuilder(std::shared_ptr<Metadata> metadata) :
  metadata_(metadata) {

  pose_ = std::make_shared<XVIZPoseBuilder>(metadata_);
}

std::shared_ptr<XVIZPoseBuilder> XVIZBuilder::Pose(const std::string& stream_id) {
  return pose_->Stream(stream_id);
}

XVIZFrame XVIZBuilder::GetData() {
  auto data = std::make_shared<StreamSet>();
  auto poses = pose_->GetData();
  if (poses->find(primary_pose_stream) == poses->end()) {
    LOG_ERROR("every frame requires a %s message", primary_pose_stream.c_str());
  }
  data->set_timestamp((*poses)[primary_pose_stream].timestamp());
  auto map = data->mutable_poses();
  ConvertFromStdMapToProtoBufMap<std::string, xviz::Pose>(map, *poses);

  return XVIZFrame(data);
}

XVIZMessage XVIZBuilder::GetMessage() {
  auto state_update = std::make_shared<StateUpdate>();
  state_update->set_update_type(StateUpdate::UpdateType::StateUpdate_UpdateType_SNAPSHOT);
  auto new_update = state_update->add_updates();
  auto frame = GetData();
  *new_update = *(frame.Data());
  return XVIZMessage(state_update);
}