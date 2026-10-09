#pragma once

#include <JuceHeader.h>

#include <memory>
#include <vector>

#include "ui/SlotPage.h"

class CombScannerLabAudioProcessor;

// Редактор: вкладки = страницы (варианты DSP), внизу общие ручки.
class CombScannerLabAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                                 private juce::AudioProcessorValueTreeState::Listener,
                                                 private juce::ChangeListener,
                                                 private juce::AsyncUpdater
{
public:
    explicit CombScannerLabAudioProcessorEditor (CombScannerLabAudioProcessor&);
    ~CombScannerLabAudioProcessorEditor() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    bool keyPressed (const juce::KeyPress& key) override;

private:
    struct MiniKnob
    {
        juce::Label label;
        juce::Slider slider;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    };

    void parameterChanged (const juce::String& id, float newValue) override;
    void handleAsyncUpdate() override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

    void buildGlobalControls();
    void switchToSlot (int slot, bool fromHost);
    void updateStatusText (int slot);

    CombScannerLabAudioProcessor& processor;

    juce::Label titleLabel, statusLabel, hintLabel;
    juce::TextButton prevButton { "<" }, nextButton { ">" };
    juce::TextButton copyButton, resetButton;
    juce::ToggleButton resetOnSwitchButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> resetSwitchAttachment;

    juce::TabbedComponent tabs { juce::TabbedButtonBar::TabsAtTop };
    std::vector<SlotPage*> pages;

    std::vector<std::unique_ptr<MiniKnob>> globalKnobs;

    bool syncing = false;
    int lastKnownSlot = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CombScannerLabAudioProcessorEditor)
};
