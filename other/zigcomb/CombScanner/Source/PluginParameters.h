#pragma once

#include <JuceHeader.h>

namespace combscanner
{
namespace ParamIDs
{
inline juce::String comb(int oneBasedIndex, const juce::String& name)
{
    return "comb" + juce::String(oneBasedIndex) + "_" + name;
}
inline juce::String fx(int oneBasedIndex, const juce::String& name)
{
    return "fx" + juce::String(oneBasedIndex) + "_" + name;
}
inline juce::String lfo(int oneBasedIndex, const juce::String& name)
{
    return "lfo" + juce::String(oneBasedIndex) + "_" + name;
}
inline juce::String sequence(int oneBasedIndex, const juce::String& name)
{
    return "seq" + juce::String(oneBasedIndex) + "_" + name;
}
inline juce::String route(int oneBasedIndex, const juce::String& name)
{
    return "route" + juce::String(oneBasedIndex) + "_" + name;
}
} // namespace ParamIDs

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

juce::StringArray getModelNames();
juce::StringArray getFxTypeNames();
juce::StringArray getLfoShapeNames();
juce::StringArray getSequenceModeNames();
juce::StringArray getModSourceNames();
juce::StringArray getModDestinationNames();
juce::StringArray getSyncDivisionNames();

} // namespace combscanner
