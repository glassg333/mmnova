#include "SlotPage.h"

#include "../PluginProcessor.h"

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

    noteLabel.setText (juce::String (engineInfo.note), juce::dontSendNotification);
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

    buildKnobs();
}

void SlotPage::buildKnobs()
{
    const auto* table = lab::engineParams (slot);
    const int count = lab::engineParamCount (slot);

    for (int i = 0; i < count; ++i)
    {
        const auto& info = table[i];
        Knob knob;
        knob.info = info;

        knob.label = std::make_unique<juce::Label>();
        knob.label->setText (info.name, juce::dontSendNotification);
        knob.label->setJustificationType (juce::Justification::centred);
        knob.label->setFont (juce::FontOptions (12.0f));
        knob.label->setColour (juce::Label::textColourId, juce::Colour (0xffcfcfe0));
        addAndMakeVisible (*knob.label);

        knob.slider = std::make_unique<juce::Slider>();
        knob.slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        knob.slider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 78, 18);
        knob.slider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff5ac8fa));
        knob.slider->setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff3a3a48));
        knob.slider->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffe8e8f0));
        knob.slider->setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        knob.slider->setDoubleClickReturnValue (true, (double) info.def);
        knob.slider->setPopupDisplayEnabled (true, false, nullptr);
        addAndMakeVisible (*knob.slider);

        knob.attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            apvts, lab::slotParamId (slot, info.key), *knob.slider);

        knobs.push_back (std::move (knob));
    }
}

void SlotPage::paint (juce::Graphics& g)
{
    g.setColour (juce::Colour (0xff20202a));
    g.fillRoundedRectangle (headerArea.toFloat(), 6.0f);
    g.setColour (juce::Colour (0xff2e2e3a));
    g.drawRoundedRectangle (headerArea.toFloat(), 6.0f, 1.0f);

    g.setColour (juce::Colour (0xff181820));
    g.fillRoundedRectangle (gridArea.toFloat().expanded (6.0f, 6.0f), 6.0f);
}

void SlotPage::resized()
{
    auto area = getLocalBounds().reduced (14);

    // --- заголовок страницы -------------------------------------------------
    headerArea = area.removeFromTop (86);
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

    area.removeFromTop (10);

    // --- сетка ручек ---------------------------------------------------------
    const int count = (int) knobs.size();

    if (count == 0)
        return;

    int columns = juce::jlimit (1, count, area.getWidth() / 104);
    int rows = (count + columns - 1) / columns;

    // подгоняем число столбцов так, чтобы высота ручек осталась разумной
    while (rows > 1 && area.getHeight() / rows < 92 && columns < count)
    {
        ++columns;
        rows = (count + columns - 1) / columns;
    }

    const int cellWidth = area.getWidth() / columns;
    const int cellHeight = juce::jlimit (76, 118, area.getHeight() / juce::jmax (1, rows));

    gridArea = area.withHeight (cellHeight * rows + 12);

    int index = 0;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns && index < count; ++col, ++index)
        {
            auto cell = juce::Rectangle<int> (area.getX() + col * cellWidth,
                                              area.getY() + row * cellHeight + 6,
                                              cellWidth, cellHeight - 8).reduced (4, 0);
            auto& knob = knobs[(size_t) index];
            knob.label->setBounds (cell.removeFromTop (16));
            knob.slider->setBounds (cell);
        }
    }
}
