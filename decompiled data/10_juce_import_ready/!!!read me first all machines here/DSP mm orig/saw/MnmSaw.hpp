// =============================================================================
// MnmSaw.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m4 "SWAVE-SAW" — instruction-exact transcription of DSP1
// P:$145932-$145AB4 (INIT $145932, CONFIG $145949 = rts, PROC $14594A).
// Includes 1:1 transcriptions of the shared kernel helpers func_0003E0 /
// func_0003AA (wave-frame interpolation engine) as invoked by this machine.
//
// Knob map (Y:(r6+off), raw q23 words = knob<<16):
//   UNIL=$4  UNIW=$5  UNIX=$6  ---=$7  SUBX=$8  SUB1=$9  SUB2=$A   TUNE in A
// Machine input bus: x:(r6-$27).
// Tables: X:$101BFB/$101CFB/$101B7B/$101C7B (gain-indexed), X:$143D06 /
// X:$1435C6 (morph) — extracted bit-exact in mnm_saw_tables.h; the saw table
// X:$14A000 (8K, m2=$1FFF) and machine-wave X:$140000 use the documented
// runtime default tbl[i] = i << 11 (host-uploaded data in real hardware).
//
// *** STATUS: WORK-IN-PROGRESS, NOT YET VALIDATED ***  (transcription draft;
// test-driven fix pass pending — vectors: research/m4_vectors.txt,
// test: research/test_mnm_saw.cpp). Do NOT treat as firmware-identical yet.
// =============================================================================

#pragma once
#include <cstdint>
#include <cstring>
#include "mnm_saw_tables.h"

namespace mnm {
namespace saw {

static const uint32_t M24 = 0xFFFFFFu;
static const uint64_t M48 = 0xFFFFFFFFFFFFull;
static const uint64_t M56 = 0xFFFFFFFFFFFFFFull;

// ---------------------------------------------------------------- primitives
inline int64_t sext56(int64_t v) {
    v &= (int64_t)M56;
    if (v & (1ll << 55)) v -= (1ll << 56);
    return v;
}
inline int64_t sext48(int64_t v) {              // L-memory pair -> acc (sign from bit 47)
    v &= (int64_t)M48;
    if (v & (1ll << 47)) v -= (1ll << 48);
    return v;
}
inline int64_t sgn24(int64_t w) {
    w &= M24;
    return (w ^ 0x800000) - 0x800000;
}
inline int64_t compose24(uint32_t w) { return sgn24(w) << 24; }
inline uint32_t satA1(int64_t a) {
    int64_t v = sext56(a);
    if (v > 0x007FFFFFFFFFFFll) return 0x7FFFFF;
    if (v < -0x00800000000000ll) return 0x800000;
    return (uint32_t)((v >> 24) & M24);
}
inline uint32_t rawA1(int64_t a) { return (uint32_t)((sext56(a) >> 24) & M24); }
inline uint32_t rawA0(int64_t a) { return (uint32_t)(sext56(a) & M24); }
inline int64_t mac_s(int64_t acc, uint32_t x, uint32_t y) {
    return sext56(acc + sgn24(x) * sgn24(y) * 2);
}
inline int64_t mac_uu(int64_t acc, uint32_t x, uint32_t y) {
    return sext56(acc + (int64_t)(x & M24) * (y & M24) * 2);
}
inline int64_t mac_su(int64_t acc, uint32_t x, uint32_t y) {   // mpysu: s1 x u2
    return sext56(acc + sgn24(x) * (int64_t)(y & M24) * 2);
}
inline int64_t dmac_su(int64_t acc, uint32_t x, uint32_t y) { return mac_su(acc, x, y); }
inline int64_t dmac_ss(int64_t acc, uint32_t x, uint32_t y) { return mac_s(acc, x, y); }
inline int64_t mpyr_s(uint32_t x, uint32_t y) {
    int64_t v = sext56(sgn24(x) * sgn24(y) * 2 + 0x800000);
    return v & ~(int64_t)M24;
}
inline int64_t mpyi_s(uint32_t imm, uint32_t x) { return mac_s(0, imm, x); }
inline int64_t maci_s(int64_t acc, uint32_t imm, uint32_t x) { return mac_s(acc, imm, x); }
inline int64_t mpyri_s(uint32_t imm, uint32_t x) { return mpyr_s(imm, x); }
inline uint32_t neg24(uint32_t v) { return (uint32_t)(-(int64_t)(v & M24)) & M24; }
inline int64_t asr56(int64_t v, int n) { return sext56(v) >> n; }
inline int64_t lsr56(int64_t v, int n) {
    return (int64_t)(((uint64_t)sext56(v) & M56) >> n);
}
inline int64_t asl56(int64_t v, int n) {
    if (n <= 0) return sext56(v);
    uint64_t u = (uint64_t)sext56(v) & M56;
    return sext56((int64_t)((u << n) & M56));
}
inline int64_t iabs56(int64_t v) {
    v = sext56(v);
    return sext56(v < 0 ? -v : v);
}
inline int64_t logicA1(int64_t a, uint32_t w) {
    // and/or/eor touch A1 only; A2 and A0 bits are preserved as-is.
    int64_t s = sext56(a);
    uint32_t a1 = (uint32_t)((s >> 24) & M24) & w;
    return (s & ~(int64_t)M48) | ((int64_t)a1 << 24);
}
inline uint32_t clbA1(int64_t a) {
    uint32_t v = rawA1(a);
    uint32_t sign = (v >> 23) & 1;
    uint32_t cnt = 1;
    for (int bit = 22; bit >= 0; --bit) {
        if (((v >> bit) & 1) == sign) ++cnt; else break;
    }
    return cnt & M24;
}
inline int64_t normfB1(int64_t a, int64_t b) {
    int32_t sh = (int32_t)sgn24(rawA1(b)) - 1;
    if (sh >= 0) return sext56((int64_t)((uint64_t)sext56(a) << sh) & (int64_t)M56);
    return asr56(a, -sh);
}

class MnmSaw {
public:
    // ------------------------------------------------------------------
    // INIT $145932. CONFIG $145949 is an rts (no-op).
    // ------------------------------------------------------------------
    void init() {
        for (int i = 0; i < 48; ++i) Xw[0x58 + i] = 0;         // rep #$30
        for (int i = 0; i < 15; ++i) {                          // rep #$f (L space)
            Xw[0x18 + i] = 0; Yw[0x18 + i] = 0;
        }
        Xa[0xC] = 0; Xa[0xF] = 0;
        Yw[0x27] = 0x88;
        Yw[0x2D] = 0x80;
        Xw[0xD] = 0x14A000; Xw[0x10] = 0x14A000;
        for (int i = 0; i < 0x800; ++i) mw[i] = (uint32_t)(i << 11) & M24;
        for (int i = 0; i < 0x2000; ++i) sw[i] = (uint32_t)(i << 11) & M24;
    }
    void config() {}                                             // rts

    void setFrameKnobs(uint32_t unil, uint32_t uniw, uint32_t unixk,
                       uint32_t subx, uint32_t sub1, uint32_t sub2) {
        kUNIL = unil & M24; kUNIW = uniw & M24; kUNIX = unixk & M24;
        kSUBX = subx & M24; kSUB1 = sub1 & M24; kSUB2 = sub2 & M24;
    }
    void setPitchA1(uint32_t a1) { pitchA1 = a1 & M24; }
    void setMachineInput(uint32_t xw) { inX = xw & M24; }

    void processFrame(uint32_t* out32);

    // ---- state: r6-relative voice page ----
    uint32_t Xw[0x90] = {0}, Yw[0x90] = {0};
    // ---- kernel low-page scratch (absolute X/Y, sparse-zero beyond) ----
    uint32_t Xa[0x1000] = {0}, Ya[0x1000] = {0};
    uint32_t mw[0x800];                    // X:$140000 machine-wave (saw default)
    uint32_t sw[0x2000];                   // X:$14A000 saw table (saw default)

    // Absolute X read with firmware-table resolution (bit-exact regions from
    // mnm_saw_tables.h) + the runtime saw/machine-wave defaults.
    inline uint32_t rdXA(uint32_t p) const {
        if (p >= 0x101BFB && p < 0x101BFB + 0x100) return T_UNI_A[p - 0x101BFB];
        if (p >= 0x101CFB && p < 0x101CFB + 0x100) return T_UNI_B[p - 0x101CFB];
        if (p >= 0x101B7B && p < 0x101B7B + 0x100) return T_UNI_C[p - 0x101B7B];
        if (p >= 0x101C7B && p < 0x101C7B + 0x100) return T_UNI_D[p - 0x101C7B];
        if (p >= 0x143D06 && p < 0x143D06 + 0x640) return T_MORPH_A[p - 0x143D06];
        if (p >= 0x1435C6 && p < 0x1435C6 + 0x640) return T_MORPH_B[p - 0x1435C6];
        if (p >= 0x140000 && p < 0x140800) return mw[p - 0x140000];
        if (p >= 0x14A000 && p < 0x14C000) return sw[p - 0x14A000];
        return rdX(p);
    }

    // absolute low-page accessors (emulator Y/X dicts read 0 outside content)
    inline uint32_t rdX(uint32_t a) const {
        return a < 0x1000 ? Xa[a] : 0;
    }
    inline uint32_t rdY(uint32_t a) const {
        return a < 0x1000 ? Ya[a] : 0;
    }
    inline void wrX(uint32_t a, uint32_t v) { if (a < 0x1000) Xa[a] = v & M24; }
    inline void wrY(uint32_t a, uint32_t v) { if (a < 0x1000) Ya[a] = v & M24; }

private:
    uint32_t auxXm1d = 0;                  // x:(r6-$1d) machine cross-state
    uint32_t kUNIL = 0, kUNIW = 0, kUNIX = 0, kSUBX = 0, kSUB1 = 0, kSUB2 = 0;
    uint32_t pitchA1 = 0;
    uint32_t inX = 0;

    // ------------------------------------------------------------------
    // func_0003E0 — wave-frame position stepper (r1 = 0 on entry).
    // B in/out: 56-bit position; x1v:x0v = the 48-bit step.
    // ------------------------------------------------------------------
    void helper3E0(int64_t& B, uint32_t x1v, uint32_t x0v, uint32_t& r1) {
        int64_t b = B;
        uint32_t y1 = 0x10;                                   // move #>$10,y1
        for (;;) {
            // L3e7: cmp #<$10,b ; ble L3e3
            if (sext56(b) > ((int64_t)0x10 << 24)) break;
            // L3e3: move b0,y0 ; mpysu y1,y0,a ; add x,b b1,x:(r1)+ ; move a,l:(r1)+
            uint32_t y0 = rawA0(b);
            int64_t a = mac_su(0, y1, y0);
            int64_t xstep = ((int64_t)x1v << 24) | x0v;
            int64_t bold = b;
            b = sext56(b + xstep);                            // add x,b
            wrX(r1, rawA1(bold)); r1 = (r1 + 1) & M24;        // b1,x:(r1)+
            wrX(r1, rawA1(a)); wrY(r1, rawA0(a));             // a,l:(r1)+
            r1 = (r1 + 1) & M24;
        }
        b = sext56(b - ((int64_t)0x10 << 24));                // sub #<$10,b
        wrX(r1, r1);                                          // move r1,x:(r1)
        B = b;
    }

    // func_0003AA — wave interpolation engine (body in MnmSaw_body.hpp).
    // Firmware register ABI: r3/r7/m7 are inputs, r7 (ring ptr) is also output;
    // r1 entry = the r1 left by the preceding func_0003E0 call (its final
    // 'move r1,x:(r1)' stores the walk end, which 3AA reads as do-count*2).
    void helper3AA(int64_t& B, int& r7, uint32_t m7,
                   uint32_t r3_in, uint32_t r4_in, uint32_t r5_in,
                   uint32_t n0_in, uint32_t r1_in);
};

} // namespace saw
} // namespace mnm

#include "MnmSaw_body.hpp"
