#pragma once
#include "../Core/Dsp56300Core.h"
#include "../Core/MmNovaTables_Fixed.h"
#include <cmath>
#include <algorithm>

namespace mmnova::kernel {

// AMP ENVELOPE — kernel func_0004A8 (P:$04A8-$04F5), KERNEL_VOICE_CHAIN.md §4.1/§4.3/§4.4
//   state 1 ATTACK : Y:$4FF += rate[Y:$141800 + (V-$10 ATK)>>16]
//   state 4 DECAY  : toward sustain=(V-$0E DEC)^2 using rate[Y:$141880]
//   state 5 SUSTAIN: Y:$4FF = (V-$0E)^2
//   state 2 RELEASE: Y:$4FF -= rate[Y:$141880 + (V-$0D REL)>>16]  (P:$4E5-$4EE)
//   default        : Y:$4FF = $7FFFFF
//   trigger (P:$4FF): V-$08==1 resets LFO state
//   LFO (P:$0506): state1 multiply by $791FD0 = 0.9462824 per step
class AmpEnvelopeBlock {
public:
    enum class State : int { Idle=0, Release=2, Attack=1, Decay=4, Sustain=5 };
    struct Params { int atk=0, hold=16, dec=55, rel=40; float decayCurvePct=150.0f; bool ahdMode=true; };
    static constexpr int32_t kFullLevel = 0x7FFFFF;
    static constexpr float kLfoDecay_791FD0 = 0.9462824f;

    void noteOn() noexcept { state_=State::Attack; levelQ23_=0; decayProgress_=0; releaseProgress_=0; lfoPhase_=0; lfoState_=0; }
    void noteOff() noexcept { state_=State::Release; }

    static float shapeCurve150(float t01, float pct) noexcept {
        const float t=std::clamp(t01,0.0f,1.0f), p=std::clamp(pct,0.0f,150.0f);
        if (p<=1.0f) return t;
        const float b=p/100.0f, k=1.0f+7.5f*(b*b);
        return (1.0f-std::exp(-k*t))/(1.0f-std::exp(-k));
    }

    float processBlockGain(double sr, const Params& p) noexcept {
        const float srS=(float)(44100.0/sr);
        const float dN=(float)std::clamp(p.dec,0,127)/127.0f;
        const int32_t sus=(int32_t)((dN*dN)*(float)kFullLevel);
        switch(state_){
            case State::Attack:{
                const float r=tables::lookupEnvOrLfoRate_Y141800(p.atk)*srS*(float)kFullLevel*32.0f;
                levelQ23_+=(int32_t)r; if(levelQ23_>=kFullLevel){levelQ23_=kFullLevel; state_=State::Decay; decayProgress_=0;} break; }
            case State::Decay:{
                const float r=tables::lookupEnvOrLfoRate_Y141800(p.dec)*srS*18.0f;
                decayProgress_=std::min(1.0f,decayProgress_+r);
                const float sh=1.0f-shapeCurve150(decayProgress_,p.decayCurvePct);
                const int32_t tgt=p.ahdMode?0:sus; levelQ23_=tgt+(int32_t)(sh*(float)(kFullLevel-tgt));
                if(decayProgress_>=1.0f){ state_=p.ahdMode?State::Idle:State::Sustain; if(p.ahdMode)levelQ23_=0; } break; }
            case State::Sustain: levelQ23_=sus; break;
            case State::Release:{
                const float r=tables::lookupEnvOrLfoRate_Y141800(p.rel)*srS*18.0f;
                releaseProgress_=std::min(1.0f,releaseProgress_+r);
                const float sh=1.0f-shapeCurve150(releaseProgress_,p.decayCurvePct);
                levelQ23_=(int32_t)(sh*(float)kFullLevel);
                if(releaseProgress_>=1.0f){levelQ23_=0; state_=State::Idle;} break; }
            default: levelQ23_=0; break;
        }
        return (float)levelQ23_/(float)kFullLevel;
    }

    float processLfo(double sr, int spd7, int depth7) noexcept {
        const float r=tables::lookupEnvOrLfoRate_Y141800(spd7)*(float)(44100.0/sr);
        lfoPhase_+=r; if(lfoState_==1) lfoValue_*=kLfoDecay_791FD0; if(lfoPhase_>=1.0f) lfoPhase_-=1.0f;
        const float d=(float)std::clamp(depth7,0,127)/127.0f; lfoValue_=std::sin(lfoPhase_*6.2831853f)*d; return lfoValue_;
    }
    State getState() const noexcept { return state_; }
private:
    State state_=State::Idle; int32_t levelQ23_=0;
    float decayProgress_=0, releaseProgress_=0, lfoPhase_=0, lfoValue_=0; int lfoState_=0;
};

} // namespace
