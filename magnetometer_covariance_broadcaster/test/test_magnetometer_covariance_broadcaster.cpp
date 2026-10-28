// Copyright 2026 RbSCR
// Copyright 2026 ros2_control development team
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
 * Authors original MagnetometerBroadcaster: Christian Rauch, Wiktor Bajor, Jakub Delicat
 *
 * Author: RbSCR
 * Adapted from https://github.com/ros-controls/ros2_controllers/tree/kilted/magnetometer_broadcaster
 */

#include <gmock/gmock.h>
#include <tuple>
#include <utility>

#include <controller_interface/controller_interface.hpp>
#include <hardware_interface/loaned_state_interface.hpp>
#include <hardware_interface/types/hardware_interface_return_values.hpp>
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <rclcpp/node.hpp>
#include <rclcpp/wait_result_kind.hpp>
#include <rclcpp/wait_set.hpp>
#include <rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp>
#include <ros2_control_test_assets/descriptions.hpp>
#include <sensor_msgs/msg/magnetic_field.hpp>
#include "controller_interface/test_utils.hpp"

#include "magnetometer_covariance_broadcaster/magnetometer_covariance_broadcaster.hpp"
#include "magnetometer_covariance_broadcaster/magnetometer_covariance_broadcaster_parameters.hpp"

using controller_interface::activate_succeeds;
using controller_interface::configure_succeeds;
using controller_interface::deactivate_succeeds;

using testing::IsEmpty;
using testing::SizeIs;

namespace
{
rclcpp::WaitResultKind wait_for(std::shared_ptr<rclcpp::SubscriptionBase> subscription) {
  rclcpp::WaitSet wait_set;
  wait_set.add_subscription(subscription);
  return wait_set.wait().kind();
}

rclcpp::NodeOptions create_node_options_with_overriden_parameters(
  std::vector<rclcpp::Parameter> parameters) {
  auto node_options = rclcpp::NodeOptions();
  node_options.parameter_overrides(parameters);
  return node_options;
}
}  // namespace

class MagnetometerCovarianceBroadcasterTest : public ::testing::Test
{
public:
  void SetUp() {
    broadcaster =
      std::make_unique<magnetometer_covariance_broadcaster::MagnetometerCovarianceBroadcaster>();

    magnetic_field_x =
      std::make_shared<hardware_interface::StateInterface>(sensor_name_, "magnetic_field.x");
    std::ignore = magnetic_field_x->set_value(sensor_values_[0]);
    magnetic_field_y =
      std::make_shared<hardware_interface::StateInterface>(sensor_name_, "magnetic_field.y");
    std::ignore = magnetic_field_y->set_value(sensor_values_[1]);
    magnetic_field_z =
      std::make_shared<hardware_interface::StateInterface>(sensor_name_, "magnetic_field.z");
    std::ignore = magnetic_field_z->set_value(sensor_values_[2]);
    magnetic_field_covariance = std::make_shared<hardware_interface::StateInterface>(
      sensor_name_, "magnetic_field_covariance");
    std::ignore = magnetic_field_covariance->set_value(sensor_values_[3]);
  }

  void TearDown() { broadcaster.reset(); }

  void setup_broadcaster() {
    std::vector<hardware_interface::LoanedStateInterface> state_ifs;
    state_ifs.emplace_back(magnetic_field_x, nullptr);
    state_ifs.emplace_back(magnetic_field_y, nullptr);
    state_ifs.emplace_back(magnetic_field_z, nullptr);
    state_ifs.emplace_back(magnetic_field_covariance, nullptr);

    broadcaster->assign_interfaces({}, std::move(state_ifs));
  }

  sensor_msgs::msg::MagneticField subscribe_and_get_message() {
    rclcpp::Node test_subscription_node("test_subscription_node");
    auto subscription = test_subscription_node.create_subscription<sensor_msgs::msg::MagneticField>(
      "/test_magnetometer_covariance_broadcaster/magnetic_field", 10,
      [](const sensor_msgs::msg::MagneticField::SharedPtr) {});
    broadcaster->update(rclcpp::Time{}, rclcpp::Duration::from_seconds(0));
    wait_for(subscription);

    rclcpp::MessageInfo msg_info;
    sensor_msgs::msg::MagneticField msg;
    subscription->take(msg, msg_info);
    return msg;
  }

  controller_interface::ControllerInterfaceParams create_ctrl_params(
    const rclcpp::NodeOptions & node_options, const std::string & robot_description = "") {
    controller_interface::ControllerInterfaceParams params;
    params.controller_name = "test_magnetometer_covariance_broadcaster";
    params.robot_description = robot_description;
    params.update_rate = 0;
    params.node_namespace = "";
    params.node_options = node_options;
    return params;
  }

protected:
  const std::string sensor_name_ = "magnetometer";
  const rclcpp::Parameter sensor_name_param_ = rclcpp::Parameter("sensor_name", sensor_name_);
  const std::string frame_id_ = "magnetometer_frame";
  const rclcpp::Parameter frame_id_param_ = rclcpp::Parameter("frame_id", frame_id_);

  const std::array<double, 4> sensor_values_ = {{20e-6, 30e-6, 40e-6, 50e-6}};
  // Note: the covariance value (element 3) in the sensor_values_ array should be kept
  // equal to the elements 0, 4 and 8 in the expect_covariance_arrays below.
  const std::array<double, 9> expect_covar_ = {{50e-6, 0.0, 0.0, 0.0, 50e-6, 0.0, 0.0, 0.0, 50e-6}};
  hardware_interface::StateInterface::SharedPtr magnetic_field_x;
  hardware_interface::StateInterface::SharedPtr magnetic_field_y;
  hardware_interface::StateInterface::SharedPtr magnetic_field_z;
  hardware_interface::StateInterface::SharedPtr magnetic_field_covariance;

  std::unique_ptr<magnetometer_covariance_broadcaster::MagnetometerCovarianceBroadcaster>
    broadcaster;
};

TEST_F(MagnetometerCovarianceBroadcasterTest, whenNoParamsAreSetThenInitShouldFail) {
  const auto result = broadcaster->init(create_ctrl_params(
    broadcaster->define_custom_node_options(), ros2_control_test_assets::minimal_robot_urdf));
  ASSERT_EQ(result, controller_interface::return_type::ERROR);
}

TEST_F(MagnetometerCovarianceBroadcasterTest, whenOnlySensorNameIsSetThenInitShouldFail) {
  const auto node_options = create_node_options_with_overriden_parameters({sensor_name_param_});
  const auto result = broadcaster->init(
    create_ctrl_params(node_options, ros2_control_test_assets::minimal_robot_urdf));
  ASSERT_EQ(result, controller_interface::return_type::ERROR);
}

TEST_F(
  MagnetometerCovarianceBroadcasterTest,
  whenAllRequiredArgumentsAreSetThenInitConfigureAndActivationShouldSucceed) {
  const auto node_options =
    create_node_options_with_overriden_parameters({sensor_name_param_, frame_id_param_});
  const auto result = broadcaster->init(
    create_ctrl_params(node_options, ros2_control_test_assets::minimal_robot_urdf));
  ASSERT_EQ(result, controller_interface::return_type::OK);
  ASSERT_TRUE(configure_succeeds(broadcaster));
  ASSERT_TRUE(activate_succeeds(broadcaster));
}

TEST_F(
  MagnetometerCovarianceBroadcasterTest,
  whenAllRequiredArgumentsAreSetThenInitConfigureActivateAndDeactivateShouldSucceed) {
  const auto node_options =
    create_node_options_with_overriden_parameters({sensor_name_param_, frame_id_param_});
  const auto result = broadcaster->init(
    create_ctrl_params(node_options, ros2_control_test_assets::minimal_robot_urdf));
  ASSERT_EQ(result, controller_interface::return_type::OK);
  ASSERT_TRUE(configure_succeeds(broadcaster));
  ASSERT_TRUE(activate_succeeds(broadcaster));

  // check interface configuration
  auto cmd_if_conf = broadcaster->command_interface_configuration();
  ASSERT_THAT(cmd_if_conf.names, IsEmpty());
  EXPECT_EQ(cmd_if_conf.type, controller_interface::interface_configuration_type::NONE);
  auto state_if_conf = broadcaster->state_interface_configuration();
  ASSERT_THAT(state_if_conf.names, SizeIs(4lu));
  EXPECT_EQ(state_if_conf.type, controller_interface::interface_configuration_type::INDIVIDUAL);

  // deactivate
  ASSERT_TRUE(deactivate_succeeds(broadcaster));

  // check interface configuration again
  cmd_if_conf = broadcaster->command_interface_configuration();
  ASSERT_THAT(cmd_if_conf.names, IsEmpty());
  EXPECT_EQ(cmd_if_conf.type, controller_interface::interface_configuration_type::NONE);
  state_if_conf = broadcaster->state_interface_configuration();
  ASSERT_THAT(state_if_conf.names, SizeIs(4lu));
  EXPECT_EQ(state_if_conf.type, controller_interface::interface_configuration_type::INDIVIDUAL);
}

TEST_F(MagnetometerCovarianceBroadcasterTest, whenBroadcasterIsActiveShouldPublishWithCovariance) {
  const auto node_options =
    create_node_options_with_overriden_parameters({sensor_name_param_, frame_id_param_});
  const auto result = broadcaster->init(
    create_ctrl_params(node_options, ros2_control_test_assets::minimal_robot_urdf));
  ASSERT_EQ(result, controller_interface::return_type::OK);
  ASSERT_TRUE(configure_succeeds(broadcaster));
  setup_broadcaster();
  ASSERT_TRUE(activate_succeeds(broadcaster));

  const auto msg = subscribe_and_get_message();
  EXPECT_EQ(msg.header.frame_id, frame_id_);
  EXPECT_EQ(msg.magnetic_field.x, sensor_values_[0]);
  EXPECT_EQ(msg.magnetic_field.y, sensor_values_[1]);
  EXPECT_EQ(msg.magnetic_field.z, sensor_values_[2]);

  ASSERT_THAT(msg.magnetic_field_covariance, ::testing::ElementsAreArray(expect_covar_));
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  rclcpp::init(argc, argv);
  int result = RUN_ALL_TESTS();
  rclcpp::shutdown();
  return result;
}
