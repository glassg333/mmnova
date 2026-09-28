// =============================================================================
// MnmFm.hpp — FM+STAT / FM+PAR / FM+DYN integration core (Monomachine Nova)
//
// WHAT CHANGED AND WHY (iteration 38, measured, not guessed):
//
// The user reported: "FM STAT in the original can lower the frequency to 0,
// in my port the minimum is 0.2". Root cause found by measuring the ORIGINAL
// firmware on the bit-precise DSP emulator (scripts/fm_knob_sweep.py,
// register-level capture of the table index at both fetch sites $145D38 and
// $145DAD):
//
//   * ORIGINAL ratio law (REGISTER-MEASURED, both operators identical):
//       n = floor(((K<<16) + $8000) * 48 / 2^24) = floor((K + 0.5) * 3 / 16)
//     K = knob 0..127 -> n = 0..23 -> ALL 24 entries of the ratio table
//     Y:$141A80 are reachable: 1/32, 1/16, 1/8, 3/16, 1/4, 5/16, 3/8, 1/2,
//     5/8, 3/4, 7/8, 1, 1.25, 1.5, 1.75, 2, 2.5, 3, 3.5, 4, 5, 6, 7, 8.
//     Knob 0 = 1/32 (0.03125). The frequency never reaches exactly 0 Hz
//     through the table; at the bottom of its range the 13-bit phase
//     accumulator quantises the operator into sub-audio (near-frozen) rates.
//
//   * THE OLD PORT USED THE HALVED LAW (missing the <<1 of the fractional
//     MPY): n = ((param+$8000)*24)>>24 -> only 12 entries reachable, knob 127
//     gave ratio 1.25 instead of 8.0. That is why "the knob values do not
//     match" and why the low end felt wrong.
//
//   * The UI display used a THIRD table (32-entry getFmListedRatio(raw/4),
//     1/64..12) that matches neither the audio path nor the firmware.
//     Patch for PluginEditor.cpp is shipped in README_FM_KNOB_FIX.md.
//
//   * 1ENV depth law measured: depth = (K/128)^2 (squared, from
//     `mpy x0,x0,a; asl #$2` at $145DFB/$145E14), NOT linear K/127.
//   * 2VOL is GATED below 64 (op2 silent; `cmp #>$400000,b` at $145D8C).
//   * 1FB depth = K^2 word law ($145DFB mpy x0,x0,a; asl #$2).
//   * TUNE is not read by the STAT PROC at all (applied by the kernel to the
//     pitch word); the float core keeps a centred pitch trim for usability.
//
// FM+PAR and FM+DYN now render through the BIT-EXACT transcribed cores
// (MnmFmPar.hpp / MnmFmDyn.hpp, verified 29568/29568 and 33792/33792 words
// = 100% against the emulator, worklog tasks 27). Their knob laws are exact
// by construction.
//
// FM+STAT still renders through the float core below, now with the exact
// knob laws. Instruction-exact STAT transcription is the next step.
// =============================================================================
#pragma once

#include "MnmKernel.hpp"

#include "MnmFmDsp.hpp"
#include "MnmFmPar.hpp"
#include "MnmFmDyn.hpp"

#include <array>
#include <cmath>

namespace monomachine {
namespace mnm {

enum class FmKind { Stat, Par, Dyn };

// Parameter order is the hardware order from the ColdFire descriptors:
//  STAT: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE   (defaults 3C 40 50 1E 50 40 62 40)
//  PAR:  1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE  (defaults 3C 40 50 40 66 50 62 40)
//  DYN:  1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB  TUNE   (defaults 40 40 40 40 4A 50 1E 40)

// Ratio table Y:$141A80, 24 entries (raw/0x80000), dumped from dsp1_pmem.bin.
inline constexpr std::array<float, 24> kFmRatioExact = {
    0.03125f, 0.0625f, 0.125f, 0.1875f, 0.25f, 0.3125f,
    0.375f, 0.5f, 0.625f, 0.75f, 0.875f, 1.0f,
    1.25f, 1.5f, 1.75f, 2.0f, 2.5f, 3.0f,
    3.5f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
};

class FmCore {
public:
    void reset(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : kDspRate;
        tone.setSampleRate(sr);
        phase.fill(0.0f);
        fbState = 0.0f;
        env.reset();
        lastOut = 0.0f;
        parCore.init();
        dynCore.init();
        fifoLen = 0;
    }

    void noteOn(float midiNote, float velocity = 1.0f) {
        note = midiNote;
        vel = velocity;
        phase.fill(0.0f);
        fbState = 0.0f;
        lastOut = 0.0f;
        env.trigger();
        parCore.init();
        dynCore.init();
        fifoLen = 0;
    }
    void noteOff() { env.release(); }
    void setEnvelope(float atk, float dec, float rel, float sustainLevel) {
        env.setParameters(atk, dec, rel, sustainLevel);
    }

    void setParameters(FmKind k, const std::array<float, 8>& p) {
        kind = k;
        params = p;
        // STAT/PAR have TONE in slot 6 and share the P:$144AC7 one-pole;
        // FM+DYN has no TONE knob (slot 6 is 2FB) -> post stage stays open.
        tone.setTone(kind == FmKind::Dyn ? 127.0f : p[6]);
        for (int i = 0; i < 8; ++i)
            knobWords[(size_t)i] = (uint32_t)std::clamp(p[(size_t)i], 0.0f, 127.0f);
    }
    void setPitchMod(float semitones) { pitchMod = semitones; }
    void setEnvelopeBypass(bool bypass) { envelopeBypass = bypass; }
    // Direct pitch-word override for the exact cores (the kernel pitch word,
    // register A on PROC entry). The original firmware accepts pitch word 0 =
    // frozen carrier (frequency truly 0) — use pitchWordValid to enable it.
    void setPitchWordOverride(uint32_t w, bool valid = true) {
        pitchWordOverride = w; pitchWordValid = valid;
    }

    // Renders frames, mono.
    void processBlock(float* out, int frames) {
        if (kind != FmKind::Stat) {
            processExact(out, frames);
            return;
        }
        processStatFloat(out, frames);
    }

    float lastValue() const { return lastOut; }

private:
    // ------------------------------------------------------------------ STAT
    // Exact knob laws (see header comment), float oscillators.
    void processStatFloat(float* out, int frames) {
        const float f0 = noteToHz(note + pitchMod);
        const float dt = static_cast<float>(f0 / sr);

        // 1FRQ/2FRQ: n = floor(((K<<16)+$8000)*48 / 2^24), table 1/32..8.
        const float ratio1 = kFmRatioExact[(size_t)ratioIndexExact(params[0])] *
                             fineWord(params[1]);                       // 1FRQ, 1FIN
        const float ratio2 = kFmRatioExact[(size_t)ratioIndexExact(params[4])];  // 2FRQ

        // 1ENV: depth = (K/128)^2 (squared law, $145DFB/$145E14).
        const float depth1 = squaredDepth(params[2]);
        // 2VOL: gated below 64 (op2 silent, $145D8C); above 64 the level is
        // smoothed by a one-pole ramp (float approximation: fixed gain).
        const bool op2On = params[5] >= 64.0f;
        const float depth2 = op2On ? 1.0f : 0.0f;
        // 1FB: K^2 word law ($145DFB), scaled into the phase feedback.
        const float fb = fbDepth(params[3]);

        for (int i = 0; i < frames; ++i) {
            const float e = envelopeBypass ? 1.0f : env.tick();

            const float mod1Phase = phase[0] + fbState * fb;
            const float mod1 = SineTable::instance().read(mod1Phase) * e;
            const float mod2 = SineTable::instance().read(phase[1]) * e;

            phase[1] = wrapf(phase[1] + dt * ratio2);
            const float carrierPhase = phase[2] + (mod1 + mod2 * depth2) * depth1 * 8.0f;

            const float carrier = SineTable::instance().read(carrierPhase);
            lastOut = carrier * vel;
            out[i] = tone.process(0, lastOut);

            phase[0] = wrapf(phase[0] + dt * ratio1);
            phase[2] = wrapf(phase[2] + dt);
            fbState = mod1;
        }
    }

    // --------------------------------------------------- exact PAR/DYN path
    // The transcribed cores render 32-sample blocks: CONF + PROC per block,
    // output 32 words (16 mono L/R pairs). A small FIFO carries partial
    // host blocks. Pitch word = note frequency in Hz (the kernel pitch word
    // law measured: f/A = 1.000 +- 0.03 for A = 1000..8000).
    void processExact(float* out, int frames) {
        int done = 0;
        while (done < frames) {
            if (fifoLen == 0) {
                const uint32_t pitchA = pitchWordValid
                    ? pitchWordOverride
                    : (uint32_t)std::clamp(noteToHz(note + pitchMod), 1.0f, 20000.0f);
                if (kind == FmKind::Par) {
                    parCore.conf(knobWords.data());
                    parCore.proc(pitchA, fifoBuf);
                } else {
                    dynCore.conf(knobWords.data());
                    dynCore.proc(pitchA, fifoBuf);
                }
                fifoLen = 32;
                fifoPos = 0;
            }
            const int n = std::min(frames - done, fifoLen);
            for (int i = 0; i < n; ++i) {
                // Q23 word -> float: sign-extend 24-bit first, then normalise
                // to the measured default-knob peak (2.14 Q23 at descriptor
                // defaults for all 3 machines).
                const int32_t sw = (int32_t)(fifoBuf[fifoPos + i] << 8) >> 8;
                const float v = static_cast<float>(sw) * (1.0f / 17949485.0f);
                out[done + i] = v;
                lastOut = v;
            }
            fifoPos += n;
            fifoLen -= n;
            done += n;
        }
    }

    // ------------------------------------------------------------- knob laws
    static float wrapf(float v) { return v - std::floor(v); }

    // REGISTER-MEASURED on the emulator at both fetch sites ($145D38/$145DAD):
    // n = floor(((K<<16) + $8000) * 48 / 2^24) = floor((K + 0.5) * 3 / 16).
    static int ratioIndexExact(float k0to127) {
        const int K = static_cast<int>(std::clamp(k0to127, 0.0f, 127.0f));
        const int n = (((K << 16) + 0x8000) * 48) >> 24;
        return std::clamp(n, 0, 23);
    }

    // 1FIN word-exact: w = ((K<<16) - $400000) >> 2 (arithmetic), w += $400000;
    // multiplier = w / $400000 -> 0.75 .. 1.2461, centre 1.0 at K=64.
    static float fineWord(float k0to127) {
        const int K = static_cast<int>(std::clamp(k0to127, 0.0f, 127.0f));
        int w = (K << 16) - 0x400000;
        w >>= 2;                     // asr #$2  ($145DB1)
        w += 0x400000;               // ($145DB2)
        return static_cast<float>(w) * (1.0f / 4194304.0f);
    }

    // 1ENV depth: (K/128)^2 — from `mpy x0,x0,a; asl #$2` ($145DFB/$145E14),
    // confirmed by the measured mix law mix = 1.885 * (K/128)^2 * sine_peak.
    static float squaredDepth(float k0to127) {
        const float x = std::clamp(k0to127, 0.0f, 127.0f) / 128.0f;
        return x * x;
    }

    // 1FB: K^2 word law; normalised so K=127 gives the measured peak feedback
    // depth (~0.5 cycle phase self-modulation at the top of the range).
    static float fbDepth(float k0to127) {
        const float x = std::clamp(k0to127, 0.0f, 127.0f) / 127.0f;
        return x * x * 0.5f;
    }

    static float noteToHz(float midi) {
        return 440.0f * std::pow(2.0f, (midi - 69.0f) / 12.0f);
    }

    FmKind kind = FmKind::Stat;
    std::array<float, 8> params{};
    std::array<uint32_t, 8> knobWords{};
    std::array<float, 5> phase{};
    AmpEnvelope env;
    ToneLowpass tone;
    mnmfm::MnmFmPar parCore;
    mnmfm::MnmFmDyn dynCore;
    uint32_t fifoBuf[32]{};
    int fifoLen = 0, fifoPos = 0;
    float fbState = 0.0f, lastOut = 0.0f, vel = 1.0f, note = 60.0f, pitchMod = 0.0f;
    uint32_t pitchWordOverride = 0;
    bool pitchWordValid = false;
    bool envelopeBypass = true;
    double sr = kDspRate;
};

}  // namespace mnm
}  // namespace monomachine
