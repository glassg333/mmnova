#pragma once

#include <array>
#include <complex>
#include <cstddef>
#include <limits>
#include <vector>

#include <signalsmith-fft.h>

namespace scintillate
{

struct Parameters
{
    float mix = 0.5f;
    float width = 1.0f;
    float lowCut = 0.0f;
    float highCut = 1.0f;
    float length = 0.5f;
    float tone = 0.8f;
    float rate = 5.0f;
    float decay = 0.0f;
    float density = 1.0f;
    float shimmer = 0.0f;
};

class DSP
{
public:
    static constexpr int fftSize = 512;
    static constexpr int analysisHop = 128;
    static constexpr int maxSparkles = 24;

    void prepare(double newSampleRate, int maximumBlockSize);
    void reset() noexcept;
    void setParameters(Parameters newParameters) noexcept;
    void processBlock(float* const* channels, int numChannels, int numSamples) noexcept;

    // The dry path and the feedback path are rendered in the current block.
    // The FFT is an analysis side-chain, so no host compensation is requested.
    int latencySamples() const noexcept { return 0; }

    static float normalizedToLengthMs(float normalized) noexcept;
    static float normalizedToCutHz(float normalized) noexcept;

private:
    struct DelayLine
    {
        void prepare(std::size_t size);
        void clear() noexcept;
        float read(float samples) const noexcept;
        void write(float value) noexcept;

        std::vector<float> buffer;
        std::size_t writeIndex = 0;
    };

    struct Comb
    {
        DelayLine delay;
        float filterState = 0.0f;
        float delayMs = 0.0f;
    };

    struct Peak
    {
        float frequency = 0.0f;
        float amplitude = 0.0f;
    };

    struct Sparkle
    {
        float phase = 0.0f;
        float lfoPhase = 0.0f;
        float frequency = 0.0f;
        float targetFrequency = 0.0f;
        float amplitude = 0.0f;
        float targetAmplitude = 0.0f;
        float age = 0.0f;
        float pan = 0.5f;
    };

    float processReverb(int channel, float input) noexcept;
    void analyse(float sample) noexcept;
    void updatePeaks() noexcept;
    void renderSparkles(float& left, float& right) noexcept;
    void clearAnalysis() noexcept;

    double sampleRate = 48000.0;
    int maximumBlockSize = 512;
    Parameters parameters;

    std::array<std::array<Comb, 4>, 2> combs;
    std::array<float, 2> reverbFilter{};
    std::array<float, 2> previousInput{};

    std::vector<float> analysisBuffer;
    std::vector<float> fftTime;
    std::vector<std::complex<float>> fftSpectrum;
    signalsmith::fft::RealFFT<float> fft { static_cast<std::size_t>(fftSize) };
    std::array<Peak, fftSize / 2> detectedPeaks{};
    int detectedPeakCount = 0;
    std::array<Sparkle, maxSparkles> sparkles{};
    std::size_t analysisWriteIndex = 0;
    int samplesSinceAnalysis = analysisHop;
};

} // namespace scintillate
