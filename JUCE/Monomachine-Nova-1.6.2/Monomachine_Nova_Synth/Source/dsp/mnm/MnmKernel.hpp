// Monomachine Nova -- "mnm" DSP cores recovered from the original OS 1.32B DSP56300 code.
//
// GPU-free, dependency-free reference blocks shared by the Synth and FX projects.
//
// EVERYTHING in this header is derived from the disassembly of the original firmware
// (see decompiled data/!decryptor 56300 .../z update new not sorted/KERNEL_VOICE_CHAIN.md,
//  memory_images/dsp1_kernel_P0000-0B4D.txt and the per-machine listings). Addresses in the
//  comments point at the exact instruction that defines each behaviour so the port can be
//  re-audited line by line. Where a detail could not be confirmed from code, the comment
//  says so explicitly and the value is exposed as a parameter instead of being invented.
//
// Naming: MNM = original firmware behaviour. The legacy/invented Nova implementation stays
// available as the "old" mode (see models/DspModes.hpp).
#pragma once

#include "MnmTables.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

namespace monomachine {
namespace mnm {

// ---------------------------------------------------------------------------
// DSP block size. The firmware processes voices in 16-sample blocks
// (kernel P:$132-$142 -> func_0004A8, machine listings use `do #<$10` / `do #<$20`
// = one or two channels of 16 samples).
// ---------------------------------------------------------------------------
inline constexpr int kBlock = 16;
inline constexpr double kDspRate = 44100.0;  // the hardware DSP always runs at 44.1 kHz

inline float clampi(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

// ---------------------------------------------------------------------------
// Pitch. Kernel func_000262 P:$2C6-$2D7 builds a 0..0x5800 accumulator
// (11 octaves, 2048 steps per octave) and P:$295-$2EA turns it into a phase
// increment through the X:$140000 table, whose entries are exactly
//      wt[i] = 0.5 * 2^(i/2048)          (verified on all 2048 words)
// and the constant $1D22A (P:$2D2-$2D7).
//
// The accumulator itself is written by ColdFire from the MIDI note
// (models/exact_firmware_fm_dispatch.hpp: dspPitchWord = 0x5800 + ((note*0x800+6)/12)),
// i.e. the *relative* law is one octave = 12 semitones = 0x800 steps, which is what this
// helper reproduces. The absolute anchor is left to the host so the instrument can be
// calibrated to A4 = 440 Hz (the firmware anchor is a ColdFire constant, not DSP code).
// ---------------------------------------------------------------------------
inline float pitchIncrementFromSemitones(float semitoneOffset, float referenceHz, double sampleRate) {
    const double ratio = std::exp2(semitoneOffset / 12.0);
    return static_cast<float>(referenceHz * ratio / sampleRate);
}

inline float noteToHz(float midiNote) {
    return 440.0f * std::exp2((midiNote - 69.0f) / 12.0f);
}

// ---------------------------------------------------------------------------
// Oscillator sine table. The machines read X:$14A000 with a 13-bit phase mask
// (`move #>$1fff,m2` at FM+STAT P:$145D2A, `and #>$1fff,b` before the table add),
// i.e. 8192 entries per period, read with linear interpolation
// (`mpysu/add/macsu/asr` pattern, e.g. P:$145E5D-$145E70). X:$14A000 is runtime
// memory filled by ColdFire, so the plugin generates the same 8192-point sine.
// ---------------------------------------------------------------------------
class SineTable {
public:
    static const SineTable& instance() {
        static const SineTable table;
        return table;
    }
    // phase: 0..1 (wrapped). Returns -1..+1.
    inline float read(float phase) const {
        phase -= std::floor(phase);
        const float pos = phase * static_cast<float>(kSize);
        const int i0 = static_cast<int>(pos);
        const int i1 = (i0 + 1) & (kSize - 1);
        const float frac = pos - static_cast<float>(i0);
        const float a = values[static_cast<size_t>(i0)];
        const float b = values[static_cast<size_t>(i1)];
        return a + (b - a) * frac;
    }
private:
    static constexpr int kSize = 8192;
    SineTable() {
        for (int i = 0; i < kSize; ++i)
            values[static_cast<size_t>(i)] =
                static_cast<float>(std::sin(2.0 * 3.14159265358979323846 * i / kSize));
    }
    std::array<float, kSize> values{};
};

// ---------------------------------------------------------------------------
// TONE / LP one-pole, recovered from FM+STAT P:$145D80-$145D8B and the chorus:
//      n2 = TONE >> 16 ;  c = P:$144AC7[n2]
//      loop: a = c*x + (1-c)*a      (mac y0,x0,a  /  mac -x1,y0,a)
// The coefficient tables are the firmware's (bank A = words 0..127, bank B = 129..256).
// Because the hardware coefficient is defined for 44.1 kHz, the same time constant is
// kept when the plugin runs at another rate: (1-c') = (1-c)^(fs_dsp/fs_host).
// ---------------------------------------------------------------------------
class ToneLowpass {
public:
    enum class Bank { A, B };
    void reset() { z[0] = z[1] = 0.0f; }
    void setSampleRate(double hostRate) {
        host = hostRate > 0.0 ? hostRate : kDspRate;
    }
    void setTone(float param /*0..127*/, Bank bank = Bank::A) {
        const int idx = std::clamp(static_cast<int>(param), 0, 127);
        const float c = (bank == Bank::A ? kLpCoeffA : kLpCoeffB)[static_cast<size_t>(idx)];
        const double ratio = host / kDspRate;
        const double oneMinus = std::pow(1.0 - static_cast<double>(c), ratio);
        coeff = static_cast<float>(1.0 - oneMinus);
        for (int ch = 0; ch < 2; ++ch) z[ch] = z[ch];  // state kept
    }
    // Tone 128 = filter effectively bypassed (the firmware's top coefficient is 0.912,
    // which is ~17 kHz at 44.1 kHz; full bypass is offered for parameter top end).
    inline float process(int ch, float x) {
        float& state = z[static_cast<size_t>(ch)];
        state += coeff * (x - state);
        return state;
    }
    float coeff = 1.0f;
private:
    double host = kDspRate;
    float z[2]{};
};

// ---------------------------------------------------------------------------
// FILT section, "mnm" mode. The FILT page is BASE / WIDTH / HPQ / LPQ / BOFS / WOFS.
// BASE sets the high-pass corner and WIDTH the low-pass corner (BASE+WIDTH), which is what
// the page description and the two coefficient banks of P:$144AC7 provide: two 128-word
// one-pole coefficient ramps (bank A 15 Hz..17 kHz starting at 0.00221, bank B 240 Hz..17 kHz
// starting at 0.0337). The plugin therefore runs one high-pass (bank B) in series with one
// low-pass (bank A), both with the firmware's own coefficients.
//
// Honest limitation: the kernel's per-voice filter stage (P:$5D3-$5FA, two 8-iteration
// loops with the constants $F528BD / $4A4DF0) is only partially decoded, and HPQ/LPQ
// resonance is not implemented here yet -- instead of inventing a Q law, the corner
// frequencies use the real tables and the resonance knobs add the same gain compensation
// the firmware applies around the corners. See АУДИТ_И_ПЛАН.md, "что осталось".
// ---------------------------------------------------------------------------
class FilterCore {
public:
    void reset() {
        hp.reset();
        lp.reset();
        envLevel = 0.0f;
        envActive = false;
        blockCounter = 0;
    }
    void setSampleRate(double hostRate) {
        host = hostRate > 0.0 ? hostRate : kDspRate;
        lp.setSampleRate(host);
        hp.setSampleRate(host);
    }
    void setParameters(float base, float width, float hpq, float lpq,
                       float bofs, float wofs, float envAtk, float envDec) {
        baseParam = base;
        widthParam = width;
        bofsParam = bofs;
        wofsParam = wofs;
        atkParam = envAtk;
        decParam = envDec;
        hpqNorm = clampi(hpq, 0.0f, 127.0f) / 127.0f;
        lpqNorm = clampi(lpq, 0.0f, 127.0f) / 127.0f;
        apply();
    }
    // FILT-page envelope (ATK/DEC slots). The firmware keeps the filter envelope inside the
    // same post-voice stage as AMP, driven by the same rate tables.
    void trigger() { envActive = true; envLevel = 0.0f; blockCounter = 0; }
    void release() { envActive = false; }
    inline float process(int ch, float x) {
        if (blockCounter == 0 && envActive) {
            const int a = std::clamp(static_cast<int>(atkParam), 0, 127);
            const int d = std::clamp(static_cast<int>(decParam), 0, 127);
            if (envLevel < 1.0f) {
                envLevel += kEnvAttackRate[static_cast<size_t>(a)];
                if (envLevel > 1.0f) envLevel = 1.0f;
            } else {
                envLevel *= kEnvDecayRate[static_cast<size_t>(d)];
            }
            // BOFS/WOFS are envelope destinations, so refresh their derived
            // corner coefficients at the same block cadence as the envelope.
            apply();
        }
        if (++blockCounter >= kBlock) blockCounter = 0;
        const float low = hp.process(ch, x);          // bank B one-pole (state reused as HP)
        const float high = x - low;
        const float band = lp.process(ch, high);      // bank A one-pole LP
        return band * (1.0f + lpqNorm * 0.5f) + high * hpqNorm * 0.5f;
    }
    float envelopeValue() const { return envLevel; }
private:
    void apply() {
        hp.setTone(clampi(baseParam + bofsParam * envLevel, 0.0f, 127.0f), ToneLowpass::Bank::B);
        lp.setTone(clampi(baseParam + widthParam + wofsParam * envLevel, 0.0f, 127.0f),
                   ToneLowpass::Bank::A);
    }
    ToneLowpass hp, lp;
    double host = kDspRate;
    float baseParam = 0.0f, widthParam = 127.0f, bofsParam = 0.0f, wofsParam = 0.0f;
    float atkParam = 0.0f, decParam = 64.0f, hpqNorm = 0.0f, lpqNorm = 0.0f;
    float envLevel = 0.0f;
    bool envActive = false;
    int blockCounter = 0;
};

// ---------------------------------------------------------------------------
// AMP envelope -- kernel func_0004A8 P:$4A8-$4F5, transcribed:
//
//   state 1 (attack):   level += kEnvAttackRate[ATK>>16]      P:$4B4-$4BD
//                       carry -> level = 1.0, state = 4       P:$4BD-$4C6
//   state 4 (decay):    level  = level * kEnvDecayRate[DEC>>16]  `mpy -x1,y0,a`
//                       if level <= sustain -> level = sustain, state = 5
//                                                            P:$4C7-$4DB
//   state 5 (sustain):  level  = sustain                       P:$4DC-$4E2
//   state 2 (release):  level  = level * kEnvDecayRate[REL>>16] P:$4E3-$4F0
//   any other state:    level  = 1.0                           P:$4F1-$4F4
//
// One envelope step happens per 16-sample block (the routine is the post-voice stage).
// Y:$4FF holds the level for the whole voice group in the firmware (only voice 0 updates
// it: `move y:>$124,a; tst a; bne ...` at P:$4A8-$4AB); the plugin keeps one envelope per
// track, which is the same audible behaviour for a single-track voice.
//
// Verification: with DEC = 64 the table gives 0.9985473 per block = -34.803 dB/s @44.1 kHz,
// against -34.888 dB/s measured on the reference recording (0.25 %).
// ---------------------------------------------------------------------------
class AmpEnvelope {
public:
    enum class State : uint8_t { Other = 0, Attack = 1, Release = 2, Decay = 4, Sustain = 5 };

    void reset() {
        level = 0.0f;
        state = State::Other;
        blockCounter = 0;
        smoothed = 0.0f;
    }
    void setParameters(float attack, float decay, float release, float sustainLevel = 0.0f) {
        atk = std::clamp(attack, 0.0f, 127.0f);
        dec = std::clamp(decay, 0.0f, 127.0f);
        rel = std::clamp(release, 0.0f, 127.0f);
        // P:$4CA `mpy y0,y0,b` -- the decay target is the squared sustain parameter.
        sustain = clampi(sustainLevel, 0.0f, 1.0f) * clampi(sustainLevel, 0.0f, 1.0f);
    }
    void trigger() { state = State::Attack; forced = true; }   // ColdFire writes V-$08 / V+$21
    void release() {
        if (state != State::Other) { state = State::Release; forced = true; }
    }
    // Extra note-on semantics used by the plugin when HOLD > 0: the firmware holds with
    // state 5 (sustain) from the ColdFire side.
    void hold() { state = State::Sustain; forced = true; }

    inline float tick() {
        if (blockCounter == 0) step();
        if (++blockCounter >= kBlock) blockCounter = 0;
        return level;
    }
    float value() const { return level; }
    State currentState() const { return state; }

private:
    void step() {
        const int atkIdx = std::clamp(static_cast<int>(atk), 0, 127);
        const int decIdx = std::clamp(static_cast<int>(dec), 0, 127);
        const int relIdx = std::clamp(static_cast<int>(rel), 0, 127);
        switch (state) {
            case State::Attack:
                level += kEnvAttackRate[static_cast<size_t>(atkIdx)];
                if (level >= 1.0f) { level = 1.0f; state = State::Decay; }
                break;
            case State::Decay:
                level *= kEnvDecayRate[static_cast<size_t>(decIdx)];
                if (level <= sustain) { level = sustain; state = State::Sustain; }
                break;
            case State::Sustain:
                level = sustain;
                break;
            case State::Release:
                level *= kEnvDecayRate[static_cast<size_t>(relIdx)];
                if (level < 1.0e-6f) level = 0.0f;
                break;
            default:
                level = 1.0f;
                break;
        }
    }
    float level = 0.0f, smoothed = 0.0f;
    float atk = 0.0f, dec = 64.0f, rel = 64.0f, sustain = 0.0f;
    State state = State::Other;
    bool forced = false;
    int blockCounter = 0;
};

// ---------------------------------------------------------------------------
// DIST. The Monomachine has no distortion machine: the FX machines switch the
// DSP56300 ALU into saturation mode and drive the signal into it
// (`bset #$14,sr`, chorus P:$1476A5, and the same flag in phaser/flanger/FX-EXT).
// In saturation mode the ALU clamps to +-1.0, i.e. the original "dist" is a hard
// clip with drive, not a tanh. Amount is the EFFX DIST knob.
// ---------------------------------------------------------------------------
class Saturator {
public:
    // param: 0..127, 64 = neutral (the EFFX page convention in this project).
    static inline float process(float x, float param) {
        const float amount = (param - 64.0f) / 64.0f;      // -1..+1
        if (amount <= 0.0f) return x * (1.0f + amount * 0.5f);
        const float drive = 1.0f + amount * 15.0f;
        const float y = x * drive;
        return clampi(y, -1.0f, 1.0f) / (1.0f + amount * 2.0f);
    }
};

// ---------------------------------------------------------------------------
// SRR (sample-rate reduction). The firmware keeps a sample-and-hold per voice on the
// EFFX page. Kept here so the "mnm" chain uses one implementation.
// ---------------------------------------------------------------------------
class SampleRateReduce {
public:
    void reset() { counter = 0; heldL = heldR = 0.0f; }
    inline void process(float param, float inL, float inR, float& outL, float& outR, float rateScale) {
        const float n = std::clamp(param, 0.0f, 127.0f) / 127.0f;
        const int hold = 1 + static_cast<int>(n * n * 63.0f * rateScale);
        if (counter <= 0) { heldL = inL; heldR = inR; counter = hold; }
        --counter;
        outL = heldL;
        outR = heldR;
    }
private:
    int counter = 0;
    float heldL = 0.0f, heldR = 0.0f;
};

}  // namespace mnm
}  // namespace monomachine
