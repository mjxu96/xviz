/*
 * File: defines.h
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Saturday, 7th December 2019 2:46:43 pm
 */


#ifndef MINJUN_DEFINES_H_
#define MINJUN_DEFINES_H_

#include "proto/session.pb.h"
#include "proto/style.pb.h"

namespace minjun {

using AnnotationType = xviz::StreamMetadata::AnnotationType;
using Category = xviz::StreamMetadata::Category;
using CoordinateType = xviz::StreamMetadata::CoordinateType;
  
} // namespace minjun


#endif