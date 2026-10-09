#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "../lab/Declick.h"

namespace csmm
{
class CsMmVariantsDSP
{
public:
    static constexpr int variantCount = 20;

    struct Parameters
    {
        float feedback = 0.62f;
        float damp = 0.62f;
        float phase = 0.35f;
        float diffusion = 0.45f;
        float crossMix = 0.50f;
        float motion = 0.50f;
        float chorusMix = 0.58f;
        float chorusDepth = 0.60f;
        float chorusRate = 0.35f;
        float delay1Ms = 20.0f;   // мелко и металлично, как в оригинале
        float delay2Ms = 45.0f;
        float scan = 0.0f;
        int variant = 0;
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
        float read(float delaySamples) const noexcept;
        void write(float value) noexcept;
    private:
        std::vector<float> buffer;
        std::size_t writeIndex = 0;
    };

    struct ChorusLine
    {
        DelayLine left;
        DelayLine right;
        float phase = 0.0f;
    };

    struct Voice
    {
        DelayLine delay1;
        DelayLine delay2;
        ChorusLine chorus;
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
        float chorusBaseMs;
        float chorusDepthMs;
        float chorusRateHz;
        float chorusMix;
        float chorusSpread;
        bool reverseScan;
        bool alternatePan;
    };

    static Config configFor(int variant) noexcept;
    static float clamp(float value, float low, float high) noexcept;
    static float dcBlock(Voice&, float input) noexcept;
    float processMono(float input) noexcept;

    double sampleRate = 48000.0;
    int maximumBlockSize = 512;
    Parameters parameters;
    Config config = configFor(0);
    std::array<Voice, 8> voices;
    float wetLeft = 0.0f;
    float wetRight = 0.0f;
    float smoothedScan = 0.0f;
    float smoothedMono = 0.0f;
    float smoothedWetLeft = 0.0f;
    float smoothedWetRight = 0.0f;
};
}
