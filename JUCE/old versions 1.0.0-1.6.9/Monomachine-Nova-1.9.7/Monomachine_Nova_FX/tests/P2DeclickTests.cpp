// Regression coverage for P2 manual-control de-zipping.
//
// A matched processor pair renders the same audio until one P2 control changes.
// The assertion is deliberately about the first changed sample: it catches a
// control-cadence discontinuity without treating the later, intentional tone
// change as an error.  Both the Synth voice and the FX input path execute this
// same source file.
#include "PluginProcessor.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>

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

void configureP2(MonomachineNovaAudioProcessor& processor)
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
    setParameter(processor.parameters, "p2_mix", 127);
}

void prepare(MonomachineNovaAudioProcessor& processor)
{
    processor.setRateAndBufferSizeDetails(44100, 64);
    processor.prepareToPlay(44100, 64);
    if (MonomachineNovaAudioProcessor::isSynthVersion)
    {
        // Keep the voice and its serial P2 envelope open for the measurement.
        setParameter(processor.parameters, "p0_1", 127);
        setParameter(processor.parameters, "p2_amp_hold", 127);
    }
    else
    {
        const int thru = p1MachineIndex(12);
        require(thru >= 0, "P1 FX-THRU is missing");
        setParameter(processor.parameters, "machine", static_cast<float>(thru));
        setParameter(processor.parameters, "m12_7", 64);
    }
    configureP2(processor);
}

struct Transition
{
    float preChangeMaximum = 0.0f;
    float firstChangedSample = 0.0f;
    float settledDifference = 0.0f;
};

Transition measureP2Gesture(const char* parameter, float initial, float target,
                            bool makeP2WetDifferent = false,
                            int distMode = monomachine::dspModeMnm)
{
    MonomachineNovaAudioProcessor reference, changed;
    prepare(reference);
    prepare(changed);
    setParameter(reference.parameters, "p2_mode_dist", static_cast<float>(distMode));
    setParameter(changed.parameters, "p2_mode_dist", static_cast<float>(distMode));

    if (makeP2WetDifferent)
    {
        // P2 MIX must crossfade genuinely different dry/wet paths.
        setParameter(reference.parameters, "p2_0_4", 127);
        setParameter(changed.parameters, "p2_0_4", 127);
    }
    setParameter(reference.parameters, parameter, initial);
    setParameter(changed.parameters, parameter, initial);

    juce::MidiBuffer referenceMidi, changedMidi;
    Transition result;
    bool haveFirst = false;
    constexpr int transitionBlock = 240;

    for (int block = 0; block < 440; ++block)
    {
        juce::AudioBuffer<float> referenceAudio(2, 64), changedAudio(2, 64);
        if (MonomachineNovaAudioProcessor::isSynthVersion)
        {
            if (block == 0)
            {
                referenceMidi.addEvent(juce::MidiMessage::noteOn(1, 60, juce::uint8(127)), 0);
                changedMidi.addEvent(juce::MidiMessage::noteOn(1, 60, juce::uint8(127)), 0);
            }
        }
        else
        {
            for (int i = 0; i < 64; ++i)
            {
                const float sample = static_cast<float>(block * 64 + i);
                const float left = 0.20f * std::sin(0.037f * sample) + 0.08f * std::sin(0.112f * sample);
                const float right = 0.16f * std::cos(0.071f * sample);
                referenceAudio.setSample(0, i, left); referenceAudio.setSample(1, i, right);
                changedAudio.setSample(0, i, left); changedAudio.setSample(1, i, right);
            }
        }

        if (block == transitionBlock)
            setParameter(changed.parameters, parameter, target);

        reference.processBlock(referenceAudio, referenceMidi);
        changed.processBlock(changedAudio, changedMidi);
        referenceMidi.clear();
        changedMidi.clear();

        for (int i = 0; i < 64; ++i)
            for (int channel = 0; channel < 2; ++channel)
            {
                const float difference = changedAudio.getSample(channel, i) - referenceAudio.getSample(channel, i);
                if (block < transitionBlock)
                    result.preChangeMaximum = std::max(result.preChangeMaximum, std::abs(difference));
                else
                {
                    if (! haveFirst)
                    {
                        result.firstChangedSample = std::abs(difference);
                        haveFirst = true;
                    }
                    if (block >= 400)
                        result.settledDifference = std::max(result.settledDifference, std::abs(difference));
                }
            }
    }
    return result;
}

void requireGesture(const char* name, const Transition& result, float firstSampleLimit)
{
    require(result.preChangeMaximum < 1.0e-7f, "matched P2 control pair diverged before its gesture");
    require(result.firstChangedSample < firstSampleLimit, "P2 control changed the waveform on its first sample");
    require(result.settledDifference > 1.0e-3f, "P2 gesture had no eventual audible/control effect");
    std::printf("P2DECLICK %s pre=%.9g first=%.9g settled=%.9g PASS\n",
                name, result.preChangeMaximum, result.firstChangedSample, result.settledDifference);
}
}

int main()
{
    try
    {
        juce::MessageManager::getInstance();
        requireGesture("MACHINE-MIX", measureP2Gesture("p2m15_3", 0, 127), 0.01f);
        requireGesture("GROUP-MIX", measureP2Gesture("p2_mix", 0, 127, true), 0.01f);
        // The static compatibility saturation is especially sensitive to an
        // 8-sample control step.  Its audio-rate de-zipper must make the first
        // sample far smaller than the general MIX/PAN/VOL threshold.
        requireGesture("DIST", measureP2Gesture("p2_0_4", 64, 127), 0.001f);
        requireGesture("DIST-MNM", measureP2Gesture("p2_0_4", 64, 127, false, monomachine::dspModeMnm), 0.001f);
        requireGesture("VOL", measureP2Gesture("p2_0_5", 64, 0), 0.01f);
        requireGesture("PAN", measureP2Gesture("p2_0_6", 64, 127), 0.01f);
        std::printf("P2DECLICK %s PASS\n", MonomachineNovaAudioProcessor::isSynthVersion ? "Synth" : "FX");
        juce::MessageManager::deleteInstance();
        return 0;
    }
    catch (const std::exception& error)
    {
        std::fprintf(stderr, "P2DECLICK FAIL: %s\n", error.what());
        juce::MessageManager::deleteInstance();
        return 1;
    }
}
