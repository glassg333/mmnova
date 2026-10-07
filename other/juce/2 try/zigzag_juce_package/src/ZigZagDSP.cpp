#include "zigzag/ZigZagDSP.h"

#include <algorithm>
#include <cmath>

namespace zigzag
{
namespace
{
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = 2.0f * pi;
}

void ZigZagDSP::DelayLine::prepare(double newSampleRate, double maximumDelayMs)
{
    rate = std::max(1.0, newSampleRate);
    const auto samples = static_cast<std::size_t>(std::ceil(rate * maximumDelayMs / 1000.0)) + 4u;
    buffer.assign(std::max<std::size_t>(samples, 8u), 0.0f);
    writeIndex = 0;
}

void ZigZagDSP::DelayLine::clear() noexcept
{
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
}

float ZigZagDSP::DelayLine::read(float delaySamples) const noexcept
{
    if (buffer.empty())
        return 0.0f;

    const float maximum = static_cast<float>(buffer.size() - 2u);
    delaySamples = std::clamp(delaySamples, 1.0f, maximum);

    double position = static_cast<double>(writeIndex) - delaySamples;
    while (position < 0.0)
        position += static_cast<double>(buffer.size());

    const auto index0 = static_cast<std::size_t>(position) % buffer.size();
    const auto index1 = (index0 + 1u) % buffer.size();
    const float fraction = static_cast<float>(position - std::floor(position));
    return buffer[index0] + fraction * (buffer[index1] - buffer[index0]);
}

void ZigZagDSP::DelayLine::write(float sample) noexcept
{
    if (buffer.empty())
        return;

    buffer[writeIndex] = sample;
    writeIndex = (writeIndex + 1u) % buffer.size();
}

void ZigZagDSP::Allpass::prepare(double sampleRate, double delayMs, float coefficient)
{
    delay.prepare(sampleRate, delayMs + 1.0);
    feedback = coefficient;
    delaySamples = static_cast<float>(sampleRate * delayMs / 1000.0);
}

void ZigZagDSP::Allpass::clear() noexcept
{
    delay.clear();
}

float ZigZagDSP::Allpass::process(float input) noexcept
{
    const float delayed = delay.read(delaySamples);
    const float inner = input - feedback * delayed;
    const float output = delayed + feedback * inner;
    delay.write(inner);
    return output;
}

float ZigZagDSP::clampParameter(float value, float low, float high) noexcept
{
    return std::clamp(value, low, high);
}

float ZigZagDSP::equalPower(float value) noexcept
{
    return std::sin(value * pi * 0.5f);
}

void ZigZagDSP::prepare(double newSampleRate, int newMaximumBlockSize)
{
    sampleRate = std::max(1.0, newSampleRate);
    maximumBlockSize = std::max(1, newMaximumBlockSize);
    (void) maximumBlockSize;

    for (auto& delay : delays)
        delay.prepare(sampleRate, 3000.0);

    diffusers[0].prepare(sampleRate, 17.0, 0.58f);
    diffusers[1].prepare(sampleRate, 31.0, 0.63f);
    diffusers[2].prepare(sampleRate, 53.0, 0.69f);
    diffusers[3].prepare(sampleRate, 79.0, 0.74f);
    reset();
}

void ZigZagDSP::reset() noexcept
{
    for (auto& delay : delays)
        delay.clear();
    for (auto& diffuser : diffusers)
        diffuser.clear();

    dampingState.fill(0.0f);
    phase = { 0.0f, 1.7f, 3.1f, 4.8f };
    randomState = 0.1234567f;
    rotatePhase = 0.0f;
    wetLeft = 0.0f;
    wetRight = 0.0f;
}

void ZigZagDSP::setParameters(Parameters newParameters) noexcept
{
    parameters.decay = clampParameter(newParameters.decay, 0.0f, 127.0f);
    parameters.damping = clampParameter(newParameters.damping, 0.1f, 0.9839f);
    parameters.rotate = clampParameter(newParameters.rotate, 0.0f, 1.0f);
    parameters.fluctuate = clampParameter(newParameters.fluctuate, 0.0f, 1.0f);
}

float ZigZagDSP::nextNoise() noexcept
{
    randomState = std::fmod(randomState * 3.9898f + 0.2113f, 1.0f);
    if (randomState < 0.0f)
        randomState += 1.0f;
    return randomState * 2.0f - 1.0f;
}

float ZigZagDSP::processMono(float input) noexcept
{
    const float decayNorm = parameters.decay / 127.0f;
    const float rt60 = 0.25f + 8.0f * std::pow(decayNorm, 0.78f);
    const float damping = parameters.damping;
    const float fluctuation = parameters.fluctuate;
    const float rotation = parameters.rotate;
    std::array<float, 4> delayed{};

    rotatePhase += (0.0025f + 0.015f * rotation) / static_cast<float>(sampleRate);
    if (rotatePhase >= 1.0f)
        rotatePhase -= 1.0f;

    for (std::size_t i = 0; i < delayed.size(); ++i)
    {
        phase[i] += (0.017f + 0.003f * static_cast<float>(i))
                    * (0.35f + rotation) / static_cast<float>(sampleRate);
        if (phase[i] >= twoPi)
            phase[i] -= twoPi;

        const float animated = std::sin(phase[i]);
        const float randomDrift = nextNoise() * fluctuation;
        const float delayMs = std::clamp(
            delayBaseMs[i] * (0.78f + 0.22f * animated)
                + randomDrift * (8.0f + 18.0f * rotation),
            2.0f,
            2990.0f);

        delayed[i] = delays[i].read(delayMs * static_cast<float>(sampleRate) / 1000.0f);
        dampingState[i] += (1.0f - damping) * (delayed[i] - dampingState[i]);
        delayed[i] = dampingState[i];
    }

    // Four-channel Hadamard feedback, followed by two rotating pairs.
    const float h0 = 0.5f * (delayed[0] + delayed[1] + delayed[2] + delayed[3]);
    const float h1 = 0.5f * (delayed[0] - delayed[1] + delayed[2] - delayed[3]);
    const float h2 = 0.5f * (delayed[0] + delayed[1] - delayed[2] - delayed[3]);
    const float h3 = 0.5f * (delayed[0] - delayed[1] - delayed[2] + delayed[3]);
    const float angle = rotation * pi * 0.5f + rotatePhase * twoPi * 0.07f;
    const float sine = std::sin(angle);
    const float cosine = std::cos(angle);
    const std::array<float, 4> mixed {
        h0 * cosine - h1 * sine,
        h0 * sine + h1 * cosine,
        h2 * cosine - h3 * sine,
        h2 * sine + h3 * cosine
    };

    std::array<float, 4> feedback{};
    for (std::size_t i = 0; i < feedback.size(); ++i)
    {
        const float delaySeconds = delayBaseMs[i] / 1000.0f;
        const float loopGain = std::pow(10.0f, -3.0f * delaySeconds / rt60);
        const float weight = 0.88f + 0.12f * std::sin(phase[i] * (1.0f + 0.17f * i));
        feedback[i] = mixed[i] * loopGain * weight;
        delays[i].write(input * 0.52f + feedback[i]);
    }

    const std::array<float, 4> diffused {
        diffusers[0].process(delayed[0]),
        diffusers[1].process(delayed[1]),
        diffusers[2].process(delayed[2]),
        diffusers[3].process(delayed[3])
    };

    wetLeft = 0.5f * (diffused[0] + diffused[2]);
    wetRight = 0.5f * (diffused[1] + diffused[3]);
    return 0.5f * (wetLeft + wetRight);
}

void ZigZagDSP::processBlock(float* left, float* right, int numSamples) noexcept
{
    if (left == nullptr || numSamples <= 0)
        return;

    constexpr float dry = 0.18f;
    constexpr float wet = 0.82f;
    if (right == nullptr)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            const float input = left[sample];
            const float reverb = processMono(input);
            left[sample] = dry * input + wet * reverb;
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

void ZigZagDSP::processBlock(float* const* channels, int numChannels, int numSamples) noexcept
{
    if (channels == nullptr || numChannels <= 0)
        return;

    processBlock(channels[0], numChannels > 1 ? channels[1] : nullptr, numSamples);
}
}
