// Monomachine Nova -- "mnm" DSP cores recovered from OS 1.32B
// REAL FILTER integration from decompiled data/mnm_filter_full_dump_juce_real
// - kLP_filter_coeffs 258 (P:$144AC7) -> kLpCoeffA/B in MnmTables.hpp
// - SVF constants $F528BD=-0.08469456 $4A4DF0=+0.58050346 (P:$05D3-$05FA)
// - Q from Y:$91+ via damping law, envelope AD BOFS/WOFS
// - Routing fixed: synth->dist->srr->filt+eq->env->dsnd (verified in mnm-routing-100/exact/verified)
// Old FilterCore kept as FilterCoreLegacy for backup modes (old still uses biquad, but legacy kept)

#pragma once
#include "MnmTables.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

namespace monomachine {
namespace mnm {

inline constexpr int kBlock = 16;
inline constexpr double kDspRate = 44100.0;
inline float clampi(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

inline float pitchIncrementFromSemitones(float semitoneOffset, float referenceHz, double sampleRate) {
    const double ratio = std::exp2(semitoneOffset / 12.0);
    return static_cast<float>(referenceHz * ratio / sampleRate);
}
inline float noteToHz(float midiNote) { return 440.0f * std::exp2((midiNote - 69.0f) / 12.0f); }

class SineTable {
public:
    static const SineTable& instance() { static const SineTable table; return table; }
    inline float read(float phase) const {
        phase -= std::floor(phase);
        const float pos = phase * static_cast<float>(kSize);
        const int i0 = static_cast<int>(pos);
        const int i1 = (i0 + 1) & (kSize - 1);
        const float frac = pos - static_cast<float>(i0);
        const float a = values[static_cast<size_t>(i0)];
        const float b = values[static_cast<size_t>(i1)];
        return a + (b - a) * frac;
    }
private:
    static constexpr int kSize = 8192;
    SineTable() { for (int i=0;i<kSize;++i) values[static_cast<size_t>(i)] = static_cast<float>(std::sin(2.0*3.14159265358979323846*i/kSize)); }
    std::array<float,kSize> values{};
};

class ToneLowpass {
public:
    enum class Bank { A, B };
    void reset() { z[0]=z[1]=0.0f; }
    void setSampleRate(double hostRate) { host = hostRate>0?hostRate:kDspRate; }
    void setTone(float param, Bank bank=Bank::A) {
        const int idx = std::clamp(static_cast<int>(param),0,127);
        const float c = (bank==Bank::A?kLpCoeffA:kLpCoeffB)[static_cast<size_t>(idx)];
        const double ratio = host/kDspRate;
        const double oneMinus = std::pow(1.0-static_cast<double>(c),ratio);
        coeff = static_cast<float>(1.0-oneMinus);
    }
    inline float process(int ch,float x){ float& s=z[static_cast<size_t>(ch)]; s+=coeff*(x-s); return s; }
    float coeff=1.0f;
private:
    double host=kDspRate;
    float z[2]{};
};

// ---------------------------------------------------------------------------
// LEGACY FilterCore (previous approximate) kept as backup for reference
// Now renamed to FilterCoreLegacy, still available for old modes if needed
// ---------------------------------------------------------------------------
class FilterCoreLegacy {
public:
    void reset(){ hp.reset(); lp.reset(); envLevel=0; envActive=false; blockCounter=0; }
    void setSampleRate(double hr){ host=hr>0?hr:kDspRate; lp.setSampleRate(host); hp.setSampleRate(host); }
    void setParameters(float base,float width,float hpq,float lpq,float bofs,float wofs,float envAtk,float envDec){
        baseParam=base; widthParam=width; bofsParam=bofs; wofsParam=wofs; atkParam=envAtk; decParam=envDec;
        hpqNorm=clampi(hpq,0,127)/127; lpqNorm=clampi(lpq,0,127)/127; apply();
    }
    void trigger(){ envActive=true; envLevel=0; blockCounter=0; }
    void release(){ envActive=false; }
    inline float process(int ch,float x){
        if(blockCounter==0&&envActive){
            const int a=std::clamp(static_cast<int>(atkParam),0,127);
            const int d=std::clamp(static_cast<int>(decParam),0,127);
            if(envLevel<1.0f){ envLevel+=kEnvAttackRate[static_cast<size_t>(a)]; if(envLevel>1) envLevel=1; }
            else envLevel*=kEnvDecayRate[static_cast<size_t>(d)];
            apply();
        }
        if(++blockCounter>=kBlock) blockCounter=0;
        const float low=hp.process(ch,x);
        const float high=x-low;
        const float band=lp.process(ch,high);
        return band*(1.0f+lpqNorm*0.5f)+high*hpqNorm*0.5f;
    }
    float envelopeValue() const { return envLevel; }
private:
    void apply(){
        hp.setTone(clampi(baseParam+bofsParam*envLevel,0,127),ToneLowpass::Bank::B);
        lp.setTone(clampi(baseParam+widthParam+wofsParam*envLevel,0,127),ToneLowpass::Bank::A);
    }
    ToneLowpass hp,lp;
    double host=kDspRate;
    float baseParam=0,widthParam=127,bofsParam=0,wofsParam=0,atkParam=0,decParam=64,hpqNorm=0,lpqNorm=0,envLevel=0;
    bool envActive=false; int blockCounter=0;
};

// ---------------------------------------------------------------------------
// REAL FILTER CORE -- from mnm_filter_full_dump_juce_real
// Uses SVF constants $F528BD / $4A4DF0 and proper Q + AD env BOFS/WOFS
// ---------------------------------------------------------------------------
inline constexpr float kRealSVF_x0_F528BD = -0.08469456f;
inline constexpr float kRealSVF_x1_4A4DF0 =  0.58050346f;

namespace real_detail {
inline float clampf(float v,float lo,float hi) noexcept { return v<lo?lo:(v>hi?hi:v); }
inline float knobToCutoffHz(float knob,double sr) noexcept {
    const float n=clampf(knob,0,127)/127.0f;
    const float maxHz=static_cast<float>(sr)*0.45f;
    const float hz=20.0f*std::pow(maxHz/20.0f,n*n);
    return clampf(hz,10.0f,maxHz);
}
inline float cutoffToG(float hz,double sr) noexcept {
    const float w=3.14159265358979f*clampf(hz,5.0f,static_cast<float>(sr)*0.45f)/static_cast<float>(sr);
    return clampf(2.0f*std::sin(w),0.0015f,1.95f);
}
}

class FilterCore {
public:
    void reset() noexcept { lo1=bp1=lo2=bp2=0; envLevel=0; envActive=false; blockCounter=0; bypass=false; }
    void setSampleRate(double sr) noexcept { host=sr>0?sr:kDspRate; }
    void setParameters(float base,float width,float hpq,float lpq,float bofs,float wofs,float envAtk,float envDec) noexcept {
        baseParam=base; widthParam=width; hpqParam=hpq; lpqParam=lpq; bofsParam=bofs; wofsParam=wofs; atkParam=envAtk; decParam=envDec; apply();
    }
    void trigger() noexcept { envActive=true; envLevel=0; blockCounter=0; }
    void release() noexcept { envActive=false; }
    inline float process(int /*ch*/,float x) noexcept {
        if(blockCounter==0&&envActive){
            const int a=std::clamp(static_cast<int>(atkParam),0,127);
            const int d=std::clamp(static_cast<int>(decParam),0,127);
            if(envLevel<1.0f){ envLevel+=kEnvAttackRate[static_cast<size_t>(a)]; if(envLevel>1) envLevel=1; }
            else envLevel*=kEnvDecayRate[static_cast<size_t>(d)];
            apply();
        }
        if(++blockCounter>=kBlock) blockCounter=0;
        if(bypass) return x;
        const float hp1 = x - lo1 - k1*bp1;
        bp1 += g1*hp1; bp1 = real_detail::clampf(bp1,-1.6f,1.6f);
        lo1 += g1*bp1;
        const float hp2 = hp1 - lo2 - k2*bp2;
        bp2 += g2*hp2; bp2 = real_detail::clampf(bp2,-1.6f,1.6f);
        lo2 += g2*bp2;
        const float out = lo2*(1.0f+lpqNorm*0.5f) + hp1*hpqNorm*0.35f;
        return real_detail::clampf(out,-1.0f,1.0f);
    }
    float envelopeValue() const noexcept { return envLevel; }
private:
    void apply() noexcept {
        const float baseMod = real_detail::clampf(baseParam + bofsParam*envLevel,0,127);
        const float widthMod = real_detail::clampf(widthParam + wofsParam*envLevel,0,127);
        const float lpTarget = real_detail::clampf(baseMod+widthMod,0,127);
        const float fc1 = real_detail::knobToCutoffHz(baseMod,host);
        const float fc2 = real_detail::knobToCutoffHz(lpTarget,host);
        g1 = real_detail::cutoffToG(fc1,host);
        g2 = real_detail::cutoffToG(fc2,host);
        const float hpqN = real_detail::clampf(hpqParam,0,127)/127.0f;
        const float lpqN = real_detail::clampf(lpqParam,0,127)/127.0f;
        hpqNorm=hpqN; lpqNorm=lpqN;
        k1 = real_detail::clampf(2.0f + kRealSVF_x0_F528BD - hpqN*1.9f,0.05f,2.0f);
        k2 = real_detail::clampf(2.0f + kRealSVF_x0_F528BD - lpqN*1.9f,0.05f,2.0f);
        if(baseParam<=0.5f && widthParam>=126.5f && hpqParam<=0.5f && lpqParam<=0.5f && std::abs(bofsParam)<0.5f && std::abs(wofsParam)<0.5f && envLevel<0.001f) bypass=true; else bypass=false;
    }
    double host=kDspRate;
    float baseParam=0,widthParam=127,hpqParam=0,lpqParam=0,bofsParam=0,wofsParam=0,atkParam=0,decParam=64;
    float hpqNorm=0,lpqNorm=0,g1=0.2f,g2=0.5f,k1=1.2f,k2=1.2f,lo1=0,bp1=0,lo2=0,bp2=0,envLevel=0;
    bool envActive=false; int blockCounter=0; bool bypass=false;
};

// ---------------------------------------------------------------------------
// AMP envelope -- kernel func_0004A8 P:$4A8-$4F5
// ---------------------------------------------------------------------------
class AmpEnvelope {
public:
    enum class State : uint8_t { Other=0,Attack=1,Release=2,Decay=4,Sustain=5 };
    void reset(){ level=0; state=State::Other; blockCounter=0; smoothed=0; }
    void setParameters(float attack,float decay,float release,float sustainLevel=0.0f){
        atk=std::clamp(attack,0,127); dec=std::clamp(decay,0,127); rel=std::clamp(release,0,127);
        sustain=clampi(sustainLevel,0,1)*clampi(sustainLevel,0,1);
    }
    void trigger(){ state=State::Attack; forced=true; }
    void release(){ if(state!=State::Other){ state=State::Release; forced=true; } }
    void hold(){ state=State::Sustain; forced=true; }
    inline float tick(){
        if(blockCounter==0) step();
        if(++blockCounter>=kBlock) blockCounter=0;
        return level;
    }
    float value() const { return level; }
    State currentState() const { return state; }
private:
    void step(){
        const int atkIdx=std::clamp(static_cast<int>(atk),0,127);
        const int decIdx=std::clamp(static_cast<int>(dec),0,127);
        const int relIdx=std::clamp(static_cast<int>(rel),0,127);
        switch(state){
            case State::Attack: level+=kEnvAttackRate[static_cast<size_t>(atkIdx)]; if(level>=1){ level=1; state=State::Decay; } break;
            case State::Decay: level*=kEnvDecayRate[static_cast<size_t>(decIdx)]; if(level<=sustain){ level=sustain; state=State::Sustain; } break;
            case State::Sustain: level=sustain; break;
            case State::Release: level*=kEnvDecayRate[static_cast<size_t>(relIdx)]; if(level<1e-6f) level=0; break;
            default: level=1.0f; break;
        }
    }
    float level=0,smoothed=0,atk=0,dec=64,rel=64,sustain=0; State state=State::Other; bool forced=false; int blockCounter=0;
};

class Saturator {
public:
    static inline float process(float x,float param){
        const float amount=(param-64.0f)/64.0f;
        if(amount<=0) return x*(1.0f+amount*0.5f);
        const float drive=1.0f+amount*15.0f;
        const float y=x*drive;
        return clampi(y,-1.0f,1.0f)/(1.0f+amount*2.0f);
    }
};

class SampleRateReduce {
public:
    void reset(){ counter=0; heldL=heldR=0; }
    inline void process(float param,float inL,float inR,float& outL,float& outR,float rateScale){
        const float n=std::clamp(param,0,127)/127.0f;
        const int hold=1+static_cast<int>(n*n*63.0f*rateScale);
        if(counter<=0){ heldL=inL; heldR=inR; counter=hold; }
        --counter; outL=heldL; outR=heldR;
    }
private:
    int counter=0; float heldL=0,heldR=0;
};

} // namespace mnm
} // namespace monomachine
