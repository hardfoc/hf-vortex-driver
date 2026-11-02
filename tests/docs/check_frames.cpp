// Runs the documentation code against the loopback transport and checks the bytes it puts on the bus.
#include <cstdio>
#include <initializer_list>

#include "loopback_transport.hpp"
#include "vortex/VortexHostSession.hpp"

bool start_node_in_velocity_mode(hf::vortex::VortexCanTransport& bus);
bool reset_and_start_all(hf::vortex::VortexHostSession& session);
bool write_objects(hf::vortex::VortexHostSession& session, std::uint8_t node);
bool enable_drive(hf::vortex::VortexHostSession& session, std::uint8_t node);

namespace {
int failures = 0;

void expect(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    }
}

bool bytes(const hf::vortex::VortexCanFrame& f, std::uint32_t id, std::uint8_t dlc,
           std::initializer_list<std::uint8_t> data) {
    if (f.id != id || f.dlc != dlc || f.extended || f.rtr) return false;
    std::size_t i = 0;
    for (std::uint8_t b : data) {
        if (f.data[i++] != b) return false;
    }
    return true;
}
}  // namespace

int main() {
    {
        LoopbackTransport bus;
        expect(start_node_in_velocity_mode(bus), "quickstart returns true");
        expect(bus.count() == 2, "quickstart sends two frames");
        expect(bytes(bus.frame(0), 0x000, 2, {0x01, 0x05}), "NMT start node 5");
        expect(bytes(bus.frame(1), 0x605, 8, {0x2F, 0x60, 0x60, 0x00, 0x03, 0, 0, 0}), "SDO 0x6060:00 = 3");
    }
    {
        LoopbackTransport bus;
        hf::vortex::VortexHostSession session(bus);
        expect(reset_and_start_all(session), "NMT broadcast");
        expect(bytes(bus.frame(0), 0x000, 2, {0x82, 0x00}), "NMT reset communication, all");
        expect(write_objects(session, 3), "SDO writes");
        expect(bytes(bus.frame(3), 0x603, 8, {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0, 0}), "SDO 2-byte");
        expect(bytes(bus.frame(4), 0x603, 8, {0x23, 0x00, 0x20, 0x01, 0x01, 0x00, 0xFE, 0xCA}), "SDO 4-byte");
        expect(!session.sdoDownloadExpedited(3, 0x6040, 0, 0, 3), "size 3 is rejected");
    }
    {
        LoopbackTransport bus;
        hf::vortex::VortexHostSession session(bus);
        expect(enable_drive(session, 1), "CiA 402 enable sequence");
        expect(bus.count() == 4, "four controlword writes");
        expect(bytes(bus.frame(3), 0x601, 8, {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0, 0}), "enable operation");
    }
    if (failures == 0) std::printf("documentation frames OK\n");
    return failures == 0 ? 0 : 1;
}
