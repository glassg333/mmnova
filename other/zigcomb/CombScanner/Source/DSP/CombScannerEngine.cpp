#include "CombScannerEngine.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <complex>
#include <cstdint>
#include <limits>
#include <vector>

#include "fft.h" // Signalsmith Audio DSP (MIT; see ThirdParty/SignalsmithDSP/LICENSE.txt)

namespace combscanner
{
namespace
{
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = 2.0f * pi;
constexpr float maxFeedback = 0.985f;
constexpr float maxDelayMs = 8000.0f;
constexpr std::array<float, kNumCombs> classicRatios {
    0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f
};
constexpr std::array<float, 7> syncCyclesPerBeat {
    4.0f, 2.0f, 1.0f, 0.5f, 0.25f, 0.125f, 0.0625f
};

struct ModelConfig
{
    std::array<float, kNumCombs> ratios;
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

ModelConfig configForModel(int model) noexcept
{
    switch (model)
    {
        case 1: return { { 0.25f, 0.375f, 0.50f, 0.667f, 0.75f, 1.0f, 1.333f, 1.50f },
            0.58f, 0.38f, 1.00f, 0.025f, 0.32f, 0.70f, 0.15f, 0.90f, 0.84f, false, false };
        case 2: return { { 0.50f, 0.75f, 1.0f, 1.25f, 1.50f, 1.875f, 2.25f, 2.75f },
            0.62f, 0.34f, 1.00f, 0.035f, 0.66f, 0.72f, 0.20f, 0.92f, 0.86f, false, false };
        case 3: return { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.54f, 0.42f, 0.55f, 0.14f, 0.24f, 0.78f, 0.10f, 1.00f, 0.86f, false, false };
        case 4: return { { 0.33f, 0.50f, 0.75f, 0.90f, 1.20f, 1.60f, 2.40f, 3.10f },
            0.55f, 0.40f, 0.80f, 0.20f, 0.48f, 0.92f, 0.35f, 1.05f, 0.88f, false, false };
        case 5: return { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.56f, 0.40f, 1.00f, 0.055f, 0.40f, 0.78f, 0.20f, 1.35f, 0.88f, false, true };
        case 6: return { { 0.40f, 0.60f, 0.80f, 1.0f, 1.30f, 1.80f, 2.50f, 3.20f },
            0.64f, 0.30f, 1.10f, 0.018f, 0.62f, 0.42f, 0.20f, 0.76f, 0.90f, false, false };
        case 7: return { { 0.28f, 0.44f, 0.59f, 0.88f, 1.07f, 1.34f, 2.08f, 3.00f },
            0.48f, 0.47f, 0.92f, 0.065f, 0.10f, 0.88f, 0.30f, 1.12f, 0.84f, false, false };
        case 8: return { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.70f, 0.27f, 1.35f, 0.09f, 0.72f, 0.86f, 0.42f, 0.98f, 0.90f, true, false };
        case 9: return { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.57f, 0.41f, 0.85f, 0.045f, 0.36f, 0.78f, 0.65f, 1.0f, 0.88f, false, false };
        default: return { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.55f, 0.435f, 1.00f, 0.045f, 0.12f, 0.80f, 0.0f, 0.86f, 0.86f, false, false };
    }
}

float clamp01(float x) noexcept { return std::clamp(x, 0.0f, 1.0f); }
float clampBipolar(float x) noexcept { return std::clamp(x, -1.0f, 1.0f); }
float dbToGain(float db) noexcept { return std::pow(10.0f, db * 0.05f); }
float wrapPhase(float phase) noexcept
{
    while (phase > pi) phase -= twoPi;
    while (phase < -pi) phase += twoPi;
    return phase;
}
float wrap01(float x) noexcept
{
    x -= std::floor(x);
    return x < 0.0f ? x + 1.0f : x;
}
float lerp(float a, float b, float t) noexcept { return a + (b - a) * t; }

std::uint32_t xorshift(std::uint32_t& state) noexcept
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}
float randomBipolar(std::uint32_t& state) noexcept
{
    return static_cast<float>(xorshift(state) & 0x00ffffffu) / 8388607.5f - 1.0f;
}

} // namespace

EngineParameters makeDefaultEngineParameters() noexcept
{
    EngineParameters p;
    for (int i = 0; i < kNumCombs; ++i)
    {
        auto& c = p.combs[static_cast<std::size_t>(i)];
        c.ratio = classicRatios[static_cast<std::size_t>(i)];
        c.ratio2 = classicRatios[static_cast<std::size_t>(i)];
        c.pan = -0.8f + 1.6f * static_cast<float>(i) / static_cast<float>(kNumCombs - 1);
        c.scanWeight = 1.0f;
    }
    for (int i = 0; i < kNumFxSlots; ++i)
    {
        auto& fx = p.fxSlots[static_cast<std::size_t>(i)];
        fx.type = static_cast<int>(FxType::off);
        fx.afterComb = std::min(i + 1, kNumCombs);
        fx.mix = 0.45f;
        fx.size = 0.50f;
        fx.pitch = 0.0f;
        fx.blur = 0.20f;
    }
    for (int i = 0; i < kNumSequences; ++i)
    {
        auto& seq = p.sequences[static_cast<std::size_t>(i)];
        seq.length = kSequenceSteps;
        seq.stepRateHz = 2.0f;
        seq.steps.fill(0.0f);
    }
    // A gentle default route demonstrates the step sequencer without forcing a large sweep.
    p.routes[0] = { 5, 1, 0.12f }; // Sequence 1 -> Scanner position
    return p;
}

struct CombScannerEngine::Impl
{
    struct StereoFrame
    {
        float l = 0.0f;
        float r = 0.0f;
    };

    struct DelayLine
    {
        std::vector<float> samples;
        std::size_t writeIndex = 0;

        void prepare(double sampleRate, double maximumMs)
        {
            const auto count = static_cast<std::size_t>(std::ceil(std::max(1.0, sampleRate) * maximumMs / 1000.0)) + 4u;
            samples.assign(std::max<std::size_t>(count, 16u), 0.0f);
            writeIndex = 0;
        }

        void clear() noexcept
        {
            std::fill(samples.begin(), samples.end(), 0.0f);
            writeIndex = 0;
        }

        float read(float delaySamples) const noexcept
        {
            if (samples.empty())
                return 0.0f;
            delaySamples = std::clamp(delaySamples, 1.0f, static_cast<float>(samples.size() - 2u));
            double position = static_cast<double>(writeIndex) - static_cast<double>(delaySamples);
            if (position < 0.0)
                position += static_cast<double>(samples.size());
            const auto index0 = static_cast<std::size_t>(position) % samples.size();
            const auto index1 = (index0 + 1u) % samples.size();
            const float fraction = static_cast<float>(position - std::floor(position));
            return samples[index0] + fraction * (samples[index1] - samples[index0]);
        }

        void write(float value) noexcept
        {
            if (! samples.empty())
            {
                samples[writeIndex] = value;
                writeIndex = (writeIndex + 1u) % samples.size();
            }
        }
    };

    struct AlignDelay
    {
        static constexpr int capacity = 1024;
        std::array<float, capacity> buffer {};
        int writeIndex = 0;

        void reset() noexcept
        {
            buffer.fill(0.0f);
            writeIndex = 0;
        }

        float process(float input, int delaySamples) noexcept
        {
            buffer[static_cast<std::size_t>(writeIndex)] = input;
            delaySamples = std::clamp(delaySamples, 0, capacity - 2);
            const int readIndex = (writeIndex - delaySamples + capacity) % capacity;
            const float output = buffer[static_cast<std::size_t>(readIndex)];
            writeIndex = (writeIndex + 1) % capacity;
            return output;
        }
    };

    struct CombState
    {
        std::array<DelayLine, 2> delay;
        std::array<float, 2> dampingState {};
        std::array<float, 2> allpassState {};
        float phase = 0.0f;
    };

    struct Grain
    {
        double start = 0.0;
        float phase = 0.0f;
        bool active = false;
    };

    struct GranularChannel
    {
        std::vector<float> buffer;
        std::array<Grain, 2> grains {};
        std::size_t writeIndex = 0;
        int samplesToSpawn = 0;
        int nextGrain = 0;
        std::uint32_t randomState = 0x1234567u;
        int sampleRateInt = 48000;

        void prepare(double sampleRate)
        {
            sampleRateInt = static_cast<int>(std::max(1.0, sampleRate));
            const auto length = static_cast<std::size_t>(sampleRateInt * 1.5) + 8u;
            buffer.assign(std::max<std::size_t>(length, 2048u), 0.0f);
            reset();
        }

        void reset() noexcept
        {
            std::fill(buffer.begin(), buffer.end(), 0.0f);
            grains = {};
            writeIndex = 0;
            samplesToSpawn = 0;
            nextGrain = 0;
            randomState = 0x1234567u;
        }

        float read(double position) const noexcept
        {
            if (buffer.empty())
                return 0.0f;
            const double size = static_cast<double>(buffer.size());
            position = std::fmod(position, size);
            if (position < 0.0)
                position += size;
            const auto index0 = static_cast<std::size_t>(position);
            const auto index1 = (index0 + 1u) % buffer.size();
            const float frac = static_cast<float>(position - std::floor(position));
            return buffer[index0] + frac * (buffer[index1] - buffer[index0]);
        }

        void launchGrain(Grain& grain, int grainSamples, float spray) noexcept
        {
            const float jitter = randomBipolar(randomState) * spray * static_cast<float>(grainSamples) * 0.35f;
            const float preRoll = static_cast<float>(grainSamples) * 2.0f + jitter;
            grain.start = static_cast<double>(writeIndex) - static_cast<double>(preRoll);
            grain.phase = 0.0f;
            grain.active = true;
        }

        float process(float input, float grainSizeMs, float pitchSemitones, float spray, bool freeze,
                      double sampleRate) noexcept
        {
            if (buffer.empty())
                return input;

            if (! freeze)
            {
                buffer[writeIndex] = input;
                writeIndex = (writeIndex + 1u) % buffer.size();
            }

            const int grainSamples = std::clamp(static_cast<int>(grainSizeMs * sampleRate / 1000.0), 32,
                                                std::max(33, sampleRateInt / 2));
            const int spawnPeriod = std::max(16, grainSamples / 2);
            if (samplesToSpawn <= 0)
            {
                launchGrain(grains[static_cast<std::size_t>(nextGrain)], grainSamples, spray);
                nextGrain = 1 - nextGrain;
                samplesToSpawn = spawnPeriod;
            }
            --samplesToSpawn;

            const float rate = std::clamp(std::pow(2.0f, pitchSemitones / 12.0f), 0.5f, 2.0f);
            float sum = 0.0f;
            float windowSum = 0.0f;
            for (auto& grain : grains)
            {
                if (! grain.active)
                    continue;
                const float phase = std::clamp(grain.phase, 0.0f, 1.0f);
                const float window = 0.5f - 0.5f * std::cos(twoPi * phase);
                const double position = grain.start + static_cast<double>(phase * static_cast<float>(grainSamples) * rate);
                sum += read(position) * window;
                windowSum += window;
                grain.phase += rate / static_cast<float>(grainSamples);
                if (grain.phase >= 1.0f)
                    grain.active = false;
            }
            return windowSum > 1.0e-4f ? sum / windowSum : 0.0f;
        }
    };

    struct SpectralChannel
    {
        static constexpr int fftSize = 128;
        static constexpr int hopSize = 32;
        static constexpr int ringSize = 2048;
        using Complex = std::complex<float>;

        signalsmith::fft::FFT<float> fft { static_cast<std::size_t>(fftSize) };
        std::array<Complex, fftSize> spectrum {};
        std::array<Complex, fftSize> transformed {};
        std::array<float, fftSize> inputRing {};
        std::array<float, fftSize> magnitudes {};
        std::array<float, fftSize> heldMagnitudes {};
        std::array<float, fftSize> previousPhase {};
        std::array<float, fftSize> synthPhase {};
        std::array<float, ringSize> outputRing {};
        int inputWrite = 0;
        int outputSamples = 0;
        int sinceHop = 0;
        bool phaseInitialised = false;
        bool freezeWasOn = false;
        std::uint64_t samplesProcessed = 0;

        void reset() noexcept
        {
            inputRing.fill(0.0f);
            magnitudes.fill(0.0f);
            heldMagnitudes.fill(0.0f);
            previousPhase.fill(0.0f);
            synthPhase.fill(0.0f);
            outputRing.fill(0.0f);
            spectrum.fill(Complex {});
            transformed.fill(Complex {});
            inputWrite = 0;
            outputSamples = 0;
            sinceHop = 0;
            phaseInitialised = false;
            freezeWasOn = false;
            samplesProcessed = 0;
        }

        void renderFrame(std::uint64_t outputStart, float warp, float pitch, float blur, bool freeze) noexcept
        {
            for (int i = 0; i < fftSize; ++i)
            {
                const int readIndex = (inputWrite + i) % fftSize;
                const float window = 0.5f - 0.5f * std::cos(twoPi * static_cast<float>(i) / static_cast<float>(fftSize));
                spectrum[static_cast<std::size_t>(i)] = Complex(inputRing[static_cast<std::size_t>(readIndex)] * window, 0.0f);
            }
            fft.fft(spectrum.data(), transformed.data());

            for (int k = 0; k <= fftSize / 2; ++k)
            {
                const auto value = transformed[static_cast<std::size_t>(k)];
                const float phase = std::atan2(value.imag(), value.real());
                magnitudes[static_cast<std::size_t>(k)] = std::abs(value);

                if (! phaseInitialised)
                {
                    previousPhase[static_cast<std::size_t>(k)] = phase;
                    synthPhase[static_cast<std::size_t>(k)] = phase;
                }
                else
                {
                    const float expected = twoPi * static_cast<float>(k * hopSize) / static_cast<float>(fftSize);
                    const float delta = wrapPhase(phase - previousPhase[static_cast<std::size_t>(k)] - expected);
                    const float trueFrequency = twoPi * static_cast<float>(k) / static_cast<float>(fftSize)
                                                + delta / static_cast<float>(hopSize);
                    synthPhase[static_cast<std::size_t>(k)] += trueFrequency * static_cast<float>(hopSize) * warp;
                    previousPhase[static_cast<std::size_t>(k)] = phase;
                }
            }
            phaseInitialised = true;

            if (freeze && ! freezeWasOn)
                for (int k = 0; k <= fftSize / 2; ++k)
                    heldMagnitudes[static_cast<std::size_t>(k)] = magnitudes[static_cast<std::size_t>(k)];
            freezeWasOn = freeze;

            const float pitchRatio = std::clamp(std::pow(2.0f, pitch), 0.5f, 2.0f);
            for (int k = 0; k <= fftSize / 2; ++k)
            {
                const int left = std::max(0, k - 1);
                const int right = std::min(fftSize / 2, k + 1);
                const float sourcePosition = std::clamp(static_cast<float>(k) / pitchRatio, 0.0f,
                                                        static_cast<float>(fftSize / 2));
                const int source0 = static_cast<int>(sourcePosition);
                const int source1 = std::min(fftSize / 2, source0 + 1);
                const float sourceFrac = sourcePosition - static_cast<float>(source0);
                const auto magnitudeAt = [&](int index)
                {
                    return freeze ? heldMagnitudes[static_cast<std::size_t>(index)]
                                  : magnitudes[static_cast<std::size_t>(index)];
                };
                float mag = lerp(magnitudeAt(source0), magnitudeAt(source1), sourceFrac);
                const float localAverage = (magnitudeAt(left) + magnitudeAt(k) + magnitudeAt(right)) / 3.0f;
                mag = lerp(mag, localAverage, clamp01(blur));
                if (freeze && heldMagnitudes[static_cast<std::size_t>(k)] == 0.0f)
                    mag = 0.0f;

                const float phase = synthPhase[static_cast<std::size_t>(k)];
                Complex bin(mag * std::cos(phase), mag * std::sin(phase));
                if (k == 0 || k == fftSize / 2)
                    bin = Complex(bin.real(), 0.0f);
                transformed[static_cast<std::size_t>(k)] = bin;
                if (k > 0 && k < fftSize / 2)
                    transformed[static_cast<std::size_t>(fftSize - k)] = std::conj(bin);
            }

            fft.ifft(transformed.data(), spectrum.data());
            constexpr float inverseScale = 1.0f / static_cast<float>(fftSize);
            constexpr float overlapScale = 2.0f / 3.0f; // Hann^2, 75% overlap.
            for (int i = 0; i < fftSize; ++i)
            {
                const float window = 0.5f - 0.5f * std::cos(twoPi * static_cast<float>(i) / static_cast<float>(fftSize));
                const float sample = spectrum[static_cast<std::size_t>(i)].real() * inverseScale * window * overlapScale;
                const auto ringIndex = static_cast<std::size_t>((outputStart + static_cast<std::uint64_t>(i))
                                                                % static_cast<std::uint64_t>(ringSize));
                outputRing[ringIndex] += sample;
            }
        }

        float process(float input, float warp, float pitch, float blur, bool freeze) noexcept
        {
            const int readIndex = static_cast<int>(samplesProcessed % static_cast<std::uint64_t>(ringSize));
            const float output = outputRing[static_cast<std::size_t>(readIndex)];
            outputRing[static_cast<std::size_t>(readIndex)] = 0.0f;

            inputRing[static_cast<std::size_t>(inputWrite)] = input;
            inputWrite = (inputWrite + 1) % fftSize;
            ++outputSamples;
            ++sinceHop;

            if (outputSamples >= fftSize && sinceHop >= hopSize)
            {
                sinceHop = 0;
                renderFrame(samplesProcessed + 1u, warp, pitch, blur, freeze);
            }
            ++samplesProcessed;
            return output;
        }
    };

    struct FxSlotState
    {
        int lastType = static_cast<int>(FxType::off);
        std::array<float, 2> lowpassState {};
        std::array<float, 2> limiterEnvelope {};
        std::array<GranularChannel, 2> granular;
        std::array<SpectralChannel, 2> spectral;
        std::array<AlignDelay, 2> fftDryDelay;
        std::uint32_t saturatorSeed = 0x87654321u;

        void prepare(double sampleRate)
        {
            for (auto& g : granular)
                g.prepare(sampleRate);
            reset();
        }

        void reset() noexcept
        {
            lowpassState.fill(0.0f);
            limiterEnvelope.fill(0.0f);
            for (auto& g : granular) g.reset();
            for (auto& s : spectral) s.reset();
            for (auto& d : fftDryDelay) d.reset();
            saturatorSeed = 0x87654321u;
        }

        void process(StereoFrame& frame, const FxParameters& p, double sampleRate) noexcept
        {
            const int type = std::clamp(p.type, 0, static_cast<int>(FxType::saturator));
            if (type != lastType)
            {
                reset();
                lastType = type;
            }
            if (type == static_cast<int>(FxType::off) || p.mix <= 0.0001f)
                return;

            const float mix = clamp01(p.mix);
            const std::array<float, 2> in { frame.l, frame.r };
            std::array<float, 2> out = in;
            for (int ch = 0; ch < 2; ++ch)
            {
                const auto index = static_cast<std::size_t>(ch);
                float wet = in[index];
                switch (static_cast<FxType>(type))
                {
                    case FxType::off: break;
                    case FxType::filter:
                    {
                        const float cutoff = 120.0f + clamp01(p.size) * 17880.0f;
                        const float coeff = 1.0f - std::exp(-twoPi * cutoff / static_cast<float>(sampleRate));
                        lowpassState[index] += coeff * (in[index] - lowpassState[index]);
                        const float resonance = clamp01(p.blur) * 0.85f;
                        wet = lowpassState[index] + resonance * (lowpassState[index] - in[index]);
                        break;
                    }
                    case FxType::granular:
                    {
                        const float grainMs = 24.0f + clamp01(p.size) * 240.0f;
                        const float pitchSemitones = clampBipolar(p.pitch) * 12.0f;
                        wet = granular[index].process(in[index], grainMs, pitchSemitones, clamp01(p.blur),
                                                      p.freeze, sampleRate);
                        break;
                    }
                    case FxType::fftStretch:
                    {
                        const float warp = 0.5f + clamp01(p.size);
                        const float pitch = clampBipolar(p.pitch);
                        const float spectralOutput = spectral[index].process(in[index], warp, pitch,
                                                                             clamp01(p.blur), p.freeze);
                        const float dry = fftDryDelay[index].process(in[index], kFftLatencySamples);
                        out[index] = lerp(dry, spectralOutput, mix);
                        break;
                    }
                    case FxType::limiter:
                    {
                        const float threshold = 0.20f + clamp01(p.size) * 0.78f;
                        const float absIn = std::abs(in[index]);
                        const float attack = 1.0f - std::exp(-1.0f / (0.001f * static_cast<float>(sampleRate)));
                        const float release = 1.0f - std::exp(-1.0f / ((0.025f + 0.35f * p.blur)
                                                                    * static_cast<float>(sampleRate)));
                        auto& env = limiterEnvelope[index];
                        env += (absIn > env ? attack : release) * (absIn - env);
                        const float gain = env > threshold ? threshold / std::max(env, 1.0e-6f) : 1.0f;
                        wet = std::tanh(in[index] * gain * (1.0f + 4.0f * clamp01(p.blur)));
                        break;
                    }
                    case FxType::saturator:
                    {
                        const float drive = 1.0f + 15.0f * clamp01(p.size);
                        const float compensation = 1.0f / std::tanh(drive);
                        wet = std::tanh(in[index] * drive) * compensation;
                        const float tone = 0.02f + 0.96f * clamp01(p.blur);
                        lowpassState[index] += tone * (wet - lowpassState[index]);
                        wet = lerp(lowpassState[index], wet, tone);
                        break;
                    }
                }
                if (type != static_cast<int>(FxType::fftStretch))
                    out[index] = lerp(in[index], wet, mix);
            }
            frame.l = out[0];
            frame.r = out[1];
        }
    };

    double sampleRate = 48000.0;
    int maxBlockSize = 512;
    int latencySamples = 0;
    EngineParameters target = makeDefaultEngineParameters();
    EngineParameters current = makeDefaultEngineParameters();
    std::array<CombState, kNumCombs> combStates;
    std::array<FxSlotState, kNumFxSlots> fxStates;
    std::array<std::array<AlignDelay, 2>, kNumCombs> tapAlign;
    std::array<AlignDelay, 2> dryAlign;
    std::array<float, kNumLfos> lfoPhase {};
    std::array<float, kNumLfos> lfoHold {};
    std::array<float, kNumSequences> seqPhase {};
    std::array<int, kNumSequences> seqStep {};
    std::array<int, kNumSequences> seqDirection {};
    std::array<float, kNumSequences> seqSmoothValue {};
    float scanPhase = 0.0f;
    float envelope = 0.0f;
    std::uint32_t randomState = 0x31415926u;
    float smoothingCoefficient = 0.0f;

    void prepare(double newSampleRate, int newMaxBlock)
    {
        sampleRate = std::max(1.0, newSampleRate);
        maxBlockSize = std::max(1, newMaxBlock);
        smoothingCoefficient = 1.0f - std::exp(-1.0f / (0.030f * static_cast<float>(sampleRate)));
        for (auto& state : combStates)
            for (auto& delay : state.delay)
                delay.prepare(sampleRate, maxDelayMs);
        for (auto& fx : fxStates)
            fx.prepare(sampleRate);
        reset();
    }

    void reset() noexcept
    {
        for (auto& state : combStates)
        {
            for (auto& delay : state.delay) delay.clear();
            state.dampingState.fill(0.0f);
            state.allpassState.fill(0.0f);
            state.phase = 0.0f;
        }
        for (auto& fx : fxStates) fx.reset();
        for (auto& channels : tapAlign)
            for (auto& delay : channels) delay.reset();
        for (auto& delay : dryAlign) delay.reset();
        lfoPhase.fill(0.0f);
        lfoHold.fill(0.0f);
        seqPhase.fill(0.0f);
        seqStep.fill(0);
        seqDirection.fill(1);
        seqSmoothValue.fill(0.0f);
        scanPhase = 0.0f;
        envelope = 0.0f;
        randomState = 0x31415926u;
    }

    void updateLatency() noexcept
    {
        int activeFftSlots = 0;
        for (std::size_t i = 0; i < target.fxSlots.size(); ++i)
        {
            const auto& desired = target.fxSlots[i];
            const auto& active = current.fxSlots[i];
            const bool targetNeedsFft = desired.type == static_cast<int>(FxType::fftStretch) && desired.mix > 0.0001f;
            const bool currentNeedsFft = active.type == static_cast<int>(FxType::fftStretch) && active.mix > 0.0001f;
            if (targetNeedsFft || currentNeedsFft)
                ++activeFftSlots;
        }
        latencySamples = activeFftSlots * kFftLatencySamples;
    }

    void smoothContinuousParameters() noexcept
    {
        const float a = smoothingCoefficient;
        auto s = [a](float& currentValue, float targetValue) { currentValue += a * (targetValue - currentValue); };
        s(current.classicFeedback, target.classicFeedback);
        s(current.classicDamp, target.classicDamp);
        s(current.classicPhase, target.classicPhase);
        s(current.classicDelay1Ms, target.classicDelay1Ms);
        s(current.classicDelay2Ms, target.classicDelay2Ms);
        s(current.scan, target.scan);
        s(current.scanWidth, target.scanWidth);
        s(current.scanRateHz, target.scanRateHz);
        s(current.globalDiffusion, target.globalDiffusion);
        s(current.globalCrossMix, target.globalCrossMix);
        s(current.motion, target.motion);
        s(current.dryWet, target.dryWet);
        s(current.outputDb, target.outputDb);
        s(current.stereoWidth, target.stereoWidth);
        s(current.midiWheel, target.midiWheel);
        s(current.midiVelocity, target.midiVelocity);
        for (int i = 0; i < kNumCombs; ++i)
        {
            auto& c = current.combs[static_cast<std::size_t>(i)];
            const auto& t = target.combs[static_cast<std::size_t>(i)];
            s(c.ratio, t.ratio); s(c.ratio2, t.ratio2); s(c.delay1Ms, t.delay1Ms); s(c.delay2Ms, t.delay2Ms);
            s(c.feedback, t.feedback); s(c.damp, t.damp); s(c.phase, t.phase); s(c.diffusion, t.diffusion);
            s(c.crossMix, t.crossMix); s(c.driveDb, t.driveDb); s(c.levelDb, t.levelDb); s(c.pan, t.pan);
            s(c.scanWeight, t.scanWeight);
        }
        for (int i = 0; i < kNumLfos; ++i)
        {
            auto& c = current.lfos[static_cast<std::size_t>(i)];
            const auto& t = target.lfos[static_cast<std::size_t>(i)];
            s(c.rateHz, t.rateHz); s(c.depth, t.depth);
        }
        for (int i = 0; i < kNumSequences; ++i)
        {
            auto& c = current.sequences[static_cast<std::size_t>(i)];
            const auto& t = target.sequences[static_cast<std::size_t>(i)];
            s(c.stepRateHz, t.stepRateHz); s(c.slew, t.slew);
            for (int step = 0; step < kSequenceSteps; ++step)
                s(c.steps[static_cast<std::size_t>(step)], t.steps[static_cast<std::size_t>(step)]);
        }
        for (int i = 0; i < kNumFxSlots; ++i)
        {
            auto& c = current.fxSlots[static_cast<std::size_t>(i)];
            const auto& t = target.fxSlots[static_cast<std::size_t>(i)];
            s(c.mix, t.mix); s(c.size, t.size); s(c.pitch, t.pitch); s(c.blur, t.blur);
        }
        for (int i = 0; i < kNumModRoutes; ++i)
            s(current.routes[static_cast<std::size_t>(i)].amount, target.routes[static_cast<std::size_t>(i)].amount);

        // Discrete settings are switched at block boundaries.
        current.mode = target.mode;
        current.model = target.model;
        current.bypass = target.bypass;
        current.scanShape = target.scanShape;
        for (int i = 0; i < kNumCombs; ++i)
            current.combs[static_cast<std::size_t>(i)].enabled = target.combs[static_cast<std::size_t>(i)].enabled;
        for (int i = 0; i < kNumFxSlots; ++i)
        {
            current.fxSlots[static_cast<std::size_t>(i)].type = target.fxSlots[static_cast<std::size_t>(i)].type;
            current.fxSlots[static_cast<std::size_t>(i)].afterComb = target.fxSlots[static_cast<std::size_t>(i)].afterComb;
            current.fxSlots[static_cast<std::size_t>(i)].freeze = target.fxSlots[static_cast<std::size_t>(i)].freeze;
        }
        for (int i = 0; i < kNumLfos; ++i)
        {
            current.lfos[static_cast<std::size_t>(i)].shape = target.lfos[static_cast<std::size_t>(i)].shape;
            current.lfos[static_cast<std::size_t>(i)].tempoSync = target.lfos[static_cast<std::size_t>(i)].tempoSync;
            current.lfos[static_cast<std::size_t>(i)].division = target.lfos[static_cast<std::size_t>(i)].division;
        }
        for (int i = 0; i < kNumSequences; ++i)
        {
            current.sequences[static_cast<std::size_t>(i)].length = target.sequences[static_cast<std::size_t>(i)].length;
            current.sequences[static_cast<std::size_t>(i)].mode = target.sequences[static_cast<std::size_t>(i)].mode;
            current.sequences[static_cast<std::size_t>(i)].tempoSync = target.sequences[static_cast<std::size_t>(i)].tempoSync;
            current.sequences[static_cast<std::size_t>(i)].division = target.sequences[static_cast<std::size_t>(i)].division;
        }
        for (int i = 0; i < kNumModRoutes; ++i)
        {
            current.routes[static_cast<std::size_t>(i)].source = target.routes[static_cast<std::size_t>(i)].source;
            current.routes[static_cast<std::size_t>(i)].destination = target.routes[static_cast<std::size_t>(i)].destination;
        }
    }

    float renderLfo(int index, double bpm) noexcept
    {
        auto& p = current.lfos[static_cast<std::size_t>(index)];
        const int div = std::clamp(p.division, 0, static_cast<int>(syncCyclesPerBeat.size()) - 1);
        const float hz = p.tempoSync
            ? static_cast<float>(std::max(20.0, bpm) / 60.0) * syncCyclesPerBeat[static_cast<std::size_t>(div)]
            : std::clamp(p.rateHz, 0.01f, 20.0f);
        lfoPhase[static_cast<std::size_t>(index)] += hz / static_cast<float>(sampleRate);
        if (lfoPhase[static_cast<std::size_t>(index)] >= 1.0f)
        {
            lfoPhase[static_cast<std::size_t>(index)] = wrap01(lfoPhase[static_cast<std::size_t>(index)]);
            if (p.shape == static_cast<int>(LfoShape::sampleHold))
                lfoHold[static_cast<std::size_t>(index)] = randomBipolar(randomState);
        }
        const float phase = lfoPhase[static_cast<std::size_t>(index)];
        float value = 0.0f;
        switch (static_cast<LfoShape>(std::clamp(p.shape, 0, static_cast<int>(LfoShape::sampleHold))))
        {
            case LfoShape::sine: value = std::sin(twoPi * phase); break;
            case LfoShape::triangle: value = 1.0f - 4.0f * std::abs(phase - 0.5f); break;
            case LfoShape::saw: value = 2.0f * phase - 1.0f; break;
            case LfoShape::square: value = phase < 0.5f ? 1.0f : -1.0f; break;
            case LfoShape::sampleHold: value = lfoHold[static_cast<std::size_t>(index)]; break;
        }
        return value * clamp01(p.depth);
    }

    float renderSequence(int index, double bpm) noexcept
    {
        auto& p = current.sequences[static_cast<std::size_t>(index)];
        const int length = std::clamp(p.length, 1, kSequenceSteps);
        const int div = std::clamp(p.division, 0, static_cast<int>(syncCyclesPerBeat.size()) - 1);
        const float stepsPerBeat = syncCyclesPerBeat[static_cast<std::size_t>(div)];
        const float stepRate = p.tempoSync
            ? static_cast<float>(std::max(20.0, bpm) / 60.0) * stepsPerBeat
            : std::clamp(p.stepRateHz, 0.01f, 32.0f);
        auto& phase = seqPhase[static_cast<std::size_t>(index)];
        phase += stepRate / static_cast<float>(sampleRate);
        while (phase >= 1.0f)
        {
            phase -= 1.0f;
            auto& step = seqStep[static_cast<std::size_t>(index)];
            auto& direction = seqDirection[static_cast<std::size_t>(index)];
            switch (static_cast<SequenceMode>(std::clamp(p.mode, 0, static_cast<int>(SequenceMode::random))))
            {
                case SequenceMode::forward: step = (step + 1) % length; break;
                case SequenceMode::reverse: step = (step + length - 1) % length; break;
                case SequenceMode::pingPong:
                    if (length > 1 && (step + direction >= length || step + direction < 0))
                        direction = -direction;
                    step = std::clamp(step + direction, 0, length - 1);
                    break;
                case SequenceMode::random:
                    step = static_cast<int>(xorshift(randomState) % static_cast<std::uint32_t>(length));
                    break;
            }
        }
        const int stepIndex = std::clamp(seqStep[static_cast<std::size_t>(index)], 0, length - 1);
        const float targetValue = clampBipolar(p.steps[static_cast<std::size_t>(stepIndex)]);
        const float slew = clamp01(p.slew);
        if (slew <= 0.0001f)
            seqSmoothValue[static_cast<std::size_t>(index)] = targetValue;
        else
        {
            const float timeSeconds = 0.002f + 0.250f * slew;
            const float coeff = 1.0f - std::exp(-1.0f / (timeSeconds * static_cast<float>(sampleRate)));
            seqSmoothValue[static_cast<std::size_t>(index)] += coeff
                * (targetValue - seqSmoothValue[static_cast<std::size_t>(index)]);
        }
        return seqSmoothValue[static_cast<std::size_t>(index)];
    }

    void applyRoute(int destination, float amount, std::array<float, 12>& globals,
                    std::array<std::array<float, 5>, kNumCombs>& combMods) noexcept
    {
        if (destination >= 1 && destination <= 12)
        {
            globals[static_cast<std::size_t>(destination - 1)] += amount;
            return;
        }
        const auto applyBank = [&](int base, int field)
        {
            if (destination >= base && destination < base + kNumCombs)
                combMods[static_cast<std::size_t>(destination - base)][static_cast<std::size_t>(field)] += amount;
        };
        applyBank(kDestinationCombRatioBase, 0);
        applyBank(kDestinationCombFeedbackBase, 1);
        applyBank(kDestinationCombDelay1Base, 2);
        applyBank(kDestinationCombDelay2Base, 3);
        applyBank(kDestinationCombLevelBase, 4);
    }

    StereoFrame processComb(int index, StereoFrame input, const CombParameters& c,
                            float ratioMod, float feedbackMod, float delay1Mod, float delay2Mod,
                            float levelMod, float globalFeedback, float globalDamp, float globalPhase,
                            float globalDiffusion, float globalCross, float motion,
                            const ModelConfig& config, float scanPosition) noexcept
    {
        if (! c.enabled)
            return input;

        auto& state = combStates[static_cast<std::size_t>(index)];
        const bool more = current.mode == static_cast<int>(DspMode::moreControl);
        const float classicRatio = config.ratios[static_cast<std::size_t>(index)];
        const float ratio1 = std::clamp((more ? c.ratio : classicRatio) * std::exp2(ratioMod * 2.0f), 0.125f, 16.0f);
        const float ratio2 = std::clamp((more ? c.ratio2 : classicRatio) * std::exp2(ratioMod * 2.0f), 0.125f, 16.0f);

        float delay1Ms = more ? c.delay1Ms : current.classicDelay1Ms;
        float delay2Ms = more ? c.delay2Ms : current.classicDelay2Ms;
        delay1Ms = std::clamp(delay1Ms * std::exp2(std::clamp(delay1Mod, -2.0f, 2.0f)), 0.0f, 2000.0f);
        delay2Ms = std::clamp(delay2Ms * std::exp2(std::clamp(delay2Mod, -2.0f, 2.0f)), 0.0f, 2000.0f);

        const float feedbackBase = more ? lerp(c.feedback, globalFeedback, 0.22f)
                                        : config.feedbackFloor + config.feedbackRange * globalFeedback;
        const float feedback = std::clamp(feedbackBase + feedbackMod * 0.45f, 0.0f, maxFeedback);
        const float damp = clamp01(more ? lerp(c.damp, globalDamp, 0.15f) : globalDamp);
        const float phaseAmount = clamp01(more ? lerp(c.phase, globalPhase, 0.10f) : globalPhase);
        const float diffusion = clamp01(more ? lerp(c.diffusion, globalDiffusion, 0.10f) : globalDiffusion);
        const float crossMix = clamp01(more ? lerp(c.crossMix, globalCross, 0.15f) : config.crossMix);
        const float driveDb = more ? std::clamp(c.driveDb, 0.0f, 18.0f) : 0.0f;
        const float drive = dbToGain(driveDb);
        const float level = dbToGain(std::clamp((more ? c.levelDb : 0.0f) + levelMod * 18.0f, -36.0f, 12.0f));

        state.phase += (0.05f + 1.5f * phaseAmount + 0.6f * motion) / static_cast<float>(sampleRate);
        state.phase = wrap01(state.phase);
        const float phaseMove = std::sin(twoPi * state.phase) * config.modulationDepth
                                * (0.1f + 1.9f * motion);
        const float d1s = std::max(1.0f, delay1Ms * ratio1 * static_cast<float>(sampleRate) / 1000.0f
                                            * (1.0f + phaseMove * phaseAmount));
        const float d2s = std::max(1.0f, delay2Ms * ratio2 * static_cast<float>(sampleRate) / 1000.0f
                                            * (1.0f + phaseMove * phaseAmount * 0.5f));

        const std::array<float, 2> in { input.l, input.r };
        std::array<float, 2> delayed {};
        std::array<float, 2> filtered {};
        std::array<float, 2> allpass {};
        const float cutoff = std::clamp(18000.0f * std::pow(1.0f - damp, 2.0f) + 120.0f, 80.0f, 18000.0f);
        const float dampingCoeff = std::clamp(1.0f - std::exp(-twoPi * cutoff / static_cast<float>(sampleRate)), 0.0001f, 1.0f);
        for (int ch = 0; ch < 2; ++ch)
        {
            const auto ci = static_cast<std::size_t>(ch);
            const float first = state.delay[ci].read(ch == 0 ? d1s : d1s);
            const float second = state.delay[ci].read(ch == 0 ? d2s : d2s);
            delayed[ci] = 0.5f * (first + second);
            state.dampingState[ci] += dampingCoeff * (delayed[ci] - state.dampingState[ci]);
            const float resonant = 0.70f * delayed[ci] + 0.30f * state.dampingState[ci];
            const float coefficient = std::clamp(config.allpassBase * (0.2f + 0.8f * diffusion)
                                                     + config.allpassPhaseDepth * phaseAmount
                                                     + 0.04f * std::sin(state.phase * twoPi * 1.7f),
                                                 -0.92f, 0.92f);
            allpass[ci] = -coefficient * resonant + state.allpassState[ci];
            state.allpassState[ci] = resonant + coefficient * allpass[ci];
            filtered[ci] = lerp(resonant, allpass[ci], phaseAmount * (0.2f + 0.8f * diffusion));
        }

        const float cross = std::clamp(crossMix, 0.0f, 0.92f);
        const std::array<float, 2> crossFiltered {
            lerp(filtered[0], filtered[1], cross),
            lerp(filtered[1], filtered[0], cross)
        };
        const float feedbackDrive = more ? drive : 1.0f;
        for (int ch = 0; ch < 2; ++ch)
        {
            const auto ci = static_cast<std::size_t>(ch);
            const float excitation = in[ci] * 0.42f * feedbackDrive + feedback * crossFiltered[ci];
            state.delay[ci].write(std::tanh(excitation));
        }

        float pan = more ? std::clamp(c.pan, -1.0f, 1.0f)
                         : (config.alternatePan && (index & 1) ? -0.65f : 0.0f);
        if (! more)
            pan *= config.panSpread;
        const float panLeft = pan > 0.0f ? 1.0f - 0.35f * pan : 1.0f;
        const float panRight = pan < 0.0f ? 1.0f + 0.35f * pan : 1.0f;
        const float wet = config.wet * (more ? 0.62f : 0.70f);
        const float mixL = std::clamp(wet * level, 0.0f, 1.3f);
        const float mixR = mixL;
        (void) scanPosition;
        return {
            in[0] * (1.0f - std::min(0.85f, mixL)) + (0.72f * delayed[0] + 0.28f * allpass[0]) * mixL * panLeft,
            in[1] * (1.0f - std::min(0.85f, mixR)) + (0.72f * delayed[1] + 0.28f * allpass[1]) * mixR * panRight
        };
    }

    float scannerWeight(int index, float position, float width, int shape, float perCombWeight) const noexcept
    {
        const float distance = std::abs(static_cast<float>(index) - position);
        const float radius = 0.16f + clamp01(width) * 3.25f;
        float weight = 0.0f;
        switch (shape)
        {
            case 1: // Cosine window.
                weight = distance >= radius ? 0.0f : 0.5f + 0.5f * std::cos(pi * distance / radius);
                break;
            case 2: // Gaussian window.
                weight = std::exp(-0.5f * distance * distance / (radius * radius));
                break;
            case 3: // Stepped selection with a short edge crossfade.
                weight = distance < 0.55f ? 1.0f : 0.0f;
                break;
            default: // Linear window.
                weight = std::max(0.0f, 1.0f - distance / radius);
                break;
        }
        return weight * clamp01(perCombWeight);
    }

    void processBlock(float* left, float* right, int numSamples, double bpm) noexcept
    {
        if (left == nullptr || numSamples <= 0)
            return;
        bpm = std::clamp(bpm, 20.0, 300.0);
        const bool mono = right == nullptr;

        for (int sample = 0; sample < numSamples; ++sample)
        {
            smoothContinuousParameters();
            const float inL = left[sample];
            const float inR = mono ? inL : right[sample];
            const float inputLevel = 0.5f * (std::abs(inL) + std::abs(inR));
            const float envCoeff = 1.0f - std::exp(-1.0f / ((inputLevel > envelope ? 0.005f : 0.150f)
                                                          * static_cast<float>(sampleRate)));
            envelope += envCoeff * (inputLevel - envelope);

            std::array<float, kModSourceCount> sourceValues {};
            for (int i = 0; i < kNumLfos; ++i)
                sourceValues[static_cast<std::size_t>(1 + i)] = renderLfo(i, bpm);
            for (int i = 0; i < kNumSequences; ++i)
                sourceValues[static_cast<std::size_t>(5 + i)] = renderSequence(i, bpm);
            sourceValues[9] = envelope;
            sourceValues[10] = clampBipolar(current.midiWheel * 2.0f - 1.0f);
            sourceValues[11] = clampBipolar(current.midiVelocity * 2.0f - 1.0f);

            std::array<float, 12> globalMods {};
            std::array<std::array<float, 5>, kNumCombs> combMods {};
            for (const auto& route : current.routes)
            {
                const int src = std::clamp(route.source, 0, kModSourceCount - 1);
                const int dst = std::clamp(route.destination, 0, kModDestinationCount - 1);
                if (src == 0 || dst == 0)
                    continue;
                applyRoute(dst, sourceValues[static_cast<std::size_t>(src)] * route.amount, globalMods, combMods);
            }

            const auto config = configForModel(current.model);
            const float scanRate = std::clamp(current.scanRateHz * std::exp2(globalMods[10]), 0.0f, 12.0f);
            scanPhase += scanRate / static_cast<float>(sampleRate);
            scanPhase = wrap01(scanPhase);
            float scan = current.scan + globalMods[0] + std::sin(twoPi * scanPhase) * std::min(0.5f, scanRate * 0.05f);
            scan = wrap01(scan);
            if (config.reverseScan)
                scan = 1.0f - scan;
            scan = std::pow(clamp01(scan), std::clamp(config.scanCurve, 0.25f, 2.0f));
            const float scanPosition = scan * static_cast<float>(kNumCombs - 1);
            const float scanWidth = clamp01(current.scanWidth + globalMods[1]);
            const float globalFeedback = clamp01(current.classicFeedback + globalMods[2]);
            const float globalDelay1Mod = std::clamp(globalMods[3], -2.0f, 2.0f);
            const float globalDelay2Mod = std::clamp(globalMods[4], -2.0f, 2.0f);
            const float globalDamp = clamp01(current.classicDamp + globalMods[5] * 0.5f);
            const float globalPhase = clamp01(current.classicPhase + globalMods[6] * 0.5f);
            const float globalDiffusion = clamp01(current.globalDiffusion + globalMods[7] * 0.5f);
            const float globalCross = clamp01(current.globalCrossMix + globalMods[8] * 0.5f);
            const float motion = clamp01(current.motion + globalMods[9] * 0.5f);
            const float outputDb = std::clamp(current.outputDb + globalMods[11] * 12.0f, -24.0f, 12.0f);

            float chainScanPosition = scanPosition;
            StereoFrame chain { inL, inR };
            std::array<StereoFrame, kNumCombs> taps {};
            int latencySoFar = 0;

            for (int stage = 0; stage < kNumCombs; ++stage)
            {
                const auto idx = static_cast<std::size_t>(stage);
                const auto& c = current.combs[idx];
                const float combRatioMod = combMods[idx][0];
                const float combFeedbackMod = combMods[idx][1];
                const float delay1Mod = combMods[idx][2] + globalDelay1Mod;
                const float delay2Mod = combMods[idx][3] + globalDelay2Mod;
                const float levelMod = combMods[idx][4];
                chain = processComb(stage, chain, c, combRatioMod, combFeedbackMod, delay1Mod, delay2Mod,
                                    levelMod, globalFeedback, globalDamp, globalPhase, globalDiffusion,
                                    globalCross, motion, config, chainScanPosition);

                for (int slot = 0; slot < kNumFxSlots; ++slot)
                {
                    const auto& fx = current.fxSlots[static_cast<std::size_t>(slot)];
                    if (std::clamp(fx.afterComb, 1, kNumCombs) != stage + 1)
                        continue;
                    fxStates[static_cast<std::size_t>(slot)].process(chain, fx, sampleRate);
                    if (fx.type == static_cast<int>(FxType::fftStretch) && fx.mix > 0.0001f)
                        latencySoFar += kFftLatencySamples;
                }

                taps[idx] = chain;
                const int alignment = std::max(0, latencySamples - latencySoFar);
                taps[idx].l = tapAlign[idx][0].process(taps[idx].l, alignment);
                taps[idx].r = tapAlign[idx][1].process(taps[idx].r, alignment);
            }

            const float dryL = dryAlign[0].process(inL, latencySamples);
            const float dryR = dryAlign[1].process(inR, latencySamples);
            float weightSum = 0.0f;
            std::array<float, kNumCombs> weights {};
            for (int i = 0; i < kNumCombs; ++i)
            {
                weights[static_cast<std::size_t>(i)] = scannerWeight(i, chainScanPosition, scanWidth,
                                                                      current.scanShape,
                                                                      current.combs[static_cast<std::size_t>(i)].scanWeight);
                weightSum += weights[static_cast<std::size_t>(i)];
            }
            if (weightSum < 1.0e-5f)
            {
                weights.fill(0.0f);
                const auto nearest = static_cast<std::size_t>(std::clamp(static_cast<int>(std::round(chainScanPosition)), 0,
                                                                        kNumCombs - 1));
                weights[nearest] = 1.0f;
                weightSum = 1.0f;
            }

            StereoFrame scanned {};
            for (int i = 0; i < kNumCombs; ++i)
            {
                const float w = weights[static_cast<std::size_t>(i)] / weightSum;
                scanned.l += taps[static_cast<std::size_t>(i)].l * w;
                scanned.r += taps[static_cast<std::size_t>(i)].r * w;
            }
            float outL = current.bypass ? dryL : lerp(dryL, scanned.l, clamp01(current.dryWet));
            float outR = current.bypass ? dryR : lerp(dryR, scanned.r, clamp01(current.dryWet));
            const float gain = dbToGain(outputDb);
            outL *= gain;
            outR *= gain;
            // Safety limiter only catches accidental runaway; it is effectively transparent at normal levels.
            outL = 1.25f * std::tanh(outL / 1.25f);
            outR = 1.25f * std::tanh(outR / 1.25f);

            const float width = clamp01(current.stereoWidth);
            const float mid = 0.5f * (outL + outR);
            const float side = 0.5f * (outL - outR) * width;
            left[sample] = mid + side;
            if (! mono)
                right[sample] = mid - side;
        }
    }
};

CombScannerEngine::CombScannerEngine() : impl(std::make_unique<Impl>()) {}
CombScannerEngine::~CombScannerEngine() = default;

void CombScannerEngine::prepare(double sampleRate, int maximumBlockSize)
{
    impl->prepare(sampleRate, maximumBlockSize);
}

void CombScannerEngine::reset() noexcept
{
    impl->reset();
}

void CombScannerEngine::setParameters(const EngineParameters& parameters) noexcept
{
    impl->target = parameters;
    impl->target.mode = std::clamp(parameters.mode, 0, 1);
    impl->target.model = std::clamp(parameters.model, 0, 9);
    impl->updateLatency();
}

void CombScannerEngine::processBlock(float* left, float* right, int numSamples, double tempoBpm) noexcept
{
    impl->processBlock(left, right, numSamples, tempoBpm);
}

int CombScannerEngine::getLatencySamples() const noexcept
{
    return impl->latencySamples;
}

double CombScannerEngine::getTailLengthSeconds() const noexcept
{
    return 8.25;
}

float CombScannerEngine::getEnvelopeFollower() const noexcept
{
    return impl->envelope;
}

} // namespace combscanner
