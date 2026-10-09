#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "../lab/Declick.h"

namespace var4
{
// Базовый комб-сканер. Два режима скана:
//   * мультиплексор (по умолчанию) — скан ВЫБИРАЕТ комб с коротким кроссфейдом;
//   * непрерывный свип — кроссфейд между соседними комбами.
class CombScannerVariantsDSP
{
public:
    static constexpr int variantCount = 7;
    static constexpr int numVoices = 8;

    struct Parameters
    {
        float gain = 0.99f;
        float feedback = 0.99f;
        float damp = 0.0f;
        float phase = 0.0f;
        float delay1Ms = 32.6f;
        float delay2Ms = 13.1f;
        float scan = 0.0f;
        int variant = 0;

        float ratios[numVoices] = { 0.33f, 0.50f, 0.66f, 1.00f, 1.11f, 1.45f, 2.22f, 3.33f };
        float manualRatios = 0.0f;
        float scanCurve = 1.0f;
        float modX = 1.0f;
        float apBaseX = 1.0f;
        float apPhaseX = 1.0f;
        float fbFloorX = 1.0f;
        float fbRangeX = 1.0f;
        float wetX = 1.0f;
        float dry = 0.12f;
        float outGain = 4.5f;
        float spread = 0.12f;
        float scanFade = 0.02f;
        float scanMode = 1.0f; // mux как оригинал (раздвоение var3)
        float diffusion = 0.35f; // второй all-pass в петле, |H|=1
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
        float allpassState2 = 0.0f;
        float dcInput = 0.0f;
        float dcOutput = 0.0f;
        float phase = 0.0f;
        lab::DeclickTap tap1;
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
