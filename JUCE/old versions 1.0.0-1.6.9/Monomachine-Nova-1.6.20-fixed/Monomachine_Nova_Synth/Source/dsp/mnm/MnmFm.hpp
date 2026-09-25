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
// Not yet instruction-exact (documented, not guessed): the exact 48-bit accumulator
// pipeline and the operator envelope tap points. The amplitude envelope reuses the
// recovered kernel AMP envelope (real rate tables) instead of the firmware's per-operator
// taps, and the parameter->index mapping is applied at block rate.
#pragma once

#include "MnmKernel.hpp"

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
    }

    void noteOn(float midiNote, float velocity = 1.0f) {
        note = midiNote;
        vel = velocity;
        phase.fill(0.0f);
        fbState = 0.0f;
        lastOut = 0.0f;
        env.trigger();
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
    void processBlock(float* out, int frames) {
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
