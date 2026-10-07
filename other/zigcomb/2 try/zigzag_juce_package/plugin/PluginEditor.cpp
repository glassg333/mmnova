#include "PluginEditor.h"

namespace
{
constexpr std::array<const char*, 4> parameterIds { "radius", "jump", "angle", "move" };
constexpr std::array<const char*, 4> parameterNames { "RADIUS", "JUMP", "ANGLE", "MOVE" };
constexpr std::array<double, 4> sliderDefaults { 0.766, 0.0, 0.25, 0.0 };
}

ZigZagAudioProcessorEditor::ZigZagAudioProcessorEditor(ZigZagAudioProcessor& audioProcessor)
    : AudioProcessorEditor(&audioProcessor), processor(audioProcessor)
{
    setResizable(true, true);
    setResizeLimits(440, 260, 900, 500);
    setSize(640, 350);

    for (std::size_t i = 0; i < sliders.size(); ++i)
    {
        auto& slider = sliders[i];
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 88, 20);
        slider.setRange(0.0, 1.0);
        slider.setDoubleClickReturnValue(true, sliderDefaults[i]);
        slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffe9c46a));
        slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xfffff3c4));
        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xfff4f1de));
        addAndMakeVisible(slider);

        auto& label = labels[i];
        label.setText(parameterNames[i], juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colour(0xfff4f1de));
        label.setFont(juce::Font(12.0f, juce::Font::bold));
        addAndMakeVisible(label);

        attachments[i] = std::make_unique<Attachment>(processor.parameters, parameterIds[i], slider);
    }
}

void ZigZagAudioProcessorEditor::paint(juce::Graphics& graphics)
{
    graphics.fillAll(juce::Colour(0xff111827));
    auto bounds = getLocalBounds().toFloat();

    graphics.setColour(juce::Colour(0xff233047));
    graphics.fillRoundedRectangle(bounds.reduced(12.0f), 12.0f);
    graphics.setColour(juce::Colour(0xffe9c46a));
    graphics.fillRect(28.0f, 26.0f, 4.0f, 42.0f);

    graphics.setColour(juce::Colour(0xfff4f1de));
    graphics.setFont(juce::Font(27.0f, juce::Font::bold));
    graphics.drawText("ZIGZAG", 46, 22, 220, 32, juce::Justification::left);

    graphics.setColour(juce::Colour(0xffa9b7c9));
    graphics.setFont(juce::Font(12.0f));
    graphics.drawText("four-line modulated FDN / RNBO port", 47, 53, 270, 20, juce::Justification::left);
    graphics.drawText("4 PARAMETERS", getWidth() - 125, 34, 90, 18, juce::Justification::right);
}

void ZigZagAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(34, 100);
    const int gap = 10;
    const int width = (area.getWidth() - gap * 3) / 4;

    for (std::size_t i = 0; i < sliders.size(); ++i)
    {
        auto column = area.removeFromLeft(width);
        sliders[i].setBounds(column);
        labels[i].setBounds(column.getX(), column.getY() - 22, column.getWidth(), 18);
        area.removeFromLeft(gap);
    }
}
