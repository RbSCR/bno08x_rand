// Copyright 2021 PAL Robotics SL.
//
// Copyright 2026 RbSCR
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * Authors original imu semantic component: Subhas Das, Denis Stogl, Victor Lopez
 *
 * Author: RbSCR
 * Adapted from https://github.com/ros-controls/ros2_control/blob/jazzy/controller_interface/test/test_imu_sensor.hpp
 */

#ifndef SEMANTIC_COMPONENT__TEST_IMU_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_
#define SEMANTIC_COMPONENT__TEST_IMU_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_

#include <memory>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "imu_sensor_covariance_broadcaster/imu_sensor_covariance_semantic_component.hpp"

// implementing and friending so we can access member variables
class TestableIMUSensor : public semantic_components::IMUSensorCovariance
{
  FRIEND_TEST(IMUSensorCovarianceTest, validate_all);

public:
  // Use generation of interface names
  explicit TestableIMUSensor(const std::string & name) : IMUSensorCovariance(name) {}

  virtual ~TestableIMUSensor() = default;
};

class IMUSensorCovarianceTest : public ::testing::Test
{
public:
  void SetUp() {
    full_interface_names_.reserve(size_);
    for (auto index = 0u; index < size_; ++index) {
      full_interface_names_.emplace_back(sensor_name_ + "/" + imu_interface_names_[index]);
    }
  }

  void TearDown();

protected:
  const size_t size_ = 13;
  const std::string sensor_name_ = "test_IMU";
  std::array<double, 4> orientation_values_ = {{1.1, 1.2, 1.3, 1.4}};
  std::array<double, 3> angular_velocity_values_ = {{2.1, 2.2, 2.3}};
  std::array<double, 3> linear_acceleration_values_ = {{3.1, 3.2, 3.3}};
  std::array<double, 9> orientation_covariance_values_{
    {0.11, 0.0, 0.0, 0.0, 0.11, 0.0, 0.0, 0.0, 0.11}};
  std::array<double, 9> angular_velocity_covariance_values_{
    {0.21, 0.0, 0.0, 0.0, 0.21, 0.0, 0.0, 0.0, 0.21}};
  std::array<double, 9> linear_acceleration_covariance_values_{
    {0.31, 0.0, 0.0, 0.0, 0.31, 0.0, 0.0, 0.0, 0.31}};
  std::unique_ptr<TestableIMUSensor> imu_sensor_;

  std::vector<std::string> full_interface_names_;
  const std::vector<std::string> imu_interface_names_ = {
    "orientation.x",
    "orientation.y",
    "orientation.z",
    "orientation.w",
    "angular_velocity.x",
    "angular_velocity.y",
    "angular_velocity.z",
    "linear_acceleration.x",
    "linear_acceleration.y",
    "linear_acceleration.z",
    "orientation_covariance",
    "angular_velocity_covariance",
    "linear_acceleration_covariance"};
};

#endif  // SEMANTIC_COMPONENT__TEST_IMU_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_
