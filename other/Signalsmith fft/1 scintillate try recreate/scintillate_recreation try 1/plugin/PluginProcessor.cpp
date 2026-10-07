#include "PluginProcessor.h"

#include "PluginEditor.h"

#include <array>

ScintillateAudioProcessor::ScintillateAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout ScintillateAudioProcessor::createParameterLayout()
{
    using Float = juce::AudioParameterFloat;
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<Float>("mix", "Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.5f));
    layout.add(std::make_unique<Float>("width", "Width", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f));
    layout.add(std::make_unique<Float>("lowCut", "Low Cut", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
    layout.add(std::make_unique<Float>("highCut", "High Cut", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f));
    layout.add(std::make_unique<Float>("length", "Length", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.5f));
    layout.add(std::make_unique<Float>("tone", "Tone", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.8f));
    layout.add(std::make_unique<Float>("rate", "Rate", juce::NormalisableRange<float>(0.1f, 100.0f, 0.001f, 0.35f), 5.0f));
    layout.add(std::make_unique<Float>("decay", "Decay", juce::NormalisableRange<float>(-1.0f, 1.0f, 0.001f), 0.0f));
    layout.add(std::make_unique<Float>("density", "Density", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f));
    layout.add(std::make_unique<Float>("shimmer", "Shimmer", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
    return layout;
}

void ScintillateAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dsp.prepare(sampleRate, samplesPerBlock);
    dsp.setParameters(readParameters());
}

void ScintillateAudioProcessor::releaseResources()
{
}

bool ScintillateAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto output = layouts.getMainOutputChannelSet();
    return output == juce::AudioChannelSet::mono() || output == juce::AudioChannelSet::stereo();
}

scintillate::Parameters ScintillateAudioProcessor::readParameters() const noexcept
{
    scintillate::Parameters values;
    values.mix = parameters.getRawParameterValue("mix")->load();
    values.width = parameters.getRawParameterValue("width")->load();
    values.lowCut = parameters.getRawParameterValue("lowCut")->load();
    values.highCut = parameters.getRawParameterValue("highCut")->load();
    values.length = parameters.getRawParameterValue("length")->load();
    values.tone = parameters.getRawParameterValue("tone")->load();
    values.rate = parameters.getRawParameterValue("rate")->load();
    values.decay = parameters.getRawParameterValue("decay")->load();
    values.density = parameters.getRawParameterValue("density")->load();
    values.shimmer = parameters.getRawParameterValue("shimmer")->load();
    return values;
}

void ScintillateAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    dsp.setParameters(readParameters());

    std::array<float*, 2> channels { nullptr, nullptr };
    channels[0] = buffer.getWritePointer(0);
    channels[1] = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : channels[0];
    dsp.processBlock(channels.data(), buffer.getNumChannels(), buffer.getNumSamples());
}

void ScintillateAudioProcessor::getStateInformation(juce::MemoryBlock& destination)
{
    if (const auto state = parameters.copyState(); auto xml = state.createXml())
        copyXmlToBinary(*xml, destination);
}

void ScintillateAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (const auto xml = getXmlFromBinary(data, sizeInBytes))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorEditor* ScintillateAudioProcessor::createEditor()
{
    return new ScintillateAudioProcessorEditor(*this);
}
