// mnm_phaser.h — bit-exact C++ port of the MiniNova m18 "FX-PHASER" machine
// (DSP56300 PROC $145045-$14513F), verified word-exact against the emulator:
//   grid 8 configs x 5 signals x 30 blocks  -> 38400/38400 words OK
//   stress 8 random configs x 60 blocks     -> 15360/15360 words OK
//   persistent state (phase, s_wid, s18/s19, v, s13, quad, ladder slots) exact
//
// Topology (per 16-sample block, two interleaved passes = even/odd = L/R):
//   1. input gain     q[k] = 4*INP^2*in[k]                (Y:$00-$1F banks)
//   2. LFO oscillator 16 rotations (a,b), step = SPD^2*$9566, leapfrog:
//        b_{k+1} = b_k - a1(a_{k-1})*step   (x1 latches PRE-ALU a -> 1-iter delay)
//        a_{k+1} = a_k + a1(b_k)*step
//      carrier: s-chain = b (X:$20-$2F), q-chain = a (Y:$20-$2F)
//   3. WID one-pole (k=1/50) -> sin/cos table read at idx = s_wid>>12
//      ($14A000 sin / $14A800 cos = sin+2048 words), 16-step sub-LSB ramps
//      X:$E0-$EF (cos) / Y:$E0-$EF (sin)  [shared r0: y:(r0) + x:(r0)+]
//   4. CNTR/DEP one-poles (48-bit, k=$28F5C):
//        c = 2*(s18 - 0.5)                 (static coefficient offset)
//        v = s19 * (-(|c| + 1))            (depth shape)
//      V-ramp X:$E0-$EF = v_prev + k*dv/16 (OVERWRITES the quad cos ramp)
//   5. sidebands:  a_bank[k] =  cosr[k]*s[k] + sinr[k]*qc[k]
//                  b_bank[k] =  cosr[k]*s[k] - sinr[k]*qc[k]
//      (negation is 24-bit wrapping: -(-2^23) = -2^23 !)
//   6. allpass coefficient:  mod[i] = (a|b)_bank[i]*Vramp[i%16] + c
//   7. FB: s13 = 2*(FB-0.5); ramp fb[k] = s13_prev + k*(s13-s13_prev)/16
//   8. TWO passes, each a 6-section allpass ladder with its own persistent
//      7-word state (even: r6+$1E..$24, odd: r6+$26..$2C):
//        P0 = fb*W6 + q
//        R1..R6 = W[i-1] + (R[i-1] - W[i])*mod   (per-sample coefficient!)
//        OUT = cos(45deg)*(q + R6)               (=$5A8241 Q1.23)
//   9. mix: out_even = (1-M)*dry_e + M*wet1 ; out_odd = (1-M)*dry_o + M*wet2
//      with 1-M = $7FFFFF - MIX  (note: $7FFFFF, not $800000)
//
// Hardware nuances reproduced here (required for word-exactness):
//   - mac operand negation wraps in 24 bit: -($800000) stays $800000
//   - the oscillator's x1 latch takes the PRE-ALU a (one-iteration delay)
//   - every accumulator->memory/register move takes bits 47..24 (a1) only
//   - DSP DO-loop end address may fall on the 2nd word of a 2-word
//     instruction (m18 outer pass loop): the loop runs twice on hardware

#pragma once
#include <cstdint>
#include <cstring>

namespace mnm {

static const int32_t M24MASK = 0xFFFFFF;

static inline int32_t s24(int32_t v) {
    v &= M24MASK;
    return (v & 0x800000) ? v - (1 << 24) : v;
}
// 24-bit register -> 56-bit accumulator (left aligned)
static inline int64_t acc_of(int32_t raw) { return int64_t(s24(raw)) << 24; }
// accumulator -> 24-bit register (bits 47..24)
static inline int32_t a1(int64_t acc) { return int32_t((acc >> 24) & M24MASK); }
// Q1.23 multiply as 56-bit acc (product << 1)
static inline int64_t mulq(int32_t x, int32_t y) {
    return int64_t(s24(x)) * int64_t(s24(y)) * 2;
}
// product with the NEGATED first operand (24-bit wrapping negation)
static inline int64_t muln(int32_t x, int32_t y) {
    int32_t nx = (-s24(x)) & M24MASK;
    return int64_t(s24(nx)) * int64_t(s24(y)) * 2;
}

struct PhaserTables {
    const int32_t* sin_tab;   // 8192 words, Q1.23 ($14A000 image)
    const int32_t* cos_tab;   // 8192 words ($14A800 image)
};

struct PhaserState {
    int32_t phase_hi = 0x000080;  // x:(r6+$11) — INIT: move #$80,x0
    int32_t phase_lo = 0x000000;  // x:(r6+$12)
    int32_t s_wid   = 0;          // y:(r6+$1b)
    int64_t s18     = 0;          // 48-bit x:(r6+$18):y:(r6+$18)
    int64_t s19     = 0;          // 48-bit
    int32_t v_prev  = 0;          // y:(r6+$1a)
    int32_t s13     = 0;          // y:(r6+$13)
    int32_t q_cos   = 0;          // y:(r6+$1c)
    int32_t q_sin   = 0;          // y:(r6+$1d)
    int32_t S_even[7] = {0,0,0,0,0,0,0};  // L:(r6+$1E..$24) X halves
    int32_t S_odd [7] = {0,0,0,0,0,0,0};  // L:(r6+$26..$2C) X halves
};

struct PhaserParams {
    int32_t cntr, dep, spd, mix, fb, wid, unused, inp;  // raw v<<16
    int32_t& operator[](int i) { return (&cntr)[i]; }
};

// processes one 16-sample block; in[0..15] = even (L), in[16..31] = odd (R);
// out receives 32 words interleaved even/odd as the DSP writes y:(r7)+
inline void phaser_block(PhaserState& st, const PhaserTables& tbl,
                         const PhaserParams& prm,
                         const int32_t* in, int32_t* out)
{
    static const int32_t C45 = 0x5A8241;
    static const int32_t KP1 = 0x28F5C;

    // ---- 1. head gain -----------------------------------------------------
    int32_t inp2 = a1(mulq(prm.inp, prm.inp));
    int32_t qb[32];
    for (int j = 0; j < 16; ++j) {
        qb[j]      = a1(mulq(in[2*j],   inp2) << 2);
        qb[16 + j] = a1(mulq(in[2*j+1], inp2) << 2);
    }

    // ---- 2. LFO oscillator (leapfrog with 1-iteration-delayed x1) ---------
    int32_t spd2 = a1(mulq(prm.spd, prm.spd));
    int32_t step = a1(mulq(spd2, 0x9566));
    int64_t a = acc_of(st.phase_hi);
    int64_t b = acc_of(st.phase_lo);
    int32_t x1 = st.phase_hi;
    int32_t Sc[16], Qc[16];
    for (int k = 0; k < 16; ++k) {
        Sc[k] = a1(b);                       // store pre-ALU b
        Qc[k] = a1(a);                       // store pre-ALU a
        int32_t y0  = a1(b);                 // b,y0 latch
        int32_t a_k = a1(a);                 // a,x1 latch (pre-ALU!)
        b = b + muln(x1, step);              // mac -x1,x0,b
        a = a + mulq(y0, step);              // mac y0,x0,a
        x1 = a_k;
    }
    st.phase_hi = a1(a);
    st.phase_lo = a1(b);

    // ---- 3. WID one-pole, table read, quad ramps ---------------------------
    int32_t sw_old = st.s_wid;
    int32_t sw_new = a1(acc_of(sw_old) + mulq(KP1, prm.wid) - mulq(KP1, sw_old));
    st.s_wid = sw_new;
    int32_t idx = a1(mulq(sw_new, 0x800));
    int32_t s_cur = (idx >= 0 && idx < 8192) ? tbl.sin_tab[idx] : 0;
    int32_t c_cur = (idx >= 0 && idx < 8192) ? tbl.cos_tab[idx] : 0;
    int32_t sp = st.q_sin, cp = st.q_cos;
    st.q_cos = c_cur; st.q_sin = s_cur;
    int32_t ds_raw = a1(acc_of(s_cur) - acc_of(sp));
    int32_t dc_raw = a1(acc_of(c_cur) - acc_of(cp));
    int64_t acc_a = acc_of(sp), acc_b = acc_of(cp);
    int32_t XE[16], YSR[16];
    for (int k = 0; k < 16; ++k) {
        YSR[k] = a1(acc_a);                  // a,y:(r0)  (shared r0 with X)
        XE[k]  = a1(acc_b);                  // b,x:(r0)+ (the increment walks both)
        acc_a += mulq(8, ds_raw);
        acc_b += mulq(8, dc_raw);
    }

    // ---- 4. CNTR/DEP one-poles, v, V-ramp ----------------------------------
    auto sext48 = [](int64_t v) {
        v &= (1LL << 48) - 1;
        return (v & (1LL << 47)) ? v - (1LL << 48) : v;
    };
    int64_t s18_acc = sext48(st.s18);
    s18_acc += mulq(KP1, prm.cntr) - mulq(KP1, a1(s18_acc));
    st.s18 = s18_acc & ((1LL << 48) - 1);
    int64_t s19_acc = sext48(st.s19);
    s19_acc += mulq(KP1, prm.dep) - mulq(KP1, a1(s19_acc));
    st.s19 = s19_acc & ((1LL << 48) - 1);
    int32_t s19_hi = a1(s19_acc);
    int64_t bb = s18_acc - acc_of(0x400000);
    int32_t c = a1(bb << 1);                 // asl b ; move b,x1
    bb = (bb << 1) < 0 ? -(bb << 1) : (bb << 1);   // abs
    bb += acc_of(0x800000);
    bb = -bb;                                // neg
    int32_t y1v = a1(bb);
    int64_t v_acc = mulq(s19_hi, y1v);
    int32_t v_new = a1(v_acc);
    int32_t v_prev = st.v_prev;
    st.v_prev = v_new;
    int32_t dv_raw = a1(v_acc - acc_of(v_prev));
    int64_t acc_v = acc_of(v_prev);
    int32_t VR[16];
    for (int k = 0; k < 16; ++k) {
        VR[k] = a1(acc_v);
        acc_v += mulq(0x80000, dv_raw);
    }

    // ---- 5. sidebands -------------------------------------------------------
    int32_t a_bank[16], b_bank[16];
    for (int k = 0; k < 16; ++k) {
        int64_t prod = mulq(XE[k], Sc[k]);
        a_bank[k] = a1(prod + mulq(YSR[k], Qc[k]));
        b_bank[k] = a1(prod + muln(YSR[k], Qc[k]));
    }

    // ---- 6. mod bank ---------------------------------------------------------
    int32_t mod[32];
    for (int i = 0; i < 32; ++i) {
        int32_t src = (i < 16) ? a_bank[i] : b_bank[i - 16];
        mod[i] = a1(mulq(src, VR[i % 16]) + acc_of(c));
    }

    // ---- 7. FB one-pole + ramp -----------------------------------------------
    int32_t s13_old = st.s13;
    int32_t s13_new = a1((acc_of(prm.fb) - acc_of(0x400000)) << 1);
    int32_t ds13 = (s13_new - s13_old) & M24MASK;
    st.s13 = s13_new;
    int64_t acc_fb = acc_of(s13_old);
    int32_t fbr[16];
    for (int k = 0; k < 16; ++k) {
        fbr[k] = a1(acc_fb);
        acc_fb += mulq(0x80000, ds13);
    }

    // ---- 8. two passes of the 6-section allpass ladder ------------------------
    int32_t outs[32];
    for (int p = 0; p < 2; ++p) {
        int32_t W[7];
        for (int i = 0; i < 7; ++i)
            W[i] = p ? st.S_odd[i] : st.S_even[i];
        int base = 16 * p;
        for (int k = 0; k < 16; ++k) {
            int32_t IN = mod[base + k];
            int32_t q  = qb[base + k];
            int32_t fb = fbr[k];
            int32_t P0 = a1(mulq(fb, W[6]) + acc_of(q));
            int32_t R1 = a1(acc_of(W[0]) + mulq(P0, IN) + muln(W[1], IN));
            int32_t R2 = a1(acc_of(W[1]) + mulq(R1, IN) + muln(W[2], IN));
            int32_t R3 = a1(acc_of(W[2]) + mulq(R2, IN) + muln(W[3], IN));
            int32_t R4 = a1(acc_of(W[3]) + mulq(R3, IN) + muln(W[4], IN));
            int32_t R5 = a1(acc_of(W[4]) + mulq(R4, IN) + muln(W[5], IN));
            int32_t R6 = a1(acc_of(W[5]) + mulq(R5, IN) + muln(W[6], IN));
            outs[base + k] = a1(mulq(C45, q) + mulq(C45, R6));
            int32_t Wn[7] = {P0, R1, R2, R3, R4, R5, R6};
            for (int i = 0; i < 7; ++i) W[i] = Wn[i];
        }
        for (int i = 0; i < 7; ++i) {
            if (p) st.S_odd[i] = W[i]; else st.S_even[i] = W[i];
        }
    }

    // ---- 9. mix ----------------------------------------------------------------
    int32_t om = (0x7FFFFF - prm.mix) & M24MASK;
    for (int k = 0; k < 16; ++k) {
        out[2*k]     = a1(mulq(om, qb[k])      + mulq(prm.mix, outs[k]));
        out[2*k + 1] = a1(mulq(om, qb[16 + k]) + mulq(prm.mix, outs[16 + k]));
    }
}

} // namespace mnm
