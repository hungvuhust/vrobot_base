#ifndef DIFF_DRIVE_CONTROLLER__ODOMETRY_HPP_
#define DIFF_DRIVE_CONTROLLER__ODOMETRY_HPP_

#include "rclcpp/time.hpp"
// \note The versions conditioning is added here to support the
// source-compatibility with Humble
#if RCPPUTILS_VERSION_MAJOR >= 2 && RCPPUTILS_VERSION_MINOR >= 6
#include "rcpputils/rolling_mean_accumulator.hpp"
#else
#include "rcppmath/rolling_mean_accumulator.hpp"
#endif

enum class RobotType {
  DIFFERENTIAL_DRIVE, // 2-wheel differential drive
  MECANUM_WHEELS,     // 4-wheel mecanum
  OMNIDIRECTIONAL,    // 3+ omniwheels
  ACKERMANN_STEERING  // Car-like steering
};

struct RobotConfig {
  RobotType robot_type = RobotType::DIFFERENTIAL_DRIVE;

  // Common parameters
  double wheel_base  = 0.0; // For differential/ackermann
  double track_width = 0.0; // For mecanum/omni

  // Wheel parameters
  std::vector<double> wheel_radius;    // Can have different sizes
  std::vector<double> wheel_positions; // Wheel positions for omni robots

  // Differential drive specific
  double wheel_separation   = 0.0;
  double left_wheel_radius  = 0.0;
  double right_wheel_radius = 0.0;
};

class OdometryVrobot {
public:
  explicit OdometryVrobot(size_t velocity_rolling_window_size = 10);

  void init(const rclcpp::Time &time);
  bool update(double left_pos, double right_pos, const rclcpp::Time &time);
  bool updateFromVelocity(double left_vel, double right_vel,
                          const rclcpp::Time &time);
  void updateOpenLoop(double linear, double angular, const rclcpp::Time &time);
  void resetOdometry();

  double getX() const { return x_; }
  double getY() const { return y_; }
  double getHeading() const { return heading_; }
  double getLinear() const { return linear_; }
  double getAngular() const { return angular_; }

  // Legacy method for backward compatibility
  void setWheelParams(double wheel_separation, double left_wheel_radius,
                      double right_wheel_radius);

  // New method for flexible robot configuration
  void setRobotConfig(const RobotConfig &config);

  void setVelocityRollingWindowSize(size_t velocity_rolling_window_size);

  // Multi-wheel support methods
  bool updateMultiWheel(const std::vector<double> &wheel_positions,
                        const std::vector<double> &wheel_velocities,
                        const rclcpp::Time        &time);

private:
// \note The versions conditioning is added here to support the
// source-compatibility with Humble
#if RCPPUTILS_VERSION_MAJOR >= 2 && RCPPUTILS_VERSION_MINOR >= 6
  using RollingMeanAccumulator = rcpputils::RollingMeanAccumulator<double>;
#else
  using RollingMeanAccumulator = rcppmath::RollingMeanAccumulator<double>;
#endif

  void integrateRungeKutta2(double linear, double angular);
  void integrateExact(double linear, double angular);
  void resetAccumulators();

  // Kinematics computation methods for different robot types
  void computeDifferentialKinematics(double left_vel, double right_vel,
                                     double &linear, double &angular);
  void computeMecanumKinematics(const std::vector<double> &wheel_vels,
                                double &vx, double &vy, double &angular);

  // Current timestamp:
  rclcpp::Time timestamp_;

  // Current pose:
  double x_;       //   [m]
  double y_;       //   [m]
  double heading_; // [rad]

  // Current velocity:
  double linear_;  //   [m/s]
  double angular_; // [rad/s]

  // Robot configuration
  RobotConfig robot_config_;

  // Legacy wheel kinematic parameters [m] - for backward compatibility:
  double wheel_separation_;
  double left_wheel_radius_;
  double right_wheel_radius_;

  // Previous wheel position/state [rad]:
  double left_wheel_old_pos_;
  double right_wheel_old_pos_;
  double offset_left_{0.0};
  double offset_right_{0.0};
  bool   is_offset_set_{false};

  // Multi-wheel support
  std::vector<double> wheel_old_positions_;
  std::vector<double> wheel_offsets_;

  // Rolling mean accumulators for the linear and angular velocities:
  size_t                 velocity_rolling_window_size_;
  RollingMeanAccumulator linear_accumulator_;
  RollingMeanAccumulator angular_accumulator_;
};

#endif // DIFF_DRIVE_CONTROLLER__ODOMETRY_HPP_