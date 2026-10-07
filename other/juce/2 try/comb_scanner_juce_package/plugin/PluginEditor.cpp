#include "PluginEditor.h"

namespace
{
constexpr std::array<const char*, 6> parameterIds {
    "gain", "damp", "phase", "delay1", "delay2", "scan"
};
constexpr std::array<const char*, 6> parameterNames {
    "GAIN", "DAMP", "PHASE", "DELAY 1", "DELAY 2", "SCAN"
};
constexpr std::array<double, 6> sliderMinimums { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
constexpr std::array<double, 6> sliderMaximums { 0.999, 0.999, 1.0, 2000.0, 2000.0, 2.0 };
constexpr std::array<double, 6> sliderDefaults { 0.99, 0.90, 0.75, 115.0, 500.0, 0.0 };
}

CombScannerAudioProcessorEditor::CombScannerAudioProcessorEditor(CombScannerAudioProcessor& audioProcessor)
    : AudioProcessorEditor(&audioProcessor), processor(audioProcessor)
{
    setResizable(true, true);
    setResizeLimits(620, 260, 1200, 500);
    setSize(920, 350);

    for (std::size_t i = 0; i < sliders.size(); ++i)
    {
        auto& slider = sliders[i];
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 94, 20);
        slider.setRange(sliderMinimums[i], sliderMaximums[i]);
        slider.setDoubleClickReturnValue(true, sliderDefaults[i]);
        slider.setNumDecimalPlacesToDisplay(i >= 3 ? 1 : 2);
        if (i >= 3 && i <= 4)
            slider.setTextValueSuffix(" ms");
        slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff4a261));
        slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffffe8d1));
        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xfff7ede2));
        addAndMakeVisible(slider);

        auto& label = labels[i];
        label.setText(parameterNames[i], juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colour(0xfff7ede2));
        label.setFont(juce::Font(11.0f, juce::Font::bold));
        addAndMakeVisible(label);

        attachments[i] = std::make_unique<Attachment>(processor.parameters, parameterIds[i], slider);
    }
}

void CombScannerAudioProcessorEditor::paint(juce::Graphics& graphics)
{
    graphics.fillAll(juce::Colour(0xff161b22));
    const auto bounds = getLocalBounds().toFloat();
    graphics.setColour(juce::Colour(0xff272d38));
    graphics.fillRoundedRectangle(bounds.reduced(12.0f), 12.0f);
    graphics.setColour(juce::Colour(0xfff4a261));
    graphics.fillRect(28.0f, 25.0f, 4.0f, 42.0f);

    graphics.setColour(juce::Colour(0xfff7ede2));
    graphics.setFont(juce::Font(26.0f, juce::Font::bold));
    graphics.drawText("COMB SCANNER", 46, 20, 300, 34, juce::Justification::left);
    graphics.setColour(juce::Colour(0xffb7c0cc));
    graphics.setFont(juce::Font(12.0f));
    graphics.drawText("8 nested comb voices / scanning multiplexer", 47, 53, 300, 20,
                      juce::Justification::left);
    graphics.drawText("8 VOICES", getWidth() - 115, 34, 80, 18, juce::Justification::right);
}

void CombScannerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(30, 100);
    const int gap = 8;
    const int width = (area.getWidth() - gap * 5) / 6;

    for (std::size_t i = 0; i < sliders.size(); ++i)
    {
        auto column = area.removeFromLeft(width);
        sliders[i].setBounds(column);
        labels[i].setBounds(column.getX(), column.getY() - 22, column.getWidth(), 18);
        area.removeFromLeft(gap);
    }
}
