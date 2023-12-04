cmake_minimum_required(VERSION 3.8)
project(can_logger_pkg)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(can_msgs REQUIRED)
find_package(rosbag2_cpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(reader src/can_q.cpp)
ament_target_dependencies(reader rclcpp can_msgs std_msgs rosbag2_cpp)

install(TARGETS
	reader
  DESTINATION lib/${PROJECT_NAME})



ament_package()
