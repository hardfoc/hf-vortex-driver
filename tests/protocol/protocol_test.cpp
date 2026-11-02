// Checks the object-dictionary indices and NMT command bytes in VortexProtocol.hpp against CiA 301 and CiA 402,
// and the SDO frame the session builds for the error behaviour object.
#include <cstdint>
#include <cstdio>

#include "vortex/VortexHostSession.hpp"
#include "vortex/VortexProtocol.hpp"

using namespace hf::vortex;

// CiA 301 communication profile area
static_assert(kIdxErrorRegister == 0x1001U, "CiA 301: error register is 0x1001");
static_assert(kIdxPredefError == 0x1003U, "CiA 301: pre-defined error field is 0x1003");
static_assert(kIdxErrorBehavior == 0x1029U, "CiA 301: error behavior is 0x1029 (0x1021 is store EDS)");

// CiA 402 drive profile
static_assert(kIdxControlword == 0x6040U, "CiA 402: controlword is 0x6040");
static_assert(kIdxStatusword == 0x6041U, "CiA 402: statusword is 0x6041");
static_assert(kIdxModesOfOperation == 0x6060U, "CiA 402: modes of operation is 0x6060");
static_assert(kIdxModesDisplay == 0x6061U, "CiA 402: modes of operation display is 0x6061");
static_assert(kIdxPosActual == 0x6064U, "CiA 402: position actual value is 0x6064");
static_assert(kIdxVelActual == 0x606CU, "CiA 402: velocity actual value is 0x606C");

// CiA 301 manufacturer-specific profile area is 0x2000 to 0x5FFF
static_assert(kIdxVendorBase >= 0x2000U && kIdxVendorBase <= 0x5FFFU, "vendor base in manufacturer area");
static_assert(kIdxVendorDeviceId >= 0x2000U && kIdxVendorDeviceId <= 0x5FFFU, "vendor id in manufacturer area");

// CiA 301 NMT command specifiers
static_assert(static_cast<std::uint8_t>(NmtCommand::Start) == 0x01U, "NMT start");
static_assert(static_cast<std::uint8_t>(NmtCommand::Stop) == 0x02U, "NMT stop");
static_assert(static_cast<std::uint8_t>(NmtCommand::PreOperational) == 0x80U, "NMT enter pre-operational");
static_assert(static_cast<std::uint8_t>(NmtCommand::ResetNode) == 0x81U, "NMT reset node");
static_assert(static_cast<std::uint8_t>(NmtCommand::ResetCommunication) == 0x82U, "NMT reset communication");

namespace {

class RecordingTransport final : public VortexCanTransport {
public:
  bool send(const VortexCanFrame& frame) noexcept override {
    last = frame;
    ++count;
    return true;
  }
  VortexCanFrame last{};
  int count = 0;
};

}  // namespace

int main() {
  int failures = 0;
  auto expect = [&failures](bool ok, const char* what) {
    if (!ok) {
      std::printf("FAIL: %s\n", what);
      ++failures;
    }
  };

  // Error behaviour, sub 1 (communication error) = 1 (no state change): 1-byte expedited download to node 4.
  RecordingTransport bus;
  VortexHostSession session(bus);
  expect(session.sdoDownloadExpedited(4, kIdxErrorBehavior, 1, 1, 1), "SDO write accepted");
  expect(bus.count == 1, "one frame sent");
  const std::uint8_t want[8] = {0x2F, 0x29, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00};
  expect(bus.last.id == 0x604U && bus.last.dlc == 8, "SDO request to 0x604 with 8 bytes");
  for (int i = 0; i < 8; ++i) {
    expect(bus.last.data[i] == want[i], "SDO bytes 2F 29 10 01 01 00 00 00");
  }

  if (failures == 0) std::printf("protocol constants OK\n");
  return failures == 0 ? 0 : 1;
}
