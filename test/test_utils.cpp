
/*
 * File: test_utils.cpp
 * Author: Minjun Xu (mjxu96@gmail.com)
 * File Created: Friday, 14th February 2020 11:04:08 am
 */

#include "test_utils.h"

::testing::AssertionResult xviz::test::IsSameJson(const nlohmann::json& expected_json, const nlohmann::json& given_json) {
  if (expected_json == given_json) {
    return ::testing::AssertionSuccess();
  }
  return ::testing::AssertionFailure() << "\nexpected: \n" <<
    expected_json << "\nbut get:\n" << given_json << "\ndifference: \n" <<
    nlohmann::json::diff(given_json, expected_json) << std::endl;
}

::testing::AssertionResult xviz::test::IsDifferentJson(const nlohmann::json& expected_json, const nlohmann::json& given_json) {
  if (expected_json != given_json) {
    return ::testing::AssertionSuccess();
  }
  return ::testing::AssertionFailure() << "\nexpected different json " <<
    "but get same json: \n" << expected_json << std::endl;
}