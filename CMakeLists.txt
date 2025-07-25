cmake_minimum_required(VERSION 3.8)
project(vrobot_base)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Set all libraries 3rd party libraries
file(GLOB 3RD_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/3rd/*/include
)
# GLOB tất cả file .so và .a trong thư mục 3rd/
file(GLOB 3RD_LIBRARIES_DIRS "3rd/*/lib/*.so" "3rd/*/lib/*.a")


# find dependencies
find_package(ament_cmake REQUIRED)


set(THIS_PACKAGE_DEPENDENCIES
    rclcpp
    std_msgs
    sensor_msgs
    geometry_msgs
    nav_msgs
    tf2_ros
    tf2_geometry_msgs
)
foreach(DEPENDENCY ${THIS_PACKAGE_DEPENDENCIES})
  find_package(${DEPENDENCY} REQUIRED)
endforeach()

include_directories(
    include
    ${3RD_INCLUDE_DIRS}
)

find_package(rosidl_default_generators REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/SetPID.srv"
  DEPENDENCIES std_msgs
)
ament_export_dependencies(rosidl_default_runtime)


add_executable(vrobot_base_node
    src/motor_esp.cpp
    src/motor_kinco.cpp
    src/odometry.cpp
    src/diff_controller.cpp
    src/vrobot_communication.cpp
)
target_link_libraries(vrobot_base_node
    ${3RD_LIBRARIES_DIRS}
)
ament_target_dependencies(vrobot_base_node
    ${THIS_PACKAGE_DEPENDENCIES}
)
rosidl_target_interfaces(vrobot_base_node
  ${PROJECT_NAME} "rosidl_typesupport_cpp")


install(TARGETS vrobot_base_node
   RUNTIME DESTINATION lib/${PROJECT_NAME} 
)

install(DIRECTORY include/
    DESTINATION include
)

ament_package()
