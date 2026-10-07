#pragma once

#include <JuceHeader.h>
#include "zigzag/ZigZagDSP.h"

class ZigZagAudioProcessorEditor;

class ZigZagAudioProcessor final : public juce::AudioProcessor
{
public:
    ZigZagAudioProcessor();
    ~ZigZagAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override;
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 10.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destination) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    using Parameters = juce::AudioProcessorValueTreeState;
    Parameters parameters;

private:
    static Parameters::ParameterLayout createParameterLayout();
    zigzag::ZigZagDSP dsp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZigZagAudioProcessor)
};
