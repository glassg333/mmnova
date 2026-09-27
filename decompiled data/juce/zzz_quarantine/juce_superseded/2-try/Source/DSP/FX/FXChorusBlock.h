#pragma once
#include "../Core/Dsp56300Core.h"
#include "../Core/MmNovaTables_Fixed.h"
#include <vector>
#include <cmath>
#include <algorithm>

namespace mmnova::fx {

class FXChorusBlock {
public:
    enum class RoutingMixMode { HardwareAuthentic_078Dry, SoloFxTrack_100Wet };
    struct Params { int del=64,dep=64,spd=64,mix=127,fb=0,wid=64,lp=127,inp=64;
        RoutingMixMode mixMode=RoutingMixMode::HardwareAuthentic_078Dry; bool enable128BlockStartFadeIn=true; };

    void prepare(double sr) noexcept {
        sr_ = sr>0?sr:44100.0;
        if (ringL_.size()!=kRing){ ringL_.assign(kRing,0.0f); ringR_.assign(kRing,0.0f);} reset();
    }
    void reset() noexcept { oscC_=1.0f; oscS_=0.0f; startBlk_=0; wetGate_=0.0f; lpL_=lpR_=0.0f; widS_=0.0f; }

    void processBlock(float* L, float* R, int n, const Params& p) noexcept {
        int off=0; while(off<n){ const int s=std::min(16,n-off); sub16(L+off,R+off,s,p); off+=s; }
    }
private:
    static constexpr int kRing=2048, kMask=kRing-1;
    static constexpr float kTap=0.33333334f, kS3=0.86602540f;
    double sr_=44100.0; std::vector<float> ringL_, ringR_; int wp_=0;
    float oscC_=1,oscS_=0,wetGate_=0,lpL_=0,lpR_=0,widS_=0; int startBlk_=0;

    float rd(const std::vector<float>& b, float d) const noexcept {
        const float c=std::clamp(d,1.0f,(float)(kRing-4));
        const float rp=(float)wp_-c+(float)kRing; const int i0=(int)rp&kMask, i1=(i0+1)&kMask;
        const float f=rp-std::floor(rp); return b[i0]+f*(b[i1]-b[i0]);
    }
    void sub16(float* L,float* R,int n,const Params& p) noexcept {
        if (p.enable128BlockStartFadeIn){ if(startBlk_<128){startBlk_++; wetGate_=(float)startBlk_/128.0f;} else wetGate_=1.0f; } else wetGate_=1.0f;
        const float inpN=(float)std::clamp(p.inp,0,127)/128.0f; const float inpG=(inpN*inpN)*4.0f;
        const float spdN=(float)std::clamp(p.spd,0,127)/128.0f; const float step=(spdN*spdN)*0.0045f*(float)(44100.0/sr_);
        const float widT=(float)std::clamp(p.wid,0,127)/127.0f; widS_=0.02f*widT+0.98f*widS_;
        const float baseD=24.0f+((float)p.del/127.0f)*680.0f, modD=((float)p.dep/127.0f)*96.0f;
        const float fb=((float)std::clamp(p.fb,0,127)/128.0f)*0.92f; const float lpc=tables::lookupLpCoeff_P144AC7(p.lp);
        const int32_t mixQ=std::clamp(p.mix,0,127)<<16;
        float wet=(float)mixQ/(float)dsp56k::Q23_MAX, dry=(float)(dsp56k::Q23_MAX-mixQ)/(float)dsp56k::Q23_MAX;
        if (p.mixMode==RoutingMixMode::SoloFxTrack_100Wet && p.mix==127){ wet=1.0f; dry=0.0f; }
        for (int i=0;i<n;++i){
            oscS_=std::clamp(oscS_-oscC_*step,-1.0f,1.0f); oscC_=std::clamp(oscC_+oscS_*step,-1.0f,1.0f);
            const float m0=oscS_, m1=-0.5f*oscS_+kS3*oscC_, m2=-0.5f*oscS_-kS3*oscC_;
            const float inL=L[i]*inpG, inR=R[i]*inpG;
            const float tL=(rd(ringL_,baseD+modD*(1.0f+m0))+rd(ringL_,baseD+modD*(1.0f+m1))+rd(ringL_,baseD+modD*(1.0f+m2)))*kTap*wetGate_;
            const float tR=(rd(ringR_,baseD+modD*(1.0f-m0*widS_))+rd(ringR_,baseD+modD*(1.0f-m1*widS_))+rd(ringR_,baseD+modD*(1.0f-m2*widS_)))*kTap*wetGate_;
            lpL_+=lpc*(tL-lpL_); lpR_+=lpc*(tR-lpR_);
            ringL_[wp_]=std::clamp(inL*(1.0f+fb)*0.5f-fb*lpL_,-1.0f,1.0f);
            ringR_[wp_]=std::clamp(inR*(1.0f+fb)*0.5f-fb*lpR_,-1.0f,1.0f);
            wp_=(wp_+1)&kMask;
            L[i]=inL*dry+tL*wet; R[i]=inR*dry+tR*wet;
        }
    }
};

} // namespace
