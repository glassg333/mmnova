#include "scintillate/ScintillateDSP.h"

#include <algorithm>
#include <cmath>
namespace scintillate
{
namespace
{
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = 2.0f * pi;
constexpr float minDelayMs[4] = { 17.1f, 23.7f, 31.3f, 43.9f };

float clamp01(float value) noexcept
{
    return std::clamp(value, 0.0f, 1.0f);
}

float fastNoise(int index) noexcept
{
    const auto value = std::sin(static_cast<float>(index) * 12.9898f) * 43758.5453f;
    return value - std::floor(value);
}
}

void DSP::DelayLine::prepare(std::size_t size)
{
    buffer.assign(std::max<std::size_t>(size, 8u), 0.0f);
    writeIndex = 0;
}

void DSP::DelayLine::clear() noexcept
{
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
}

float DSP::DelayLine::read(float samples) const noexcept
{
    if (buffer.empty())
        return 0.0f;

    samples = std::clamp(samples, 1.0f, static_cast<float>(buffer.size() - 2));
    double position = static_cast<double>(writeIndex) - samples;
    while (position < 0.0)
        position += static_cast<double>(buffer.size());

    const auto index0 = static_cast<std::size_t>(position) % buffer.size();
    const auto index1 = (index0 + 1u) % buffer.size();
    const auto fraction = static_cast<float>(position - std::floor(position));
    return buffer[index0] + fraction * (buffer[index1] - buffer[index0]);
}

void DSP::DelayLine::write(float value) noexcept
{
    if (!buffer.empty())
    {
        buffer[writeIndex] = value;
        writeIndex = (writeIndex + 1u) % buffer.size();
    }
}

float DSP::normalizedToLengthMs(float normalized) noexcept
{
    const auto value = clamp01(normalized);
    if (value <= 0.5f)
        return 5.0f + value * 2.0f * (1000.0f - 5.0f);

    return 1000.0f * std::pow(50.0f, (value - 0.5f) * 2.0f);
}

float DSP::normalizedToCutHz(float normalized) noexcept
{
    const auto value = clamp01(normalized);
    if (value <= 0.5f)
        return 5.0f + value * 2.0f * (1000.0f - 5.0f);

    return 1000.0f * std::pow(50.0f, (value - 0.5f) * 2.0f);
}

void DSP::prepare(double newSampleRate, int newMaximumBlockSize)
{
    sampleRate = std::max(1.0, newSampleRate);
    maximumBlockSize = std::max(1, newMaximumBlockSize);
    (void) maximumBlockSize;

    const auto maxDelay = static_cast<std::size_t>(sampleRate * 0.12) + 4u;
    for (auto& channel : combs)
    {
        for (std::size_t i = 0; i < channel.size(); ++i)
        {
            channel[i].delay.prepare(maxDelay);
            channel[i].delayMs = minDelayMs[i];
        }
    }

    analysisBuffer.assign(fftSize, 0.0f);
    fftTime.assign(fftSize, 0.0f);
    fftSpectrum.assign(fftSize / 2, {});
    reset();
}

void DSP::clearAnalysis() noexcept
{
    std::fill(analysisBuffer.begin(), analysisBuffer.end(), 0.0f);
    std::fill(fftTime.begin(), fftTime.end(), 0.0f);
    std::fill(fftSpectrum.begin(), fftSpectrum.end(), std::complex<float>{});
    analysisWriteIndex = 0;
    samplesSinceAnalysis = analysisHop;
    detectedPeakCount = 0;
}

void DSP::reset() noexcept
{
    for (auto& channel : combs)
    {
        for (auto& comb : channel)
        {
            comb.delay.clear();
            comb.filterState = 0.0f;
        }
    }

    reverbFilter = {};
    previousInput = {};
    clearAnalysis();

    for (std::size_t i = 0; i < sparkles.size(); ++i)
    {
        sparkles[i] = {};
        sparkles[i].pan = 0.5f + 0.5f * std::sin(static_cast<float>(i) * 1.73f);
        sparkles[i].phase = twoPi * fastNoise(static_cast<int>(i) + 3);
        sparkles[i].lfoPhase = twoPi * fastNoise(static_cast<int>(i) + 17);
    }
}

void DSP::setParameters(Parameters newParameters) noexcept
{
    parameters.mix = clamp01(newParameters.mix);
    parameters.width = clamp01(newParameters.width);
    parameters.lowCut = clamp01(newParameters.lowCut);
    parameters.highCut = clamp01(newParameters.highCut);
    parameters.length = clamp01(newParameters.length);
    parameters.tone = clamp01(newParameters.tone);
    parameters.rate = std::clamp(newParameters.rate, 0.1f, 100.0f);
    parameters.decay = std::clamp(newParameters.decay, -1.0f, 1.0f);
    parameters.density = clamp01(newParameters.density);
    parameters.shimmer = clamp01(newParameters.shimmer);
}

float DSP::processReverb(int channel, float input) noexcept
{
    const auto lengthSeconds = std::max(0.005f, normalizedToLengthMs(parameters.length) * 0.001f);
    const auto toneCoefficient = 0.03f + 0.48f * parameters.tone;
    const auto damping = 0.20f + 0.30f * (1.0f - parameters.tone);
    const auto inputDifference = input - previousInput[static_cast<std::size_t>(channel)];
    previousInput[static_cast<std::size_t>(channel)] = input;

    float output = 0.0f;
    std::array<float, 4> taps{};
    for (std::size_t i = 0; i < 4; ++i)
    {
        auto& comb = combs[static_cast<std::size_t>(channel)][i];
        const auto delaySamples = comb.delayMs * static_cast<float>(sampleRate) * 0.001f;
        taps[i] = comb.delay.read(delaySamples);
        comb.filterState += toneCoefficient * (taps[i] - comb.filterState);
        output += comb.filterState * (0.27f - 0.025f * static_cast<float>(i));
    }

    for (std::size_t i = 0; i < 4; ++i)
    {
        auto& comb = combs[static_cast<std::size_t>(channel)][i];
        const auto next = taps[(i + 1u) % 4u];
        const auto feedbackTime = comb.delayMs * 0.001f;
        const auto feedback = std::exp(-6.9078f * feedbackTime / lengthSeconds) * 0.9995f;
        const auto diffused = 0.72f * comb.filterState + 0.28f * next;
        comb.delay.write(0.33f * input + 0.18f * inputDifference + damping * feedback * diffused);
    }

    reverbFilter[static_cast<std::size_t>(channel)] += 0.04f * (output - reverbFilter[static_cast<std::size_t>(channel)]);
    return 0.70f * output + 0.30f * reverbFilter[static_cast<std::size_t>(channel)];
}

void DSP::analyse(float sample) noexcept
{
    if (analysisBuffer.empty())
        return;

    analysisBuffer[analysisWriteIndex] = sample;
    analysisWriteIndex = (analysisWriteIndex + 1u) % analysisBuffer.size();
    if (++samplesSinceAnalysis < analysisHop)
        return;

    samplesSinceAnalysis = 0;
    for (int i = 0; i < fftSize; ++i)
    {
        const auto index = (analysisWriteIndex + static_cast<std::size_t>(i)) % analysisBuffer.size();
        const auto phase = twoPi * (static_cast<float>(i) + 0.5f) / static_cast<float>(fftSize);
        const auto window = 0.42f - 0.50f * std::cos(phase) + 0.08f * std::cos(phase * 2.0f);
        fftTime[static_cast<std::size_t>(i)] = analysisBuffer[index] * window;
    }

    fft.fft(fftTime.data(), fftSpectrum.data());
    updatePeaks();
}

void DSP::updatePeaks() noexcept
{
    const auto lowHz = normalizedToCutHz(parameters.lowCut);
    const auto highHz = std::max(lowHz + 1.0f, normalizedToCutHz(parameters.highCut));
    std::array<Peak, fftSize / 2> candidates{};
    int candidateCount = 0;

    float maximum = 0.0f;
    for (int bin = 2; bin < fftSize / 2 - 1; ++bin)
    {
        const auto frequency = static_cast<float>(bin) * static_cast<float>(sampleRate) / static_cast<float>(fftSize);
        if (frequency < lowHz || frequency > highHz)
            continue;

        const auto magnitude = std::abs(fftSpectrum[static_cast<std::size_t>(bin)])
            / static_cast<float>(fftSize);
        maximum = std::max(maximum, magnitude);
        const auto previous = std::abs(fftSpectrum[static_cast<std::size_t>(bin - 1)]);
        const auto next = std::abs(fftSpectrum[static_cast<std::size_t>(bin + 1)]);
        if (magnitude > previous / static_cast<float>(fftSize)
            && magnitude >= next / static_cast<float>(fftSize))
        {
            if (candidateCount < static_cast<int>(candidates.size()))
                candidates[static_cast<std::size_t>(candidateCount++)] = { frequency, magnitude };
        }
    }

    auto byAmplitude = [](const Peak& a, const Peak& b)
    {
        return a.amplitude > b.amplitude;
    };
    std::sort(candidates.begin(), candidates.begin() + candidateCount, byAmplitude);

    const auto keep = std::min(maxSparkles,
        std::max(1, static_cast<int>(std::ceil(1.0f + parameters.density * (maxSparkles - 1)))));
    candidateCount = std::min(candidateCount, keep);

    const auto amplitudeFloor = maximum * (0.015f + 0.30f * (1.0f - parameters.density));
    int keptCount = 0;
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[static_cast<std::size_t>(i)].amplitude >= amplitudeFloor)
            candidates[static_cast<std::size_t>(keptCount++)] = candidates[static_cast<std::size_t>(i)];
    }
    candidateCount = keptCount;

    std::sort(candidates.begin(), candidates.begin() + candidateCount, [](const Peak& a, const Peak& b)
    {
        return a.frequency < b.frequency;
    });
    detectedPeakCount = candidateCount;
    for (int i = 0; i < detectedPeakCount; ++i)
        detectedPeaks[static_cast<std::size_t>(i)] = candidates[static_cast<std::size_t>(i)];

    const auto shimmer = parameters.shimmer;
    for (int i = 0; i < maxSparkles; ++i)
    {
        auto& sparkle = sparkles[static_cast<std::size_t>(i)];
        if (i >= detectedPeakCount)
        {
            sparkle.targetAmplitude = 0.0f;
            continue;
        }

        const auto sourcePosition = static_cast<float>(i) + shimmer;
        const auto first = std::min(detectedPeakCount - 1,
                                    static_cast<int>(std::floor(sourcePosition)));
        const auto second = std::min(detectedPeakCount - 1, first + 1);
        const auto fraction = sourcePosition - static_cast<float>(first);
        const auto& a = detectedPeaks[static_cast<std::size_t>(first)];
        const auto& b = detectedPeaks[static_cast<std::size_t>(second)];
        const auto frequency = a.frequency + fraction * (b.frequency - a.frequency);
        const auto amplitude = a.amplitude + fraction * (b.amplitude - a.amplitude);

        if (sparkle.frequency > 0.0f && std::abs(std::log2(std::max(1.0f, frequency)
                                                          / std::max(1.0f, sparkle.frequency))) > 0.15f)
            sparkle.age = 0.0f;
        sparkle.targetFrequency = frequency * std::pow(2.0f, 0.5f * shimmer);
        sparkle.targetAmplitude = std::sqrt(std::max(0.0f, amplitude / std::max(maximum, 1.0e-7f)))
            * (0.15f + 0.55f * parameters.density);
    }
}

void DSP::renderSparkles(float& left, float& right) noexcept
{
    const auto decayShape = (parameters.decay + 1.0f) * 0.5f;
    const auto lifetime = 0.045f + 2.2f * decayShape;
    const auto reverseAttack = parameters.decay < 0.0f ? -parameters.decay : 0.0f;
    const auto frequencySmoothing = 1.0f - std::exp(-1.0f / (0.012f * static_cast<float>(sampleRate)));
    const auto amplitudeSmoothing = 1.0f - std::exp(-1.0f / (0.018f * static_cast<float>(sampleRate)));
    const auto rate = parameters.rate;

    left = 0.0f;
    right = 0.0f;
    for (auto& sparkle : sparkles)
    {
        sparkle.frequency += frequencySmoothing * (sparkle.targetFrequency - sparkle.frequency);
        sparkle.amplitude += amplitudeSmoothing * (sparkle.targetAmplitude - sparkle.amplitude);
        sparkle.phase += twoPi * sparkle.frequency / static_cast<float>(sampleRate);
        sparkle.lfoPhase += twoPi * rate / static_cast<float>(sampleRate);
        sparkle.age += 1.0f / static_cast<float>(sampleRate);

        if (sparkle.phase >= twoPi)
            sparkle.phase -= twoPi;
        if (sparkle.lfoPhase >= twoPi)
            sparkle.lfoPhase -= twoPi;

        const auto fade = std::exp(-sparkle.age / lifetime);
        const auto attack = reverseAttack > 0.0f
            ? std::min(1.0f, sparkle.age / (0.012f + reverseAttack * 0.18f))
            : 1.0f;
        const auto gate = 0.20f + 0.80f * std::max(0.0f, std::sin(sparkle.lfoPhase));
        const auto value = std::sin(sparkle.phase) * sparkle.amplitude * fade * attack * gate;
        const auto pan = 0.5f + (sparkle.pan - 0.5f) * parameters.width;
        left += value * std::cos(pan * pi * 0.5f);
        right += value * std::sin(pan * pi * 0.5f);
    }
}

void DSP::processBlock(float* const* channels, int numChannels, int numSamples) noexcept
{
    if (channels == nullptr || numChannels <= 0 || numSamples <= 0)
        return;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const auto inputLeft = channels[0][sample];
        const auto inputRight = numChannels > 1 ? channels[1][sample] : inputLeft;
        const auto inputMono = 0.5f * (inputLeft + inputRight);
        const auto reverbLeft = processReverb(0, inputLeft);
        const auto reverbRight = processReverb(1, inputRight);

        analyse(0.5f * (reverbLeft + reverbRight));
        float sparkleLeft = 0.0f;
        float sparkleRight = 0.0f;
        renderSparkles(sparkleLeft, sparkleRight);

        const auto wetLeft = 0.78f * reverbLeft + sparkleLeft;
        const auto wetRight = 0.78f * reverbRight + sparkleRight;
        const auto wet = clamp01(parameters.mix);
        channels[0][sample] = inputLeft * (1.0f - wet) + wet * wetLeft;
        if (numChannels > 1)
            channels[1][sample] = inputRight * (1.0f - wet) + wet * wetRight;

        (void) inputMono;
    }
}

} // namespace scintillate
