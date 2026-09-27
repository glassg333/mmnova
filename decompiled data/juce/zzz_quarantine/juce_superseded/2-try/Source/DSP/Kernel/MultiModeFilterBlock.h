#pragma once
#include "../Core/Dsp56300Core.h"
#include <cmath>
#include <algorithm>

namespace mmnova::kernel {

// MULTI-MODE FILTER (Base/Width) — kernel func_0004A8 tail P:$05D3-$05FA
// KERNEL_VOICE_CHAIN.md §4.6/§8.5: 2-pole SVF, coeffs $F528BD / $4A4DF0, two loops.
//   $F528BD = -0x0AD743/2^23 = -0.08475685  (damping / feedback)
//   $4A4DF0 = +0x4A4DF0/2^23 = +0.58044434  (frequency coeff base)
// Routing (README_architecture_routing §1): Machine -> Dist&SRR -> [FILTER] -> ModMatrix (FLTR_BASE Y:$0510)
class MultiModeFilterBlock {
public:
    enum class Mode { LowPass, BandPass, HighPass, BandStop };
    static constexpr float kCoeffF_4A4DF0  =  0.58044434f;
    static constexpr float kCoeffFB_F528BD = -0.08475685f;

    struct Params { int base=88, width=64, q=36; Mode mode=Mode::LowPass; int keytrackNote=60, playedNote=60; };

    void reset() noexcept { low1_=band1_=low2_=band2_=0.0f; }

    void processBlock16(float* mono, int n, double sr, const Params& p) noexcept {
        const float keytrack = (float)(p.playedNote - p.keytrackNote)/60.0f;
        const float baseN = std::clamp((float)p.base/127.0f + keytrack*0.35f, 0.02f, 0.98f);
        const float srr = (float)(44100.0/sr);
        const float fc  = std::clamp(2.0f*std::sin(3.14159265f*0.30f*baseN*baseN)*(0.4f+0.6f*kCoeffF_4A4DF0)*srr, 0.005f, 0.99f);
        const float qd  = std::clamp(2.0f + kCoeffFB_F528BD - ((float)p.q/127.0f)*1.9f, 0.05f, 2.0f);
        const float wN  = (float)std::clamp(p.width,0,127)/127.0f;
        const float fc2 = std::clamp(fc*(0.5f+wN), 0.005f, 0.99f);
        for (int i=0;i<n;++i){
            const float in=mono[i];
            low1_+=fc*band1_; const float h1=in-low1_-qd*band1_; band1_+=fc*h1; band1_=std::clamp(band1_,-1.5f,1.5f);
            const float s1=sel(p.mode,low1_,band1_,h1);
            low2_+=fc2*band2_; const float h2=s1-low2_-qd*band2_; band2_+=fc2*h2; band2_=std::clamp(band2_,-1.5f,1.5f);
            mono[i]=std::clamp(sel(p.mode,low2_,band2_,h2),-1.0f,1.0f);
        }
    }
private:
    static float sel(Mode m,float lp,float bp,float hp) noexcept {
        switch(m){case Mode::LowPass:return lp;case Mode::BandPass:return bp;case Mode::HighPass:return hp;case Mode::BandStop:return lp+hp;} return lp; }
    float low1_=0,band1_=0,low2_=0,band2_=0;
};

// LITERAL capture from 01_GND-SIN_full.txt (P:$144D21-$144D4C):
//   144d29 move #>$1e454e,x1 ; C1=0.23649215
//   144d2b move #>$5b75b8,y0 ; C2=0.71452600
//   144d35 mac y0,x0,a ; 144d36 mac -y1,y0,a ; 144d37 mac x1,x0,b ; 144d38 mac -y1,x1,b
class TwoPoleResonator_P144D21 {
public:
    static constexpr float kC1_1E454E = 0.23649215f;
    static constexpr float kC2_5B75B8 = 0.71452600f;
    void reset() noexcept { s1_=0.0f; s2_=0.0f; }
    void processBlock16(float* mono, int n) noexcept {
        for (int i=0;i<n;++i){
            const float x=mono[i];
            const float a = kC2_5B75B8*x - s1_*kC2_5B75B8;
            const float b = kC1_1E454E*x - s1_*kC1_1E454E + s2_;
            s1_=a; s2_=b; mono[i]=std::clamp(b,-1.0f,1.0f);
        }
    }
private: float s1_=0,s2_=0;
};

} // namespace
