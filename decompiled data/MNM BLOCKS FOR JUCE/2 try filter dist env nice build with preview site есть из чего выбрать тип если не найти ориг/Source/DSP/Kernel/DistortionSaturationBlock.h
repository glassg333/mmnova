#pragma once
#include "../Core/Dsp56300Core.h"
#include <cmath>
#include <algorithm>

namespace mmnova::kernel {

// DISTORTION & SRR — runs BEFORE filter (README_architecture_routing §1):
//   Machine -> [Distortion & SRR] -> Multi-Mode Filter -> Mod Matrix
// Firmware (00_ОТЧЁТ §3.2): no dedicated distortion machine. It's the ALU SR.SM flag:
//   1476a5 bset #$14,sr  -> hard clip ±1.0 (Q23)
//   1476aa bclr #$14,sr
// SRR = FX-EXT bitcrush: fractional lookup (mpysu+dmac) w/ pitch tables $101AFB/$101BFB/$101CFB.
class DistortionSaturationBlock {
public:
    enum class Character { HardClip_SRSM, Overdrive_Soft, Fold };
    struct Params { int drive=64, amount=0, srr=0; Character character=Character::HardClip_SRSM; };

    void reset() noexcept { srrHold_=0.0f; srrPhase_=0.0f; }

    static inline float saturateSRSM(float x) noexcept {
        const int32_t q = dsp56k::floatToQ23(x);
        return dsp56k::q23ToFloat(dsp56k::sat24((int64_t)q, true));
    }

    void processBlock16(float* mono, int n, double /*sr*/, const Params& p) noexcept {
        const float g = 1.0f + ((float)std::clamp(p.drive,0,127)/127.0f)*7.0f;
        const float wet = (float)std::clamp(p.amount,0,127)/127.0f;
        const float srrN = (float)std::clamp(p.srr,0,127)/127.0f;
        const float step = std::max(1.0f/32.0f, 1.0f - srrN*(31.0f/32.0f));
        for (int i=0;i<n;++i){
            float x=mono[i];
            if (p.srr>0){ srrPhase_+=step; if(srrPhase_>=1.0f){srrPhase_-=1.0f; srrHold_=x;} x=srrHold_; }
            const float d=x*g; float s;
            switch(p.character){
                case Character::HardClip_SRSM: s=saturateSRSM(d); break;
                case Character::Overdrive_Soft: s=std::tanh(d); break;
                case Character::Fold: { float f=d; while(f>1.0f)f=2.0f-f; while(f<-1.0f)f=-2.0f-f; s=f; break; }
                default: s=saturateSRSM(d); break;
            }
            mono[i]=std::clamp(x*(1.0f-wet)+s*wet, -1.0f, 1.0f);
        }
    }
private: float srrHold_=0, srrPhase_=0;
};

} // namespace
