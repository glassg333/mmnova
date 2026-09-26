// =============================================================================
// MnmGroundSin.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m1 "GND-SIN" — bit-exact port of DSP1 P:$144CC9-$144D20.
// =============================================================================
// Firmware provenance (verified against a bit-exact DSP56300 emulator running
// the original firmware; experiments exp51/exp52/exp53):
//
//   P:$144CC9  INIT: X:(r6+$10)=$14A000 (sin-LUT base), Y:(r6+$10)=0 (phase=0)
//   P:$144CD0  PROC (per sample):
//     target48 = 2 * $17C6F9 * (a1 + a0)          ; mpysu x0,y0,a; dmac ss x0,y1,a
//                 (a1 = pitch param as Q23, a0 = 0 from the kernel convention)
//     delta48  = (target48 - targetPrev48) >> 4   ; sub b,a; asr #$4,a,a
//     inc48    = targetPrev48
//     16 substeps (do #$10):
//       inc48  += delta48                          ; add y,a
//       phase48 += inc48                           ; add a,b (48-bit, carry LSW->MSW)
//       idx     = phase.MSW & $1FFF                ; and x0,b  (13-bit, 8192 LUT)
//       ptr     = $14A000 + idx                    ; add x1,b  ($14A000 % 8192 == 0)
//       store ptr:frac to L:(r1)+
//     phase state kept = ptr-form (base == 0 mod 8192, equivalent to raw idx)
//     store last phase to r6+$10, target48 to r6+$12
//   P:$144CF2-$144D0A  interpolation, do #$8, iteration i:
//       a = interp(phase[2i]),  b = interp(phase[2i+1]) with the EXACT chain:
//         acc  = (-LUT[p])   * frac   * 2         ; mpysu -x1,y0,a   (frac unsigned)
//         acc +=  LUT[p] << 24  (twice)           ; add x1,a / add x1,a
//         acc +=  LUT[p+1]   * frac   * 2         ; macsu x0,y0,a
//         acc >>= 1 ; acc >>= 2                   ; asr a / asr #$2,a,a
//         v = (acc >> 24) & $FFFFFF               ; a1
//       => v = (LUT[p]*(2^24 - frac) + LUT[p+1]*frac) >> 2   (Q23, +-0.25)
//   P:$144D02/D07/D08/D09  store order per iteration: a, a, b, b
//       => 32-word Y:(r7)+ stream (v0,v0,v1,v1,...,v15,v15)
//   Kernel consumes (Y:$100, Y:$101) = (v0, v0) as L/R per sample.
//   P:$144D21-$144D99  two 32-tap tail FIRs — NO callers anywhere in the
//       kernel or machine pages (dead code within the slice); not ported.
//
//   ROM sine LUT X/Y:$14A000, 8192 words, quadrature offset +2048 (see
//   MNM_RING_MOD_MINING.md): table[i] = round(sin(2*pi*i/8192) * (2^23-1)).
//   Amplitude of the machine output: +-0.25 (LUT/4).
//
//   TUNE knob: the phase advances 16 substeps/sample; the audible pitch is the
//   folded alias of 16*inc (the stock machine's known alias-sweep character);
//   reproducing the algorithm reproduces the exact pitch law by construction.
// =============================================================================

#pragma once
#include <cstdint>
#include <cmath>

namespace mnm {
namespace gs {

static const uint32_t M24 = 0xFFFFFFu;

inline int64_t sext48(int64_t v) {
    v &= (1ll << 48) - 1;
    const int64_t s = 1ll << 47;
    return (v ^ s) - s;
}
inline int32_t s24(int32_t v) {
    v &= (int32_t)M24;
    return (v ^ 0x800000) - 0x800000;
}

// ROM sine LUT (8192 words) — identical to firmware X/Y:$14A000.
struct SinLUT {
    int32_t t[8192];
    SinLUT() {
        for (int i = 0; i < 8192; ++i)
            t[i] = (int32_t)llround(sin(2.0 * M_PI * i / 8192.0) * 8388607.0);
    }
};

class GroundSin {
public:
    GroundSin() : phase(0), targetPrev(0) {}

    // pitch = machine pitch value as it arrives in accumulator A (a1), i.e.
    // the Q23 word the kernel passes (knob value << 16 for raw knobs).
    void setPitchQ23(int32_t a1) { pitchA1 = a1 & (int32_t)M24; }

    // One 16-sample frame. Emits the firmware's 32-word Y:(r7)+ stream:
    // (v0,v0, v1,v1, ..., v15,v15) — 16 mono samples, L==R (kernel reads the
    // interleaved stereo pairs). The Monomachine engine runs machines on
    // 16-sample frames; the 16 phase steps ARE the 16 samples of the frame.
    void processFrame(int32_t* out32) {
        const static SinLUT lut;

        // --- target increment (P:$144CD0-$144CD9) -----------------------
        const int64_t x0 = (int64_t)0x17C6F9;               // signed
        int64_t target = (x0 * (int64_t)pitchA1 * 2);       // acc = x0*a1<<1 (a0=0)
        target = sext48(target);

        // --- glide (P:$144CDA-$144CE0) ----------------------------------
        int64_t delta = target - targetPrev;
        delta = sext48(delta) >> 4;                          // asr #$4 (48-bit)
        int64_t inc = targetPrev;                            // tfr b,a

        // --- 16 frame samples (P:$144CE7-$144CEC + interp $144CF2-$144D0A)
        // The parallel store `add a,b  b,l:(r1)+` captures the OLD accumulator,
        // so sample k is the interpolation of the phase after k adds.
        uint64_t ph = phase;                                 // 37-bit after mask
        for (int k = 0; k < 16; ++k) {
            const uint32_t idx = (uint32_t)(ph >> 24) & 0x1FFFu;
            const int32_t s0 = lut.t[idx];
            const int32_t s1 = lut.t[(idx + 1) & 0x1FFF];
            const int64_t frac = (int64_t)(ph & M24);        // unsigned (macsu)
            int64_t acc = -(int64_t)s24(s0) * frac * 2;      // mpysu -x1,y0,a
            acc += (int64_t)s24(s0) << 24;                   // add x1,a
            acc += (int64_t)s24(s0) << 24;                   // add x1,a
            acc += (int64_t)s24(s1) * frac * 2;              // macsu x0,y0,a
            acc >>= 1;                                       // asr a
            acc >>= 2;                                       // asr #$2,a,a
            const int32_t v = (int32_t)((uint64_t)(acc >> 24) & M24);  // a1
            out32[2 * k] = v;                                // store order a,a,b,b
            out32[2 * k + 1] = v;                            // => (v_k, v_k)
            inc += delta;                                    // add y,a
            ph += (uint64_t)inc;                             // add a,b (48-bit wrap)
            ph &= 0x1FFFFFFFFFull;                           // and $1FFF on MSW == mod 2^37
        }
        phase = ph;
        targetPrev = target;
    }

    // Single-sample convenience wrapper (advances a whole frame; use
    // processFrame for the exact firmware sample stream).
    inline void process(int32_t* L, int32_t* R) {
        int32_t buf[32];
        processFrame(buf);
        *L = buf[0];
        *R = buf[1];
    }

private:
    uint64_t phase = 0;       // 37-bit used (13-bit LUT idx << 24 | 24-bit frac)
    int64_t  targetPrev = 0;  // 48-bit
    int32_t  pitchA1 = 0;
};

} // namespace gs
} // namespace mnm
