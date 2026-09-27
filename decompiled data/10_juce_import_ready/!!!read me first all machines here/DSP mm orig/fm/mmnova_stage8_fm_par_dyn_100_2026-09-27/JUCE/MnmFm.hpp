// FM+STAT / FM+PAR / FM+DYN ("mnm" cores) -- Monomachine Nova 1.6.0
//
// Recovered structure, tables and parameter laws. Addresses point at the original
// DSP56300 code so every claim can be re-checked.
//
// Confirmed from the listings (see decompiled data/.../listings/fm_*_full.txt):
//  * dispatch:        init/config/process  FM+STAT $145D12/$145D1D/$145D21,
//                     FM+PAR $145EC9/$145ED4/$145EDB, FM+DYN $14619D/$1461A8/$1461C1
//  * oscillator:      X:$14A000 sine, 13-bit phase mask ($1FFF, `move #>$1fff,m2`
//                     P:$145D2A) read with linear interpolation
//                     (mpysu/add/macsu/asr at P:$145E5D-$145E70)
//  * operator ratio:  P:$141A80 table, 24 entries, firmware index
//                     n = ((param + 0x8000) * 24) >> 24   (P:$145DA5-$145DAD)
//                     ratio = raw / 0x80000  ->  1/32 ... 8
//  * fine detune:     1FIN is centred at 0x400000 and scaled by 1/4
//                     (P:$145DAE-$145DB7) -> ratio multiplier 0.75 ... 1.246
//  * phase scaling:   modulator output is shifted left by 3 (P:$145DBC) before it is
//                     added to the carrier phase
//  * feedback:        FM+DYN config P:$1461AB-$1461BF -> feedback gain ~ 4 * (2FB-64) * 1VOL
//  * TONE:            shared one-pole LP through P:$144AC7 (P:$145D80-$145D8B)
//  * FM+DYN wave:     reads the P:$141880 curve (the same 128-word table the AMP decay
//                     stage uses) and X:$140000 (pitch table) for its dynamic scan.
//
// BIT-EXACT STATUS (see worklog + 22_fm_machines research pack):
//  * FM+PAR (m9): full instruction transcription P:%EC9-ı9C, validated
//    word-for-word against the emulator (14 knob sets x 16 blocks = 29568 words).
//  * FM+DYN (m10): full instruction transcription P:ı9D-ĳ6E, validated
//    (16 knob sets x 16 blocks = 33792 words).
//  * FM+STAT (m8): knob laws verified in iteration 27, the render path below is
//    still the recovered float core (transcription pass pending).
// The amplitude envelope is the kernel AMP stage (post-voice), exactly as in the
// firmware: the machines' own CONF resets their droop states every block.
#pragma once

#include "MnmKernel.hpp"
#include "MnmFmPar.hpp"
#include "MnmFmDyn.hpp"

#include <array>
#include <cmath>

namespace monomachine {
namespace mnm {

enum class FmKind { Stat, Par, Dyn };

// Parameter order is the hardware order from the ColdFire descriptors
// (descriptors/all_machine_descriptors.json -> fm_descriptors.json):
//  STAT: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE   (defaults 3C 40 50 1E 50 40 62 40)
//  PAR:  1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE  (defaults 3C 40 50 40 66 50 62 40)
//  DYN:  1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB  TUNE  (defaults 40 40 40 40 4A 50 1E 40)
class FmCore {
public:
    void reset(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : kDspRate;
        tone.setSampleRate(sr);
        phase.fill(0.0f);
        fbState = 0.0f;
        env.reset();
        lastOut = 0.0f;
        fifoPos = 0;
        parEx.init();                // bit-exact cores: firmware INIT
        dynEx.init();
    }

    void noteOn(float midiNote, float velocity = 1.0f) {
        note = midiNote;
        vel = velocity;
        phase.fill(0.0f);
        fbState = 0.0f;
        lastOut = 0.0f;
        env.trigger();
        fifoPos = 0;                 // drop stale exact-core samples
    }
    void noteOff() { env.release(); }
    void setEnvelope(float atk, float dec, float rel, float sustainLevel) {
        env.setParameters(atk, dec, rel, sustainLevel);
    }

    void setParameters(FmKind k, const std::array<float, 8>& p) {
        this->kind = k;
        params = p;
        // STAT/PAR have TONE in slot 6 and share the P:$144AC7 one-pole; FM+DYN has no TONE
        // knob (slot 6 is 2FB), so its post stage stays open.
        tone.setTone(kind == FmKind::Dyn ? 127.0f : p[6]);
    }
    void setPitchMod(float semitones) { pitchMod = semitones; }
    // The AMP page (kernel envelope, post-voice) drives the amplitude in the host chain, so
    // the core runs open by default; set false to use the core's own envelope.
    void setEnvelopeBypass(bool bypass) { envelopeBypass = bypass; }

    // Renders one 16-sample block, mono (the voice mixer places the block).
    // FM+PAR and FM+DYN run the BIT-EXACT firmware transcriptions
    // (MnmFmPar/MnmFmDyn, validated word-for-word against the DSP56300
    // emulator: 29568/29568 and 33792/33792 words). FM+STAT keeps the
    // recovered float core until its own transcription pass.
    void processBlock(float* out, int frames) {
        if (kind == FmKind::Par || kind == FmKind::Dyn) {
            processBlockExact(out, frames);
            return;
        }
        const float f0 = noteToHz(note + pitchMod + tuneSemitones());
        const float dt = static_cast<float>(f0 / sr);

        // Operator ratios. STAT/PAR quantise the two/three operator frequencies through the
        // hardware table; DYN uses a continuous ratio (its own "dynamic" character).
        float ratio1 = 1.0f, ratio2 = 1.0f, ratio3 = 1.0f;
        float depth1 = 0.0f, depth2 = 0.0f, depth3 = 0.0f, fb = 0.0f, waveScan = 0.0f;
        switch (kind) {
            case FmKind::Stat:
                ratio1 = kFmRatio[ratioIndex(params[0])] * fineMultiplier(params[1]);  // 1FRQ, 1FIN
                ratio2 = kFmRatio[ratioIndex(params[4])];                              // 2FRQ
                depth1 = indexDepth(params[2]);                                        // 1ENV
                depth2 = indexDepth(params[5]);                                        // 2VOL
                // 1FB: the 1-sample feedback structure (X:$80 / X:$C0) is confirmed, the
                // exact gain law is not, so the knob is mapped linearly over 0..4.
                fb = std::clamp(params[3], 0.0f, 127.0f) / 127.0f * 4.0f;
                break;
            case FmKind::Par:
                ratio1 = kFmRatio[ratioIndex(params[0])];
                ratio2 = kFmRatio[ratioIndex(params[2])];
                ratio3 = kFmRatio[ratioIndex(params[4])];
                depth1 = indexDepth(params[1]);
                depth2 = indexDepth(params[3]);
                depth3 = indexDepth(params[5]);
                break;
            case FmKind::Dyn: {
                ratio1 = continuousRatio(params[0]);                 // 1FRQ = continuous ratio
                ratio2 = continuousRatio(params[4]);                 // 2FRQ
                depth1 = indexDepth(params[2]) * (0.5f + levelScale(params[1]));  // 1VOL, 1FEN
                depth2 = levelScale(params[3]);                      // 1VEN
                // 2FB x 1VOL. 1.6.6: unipolar law per user ears -- 0 must give NO feedback
                // (the old bipolar map made 0 sound like full-scale feedback), 64 ~ half, 127 full.
                fb = (std::clamp(params[6], 0.0f, 127.0f) / 127.0f) * 4.0f * (params[2] / 64.0f);
                waveScan = (params[5] - 64.0f) / 64.0f;              // 2ENV drives the wave scan
                break;
            }
        }

        for (int i = 0; i < frames; ++i) {
            const float e = envelopeBypass ? 1.0f : env.tick();

            // Operator 1 (modulator). Feedback is taken from its own previous output.
            const float mod1Phase = phase[0] + fbState * fb * 0.25f;
            const float mod1 = SineTable::instance().read(mod1Phase) * e;

            // Operator 2 (second modulator for STAT/DYN, third parallel for PAR).
            const float mod2 = SineTable::instance().read(phase[1]) * e;

            float carrierPhase = 0.0f;
            switch (kind) {
                case FmKind::Stat:
                    // 2VOL modulates operator 1, 1ENV modulates the carrier (serial pair).
                    phase[1] = wrapf(phase[1] + dt * ratio2);
                    carrierPhase = phase[2] + (mod1 + mod2 * depth2) * depth1 * 8.0f;  // asl #$3, P:$145DBC
                    break;
                case FmKind::Par:
                    phase[1] = wrapf(phase[1] + dt * ratio2);
                    phase[3] = wrapf(phase[3] + dt * ratio3);
                    carrierPhase = phase[2] + (mod1 * depth1 + mod2 * depth2 +
                                               SineTable::instance().read(phase[3]) * depth3) * 8.0f;
                    break;
                case FmKind::Dyn: {
                    // Dynamic wave: the table index is swept by 2ENV; the curve is the
                    // firmware's P:$141880 monotonic table (not a sine).
                    const float scan = wrapf(phase[4] + 0.5f + waveScan * 0.5f);
                    const int idx = std::clamp(static_cast<int>(scan * 128.0f), 0, 127);
                    const float wave = kEnvDecayRate[static_cast<size_t>(idx)] * -1.0f;
                    phase[1] = wrapf(phase[1] + dt * ratio2);
                    carrierPhase = phase[2] + (mod1 * depth1 + wave * depth2) * 8.0f;
                    phase[4] = wrapf(phase[4] + dt * 0.25f);
                    break;
                }
            }

            const float carrier = SineTable::instance().read(carrierPhase);
            lastOut = carrier * vel;
            // Shared TONE low-pass (P:$144AC7), identical to the firmware's post stage.
            out[i] = tone.process(0, lastOut);

            phase[0] = wrapf(phase[0] + dt * ratio1);
            phase[2] = wrapf(phase[2] + dt);
            fbState = mod1;
        }
    }

    float lastValue() const { return lastOut; }

private:
    // ---- bit-exact FM+PAR / FM+DYN path ---------------------------------------
    // The firmware machine is one INIT + per-block CONF + PROC over a 16-frame
    // block (32 output words = 16 mono L/R pairs, Q23, +-16 raw).  Pitch enters
    // as the kernel pitch word A; verified machine law: f = A Hz (measured
    // f/A = 1.000 +-0.03 on the vectors at A = 1000..8000).  TUNE is additive
    // in the 2048-units/octave pitch-word domain, i.e. +3.127 cents/step,
    // unipolar (knob 64 = +200 cents).  A 16-frame FIFO keeps output aligned
    // when the host asks for partial blocks.
    void processBlockExact(float* out, int frames) {
        uint32_t knob[8];
        for (int i = 0; i < 8; ++i)
            knob[i] = (uint32_t)std::lround(std::clamp(params[(size_t)i], 0.0f, 127.0f));

        const float f0 = 440.0f * std::pow(2.0f, (note + pitchMod - 69.0f) / 12.0f);
        // Machine law (measured on the vectors): f = A Hz (inc48 = 2*$0BE37C*A,
        // 32 carrier steps per 16-frame block at 2x internal rate).
        // TUNE (kernel $02C0): ADDITIVE in the 2048-units/octave pitch-word
        // domain => multiplicative in f: +5.336 units/step = +3.127 cents/step,
        // unipolar 0..+397 cents (knob 64 = +200 cents, the firmware default).
        const float tuneOct = static_cast<float>(knob[7]) * 5.336f / 2048.0f;
        int32_t A = static_cast<int32_t>(std::lround(f0 * std::exp2(tuneOct)));
        A = std::clamp(A, 0, 16744);               // kernel note clamp $5800 units -> f(note 132)

        int done = 0;
        while (done < frames) {
            if (fifoPos == 0) {
                if (kind == FmKind::Par) {
                    parEx.conf(knob);
                    parEx.proc((uint32_t)A, fifoBuf);
                } else {
                    dynEx.conf(knob);
                    dynEx.proc((uint32_t)A, fifoBuf);
                }
                fifoPos = 16;
            }
            const int n = std::min(fifoPos, frames - done);
            for (int i = 0; i < n; ++i) {
                const uint32_t p = fifoBuf[2 * (16 - fifoPos + i)];  // frame i = word pair (2i,2i+1), mono
                const float v = (p & 0x800000u)
                    ? (float)((int32_t)p - (1 << 24)) / 8388608.0f
                    : (float)(int32_t)p / 8388608.0f;
                out[done + i] = v;
            }
            fifoPos -= n;
            done += n;
        }
        lastOut = out[frames - 1];
    }

    mnmfm::MnmFmPar parEx;
    mnmfm::MnmFmDyn dynEx;
    uint32_t fifoBuf[32]{};      // full 32-word PROC output (16 L/R pairs)
    int fifoPos = 0;             // frames remaining in the buffer
    static float wrapf(float v) { return v - std::floor(v); }

    // P:$145DA5-$145DAD: n = ((param + 0x8000) * 24) >> 24 with a 24-bit knob word.
    static int ratioIndex(float param0to127) {
        const float word = std::clamp(param0to127, 0.0f, 127.0f) * (8388607.0f / 127.0f);
        const int n = static_cast<int>(((word + 32768.0f) * 24.0f) / 16777216.0f);
        return std::clamp(n, 0, 23);
    }
    // P:$145DAE-$145DB7: multiplier = 0.75 ... 1.246 around 0x400000.
    static float fineMultiplier(float fin0to127) {
        const float word = std::clamp(fin0to127, 0.0f, 127.0f) * (8388607.0f / 127.0f);
        const float x1 = 4194304.0f + (word - 4194304.0f) * 0.25f;
        return std::clamp(x1 / 4194304.0f, 0.75f, 1.25f);
    }
    // FM+DYN is "dynamic": its ratio knob is a continuous offset, not a table lookup.
    static float continuousRatio(float param0to127) {
        return std::exp2((param0to127 - 64.0f) / 64.0f * 2.0f);
    }
    // Modulation depth: the knob scales the modulator output that is added to the carrier
    // phase, which the firmware shifts left by 3 (P:$145DBC).
    static float indexDepth(float param0to127) {
        return std::clamp(param0to127, 0.0f, 127.0f) / 127.0f;
    }
    static float levelScale(float param0to127) {
        return std::clamp(param0to127, 0.0f, 127.0f) / 127.0f;
    }
    float tuneSemitones() const {
        // TUNE is the last slot of every FM descriptor; centred at 64.
        const float raw = (kind == FmKind::Dyn) ? params[7] : params[7];
        return (raw - 64.0f) / 64.0f * 2.0f;
    }

    FmKind kind = FmKind::Stat;
    std::array<float, 8> params{};
    std::array<float, 5> phase{};
    AmpEnvelope env;
    ToneLowpass tone;
    float fbState = 0.0f, lastOut = 0.0f, vel = 1.0f, note = 60.0f, pitchMod = 0.0f;
    bool envelopeBypass = true;
    double sr = kDspRate;
};

}  // namespace mnm
}  // namespace monomachine
