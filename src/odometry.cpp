#include <cmath>

#include "vrobot_base/odometry.hpp"

OdometryVrobot::OdometryVrobot(size_t velocity_rolling_window_size)
    : timestamp_(0.0), x_(0.0), y_(0.0), heading_(0.0), linear_(0.0),
      angular_(0.0), wheel_separation_(0.0), left_wheel_radius_(0.0),
      right_wheel_radius_(0.0), left_wheel_old_pos_(0.0),
      right_wheel_old_pos_(0.0),
      velocity_rolling_window_size_(velocity_rolling_window_size),
      linear_accumulator_(velocity_rolling_window_size),
      angular_accumulator_(velocity_rolling_window_size) {
  // Initialize with default differential drive config
  robot_config_.robot_type = RobotType::DIFFERENTIAL_DRIVE;
}

void OdometryVrobot::init(const rclcpp::Time &time) {
  // Reset accumulators and timestamp:
  resetAccumulators();
  timestamp_ = time;
}

bool OdometryVrobot::update(double left_pos, double right_pos,
                            const rclcpp::Time &time) {
  // We cannot estimate the speed with very small time intervals:
  const double dt = time.seconds() - timestamp_.seconds();
  if (dt < 0.001) {
    return false; // Interval too small to integrate with
  }

  if (!is_offset_set_) {
    offset_left_   = left_pos;
    offset_right_  = right_pos;
    is_offset_set_ = true;
  }

  // Get current wheel joint positions:
  const double left_wheel_cur_pos =
      (left_pos - offset_left_) * left_wheel_radius_;
  const double right_wheel_cur_pos =
      (right_pos - offset_right_) * right_wheel_radius_;

  // Estimate velocity of wheels using old and current position:
  const double left_wheel_est_vel  = left_wheel_cur_pos - left_wheel_old_pos_;
  const double right_wheel_est_vel = right_wheel_cur_pos - right_wheel_old_pos_;

  // Update old position with current:
  left_wheel_old_pos_  = left_wheel_cur_pos;
  right_wheel_old_pos_ = right_wheel_cur_pos;

  updateFromVelocity(left_wheel_est_vel, right_wheel_est_vel, time);

  return true;
}

bool OdometryVrobot::updateFromVelocity(double left_vel, double right_vel,
                                        const rclcpp::Time &time) {
  const double dt = time.seconds() - timestamp_.seconds();
  if (dt < 0.001) {
    return false; // Interval too small to integrate with
  }

  double linear, angular;

  // Use appropriate kinematics based on robot type
  switch (robot_config_.robot_type) {
  case RobotType::DIFFERENTIAL_DRIVE:
  default:
    computeDifferentialKinematics(left_vel, right_vel, linear, angular);
    break;

  case RobotType::MECANUM_WHEELS:
    // For mecanum, would need 4 wheel velocities
    // This is a simplified fallback to differential
    computeDifferentialKinematics(left_vel, right_vel, linear, angular);
    break;

    // Add other robot types as needed
  }

  // Integrate odometry:
  integrateExact(linear, angular);

  timestamp_ = time;

  // Estimate speeds using a rolling mean to filter them out:
  linear_accumulator_.accumulate(linear / dt);
  angular_accumulator_.accumulate(angular / dt);

  linear_  = linear_accumulator_.getRollingMean();
  angular_ = angular_accumulator_.getRollingMean();

  return true;
}

bool OdometryVrobot::updateMultiWheel(
    const std::vector<double> &wheel_positions,
    const std::vector<double> &wheel_velocities, const rclcpp::Time &time) {
  const double dt = time.seconds() - timestamp_.seconds();
  if (dt < 0.001) {
    return false;
  }

  double vx = 0.0, vy = 0.0, angular = 0.0;

  switch (robot_config_.robot_type) {
  case RobotType::MECANUM_WHEELS:
    if (wheel_velocities.size() >= 4) {
      computeMecanumKinematics(wheel_velocities, vx, vy, angular);
    }
    break;

  case RobotType::OMNIDIRECTIONAL:
    // Implementation for omnidirectional robots
    // This would require specific wheel configuration
    break;

  case RobotType::DIFFERENTIAL_DRIVE:
  default:
    if (wheel_velocities.size() >= 2) {
      computeDifferentialKinematics(wheel_velocities[0], wheel_velocities[1],
                                    vx, angular);
    }
    break;
  }

  // For now, use linear velocity (vx) as the main linear component
  integrateExact(vx, angular);

  timestamp_ = time;

  linear_accumulator_.accumulate(vx / dt);
  angular_accumulator_.accumulate(angular / dt);

  linear_  = linear_accumulator_.getRollingMean();
  angular_ = angular_accumulator_.getRollingMean();

  return true;
}

void OdometryVrobot::updateOpenLoop(double linear, double angular,
                                    const rclcpp::Time &time) {
  /// Save last linear and angular velocity:
  linear_  = linear;
  angular_ = angular;

  /// Integrate odometry:
  const double dt = time.seconds() - timestamp_.seconds();
  timestamp_      = time;
  integrateExact(linear * dt, angular * dt);
}

void OdometryVrobot::resetOdometry() {
  x_       = 0.0;
  y_       = 0.0;
  heading_ = 0.0;
}

void OdometryVrobot::setWheelParams(double wheel_separation,
                                    double left_wheel_radius,
                                    double right_wheel_radius) {
  wheel_separation_   = wheel_separation;
  left_wheel_radius_  = left_wheel_radius;
  right_wheel_radius_ = right_wheel_radius;

  // Also update the robot config for consistency
  robot_config_.robot_type         = RobotType::DIFFERENTIAL_DRIVE;
  robot_config_.wheel_separation   = wheel_separation;
  robot_config_.left_wheel_radius  = left_wheel_radius;
  robot_config_.right_wheel_radius = right_wheel_radius;
}

void OdometryVrobot::setRobotConfig(const RobotConfig &config) {
  robot_config_ = config;

  // Update legacy parameters for backward compatibility
  if (config.robot_type == RobotType::DIFFERENTIAL_DRIVE) {
    wheel_separation_   = config.wheel_separation;
    left_wheel_radius_  = config.left_wheel_radius;
    right_wheel_radius_ = config.right_wheel_radius;
  }

  // Initialize wheel arrays for multi-wheel robots
  if (config.wheel_radius.size() > 0) {
    wheel_old_positions_.resize(config.wheel_radius.size(), 0.0);
    wheel_offsets_.resize(config.wheel_radius.size(), 0.0);
  }
}

void OdometryVrobot::setVelocityRollingWindowSize(
    size_t velocity_rolling_window_size) {
  velocity_rolling_window_size_ = velocity_rolling_window_size;

  resetAccumulators();
}

void OdometryVrobot::computeDifferentialKinematics(double  left_vel,
                                                   double  right_vel,
                                                   double &linear,
                                                   double &angular) {
  // Compute linear and angular velocities for differential drive
  linear  = (left_vel + right_vel) * 0.5;
  angular = (left_vel - right_vel) / (robot_config_.wheel_separation > 0
                                          ? robot_config_.wheel_separation
                                          : wheel_separation_);
}

void OdometryVrobot::computeMecanumKinematics(
    const std::vector<double> &wheel_vels, double &vx, double &vy,
    double &angular) {
  // Mecanum wheel kinematics
  // Assumes wheel order: front-left, front-right, rear-left, rear-right
  if (wheel_vels.size() < 4)
    return;

  const double wheel_radius = robot_config_.wheel_radius.size() > 0
                                  ? robot_config_.wheel_radius[0]
                                  : 0.1;                      // Default radius
  const double lx           = robot_config_.wheel_base / 2.0; // Half wheelbase
  const double ly = robot_config_.track_width / 2.0; // Half track width

  // Forward kinematics for mecanum wheels
  vx = wheel_radius *
       (wheel_vels[0] + wheel_vels[1] + wheel_vels[2] + wheel_vels[3]) / 4.0;
  vy = wheel_radius *
       (-wheel_vels[0] + wheel_vels[1] + wheel_vels[2] - wheel_vels[3]) / 4.0;
  angular = wheel_radius *
            (-wheel_vels[0] + wheel_vels[1] - wheel_vels[2] + wheel_vels[3]) /
            (4.0 * (lx + ly));
}

void OdometryVrobot::integrateRungeKutta2(double linear, double angular) {
  const double direction = heading_ + angular * 0.5;

  /// Runge-Kutta 2nd order integration:
  x_ += linear * std::cos(direction);
  y_ += linear * std::sin(direction);
  heading_ += angular;
}

void OdometryVrobot::integrateExact(double linear, double angular) {
  if (fabs(angular) < 1e-6) {
    integrateRungeKutta2(linear, angular);
  } else {
    /// Exact integration (should solve problems when angular is zero):
    const double heading_old = heading_;
    const double r           = linear / angular;
    heading_ += angular;
    x_ += r * (std::sin(heading_) - std::sin(heading_old));
    y_ += -r * (std::cos(heading_) - std::cos(heading_old));
  }
}

void OdometryVrobot::resetAccumulators() {
  linear_accumulator_  = RollingMeanAccumulator(velocity_rolling_window_size_);
  angular_accumulator_ = RollingMeanAccumulator(velocity_rolling_window_size_);
}
