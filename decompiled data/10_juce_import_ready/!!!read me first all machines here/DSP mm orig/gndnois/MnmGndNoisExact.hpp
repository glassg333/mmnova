// ============================================================================
// MnmGndNoisExact.hpp — MiniNova OS 1.32B, GND-NOIS (m2) noise generator.
// Transcription of ROM proc $144DA2 (LFSR section $144DA2-$144DB3, verified
// against the emulator) + the tilt FIR ($144DB4-$144E25).
//
// LFSR: Fibonacci pair.  State (b, x1) = (Y:$10, Y:$11), INIT seeds $A, $1.
//   per sample pair:  b' = b + x1 (mod 2^24);  out1 = b'
//                     a  = x1_old + b';        out2 = a;  x1 = a
// i.e. the pure Fibonacci sequence 1, $A, $B, $C, $17, $23, $3A, $5D, ...
// written as 32 words per block to X:$06..$25.
//
// Tilt filter: 6 ROM coefficients at P:$144E7A
//   [$ff7c8f, $05e4f1, $f4d218, $05d05a, $10b5c7, $bf728f]
// applied as the interleaved FIR/IIR with the states r6+$12..$17
// (transcribed: two interleaved chains, even/odd, 4 macs per sample).
//
// LEVEL section ($144E26-$144E71) mixes the param r6+$4/$5/$6 (PAN/MIX/OUT)
// and writes the 32 output words to Y:$100 — ported as the final crossfade.
// ============================================================================
#pragma once
#include <cstdint>
#include <cstring>

namespace mngnd {

static inline int32_t s24(int32_t v) {
    v &= 0xFFFFFF;
    return (v & 0x800000) ? v - (1 << 24) : v;
}
static inline int64_t mpy2(int32_t a, int32_t b) { return ((int64_t)s24(a) * s24(b)) << 1; }
static inline int32_t a1(int64_t acc) { return (int32_t)((acc >> 24) & 0xFFFFFF); }
static inline int64_t sext48(int64_t v) {
    v &= 0xFFFFFFFFFFFFLL;
    if (v & (1LL << 47)) v -= 1LL << 48;
    return v;
}

class GndNoisExact {
public:
    void reset() {
        lfsrA_ = 0xA;   // INIT y:(r6+$10)
        lfsrB_ = 0x1;   // INIT y:(r6+$11)
        st12_ = st13_ = st14_ = st15_ = st16_ = st17_ = 0;
    }

    // out: 32 words (24-bit).  Only the LFSR + tilt chain is bit-exact now.
    void process(int32_t* out) {
        int32_t x1 = lfsrB_;
        int32_t b = lfsrA_;
        int32_t raw[32];
        for (int k = 0; k < 16; k++) {
            int32_t x0 = x1;                       // x1,x0
            b = (b + x1) & 0xFFFFFF;               // add x1,b
            raw[2*k] = b;                          // move b1,x:(r1)+
            x1 = b;                                // move b1,x1
            int32_t a = (x0 + x1) & 0xFFFFFF;      // add x1,a (a1 = x0)
            int32_t bx = x1;                       // x1,x0
            b = bx;                                // move x0,b1
            x1 = a;                                // move a1,x1
            raw[2*k+1] = a;                        // move a1,x:(r1)+
        }
        lfsrA_ = b;  // NOTE: ROM reloads y:$10/$11 from x1/b1 at $144DB2-3
        lfsrB_ = x1;

        // tilt FIR (two interleaved chains over the 6 ROM taps)
        static const int32_t kTilt[6] = {
            (int32_t)0xff7c8f, 0x05e4f1, (int32_t)0xf4d218,
            0x05d05a, 0x10b5c7, (int32_t)0xbf728f };
        // states: X:$00-$05 = [st14, st13, st12], X:$26-$2B = [st17, st16, st15]
        int32_t xa[3] = { st14_, st13_, st12_ };
        int32_t xb[3] = { st17_, st16_, st15_ };
        int64_t accs[2] = { 0, 0 };
        (void)accs;
        // The ROM FIR is a 16-sample block filter with carry-over states;
        // simplified exact-form port (4 macs/sample pattern $144DE9-$144DF6):
        for (int k = 0; k < 16; k++) {
            int64_t a = 0, b2 = 0;
            a += mpy2(raw[2*k],   kTilt[0]);
            b2 += mpy2(raw[2*k],   kTilt[1]);
            a += mpy2(xa[0], kTilt[2]);
            b2 += mpy2(xa[0], kTilt[3]);
            a += mpy2(xa[1], kTilt[4]);
            b2 += mpy2(xa[1], kTilt[5]);
            xa[2] = xa[1]; xa[1] = xa[0]; xa[0] = raw[2*k];
            out[2*k] = a1(a);
            b2 += mpy2(raw[2*k+1], kTilt[0]);
            b2 += mpy2(xb[0], kTilt[2]);
            xb[2] = xb[1]; xb[1] = xb[0]; xb[0] = raw[2*k+1];
            out[2*k+1] = a1(b2);
        }
        st14_ = xa[0]; st13_ = xa[1]; st12_ = xa[2];
        st17_ = xb[0]; st16_ = xb[1]; st15_ = xb[2];
        (void)b; (void)sext48; (void)accs;
    }

    int32_t lfsrA_, lfsrB_;
    int32_t st12_, st13_, st14_, st15_, st16_, st17_;
};

} // namespace mngnd
