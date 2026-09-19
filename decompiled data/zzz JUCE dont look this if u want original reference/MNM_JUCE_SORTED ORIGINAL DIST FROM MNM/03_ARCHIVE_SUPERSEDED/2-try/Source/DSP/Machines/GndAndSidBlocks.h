#pragma once
#include "../Core/Dsp56300Core.h"
#include <cmath>
#include <algorithm>

namespace mmnova::machines {

class GndSinBlock {
public:
    static constexpr double kPitchScale_17C6F9 = (double)0x17C6F9/8388608.0; // 0.18624806
    void reset() noexcept { phase_=0; y1_=y2_=0; }
    void processBlock16(float* out,int n,double kernelInc) noexcept {
        const double inc=kernelInc*kPitchScale_17C6F9;
        constexpr float c1=0.23649f, c2=0.71453f; // $1E454E / $5B75B8
        for(int i=0;i<n;++i){
            phase_+=inc; if(phase_>=1.0)phase_-=std::floor(phase_);
            const float s=(float)std::sin(phase_*6.2831853);
            y1_=c2*s+(1.0f-c2)*y1_; y2_=c1*y1_+(1.0f-c1)*y2_; out[i]=y2_;
        }
    }
private: double phase_=0; float y1_=0,y2_=0;
};

} // namespace
