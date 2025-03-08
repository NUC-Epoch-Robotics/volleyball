#ifndef CHASSIS_SERIAL_DRIVER__PACKET_HPP_
#define CHASSIS_SERIAL_DRIVER__PACKET_HPP_

#include <algorithm>
#include <cstdint>
#include <vector>

namespace chassis_serial_driver
{
struct ReceivePacket
{
  uint8_t header = 0x5A;
  uint8_t Rxdata;  // 0-red 1-blue

  uint16_t checksum = 0;
} __attribute__((packed));

struct SendPacket
{
  uint8_t header_M = 0x4d;
  uint8_t header_o = 0x6f;
  uint8_t header_t = 0x74;
  uint8_t header_o2 = 0x6f;
  uint8_t header_r = 0x72;
  uint8_t motor_number : 1;
  uint8_t header__ = 0x3a;
  uint8_t header__1 = 0x5f;
  uint8_t header_S = 0x53;
  uint8_t header_e = 0x65;
  uint8_t header_t2= 0x74; 
  uint8_t header__2= 0x5f;
  float pos;
  uint8_t header__3 = 0x5f;
  float vel;
  uint8_t header__4 = 0x5f;
  float kp;
  uint8_t header__5 = 0x5f;
  float ki;
  uint8_t header__6 = 0x5f;
  float kd;       // 微分增益
  uint8_t header__7 = 0x5f;
  float tor;      // 力矩

} __attribute__((packed));

inline ReceivePacket fromVector(const std::vector<uint8_t> & data)
{
  ReceivePacket packet;
  std::copy(data.begin(), data.end(), reinterpret_cast<uint8_t *>(&packet));
  return packet;
}

inline std::vector<uint8_t> toVector(const SendPacket & data)
{
  std::vector<uint8_t> packet(sizeof(SendPacket));
  std::copy(
    reinterpret_cast<const uint8_t *>(&data),
    reinterpret_cast<const uint8_t *>(&data) + sizeof(SendPacket), packet.begin());
  return packet;
}


}  // namespace chassis_serial_driver

#endif  // RM_SERIAL_DRIVER__PACKET_HPP_
