// PluginProcessor.h — Omega8 VST: patch loading, morph, arpeggiator, MIDI
#pragma once
#include <JuceHeader.h>   // обязателен ПЕРВЫМ (как в Monomachine Nova): иначе
                          // juce_TargetPlatform.h -> error C1189 "No global header file"
#include <juce_audio_processors/juce_audio_processors.h>
#include "OmegaEngine.h"

// Ревизия сборки: ищи "r9-20260929" в логе MSBuild — если нет, скомпилированы старые файлы.
inline constexpr const char* kOmega8BuildRev = "r9-20260929";
#if defined(_MSC_VER)
  #pragma message ("omega8 build rev r9-20260929")
#endif

struct Omega8AudioProcessor : juce::AudioProcessor
{
    Omega8AudioProcessor();

    // --- juce::AudioProcessor ---
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Omega8"; }
    bool acceptsMidi() const override  { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }

    int getNumPrograms() override { return 128; }
    int getCurrentProgram() override { return (int) getParamValue ("presetA"); }
    void setCurrentProgram (int i) override;
    const juce::String getProgramName (int i) override;
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // --- bank handling ---
    bool loadBankFile (const juce::File& f);
    bool loadBankBytes (const std::vector<uint8_t>& v);
    omega8::Patch& patchAt (int idx) { return bank[idx]; }
    int bankSize() const { return bankCount; }
    juce::String patchNameAt (int idx) const;
    juce::File lastBankFile;

    const omega8::Patch& currentPatch() const { return engine.patch; }
    float morphAmount() const { return rawParam ("morph"); }
    int   presetA() const { return (int) rawParam ("presetA"); }
    int   presetB() const { return (int) rawParam ("presetB"); }

    // --- r5: живое редактирование патча ручками (UI ↔ аудио под SpinLock) ---
    int           editSlot() const;   // какой патч правят ручки: A при morph<=50%, иначе B
    void          setPatchByte (int patchIdx, int ofs, uint8_t v);
    omega8::Patch getEditPatch();     // копия слота (для дисплея/ручек)

    // APVTS
    juce::AudioProcessorValueTreeState apvts;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    float getParamValue (const juce::String& id) const;
    void  refreshPatchFromParams();

    omega8::Engine engine;
    omega8::Patch  bank[128];
    int            bankCount = 0;
    mutable juce::SpinLock bankLock;   // bank[] vs аудиопоток (refresh под TryLock)

    // cached MIDI state
    float curBend = 0, curPressure = 0, curCont1 = 0, curCont2 = 0;
    std::vector<float> scratchL, scratchR;

    float rawParam (const char* id) const
    {
        if (auto* p = apvts.getRawParameterValue (id)) return p->load();
        return 0.0f;
    }

    // arpeggiator
    struct Arp
    {
        std::vector<int> held;
        int   idx = 0, dir = 1, lastNote = -1;
        double phase = 0, interval = 0.125;
        int   pattern = 0, octaves = 1;
        bool  on = false;
        juce::Random rnd;
    } arp;

    void handleArp (double dt, juce::MidiBuffer* midiOut);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Omega8AudioProcessor)
};
