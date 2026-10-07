#include "PluginParameters.h"

#include "DSP/CombScannerEngine.h"

namespace combscanner
{
namespace
{
using Layout = juce::AudioProcessorValueTreeState::ParameterLayout;

void addFloat(Layout& layout, const juce::String& id, const juce::String& name,
              float minimum, float maximum, float step, float defaultValue,
              float skewCentre = 0.0f)
{
    juce::NormalisableRange<float> range(minimum, maximum, step);
    if (skewCentre > minimum && skewCentre < maximum)
        range.setSkewForCentre(skewCentre);
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { id, 1 }, name,
                                                           range, defaultValue));
}

void addChoice(Layout& layout, const juce::String& id, const juce::String& name,
               const juce::StringArray& choices, int defaultIndex)
{
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { id, 1 }, name,
                                                            choices, defaultIndex));
}

void addBool(Layout& layout, const juce::String& id, const juce::String& name, bool defaultValue)
{
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID { id, 1 }, name,
                                                          defaultValue));
}

juce::StringArray makeSyncDivisions()
{
    return { "1/16", "1/8", "1/4", "1/2", "1 bar", "2 bars", "4 bars" };
}

} // namespace

juce::StringArray getModelNames()
{
    return { "01 Original Hypothesis", "02 Tight Crossfade", "03 Long Resonator",
             "04 Scrub Stretch", "05 Phase Cloud", "06 Ping Pong", "07 Dark Bloom",
             "08 Bright Teeth", "09 Unstable Edge", "10 Balanced Matrix" };
}

juce::StringArray getFxTypeNames()
{
    return { "Off", "Filter", "Granular", "FFT Stretch", "Limiter", "Saturator" };
}

juce::StringArray getLfoShapeNames()
{
    return { "Sine", "Triangle", "Saw", "Square", "Sample & Hold" };
}

juce::StringArray getSequenceModeNames()
{
    return { "Forward", "Reverse", "Ping-Pong", "Random" };
}

juce::StringArray getModSourceNames()
{
    return { "Off", "LFO 1", "LFO 2", "LFO 3", "LFO 4", "SEQ 1", "SEQ 2", "SEQ 3", "SEQ 4",
             "Envelope", "Mod Wheel", "Velocity" };
}

juce::StringArray getModDestinationNames()
{
    juce::StringArray names { "Off", "Scanner: Position", "Scanner: Width", "Global: Feedback",
                              "Global: Delay 1", "Global: Delay 2", "Global: Damp", "Global: Phase",
                              "Global: Diffusion", "Global: Cross Mix", "Global: Motion",
                              "Scanner: Rate", "Master: Output" };
    for (int i = 1; i <= kNumCombs; ++i) names.add("Comb " + juce::String(i) + ": Ratio");
    for (int i = 1; i <= kNumCombs; ++i) names.add("Comb " + juce::String(i) + ": Feedback");
    for (int i = 1; i <= kNumCombs; ++i) names.add("Comb " + juce::String(i) + ": Delay 1");
    for (int i = 1; i <= kNumCombs; ++i) names.add("Comb " + juce::String(i) + ": Delay 2");
    for (int i = 1; i <= kNumCombs; ++i) names.add("Comb " + juce::String(i) + ": Level");
    return names;
}

juce::StringArray getSyncDivisionNames()
{
    return makeSyncDivisions();
}

Layout createParameterLayout()
{
    Layout layout;
    const auto models = getModelNames();
    const auto fxTypes = getFxTypeNames();
    const auto lfoShapes = getLfoShapeNames();
    const auto seqModes = getSequenceModeNames();
    const auto syncDivisions = makeSyncDivisions();
    const auto modSources = getModSourceNames();
    const auto modDestinations = getModDestinationNames();

    addChoice(layout, "global_mode", "DSP Mode", { "Classic Fav", "More Control" }, 0);
    addChoice(layout, "global_model", "DSP Model", models, 0);
    addBool(layout, "global_bypass", "Bypass", false);

    addFloat(layout, "global_feedback", "Classic Feedback", 0.0f, 1.0f, 0.001f, 0.80f);
    addFloat(layout, "global_damp", "Classic Damp", 0.0f, 1.0f, 0.001f, 0.90f);
    addFloat(layout, "global_phase", "Classic Phase", 0.0f, 1.0f, 0.001f, 0.75f);
    addFloat(layout, "global_delay1", "Classic Delay 1", 0.0f, 2000.0f, 0.01f, 115.0f, 180.0f);
    addFloat(layout, "global_delay2", "Classic Delay 2", 0.0f, 2000.0f, 0.01f, 500.0f, 500.0f);

    addFloat(layout, "global_scan", "Scan", 0.0f, 1.0f, 0.001f, 0.0f);
    addFloat(layout, "global_scan_width", "Scan Width", 0.0f, 1.0f, 0.001f, 0.22f);
    addFloat(layout, "global_scan_rate", "Scan Rate", 0.0f, 12.0f, 0.001f, 0.0f);
    addChoice(layout, "global_scan_shape", "Scan Shape", { "Linear", "Cosine", "Gaussian", "Stepped" }, 0);
    addFloat(layout, "global_diffusion", "Diffusion", 0.0f, 1.0f, 0.001f, 0.55f);
    addFloat(layout, "global_crossmix", "Cross Mix", 0.0f, 1.0f, 0.001f, 0.50f);
    addFloat(layout, "global_motion", "Motion", 0.0f, 1.0f, 0.001f, 0.45f);
    addFloat(layout, "global_mix", "Dry Wet", 0.0f, 1.0f, 0.001f, 0.82f);
    addFloat(layout, "global_output", "Output", -24.0f, 12.0f, 0.01f, 0.0f);
    addFloat(layout, "global_width", "Stereo Width", 0.0f, 1.0f, 0.001f, 1.0f);

    const std::array<float, kNumCombs> defaultRatios { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f };
    for (int i = 1; i <= kNumCombs; ++i)
    {
        const auto prefix = "Comb " + juce::String(i) + " ";
        addBool(layout, ParamIDs::comb(i, "on"), prefix + "On", true);
        addFloat(layout, ParamIDs::comb(i, "ratio"), prefix + "Ratio", 0.125f, 4.0f, 0.001f,
                 defaultRatios[static_cast<std::size_t>(i - 1)], 1.0f);
        addFloat(layout, ParamIDs::comb(i, "ratio2"), prefix + "Ratio 2", 0.125f, 4.0f, 0.001f,
                 defaultRatios[static_cast<std::size_t>(i - 1)], 1.0f);
        addFloat(layout, ParamIDs::comb(i, "delay1"), prefix + "Delay 1", 0.0f, 2000.0f, 0.01f, 115.0f, 180.0f);
        addFloat(layout, ParamIDs::comb(i, "delay2"), prefix + "Delay 2", 0.0f, 2000.0f, 0.01f, 500.0f, 500.0f);
        addFloat(layout, ParamIDs::comb(i, "feedback"), prefix + "Feedback", 0.0f, 0.985f, 0.001f, 0.72f);
        addFloat(layout, ParamIDs::comb(i, "damp"), prefix + "Damp", 0.0f, 1.0f, 0.001f, 0.78f);
        addFloat(layout, ParamIDs::comb(i, "phase"), prefix + "Phase", 0.0f, 1.0f, 0.001f, 0.65f);
        addFloat(layout, ParamIDs::comb(i, "diffusion"), prefix + "Diffusion", 0.0f, 1.0f, 0.001f, 0.50f);
        addFloat(layout, ParamIDs::comb(i, "crossmix"), prefix + "Cross Mix", 0.0f, 1.0f, 0.001f, 0.35f);
        addFloat(layout, ParamIDs::comb(i, "drive"), prefix + "Drive", 0.0f, 18.0f, 0.01f, 0.0f);
        addFloat(layout, ParamIDs::comb(i, "level"), prefix + "Level", -36.0f, 12.0f, 0.01f, 0.0f);
        addFloat(layout, ParamIDs::comb(i, "pan"), prefix + "Pan", -1.0f, 1.0f, 0.001f,
                 -0.8f + 1.6f * static_cast<float>(i - 1) / static_cast<float>(kNumCombs - 1));
        addFloat(layout, ParamIDs::comb(i, "scanweight"), prefix + "Scan Weight", 0.0f, 1.0f, 0.001f, 1.0f);
    }

    const juce::StringArray fxPositions { "After Comb 1", "After Comb 2", "After Comb 3", "After Comb 4",
                                          "After Comb 5", "After Comb 6", "After Comb 7", "After Comb 8" };
    for (int i = 1; i <= kNumFxSlots; ++i)
    {
        const auto prefix = "FX " + juce::String(i) + " ";
        addChoice(layout, ParamIDs::fx(i, "type"), prefix + "Type", fxTypes, 0);
        addChoice(layout, ParamIDs::fx(i, "position"), prefix + "Position", fxPositions, std::min(i - 1, kNumCombs - 1));
        addFloat(layout, ParamIDs::fx(i, "mix"), prefix + "Mix", 0.0f, 1.0f, 0.001f, 0.45f);
        addFloat(layout, ParamIDs::fx(i, "size"), prefix + "Size", 0.0f, 1.0f, 0.001f, 0.50f);
        addFloat(layout, ParamIDs::fx(i, "pitch"), prefix + "Pitch", -1.0f, 1.0f, 0.001f, 0.0f);
        addFloat(layout, ParamIDs::fx(i, "blur"), prefix + "Blur", 0.0f, 1.0f, 0.001f, 0.20f);
        addBool(layout, ParamIDs::fx(i, "freeze"), prefix + "Freeze", false);
    }

    for (int i = 1; i <= kNumLfos; ++i)
    {
        const auto prefix = "LFO " + juce::String(i) + " ";
        addFloat(layout, ParamIDs::lfo(i, "rate"), prefix + "Rate", 0.01f, 20.0f, 0.001f, i == 1 ? 0.35f : 1.0f, 1.0f);
        addFloat(layout, ParamIDs::lfo(i, "depth"), prefix + "Depth", 0.0f, 1.0f, 0.001f, i == 1 ? 0.50f : 0.0f);
        addChoice(layout, ParamIDs::lfo(i, "shape"), prefix + "Shape", lfoShapes, 0);
        addBool(layout, ParamIDs::lfo(i, "sync"), prefix + "Sync", false);
        addChoice(layout, ParamIDs::lfo(i, "division"), prefix + "Division", syncDivisions, 2);
    }

    for (int i = 1; i <= kNumSequences; ++i)
    {
        const auto prefix = "Sequence " + juce::String(i) + " ";
        addFloat(layout, ParamIDs::sequence(i, "rate"), prefix + "Step Rate", 0.01f, 32.0f, 0.001f, 2.0f, 2.0f);
        addFloat(layout, ParamIDs::sequence(i, "slew"), prefix + "Slew", 0.0f, 1.0f, 0.001f, 0.0f);
        layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID { ParamIDs::sequence(i, "length"), 1 },
                                                             prefix + "Length", 1, kSequenceSteps, kSequenceSteps));
        addChoice(layout, ParamIDs::sequence(i, "mode"), prefix + "Mode", seqModes, 0);
        addBool(layout, ParamIDs::sequence(i, "sync"), prefix + "Sync", false);
        addChoice(layout, ParamIDs::sequence(i, "division"), prefix + "Division", syncDivisions, 0);
        for (int step = 1; step <= kSequenceSteps; ++step)
        {
            const auto stepId = ParamIDs::sequence(i, "step" + juce::String(step));
            const float defaultValue = i == 1 && (step == 1 || step == 9) ? 0.75f
                                    : i == 1 && (step == 5 || step == 13) ? -0.45f : 0.0f;
            addFloat(layout, stepId, prefix + "Step " + juce::String(step), -1.0f, 1.0f, 0.001f, defaultValue);
        }
    }

    for (int i = 1; i <= kNumModRoutes; ++i)
    {
        const auto prefix = "Mod Route " + juce::String(i) + " ";
        const int defaultSource = i == 1 ? 5 : 0;
        const int defaultDestination = i == 1 ? 1 : 0;
        const float defaultAmount = i == 1 ? 0.12f : 0.0f;
        addChoice(layout, ParamIDs::route(i, "source"), prefix + "Source", modSources, defaultSource);
        addChoice(layout, ParamIDs::route(i, "destination"), prefix + "Destination", modDestinations, defaultDestination);
        addFloat(layout, ParamIDs::route(i, "amount"), prefix + "Amount", -1.0f, 1.0f, 0.001f, defaultAmount);
    }

    return layout;
}

} // namespace combscanner
