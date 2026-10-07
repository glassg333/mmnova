#include "PluginProcessor.h"
#include "PluginEditor.h"

CombScannerVariantsAudioProcessor::CombScannerVariantsAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
CombScannerVariantsAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> layout;
    juce::StringArray variantNames;
    for (int i = 0; i < combscanner::CombScannerVariantsDSP::variantCount; ++i)
        variantNames.add(combscanner::CombScannerVariantsDSP::getVariantName(i));
    layout.push_back(std::make_unique<juce::AudioParameterChoice>("variant", "Model", variantNames, 0));
    layout.push_back(std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.99f));
    layout.push_back(std::make_unique<juce::AudioParameterFloat>("damp", "Damp", 0.0f, 1.0f, 0.90f));
    layout.push_back(std::make_unique<juce::AudioParameterFloat>("phase", "Phase", 0.0f, 1.0f, 0.75f));
    layout.push_back(std::make_unique<juce::AudioParameterFloat>("delay1", "Delay 1", 0.0f, 2000.0f, 115.0f));
    layout.push_back(std::make_unique<juce::AudioParameterFloat>("delay2", "Delay 2", 0.0f, 2000.0f, 500.0f));
    layout.push_back(std::make_unique<juce::AudioParameterFloat>("scan", "Scan", 0.0f, 2.0f, 0.0f));
    layout.push_back(std::make_unique<juce::AudioParameterFloat>("character", "Character", 0.0f, 1.0f, 0.55f));
    return { layout.begin(), layout.end() };
}

void CombScannerVariantsAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dsp.prepare(sampleRate, samplesPerBlock);
}

void CombScannerVariantsAudioProcessor::releaseResources()
{
    dsp.reset();
}

bool CombScannerVariantsAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& input = layouts.getMainInputChannelSet();
    const auto& output = layouts.getMainOutputChannelSet();
    return ! input.isDisabled() && (input == output)
        && (output == juce::AudioChannelSet::mono() || output == juce::AudioChannelSet::stereo());
}

void CombScannerVariantsAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                      juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    combscanner::CombScannerVariantsDSP::Parameters values;
    values.gain = parameters.getRawParameterValue("gain")->load();
    values.damp = parameters.getRawParameterValue("damp")->load();
    values.phase = parameters.getRawParameterValue("phase")->load();
    values.delay1Ms = parameters.getRawParameterValue("delay1")->load();
    values.delay2Ms = parameters.getRawParameterValue("delay2")->load();
    values.scan = parameters.getRawParameterValue("scan")->load() * 0.5f;
    values.character = parameters.getRawParameterValue("character")->load();
    values.variant = static_cast<int>(parameters.getRawParameterValue("variant")->load());
    dsp.setParameters(values);
    dsp.processBlock(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
}

void CombScannerVariantsAudioProcessor::getStateInformation(juce::MemoryBlock& destination)
{
    if (auto state = parameters.copyState(); auto xml = state.createXml())
        copyXmlToBinary(*xml, destination);
}

void CombScannerVariantsAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CombScannerVariantsAudioProcessor();
}
