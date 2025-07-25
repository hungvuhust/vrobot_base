#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

const uint8_t STX = 0xEA;
const uint8_t ETX = 0x3B;

const size_t LEN_GET_POSITION = 12;

typedef enum MOTOR_ID {
  LEFT_MOTOR  = 0x01,
  RIGHT_MOTOR = 0x02,
} motor_id_t;

typedef enum COMMAND_REQUEST {
  CMD_SET_VELOCITY = 0x01,
  CMD_SET_PID      = 0x02,
} cmd_request_t;

typedef enum COMMAND_RESPONSE {
  CMD_GET_POSITION = 0x12,
} cmd_response_t;

typedef enum COMMAND_SDO {
  READ    = 0x40, // read
  WRITE_1 = 0x2F, // write 1 byte
  WRITE_2 = 0x2B, // write 2 bytes (16 bits)
  WRITE_4 = 0x23, // write 4 bytes (32 bits)
} command_sdo_t;

typedef struct SDOFrame {
  uint8_t  id;       // node ID
  uint8_t  cmd;      // command: 0x40 = read, 0x2B/0x2F/0x23 = write
  uint16_t index;    // index
  uint8_t  subindex; // subindex
  int32_t  data;     // data

  std::array<uint8_t, 10> toBytes() const {
    std::array<uint8_t, 10> bytes;
    bytes[0] = id;
    bytes[1] = cmd;
    bytes[2] = index & 0xFF;
    bytes[3] = (index >> 8) & 0xFF;
    bytes[4] = subindex;
    bytes[5] = data & 0xFF;
    bytes[6] = (data >> 8) & 0xFF;
    bytes[7] = (data >> 16) & 0xFF;
    bytes[8] = (data >> 24) & 0xFF;

    std::array<uint8_t, 9> frame_for_checksum;
    std::copy(bytes.begin(), bytes.begin() + 9, frame_for_checksum.begin());

    bytes[9] = calculateChecksum(frame_for_checksum);

    return bytes;
  }

  static uint8_t calculateChecksum(const std::array<uint8_t, 9> &frame) {
    uint16_t sum = 0;
    for (uint8_t b : frame) {
      sum += b;
    }
    return static_cast<uint8_t>(-sum);
  }

  static bool parse(const uint8_t *raw, SDOFrame &out) {
    std::array<uint8_t, 9> tmp;
    std::copy(raw, raw + 9, tmp.begin());

    uint8_t expected_ckc = calculateChecksum(tmp);
    if (expected_ckc != raw[9])
      return false;

    // ID: ID of motor
    // CMD: Response command: 0x43,0x4B,0x4F: OK, 0x80: NG
    // INDEX: Index of the parameter
    // SUBINDEX: Subindex of the parameter
    // DATA: Data of the parameter
    // CHECKSUM: Checksum of the frame
    // IF CMD = 0x80 -> bytes(2-8) is error code

    out.id       = raw[0];
    out.cmd      = raw[1];
    out.index    = raw[2] | (raw[3] << 8);
    out.subindex = raw[4];
    out.data     = raw[5] | (raw[6] << 8) | (raw[7] << 16) | (raw[8] << 24);
    return true;
  }
} sdo_frame_t;

class ObjectDictionary {
public:
  using Key = std::pair<uint16_t, uint8_t>; // index, subindex

  void update(uint16_t index, uint8_t subindex, uint32_t value) {
    std::lock_guard<std::mutex> lock(mutex_);
    od_[{index, subindex}] = value;
  }

  bool get(uint16_t index, uint8_t subindex, uint32_t &out_value) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto                        it = od_.find({index, subindex});
    if (it != od_.end()) {
      out_value = it->second;
      return true;
    }
    return false;
  }

  void print() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "📦 Object Dictionary:" << std::endl;
    for (const auto &[key, val] : od_) {
      std::cout << "  0x" << std::hex << std::setw(4) << std::setfill('0')
                << key.first << ":" << std::setw(2) << std::setfill('0')
                << static_cast<int>(key.second) << " = 0x" << std::setw(8)
                << val << std::endl;
    }
  }

private:
  std::map<Key, uint32_t> od_;
  mutable std::mutex      mutex_;
};