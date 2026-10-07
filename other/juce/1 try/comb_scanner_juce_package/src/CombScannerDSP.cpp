#include "combscanner/CombScannerDSP.h"

#include <algorithm>
#include <cmath>

namespace combscanner
{
namespace
{
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = 2.0f * pi;
}

void CombScannerDSP::DelayLine::prepare(double sampleRate, double maximumDelayMs)
{
    const auto size = static_cast<std::size_t>(std::ceil(std::max(1.0, sampleRate)
                                                         * maximumDelayMs / 1000.0)) + 4u;
    buffer.assign(std::max<std::size_t>(size, 8u), 0.0f);
    writeIndex = 0;
}

void CombScannerDSP::DelayLine::clear() noexcept
{
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
}

float CombScannerDSP::DelayLine::read(float delaySamples) const noexcept
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

void CombScannerDSP::DelayLine::write(float value) noexcept
{
    if (buffer.empty())
        return;
    buffer[writeIndex] = value;
    writeIndex = (writeIndex + 1u) % buffer.size();
}

float CombScannerDSP::clamp(float value, float low, float high) noexcept
{
    return std::clamp(value, low, high);
}

float CombScannerDSP::dcBlock(Voice& voice, float input) noexcept
{
    const float output = input - voice.dcInput + 0.995f * voice.dcOutput;
    voice.dcInput = input;
    voice.dcOutput = output;
    return output;
}

void CombScannerDSP::prepare(double newSampleRate, int newMaximumBlockSize)
{
    sampleRate = std::max(1.0, newSampleRate);
    maximumBlockSize = std::max(1, newMaximumBlockSize);
    (void) maximumBlockSize;

    for (auto& voice : voices)
    {
        // 3.33 * 1500 ms plus interpolation margin covers the largest scanner ratio.
        voice.delay1.prepare(sampleRate, 5200.0);
        voice.delay2.prepare(sampleRate, 5200.0);
    }
    reset();
}

void CombScannerDSP::reset() noexcept
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
    scanPhase = 0.0f;
}

void CombScannerDSP::setParameters(Parameters newParameters) noexcept
{
    parameters.gain = clamp(newParameters.gain, 0.0f, 0.999f);
    parameters.damp = clamp(newParameters.damp, 0.0f, 0.999f);
    parameters.phase = clamp(newParameters.phase, 0.0f, 1.0f);
    parameters.delay1Ms = clamp(newParameters.delay1Ms, 5.0f, 1000.0f);
    parameters.delay2Ms = clamp(newParameters.delay2Ms, 5.0f, 1500.0f);
    parameters.scan = clamp(newParameters.scan, 0.0f, 1.0f);
}

float CombScannerDSP::processMono(float input) noexcept
{
    const float delay1 = parameters.delay1Ms;
    const float delay2 = std::max(parameters.delay2Ms, delay1 + 1.0f);
    const float feedback = 0.55f + 0.435f * parameters.gain;
    const float damping = parameters.damp;
    const float phaseAmount = parameters.phase;
    std::array<float, 8> outputs{};

    scanPhase += (0.02f + 0.10f * phaseAmount) / static_cast<float>(sampleRate);
    if (scanPhase >= 1.0f)
        scanPhase -= 1.0f;

    for (std::size_t i = 0; i < voices.size(); ++i)
    {
        auto& voice = voices[i];
        voice.phase += (0.015f + 0.025f * phaseAmount) * (1.0f + 0.08f * i)
                       / static_cast<float>(sampleRate);
        if (voice.phase >= twoPi)
            voice.phase -= twoPi;

        const float movement = std::sin(voice.phase) * (0.015f + 0.04f * phaseAmount);
        const float ratio = delayRatios[i] * (1.0f + movement);
        const float time1 = std::max(2.0f, delay1 * ratio);
        const float time2 = std::max(time1 + 1.0f, delay2 * ratio);
        const float tap1 = voice.delay1.read(time1 * static_cast<float>(sampleRate) / 1000.0f);
        const float tap2 = voice.delay2.read(time2 * static_cast<float>(sampleRate) / 1000.0f);
        const float comb = 0.5f * (tap1 + tap2);

        voice.dampingState += (1.0f - damping) * (comb - voice.dampingState);
        const float allpassCoefficient = clamp(0.12f + 0.80f * phaseAmount
                                                    + 0.04f * std::sin(voice.phase * 1.7f),
                                                -0.92f, 0.92f);
        const float allpassOutput = -allpassCoefficient * voice.dampingState + voice.allpassState;
        voice.allpassState = voice.dampingState + allpassCoefficient * allpassOutput;

        const float excitation = input * 0.18f
            + feedback * (0.78f * voice.dampingState + 0.22f * allpassOutput);
        voice.delay1.write(excitation);
        voice.delay2.write(input * 0.18f + feedback * (0.52f * voice.dampingState
                                                        + 0.48f * allpassOutput));
        outputs[i] = dcBlock(voice, 0.36f * comb + 0.64f * allpassOutput);
    }

    // The multiplexer crossfades adjacent comb voices and keeps their stereo positions.
    const float position = parameters.scan * static_cast<float>(voices.size() - 1u);
    const auto first = static_cast<std::size_t>(position);
    const auto second = (first + 1u) % voices.size();
    const float fraction = position - static_cast<float>(first);
    const float panFirst = static_cast<float>(first) / static_cast<float>(voices.size() - 1u);
    const float panSecond = static_cast<float>(second) / static_cast<float>(voices.size() - 1u);
    const float leftFirst = std::cos(panFirst * pi * 0.5f);
    const float rightFirst = std::sin(panFirst * pi * 0.5f);
    const float leftSecond = std::cos(panSecond * pi * 0.5f);
    const float rightSecond = std::sin(panSecond * pi * 0.5f);
    wetLeft = 0.86f * ((1.0f - fraction) * outputs[first] * leftFirst
                       + fraction * outputs[second] * leftSecond);
    wetRight = 0.86f * ((1.0f - fraction) * outputs[first] * rightFirst
                        + fraction * outputs[second] * rightSecond);
    return 0.5f * (wetLeft + wetRight);
}

void CombScannerDSP::processBlock(float* left, float* right, int numSamples) noexcept
{
    if (left == nullptr || numSamples <= 0)
        return;

    constexpr float dry = 0.16f;
    constexpr float wet = 0.84f;
    if (right == nullptr)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            const float input = left[sample];
            left[sample] = dry * input + wet * processMono(input);
        }
        return;
    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float inputLeft = left[sample];
        const float inputRight = right[sample];
        processMono(0.5f * (inputLeft + inputRight));
        left[sample] = dry * inputLeft + wet * wetLeft;
        right[sample] = dry * inputRight + wet * wetRight;
    }
}

void CombScannerDSP::processBlock(float* const* channels, int numChannels, int numSamples) noexcept
{
    if (channels == nullptr || numChannels <= 0)
        return;
    processBlock(channels[0], numChannels > 1 ? channels[1] : nullptr, numSamples);
}
}
