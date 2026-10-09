#include "SlotPage.h"

#include "../PluginProcessor.h"

namespace
{
// Длинное описание движка рисуем в 2-3 строки: Label не переносит текст сам.
juce::String wrapText (const juce::String& text, int maxCharsPerLine, int maxLines)
{
    juce::StringArray words;
    words.addTokens (text, " ", "");

    juce::StringArray lines;
    juce::String current;

    for (const auto& word : words)
    {
        const auto candidate = current.isEmpty() ? word : current + " " + word;

        if (candidate.length() > maxCharsPerLine && current.isNotEmpty())
        {
            lines.add (current);
            current = word;
        }
        else
        {
            current = candidate;
        }
    }

    if (current.isNotEmpty())
        lines.add (current);

    if (lines.size() > maxLines)
    {
        lines.removeRange (maxLines, lines.size());
        lines.set (maxLines - 1, lines[maxLines - 1].substring (0, juce::jmax (0, maxCharsPerLine - 3)) + "...");
    }

    return lines.joinIntoString ("\n");
}
}

SlotPage::SlotPage (CombScannerLabAudioProcessor& processorIn, int slotIndex)
    : processor (processorIn),
      apvts (processorIn.apvts),
      slot (slotIndex),
      engineInfo (lab::engineInfo (slotIndex))
{
    titleLabel.setText (juce::String (slot + 1) + ". " + engineInfo.title, juce::dontSendNotification);
    titleLabel.setFont (juce::FontOptions (20.0f, juce::Font::bold));
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (0xffe8e8f0));
    addAndMakeVisible (titleLabel);

    sourceLabel.setText (juce::String ("code: ") + engineInfo.source, juce::dontSendNotification);
    sourceLabel.setFont (juce::FontOptions (12.0f));
    sourceLabel.setColour (juce::Label::textColourId, juce::Colour (0xff8fd6a0));
    addAndMakeVisible (sourceLabel);

    noteLabel.setText (wrapText (juce::String (engineInfo.note), 150, 3), juce::dontSendNotification);
    noteLabel.setFont (juce::FontOptions (13.0f));
    noteLabel.setColour (juce::Label::textColourId, juce::Colour (0xffb9b9c8));
    noteLabel.setJustificationType (juce::Justification::topLeft);
    addAndMakeVisible (noteLabel);

    const int models = lab::engineModelCount (slot);

    if (models > 0)
    {
        modelLabel.setText ("Model", juce::dontSendNotification);
        modelLabel.setFont (juce::FontOptions (13.0f, juce::Font::bold));
        modelLabel.setColour (juce::Label::textColourId, juce::Colour (0xffb9b9c8));
        addAndMakeVisible (modelLabel);

        for (int m = 0; m < models; ++m)
            modelBox.addItem (lab::engineModelName (slot, m), m + 1);

        addAndMakeVisible (modelBox);
        modelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
            apvts, lab::slotModelId (slot), modelBox);
    }

    advancedTitle.setText ("Internal values \xE2\x80\x94 the exact numbers the code uses "
                           "(hover a knob for its description)",
                           juce::dontSendNotification);
    advancedTitle.setFont (juce::FontOptions (12.0f, juce::Font::italic));
    advancedTitle.setColour (juce::Label::textColourId, juce::Colour (0xff9a9ab0));
    addAndMakeVisible (advancedTitle);

    buildKnobs();

    if (advancedKnobs.empty())
        advancedTitle.setVisible (false);
}

void SlotPage::buildKnobs()
{
    const auto* table = lab::engineParams (slot);
    const int count = lab::engineParamCount (slot);

    for (int i = 0; i < count; ++i)
    {
        const auto& info = table[i];

        if (info.group == 0)
            buildOneKnob (info, knobs);
        else
            buildOneKnob (info, advancedKnobs);
    }
}

void SlotPage::buildOneKnob (const lab::ParamInfo& info, std::vector<Knob>& destination)
{
    const bool small = info.group != 0;
    Knob knob;
    knob.info = info;

    knob.label = std::make_unique<juce::Label>();
    knob.label->setText (info.name, juce::dontSendNotification);
    knob.label->setJustificationType (juce::Justification::centred);
    knob.label->setFont (juce::FontOptions (small ? 11.0f : 12.0f));
    knob.label->setColour (juce::Label::textColourId, juce::Colour (0xffcfcfe0));

    if (info.desc != nullptr)
        knob.label->setTooltip (info.desc);

    addAndMakeVisible (*knob.label);

    knob.slider = std::make_unique<juce::Slider>();
    knob.slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    knob.slider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, small ? 62 : 78, small ? 15 : 18);
    knob.slider->setColour (juce::Slider::rotarySliderFillColourId, small ? juce::Colour (0xff8f7ffa)
                                                                          : juce::Colour (0xff5ac8fa));
    knob.slider->setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff3a3a48));
    knob.slider->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffe8e8f0));
    knob.slider->setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    knob.slider->setDoubleClickReturnValue (true, (double) info.def);
    knob.slider->setPopupDisplayEnabled (true, false, nullptr);

    if (info.desc != nullptr)
        knob.slider->setTooltip (info.desc);

    addAndMakeVisible (*knob.slider);

    knob.attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        apvts, lab::slotParamId (slot, info.key), *knob.slider);

    destination.push_back (std::move (knob));
}

void SlotPage::paint (juce::Graphics& g)
{
    g.setColour (juce::Colour (0xff20202a));
    g.fillRoundedRectangle (headerArea.toFloat(), 6.0f);
    g.setColour (juce::Colour (0xff2e2e3a));
    g.drawRoundedRectangle (headerArea.toFloat(), 6.0f, 1.0f);

    g.setColour (juce::Colour (0xff181820));
    g.fillRoundedRectangle (gridArea.toFloat().expanded (6.0f, 6.0f), 6.0f);

    if (! advancedKnobs.empty())
    {
        g.setColour (juce::Colour (0xff14141c));
        g.fillRoundedRectangle (advancedArea.toFloat().expanded (6.0f, 4.0f), 6.0f);
        g.setColour (juce::Colour (0xff2a2a38));
        g.drawRoundedRectangle (advancedArea.toFloat().expanded (6.0f, 4.0f), 6.0f, 1.0f);
    }
}

void SlotPage::layoutGrid (std::vector<Knob>& list, juce::Rectangle<int> area, int columns)
{
    const int count = (int) list.size();

    if (count == 0 || columns <= 0 || area.getHeight() < 30)
        return;

    columns = juce::jlimit (1, count, columns);
    const int rows = (count + columns - 1) / columns;
    const int cellWidth = area.getWidth() / columns;
    const int cellHeight = area.getHeight() / juce::jmax (1, rows);
    int index = 0;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns && index < count; ++col, ++index)
        {
            const bool small = list[(size_t) index].info.group != 0;
            // Ограничиваем ширину ячейки, иначе при 4-7 ручках в одном ряду
            // крутилки разрастаются на пол-страницы.
            const int maxCellWidth = small ? 96 : 150;
            auto cell = juce::Rectangle<int> (area.getX() + col * cellWidth,
                                              area.getY() + row * cellHeight,
                                              cellWidth, cellHeight)
                            .reduced (4, small ? 1 : 3)
                            .withSizeKeepingCentre (juce::jmin (cellWidth - 8, maxCellWidth), cellHeight - (small ? 2 : 6));
            auto& knob = list[(size_t) index];
            knob.label->setBounds (cell.removeFromTop (small ? 13 : 16));
            knob.slider->setBounds (cell);
        }
    }
}

void SlotPage::resized()
{
    auto area = getLocalBounds().reduced (14);

    // --- заголовок страницы -------------------------------------------------
    headerArea = area.removeFromTop (120);
    {
        auto header = headerArea.reduced (12, 8);
        titleLabel.setBounds (header.removeFromTop (24));

        if (lab::engineModelCount (slot) > 0)
        {
            auto modelRow = header.removeFromTop (26);
            modelLabel.setBounds (modelRow.removeFromLeft (60));
            modelBox.setBounds (modelRow.removeFromLeft (juce::jmin (360, modelRow.getWidth())));
        }

        sourceLabel.setBounds (header.removeFromTop (18));
        noteLabel.setBounds (header);
    }

    area.removeFromTop (8);

    if (knobs.empty() && advancedKnobs.empty())
        return;

    // --- основные ручки: один ряд во всю ширину ------------------------------
    const auto fullArea = area;
    const int mainHeight = advancedKnobs.empty() ? juce::jmin (area.getHeight(), 150) : 132;
    auto mainArea = area.removeFromTop (juce::jmin (mainHeight, area.getHeight()));
    layoutGrid (knobs, mainArea, (int) knobs.size());

    if (advancedKnobs.empty())
    {
        gridArea = fullArea;
        return;
    }

    gridArea = mainArea;

    // --- секция "внутренних" значений ---------------------------------------
    area.removeFromTop (6);
    auto titleRow = area.removeFromTop (20);
    advancedTitle.setBounds (titleRow.reduced (8, 0));

    advancedArea = area;
    layoutGrid (advancedKnobs, advancedArea.reduced (2, 2), 7);
}
