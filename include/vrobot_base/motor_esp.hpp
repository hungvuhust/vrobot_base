#pragma once

#include "rclcpp/rclcpp.hpp"
#include "vrobot_base/params.hpp"
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <memory>
#include <serial/serial.h>
#include <string>
#include <thread>
#include <vector>

#define DEBUG

const rclcpp::Logger kLoggerMotor = rclcpp::get_logger("vrobot_motor");

namespace vrobot_base {

class MotorESP {
public:
  MotorESP(const std::string &port, uint32_t baudrate, int timeout = 1000);
  ~MotorESP();

  MotorESP(const MotorESP &)            = delete;
  MotorESP &operator=(const MotorESP &) = delete;

  bool Init();

public:
  bool SetVelocity(int16_t left_velocity, int16_t right_velocity);
  bool SetPID(float kp, float ki, float kd, uint8_t motor);
  bool GetPosition(int32_t &left_position, int32_t &right_position);
  bool IsReady() const { return is_connected_ && is_active_; }
  void StopMotor();

private:
  bool    SendCommand(const std::vector<uint8_t> &data);
  bool    ReceiveResponse(std::vector<uint8_t> &response);
  void    ReadThread();
  void    printData(const std::vector<uint8_t> &data, std::string prefix = "");
  uint8_t CalculateChecksum(const std::vector<uint8_t> &data);

private:
  // Serial port settings
  serial::Timeout                 timeout_;
  std::string                     port_;
  uint32_t                        baudrate_;
  std::atomic<bool>               is_connected_;
  std::unique_ptr<serial::Serial> serial_port_;

  // Thread for reading data
  std::shared_ptr<std::thread> read_thread_;
  std::atomic<bool>            is_active_;
  std::mutex                   mutex_;

  //   Data
  std::atomic_int32_t left_position_{0};
  std::atomic_int32_t right_position_{0};
};

} // namespace vrobot_base