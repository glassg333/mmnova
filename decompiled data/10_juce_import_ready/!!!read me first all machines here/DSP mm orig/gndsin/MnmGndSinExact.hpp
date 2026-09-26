// ============================================================================
// MnmGndSinExact.hpp — MiniNova OS 1.32B, GND-SIN (m1) oscillator.
// Transcription of ROM proc $144CD0-$144D20 (+ INIT $144CC9).
//
// 16 samples per call, each written twice (L/R) -> 32 output words.
// Pitch = the kernel's 48-bit word (a1:a0).  Increment = 2*$17C6F9*pitch
// (mpysu s*u on a0 + dmac s*s on a1), ramped 1/16 of the delta per block.
// Phase: 48-bit.  Per sample an L-pair is built:
//   L:X[j] = $14A000 + (phase & $1FFF)   (sin table pointer)
//   L:Y[j] = fraction (b0 before masking)
// then linear interpolation:
//   s = (2*sin[p] - 2*frac*sin[p] + 2*frac*sin[p+1] + 2*sin[p]·2^24 terms) >>3
//   (exact: a = -mpysu(x1,y0) + add x1 + add x1 + macsu(x0,y0); asr; asr#2)
// ROM write pattern per iteration: [a,a,b,b] -> out[4k..4k+3].
// $144D0E-$144D1F (do #<10 / do #<18 nops) = ROM timing filler, no-op.
// ============================================================================
#pragma once
#include <cstdint>
#include <cstring>

namespace mngnd {

static inline int32_t s24(int32_t v) {
    v &= 0xFFFFFF;
    return (v & 0x800000) ? v - (1 << 24) : v;
}
static inline int64_t mpy2s(int32_t a, int32_t b) { return ((int64_t)s24(a) * s24(b)) << 1; }
static inline int64_t mpy2su(int32_t a, uint32_t b) { return ((int64_t)s24(a) * (b & 0xFFFFFF)) << 1; }
static inline int64_t ld24(int32_t v) { return (int64_t)s24(v) << 24; }
static inline int64_t sext48(int64_t v) {
    v &= 0xFFFFFFFFFFFFLL;
    if (v & (1LL << 47)) v -= 1LL << 48;
    return v;
}
extern int32_t kSinTabG[8192];   // = mnmrev::kSinTab (alias defined in the .cpp/test)

class GndSinExact {
public:
    void reset() {
        incr_ = 0;
        phaseX_ = 0x14A000;   // INIT $144CC9: x:(r6+$10) = $14A000
        phaseF_ = 0;          //              y:(r6+$10) = 0
    }

    void process(int64_t pitch, int32_t* out) {
        // increment: mpysu x0,y0,a (s*u, y0 = pitch a0 unsigned)
        //            dmac  ss x0,y1,a (s*s, y1 = pitch a1)
        int64_t newIncr = sext48(
            (int64_t)0x17C6F9 * (pitch & 0xFFFFFF) +
            (int64_t)0x17C6F9 * s24((int32_t)((pitch >> 24) & 0xFFFFFF)) );
        // <<1 happens inside mpysu/dmac; the sum is already the final acc:
        newIncr = sext48(newIncr << 1);
        int64_t oldIncr = incr_;
        int64_t delta = sext48(newIncr - oldIncr);   // sub b,a
        incr_ = newIncr;                             // save r6+$12
        delta >>= 4;                                 // asr #$4
        int64_t a = oldIncr;                         // tfr b,a
        int64_t b = ld24(phaseX_) | (phaseF_ & 0xFFFFFF);
        for (int j = 0; j < 16; j++) {
            a += delta;                              // add y,a
            // parallel move latches the PRE-ALU accumulator:
            lbufX_[j] = (int32_t)((b >> 24) & 0xFFFFFF);  // b,l:(r1)+
            lbufY_[j] = (int32_t)(b & 0xFFFFFF);
            b += a;                                  // add a,b
            b &= 0x1FFFLL << 24;                     // and #$1fff (a1)
            b += 0x14A000LL << 24;                   // add #$14a000
        }
        phaseX_ = (int32_t)((b >> 24) & 0xFFFFFF);   // save r6+$10
        phaseF_ = (int32_t)(b & 0xFFFFFF);

        // interpolation ($144CF2-$144D09) — exact pointer walk
        int r1 = 0, r4 = 0;
        int r2 = lbufX_[r1++] & 0x1FFF;              // move x:(r1)+,r2
        int32_t y0 = lbufY_[r4++];                   // move y:(r4)+,y0
        int32_t x1 = kSinTabG[r2]; r2 = (r2 + 1) & 0x1FFF;   // x:(r2)+,x1
        int r0 = lbufX_[r1++] & 0x1FFF;              // x:(r1)+,r0
        for (int k = 0; k < 8; k++) {
            // a-chain: sample 2k
            int32_t x0 = kSinTabG[r2];               // x:(r2),x0
            int64_t acc = -mpy2su(x1, y0);           // mpysu -x1,y0,a
            acc += ld24(x1);                         // add x1,a
            acc += ld24(x1);                         // add x1,a
            acc += mpy2su(x0, y0);                   // macsu x0,y0,a
            acc >>= 1;                               // asr a
            x1 = kSinTabG[r0]; r0 = (r0 + 1) & 0x1FFF;   // x:(r0)+,x1
            y0 = lbufY_[r4++];                       // y:(r4)+,y0
            acc >>= 2;                               // asr #$2
            int32_t av = (int32_t)((acc >> 24) & 0xFFFFFF);
            // b-chain: sample 2k+1
            int32_t bx0 = kSinTabG[r0];              // x:(r0),x0
            int64_t bc = -mpy2su(x1, y0);            // mpysu -x1,y0,b
            bc += ld24(x1);                          // add x1,b
            bc += ld24(x1);                          // add x1,b
            bc += mpy2su(bx0, y0);                   // macsu x0,y0,b
            bc >>= 1;                                // asr b
            x1 = kSinTabG[r2]; r2 = (r2 + 1) & 0x1FFF;   // x:(r2)+,x1
            y0 = lbufY_[r4++];                       // y:(r4)+,y0
            bc >>= 2;                                // asr #$2
            int32_t bv = (int32_t)((bc >> 24) & 0xFFFFFF);
            out[4*k+0] = av; out[4*k+1] = av;        // a,a
            out[4*k+2] = bv; out[4*k+3] = bv;        // b,b
            // r1 walk for the next iteration
            r2 = lbufX_[r1++] & 0x1FFF;              // x:(r1)+,r2 ($144CFD)
            r0 = lbufX_[r1++] & 0x1FFF;              // x:(r1)+,r0 ($144D03)
        }
    }

    int64_t incr_;        // r6+$12 (48-bit increment)
    int32_t phaseX_;      // r6+$10 X part (ptr+phase)
    int32_t phaseF_;      // r6+$10 Y part (fraction)
    int32_t lbufX_[16], lbufY_[16];
};

} // namespace mngnd
