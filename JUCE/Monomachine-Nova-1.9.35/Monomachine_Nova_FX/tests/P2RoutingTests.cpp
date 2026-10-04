// Regression coverage for P2 as a serial, transparent-by-default insert.
// It guards P2-only failure modes:
//   - neutral P2 changed the Synth signal (second velocity/envelope application);
//   - Synth P2 VOL=64 was not unity;
//   - a neutral triggered P2 FILTER still coloured the signal;
//   - neutral FX P2 changed the signal despite native CHORUS MIX=0;
//   - equal P1/P2 DELAY controls did not null; or CHORUS PDC omitted P2.
#include "PluginProcessor.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <vector>

namespace
{
void require(bool value, const char* message)
{
    if (! value)
        throw std::runtime_error(message);
}

void setParameter(juce::AudioProcessorValueTreeState& state, const char* id, float value)
{
    auto* parameter = state.getParameter(id);
    if (parameter == nullptr)
        throw std::runtime_error((juce::String("missing parameter: ") + id).toStdString());
    parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
}

int p1MachineIndex(int machineId)
{
    const auto& machines = nova::machines();
    for (int i = 0; i < static_cast<int>(machines.size()); ++i)
        if (machines[static_cast<size_t>(i)].id == machineId)
            return i;
    return -1;
}

int p2MachineIndex(int machineId)
{
    const auto& machines = MonomachineNovaAudioProcessor::p2FxMachines();
    for (int i = 0; i < static_cast<int>(machines.size()); ++i)
        if (machines[static_cast<size_t>(i)].id == machineId)
            return i;
    return -1;
}

void setNeutralP2Chorus(MonomachineNovaAudioProcessor& processor, float groupMix)
{
    const int chorus = p2MachineIndex(15);
    require(chorus >= 0, "P2 native CHORUS is missing");
    setParameter(processor.parameters, "p2_machine", static_cast<float>(chorus));
    setParameter(processor.parameters, "p2m15_0", 70); setParameter(processor.parameters, "p2m15_1", 95);
    setParameter(processor.parameters, "p2m15_2", 41); setParameter(processor.parameters, "p2m15_3", 0);
    setParameter(processor.parameters, "p2m15_4", 127); setParameter(processor.parameters, "p2m15_5", 127);
    setParameter(processor.parameters, "p2m15_6", 127); setParameter(processor.parameters, "p2m15_7", 64);
    setParameter(processor.parameters, "p2_0_4", 64); setParameter(processor.parameters, "p2_0_5", 64);
    setParameter(processor.parameters, "p2_0_6", 64);
    setParameter(processor.parameters, "p2_1_0", 0); setParameter(processor.parameters, "p2_1_1", 127);
    setParameter(processor.parameters, "p2_1_2", 0); setParameter(processor.parameters, "p2_1_3", 0);
    setParameter(processor.parameters, "p2_1_4", 0); setParameter(processor.parameters, "p2_1_5", 93);
    setParameter(processor.parameters, "p2_1_6", 64); setParameter(processor.parameters, "p2_1_7", 64);
    setParameter(processor.parameters, "p2_2_0", 64); setParameter(processor.parameters, "p2_2_1", 64);
    setParameter(processor.parameters, "p2_2_2", 0); setParameter(processor.parameters, "p2_2_3", 64);
    setParameter(processor.parameters, "p2_2_4", 64); setParameter(processor.parameters, "p2_2_5", 28);
    setParameter(processor.parameters, "p2_2_6", 0); setParameter(processor.parameters, "p2_2_7", 127);
    setParameter(processor.parameters, "p2_mix", groupMix);
}

float maximumDifference(const std::vector<float>& a, const std::vector<float>& b)
{
    require(a.size() == b.size(), "comparison buffers differ in size");
    float result = 0.0f;
    for (size_t i = 0; i < a.size(); ++i)
        result = std::max(result, std::abs(a[i] - b[i]));
    return result;
}

std::vector<float> renderSynth(float p2Mix, bool p2Delay = false)
{
    MonomachineNovaAudioProcessor processor;
    processor.setRateAndBufferSizeDetails(44100, 64);
    processor.prepareToPlay(44100, 64);
    // Keep both serial envelopes open.  P2 must not apply MIDI velocity a
    // second time, and its neutral FILTER must not colour a triggered voice.
    setParameter(processor.parameters, "p0_1", 127);
    setParameter(processor.parameters, "p2_amp_hold", 127);
    setNeutralP2Chorus(processor, p2Mix);
    if (p2Delay)
    {
        // P2 EFFX DELAY remains part of the P2 wet path. At group MIX=0 it
        // must not leak into the rendered Synth signal.
        setParameter(processor.parameters, "p2_2_3", 64);
        setParameter(processor.parameters, "p2_2_4", 110);
        setParameter(processor.parameters, "p2_2_5", 80);
    }

    juce::MidiBuffer midi;
    midi.addEvent(juce::MidiMessage::noteOn(1, 60, juce::uint8(100)), 0);
    std::vector<float> result;
    for (int block = 0; block < 360; ++block)
    {
        juce::AudioBuffer<float> audio(2, 64);
        audio.clear();
        processor.processBlock(audio, midi);
        midi.clear();
        if (block >= 80)
            for (int i = 0; i < audio.getNumSamples(); ++i)
            {
                result.push_back(audio.getSample(0, i));
                result.push_back(audio.getSample(1, i));
            }
    }
    return result;
}

void configureFx(MonomachineNovaAudioProcessor& processor, bool p2Full, bool p1Delay, bool p2Delay)
{
    processor.setRateAndBufferSizeDetails(44100, 64);
    processor.prepareToPlay(44100, 64);
    const int thru = p1MachineIndex(12);
    require(thru >= 0, "P1 FX-THRU is missing");
    setParameter(processor.parameters, "machine", static_cast<float>(thru));
    setParameter(processor.parameters, "m12_7", 64);
    setNeutralP2Chorus(processor, p2Full ? 127.0f : 0.0f);
    // Same native delay values on the corresponding P1/P2 FX page.  P1's
    // historical `p2_*` name is the FX page; the P2 FX page is `p2_2_*`.
    setParameter(processor.parameters, "p2_3", 64); setParameter(processor.parameters, "p2_4", p1Delay ? 110.0f : 64.0f);
    setParameter(processor.parameters, "p2_5", p1Delay ? 80.0f : 28.0f);
    setParameter(processor.parameters, "p2_2_3", 64); setParameter(processor.parameters, "p2_2_4", p2Delay ? 110.0f : 64.0f);
    setParameter(processor.parameters, "p2_2_5", p2Delay ? 80.0f : 28.0f);
}

struct FxPairResult { std::vector<float> first, second; int firstLatency = 0, secondLatency = 0; };

FxPairResult renderFxPair(bool firstP2Full, bool firstP1Delay, bool firstP2Delay,
                          bool secondP2Full, bool secondP1Delay, bool secondP2Delay)
{
    MonomachineNovaAudioProcessor first, second;
    configureFx(first, firstP2Full, firstP1Delay, firstP2Delay);
    configureFx(second, secondP2Full, secondP1Delay, secondP2Delay);
    juce::MidiBuffer firstMidi, secondMidi;
    FxPairResult result;
    for (int block = 0; block < 620; ++block)
    {
        juce::AudioBuffer<float> firstAudio(2, 64), secondAudio(2, 64);
        for (int i = 0; i < 64; ++i)
        {
            const int sample = block * 64 + i;
            const float left = 0.20f * std::sin(0.037f * sample) + 0.08f * std::sin(0.112f * sample);
            const float right = 0.16f * std::cos(0.071f * sample);
            firstAudio.setSample(0, i, left); firstAudio.setSample(1, i, right);
            secondAudio.setSample(0, i, left); secondAudio.setSample(1, i, right);
        }
        first.processBlock(firstAudio, firstMidi);
        second.processBlock(secondAudio, secondMidi);
        if (block >= 360)
            for (int i = 0; i < 64; ++i)
            {
                result.first.push_back(firstAudio.getSample(0, i)); result.first.push_back(firstAudio.getSample(1, i));
                result.second.push_back(secondAudio.getSample(0, i)); result.second.push_back(secondAudio.getSample(1, i));
            }
    }
    result.firstLatency = first.getLatencySamples();
    result.secondLatency = second.getLatencySamples();
    return result;
}

int fxDoubleChorusLatency()
{
    MonomachineNovaAudioProcessor processor;
    processor.setRateAndBufferSizeDetails(44100, 64);
    processor.prepareToPlay(44100, 64);
    const int chorus = p1MachineIndex(15);
    require(chorus >= 0, "P1 native CHORUS is missing");
    setParameter(processor.parameters, "machine", static_cast<float>(chorus));
    setParameter(processor.parameters, "m15_0", 70); setParameter(processor.parameters, "m15_1", 95);
    setParameter(processor.parameters, "m15_2", 41); setParameter(processor.parameters, "m15_3", 0);
    setParameter(processor.parameters, "m15_4", 127); setParameter(processor.parameters, "m15_5", 127);
    setParameter(processor.parameters, "m15_6", 127); setParameter(processor.parameters, "m15_7", 64);
    setNeutralP2Chorus(processor, 127.0f);
    juce::MidiBuffer midi;
    juce::AudioBuffer<float> audio(2, 64);
    audio.clear();
    processor.processBlock(audio, midi);
    return processor.getLatencySamples();
}
}

int main()
{
    try
    {
        juce::MessageManager::getInstance();
        if (MonomachineNovaAudioProcessor::isSynthVersion)
        {
            const auto dry = renderSynth(0.0f);
            const auto dryWithP2Delay = renderSynth(0.0f, true);
            const auto wet = renderSynth(127.0f);
            const float difference = maximumDifference(dry, wet);
            const float mutedDelayDifference = maximumDifference(dry, dryWithP2Delay);
            require(difference < 1.0e-6f, "neutral Synth P2 changed the rendered voice");
            require(mutedDelayDifference < 1.0e-6f,
                    "P2 DELAY leaked while Synth P2 group MIX was zero");
            std::printf("P2ROUTING Synth neutral maxDiff=%.9g mutedDelay=%.9g PASS\n", difference, mutedDelayDifference);
        }
        else
        {
            // Render the compared paths side by side: this keeps any process-wide native state
            // in the same phase and makes the assertion a direct routing null test.
            const auto neutral = renderFxPair(false, false, false, true, false, false);
            const auto delay = renderFxPair(false, true, false, true, false, true);
            const auto mutedDelay = renderFxPair(false, false, false, false, false, true);
            const float neutralDifference = maximumDifference(neutral.first, neutral.second);
            const float delayDifference = maximumDifference(delay.first, delay.second);
            const float mutedDelayDifference = maximumDifference(mutedDelay.first, mutedDelay.second);
            require(neutralDifference < 1.0e-6f, "neutral FX P2 changed the signal");
            require(delayDifference < 1.0e-6f, "P2 delay differs from its matching P1 delay path");
            require(mutedDelayDifference < 1.0e-6f,
                    "P2 DELAY leaked while FX P2 group MIX was zero");
            require(neutral.firstLatency == 16 && neutral.secondLatency == 16
                 && delay.firstLatency == 16 && delay.secondLatency == 16,
                    "P2 native CHORUS latency is not reported to the FX host");
            const int doubleChorusLatency = fxDoubleChorusLatency();
            require(doubleChorusLatency == 32, "P1 + P2 native CHORUS latency is not reported as 32 samples");
            std::printf("P2ROUTING FX neutral=%.9g delay=%.9g mutedDelay=%.9g latency=%d doubleChorus=%d PASS\n", neutralDifference, delayDifference, mutedDelayDifference, neutral.secondLatency, doubleChorusLatency);
        }
        juce::MessageManager::deleteInstance();
        return 0;
    }
    catch (const std::exception& error)
    {
        std::fprintf(stderr, "P2ROUTING FAIL: %s\n", error.what());
        juce::MessageManager::deleteInstance();
        return 1;
    }
}
