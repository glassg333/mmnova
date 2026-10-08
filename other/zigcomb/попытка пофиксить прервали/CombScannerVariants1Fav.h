#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "../lab/Declick.h"

namespace var1
{
// Базовый комб-сканер (comb_scanner_variants 1 fav, модель 01), сверен с оригинальной
// Max-схемой. Два режима скана:
//   * мультиплексор (по умолчанию) - скан ВЫБИРАЕТ комб с коротким кроссфейдом
//     (как scanning-multiplexer в оригинале), поэтому не звучит как плавный увод времени;
//   * непрерывный свип - старый кроссфейд между соседними комбами.
class CombScannerVariantsDSP
{
public:
    static constexpr int variantCount = 10;
    static constexpr int numVoices = 8;

    struct Parameters
    {
        // --- основные ручки ---
        float gain = 0.99f;       // раскачка банка, при 1.0 = как в оригинале
        float feedback = 0.99f;   // общий фидбек петли (длина хвоста)
        float damp = 0.0f;        // демпфирование в петле (0 = не вмешивается)
        float phase = 0.0f;       // all-pass в петле (0 = не мылит)
        float delay1Ms = 32.6f;
        float delay2Ms = 13.1f;
        float scan = 0.0f;
        int variant = 0;

        // --- продвинутые (секция ниже основных крутилок) ---
        float ratios[numVoices] = { 0.33f, 0.50f, 0.66f, 1.00f, 1.11f, 1.45f, 2.22f, 3.33f };
        float manualRatios = 0.0f;   // 1 = брать ratios выше, 0 = набор из модели
        float scanCurve = 1.0f;
        float modX = 1.0f;           // x к глубине модуляции модели
        float apBaseX = 1.0f;        // x к all-pass base модели
        float apPhaseX = 1.0f;       // x к размаху Phase
        float fbFloorX = 1.0f;       // x к нижнему фидбеку модели
        float fbRangeX = 1.0f;       // x к диапазону фидбека модели
        float wetX = 1.0f;           // x к уровню wet модели
        float dry = 0.12f;           // сухой сигнал на выходе движка
        float outGain = 4.5f;        // усиление перед tanh
        float spread = 0.12f;        // насколько комбы разные по характеру
        float scanFade = 0.02f;      // с, кроссфейд/сглаживание скана
        float scanMode = 1.0f;       // 0 = свип, 1 = мультиплексор
    };

    static const char* getVariantName(int index) noexcept;
    void prepare(double sampleRate, int maximumBlockSize);
    void reset() noexcept;
    void setParameters(Parameters);
    void processBlock(float* const* channels, int numChannels, int numSamples) noexcept;
    void processBlock(float* left, float* right, int numSamples) noexcept;

private:
    class DelayLine
    {
    public:
        void prepare(double sampleRate, double maximumDelayMs);
        void clear() noexcept;
        [[nodiscard]] float read(float delaySamples) const noexcept;
        void write(float value) noexcept;

    private:
        std::vector<float> buffer;
        std::size_t writeIndex = 0;
    };

    struct Voice
    {
        DelayLine delay1;
        DelayLine delay2;
        float dampingState = 0.0f;
        float allpassState = 0.0f;
        float dcInput = 0.0f;
        float dcOutput = 0.0f;
        float phase = 0.0f;
        lab::DeclickTap tap1;   // время прыгает мгновенно, де-клик — кроссфейд ~5 мс
        lab::DeclickTap tap2;
    };

    struct Config
    {
        std::array<float, 8> ratios;
        float feedbackFloor;
        float feedbackRange;
        float scanCurve;
        float modulationDepth;
        float allpassBase;
        float allpassPhaseDepth;
        float crossMix;
        float panSpread;
        float wet;
        bool reverseScan;
        bool alternatePan;
    };

    static Config configFor(int variant) noexcept;
    static float clamp(float value, float low, float high) noexcept;
    static float dcBlock(Voice&, float input) noexcept;
    float processMono(float input) noexcept;
    int muxIndexOf(float scan) const noexcept;

    double sampleRate = 48000.0;
    int maximumBlockSize = 512;
    Parameters parameters;
    Config config = configFor(0);
    std::array<Voice, 8> voices;
    float wetLeft = 0.0f;
    float wetRight = 0.0f;
    float smoothedScan = 0.0f;
    int muxIndex = 0;
    int muxPrevious = 0;
    float muxXf = 1.0f;
};
}
