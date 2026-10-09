#pragma once

#include <JuceHeader.h>

#include <array>
#include <atomic>
#include <memory>
#include <vector>

#include "lab/Engine.h"
#include "lab/EngineRegistry.h"

// Comb Scanner Lab: один плагин, 7 страниц-слотов, на каждой — свой DSP из other/zigcomb.
// Активная страница выбирается параметром "slot" (автоматизируется), остальные не считаются (CPU не тратится).
class CombScannerLabAudioProcessor final : public juce::AudioProcessor
{
public:
    static constexpr int numSlots = lab::kNumSlots;

    // --- ID глобальных параметров -------------------------------------------
    static constexpr const char* activeSlotId  = "slot";
    static constexpr const char* mixId         = "mix";
    static constexpr const char* outputId      = "output";
    static constexpr const char* scanRateId    = "scan_rate";
    static constexpr const char* scanDepthId   = "scan_depth";
    static constexpr const char* resetSwitchId = "reset_on_switch";

    CombScannerLabAudioProcessor();
    ~CombScannerLabAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 10.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // --- для редактора -------------------------------------------------------
    int getActiveSlot() const;
    void setActiveSlot (int slot);

    void copySlotToAll (int slot);        // перенести настройки страницы на все остальные (честное A/B)
    void resetSlotToDefaults (int slot);  // вернуть ручки страницы к значениям по умолчанию

private:
    // ВАЖНО: порядок объявления = порядок инициализации.
    // Сначала создаются движки, потом создаётся дерево параметров (оно читает их описания).
    using EngineArray = std::array<std::unique_ptr<lab::Engine>, numSlots>;

    struct SlotRuntime
    {
        std::vector<std::atomic<float>*> values;  // указатели на параметры, по индексу params()
        std::atomic<float>* model = nullptr;      // nullptr, если у движка нет выбора модели
        std::atomic<float>* trim = nullptr;       // дБ, свой на каждую страницу
        int scanIndex = -1;                       // индекс ручки Scan в params(), -1 если нет
        int lastModel = -1;
    };

    static EngineArray makeEngines();
    static juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout (const EngineArray& engines);

    void cacheSlotPointers (const EngineArray& enginesInOrder);
    void applySlotParameters (int slot, float scanOverride);
    float readParam (const std::atomic<float>* value, float fallback) const;

    EngineArray engines;
    std::array<SlotRuntime, numSlots> runtime {};

    // Указатели на глобальные параметры (чтобы не искать их по строке в audio thread).
    std::atomic<float>* pSlot = nullptr;
    std::atomic<float>* pMix = nullptr;
    std::atomic<float>* pOutput = nullptr;
    std::atomic<float>* pScanRate = nullptr;
    std::atomic<float>* pScanDepth = nullptr;
    std::atomic<float>* pResetOnSwitch = nullptr;
    std::array<bool, numSlots> slotPrepared {};   // движок готовится лениво, только когда станет активным
    juce::AudioBuffer<float> dryBuffer;           // для Dry/Wet параметра, без аллокаций в audio thread
    int preparedBlockSize = 512;

    // Переключение страниц с коротким фейдом, чтобы не щёлкало.
    enum class FadeState { steady, out, in };
    FadeState fadeState = FadeState::steady;
    int currentSlot = 0;
    float fadeGain = 1.0f;
    float lastLoudnessGain = 1.0f;

    double currentSampleRate = 48000.0;
    double scanPhase = 0.0;

public:
    juce::AudioProcessorValueTreeState apvts;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CombScannerLabAudioProcessor)
};
