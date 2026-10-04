// Package-5 FM/AMP page map used by TRY4 (OS 1.32B).
// Provenance: 5 fix pitch+env fm full / 00_README_FIRST_RU.txt.
// $414..$417 remain named as an audit boundary only: TRY4 does not fake them
// from P1 filter controls and does not execute the full Package-8 tail.
#pragma once
#include <array>
#include <cstdint>

namespace monomachine::fm_try4_voice {
struct VoicePageMap final {
    static constexpr uint32_t kPage = 0x400;
    static constexpr uint32_t kMachine = 0x42c;
    static constexpr uint32_t kTempo = 0x423;
    static constexpr uint32_t kTrigger = 0x428;
    static constexpr uint32_t kPitch = 0x429;
    static constexpr uint32_t kTick = 0x42a;
    static constexpr uint32_t kFilterAttack = 0x40c;
    static constexpr uint32_t kFilterDecay = 0x40d;
    static constexpr uint32_t kStage2Attack = 0x414;
    static constexpr uint32_t kStage2Decay = 0x415;
    static constexpr uint32_t kStage2BaseOffset = 0x416;
    static constexpr uint32_t kStage2WidthOffset = 0x417;
    std::array<uint32_t, 8> machineWords{};
    std::array<uint32_t, 4> ampWords{};
    uint32_t volumeWord = 127u << 16;
    uint32_t panWord = 64u << 16;
    uint32_t tempoWord = 120;
    uint32_t triggerWord = 0;
    uint64_t pitchWord = 0;
    void setMachine(const std::array<float, 8>& values) noexcept {
        for (size_t i = 0; i < machineWords.size(); ++i) {
            const float v = values[i] < 0.0f ? 0.0f : (values[i] > 127.0f ? 127.0f : values[i]);
            machineWords[i] = static_cast<uint32_t>(v) << 16;
        }
    }
    void setAmp(int attack, int hold, int decay, int release, int volume, int pan) noexcept {
        const auto word=[](int v) noexcept { return static_cast<uint32_t>(v < 0 ? 0 : (v > 127 ? 127 : v)) << 16; };
        ampWords = {word(attack), word(hold), word(decay), word(release)};
        volumeWord = word(volume); panWord = word(pan);
    }
};
} // namespace monomachine::fm_try4_voice
