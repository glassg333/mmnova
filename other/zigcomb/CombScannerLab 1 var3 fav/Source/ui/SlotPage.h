#pragma once

#include <JuceHeader.h>

#include <memory>
#include <vector>

#include "../lab/Engine.h"
#include "../lab/EngineRegistry.h"

class CombScannerLabAudioProcessor;

// Одна страница = один вариант DSP. Ручки, модель и подписи строятся
// автоматически из описания движка (lab::Engine), поэтому добавить новую
// страницу можно, отредактировав только EngineRegistry.cpp.
class SlotPage final : public juce::Component
{
public:
    SlotPage (CombScannerLabAudioProcessor& processor, int slotIndex);
    ~SlotPage() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    struct Knob
    {
        std::unique_ptr<juce::Label> label;
        std::unique_ptr<juce::Slider> slider;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
        lab::ParamInfo info;
    };

    void buildKnobs();

    CombScannerLabAudioProcessor& processor;
    juce::AudioProcessorValueTreeState& apvts;
    const int slot;
    lab::EngineInfo engineInfo;

    juce::Label titleLabel, sourceLabel, noteLabel, modelLabel;
    juce::ComboBox modelBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modelAttachment;

    std::vector<Knob> knobs;

    juce::Rectangle<int> headerArea, modelArea, gridArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlotPage)
};
