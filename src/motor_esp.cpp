#include "vrobot_base/motor_esp.hpp"
#include <cstdint>
#include <mutex>
#include <string>

namespace vrobot_base {

MotorESP::MotorESP(const std::string &port, uint32_t baudrate, int timeout)
    : timeout_(serial::Timeout::simpleTimeout(timeout)) {
  port_         = port;
  baudrate_     = baudrate;
  is_connected_ = false;
  is_active_    = false;

  if (port_.empty()) {
    RCLCPP_ERROR(kLoggerMotor, "Serial port is empty");
    throw std::invalid_argument("Serial port is empty");
  }
  if (baudrate_ == 0) {
    RCLCPP_ERROR(kLoggerMotor, "Baudrate is zero");
    throw std::invalid_argument("Baudrate is zero");
  }

  RCLCPP_INFO(kLoggerMotor, "MotorESP initialized with port: %s, baudrate: %u",
              port_.c_str(), baudrate_);
  RCLCPP_INFO(kLoggerMotor, "MotorESP initialized with timeout: %d ms",
              timeout);
  RCLCPP_INFO(kLoggerMotor, "MotorESP initialized with port: %s",
              port_.c_str());
  RCLCPP_INFO(kLoggerMotor, "MotorESP initialized with baudrate: %u",
              baudrate_);

}

MotorESP::~MotorESP() {
  is_active_ = false;

  if (read_thread_) {
    if (read_thread_->joinable()) {
      read_thread_->join();
    }
  }

  if (serial_port_) {
    try {
      serial_port_->close();
    } catch (const std::exception &e) {
      RCLCPP_ERROR(kLoggerMotor, "Failed to close serial port: %s", e.what());
    }
  }

  RCLCPP_INFO(kLoggerMotor, "MotorESP destroyed");
  is_connected_ = false;
}

bool MotorESP::Init() {
  try {
    serial_port_ = std::make_unique<serial::Serial>(port_, baudrate_, timeout_);
    if (!serial_port_->isOpen()) {
      RCLCPP_ERROR(kLoggerMotor, "Failed to open serial port");
      return false;
    }
    is_connected_ = true;
    is_active_    = true;

    read_thread_ = std::make_shared<std::thread>(&MotorESP::ReadThread, this);
  } catch (const std::exception &e) {
    RCLCPP_ERROR(kLoggerMotor, "Exception: %s", e.what());
    return false;
  }
  return true;
}

bool MotorESP::SetVelocity(int16_t left_velocity, int16_t right_velocity) {
  if (!is_connected_) {
    RCLCPP_ERROR(kLoggerMotor, "Motor is not connected");
    return false;
  }

  if (!is_active_) {
    RCLCPP_ERROR(kLoggerMotor, "Read thread is not active");
    return false;
  }

  std::vector<uint8_t> data = {CMD_SET_VELOCITY,
                                static_cast<uint8_t>(left_velocity & 0xFF),
                                static_cast<uint8_t>(left_velocity >> 8),
                                static_cast<uint8_t>(right_velocity & 0xFF),
                                static_cast<uint8_t>(right_velocity >> 8)};

    uint8_t checksum = CalculateChecksum(data);

    std::vector<uint8_t> command = {STX};
    command.insert(command.end(), data.begin(), data.end());
    command.push_back(checksum);
    command.push_back(ETX);
  
    return SendCommand(command);
  }

// bool MotorESP::SetPID(float kp, float ki, float kd, uint8_t motor) {
//   if (!is_connected_) {
//     RCLCPP_ERROR(kLoggerMotor, "Motor is not connected");
//     return false;
//   }

//   if (!is_active_) {
//     RCLCPP_ERROR(kLoggerMotor, "Read thread is not active");
//     return false;
//   }

//   if (motor != LEFT_MOTOR and motor != RIGHT_MOTOR) {
//     RCLCPP_ERROR(kLoggerMotor, "Invalid motor ID");
//     return false;
//   }

//   std::vector<uint8_t> data = {CMD_SET_PID,
//                                   static_cast<uint8_t>(kp),
//                                   static_cast<uint8_t>(ki),
//                                   static_cast<uint8_t>(kd),
//                                   static_cast<uint8_t>(motor)};


//   uint8_t checksum = CalculateChecksum(data);

//   std::vector<uint8_t> command = {STX};
//   command.insert(command.end(), data.begin(), data.end());
//   command.push_back(checksum);
//   command.push_back(ETX);

//   return SendCommand(command);
// }

bool MotorESP::SetPID(float kp, float ki, float kd, uint8_t motor_id) {
  std::vector<uint8_t> data = {0x02};  // CMD = 0x02

  // Chuyển float → 4 byte mỗi giá trị
  uint8_t* kp_bytes = reinterpret_cast<uint8_t*>(&kp);
  uint8_t* ki_bytes = reinterpret_cast<uint8_t*>(&ki);
  uint8_t* kd_bytes = reinterpret_cast<uint8_t*>(&kd);

  data.insert(data.end(), kp_bytes, kp_bytes + 4);
  data.insert(data.end(), ki_bytes, ki_bytes + 4);
  data.insert(data.end(), kd_bytes, kd_bytes + 4);
  data.push_back(motor_id);  // 14 byte payload

  // Tính checksum trên CMD + PAYLOAD
  uint8_t sum = 0;
  for (size_t i = 0; i < data.size(); ++i) sum += data[i];
  uint8_t checksum = (~sum + 1) & 0xFF;

  std::vector<uint8_t> frame = {0xEA};  // STX
  frame.insert(frame.end(), data.begin(), data.end());  // CMD + PAYLOAD
  frame.push_back(checksum);  // Checksum
  frame.push_back(0x3B);      // ETX

  // Gửi toàn bộ frame
  return SendCommand(frame);
}




bool MotorESP::GetPosition(int32_t &left_position, int32_t &right_position) {
  if (!is_connected_) {
    RCLCPP_ERROR(kLoggerMotor, "Motor is not connected");
    return false;
  }

  if (!is_active_) {
    RCLCPP_ERROR(kLoggerMotor, "Read thread is not active");
    return false;
  }

  left_position  = left_position_.load();
  right_position = right_position_.load();
  return true;
}

void MotorESP::StopMotor() { SetVelocity(0, 0); }

bool MotorESP::SendCommand(const std::vector<uint8_t> &data) {
  if (!is_connected_) {
    RCLCPP_ERROR(kLoggerMotor, "Motor is not connected");
    return false;
  }

  if (!is_active_) {
    RCLCPP_ERROR(kLoggerMotor, "Read thread is not active");
    return false;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  try {
    serial_port_->write(data);
#ifdef DEBUG
    // printData(data, "SendCommand------------------>");
#endif

  } catch (const std::exception &e) {
    RCLCPP_ERROR(kLoggerMotor, "Failed to send command: %s", e.what());
    return false;
  }
  return true;
}

void MotorESP::ReadThread() {
  while (is_active_) {
    if (!is_connected_) {
      RCLCPP_ERROR(kLoggerMotor, "Motor is not connected");
      return;
    }

    if (serial_port_) {
      uint8_t buffer = serial_port_->read(1)[0];

      if (buffer != STX) {
        continue;
      }

      uint8_t cmd = serial_port_->read(1)[0];
      switch (cmd) {

      case CMD_GET_POSITION: {
        std::vector<uint8_t> response(LEN_GET_POSITION - 2);
        serial_port_->read(response.data(), LEN_GET_POSITION - 2);

        if (response.size() !=( LEN_GET_POSITION - 2) and
            response[response.size() - 1] != ETX) {
          RCLCPP_ERROR(kLoggerMotor, "Failed to read response");
          continue;
        }

// #ifdef DEBUG
//           printData(response);
// #endif

        // L0 L1 L2 L3 R0 R1 R2 R3 CKS ETX

        left_position_ = int32_t(response[3]) << 24 |
                         int32_t(response[2]) << 16 |
                         int32_t(response[1]) << 8 | int32_t(response[0]);
        right_position_ = int32_t(response[7]) << 24 |
                          int32_t(response[6]) << 16 |
                          int32_t(response[5]) << 8 | int32_t(response[4]);

        std::vector<uint8_t> data;
        data.push_back(CMD_GET_POSITION);
        // push_back the response data (no ETX, no checksum)
        data.insert(data.end(), response.begin(), response.end() - 2);
        uint8_t checksum = CalculateChecksum(data);

        if (checksum != response[response.size() - 2]) {
          RCLCPP_ERROR(kLoggerMotor, "Checksum mismatch");
          continue;
        }
        break;
      }

      default:
        RCLCPP_ERROR(kLoggerMotor, "Unknown command: %02X", cmd & 0xFF);
        continue;
      }
    }
  }
}

void MotorESP::printData(const std::vector<uint8_t> &data, std::string prefix){
  std::string hex_string { ""};
  for (const auto &byte : data) {
    char byte_hex[4]; // Allocate enough space for the formatted string
    snprintf(byte_hex, sizeof(byte_hex), "%02X ", byte & 0xFF); 
    hex_string += byte_hex;
  }
  RCLCPP_INFO(kLoggerMotor, "%s[%ld]: %s",prefix.c_str(),hex_string.size(), hex_string.c_str());
}

uint8_t MotorESP::CalculateChecksum(const std::vector<uint8_t> &data){
  uint8_t checksum = 0;
  for (size_t i = 0; i < data.size(); ++i) {
    checksum += data[i];
  }
  checksum = (~checksum + 1) & 0xFF; // Two's complement
  return checksum;
}

} // namespace vrobot_base
