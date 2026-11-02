// Code shown on docs/quickstart.md and in the README. Compiled by the host build (HF_VORTEX_BUILD_DOC_SNIPPETS).
// --8<-- [start:quickstart]
#include "vortex/VortexHostSession.hpp"
#include "vortex/VortexProtocol.hpp"

using namespace hf::vortex;

// Put node 5 in Operational and select CiA 402 profile velocity mode (3).
bool start_node_in_velocity_mode(VortexCanTransport& bus) {
    VortexHostSession session(bus);
    constexpr std::uint8_t kNode = 5;

    if (!session.sendNmt(NmtCommand::Start, kNode)) {               // 0x000: 01 05
        return false;                                               // transport refused the frame
    }
    return session.sdoDownloadExpedited(kNode, kIdxModesOfOperation, // 0x605: 2F 60 60 00 03
                                        /*sub=*/0, /*value=*/3, /*size=*/1);
}
// --8<-- [end:quickstart]
