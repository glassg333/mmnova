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
//     pitch word).
//
// ALL THREE machines now render through the BIT-EXACT transcribed cores:
//   MnmFmPar.hpp  (m9)  — 7168/7168 output words  = 100% vs emulator
//   MnmFmDyn.hpp  (m10) — 8192/8192 output words  = 100% vs emulator
//   MnmFmStat.hpp (m8)  — 12800/12800 output + 40000/40000 state words = 100%
//                         (25 knob sets x 16 blocks, incl. gate edges 63/64/65,
//                         ratio-table edges, 1FB max, TONE 0..127, A=0)
// verified against scripts/dsp_emu.py running the original OS 1.32 firmware
// (worklog tasks 27, 38, 40). Their knob laws are exact by construction.
//
// The TONE knob is implemented INSIDE the exact cores (the $144AC7 one-pole
// of the firmware itself), so the wrapper's post tone stage is kept OPEN —
// otherwise the tone would be applied twice.
// ---------------------------------------------------------------------------
// PITCH LAW (iteration 44, measured from the ORIGINAL OS on the dsp56300
// emulator — the "Monomodule" etalon runs the real firmware, we captured the
// 48-bit accumulator A on every machine PROC entry $145D21/$145EC9/$14619D):
//
//   * The kernel feeds ALL THREE FM machines the SAME 48-bit acc, a LINEAR
//     frequency word (exactly doubles per octave, ~f * 2^24).
//   * The OLD wrapper fed (int)noteToHz(note) = 1x Hz. Measured machine law:
//     the acc the OS sends for note 69 / tune 440 is 0x1B7CD1771 (~440*2^24),
//     so feeding 440 made the plugin render an OCTAVE LOW.
//   * The exact integers are now shipped in MnmFmPitch.h (kFmGoldenAcc[128],
//     captured from the OS itself): bit-exact pitch at master tune 440.
//     Other tunes / pitch bend scale the anchor by 2^(delta/2048 octave),
//     the OS's own pitch-word quantisation (1 unit = 0.586 cents).
//   * Negative pitch index is CLAMPED by the OS to kFmAccFloor (measured:
//     tune 400 Hz, low notes all render the same frozen-low pitch).
// =============================================================================
#pragma once

#include "MnmKernel.hpp"

#include "MnmFmDsp.hpp"
#include "MnmFmPitch.h"
#include "MnmFmPar.hpp"
#include "MnmFmDyn.hpp"
#include "MnmFmStat.hpp"

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
        env.reset();
        lastOut = 0.0f;
        parCore.init();
        dynCore.init();
        statCore.init();
        fifoLen = 0;
    }

    // Master tune in Hz (400..440 like the hardware); scales the pitch acc.
    void setMasterTuneHz(double hz) {
        masterTuneHz = (hz >= 300.0 && hz <= 500.0) ? hz : 440.0;
    }
    double getMasterTuneHz() const { return masterTuneHz; }

    void noteOn(float midiNote, float velocity = 1.0f) {
        note = midiNote;
        vel = velocity;
        lastOut = 0.0f;
        env.trigger();
        parCore.init();
        dynCore.init();
        statCore.init();
        fifoLen = 0;
    }
    void noteOff() { env.release(); }
    void setEnvelope(float atk, float dec, float rel, float sustainLevel) {
        env.setParameters(atk, dec, rel, sustainLevel);
    }

    void setParameters(FmKind k, const std::array<float, 8>& p) {
        kind = k;
        params = p;
        // TONE is rendered INSIDE the bit-exact cores (firmware's own $144AC7
        // one-pole); the wrapper's post stage stays open to avoid double filtering.
        tone.setTone(127.0f);
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

    // Renders frames, mono. All three machines go through the bit-exact cores.
    void processBlock(float* out, int frames) {
        processExact(out, frames);
    }

    float lastValue() const { return lastOut; }

private:
    // ------------------------------------------------------- exact core path
    // The transcribed cores render 32-sample blocks: CONF + PROC per block,
    // output 32 words (16 mono L/R pairs). A small FIFO carries partial
    // host blocks. Pitch = the 48-bit acc the OS kernel itself puts on the
    // machine's PROC entry (kFmGoldenAcc), or the direct override
    // (incl. 0 = frozen carrier, as in the original).
    void processExact(float* out, int frames) {
        int done = 0;
        while (done < frames) {
            if (fifoLen == 0) {
                const int64_t pitchA = pitchWordValid
                    ? (int64_t)pitchWordOverride
                    : accForNote(note + pitchMod);
                if (kind == FmKind::Par) {
                    parCore.conf(knobWords.data());
                    parCore.proc(pitchA, fifoBuf);
                } else if (kind == FmKind::Dyn) {
                    dynCore.conf(knobWords.data());
                    dynCore.proc(pitchA, fifoBuf);
                } else {
                    statCore.conf(knobWords.data());
                    statCore.proc(pitchA, fifoBuf);
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

    // ------------------------------------------------------- helpers
    // REGISTER-MEASURED ratio law (both fetch sites $145D38/$145DAD):
    // n = floor(((K<<16) + $8000) * 48 / 2^24); now implemented word-exact
    // inside MnmFmStat.hpp. Kept here as the documented reference.
    static int ratioIndexExact(float k0to127) {
        const int K = static_cast<int>(std::clamp(k0to127, 0.0f, 127.0f));
        const int n = (((K << 16) + 0x8000) * 48) >> 24;
        return std::clamp(n, 0, 23);
    }

    // OS tune word: int16 of the u16 counter (65536*tuneHz/440, clamped to
    // 65535 like the hardware's byte order: at 440 Hz the OS sends -1).
    static int osTuneWord(double tuneHz) {
        long v = std::lround(65536.0 * tuneHz / 440.0);
        if (v > 65535) v = 65535;
        if (v > 32767) v -= 65536;
        return (int)v;
    }

public:
    // The 48-bit acc the OS kernel feeds the FM machines for note `noteFloat`
    // (MIDI note + pitch bend/pitch-mod in semitones). Bit-exact vs the OS at
    // master tune 440 Hz; other tunes/bend scale the anchor by the OS's own
    // pitch-word quantum (1/2048 octave units). Exposed for the bit-exact tests.
    //
    // kFmIdxOffset: the kernel's internal pitch index runs +2 semitones
    // (341.333 units) above the host pitch word — measured on the OS (the
    // golden acc for note 0 corresponds to index 340.3, not -1). It cancels
    // in the anchor ratio; it only moves the bottom clamp threshold.
    static constexpr double kFmIdxOffset = 2048.0 * 2.0 / 12.0;   // +2 semitones

    int64_t accForNote(double noteFloat) const {
        const int tw = osTuneWord(masterTuneHz);
        // OS pitch word 41 = (note<<11)/12 with C integer division (floor).
        double idxF = std::floor(noteFloat * (2048.0 / 12.0)) + (double)tw + kFmIdxOffset;
        if (idxF <= 0.0) return kFmAccFloor;       // OS clamp (measured)
        int anchor = (int)std::lround(noteFloat);
        anchor = std::clamp(anchor, 0, 127);
        const double idxAnchor = (double)((long)anchor * 2048L / 12L) + (double)kFmTuneWord440 + kFmIdxOffset;
        const double ratio = std::pow(2.0, (idxF - idxAnchor) / 2048.0);
        return (int64_t)std::llround((double)kFmGoldenAcc[anchor] * ratio);
    }

private:

    FmKind kind = FmKind::Stat;
    std::array<float, 8> params{};
    std::array<uint32_t, 8> knobWords{};
    AmpEnvelope env;
    ToneLowpass tone;
    mnmfm::MnmFmPar parCore;
    mnmfm::MnmFmDyn dynCore;
    mnmfm::MnmFmStat statCore;
    uint32_t fifoBuf[32]{};
    int fifoLen = 0, fifoPos = 0;
    float lastOut = 0.0f, vel = 1.0f, note = 60.0f, pitchMod = 0.0f;
    uint32_t pitchWordOverride = 0;
    bool pitchWordValid = false;
    bool envelopeBypass = true;
    double sr = kDspRate;
    double masterTuneHz = 440.0;
};

}  // namespace mnm
}  // namespace monomachine
