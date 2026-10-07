#pragma once

#include <JuceHeader.h>

#include <array>
#include <memory>
#include <vector>

#include "PluginProcessor.h"
#include "UI/DarkLookAndFeel.h"

namespace combscanner::ui
{
using APVTS = CombScannerAudioProcessor::APVTS;

class SliderCell final : public juce::Component
{
public:
    SliderCell(APVTS& state, DarkLookAndFeel& lookAndFeel, const juce::String& label,
               const juce::String& parameterID,
               juce::Slider::SliderStyle style = juce::Slider::LinearHorizontal);
    void bindTo(const juce::String& parameterID);
    void setCaption(const juce::String& caption);
    void setCompact(bool shouldBeCompact) noexcept { compact = shouldBeCompact; }
    juce::Slider& slider() noexcept { return control; }
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    APVTS& state;
    DarkLookAndFeel& lookAndFeel;
    juce::Label captionLabel;
    juce::Label valueLabel;
    juce::Slider control;
    std::unique_ptr<APVTS::SliderAttachment> attachment;
    bool compact = false;
};

class ChoiceCell final : public juce::Component
{
public:
    ChoiceCell(APVTS& state, DarkLookAndFeel& lookAndFeel, const juce::String& label,
               const juce::String& parameterID, const juce::StringArray& choices);
    void bindTo(const juce::String& parameterID);
    void setCaption(const juce::String& caption);
    void setCompact(bool shouldBeCompact) noexcept { compact = shouldBeCompact; }
    juce::ComboBox& comboBox() noexcept { return control; }
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    APVTS& state;
    DarkLookAndFeel& lookAndFeel;
    juce::StringArray choices;
    juce::Label captionLabel;
    juce::ComboBox control;
    std::unique_ptr<APVTS::ComboBoxAttachment> attachment;
    bool compact = false;
};

class ToggleCell final : public juce::Component
{
public:
    ToggleCell(APVTS& state, DarkLookAndFeel& lookAndFeel, const juce::String& label,
               const juce::String& parameterID);
    void bindTo(const juce::String& parameterID);
    juce::TextButton& button() noexcept { return control; }
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    APVTS& state;
    DarkLookAndFeel& lookAndFeel;
    juce::String caption;
    juce::TextButton control;
    std::unique_ptr<APVTS::ButtonAttachment> attachment;
};

class FxRow final : public juce::Component
{
public:
    FxRow(APVTS& state, DarkLookAndFeel& lookAndFeel, int oneBasedSlot);
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    int slot;
    juce::Label title;
    ChoiceCell type;
    ChoiceCell position;
    SliderCell mix;
    SliderCell size;
    SliderCell pitch;
    SliderCell blur;
    ToggleCell freeze;
};

class ModulationPanel final : public juce::Component
{
public:
    ModulationPanel(APVTS& state, DarkLookAndFeel& lookAndFeel);
    ~ModulationPanel() override;
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    class LfoRow;
    class RouteRow;
    APVTS& state;
    DarkLookAndFeel& lookAndFeel;
    juce::TextButton lfoTab { "LFO" };
    juce::TextButton routeTab { "MATRIX" };
    juce::TextButton previousPage { "<" };
    juce::TextButton nextPage { ">" };
    juce::Label pageLabel;
    std::array<std::unique_ptr<LfoRow>, kNumLfos> lfoRows;
    std::array<std::unique_ptr<RouteRow>, 8> routeRows;
    bool showingRoutes = false;
    int routePage = 0;
    void setRoutePage(int page);
    void updateVisibility();
};

class SequencerPanel final : public juce::Component
{
public:
    SequencerPanel(APVTS& state, DarkLookAndFeel& lookAndFeel);
    ~SequencerPanel() override;
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    class StepCell;
    APVTS& state;
    DarkLookAndFeel& lookAndFeel;
    juce::Label title;
    std::array<juce::TextButton, kNumSequences> sequenceButtons;
    std::array<std::unique_ptr<StepCell>, kSequenceSteps> steps;
    SliderCell length;
    SliderCell rate;
    SliderCell slew;
    ChoiceCell mode;
    ChoiceCell division;
    ToggleCell sync;
    int selectedSequence = 0;
    void selectSequence(int index);
};

} // namespace combscanner::ui

class CombScannerAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit CombScannerAudioProcessorEditor(CombScannerAudioProcessor& processor);
    ~CombScannerAudioProcessorEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    CombScannerAudioProcessor& processor;
    combscanner::ui::DarkLookAndFeel lookAndFeel;

    juce::Label title;
    juce::Label subtitle;
    juce::ComboBox presetBox;
    juce::Label presetLabel;
    combscanner::ui::ChoiceCell modeCell;
    combscanner::ui::ChoiceCell modelCell;
    combscanner::ui::ToggleCell bypassCell;
    combscanner::ui::SliderCell masterMix;
    combscanner::ui::SliderCell outputLevel;

    juce::Label combSectionTitle;
    std::array<juce::TextButton, combscanner::kNumCombs> combButtons;
    combscanner::ui::ToggleCell combEnabled;
    std::vector<std::unique_ptr<combscanner::ui::SliderCell>> combControls;
    std::vector<juce::String> combControlKeys;
    std::vector<std::unique_ptr<combscanner::ui::SliderCell>> scannerControls;
    std::vector<juce::String> scannerControlKeys;
    combscanner::ui::ChoiceCell scanShapeCell;
    std::vector<std::unique_ptr<combscanner::ui::SliderCell>> classicControls;
    std::vector<juce::String> classicControlKeys;
    std::array<std::unique_ptr<combscanner::ui::FxRow>, combscanner::kNumFxSlots> fxRows;
    combscanner::ui::ModulationPanel modulationPanel;
    combscanner::ui::SequencerPanel sequencerPanel;

    int selectedComb = 0;
    juce::Rectangle<int> combPanelBounds, fxPanelBounds, modPanelBounds, sequencerBounds;
    void selectComb(int index);
    void updateModeState();
    void layoutControlGrid(std::vector<std::unique_ptr<combscanner::ui::SliderCell>>& controls,
                           juce::Rectangle<int> area, int columns);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CombScannerAudioProcessorEditor)
};
