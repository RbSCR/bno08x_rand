
# bno08x_try_sensor_status

![Project Status](https://img.shields.io/badge/Status-Finished-green)
![ROS 2](https://img.shields.io/badge/ROS%202-Jazzy%20|%20Kilted%20(Ubuntu%2024.04)-blue?style=flat&logo=ros&logoSize=auto)
![C++](https://img.shields.io/badge/C++-17-blue?style=flat&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/github/license/rbscr/bno08x_rand?label=License)

## Package bno08x try sensor status

Researching the use of the `status` field from the measurement reports.
The field indicates the accuracy status of the sensor and could be used to dynamical determine the covariences of the sensors.

| Value | Description |
| ----------- | ----------- |
| 0 | Unreliable |
| 1 | Low accuracy |
| 2 | Medium accuracy |
| 3 | High accuracy |

All reports, with the exception of SH2_GYRO_INTEGRATED_RV, support the status field.

References

- BNO08x datasheet ; par 1.3.5.2, par. 3.1.5
- SH2 Reference manual

## Status / progress

- initial node created ~~, does not yet use status field~~
- created related package `bno08x_imu_msgs`, contains messages with a status field for each of the sensor types
- updated `try_sensor_status` node to use the new messages
- created related package `bno08x_imu_srvs`, contains service to calibrate the sensors
- updated `try_sensor_status` node to use the new service ~~, service does not yet do any calibration~~
- opdated `try_sensor_status` node, service now uses the getCalibrationConfig and setCalibrationConfig methods of the BNO08x library
- updated `try_sensor_status` node, now used the (renamed) `SetSensorsCalibration` service
- added `GetSensorsCalibration` service
- removed the logging of the current calibration config in the `SetSensorsCalibration` service
- removed info-logging (was used for testing)

## Conclusion

- The status field is available from the measurement reports and could be used to determine the covariances of the sensors.
- Setting the calibration config for all 3 sensors has a positive effect on the status (i.e. accuracy) values.
- Moving the IMU has a positive effect on the status (i.e. accuracy) of the magnetometer
  - Without moving the magnetometer status remains 0.

## Next

In the ROS Control hardware-interface research

- passing the status field to the controller in the state interface
- and using the status field to set/determine the covariance values either in the controller or in the imu-broadcaster

## Related research packages

- [bno08x_imu_msgs](../bno08x_imu_msgs/README.md)
- [bno08x_imu_srvs](../bno08x_imu_srvs/README.md)

See `package.xml` and/or `CMakeLists.txt` for other used packages.
