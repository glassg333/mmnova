#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

class ScintillateAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ScintillateAudioProcessorEditor(ScintillateAudioProcessor&);
    ~ScintillateAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    struct Control
    {
        juce::Slider slider;
        juce::Label label;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    };

    ScintillateAudioProcessor& processor;
    std::array<Control, 10> controls;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScintillateAudioProcessorEditor)
};
