// ============================================================================
// MnmFilterDistJuceExample.h — ПРИМЕР подключения к JUCE-проекту
// ----------------------------------------------------------------------------
// Ядро модуля (source/*.h) НЕ зависит от JUCE — чистый C++17. Этот файл
// показывает типовую обвязку juce::AudioProcessor: буферизация 16-сэмпловых
// кадров, триггеры огибающих, параметры как на панели Monomachine.
//
// Порядок вставки в проект:
//   1. Скопировать папку source/ в JuceProject/Source/mnm_filter_dist/
//   2. Добавить все .h в таргет (CMake: target_sources(...); Projucer: Add files)
//   3. В процессоре: #include "MnmFilterDistJuceExample.h" и завести
//      MnmTrackFx на трек/голос.
// ============================================================================
#pragma once
#include <JuceHeader.h>
#include "MnmVoiceFilterDist.h"

// Один "трек" Monomachine (FLT + DIST + AMP-гейт)
struct MnmTrackFx
{
    mnm::MnmVoiceFilterDist fx;

    // --- значения ручек (как на панели машины) ------------------------------
    int base = 0, wdth = 127, hpq = 0, lpq = 0;          // страница FLT
    int fAtk = 0, fDec = 64;                             // фильтровая огибающая
    int bofs = 64, wofs = 64;                            // env→BASE/WDTH (биполярные)
    int dist = 0;                                        // −64..+63
    int aAtk = 0, aHold = 0, aDec = 64, aRel = 64, aSus = 0;  // страница AMP

    void updateParams()
    {
        using namespace mnm;
        fx.setFiltWords(wordFromKnob(base), wordFromKnob(wdth),
                        wordFromKnob(hpq),  wordFromKnob(lpq),
                        wordFromKnob(fAtk), wordFromKnob(fDec),
                        wordFromKnob(bofs), wordFromKnob(wofs));
        fx.setDistKnob(dist);
        fx.setAmpWords(wordFromKnob(aAtk), wordFromKnob(aHold),
                       wordFromKnob(aDec), wordFromKnob(aRel),
                       wordFromKnob(aSus));
    }

    void prepare(double sampleRate) { fx.setSampleRate(sampleRate); fx.reset(); }
    void noteOn()  { updateParams(); fx.trigger(); }
    void noteOff() { fx.release(); }
};

class MnmFilterDistVoice   // вложить в AudioProcessor или в голос синта
{
public:
    void prepare(double sampleRate) { track.prepare(sampleRate); }

    // FILTER-trig (например, из секвенсора/MIDI как на машине)
    void filterTrig() { track.fx.trigger(); }

    void process(juce::AudioBuffer<float>& buffer)
    {
        auto* L = buffer.getWritePointer(0);
        auto* R = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;
        const int n = buffer.getNumSamples();
        for (int i = 0; i < n; ++i)
        {
            // ядро принимает по сэмплу и само буферизует кадры по 16;
            // возврат = сэмпл предыдущего кадра (латентность 16 сэмплов)
            const float outL = track.fx.processL(L[i]);
            const float outR = R ? track.fx.processR(R[i]) : outL;
            L[i] = outL;
            if (R) R[i] = outR;
        }
    }

    MnmTrackFx track;
};

/* ----------------------------------------------------------------------------
Пример минимального AudioProcessor::processBlock:

void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override
{
    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())  voice.track.noteOn();
        if (msg.isNoteOff()) voice.track.noteOff();
        // FILTER-trig: на машинеFLT-огибающая триггерится отдельным триггером;
        // простейший вариант — триггерить вместе с нотой:
        if (msg.isNoteOn()) voice.filterTrig();
    }
    voice.process(buffer);
}

Пример привязки автоматизации (ValueTree/parameters):
    attach: base  -> track.base  (0..127)
            wdth  -> track.wdth  (0..127)
            hpq   -> track.hpq   (0..127)   // резонанс ВЕРХНЕГО фильтра
            lpq   -> track.lpq   (0..127)   // резонанс НИЖНЕГО фильтра
            atk   -> track.fAtk  (0..127)
            dec   -> track.fDec  (0..127)
            bofs  -> track.bofs  (0..127, 64 = нейтраль)
            wofs  -> track.wofs  (0..127, 64 = нейтраль)
            dist  -> track.dist  (-64..+63)
---------------------------------------------------------------------------- */
