# bno08x_try_hal_dynamic_covariance

![Project Status](https://img.shields.io/badge/Status-Work%20In%20Progress-orange)
![ROS 2](https://img.shields.io/badge/ROS%202-Jazzy%20|%20Kilted%20(Ubuntu%2024.04)-blue?style=flat&logo=ros&logoSize=auto)
![C++](https://img.shields.io/badge/C++-17-blue?style=flat&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/github/license/RbSCR/bno08x_hardware_interface?label=License)

- [bno08x\_try\_hal\_dynamic\_covariance](#bno08x_try_hal_dynamic_covariance)
  - [Package bno08x try hal dynamic covariance](#package-bno08x-try-hal-dynamic-covariance)
  - [(Intended) package functionality](#intended-package-functionality)
    - [Features](#features)
    - [Hardware parameters and state interfaces](#hardware-parameters-and-state-interfaces)
      - [Hardware parameters](#hardware-parameters)
      - [State interfaces](#state-interfaces)
    - [Launch files and parameters](#launch-files-and-parameters)
      - [Launch files](#launch-files)
        - [bno08x](#bno08x)
        - [bno08x\_magnetometer](#bno08x_magnetometer)
        - [bno08x\_fixedhwparams](#bno08x_fixedhwparams)
      - [Launch parameters](#launch-parameters)
  - [Research intention](#research-intention)
  - [Research status / progress](#research-status--progress)
  - [Related research packages](#related-research-packages)

## Package bno08x try hal dynamic covariance

Researching the use of the `status` field from the measurement reports in the ROS2 Control chain to have IMU messages with a dynamic covariance.
Also see [intension](#research-intention).

This package is copied from the `BNO08x_hardware_interface` and updated for the research goal.
Several filenames have been changed to distinguish them from the original (in case the original package and this packaege will be tested on the same device).

## (Intended) package functionality

New researched parts are documented here, see the original package `BNO08x_hardware_interface` for other information.

### Features

No new or changed features yet.

### Hardware parameters and state interfaces

#### Hardware parameters

See original package.

#### State interfaces

| Interface | Unit | Notes |
| --------- | ---- | ----- |
| `orientation.x` | – | Quaternion X |
| `orientation.y` | – | Quaternion Y |
| `orientation.z` | – | Quaternion Z |
| `orientation.w` | – | Quaternion W |
| **`orientation.status`** | – | **Status** |
| `angular_velocity.x` | rad/s | Gyroscope X |
| `angular_velocity.y` | rad/s | Gyroscope Y |
| `angular_velocity.z` | rad/s | Gyroscope Z |
| **`angular_velocity.status`** | - | **Gyroscope status** |
| `linear_acceleration.x` | m/s² | Accelerometer X |
| `linear_acceleration.y` | m/s² | Accelerometer Y |
| `linear_acceleration.z` | m/s² | Accelerometer Z |
| **`linear_acceleration.status`** | - | **Accelerometer status** |
| `magnetic_field.x` | Tesla | Magnetometer X - when magnetometer enabled |
| `magnetic_field.y` | Tesla | Magnetometer Y - when magnetometer enabled |
| `magnetic_field.z` | Tesla | Magnetometer Z - when magnetometer enabled |
| **`magnetic_field.status`** | - | **Magnetometer status** - when magnetometer enabled |

New state interfaces marked **bold**.

### Launch files and parameters

#### Launch files

This package has 3 (example) launch-files:

- `bno08x_hal.launch.py`
- `bno08x_hal_magnetometer.launch.py`
- `bno08x_hal_fixedhwparams.launch.py`

##### bno08x

This file default launches the BNO08x with only the IMU enabled.
The related urdf-file is `bno08x_hal.urdf.xacro` and the related controller-config-file
is `imu_hal_broadcaster.yaml`.

The urdf- and config-file are not configured for the magnetometer.
The urdf-file contains defaults for the hardware parameters, which can be overruled by the
parameters in the launch-file.

##### bno08x_magnetometer

This file default launches the BNO08x with the IMU and the magnetometer enabled.
The related urdf-file is `bno08x_hal_agnetometer.urdf.xacro` and the related controller-config-file
is `imu_hal_magnetometer_broadcaster.yaml`.

The urdf- and config-file are configured for the magnetometer.
The urdf-file contains defaults for the hardware parameters, which can be overruled by the
parameters in the launch-file.

##### bno08x_fixedhwparams

This file default launches the BNO08x with only the IMU enabled.
The related urdf-file is `bno08x_hal_fixedhwparams.urdf.xacro` and the related controller-config-file
is `imu_hal_broadcaster.yaml`.

The urdf- and config-file are not configured for the magnetometer.
The urdf-file contains the values of the hardware parameters, the related "launch-parameters" have
been removed from the launch-file.

This launch-file, urdf-file and controller-config-file can be used as a starting point for an
actual robot.

Note: because this launch-file and urdf-file are also used in a test, the hardware
parameter `enable_moch_mode` does not have a 'fixed' value in the urdf-file, but is still used as
a parameter from the launch-file.

#### Launch parameters

The launch parameters enable additional publishers/broadcasters.

The IMU measurements (orientation, angular velocity and linear acceleration) are always broadcasted using the imu_sensor_broadcaster.

| Parameter | Type | Default | Description |
| --------- | ---- | ------- | ----------- |
| `publish_tf` | `bool` | `"true"` | Publish a dynamic world→base_link TF from IMU orientation for RViz visualization |
| `broadcast_magnetometer` | `bool` | `"true"` | Broadcast magnetometer measurements using the magnetometer_broadcaster. To be usefull also set enable_magnetometer to true |

The hardware parameters (see the hardware parameter table in original package) can also be used/set in the launch files `bno08x.launch.py` and `bno08x_magnetometer.launch.py` to overrule the defaults set in the related `urdf.xacro`-file.

## Research intention

- Add `status` to the state interface
- Create a new `imu_sensor_dynamic_covariance_broadcaster` (based upon the ROS2 Control imu_sensor_broadcaster) that will update the covariance-matrix with values depending on the `status`

## Research status / progress

- created package ~~, no research enhancements yet, same functionality as original package~~
- added
  - status field (.cpp/.hpp , urdf.xacro's , cpp-test)
  - (ROS2 Control) state_interfaces_broadcaster
- result physical test (Jazzy Ubuntu 24.04):
  - status field is published in state interface
  - (ROS2 Control) imu_sensor_broadcaster and magnetometer_broadcaster still publish respective sensor messages
- possible change:
  - put standard deviation instead of status/accuracy in the state interface
  - advantages:
    - new imu_sensor_dynamic_covariance_broadcaster doesn't need to 'know' about the meaning of the status/accuracy values
    - new broadcaster will be more generic and not specific to the BNO08x sensor
    - values of the standard deviation are controlled/determined by the hardware interface (i.e. close to measurement source)
  - reason for 1 value standard deviation
    - keep state interface small
      - covariance-matrix-diagonal (or standard-deviation-matrix-diagonal) will add 2 addition values to the interface per sensor
    - standard deviation is often mentioned in datasheets
    - covariance-matrix often has the same value on the matrix-diagonal

## Related research packages
