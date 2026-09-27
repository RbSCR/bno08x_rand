cmake_minimum_required(VERSION 3.20)
project(bno08x_imu_msgs)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# = = = Messages = = =

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/ImuWithStatus.msg"
  "msg/MagneticFieldWithStatus.msg"
  DEPENDENCIES geometry_msgs std_msgs # Add packages that above messages depend on
)

if(BUILD_TESTING)

    # = = = Linting = = =
  find_package(ament_lint_auto REQUIRED)
  # Linters to be used in the 'auto-linting' are mentioned in package.xml

  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
