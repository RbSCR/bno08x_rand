

.. _imu_sensor_covariance_broadcaster_userdoc:

IMU Sensor Covariance Broadcaster
--------------------------------
Broadcaster of messages from IMU sensor.
The published message type is ``sensor_msgs/msg/Imu``.

The controller is a wrapper around ``IMUSensorCovariance`` semantic component (provided in this package).

Parameters
^^^^^^^^^^^
This controller uses the `generate_parameter_library <https://github.com/PickNikRobotics/generate_parameter_library>`_ to handle its parameters. The parameter `definition file located in the src folder <https://github.com/ros-controls/ros2_controllers/blob/{REPOS_FILE_BRANCH}/imu_sensor_covariance_broadcaster/src/imu_sensor_covariance_broadcaster_parameters.yaml>`_ contains descriptions for all the parameters used by the controller.

List of parameters
=========================
.. generate_parameter_library_details:: ../src/imu_sensor_covariance_broadcaster_parameters.yaml

