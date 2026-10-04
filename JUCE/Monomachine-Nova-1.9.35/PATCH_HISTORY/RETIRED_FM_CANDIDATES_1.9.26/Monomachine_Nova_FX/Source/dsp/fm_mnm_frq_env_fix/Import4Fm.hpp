// Isolated MnmFrqEnvFix import. Derived solely from the user-named
// `4 fix pitch+env fm` package; this route does not include retained MNM/OLD code.
// =============================================================================
// MnmFrqEnvFix imported package-4 FM wrapper — FM+STAT / FM+PAR / FM+DYN integration core (Monomachine Nova)
//
// PACK 8 ("FM CLOSED") — plugin-level fixes, measured not guessed:
//
//  1. BIT-EXACT CORES (pack 7, unchanged): all three machines render through
//     MnmFmStat/MnmFmPar/MnmFmDyn — 100% vs the original OS 1.32B on the
//     dsp56300 emulator (STAT 12800 out + 40000 state words, PAR 7168,
//     DYN 8192; 168-set knob sweep 177 408 words, 0 mismatches).
//     This removes the OLD float engine whose FM+DYN wave read the DECAY-RATE
//     table as a waveform (staircase = the "bitcrush" sound) and whose
//     operator envelopes were generic AHDSR approximations.
//
//  2. HOST SAMPLE-RATE LOCK (new in pack 8): the cores run on the MACHINE's
//     audio clock: one proc() = 32 mono samples at 44100 Hz (measured in
//     scripts/fm_knob_sweep.py: zc/2/(n/SR) with SR=44100 matched the table
//     ratios by sideband spectrometry). The old wrapper fed core blocks to
//     the host 1:1, so at a 48 kHz host EVERYTHING (pitch, operator
//     envelopes, TONE) ran 8.84% fast. Now the wrapper keeps a 44.1 kHz
//     core timeline and linearly resamples to the host rate. At 44.1 kHz
//     the path is 1:1 (no interpolation, bit-exact as before).
//
//  3. PITCH LAW (pack 7, unchanged): acc = golden table captured from the OS
//     itself, tune word int16(65536*tune/440) with the OS's 65535 clamp,
//     bottom clamp kFmAccFloor. Bit-exact vs OS at 440 Hz, <=0.3 cent
//     elsewhere (OS's own LUT half-quantum).
//
//  4. TONE is INSIDE the exact cores (the firmware's own $144AC7 one-pole),
//     so the wrapper's post filter stays open — the "tone cuts highs
//     differently" complaint was the old double/incorrect filtering.
//
//  5. THE TRACK AMP ENVELOPE IS NO LONGER HERE. The firmware's track
//     envelope is a separate engine (P:$088E-$08D7) shared by every machine;
//     pack 8 installs its bit-exact port (MnmEnvExact.hpp) into
//     nova::AmpEnvelope. The FmCore-internal env below stays bypassed for
//     API compatibility only.
// =============================================================================
#pragma once

#include "MnmFrqEnvFixSupport.hpp"

#include "Import4FmDsp.hpp"
#include "Import4FmPitch.h"
#include "Import4FmPar.hpp"
#include "Import4FmDyn.hpp"
#include "Import4FmStat.hpp"

#include <algorithm>
#include <array>
#include <cmath>

namespace monomachine {
namespace fm_mnm_frq_env_fix {

enum class FmKind { Stat, Par, Dyn };

// Parameter order is the hardware order from the ColdFire descriptors:
//  STAT: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE   (defaults 3C 40 50 1E 50 40 62 40)
//  PAR:  1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE  (defaults 3C 40 50 40 66 50 62 40)
//  DYN:  1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB  TUNE   (defaults 40 40 40 40 4A 50 1E 40)

// Ratio table Y:$141A80, 24 entries (raw/0x80000), dumped from dsp1_pmem.bin.
// The OS SCREEN labels are the audio ratio divided by 2 (raw 60 -> "1/2",
// 80 -> "1", 102 -> "2"); see PluginEditor patch of pack 8.
inline constexpr std::array<float, 24> kFmRatioExact = {
    0.03125f, 0.0625f, 0.125f, 0.1875f, 0.25f, 0.3125f,
    0.375f, 0.5f, 0.625f, 0.75f, 0.875f, 1.0f,
    1.25f, 1.5f, 1.75f, 2.0f, 2.5f, 3.0f,
    3.5f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
};

class FmCore {
public:
    void reset(double sampleRate) {
        hostSR = sampleRate > 0.0 ? sampleRate : kDspRate;
        tone.setSampleRate(hostSR);
        env.reset();
        lastOut = 0.0f;
        parCore.init();
        dynCore.init();
        statCore.init();
        coreLen = 0;
        readPos = 0.0;
        // exact 1:1 shortcut only when the host really runs at the machine rate
        oneToOne = std::fabs(hostSR - kDspRate) < 1e-9;
        step44 = kDspRate / hostSR;
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
        coreLen = 0;
        readPos = 0.0;
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

    // Renders `frames` host samples, mono. All three machines go through the
    // bit-exact cores on a 44.1 kHz core timeline.
    void processBlock(float* out, int frames) {
        if (oneToOne) {
            int done = 0;
            while (done < frames) {
                if (coreLen == 0) renderCore32();
                const int n = std::min(frames - done, coreLen);
                for (int i = 0; i < n; ++i) {
                    out[done + i] = coreBuf[(size_t)i];
                    lastOut = coreBuf[(size_t)i];
                }
                consumeCore(n);
                done += n;
            }
            return;
        }
        // Host SR != 44.1 kHz: keep the core on its native clock, resample.
        int done = 0;
        while (done < frames) {
            // guarantee floor(readPos)+1 and a lookahead sample exist
            while (coreLen < (int)readPos + 3) renderCore32();
            const int i0 = (int)readPos;
            const double frac = readPos - (double)i0;
            const float a = coreBuf[(size_t)i0];
            const float b = coreBuf[(size_t)i0 + 1];
            const float v = a + (b - a) * (float)frac;
            out[done++] = v;
            lastOut = v;
            readPos += step44;
            consumeCore((int)readPos);                   // pop whole consumed samples
            readPos -= (double)(int)readPos;
        }
    }

    float lastValue() const { return lastOut; }

private:
    // One exact-core pass: CONF + PROC on the machine's own 32-sample block,
    // Q23 words -> float (normalised to the measured default-knob peak).
    void renderCore32() {
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
        // Q23 word -> float: sign-extend 24-bit first, then normalise
        // to the measured default-knob peak (2.14 Q23 at descriptor
        // defaults for all 3 machines).
        for (int i = 0; i < 32; ++i) {
            const int32_t sw = (int32_t)(fifoBuf[i] << 8) >> 8;
            coreBuf[(size_t)(coreLen + i)] = static_cast<float>(sw) * (1.0f / 17949485.0f);
        }
        coreLen += 32;
    }
    void consumeCore(int n) {
        if (n <= 0) return;
        n = std::min(n, coreLen);
        for (int i = n; i < coreLen; ++i) coreBuf[(size_t)(i - n)] = coreBuf[(size_t)i];
        coreLen -= n;
    }

    // ------------------------------------------------------- helpers
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
    ImportEnvelopeCompat env;
    ImportToneBypass tone;
    mnmfrqenvfm::MnmFmPar parCore;
    mnmfrqenvfm::MnmFmDyn dynCore;
    mnmfrqenvfm::MnmFmStat statCore;
    uint32_t fifoBuf[32]{};
    float coreBuf[128]{};                       // core-stream buffer (32 per proc)
    int coreLen = 0;                            // samples in coreBuf
    double readPos = 0.0;                       // host read position (core samples)
    bool oneToOne = true;
    double step44 = 1.0;                        // core samples per host sample
    float lastOut = 0.0f, vel = 1.0f, note = 60.0f, pitchMod = 0.0f;
    uint32_t pitchWordOverride = 0;
    bool pitchWordValid = false;
    bool envelopeBypass = true;
    double hostSR = kDspRate;
    double masterTuneHz = 440.0;
};

}  // namespace fm_mnm_frq_env_fix
}  // namespace monomachine
