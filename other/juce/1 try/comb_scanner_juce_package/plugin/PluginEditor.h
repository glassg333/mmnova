#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CombScannerAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit CombScannerAudioProcessorEditor(CombScannerAudioProcessor&);
    ~CombScannerAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    CombScannerAudioProcessor& processor;
    std::array<juce::Slider, 6> sliders;
    std::array<juce::Label, 6> labels;
    std::array<std::unique_ptr<Attachment>, 6> attachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CombScannerAudioProcessorEditor)
};
