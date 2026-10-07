#pragma once

#include <JuceHeader.h>
#include "combscanner/CombScannerDSP.h"

class CombScannerAudioProcessorEditor;

class CombScannerAudioProcessor final : public juce::AudioProcessor
{
public:
    CombScannerAudioProcessor();
    ~CombScannerAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override;
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 8.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    using Parameters = juce::AudioProcessorValueTreeState;
    Parameters parameters;

private:
    static Parameters::ParameterLayout createParameterLayout();
    combscanner::CombScannerDSP dsp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CombScannerAudioProcessor)
};
