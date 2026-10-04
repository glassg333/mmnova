// =============================================================================
// MnmAmpEnv.hpp — Monomachine track AMP envelope + per-sample gain path,
// bit-exact transcription of the DSP1 kernel P:$088E-$08FA (OS 1.32B).
//
// Verified frame-by-frame against the real firmware on the bit-precise
// DSP56300 emulator (worklog task 43, vectors exp64_fm_pluginlevel.json.gz):
//
//   * state machine P:$088E-$08D7, one tick per 16-sample frame:
//       TRIG  Y[P+$28]: 1=note-on 2=note-off 3=kill
//         trig==1 -> phase := 0 (ATK); level is NOT reset (native retrigger)
//         trig>=2 -> phase := 3 (REL); trig==3 -> phase := 4 (KILL)
//       PHASE 0 ATK:  level += kMnmEnvAttack[ATK>>16];
//                     if sum >= 1.0: stored level = $7FFFFF (DSP data limiter,
//                     NOT a wrap to -1.0) and phase := 1, hold counter := 1
//       PHASE 1 HOLD: t0 = (HOLDword >= $10000) ? HOLDword : 0
//                     x1 = (t0 * tempoWord) >> 23            (frac mpy)
//                     thr = x1 * $791FD0 * 4                 (mpyi + asl, both <<1)
//                     counter++; fire DEC when (counter << 24) > thr
//       PHASE 2 DEC:  rateWord = DECword      (raw << 16)
//       PHASE 3 REL:  rateWord = RELword
//       PHASE 4 KILL: rateWord = $20 (raw!)
//       common:       idx = (rateWord + $FFFF) >> 16      (asr16 + rnd $800000)
//                     level = (-level * kMnmEnvDecay[idx]) >> 23   (frac mpy)
//                     DEC=127 -> factor -1.0 -> level frozen; KILL -> idx 1
//                     (0.904016/frame, -60dB in ~16 ms)  [all measured]
//
//   * gain path P:$08D8-$08FA, per 16-sample frame:
//       VOL^2q = (VOLword * VOLword) >> 23          (mpy x0,x0)
//       v      = (level * VOL^2q) >> 23
//       panIdx = (PANword * $408E05) >> 34          (mpy + asl + asr12)
//       L_t    = (v * cosTable[panIdx + 1]) >> 23   ($14A801 base, note +1!)
//       R_t    = (v * sinTable[panIdx]) >> 23       ($14A000 base)
//       16-sample linear ramp per channel with exact integer truncation:
//         out[i] = (prev << 24 + i * (2^20 * (target - prev))) >> 24, i = 0..15
//       (mac y0,x0,a with y0 = 1/16, stores latch the pre-mac accumulator;
//        the accumulator reaches the target exactly at i = 16)
//       pan 0 -> L = cos[1] = full, R = sin[0] = 0  =>  PAN 0 = hard LEFT.
//
//   * Emulator quirk (documented, NOT ported): the python dsp_emu stores
//     `move ab,l:aa` as B->X / A->Y (the real DSP56300 stores A->X / B->Y).
//     This swaps the two channel cells and would make every frame's gain ramp
//     start from the OTHER channel's previous value (a -41 dBFS 2.756 kHz
//     sawtooth). The C++ port uses the real-hardware (natural) semantics:
//     each channel ramps from its own previous target, which is the original
//     anti-click design. Targets themselves are swap-independent and are
//     verified bit-exact against the emulator's X[$FA]/Y[$FA] cells.
// =============================================================================
#pragma once
#include <cstdint>
#include <cmath>
#include <cstdlib>

#include "Try4VoiceAmpEnvTables.h"

namespace try4voicefm {

// ---------------------------------------------------------------------------
// Pan tables: 8192-entry math-recovered sin/cos, identical to the emulator's
// injected tables (round(sin/cos(2*pi*i/8192) * $7FFFFF)). Built once.
// ---------------------------------------------------------------------------
class MnmPanTables {
public:
    static const int32_t* sinTable() { build(); return s_sin; }
    static const int32_t* cosTable() { build(); return s_cos; }
private:
    static void build() {
        if (s_built) return;
        for (int i = 0; i < 8192; ++i) {
            const double ph = 2.0 * 3.14159265358979323846 * double(i) / 8192.0;
            double s = sin(ph) * 8388607.0;
            double c = cos(ph) * 8388607.0;
            s_sin[i] = int32_t(s >= 0 ? s + 0.5 : s - 0.5);
            s_cos[i] = int32_t(c >= 0 ? c + 0.5 : c - 0.5);
        }
        s_built = true;
    }
    static int32_t s_sin[8192];
    static int32_t s_cos[8192];
    static bool s_built;
};

// C++17 inline storage is required because this imported header is included by
// both PluginProcessor.cpp and PluginEditor.cpp in the VST3 target. Without
// inline, MSVC correctly reports LNK2005 for these three ODR definitions.
inline int32_t MnmPanTables::s_sin[8192];
inline int32_t MnmPanTables::s_cos[8192];
inline bool MnmPanTables::s_built = false;

// ---------------------------------------------------------------------------
// Envelope state machine (P:$088E-$08D7). level/counter are raw 24-bit words.
// ---------------------------------------------------------------------------
class MnmAmpEnv {
public:
    enum Phase : int { kAtk = 0, kHold = 1, kDec = 2, kRel = 3, kKill = 4 };

    int   phase   = kAtk;
    int32_t level = 0;      // Y[P+$D7], signed 24-bit (always >= 0 in practice)
    int32_t counter = 0;    // Y[P+$D8] hold counter

    void reset() { phase = kAtk; level = 0; counter = 0; }

    // trig: 1 = note-on, 2 = note-off, 3 = kill (the raw TRIG word semantics)
    void trig(int t) {
        if (t == 1) { phase = kAtk; return; }          // level kept (retrigger)
        if (t >= 2) phase = (t == 3) ? kKill : kRel;
    }

    // tempoWord = Y[P+$23] as written by the OS (TickRecip = $800000/tick;
    // the verification vectors used the raw BPM value 120 — the law is
    // input-agnostic and was verified against that exact input).
    void tick(int atkRaw, int holdRaw, int decRaw, int relRaw, int tempoWord) {
        switch (phase) {
        case kAtk: {
            // 08A0-08AD: inc = tblA[ATKword>>16]; level += inc; store a1
            // (data limiter clamps at $7FFFFF); E-flag -> phase 1, counter := 1
            const int32_t inc = kMnmEnvAttack[atkRaw & 0x7F];
            const int64_t sum = int64_t(level) + inc;
            if (sum >= 0x800000) {
                level = 0x7FFFFF;                       // data limiter, traced
                phase = kHold;
                counter = 1;
            } else {
                level = int32_t(sum);
            }
            break;
        }
        case kHold: {
            // 08B0-08C5
            const int32_t holdWord = holdRaw << 16;
            const int32_t t0 = (holdWord >= 0x10000) ? holdWord : 0;
            const int64_t x1 = (int64_t(t0) * tempoWord) >> 23;      // mpy
            const int64_t thr = x1 * 0x791FD0 * 4;                   // mpyi + asl
            const int64_t cnt = int64_t(counter) + 1;
            if ((cnt << 24) > thr) {
                phase = kDec;
            } else {
                counter = int32_t(cnt);
            }
            break;
        }
        case kDec: stepDecay(decRaw << 16); break;
        case kRel: stepDecay(relRaw << 16); break;
        case kKill: stepDecay(0x20); break;              // raw $20 word
        default: break;
        }
    }

    // |level| — the gain uses the absolute value (level stays >= 0 here, but
    // the firmware applies -x1 * tblB with the negation inside the mpy).
    int32_t absLevel() const { return level < 0 ? -level : level; }

private:
    // 08CE-08D7: idx = (rateWord + $FFFF) >> 16; level = -level * tblB[idx]
    void stepDecay(int32_t rateWord) {
        int idx;
        if (rateWord == 0x20) {                          // KILL: raw word $20
            idx = (0x20 + 0xFFFF) >> 16;                 // -> 1 (traced)
        } else {
            idx = (rateWord + 0xFFFF) >> 16;             // = raw for 0..127
        }
        if (idx < 0) idx = 0;
        if (idx > 127) idx = 127;
        const int64_t prod = int64_t(-level) * kMnmEnvDecay[idx];
        level = int32_t(prod >> 23);                     // frac mpy, a1 = prod>>23
        if (level > 0x7FFFFF) level = 0x7FFFFF;          // data limiter (defensive)
    }
};

// ---------------------------------------------------------------------------
// Per-frame gain path (P:$08D8-$08FA). Natural (real-hardware) semantics:
// each channel ramps from its own previous target to the new target.
// ---------------------------------------------------------------------------
class MnmGainPath {
public:
    int32_t prevL = 0;      // LEFT  target of the previous frame (cos channel)
    int32_t prevR = 0;      // RIGHT target of the previous frame (sin channel)

    void reset() { prevL = prevR = 0; }

    // sinTable/cosTable: 8192-entry math-recovered tables, identical to the
    // emulator's injected tables: round(sin/cos(2*pi*i/8192) * $7FFFFF).
    // ring: 32 words = 16 interleaved (L, R) gain pairs, raw Q23.
    void render(int32_t level, int volRaw, int panRaw,
                const int32_t* sinTable, const int32_t* cosTable,
                int32_t ring[32]) {
        const int32_t volWord = volRaw << 16;
        const int32_t panWord = panRaw << 16;
        const int64_t v2 = (int64_t(volWord) * volWord) >> 23;       // VOL^2
        const int64_t v = (int64_t(level) * v2) >> 23;               // level*VOL^2
        const int64_t panIdx = (int64_t(panWord) * 0x408E05) >> 34;
        int pidx = int(panIdx);
        if (pidx < 0) pidx = 0;
        if (pidx > 8191) pidx = 8191;                    // HW reads past the end
        const int32_t cosV = cosTable[pidx + 1];         // $14A801 base: +1!
        const int32_t sinV = sinTable[pidx];
        const int32_t Lt = int32_t((v * cosV) >> 23);
        const int32_t Rt = int32_t((v * sinV) >> 23);
        const int64_t dL = int64_t(Lt) - prevL;
        const int64_t dR = int64_t(Rt) - prevR;
        for (int i = 0; i < 16; ++i) {
            ring[2 * i]     = int32_t(((int64_t(prevL) << 24) + i * (dL << 20)) >> 24);
            ring[2 * i + 1] = int32_t(((int64_t(prevR) << 24) + i * (dR << 20)) >> 24);
        }
        prevL = Lt;
        prevR = Rt;
    }
};

// ---------------------------------------------------------------------------
// Kernel pitch chain ($0262-$02EA): pitch word -> A (Hz) at the PROC entry.
// Verified bit-exact on 216 grid points (w41 x TUNE x w30, worklog task 43):
//   tuneDelta = (tuneWord>>16) * 683 >> 7        [TUNE word, raw<<16]
//   modDelta  = ((w30word * $B000) >> 23) - $5800  [w30 = pitch mod word, rest 64]
//   word      = clamp(glide + tuneDelta + modDelta, 0, $5800)
//   A         = ($1D22A * ((kMnmPitchTable[word&2047] << 14) << (word>>11)
//                         >> 24)) >> 26
// glide = the portamento state (X[P+$6..$9]); with PORT=0 the plugin sets it
// to the new note instantly (native note-on behavior).
// ---------------------------------------------------------------------------
class MnmPitchChain {
public:
    // glideWord: the settled glide state = (note<<11)/12 (+ pitch mod in word
    // units = semitones*2048/12) when PORT = 0.
    // tuneRaw: 0..127 raw TUNE knob; the OS bipolarizes the PAGE word
    // ((knob-64)<<16), so the plugin passes (knob - 64) here.
    // w30Raw: 0..127 pitch-mod page word (rest = 64). One w30 step = 33/16
    // semitones; the host converts its pitch-mod semitones via
    // w30Raw = 64 + round(semitones * 16 / 33).
    // Full 48-bit accumulator A (a1:a0) at the PROC entry. The FM machines
    // multiply $0BE37C by the WHOLE accumulator (mpysu/dmac over a1:a0), so
    // the fractional low word a0 participates in the inc and MUST be kept.
    static int64_t computeA48(int glideWord, int tuneRaw, int w30Raw) {
        const int64_t tuneDelta = (int64_t(tuneRaw) * 683) >> 7;
        const int64_t w30word = int64_t(w30Raw) << 16;
        const int64_t modDelta = ((w30word * 0xB000) >> 23) - 0x5800;
        int64_t word = glideWord + tuneDelta + modDelta;
        if (word < 0) word = 0;
        if (word > 0x5800) word = 0x5800;
        const int frac = int(word) & 0x7FF;
        const int ip = int(word) >> 11;
        const int64_t x = ((int64_t(kMnmPitchTable[frac]) << 24) >> 10) << ip;
        const int64_t x0 = x >> 24;                      // move a,x0 (a1)
        // $1D22A mpy (acc = prod<<1) + asr 3  =>  acc = prod>>2 (48-bit)
        return (119338 * x0) >> 2;
    }
    // The integer Hz part (a1) — the human-visible tuning law.
    static int computeA(int glideWord, int tuneRaw, int w30Raw) {
        return int((computeA48(glideWord, tuneRaw, w30Raw) >> 24) & 0xFFFFFF);
    }
};

}  // namespace try4voicefm
