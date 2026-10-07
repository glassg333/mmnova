#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace combscanner
{
class CombScannerVariantsDSP
{
public:
    static constexpr int variantCount = 10;

    struct Parameters
    {
        float feedback = 0.99f;
        float damp = 0.90f;
        float phase = 0.75f;
        float diffusion = 0.55f;
        float crossMix = 0.50f;
        float motion = 0.55f;
        float delay1Ms = 115.0f;
        float delay2Ms = 500.0f;
        float scan = 0.0f; // normalized internal value; plugin UI maps 0..2 to 0..1
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
