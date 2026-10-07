#include "combscanner/CombScannerVariantsDSP.h"

#include <algorithm>
#include <cmath>

namespace combscanner
{
namespace
{
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = 2.0f * pi;
constexpr std::array<const char*, CombScannerVariantsDSP::variantCount> names {
    "01 Original Hypothesis", "02 Tight Crossfade", "03 Long Resonator", "04 Scrub Stretch",
    "05 Phase Cloud", "06 Ping Pong", "07 Dark Bloom", "08 Bright Teeth",
    "09 Unstable Edge", "10 Balanced Matrix"
};
}

const char* CombScannerVariantsDSP::getVariantName(int index) noexcept
{
    const auto safeIndex = static_cast<std::size_t>(clamp(static_cast<float>(index), 0.0f,
                                                           static_cast<float>(variantCount - 1)));
    return names[safeIndex];
}

CombScannerVariantsDSP::Config CombScannerVariantsDSP::configFor(int variant) noexcept
{
    switch (variant)
    {
        case 1: return Config { { 0.25f, 0.375f, 0.50f, 0.667f, 0.75f, 1.0f, 1.333f, 1.50f },
            0.58f, 0.38f, 1.00f, 0.025f, 0.32f, 0.70f, 0.15f, 0.90f, 0.84f, false, false };
        case 2: return Config { { 0.50f, 0.75f, 1.0f, 1.25f, 1.50f, 1.875f, 2.25f, 2.75f },
            0.62f, 0.34f, 1.00f, 0.035f, 0.66f, 0.72f, 0.20f, 0.92f, 0.86f, false, false };
        case 3: return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.54f, 0.42f, 0.55f, 0.14f, 0.24f, 0.78f, 0.10f, 1.00f, 0.86f, false, false };
        case 4: return Config { { 0.33f, 0.50f, 0.75f, 0.90f, 1.20f, 1.60f, 2.40f, 3.10f },
            0.55f, 0.40f, 0.80f, 0.20f, 0.48f, 0.92f, 0.35f, 1.05f, 0.88f, false, false };
        case 5: return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.56f, 0.40f, 1.00f, 0.055f, 0.40f, 0.78f, 0.20f, 1.35f, 0.88f, false, true };
        case 6: return Config { { 0.40f, 0.60f, 0.80f, 1.0f, 1.30f, 1.80f, 2.50f, 3.20f },
            0.64f, 0.30f, 1.10f, 0.018f, 0.62f, 0.42f, 0.20f, 0.76f, 0.90f, false, false };
        case 7: return Config { { 0.28f, 0.44f, 0.59f, 0.88f, 1.07f, 1.34f, 2.08f, 3.00f },
            0.48f, 0.47f, 0.92f, 0.065f, 0.10f, 0.88f, 0.30f, 1.12f, 0.84f, false, false };
        case 8: return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.70f, 0.27f, 1.35f, 0.09f, 0.72f, 0.86f, 0.42f, 0.98f, 0.90f, true, false };
        case 9: return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.57f, 0.41f, 0.85f, 0.045f, 0.36f, 0.78f, 0.65f, 1.0f, 0.88f, false, false };
        default: return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
            0.55f, 0.435f, 1.00f, 0.045f, 0.12f, 0.80f, 0.0f, 0.86f, 0.86f, false, false };
    }
}

void CombScannerVariantsDSP::DelayLine::prepare(double sampleRate, double maximumDelayMs)
{
    const auto size = static_cast<std::size_t>(std::ceil(std::max(1.0, sampleRate)
                                                         * maximumDelayMs / 1000.0)) + 4u;
    buffer.assign(std::max<std::size_t>(size, 8u), 0.0f);
    writeIndex = 0;
}

void CombScannerVariantsDSP::DelayLine::clear() noexcept
{
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
}

float CombScannerVariantsDSP::DelayLine::read(float delaySamples) const noexcept
{
    if (buffer.empty())
        return 0.0f;
    delaySamples = std::clamp(delaySamples, 1.0f, static_cast<float>(buffer.size() - 2u));
    double position = static_cast<double>(writeIndex) - delaySamples;
    while (position < 0.0)
        position += static_cast<double>(buffer.size());
    const auto index0 = static_cast<std::size_t>(position) % buffer.size();
    const auto index1 = (index0 + 1u) % buffer.size();
    const float fraction = static_cast<float>(position - std::floor(position));
    return buffer[index0] + fraction * (buffer[index1] - buffer[index0]);
}

void CombScannerVariantsDSP::DelayLine::write(float value) noexcept
{
    if (! buffer.empty())
    {
        buffer[writeIndex] = value;
        writeIndex = (writeIndex + 1u) % buffer.size();
    }
}

float CombScannerVariantsDSP::clamp(float value, float low, float high) noexcept
{
    return std::clamp(value, low, high);
}

float CombScannerVariantsDSP::dcBlock(Voice& voice, float input) noexcept
{
    const float output = input - voice.dcInput + 0.995f * voice.dcOutput;
    voice.dcInput = input;
    voice.dcOutput = output;
    return output;
}

void CombScannerVariantsDSP::prepare(double newSampleRate, int newMaximumBlockSize)
{
    sampleRate = std::max(1.0, newSampleRate);
    maximumBlockSize = std::max(1, newMaximumBlockSize);
    (void) maximumBlockSize;
    for (auto& voice : voices)
    {
        voice.delay1.prepare(sampleRate, 7000.0);
        voice.delay2.prepare(sampleRate, 7000.0);
    }
    reset();
}

void CombScannerVariantsDSP::reset() noexcept
{
    for (std::size_t i = 0; i < voices.size(); ++i)
    {
        auto& voice = voices[i];
        voice.delay1.clear();
        voice.delay2.clear();
        voice.dampingState = 0.0f;
        voice.allpassState = 0.0f;
        voice.dcInput = 0.0f;
        voice.dcOutput = 0.0f;
        voice.phase = static_cast<float>(i) * 0.73f;
    }
    wetLeft = 0.0f;
    wetRight = 0.0f;
    smoothedScan = 0.0f;
}

void CombScannerVariantsDSP::setParameters(Parameters newParameters)
{
    parameters.gain = clamp(newParameters.gain, 0.0f, 0.999f);
    parameters.damp = clamp(newParameters.damp, 0.0f, 0.999f);
    parameters.phase = clamp(newParameters.phase, 0.0f, 1.0f);
    parameters.delay1Ms = clamp(newParameters.delay1Ms, 0.0f, 2000.0f);
    parameters.delay2Ms = clamp(newParameters.delay2Ms, 0.0f, 2000.0f);
    parameters.scan = clamp(newParameters.scan, 0.0f, 1.0f);
    parameters.variant = static_cast<int>(clamp(static_cast<float>(newParameters.variant), 0.0f,
                                                static_cast<float>(variantCount - 1)));
    config = configFor(parameters.variant);
}

float CombScannerVariantsDSP::processMono(float input) noexcept
{
    const float oneSampleMs = 1000.0f / static_cast<float>(sampleRate);
    const float feedback = config.feedbackFloor + config.feedbackRange * parameters.gain;
    const float phaseAmount = parameters.phase;
    const float delay1 = parameters.delay1Ms;
    const float delay2 = parameters.delay2Ms;
    std::array<float, 8> outputs{};
    float previous = 0.0f;

    for (std::size_t i = 0; i < voices.size(); ++i)
    {
        auto& voice = voices[i];
        voice.phase += (0.012f + 0.028f * phaseAmount) * (1.0f + 0.08f * i)
                       / static_cast<float>(sampleRate);
        if (voice.phase >= twoPi)
            voice.phase -= twoPi;
        const float movement = std::sin(voice.phase) * config.modulationDepth;
        const float ratio = config.ratios[i] * (1.0f + movement);
        const float time1 = std::max(oneSampleMs, delay1 * ratio);
        const float time2 = std::max(oneSampleMs, delay2 * ratio);
        const float tap1 = voice.delay1.read(time1 * static_cast<float>(sampleRate) / 1000.0f);
        const float tap2 = voice.delay2.read(time2 * static_cast<float>(sampleRate) / 1000.0f);
        const float rawComb = 0.5f * (tap1 + tap2);
        const float comb = rawComb * (1.0f - config.crossMix) + previous * config.crossMix;
        const float dampingStep = 0.08f + 0.20f * (1.0f - parameters.damp);
        voice.dampingState += dampingStep * (comb - voice.dampingState);
        const float resonant = 0.70f * comb + 0.30f * voice.dampingState;
        const float coefficient = clamp(config.allpassBase + config.allpassPhaseDepth * phaseAmount
                                           + 0.04f * std::sin(voice.phase * 1.7f), -0.92f, 0.92f);
        const float allpassOutput = -coefficient * resonant + voice.allpassState;
        voice.allpassState = resonant + coefficient * allpassOutput;
        const float excitation = input * 0.42f
            + feedback * (0.72f * resonant + 0.28f * allpassOutput);
        voice.delay1.write(clamp(excitation, -1.5f, 1.5f));
        voice.delay2.write(clamp(input * 0.42f + feedback * (0.52f * resonant
                                                        + 0.48f * allpassOutput)));
        outputs[i] = dcBlock(voice, 0.72f * comb + 0.28f * allpassOutput);
        previous = outputs[i];
    }

    const float scanSmoothing = 1.0f - std::exp(-1.0f / (0.025f * static_cast<float>(sampleRate)));
    smoothedScan += scanSmoothing * (parameters.scan - smoothedScan);
    float position = std::pow(smoothedScan, config.scanCurve) * 7.0f;
    if (config.reverseScan)
        position = (1.0f - smoothedScan) * 7.0f;
    const auto first = static_cast<std::size_t>(std::min(7.0f, position));
    const auto second = (first + 1u) % voices.size();
    const float fraction = position - static_cast<float>(first);
    auto panFor = [this] (std::size_t index) {
        float pan = static_cast<float>(index) / 7.0f;
        if (config.alternatePan && (index % 2u) != 0u)
            pan = 1.0f - pan;
        pan = 0.5f + (pan - 0.5f) * config.panSpread;
        return clamp(pan, 0.0f, 1.0f);
    };
    const float panFirst = panFor(first);
    const float panSecond = panFor(second);
    const float leftFirst = std::cos(panFirst * pi * 0.5f);
    const float rightFirst = std::sin(panFirst * pi * 0.5f);
    const float leftSecond = std::cos(panSecond * pi * 0.5f);
    const float rightSecond = std::sin(panSecond * pi * 0.5f);
    const float firstWeight = std::cos(fraction * pi * 0.5f);
    const float secondWeight = std::sin(fraction * pi * 0.5f);
    wetLeft = config.wet * (firstWeight * outputs[first] * leftFirst
                            + secondWeight * outputs[second] * leftSecond);
    wetRight = config.wet * (firstWeight * outputs[first] * rightFirst
                             + secondWeight * outputs[second] * rightSecond);
    return 0.5f * (wetLeft + wetRight);
}

void CombScannerVariantsDSP::processBlock(float* left, float* right, int numSamples) noexcept
{
    if (left == nullptr || numSamples <= 0)
        return;
    constexpr float dry = 0.12f;
    constexpr float outputGain = 4.5f;
    if (right == nullptr)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            const float input = left[sample];
            left[sample] = std::tanh(outputGain * (dry * input + (1.0f - dry) * processMono(input)));
        }
        return;
    }
    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float inputLeft = left[sample];
        const float inputRight = right[sample];
        processMono(0.5f * (inputLeft + inputRight));
        left[sample] = std::tanh(outputGain * (dry * inputLeft + (1.0f - dry) * wetLeft));
        right[sample] = std::tanh(outputGain * (dry * inputRight + (1.0f - dry) * wetRight));
    }
}

void CombScannerVariantsDSP::processBlock(float* const* channels, int numChannels, int numSamples) noexcept
{
    if (channels != nullptr && numChannels > 0)
        processBlock(channels[0], numChannels > 1 ? channels[1] : nullptr, numSamples);
}
}
