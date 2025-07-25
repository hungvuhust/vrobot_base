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

const rclcpp::Logger kLoggerMotorKinco =
    rclcpp::get_logger("vrobot_motor_kinco");

namespace vrobot_base {

class MotorKinco {
public:
  MotorKinco(const std::string &port, uint32_t baudrate, int timeout = 1000);
  ~MotorKinco();

  MotorKinco(const MotorKinco &)            = delete;
  MotorKinco &operator=(const MotorKinco &) = delete;

  bool Init();

public:
  bool SetVelocity(int32_t left_velocity, int32_t right_velocity);
  bool GetPosition(int32_t &left_position, int32_t &right_position);
  bool SetMode(uint8_t mode);
  bool GetMode(uint8_t &mode_left, uint8_t &mode_right);
  bool GetState(uint8_t &state_left, uint8_t &state_right);

  bool IsReady() const { return is_connected_ && is_active_; }
  void StopMotor();

private:
  bool    SetValue(uint8_t motor_id, uint8_t cmd, uint16_t index,
                   uint8_t subindex = 0x00, int32_t value = 0x00000000);
  bool    SendCommand(const std::vector<uint8_t> &data);
  bool    ReceiveResponse(std::vector<uint8_t> &response);
  void    ReadThread();
  void    StateThread();
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
  std::shared_ptr<std::thread> state_thread_;
  std::atomic<bool>            is_active_;
  std::mutex                   mutex_;

  // Object Dictionary
  ObjectDictionary od_left_;
  ObjectDictionary od_right_;
};

} // namespace vrobot_base