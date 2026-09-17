#pragma once
#include "PluginProcessor.h"
class MonomachineNovaAudioProcessorEditor final : public juce::AudioProcessorEditor,private juce::Timer {
public:
    explicit MonomachineNovaAudioProcessorEditor(MonomachineNovaAudioProcessor&);
    ~MonomachineNovaAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    struct Surface;
    MonomachineNovaAudioProcessor& processor;
    std::unique_ptr<Surface> surface;
    void timerCallback() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonomachineNovaAudioProcessorEditor)
};
