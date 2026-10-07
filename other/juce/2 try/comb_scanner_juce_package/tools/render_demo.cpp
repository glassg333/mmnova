#include "combscanner/CombScannerDSP.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
constexpr double sampleRate = 48000.0;
constexpr double pi = 3.14159265358979323846;

void writeU16(std::ofstream& file, std::uint16_t value)
{
    const char bytes[] { static_cast<char>(value & 0xffu), static_cast<char>((value >> 8u) & 0xffu) };
    file.write(bytes, 2);
}

void writeU32(std::ofstream& file, std::uint32_t value)
{
    const char bytes[] {
        static_cast<char>(value & 0xffu), static_cast<char>((value >> 8u) & 0xffu),
        static_cast<char>((value >> 16u) & 0xffu), static_cast<char>((value >> 24u) & 0xffu)
    };
    file.write(bytes, 4);
}

void writeWav(const std::string& path, const std::vector<float>& left, const std::vector<float>& right)
{
    const auto frames = static_cast<std::uint32_t>(std::min(left.size(), right.size()));
    constexpr std::uint32_t bytesPerFrame = 4;
    std::ofstream file(path, std::ios::binary);
    file.write("RIFF", 4);
    writeU32(file, 36u + frames * bytesPerFrame);
    file.write("WAVEfmt ", 8);
    writeU32(file, 16u);
    writeU16(file, 1u);
    writeU16(file, 2u);
    writeU32(file, static_cast<std::uint32_t>(sampleRate));
    writeU32(file, static_cast<std::uint32_t>(sampleRate) * bytesPerFrame);
    writeU16(file, static_cast<std::uint16_t>(bytesPerFrame));
    writeU16(file, 16u);
    file.write("data", 4);
    writeU32(file, frames * bytesPerFrame);
    for (std::uint32_t i = 0; i < frames; ++i)
    {
        const auto l = static_cast<std::int16_t>(std::clamp(left[i], -1.0f, 1.0f) * 32767.0f);
        const auto r = static_cast<std::int16_t>(std::clamp(right[i], -1.0f, 1.0f) * 32767.0f);
        writeU16(file, static_cast<std::uint16_t>(l));
        writeU16(file, static_cast<std::uint16_t>(r));
    }
}
}

int main(int argc, char** argv)
{
    const std::string output = argc > 1 ? argv[1] : "comb_scanner_demo.wav";
    constexpr int seconds = 12;
    const int totalSamples = static_cast<int>(sampleRate * seconds);
    std::vector<float> left(static_cast<std::size_t>(totalSamples));
    std::vector<float> right(static_cast<std::size_t>(totalSamples));
    std::vector<float> blockLeft(256, 0.0f);
    std::vector<float> blockRight(256, 0.0f);
    float* channels[] { blockLeft.data(), blockRight.data() };

    combscanner::CombScannerDSP dsp;
    dsp.prepare(sampleRate, 256);

    for (int offset = 0; offset < totalSamples; offset += 256)
    {
        const int blockSize = std::min(256, totalSamples - offset);
        const double time = static_cast<double>(offset) / sampleRate;
        const float scan = static_cast<float>(std::fmod(time / seconds, 1.0));
        dsp.setParameters({ 0.99f, 0.90f, 0.75f, 115.0f, 500.0f, scan });
        std::fill(blockLeft.begin(), blockLeft.end(), 0.0f);
        std::fill(blockRight.begin(), blockRight.end(), 0.0f);

        for (int i = 0; i < blockSize; ++i)
        {
            const int sample = offset + i;
            const double now = static_cast<double>(sample) / sampleRate;
            const int section = static_cast<int>(now / 2.0);
            const double local = now - section * 2.0;
            const double envelope = std::exp(-local * 2.1);
            const double tone = 0.24 * std::sin(2.0 * pi * (146.83 + section * 19.0) * now)
                + 0.12 * std::sin(2.0 * pi * (220.0 + section * 23.0) * now);
            float source = static_cast<float>(tone * envelope);
            if (std::fmod(now, 2.0) < 0.006)
                source += static_cast<float>(0.55 * std::exp(-local * 20.0));
            blockLeft[i] = source;
            blockRight[i] = source * 0.94f;
        }

        dsp.processBlock(channels, 2, blockSize);
        std::copy(blockLeft.begin(), blockLeft.begin() + blockSize, left.begin() + offset);
        std::copy(blockRight.begin(), blockRight.begin() + blockSize, right.begin() + offset);
    }

    writeWav(output, left, right);
    std::cout << "Wrote " << output << " (" << seconds << " seconds, stereo, 48 kHz)\n";
    return 0;
}
