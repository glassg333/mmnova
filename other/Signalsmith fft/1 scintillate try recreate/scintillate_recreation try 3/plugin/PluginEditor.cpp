#include "PluginEditor.h"

#include <array>
#include <utility>

ScintillateAudioProcessorEditor::ScintillateAudioProcessorEditor(ScintillateAudioProcessor& owner)
    : AudioProcessorEditor(owner), processor(owner)
{
    const std::array<const char*, 10> names {
        "Mix", "Width", "Low Cut", "High Cut", "Length",
        "Tone", "Rate", "Decay", "Density", "Shimmer"
    };
    const std::array<const char*, 10> ids {
        "mix", "width", "lowCut", "highCut", "length",
        "tone", "rate", "decay", "density", "shimmer"
    };
    const std::array<std::pair<double, double>, 10> ranges {
        std::pair { 0.0, 1.0 }, std::pair { 0.0, 1.0 },
        std::pair { 0.0, 1.0 }, std::pair { 0.0, 1.0 },
        std::pair { 0.0, 1.0 }, std::pair { 0.0, 1.0 },
        std::pair { 0.1, 100.0 }, std::pair { -1.0, 1.0 },
        std::pair { 0.0, 1.0 }, std::pair { 0.0, 1.0 }
    };

    for (std::size_t i = 0; i < controls.size(); ++i)
    {
        auto& control = controls[i];
        control.slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        control.slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 18);
        control.slider.setRange(ranges[i].first, ranges[i].second, 0.001);
        control.slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff21d9b4));
        control.label.setText(names[i], juce::dontSendNotification);
        control.label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(control.slider);
        addAndMakeVisible(control.label);
        control.attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, ids[i], control.slider);
    }

    setSize(760, 360);
}

void ScintillateAudioProcessorEditor::paint(juce::Graphics& graphics)
{
    graphics.fillAll(juce::Colour(0xff07151a));
    graphics.setColour(juce::Colour(0xff35f0bd).withAlpha(0.20f));
    graphics.fillRect(getLocalBounds().removeFromTop(120));
    graphics.setColour(juce::Colours::white);
    graphics.setFont(juce::Font(22.0f, juce::Font::bold));
    graphics.drawText("SCINTILLATE RECREATION", 24, 18, 400, 28, juce::Justification::left);
    graphics.setColour(juce::Colour(0xffa9b8b7));
    graphics.setFont(13.0f);
    graphics.drawText("Low-latency spectral sparkle prototype", 26, 52, 400, 20, juce::Justification::left);
}

void ScintillateAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20).withTop(130);
    const auto cellWidth = area.getWidth() / static_cast<int>(controls.size());
    for (auto& control : controls)
    {
        auto cell = area.removeFromLeft(cellWidth).reduced(5);
        control.label.setBounds(cell.removeFromBottom(22));
        control.slider.setBounds(cell);
    }
}
