from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('bno08x_try_sensor_status'),
        'config',
        'bno085_try.yaml'
    )

    return LaunchDescription([
        Node(
            package='bno08x_try_sensor_status',
            executable='bno08x_try_sensor_status',
            name='bno08x_try_sensor_status',
            output='screen',
            parameters=[config]
        ),
    ])

