#pragma once
//==============================================================================
//  FXCommon.h — общие вспомогательные классы для пакета juce_fx_pack.
//
//  Справочник (только для номеров строк, НЕ копируется в код):
//  JUCE 8.0.8, коммит d6181bde38d858c283c3b7bf699ce6340c050b5d
//    [C.h:NN]     modules/juce_dsp/widgets/juce_Chorus.h
//    [C.cpp:NN]   modules/juce_dsp/widgets/juce_Chorus.cpp
//    [P.h:NN]     modules/juce_dsp/widgets/juce_Phaser.h
//    [P.cpp:NN]   modules/juce_dsp/widgets/juce_Phaser.cpp
//    [FT.cpp:NN]  modules/juce_dsp/processors/juce_FirstOrderTPTFilter.cpp
//    [R.h:NN]     modules/juce_audio_basics/utilities/juce_Reverb.h
//    [WS.h:NN]    modules/juce_dsp/widgets/juce_WaveShaper.h
//    [MF.h:NN]    modules/juce_core/maths/juce_MathsFunctions.h
//==============================================================================

#include <juce_core/juce_core.h>
#include <cmath>
#include <vector>

namespace fxc
{

//==============================================================================
//  Линейный сглаживатель параметров — компактный аналог
//  juce::SmoothedValue<SampleType, ValueSmoothingTypes::Linear>.
//
//  В JUCE параметры эффектов сглаживаются с постоянной времени 0.05 c:
//    [C.cpp:126]  oscVolume.reset (sampleRate, 0.05);
//    [C.cpp:128-129]  feedbackVolume: vol.reset (sampleRate, 0.05);
//    [P.cpp:137]  oscVolume.reset (sampleRate / 4, 0.05);
//    [P.cpp:139-140]  feedbackVolume: vol.reset (sampleRate, 0.05);
//    [R.h:121-126]  smoothing-гейны ревера: reset (sampleRate, 0.01);
//==============================================================================
template <typename SampleType>
struct LinearSmoother
{
    void reset (double sampleRate, double rampSeconds)
    {
        increment = rampSeconds > 0.0
            ? (SampleType) (1.0 / (sampleRate * rampSeconds))
            : (SampleType) 1;
    }

    void setTargetValue (SampleType newTarget) { target = newTarget; }

    void setCurrentAndTarget (SampleType newValue) { value = target = newValue; }

    inline SampleType getNextValue() noexcept
    {
        if (value < target)  value = juce::jmin (target, value + increment);
        else                 value = juce::jmax (target, value - increment);
        return value;
    }

    SampleType value = 0, target = 0, increment = (SampleType) 1;
};

//==============================================================================
//  Нормализованный LFO-синус (аналог dsp::Oscillator с sin-функцией):
//    [C.cpp:42-43]  oscFunction = [] (SampleType x) { return std::sin (x); };
//    [C.cpp:135]    osc.setFrequency (rate);
//==============================================================================
template <typename SampleType>
struct SinOsc
{
    void reset() { phase = (SampleType) 0; }

    void setFrequency (double freqHz, double sampleRate, double rateDivisor = 1.0)
    {
        // осциллятор фазера тикает с частотой sr / maxUpdateCounter
        // ([P.cpp:115-119] specDown.sampleRate /= maxUpdateCounter)
        phaseInc = juce::MathConstants<double>::twoPi * freqHz
                 * rateDivisor / sampleRate;
    }

    // [C.h:117] osc.process(...) -> в блоке bufferDelayTimes/Frequency,
    // далее [C.h:118]/[P.h:129] multiplyBy (oscVolume)
    inline SampleType getNextSample() noexcept
    {
        const auto s = (SampleType) std::sin (phase);
        phase += (SampleType) phaseInc;
        if (phase >= (SampleType) juce::MathConstants<double>::twoPi)
            phase -= (SampleType) juce::MathConstants<double>::twoPi;
        return s;
    }

    SampleType phase = 0;
    double phaseInc = 0.0;
};

} // namespace fxc
