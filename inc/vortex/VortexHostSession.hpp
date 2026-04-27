/**
 * @file VortexHostSession.hpp
 * @brief High-level **host** helpers: NMT + expedited SDO download.
 * @details SDO request encoding is aligned with `hf-utils-canopen` / CiA 301
 *          (client→server 0x600+node, expedited 1/2/4-byte). For identical wire images,
 *          use `CanOpen::BuildSdoDownload` in the application layer, or this API alone.
 * @note No RX / SDO response parsing here — add in middleware or link `CanOpenExtras.h` at handler level.
 * @copyright GPL-3.0-or-later
 */
#pragma once

#include "VortexCanFrame.hpp"
#include "VortexCanTransport.hpp"
#include "VortexProtocol.hpp"
#include <cstdint>

namespace hf::vortex {

/**
 * @brief Small host-side session: sends NMT and expedited SDO download frames.
 */
class VortexHostSession {
public:
  explicit VortexHostSession(VortexCanTransport& transport) noexcept
      : transport_(transport) {}

  /** @brief NMT 0x000, 2 bytes: command + target node (0 = all). */
  bool sendNmt(NmtCommand cmd, std::uint8_t targetNode) noexcept;

  /**
   * @brief SDO download expedited, 1/2/4 bytes to object @p index / @p sub.
   * @param size 1, 2, or 4
   */
  bool sdoDownloadExpedited(std::uint8_t nodeId, std::uint16_t index, std::uint8_t sub,
                            std::uint32_t value, std::uint8_t size) noexcept;

  VortexCanTransport& transport() noexcept { return transport_; }

private:
  VortexCanTransport& transport_;
};

}  // namespace hf::vortex
