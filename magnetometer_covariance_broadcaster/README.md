# magnetometer_covariance_broadcaster

![Project Status](https://img.shields.io/badge/Research%20status-Finished-green)
![ROS 2](https://img.shields.io/badge/ROS%202-Jazzy%20|%20Kilted%20(Ubuntu%2024.04)-blue?style=flat&logo=ros&logoSize=auto)
![C++](https://img.shields.io/badge/C++-17-blue?style=flat&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/github/license/rbscr/bno08x_rand?label=License)

- [magnetometer\_covariance\_broadcaster](#magnetometer_covariance_broadcaster)
  - [Package magnetometer\_covariance\_broadcaster](#package-magnetometer_covariance_broadcaster)
  - [Package functionality](#package-functionality)
    - [Parameters](#parameters)
    - [State interfaces](#state-interfaces)
    - [PluginLib library](#pluginlib-library)
    - [Launch files](#launch-files)
  - [Related to research package](#related-to-research-package)

## Package magnetometer_covariance_broadcaster

Researching the use of the new `covariance` fields in the state interface in the ROS2 Control chain to have magnetic_field messages with a dynamic covariance.
The related research package  [bno08x_try_hal_dynamic_covariance](../bno08x_try_hal_dynamic_covariance/README.md) provides the covariance values in the state interface.

This package is copied from the ROS2 Control package `magnetometer_broadcaster` <https://github.com/ros-controls/ros2_controllers/tree/kilted/magnetometer_broadcaster> and updated for the research goal.

The semantic component is copied from the ROS2 Control package `controller_interface` <https://github.com/ros-controls/ros2_control/tree/jazzy/controller_interface> and updated for the research goal.

Several filenames have been changed to distinguish them from the original (in case the original package and this package will be tested on the same device).

## Package functionality

New or changed researched parts are documented here, see the original package `magnetometer_broadcaster` for other information.

### Parameters

The following parameters used in the original package have been removed:

| Name | Description |
| ---- | ----------- |
| static_covariance | Static covariance. Row major about x, y, z axes |

### State interfaces

| Interface | Unit | Notes |
| --------- | ---- | ----- |
| `magnetic_field.x` | – | Magnetic field X |
| `magnetic_field.y` | – | Magnetic field Y |
| `magnetic_field.z` | – | Magnetic field Z |
| **`magnetic_field_covariance`** | – | **Magnetic field covariance** |

The new state interfaces marked in **bold**.

### PluginLib library

Controller/broadcaster to publish readings of magnetometer with dynamic covariance from the sensor.

Pluginlib-Library: magnetometer_covariance_broadcaster

Plugin: magnetometer_covariance_broadcaster/MagnetometerCovarianceBroadcaster (controller_interface::ControllerInterface)

### Launch files

This package has no launch files.
The related research package will have a launch-file to launch the broadcaster in this package.

## Related to research package

- [bno08x_try_hal_dynamic_covariance](../bno08x_try_hal_dynamic_covariance/README.md)
