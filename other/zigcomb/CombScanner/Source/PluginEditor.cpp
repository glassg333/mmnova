#include "PluginEditor.h"

#include "PluginParameters.h"

#include <algorithm>
#include <cmath>

namespace combscanner::ui
{
namespace
{
void styleSmallLabel(juce::Label& label, const juce::String& text, bool rightAligned = false)
{
    label.setText(text, juce::dontSendNotification);
    label.setFont(juce::Font(8.5f, juce::Font::bold));
    label.setColour(juce::Label::textColourId, Colours::muted);
    label.setJustificationType(rightAligned ? juce::Justification::centredRight : juce::Justification::centredLeft);
    label.setInterceptsMouseClicks(false, false);
}

juce::String twoDigit(int n)
{
    return n < 10 ? "0" + juce::String(n) : juce::String(n);
}

} // namespace

SliderCell::SliderCell(APVTS& stateToUse, DarkLookAndFeel& lookAndFeelToUse,
                       const juce::String& label, const juce::String& parameterID,
                       juce::Slider::SliderStyle style)
    : state(stateToUse), lookAndFeel(lookAndFeelToUse)
{
    styleSmallLabel(captionLabel, label.toUpperCase());
    styleSmallLabel(valueLabel, "0", true);
    addAndMakeVisible(captionLabel);
    addAndMakeVisible(valueLabel);

    control.setSliderStyle(style);
    control.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    control.setLookAndFeel(&lookAndFeel);
    control.setPopupDisplayEnabled(true, false, this);
    control.setScrollWheelEnabled(false);
    control.setColour(juce::Slider::thumbColourId, Colours::text);
    control.setColour(juce::Slider::trackColourId, Colours::accent);
    control.onValueChange = [this]
    {
        valueLabel.setText(control.getTextFromValue(control.getValue()), juce::dontSendNotification);
    };
    addAndMakeVisible(control);
    bindTo(parameterID);
}

void SliderCell::bindTo(const juce::String& parameterID)
{
    attachment.reset();
    attachment = std::make_unique<APVTS::SliderAttachment>(state, parameterID, control);
    valueLabel.setText(control.getTextFromValue(control.getValue()), juce::dontSendNotification);
}

void SliderCell::setCaption(const juce::String& caption)
{
    captionLabel.setText(caption.toUpperCase(), juce::dontSendNotification);
}

void SliderCell::resized()
{
    auto bounds = getLocalBounds().reduced(4, 2);
    const int labelHeight = compact ? 12 : 14;
    auto labels = bounds.removeFromTop(labelHeight);
    captionLabel.setBounds(labels.removeFromLeft(std::max(24, labels.getWidth() - 48)));
    valueLabel.setBounds(labels);
    control.setBounds(bounds.reduced(0, compact ? 1 : 2));
}

void SliderCell::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(0.5f);
    g.setColour(Colours::cell);
    g.fillRoundedRectangle(bounds, 2.5f);
    g.setColour(Colours::edge.withAlpha(isEnabled() ? 0.72f : 0.35f));
    g.drawRoundedRectangle(bounds, 2.5f, 1.0f);
}

ChoiceCell::ChoiceCell(APVTS& stateToUse, DarkLookAndFeel& lookAndFeelToUse,
                       const juce::String& label, const juce::String& parameterID,
                       const juce::StringArray& choiceNames)
    : state(stateToUse), lookAndFeel(lookAndFeelToUse), choices(choiceNames)
{
    styleSmallLabel(captionLabel, label.toUpperCase());
    addAndMakeVisible(captionLabel);
    control.addItemList(choices, 1);
    control.setLookAndFeel(&lookAndFeel);
    control.setColour(juce::ComboBox::textColourId, Colours::text);
    control.setColour(juce::ComboBox::backgroundColourId, Colours::cell);
    control.setJustificationType(juce::Justification::centredLeft);
    control.setScrollWheelEnabled(false);
    addAndMakeVisible(control);
    bindTo(parameterID);
}

void ChoiceCell::bindTo(const juce::String& parameterID)
{
    attachment.reset();
    attachment = std::make_unique<APVTS::ComboBoxAttachment>(state, parameterID, control);
}

void ChoiceCell::setCaption(const juce::String& caption)
{
    captionLabel.setText(caption.toUpperCase(), juce::dontSendNotification);
}

void ChoiceCell::resized()
{
    auto bounds = getLocalBounds().reduced(3, 2);
    if (! compact)
    {
        captionLabel.setBounds(bounds.removeFromTop(13));
        bounds.removeFromTop(1);
    }
    else
    {
        captionLabel.setBounds(0, 0, 0, 0);
    }
    control.setBounds(bounds);
}

void ChoiceCell::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(0.5f);
    g.setColour(Colours::cell);
    g.fillRoundedRectangle(bounds, 2.5f);
    g.setColour(Colours::edge.withAlpha(isEnabled() ? 0.72f : 0.35f));
    g.drawRoundedRectangle(bounds, 2.5f, 1.0f);
}

ToggleCell::ToggleCell(APVTS& stateToUse, DarkLookAndFeel& lookAndFeelToUse,
                       const juce::String& label, const juce::String& parameterID)
    : state(stateToUse), lookAndFeel(lookAndFeelToUse), caption(label.toUpperCase())
{
    control.setButtonText(caption);
    control.setClickingTogglesState(true);
    control.setLookAndFeel(&lookAndFeel);
    control.setColour(juce::TextButton::buttonColourId, Colours::panelRaised);
    control.setColour(juce::TextButton::buttonOnColourId, Colours::accent);
    addAndMakeVisible(control);
    bindTo(parameterID);
}

void ToggleCell::bindTo(const juce::String& parameterID)
{
    attachment.reset();
    attachment = std::make_unique<APVTS::ButtonAttachment>(state, parameterID, control);
}

void ToggleCell::resized()
{
    control.setBounds(getLocalBounds().reduced(3, 2));
}

void ToggleCell::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(0.5f);
    g.setColour(Colours::cell);
    g.fillRoundedRectangle(bounds, 2.5f);
    g.setColour(Colours::edge.withAlpha(isEnabled() ? 0.72f : 0.35f));
    g.drawRoundedRectangle(bounds, 2.5f, 1.0f);
}

FxRow::FxRow(APVTS& state, DarkLookAndFeel& lookAndFeel, int oneBasedSlot)
    : slot(oneBasedSlot),
      type(state, lookAndFeel, "TYPE", ParamIDs::fx(oneBasedSlot, "type"), getFxTypeNames()),
      position(state, lookAndFeel, "AFTER", ParamIDs::fx(oneBasedSlot, "position"),
               { "Comb 1", "Comb 2", "Comb 3", "Comb 4", "Comb 5", "Comb 6", "Comb 7", "Comb 8" }),
      mix(state, lookAndFeel, "MIX", ParamIDs::fx(oneBasedSlot, "mix")),
      size(state, lookAndFeel, "SIZE", ParamIDs::fx(oneBasedSlot, "size")),
      pitch(state, lookAndFeel, "PITCH", ParamIDs::fx(oneBasedSlot, "pitch")),
      blur(state, lookAndFeel, "BLUR", ParamIDs::fx(oneBasedSlot, "blur")),
      freeze(state, lookAndFeel, "FREEZE", ParamIDs::fx(oneBasedSlot, "freeze"))
{
    styleSmallLabel(title, "INSERT " + twoDigit(slot));
    addAndMakeVisible(title);
    for (auto* component : { static_cast<juce::Component*>(&type), static_cast<juce::Component*>(&position),
                             static_cast<juce::Component*>(&mix), static_cast<juce::Component*>(&size),
                             static_cast<juce::Component*>(&pitch), static_cast<juce::Component*>(&blur),
                             static_cast<juce::Component*>(&freeze) })
        addAndMakeVisible(*component);
    type.setCompact(true);
    position.setCompact(true);
    mix.setCompact(true);
    size.setCompact(true);
    pitch.setCompact(true);
    blur.setCompact(true);
}

void FxRow::resized()
{
    auto area = getLocalBounds().reduced(4, 3);
    auto top = area.removeFromTop(26);
    title.setBounds(top.removeFromLeft(62));
    freeze.setBounds(top.removeFromRight(62));
    position.setBounds(top.removeFromRight(std::max(82, top.getWidth() / 3)).reduced(2, 1));
    type.setBounds(top.reduced(2, 1));

    area.removeFromTop(2);
    const int cellWidth = area.getWidth() / 4;
    mix.setBounds(area.removeFromLeft(cellWidth).reduced(2, 1));
    size.setBounds(area.removeFromLeft(cellWidth).reduced(2, 1));
    pitch.setBounds(area.removeFromLeft(cellWidth).reduced(2, 1));
    blur.setBounds(area.reduced(2, 1));
}

void FxRow::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(0.5f);
    g.setColour(Colours::panelRaised);
    g.fillRoundedRectangle(bounds, 3.0f);
    g.setColour(Colours::edge);
    g.drawRoundedRectangle(bounds, 3.0f, 1.0f);
}

class ModulationPanel::LfoRow final : public juce::Component
{
public:
    LfoRow(APVTS& state, DarkLookAndFeel& lookAndFeel, int index)
        : shape(state, lookAndFeel, "WAVE", ParamIDs::lfo(index, "shape"), getLfoShapeNames()),
          rate(state, lookAndFeel, "RATE", ParamIDs::lfo(index, "rate")),
          depth(state, lookAndFeel, "DEPTH", ParamIDs::lfo(index, "depth")),
          sync(state, lookAndFeel, "SYNC", ParamIDs::lfo(index, "sync")),
          division(state, lookAndFeel, "DIV", ParamIDs::lfo(index, "division"), getSyncDivisionNames())
    {
        styleSmallLabel(number, "LFO " + twoDigit(index));
        addAndMakeVisible(number);
        addAndMakeVisible(shape);
        addAndMakeVisible(rate);
        addAndMakeVisible(depth);
        addAndMakeVisible(sync);
        addAndMakeVisible(division);
        shape.setCompact(true);
        rate.setCompact(true);
        depth.setCompact(true);
        division.setCompact(true);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(2, 1);
        number.setBounds(area.removeFromLeft(34));
        const int divisionW = 58;
        const int syncW = 42;
        const int dynamicControlWidth = std::max(1, area.getWidth() - divisionW - syncW);
        const int rateW = dynamicControlWidth / 3;
        const int depthW = dynamicControlWidth / 3;
        division.setBounds(area.removeFromRight(divisionW).reduced(1, 1));
        sync.setBounds(area.removeFromRight(syncW).reduced(1, 1));
        depth.setBounds(area.removeFromRight(depthW).reduced(1, 1));
        rate.setBounds(area.removeFromRight(rateW).reduced(1, 1));
        shape.setBounds(area.reduced(1, 1));
    }

private:
    juce::Label number;
    ChoiceCell shape;
    SliderCell rate;
    SliderCell depth;
    ToggleCell sync;
    ChoiceCell division;
};

class ModulationPanel::RouteRow final : public juce::Component
{
public:
    RouteRow(APVTS& state, DarkLookAndFeel& lookAndFeel, int routeIndex)
        : source(state, lookAndFeel, "SRC", ParamIDs::route(routeIndex, "source"), getModSourceNames()),
          destination(state, lookAndFeel, "DEST", ParamIDs::route(routeIndex, "destination"), getModDestinationNames()),
          amount(state, lookAndFeel, "AMT", ParamIDs::route(routeIndex, "amount"))
    {
        styleSmallLabel(number, "01");
        addAndMakeVisible(number);
        addAndMakeVisible(source);
        addAndMakeVisible(destination);
        addAndMakeVisible(amount);
        source.setCompact(true);
        destination.setCompact(true);
        amount.setCompact(true);
    }

    void bindTo(int routeIndex)
    {
        number.setText(twoDigit(routeIndex), juce::dontSendNotification);
        source.bindTo(ParamIDs::route(routeIndex, "source"));
        destination.bindTo(ParamIDs::route(routeIndex, "destination"));
        amount.bindTo(ParamIDs::route(routeIndex, "amount"));
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(2, 1);
        number.setBounds(area.removeFromLeft(26));
        amount.setBounds(area.removeFromRight(76).reduced(1, 1));
        source.setBounds(area.removeFromLeft(std::max(78, area.getWidth() / 3)).reduced(1, 1));
        destination.setBounds(area.reduced(1, 1));
    }

private:
    juce::Label number;
    ChoiceCell source;
    ChoiceCell destination;
    SliderCell amount;
};

ModulationPanel::~ModulationPanel() = default;

ModulationPanel::ModulationPanel(APVTS& stateToUse, DarkLookAndFeel& lookAndFeelToUse)
    : state(stateToUse), lookAndFeel(lookAndFeelToUse)
{
    lfoTab.setLookAndFeel(&lookAndFeel);
    routeTab.setLookAndFeel(&lookAndFeel);
    previousPage.setLookAndFeel(&lookAndFeel);
    nextPage.setLookAndFeel(&lookAndFeel);
    for (auto* button : { &lfoTab, &routeTab, &previousPage, &nextPage })
        addAndMakeVisible(*button);
    styleSmallLabel(pageLabel, "ROUTES 01-08", true);
    addAndMakeVisible(pageLabel);

    lfoTab.onClick = [this] { showingRoutes = false; updateVisibility(); };
    routeTab.onClick = [this] { showingRoutes = true; updateVisibility(); };
    previousPage.onClick = [this] { setRoutePage(0); };
    nextPage.onClick = [this] { setRoutePage(1); };

    for (int i = 0; i < kNumLfos; ++i)
    {
        lfoRows[static_cast<std::size_t>(i)] = std::make_unique<LfoRow>(state, lookAndFeel, i + 1);
        addAndMakeVisible(*lfoRows[static_cast<std::size_t>(i)]);
    }
    for (int i = 0; i < 8; ++i)
    {
        routeRows[static_cast<std::size_t>(i)] = std::make_unique<RouteRow>(state, lookAndFeel, i + 1);
        addAndMakeVisible(*routeRows[static_cast<std::size_t>(i)]);
    }
    setRoutePage(0);
    updateVisibility();
}

void ModulationPanel::setRoutePage(int page)
{
    routePage = std::clamp(page, 0, 1);
    for (int i = 0; i < 8; ++i)
        routeRows[static_cast<std::size_t>(i)]->bindTo(routePage * 8 + i + 1);
    pageLabel.setText(routePage == 0 ? "ROUTES 01-08" : "ROUTES 09-16", juce::dontSendNotification);
    previousPage.setEnabled(routePage > 0);
    nextPage.setEnabled(routePage < 1);
}

void ModulationPanel::updateVisibility()
{
    lfoTab.setToggleState(! showingRoutes, juce::dontSendNotification);
    routeTab.setToggleState(showingRoutes, juce::dontSendNotification);
    previousPage.setVisible(showingRoutes);
    nextPage.setVisible(showingRoutes);
    pageLabel.setVisible(showingRoutes);
    for (auto& row : lfoRows) row->setVisible(! showingRoutes);
    for (auto& row : routeRows) row->setVisible(showingRoutes);
    resized();
    repaint();
}

void ModulationPanel::resized()
{
    auto area = getLocalBounds().reduced(7, 6);
    auto tabs = area.removeFromTop(27);
    lfoTab.setBounds(tabs.removeFromLeft(56).reduced(1, 1));
    routeTab.setBounds(tabs.removeFromLeft(76).reduced(1, 1));
    if (showingRoutes)
    {
        pageLabel.setBounds(tabs.removeFromLeft(std::max(88, tabs.getWidth() - 58)));
        nextPage.setBounds(tabs.removeFromRight(28).reduced(1, 1));
        previousPage.setBounds(tabs.removeFromRight(28).reduced(1, 1));
    }
    area.removeFromTop(4);
    if (showingRoutes)
    {
        const int rowHeight = std::max(26, area.getHeight() / 8);
        for (int i = 0; i < 8; ++i)
            routeRows[static_cast<std::size_t>(i)]->setBounds(area.removeFromTop(rowHeight).reduced(0, 1));
    }
    else
    {
        const int rowHeight = std::max(46, area.getHeight() / 4);
        for (auto& row : lfoRows)
            row->setBounds(area.removeFromTop(rowHeight).reduced(0, 2));
    }
}

void ModulationPanel::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(0.5f);
    g.setColour(Colours::panel);
    g.fillRoundedRectangle(bounds, 4.0f);
    g.setColour(Colours::edge);
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
    g.setColour(Colours::text);
    g.setFont(juce::Font(10.0f, juce::Font::bold));
    g.drawText("MODULATORS", 9, 2, 150, 15, juce::Justification::centredLeft);
}

class SequencerPanel::StepCell final : public juce::Component
{
public:
    StepCell(APVTS& stateToUse, DarkLookAndFeel& lookAndFeel, int stepIndex)
        : state(stateToUse), step(stepIndex)
    {
        slider.setSliderStyle(juce::Slider::LinearVertical);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.setLookAndFeel(&lookAndFeel);
        slider.setScrollWheelEnabled(false);
        slider.setPopupDisplayEnabled(true, false, this);
        slider.setColour(juce::Slider::thumbColourId, Colours::text);
        addAndMakeVisible(slider);
        styleSmallLabel(number, juce::String(stepIndex), false);
        number.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(number);
        bindTo(ParamIDs::sequence(1, "step" + juce::String(stepIndex)));
    }

    void bindTo(const juce::String& parameterID)
    {
        attachment.reset();
        attachment = std::make_unique<APVTS::SliderAttachment>(state, parameterID, slider);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(1, 2);
        number.setBounds(area.removeFromBottom(13));
        slider.setBounds(area.reduced(2, 1));
    }

    void paint(juce::Graphics& g) override
    {
        const auto palette = std::array<juce::Colour, 4> { Colours::orange, Colours::blue, Colours::red, Colours::yellow };
        const auto colour = palette[static_cast<std::size_t>((step - 1) % static_cast<int>(palette.size()))];
        auto bounds = getLocalBounds().toFloat().reduced(0.5f);
        g.setColour(Colours::cell);
        g.fillRect(bounds);
        g.setColour(Colours::edge.withAlpha(0.8f));
        g.drawRect(bounds, 1.0f);
        g.setColour(colour.withAlpha(0.10f));
        g.fillRect(bounds.removeFromBottom(4.0f));
    }

private:
    APVTS& state;
    int step;
    juce::Slider slider;
    juce::Label number;
    std::unique_ptr<APVTS::SliderAttachment> attachment;
};

SequencerPanel::~SequencerPanel() = default;

SequencerPanel::SequencerPanel(APVTS& stateToUse, DarkLookAndFeel& lookAndFeelToUse)
    : state(stateToUse), lookAndFeel(lookAndFeelToUse),
      length(stateToUse, lookAndFeelToUse, "LENGTH", ParamIDs::sequence(1, "length")),
      rate(stateToUse, lookAndFeelToUse, "STEP RATE", ParamIDs::sequence(1, "rate")),
      slew(stateToUse, lookAndFeelToUse, "SLEW", ParamIDs::sequence(1, "slew")),
      mode(stateToUse, lookAndFeelToUse, "MODE", ParamIDs::sequence(1, "mode"), getSequenceModeNames()),
      division(stateToUse, lookAndFeelToUse, "DIV", ParamIDs::sequence(1, "division"), getSyncDivisionNames()),
      sync(stateToUse, lookAndFeelToUse, "SYNC", ParamIDs::sequence(1, "sync"))
{
    styleSmallLabel(title, "PATTERN / DSP MOD SEQUENCER");
    addAndMakeVisible(title);
    for (int i = 0; i < kNumSequences; ++i)
    {
        auto& button = sequenceButtons[static_cast<std::size_t>(i)];
        button.setButtonText("SEQ " + juce::String(i + 1));
        button.setClickingTogglesState(false);
        button.setLookAndFeel(&lookAndFeel);
        button.onClick = [this, i] { selectSequence(i); };
        addAndMakeVisible(button);
    }
    for (int i = 1; i <= kSequenceSteps; ++i)
    {
        steps[static_cast<std::size_t>(i - 1)] = std::make_unique<StepCell>(state, lookAndFeel, i);
        addAndMakeVisible(*steps[static_cast<std::size_t>(i - 1)]);
    }
    addAndMakeVisible(length);
    addAndMakeVisible(rate);
    addAndMakeVisible(slew);
    addAndMakeVisible(mode);
    addAndMakeVisible(division);
    addAndMakeVisible(sync);
    length.setCompact(true);
    rate.setCompact(true);
    slew.setCompact(true);
    mode.setCompact(true);
    division.setCompact(true);
    selectSequence(0);
}

void SequencerPanel::selectSequence(int index)
{
    selectedSequence = std::clamp(index, 0, kNumSequences - 1);
    const int oneBased = selectedSequence + 1;
    length.bindTo(ParamIDs::sequence(oneBased, "length"));
    rate.bindTo(ParamIDs::sequence(oneBased, "rate"));
    slew.bindTo(ParamIDs::sequence(oneBased, "slew"));
    mode.bindTo(ParamIDs::sequence(oneBased, "mode"));
    division.bindTo(ParamIDs::sequence(oneBased, "division"));
    sync.bindTo(ParamIDs::sequence(oneBased, "sync"));
    for (int i = 1; i <= kSequenceSteps; ++i)
        steps[static_cast<std::size_t>(i - 1)]->bindTo(ParamIDs::sequence(oneBased, "step" + juce::String(i)));

    for (int i = 0; i < kNumSequences; ++i)
        sequenceButtons[static_cast<std::size_t>(i)].setToggleState(i == selectedSequence, juce::dontSendNotification);
    title.setText("PATTERN / SEQ " + juce::String(oneBased) + " / DSP MOD", juce::dontSendNotification);
}

void SequencerPanel::resized()
{
    auto area = getLocalBounds().reduced(8, 5);
    auto header = area.removeFromTop(37);
    title.setBounds(header.removeFromLeft(185));
    for (auto& button : sequenceButtons)
        button.setBounds(header.removeFromLeft(49).reduced(1, 2));
    header.removeFromLeft(5);
    division.setBounds(header.removeFromRight(92).reduced(1, 1));
    sync.setBounds(header.removeFromRight(58).reduced(1, 1));
    slew.setBounds(header.removeFromRight(77).reduced(1, 1));
    mode.setBounds(header.removeFromRight(95).reduced(1, 1));
    rate.setBounds(header.removeFromRight(92).reduced(1, 1));
    length.setBounds(header.removeFromRight(83).reduced(1, 1));

    area.removeFromTop(2);
    const int cellWidth = std::max(20, area.getWidth() / kSequenceSteps);
    for (int i = 0; i < kSequenceSteps; ++i)
        steps[static_cast<std::size_t>(i)]->setBounds(area.removeFromLeft(cellWidth).reduced(1, 1));
}

void SequencerPanel::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(0.5f);
    g.setColour(Colours::panel);
    g.fillRoundedRectangle(bounds, 4.0f);
    g.setColour(Colours::edge);
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
    g.setColour(Colours::accent.withAlpha(0.35f));
    g.fillRoundedRectangle(8.0f, 3.0f, 62.0f, 2.0f, 1.0f);
}

} // namespace combscanner::ui

using namespace combscanner::ui;

CombScannerAudioProcessorEditor::CombScannerAudioProcessorEditor(CombScannerAudioProcessor& processorToUse)
    : juce::AudioProcessorEditor(processorToUse),
      processor(processorToUse),
      modeCell(processor.getValueTreeState(), lookAndFeel, "DSP MODE", "global_mode", { "Classic Fav", "More Control" }),
      modelCell(processor.getValueTreeState(), lookAndFeel, "MODEL", "global_model", combscanner::getModelNames()),
      bypassCell(processor.getValueTreeState(), lookAndFeel, "BYPASS", "global_bypass"),
      masterMix(processor.getValueTreeState(), lookAndFeel, "MASTER MIX", "global_mix"),
      outputLevel(processor.getValueTreeState(), lookAndFeel, "OUTPUT", "global_output"),
      combEnabled(processor.getValueTreeState(), lookAndFeel, "ON", combscanner::ParamIDs::comb(1, "on")),
      scanShapeCell(processor.getValueTreeState(), lookAndFeel, "SHAPE", "global_scan_shape",
                    { "Linear", "Cosine", "Gaussian", "Stepped" }),
      modulationPanel(processor.getValueTreeState(), lookAndFeel),
      sequencerPanel(processor.getValueTreeState(), lookAndFeel)
{
    setLookAndFeel(&lookAndFeel);
    setOpaque(true);
    setResizable(true, true);
    setResizeLimits(1180, 760, 2200, 1400);
    setSize(1280, 800);

    title.setText("COMB SCANNER", juce::dontSendNotification);
    title.setFont(juce::Font(18.0f, juce::Font::bold));
    title.setColour(juce::Label::textColourId, Colours::text);
    title.setJustificationType(juce::Justification::centredLeft);
    subtitle.setText("VST3  /  MULTI-COMB RESONATOR", juce::dontSendNotification);
    subtitle.setFont(juce::Font(8.5f, juce::Font::bold));
    subtitle.setColour(juce::Label::textColourId, Colours::muted);
    subtitle.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(title);
    addAndMakeVisible(subtitle);

    presetLabel.setText("PRESET", juce::dontSendNotification);
    presetLabel.setFont(juce::Font(8.5f, juce::Font::bold));
    presetLabel.setColour(juce::Label::textColourId, Colours::muted);
    presetBox.setLookAndFeel(&lookAndFeel);
    presetBox.addItemList(combscanner::getModelNames(), 1);
    presetBox.addItem("11 More Control - Manual Ratios", 11);
    presetBox.setColour(juce::ComboBox::textColourId, Colours::text);
    presetBox.setColour(juce::ComboBox::backgroundColourId, Colours::cell);
    presetBox.setScrollWheelEnabled(false);
    presetBox.onChange = [this]
    {
        const int selected = presetBox.getSelectedId() - 1;
        if (selected >= 0 && selected < processor.getNumPrograms())
        {
            processor.loadFactoryPreset(selected);
            updateModeState();
        }
    };
    addAndMakeVisible(presetLabel);
    addAndMakeVisible(presetBox);

    for (auto* component : { static_cast<juce::Component*>(&modeCell), static_cast<juce::Component*>(&modelCell),
                             static_cast<juce::Component*>(&bypassCell), static_cast<juce::Component*>(&masterMix),
                             static_cast<juce::Component*>(&outputLevel), static_cast<juce::Component*>(&combEnabled),
                             static_cast<juce::Component*>(&scanShapeCell), static_cast<juce::Component*>(&modulationPanel),
                             static_cast<juce::Component*>(&sequencerPanel) })
        addAndMakeVisible(*component);

    modeCell.comboBox().onChange = [this] { updateModeState(); };
    modelCell.comboBox().onChange = [this] { updateModeState(); };

    styleSmallLabel(combSectionTitle, "COMB 01 / INDIVIDUAL VOICE");
    combSectionTitle.setColour(juce::Label::textColourId, Colours::text);
    combSectionTitle.setFont(juce::Font(10.0f, juce::Font::bold));
    addAndMakeVisible(combSectionTitle);

    for (int i = 0; i < combscanner::kNumCombs; ++i)
    {
        auto& button = combButtons[static_cast<std::size_t>(i)];
        button.setButtonText(twoDigit(i + 1));
        button.setClickingTogglesState(false);
        button.setLookAndFeel(&lookAndFeel);
        button.onClick = [this, i] { selectComb(i); };
        addAndMakeVisible(button);
    }

    const std::array<std::pair<const char*, const char*>, 13> combSpecs {{
        { "delay1", "D1 / MS" }, { "delay2", "D2 / MS" }, { "ratio", "RATIO 1" },
        { "ratio2", "RATIO 2" }, { "feedback", "FEEDBACK" }, { "damp", "DAMP" },
        { "phase", "PHASE" }, { "diffusion", "DIFFUSION" }, { "crossmix", "CROSS MIX" },
        { "drive", "DRIVE" }, { "level", "LEVEL / DB" }, { "pan", "PAN" }, { "scanweight", "SCAN WEIGHT" }
    }};
    for (const auto& spec : combSpecs)
    {
        auto cell = std::make_unique<combscanner::ui::SliderCell>(processor.getValueTreeState(), lookAndFeel,
                                                                  spec.second,
                                                                  combscanner::ParamIDs::comb(1, spec.first));
        cell->setComponentID(spec.first);
        combControlKeys.emplace_back(spec.first);
        addAndMakeVisible(*cell);
        combControls.push_back(std::move(cell));
    }

    const std::array<std::pair<const char*, const char*>, 6> scanSpecs {{
        { "global_scan", "SCAN" }, { "global_scan_width", "WIDTH" }, { "global_scan_rate", "RATE / HZ" },
        { "global_diffusion", "DIFFUSION" }, { "global_crossmix", "CROSS MIX" }, { "global_motion", "MOTION" }
    }};
    for (const auto& spec : scanSpecs)
    {
        auto cell = std::make_unique<combscanner::ui::SliderCell>(processor.getValueTreeState(), lookAndFeel,
                                                                  spec.second, spec.first);
        cell->setCompact(true);
        scannerControlKeys.emplace_back(spec.first);
        addAndMakeVisible(*cell);
        scannerControls.push_back(std::move(cell));
    }

    const std::array<std::pair<const char*, const char*>, 5> classicSpecs {{
        { "global_delay1", "BASE D1" }, { "global_delay2", "BASE D2" },
        { "global_feedback", "GLOBAL FB" }, { "global_damp", "GLOBAL DAMP" },
        { "global_phase", "GLOBAL PHASE" }
    }};
    for (const auto& spec : classicSpecs)
    {
        auto cell = std::make_unique<combscanner::ui::SliderCell>(processor.getValueTreeState(), lookAndFeel,
                                                                  spec.second, spec.first);
        cell->setCompact(true);
        classicControlKeys.emplace_back(spec.first);
        addAndMakeVisible(*cell);
        classicControls.push_back(std::move(cell));
    }

    for (int i = 0; i < combscanner::kNumFxSlots; ++i)
    {
        fxRows[static_cast<std::size_t>(i)] = std::make_unique<combscanner::ui::FxRow>(processor.getValueTreeState(),
                                                                                      lookAndFeel, i + 1);
        addAndMakeVisible(*fxRows[static_cast<std::size_t>(i)]);
    }

    selectComb(0);
    updateModeState();
    presetBox.setSelectedId(processor.getCurrentProgram() + 1, juce::dontSendNotification);
}

CombScannerAudioProcessorEditor::~CombScannerAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void CombScannerAudioProcessorEditor::layoutControlGrid(
    std::vector<std::unique_ptr<combscanner::ui::SliderCell>>& controls,
    juce::Rectangle<int> area, int columns)
{
    if (controls.empty() || columns <= 0)
        return;
    const int rows = (static_cast<int>(controls.size()) + columns - 1) / columns;
    const int cellWidth = std::max(1, area.getWidth() / columns);
    const int cellHeight = std::max(1, area.getHeight() / rows);
    for (int i = 0; i < static_cast<int>(controls.size()); ++i)
    {
        const int row = i / columns;
        const int col = i % columns;
        auto cell = juce::Rectangle<int>(area.getX() + col * cellWidth, area.getY() + row * cellHeight,
                                         col == columns - 1 ? area.getRight() - (area.getX() + col * cellWidth) : cellWidth,
                                         row == rows - 1 ? area.getBottom() - (area.getY() + row * cellHeight) : cellHeight);
        controls[static_cast<std::size_t>(i)]->setBounds(cell.reduced(2, 2));
    }
}

void CombScannerAudioProcessorEditor::selectComb(int index)
{
    selectedComb = std::clamp(index, 0, combscanner::kNumCombs - 1);
    const int oneBased = selectedComb + 1;
    combSectionTitle.setText("COMB " + twoDigit(oneBased) + " / INDIVIDUAL VOICE", juce::dontSendNotification);
    combEnabled.bindTo(combscanner::ParamIDs::comb(oneBased, "on"));
    for (std::size_t i = 0; i < combControls.size(); ++i)
        combControls[i]->bindTo(combscanner::ParamIDs::comb(oneBased, combControlKeys[i]));
    for (int i = 0; i < combscanner::kNumCombs; ++i)
        combButtons[static_cast<std::size_t>(i)].setToggleState(i == selectedComb, juce::dontSendNotification);
    updateModeState();
}

void CombScannerAudioProcessorEditor::updateModeState()
{
    const bool moreControl = modeCell.comboBox().getSelectedItemIndex() == 1;
    for (std::size_t i = 0; i < combControls.size(); ++i)
    {
        const auto key = combControlKeys[i];
        const bool manualOnly = key == "ratio" || key == "ratio2" || key == "delay1" || key == "delay2"
                             || key == "feedback" || key == "damp" || key == "phase" || key == "diffusion"
                             || key == "crossmix" || key == "drive";
        combControls[i]->setEnabled(! manualOnly || moreControl);
    }
    for (auto& cell : classicControls)
        cell->setEnabled(! moreControl);

    if (processor.getCurrentProgram() >= 0)
        presetBox.setSelectedId(processor.getCurrentProgram() + 1, juce::dontSendNotification);
    repaint();
}

void CombScannerAudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::ColourGradient background(juce::Colour(0xff101016), 0.0f, 0.0f,
                                    juce::Colour(0xff08080c), 0.0f, static_cast<float>(getHeight()), false);
    g.setGradientFill(background);
    g.fillAll();

    auto header = getLocalBounds().reduced(10).removeFromTop(56).toFloat();
    g.setColour(juce::Colour(0xff121219));
    g.fillRoundedRectangle(header, 4.0f);
    g.setColour(Colours::edge);
    g.drawRoundedRectangle(header, 4.0f, 1.0f);

    const auto drawPanel = [&g](juce::Rectangle<int> r, const juce::String& label)
    {
        auto bounds = r.toFloat().reduced(0.5f);
        g.setColour(Colours::panel);
        g.fillRoundedRectangle(bounds, 4.0f);
        g.setColour(Colours::edge);
        g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
        g.setColour(Colours::accent.withAlpha(0.34f));
        g.fillRoundedRectangle(bounds.getX() + 8.0f, bounds.getY() + 3.0f, 58.0f, 2.0f, 1.0f);
        g.setColour(Colours::muted);
        g.setFont(juce::Font(8.0f, juce::Font::bold));
        g.drawText(label, r.getX() + 9, r.getY() + 2, r.getWidth() - 18, 15,
                   juce::Justification::centredRight);
    };
    drawPanel(combPanelBounds, "VOICE / SCANNER PARAMETERS");
    drawPanel(fxPanelBounds, "INSERTS BETWEEN COMBS");
    drawPanel(modPanelBounds, "LFO + ROUTING MATRIX");
}

void CombScannerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    auto header = area.removeFromTop(56);
    area.removeFromTop(8);
    sequencerBounds = area.removeFromBottom(198);
    area.removeFromBottom(8);

    const int available = std::max(0, area.getWidth());
    const int gap = std::min(8, available / 6);
    int leftWidth = static_cast<int>(static_cast<float>(available) * 0.365f);
    int fxWidth = static_cast<int>(static_cast<float>(available) * 0.345f);

    // A plug-in host or bridge may ignore the advertised minimum size. Keep all
    // std::clamp bounds ordered so a compact embedded editor cannot trigger UB.
    const int sideMinimum = std::min(300, std::max(0, (available - 2 * gap) / 3));
    const int leftMinimum = std::min(340, std::max(0, available - 2 * gap - 2 * sideMinimum));
    const int leftMaximum = std::max(leftMinimum, available - 2 * gap - 2 * sideMinimum);
    leftWidth = std::clamp(leftWidth, leftMinimum, leftMaximum);

    const int remainingAfterLeft = std::max(0, available - leftWidth - 2 * gap);
    const int fxMinimum = std::min(300, remainingAfterLeft / 2);
    const int fxMaximum = std::max(fxMinimum, remainingAfterLeft - fxMinimum);
    fxWidth = std::clamp(fxWidth, fxMinimum, fxMaximum);
    combPanelBounds = area.removeFromLeft(leftWidth);
    area.removeFromLeft(gap);
    fxPanelBounds = area.removeFromLeft(fxWidth);
    area.removeFromLeft(gap);
    modPanelBounds = area;

    title.setBounds(header.removeFromLeft(215).reduced(7, 5).withHeight(24));
    subtitle.setBounds(18, 35, 215, 14);
    auto presetArea = header.removeFromLeft(220).reduced(3, 5);
    presetLabel.setBounds(presetArea.removeFromTop(13));
    presetBox.setBounds(presetArea);
    modeCell.setBounds(header.removeFromLeft(154).reduced(3, 4));
    modelCell.setBounds(header.removeFromLeft(232).reduced(3, 4));
    bypassCell.setBounds(header.removeFromLeft(82).reduced(3, 5));
    masterMix.setBounds(header.removeFromLeft(132).reduced(3, 4));
    outputLevel.setBounds(header.reduced(3, 4));

    auto combInner = combPanelBounds.reduced(7, 8);
    auto combTitle = combInner.removeFromTop(20);
    combSectionTitle.setBounds(combTitle.removeFromLeft(std::max(150, combTitle.getWidth() - 70)));
    combEnabled.setBounds(combTitle.removeFromRight(62).reduced(1, 0));
    auto selectors = combInner.removeFromTop(28);
    const int selectorWidth = std::max(24, selectors.getWidth() / combscanner::kNumCombs);
    for (int i = 0; i < combscanner::kNumCombs; ++i)
        combButtons[static_cast<std::size_t>(i)].setBounds(selectors.removeFromLeft(selectorWidth).reduced(1, 2));
    combInner.removeFromTop(3);

    const int controlGridHeight = std::min(250, std::max(190, combInner.getHeight() * 47 / 100));
    layoutControlGrid(combControls, combInner.removeFromTop(controlGridHeight), 3);
    combInner.removeFromTop(4);
    auto scannerArea = combInner.removeFromTop(std::min(100, std::max(76, combInner.getHeight() / 2)));
    auto shapeArea = scannerArea.removeFromRight(78);
    scanShapeCell.setBounds(shapeArea.reduced(2, 2));
    layoutControlGrid(scannerControls, scannerArea, 3);
    combInner.removeFromTop(4);
    layoutControlGrid(classicControls, combInner, 3);

    auto fxInner = fxPanelBounds.reduced(7, 21);
    const int fxRowHeight = std::max(72, fxInner.getHeight() / combscanner::kNumFxSlots);
    for (auto& row : fxRows)
        row->setBounds(fxInner.removeFromTop(fxRowHeight).reduced(1, 2));

    modulationPanel.setBounds(modPanelBounds.reduced(2, 18));
    sequencerPanel.setBounds(sequencerBounds);
    repaint();
}
