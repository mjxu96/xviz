/*
 * File: test_utils.h
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Friday, 14th February 2020 11:03:25 am
 */


#ifndef XVIZ_TEST_UTILS_H_
#define XVIZ_TEST_UTILS_H_

#include <gtest/gtest.h>
#include "utils/json.hpp"
#include "builder/metadata.h"
#include "builder/xviz_builder.h"

namespace xviz {
namespace test {

::testing::AssertionResult IsSameJson(const nlohmann::json& expected_json, const nlohmann::json& given_json);

::testing::AssertionResult IsDifferentJson(const nlohmann::json& expected_json, const nlohmann::json& given_json);

std::unordered_map<std::string, xviz::XVIZUIBuilder> GetTestUIBuilders();

XVIZMetadataBuilder GetTestMetadataBuilder();

nlohmann::json GetTestMetadataExpectedJson();

xviz::XVIZBuilder GetTestUpdateBuilder(const std::shared_ptr<xviz::Metadata>& metadata);

} // namespace test
} // namespace xviz


#endif