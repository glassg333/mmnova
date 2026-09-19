#pragma once
#include "AmpEnvelopeBlock.h"
#include "MultiModeFilterBlock.h"
#include "DistortionSaturationBlock.h"

namespace mmnova::kernel {

// Voice chain per KERNEL_VOICE_CHAIN.md + README_architecture_routing.md §1:
//   machine osc -> [Distortion & SRR] -> [Multi-Mode Filter] -> [AMP env] -> mixer
class KernelVoiceChainBlock {
public:
    // UI guard (1.6.0): forbid knob rotation with Right Mouse Button.
    static constexpr bool shouldAllowKnobRotation(bool rmbDown) noexcept { return !rmbDown; }

    void reset(double sr) noexcept { sr_=sr; dist_.reset(); filter_.reset(); }
    void noteOn() noexcept { env_.noteOn(); }
    void noteOff() noexcept { env_.noteOff(); }

    void processBlock16(float* mono, int n,
                        const DistortionSaturationBlock::Params& dp,
                        const MultiModeFilterBlock::Params& fp,
                        const AmpEnvelopeBlock::Params& ep) noexcept {
        dist_.processBlock16(mono, n, sr_, dp);      // 1) Distortion & SRR (SR.SM clip)
        filter_.processBlock16(mono, n, sr_, fp);    // 2) Multi-Mode SVF (Base/Width)
        const float g = env_.processBlockGain(sr_, ep); // 3) AMP envelope (150% decay)
        for (int i=0;i<n;++i) mono[i]*=g;
    }

    DistortionSaturationBlock& dist() noexcept { return dist_; }
    MultiModeFilterBlock& filter() noexcept { return filter_; }
    AmpEnvelopeBlock& env() noexcept { return env_; }
private:
    double sr_=44100.0;
    DistortionSaturationBlock dist_;
    MultiModeFilterBlock filter_;
    AmpEnvelopeBlock env_;
};

} // namespace
