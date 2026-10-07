#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class CsMmAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit CsMmAudioProcessorEditor(CsMmAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    using SliderAttachment=juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment=juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    CsMmAudioProcessor& processor; juce::ComboBox algorithm; juce::Label algorithmLabel;
    std::array<juce::Slider,12> sliders; std::array<juce::Label,12> labels;
    std::array<std::unique_ptr<SliderAttachment>,12> attachments; std::unique_ptr<ComboAttachment> algorithmAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CsMmAudioProcessorEditor)
};
