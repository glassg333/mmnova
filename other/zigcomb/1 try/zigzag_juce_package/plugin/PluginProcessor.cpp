#include "PluginProcessor.h"
#include "PluginEditor.h"

ZigZagAudioProcessor::ZigZagAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

const juce::String ZigZagAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

ZigZagAudioProcessor::Parameters::ParameterLayout ZigZagAudioProcessor::createParameterLayout()
{
    Parameters::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "decay", "Decay", juce::NormalisableRange<float>(0.0f, 127.0f, 0.01f), 29.74f,
        "", juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String(value, 1); },
        [] (const juce::String& text) { return text.getFloatValue(); }));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "damping", "Damping", juce::NormalisableRange<float>(0.1f, 0.9839f, 0.0001f), 0.1f,
        "", juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String(value, 3); },
        [] (const juce::String& text) { return text.getFloatValue(); }));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "rotate", "Rotate", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f), 0.25f,
        "", juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String(value, 3); },
        [] (const juce::String& text) { return text.getFloatValue(); }));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "fluctuate", "Fluctuate", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f), 0.0f,
        "", juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String(value, 3); },
        [] (const juce::String& text) { return text.getFloatValue(); }));
    return layout;
}

void ZigZagAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dsp.prepare(sampleRate, samplesPerBlock);
}

void ZigZagAudioProcessor::releaseResources()
{
    dsp.reset();
}

bool ZigZagAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    const auto output = layouts.getMainOutputChannelSet();
    if (input != output)
        return false;
    return input == juce::AudioChannelSet::mono() || input == juce::AudioChannelSet::stereo();
}

void ZigZagAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const auto numChannels = buffer.getNumChannels();
    const auto numSamples = buffer.getNumSamples();

    zigzag::ZigZagDSP::Parameters values;
    values.decay = parameters.getRawParameterValue("decay")->load();
    values.damping = parameters.getRawParameterValue("damping")->load();
    values.rotate = parameters.getRawParameterValue("rotate")->load();
    values.fluctuate = parameters.getRawParameterValue("fluctuate")->load();
    dsp.setParameters(values);
    dsp.processBlock(buffer.getArrayOfWritePointers(), numChannels, numSamples);
}

juce::AudioProcessorEditor* ZigZagAudioProcessor::createEditor()
{
    return new ZigZagAudioProcessorEditor(*this);
}

void ZigZagAudioProcessor::getStateInformation(juce::MemoryBlock& destination)
{
    const auto state = parameters.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary(*xml, destination);
}

void ZigZagAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZigZagAudioProcessor();
}
