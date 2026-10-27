cmake_minimum_required(VERSION 3.16)
project(magnetometer_covariance_broadcaster)

find_package(ros2_control_cmake REQUIRED)
set_compiler_options()
export_windows_symbols()

# TODO update with depend for semantic component?
# fmt pal_statistics
set(THIS_PACKAGE_INCLUDE_DEPENDS
  controller_interface
  generate_parameter_library
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
  realtime_tools
  sensor_msgs
)

find_package(ament_cmake REQUIRED)
find_package(backward_ros REQUIRED)
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()


generate_parameter_library(magnetometer_covariance_broadcaster_parameters
  src/magnetometer_covariance_broadcaster_parameters.yaml
)

add_library(magnetometer_covariance_broadcaster SHARED
  src/magnetometer_covariance_broadcaster.cpp
)
target_compile_features(magnetometer_covariance_broadcaster PUBLIC cxx_std_20)
target_include_directories(magnetometer_covariance_broadcaster PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/magnetometer_covariance_broadcaster>
)
target_link_libraries(magnetometer_covariance_broadcaster PUBLIC
  magnetometer_covariance_broadcaster_parameters
  controller_interface::controller_interface
  hardware_interface::hardware_interface
  pluginlib::pluginlib
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
  realtime_tools::realtime_tools
  ${sensor_msgs_TARGETS}
)

pluginlib_export_plugin_description_file(
  controller_interface magnetometer_covariance_broadcaster.xml
)

if(BUILD_TESTING)
  find_package(ament_cmake_gmock REQUIRED)
  find_package(controller_interface REQUIRED)
  find_package(controller_manager REQUIRED)
  find_package(hardware_interface REQUIRED)
  find_package(ros2_control_test_assets REQUIRED)
  find_package(generate_parameter_library REQUIRED)

  add_definitions(-DTEST_FILES_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/test")

  ament_add_gmock(test_magnetic_field_sensor_covariance_semantic_component
    test/test_magnetic_field_sensor_covariance_semantic_component.cpp
  )
  target_include_directories(test_magnetic_field_sensor_covariance_semantic_component PRIVATE include)
  target_link_libraries(test_magnetic_field_sensor_covariance_semantic_component
    controller_interface::controller_interface
    hardware_interface::hardware_interface
    ${sensor_msgs_TARGETS}
  )

  ament_add_gmock(test_load_magnetometer_covariance_broadcaster
    test/test_load_magnetometer_covariance_broadcaster.cpp
  )
  target_link_libraries(test_load_magnetometer_covariance_broadcaster
    magnetometer_covariance_broadcaster
    controller_manager::controller_manager
    ros2_control_test_assets::ros2_control_test_assets
  )

  ament_add_gmock(test_magnetometer_covariance_broadcaster
    test/test_magnetometer_covariance_broadcaster.cpp
  )
  target_link_libraries(test_magnetometer_covariance_broadcaster
    magnetometer_covariance_broadcaster
    ros2_control_test_assets::ros2_control_test_assets
  )
endif()

install(
  DIRECTORY include/
  DESTINATION include/magnetometer_covariance_broadcaster
)

install(
  TARGETS
    magnetometer_covariance_broadcaster
    magnetometer_covariance_broadcaster_parameters
  EXPORT export_magnetometer_covariance_broadcaster
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
)

ament_export_targets(export_magnetometer_covariance_broadcaster HAS_LIBRARY_TARGET)
ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS})
ament_package()
