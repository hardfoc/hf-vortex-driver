/**
 * @file VortexCanFrame.hpp
 * @brief Minimal classic-CAN frame for hardware-agnostic Vortex / CANopen host use.
 * @details Layout matches the shared `CanFrame` in `hf-utils-canopen` (11-bit id, 0–8 data).
 *          Handlers may convert between the two with trivial copy. No dependency on HAL.
 * @copyright GPL-3.0-or-later
 */
#pragma once

#include <cstdint>

namespace hf::vortex {

/**
 * @brief Classic CAN 2.0A frame (11-bit ID, RTR, up to 8 data bytes).
 */
struct VortexCanFrame {
  std::uint32_t id{0};
  std::uint8_t dlc{0};
  bool extended{false};
  bool rtr{false};
  std::uint8_t data[8]{};

  /** @brief Zero-initialize data beyond DLC for debug consistency. */
  void pad() noexcept {
    for (std::uint8_t i = dlc; i < 8; ++i) data[i] = 0;
  }
};

}  // namespace hf::vortex
