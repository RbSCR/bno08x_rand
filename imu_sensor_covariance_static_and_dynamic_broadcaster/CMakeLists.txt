cmake_minimum_required(VERSION 3.16)
project(imu_sensor_covariance_static_and_dynamic_broadcaster LANGUAGES CXX)

find_package(ros2_control_cmake REQUIRED)
set_compiler_options()
export_windows_symbols()

# using this instead of visibility macros
# S1 from https://github.com/ros-controls/ros2_controllers/issues/1053
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(THIS_PACKAGE_INCLUDE_DEPENDS
  controller_interface
  Eigen3
  generate_parameter_library
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
  realtime_tools
  sensor_msgs
)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_gen_version_h REQUIRED)
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()


generate_parameter_library(imu_sensor_covariance_static_and_dynamic_broadcaster_parameters
  src/imu_sensor_covariance_static_and_dynamic_broadcaster_parameters.yaml
)

add_library(imu_sensor_covariance_static_and_dynamic_broadcaster SHARED
  src/imu_sensor_covariance_static_and_dynamic_broadcaster.cpp
)

target_compile_features(imu_sensor_covariance_static_and_dynamic_broadcaster PUBLIC cxx_std_17)
target_include_directories(imu_sensor_covariance_static_and_dynamic_broadcaster PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/imu_sensor_covariance_static_and_dynamic_broadcaster>
)

target_link_libraries(imu_sensor_covariance_static_and_dynamic_broadcaster PUBLIC
  imu_sensor_covariance_static_and_dynamic_broadcaster_parameters
  controller_interface::controller_interface
  Eigen3::Eigen
  hardware_interface::hardware_interface
  pluginlib::pluginlib
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
  realtime_tools::realtime_tools
  ${sensor_msgs_TARGETS}
)


pluginlib_export_plugin_description_file(
  controller_interface imu_sensor_covariance_static_and_dynamic_broadcaster.xml)

install(
  DIRECTORY include/
  DESTINATION include/imu_sensor_covariance_static_and_dynamic_broadcaster
)
install(
  TARGETS
    imu_sensor_covariance_static_and_dynamic_broadcaster
    imu_sensor_covariance_static_and_dynamic_broadcaster_parameters
  EXPORT export_imu_sensor_covariance_static_and_dynamic_broadcaster
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
)

if(BUILD_TESTING)
  find_package(ament_cmake_gmock REQUIRED)
  find_package(controller_interface REQUIRED)
  find_package(controller_manager REQUIRED)
  find_package(hardware_interface REQUIRED)
  find_package(ros2_control_test_assets REQUIRED)

  add_definitions(-DTEST_FILES_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/test")
  ament_add_gmock(test_load_imu_sensor_covariance_static_and_dynamic_broadcaster
    test/test_load_imu_sensor_covariance_static_and_dynamic_broadcaster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/imu_sensor_covariance_static_and_dynamic_broadcaster_params.yaml
  )
  target_include_directories(test_load_imu_sensor_covariance_static_and_dynamic_broadcaster PRIVATE include)
  target_link_libraries(test_load_imu_sensor_covariance_static_and_dynamic_broadcaster
    imu_sensor_covariance_static_and_dynamic_broadcaster
    controller_manager::controller_manager
    ros2_control_test_assets::ros2_control_test_assets
  )

  ament_add_gmock(test_imu_sensor_covariance_static_and_dynamic_broadcaster
    test/test_imu_sensor_covariance_static_and_dynamic_broadcaster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/imu_sensor_covariance_static_and_dynamic_broadcaster_params.yaml
  )
  target_include_directories(test_imu_sensor_covariance_static_and_dynamic_broadcaster PRIVATE include)
  target_link_libraries(test_imu_sensor_covariance_static_and_dynamic_broadcaster
    imu_sensor_covariance_static_and_dynamic_broadcaster
    ros2_control_test_assets::ros2_control_test_assets
  )

  ament_add_gmock(test_imu_sensor_covariance_semantic_component
    test/test_imu_sensor_covariance_semantic_component.cpp
  )
  target_include_directories(test_imu_sensor_covariance_semantic_component PRIVATE include)
  target_link_libraries(test_imu_sensor_covariance_semantic_component
    controller_interface::controller_interface
    hardware_interface::hardware_interface
    ${sensor_msgs_TARGETS}
  )

endif()

ament_export_targets(export_imu_sensor_covariance_static_and_dynamic_broadcaster HAS_LIBRARY_TARGET)
ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS})
ament_package()
ament_generate_version_header(${PROJECT_NAME})
