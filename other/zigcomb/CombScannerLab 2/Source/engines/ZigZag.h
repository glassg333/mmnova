#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace zzg
{
class ZigZagDSP
{
public:
    struct Parameters
    {
        float decay = 29.74f;       // RNBO range: 0..127
        float damping = 0.10f;      // RNBO range: 0.1..0.9839
        float rotate = 0.25f;       // RNBO range: 0..1
        float fluctuate = 0.0f;     // RNBO range: 0..1
    };

    void prepare(double sampleRate, int maximumBlockSize);
    void reset() noexcept;
    void setParameters(Parameters parameters) noexcept;

    void processBlock(float* const* channels, int numChannels, int numSamples) noexcept;
    void processBlock(float* left, float* right, int numSamples) noexcept;

    [[nodiscard]] Parameters getParameters() const noexcept { return parameters; }
    [[nodiscard]] double getSampleRate() const noexcept { return sampleRate; }

private:
    class DelayLine
    {
    public:
        void prepare(double sampleRate, double maximumDelayMs);
        void clear() noexcept;
        [[nodiscard]] float read(float delaySamples) const noexcept;
        void write(float sample) noexcept;

    private:
        std::vector<float> buffer;
        std::size_t writeIndex = 0;
        double rate = 48000.0;
    };

    struct Allpass
    {
        DelayLine delay;
        float feedback = 0.6f;
        float delaySamples = 1.0f;

        void prepare(double sampleRate, double delayMs, float coefficient);
        void clear() noexcept;
        [[nodiscard]] float process(float input) noexcept;
    };

    static float clampParameter(float value, float low, float high) noexcept;
    static float equalPower(float value) noexcept;
    float processMono(float input) noexcept;
    float nextNoise() noexcept;

    double sampleRate = 48000.0;
    int maximumBlockSize = 512;
    Parameters parameters;
    std::array<DelayLine, 4> delays;
    std::array<Allpass, 4> diffusers;
    std::array<float, 4> dampingState{};
    std::array<float, 4> phase{};
    std::array<float, 4> delayBaseMs { 43.0f, 67.0f, 97.0f, 139.0f };
    float randomState = 0.1234567f;
    float rotatePhase = 0.0f;
    float wetLeft = 0.0f;
    float wetRight = 0.0f;
};
}
