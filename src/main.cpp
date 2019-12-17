/*
 * File: main.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Saturday, 7th December 2019 1:35:32 pm
 */


#include <iostream>
#include "proto/primitives.pb.h"
#include "builder/pose.h"

using namespace xviz;


int main() {
  Circle circle;
  circle.add_center(0);
  auto builder = std::make_shared<XVIZPoseBuilder>(Metadata());
  builder->Stream("123")
    ->Timestamp(123.0)
    ->MapOrigin(1, 2, 3)
    ->Stream("234")
    ->Timestamp(123123123)
    ->Position(1, 2, 3);
  auto res = builder->GetData();
  for (const auto& pa : *res) {
    std::cout << pa.first << std::endl;
    std::cout << pa.second.SerializeAsString() << std::endl;
  }
  return 0;
}