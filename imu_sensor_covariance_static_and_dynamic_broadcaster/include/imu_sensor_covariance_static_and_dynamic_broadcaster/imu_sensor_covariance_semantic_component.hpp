// Copyright 2025 ros2_control development team
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

#ifndef imu_sensor_covariance_static_and_dynamic_broadcaster__IMU_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_
#define imu_sensor_covariance_static_and_dynamic_broadcaster__IMU_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_

#include <array>
#include <limits>
<<<<<<< HEAD
#include <string>
=======
#include <cstring>
>>>>>>> origin/main

#include "semantic_components/semantic_component_interface.hpp"
#include "sensor_msgs/msg/imu.hpp"

namespace my_semantic_component
{

enum class IMUSensorCovarianceOption { WithCovariance, WithoutCovariance };

template <IMUSensorCovarianceOption sensor_option>
class IMUSensorCovariance
: public semantic_components::SemanticComponentInterface<sensor_msgs::msg::Imu>
{
public:
  static_assert(
    sensor_option == IMUSensorCovarianceOption::WithCovariance ||
      sensor_option == IMUSensorCovarianceOption::WithoutCovariance,
    "Invalid IMUSensorCovarianceOption");
  explicit IMUSensorCovariance(const std::string & name)
  : SemanticComponentInterface(
      name, {{name + "/" + "orientation.x"},
             {name + "/" + "orientation.y"},
             {name + "/" + "orientation.z"},
             {name + "/" + "orientation.w"},
             {name + "/" + "angular_velocity.x"},
             {name + "/" + "angular_velocity.y"},
             {name + "/" + "angular_velocity.z"},
             {name + "/" + "linear_acceleration.x"},
             {name + "/" + "linear_acceleration.y"},
             {name + "/" + "linear_acceleration.z"}}) {
    if constexpr (sensor_option == IMUSensorCovarianceOption::WithCovariance) {
      interface_names_.emplace_back(name + "/" + "orientation_covariance_xx");
      interface_names_.emplace_back(name + "/" + "orientation_covariance_yy");
      interface_names_.emplace_back(name + "/" + "orientation_covariance_zz");
      interface_names_.emplace_back(name + "/" + "angular_velocity_covariance_xx");
      interface_names_.emplace_back(name + "/" + "angular_velocity_covariance_yy");
      interface_names_.emplace_back(name + "/" + "angular_velocity_covariance_zz");
      interface_names_.emplace_back(name + "/" + "linear_acceleration_covariance_xx");
      interface_names_.emplace_back(name + "/" + "linear_acceleration_covariance_yy");
      interface_names_.emplace_back(name + "/" + "linear_acceleration_covariance_zz");
    }
  }

  /**
   * @brief Return orientation x value.
   *
   * @return Return orientation x reported by the IMU
   */
  double get_orientation_x() const {
    const auto data = state_interfaces_[0].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return orientation y value.
   *
   * @return Return orientation y reported by the IMU
   */
  double get_orientation_y() const {
    const auto data = state_interfaces_[1].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return orientation z value.
   *
   * @return Orientation z reported by the IMU
   */
  double get_orientation_z() const {
    const auto data = state_interfaces_[2].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return orientation w value.
   *
   * @return Orientation w reported by the IMU
   */
  double get_orientation_w() const {
    const auto data = state_interfaces_[3].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return angular velocity x value.
   *
   * @return Angular velocity x reported by the IMU
   */
  double get_angular_velocity_x() const {
    const auto data = state_interfaces_[4].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return angular velocity y value.
   *
   * @return Angular velocity y reported by the IMU
   */
  double get_angular_velocity_y() const {
    const auto data = state_interfaces_[5].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return angular velocity z value.
   *
   * @return Angular velocity z reported by the IMU
   */
  double get_angular_velocity_z() const {
    const auto data = state_interfaces_[6].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return linear acceleration x value.
   *
   * @return Linear acceleration x reported by the IMU
   */
  double get_linear_acceleration_x() const {
    const auto data = state_interfaces_[7].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return linear acceleration y value.
   *
   * @return Linear acceleration y reported by the IMU
   */
  double get_linear_acceleration_y() const {
    const auto data = state_interfaces_[8].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return linear acceleration z value.
   *
   * @return Linear acceleration z reported by the IMU
   */
  double get_linear_acceleration_z() const {
    const auto data = state_interfaces_[9].get().get_optional();
    if (data.has_value()) {
      return data.value();
    }

    return std::numeric_limits<double>::quiet_NaN();
  }

  /**
   * @brief Return orientation covariance reported by a IMU.
   *
   * @return Covariance array.
   */
  template <
    typename U = void,
    typename = std::enable_if_t<sensor_option == IMUSensorCovarianceOption::WithCovariance, U>>
<<<<<<< HEAD
  const std::array<double, 9> & get_orientation_covariance() {
=======
  const std::array<double, 9> & get_orientation_covariance() const {
>>>>>>> origin/main
    const auto data_1 = state_interfaces_[10].get().get_optional();
    const auto data_2 = state_interfaces_[11].get().get_optional();
    const auto data_3 = state_interfaces_[12].get().get_optional();
    if (data_1.has_value()) {
      covariance_[0] = data_1.value();
    }
    if (data_2.has_value()) {
      covariance_[4] = data_2.value();
    }
    if (data_3.has_value()) {
      covariance_[8] = data_3.value();
    }
    return covariance_;
  }

  /**
   * @brief Return angular velocity covariance reported by a IMU.
   *
   * @return Covariance array.
   */
  template <
    typename U = void,
    typename = std::enable_if_t<sensor_option == IMUSensorCovarianceOption::WithCovariance, U>>
<<<<<<< HEAD
  const std::array<double, 9> & get_angular_velocity_covariance() {
=======
  const std::array<double, 9> & get_angular_velocity_covariance() const {
>>>>>>> origin/main
    const auto data_1 = state_interfaces_[13].get().get_optional();
    const auto data_2 = state_interfaces_[14].get().get_optional();
    const auto data_3 = state_interfaces_[15].get().get_optional();
    if (data_1.has_value()) {
      covariance_[0] = data_1.value();
    }
    if (data_2.has_value()) {
      covariance_[4] = data_2.value();
    }
    if (data_3.has_value()) {
      covariance_[8] = data_3.value();
    }
    return covariance_;
  }

  /**
   * @brief Return linear acceleration covariance reported by a IMU.
   *
   * @return Covariance array.
   */
  template <
    typename U = void,
    typename = std::enable_if_t<sensor_option == IMUSensorCovarianceOption::WithCovariance, U>>
<<<<<<< HEAD
  const std::array<double, 9> & get_linear_acceleration_covariance() {
=======
  const std::array<double, 9> & get_linear_acceleration_covariance() const {
>>>>>>> origin/main
    const auto data_1 = state_interfaces_[16].get().get_optional();
    const auto data_2 = state_interfaces_[17].get().get_optional();
    const auto data_3 = state_interfaces_[18].get().get_optional();
    if (data_1.has_value()) {
      covariance_[0] = data_1.value();
    }
    if (data_2.has_value()) {
      covariance_[4] = data_2.value();
    }
    if (data_3.has_value()) {
      covariance_[8] = data_3.value();
    }
    return covariance_;
  }

  /**
   * @brief Fills Imu message from the current state values
   *
   * Constructs and return a IMU message from the current values.
   * @param[out] message IMU message from values
   * @return always returns true
   */
  bool get_values_as_message(sensor_msgs::msg::Imu & message) const {
    message.orientation.x = get_orientation_x();
    message.orientation.y = get_orientation_y();
    message.orientation.z = get_orientation_z();
    message.orientation.w = get_orientation_w();

    message.angular_velocity.x = get_angular_velocity_x();
    message.angular_velocity.y = get_angular_velocity_y();
    message.angular_velocity.z = get_angular_velocity_z();

    message.linear_acceleration.x = get_linear_acceleration_x();
    message.linear_acceleration.y = get_linear_acceleration_y();
    message.linear_acceleration.z = get_linear_acceleration_z();

    // In the semantic_component gps_sensor the covariance of the GPS message is also
    // filled when the option is 'WithCovariance' (see code example below).

    // code from gps sementic component
    // if constexpr (sensor_option == GPSSensorOption::WithCovariance)
    // {
    //    message.position_covariance = get_covariance();
    // }

<<<<<<< HEAD
    // TODO(rbscr)
    if constexpr (sensor_option == IMUSensorCovarianceOption::WithCovariance) {
      // message.orientation_covariance = get_orientation_covariance();
      // message.angular_velocity_covariance = get_angular_velocity_covariance();
      // message.linear_acceleration_covariance = get_linear_acceleration_covariance();
=======
    if constexpr (sensor_option == IMUSensorCovarianceOption::WithCovariance) {
      message.orientation_covariance = get_orientation_covariance();
      message.angular_velocity_covariance = get_angular_velocity_covariance();
      message.linear_acceleration_covariance = get_linear_acceleration_covariance();
>>>>>>> origin/main
    }

    return true;
  }

private:
<<<<<<< HEAD
  std::array<double, 9> covariance_{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
=======
  mutable std::array<double, 9> covariance_{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
>>>>>>> origin/main
};

}  // namespace my_semantic_component

#endif  // imu_sensor_covariance_static_and_dynamic_broadcaster__IMU_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_
