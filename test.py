#! /usr/bin/env python3

import serial

ser = serial.Serial("/dev/kinco", 115200, timeout=1)


# uint8_t MotorKinco::CalculateChecksum(const std::vector<uint8_t> &data) {
#   uint16_t sum = 0;
#   for (uint8_t byte : data) {
#     sum += byte;
#   }
#   return static_cast<uint8_t>(-sum);
# }

def cksum(data: list[int]) -> int:
    cksum = 0
    for i in range(len(data)):
        cksum += data[i]
    return -cksum & 0xFF


while True:

    send_data = [0x01, 0x40, 0x40, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
    send_data[8] = cksum(send_data)
    ser.write(send_data)

    data = ser.read(10)

    print(data)
