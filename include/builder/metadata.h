/*
 * File: metadata.h
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Tuesday, 24th December 2019 8:06:16 pm
 */

#ifndef XVIZ_METADATA_BUILDER_H_
#define XVIZ_METADATA_BUILDER_H_

#include "utils/macrologger.h"
#include "utils/definitions.h"
#include "utils/utils.h"

#include "message.h"
#include "proto/session.pb.h"

#include <memory>

namespace xviz {

class XVIZMetadataBuilder {
public:
  XVIZMetadataBuilder();
  std::shared_ptr<Metadata> GetData();
  XVIZMessage GetMessage();

  XVIZMetadataBuilder& Stream(const std::string& stream_id);
  // XVIZMetadataBuilder& Stream(std::string&& stream_id);

  XVIZMetadataBuilder& Category(Category category);
  XVIZMetadataBuilder& Type(Primitive primitive_type);
  XVIZMetadataBuilder& Type(ScalarType scalar_type);

  // TODO determine this type
  XVIZMetadataBuilder& Coordinate(CoordinateType coordinate_type);

  XVIZMetadataBuilder& StreamStyle(const std::string& style_str);
private:
  void Flush();
  void Reset();

  std::shared_ptr<Metadata> data_{nullptr};
  std::string stream_id_{};
  StreamMetadata temp_stream_{};
  uint32_t type_ = -1;
  // TODO TMP TYPE
};
  
} // namespace xviz



#endif