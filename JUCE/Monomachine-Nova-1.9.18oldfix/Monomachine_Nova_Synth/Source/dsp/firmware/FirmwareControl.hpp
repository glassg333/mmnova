// Firmware runtime control contract.
// The plugin's existing pages/LFO/matrix/P-lock are authoritative.  A runtime
// receives their already-modulated 0..127 values in this compact control frame;
// it does not own a second hidden LFO or an alternate parameter bank.
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

namespace nova::firmware {

enum class Section : std::uint8_t {
    Synth = 0,
    Amp,
    Filter,
    Dist,
    Echo,
    SampleRate,
    VolPan,
    Count
};

using SectionMask = std::uint32_t;

constexpr SectionMask sectionBit(Section section) noexcept
{
    return SectionMask{1u} << static_cast<std::uint8_t>(section);
}

constexpr SectionMask kSynth = sectionBit(Section::Synth);
constexpr SectionMask kAmp = sectionBit(Section::Amp);
constexpr SectionMask kFilter = sectionBit(Section::Filter);
constexpr SectionMask kDist = sectionBit(Section::Dist);
constexpr SectionMask kEcho = sectionBit(Section::Echo);
constexpr SectionMask kSampleRate = sectionBit(Section::SampleRate);
constexpr SectionMask kVolPan = sectionBit(Section::VolPan);
constexpr SectionMask kWholeTrack = kSynth | kAmp | kFilter | kDist | kEcho | kSampleRate | kVolPan;

constexpr bool selects(SectionMask mask, Section section) noexcept
{
    return (mask & sectionBit(section)) != 0;
}

constexpr bool isWholeTrack(SectionMask mask) noexcept
{
    return (mask & kWholeTrack) == kWholeTrack;
}

inline std::uint8_t raw127(float value) noexcept
{
    if (!std::isfinite(value)) return 0;
    return static_cast<std::uint8_t>(std::clamp(std::lround(value), 0l, 127l));
}

inline std::array<std::uint8_t, 8> rawPage(const float* values) noexcept
{
    std::array<std::uint8_t, 8> out{};
    if (values == nullptr) return out;
    for (std::size_t i = 0; i < out.size(); ++i) out[i] = raw127(values[i]);
    return out;
}

// `pages` follows the physical current VST pages: SYN, AMP, FILT, EFFX.
// AMP contains DIST/VOL/PAN; EFFX contains SRR and the whole delay feedback
// family (DTIM/DSND/DFB/DBAS/DWID), so left/right/send/filter values retain
// their existing IDs and modulation targets.
struct ControlFrame {
    std::array<std::uint8_t, 8> synth{};
    std::array<std::uint8_t, 8> amp{};
    std::array<std::uint8_t, 8> filter{};
    std::array<std::uint8_t, 8> effect{};
    int machineId = 0;
    int midiNote = 60;
    float pitchSemitones = 0.0f;
    float velocity = 1.0f;
    double bpm = 120.0;
    std::uint64_t sequence = 0;
    SectionMask selected = 0;

    static ControlFrame fromModulatedPages(const float* pages32,
                                           int machine,
                                           int note,
                                           float pitch,
                                           float noteVelocity,
                                           double tempo,
                                           std::uint64_t sequenceNumber,
                                           SectionMask sections) noexcept
    {
        ControlFrame out;
        out.synth = rawPage(pages32);
        out.amp = rawPage(pages32 == nullptr ? nullptr : pages32 + 8);
        out.filter = rawPage(pages32 == nullptr ? nullptr : pages32 + 16);
        out.effect = rawPage(pages32 == nullptr ? nullptr : pages32 + 24);
        out.machineId = std::clamp(machine, 0, 127);
        out.midiNote = std::clamp(note, 0, 127);
        out.pitchSemitones = std::isfinite(pitch) ? pitch : 0.0f;
        out.velocity = std::clamp(std::isfinite(noteVelocity) ? noteVelocity : 0.0f, 0.0f, 1.0f);
        out.bpm = std::clamp(std::isfinite(tempo) ? tempo : 120.0, 20.0, 400.0);
        out.sequence = sequenceNumber;
        out.selected = sections & kWholeTrack;
        return out;
    }
};

// Firmware image loading and DSP execution deliberately remain separate: a
// valid reference does not imply a linked runtime adapter, and an unavailable
// adapter must fall back without changing a retained native processor.
enum class RuntimeAvailability : std::uint8_t {
    NoSource = 0,
    ReferenceInvalid,
    AdapterUnavailable,
    WholeTrackReady,
    StageHarnessReady,
    Faulted
};

inline const char* availabilityText(RuntimeAvailability value) noexcept
{
    switch (value) {
        case RuntimeAvailability::NoSource: return "no SysEx selected";
        case RuntimeAvailability::ReferenceInvalid: return "SysEx path/hash validation failed";
        case RuntimeAvailability::AdapterUnavailable: return "runtime adapter is not linked";
        case RuntimeAvailability::WholeTrackReady: return "whole firmware track ready";
        case RuntimeAvailability::StageHarnessReady: return "firmware stage rack ready";
        case RuntimeAvailability::Faulted: return "firmware runtime faulted";
        default: return "firmware runtime status unknown";
    }
}

inline bool canRenderWholeTrack(RuntimeAvailability value) noexcept
{
    return value == RuntimeAvailability::WholeTrackReady || value == RuntimeAvailability::StageHarnessReady;
}

inline bool canRenderSelectedSections(RuntimeAvailability value) noexcept
{
    return value == RuntimeAvailability::StageHarnessReady;
}

} // namespace nova::firmware
