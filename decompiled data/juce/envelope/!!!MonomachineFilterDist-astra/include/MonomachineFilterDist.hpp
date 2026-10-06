// Public dependency-free C++17 interface. No oscillator, AMP, delay or SRR.
#pragma once
#include "FirmwareFilterDist.hpp"
#include <array>
#include <cmath>
#include <cstddef>

namespace mnm132 {
struct Parameters {
    // Hardware/raw 0..127, not Hz or an invented Q factor.
    int base=0,width=127,hpq=0,lpq=0,attack=0,decay=32;
    int baseOffset=64,widthOffset=64,distortion=64;
    bool highPassKeyTracking=true,lowPassKeyTracking=true;
};
struct RawOutput {
    // Exact X/Y memory pairs at the filter/AMP boundary P:$088E.
    // Do not discard the low words when comparing against a DSP trace.
    std::array<uint64_t,16> left{},right{};
};
struct Diagnostics {
    uint32_t envelopeLevel=0,envelopePhase=0;
    int32_t highPassIndex=0,lowPassIndex=0;
    bool memoryFault=false;
};
class FilterDist {
public:
    static constexpr unsigned blockSize=16;
    static constexpr double nativeSampleRate=44100.0;
    FilterDist() noexcept { reset(); }
    void reset() noexcept {
        s_=detail::State{};parameters_=Parameters{};pitch_=pitchWord(60);
        // Kernel P:$00F0..$00F1 default, before machine-specific overrides.
        s_.X[page+0x0b]=0x200000u;
        setParameters(parameters_);
        pendingTrigger_=false;
    }
    void setParameters(const Parameters& p) noexcept {
        parameters_=p;
        const int raw[8]={p.base,p.width,p.hpq,p.lpq,p.attack,p.decay,p.baseOffset,p.widthOffset};
        for(unsigned i=0;i<8;++i)s_.Y[page+8+i]=knob(raw[i]);
        s_.Y[page+4]=knob(p.distortion);
        s_.Y[page+0x25]=(p.lowPassKeyTracking?1u<<9:0u)|(p.highPassKeyTracking?1u<<11:0u);
    }
    // UI convention: display -64..+63 equals raw-64, for DIST/BOFS/WOFS.
    static uint32_t knob(int raw) noexcept {return uint32_t(std::clamp(raw,0,127))<<16;}
    static uint32_t pitchWord(int midiNote) noexcept {return uint32_t((std::clamp(midiNote,0,127)*2048)/12);}
    void setNote(int midiNote) noexcept {pitch_=pitchWord(midiNote);}
    // Already slewed/tuned pitch from a host or full emulator, X:(page+$01).
    void setPitchWord(uint32_t word) noexcept {pitch_=std::min(word,0x5800u);}
    void triggerFilter() noexcept {pendingTrigger_=true;}
    // This is NOT the DIST knob. Some original machines override X:(page+$0B).
    // To splice into a machine, pass the captured original context word.
    bool setMachineHeadroomWord(uint32_t word) noexcept {
        if(word==0||word>=0x800000u)return false;
        s_.X[page+0x0b]=word;return true;
    }
    // Supports exact smoothed host words without re-quantizing to 7-bit knobs.
    void setFilterWords(const std::array<uint32_t,8>& words,uint32_t distWord) noexcept {
        for(unsigned i=0;i<8;++i)s_.Y[page+8+i]=words[i]&0xffffffu;
        s_.Y[page+4]=distWord&0xffffffu;
    }
    // Input boundary: post-EQ Y:$94..$A3 and Y:$D4..$E3, signed Q1.23.
    // Caller must pass exactly 16 samples/channel. No allocation, decoding or I/O.
    bool processRaw16(const int32_t* left,const int32_t* right,RawOutput& output) noexcept {
        if(s_.fault){output={};return false;}
        s_.m.fill(detail::State::mask24);
        s_.r[6]=page;s_.r[7]=page+0xdc;
        s_.sr=0x080300;
        s_.X[page+1]=pitch_;
        s_.X[page+0x0f]=0; // stereo path (mono optimization is not requested)
        s_.Y[page+0x20]=pendingTrigger_?1:0;pendingTrigger_=false;
        detail::filterEnvelope(s_);
        // EQ P:$056D..$05BE is outside this component. Its exact OUTPUT
        // buffer and post-call r7 value are the input contract of the slice.
        for(unsigned i=0;i<16;++i){
            s_.Y[0x94+i]=uint32_t(left[i])&0xffffffu;
            s_.Y[0xd4+i]=uint32_t(right[i])&0xffffffu;
        }
        s_.r[7]=page+0xe0;
        detail::filterAudio(s_);
        for(unsigned i=0;i<16;++i){
            output.left[i]=s_.readLong(0x6d+i);
            output.right[i]=s_.readLong(0xad+i);
        }
        if(s_.fault){output={};return false;}
        return true;
    }
    // Normalization here is explicitly raw Q1.47 at the AMP INPUT. This
    // component does not invent AMP gain, a unity bypass, or makeup gain.
    bool processFloat16(const float* left,const float* right,float* outLeft,float* outRight) noexcept {
        std::array<int32_t,16> l{},r{};RawOutput result;
        for(unsigned i=0;i<16;++i){l[i]=toQ23(left[i]);r[i]=toQ23(right[i]);}
        const bool ok=processRaw16(l.data(),r.data(),result);
        constexpr double inv=1.0/140737488355328.0;
        for(unsigned i=0;i<16;++i){
            outLeft[i]=float(double(detail::State::sign48(result.left[i]))*inv);
            outRight[i]=float(double(detail::State::sign48(result.right[i]))*inv);
        }
        return ok;
    }
    Diagnostics diagnostics() const noexcept {
        return {s_.X[page+0xdb],s_.Y[page+0xdb],detail::State::sign24(s_.X[page+0xd9]),
            detail::State::sign24(s_.Y[page+0xda]),s_.fault};
    }
    const detail::State& rawState() const noexcept {return s_;}
private:
    static int32_t toQ23(float value) noexcept {
        if(!std::isfinite(value))return 0;
        const double x=std::clamp(double(value),-1.0,8388607.0/8388608.0)*8388608.0;
        return int32_t(std::llround(x));
    }
    static constexpr uint32_t page=0x500;
    detail::State s_;
    Parameters parameters_;
    uint32_t pitch_=0;
    bool pendingTrigger_=false;
};
}
