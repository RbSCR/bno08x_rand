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
 * https://github.com/ros-controls/ros2_control/blob/jazzy/controller_interface/test/test_imu_sensor.cpp
 */

#include "test_imu_sensor_covariance_semantic_component.hpp"

#include <memory>
#include <string>
#include <vector>

void IMUSensorCovarianceTest::TearDown() { imu_sensor_.reset(nullptr); }

TEST_F(IMUSensorCovarianceTest, validate_all) {
  // create the IMU sensor
  imu_sensor_ = std::make_unique<TestableIMUSensor>(sensor_name_);

  // validate the component name
  ASSERT_EQ(imu_sensor_->name_, sensor_name_);

  // validate the space reserved for interface_names_ and state_interfaces_
  // Note : Using capacity() for state_interfaces_ as no such interfaces are defined yet
  ASSERT_EQ(imu_sensor_->interface_names_.size(), size_);
  ASSERT_EQ(imu_sensor_->state_interfaces_.capacity(), size_);

  // validate the default interface_names_
  ASSERT_TRUE(std::equal(
    imu_sensor_->interface_names_.begin(), imu_sensor_->interface_names_.end(),
    full_interface_names_.begin(), full_interface_names_.end()));

  // get the interface names
  std::vector<std::string> interface_names = imu_sensor_->get_state_interface_names();

  // assign values to orientation
  auto orientation_x = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[0], &orientation_values_[0]);
  auto orientation_y = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[1], &orientation_values_[1]);
  auto orientation_z = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[2], &orientation_values_[2]);
  auto orientation_w = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[3], &orientation_values_[3]);

  // assign values to angular velocity
  auto angular_velocity_x = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[4], &angular_velocity_values_[0]);
  auto angular_velocity_y = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[5], &angular_velocity_values_[1]);
  auto angular_velocity_z = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[6], &angular_velocity_values_[2]);

  // assign values to linear acceleration
  auto linear_acceleration_x = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[7], &linear_acceleration_values_[0]);
  auto linear_acceleration_y = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[8], &linear_acceleration_values_[1]);
  auto linear_acceleration_z = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[9], &linear_acceleration_values_[2]);

  // assign values to orientation covariance
  auto orientation_covariance_xx = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[10], &orientation_covariance_values_[0]);
  auto orientation_covariance_yy = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[11], &orientation_covariance_values_[4]);
  auto orientation_covariance_zz = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[12], &orientation_covariance_values_[8]);

  // assign values to angular velocity covariance
  auto angular_velocity_covariance_xx = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[13], &angular_velocity_covariance_values_[0]);
  auto angular_velocity_covariance_yy = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[14], &angular_velocity_covariance_values_[4]);
  auto angular_velocity_covariance_zz = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[15], &angular_velocity_covariance_values_[8]);

  // assign values to linear acceleration covariance
  auto linear_acceleration_covariance_xx = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[16], &linear_acceleration_covariance_values_[0]);
  auto linear_acceleration_covariance_yy = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[17], &linear_acceleration_covariance_values_[4]);
  auto linear_acceleration_covariance_zz = std::make_shared<hardware_interface::StateInterface>(
    sensor_name_, imu_interface_names_[18], &linear_acceleration_covariance_values_[8]);

  // create local state interface vector
  std::vector<hardware_interface::LoanedStateInterface> temp_state_interfaces;
  temp_state_interfaces.reserve(19);

  // insert the interfaces in jumbled sequence
  temp_state_interfaces.emplace_back(orientation_covariance_yy);
  temp_state_interfaces.emplace_back(angular_velocity_y);
  temp_state_interfaces.emplace_back(orientation_y);
  temp_state_interfaces.emplace_back(linear_acceleration_covariance_yy);
  temp_state_interfaces.emplace_back(linear_acceleration_y);
  temp_state_interfaces.emplace_back(orientation_x);
  temp_state_interfaces.emplace_back(angular_velocity_covariance_zz);
  temp_state_interfaces.emplace_back(linear_acceleration_covariance_xx);
  temp_state_interfaces.emplace_back(linear_acceleration_z);
  temp_state_interfaces.emplace_back(angular_velocity_z);
  temp_state_interfaces.emplace_back(orientation_z);
  temp_state_interfaces.emplace_back(angular_velocity_covariance_yy);
  temp_state_interfaces.emplace_back(orientation_covariance_xx);
  temp_state_interfaces.emplace_back(angular_velocity_covariance_xx);
  temp_state_interfaces.emplace_back(orientation_w);
  temp_state_interfaces.emplace_back(linear_acceleration_covariance_zz);
  temp_state_interfaces.emplace_back(angular_velocity_x);
  temp_state_interfaces.emplace_back(linear_acceleration_x);
  temp_state_interfaces.emplace_back(orientation_covariance_zz);

  // now call the function to make them in order like interface_names
  imu_sensor_->assign_loaned_state_interfaces(temp_state_interfaces);

  // validate the count of state_interfaces_
  ASSERT_EQ(imu_sensor_->state_interfaces_.size(), size_);

  // validate the orientation values
  std::array<double, 4> temp_orientation_values = imu_sensor_->get_orientation();
  ASSERT_EQ(temp_orientation_values, orientation_values_);

  // validate the angular_velocity values
  std::array<double, 3> temp_angular_velocity_values = imu_sensor_->get_angular_velocity();
  ASSERT_EQ(temp_angular_velocity_values, angular_velocity_values_);

  // validate the linear_acceleration values
  std::array<double, 3> temp_linear_acceleration_values = imu_sensor_->get_linear_acceleration();
  ASSERT_EQ(temp_linear_acceleration_values, linear_acceleration_values_);

  // validate the orientation covariance values
  std::array<double, 9> temp_orientation_covariance_values =
    imu_sensor_->get_orientation_covariance();
  ASSERT_EQ(temp_orientation_covariance_values, orientation_covariance_values_);

  // validate the angular_velocity covariance values
  std::array<double, 9> temp_angular_velocity_covariance_values =
    imu_sensor_->get_angular_velocity_covariance();
  ASSERT_EQ(temp_angular_velocity_covariance_values, angular_velocity_covariance_values_);

  // validate the linear_acceleration covariance values
  std::array<double, 9> temp_linear_acceleration_covariance_values =
    imu_sensor_->get_linear_acceleration_covariance();
  ASSERT_EQ(temp_linear_acceleration_covariance_values, linear_acceleration_covariance_values_);

  // validate get_values_as_message
  sensor_msgs::msg::Imu temp_message;
  ASSERT_TRUE(imu_sensor_->get_values_as_message(temp_message));
  ASSERT_EQ(temp_message.orientation.x, orientation_values_[0]);
  ASSERT_EQ(temp_message.orientation.y, orientation_values_[1]);
  ASSERT_EQ(temp_message.orientation.z, orientation_values_[2]);
  ASSERT_EQ(temp_message.orientation.w, orientation_values_[3]);
  ASSERT_EQ(temp_message.angular_velocity.x, angular_velocity_values_[0]);
  ASSERT_EQ(temp_message.angular_velocity.y, angular_velocity_values_[1]);
  ASSERT_EQ(temp_message.angular_velocity.z, angular_velocity_values_[2]);
  ASSERT_EQ(temp_message.linear_acceleration.x, linear_acceleration_values_[0]);
  ASSERT_EQ(temp_message.linear_acceleration.y, linear_acceleration_values_[1]);
  ASSERT_EQ(temp_message.linear_acceleration.z, linear_acceleration_values_[2]);

  ASSERT_EQ(temp_message.orientation_covariance[0], orientation_covariance_values_[0]);
  ASSERT_EQ(temp_message.orientation_covariance[4], orientation_covariance_values_[4]);
  ASSERT_EQ(temp_message.orientation_covariance[8], orientation_covariance_values_[8]);

  ASSERT_EQ(temp_message.angular_velocity_covariance[0], angular_velocity_covariance_values_[0]);
  ASSERT_EQ(temp_message.angular_velocity_covariance[4], angular_velocity_covariance_values_[4]);
  ASSERT_EQ(temp_message.angular_velocity_covariance[8], angular_velocity_covariance_values_[8]);

  ASSERT_EQ(
    temp_message.linear_acceleration_covariance[0], linear_acceleration_covariance_values_[0]);
  ASSERT_EQ(
    temp_message.linear_acceleration_covariance[4], linear_acceleration_covariance_values_[4]);
  ASSERT_EQ(
    temp_message.linear_acceleration_covariance[8], linear_acceleration_covariance_values_[8]);

  // release the state_interfaces_
  imu_sensor_->release_interfaces();

  // validate the count of state_interfaces_
  ASSERT_EQ(imu_sensor_->state_interfaces_.size(), 0u);
}
