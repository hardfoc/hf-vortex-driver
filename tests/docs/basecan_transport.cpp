// Code shown on docs/porting.md: a VortexCanTransport over BaseCan from hf-internal-interface-wrap.
// Compiled by the host build when HF_VORTEX_BASECAN_INCLUDE_DIR points at that repository's inc/base.
// --8<-- [start:basecan]
#include <cstring>

#include "BaseCan.h"
#include "vortex/VortexCanTransport.hpp"

class BaseCanVortexTransport final : public hf::vortex::VortexCanTransport {
public:
    explicit BaseCanVortexTransport(BaseCan& can) noexcept : can_(can) {}

    bool send(const hf::vortex::VortexCanFrame& frame) noexcept override {
        if (frame.dlc > 8) {
            return false;                              // classic CAN only
        }
        hf_can_message_t msg{};                        // classic frame: is_canfd = false
        msg.id = frame.id;
        msg.is_extended = frame.extended;
        msg.is_rtr = frame.rtr;
        msg.dlc = frame.dlc;
        std::memcpy(msg.data, frame.data, frame.dlc);
        return can_.TrySendMessage(msg) == hf_can_err_t::CAN_SUCCESS;  // never blocks
    }

private:
    BaseCan& can_;
};
// --8<-- [end:basecan]

bool make_transport_compiles(BaseCan& can) {
    BaseCanVortexTransport transport(can);
    hf::vortex::VortexCanFrame f{};
    return transport.send(f);
}
