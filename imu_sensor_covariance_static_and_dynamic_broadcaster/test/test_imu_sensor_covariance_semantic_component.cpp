// Copyright 2021 ros2_control development team
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

#include <algorithm>
#include <array>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "hardware_interface/handle.hpp"
#include "hardware_interface/loaned_state_interface.hpp"
#include "imu_sensor_covariance_static_and_dynamic_broadcaster/imu_sensor_covariance_semantic_component.hpp"
#include "sensor_msgs/msg/imu.hpp"

struct IMUSensorCovarianceWithoutTest : public testing::Test {
  IMUSensorCovarianceWithoutTest() {
    std::transform(
      imu_interface_names.cbegin(), imu_interface_names.cend(),
      std::back_inserter(full_interface_names),
      [this](const auto & interface_name) { return imu_sensor_name + '/' + interface_name; });
    state_interface.emplace_back(orientation_x);
    state_interface.emplace_back(orientation_y);
    state_interface.emplace_back(orientation_z);
    state_interface.emplace_back(orientation_w);
    state_interface.emplace_back(angular_velocity_x);
    state_interface.emplace_back(angular_velocity_y);
    state_interface.emplace_back(angular_velocity_z);
    state_interface.emplace_back(linear_acceleration_x);
    state_interface.emplace_back(linear_acceleration_y);
    state_interface.emplace_back(linear_acceleration_z);
  }

  const std::string imu_sensor_name{"imu_sensor"};
  const std::array<std::string, 10> imu_interface_names{
    {"orientation.x", "orientation.y", "orientation.z", "orientation.w", "angular_velocity.x",
     "angular_velocity.y", "angular_velocity.z", "linear_acceleration.x", "linear_acceleration.y",
     "linear_acceleration.z"}};
  const size_t interfaces_names_size = 10;
  std::array<double, 10> imu_states{};
  my_semantic_component::IMUSensorCovariance<
    my_semantic_component::IMUSensorCovarianceOption::WithoutCovariance>
    sut{imu_sensor_name};
  std::vector<std::string> full_interface_names;

  hardware_interface::StateInterface::SharedPtr orientation_x =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(0), &imu_states.at(0));
  hardware_interface::StateInterface::SharedPtr orientation_y =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(1), &imu_states.at(1));
  hardware_interface::StateInterface::SharedPtr orientation_z =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(2), &imu_states.at(2));
  hardware_interface::StateInterface::SharedPtr orientation_w =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(3), &imu_states.at(3));
  hardware_interface::StateInterface::SharedPtr angular_velocity_x =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(4), &imu_states.at(4));
  hardware_interface::StateInterface::SharedPtr angular_velocity_y =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(5), &imu_states.at(5));
  hardware_interface::StateInterface::SharedPtr angular_velocity_z =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(6), &imu_states.at(6));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_x =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(7), &imu_states.at(7));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_y =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(8), &imu_states.at(8));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_z =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(9), &imu_states.at(9));

  std::vector<hardware_interface::LoanedStateInterface> state_interface;
};

TEST_F(IMUSensorCovarianceWithoutTest, interfaces_name_should_be_expected_size) {
  const auto sensor_interfaces_name = sut.get_state_interface_names();

  ASSERT_EQ(sensor_interfaces_name.size(), interfaces_names_size);
}

TEST_F(
  IMUSensorCovarianceWithoutTest,
  interfaces_name_should_contain_orientation_angular_linear_and_sensor_name_prefix) {
  const auto sensor_interfaces_name = sut.get_state_interface_names();
  EXPECT_THAT(sensor_interfaces_name, testing::ElementsAreArray(full_interface_names));

  ASSERT_EQ(sensor_interfaces_name.size(), interfaces_names_size);
}

TEST_F(
  IMUSensorCovarianceWithoutTest,
  orientation_angular_linear_values_should_be_equal_to_corresponding_values_in_state_interface) {
  EXPECT_TRUE(sut.assign_loaned_state_interfaces(state_interface));

  EXPECT_DOUBLE_EQ(imu_states.at(0), sut.get_orientation_x());
  EXPECT_DOUBLE_EQ(imu_states.at(1), sut.get_orientation_y());
  EXPECT_DOUBLE_EQ(imu_states.at(2), sut.get_orientation_z());
  EXPECT_DOUBLE_EQ(imu_states.at(3), sut.get_orientation_w());

  EXPECT_DOUBLE_EQ(imu_states.at(4), sut.get_angular_velocity_x());
  EXPECT_DOUBLE_EQ(imu_states.at(5), sut.get_angular_velocity_y());
  EXPECT_DOUBLE_EQ(imu_states.at(6), sut.get_angular_velocity_z());

  EXPECT_DOUBLE_EQ(imu_states.at(7), sut.get_linear_acceleration_x());
  EXPECT_DOUBLE_EQ(imu_states.at(8), sut.get_linear_acceleration_y());
  EXPECT_DOUBLE_EQ(imu_states.at(9), sut.get_linear_acceleration_z());

  imu_states.at(0) = 1.1;  // orientation
  imu_states.at(1) = 1.2;
  imu_states.at(2) = 1.3;
  imu_states.at(3) = 1.4;

  imu_states.at(4) = 2.1;  // angular velocity
  imu_states.at(5) = 2.2;
  imu_states.at(6) = 2.3;

  imu_states.at(7) = 3.1;  // linear acceleration
  imu_states.at(8) = 3.2;
  imu_states.at(9) = 3.3;

  EXPECT_DOUBLE_EQ(imu_states.at(0), sut.get_orientation_x());
  EXPECT_DOUBLE_EQ(imu_states.at(1), sut.get_orientation_y());
  EXPECT_DOUBLE_EQ(imu_states.at(2), sut.get_orientation_z());
  EXPECT_DOUBLE_EQ(imu_states.at(3), sut.get_orientation_w());

  EXPECT_DOUBLE_EQ(imu_states.at(4), sut.get_angular_velocity_x());
  EXPECT_DOUBLE_EQ(imu_states.at(5), sut.get_angular_velocity_y());
  EXPECT_DOUBLE_EQ(imu_states.at(6), sut.get_angular_velocity_z());

  EXPECT_DOUBLE_EQ(imu_states.at(7), sut.get_linear_acceleration_x());
  EXPECT_DOUBLE_EQ(imu_states.at(8), sut.get_linear_acceleration_y());
  EXPECT_DOUBLE_EQ(imu_states.at(9), sut.get_linear_acceleration_z());
}

TEST_F(IMUSensorCovarianceWithoutTest, should_fill_imu_msg_with_value_from_state_interface) {
  EXPECT_TRUE(sut.assign_loaned_state_interfaces(state_interface));

  sensor_msgs::msg::Imu message;
  EXPECT_TRUE(sut.get_values_as_message(message));

  EXPECT_DOUBLE_EQ(imu_states.at(0), message.orientation.x);
  EXPECT_DOUBLE_EQ(imu_states.at(1), message.orientation.y);
  EXPECT_DOUBLE_EQ(imu_states.at(2), message.orientation.z);
  EXPECT_DOUBLE_EQ(imu_states.at(3), message.orientation.w);

  EXPECT_DOUBLE_EQ(imu_states.at(4), message.angular_velocity.x);
  EXPECT_DOUBLE_EQ(imu_states.at(5), message.angular_velocity.y);
  EXPECT_DOUBLE_EQ(imu_states.at(6), message.angular_velocity.z);

  EXPECT_DOUBLE_EQ(imu_states.at(7), message.linear_acceleration.x);
  EXPECT_DOUBLE_EQ(imu_states.at(8), message.linear_acceleration.y);
  EXPECT_DOUBLE_EQ(imu_states.at(9), message.linear_acceleration.z);

  imu_states.at(0) = 1.01;  // orientation
  imu_states.at(1) = 1.02;
  imu_states.at(2) = 1.03;
  imu_states.at(3) = 1.04;

  imu_states.at(4) = 2.01;  // angular velocity
  imu_states.at(5) = 2.02;
  imu_states.at(6) = 2.03;

  imu_states.at(7) = 3.01;  // linear_acceleration
  imu_states.at(8) = 3.02;
  imu_states.at(9) = 3.03;

  EXPECT_TRUE(sut.get_values_as_message(message));
  EXPECT_DOUBLE_EQ(imu_states.at(0), message.orientation.x);
  EXPECT_DOUBLE_EQ(imu_states.at(1), message.orientation.y);
  EXPECT_DOUBLE_EQ(imu_states.at(2), message.orientation.z);
  EXPECT_DOUBLE_EQ(imu_states.at(3), message.orientation.w);

  EXPECT_DOUBLE_EQ(imu_states.at(4), message.angular_velocity.x);
  EXPECT_DOUBLE_EQ(imu_states.at(5), message.angular_velocity.y);
  EXPECT_DOUBLE_EQ(imu_states.at(6), message.angular_velocity.z);

  EXPECT_DOUBLE_EQ(imu_states.at(7), message.linear_acceleration.x);
  EXPECT_DOUBLE_EQ(imu_states.at(8), message.linear_acceleration.y);
  EXPECT_DOUBLE_EQ(imu_states.at(9), message.linear_acceleration.z);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

struct IMUSensorCovarianceWithTest : public testing::Test {
  IMUSensorCovarianceWithTest() {
    std::transform(
      imu_interface_names.cbegin(), imu_interface_names.cend(),
      std::back_inserter(full_interface_names),
      [this](const auto & interface_name) { return imu_sensor_name + '/' + interface_name; });
    state_interface.emplace_back(orientation_x);
    state_interface.emplace_back(orientation_y);
    state_interface.emplace_back(orientation_z);
    state_interface.emplace_back(orientation_w);
    state_interface.emplace_back(angular_velocity_x);
    state_interface.emplace_back(angular_velocity_y);
    state_interface.emplace_back(angular_velocity_z);
    state_interface.emplace_back(linear_acceleration_x);
    state_interface.emplace_back(linear_acceleration_y);
    state_interface.emplace_back(linear_acceleration_z);
    state_interface.emplace_back(orientation_covariance_xx);
    state_interface.emplace_back(orientation_covariance_yy);
    state_interface.emplace_back(orientation_covariance_zz);
    state_interface.emplace_back(angular_velocity_covariance_xx);
    state_interface.emplace_back(angular_velocity_covariance_yy);
    state_interface.emplace_back(angular_velocity_covariance_zz);
    state_interface.emplace_back(linear_acceleration_covariance_xx);
    state_interface.emplace_back(linear_acceleration_covariance_yy);
    state_interface.emplace_back(linear_acceleration_covariance_zz);
  }

  const std::string imu_sensor_name{"imu_sensor"};
  const std::array<std::string, 19> imu_interface_names{
    {"orientation.x", "orientation.y", "orientation.z", "orientation.w", "angular_velocity.x",
     "angular_velocity.y", "angular_velocity.z", "linear_acceleration.x", "linear_acceleration.y",
     "linear_acceleration.z", "orientation_covariance_xx", "orientation_covariance_yy",
     "orientation_covariance_zz", "angular_velocity_covariance_xx",
     "angular_velocity_covariance_yy", "angular_velocity_covariance_zz",
     "linear_acceleration_covariance_xx", "linear_acceleration_covariance_yy",
     "linear_acceleration_covariance_zz"}};
  const size_t interfaces_names_size = 19;
  std::array<double, 19> imu_states{};
  my_semantic_component::IMUSensorCovariance<
    my_semantic_component::IMUSensorCovarianceOption::WithCovariance>
    sut{imu_sensor_name};
  std::vector<std::string> full_interface_names;

  hardware_interface::StateInterface::SharedPtr orientation_x =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(0), &imu_states.at(0));
  hardware_interface::StateInterface::SharedPtr orientation_y =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(1), &imu_states.at(1));
  hardware_interface::StateInterface::SharedPtr orientation_z =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(2), &imu_states.at(2));
  hardware_interface::StateInterface::SharedPtr orientation_w =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(3), &imu_states.at(3));
  hardware_interface::StateInterface::SharedPtr angular_velocity_x =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(4), &imu_states.at(4));
  hardware_interface::StateInterface::SharedPtr angular_velocity_y =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(5), &imu_states.at(5));
  hardware_interface::StateInterface::SharedPtr angular_velocity_z =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(6), &imu_states.at(6));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_x =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(7), &imu_states.at(7));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_y =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(8), &imu_states.at(8));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_z =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(9), &imu_states.at(9));
  hardware_interface::StateInterface::SharedPtr orientation_covariance_xx =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(10), &imu_states.at(10));
  hardware_interface::StateInterface::SharedPtr orientation_covariance_yy =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(11), &imu_states.at(11));
  hardware_interface::StateInterface::SharedPtr orientation_covariance_zz =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(12), &imu_states.at(12));
  hardware_interface::StateInterface::SharedPtr angular_velocity_covariance_xx =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(13), &imu_states.at(13));
  hardware_interface::StateInterface::SharedPtr angular_velocity_covariance_yy =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(14), &imu_states.at(14));
  hardware_interface::StateInterface::SharedPtr angular_velocity_covariance_zz =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(15), &imu_states.at(15));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_covariance_xx =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(16), &imu_states.at(16));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_covariance_yy =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(17), &imu_states.at(17));
  hardware_interface::StateInterface::SharedPtr linear_acceleration_covariance_zz =
    std::make_shared<hardware_interface::StateInterface>(
      imu_sensor_name, imu_interface_names.at(18), &imu_states.at(18));

  std::vector<hardware_interface::LoanedStateInterface> state_interface;
};

TEST_F(IMUSensorCovarianceWithTest, interfaces_name_should_be_expected_size) {
  const auto sensor_interfaces_name = sut.get_state_interface_names();

  ASSERT_EQ(sensor_interfaces_name.size(), interfaces_names_size);
}

TEST_F(
  IMUSensorCovarianceWithTest,
  interfaces_name_should_contain_orientation_angular_linear_and_sensor_name_prefix) {
  const auto sensor_interfaces_name = sut.get_state_interface_names();

  EXPECT_EQ(sensor_interfaces_name.size(), full_interface_names.size());
  EXPECT_THAT(sensor_interfaces_name, testing::ElementsAreArray(full_interface_names));
}

TEST_F(
  IMUSensorCovarianceWithTest,
  orientation_angular_linear_values_should_be_equal_to_corresponding_values_in_state_interface) {
  EXPECT_TRUE(sut.assign_loaned_state_interfaces(state_interface));

  EXPECT_DOUBLE_EQ(imu_states.at(0), sut.get_orientation_x());
  EXPECT_DOUBLE_EQ(imu_states.at(1), sut.get_orientation_y());
  EXPECT_DOUBLE_EQ(imu_states.at(2), sut.get_orientation_z());
  EXPECT_DOUBLE_EQ(imu_states.at(3), sut.get_orientation_w());

  EXPECT_DOUBLE_EQ(imu_states.at(4), sut.get_angular_velocity_x());
  EXPECT_DOUBLE_EQ(imu_states.at(5), sut.get_angular_velocity_y());
  EXPECT_DOUBLE_EQ(imu_states.at(6), sut.get_angular_velocity_z());

  EXPECT_DOUBLE_EQ(imu_states.at(7), sut.get_linear_acceleration_x());
  EXPECT_DOUBLE_EQ(imu_states.at(8), sut.get_linear_acceleration_y());
  EXPECT_DOUBLE_EQ(imu_states.at(9), sut.get_linear_acceleration_z());

  EXPECT_THAT(
    sut.get_orientation_covariance(),
    testing::ElementsAreArray({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}));
  EXPECT_THAT(
    sut.get_angular_velocity_covariance(),
    testing::ElementsAreArray({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}));
  EXPECT_THAT(
    sut.get_linear_acceleration_covariance(),
    testing::ElementsAreArray({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}));

  imu_states.at(0) = 1.1;  // orientation
  imu_states.at(1) = 1.2;
  imu_states.at(2) = 1.3;
  imu_states.at(3) = 1.4;

  imu_states.at(4) = 2.1;  // angular velocity
  imu_states.at(5) = 2.2;
  imu_states.at(6) = 2.3;

  imu_states.at(7) = 3.1;  // linear acceleration
  imu_states.at(8) = 3.2;
  imu_states.at(9) = 3.3;

  imu_states.at(10) = 0.1;  // orientation covariance diagonal
  imu_states.at(11) = 0.2;
  imu_states.at(12) = 0.3;

  imu_states.at(13) = 0.4;  // angular velocity covariance diagonal
  imu_states.at(14) = 0.5;
  imu_states.at(15) = 0.6;

  imu_states.at(16) = 0.7;  // linear acceleration covariance diagonal
  imu_states.at(17) = 0.8;
  imu_states.at(18) = 0.9;

  EXPECT_DOUBLE_EQ(imu_states.at(0), sut.get_orientation_x());
  EXPECT_DOUBLE_EQ(imu_states.at(1), sut.get_orientation_y());
  EXPECT_DOUBLE_EQ(imu_states.at(2), sut.get_orientation_z());
  EXPECT_DOUBLE_EQ(imu_states.at(3), sut.get_orientation_w());

  EXPECT_DOUBLE_EQ(imu_states.at(4), sut.get_angular_velocity_x());
  EXPECT_DOUBLE_EQ(imu_states.at(5), sut.get_angular_velocity_y());
  EXPECT_DOUBLE_EQ(imu_states.at(6), sut.get_angular_velocity_z());

  EXPECT_DOUBLE_EQ(imu_states.at(7), sut.get_linear_acceleration_x());
  EXPECT_DOUBLE_EQ(imu_states.at(8), sut.get_linear_acceleration_y());
  EXPECT_DOUBLE_EQ(imu_states.at(9), sut.get_linear_acceleration_z());

  EXPECT_THAT(
    sut.get_orientation_covariance(),
    testing::ElementsAreArray({0.1, 0.0, 0.0, 0.0, 0.2, 0.0, 0.0, 0.0, 0.3}));
  EXPECT_THAT(
    sut.get_angular_velocity_covariance(),
    testing::ElementsAreArray({0.4, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.6}));
  EXPECT_THAT(
    sut.get_linear_acceleration_covariance(),
    testing::ElementsAreArray({0.7, 0.0, 0.0, 0.0, 0.8, 0.0, 0.0, 0.0, 0.9}));
}

TEST_F(IMUSensorCovarianceWithTest, should_fill_imu_msg_with_value_from_state_interface) {
  EXPECT_TRUE(sut.assign_loaned_state_interfaces(state_interface));

  sensor_msgs::msg::Imu message;
  EXPECT_TRUE(sut.get_values_as_message(message));

  EXPECT_DOUBLE_EQ(imu_states.at(0), message.orientation.x);
  EXPECT_DOUBLE_EQ(imu_states.at(1), message.orientation.y);
  EXPECT_DOUBLE_EQ(imu_states.at(2), message.orientation.z);
  EXPECT_DOUBLE_EQ(imu_states.at(3), message.orientation.w);

  EXPECT_DOUBLE_EQ(imu_states.at(4), message.angular_velocity.x);
  EXPECT_DOUBLE_EQ(imu_states.at(5), message.angular_velocity.y);
  EXPECT_DOUBLE_EQ(imu_states.at(6), message.angular_velocity.z);

  EXPECT_DOUBLE_EQ(imu_states.at(7), message.linear_acceleration.x);
  EXPECT_DOUBLE_EQ(imu_states.at(8), message.linear_acceleration.y);
  EXPECT_DOUBLE_EQ(imu_states.at(9), message.linear_acceleration.z);

  EXPECT_THAT(
    message.orientation_covariance,
    testing::ElementsAreArray({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}));
  EXPECT_THAT(
    message.angular_velocity_covariance,
    testing::ElementsAreArray({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}));
  EXPECT_THAT(
    message.linear_acceleration_covariance,
    testing::ElementsAreArray({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}));

  imu_states.at(0) = 1.01;  // orientation
  imu_states.at(1) = 1.02;
  imu_states.at(2) = 1.03;
  imu_states.at(3) = 1.04;

  imu_states.at(4) = 2.01;  // angular velocity
  imu_states.at(5) = 2.02;
  imu_states.at(6) = 2.03;

  imu_states.at(7) = 3.01;  // linear_acceleration
  imu_states.at(8) = 3.02;
  imu_states.at(9) = 3.03;

  imu_states.at(10) = 0.11;  // orientation covariance diagonal
  imu_states.at(11) = 0.22;
  imu_states.at(12) = 0.33;

  imu_states.at(13) = 0.44;  // angular velocity covariance diagonal
  imu_states.at(14) = 0.55;
  imu_states.at(15) = 0.66;

  imu_states.at(16) = 0.77;  // linear acceleration covariance diagonal
  imu_states.at(17) = 0.88;
  imu_states.at(18) = 0.99;

  EXPECT_TRUE(sut.get_values_as_message(message));

  EXPECT_DOUBLE_EQ(imu_states.at(0), message.orientation.x);
  EXPECT_DOUBLE_EQ(imu_states.at(1), message.orientation.y);
  EXPECT_DOUBLE_EQ(imu_states.at(2), message.orientation.z);
  EXPECT_DOUBLE_EQ(imu_states.at(3), message.orientation.w);

  EXPECT_DOUBLE_EQ(imu_states.at(4), message.angular_velocity.x);
  EXPECT_DOUBLE_EQ(imu_states.at(5), message.angular_velocity.y);
  EXPECT_DOUBLE_EQ(imu_states.at(6), message.angular_velocity.z);

  EXPECT_DOUBLE_EQ(imu_states.at(7), message.linear_acceleration.x);
  EXPECT_DOUBLE_EQ(imu_states.at(8), message.linear_acceleration.y);
  EXPECT_DOUBLE_EQ(imu_states.at(9), message.linear_acceleration.z);

  EXPECT_THAT(
    message.orientation_covariance,
    testing::ElementsAreArray({0.11, 0.0, 0.0, 0.0, 0.22, 0.0, 0.0, 0.0, 0.33}));
  EXPECT_THAT(
    message.angular_velocity_covariance,
    testing::ElementsAreArray({0.44, 0.0, 0.0, 0.0, 0.55, 0.0, 0.0, 0.0, 0.66}));
  EXPECT_THAT(
    message.linear_acceleration_covariance,
    testing::ElementsAreArray({0.77, 0.0, 0.0, 0.0, 0.88, 0.0, 0.0, 0.0, 0.99}));
}
