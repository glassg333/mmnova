#pragma once
//==============================================================================
//  OtherFX.h — прочие FX (WaveShaper-сатуратор + фидбек-эхо). float/double.
//
//  ЭТО ТВОЙ СОБСТВЕННЫЙ КОД. Что подтверждено строками JUCE 8.0.8
//  (коммит d6181bde38d858c283c3b7bf699ce6340c050b5d):
//
//  1) WaveShaper [WS.h:NN] modules/juce_dsp/widgets/juce_WaveShaper.h:
//     - [WS.h:43-46] это ШАБЛОН с типом Function (передаточная функция
//       задаётся ПОЛЬЗОВАТЕЛЕМ — std::function/указатель на функцию);
//     - [WS.h:55-58] processSample: return functionToUse (inputSample);
//     - [WS.h:64-68] bypass: копия входа;
//     - [WS.h:71-74] блочный process через AudioBlock::process (functionToUse).
//     => В самом JUCE НЕТ никакой «формулы саturation» — есть только механизм.
//        Конкретная функция (tanh-софтклип) здесь — стандартная инженерная
//        практика, помечена как НЕ из JUCE.
//
//  2) EchoFX — фидбек-эхо. В juce_dsp есть строительный блок dsp::DelayLine
//     (modules/juce_dsp/processors/juce_DelayLine.h), топология же
//     «задержка -> feedback -> mix» собрана здесь явно (стандартная схема).
//==============================================================================

#include <juce_dsp/juce_dsp.h>
#include <cmath>
#include <vector>

//==============================================================================
//  WaveShaperFX — сатуратор/драйв на tanh.
//
//  Контракт повторяет dsp::WaveShaper [WS.h:43-77]: prepare/reset пустые,
//  обработка — применением функции к каждому сэмплу [WS.h:55-58].
//==============================================================================
template <typename SampleType>
class WaveShaperFX
{
public:
    // Drive 0.1..20. В НЕ-JUCE формуле: y = tanh (k * x) / tanh (k),
    // при k->0 стремится к линейному усилению, при больших k — жёсткий clip.
    void setDrive (SampleType newDrive)
    {
        drive = juce::jlimit ((SampleType) 0.1, (SampleType) 20.0, newDrive);
    }

    void prepare (const juce::dsp::ProcessSpec&) {}   // [WS.h:50] prepare пустой
    void reset() {}                                   // [WS.h:77] reset пустой

    template <typename ProcessContext>
    void process (const ProcessContext& context)
    {
        const auto& in = context.getInputBlock();
        auto& out      = context.getOutputBlock();
        const auto nCh = out.getNumChannels();
        const auto n   = out.getNumSamples();

        if (context.isBypassed)                       // [WS.h:64-68]
        {
            out.copyFrom (in);
            return;
        }

        const auto norm = (SampleType) 1 / std::tanh (drive); // нормировка на единицу

        for (size_t ch = 0; ch < nCh; ++ch)
        {
            auto* src = in .getChannelPointer (ch);
            auto* dst = out.getChannelPointer (ch);

            for (size_t i = 0; i < n; ++i)
                dst[i] = std::tanh (drive * src[i]) * norm;   // механизм [WS.h:55-58]
        }
    }

private:
    SampleType drive = 1.0;
};

//==============================================================================
//  EchoFX — простое фидбек-эхо (задержка 1..2000 мс, feedback 0..0.95).
//  Топология:  buf = in + delayed * feedback;  out = in*(1-mix) + delayed*mix.
//  (строительный блок в JUCE — dsp::DelayLine; линейная интерполяция — как у
//  ChorusFX::readLinear)
//==============================================================================
template <typename SampleType>
class EchoFX
{
public:
    void setDelayMs (SampleType newDelayMs)   { delayMs = juce::jlimit ((SampleType) 1, (SampleType) 2000, newDelayMs); }
    void setFeedback (SampleType newFeedback) { feedback = juce::jlimit ((SampleType) 0, (SampleType) 0.95, newFeedback); }
    void setMix (SampleType newMix)           { mix = juce::jlimit ((SampleType) 0, (SampleType) 1, newMix); }

    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        // буфер с запасом: 2000 мс + 1 сэмпл
        const auto maxSamples = (size_t) std::ceil (2000.0 * sampleRate / 1000.0);
        buffer.assign (maxSamples + 1, (SampleType) 0);
        writePos = 0;
    }

    void reset() { std::fill (buffer.begin(), buffer.end(), (SampleType) 0); writePos = 0; }

    template <typename ProcessContext>
    void process (const ProcessContext& context)
    {
        const auto& in = context.getInputBlock();
        auto& out      = context.getOutputBlock();
        const auto nCh = out.getNumChannels();
        const auto n   = out.getNumSamples();

        if (context.isBypassed) { out.copyFrom (in); return; }

        const auto bufSize = buffer.size();

        for (size_t ch = 0; ch < nCh; ++ch)
        {
            auto* src = in .getChannelPointer (ch);
            auto* dst = out.getChannelPointer (ch);

            for (size_t i = 0; i < n; ++i)
            {
                // позиция чтения: на delaySmp позади записи (линейная интерполяция)
                const auto delaySmp = delayMs * (SampleType) (sampleRate / 1000.0);
                SampleType readPos  = (SampleType) writePos - delaySmp;
                while (readPos < 0) readPos += (SampleType) bufSize;

                const auto i0   = (size_t) std::floor (readPos) % bufSize;
                const auto i1   = (i0 + 1) % bufSize;
                const auto frac = (SampleType) (readPos - std::floor (readPos));
                const auto delayed = buffer[i0] * ((SampleType) 1 - frac) + buffer[i1] * frac;

                buffer[writePos] = src[i] + delayed * feedback;   // петля обратной связи
                writePos = (writePos + 1) % bufSize;

                dst[i] = src[i] * ((SampleType) 1 - mix) + delayed * mix;
            }
        }
    }

private:
    std::vector<SampleType> buffer;
    size_t writePos = 0;
    double sampleRate = 44100.0;
    SampleType delayMs = 250.0, feedback = 0.35, mix = 0.4;
};
