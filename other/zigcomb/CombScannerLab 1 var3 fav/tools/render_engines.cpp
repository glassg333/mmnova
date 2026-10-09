// Оффлайн-рендерер: прогоняет один и тот же тестовый сигнал через все страницы
// лаборатории и пишет WAV-файлы, чтобы можно было сравнить варианты без DAW.
//
// Сборка (CMake: цель combscannerlab_render) или вручную:
//   g++ -std=c++17 -O2 -I Source tools/render_engines.cpp Source/lab/EngineRegistry.cpp \
//       Source/engines/*.cpp -o render_engines
//
// Запуск:  render_engines <выходная_папка> [секунды] [all|first]

#include "../Source/lab/Engine.h"
#include "../Source/lab/EngineRegistry.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

namespace
{
constexpr double kSampleRate = 48000.0;
constexpr int    kBlockSize  = 256;
constexpr double kPi         = 3.14159265358979323846;

void writeU16 (std::vector<uint8_t>& out, uint16_t v)
{
    out.push_back ((uint8_t) (v & 0xff));
    out.push_back ((uint8_t) ((v >> 8) & 0xff));
}

void writeU32 (std::vector<uint8_t>& out, uint32_t v)
{
    for (int i = 0; i < 4; ++i)
        out.push_back ((uint8_t) ((v >> (8 * i)) & 0xff));
}

bool writeWav (const std::string& path, const std::vector<float>& left, const std::vector<float>& right)
{
    const auto numFrames = (uint32_t) left.size();
    const uint32_t dataBytes = numFrames * 2u * 2u;

    std::vector<uint8_t> pcm;
    pcm.reserve (44 + dataBytes);
    const char* riff = "RIFF";
    pcm.insert (pcm.end(), riff, riff + 4);
    writeU32 (pcm, 36u + dataBytes);
    const char* wave = "WAVEfmt ";
    pcm.insert (pcm.end(), wave, wave + 8);
    writeU32 (pcm, 16u);
    writeU16 (pcm, 1u);
    writeU16 (pcm, 2u);
    writeU32 (pcm, (uint32_t) kSampleRate);
    writeU32 (pcm, (uint32_t) (kSampleRate * 2 * 2));
    writeU16 (pcm, 4u);
    writeU16 (pcm, 16u);
    const char* data = "data";
    pcm.insert (pcm.end(), data, data + 4);
    writeU32 (pcm, dataBytes);

    for (size_t i = 0; i < numFrames; ++i)
    {
        const float l = std::clamp (left[i], -1.0f, 1.0f);
        const float r = std::clamp (right[i], -1.0f, 1.0f);
        const int16_t li = (int16_t) std::lround (l * 32767.0f);
        const int16_t ri = (int16_t) std::lround (r * 32767.0f);
        writeU16 (pcm, (uint16_t) li);
        writeU16 (pcm, (uint16_t) ri);
    }

    std::FILE* f = std::fopen (path.c_str(), "wb");

    if (f == nullptr)
        return false;

    const size_t written = std::fwrite (pcm.data(), 1, pcm.size(), f);
    std::fclose (f);
    return written == pcm.size();
}

// Тестовый сигнал: короткие щипки, импульсы каждые 2 с, шумовые всплески и паузы.
float testSignal (double timeSeconds, uint64_t sampleIndex, double& envState)
{
    const double beat = 0.5; // 120 BPM
    const double posInBeat = std::fmod (timeSeconds, beat);

    if (posInBeat < 0.002)
        envState = 1.0;

    envState *= std::exp (-1.0 / (0.28 * kSampleRate));

    const double f0 = 110.0 * std::pow (2.0, std::floor (timeSeconds / 4.0) * 3.0 / 12.0);
    const double phase = std::fmod (timeSeconds * f0, 1.0);
    const double pluck = (2.0 * phase - 1.0) * 0.55
                       + std::sin (timeSeconds * f0 * 2.0 * kPi) * 0.22
                       + std::sin (timeSeconds * f0 * 3.0 * kPi) * 0.12;

    const double impulse = (std::fmod (timeSeconds, 2.0) < 0.001) ? 0.5 : 0.0;

    uint32_t h = (uint32_t) (sampleIndex * 2654435761u);
    h ^= h >> 13;
    const double noise = ((double) (h & 0xffffu) / 32768.0) - 1.0;
    const double burst = (posInBeat > 0.25 && posInBeat < 0.31) ? noise * 0.18 : 0.0;

    return (float) (0.55 * envState * pluck + impulse + burst + 0.06 * noise);
}

struct RenderSettings
{
    float gain      = 0.99f;
    float damp      = 0.90f;
    float phase     = 0.75f;
    float delay1Ms  = 115.0f;
    float delay2Ms  = 500.0f;
    float scanDepth = 0.55f;
    float scanRate  = 0.09f;
};

// Прогон одной страницы. Возвращает пик выходного сигнала.
float renderEngine (lab::Engine& engine, int model, float seconds,
                    const RenderSettings& settings,
                    std::vector<float>& left, std::vector<float>& right)
{
    const int total = (int) (seconds * kSampleRate);
    left.assign ((size_t) total, 0.0f);
    right.assign ((size_t) total, 0.0f);

    engine.prepare (kSampleRate, kBlockSize);
    engine.reset();

    if (engine.numModels() > 0)
        engine.setModel (model);

    std::vector<float> blockLeft ((size_t) kBlockSize), blockRight ((size_t) kBlockSize);
    float* channels[2] = { blockLeft.data(), blockRight.data() };

    double scanPhase = 0.0;
    double envState = 0.0;
    float peak = 0.0f;

    for (int offset = 0; offset < total; offset += kBlockSize)
    {
        const int num = std::min (kBlockSize, total - offset);

        // Scan LFO считается на блок — так же, как это делает плагин.
        const float lfo = (float) std::sin (scanPhase * 2.0 * kPi);
        const float scanValue = std::clamp (0.5f + 0.5f * settings.scanDepth * lfo, 0.0f, 1.0f);
        scanPhase += settings.scanRate * (double) num / kSampleRate;

        if (scanPhase >= 1.0)
            scanPhase -= 1.0;

        for (int i = 0; i < num; ++i)
        {
            const double t = (double) (offset + i) / kSampleRate;
            const float in = testSignal (t, (uint64_t) (offset + i), envState);
            blockLeft[(size_t) i] = in;
            blockRight[(size_t) i] = in;
        }

        // Раскладываем общие настройки в ручки по их ключам (остальные остаются по умолчанию движка).
        for (int i = 0; i < engine.numParams(); ++i)
        {
            const auto& info = engine.params()[i];
            const std::string key (info.key);
            float value = info.def;

            if (key == "gain" || key == "feedback")
                value = settings.gain;
            else if (key == "damp" || key == "damping")
                value = settings.damp;
            else if (key == "phase")
                value = settings.phase;
            else if (key == "delay1")
                value = settings.delay1Ms;
            else if (key == "delay2")
                value = settings.delay2Ms;
            else if (key == "scan")
                value = scanValue;

            value = std::clamp (value, info.min, info.max);
            engine.setParam (i, value);
        }

        engine.process (channels, 2, num);

        for (int i = 0; i < num; ++i)
        {
            const float outL = blockLeft[(size_t) i];
            const float outR = blockRight[(size_t) i];
            peak = std::max (peak, std::max (std::abs (outL), std::abs (outR)));
            left[(size_t) (offset + i)] = outL;
            right[(size_t) (offset + i)] = outR;
        }
    }

    return peak;
}

std::string makeFileName (const char* title)
{
    std::string name (title);

    for (auto& c : name)
    {
        if (! (std::isalnum ((unsigned char) c) || c == '-' || c == '_'))
            c = '_';
    }

    return name;
}
} // namespace

int main (int argc, char** argv)
{
    const std::string outDir = (argc > 1) ? argv[1] : ".";
    const double seconds = (argc > 2) ? std::atof (argv[2]) : 12.0;
    const bool allModels = (argc > 3) && (std::string (argv[3]) == "all");

    std::printf ("Rendering %d pages x %.1f s @ %.0f Hz (models: %s)\n",
                 lab::numEngines(), seconds, kSampleRate, allModels ? "all" : "first only");

    for (int slot = 0; slot < lab::numEngines(); ++slot)
    {
        std::unique_ptr<lab::Engine> engine (lab::createEngine (slot));

        if (engine == nullptr)
            continue;

        const auto info = engine->info();
        const int models = allModels ? std::max (1, engine->numModels()) : 1;

        for (int model = 0; model < models; ++model)
        {
            std::vector<float> left, right;
            const RenderSettings settings;
            const float peak = renderEngine (*engine, model, seconds, settings, left, right);

            std::string file = outDir + "/" + makeFileName (info.id);

            if (engine->numModels() > 0)
                file += "-m" + std::to_string (model + 1) + "-" + makeFileName (engine->modelName (model));

            file += ".wav";

            if (writeWav (file, left, right))
                std::printf ("  %-60s peak %.3f\n", file.c_str(), (double) peak);
            else
            {
                std::printf ("  %-60s WRITE FAILED\n", file.c_str());
                return 1;
            }
        }
    }

    return 0;
}
