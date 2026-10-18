// Copyright 2021 PAL Robotics SL.                (ROS2 Control imu_sensor_broadcaster)
// Copyright 2025 ros2_control development team   (ROS2 Control gps_sensor_broadcaster)
//
// Copyright 2026 RbSCR                           (imu_sensor_covariance_static_and_dynamic_broadcaster)
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
 * Authors original gps_sensor_broadcaster: Wiktor Bajor, Jakub Delicat
 * Authors original imu_sensor_broadcaster: Subhas Das, Denis Stogl, Victor Lopez
 *
 * Author: RbSCR
 */

#include "imu_sensor_covariance_static_and_dynamic_broadcaster/imu_sensor_covariance_static_and_dynamic_broadcaster.hpp"
#include "imu_sensor_covariance_static_and_dynamic_broadcaster/imu_transform.hpp"
#include <map>

namespace
{
template <class... Ts>
struct Visitor : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;
constexpr std::size_t COVARIANCE_3x3_SIZE = 9;
}  // namespace

namespace imu_sensor_covariance_static_and_dynamic_broadcaster
{
callback_return_type IMUSensorCovarianceBroadcaster::on_init() {
  try {
    param_listener_ =
      std::make_shared<imu_sensor_covariance_static_and_dynamic_broadcaster::ParamListener>(get_node());
    params_ = param_listener_->get_params();
    return CallbackReturn::SUCCESS;
  } catch (const std::exception & e) {
    fprintf(stderr, "Exception thrown during init stage with message: %s \n", e.what());
    return CallbackReturn::ERROR;
  }
}

// FIXME(rbscr) compare to standard  ROS2 Control imu_sensor_broadcaster - on_configure
callback_return_type IMUSensorCovarianceBroadcaster::on_configure(const rclcpp_lifecycle::State &) {
  param_listener_->refresh_dynamic_parameters();
  params_ = param_listener_->get_params();
  // FIXME(rbscr) use r_ somewhere
  // imu_sensor_broadcaster has a method quat_from_euler() and uses param_rotation_offset.xxx
  // also this is in a try-catch block
  //
  // in imu_sensor_broadcaster r_ is used in a do_transform(state_message, input_imu, r_) call in update_and_write_commands()

  r_ = quat_from_euler(
    params_.rotation_offset.roll, params_.rotation_offset.pitch, params_.rotation_offset.yaw);
  r_.normalize();

  if (params_.read_covariance_from_interface) {
    imu_sensor_.emplace<
      my_semantic_component::IMUSensorCovariance<IMUSensorCovarianceOption::WithoutCovariance>>(
      params_.sensor_name);
  } else {
    imu_sensor_.emplace<
      my_semantic_component::IMUSensorCovariance<IMUSensorCovarianceOption::WithCovariance>>(
      params_.sensor_name);
  }

  std::visit(
    Visitor{
      [this](auto & sensor) { state_names_ = sensor.get_state_interface_names(); },
      [](std::monostate &) {}},
    imu_sensor_);

  return setup_publisher();
}

callback_return_type IMUSensorCovarianceBroadcaster::setup_publisher() {
  try {
    sensor_state_publisher_ =
      get_node()->create_publisher<sensor_msgs::msg::Imu>("~/imu", rclcpp::SystemDefaultsQoS());
    realtime_publisher_ = std::make_unique<StatePublisher>(sensor_state_publisher_);
    state_message_.header.frame_id = params_.frame_id;
    setup_covariance();

    return callback_return_type::SUCCESS;
  } catch (const std::exception & e) {
    fprintf(
      stderr, "Exception thrown during publisher creation at configure stage with message: %s \n",
      e.what());
    return callback_return_type::ERROR;
  }
}

void IMUSensorCovarianceBroadcaster::setup_covariance() {
  if (params_.read_covariance_from_interface) {
    return;
  }

  for (size_t i = 0; i < COVARIANCE_3x3_SIZE; ++i) {
    state_message_.orientation_covariance[i] = params_.static_covariance_orientation[i];
    state_message_.angular_velocity_covariance[i] = params_.static_covariance_angular_velocity[i];
    state_message_.linear_acceleration_covariance[i] =
      params_.static_covariance_linear_acceleration[i];
  }
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
  state_interfaces_config.names =
    state_names_;  // TODO(rbscr) in imu_sensor_broadcaster: state_interfaces_config.names = imu_sensor_->get_state_interface_names();

  return state_interfaces_config;
}

callback_return_type IMUSensorCovarianceBroadcaster::on_activate(const rclcpp_lifecycle::State &) {
  std::visit(
    Visitor{
      [this](auto & sensor) { sensor.assign_loaned_state_interfaces(state_interfaces_); },
      [](std::monostate &) {}},
    imu_sensor_);
  return callback_return_type::SUCCESS;
}

callback_return_type IMUSensorCovarianceBroadcaster::on_deactivate(
  const rclcpp_lifecycle::State &) {
  std::visit(
    Visitor{[](auto & sensor) { sensor.release_interfaces(); }, [](std::monostate &) {}},
    imu_sensor_);
  return callback_return_type::SUCCESS;
}

// FIXME(rbscr) compare to standard  ROS2 Control imu_sensor_broadcaster, it has
// - update_and_write_commands()
// - update_reference_from_subscribers()
controller_interface::return_type IMUSensorCovarianceBroadcaster::update(
  const rclcpp::Time &, const rclcpp::Duration &) {
  if (realtime_publisher_) {
    state_message_.header.stamp = get_node()->now();
    std::visit(
      Visitor{
        [this](auto & sensor) {
          sensor_msgs::msg::Imu message;
          sensor.get_values_as_message(state_message_);
        },
        [](std::monostate &) {}},
      imu_sensor_);
    realtime_publisher_->try_publish(state_message_);
  }
  return controller_interface::return_type::OK;
}

// FIXME(rbscr) compare to standard  ROS2 Control imu_sensor_broadcaster, it also has
// an update_export_state_interfaces_list()

}  // namespace imu_sensor_covariance_static_and_dynamic_broadcaster

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
  imu_sensor_covariance_static_and_dynamic_broadcaster::IMUSensorCovarianceBroadcaster,
  controller_interface::ControllerInterface)
