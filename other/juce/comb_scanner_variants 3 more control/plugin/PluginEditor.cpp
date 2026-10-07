#include "PluginEditor.h"

CombScannerVariantsAudioProcessorEditor::CombScannerVariantsAudioProcessorEditor(
    CombScannerVariantsAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    modelLabel.setText("MODEL", juce::dontSendNotification);
    modelLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(modelLabel);
    for (int i = 0; i < combscanner::CombScannerVariantsDSP::variantCount; ++i)
        model.addItem(combscanner::CombScannerVariantsDSP::getVariantName(i), i + 1);
    addAndMakeVisible(model);
    modelAttachment = std::make_unique<ComboAttachment>(processor.parameters, "variant", model);

    const std::array<juce::String, 9> names { "FEEDBACK", "DAMP", "PHASE", "DIFFUSION", "CROSS MIX", "MOTION", "DELAY 1 MS", "DELAY 2 MS", "SCAN" };
    const std::array<const char*, 9> ids { "feedback", "damp", "phase", "diffusion", "crossMix", "motion", "delay1", "delay2", "scan" };
    for (std::size_t i = 0; i < sliders.size(); ++i)
    {
        labels[i].setText(names[i], juce::dontSendNotification);
        labels[i].setColour(juce::Label::textColourId, juce::Colours::lightgrey);
        labels[i].setJustificationType(juce::Justification::centred);
        sliders[i].setSliderStyle(juce::Slider::LinearVertical);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
        addAndMakeVisible(labels[i]);
        addAndMakeVisible(sliders[i]);
        sliderAttachments[i] = std::make_unique<SliderAttachment>(processor.parameters, ids[i], sliders[i]);
    }
    setSize(940, 330);
}

void CombScannerVariantsAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff17191d));
    g.setColour(juce::Colour(0xff30343b));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(10.0f), 10.0f);
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(22.0f, juce::Font::bold));
    g.drawText("COMB SCANNER VARIANTS", 24, 18, getWidth() - 48, 30, juce::Justification::centredLeft);
}

void CombScannerVariantsAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(24);
    area.removeFromTop(42);
    auto modelArea = area.removeFromTop(44);
    modelLabel.setBounds(modelArea.removeFromLeft(70));
    model.setBounds(modelArea);
    area.removeFromTop(12);
    const int width = area.getWidth() / static_cast<int>(sliders.size());
    for (std::size_t i = 0; i < sliders.size(); ++i)
    {
        auto column = area.removeFromLeft(width).reduced(5, 0);
        labels[i].setBounds(column.removeFromTop(24));
        sliders[i].setBounds(column);
    }
}
