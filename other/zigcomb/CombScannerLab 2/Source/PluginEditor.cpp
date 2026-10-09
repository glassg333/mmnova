#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace
{
constexpr int kTopBarHeight = 58;
constexpr int kBottomBarHeight = 104;
constexpr int kKnobWidth = 92;
constexpr int kKnobHeight = 84;
}

CombScannerLabAudioProcessorEditor::CombScannerLabAudioProcessorEditor (CombScannerLabAudioProcessor& processorIn)
    : AudioProcessorEditor (&processorIn),
      processor (processorIn)
{
    // используется стандартная тёмная схема LookAndFeel_V4

    titleLabel.setText ("Comb Scanner Lab", juce::dontSendNotification);
    titleLabel.setFont (juce::FontOptions (22.0f, juce::Font::bold));
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (0xffe8e8f0));
    addAndMakeVisible (titleLabel);

    statusLabel.setFont (juce::FontOptions (15.0f));
    statusLabel.setColour (juce::Label::textColourId, juce::Colour (0xff5ac8fa));
    statusLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (statusLabel);

    for (auto* button : { &prevButton, &nextButton })
    {
        button->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3a3a48));
        button->onClick = [this, button]
        {
            switchToSlot (processor.getActiveSlot() + (button == &prevButton ? -1 : 1), false);
        };
        addAndMakeVisible (*button);
    }

    tabs.setOutline (0);
    tabs.setTabBarDepth (34);
    tabs.setColour (juce::TabbedComponent::backgroundColourId, juce::Colour (0xff15151c));
    tabs.setColour (juce::TabbedComponent::outlineColourId, juce::Colour (0xff2e2e3a));
    tabs.getTabbedButtonBar().setColour (juce::TabbedButtonBar::tabOutlineColourId, juce::Colour (0xff2e2e3a));
    tabs.getTabbedButtonBar().setColour (juce::TabbedButtonBar::tabTextColourId, juce::Colour (0xffb9b9c8));
    tabs.getTabbedButtonBar().setColour (juce::TabbedButtonBar::frontTextColourId, juce::Colour (0xffffffff));

    for (int slot = 0; slot < CombScannerLabAudioProcessor::numSlots; ++slot)
    {
        const auto info = lab::engineInfo (slot);
        auto* page = new SlotPage (processor, slot);
        pages.push_back (page);
        tabs.addTab (juce::String (slot + 1) + " " + info.title, juce::Colour (0xff15151c), page, true);
    }

    addAndMakeVisible (tabs);
    tabs.getTabbedButtonBar().addChangeListener (this);

    buildGlobalControls();

    processor.apvts.addParameterListener (CombScannerLabAudioProcessor::activeSlotId, this);

    const int slot = processor.getActiveSlot();
    syncing = true;
    tabs.setCurrentTabIndex (slot, false);
    syncing = false;
    lastKnownSlot = slot;
    updateStatusText (slot);

    setResizable (true, true);
    setResizeLimits (920, 560, 1800, 1200);
    setSize (1180, 820);
    setWantsKeyboardFocus (true);
}

CombScannerLabAudioProcessorEditor::~CombScannerLabAudioProcessorEditor()
{
    tabs.getTabbedButtonBar().removeChangeListener (this);
    processor.apvts.removeParameterListener (CombScannerLabAudioProcessor::activeSlotId, this);
}

void CombScannerLabAudioProcessorEditor::buildGlobalControls()
{
    auto addKnob = [this] (const juce::String& title, const juce::String& paramId, const juce::String& tooltip)
    {
        auto knob = std::make_unique<MiniKnob>();
        knob->label.setText (title, juce::dontSendNotification);
        knob->label.setJustificationType (juce::Justification::centred);
        knob->label.setFont (juce::FontOptions (12.0f));
        knob->label.setColour (juce::Label::textColourId, juce::Colour (0xffcfcfe0));
        addAndMakeVisible (knob->label);

        knob->slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        knob->slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 68, 17);
        knob->slider.setTooltip (tooltip);
        knob->slider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffffa94d));
        knob->slider.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff3a3a48));
        knob->slider.setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffe8e8f0));
        knob->slider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        addAndMakeVisible (knob->slider);

        knob->attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            processor.apvts, paramId, knob->slider);

        globalKnobs.push_back (std::move (knob));
    };

    addKnob ("Dry/Wet",    CombScannerLabAudioProcessor::mixId,
             "1.0 = engine output only (as in the original presets); lower blends the dry signal back in");
    addKnob ("Output",     CombScannerLabAudioProcessor::outputId, "Master output level in dB");
    addKnob ("Scan Rate",  CombScannerLabAudioProcessor::scanRateId,
             "Rate of the built-in LFO on the Scan knob (0 = static scan)");
    addKnob ("Scan Depth", CombScannerLabAudioProcessor::scanDepthId,
             "Depth of the Scan LFO sweep");

    resetOnSwitchButton.setButtonText ("Reset tails on page switch");
    resetOnSwitchButton.setColour (juce::ToggleButton::textColourId, juce::Colour (0xffcfcfe0));
    resetOnSwitchButton.setColour (juce::ToggleButton::tickColourId, juce::Colour (0xff5ac8fa));
    addAndMakeVisible (resetOnSwitchButton);
    resetOnSwitchButton.setTooltip ("Clear engine state when leaving/entering a page, so A/B comparison stays honest");

    resetSwitchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        processor.apvts, CombScannerLabAudioProcessor::resetSwitchId, resetOnSwitchButton);

    copyButton.setButtonText ("Copy page to all");
    copyButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2f5d3a));
    copyButton.onClick = [this] { processor.copySlotToAll (processor.getActiveSlot()); };
    addAndMakeVisible (copyButton);

    resetButton.setButtonText ("Reset page");
    resetButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff5d3a2f));
    resetButton.onClick = [this] { processor.resetSlotToDefaults (processor.getActiveSlot()); };
    addAndMakeVisible (resetButton);

    hintLabel.setText ("Left/right arrows or the < > buttons switch pages. Use the page Trim to match loudness for an honest A/B.",
                       juce::dontSendNotification);
    hintLabel.setFont (juce::FontOptions (12.0f));
    hintLabel.setColour (juce::Label::textColourId, juce::Colour (0xff8a8a9a));
    hintLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (hintLabel);
}

void CombScannerLabAudioProcessorEditor::switchToSlot (int slot, bool fromHost)
{
    const int count = CombScannerLabAudioProcessor::numSlots;
    slot = ((slot % count) + count) % count;

    if (! fromHost)
        processor.setActiveSlot (slot);

    syncing = true;
    tabs.setCurrentTabIndex (slot, false);
    syncing = false;
    lastKnownSlot = slot;
    updateStatusText (slot);
}

void CombScannerLabAudioProcessorEditor::updateStatusText (int slot)
{
    const auto info = lab::engineInfo (slot);
    statusLabel.setText (juce::String ("Page ") + juce::String (slot + 1) + "/"
                             + juce::String (CombScannerLabAudioProcessor::numSlots) + " - " + info.title,
                         juce::dontSendNotification);
}

void CombScannerLabAudioProcessorEditor::parameterChanged (const juce::String&, float)
{
    triggerAsyncUpdate();
}

void CombScannerLabAudioProcessorEditor::handleAsyncUpdate()
{
    const int slot = processor.getActiveSlot();

    if (slot != lastKnownSlot)
        switchToSlot (slot, true);
}

void CombScannerLabAudioProcessorEditor::changeListenerCallback (juce::ChangeBroadcaster*)
{
    if (syncing)
        return;

    const int index = tabs.getCurrentTabIndex();

    if (index >= 0 && index != processor.getActiveSlot())
        processor.setActiveSlot (index);

    lastKnownSlot = index;
    updateStatusText (index);
}

bool CombScannerLabAudioProcessorEditor::keyPressed (const juce::KeyPress& key)
{
    if (key == juce::KeyPress::leftKey)
    {
        switchToSlot (processor.getActiveSlot() - 1, false);
        return true;
    }

    if (key == juce::KeyPress::rightKey)
    {
        switchToSlot (processor.getActiveSlot() + 1, false);
        return true;
    }

    return false;
}

void CombScannerLabAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff101016));

    auto topBar = getLocalBounds().removeFromTop (kTopBarHeight);
    g.setColour (juce::Colour (0xff1b1b24));
    g.fillRect (topBar);

    auto bottomBar = getLocalBounds().removeFromBottom (kBottomBarHeight);
    g.setColour (juce::Colour (0xff1b1b24));
    g.fillRect (bottomBar);

    g.setColour (juce::Colour (0xff2e2e3a));
    g.drawHorizontalLine (kTopBarHeight, 0.0f, (float) getWidth());
    g.drawHorizontalLine ((float) (getHeight() - kBottomBarHeight), 0.0f, (float) getWidth());
}

void CombScannerLabAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    auto topBar = area.removeFromTop (kTopBarHeight).reduced (12, 8);
    titleLabel.setBounds (topBar.removeFromLeft (260));
    nextButton.setBounds (topBar.removeFromRight (40).reduced (0, 6));
    prevButton.setBounds (topBar.removeFromRight (40).reduced (0, 6));
    statusLabel.setBounds (topBar);

    auto bottomBar = area.removeFromBottom (kBottomBarHeight).reduced (12, 8);

    for (auto& knob : globalKnobs)
    {
        auto cell = bottomBar.removeFromLeft (kKnobWidth);
        knob->label.setBounds (cell.removeFromTop (15));
        knob->slider.setBounds (cell.withHeight (kKnobHeight - 12));
    }

    auto rightSide = bottomBar;

    auto buttonsRow = rightSide.removeFromRight (280);
    resetButton.setBounds (buttonsRow.removeFromBottom (26));
    buttonsRow.removeFromBottom (6);
    copyButton.setBounds (buttonsRow.removeFromBottom (26));

    rightSide.removeFromRight (12);
    auto toggleRow = rightSide.removeFromTop (24);
    resetOnSwitchButton.setBounds (toggleRow.removeFromLeft (juce::jmin (270, toggleRow.getWidth())));
    hintLabel.setBounds (rightSide.reduced (0, 4));

    tabs.setBounds (area);
}
