// Code shown on docs/api-guide.md. Compiled by the host build (HF_VORTEX_BUILD_DOC_SNIPPETS).
#include <cstdint>
#include <initializer_list>

#include "vortex/VortexHostSession.hpp"
#include "vortex/VortexProtocol.hpp"

using namespace hf::vortex;

// --8<-- [start:nmt]
bool reset_and_start_all(VortexHostSession& session) {
    constexpr std::uint8_t kAllNodes = 0;                          // NMT broadcast
    return session.sendNmt(NmtCommand::ResetCommunication, kAllNodes) &&
           session.sendNmt(NmtCommand::Start, kAllNodes);
}
// --8<-- [end:nmt]

// --8<-- [start:sdo]
bool write_objects(VortexHostSession& session, std::uint8_t node) {
    // 1 byte: modes of operation 0x6060:00 = 1 (profile position)
    if (!session.sdoDownloadExpedited(node, kIdxModesOfOperation, 0, 1, 1)) return false;
    // 2 bytes: controlword 0x6040:00 = 0x0006 (CiA 402 Shutdown)
    if (!session.sdoDownloadExpedited(node, kIdxControlword, 0, 0x0006, 2)) return false;
    // 4 bytes: vendor device id 0x2000:01 (placeholder index, see the protocol table)
    return session.sdoDownloadExpedited(node, kIdxVendorDeviceId, kSubVendorDeviceId, 0xCAFE0001u, 4);
}
// --8<-- [end:sdo]

// --8<-- [start:cia402]
// CiA 402 controlword commands (CiA 402 part 2, controlword 0x6040).
constexpr std::uint16_t kCwShutdown = 0x0006;         // -> Ready to switch on
constexpr std::uint16_t kCwSwitchOn = 0x0007;         // -> Switched on
constexpr std::uint16_t kCwEnableOperation = 0x000F;  // -> Operation enabled
constexpr std::uint16_t kCwFaultReset = 0x0080;       // Fault -> Switch on disabled (rising edge of bit 7)

bool enable_drive(VortexHostSession& session, std::uint8_t node) {
    for (std::uint16_t cw : {kCwFaultReset, kCwShutdown, kCwSwitchOn, kCwEnableOperation}) {
        if (!session.sdoDownloadExpedited(node, kIdxControlword, 0, cw, 2)) {
            return false;
        }
        // The session sends only: confirm each state in the statusword (0x6041) with your
        // own SDO upload or a TPDO before sending the next command.
    }
    return true;
}
// --8<-- [end:cia402]
