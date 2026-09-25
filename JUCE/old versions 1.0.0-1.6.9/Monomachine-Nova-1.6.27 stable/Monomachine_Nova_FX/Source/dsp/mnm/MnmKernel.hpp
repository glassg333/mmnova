// Monomachine Nova -- "mnm" DSP cores recovered from OS 1.32B
// REAL FILTER integration from decompiled data/mnm_filter_full_dump_juce_real
// - kLP_filter_coeffs 258 (P:$144AC7) -- реальный фильтр в MnmRealFilter.hpp
// - SVF constants $F528BD=-0.084747577 $4A4DF0=+0.581161499 (P:$05D3-$05FA)
// - Q from Y:$91+ via damping law, envelope AD BOFS/WOFS
// - Routing fixed: synth->dist->srr->filt+eq->env->dsnd (verified in mnm-routing-100/exact/verified)
// FilterCoreLegacy -- резервный приближённый фильтр; основной режим mnm -- в MnmRealFilter.hpp

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
// РЕАЛЬНОЕ ядро фильтра -- в MnmRealFilter.hpp (RealFilterCore, он же FilterCore)
// на таблицах прошивки из decompiled data/mnm_filter_full_dump_juce_real.
// Здесь раньше была ВТОРАЯ приближённая копия того же класса с теми же
// константами -- источник MSVC C2374/C2084/C2371 и лавины ложных C2568/C2440.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// AMP envelope -- kernel func_0004A8 P:$4A8-$4F5
// ---------------------------------------------------------------------------
class AmpEnvelope {
public:
    enum class State : uint8_t { Other=0,Attack=1,Release=2,Decay=4,Sustain=5 };
    void reset(){ level=0; state=State::Other; blockCounter=0; smoothed=0; }
    void setParameters(float attack,float decay,float release,float sustainLevel=0.0f){
        atk=std::clamp(attack,0.0f,127.0f); dec=std::clamp(decay,0.0f,127.0f); rel=std::clamp(release,0.0f,127.0f);
        sustain=clampi(sustainLevel,0,1)*clampi(sustainLevel,0,1);
    }
    void trigger(){ state=State::Attack; forced=true; }
    void release(){ if(state!=State::Other){ state=State::Release; forced=true; } }
    void hold(){ state=State::Sustain; forced=true; }
    // 1.6.6: AHDSR hold support -- freeze at peak after attack until hold expires.
    void holdPeak(){ state=State::Sustain; level=1.0f; }
    void resumeAfterHold(){ if(state==State::Sustain) state=State::Decay; }
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
        const float n=std::clamp(param,0.0f,127.0f)/127.0f;
        const int hold=1+static_cast<int>(n*n*63.0f*rateScale);
        if(counter<=0){ heldL=inL; heldR=inR; counter=hold; }
        --counter; outL=heldL; outR=heldR;
    }
private:
    int counter=0; float heldL=0,heldR=0;
};

} // namespace mnm
} // namespace monomachine
