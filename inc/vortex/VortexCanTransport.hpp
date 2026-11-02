/**
 * @file VortexCanTransport.hpp
 * @brief Abstract transport for Vortex / CANopen — implement with BaseCan, socketcan, or stubs.
 * @details Same role as `IBNO085Transport` or `TMC9660CommInterface` in other HardFOC drivers:
 *          the **external** library depends only on this interface. Platform code supplies
 *          an adapter over `BaseCan` (HAL) or a test `LoopbackTransport`.
 * @copyright GPL-3.0-or-later
 */
#pragma once

#include "VortexCanFrame.hpp"

namespace hf::vortex {

/**
 * @brief User-provided link to a CAN bus (one controller).
 */
class VortexCanTransport {
public:
  virtual ~VortexCanTransport() = default;

  /**
   * @brief Transmit a frame.
   * @return true if accepted for TX (or loopback recorded); false on error.
   */
  virtual bool send(const VortexCanFrame& frame) noexcept = 0;
};

}  // namespace hf::vortex
