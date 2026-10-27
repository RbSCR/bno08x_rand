// Copyright 2026 RbSCR
// Copyright 2025 Aarav Gupta
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
 * Author original magnetic field semantic component: Aarav Gupta
 *
 * Author: RbSCR
 * Adapted from https://github.com/ros-controls/ros2_control/blob/kilted/controller_interface/include/semantic_components/magnetic_field_sensor.hpp
 */

#ifndef MAGNETOMETER_COVARIANCE_BROADCASTER__MAGNETIC_FIELD_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_
#define MAGNETOMETER_COVARIANCE_BROADCASTER__MAGNETIC_FIELD_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_

#include <string>

#include "semantic_components/semantic_component_interface.hpp"
#include "sensor_msgs/msg/magnetic_field.hpp"

namespace semantic_components
{
class MagneticFieldSensorCovariance
: public SemanticComponentInterface<sensor_msgs::msg::MagneticField>
{
public:
  explicit MagneticFieldSensorCovariance(const std::string & name)
  : SemanticComponentInterface(
      name, {{name + "/" + "magnetic_field.x"},
             {name + "/" + "magnetic_field.y"},
             {name + "/" + "magnetic_field.z"},
             {name + "/" + "magnetic_field_covariance"}}) {}

  /**
   * @brief Returns values as sensor_msgs::msg::MagneticField
   *
   * @param[out] message MagneticField message from values
   * @return always returns true
   */
  bool get_values_as_message(sensor_msgs::msg::MagneticField & message) {
    update_data_from_interfaces();
    message.magnetic_field.x = data_[0];
    message.magnetic_field.y = data_[1];
    message.magnetic_field.z = data_[2];

    message.magnetic_field_covariance = get_covariance_from_data();
    return true;
  }

private:
  /**
   * @brief Update the data array from the state interfaces.
   * @note This method is thread-safe and non-blocking.
   * @note This method might return stale data if the data is not updated. This is to ensure that
   * the data from the sensor is not discontinuous.
   */
  void update_data_from_interfaces() {
    for (auto i = 0u; i < data_.size(); ++i) {
      const auto data = state_interfaces_[i].get().get_optional();
      if (data.has_value()) {
        data_[i] = data.value();
      }
    }
  }

  /**
   * @brief Return covariance from data array
   *
   * @return array of size 9 with ocovariance values
   */
  std::array<double, 9> get_covariance_from_data() const {
    std::array<double, 9> covariance{{data_[3], 0.0, 0.0, 0.0, data_[3], 0.0, 0.0, 0.0, data_[3]}};

    return covariance;
  }

  /**
   * @brief Array to store the data of the magnetic field sensor
   */
  std::array<double, 4> data_{{0.0, 0.0, 0.0, 0.0}};
};

}  // namespace semantic_components

#endif  // MAGNETOMETER_COVARIANCE_BROADCASTER__MAGNETIC_FIELD_SENSOR_COVARIANCE_SEMANTIC_COMPONENT_HPP_
