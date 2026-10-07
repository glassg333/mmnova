#include "PluginProcessor.h"
#include "PluginEditor.h"

CombScannerAudioProcessor::CombScannerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

const juce::String CombScannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

CombScannerAudioProcessor::Parameters::ParameterLayout CombScannerAudioProcessor::createParameterLayout()
{
    Parameters::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "gain", "Gain", juce::NormalisableRange<float>(0.0f, 0.999f, 0.001f), 0.99f,
        "", juce::AudioProcessorParameter::genericParameter));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "damp", "Damp", juce::NormalisableRange<float>(0.0f, 0.999f, 0.001f), 0.90f,
        "", juce::AudioProcessorParameter::genericParameter));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "phase", "Phase", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.75f,
        "", juce::AudioProcessorParameter::genericParameter));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "delay1", "Delay 1", juce::NormalisableRange<float>(0.0f, 2000.0f, 0.1f), 115.0f,
        "ms", juce::AudioProcessorParameter::genericParameter));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "delay2", "Delay 2", juce::NormalisableRange<float>(0.0f, 2000.0f, 0.1f), 500.0f,
        "ms", juce::AudioProcessorParameter::genericParameter));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "scan", "Scan", juce::NormalisableRange<float>(0.0f, 2.0f, 0.001f), 0.0f,
        "", juce::AudioProcessorParameter::genericParameter));
    return layout;
}

void CombScannerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dsp.prepare(sampleRate, samplesPerBlock);
}

void CombScannerAudioProcessor::releaseResources()
{
    dsp.reset();
}

bool CombScannerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    const auto output = layouts.getMainOutputChannelSet();
    if (input != output)
        return false;
    return input == juce::AudioChannelSet::mono() || input == juce::AudioChannelSet::stereo();
}

void CombScannerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    combscanner::CombScannerDSP::Parameters values;
    values.gain = parameters.getRawParameterValue("gain")->load();
    values.damp = parameters.getRawParameterValue("damp")->load();
    values.phase = parameters.getRawParameterValue("phase")->load();
    values.delay1Ms = parameters.getRawParameterValue("delay1")->load();
    values.delay2Ms = parameters.getRawParameterValue("delay2")->load();
    // The source multiplexer is normalized internally; the user-facing control spans 0..2.
    values.scan = parameters.getRawParameterValue("scan")->load() * 0.5f;
    dsp.setParameters(values);
    dsp.processBlock(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
}

juce::AudioProcessorEditor* CombScannerAudioProcessor::createEditor()
{
    return new CombScannerAudioProcessorEditor(*this);
}

void CombScannerAudioProcessor::getStateInformation(juce::MemoryBlock& destination)
{
    const auto state = parameters.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary(*xml, destination);
}

void CombScannerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CombScannerAudioProcessor();
}
