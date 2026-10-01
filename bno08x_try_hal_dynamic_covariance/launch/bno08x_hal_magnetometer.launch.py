#!/usr/bin/env python3
"""
Launch file for the BNO08x IMU hardware interface.

Starts the complete ros2_control stack for the BNO08x IMU, including:
- Robot state publisher for TF transforms
- Controller manager with the BNO08x SensorInterface hardware plugin
- State interfaces broadcaster
- IMU sensor broadcaster publishing sensor_msgs/Imu to /imu_sensor_broadcaster/imu
- Magnetometer broadcaster publishing sensor_msgs/MagneticField to /magnetometer_broadcaster/magnetic_field

This is a convenience launch file. The same functionality can be achieved
by using the 'bno08x.launch.py' file with the parameters 'enable_magnetometer:=true'
and 'broadcast_magnetometer:=true'
and -if the magnetometer values should be broadcasted - controller config-yaml with a
reference to the magnetometer-broadcaster.

Base usage:
    ros2 launch bno08x_try_hal_dynamic_covariance bno08x_hal_magnetometer.launch.py

    See the declared_arguments below for the default values.

Example (other) usage:
    <base-usage> i2c_bus:=1 i2c_addr:=4B
    <base-usage> axis_remap:=North-West-Up
    <base-usage> enable_mock_mode:=true
    <base-usage> publish_tf:=false
    <base-usage> broadcast_magnetometer:=false
    <base-usage> publish_diagnostics:=true
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import Command
from launch.substitutions import FindExecutable
from launch.substitutions import LaunchConfiguration
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Declare arguments
    declared_arguments = [
        DeclareLaunchArgument(
            'i2c_bus',
            default_value='1',
            description='I2C bus number (e.g. 1  >> /dev/i2c-1)'
        ),
        DeclareLaunchArgument(
            'i2c_addr',
            default_value='4A',
            description='I2C device address in hex without 0x prefix (default: 4A = 0x4A, alternative: 4B)'
        ),
        DeclareLaunchArgument(
            'axis_remap',
            default_value='East-North-Up',
            description='BNO08X axis placement configuration: valid combination of North East South West Up Down'
                'in the format <xxx>-<xxx>-<xxx>'
                'See datasheet Figure 4-3  page 41 for the valid combinations.'
        ),
        DeclareLaunchArgument(
            'imu_rate',
            default_value='100',
            description='IMU sensor rate in Hz.'
                'See datasheet Figure 6-16  page 50.'
        ),
        DeclareLaunchArgument(
            'enable_magnetometer',
            default_value='true',
            description='Enable magnetometer in hardware_interface'
        ),
        DeclareLaunchArgument(
            'magnetometer_rate',
            default_value='100',
            description='Magnetometer rate in Hz.'
                'See datasheet Figure 6-16  page 50.'
        ),
        DeclareLaunchArgument(
            'enable_mock_mode',
            default_value='false',
            description='Use mock/simulation mode (no hardware required)'
        ),
        DeclareLaunchArgument(
            'publish_tf',
            default_value='true',
            description=(
                'Publish a dynamic world→base_link TF from IMU orientation for RViz visualization'
            ),
        ),
        DeclareLaunchArgument(
            'broadcast_magnetometer',
            default_value='true',
            description=(
                'Broadcast magnetic orientation'
                'Note: to be usefull also set enable_magnetometer to true'
            ),
        ),
    ]

    i2c_bus = LaunchConfiguration('i2c_bus')
    i2c_addr = LaunchConfiguration('i2c_addr')
    axis_remap = LaunchConfiguration('axis_remap')
    imu_rate = LaunchConfiguration('imu_rate')
    enable_magnetometer = LaunchConfiguration('enable_magnetometer')
    magnetometer_rate = LaunchConfiguration('magnetometer_rate')
    enable_mock = LaunchConfiguration('enable_mock_mode')
    publish_tf = LaunchConfiguration('publish_tf')
    broadcast_magnetometer = LaunchConfiguration('broadcast_magnetometer')

    # Get URDF via xacro
    robot_description_content = Command(
        [
            PathJoinSubstitution([FindExecutable(name='xacro')]),
            ' ',
            PathJoinSubstitution(
                [FindPackageShare('bno08x_try_hal_dynamic_covariance'), 'config', 'bno08x_hal_magnetometer.urdf.xacro']
            ),
            ' ',
            'i2c_bus:=', i2c_bus,
            ' ',
            'i2c_addr:=', i2c_addr,
            ' ',
            'axis_remap:=', axis_remap,
            ' ',
            'imu_rate:=', imu_rate,
            ' ',
            'enable_magnetometer:=', enable_magnetometer,
            ' ',
            'magnetometer_rate:=', magnetometer_rate,
            ' ',
            'enable_mock_mode:=', enable_mock,
        ]
    )
    robot_description = {
        'robot_description': ParameterValue(robot_description_content, value_type=str)
    }

    # Robot state publisher
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='both',
        parameters=[robot_description],
    )

    # Controller configuration
    controller_config = PathJoinSubstitution(
        [FindPackageShare('bno08x_try_hal_dynamic_covariance'), 'config', 'imu_hal_magnetometer_broadcaster.yaml']
    )

    # Controller manager (ros2_control_node)
    controller_manager_node = Node(
        package='controller_manager',
        executable='ros2_control_node',
        output='both',
        parameters=[robot_description, controller_config],
    )

    # state interface broadcaster spawner
    state_interfaces_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['state_interfaces_broadcaster', '--controller-manager', '/controller_manager'],
    )

    # IMU sensor broadcaster spawner
    imu_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['imu_sensor_broadcaster', '--controller-manager', '/controller_manager'],
    )

    # Optional: Magnetometer broadcaster spawner
    magnetometer_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['magnetometer_broadcaster', '--controller-manager', '/controller_manager'],
        condition=IfCondition(broadcast_magnetometer)
    )

    # Optional: relay IMU orientation to TF for RViz 3D visualization.
    # Set fixed frame to 'world' in RViz to see the sensor orientation animate.
    imu_hal_tf_broadcaster_node = Node(
        package='bno08x_try_hal_dynamic_covariance',
        executable='imu_hal_tf_broadcaster',
        name='imu_hal_tf_broadcaster',
        output='screen',
        condition=IfCondition(publish_tf),
    )

    return LaunchDescription(
        declared_arguments + [
            robot_state_publisher_node,
            controller_manager_node,
            state_interfaces_spawner,
            imu_broadcaster_spawner,
            magnetometer_broadcaster_spawner,
            imu_hal_tf_broadcaster_node,
        ]
    )
