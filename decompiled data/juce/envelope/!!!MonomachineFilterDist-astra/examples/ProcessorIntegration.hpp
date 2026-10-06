#pragma once
#include <JuceMonomachineFilterDist.hpp>

// Store one of these as a member of your existing juce::AudioProcessor.
// The owning processor supplies audio/MIDI buses, parameters and state saving.
// This file is integration code, not firmware code.
class MonomachineInsert {
public:
    bool prepare(double sampleRate) noexcept { return effect_.prepare(sampleRate); }
    static constexpr int latencySamples() noexcept { return mnm132::JuceFilterDist::latencySamples(); }

    // Call from prepareToPlay/processBlock, never concurrently from the editor.
    void setParameters(const mnm132::Parameters& parameters) noexcept {
        effect_.setParameters(parameters);
    }
    void process(juce::AudioBuffer<float>& audio,const juce::MidiBuffer& midi) noexcept {
        effect_.process(audio,midi);
    }
    void triggerFilter() noexcept { effect_.triggerFilter(); }
    void setNote(int note) noexcept { effect_.setNote(note); }
    mnm132::JuceFilterDist& effect() noexcept { return effect_; }
private:
    mnm132::JuceFilterDist effect_;
};

/* In your processor:

    MonomachineInsert monomachine;

    void prepareToPlay(double rate,int) override {
        const bool supported = monomachine.prepare(rate);
        setLatencySamples(MonomachineInsert::latencySamples());
        // Report unsupported rate in your UI. The adapter returns silence then.
        juce::ignoreUnused(supported);
    }

    void processBlock(juce::AudioBuffer<float>& audio,juce::MidiBuffer& midi) override {
        mnm132::Parameters p;
        // Replace these example constants with atomic APVTS parameter reads.
        p.base=24; p.width=72; p.hpq=40; p.lpq=48;
        p.attack=8; p.decay=48; p.baseOffset=80; p.widthOffset=64;
        p.distortion=64; // displayed DIST = 0
        monomachine.setParameters(p);
        monomachine.process(audio,midi);
    }

For an audio effect without MIDI, use setNote() and triggerFilter() explicitly.
No level detector or amplitude-gate envelope is added by this component.
*/
