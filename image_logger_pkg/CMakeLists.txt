cmake_minimum_required(VERSION 3.8)
project(image_logger_pkg)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rosbag2_cpp REQUIRED)

add_executable(reader src/image_q.cpp)
ament_target_dependencies(reader rclcpp sensor_msgs rosbag2_cpp)

install(TARGETS
	reader
  DESTINATION lib/${PROJECT_NAME})



ament_package()
