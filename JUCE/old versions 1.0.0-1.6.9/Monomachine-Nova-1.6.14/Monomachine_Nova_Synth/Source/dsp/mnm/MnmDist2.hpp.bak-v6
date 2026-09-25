#pragma once
// MnmDist2.hpp -- alternative DIST block from 2 try filter dist env nice build
// Source/DSP/Kernel/DistortionSaturationBlock.h
// Provides HardClip_SRSM (bset #$14,sr), Overdrive_Soft (tanh), Fold
// Plus SRR as bitcrush fractional lookup
// Named dist2 to avoid repeat

#include <cmath>
#include <algorithm>
#include <cstdint>

namespace monomachine {
namespace mnm {

class DistortionSaturationBlock2 {
public:
    enum class Character { HardClip_SRSM, Overdrive_Soft, Fold };
    struct Params { int drive=64; int amount=0; int srr=0; Character character=Character::HardClip_SRSM; };

    void reset() noexcept { srrHold_=0.0f; srrPhase_=0.0f; }

    static inline float saturateSRSM(float x) noexcept {
        // Q23 sat24 emulation: clamp to +-1.0
        if (x > 1.0f) return 1.0f;
        if (x < -1.0f) return -1.0f;
        return x;
    }

    void processBlock16(float* mono, int n, double /*sr*/, const Params& p) noexcept {
        const float g = 1.0f + (std::clamp(p.drive,0,127)/127.0f)*7.0f;
        const float wet = std::clamp(p.amount,0,127)/127.0f;
        const float srrN = std::clamp(p.srr,0,127)/127.0f;
        const float step = std::max(1.0f/32.0f, 1.0f - srrN*(31.0f/32.0f));
        for (int i=0;i<n;++i){
            float x=mono[i];
            if (p.srr>0){ srrPhase_+=step; if(srrPhase_>=1.0f){ srrPhase_-=1.0f; srrHold_=x; } x=srrHold_; }
            const float d=x*g; float s;
            switch(p.character){
                case Character::HardClip_SRSM: s=saturateSRSM(d); break;
                case Character::Overdrive_Soft: s=std::tanh(d); break;
                case Character::Fold: { float f=d; while(f>1.0f) f=2.0f-f; while(f<-1.0f) f=-2.0f-f; s=f; break; }
                default: s=saturateSRSM(d); break;
            }
            mono[i]=std::clamp(x*(1.0f-wet)+s*wet, -1.0f, 1.0f);
        }
    }

    // stereo helper
    inline float processSample(float x, float driveParam, float amountParam, float srrParam, Character ch) noexcept {
        // driveParam 0..127, amount 0..127, srr 0..127
        Params p; p.drive = static_cast<int>(driveParam); p.amount = static_cast<int>(amountParam); p.srr = static_cast<int>(srrParam); p.character = ch;
        float mono[1]={x};
        processBlock16(mono,1,44100.0,p);
        return mono[0];
    }

private:
    float srrHold_=0, srrPhase_=0;
};

// Wrapper for TrackChain usage: uses DIST param as amount, with drive derived
class Dist2Core {
public:
    void reset() { block.reset(); }
    inline float process(float x, float distParam, float srrParam) noexcept {
        // distParam 0..127 (64 neutral), map to amount and drive
        // For mnm dist2: amount = distParam, drive = 64 + (distParam-64)*0.5 if >64
        int amount = std::clamp(static_cast<int>(distParam),0,127);
        int drive = 64;
        if (distParam > 64) drive = 64 + static_cast<int>((distParam-64)*0.8f);
        float out[1]={x};
        typename DistortionSaturationBlock2::Params p;
        p.drive = drive;
        p.amount = amount > 64 ? amount : 0; // only when positive
        p.srr = static_cast<int>(srrParam);
        p.character = DistortionSaturationBlock2::Character::HardClip_SRSM;
        block.processBlock16(out,1,44100.0,p);
        return out[0];
    }
private:
    DistortionSaturationBlock2 block;
};

} // namespace mnm
} // namespace monomachine
