// mnm_fm_exact.hpp — Monomachine FM machines (m8 FM+STAT, m9 FM+PAR, m10 FM+DYN)
// Exact firmware laws, iteration 27 + 28 corrections. Drop-in replacement for
// the knob laws of MnmFm.hpp (1.6.20). Every constant cites the firmware address.
//
// ITERATION-28 CORRECTIONS (see README §9):
//  - the code region $14636F-$1464FB is machine m3 SID (dispatch table
//    $10016B/$10018D/$1001AF entries 3 = $14636F/$14637A/$146382), NOT part of
//    m10; the old "1VEN 5-way shaper selector" was SID's WAVE knob ($1463E8).
//  - m10's cross-FM is structurally silent: fixed LP coeff $40/2^23 on every
//    mod path + the >>12 phase-FM quantizer => mix>>12 == 0 always (measured:
//    carrier dph identical at mods 0 vs max; output corr = 1.0000 across knobs).
//
// Machine contract (DSP): r6 = P+$28, params y:(r6+$4..$B) = knob<<16,
// A = pitch word on PROC entry, output = 32 words (16 L/R pairs).
//
// THIS HEADER IS MONO-VOICE; the carrier/topology section mirrors the firmware:
//   modulator sine -> first difference -> one-pole LP ($144AC7[TONE]) ->
//   peak shaper d[i] + G*d[i-1] (G = 16*max(0, K-64)/256, K = gated knob) ->
//   weighted mix -> carrier phase increment + mix[i]>>12 per sample ->
//   sine (8192-word table, lerped) -> 2 quadrature rotators -> x0.5 -> out.
#pragma once
#include <array>
#include <cmath>
#include <cstdint>

namespace mnm_fm {

// ---- P:$141A80 ratio table (24 words), ratio = raw / 2^19 ------------------
static const uint32_t kRatioRaw[24] = {
    0x004000,0x008000,0x010000,0x018000,0x020000,0x028000,0x030000,0x040000,
    0x050000,0x060000,0x070000,0x080000,0x0A0000,0x0C0000,0x0E0000,0x100000,
    0x140000,0x180000,0x1C0000,0x200000,0x280000,0x300000,0x380000,0x400000
};
inline float ratioOf(int idx) {           // idx 0..23
    return (float)kRatioRaw[idx] / 524288.0f;   // 2^19
}
// P:$145D36/DAA: n = ((K_word + $8000)*24) >> 24 = floor((K+0.5)*3/32)
inline int ratioIndex(int knob0to127) {
    int64_t n = (((int64_t)knob0to127 << 16) + 0x8000) * 24 >> 24;
    if (n < 0) n = 0; if (n > 23) n = 23;
    return (int)n;
}

// ---- P:$145DAE: 1FIN fine multiplier on modulator-1 ratio (STAT only) ------
// finmult = 1 + (FIN-64)/256 ; NOTE the firmware multiplies ratio1 by
// 2*finmult (the extra x2 is part of the mod-1 path, $145DB6 `asl b`).
inline float finMultiplier(int fin0to127) {
    return 1.0f + (fin0to127 - 64) / 256.0f;
}

// ---- 1ENV / 2VOL wrap level law ($145DFB-$145E17) ---------------------------
// level_word = (ENV^2 * 2048) mod 2^24 read as signed 1.23, i.e. the level
// follows (K/64)^2 and WRAPS through the sign bit above K=64: zero near K=90.5,
// sign inversion past it. drop = 1 - ((K-64)/128)^4 for K>=64 (env states only).
inline float wrapLevelSq(int k0to127) {
    float x = (float)k0to127 / 64.0f;
    float s = x * x;                       // (k/64)^2
    int wrap = (int)(s * 8388608.0f);      // to 1.23 raw
    wrap &= 0xFFFFFF;
    if (wrap & 0x800000) wrap -= 0x1000000;
    return (float)wrap / 8388608.0f;       // signed, may be negative / zero
}
inline float envDroop(int k0to127) {
    if (k0to127 < 64) return 1.0f;
    float t = (k0to127 - 64) / 128.0f;
    float t2 = t * t; return 1.0f - t2 * t2;   // quartic, $1460AC-$1460B1
}

// ---- peak shaper gain ($145D8C-$145DA0): G = 16*max(0, K-64)/256 ------------
inline float shaperGain(int k0to127) {
    return k0to127 >= 64 ? (k0to127 - 64) / 16.0f : 0.0f;
}

// ---- feedback ($145DC1, $146270): phase feedback = sin_prev * K*32 ---------
// in table units (8192 per cycle): up to 127*32 = 4064 = 0.496 cycle.
inline float fbPhaseUnits(int k0to127) { return (float)k0to127 * 32.0f; }

// ---- TUNE (kernel $02C0-$02C9, NOT the machine): pitch += TUNE*683/128 -----
// table $140000 = 2048 units/octave -> +3.127 cents per step, UNIPOLAR.
inline float tuneCents(int k0to127) {
    return (float)k0to127 * 683.0f / 128.0f / 2048.0f * 1200.0f;  // 0..+397
}

// ---- DYN (m10) laws ----------------------------------------------------------
// 1FRQ: continuous ratio, f1 = f_car * (1FRQ/2^23)*2, clamp $7FFFFF ($1461E3)
inline float dynRatio1(int k0to127) {
    int64_t w = (int64_t)k0to127 << 16; if (w > 0x7FFFFF) w = 0x7FFFFF;
    return (float)w / 8388608.0f * 2.0f;
}
// 2FRQ: quadratic continuous ratio, f2 = f_car * (K/128)^2*4 ($14625D-$14626A)
inline float dynRatio2(int k0to127) {
    float x = (float)k0to127 / 128.0f;
    return x * x * 4.0f;
}
// 1FEN: wavetable-recursion FM on modulator 1; n1 = 128 - |1FEN-64|*2;
// state <- -table$141880[n1] * state ; increment += state >> 12 ($1461D0-$1461DD)
inline int   dynFenIndex(int k0to127) { int a = 2 * (k0to127 - 64); return 128 - (a < 0 ? -a : a); }
// 1VEN (iter-28 fix): NOT a shaper selector (that code is m3 SID, $1463E8 reads
// SID's WAVE param). In m10, 1VEN sets the CONF init of the $2C level-recursion
// ($1461AB-$1461B7) and its per-block decay rate via n1 = 128-|1VEN-64|*2 into
// table $141880 ($14623C-$146248). $2C adds into the mod-1 level before the x4:
//   $2C_init = 4*(1VEN-64)*1VOL  if 1VEN < 64, else 0   (1.23 acc, wraps mod 2)
inline float dynVenInit(int ven0to127, int vol0to127) {
    if (ven0to127 >= 64) return 0.0f;
    return 4.0f * ((ven0to127 - 64) / 128.0f) * (vol0to127 / 128.0f);
}
// fixed LP coeff of DYN (replaces TONE table): y0 = #$40 => c = 64/2^23
// ($146233/$146297/$1462D3). tau ~= 1.49 s @ 88200 iter/s; makes every mod
// path converge to its DC (~1.2e-4), so the mix>>12 phase-FM term is 0.

// ---- FM+STAT knob -> engine map (exact) -------------------------------------
struct StatParams {
    float ratio1;      // = ratioOf(idx1) * 2 * finmult
    float ratio2;      // = ratioOf(idx2)
    float level1;      // signed, wrapped; * droop
    float level2;      // signed, wrapped; no droop
    float fb1Units;    // feedback in table units
    float shaper2;     // G for op-2 peak shaper (2VOL)
    float toneCoeff;   // $144AC7[TONE]
};
inline StatParams statLaws(const std::array<int, 8>& p) {
    StatParams r;
    // p: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE
    int idx1 = ratioIndex(p[0]);
    r.ratio1 = ratioOf(idx1) * 2.0f * finMultiplier(p[1]);
    r.ratio2 = ratioOf(ratioIndex(p[4]));
    r.level1 = wrapLevelSq(p[2]) * envDroop(p[2]);
    r.level2 = wrapLevelSq(p[5]);
    r.fb1Units = fbPhaseUnits(p[3]);
    r.shaper2 = shaperGain(p[5]);
    r.toneCoeff = 0.0f; // caller: toneCoeff = kTone144AC7[p[6]]
    return r;
}

// ---- FM+PAR knob -> engine map ----------------------------------------------
// params: 1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE
// ratios = table, NO x2, NO fin; each op: gate>=64 + wrap level + droop +
// shaper gain from its own ENV knob; TONE applied per-op AND post-mix (4x).
struct ParParams {
    float ratio1, ratio2, ratio3;
    float level1, level2, level3;
    float shaper1, shaper2, shaper3;
};
inline ParParams parLaws(const std::array<int, 8>& p) {
    ParParams r;
    r.ratio1 = ratioOf(ratioIndex(p[0]));
    r.ratio2 = ratioOf(ratioIndex(p[2]));
    r.ratio3 = ratioOf(ratioIndex(p[4]));
    r.level1 = wrapLevelSq(p[1]) * envDroop(p[1]);
    r.level2 = wrapLevelSq(p[3]) * envDroop(p[3]);
    r.level3 = wrapLevelSq(p[5]) * envDroop(p[5]);
    r.shaper1 = shaperGain(p[1]);
    r.shaper2 = shaperGain(p[3]);
    r.shaper3 = shaperGain(p[5]);
    return r;
}

// ---- FM+DYN knob -> engine map (iter-28 corrected) --------------------------
// params: 1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB TUNE ; NO TONE knob (fixed LPs)
// EMPIRICAL FACTS (harness, 24-block runs):
//  - carrier phase advance is IDENTICAL (2208 units/blk @ A=11776) with all mod
//    knobs at 0 and at max => the mod->carrier FM term is exactly 0;
//  - output corr = 1.0000 across all knob extremes => audible output = pure
//    carrier sine (+ rotator stereo detune). The 1FEN offset goes into the
//    MOD-1 increment ($1461E0 feeds the mpysu at $1461E8), NOT into L:$5.
//  - 2FB wobbles mod-2's own rate (2192+-3 vs 2208 units/blk) - inaudible.
// The ratio/level laws below are still exact (they describe the silent
// internals); keep them for a bit-faithful port of the whole machine.
struct DynParams {
    float ratio1;        // continuous
    float ratio2;        // quadratic
    float fb2Units;      // 2FB*32 (feedback into mod-2's own phase)
    float level2;        // wrapped sq * droop
    float shaper1;       // 1VOL gated shaper (silent path, exact anyway)
    int   fenIdx;        // wavetable index for 1FEN recursion (mod-1 inc)
    float venInit;       // CONF init of the $2C recursion
    float lpCoeff;       // 64/2^23 fixed LP on all mod paths
};
inline DynParams dynLaws(const std::array<int, 8>& p) {
    DynParams r;
    r.ratio1 = dynRatio1(p[0]);
    r.ratio2 = dynRatio2(p[4]);
    r.fb2Units = fbPhaseUnits(p[6]);
    r.level2 = wrapLevelSq(p[5]) * envDroop(p[5]);
    r.shaper1 = shaperGain(p[2]);
    r.fenIdx = dynFenIndex(p[1]);
    r.venInit = dynVenInit(p[3], p[2]);
    r.lpCoeff = 64.0f / 8388608.0f;
    return r;
}

} // namespace mnm_fm
