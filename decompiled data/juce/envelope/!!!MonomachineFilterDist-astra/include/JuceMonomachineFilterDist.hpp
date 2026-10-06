// Optional JUCE adapter. Requires juce_audio_basics; no juce_dsp filters.
#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include "MonomachineFilterDist.hpp"
#include "ParameterSmoothing.hpp"

namespace mnm132 {
class JuceFilterDist {
public:
    // The original coefficient/time tables run at 44.1 kHz. Return failure
    // for other rates instead of silently retuning the firmware algorithm.
    bool prepare(double sampleRate) noexcept {
        ready_=std::abs(sampleRate-FilterDist::nativeSampleRate)<0.001;
        reset();return ready_;
    }
    void reset() noexcept {
        core_.reset();smoothing_.reset(parameters_);core_.setParameters(parameters_);
        inputL_.fill(0);inputR_.fill(0);outputL_.fill(0);outputR_.fill(0);
        position_=0;blockTrigger_=false;blockNote_=60;pendingNote_=60;
    }
    void setParameters(const Parameters& p) noexcept {
        parameters_=p;smoothing_.setTarget(p);core_.setParameters(p);
    }
    // Enabled by default, can be disabled for raw DSP comparisons.
    void enableHostSmoothing(bool enabled) noexcept {
        smooth_=enabled;smoothing_.reset(parameters_);
        if(!enabled)core_.setParameters(parameters_);
    }
    void setNote(int note) noexcept {pendingNote_=note;}
    void triggerFilter() noexcept {blockTrigger_=true;}
    bool ready() const noexcept {return ready_;}
    static constexpr int latencySamples() noexcept {return 16;}
    // Call on the audio thread. Host parameter synchronization belongs to
    // the AudioProcessor. Do not mutate this class concurrently from the UI.
    void process(juce::AudioBuffer<float>& buffer,const juce::MidiBuffer& midi) noexcept {
        juce::ScopedNoDenormals noDenormals;
        if(!ready_ || buffer.getNumChannels()<1){buffer.clear();return;}
        const int channels=buffer.getNumChannels();
        auto* l=buffer.getWritePointer(0);auto* r=channels>1?buffer.getWritePointer(1):l;
        for(int ch=2;ch<channels;++ch)buffer.clear(ch,0,buffer.getNumSamples());
        auto it=midi.cbegin();
        for(int i=0;i<buffer.getNumSamples();++i){
            while(it!=midi.cend() && (*it).samplePosition<=i){
                const auto message=(*it).getMessage();
                if(message.isNoteOn()){pendingNote_=message.getNoteNumber();blockTrigger_=true;}
                ++it;
            }
            if(position_==0)blockNote_=pendingNote_;
            inputL_[position_]=l[i];inputR_[position_]=r[i];
            l[i]=outputL_[position_];if(channels>1)r[i]=outputR_[position_];
            if(++position_==16){
                if(smooth_)smoothing_.applyNextBlock(core_);
                core_.setNote(blockNote_);
                if(blockTrigger_){core_.setNote(pendingNote_);core_.triggerFilter();blockTrigger_=false;}
                core_.processFloat16(inputL_.data(),inputR_.data(),outputL_.data(),outputR_.data());
                position_=0;
            }
        }
    }
    FilterDist& core() noexcept {return core_;}
    const FilterDist& core() const noexcept {return core_;}
private:
    FilterDist core_;
    ParameterSmoothing smoothing_;
    Parameters parameters_;
    std::array<float,16> inputL_{},inputR_{},outputL_{},outputR_{};
    unsigned position_=0;int pendingNote_=60,blockNote_=60;
    bool blockTrigger_=false,ready_=false,smooth_=true;
};
}
