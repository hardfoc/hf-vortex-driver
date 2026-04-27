/**
 * @file main.cpp
 * @brief **Host-executable** smoke test: loopback transport + NMT/SDO encoding.
 * @details No ESP-IDF, no FreeRTOS, no CANopenNode. Verifies the driver API and byte layout
 *          before integration in HAL. Build: see `README.md` in this folder.
 */
#include "vortex/VortexCanFrame.hpp"
#include "vortex/VortexCanTransport.hpp"
#include "vortex/VortexHostSession.hpp"
#include "vortex/VortexProtocol.hpp"

#include <cstdio>
#include <vector>

namespace {

class LoopbackTransport final : public hf::vortex::VortexCanTransport {
public:
  bool send(const hf::vortex::VortexCanFrame& frame) noexcept override {
    recorded_.push_back(frame);
    return true;
  }
  const std::vector<hf::vortex::VortexCanFrame>& frames() const { return recorded_; }

private:
  std::vector<hf::vortex::VortexCanFrame> recorded_{};
};

}  // namespace

int main() {
  LoopbackTransport bus;
  hf::vortex::VortexHostSession session(bus);

  if (!session.sendNmt(hf::vortex::NmtCommand::Start, 1)) {
    std::printf("NMT start failed\n");
    return 1;
  }
  if (!session.sdoDownloadExpedited(1, hf::vortex::kIdxModesOfOperation, 0, 3, 1)) {
    std::printf("SDO download failed\n");
    return 1;
  }

  const auto& f = bus.frames();
  if (f.size() < 2) {
    std::printf("expected 2 frames, got %zu\n", f.size());
    return 1;
  }
  if (f[0].id != 0 || f[0].dlc != 2 || f[0].data[0] != 1 || f[0].data[1] != 1) {
    std::printf("NMT frame mismatch\n");
    return 1;
  }
  if (f[1].id != 0x601 || f[1].dlc != 8) {  // 0x600 + node 1
    std::printf("SDO frame id/dlc mismatch\n");
    return 1;
  }
  std::printf("hf-vortex-driver generic self-test OK (2 frames)\n");
  return 0;
}
