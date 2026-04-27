/**
 * @file VortexHostSession.cpp
 * @brief Wire encoding (same bit layout as `hf-utils-canopen` / CiA 301).
 */
#include "vortex/VortexHostSession.hpp"

namespace hf::vortex {

namespace {

VortexCanFrame makeNmt(NmtCommand cmd, std::uint8_t targetNode) noexcept {
  VortexCanFrame f{};
  f.id = 0x000U;
  f.dlc = 2;
  f.extended = false;
  f.rtr = false;
  f.data[0] = static_cast<std::uint8_t>(cmd);
  f.data[1] = targetNode;
  return f;
}

VortexCanFrame makeSdoDownload(std::uint8_t nodeId, std::uint16_t index, std::uint8_t sub,
                               std::uint32_t data, std::uint8_t size) noexcept {
  VortexCanFrame f{};
  f.id = 0x600U + nodeId;
  f.dlc = 8;
  f.extended = false;
  f.rtr = false;

  std::uint8_t command = 0x23U;
  if (size == 1) {
    command = 0x2FU;
  } else if (size == 2) {
    command = 0x2BU;
  } else {
    size = 4;
  }

  f.data[0] = command;
  f.data[1] = static_cast<std::uint8_t>(index & 0xFFU);
  f.data[2] = static_cast<std::uint8_t>((index >> 8) & 0xFFU);
  f.data[3] = sub;
  f.data[4] = static_cast<std::uint8_t>(data & 0xFFU);
  f.data[5] = static_cast<std::uint8_t>((data >> 8) & 0xFFU);
  f.data[6] = static_cast<std::uint8_t>((data >> 16) & 0xFFU);
  f.data[7] = static_cast<std::uint8_t>((data >> 24) & 0xFFU);
  if (size < 4) {
    for (std::uint8_t i = static_cast<std::uint8_t>(4U + size); i < 8; ++i) f.data[i] = 0;
  }
  return f;
}

}  // namespace

bool VortexHostSession::sendNmt(NmtCommand cmd, std::uint8_t targetNode) noexcept {
  return transport_.send(makeNmt(cmd, targetNode));
}

bool VortexHostSession::sdoDownloadExpedited(std::uint8_t nodeId, std::uint16_t index,
                                            std::uint8_t sub, std::uint32_t value,
                                            std::uint8_t size) noexcept {
  if (size != 1 && size != 2 && size != 4) return false;
  return transport_.send(makeSdoDownload(nodeId, index, sub, value, size));
}

}  // namespace hf::vortex
