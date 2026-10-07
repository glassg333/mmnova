#include "PluginProcessor.h"
#include "PluginEditor.h"

CsMmAudioProcessor::CsMmAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true).withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout CsMmAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    juce::StringArray names;
    for (int i = 0; i < csmm::CsMmVariantsDSP::variantCount; ++i) names.add(csmm::CsMmVariantsDSP::getVariantName(i));
    p.push_back(std::make_unique<juce::AudioParameterChoice>("variant", "Algorithm", names, 0));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", 0.0f, 1.0f, 0.62f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("damp", "Damp", 0.0f, 1.0f, 0.62f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("phase", "Phase", 0.0f, 1.0f, 0.35f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("diffusion", "Diffusion", 0.0f, 1.0f, 0.45f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("crossMix", "Cross Mix", 0.0f, 1.0f, 0.50f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("motion", "Motion", 0.0f, 1.0f, 0.50f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("chorusMix", "Chorus Mix", 0.0f, 1.0f, 0.58f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("chorusDepth", "Chorus Depth", 0.0f, 1.0f, 0.60f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("chorusRate", "Chorus Rate", 0.0f, 1.0f, 0.35f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("delay1", "Delay 1", 0.0f, 2000.0f, 75.0f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("delay2", "Delay 2", 0.0f, 2000.0f, 310.0f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("scan", "Scan", 0.0f, 2.0f, 0.25f));
    return { p.begin(), p.end() };
}

void CsMmAudioProcessor::prepareToPlay(double rate, int block) { dsp.prepare(rate, block); }
void CsMmAudioProcessor::releaseResources() { dsp.reset(); }

bool CsMmAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& in = layouts.getMainInputChannelSet(); const auto& out = layouts.getMainOutputChannelSet();
    return !in.isDisabled() && in == out && (out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo());
}

void CsMmAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals; csmm::CsMmVariantsDSP::Parameters p;
    auto value = [this](const char* id) { return parameters.getRawParameterValue(id)->load(); };
    p.feedback=value("feedback");p.damp=value("damp");p.phase=value("phase");p.diffusion=value("diffusion");p.crossMix=value("crossMix");p.motion=value("motion");p.chorusMix=value("chorusMix");p.chorusDepth=value("chorusDepth");p.chorusRate=value("chorusRate");p.delay1Ms=value("delay1");p.delay2Ms=value("delay2");p.scan=value("scan")*.5f;p.variant=static_cast<int>(value("variant"));
    dsp.setParameters(p); dsp.processBlock(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
}

void CsMmAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{ if (auto state=parameters.copyState(); auto xml=state.createXml()) copyXmlToBinary(*xml,dest); }
void CsMmAudioProcessor::setStateInformation(const void* data,int size)
{ if(auto xml=getXmlFromBinary(data,size)) if(xml->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*xml)); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new CsMmAudioProcessor(); }
