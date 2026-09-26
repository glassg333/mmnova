// =============================================================================
// MnmSid.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m3 "SID-6581" — verbatim transcription of DSP1 P:$14636F-$1464FB
// (listing 03_SID_full.txt, 323 words).
//
// STATUS: validated bit-exact against the DSP56300 emulator vectors
//         research/m3_vectors.txt (16 knob sets x 16 frames x 32 output words,
//         generator: exp58_m3_harness.py; covers all five WAVE branches,
//         the MOD bit-21 wave-mix, both phase-run paths incl. carry-kill,
//         the full 10-tap polyphase filter incl. the +2 -> +1 wheel step
//         switch at $14644B and the pre-mpy parallel stores at
//         $14643D/$146449/$146453/$146459).
//
// Branch map (WAVE = Y:(r6+$7)).  DSP56300 carry convention: after
// CMP, C = NO-borrow, so BCS = branch on dst >= src.  Therefore:
//   WAVE >= 0.1 -> $14647D triangle fold; WAVE < 0.1 (incl. negative)
//   -> $1464C0 LFSR noise.  $146474 (copy), $1464A1 (pulse) and $1464DE
//   (dual wavetable) are UNREACHABLE dead code: the first bcs catches every
//   value >= 0.1, and everything below 0.1 fails all later thresholds
//   (monotone bcs chain).  MOD bit21 -> $146493 wave-mix before the chain.
//
// Engine notes:
//   * one frame = 16 machine iterations = 16 samples; output 32 words =
//     16 x (L,R), L == R.
//   * X/Y scratch pages $000-$1FF are PERSISTENT across frames (the sliding
//     filter window reads cells beyond its writes; INIT does not clear them).
//   * machine-wave page X:$140000 (2048 words) is host runtime data on
//     hardware; default here = the documented saw ramp tbl[i] = i<<11 used
//     by the vector harness (setWavePage() overrides).
//   * SID wavetable pages X/Y:$140800-$1417FF default to the firmware image
//     dump (mnm_sid_tables.h); setWavetables() overrides.
//   * state cells X/Y:(r6-$1) (MSRC>=0.5 aux) are kernel-page-zeroed; the
//     harness runs them as 0 (setAux() overrides).
// =============================================================================

#pragma once
#include <cstdint>
#include "mnm_sid_tables.h"

namespace mnm {
namespace sid {

static const uint32_t M24 = 0xFFFFFFu;
static const int64_t  M56 = 0xFFFFFFFFFFFFFFll;

inline int64_t wrap56(int64_t v) {
    v &= M56;
    if (v & (1ll << 55)) v -= (1ll << 56);
    return v;
}
inline int64_t sx24(int64_t v) { v &= M24; return (v ^ 0x800000) - 0x800000; }
inline uint32_t a1(int64_t a) { return uint32_t((a >> 24) & M24); }
inline uint32_t a0(int64_t a) { return uint32_t(a & M24); }
inline int64_t abs56(int64_t v) { return v < 0 ? wrap56(-v) : v; }

// ---- ALU primitives (bit-exact with scripts/dsp_emu.py) --------------------
inline int64_t mpy(int64_t x, int64_t y)  { return wrap56(sx24(x) * sx24(y) * 2); }
inline int64_t mac(int64_t acc, int64_t x, int64_t y) {
    return wrap56(acc + sx24(x) * sx24(y) * 2);
}
// "-y0" operand form: negate BEFORE sign interpretation (emulator does
// (-sgn24(v)) & MASK24 first; equal to -sx24(v) for all 24-bit words).
inline int64_t mpy_n(int64_t x, int64_t y) { return wrap56(sx24(x) * (-sx24(y)) * 2); }
inline int64_t mac_n(int64_t acc, int64_t x, int64_t y) {
    return wrap56(acc + sx24(x) * (-sx24(y)) * 2);
}
inline int64_t rnd56(int64_t v) { return wrap56(v + 0x800000) & ~(int64_t)M24; }
// NOTE: the ~(int64_t) cast is REQUIRED — ~M24 on the uint32_t constant would
// yield the 32-bit complement 0xFF000000 and destroy the accumulator.
inline int64_t mpyr(int64_t x, int64_t y) { return rnd56(mpy(x, y)); }
inline int64_t macr(int64_t acc, int64_t x, int64_t y) { return rnd56(mac(acc, x, y)); }
inline int64_t macuu(int64_t acc, uint32_t x, uint32_t y) {
    return wrap56(acc + (int64_t)((uint64_t)(x & M24) * (uint64_t)(y & M24)) * 2);
}
inline int64_t asr56(int64_t v, int n) { return wrap56(v >> n); }
inline int64_t lsr56(int64_t v, int n) {
    return wrap56((int64_t)((uint64_t)v & (uint64_t)M56) >> n);
}
inline int64_t asl56(int64_t v, int n) { return wrap56(v << n); }

class Sid {
public:
    Sid() {
        for (int i = 0; i < 2048; ++i) wavePage[i] = uint32_t((i << 11) & M24);
        for (int i = 0; i < 4096; ++i) { wtX[i] = SID_WAVETABLE_X[i]; wtY[i] = SID_WAVETABLE_Y[i]; }
    }

    // INIT $14636F (does NOT touch the X/Y scratch pages — firmware doesn't)
    void init() {
        st19 = 0; st1B = 0; st1C = 0; st1D = 0; st1E = 0; st1F = 0;
        st1A = 0x0019B5;
    }
    // CONFIG $14637A (called by the dispatcher per voice update; the vector
    // harness does not execute it)
    void config() {
        if (sx24(kPWRS) >= (int64_t)0x400000) st19 = 0;   // cmp #$400000 ; clr b ifge
    }

    void setKnobs(uint32_t pw, uint32_t pwad, uint32_t pwrs, uint32_t wave,
                  uint32_t mod, uint32_t msrc, uint32_t mfrq) {
        kPW = pw & M24; kPWAD = pwad & M24; kPWRS = pwrs & M24;
        kWAVE = wave & M24; kMOD = mod & M24; kMSRC = msrc & M24; kMFRQ = mfrq & M24;
    }
    void setPitchQ23(int32_t a1v) { pitchA1 = a1v & (int32_t)M24; }
    void setWavePage(const uint32_t* t2048) {
        for (int i = 0; i < 2048; ++i) wavePage[i] = t2048[i] & M24;
    }
    void setWavetables(const uint32_t* tx4096, const uint32_t* ty4096) {
        for (int i = 0; i < 4096; ++i) { wtX[i] = tx4096[i] & M24; wtY[i] = ty4096[i] & M24; }
    }
    void setAux(uint32_t xa, uint32_t ya) { auxX = xa & M24; auxY = ya & M24; }

    // PROC $146382 — one 16-sample frame -> 32 words (16 x L,R)
    void processFrame(uint32_t* out32) {
        int64_t A = 0, B = 0;
        uint32_t x0 = 0, y0 = 0;

        // ---- A. frequency step ($146382-$146390) --------------------------
        A = asr56((int64_t)pitchA1 << 24, 15);            // asr #$f,a,a
        x0 = a0(A);                                       // move a0,x0
        A = macuu(A, x0, 0xFD9FB3u);                      // macuu x0,y0,a
        A = lsr56(A, 5);                                  // lsr #$5,a
        x0 = a1(A);                                       // move a,x0
        A = asl56(A, 4);                                  // asl #$4,a,a
        if (sx24(kWAVE) < (int64_t)0x666666)              // cmp #$666666,b ; tfr x0,a iflt
            A = sx24(x0) << 24;
        st16 = a1(A);                                     // move a,y:($16)
        x0 = a1(A);                                       // move a,x0

        // ---- B. wavetable contribution ($146391-$1463C2) ------------------
        B = asr56(sx24(kMFRQ) << 24, 6);                  // move y:($a),b ; asr #$6,b,b
        B = wrap56(B + ((int64_t)0x400 << 24));           // add #>$400,b
        y0 = a1(B);                                       // move b,y0 (dead: overwritten at $1463C2)
        if (sx24(kMSRC) < (int64_t)0x400000) {            // blt $1463A9
            A = mpy((int64_t)0x2000, (int64_t)kMFRQ);     // mpy x1,x0,a
            A = wrap56(A + ((int64_t)0x800 << 24));       // add #>$800,a
        } else {                                          // $1463A2: aux cells
            A = sx24(auxX) << 24;                         // move x:(r6-$1),a
            if (A == 0) A = sx24(auxY) << 24;             // tst a ; bne ; move y:(r6-$1),a
        }
        B = asr56(A, 11);                                 // asr #$b,a,b  (B = A>>11, A kept)
        uint32_t idx = a1(A) & 0x7FF;                     // and #>$7ff,a ; move a,r2
        uint32_t shift = a1(B);                           // move b,x0
        A = sx24(wavePage[idx]) << 24;                    // move x:(r2+$140000),a
        A = asr56(A, 10);                                 // asr #$a,a,a
        A = asl56(A, (int)(shift & 63));                  // asl x0,a,a
        A = mpy((int64_t)0x20B40, (int64_t)a1(A));        // mpy y0,x0,a
        A = asr56(A, 18);                                 // asr #$12,a,a
        x0 = a0(A);                                       // move a0,x0
        A = macuu(0, x0, 0xFD9FB3u);                      // mpyuu x0,y0,a
        A = lsr56(A, 2);                                  // lsr #$2,a
        y0 = a1(A);                                       // move a,y0
        dbg[0] = st16; dbg[1] = y0;

        // ---- C. phase run ($1463C3-$1463E4) -------------------------------
        // m0 = m4 = $BF (192-word rings, base $000); r0 = r4 = $000 here.
        x0 = st16;                                        // move y:($16),x0
        {
            int r0 = 0, r4 = 0;
            if (sx24(kMOD) >= (int64_t)0x400000) {        // MOD >= 0.5: 191 steps + carry-kill
                A = (int64_t)st17 << 24;                  // clr a ; move y:($17),a1
                B = (int64_t)st18 << 24;                  // clr b ; move y:($18),b1
                B = wrap56(B + (sx24(x0) << 24));         // add x0,b
                A = wrap56(A + (sx24(y0) << 24));         // add y0,a
                {   uint32_t c = uint32_t((B >> 48) & 1); // bclr #$0,b2
                    B &= ~(1ll << 48);
                    if (c) A = 0;                         // clr a ifcs
                }
                for (int k = 0; k < 191; ++k) {           // do #$bf
                    ymem[r4++] = a1(B);                   //   b1,y:(r4)+
                    B = wrap56(B + (sx24(x0) << 24));     //   add x0,b
                    xmem[r0++] = a1(A);                   //   a1,x:(r0)+
                    A = wrap56(A + (sx24(y0) << 24));     //   add y0,a
                    {   uint32_t c = uint32_t((B >> 48) & 1);
                        B &= ~(1ll << 48);
                        if (c) A = 0;
                    }
                }
                ymem[r4] = a1(B);                         // move b1,y:(r4)+
                xmem[r0] = a1(A);                         // move a1,x:(r0)+
            } else {                                      // MOD < 0.5: 192 steps, chains swapped
                A = (int64_t)st17 << 24;
                B = (int64_t)st18 << 24;
                for (int k = 0; k < 192; ++k) {           // do #$c0
                    ymem[r4++] = a1(B);                   //   b1,y:(r4)+
                    B = wrap56(B + (sx24(y0) << 24));     //   add y0,b
                    xmem[r0++] = a1(A);                   //   a1,x:(r0)+
                    A = wrap56(A + (sx24(x0) << 24));     //   add x0,a
                }
            }
            st18 = a1(B);                                 // move b1,y:($18)
            st17 = a1(A);                                 // move a1,y:($17)
        }

        // ---- D. waveform select ($1463E5-$1464DD) -------------------------
        {
            int r0 = 0, r4 = 0;
            if (kMOD & 0x200000u) {
                // $146493 wave-mix: out[i] = mpyr|X[i] * Y[i-1]| (y0 lags r4)
                uint32_t x0m = xmem[r0++];                // move x:(r0)+,x0
                uint32_t y0m = ymem[r4];                  // y:(r4),y0
                B = mpyr(y0m, x0m);                       // mpyr y0,x0,b
                x0m = xmem[r0++]; y0m = ymem[r4];         // x:(r0)+,x0 ; y:(r4),y0
                B = abs56(B);                             // abs b
                for (int k = 0; k < 95; ++k) {            // do #$5f
                    A = mpyr(y0m, x0m);                   //   mpyr y0,x0,a
                    x0m = xmem[r0++]; y0m = ymem[r4];
                    A = abs56(A);                         //   abs a
                    ymem[r4++] = a1(B);                   //   b,y:(r4)+
                    B = mpyr(y0m, x0m);                   //   mpyr y0,x0,b
                    x0m = xmem[r0++]; y0m = ymem[r4];
                    B = abs56(B);                         //   abs b
                    ymem[r4++] = a1(A);                   //   a,y:(r4)+
                }
                A = mpyr(y0m, x0m);                       // $14649C
                A = abs56(A);
                ymem[r4++] = a1(B);                       // $14649D
                ymem[r4++] = a1(A);                       // $14649F
            } else if (sx24(kWAVE) >= (int64_t)0x199999) {
                // $14647D triangle fold (bcs: taken when WAVE >= 0.1)
                uint32_t x0t = 0x400000;                  // move #$40,x0 (imm8 << 16)
                A = sx24(xmem[r0++]) << 24;               // move x:(r0)+,a
                B = sx24(xmem[r0++]) << 24;               // move x:(r0)+,b
                for (int k = 0; k < 95; ++k) {            // do #$5f
                    A = abs56(A);                         //   abs a
                    B = abs56(B);                         //   abs b
                    A = wrap56(A - (sx24(x0t) << 24));    //   sub x0,a
                    B = wrap56(B - (sx24(x0t) << 24));    //   sub x0,b
                    A = asl56(A, 1);                      //   asl a
                    B = asl56(B, 1);                      //   asl b
                    ymem[r4++] = a1(A);                   //   a,y:(r4)+
                    A = sx24(xmem[r0++]) << 24;           //   x:(r0)+,a (parallel)
                    ymem[r4++] = a1(B);                   //   b,y:(r4)+
                    B = sx24(xmem[r0++]) << 24;           //   x:(r0)+,b (parallel)
                }
                A = abs56(A); B = abs56(B);
                A = wrap56(A - (sx24(x0t) << 24));
                B = wrap56(B - (sx24(x0t) << 24));
                A = asl56(A, 1); B = asl56(B, 1);
                ymem[r4++] = a1(A);                       // $146490
                ymem[r4++] = a1(B);                       // $146491
            } else {
                // NOTE: the listing also contains three UNREACHABLE waveform
                // branches (dead code, see M3_SID_MINING.md for their exact
                // transcriptions): $146474 straight copy (96 pairs),
                // $1464A1 pulse fold (threshold = |PWAD>>9 + st19 + 2*PW|,
                // <<7), $1464DE dual wavetable staircase (X/Y:$140800,
                // index = phase>>12, pairs).  They can never execute: the
                // first bcs (WAVE >= 0.1) catches everything above 0.1 and
                // the LFSR fallthrough takes everything below.
                // $1464C0 LFSR noise + smoother
                uint32_t x1l = 0x658100;                  // move #>$658100,x1
                A = sx24(st1A) << 24;                     // move y:($1a),a
                B = sx24(st24) << 24;                     // move y:($24),b
                for (int k = 0; k < 48; ++k) {            // do #$30
                    // rol a ; a1,y:(r4)+n4  (store latches PRE-rotation a1)
                    ymem[r4] = a1(A); r4 = (r4 + 4) % 192;
                    {
                        uint32_t c = (a1(A) >> 23) & 1;
                        uint32_t a1r = ((a1(A) << 1) | c) & M24;
                        int64_t r = ((int64_t)a1r << 24) | a0(A);
                        if (a1r & 0x800000) r |= (int64_t)0xFF << 48;
                        A = r;
                        if (c) {                          // eor x1,a ifcs
                            uint32_t a1e = a1(A) ^ x1l;
                            int64_t r2 = ((int64_t)a1e << 24) | a0(A);
                            if (a1e & 0x800000) r2 |= (int64_t)0xFF << 48;
                            A = r2;
                        }
                    }
                    // rol b ; b1,y:(r4)+n4 ; eor x1,b ifcs
                    ymem[r4] = a1(B); r4 = (r4 + 4) % 192;
                    {
                        uint32_t c = (a1(B) >> 23) & 1;
                        uint32_t b1r = ((a1(B) << 1) | c) & M24;
                        int64_t r = ((int64_t)b1r << 24) | a0(B);
                        if (b1r & 0x800000) r |= (int64_t)0xFF << 48;
                        B = r;
                        if (c) {
                            uint32_t b1e = a1(B) ^ x1l;
                            int64_t r2 = ((int64_t)b1e << 24) | a0(B);
                            if (b1e & 0x800000) r2 |= (int64_t)0xFF << 48;
                            B = r2;
                        }
                    }
                }
                st1A = a1(A);                             // move a1,y:($1a)
                st24 = a1(B);                             // move b1,y:($24)
                // smoother: 48 groups of 4, sample-hold keyed on phase delta
                uint32_t x0s = st22;                      // move y:($22),x0
                B = sx24(st23) << 24;                     // move y:($23),b
                dbg[2] = 0;
                for (int k = 0; k < 48; ++k) {            // do #$30
                    int64_t oldA = sx24(xmem[r0]) << 24;  //   move x:(r0)+n0,a
                    r0 = (r0 + 4) % 192;
                    uint32_t y0s = ymem[r4];              //   y:(r4),y0
                    A = wrap56(oldA - (sx24(x0s) << 24)); //   sub x0,a
                    x0s = a1(oldA);                       //   a,x0 (latched pre-sub)
                    if (A < 0) { B = sx24(y0s) << 24; ++dbg[2]; }  //   tmi y0,b
                    ymem[r4++] = a1(B);                   //   move b,y:(r4)+
                    ymem[r4++] = a1(B);
                    ymem[r4++] = a1(B);
                    ymem[r4++] = a1(B);
                }
                st22 = x0s;                               // move x0,y:($22)
                st23 = a1(B);                             // move b,y:($23)
            }
        }

        // ---- E. 10-tap polyphase filter ($1463FB-$146460) -----------------
        filterBlock(out32);

        // ---- F. state restore ($146461-$14646A) ---------------------------
        st1B = xmem[0xF0];                                // x:(r1+$0)   (r1 = $F0)
        st1C = xmem[0xF1];                                // x:(r1+$1)
        st1D = xmem[r2end];                               // x:(r2+$0)   (r2 drifted)
        st1E = xmem[r2end + 1];                           // x:(r2+$1)
        st1F = xmem[0xF4];                                // x:(r3+$0)   (r3 = $F4)
    }

    // state (voice page mirrors; r6 = $428 in the vector harness)
    uint32_t st16 = 0, st17 = 0, st18 = 0, st19 = 0;
    uint32_t st1A = 0x0019B5, st1B = 0, st1C = 0, st1D = 0, st1E = 0, st1F = 0;
    uint32_t st22 = 0, st23 = 0, st24 = 0;
    uint32_t auxX = 0, auxY = 0;                 // X/Y:(r6-$1)
    uint32_t dbg[4] = {0};                       // [0]=st16 [1]=wt y0 [2]=tmi fires [3]=r2end
    // X/Y scratch pages $000-$1FF — PERSISTENT across frames (the sliding
    // filter window reads cells it does not rewrite; INIT never clears them).
    uint32_t xmem[0x200] = {0};                  // $00-$BF a-chain phases, $F0-$102 filter
    uint32_t ymem[0x200] = {0};                  // $00-$BF b-chain phases / waveform, $F0-$F9 wheel
    uint32_t wavePage[2048] = {0};               // X:$140000 (host runtime data)
    uint32_t wtX[4096], wtY[4096];               // X/Y:$140800-$1417FF

private:
    uint32_t kPW = 0, kPWAD = 0, kPWRS = 0, kWAVE = 0, kMOD = 0, kMSRC = 0, kMFRQ = 0;
    int32_t pitchA1 = 0;
    int r2end = 0xF2;                            // final r2 of the last frame (restore base)

    // -----------------------------------------------------------------------
    // filterBlock — $1463FB-$146460, verbatim.
    // Coefficient wheel Y:$F0 (m5=$9 -> ring of 10).  All wheel reads before
    // $14644B use step n5=+2; from $14644B on they use plain (r5)+ = +1.
    // r1 = $F0 / r2 = $F2 / r3 = $F4 (linear addressing, net zero drift per
    // outer iteration).  Parallel-move latch discipline: every ALU operand is
    // the register value BEFORE the instruction's own parallel loads, and
    // every parallel accumulator store latches the PRE-instruction value
    // (critical for the mpy-with-store pairs at $14643D/$146449/$146453/
    // $146459).
    // -----------------------------------------------------------------------
    void filterBlock(uint32_t* out32) {
        int64_t A = 0, B = 0;
        uint32_t x0f = 0, y0f = 0, y1f = 0;
        const uint32_t x1f = 0x6186;                       // move #>$6186,x1
        static const uint32_t coeffs[10] = {
            0x3EF3C2, 0xFE01EB, 0x82F338, 0x01D9BC, 0x3CD5C5,
            0x0650D5, 0x8400A4, 0xF9AF2B, 0xC22E5F, 0x04866C
        };
        {   // move y0,y:(r5)+ x10 — r5 = $F0, m5 = 9 (ring of 10), step 1
            int off = 0;
            for (int i = 0; i < 10; ++i) {
                ymem[0xF0 + off] = coeffs[i];
                off += 1;
                if (off > 9) off -= 10;
            }
        }
        int r5off = 0;                                     // wheel offset 0..9
        int r1 = 0xF0, r2 = 0xF2, r3 = 0xF4;               // absolute X addresses
        int r4 = 0;                                        // Y waveform cursor (ring 192, base 0)
        auto yld = [&](int step) -> uint32_t {             // y:(r5)+n5  /  y:(r5)+
            uint32_t v = ymem[0xF0 + r5off];
            r5off += step;
            if (r5off > 9) r5off -= 10;
            return v;
        };
        auto nxty = [&]() -> uint32_t {                    // y:(r4)+
            uint32_t v = ymem[r4];
            r4 += 1;
            if (r4 > 191) r4 -= 192;
            return v;
        };

        // state mirror $146405-$14640E
        xmem[0xF0] = st1B; xmem[0xF1] = st1C; xmem[0xF2] = st1D;
        xmem[0xF3] = st1E; xmem[0xF4] = st1F;

        for (int k = 0; k < 16; ++k) {                     // do #$10 ($14642F)
            x0f = xmem[r1]; r1++;                          // $146431 x:(r1)+,x0
            y0f = yld(2);                                  //        y:(r5)+n5,y0
            A = mpy_n(x0f, y0f);                           // $146432 mpy -y0,x0,a
            x0f = xmem[r1]; r1--;                          //        x:(r1)-,x0
            y0f = yld(2);                                  //        y:(r5)+n5,y0
            A = mac_n(A, x0f, y0f);                        // $146433 mac -y0,x0,a
            xmem[r1] = x0f; r1++;                          //        x0,x:(r1)+
            y1f = nxty();                                  //        y:(r4)+,y1
            A = asl56(A, 1);                               // $146434 asl a
            A = macr(A, x1f, y1f);                         // $146435 macr y1,x1,a
            x0f = xmem[r2]; r2++;                          //        x:(r2)+,x0
            y0f = yld(2);                                  //        y:(r5)+n5,y0
            for (int j = 0; j < 10; ++j) {                 // do #$a ($146436)
                B = mpy_n(x0f, y0f);                       // $146438 mpy -y0,x0,b
                x0f = xmem[r2]; r2--;                      //        x:(r2)-,x0
                y0f = yld(2);                              //        y:(r5)+n5,y0
                B = mac_n(B, x0f, y0f);                    // $146439 mac -y0,x0,b
                xmem[r2] = x0f; r2++;                      //        x0,x:(r2)+
                B = asl56(B, 1);                           // $14643A asl b
                xmem[r1] = a1(A); r1--;                    //        a,x:(r1)-
                B = macr(B, x1f, y1f);                     // $14643B macr y1,x1,b
                x0f = xmem[r3];                            //        x:(r3),x0
                y0f = yld(2);                              //        y:(r5)+n5,y0
                {   uint32_t pre = a1(B);                  // $14643D mpy -y0,x0,b  b,x:(r2)-
                    B = mpy_n(x0f, y0f);                   //        (store latches PRE-mpy b1)
                    xmem[r2] = pre; r2--;
                }
                B = asl56(B, 1);                           // $14643E asl b
                B = macr(B, x1f, y1f);                     // $14643F macr y1,x1,b
                x0f = xmem[r1]; r1++;                      //        x:(r1)+,x0
                y0f = yld(2);                              //        y:(r5)+n5,y0
                A = mpy_n(x0f, y0f);                       // $146440 mpy -y0,x0,a
                x0f = xmem[r1]; r1--;                      //        x:(r1)-,x0
                y0f = yld(2);                              //        y:(r5)+n5,y0
                A = mac_n(A, x0f, y0f);                    // $146441 mac -y0,x0,a
                xmem[r1] = x0f; r1++;                      //        x0,x:(r1)+
                y1f = nxty();                              //        y:(r4)+,y1
                A = asl56(A, 1);                           // $146442 asl a
                xmem[r3] = a1(B);                          //        b,x:(r3)
                A = macr(A, x1f, y1f);                     // $146443 macr y1,x1,a
                x0f = xmem[r2]; r2++;                      //        x:(r2)+,x0
                y0f = yld(2);                              //        y:(r5)+n5,y0
            }
            // tail (once per outer iteration)
            B = mpy_n(x0f, y0f);                           // $146444 mpy -y0,x0,b
            x0f = xmem[r2]; r2--;                          //        x:(r2)-,x0
            y0f = yld(2);                                  //        y:(r5)+n5,y0
            B = mac_n(B, x0f, y0f);                        // $146445 mac -y0,x0,b
            xmem[r2] = x0f; r2++;                          //        x0,x:(r2)+
            B = asl56(B, 1);                               // $146446 asl b
            xmem[r1] = a1(A); r1--;                        //        a,x:(r1)-
            B = macr(B, x1f, y1f);                         // $146447 macr y1,x1,b
            x0f = xmem[r3];                                //        x:(r3),x0
            y0f = yld(2);                                  //        y:(r5)+n5,y0
            {   uint32_t pre = a1(B);                      // $146449 mpy -y0,x0,b  b,x:(r2)-
                B = mpy_n(x0f, y0f);                       //        (store latches PRE-mpy b1)
                xmem[r2] = pre; r2--;
            }
            B = asl56(B, 1);                               // $14644A asl b
            B = macr(B, x1f, y1f);                         // $14644B macr y1,x1,b
            x0f = xmem[r1]; r1++;                          //        x:(r1)+,x0
            y0f = yld(1);                                  //        y:(r5)+,y0   <- step +1
            A = mpy_n(x0f, y0f);                           // $14644D mpy -y0,x0,a
            xmem[r3] = a1(B);                              //        b,x:(r3)
            y0f = yld(1);                                  //        y:(r5)+,y0
            B = mpy(x0f, y0f);                             // $14644E mpy y0,x0,b  (positive)
            x0f = xmem[r1]; r1--;                          //        x:(r1)-,x0
            y0f = yld(1);                                  //        y:(r5)+,y0
            A = mac_n(A, x0f, y0f);                        // $14644F mac -y0,x0,a
            xmem[r1] = x0f; r1++;                          //        x0,x:(r1)+
            y0f = yld(1);                                  //        y:(r5)+,y0
            A = asl56(A, 1);                               // $146450 asl a
            y1f = nxty();                                  //        y:(r4)+,y1
            A = macr(A, x1f, y1f);                         // $146451 macr y1,x1,a
            B = mac(B, x0f, y0f);                          // $146452 mac y0,x0,b  (positive)
            x0f = xmem[r2]; r2++;                          //        x:(r2)+,x0
            y0f = yld(1);                                  //        y:(r5)+,y0
            {   uint32_t pre = a1(A);                      // $146453 mpy -y0,x0,a  a,x:(r1)-
                A = mpy_n(x0f, y0f);                       //        (store latches PRE-mpy a1)
                xmem[r1] = pre; r1--; y0f = yld(1);        //        y:(r5)+,y0
            }
            B = mac(B, x0f, y0f);                          // $146454 mac y0,x0,b  (positive)
            x0f = xmem[r2]; r2--;                          //        x:(r2)-,x0
            y0f = yld(1);                                  //        y:(r5)+,y0
            A = mac_n(A, x0f, y0f);                        // $146455 mac -y0,x0,a
            xmem[r2] = x0f; r2++;                          //        x0,x:(r2)+
            y0f = yld(1);                                  //        y:(r5)+,y0
            A = asl56(A, 1);                               // $146456 asl a
            A = macr(A, x1f, y1f);                         // $146457 macr y1,x1,a
            B = mac(B, x0f, y0f);                          // $146458 mac y0,x0,b  (positive)
            x0f = xmem[r3];                                //        x:(r3),x0
            y0f = yld(1);                                  //        y:(r5)+,y0
            {   uint32_t pre = a1(A);                      // $146459 mpy -y0,x0,a  a,x:(r2)-
                A = mpy_n(x0f, y0f);                       //        (store latches PRE-mpy a1)
                xmem[r2] = pre; r2--; y0f = yld(1);        //        y:(r5)+,y0
            }
            A = asl56(A, 1);                               // $14645A asl a
            A = macr(A, x1f, y1f);                         // $14645B macr y1,x1,a
            B = mac(B, x0f, y0f);                          // $14645C mac y0,x0,b  (positive)
            B = asl56(B, 6);                               // $14645D asl #$6,b,b
            xmem[r3] = a1(A);                              // $14645F a,x:(r3)
            out32[2 * k] = a1(B);                          //        b,y:(r7)+
            out32[2 * k + 1] = a1(B);                      // $146460 b,y:(r7)+
        }
        r2end = r2;                                        // final r2 ($F2 + 16)
        dbg[3] = (uint32_t)r2;
    }
};

} // namespace sid
} // namespace mnm
