#include "vrobot_base/motor_kinco.hpp"
#include "vrobot_base/params.hpp"
#include <cstdint>
#include <mutex>
#include <string>

namespace vrobot_base {

MotorKinco::MotorKinco(const std::string &port, uint32_t baudrate, int timeout)
    : timeout_(serial::Timeout::simpleTimeout(timeout)) {
  port_         = port;
  baudrate_     = baudrate;
  is_connected_ = false;
  is_active_    = false;

  if (port_.empty()) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Serial port is empty");
    throw std::invalid_argument("Serial port is empty");
  }
  if (baudrate_ == 0) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Baudrate is zero");
    throw std::invalid_argument("Baudrate is zero");
  }

  RCLCPP_INFO(kLoggerMotorKinco,
              "MotorKinco initialized with port: %s, baudrate: %u",
              port_.c_str(), baudrate_);
  RCLCPP_INFO(kLoggerMotorKinco, "MotorKinco initialized with timeout: %d ms",
              timeout);
  RCLCPP_INFO(kLoggerMotorKinco, "MotorKinco initialized with port: %s",
              port_.c_str());
  RCLCPP_INFO(kLoggerMotorKinco, "MotorKinco initialized with baudrate: %u",
              baudrate_);
}

MotorKinco::~MotorKinco() {
  is_active_ = false;

  if (read_thread_) {
    if (read_thread_->joinable()) {
      read_thread_->join();
    }
  }

  if (state_thread_) {
    if (state_thread_->joinable()) {
      state_thread_->join();
    }
  }

  if (serial_port_) {
    try {
      serial_port_->close();
    } catch (const std::exception &e) {
      RCLCPP_ERROR(kLoggerMotorKinco, "Failed to close serial port: %s",
                   e.what());
    }
  }

  RCLCPP_INFO(kLoggerMotorKinco, "MotorKinco destroyed");
  is_connected_ = false;
}

bool MotorKinco::Init() {
  try {
    serial_port_ = std::make_unique<serial::Serial>(port_, baudrate_, timeout_);
    if (!serial_port_->isOpen()) {
      RCLCPP_ERROR(kLoggerMotorKinco, "Failed to open serial port");
      return false;
    }
    is_connected_ = true;
    is_active_    = true;

    read_thread_ = std::make_shared<std::thread>(&MotorKinco::ReadThread, this);
    state_thread_ =
        std::make_shared<std::thread>(&MotorKinco::StateThread, this);
  } catch (const std::exception &e) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Exception: %s", e.what());
    return false;
  }
  return true;
}

bool MotorKinco::SetVelocity(int32_t left_velocity, int32_t right_velocity) {
  left_velocity_  = left_velocity;
  right_velocity_ = right_velocity;
  return true;
}

bool MotorKinco::SetValue(uint8_t motor_id, uint8_t cmd, uint16_t index,
                          uint8_t subindex, int32_t value) {
  if (!is_connected_) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Motor is not connected");
    return false;
  }

  if (!is_active_) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Read thread is not active");
    return false;
  }

  sdo_frame_t frame;
  frame.id                   = motor_id;
  frame.cmd                  = cmd;
  frame.index                = index;
  frame.subindex             = subindex;
  frame.data                 = value;
  auto                 bytes = frame.toBytes();
  std::vector<uint8_t> command(bytes.begin(), bytes.end());
  return SendCommand(command);
}

bool MotorKinco::GetPosition(int32_t &left_position, int32_t &right_position) {
  if (!is_connected_) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Motor is not connected");
    return false;
  }

  if (!is_active_) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Read thread is not active");
    return false;
  }

  uint32_t left_position_tmp;
  uint32_t right_position_tmp;

  od_left_.get(0x6064, 0x00, left_position_tmp);
  od_right_.get(0x6064, 0x00, right_position_tmp);

  left_position  = static_cast<int32_t>(left_position_tmp);
  right_position = static_cast<int32_t>(right_position_tmp);

  return true;
}

void MotorKinco::StopMotor() {
  SetValue(LEFT_MOTOR, WRITE_4, 0x60FF, 0x00, 0x00000000);
  SetValue(RIGHT_MOTOR, WRITE_4, 0x60FF, 0x00, 0x00000000);
}

bool MotorKinco::SendCommand(const std::vector<uint8_t> &data) {
  if (!is_connected_) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Motor is not connected");
    return false;
  }

  if (!is_active_) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Read thread is not active");
    return false;
  }

  try {
    std::lock_guard<std::mutex> lock(mutex_);
    serial_port_->write(data);
  } catch (const std::exception &e) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Failed to send command: %s", e.what());
    return false;
  }
  return true;
}

void MotorKinco::ReadThread() {
  while (is_active_) {
    if (!is_connected_) {
      RCLCPP_ERROR(kLoggerMotorKinco, "Motor is not connected");
      return;
    }
    std::vector<uint8_t> buffer(10);

    {
      std::lock_guard<std::mutex> lock(mutex_);
      while (serial_port_->available() >= 10) {
        serial_port_->read(buffer.data(), 10);
        printData(buffer, "Received");

        sdo_frame_t frame;
        if (sdo_frame_t::parse(buffer.data(), frame)) {
          RCLCPP_DEBUG(
              kLoggerMotorKinco,
              "Received frame[%02X]: %02X %02X %02X %02X %02X %02X %02X",
              frame.id & 0xFF, (frame.index >> 8) & 0xFF, frame.index & 0xFF,
              frame.subindex & 0xFF, frame.data & 0xFF,
              (frame.data >> 8) & 0xFF, (frame.data >> 16) & 0xFF,
              (frame.data >> 24) & 0xFF);
          if (frame.cmd != 0x80) {
            if (frame.id == 0x01) {
              od_left_.update(frame.index, frame.subindex, frame.data);
            } else if (frame.id == 0x02) {
              od_right_.update(frame.index, frame.subindex, frame.data);
            }
          } else {
            RCLCPP_ERROR(kLoggerMotorKinco,
                         "ERROR[%02X]: %02X %02X %02X %02X %02X %02X %02X",
                         frame.id & 0xFF, // ID
                         (frame.index >> 8) & 0xFF, frame.index & 0xFF,
                         frame.subindex & 0xFF, frame.data & 0xFF,
                         (frame.data >> 8) & 0xFF, (frame.data >> 16) & 0xFF,
                         (frame.data >> 24) & 0xFF);
          }
        } else {
          RCLCPP_ERROR(kLoggerMotorKinco, "Failed to parse frame");
        }
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}

void MotorKinco::StateThread() {
  while (is_active_) {
    if (!is_connected_) {
      RCLCPP_ERROR(kLoggerMotorKinco, "Motor is not connected");
      continue;
    }

    // Polling state of the motor by SDO
    // Position
    SetValue(LEFT_MOTOR, READ, 0x6064, 0x00, 0x00000000);
    SetValue(RIGHT_MOTOR, READ, 0x6064, 0x00, 0x00000000);
    // // State
    // SetValue(LEFT_MOTOR, READ, 0x6041, 0x00, 0x00000000);
    // SetValue(RIGHT_MOTOR, READ, 0x6041, 0x00, 0x00000000);
    // // Operation mode
    // SetValue(LEFT_MOTOR, READ, 0x6061, 0x00, 0x00000000);
    // SetValue(RIGHT_MOTOR, READ, 0x6061, 0x00, 0x00000000);

    // Set velocity
    SetValue(LEFT_MOTOR, WRITE_4, 0x60FF, 0x00, (left_velocity_ & 0xFFFF));
    SetValue(RIGHT_MOTOR, WRITE_4, 0x60FF, 0x00, (right_velocity_ & 0xFFFF));

    // LogObjectDictionary();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

bool MotorKinco::SetMode(uint8_t mode) {
  try {
    SetValue(LEFT_MOTOR, WRITE_1, 0x6060, 0x00, mode);
    SetValue(RIGHT_MOTOR, WRITE_1, 0x6060, 0x00, mode);
  } catch (const std::exception &e) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Failed to set mode: %s", e.what());
    return false;
  }
  return true;
}

bool MotorKinco::GetMode(uint8_t &mode_left, uint8_t &mode_right) {
  try {
    uint32_t mode_left_tmp;
    uint32_t mode_right_tmp;
    od_left_.get(0x6060, 0x00, mode_left_tmp);
    od_right_.get(0x6060, 0x00, mode_right_tmp);
    mode_left  = static_cast<uint8_t>(mode_left_tmp & 0xFF);
    mode_right = static_cast<uint8_t>(mode_right_tmp & 0xFF);
  } catch (const std::exception &e) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Failed to get mode: %s", e.what());
    return false;
  }
  return true;
}

bool MotorKinco::GetState(uint8_t &state_left, uint8_t &state_right) {
  try {
    uint32_t state_left_tmp;
    uint32_t state_right_tmp;
    od_left_.get(0x6041, 0x00, state_left_tmp);
    od_right_.get(0x6041, 0x00, state_right_tmp);
    state_left  = static_cast<uint8_t>(state_left_tmp & 0xFF);
    state_right = static_cast<uint8_t>(state_right_tmp & 0xFF);
  } catch (const std::exception &e) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Failed to get state: %s", e.what());
    return false;
  }
  return true;
}

bool MotorKinco::SetControlWord(uint16_t control_word) {
  try {
    SetValue(LEFT_MOTOR, WRITE_2, 0x6040, 0x00, control_word);
    SetValue(RIGHT_MOTOR, WRITE_2, 0x6040, 0x00, control_word);
  } catch (const std::exception &e) {
    RCLCPP_ERROR(kLoggerMotorKinco, "Failed to set control word: %s", e.what());
    return false;
  }
  return true;
}

void MotorKinco::printData(const std::vector<uint8_t> &data,
                           std::string                 prefix) {
  std::string hex_string{""};
  for (const auto &byte : data) {
    char byte_hex[4]; // Allocate enough space for the formatted string
    snprintf(byte_hex, sizeof(byte_hex), "%02X ", byte & 0xFF);
    hex_string += byte_hex;
  }
  RCLCPP_INFO(kLoggerMotorKinco, "%s[%ld]: %s", prefix.c_str(), data.size(),
              hex_string.c_str());
}

uint8_t MotorKinco::CalculateChecksum(const std::vector<uint8_t> &data) {
  uint16_t sum = 0;
  for (uint8_t byte : data) {
    sum += byte;
  }
  return static_cast<uint8_t>(-sum);
}

void MotorKinco::LogObjectDictionary() {
  od_left_.print("Left Motor");
  od_right_.print("Right Motor");
}

} // namespace vrobot_base
