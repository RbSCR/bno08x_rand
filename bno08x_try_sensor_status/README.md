
# bno08x_try_sensor_status

![Project Status](https://img.shields.io/badge/Status-Work%20In%20Progress-orange)
![ROS 2](https://img.shields.io/badge/ROS%202-Jazzy%20|%20Kilted%20(Ubuntu%2024.04)-blue?style=flat&logo=ros&logoSize=auto)
![C++](https://img.shields.io/badge/C++-17-blue?style=flat&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/github/license/rbscr/bno08x_rand?label=License)

## Package bno08x try sensor status

Researching the use of the `status` field from the measurement reports.
The field indicates the accuracy status of the sensor.

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
- updated ``try_sensor_satatus` node to use the new messages

### Todo

- determine how to get/read the current calibration settings
- determine how to change the calibration settings for each of the sensor types

## Related packages

- [bno08x_imu_msgs](../bno08x_imu_msgs/README.md)
