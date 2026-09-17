#pragma once
//==============================================================================
//  ChorusFX.h — ХОРУС: рабочий шаблонный класс для JUCE DSP (float / double).
//
//  ЭТО ТВОЙ СОБСТВЕННЫЙ КОД. Файлы JUCE использовались ТОЛЬКО как справочник:
//  каждая формула/константа/диапазон помечена комментарием с номером строки
//  источника. Сами исходники JUCE в этот файл не копировались — алгоритм
//  переписан с нуля в самодостаточном виде (кольцевой буфер, интерполяция,
//  сглаживание — свои; см. FXCommon.h).
//
//  Источник алгоритма и данных:
//    [C.h:NN]   modules/juce_dsp/widgets/juce_Chorus.h
//    [C.cpp:NN] modules/juce_dsp/widgets/juce_Chorus.cpp
//  (JUCE 8.0.8, коммит d6181bde38d858c283c3b7bf699ce6340c050b5d)
//==============================================================================

#include <juce_dsp/juce_dsp.h>
#include "FXCommon.h"

template <typename SampleType>
class ChorusFX
{
public:
    ChorusFX() = default;

    //==========================================================================
    //  ПАРАМЕТРЫ — те же, что у dsp::Chorus (контракт сеттеров повторён)
    //==========================================================================

    // Rate LFO в Гц. Документация: «rate must be lower than 100 Hz» [C.h:62-64],
    // assert-проверка [C.cpp:51]
    void setRate (SampleType newRateHz)
    {
        jassert (juce::isPositiveAndBelow (newRateHz, (SampleType) 100.0));
        rateHz = juce::jlimit ((SampleType) 0, (SampleType) 100, newRateHz);
        update();
    }

    // Depth LFO 0..1 [C.h:67-69], assert [C.cpp:60].
    // В JUCE глубина превращается в амплитуду модуляции так:
    //   oscVolume.setTargetValue (depth * oscVolumeMultiplier) [C.cpp:136],
    //   oscVolumeMultiplier = 0.5 [C.h:171]
    void setDepth (SampleType newDepth)
    {
        jassert (juce::isPositiveAndNotGreaterThan (newDepth, maxDepth));
        depth = juce::jlimit ((SampleType) 0, maxDepth, newDepth);
        oscVolumeSmoother.setTargetValue (depth * oscVolumeMultiplier); // [C.cpp:136]
    }

    // Центральная задержка, мс. Диапазон 1..100 мс [C.h:71-74];
    // jlimit (1, maxCentreDelayMs) [C.cpp:71]; maxCentreDelayMs = 100 [C.h:170]
    void setCentreDelay (SampleType newDelayMs)
    {
        centreDelayMs = juce::jlimit ((SampleType) 1, maxCentreDelayMs, newDelayMs);
    }

    // Feedback -1..1 («Negative values can be used to get specific chorus
    // sounds» [C.h:76-79], assert [C.cpp:77])
    void setFeedback (SampleType newFeedback)
    {
        feedback = juce::jlimit ((SampleType) -1, (SampleType) 1, newFeedback);
        for (auto& fb : feedbackSmoothers)
            fb.setTargetValue (feedback);                              // [C.cpp:139-140]
    }

    // Mix 0..1: 0 = сухой, 1 = мокрый [C.h:81-84], assert [C.cpp:86].
    // Правило смешения — DryWetMixingRule::linear [C.cpp:45]
    void setMix (SampleType newMix)
    {
        mix = juce::jlimit ((SampleType) 0, (SampleType) 1, newMix);
        mixSmoother.setTargetValue (mix);                              // [C.cpp:137]
    }

    //==========================================================================
    //  ПОДГОТОВКА
    //==========================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;

        // Размер буфера задержки — как в JUCE [C.cpp:101-103]:
        //   maxPossibleDelay = ceil ((maximumDelayModulation * maxDepth
        //                             * oscVolumeMultiplier + maxCentreDelayMs) * sr / 1000)
        //   = ceil ((20 * 1 * 0.5 + 100) * sr / 1000)  ->  110 мс
        const auto maxPossibleDelayMs = maximumDelayModulation * maxDepth
                                      * oscVolumeMultiplier + maxCentreDelayMs;
        const auto maxSamples = (size_t) std::ceil (maxPossibleDelayMs * sampleRate / 1000.0);

        channels.assign (spec.numChannels, {});
        for (auto& ch : channels)
        {
            ch.buffer.assign (maxSamples + 1, (SampleType) 0);
            ch.writePos = 0;
            ch.lastOut  = 0;
        }

        // Сглаживание 0.05 с — как в JUCE [C.cpp:126, 128-129]
        oscVolumeSmoother.reset (sampleRate, 0.05);
        mixSmoother.reset (sampleRate, 0.05);
        feedbackSmoothers.assign (spec.numChannels, {});
        for (auto& fb : feedbackSmoothers)
            fb.reset (sampleRate, 0.05);

        update();
        reset();
    }

    void reset()
    {
        for (auto& ch : channels)
        {
            std::fill (ch.buffer.begin(), ch.buffer.end(), (SampleType) 0);
            ch.writePos = 0;
            ch.lastOut  = 0;
        }
        osc.reset(); // [C.cpp:123]
        oscVolumeSmoother.setCurrentAndTarget (oscVolumeSmoother.getNextValue());
        mixSmoother.setCurrentAndTarget (mix);
        for (auto& fb : feedbackSmoothers)
            fb.setCurrentAndTarget (feedback);
    }

    //==========================================================================
    //  ОБРАБОТКА
    //==========================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context)
    {
        const auto& in = context.getInputBlock();
        auto& out      = context.getOutputBlock();
        const auto nCh = out.getNumChannels();
        const auto n   = out.getNumSamples();

        if (context.isBypassed)                                        // [C.h:107-111]
        {
            out.copyFrom (in);
            return;
        }

        for (size_t ch = 0; ch < nCh; ++ch)
        {
            auto* src = in .getChannelPointer (ch);
            auto* dst = out.getChannelPointer (ch);
            auto& st  = channels[ch];

            for (size_t i = 0; i < n; ++i)
            {
                // (1) LFO: sin-осциллятор [C.cpp:42-43] с частотой rate [C.cpp:135],
                //     амплитуда = сглаженный oscVolume (= depth * 0.5) [C.h:117-118], [C.cpp:136]
                const auto lfoGain = osc.getNextSample() * oscVolumeSmoother.getNextValue();

                // (2) Задержка в мс: max (1, 20 * lfoGain + centreDelay)  [C.h:124]
                //     перевод в сэмплы: * sr / 1000                        [C.h:125]
                const auto delayMs  = juce::jmax ((SampleType) 1,
                                                  maximumDelayModulation * lfoGain + centreDelayMs);
                const auto delaySmp = delayMs * (SampleType) (sampleRate / 1000.0);

                // (3) ВЫЧИТАЮЩАЯ обратная связь: x = input - lastOutput  [C.h:138]
                //     wet = линия задержки (линейная интерполяция)       [C.h:140-142]
                const auto x   = src[i] - st.lastOut;
                const auto wet = readLinear (st, x, delaySmp);

                // (4) lastOutput = wet * feedback                        [C.h:145]
                st.lastOut = wet * feedbackSmoothers[ch].getNextValue();

                // (5) Линейный микс: out = dry*(1-mix) + wet*mix
                //     (правило linear [C.cpp:45], пропорция wet = mix [C.cpp:137])
                const auto mixv = mixSmoother.getNextValue();
                dst[i] = src[i] * ((SampleType) 1 - mixv) + wet * mixv;
            }
        }
    }

private:
    //==========================================================================
    //  Кольцевой буфер с линейной интерполяцией (замена dsp::DelayLine
    //  с DelayLineInterpolationTypes::Linear [C.h:158], [C.cpp:103])
    //==========================================================================
    struct ChannelState
    {
        std::vector<SampleType> buffer;
        size_t writePos = 0;
        SampleType lastOut = 0;
    };

    static SampleType readLinear (ChannelState& st, SampleType newSample, SampleType delaySmp)
    {
        auto& buf = st.buffer;
        const auto bufSize = buf.size();

        buf[st.writePos] = newSample;               // pushSample [C.h:140]

        SampleType readPos = (SampleType) st.writePos - delaySmp;
        while (readPos < 0)
            readPos += (SampleType) bufSize;

        const auto i0   = (size_t) std::floor (readPos) % bufSize;
        const auto i1   = (i0 + 1) % bufSize;
        const auto frac = (SampleType) (readPos - std::floor (readPos));

        st.writePos = (st.writePos + 1) % bufSize;  // setDelay/popSample [C.h:141-142]

        return buf[i0] * ((SampleType) 1 - frac) + buf[i1] * frac;
    }

    // update() из JUCE [C.cpp:133-141]: частота LFO, амплитуда, mix, feedback
    void update()
    {
        osc.setFrequency (rateHz, sampleRate);      // [C.cpp:135]
        oscVolumeSmoother.setTargetValue (depth * oscVolumeMultiplier); // [C.cpp:136]
        mixSmoother.setTargetValue (mix);           // [C.cpp:137]
        for (auto& fb : feedbackSmoothers)
            fb.setTargetValue (feedback);           // [C.cpp:139-140]
    }

    //==========================================================================
    //  СОСТОЯНИЕ
    //==========================================================================
    // Константы JUCE [C.h:169-172]
    static constexpr SampleType maxDepth               = 1.0;
    static constexpr SampleType maxCentreDelayMs       = 100.0;
    static constexpr SampleType oscVolumeMultiplier    = 0.5;
    static constexpr SampleType maximumDelayModulation = 20.0;

    // Дефолты JUCE [C.h:166-167]: rate=1, depth=0.25, feedback=0, mix=0.5, centre=7мс
    SampleType rateHz = 1.0, depth = 0.25, feedback = 0.0, mix = 0.5, centreDelayMs = 7.0;

    double sampleRate = 44100.0;

    fxc::SinOsc<SampleType> osc;                    // [C.h:157]
    std::vector<ChannelState> channels;
    fxc::LinearSmoother<SampleType> oscVolumeSmoother, mixSmoother;   // [C.h:159,161]
    std::vector<fxc::LinearSmoother<SampleType>> feedbackSmoothers;   // [C.h:160]
};

//==============================================================================
//  Как встроить в цепочку эффектов (ProcessorChain):
//
//    juce::dsp::ProcessorChain<ChorusFX<float>> chain;
//    chain.get<0>().setRate (1.5f);
//    chain.get<0>().setDepth (0.4f);
//    chain.get<0>().setCentreDelay (8.0f);
//    chain.get<0>().setFeedback (0.25f);
//    chain.get<0>().setMix (0.5f);
//    ...
//    juce::dsp::ProcessSpec spec { sampleRate, (uint32) maxBlockSize, (uint32) numCh };
//    chain.prepare (spec);
//    juce::dsp::AudioBlock<float> block (buffer);
//    juce::dsp::ProcessContextReplacing<float> ctx (block);
//    chain.process (ctx);
//==============================================================================
