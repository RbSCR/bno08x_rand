# bno08x_try_hal_dynamic_covariance

![Project Status](https://img.shields.io/badge/Research%20status-Finished-green)
![ROS 2](https://img.shields.io/badge/ROS%202-Jazzy%20|%20Kilted%20(Ubuntu%2024.04)-blue?style=flat&logo=ros&logoSize=auto)
![C++](https://img.shields.io/badge/C++-17-blue?style=flat&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/github/license/rbscr/bno08x_rand?label=License)

- [bno08x\_try\_hal\_dynamic\_covariance](#bno08x_try_hal_dynamic_covariance)
  - [Package bno08x try hal dynamic covariance](#package-bno08x-try-hal-dynamic-covariance)
  - [Research intention](#research-intention)
  - [New features](#new-features)
    - [Hardware parameters and state interfaces](#hardware-parameters-and-state-interfaces)
      - [Hardware parameters](#hardware-parameters)
      - [State interfaces](#state-interfaces)
  - [Launch files and parameters](#launch-files-and-parameters)
    - [Launch files](#launch-files)
      - [bno08x\_hal](#bno08x_hal)
      - [bno08x\_hal\_magnetometer](#bno08x_hal_magnetometer)
      - [bno08x\_hal\_fixedhwparams](#bno08x_hal_fixedhwparams)
      - [bno08x\_hal\_covariance](#bno08x_hal_covariance)
      - [bno08x\_hal\_covariance\_magnetometer](#bno08x_hal_covariance_magnetometer)
      - [bno08x\_hal\_covariance\_fixedhwparams](#bno08x_hal_covariance_fixedhwparams)
      - [Launch parameters](#launch-parameters)
  - [Research status / progress](#research-status--progress)
  - [Related research packages](#related-research-packages)

## Package bno08x try hal dynamic covariance

Researching the use of the `status` field from the measurement reports in the ROS2 Control chain to have IMU messages with a dynamic covariance.
Also see [intension](#research-intention).

This package is copied from the `BNO08x_hardware_interface` and updated for the research goal.
Several filenames have been changed to distinguish them from the original (in case the original package and this packaege will be tested on the same device).

New researched parts are documented here, see the original package `BNO08x_hardware_interface` for other information.

References

- BNO08x datasheet ; par 1.3.5.2, par. 3.1.5
- SH2 Reference manual

## Research intention

- Add ~~`status`~~ `covariance` to the state interface
- Create a new `imu_sensor_covariance_broadcaster` (based upon the ROS2 Control imu_sensor_broadcaster) that will update the covariance-matrix in the IMU message with values depending on the ~~`status`~~ `covariance` in the state interface.
- Create a new `magnetometer_covariance_broadcaster` (based upon the ROS2 Control magnetometer_broadcaster) that will update the covariance-matrix in the IMU message with values depending on the `covariance` in the state interface.

## New features

- `covariance` hardware parameters added for each of the sensors for the 4 status values (unreliable, low, medium, high) of the sensor.
- `covariance` state interface added for each of the sensors.

### Hardware parameters and state interfaces

#### Hardware parameters

| Hardware parameter | Description |
| ------------------ | ----------- |
| `i2c_bus` | I2C bus number (e.g. 1  >> /dev/i2c-1) |
| `i2c_addr` | I2C device address in hex without 0x prefix (default: 4A = 0x4A, alternative: 4B) |
| `axis_remap` | BNO08X axis placement configuration: valid combination of North East South West Up Down |
| `imu_rate` | IMU sensor rate in Hz. |
| `enable_magnetometer` | Enable magnetometer in hardware_interface |
| `magnetometer_rate` | Magnetometer rate in Hz. |
| **`orientation_covariance_unreliable`** | **Orientation covariance when orientation-sensor status is unreliable** |
| **`orientation_covariance_low`** | **Orientation covariance when orientation-sensor status is low** |
| **`orientation_covariance_medium`** | **Orientation covariance when orientation-sensor status is medium** |
| **`orientation_covariance_high`** | **Orientation covariance when orientation-sensor status is high** |
| **`angular_velocity_covariance_unreliable`** | **Angular velocity covariance when angular-velocity-sensor status is unreliable** |
| **`angular_velocity_covariance_low`** | **Angular velocity covariance when angular-velocity-sensor status is low** |
| **`angular_velocity_covariance_medium`** | **Angular velocity covariance when angular-velocity-sensor status is medium** |
| **`angular_velocity_covariance_high`** | **Angular velocity covariance when angular-velocity-sensor status is high** |
| **`linear_acceleration_covariance_unreliable`** | **Linear acceleration covariance when linear_acceleration-sensor status is unreliable** |
| **`linear_acceleration_covariance_low`** | **Linear acceleration covariance when linear_acceleration-sensor status is low** |
| **`linear_acceleration_covariance_medium`** | **Linear acceleration covariance when linear_acceleration-sensor status is medium** |
| **`linear_acceleration_covariance_high`** | **Linear acceleration covariance when linear_acceleration-sensor status is high** |
| **`magnetometer_covariance_unreliable`** | **Magnetometer covariance when magnetometer status is unreliable** |
| **`magnetometer_covariance_low`** | **Magnetometer covariance when magnetometer status is low** |
| **`magnetometer_covariance_medium`** | **Magnetometer covariance when magnetometer status is medium** |
| **`magnetometer_covariance_high`** | **Magnetometer covariance when magnetometer status is high** |
| `enable_mock_mode` | Use mock/simulation mode (no hardware required) |

New hardware parameters marked in **bold**.

#### State interfaces

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
| **`orientation_covariance`** | – | **Orientation covariance** |
| **`angular_velocity_covariance`** | - | **Gyroscope covariance** |
| **`linear_acceleration_covariance`** | - | **Accelerometer covariance** |
| `magnetic_field.x` | Tesla | Magnetometer X - when magnetometer enabled |
| `magnetic_field.y` | Tesla | Magnetometer Y - when magnetometer enabled |
| `magnetic_field.z` | Tesla | Magnetometer Z - when magnetometer enabled |
| **`magnetic_field_covariance`** | - | **Magnetometer covariance** - when magnetometer enabled |

New state interfaces marked in **bold**.

## Launch files and parameters

### Launch files

This package has 6 (example) launch-files:

- `bno08x_hal.launch.py`
- `bno08x_hal_magnetometer.launch.py`
- `bno08x_hal_fixedhwparams.launch.py`
- `bno08x_hal_covariance.launch.py`
- `bno08x_hal_covariance_magnetometer.launch.py`
- `bno08x_hal_covariance_fixedhwparams.launch.py`

The first 3 use the standard ROS2 Control broadcasters; these broadcasters do not use/ignore the
new covariance state interfaces to determine the covariances in the imu and magnetic_field messages.

The last 3 use the "new" broadcasters; these broadcasters do use/ignore the new covariance state
interfaces to determine the covariances in the imu and magnetic_field messages.

#### bno08x_hal

This file launches the BNO08x with only the IMU enabled.
The related urdf-file is `bno08x_hal.urdf.xacro` and the related controller-config-file
is `imu_hal_broadcaster.yaml`.

The urdf- and config-file are not configured for the magnetometer.
The urdf-file contains defaults for the hardware parameters, which can be overruled by the
parameters in the launch-file.
The IMU broadcaster is the standard ROS2 Control `imu_sensor_broadcaster`.

#### bno08x_hal_magnetometer

This file launches the BNO08x with the IMU and the magnetometer enabled.
The related urdf-file is `bno08x_hal_agnetometer.urdf.xacro` and the related controller-config-file
is `imu_hal_magnetometer_broadcaster.yaml`.

The urdf- and config-file are configured for the magnetometer.
The urdf-file contains defaults for the hardware parameters, which can be overruled by the
parameters in the launch-file.
The IMU broadcaster is the standard ROS2 Control `imu_sensor_broadcaster`, the magnetometer
broadcaster is the standard ROS2 Control `magnetometer_broadcaster`.

#### bno08x_hal_fixedhwparams

This file launches the BNO08x with only the IMU enabled.
The related urdf-file is `bno08x_hal_fixedhwparams.urdf.xacro` and the related controller-config-file
is `imu_hal_broadcaster.yaml`.

The urdf- and config-file are not configured for the magnetometer.
The urdf-file contains the values of the hardware parameters, the related "launch-parameters" have
been removed from the launch-file.

This launch-file, urdf-file and controller-config-file can be used as a starting point for an
actual robot.
The IMU broadcaster is the standard ROS2 Control `imu_sensor_broadcaster`.

Note: because this launch-file and urdf-file are also used in a test, the hardware
parameter `enable_moch_mode` does not have a 'fixed' value in the urdf-file, but is still used as
a parameter from the launch-file.

#### bno08x_hal_covariance

This file launches the BNO08x with only the IMU enabled.
The related urdf-file is `bno08x_hal.urdf.xacro` and the related controller-config-file
is `imu_hal_covariance_broadcaster.yaml`.

The urdf- and config-file are not configured for the magnetometer.
The urdf-file contains defaults for the hardware parameters, which can be overruled by the
parameters in the launch-file.
The IMU broadcaster is the "new" `imu_sensor_covariance_broadcaster`.

#### bno08x_hal_covariance_magnetometer

This file launches the BNO08x with the IMU and the magnetometer enabled.
The related urdf-file is `bno08x_hal_agnetometer.urdf.xacro` and the related controller-config-file
is `imu_hal_covariance_magnetometer_broadcaster.yaml`.

The urdf- and config-file are configured for the magnetometer.
The urdf-file contains defaults for the hardware parameters, which can be overruled by the
parameters in the launch-file.
The IMU broadcaster is the "new" `imu_sensor_covariance_broadcaster`, the magnetometer
broadcaster is the "new" `magnetometer_covariance_broadcaster`.

#### bno08x_hal_covariance_fixedhwparams

This file launches the BNO08x with only the IMU enabled.
The related urdf-file is `bno08x_hal_fixedhwparams.urdf.xacro` and the related controller-config-file
is `imu_hal_covariance_broadcaster.yaml`.

The urdf- and config-file are not configured for the magnetometer.
The urdf-file contains the values of the hardware parameters, the related "launch-parameters" have
been removed from the launch-file.

This launch-file, urdf-file and controller-config-file can be used as a starting point for an
actual robot.
The IMU broadcaster is the "new" `imu_sensor_covariance_broadcaster`.

Note: because this launch-file and urdf-file are also used in a test, the hardware
parameter `enable_moch_mode` does not have a 'fixed' value in the urdf-file, but is still used as
a parameter from the launch-file.

#### Launch parameters

The launch parameters enable additional publishers/broadcasters.

The IMU measurements (orientation, angular velocity and linear acceleration) are always broadcasted using the imu broadcaster.

| Parameter | Type | Default | Description |
| --------- | ---- | ------- | ----------- |
| `publish_tf` | `bool` | `"true"` | Publish a dynamic world→base_link TF from IMU orientation for RViz visualization |
| `broadcast_magnetometer` | `bool` | `"true"` | Broadcast magnetometer measurements using the magnetometer_broadcaster. To be usefull also set enable_magnetometer to true |

The hardware parameters (see the hardware parameter table) can also be used/set in the launch files `bno08x_hal.launch.py`, `bno08x_hal_magnetometer.launch.py`, `bno08x_hal_covariance.launch.py` and `bno08x_hal_covariance_magnetometer.launch.py` to overrule the defaults set in the related `urdf.xacro`-file.

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
- replaced status fields with covariance fields in the state interface, these contain dummy values for now
- decided to use covariance ~~(for each sensor the three matrix diagonals)~~ instead of stdev fields in state interface because
  - covariance can be calculated/determined from the standard deviation from the datasheet or could be determind using calibration package [which will be another research project]
  - the ROS2 Control `gps_sensor_broadcaster` has a mechanism use the covariance from either the configuration or from the state interface,
    something simular could be done for the new `imu_sensor_dynamic_covariance_broadcaster`
  - internet searching about the performance impact of multiple new fields in the state interface mentioned that it should have negligible impact
- decided to use 1 covariance for each sensor instead of three matrix-diagonal values
  - the related new `imu_sensor_covariance_broadcaster` expects 1 covariance for each sensor (orientation, angular velocity, linear acceleration) in the state interfaces
  - the related new `magnetometer_covariance_broadcaster` expects 1 covariance in the state interfaces

## Related research packages

- [imu_sensor_covariance_broadcaster](../imu_sensor_covariance_broadcaster/README.md)
- [magnetometer_covariance_broadcaster](../magnetometer_covariance_broadcaster/README.md)
