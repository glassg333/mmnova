#pragma once
#include "../Core/Dsp56300Core.h"
#include "../Core/MmNovaTables_Fixed.h"
#include <array>
#include <cmath>
#include <algorithm>

namespace mmnova::machines {

class SwaveSawBlock {
public:
    enum class OscMode { MNM, OLD }; // renamed from RETRO / LEGACY (1.6.0 prompt)
    static const char* getModeDisplayName(OscMode m) noexcept { return (m==OscMode::MNM)?"MNM":"OLD"; }
    struct Params { OscMode mode=OscMode::MNM; int unil=64,uniw=48,unix=0,subx=40,sub1=32,sub2=16,tune=64; };

    void reset() noexcept { mp_=sp1_=sp2_=0; up_.fill(0.0); }

    void processBlock16(float* out,int n,double f0,double sr,const Params& p) noexcept {
        const double tune=std::exp2(((double)p.tune-64.0)/12.0);
        const double dt=std::clamp(f0*tune/sr,1e-5,0.48);
        const bool uOn=(p.mode==OscMode::MNM)?(p.unil>0&&p.uniw>0):(p.unil>0);
        const float ug=uOn?((float)p.unil/127.0f):0.0f, uw=(float)p.uniw/127.0f;
        const float sxg=(float)p.subx/127.0f, s1g=(float)p.sub1/127.0f, s2g=(float)p.sub2/127.0f;
        const float sc=(p.mode==OscMode::MNM)?0.316f:0.5f;
        for(int i=0;i<n;++i){
            float saw=(float)(2.0*mp_-1.0);
            if(p.mode==OscMode::MNM) saw+=0.22f*(float)(std::sin(mp_*6.2831853*2.0)+0.5*std::sin(mp_*6.2831853*3.0));
            float uni=0;
            if(uOn){ for(size_t u=0;u<4;++u){ const double ui=dt*(1.0+(double)(tables::kSwaveUnisonRatios_P101BFB[u]*uw)); up_[u]+=ui; if(up_[u]>=1.0)up_[u]-=std::floor(up_[u]); uni+=(float)(2.0*up_[u]-1.0);} uni*=0.25f*ug; }
            const float sq=(sp1_<0.5)?1.0f:-1.0f, sw=(float)(2.0*sp1_-1.0);
            const float sx=0.5f*(sq+sw)*sxg, o1=(float)std::sin(sp1_*6.2831853)*s1g, o2=(float)std::sin(sp2_*6.2831853)*s2g;
            mp_+=dt; if(mp_>=1.0)mp_-=1.0; sp1_+=dt*0.5; if(sp1_>=1.0)sp1_-=1.0; sp2_+=dt*0.25; if(sp2_>=1.0)sp2_-=1.0;
            out[i]=std::clamp((saw+uni+sx+o1+o2)*sc,-1.0f,1.0f);
        }
    }
private: double mp_=0,sp1_=0,sp2_=0; std::array<double,4> up_{};
};

} // namespace
