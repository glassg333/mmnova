#pragma once
#include "../Core/Dsp56300Core.h"
#include "../Core/MmNovaTables_Fixed.h"
#include <cmath>
#include <algorithm>

namespace mmnova::machines {

class FMPlusMachineBlock {
public:
    enum class Variant { FM_STAT, FM_PAR, FM_DYN };
    struct Params { Variant variant=Variant::FM_STAT; int p1_frq=60,p2_fin=64,p3_env=80,p4_fb=30,p5_2frq=80,p6_2vol=64,p7_tone=98,p8_tune=64; };

    void reset() noexcept { cp_=m1_=m2_=m3_=0; fb_=diff_=tone_=0; env_=1; }
    void triggerNote() noexcept { env_=1.0f; }

    void processBlock16(float* out,int n,double f0,double sr,const Params& p) noexcept {
        const double tune=std::exp2(((double)p.p8_tune-64.0)/12.0);
        const double inc=std::clamp(f0*tune/sr,1e-5,0.48);
        const double r1=tables::kFmRatioTable_P141A80[std::clamp((p.p5_2frq*24)>>7,0,23)];
        const double r2=tables::kFmRatioTable_P141A80[std::clamp((p.p1_frq*24)>>7,0,23)];
        const double r3=tables::kFmRatioTable_P141A80[std::clamp((p.p3_env*24)>>7,0,23)];
        const float fbA=((float)p.p4_fb/127.0f)*2.8f, idx=((float)p.p2_fin/127.0f)*6.5f, tc=tables::lookupLpCoeff_P144AC7(p.p7_tone);
        const float dmix=(p.p6_2vol>64)?((float)(p.p6_2vol-64)/63.0f):0.0f;
        for(int i=0;i<n;++i){
            env_*=0.9994f; const float em=0.25f+0.75f*env_; float o=0;
            if(p.variant==Variant::FM_STAT){
                const float mr=std::sin((float)(m1_*6.2831853)+fb_*fbA); const float md=mr-diff_; diff_=mr; fb_=std::clamp(0.5f*(mr+md),-1.0f,1.0f);
                tone_+=tc*(mr-tone_); o=std::clamp(std::sin((float)(cp_*6.2831853)+tone_*idx*em)+tone_*dmix,-1.0f,1.0f);
                m1_+=inc*r1; if(m1_>=1.0)m1_-=std::floor(m1_);
            } else if(p.variant==Variant::FM_PAR){
                const float a=std::sin((float)(m1_*6.2831853))*((float)p.p2_fin/127.0f);
                const float b=std::sin((float)(m2_*6.2831853))*((float)p.p4_fb/127.0f);
                const float c=std::sin((float)(m3_*6.2831853))*((float)p.p6_2vol/127.0f);
                const float sm=(a+b+c)*3.5f*em; const float co=std::sin((float)(cp_*6.2831853)+sm); tone_+=tc*(co-tone_); o=std::clamp(tone_,-1.0f,1.0f);
                m1_=std::fmod(m1_+inc*r1,1.0); m2_=std::fmod(m2_+inc*r2,1.0); m3_=std::fmod(m3_+inc*r3,1.0);
            } else {
                const float dh=1.0f+std::fabs(((float)p.p6_2vol/127.0f)*em-0.5f)*8.0f; const float sfb=(fb_<0)?-fb_:fb_;
                const float mw=std::sin((float)(m1_*6.2831853))+0.45f*std::sin((float)(m1_*6.2831853*dh)+sfb*fbA); fb_=std::clamp(mw,-1.0f,1.0f);
                o=std::clamp(std::sin((float)(cp_*6.2831853)+mw*idx*em),-1.0f,1.0f); m1_=std::fmod(m1_+inc*r1,1.0);
            }
            cp_+=inc; if(cp_>=1.0)cp_-=std::floor(cp_); out[i]=o;
        }
    }
private: double cp_=0,m1_=0,m2_=0,m3_=0; float fb_=0,diff_=0,tone_=0,env_=1;
};

} // namespace
