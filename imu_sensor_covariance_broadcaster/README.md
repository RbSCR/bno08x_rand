# imu_sensor_covariance_broadcaster

![Project Status](https://img.shields.io/badge/Status-Finished-green)
![ROS 2](https://img.shields.io/badge/ROS%202-Jazzy%20|%20Kilted%20(Ubuntu%2024.04)-blue?style=flat&logo=ros&logoSize=auto)
![C++](https://img.shields.io/badge/C++-17-blue?style=flat&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/github/license/rbscr/bno08x_rand?label=License)

- [imu\_sensor\_covariance\_broadcaster](#imu_sensor_covariance_broadcaster)
  - [Package imu\_sensor\_covariance\_broadcaster](#package-imu_sensor_covariance_broadcaster)
  - [Package functionality](#package-functionality)
    - [Parameters](#parameters)
    - [State interfaces](#state-interfaces)
    - [PluginLib library](#pluginlib-library)
    - [Launch files](#launch-files)
  - [Status / progess](#status--progess)
  - [Related to research package](#related-to-research-package)

## Package imu_sensor_covariance_broadcaster

Researching the use of the new `covariance` fields in the state interface in the ROS2 Control chain to have IMU messages with a dynamic covariance.
The related research package  [bno08x_try_hal_dynamic_covariance](../bno08x_try_hal_dynamic_covariance/README.md) provides the covariance values in the state interface.

This package is copied from the ROS2 Control package `imu_sensor_broadcaster` <https://github.com/ros-controls/ros2_controllers/tree/kilted/imu_sensor_broadcaster> and updated for the research goal.

The semantic component is copied from the ROS2 Control package `controller_interface` <https://github.com/ros-controls/ros2_control/tree/jazzy/controller_interface> and updated for the research goal.

Several filenames have been changed to distinguish them from the original (in case the original package and this package will be tested on the same device).

## Package functionality

New or changed researched parts are documented here, see the original package `imu_sensor_broadcaster` for other information.

### Parameters

The following parameters used in the original package have been removed:

| Name | Description |
| ---- | ----------- |
| static_covariance_orientation | Static orientation covariance. Row major about x, y, z axes |
| static_covariance_angular_velocity | Static angular velocity covariance. Row major about x, y, z axes |
| static_covariance_linear_acceleration | Static linear acceleration covariance. Row major about x, y, z axes |

### State interfaces

| Interface | Unit | Notes |
| --------- | ---- | ----- |
| `orientation.x` | – | Quaternion X |
| `orientation.y` | – | Quaternion Y |
| `orientation.z` | – | Quaternion Z |
| `orientation.w` | – | Quaternion W |
| `angular_velocity.x` | rad/s | Gyroscope X |
| `angular_velocity.y` | rad/s | Gyroscope Y |
| `angular_velocity.z` | rad/s | Gyroscope Z |
| `linear_acceleration.x` | m/s² | Accelerometer X |
| `linear_acceleration.y` | m/s² | Accelerometer Y |
| `linear_acceleration.z` | m/s² | Accelerometer Z |
| **`orientation_covariance_xx`** | – | **Orientation X covariance** |
| **`orientation_covariance_yy`** | – | **Orientation Y covariance** |
| **`orientation_covariance_zz`** | – | **Orientation Z covariance** |
| **`angular_velocity_covariance_xx`** | - | **Gyroscope X covariance** |
| **`angular_velocity_covariance_yy`** | - | **Gyroscope Y covariance** |
| **`angular_velocity_covariance_zz`** | - | **Gyroscope Z covariance** |
| **`linear_acceleration_covariance_xx`** | - | **Accelerometer X covariance** |
| **`linear_acceleration_covariance_yy`** | - | **Accelerometer Y covariance** |
| **`linear_acceleration_covariance_zz`** | - | **Accelerometer Z covariance** |

The new state interfaces marked in **bold**.

### PluginLib library

Controller/broadcaster to publish readings of IMU sensors with dynamic covariance from the sensor.

Pluginlib-Library: imu_sensor_covariance_broadcaster

Plugin: imu_sensor_covariance_broadcaster/IMUSensorCovarianceBroadcaster (controller_interface::ControllerInterface)

### Launch files

This package has no launch files.
The related research package will have a launch-file to launch the broadcaster in this package.

## Status / progess

- Finished

## Related to research package

- [bno08x_try_hal_dynamic_covariance](../bno08x_try_hal_dynamic_covariance/README.md)
