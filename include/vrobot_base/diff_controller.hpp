#pragma once

#include "rclcpp/rclcpp.hpp"
#include "vrobot_base/motor_kinco.hpp"
#include "vrobot_base/odometry.hpp"
#include "vrobot_base/params.hpp"

// MSG ROS2
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"

#include <memory>
#include <rclcpp/logger.hpp>
#include <rclcpp/timer.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/convert.h>
// #include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "std_msgs/msg/float32.hpp"
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

using geometry_msgs::msg::Twist;
using nav_msgs::msg::Odometry;

const std::string kOdomTopic   = "/odom";
const std::string kCmdVelTopic = "/cmd_vel_out";

namespace vrobot_base {
class DiffController : public rclcpp::Node {
public:
  DiffController();
  void update();

protected:
  void InitParameters();
  void InitPubSub();
  void InvKinematics(double linear, double angular, double &left_velocity,
                     double &right_velocity);
  void publishVelocityDebug();

private:
  void cmdVelCallback(const Twist::SharedPtr msg);
  void publishOdometry();

private:
  std::string                 serial_port_{"/dev/kinco"};
  int                         baudrate_{115200};
  std::unique_ptr<MotorKinco> motor_kinco_;
  OdometryVrobot              odometry_;

  // Timeout to consider cmd_vel commands old
  rclcpp::Duration cmd_vel_timeout_ = rclcpp::Duration::from_seconds(0.5);
  rclcpp::Time     last_cmd_vel_time_;

  rclcpp::Publisher<Odometry>::SharedPtr odometry_publisher_ = nullptr;
  rclcpp::Subscription<Twist>::SharedPtr cmd_vel_subscriber_ = nullptr;
  rclcpp::TimerBase::SharedPtr           timer_              = nullptr;

  rclcpp::Logger logger_ = rclcpp::get_logger("diff_controller");

  // Kinematic parameters
  double      linear_{0.0};
  double      angular_{0.0};
  double      wheel_radius_{0.08};
  double      base_separation_{0.502};
  double      gear_ratio_{30};
  double      pos_to_rad_{0.00062831853 / gear_ratio_}; // 1 / 1591.54943092
  double      vel_to_rpm_{2730.666666667 * 60.0 / (2 * M_PI) * gear_ratio_};
  std::string base_frame_id_ = "base_link";
  std::string odom_frame_id_ = "odom";

  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_vx_ref_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_wz_ref_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_vx_mea_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_wz_mea_;
};

} // namespace vrobot_base