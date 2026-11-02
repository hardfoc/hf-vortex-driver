/**
 * @file VortexProtocol.hpp
 * @brief CiA 301/402 and HardFOC vendor index placeholders for Vortex over CAN.
 * @details Values are **documentation and compile-time defaults**; the final Object Dictionary
 *          (CANopenNode or product EDS) is authoritative. This header keeps host and device
 *          firmware aligned on named constants without pulling in CANopenNode.
 * @see CiA 301, CiA 402, DS-402 motion profile
 * @copyright GPL-3.0-or-later
 */
#pragma once

#include <cstdint>

namespace hf::vortex {

/// CiA 301 — Error register (0x1001) subindex 0
inline constexpr std::uint16_t kIdxErrorRegister = 0x1001U;
/// CiA 301 — Pre-defined error field (0x1003) sub 0 = number of errors
inline constexpr std::uint16_t kIdxPredefError = 0x1003U;
/// CiA 301 — Error behavior (0x1029); sub 1 = communication error
inline constexpr std::uint16_t kIdxErrorBehavior = 0x1029U;

/// CiA 402 — Controlword (0x6040) — used for drive state machine
inline constexpr std::uint16_t kIdxControlword = 0x6040U;
/// CiA 402 — Statusword (0x6041)
inline constexpr std::uint16_t kIdxStatusword = 0x6041U;
/// CiA 402 — Modes of operation (0x6060)
inline constexpr std::uint16_t kIdxModesOfOperation = 0x6060U;
/// CiA 402 — Modes of operation display (0x6061)
inline constexpr std::uint16_t kIdxModesDisplay = 0x6061U;
/// CiA 402 — Position actual value (0x6064)
inline constexpr std::uint16_t kIdxPosActual = 0x6064U;
/// CiA 402 — Velocity actual value (0x606C)
inline constexpr std::uint16_t kIdxVelActual = 0x606CU;

/**
 * @brief Proposed **vendor** range (HardFOC) — 0x2000–0x2FFF manufacturer-specific.
 *        Reserve sub-ranges as products solidify.
 */
inline constexpr std::uint16_t kIdxVendorBase = 0x2000U;
/// Example: 0x2000:01 — u32 build id / device magic (placeholder)
inline constexpr std::uint16_t kIdxVendorDeviceId = 0x2000U;
inline constexpr std::uint8_t kSubVendorDeviceId = 0x01U;

/** @brief NMT command bytes (CANopen CiA 301) */
enum class NmtCommand : std::uint8_t {
  Start = 0x01U,
  Stop = 0x02U,
  PreOperational = 0x80U,
  ResetNode = 0x81U,
  ResetCommunication = 0x82U,
};

}  // namespace hf::vortex
