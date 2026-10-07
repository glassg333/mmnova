#include "../Source/DSP/CombScannerEngine.h"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

int main()
{
    using namespace combscanner;
    constexpr int blockSize = 256;
    constexpr double sampleRate = 48000.0;

    auto parameters = makeDefaultEngineParameters();
    CombScannerEngine engine;
    engine.prepare(sampleRate, blockSize);
    engine.setParameters(parameters);

    std::array<float, blockSize> left {};
    std::array<float, blockSize> right {};
    float peak = 0.0f;
    for (int block = 0; block < 120; ++block)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            const auto n = block * blockSize + i;
            left[static_cast<std::size_t>(i)] = 0.15f * std::sin(2.0 * 3.141592653589793 * 220.0 * n / sampleRate);
            right[static_cast<std::size_t>(i)] = 0.15f * std::sin(2.0 * 3.141592653589793 * 221.0 * n / sampleRate);
        }
        engine.processBlock(left.data(), right.data(), blockSize, 123.0);
        for (int i = 0; i < blockSize; ++i)
        {
            assert(std::isfinite(left[static_cast<std::size_t>(i)]));
            assert(std::isfinite(right[static_cast<std::size_t>(i)]));
            peak = std::max(peak, std::max(std::abs(left[static_cast<std::size_t>(i)]),
                                           std::abs(right[static_cast<std::size_t>(i)])));
        }
    }
    assert(peak <= 1.251f);

    parameters.mode = static_cast<int>(DspMode::moreControl);
    parameters.combs[0].ratio = 2.5f;
    parameters.combs[1].ratio = 0.375f;
    parameters.combs[0].feedback = 0.90f;
    parameters.fxSlots[0].type = static_cast<int>(FxType::fftStretch);
    parameters.fxSlots[0].afterComb = 1;
    parameters.fxSlots[0].mix = 0.8f;
    parameters.fxSlots[0].size = 0.5f;
    parameters.routes[1] = { 1, kDestinationCombRatioBase + 2, 0.25f };
    engine.setParameters(parameters);
    assert(engine.getLatencySamples() == kFftLatencySamples);

    for (int block = 0; block < 24; ++block)
    {
        for (int i = 0; i < blockSize; ++i)
        {
            const auto n = block * blockSize + i;
            left[static_cast<std::size_t>(i)] = (n == 0 ? 0.8f : 0.0f);
            right[static_cast<std::size_t>(i)] = (n == 0 ? -0.8f : 0.0f);
        }
        engine.processBlock(left.data(), right.data(), blockSize, 120.0);
        for (int i = 0; i < blockSize; ++i)
        {
            assert(std::isfinite(left[static_cast<std::size_t>(i)]));
            assert(std::isfinite(right[static_cast<std::size_t>(i)]));
        }
    }

    parameters.fxSlots[1].type = static_cast<int>(FxType::fftStretch);
    parameters.fxSlots[1].mix = 0.5f;
    engine.setParameters(parameters);
    assert(engine.getLatencySamples() == 2 * kFftLatencySamples);

    std::cout << "DSP smoke tests passed; peak=" << peak
              << ", dynamic FFT latency=" << engine.getLatencySamples() << " samples\n";
    return 0;
}
