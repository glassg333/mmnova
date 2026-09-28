// FmExactNew.hpp — isolated opt-in FM+ STAT / PAR / DYN wrapper.
//
// This wrapper selectively adapts the reviewed 2026-09-28 FM core package for
// MODE SYNT = new.  It deliberately does not include dsp/mnm/MnmFm.hpp,
// MnmKernel.hpp, or any old FM engine.  Existing mnm and old routes own their
// separate state and source files.
#pragma once

#include "FmExactDsp.hpp"
#include "FmExactPar.hpp"
#include "FmExactDyn.hpp"
#include "FmExactStat.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

namespace monomachine {
namespace fm_new {

enum class FmExactKind { Stat, Par, Dyn };

// The raw exact cores process 32 DSP words at a time (16 duplicated mono
// frames).  This FIFO makes their output invariant to arbitrary host callback
// boundaries without borrowing the current mnm wrapper or its DSP state.
class FmExactCore {
public:
    void reset(double /*sampleRate*/) {
        lastOut = 0.0f;
        statCore.init();
        parCore.init();
        dynCore.init();
        fifoLen = 0;
        fifoPos = 0;
    }

    void noteOn(float midiNote) {
        note = midiNote;
        lastOut = 0.0f;
        statCore.init();
        parCore.init();
        dynCore.init();
        fifoLen = 0;
        fifoPos = 0;
    }

    void setParameters(FmExactKind nextKind, const std::array<float, 8>& parameters) {
        kind = nextKind;
        for (int i = 0; i < 8; ++i)
            knobWords[static_cast<size_t>(i)] = static_cast<uint32_t>(
                std::clamp(parameters[static_cast<size_t>(i)], 0.0f, 127.0f));
    }

    void setPitchMod(float semitones) { pitchMod = semitones; }

    // Public only for deterministic core/vector tests and a future verified
    // kernel pitch-word bridge.  A zero word is intentionally valid.
    void setPitchWordOverride(uint32_t word, bool valid = true) {
        pitchWordOverride = word;
        pitchWordValid = valid;
    }

    void processBlock(float* output, int frames) {
        int done = 0;
        while (done < frames) {
            if (fifoLen == 0) {
                const uint32_t pitchWord = pitchWordValid
                    ? pitchWordOverride
                    : static_cast<uint32_t>(std::clamp(noteToHz(note + pitchMod), 1.0f, 20000.0f));
                if (kind == FmExactKind::Stat) {
                    statCore.conf(knobWords.data());
                    statCore.proc(pitchWord, fifo.data());
                } else if (kind == FmExactKind::Par) {
                    parCore.conf(knobWords.data());
                    parCore.proc(pitchWord, fifo.data());
                } else {
                    dynCore.conf(knobWords.data());
                    dynCore.proc(pitchWord, fifo.data());
                }
                fifoPos = 0;
                fifoLen = static_cast<int>(fifo.size());
            }
            const int count = std::min(frames - done, fifoLen);
            for (int i = 0; i < count; ++i) {
                // Sign-extend the Q23 word without a signed shift or an
                // implementation-defined out-of-range integer conversion.
                const uint32_t word = fifo[static_cast<size_t>(fifoPos + i)] & 0x00ffffffu;
                const int32_t signedWord = (word & 0x00800000u)
                    ? static_cast<int32_t>(word) - (1 << 24)
                    : static_cast<int32_t>(word);
                const float value = static_cast<float>(signedWord) * (1.0f / 17949485.0f);
                output[done + i] = value;
                lastOut = value;
            }
            fifoPos += count;
            fifoLen -= count;
            done += count;
        }
    }

    float lastValue() const { return lastOut; }

private:
    static float noteToHz(float midi) {
        return 440.0f * std::pow(2.0f, (midi - 69.0f) / 12.0f);
    }

    FmExactKind kind = FmExactKind::Stat;
    std::array<uint32_t, 8> knobWords{};
    fmnew::MnmFmStat statCore;
    fmnew::MnmFmPar parCore;
    fmnew::MnmFmDyn dynCore;
    std::array<uint32_t, 32> fifo{};
    int fifoLen = 0;
    int fifoPos = 0;
    float note = 60.0f;
    float pitchMod = 0.0f;
    float lastOut = 0.0f;
    uint32_t pitchWordOverride = 0;
    bool pitchWordValid = false;
};

} // namespace fm_new
} // namespace monomachine
