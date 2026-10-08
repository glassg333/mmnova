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
//
// Параметры делятся на две группы (ParamInfo::group):
//   0 - основные ручки страницы;
//   1 - "внутренние" значения (банк отношений, режим скана и т.п.) в секции ниже,
//       каждое с коротким английским описанием во всплывающей подсказке.
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
    void buildOneKnob (const lab::ParamInfo& info, std::vector<Knob>& destination);
    static void layoutGrid (std::vector<Knob>& list, juce::Rectangle<int> area, int columns);

    CombScannerLabAudioProcessor& processor;
    juce::AudioProcessorValueTreeState& apvts;
    const int slot;
    lab::EngineInfo engineInfo;

    juce::Label titleLabel, sourceLabel, noteLabel, modelLabel;
    juce::ComboBox modelBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modelAttachment;

    std::vector<Knob> knobs;            // группа 0: основные ручки
    std::vector<Knob> advancedKnobs;    // группа 1: внутренние значения

    juce::Label advancedTitle;

    juce::Rectangle<int> headerArea, modelArea, gridArea, advancedArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlotPage)
};
