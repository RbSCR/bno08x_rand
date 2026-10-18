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
 * Authors original Authors original test_imu_sensor_broadcaster: Subhas Das, Denis Stogl, Victor Lopez
 *
 * Author: RbSCR
 * Adapted from https://github.com/ros-controls/ros2_controllers/tree/kilted/imu_sensor_broadcaster
 */

#ifndef TEST_IMU_SENSOR_COVARIANCE_BROADCASTER_HPP_
#define TEST_IMU_SENSOR_COVARIANCE_BROADCASTER_HPP_

#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <vector>

#include "controller_interface/test_utils.hpp"
#include "imu_sensor_covariance_broadcaster/imu_sensor_covariance_broadcaster.hpp"


using controller_interface::activate_succeeds;
using controller_interface::configure_succeeds;
using controller_interface::deactivate_succeeds;

// subclassing and friending so we can access member variables
class FriendIMUSensorCovarianceBroadcaster
: public imu_sensor_covariance_broadcaster::IMUSensorCovarianceBroadcaster
{
  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, SensorNameParameterNotSet);
  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, InterfaceNamesParameterNotSet);
  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, FrameIdParameterNotSet);
  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, SensorNameParameterIsEmpty);
  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, InterfaceNameParameterIsEmpty);

  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, ActivateSuccess);
  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, UpdateTest);
  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, SensorStatePublishTest);
  FRIEND_TEST(IMUSensorCovarianceBroadcasterTest, SensorStatePublishTest_with_rotation_offset);
};

class IMUSensorCovarianceBroadcasterTest : public ::testing::Test
{
public:
  static void SetUpTestCase();
  static void TearDownTestCase();

  void SetUp();
  void TearDown();

  void SetUpIMUCovarianceBroadcaster(const std::vector<rclcpp::Parameter> & parameters = {});

protected:
  const std::string sensor_name_ = "imu_sensor";
  const std::string frame_id_ = "imu_sensor_frame";
  const std::array<double, 10> sensor_values_ = {
    {0.1826, 0.3651, 0.5477, 0.7303, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10}};
  const std::array<double, 9> orientation_covariance_values_ = {
    {0.15, 0.0, 0.0, 0.16, 0.0, 0.0, 0.0, 0.0, 0.17}};
  const std::array<double, 9> angular_velocity_covariance_values_ = {
    {0.25, 0.0, 0.0, 0.26, 0.0, 0.0, 0.0, 0.0, 0.27}};
  const std::array<double, 9> linear_acceleration_covariance_values_ = {
    {0.35, 0.0, 0.0, 0.36, 0.0, 0.0, 0.0, 0.0, 0.37}};

  hardware_interface::StateInterface::SharedPtr imu_orientation_x_;
  hardware_interface::StateInterface::SharedPtr imu_orientation_y_;
  hardware_interface::StateInterface::SharedPtr imu_orientation_z_;
  hardware_interface::StateInterface::SharedPtr imu_orientation_w_;
  hardware_interface::StateInterface::SharedPtr imu_angular_velocity_x_;
  hardware_interface::StateInterface::SharedPtr imu_angular_velocity_y_;
  hardware_interface::StateInterface::SharedPtr imu_angular_velocity_z_;
  hardware_interface::StateInterface::SharedPtr imu_linear_acceleration_x_;
  hardware_interface::StateInterface::SharedPtr imu_linear_acceleration_y_;
  hardware_interface::StateInterface::SharedPtr imu_linear_acceleration_z_;
  hardware_interface::StateInterface::SharedPtr imu_orientation_covariance_xx_;
  hardware_interface::StateInterface::SharedPtr imu_orientation_covariance_yy_;
  hardware_interface::StateInterface::SharedPtr imu_orientation_covariance_zz_;
  hardware_interface::StateInterface::SharedPtr imu_angular_velocity_covariance_xx_;
  hardware_interface::StateInterface::SharedPtr imu_angular_velocity_covariance_yy_;
  hardware_interface::StateInterface::SharedPtr imu_angular_velocity_covariance_zz_;
  hardware_interface::StateInterface::SharedPtr imu_linear_acceleration_covariance_xx_;
  hardware_interface::StateInterface::SharedPtr imu_linear_acceleration_covariance_yy_;
  hardware_interface::StateInterface::SharedPtr imu_linear_acceleration_covariance_zz_;

  std::unique_ptr<FriendIMUSensorCovarianceBroadcaster> imu_broadcaster_;

  void subscribe_and_get_message(sensor_msgs::msg::Imu & imu_msg);
};

#endif  // TEST_IMU_SENSOR_COVARIANCE_BROADCASTER_HPP_
