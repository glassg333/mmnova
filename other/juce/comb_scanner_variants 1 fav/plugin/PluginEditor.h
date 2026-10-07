#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CombScannerVariantsAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit CombScannerVariantsAudioProcessorEditor(CombScannerVariantsAudioProcessor&);
    ~CombScannerVariantsAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    CombScannerVariantsAudioProcessor& processor;
    juce::ComboBox model;
    juce::Label modelLabel;
    std::array<juce::Slider, 6> sliders;
    std::array<juce::Label, 6> labels;
    std::array<std::unique_ptr<SliderAttachment>, 6> sliderAttachments;
    std::unique_ptr<ComboAttachment> modelAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CombScannerVariantsAudioProcessorEditor)
};
