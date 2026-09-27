#pragma once
//==============================================================================
//  PhaserFX.h — ФАЗЕР: 6 каскадов TPT all-pass 1-го порядка (float / double).
//
//  ЭТО ТВОЙ СОБСТВЕННЫЙ КОД. Источники данных (только номера строк):
//    [P.h:NN]    modules/juce_dsp/widgets/juce_Phaser.h
//    [P.cpp:NN]  modules/juce_dsp/widgets/juce_Phaser.cpp
//    [FT.cpp:NN] modules/juce_dsp/processors/juce_FirstOrderTPTFilter.cpp
//    [MF.h:NN]   modules/juce_core/maths/juce_MathsFunctions.h
//  (JUCE 8.0.8, коммит d6181bde38d858c283c3b7bf699ce6340c050b5d)
//
//  Ключевой факт: dsp::Phaser построен на 6 каскадах FirstOrderTPTFilter
//  в режиме allpass [P.cpp:45-49], [P.h:39-40], [P.h:209]. Ядро TPT
//  (Topology-Preserving Transform, Zavalishin) — уравнения [FT.cpp:93-95,101].
//==============================================================================

#include <juce_dsp/juce_dsp.h>
#include "FXCommon.h"

template <typename SampleType>
class PhaserFX
{
public:
    PhaserFX() = default;

    //==========================================================================
    //  ПАРАМЕТРЫ — контракт сеттеров dsp::Phaser
    //==========================================================================

    // Rate LFO 0..100 Гц [P.h:57-60], assert [P.cpp:57]
    void setRate (SampleType newRateHz)
    {
        jassert (juce::isPositiveAndBelow (newRateHz, (SampleType) 100.0));
        rateHz = juce::jlimit ((SampleType) 0, (SampleType) 100, newRateHz);
        update();
    }

    // Depth LFO 0..1 [P.h:62-65], assert [P.cpp:66].
    // В JUCE: oscVolume.setTargetValue (depth * 0.5) [P.cpp:149]
    void setDepth (SampleType newDepth)
    {
        jassert (juce::isPositiveAndNotGreaterThan (newDepth, (SampleType) 1.0));
        depth = juce::jlimit ((SampleType) 0, (SampleType) 1, newDepth);
        update();
    }

    // Центральная частота, Гц. Ограничение: ниже Nyquist (sr * 0.5) [P.cpp:75];
    // рабочий диапазон карты: 20 Гц .. min (20000, 0.49*sr) [P.cpp:78], [P.h:140-141].
    // Дефолт 1300 Гц [P.h:208]
    void setCentreFrequency (SampleType newCentreHz)
    {
        jassert (juce::isPositiveAndBelow (newCentreHz, (SampleType) (sampleRate * 0.5)));
        centreHz = newCentreHz;

        // [P.cpp:78]:
        //   normCentreFrequency = mapFromLog10 (centreFrequency, 20, min (20000, 0.49*sr))
        normCentre = juce::mapFromLog10 (centreHz,
                                         (SampleType) 20,
                                         (SampleType) juce::jmin (20000.0, 0.49 * sampleRate));
    }

    // Feedback -1..1 [P.h:71-74], assert [P.cpp:84]
    void setFeedback (SampleType newFeedback)
    {
        feedback = juce::jlimit ((SampleType) -1, (SampleType) 1, newFeedback);
        for (auto& fb : feedbackSmoothers)
            fb.setTargetValue (feedback);                             // [P.cpp:152-153]
    }

    // Mix 0..1 [P.h:76-79], assert [P.cpp:93]; правило linear [P.cpp:51]
    void setMix (SampleType newMix)
    {
        mix = juce::jlimit ((SampleType) 0, (SampleType) 1, newMix);
    }

    //==========================================================================
    //  ПОДГОТОВКА
    //==========================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;

        // Состояние TPT-интеграторов: по одному s на КАЖДЫЙ каскад и канал
        // (в JUCE это s1 внутри каждого FirstOrderTPTFilter, [FT.cpp:84], [FT.cpp:91])
        apState.assign (spec.numChannels, {});
        for (auto& ch : apState)
            ch.fill ((SampleType) 0);

        lastOut.assign (spec.numChannels, (SampleType) 0);            // [P.h:199]

        // LFO-осциллятор фазера тикает в 4 раза реже: sr / maxUpdateCounter
        // [P.cpp:115-119] (specDown.sampleRate /= maxUpdateCounter)
        osc.reset();
        osc.setFrequency (rateHz, sampleRate, (double) maxUpdateCounter);

        // Сглаживание: oscVolume — с частотой sr/4 [P.cpp:137],
        // feedback — с полной частотой [P.cpp:139-140]
        oscVolumeSmoother.reset (sampleRate / (double) maxUpdateCounter, 0.05);
        feedbackSmoothers.assign (spec.numChannels, {});
        for (auto& fb : feedbackSmoothers)
            fb.reset (sampleRate, 0.05);

        setCentreFrequency (centreHz);   // пересчитать normCentre [P.cpp:78]
        update();
        updateCutoff (osc.getNextSample());  // стартовый срез (счётчик = 0)
        counter = 0;                                                    // [P.cpp:142]
    }

    void reset()
    {
        for (auto& ch : apState)
            ch.fill ((SampleType) 0);
        std::fill (lastOut.begin(), lastOut.end(), (SampleType) 0);     // [P.cpp:129]
        osc.reset();
        counter = 0;
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

        if (context.isBypassed)                                        // [P.h:102-106]
        {
            out.copyFrom (in);
            return;
        }

        for (size_t ch = 0; ch < nCh; ++ch)
        {
            auto* src = in .getChannelPointer (ch);
            auto* dst = out.getChannelPointer (ch);
            auto  ctr = counter;

            for (size_t i = 0; i < n; ++i)
            {
                // (1) Управление срезом — РАЗ В 4 СЭМПЛА
                //     [P.h:160-170] (counter == 0 -> setCutoffFrequency),
                //     maxUpdateCounter = 4 [P.h:205], [P.cpp:116]
                if (ctr == 0)
                {
                    const auto lfoRaw = osc.getNextSample()
                                      * oscVolumeSmoother.getNextValue();
                    updateCutoff (lfoRaw);
                }

                // (2) ВЫЧИТАЮЩАЯ обратная связь: output = input - lastOutput [P.h:158]
                auto x = src[i] - lastOut[ch];

                // (3) ПРОХОД ЧЕРЕЗ 6 ALL-PASS КАСКАДОВ [P.h:172-173]
                for (int sIdx = 0; sIdx < numStages; ++sIdx)
                {
                    auto& s = apState[ch][(size_t) sIdx];

                    const auto v = G * (x - s);     // [FT.cpp:93]  v = G*(x - s)
                    const auto y = v + s;           // [FT.cpp:94]  y = v + s
                    s = y + v;                      // [FT.cpp:95]  s = y + v  (интегратор)

                    x = (SampleType) 2 * y - x;     // [FT.cpp:101] allpass: 2y - input
                }

                // (4) lastOutput = output * feedback [P.h:176]
                lastOut[ch] = x * feedbackSmoothers[ch].getNextValue();

                // (5) Линейный микс dry/wet (правило linear [P.cpp:51])
                const auto mixv = mix;
                dst[i] = src[i] * ((SampleType) 1 - mixv) + x * mixv;

                ++ctr;
                if (ctr == maxUpdateCounter)     // [P.h:180-181]
                    ctr = 0;
            }
        }

        counter = (counter + (int) (n % (size_t) maxUpdateCounter)) % maxUpdateCounter; // [P.h:186]
    }

private:
    //==========================================================================
    //  Пересчёт LFO -> частота среза -> коэффициент G
    //==========================================================================
    void updateCutoff (SampleType lfoRaw)
    {
        // [P.h:136-138]:
        //   lfo = jlimit (0, 1, sin * oscVolume + normCentreFrequency)
        const auto lfo = juce::jlimit ((SampleType) 0, (SampleType) 1,
                                       lfoRaw + normCentre);

        // [P.h:140-141]:
        //   cutoff = mapToLog10 (lfo, 20, min (20000, 0.49 * sampleRate))
        // mapToLog10 = 10^(v*(log10max-log10min)+log10min) = 20*(hi/20)^lfo
        // (формула: [MF.h:402-411])
        const auto cutoff = juce::mapToLog10 (lfo,
                                              (SampleType) 20,
                                              (SampleType) juce::jmin (20000.0, 0.49 * sampleRate));

        // [FT.cpp:118-121] (update() FirstOrderTPTFilter):
        //   g = tan (pi * cutoffFrequency / sampleRate);
        //   G = g / (1 + g);
        const auto g = std::tan (juce::MathConstants<double>::pi * (double) cutoff / sampleRate);
        G = (SampleType) (g / (1.0 + g));
    }

    // update() из JUCE [P.cpp:146-153]
    void update()
    {
        osc.setFrequency (rateHz, sampleRate, (double) maxUpdateCounter); // [P.cpp:148]
        oscVolumeSmoother.setTargetValue (depth * (SampleType) 0.5);      // [P.cpp:149]
    }

    //==========================================================================
    //  СОСТОЯНИЕ
    //==========================================================================
    static constexpr int numStages = 6;        // [P.h:39-40], [P.h:209]
    static constexpr int maxUpdateCounter = 4; // [P.h:204-205]

    // Дефолты [P.h:207-208]: rate=1, depth=0.5, feedback=0, mix=0.5, centre=1300
    SampleType rateHz = 1.0, depth = 0.5, feedback = 0.0, mix = 0.5, centreHz = 1300.0;
    SampleType normCentre = 0.5;               // [P.h:201]
    SampleType G = 0;                          // [FT.cpp:121]

    double sampleRate = 44100.0;
    int counter = 0;                           // [P.h:204]

    fxc::SinOsc<SampleType> osc;                                        // [P.h:194]
    std::vector<std::array<SampleType, (size_t) numStages>> apState;    // s1-интеграторы [FT.cpp:84]
    std::vector<SampleType> lastOut;                                    // [P.h:199]
    fxc::LinearSmoother<SampleType> oscVolumeSmoother;                  // [P.h:196]
    std::vector<fxc::LinearSmoother<SampleType>> feedbackSmoothers;     // [P.h:197]
};
