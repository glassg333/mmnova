#pragma once

#include <JuceHeader.h>

#include <array>
#include <atomic>

#include "DSP/CombScannerEngine.h"
#include "PluginParameters.h"

class CombScannerAudioProcessor final : public juce::AudioProcessor
{
public:
    using APVTS = juce::AudioProcessorValueTreeState;

    CombScannerAudioProcessor();
    ~CombScannerAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override;

    int getNumPrograms() override { return 11; }
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destinationData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    APVTS& getValueTreeState() noexcept { return parameters; }
    const APVTS& getValueTreeState() const noexcept { return parameters; }
    void loadFactoryPreset(int index);

private:
    struct RawCombParameters
    {
        std::atomic<float>* enabled = nullptr;
        std::atomic<float>* ratio = nullptr;
        std::atomic<float>* ratio2 = nullptr;
        std::atomic<float>* delay1 = nullptr;
        std::atomic<float>* delay2 = nullptr;
        std::atomic<float>* feedback = nullptr;
        std::atomic<float>* damp = nullptr;
        std::atomic<float>* phase = nullptr;
        std::atomic<float>* diffusion = nullptr;
        std::atomic<float>* crossMix = nullptr;
        std::atomic<float>* drive = nullptr;
        std::atomic<float>* level = nullptr;
        std::atomic<float>* pan = nullptr;
        std::atomic<float>* scanWeight = nullptr;
    };

    struct RawFxParameters
    {
        std::atomic<float>* type = nullptr;
        std::atomic<float>* position = nullptr;
        std::atomic<float>* mix = nullptr;
        std::atomic<float>* size = nullptr;
        std::atomic<float>* pitch = nullptr;
        std::atomic<float>* blur = nullptr;
        std::atomic<float>* freeze = nullptr;
    };

    struct RawLfoParameters
    {
        std::atomic<float>* rate = nullptr;
        std::atomic<float>* depth = nullptr;
        std::atomic<float>* shape = nullptr;
        std::atomic<float>* sync = nullptr;
        std::atomic<float>* division = nullptr;
    };

    struct RawSequenceParameters
    {
        std::atomic<float>* rate = nullptr;
        std::atomic<float>* slew = nullptr;
        std::atomic<float>* length = nullptr;
        std::atomic<float>* mode = nullptr;
        std::atomic<float>* sync = nullptr;
        std::atomic<float>* division = nullptr;
        std::array<std::atomic<float>*, combscanner::kSequenceSteps> steps {};
    };

    struct RawRouteParameters
    {
        std::atomic<float>* source = nullptr;
        std::atomic<float>* destination = nullptr;
        std::atomic<float>* amount = nullptr;
    };

    struct RawParameterCache
    {
        std::atomic<float>* mode = nullptr;
        std::atomic<float>* model = nullptr;
        std::atomic<float>* bypass = nullptr;
        std::atomic<float>* classicFeedback = nullptr;
        std::atomic<float>* classicDamp = nullptr;
        std::atomic<float>* classicPhase = nullptr;
        std::atomic<float>* classicDelay1 = nullptr;
        std::atomic<float>* classicDelay2 = nullptr;
        std::atomic<float>* scan = nullptr;
        std::atomic<float>* scanWidth = nullptr;
        std::atomic<float>* scanRate = nullptr;
        std::atomic<float>* scanShape = nullptr;
        std::atomic<float>* diffusion = nullptr;
        std::atomic<float>* crossMix = nullptr;
        std::atomic<float>* motion = nullptr;
        std::atomic<float>* dryWet = nullptr;
        std::atomic<float>* output = nullptr;
        std::atomic<float>* stereoWidth = nullptr;
        std::array<RawCombParameters, combscanner::kNumCombs> combs {};
        std::array<RawFxParameters, combscanner::kNumFxSlots> fxSlots {};
        std::array<RawLfoParameters, combscanner::kNumLfos> lfos {};
        std::array<RawSequenceParameters, combscanner::kNumSequences> sequences {};
        std::array<RawRouteParameters, combscanner::kNumModRoutes> routes {};
    } raw;

    void cacheParameterPointers();
    combscanner::EngineParameters readEngineParameters() const noexcept;
    float load(const std::atomic<float>* parameter, float fallback = 0.0f) const noexcept;
    void setParameterActual(const juce::String& parameterID, float actualValue);

    APVTS parameters;
    combscanner::CombScannerEngine engine;
    float midiWheel = 0.0f;
    float midiVelocity = 0.0f;
    double currentTempoBpm = 120.0;
    int lastReportedLatency = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CombScannerAudioProcessor)
};
