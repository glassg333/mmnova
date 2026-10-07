#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ZigZagAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ZigZagAudioProcessorEditor(ZigZagAudioProcessor&);
    ~ZigZagAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    ZigZagAudioProcessor& processor;
    std::array<juce::Slider, 4> sliders;
    std::array<juce::Label, 4> labels;
    std::array<std::unique_ptr<Attachment>, 4> attachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZigZagAudioProcessorEditor)
};
