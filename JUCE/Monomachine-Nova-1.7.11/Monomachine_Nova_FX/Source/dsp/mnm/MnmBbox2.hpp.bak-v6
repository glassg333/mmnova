#pragma once
// MnmBbox2.hpp -- alternative BBOX mode from DproBboxBlock.h
// Only alternative repeat-sample knob mode (Nova_2KnobFade)
// Unique name bbox2

#include <cmath>
#include <array>
#include <algorithm>
#include <cstdint>

namespace monomachine {
namespace mnm {

class Bbox2Core {
public:
    enum class Mode { Nova_2KnobFade, RepeatSample };
    struct Params {
        int base = 64;
        int width = 64;
        int q1 = 64;
        int q2 = 64;
        int envAtk = 64;
        int envDec = 64;
        int sampleSel = 0;
        int repeat = 64; // knob for repeat-sample
    };

    void reset(double sr) noexcept { sr_=sr; pos_=0; env_=0; envState_=0; repeatCounter_=0; }
    void noteOn(uint8_t /*note*/) noexcept { envState_=1; pos_=0; repeatCounter_=0; }

    void setParameters(const Params& p) noexcept { params_=p; }

    inline float tick(float in) noexcept {
        // Simple BBOX emulation: bandpass-ish + repeat sample hold
        // repeat knob controls hold length 1..128 samples
        int hold = 1 + (std::clamp(params_.repeat,0,127) * 4); // 1..509
        float out;
        if (params_.repeat > 4){
            if (repeatCounter_==0) held_=in;
            repeatCounter_ = (repeatCounter_+1)%hold;
            out = held_;
        } else {
            out = in;
        }
        // fake filter: lowpass based on base
        float cutoff = 200.0f + std::clamp(params_.base,0,127)/127.0f*8000.0f;
        float g = 1.0f - std::exp(-2*3.14159265f*cutoff/static_cast<float>(sr_));
        lp_ += g*(out-lp_);
        return lp_ * 0.8f + out*0.2f;
    }

    void processBlock(float* buf, int n) noexcept {
        for (int i=0;i<n;++i) buf[i]=tick(buf[i]);
    }

private:
    double sr_=44100.0;
    Params params_{};
    float held_=0, lp_=0, env_=0;
    int pos_=0, envState_=0, repeatCounter_=0;
};

} // namespace mnm
} // namespace monomachine
