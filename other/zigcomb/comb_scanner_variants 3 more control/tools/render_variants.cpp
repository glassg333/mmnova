#include "combscanner/CombScannerVariantsDSP.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace
{
constexpr int sampleRate = 48000;
constexpr int seconds = 12;
constexpr int totalSamples = sampleRate * seconds;
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = 2.0f * pi;

float sourceAt(int sample)
{
    const float time = static_cast<float>(sample) / static_cast<float>(sampleRate);
    const int section = static_cast<int>(time / 2.0f);
    const float local = time - static_cast<float>(section * 2);
    const float envelope = std::exp(-local * 1.9f);
    const float chord = 0.22f * std::sin(twoPi * (110.0f + section * 23.0f) * time)
        + 0.15f * std::sin(twoPi * (164.81f + section * 29.0f) * time)
        + 0.10f * std::sin(twoPi * (246.94f + section * 37.0f) * time);
    const float impulse = std::fmod(time, 2.0f) < 0.006f ? 0.48f * std::exp(-local * 18.0f) : 0.0f;
    return chord * envelope + impulse;
}

void writeU16(std::ofstream& out, std::uint16_t value) { out.write(reinterpret_cast<const char*>(&value), 2); }
void writeU32(std::ofstream& out, std::uint32_t value) { out.write(reinterpret_cast<const char*>(&value), 4); }

void writeWav(const std::filesystem::path& path, const std::vector<float>& left,
              const std::vector<float>& right)
{
    std::ofstream out(path, std::ios::binary);
    const std::uint32_t dataBytes = static_cast<std::uint32_t>(left.size() * 4u);
    out.write("RIFF", 4); writeU32(out, 36u + dataBytes); out.write("WAVE", 4);
    out.write("fmt ", 4); writeU32(out, 16); writeU16(out, 1); writeU16(out, 2);
    writeU32(out, sampleRate); writeU32(out, sampleRate * 4u); writeU16(out, 4); writeU16(out, 16);
    out.write("data", 4); writeU32(out, dataBytes);
    for (std::size_t i = 0; i < left.size(); ++i)
    {
        const auto toInt = [] (float value) {
            return static_cast<std::int16_t>(std::clamp(std::lround(value * 32767.0f), -32768l, 32767l));
        };
        const auto l = toInt(left[i]);
        const auto r = toInt(right[i]);
        out.write(reinterpret_cast<const char*>(&l), 2);
        out.write(reinterpret_cast<const char*>(&r), 2);
    }
}
}

int main(int argc, char** argv)
{
    const std::filesystem::path output = argc > 1 ? argv[1] : ".";
    const std::array<const char*, combscanner::CombScannerVariantsDSP::variantCount> names {
        "01_original_hypothesis", "02_tight_crossfade", "03_long_resonator", "04_scrub_stretch",
        "05_phase_cloud", "06_ping_pong", "07_dark_bloom", "08_bright_teeth",
        "09_unstable_edge", "10_balanced_matrix"
    };
    for (int variant = 0; variant < combscanner::CombScannerVariantsDSP::variantCount; ++variant)
    {
        combscanner::CombScannerVariantsDSP dsp;
        dsp.prepare(sampleRate, 512);
        combscanner::CombScannerVariantsDSP::Parameters parameters;
        parameters.variant = variant;
        parameters.feedback = 0.99f;
        parameters.damp = 0.90f;
        parameters.phase = 0.75f;
        parameters.diffusion = 0.55f;
        parameters.crossMix = 0.50f;
        parameters.motion = 0.55f;
        parameters.delay1Ms = 115.0f;
        parameters.delay2Ms = 500.0f;
        dsp.setParameters(parameters);
        std::vector<float> left(totalSamples), right(totalSamples);
        for (int sample = 0; sample < totalSamples; ++sample)
        {
            parameters.scan = 0.5f - 0.5f * std::cos(twoPi * static_cast<float>(sample) / (sampleRate * 6.0f));
            dsp.setParameters(parameters);
            float channels[2] { sourceAt(sample), sourceAt(sample) * 0.94f };
            dsp.processBlock(channels, channels + 1, 1);
            left[static_cast<std::size_t>(sample)] = channels[0];
            right[static_cast<std::size_t>(sample)] = channels[1];
        }
        writeWav(output / (std::string(names[variant]) + ".wav"), left, right);
    }
    return 0;
}
