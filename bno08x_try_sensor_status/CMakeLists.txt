cmake_minimum_required(VERSION 3.28)
project(bno08x_try_sensor_status)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(bno08x_sh2_libraries REQUIRED)
find_package(bno08x_imu_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# = = = Executable(s) = = =

add_executable(bno08x_try_sensor_status src/bno08x_try_sensor_status.cpp)

target_include_directories(bno08x_try_sensor_status PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

target_compile_features(bno08x_try_sensor_status PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

target_link_libraries(bno08x_try_sensor_status
  ${bno08x_sh2_libraries_TARGETS}
  ${rclcpp_TARGETS}
  ${bno08x_imu_msgs_TARGETS}
  ${geometry_msgs_TARGETS}
  ${std_msgs_TARGETS}
)

install(TARGETS bno08x_try_sensor_status
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)

  # = = = Linting = = =
  find_package(ament_lint_auto REQUIRED)
  # Linters to be used in the 'auto-linting' are mentioned in package.xml

  ament_lint_auto_find_test_dependencies()

  # = = = Tests = = =

endif()

ament_package()
