#include "CombScannerVariants3.h"

#include <algorithm>
#include <cmath>

namespace var3
{
namespace
{
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = 2.0f * pi;
constexpr std::array<const char*, CombScannerVariantsDSP::variantCount> names {
    "01 Original Hypothesis", "02 Scrub Stretch", "03 Phase Cloud",
    "04 Dark Bloom", "05 Bright Teeth", "06 Unstable Edge", "07 Balanced Matrix"
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
    // Бывшие 02 Tight Crossfade / 03 Long Resonator / 06 Ping Pong удалены.
    switch (variant)
    {
        case 1: // Scrub Stretch (был 04)
            return Config { { 0.33f, 0.50f, 0.75f, 0.90f, 1.20f, 1.60f, 2.40f, 3.10f },
                0.54f, 0.42f, 0.55f, 0.14f, 0.24f, 0.78f, 0.10f, 1.00f, 0.86f, false, false };
        case 2: // Phase Cloud (был 05)
            return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
                0.55f, 0.40f, 0.80f, 0.20f, 0.48f, 0.92f, 0.35f, 1.05f, 0.88f, false, false };
        case 3: // Dark Bloom (был 07)
            return Config { { 0.40f, 0.60f, 0.80f, 1.0f, 1.30f, 1.80f, 2.50f, 3.20f },
                0.64f, 0.30f, 1.10f, 0.018f, 0.62f, 0.42f, 0.20f, 0.76f, 0.90f, false, false };
        case 4: // Bright Teeth (был 08)
            return Config { { 0.28f, 0.44f, 0.59f, 0.88f, 1.07f, 1.34f, 2.08f, 3.00f },
                0.48f, 0.47f, 0.92f, 0.065f, 0.10f, 0.88f, 0.30f, 1.12f, 0.84f, false, false };
        case 5: // Unstable Edge (был 09)
            return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
                0.70f, 0.27f, 1.35f, 0.09f, 0.72f, 0.86f, 0.42f, 0.98f, 0.90f, true, false };
        case 6: // Balanced Matrix (был 10)
            return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
                0.57f, 0.41f, 0.85f, 0.045f, 0.36f, 0.78f, 0.65f, 1.0f, 0.88f, false, false };
        default: // Original
            return Config { { 0.33f, 0.50f, 0.66f, 1.0f, 1.11f, 1.45f, 2.22f, 3.33f },
                0.55f, 0.435f, 1.00f, 0.045f, 0.12f, 0.80f, 0.0f, 0.86f, 0.86f, false, false };
    }
}

void CombScannerVariantsDSP::DelayLine::prepare(double newSampleRate, double maximumDelayMs)
{
    const auto size = static_cast<std::size_t>(std::ceil(std::max(1.0, newSampleRate)
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

    // 1000 мс * max ratio 3.33 + запас модуляции
    for (auto& voice : voices)
    {
        voice.delay1.prepare(sampleRate, 3600.0);
        voice.delay2.prepare(sampleRate, 3600.0);
    }

    reset();
}

int CombScannerVariantsDSP::muxIndexOf(float scan) const noexcept
{
    const float curve = std::max(0.2f, parameters.scanCurve);
    float position = std::pow(clamp(scan, 0.0f, 1.0f), curve) * 7.0f;

    if (config.reverseScan)
        position = 7.0f - position;

    return (int) std::lround(clamp(position, 0.0f, 7.0f));
}

void CombScannerVariantsDSP::reset() noexcept
{
    const float fs = static_cast<float>(sampleRate);

    for (std::size_t i = 0; i < voices.size(); ++i)
    {
        auto& voice = voices[i];
        voice.delay1.clear();
        voice.delay2.clear();
        voice.dampingState = 0.0f;
        voice.allpassState = 0.0f;
        voice.allpassState2 = 0.0f;
        voice.dcInput = 0.0f;
        voice.dcOutput = 0.0f;
        voice.phase = static_cast<float>(i) * 0.73f;

        const float ratio = parameters.manualRatios >= 0.5f ? parameters.ratios[i] : config.ratios[i];
        voice.tap1.reset(std::max(1.0f, parameters.delay1Ms * ratio * fs / 1000.0f));
        voice.tap2.reset(std::max(1.0f, parameters.delay2Ms * ratio * fs / 1000.0f));
    }

    wetLeft = 0.0f;
    wetRight = 0.0f;
    smoothedScan = parameters.scan;
    muxIndex = muxIndexOf(parameters.scan);
    muxPrevious = muxIndex;
    muxXf = 1.0f;
}

void CombScannerVariantsDSP::setParameters(Parameters newParameters)
{
    parameters.gain = clamp(newParameters.gain, 0.0f, 2.0f);
    parameters.feedback = clamp(newParameters.feedback, 0.0f, 0.999f);
    parameters.damp = clamp(newParameters.damp, 0.0f, 0.999f);
    parameters.phase = clamp(newParameters.phase, 0.0f, 1.0f);
    parameters.delay1Ms = clamp(newParameters.delay1Ms, 0.0f, 1000.0f);
    parameters.delay2Ms = clamp(newParameters.delay2Ms, 0.0f, 1000.0f);
    parameters.scan = clamp(newParameters.scan, 0.0f, 1.0f);
    parameters.variant = static_cast<int>(clamp(static_cast<float>(newParameters.variant), 0.0f,
                                                static_cast<float>(variantCount - 1)));

    for (int i = 0; i < numVoices; ++i)
        parameters.ratios[i] = clamp(newParameters.ratios[i], 0.05f, 8.0f);

    parameters.manualRatios = newParameters.manualRatios >= 0.5f ? 1.0f : 0.0f;
    parameters.scanCurve = clamp(newParameters.scanCurve, 0.2f, 3.0f);
    parameters.modX = clamp(newParameters.modX, 0.0f, 3.0f);
    parameters.apBaseX = clamp(newParameters.apBaseX, 0.0f, 3.0f);
    parameters.apPhaseX = clamp(newParameters.apPhaseX, 0.0f, 3.0f);
    parameters.fbFloorX = clamp(newParameters.fbFloorX, 0.0f, 3.0f);
    parameters.fbRangeX = clamp(newParameters.fbRangeX, 0.0f, 3.0f);
    parameters.wetX = clamp(newParameters.wetX, 0.0f, 3.0f);
    parameters.dry = clamp(newParameters.dry, 0.0f, 1.0f);
    parameters.outGain = clamp(newParameters.outGain, 0.0f, 8.0f);
    parameters.spread = clamp(newParameters.spread, 0.0f, 0.6f);
    parameters.scanFade = clamp(newParameters.scanFade, 0.002f, 0.25f);
    parameters.scanMode = newParameters.scanMode >= 0.5f ? 1.0f : 0.0f;
    parameters.diffusion = clamp(newParameters.diffusion, 0.0f, 1.0f);

    config = configFor(parameters.variant);
}

float CombScannerVariantsDSP::processMono(float input) noexcept
{
    const float fs = static_cast<float>(sampleRate);
    const float oneSampleMs = 1000.0f / fs;
    const float declickStep = 1.0f / (0.005f * fs);
    const float minJump = 0.0005f * fs;

    const float fbFloor = clamp(config.feedbackFloor * parameters.fbFloorX, 0.0f, 0.998f);
    const float fbRange = clamp(config.feedbackRange * parameters.fbRangeX, 0.0f, 0.998f);
    const float feedback = clamp(fbFloor + fbRange * parameters.feedback, 0.0f, 0.998f);

    const float drive = 0.42f * parameters.gain;
    // damp=0 → step=1: фильтр полностью прозрачен и не трогает хвост
    const float dampingStep = clamp(1.0f - 0.92f * parameters.damp, 0.05f, 1.0f);
    const float apBase = clamp(config.allpassBase * parameters.apBaseX, 0.0f, 0.92f);
    const float apPhase = config.allpassPhaseDepth * parameters.apPhaseX;
    const float phaseAmount = parameters.phase;
    const float spread = parameters.spread;
    const float wetLevel = clamp(config.wet * parameters.wetX, 0.0f, 1.5f);
    const float delay1 = parameters.delay1Ms;
    const float delay2 = parameters.delay2Ms;
    const bool manual = parameters.manualRatios >= 0.5f;

    std::array<float, 8> outputs{};
    float previous = 0.0f;

    for (std::size_t i = 0; i < voices.size(); ++i)
    {
        auto& voice = voices[i];

        const float swirlA = std::sin(0.7f + 2.399f * static_cast<float>(i));
        const float swirlB = std::sin(1.9f + 1.618f * static_cast<float>(i));

        voice.phase += (0.012f + 0.028f * phaseAmount) * (1.0f + 0.08f * static_cast<float>(i)) / fs;

        if (voice.phase >= twoPi)
            voice.phase -= twoPi;

        const float movement = std::sin(voice.phase) * config.modulationDepth * parameters.modX;
        const float baseRatio = manual ? parameters.ratios[i] : config.ratios[i];
        const float ratio = baseRatio * (1.0f + movement) * (1.0f + 0.10f * spread * swirlB);

        const float time1 = std::max(oneSampleMs, delay1 * ratio);
        const float time2 = std::max(oneSampleMs, delay2 * ratio);

        voice.tap1.retarget(time1 * fs / 1000.0f, minJump);
        voice.tap2.retarget(time2 * fs / 1000.0f, minJump);
        voice.tap1.advance(declickStep);
        voice.tap2.advance(declickStep);

        const float a1 = voice.delay1.read(voice.tap1.prev);
        const float b1 = voice.delay1.read(voice.tap1.pos);
        const float a2 = voice.delay2.read(voice.tap2.prev);
        const float b2 = voice.delay2.read(voice.tap2.pos);
        const float tap1 = a1 + (b1 - a1) * voice.tap1.xf;
        const float tap2 = a2 + (b2 - a2) * voice.tap2.xf;

        // Петля только на линии 1: |петли| = feedback, Phase не глотает хвост.
        const float step = clamp(dampingStep * (1.0f - 0.35f * spread * swirlA), 0.02f, 1.0f);
        voice.dampingState += step * (tap1 - voice.dampingState);
        const float resonant = (parameters.damp <= 0.0001f)
                                   ? tap1
                                   : 0.70f * tap1 + 0.30f * voice.dampingState;

        // Два каскадных all-pass в петле (|H|=1): metallic / сложный хвост.
        // diffusion=0 и phase=0 → оба каскада обходятся, хвост = чистый комб.
        const float c1 = clamp(apBase + apPhase * phaseAmount + 0.35f * spread * swirlA, -0.9f, 0.9f);
        const float c2 = clamp(0.18f + 0.62f * parameters.diffusion + 0.12f * swirlB, -0.9f, 0.9f);
        float apOut = resonant;
        if (phaseAmount > 0.0001f || apBase > 0.0001f)
        {
            const float y1 = -c1 * resonant + voice.allpassState;
            voice.allpassState = resonant + c1 * y1;
            apOut = y1;
        }
        if (parameters.diffusion > 0.0001f)
        {
            const float y2 = -c2 * apOut + voice.allpassState2;
            voice.allpassState2 = apOut + c2 * y2;
            apOut = y2;
        }

        const float voiceFeedback = clamp(feedback * (1.0f + 0.35f * spread * swirlB), 0.0f, 0.998f);
        const float excitation = input * drive + voiceFeedback * apOut;

        const float loopWrite = clamp(lab::softLimit(excitation, 0.9f), -1.5f, 1.5f);
        voice.delay1.write(loopWrite);
        voice.delay2.write(loopWrite);

        const float rawComb = 0.5f * (tap1 + tap2);
        const float comb = rawComb * (1.0f - config.crossMix) + previous * config.crossMix;
        outputs[i] = dcBlock(voice, comb);
        previous = outputs[i];
    }

    const float fadeAlpha = 1.0f - std::exp(-1.0f / (parameters.scanFade * fs));
    float scanned = 0.0f;

    if (parameters.scanMode >= 0.5f)
    {
        const int index = muxIndexOf(parameters.scan);

        if (index != muxIndex)
        {
            muxPrevious = muxIndex;
            muxIndex = index;
            muxXf = 0.0f;
        }

        muxXf = std::min(1.0f, muxXf + fadeAlpha);

        const float from = outputs[(std::size_t) muxPrevious];
        const float to = outputs[(std::size_t) muxIndex];
        scanned = from * std::cos(muxXf * pi * 0.5f) + to * std::sin(muxXf * pi * 0.5f);
    }
    else
    {
        smoothedScan += fadeAlpha * (parameters.scan - smoothedScan);
        const float curve = std::max(0.2f, parameters.scanCurve);
        float position = std::pow(clamp(smoothedScan, 0.0f, 1.0f), curve) * 7.0f;

        if (config.reverseScan)
            position = 7.0f - position;

        position = clamp(position, 0.0f, 7.0f);
        const auto first = (std::size_t) position;
        const auto second = (first + 1u) % voices.size();
        const float fraction = position - static_cast<float>(first);
        scanned = std::cos(fraction * pi * 0.5f) * outputs[first]
                + std::sin(fraction * pi * 0.5f) * outputs[second];
    }

    const float wet = wetLevel * scanned;

    if (! std::isfinite(wet))
    {
        reset();
        wetLeft = 0.0f;
        wetRight = 0.0f;
        return 0.0f;
    }

    wetLeft = wet;
    wetRight = wet;
    return wet;
}

void CombScannerVariantsDSP::processBlock(float* left, float* right, int numSamples) noexcept
{
    if (left == nullptr || numSamples <= 0)
        return;

    const float dry = parameters.dry;
    const float outputGain = parameters.outGain;

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
