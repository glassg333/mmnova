#pragma once
#include "../Core/Dsp56300Core.h"
#include <cmath>
#include <algorithm>

namespace mmnova::machines {

class DproBboxBlock {
public:
    enum class BboxDspMode { Classic_151, Nova_2KnobFade };
    struct Params { BboxDspMode dspMode=BboxDspMode::Nova_2KnobFade; int ptch=64,strt=0,rtrg=0,rtim=64; };

    void triggerHit(double sr,const Params& p,int len=22050) noexcept {
        sr_=sr>0?sr:44100.0; len_=std::max(256,len);
        const float sN=(float)std::clamp(p.strt,0,127)/127.0f;
        const float maxSkip=std::min((float)(0.030*sr_),(float)len_*0.25f);
        start_=(int)(sN*sN*maxSkip); cur_=(double)start_;
        reps_=std::clamp(p.rtrg,0,127); ri_=0; fade_=(int)(0.0005*sr_); fadeT_=std::max(1,fade_);
        const float tN=(float)(std::clamp(p.rtim,1,127)-1)/126.0f;
        interval_=std::max(1,(int)((0.255*std::pow(1.0-tN,2.2))*sr_)); until_=interval_; gain_=1.0f; active_=true;
    }
    void processBlock16(float* out,int n,const float* pcm,const Params& p) noexcept {
        const double pr=std::exp2(((double)p.ptch-64.0)/24.0); constexpr float m15=0.17782794f;
        for(int i=0;i<n;++i){
            if(!active_){ out[i]=0; continue; }
            if(reps_>0 && --until_<=0 && ri_<reps_){ ri_++; cur_=(double)start_; until_=interval_; fade_=fadeT_;
                const float pg=(float)ri_/(float)std::max(1,reps_); gain_=1.0f-pg*(1.0f-m15); }
            const int idx=(int)cur_; if(idx>=len_-1){ active_=false; out[i]=0; continue; }
            float raw; if(pcm){ const float f=(float)(cur_-std::floor(cur_)); raw=pcm[idx]+f*(pcm[idx+1]-pcm[idx]); }
            else { const double t=cur_/sr_; const double bf=52.0+190.0*std::exp(-t*48.0); raw=(float)(std::sin(6.2831853*bf*t)*std::exp(-t*9.5)); }
            float fe=1.0f; if(fade_>0){ fe=1.0f-(float)fade_/(float)fadeT_; fade_--; }
            out[i]=std::clamp(raw*gain_*fe,-1.0f,1.0f); cur_+=pr;
        }
    }
private: double sr_=44100,cur_=0; int len_=22050,start_=0,reps_=0,ri_=0,interval_=4410,until_=4410,fade_=0,fadeT_=22; float gain_=1; bool active_=false;
};

} // namespace
