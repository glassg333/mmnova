// mnm_filter_stage2.h — Novation/Monomachine DSP1 dual-filter STAGE 2
// (env-driven rotation resonator, P:$0A5D-$0AD0) — bit-exact C++ port.
//
// Iteration 18: all four blocks verified WORD-EXACT against the emulator
// (12 configs ATK/DEC/BOFS/WOFS x 8 frames, 384/384 block checks — see
// 11_stage2_model/verify_report.txt in the pack).
//
// Architecture (see 11_stage2_model/README_stage2_model.md):
//   L1: two 16-cell chains, angle c1 = K144AC7[BOFS], beta = 1 - c1/2
//       s' = s - m_prev*beta - B1(s)*c1 + m*beta   (wrap across frames)
//   L2: same chains on L1 outputs, angle c2 = K144AC7[WOFS+BOFS]
//   L3: same chains on X-bus, angle c3 = K144B48[phase >> 17]
//   DP: coeff_a[i] = 2*DEC*L2a[i] + depth*L3a[i]
//       coeff_b[i] = 2*DEC*L2b[i] + depth*L3b[i]
//       depth = mpy(2|ATK-0.5|, 2|ATK-0.5|)   (KILL gate: DEC term = 0 at
//       AMP-env phase 4)
//
// Output: coefficient banks X:$41-$4F/X:$50 (b) and X:$51-$61 (a) that drive
// the multi-tap resonator feedback (func_000397 scanner + L:$90-$A0 banks,
// tail $0AD1-$0B4C — port pending, iteration 19).
//
// Bit-exactness notes (all proven by fuzz + frame verification):
//   * cells are 24-bit signed (Q1.23); accumulators 56-bit;
//   * parallel-move stores latch the PRE-ALU accumulator (B1 = bits 47..24);
//   * "add #$800000" adds -1.0 (not +0.5!);
//   * B1() quantization inside the recurrence is part of the algorithm;
//   * the a/b branches consume INTERLEAVED (even/odd) cells — r1/r4 step
//     twice per iteration;
//   * X:$40 receives the AMP-env phase value through a register reuse
//     ($0AC0 move x:(r7-$84),b) — harmless but bit-exact.

#pragma once
#include <cstdint>
#include <cstring>
#include "mnm_tables_stage2.h"  // kCurve144AC7[258], kCurve144B48[128] (raw words)

namespace mnm {

static inline int32_t sext24(int32_t v) {
    v &= 0xFFFFFF;
    return (v & 0x800000) ? (v - 0x1000000) : v;
}
static inline int64_t acc24(int32_t v) { return int64_t(sext24(v)) << 24; }
static inline int32_t B1(int64_t acc) { return int32_t((uint64_t(acc) >> 24) & 0xFFFFFF); }
// fractional MAC: acc += (x*y) << 1, 56-bit wrap
static inline int64_t mac56(int64_t acc, int32_t x, int32_t y) {
    int64_t p = int64_t(sext24(x)) * int64_t(sext24(y));
    uint64_t r = (uint64_t(acc) + (uint64_t(p << 1) & 0xFFFFFFFFFFFFFFULL)) & 0xFFFFFFFFFFFFFFULL;
    return int64_t(r);
}

struct Stage2State {
    // rotation chain states (voice-page cells, X/Y pairs)
    int32_t l1_wrap_y = 0, l1_wrap_x = 0;   // P+$D3 (y/x parts)
    int32_t l1_state_y = 0, l1_state_x = 0; // P+$D4
    int32_t l2_state_y = 0, l2_state_x = 0; // P+$D5
    int32_t l3_state_y = 0, l3_state_x = 0; // P+$CC
    int32_t l3_phase_y = 0, l3_phase_x = 0; // P+$CF (time-ramp accumulator)
    // banks
    int32_t bank_x20[16] = {0};             // X:$20-$2F (b-inputs of L1)
    int32_t bank_x30[16] = {0};             // X:$30-$3F (a-inputs of L1)
    int32_t bus_x00[16] = {0};              // X:$00-$0F (audio bus, a-inputs L3)
    int32_t bus_x10[16] = {0};              // X:$10-$1F (b-inputs L3)
    // outputs (coefficient banks)
    int32_t coeff_a[16] = {0};              // X:$51..$60: [0]=depth, [1..15]=a[0..14]
    int32_t a_final = 0;                    // X:$61
    int32_t coeff_b[16] = {0};              // X:$40..$4F: [0]=amp phase, [1..15]=b[0..14]
    int32_t b_final = 0;                    // X:$50
    int32_t x40_phase_leak = 0;             // == coeff_b[0] (explicit)
};

class FilterStage2 {
public:
    Stage2State st;

    // One frame of stage 2 (called once per audio block, like the kernel).
    //   atk, dec   : FLT page P+$14 / P+$15, Q1.23 knob words (knob<<16... as
    //                transported by the CPU; here raw 24-bit cells)
    //   bofs, wofs : P+$16 / P+$17, same format
    //   amp_phase  : X:P+$D8 (AMP envelope phase, 0..5; 4 = KILL)
    void process(int32_t atk, int32_t dec, int32_t bofs, int32_t wofs,
                 int32_t amp_phase) {
        int32_t inter[32];   // Y:$62-$81
        int32_t l2out[32];   // Y:$20-$3F

        // ---------------- L1 ($0A5D-$0A82) ----------------
        int32_t idx1 = sext24(B1(acc24(bofs >> 16)));      // knob 0..127
        int32_t c1 = kCurve144AC7[idx1 & 0x1FF];           // X:($144AC7+BOFS)
        int64_t b = acc24(c1);
        int32_t y0 = B1(b);                                // move b,y0 (old B1)
        b >>= 1;                                           // asr b
        b = add56(b, acc24(0x800000));                     // add #>$800000 (-1.0)
        b = neg56(b);                                      // neg b
        int32_t y1 = B1(b);                                // move b,y1  (beta)
        int64_t a_acc = acc24(st.l1_state_y);
        int64_t b_acc = acc24(st.l1_state_x);
        int32_t x0 = st.l1_wrap_y, x1 = st.l1_wrap_x;
        for (int i = 0; i < 16; ++i) {
            int32_t sa = B1(a_acc);
            inter[2 * i] = sa;                             // a,y:(r4)+ (pre-ALU)
            a_acc = mac56(a_acc, (int32_t)(uint32_t)(-sext24(x0) & 0xFFFFFF), y1);
            x0 = sa;
            int32_t sb = B1(b_acc);
            inter[2 * i + 1] = sb;
            b_acc = mac56(b_acc, (int32_t)(uint32_t)(-sext24(y1) & 0xFFFFFF), x1);
            x1 = sb;
            a_acc = mac56(a_acc, (int32_t)(uint32_t)(-sext24(y0) & 0xFFFFFF), x0);
            x0 = st.bank_x30[i];                           // x:(r0) -> x0
            b_acc = mac56(b_acc, (int32_t)(uint32_t)(-sext24(x1) & 0xFFFFFF), y0);
            x1 = st.bank_x20[i];
            a_acc = mac56(a_acc, x0, y1);                  // x0,y1,a
            b_acc = mac56(b_acc, y1, x1);                  // y1,x1,b
            // x:(r0)+ / x:(r1)+ reload the SAME cell (post-inc repeats value)
        }
        st.l1_state_y = B1(a_acc);
        st.l1_state_x = B1(b_acc);
        st.l1_wrap_y = st.bank_x30[15];
        st.l1_wrap_x = st.bank_x20[15];

        // ---------------- L2 ($0A84-$0A9B) ----------------
        int32_t idx2 = sext24(B1(acc24((wofs >> 16) + (bofs >> 16))));
        int32_t c2 = kCurve144AC7[idx2 & 0x1FF];
        int32_t y0b = c2;
        a_acc = acc24(st.l2_state_y);
        b_acc = acc24(st.l2_state_x);
        int32_t w = inter[0];                              // y:(r5)+ pre-loop
        for (int i = 0; i < 16; ++i) {
            int32_t sa = B1(a_acc);
            l2out[2 * i] = sa;
            a_acc = mac56(a_acc, w, y0b);
            a_acc = mac56(a_acc, (int32_t)(uint32_t)(-sext24(y0b) & 0xFFFFFF), sa);
            w = inter[2 * i + 1];                          // y:(r5)+ (odd)
            int32_t sb = B1(b_acc);
            l2out[2 * i + 1] = sb;
            b_acc = mac56(b_acc, w, y0b);
            b_acc = mac56(b_acc, (int32_t)(uint32_t)(-sext24(y0b) & 0xFFFFFF), sb);
            if (i < 15) w = inter[2 * i + 2];              // next even
        }
        st.l2_state_y = B1(a_acc);
        st.l2_state_x = B1(b_acc);

        // ---------------- L3 ($0A9D-$0AB5) ----------------
        int64_t phase_acc = (int64_t)((uint64_t(uint32_t(st.l3_phase_y & 0xFFFFFF)) << 24) |
                                      uint32_t(st.l3_phase_x & 0xFFFFFF));
        int32_t idx3 = B1(phase_acc >> 17);                // asr #$11 then b0->r2
        int32_t c3 = kCurve144B48[idx3 & 0x1FF];
        int32_t y0c = c3;
        a_acc = acc24(st.l3_state_y);
        b_acc = acc24(st.l3_state_x);
        w = st.bus_x10[0];                                 // x:(r3)+ pre-loop
        for (int i = 0; i < 16; ++i) {
            int32_t sa = B1(a_acc);
            inter[2 * i] = sa;                             // overwrites Y:$62-81
            a_acc = mac56(a_acc, w, y0c);
            a_acc = mac56(a_acc, (int32_t)(uint32_t)(-sext24(y0c) & 0xFFFFFF), sa);
            w = st.bus_x00[i];                             // x:(r1)+
            int32_t sb = B1(b_acc);
            inter[2 * i + 1] = sb;
            b_acc = mac56(b_acc, w, y0c);
            b_acc = mac56(b_acc, (int32_t)(uint32_t)(-sext24(y0c) & 0xFFFFFF), sb);
            if (i < 15) w = st.bus_x10[i + 1];
        }
        st.l3_state_y = B1(a_acc);
        st.l3_state_x = B1(b_acc);

        // ---------------- DP ($0AB7-$0AD0) ----------------
        int64_t d = acc24(atk);
        d = add56(d, acc24(0x400000));                     // sub #>$400000 (0.5)
        if (sext24(B1(d)) < 0) d = neg56(d);               // abs b
        d = (uint64_t(d) << 1) & 0xFFFFFFFFFFFFFFULL;      // asl b (2|ATK-0.5|)
        int32_t x0d = B1(d);
        int32_t depth = B1(mac56(0, x0d, x0d));            // mpy x0,x0,a
        int32_t y0d = dec;
        if (amp_phase == 4) y0d = 0;                       // KILL gate ($0AC4)
        int32_t x1d = depth;
        int64_t a2 = 0, b2 = acc24(amp_phase);             // b reused for phase!
        int32_t xd = l2out[0];                             // x0 = Y:$20 (AC6)
        int32_t a_vals[16], b_vals[16];
        for (int i = 0; i < 16; ++i) {
            a2 = mac56(0, y0d, xd);                        // mpy y0,x0,a
            int32_t y1 = inter[2 * i];                     // y:(r4)+ (even)
            a2 = (uint64_t(a2) << 1) & 0xFFFFFFFFFFFFFFULL;// asl #$1
            a2 = mac56(a2, y1, x1d);                       // mac y1,x1,a
            a_vals[i] = B1(a2);
            int32_t xb = l2out[2 * i + 1];                 // Y:$21+2i (odd)
            b2 = mac56(0, y0d, xb);
            y1 = inter[2 * i + 1];
            b2 = (uint64_t(b2) << 1) & 0xFFFFFFFFFFFFFFULL;// asl #$1
            b2 = mac56(b2, y1, x1d);
            b_vals[i] = B1(b2);
            if (i < 15) xd = l2out[2 * (i + 1)];           // next even cell
        }
        // pipeline-exact store map (stores latch PRE-ALU accumulators):
        st.coeff_a[0] = depth;                             // X:$51 (pre_a @ iter0)
        for (int i = 0; i < 15; ++i) st.coeff_a[i + 1] = a_vals[i];   // X:$52-$60
        st.a_final = a_vals[15];                           // X:$61 (post-loop)
        st.coeff_b[0] = amp_phase;                         // X:$40 (pre_b @ iter0)
        for (int i = 0; i < 15; ++i) st.coeff_b[i + 1] = b_vals[i];   // X:$41-$4F
        st.b_final = b_vals[15];                           // X:$50 (post-loop)
        st.x40_phase_leak = amp_phase;
    }

private:
    static int64_t add56(int64_t a, int64_t b) {
        uint64_t r = (uint64_t(a) + uint64_t(b)) & 0xFFFFFFFFFFFFFFULL;
        return int64_t(r);
    }    static int64_t neg56(int64_t a) {
        return int64_t((uint64_t(0) - uint64_t(a)) & 0xFFFFFFFFFFFFFFULL);
    }
};

} // namespace mnm
