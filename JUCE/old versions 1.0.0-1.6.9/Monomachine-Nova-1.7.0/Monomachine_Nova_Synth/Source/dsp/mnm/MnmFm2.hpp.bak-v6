#pragma once
// MnmFm2.hpp -- alternative FM+ block from 2 try filter dist env nice build
// Source/DSP/Machines/FMPlusBlocks.h
// FM+ carries mod indexing etc. Provides DYN/STAT/PAR variations with unique names fm2

#include <cmath>
#include <array>
#include <cstdint>
#include <algorithm>

namespace monomachine {
namespace mnm {

class FMPlusBlock2 {
public:
    enum class Kind { Dyn, Stat, Par };
    struct Params {
        int pitch = 60;
        int mod = 64;
        int shape = 0;
        int decay = 64;
        int portamento = 0;
        int lfo = 0;
        int fmAmt = 64;
        int fmShape = 0;
    };
    void reset(double sr) noexcept { sr_=sr; phase_=0; env_=0; envState_=0; }
    void setKind(Kind k) noexcept { kind_=k; }

    void noteOn(float midiNote) noexcept { note_=midiNote; envState_=1; phase_=0; }

    inline float tick(const Params& p) noexcept {
        // envelope
        const float atk = 0.001f + (1.0f - std::clamp(p.decay,0,127)/127.0f)*0.2f;
        const float rel = 0.005f + (std::clamp(p.decay,0,127)/127.0f)*1.0f;
        if (envState_==1){ env_ += 1.0f/(sr_*atk); if(env_>=1.0f){ env_=1.0f; envState_=2; } }
        else if (envState_==2){ env_ -= 1.0f/(sr_*rel); if(env_<=0.0f){ env_=0.0f; envState_=0; } }

        float freq = 440.0f*std::pow(2.0f,(note_-69.0f)/12.0f);
        float modFreq = freq * (1.0f + std::clamp(p.fmAmt-64, -64,63)/64.0f * 2.0f);
        float modIndex = std::clamp(p.mod,0,127)/127.0f * 8.0f;

        double inc = freq / sr_;
        double modInc = modFreq / sr_;
        modPhase_ += modInc;
        if (modPhase_>=1.0) modPhase_-=1.0;

        float mod = std::sin(2.0f*3.14159265f*static_cast<float>(modPhase_)) * modIndex;
        float carrier = std::sin(2.0f*3.14159265f*static_cast<float>(phase_+mod));
        phase_ += inc;
        if (phase_>=1.0) phase_-=1.0;

        // shape
        if (p.shape==1) carrier = carrier>0?1.0f:-1.0f;
        else if (p.shape==2) carrier = 2.0f*static_cast<float>(phase_)-1.0f;

        return carrier * env_;
    }

    void processBlock(float* out, int n, const Params& p) noexcept {
        for (int i=0;i<n;++i) out[i]=tick(p);
    }

private:
    double sr_=44100.0;
    double phase_=0, modPhase_=0;
    float note_=60, env_=0;
    int envState_=0;
    Kind kind_=Kind::Dyn;
};

} // namespace mnm
} // namespace monomachine
