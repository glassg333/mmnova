// PluginProcessor.h — Omega8 VST: patch loading, morph, arpeggiator, MIDI
#pragma once
#include <JuceHeader.h>   // обязателен ПЕРВЫМ (как в Monomachine Nova): иначе
                          // juce_TargetPlatform.h -> error C1189 "No global header file"
#include <juce_audio_processors/juce_audio_processors.h>
#include "OmegaEngine.h"

// Build marker for the DK2/MW/UI regression fixes; check the host About box or build log.
inline constexpr const char* kOmega8BuildRev = "r27.1-20261004";
#if defined(_MSC_VER)
  #pragma message ("omega8 build rev r27.1-20261004")
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
    // r17: строка статуса загрузки банка (LCD в редакторе, 4 c):
    // успех / «LOAD FAILED: ...» — раньше отказ грузился молча.
    juce::String loadStatus = "FACTORY BANK A (embedded)";
    juce::int64   loadStatusTime = 0;

    const omega8::Patch& currentPatch() const { return engine.patch; }
    float morphAmount() const { return rawParam ("morph"); }
    float effectiveMorph() const noexcept
    {
        // MW->BLEND is an independent host/global option, never read from either patch.
        const float wheelMorph = mwMorphSm.load (std::memory_order_relaxed);
        return juce::jlimit (0.0f, 1.0f, rawParam ("morph") + (mwBlendEnabled() ? wheelMorph : 0.0f));
    }
    bool mwBlendEnabled() const noexcept { return rawParam ("mwBlendOn") > 0.5f; }
    void setMwBlendEnabled (bool enabled)
    {
        if (auto* p = apvts.getParameter ("mwBlendOn"))
        {
            p->beginChangeGesture();
            p->setValueNotifyingHost (p->convertTo0to1 (enabled ? 1.0f : 0.0f));
            p->endChangeGesture();
        }
    }
    int   presetA() const { return (int) rawParam ("presetA"); }
    int   presetB() const { return (int) rawParam ("presetB"); }

    // --- r5: живое редактирование патча ручками (UI ↔ аудио под SpinLock) ---
    int           editSlot() const;   // controls edit A below 50%, B at/above 50% (matches Patch::lerp)
    void          setPatchByte (int patchIdx, int ofs, uint8_t v);
    omega8::Patch getEditPatch();     // копия слота (для дисплея/ручек)

    // r18.2: RESET ALL — вернуть звук (morph->0, голоса убить,
    // arp/CC-лэтчи в ноль, свежий push патча A). GUI-тред, под bankLock.
    void resetAll();
    void accuTune();
    // r18.4: PANIC — мгновенно убить все голоса (меню справа сверху).
    void panic() { engine.allNotesOff (); seqHeld.clear (); arp.held.clear (); }
    // r18.4: debug-переключатель поколения фильтра SEM (меню справа сверху).
    void setFilterGen (bool newGen) { engine.setSemFilterNew (newGen); }
    bool filterGenNew() const { return engine.getSemFilterNew (); }

    // APVTS
    juce::AudioProcessorValueTreeState apvts;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    float getParamValue (const juce::String& id) const;
    void  refreshPatchFromParams();

    omega8::Engine engine;
    int   lastFiltParam = -1;        // r14: синк параметра Filter Type -> байт
    // (r16: jump-детекция r11 убрана — BLEND живой, кроссфейд внутри движка)
    omega8::Patch  bank[128];
    int            bankCount = 0;
    mutable juce::SpinLock bankLock;   // bank[] vs аудиопоток (refresh под TryLock)

    // r18.7: MULTI (оригинал: 8 parts, слой/split; в VST — слой: активные
    // части звучат вместе, лампочки 1-8 = состояние частей). patch = номер
    // в ТЕКУЩЕМ банке (0..127), vol/pan 0..127/-64..63. Состояние VST (не
    // байты патча) — экспорт/импорт в multi-syx добавится позже.
    // Публично: GUI-тред (панель/матрица) читает и пишет.
  public:
    struct MultiPartS
    {
        int  patch = 127;
        int  vol   = 64;
        int  pan   = 0;
        bool on    = false;
        int  step  = 0;     // r18.9: SEQ — на каком степах (0..7) часть играет
    };
    std::array<MultiPartS, 8> multi;

    // r18.9: STEP SEQUENCER — при seqOn=ON часть звучит только на своём степах;
    // часы внутренние 120 BPM, 1 шаг = 1 доля (beat), 8 шагов = круг.
    // Офф-степе части молчат (layer при seqOn=OFF — как раньше).
    bool   seqOn     = false;
    int    seqStep   = 0;
    double seqFrac   = 0.0;
    bool   arpSync   = false;
    double hostBpm   = 120.0;
    double lastSr  = 44100.0;
    double seqRate_ = 44100.0;
    void seqSetRate (double sr) { seqRate_ = sr > 0.0 ? sr : 44100.0; }
    void seqAdvance (int n)
    {
        const double bpm = (hostBpm > 1.0 ? hostBpm : 120.0);
        const double stepSec = (60.0 / bpm) * 0.5;   // v27: 1/8 доли в синке с hostBpm (при 120 BPM = 0.25 с)
        seqFrac += (double) n / std::max (1.0, stepSec * seqRate_);
        while (seqFrac >= 1.0)
        {
            seqFrac -= 1.0;
            seqStep = (seqStep + 1) & 7;
        }
    }

    // r18.9: SEQ — зажатые ноты (ре-триггер на степах частей)
    struct SeqHold { int note = -1; float vel = 1.0f; };
    std::vector<SeqHold> seqHeld;

    // r18.9: SEQ — release всех голосов MULTI-частей (смена режима / noteOff)
    void killPartVoices () noexcept
    {
        for (int qi = 0; qi < 8; ++qi) engine.noteOffPart (qi);
    }

    // r27: редактор может погасить одну MULTI-часть; Engine остаётся private.
    void killPartVoice (int part) noexcept
    {
        if (part >= 0 && part < 8) engine.noteOffPart (part);
    }

    bool multiActive() const
    {
        for (const auto& m : multi) if (m.on) return true;
        return false;
    }

  private:

    // cached MIDI state
    float curModWheel = 0, curBend = 0, curPressure = 0, curCont1 = 0, curCont2 = 0;
    std::atomic<float> mwMorphSm { 0.0f }; // cross-thread safe ModWheel smoothing for additive MW=BLEND
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
