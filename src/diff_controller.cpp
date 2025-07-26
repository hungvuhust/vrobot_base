#include "vrobot_base/diff_controller.hpp"
#include "vrobot_base/srv/set_pid.hpp"
#include <cstdint>
#include <std_msgs/msg/float32.hpp>
using SetPID = vrobot_base::srv::SetPID;

namespace vrobot_base {

DiffController::DiffController() : Node("diff_controller") {

  // Initialize parameters
  InitParameters();

  // Initialize motor
  motor_kinco_ = std::make_unique<MotorKinco>(serial_port_, baudrate_, 1000);

  if (!motor_kinco_->Init()) {
    RCLCPP_ERROR(logger_, "Failed to initialize motor");
    throw std::runtime_error("Failed to initialize motor");
  }

  motor_kinco_->SetMode(static_cast<uint8_t>(-3));
  motor_kinco_->SetControlWord(0x0006);
  motor_kinco_->SetControlWord(0x0007);
  motor_kinco_->SetControlWord(0x000F);
  motor_kinco_->ResetPosition();

  // Initialize odometry
  odometry_.setWheelParams(base_separation_, wheel_radius_, wheel_radius_);
  odometry_.resetOdometry();
  odometry_.setVelocityRollingWindowSize(10);

  // Set the initial timestamp
  last_cmd_vel_time_ = this->now();
  // Initialize publisher and subscriber
  InitPubSub();
}

void DiffController::InitPubSub() {
  // Create a publisher for odometry
  odometry_publisher_ = this->create_publisher<Odometry>(kOdomTopic, 10);

  // Create a subscriber for cmd_vel
  cmd_vel_subscriber_ = this->create_subscription<Twist>(
      kCmdVelTopic, 10,
      std::bind(&DiffController::cmdVelCallback, this, std::placeholders::_1));

  // Create a timer to call the update function at a fixed rate
  timer_ = this->create_wall_timer(std::chrono::milliseconds(50),
                                   std::bind(&DiffController::update, this));

  pub_vx_ref_ = this->create_publisher<std_msgs::msg::Float32>("vx_ref", 10);
  pub_wz_ref_ = this->create_publisher<std_msgs::msg::Float32>("wz_ref", 10);
  pub_vx_mea_ = this->create_publisher<std_msgs::msg::Float32>("vx_mea", 10);
  pub_wz_mea_ = this->create_publisher<std_msgs::msg::Float32>("wz_mea", 10);
}

void DiffController::InitParameters() {
  // Declare parameters
  this->declare_parameter("wheel_radius", wheel_radius_);
  this->declare_parameter("gear_ratio", gear_ratio_);
  this->declare_parameter("base_separation", base_separation_);
  this->declare_parameter("serial_port", serial_port_);
  this->declare_parameter("baudrate", baudrate_);

  // Get parameters
  this->get_parameter("wheel_radius", wheel_radius_);
  this->get_parameter("gear_ratio", gear_ratio_);
  this->get_parameter("base_separation", base_separation_);
  this->get_parameter("serial_port", serial_port_);
  this->get_parameter("baudrate", baudrate_);

  RCLCPP_INFO(logger_, "Wheel radius: %f", wheel_radius_);
  RCLCPP_INFO(logger_, "Base separation: %f", base_separation_);
  RCLCPP_INFO(logger_, "Serial port: %s", serial_port_.c_str());
  RCLCPP_INFO(logger_, "Baudrate: %d", baudrate_);
  RCLCPP_INFO(logger_, "Gear ratio: %f", gear_ratio_);

  RCLCPP_INFO(logger_, "Parameters initialized successfully.");
}

void DiffController::cmdVelCallback(const Twist::SharedPtr msg) {
  // Update the last command time
  last_cmd_vel_time_ = this->now();

  // Set the linear and angular velocities
  linear_  = msg->linear.x;
  angular_ = msg->angular.z;
  // publishVelocityDebug();
}

void DiffController::InvKinematics(double linear, double angular,
                                   double &left_velocity,
                                   double &right_velocity) {
  left_velocity  = (linear + angular * base_separation_ / 2.0) / wheel_radius_;
  right_velocity = (linear - angular * base_separation_ / 2.0) / wheel_radius_;
}

void DiffController::publishOdometry() {

  // Create the odometry message
  Odometry odom_msg;
  odom_msg.header.stamp    = this->now();
  odom_msg.header.frame_id = odom_frame_id_;
  odom_msg.child_frame_id  = base_frame_id_;

  // Set the position and orientation
  odom_msg.pose.pose.position.x = odometry_.getX();
  odom_msg.pose.pose.position.y = odometry_.getY();
  odom_msg.pose.pose.position.z = 0.0;

  // Set the orientation (quaternion)
  tf2::Quaternion q;
  q.setRPY(0, 0, odometry_.getHeading());
  odom_msg.pose.pose.orientation.x = q.x();
  odom_msg.pose.pose.orientation.y = q.y();
  odom_msg.pose.pose.orientation.z = q.z();
  odom_msg.pose.pose.orientation.w = q.w();

  // Set the linear and angular velocities
  odom_msg.twist.twist.linear.x  = odometry_.getLinear();
  odom_msg.twist.twist.angular.z = odometry_.getAngular();

  // Publish the odometry message
  odometry_publisher_->publish(odom_msg);
  // publishVelocityDebug();
}

void DiffController::update() {
  // Check if the command is old
  if (this->now() - last_cmd_vel_time_ > cmd_vel_timeout_) {
    RCLCPP_DEBUG(logger_, "Command is old, stopping the robot");
    linear_  = 0.0;
    angular_ = 0.0;
  }

  if (motor_kinco_ == nullptr) {
    RCLCPP_ERROR(logger_, "Motor is not initialized");
    return;
  }
  if (!motor_kinco_->IsReady()) {
    RCLCPP_ERROR(logger_, "Motor is not connected");
    return;
  }

  // motor_kinco_->LogObjectDictionary();

  // Calculate the left and right velocities
  double left_velocity, right_velocity;
  InvKinematics(linear_, angular_, left_velocity, right_velocity);

  // RCLCPP_DEBUG(logger_, "L: %f, R: %f", left_velocity, right_velocity);
  int32_t left_velocity_cmd  = int32_t(-left_velocity * vel_to_rpm_);
  int32_t right_velocity_cmd = int32_t(right_velocity * vel_to_rpm_);

  RCLCPP_INFO(logger_, "Left velocity: %d, Right velocity: %d",
               left_velocity_cmd, right_velocity_cmd);

  // Set the motor velocities
  if (!motor_kinco_->SetVelocity(left_velocity_cmd, right_velocity_cmd)) {
    RCLCPP_ERROR(logger_, "Failed to set motor velocity");
    return;
  }

  // Update odometry
  int32_t left_position_feedback{0}, right_position_feedback{0};
  if (!motor_kinco_->GetPosition(left_position_feedback,
                                 right_position_feedback)) {
    RCLCPP_ERROR(logger_, "Failed to get motor position");
    return;
  }
  double left_position =
      static_cast<double>(-left_position_feedback) * pos_to_rad_;
  double right_position =
      static_cast<double>(right_position_feedback) * pos_to_rad_;
  if (odometry_.update(right_position, left_position, this->now())) {
    publishOdometry();
  }
}

void DiffController::publishVelocityDebug() {
  std_msgs::msg::Float32 msg_vx_ref;
  std_msgs::msg::Float32 msg_wz_ref;
  std_msgs::msg::Float32 msg_vx_mea;
  std_msgs::msg::Float32 msg_wz_mea;

  msg_vx_ref.data = linear_;
  msg_wz_ref.data = angular_;
  msg_vx_mea.data = odometry_.getLinear();  // Vx_mea
  msg_wz_mea.data = odometry_.getAngular(); // Wz_mea

  pub_vx_ref_->publish(msg_vx_ref);
  pub_wz_ref_->publish(msg_wz_ref);
  pub_vx_mea_->publish(msg_vx_mea);
  pub_wz_mea_->publish(msg_wz_mea);
}

} // namespace vrobot_base