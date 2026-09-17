#pragma once
//==============================================================================
//  ReverbFX.h — РЕВЕРБ (Freeverb): 8 comb-фильтров параллельно + 4 all-pass
//  последовательно, на канал (стерео со spread). float (как в JUCE).
//
//  ЭТО ТВОЙ СОБСТВЕННЫЙ КОД. Источник данных (только номера строк):
//    [R.h:NN] modules/juce_audio_basics/utilities/juce_Reverb.h
//  (JUCE 8.0.8, коммит d6181bde38d858c283c3b7bf699ce6340c050b5d)
//
//  Факт: juce::dsp::Reverb (modules/juce_dsp/widgets/juce_Reverb.h) — это
//  ТОНКАЯ ОБЁРТКА над juce::Reverb из juce_audio_basics; весь алгоритм живёт
//  в juce_Reverb.h и основан на FreeVerb [R.h:42]:
//      "This is a simple stereo reverb, based on the technique and tunings
//       used in FreeVerb."
//==============================================================================

#include <juce_dsp/juce_dsp.h>
#include "FXCommon.h"

class ReverbFX
{
public:
    //==========================================================================
    //  ПАРАМЕТРЫ — один в один как juce::Reverb::Parameters [R.h:62-71]
    //==========================================================================
    struct Parameters
    {
        float roomSize   = 0.5f;  /**< Room size 0..1: 1.0 — большой зал, 0 — малый [R.h:64] */
        float damping    = 0.5f;  /**< Damping 0..1: 0 — без демпфирования, 1.0 — полное [R.h:65] */
        float wetLevel   = 0.33f; /**< Wet level 0..1 [R.h:66] */
        float dryLevel   = 0.4f;  /**< Dry level 0..1 [R.h:67] */
        float width      = 1.0f;  /**< Width 0..1: 1.0 — очень широко [R.h:68] */
        float freezeMode = 0.0f;  /**< Freeze: <0.5 — normal, >=0.5 — бесконечный
                                       цикл обратной связи (заморозка) [R.h:69-70] */
    };

    void setParameters (const Parameters& newParams)                     // [R.h:81-94]
    {
        const float wetScaleFactor = 3.0f;                               // [R.h:83]
        const float dryScaleFactor = 2.0f;                               // [R.h:84]

        const float wet = newParams.wetLevel * wetScaleFactor;           // [R.h:86]
        dryGain .setTargetValue (newParams.dryLevel * dryScaleFactor);   // [R.h:87]
        wetGain1.setTargetValue (0.5f * wet * (1.0f + newParams.width)); // [R.h:88]
        wetGain2.setTargetValue (0.5f * wet * (1.0f - newParams.width)); // [R.h:89]

        gain = isFrozen (newParams.freezeMode) ? 0.0f : 0.015f;          // [R.h:91]

        parameters = newParams;
        updateDamping();                                                 // [R.h:93]
    }

    const Parameters& getParameters() const noexcept { return parameters; } // [R.h:75]

    //==========================================================================
    //  ПОДГОТОВКА: масштабирование задержек под частоту сэмплирования [R.h:100-127]
    //==========================================================================
    void setSampleRate (const double newSampleRate)
    {
        static const short combTunings[] = { 1116, 1188, 1277, 1356,
                                             1422, 1491, 1557, 1617 };   // сэмплов @44100 [R.h:104]
        static const short allPassTunings[] = { 556, 441, 341, 225 };    // [R.h:105]
        const int stereoSpread = 23;                                     // [R.h:106]
        const int intSampleRate = (int) newSampleRate;

        sampleRate = newSampleRate;

        // Длина каждого буфера = sr * tuning / 44100 [R.h:111, 117];
        // правый канал: tuning + stereoSpread [R.h:112, 118]
        for (int i = 0; i < numCombs; ++i)
        {
            comb[0][i].setSize (juce::jmax (1, (intSampleRate * combTunings[i]) / 44100));
            comb[1][i].setSize (juce::jmax (1, (intSampleRate * (combTunings[i] + stereoSpread)) / 44100));
        }

        for (int i = 0; i < numAllPasses; ++i)
        {
            allPass[0][i].setSize (juce::jmax (1, (intSampleRate * allPassTunings[i]) / 44100));
            allPass[1][i].setSize (juce::jmax (1, (intSampleRate * (allPassTunings[i] + stereoSpread)) / 44100));
        }

        // Сглаживание гейнов 0.01 c [R.h:121-126]
        const double smoothTime = 0.01;
        dampingS .reset (newSampleRate, smoothTime);
        feedbackS.reset (newSampleRate, smoothTime);
        dryGain  .reset (newSampleRate, smoothTime);
        wetGain1 .reset (newSampleRate, smoothTime);
        wetGain2 .reset (newSampleRate, smoothTime);
    }

    void reset()                                                         // [R.h:130-140]
    {
        for (int j = 0; j < numChannels; ++j)
        {
            for (int i = 0; i < numCombs; ++i)     comb[j][i].clear();
            for (int i = 0; i < numAllPasses; ++i) allPass[j][i].clear();
        }
    }

    //==========================================================================
    //  ОБРАБОТКА (публичный интерфейс как в JUCE)
    //==========================================================================
    void processStereo (float* const left, float* const right, const int numSamples) // [R.h:144-178]
    {
        for (int i = 0; i < numSamples; ++i)
        {
            // Вход: моно-сумма, умноженная на фиксированный gain (0.015) [R.h:152]
            const float input = (left[i] + right[i]) * gain;
            float outL = 0, outR = 0;

            const float damp    = dampingS .getNextValue();
            const float feedbck = feedbackS.getNextValue();

            for (int j = 0; j < numCombs; ++j)          // 8 comb ПАРАЛЛЕЛЬНО [R.h:158-162]
            {
                outL += comb[0][j].process (input, damp, feedbck);
                outR += comb[1][j].process (input, damp, feedbck);
            }

            for (int j = 0; j < numAllPasses; ++j)      // 4 all-pass ПОСЛЕДОВАТЕЛЬНО [R.h:164-168]
            {
                outL = allPass[0][j].process (outL);
                outR = allPass[1][j].process (outR);
            }

            const float dry  = dryGain .getNextValue();
            const float wet1 = wetGain1.getNextValue();
            const float wet2 = wetGain2.getNextValue();

            // Кросс-подача между каналами (ширина стерео) [R.h:174-175]:
            //   L = outL*wet1 + outR*wet2 + L*dry
            //   R = outR*wet1 + outL*wet2 + R*dry
            left[i]  = outL * wet1 + outR * wet2 + left[i]  * dry;
            right[i] = outR * wet1 + outL * wet2 + right[i] * dry;
        }
    }

    void processMono (float* const samples, const int numSamples)        // [R.h:181-206]
    {
        for (int i = 0; i < numSamples; ++i)
        {
            const float input = samples[i] * gain;                       // [R.h:188]
            float output = 0;

            const float damp    = dampingS .getNextValue();
            const float feedbck = feedbackS.getNextValue();

            for (int j = 0; j < numCombs; ++j)                           // [R.h:194-195]
                output += comb[0][j].process (input, damp, feedbck);

            for (int j = 0; j < numAllPasses; ++j)                       // [R.h:197-198]
                output = allPass[0][j].process (output);

            const float dry  = dryGain .getNextValue();
            const float wet1 = wetGain1.getNextValue();

            samples[i] = output * wet1 + samples[i] * dry;               // [R.h:203]
        }
    }

    // Совместимость с juce::dsp::ProcessorChain (обёртка как у dsp::Reverb)
    void prepare (const juce::dsp::ProcessSpec& spec) { setSampleRate (spec.sampleRate); reset(); }

    template <typename ProcessContext>
    void process (const ProcessContext& context)
    {
        const auto& in = context.getInputBlock();
        auto& out      = context.getOutputBlock();
        const auto nCh = out.getNumChannels();
        const auto n   = out.getNumSamples();

        if (context.isBypassed) { out.copyFrom (in); return; }

        if (nCh >= 2)
        {
            auto* L = out.getChannelPointer (0);
            auto* R = out.getChannelPointer (1);
            for (size_t i = 0; i < n; ++i)
            {
                L[i] = in.getChannelPointer (0)[i];
                R[i] = in.getChannelPointer (1)[i];
            }
            processStereo (L, R, (int) n);
        }
        else if (nCh == 1)
        {
            auto* M = out.getChannelPointer (0);
            for (size_t i = 0; i < n; ++i) M[i] = in.getChannelPointer (0)[i];
            processMono (M, (int) n);
        }
    }

private:
    //==========================================================================
    //  COMB-ФИЛЬТР (гребенчатый): буфер + демпфер + feedback
    //  Уравнения [R.h:255-266]
    //==========================================================================
    struct Comb
    {
        std::vector<float> buffer;
        int bufferSize = 0, bufferIndex = 0;
        float last = 0.0f;                                               // [R.h:271]

        void setSize (const int size)                                    // [R.h:237-247]
        {
            if (size != bufferSize)
            {
                bufferIndex = 0;
                buffer.assign ((size_t) size, 0.0f);
                bufferSize = size;
            }
            clear();
        }

        void clear() noexcept                                            // [R.h:249-253]
        {
            last = 0;
            std::fill (buffer.begin(), buffer.end(), 0.0f);
        }

        float process (const float input, const float damp, const float feedbackLevel) noexcept
        {
            const float output = buffer[bufferIndex];                    // [R.h:257]
            last = (output * (1.0f - damp)) + (last * damp);             // [R.h:258] демпфер (lowpass)
            const float temp = input + (last * feedbackLevel);           // [R.h:261] feedback в буфер
            buffer[bufferIndex] = temp;                                  // [R.h:263]
            bufferIndex = (bufferIndex + 1) % bufferSize;                // [R.h:264]
            return output;                                               // [R.h:265]
        }
    };

    //==========================================================================
    //  ALL-PASS ФИЛЬТР: уравнения [R.h:299-307]
    //==========================================================================
    struct AllPass
    {
        std::vector<float> buffer;
        int bufferSize = 0, bufferIndex = 0;

        void setSize (const int size)                                    // [R.h:282-292]
        {
            if (size != bufferSize)
            {
                bufferIndex = 0;
                buffer.assign ((size_t) size, 0.0f);
                bufferSize = size;
            }
            clear();
        }

        void clear() noexcept                                            // [R.h:294-297]
        {
            std::fill (buffer.begin(), buffer.end(), 0.0f);
        }

        float process (const float input) noexcept
        {
            const float bufferedValue = buffer[bufferIndex];             // [R.h:301]
            const float temp = input + (bufferedValue * 0.5f);           // [R.h:302]
            buffer[bufferIndex] = temp;                                  // [R.h:304]
            bufferIndex = (bufferIndex + 1) % bufferSize;                // [R.h:305]
            return bufferedValue - input;                                // [R.h:306]
        }
    };

    //==========================================================================
    static bool isFrozen (const float freezeMode) noexcept { return freezeMode >= 0.5f; } // [R.h:210]

    // Отображение параметров 0..1 во внутренние коэффициенты [R.h:212-229]
    void updateDamping() noexcept
    {
        const float roomScaleFactor  = 0.28f;                            // [R.h:214]
        const float roomOffset       = 0.7f;                             // [R.h:215]
        const float dampScaleFactor  = 0.4f;                             // [R.h:216]

        if (isFrozen (parameters.freezeMode))
            setDamping (0.0f, 1.0f);      // заморозка: damping=0, feedback=1 [R.h:218-219]
        else
            setDamping (parameters.damping * dampScaleFactor,             // [R.h:221]
                        parameters.roomSize * roomScaleFactor + roomOffset); // [R.h:222]
    }

    void setDamping (const float dampingToUse, const float roomSizeToUse) noexcept // [R.h:225-229]
    {
        dampingS .setTargetValue (dampingToUse);
        feedbackS.setTargetValue (roomSizeToUse);
    }

    //==========================================================================
    //  СОСТОЯНИЕ
    //==========================================================================
    enum { numCombs = 8, numAllPasses = 4, numChannels = 2 };            // [R.h:317]

    Parameters parameters;
    float gain = 0.015f;                                                 // [R.h:91]

    Comb comb[numChannels][numCombs];                                    // [R.h:322]
    AllPass allPass[numChannels][numAllPasses];                          // [R.h:323]

    fxc::LinearSmoother<float> dampingS, feedbackS, dryGain, wetGain1, wetGain2; // [R.h:325]

    double sampleRate = 44100.0;
};
