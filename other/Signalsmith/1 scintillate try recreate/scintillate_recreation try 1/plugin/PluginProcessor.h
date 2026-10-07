#pragma once

#include <JuceHeader.h>

#include "scintillate/ScintillateDSP.h"

class ScintillateAudioProcessor final : public juce::AudioProcessor
{
public:
    ScintillateAudioProcessor();
    ~ScintillateAudioProcessor() override = default;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Scintillate Recreation"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 50.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

private:
    friend class ScintillateAudioProcessorEditor;

    scintillate::Parameters readParameters() const noexcept;

    juce::AudioProcessorValueTreeState parameters;
    scintillate::DSP dsp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScintillateAudioProcessor)
};
