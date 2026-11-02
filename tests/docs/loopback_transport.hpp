// A transport that records frames instead of sending them: for host tests and the documentation.
#pragma once

#include <cstddef>

#include "vortex/VortexCanFrame.hpp"
#include "vortex/VortexCanTransport.hpp"

// --8<-- [start:loopback]
class LoopbackTransport final : public hf::vortex::VortexCanTransport {
public:
    bool send(const hf::vortex::VortexCanFrame& frame) noexcept override {
        if (count_ == kMaxFrames) {
            return false;                              // full: report a TX failure
        }
        frames_[count_++] = frame;
        return true;
    }

    std::size_t count() const noexcept { return count_; }
    const hf::vortex::VortexCanFrame& frame(std::size_t i) const noexcept { return frames_[i]; }

private:
    static constexpr std::size_t kMaxFrames = 16;
    hf::vortex::VortexCanFrame frames_[kMaxFrames]{};
    std::size_t count_ = 0;
};
// --8<-- [end:loopback]
