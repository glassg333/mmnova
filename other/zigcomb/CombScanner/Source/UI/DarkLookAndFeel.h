#pragma once

#include <JuceHeader.h>

#include <algorithm>

namespace combscanner::ui
{
namespace Colours
{
inline const juce::Colour background { 0xff0b0b10 };
inline const juce::Colour panel { 0xff14141b };
inline const juce::Colour panelRaised { 0xff1b1b23 };
inline const juce::Colour cell { 0xff111117 };
inline const juce::Colour edge { 0xff383841 };
inline const juce::Colour text { 0xffe6e4ec };
inline const juce::Colour muted { 0xff9b99a6 };
inline const juce::Colour accent { 0xffe934b7 };
inline const juce::Colour accentDim { 0xff76235f };
inline const juce::Colour orange { 0xfff18a31 };
inline const juce::Colour blue { 0xff3138cc };
inline const juce::Colour red { 0xffe93025 };
inline const juce::Colour green { 0xff61ee4d };
inline const juce::Colour yellow { 0xffe9bb46 };
inline const juce::Colour purple { 0xff8123e8 };
}

class DarkLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    DarkLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, Colours::text);
        setColour(juce::Slider::trackColourId, Colours::accent);
        setColour(juce::Slider::backgroundColourId, Colours::panelRaised);
        setColour(juce::ComboBox::backgroundColourId, Colours::cell);
        setColour(juce::ComboBox::textColourId, Colours::text);
        setColour(juce::ComboBox::outlineColourId, Colours::edge);
        setColour(juce::ComboBox::arrowColourId, Colours::muted);
        setColour(juce::Label::textColourId, Colours::text);
        setColour(juce::TextButton::textColourOffId, Colours::text);
        setColour(juce::TextButton::textColourOnId, Colours::text);
        setColour(juce::PopupMenu::backgroundColourId, Colours::panelRaised);
        setColour(juce::PopupMenu::textColourId, Colours::text);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, Colours::accent);
        setColour(juce::PopupMenu::highlightedTextColourId, Colours::text);
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        juce::ignoreUnused(minSliderPos, maxSliderPos, slider);
        if (style == juce::Slider::LinearVertical || style == juce::Slider::LinearBarVertical)
        {
            const float centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
            const float top = static_cast<float>(y) + 3.0f;
            const float bottom = static_cast<float>(y + height) - 3.0f;
            const float centreY = (top + bottom) * 0.5f;
            g.setColour(Colours::panelRaised);
            g.fillRoundedRectangle(centreX - 2.0f, top, 4.0f, bottom - top, 2.0f);
            g.setColour(Colours::accent.withAlpha(0.75f));
            const float a = std::min(centreY, sliderPos);
            const float b = std::max(centreY, sliderPos);
            g.fillRoundedRectangle(centreX - 2.0f, a, 4.0f, std::max(2.0f, b - a), 2.0f);
            g.setColour(Colours::text);
            g.fillEllipse(centreX - 4.0f, sliderPos - 4.0f, 8.0f, 8.0f);
            return;
        }

        const float trackY = static_cast<float>(y) + static_cast<float>(height) * 0.55f;
        const float left = static_cast<float>(x) + 2.0f;
        const float right = static_cast<float>(x + width) - 2.0f;
        g.setColour(Colours::panelRaised);
        g.fillRoundedRectangle(left, trackY - 1.5f, std::max(0.0f, right - left), 3.0f, 1.5f);
        g.setColour(Colours::accent);
        g.fillRoundedRectangle(left, trackY - 1.5f, std::max(0.0f, sliderPos - left), 3.0f, 1.5f);
        g.setColour(Colours::text);
        g.fillEllipse(sliderPos - 4.0f, trackY - 4.0f, 8.0f, 8.0f);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override
    {
        juce::ignoreUnused(slider);
        const float size = static_cast<float>(std::min(width, height));
        const float radius = std::max(2.0f, size * 0.38f);
        const float centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
        const float centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(Colours::panelRaised);
        g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);
        g.setColour(Colours::edge);
        g.drawEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.0f);

        juce::Path arc;
        arc.addCentredArc(centreX, centreY, radius - 2.5f, radius - 2.5f, 0.0f,
                          rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(Colours::edge);
        g.strokePath(arc, juce::PathStrokeType(2.0f));
        juce::Path valueArc;
        valueArc.addCentredArc(centreX, centreY, radius - 2.5f, radius - 2.5f, 0.0f,
                               rotaryStartAngle, angle, true);
        g.setColour(Colours::accent);
        g.strokePath(valueArc, juce::PathStrokeType(2.8f));

        const float pointerLength = radius * 0.58f;
        const float px = centreX + std::sin(angle) * pointerLength;
        const float py = centreY - std::cos(angle) * pointerLength;
        g.setColour(Colours::text);
        g.drawLine(centreX, centreY, px, py, 2.0f);
        g.fillEllipse(centreX - 2.2f, centreY - 2.2f, 4.4f, 4.4f);
    }

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override
    {
        juce::ignoreUnused(isButtonDown, buttonX, buttonY, buttonW, buttonH, box);
        auto bounds = juce::Rectangle<float>(0.5f, 0.5f, static_cast<float>(width) - 1.0f,
                                             static_cast<float>(height) - 1.0f);
        g.setColour(Colours::cell);
        g.fillRoundedRectangle(bounds, 3.0f);
        g.setColour(Colours::edge);
        g.drawRoundedRectangle(bounds, 3.0f, 1.0f);
        const float cx = static_cast<float>(width) - 13.0f;
        const float cy = static_cast<float>(height) * 0.5f;
        juce::Path arrow;
        arrow.startNewSubPath(cx - 3.5f, cy - 1.5f);
        arrow.lineTo(cx, cy + 2.5f);
        arrow.lineTo(cx + 3.5f, cy - 1.5f);
        g.setColour(Colours::muted);
        g.strokePath(arrow, juce::PathStrokeType(1.4f));
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
        juce::Colour fill = backgroundColour;
        if (button.getToggleState())
            fill = Colours::accent;
        else if (isButtonDown)
            fill = Colours::accentDim;
        else if (isMouseOverButton)
            fill = Colours::panelRaised.brighter(0.12f);
        g.setColour(fill);
        g.fillRoundedRectangle(bounds, 3.0f);
        g.setColour(button.getToggleState() ? Colours::accent.brighter(0.25f) : Colours::edge);
        g.drawRoundedRectangle(bounds, 3.0f, 1.0f);
    }
};

} // namespace combscanner::ui
