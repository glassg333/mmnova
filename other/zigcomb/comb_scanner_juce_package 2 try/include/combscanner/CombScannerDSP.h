#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace combscanner
{
class CombScannerDSP
{
public:
    struct Parameters
    {
        float gain = 0.99f;
        float damp = 0.90f;
        float phase = 0.75f;
        float delay1Ms = 115.0f;
        float delay2Ms = 500.0f;
        float scan = 0.0f;
    };

    void prepare(double sampleRate, int maximumBlockSize);
    void reset() noexcept;
    void setParameters(Parameters) noexcept;
    void processBlock(float* const* channels, int numChannels, int numSamples) noexcept;
    void processBlock(float* left, float* right, int numSamples) noexcept;

    [[nodiscard]] Parameters getParameters() const noexcept { return parameters; }

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

    static float clamp(float value, float low, float high) noexcept;
    static float dcBlock(Voice&, float input) noexcept;
    float processMono(float input) noexcept;

    double sampleRate = 48000.0;
    int maximumBlockSize = 512;
    Parameters parameters;
    std::array<Voice, 8> voices;
    std::array<float, 8> delayRatios { 0.33f, 0.50f, 0.66f, 1.00f, 1.11f, 1.45f, 2.22f, 3.33f };
    float wetLeft = 0.0f;
    float wetRight = 0.0f;
    float scanPhase = 0.0f;
};
}
