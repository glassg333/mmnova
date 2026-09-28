// Standalone, firmware-free renderer used by oracle/fmplus/compare_native_oracle.py.
// It renders only the new native ORACLE core at 44.1 kHz to a temporary 24-bit
// stereo WAV. It intentionally contains no Monomodule/OS/emulator dependency.
#include "dsp/fm_oracle/OracleFm.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace {

void writeU16(std::FILE* file, uint16_t value)
{
    std::fputc(value & 0xff, file);
    std::fputc((value >> 8) & 0xff, file);
}

void writeU32(std::FILE* file, uint32_t value)
{
    for (int shift = 0; shift < 32; shift += 8) std::fputc((value >> shift) & 0xff, file);
}

bool parseRawWords(const char* text, std::array<uint8_t, 8>& result)
{
    if (text == nullptr) return false;
    std::string source(text);
    size_t begin = 0;
    for (size_t index = 0; index < result.size(); ++index) {
        const size_t end = source.find(',', begin);
        const std::string token = source.substr(begin, end == std::string::npos ? std::string::npos : end - begin);
        char* tail = nullptr;
        const long parsed = std::strtol(token.c_str(), &tail, 10);
        if (tail == token.c_str() || *tail != '\0' || parsed < 0 || parsed > 127) return false;
        result[index] = static_cast<uint8_t>(parsed);
        if (index + 1 == result.size()) return end == std::string::npos;
        if (end == std::string::npos) return false;
        begin = end + 1;
    }
    return false;
}

void writeWav24(const char* output, const std::vector<float>& left, const std::vector<float>& right)
{
    if (left.size() != right.size()) throw "channel length mismatch";
    std::FILE* file = std::fopen(output, "wb");
    if (file == nullptr) throw "cannot open output WAV";
    const uint32_t frames = static_cast<uint32_t>(left.size());
    const uint32_t dataBytes = frames * 6u;
    std::fwrite("RIFF", 1, 4, file); writeU32(file, 36u + dataBytes); std::fwrite("WAVE", 1, 4, file);
    std::fwrite("fmt ", 1, 4, file); writeU32(file, 16u); writeU16(file, 1u); writeU16(file, 2u);
    writeU32(file, 44100u); writeU32(file, 44100u * 6u); writeU16(file, 6u); writeU16(file, 24u);
    std::fwrite("data", 1, 4, file); writeU32(file, dataBytes);
    for (size_t index = 0; index < left.size(); ++index) {
        for (const float sample : {left[index], right[index]}) {
            const float bounded = std::clamp(sample, -1.0f, 1.0f);
            const int32_t word = static_cast<int32_t>(std::lround(bounded * 8388607.0f));
            std::fputc(word & 0xff, file);
            std::fputc((word >> 8) & 0xff, file);
            std::fputc((word >> 16) & 0xff, file);
        }
    }
    std::fclose(file);
}

void usage(const char* executable)
{
    std::fprintf(stderr, "Usage: %s --machine stat|par|dyn --note N --dur seconds --tail seconds --syn a,b,c,d,e,f,g,h --out temporary.wav\n", executable);
}
} // namespace

int main(int argc, char** argv)
{
    const char* machine = nullptr;
    const char* rawWords = nullptr;
    const char* output = nullptr;
    int note = 69;
    double duration = 0.125;
    double tail = 0.0;
    for (int index = 1; index < argc; ++index) {
        const char* key = argv[index];
        const auto needsValue = [&]() -> const char* {
            if (index + 1 >= argc) { usage(argv[0]); std::exit(2); }
            return argv[++index];
        };
        if (std::strcmp(key, "--machine") == 0) machine = needsValue();
        else if (std::strcmp(key, "--note") == 0) note = std::atoi(needsValue());
        else if (std::strcmp(key, "--dur") == 0) duration = std::atof(needsValue());
        else if (std::strcmp(key, "--tail") == 0) tail = std::atof(needsValue());
        else if (std::strcmp(key, "--syn") == 0) rawWords = needsValue();
        else if (std::strcmp(key, "--out") == 0) output = needsValue();
        else { usage(argv[0]); return 2; }
    }
    std::array<uint8_t, 8> raw{};
    if (machine == nullptr || output == nullptr || !parseRawWords(rawWords, raw) || note < 0 || note > 127
        || duration < 0.0 || tail < 0.0 || duration + tail > 10.0) {
        usage(argv[0]);
        return 2;
    }
    // The behavioural oracle emits whole 16-frame DSP blocks. Match its
    // observable duration boundary while remaining firmware-free.
    constexpr size_t kOracleFrameBlock = 16;
    const size_t requestedFrames = static_cast<size_t>(std::ceil((duration + tail) * 44100.0));
    const size_t frames = ((requestedFrames + kOracleFrameBlock - 1) / kOracleFrameBlock) * kOracleFrameBlock;
    std::vector<float> left(frames), right(frames);
    try {
        if (std::strcmp(machine, "stat") == 0) {
            monomachine::fm_oracle::OracleStatCore core;
            core.reset(44100.0); core.setParameters(raw[0], raw[1], raw[2], raw[3], raw[4], raw[5], raw[6], raw[7]);
            core.noteOn(static_cast<uint8_t>(note)); core.setPitchBend(0.0f); core.processStereo(left.data(), right.data(), frames);
        } else if (std::strcmp(machine, "par") == 0) {
            monomachine::fm_oracle::OracleParallelCore core;
            core.reset(44100.0); core.setParameters(raw[0], raw[1], raw[2], raw[3], raw[4], raw[5], raw[6], raw[7]);
            core.noteOn(static_cast<uint8_t>(note)); core.setPitchBend(0.0f); core.processStereo(left.data(), right.data(), frames);
        } else if (std::strcmp(machine, "dyn") == 0) {
            monomachine::fm_oracle::OracleDynamicCore core;
            core.reset(44100.0); core.setParameters(raw[0], raw[1], raw[2], raw[3], raw[4], raw[5], raw[6], raw[7]);
            core.noteOn(static_cast<uint8_t>(note)); core.setPitchBend(0.0f); core.processStereo(left.data(), right.data(), frames);
        } else {
            usage(argv[0]);
            return 2;
        }
        writeWav24(output, left, right);
    } catch (const char* error) {
        std::fprintf(stderr, "oracle native renderer: %s\n", error);
        return 1;
    }
    return 0;
}
