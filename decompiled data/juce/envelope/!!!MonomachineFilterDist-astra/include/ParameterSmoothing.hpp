// Optional HOST behavior, separate from the DSP filter slice.
// Schedule: Monomodule HostModel.cpp, commit 214d4b95, frame()/smoothing().
#pragma once
#include "MonomachineFilterDist.hpp"
namespace mnm132 {
class ParameterSmoothing {
public:
    void reset(const Parameters& p) noexcept {setTarget(p);current_=target_;block_=0;}
    void setTarget(const Parameters& p) noexcept {
        const int v[9]={p.base,p.width,p.hpq,p.lpq,p.attack,p.decay,p.baseOffset,p.widthOffset,p.distortion};
        for(unsigned i=0;i<9;++i)target_[i]=FilterDist::knob(v[i]);
    }
    void applyNextBlock(FilterDist& core) noexcept {
        // One frame = three DSP blocks. f%8==1 updates the parameter words.
        if(block_==3)for(unsigned i=0;i<9;++i)current_[i]=(3u*current_[i]+target_[i])>>2;
        block_=(block_+1)%24;
        std::array<uint32_t,8> filter{};
        for(unsigned i=0;i<8;++i)filter[i]=current_[i];
        core.setFilterWords(filter,current_[8]);
    }
private:
    std::array<uint32_t,9> current_{},target_{};
    unsigned block_=0;
};
}
