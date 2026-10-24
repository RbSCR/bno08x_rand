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
 * Authors original ImuSensorBroadcaster: Subhas Das, Denis Stogl, Victor Lopez
 *
 * Author: RbSCR
 * Adapted from https://github.com/ros-controls/ros2_controllers/tree/kilted/imu_sensor_broadcaster
 */

#include "imu_sensor_covariance_broadcaster/imu_sensor_covariance_broadcaster.hpp"

#include <memory>
#include <string>

#include "imu_sensor_covariance_broadcaster/imu_transform.hpp"

namespace imu_sensor_covariance_broadcaster
{
controller_interface::CallbackReturn IMUSensorCovarianceBroadcaster::on_init() {
  try {
    param_listener_ = std::make_shared<ParamListener>(get_node());
  } catch (const std::exception & e) {
    RCLCPP_ERROR(
      get_node()->get_logger(), "Exception thrown during init stage with message: %s \n", e.what());
    return CallbackReturn::ERROR;
  }

  return CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn IMUSensorCovarianceBroadcaster::on_configure(
  const rclcpp_lifecycle::State & /*previous_state*/) {
  try {
    params_ = param_listener_->get_params();
    r_ = quat_from_euler(
      params_.rotation_offset.roll, params_.rotation_offset.pitch, params_.rotation_offset.yaw);
    r_.normalize();
  } catch (const std::exception & e) {
    RCLCPP_ERROR(
      get_node()->get_logger(), "Exception thrown during config stage with message: %s \n",
      e.what());
    return CallbackReturn::ERROR;
  }

  imu_sensor_ = std::make_unique<semantic_components::IMUSensorCovariance>(params_.sensor_name);
  try {
    // register ft sensor data publisher
    sensor_state_publisher_ =
      get_node()->create_publisher<sensor_msgs::msg::Imu>("~/imu", rclcpp::SystemDefaultsQoS());
    realtime_publisher_ = std::make_unique<StatePublisher>(sensor_state_publisher_);
  } catch (const std::exception & e) {
    fprintf(
      stderr, "Exception thrown during publisher creation at configure stage with message : %s \n",
      e.what());
    return CallbackReturn::ERROR;
  }

  state_message_.header.frame_id = params_.frame_id;

  RCLCPP_DEBUG(get_node()->get_logger(), "configure successful");
  return CallbackReturn::SUCCESS;
}

controller_interface::InterfaceConfiguration
IMUSensorCovarianceBroadcaster::command_interface_configuration() const {
  controller_interface::InterfaceConfiguration command_interfaces_config;
  command_interfaces_config.type = controller_interface::interface_configuration_type::NONE;
  return command_interfaces_config;
}

controller_interface::InterfaceConfiguration
IMUSensorCovarianceBroadcaster::state_interface_configuration() const {
  controller_interface::InterfaceConfiguration state_interfaces_config;
  state_interfaces_config.type = controller_interface::interface_configuration_type::INDIVIDUAL;
  state_interfaces_config.names = imu_sensor_->get_state_interface_names();
  return state_interfaces_config;
}

controller_interface::CallbackReturn IMUSensorCovarianceBroadcaster::on_activate(
  const rclcpp_lifecycle::State & /*previous_state*/) {
  imu_sensor_->assign_loaned_state_interfaces(state_interfaces_);
  return CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn IMUSensorCovarianceBroadcaster::on_deactivate(
  const rclcpp_lifecycle::State & /*previous_state*/) {
  imu_sensor_->release_interfaces();
  return CallbackReturn::SUCCESS;
}

controller_interface::return_type IMUSensorCovarianceBroadcaster::update_and_write_commands(
  const rclcpp::Time & time, const rclcpp::Duration & /*period*/) {
  sensor_msgs::msg::Imu input_imu{state_message_};
  imu_sensor_->get_values_as_message(input_imu);
  do_transform(state_message_, input_imu, r_);

  if (realtime_publisher_) {
    state_message_.header.stamp = time;
    realtime_publisher_->try_publish(state_message_);
  }

  return controller_interface::return_type::OK;
}

controller_interface::return_type IMUSensorCovarianceBroadcaster::update_reference_from_subscribers(
  const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/) {
  return controller_interface::return_type::OK;
}

std::vector<hardware_interface::StateInterface::SharedPtr>
IMUSensorCovarianceBroadcaster::on_export_state_interfaces_list() {
  std::vector<hardware_interface::StateInterface::SharedPtr> exported_state_interfaces;

  std::string export_prefix = get_node()->get_name();
  if (!params_.sensor_name.empty()) {
    // Update the prefix and get the proper IMU sensor naming
    export_prefix = export_prefix + "/" + params_.sensor_name;
  }

  auto add_interface = [&](const std::string & name, double value) {
    auto si = std::make_shared<hardware_interface::StateInterface>(export_prefix, name);
    si->set_value(value);
    exported_state_interfaces.push_back(si);
  };

  add_interface("orientation.x", state_message_.orientation.x);
  add_interface("orientation.y", state_message_.orientation.y);
  add_interface("orientation.z", state_message_.orientation.z);
  add_interface("orientation.w", state_message_.orientation.w);
  add_interface("angular_velocity.x", state_message_.angular_velocity.x);
  add_interface("angular_velocity.y", state_message_.angular_velocity.y);
  add_interface("angular_velocity.z", state_message_.angular_velocity.z);
  add_interface("linear_acceleration.x", state_message_.linear_acceleration.x);
  add_interface("linear_acceleration.y", state_message_.linear_acceleration.y);
  add_interface("linear_acceleration.z", state_message_.linear_acceleration.z);

  // Covariances: use the xx diagonal entry to match the semantic component
  add_interface("orientation_covariance", state_message_.orientation_covariance[0]);
  add_interface("angular_velocity_covariance", state_message_.angular_velocity_covariance[0]);
  add_interface("linear_acceleration_covariance", state_message_.linear_acceleration_covariance[0]);

  return exported_state_interfaces;
}

}  // namespace imu_sensor_covariance_broadcaster

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
  imu_sensor_covariance_broadcaster::IMUSensorCovarianceBroadcaster,
  controller_interface::ChainableControllerInterface)
