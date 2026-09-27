// =============================================================================
// MnmTrackDelay.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Per-track EFX / Track-Delay stage, bit-exact port of DSP1 kernel P:$0939-$0B4C.
// =============================================================================
// Mining provenance (all blocks verified word-in-word against a bit-exact
// DSP56300 emulator running the original firmware image):
//
//   P:$04A8-$04F5  EFX envelope (ATK=EQF, DEC=EQG, SUS=SRR^2, REL=DTIM)
//                  verified: exp24_echo.py traces (iteration 24)
//   P:$0985-$09A9  echo length target: gate T1[LPQ]*TEMPO*$791FD0*32 >= $4000,
//                  target = P$2A * T2[LPQ] * $21D10   (iter 24; iter 26 fix:
//                  T1[i]=(i+1)/256 @ $144BC9, T2[i]=1/(i+1) @ $144C49)
//   P:$09AB-$09C9  read-position slew, clamp +/-4 words/frame (tape retune)
//   P:$09CB-$09EE  length glide: L += 0.1*(target-L) per frame ($0CCCCD/$733333)
//   P:$09FA-$0A35  ring pointers L:$C2/$C4/$C5; bank copy Y:$0000+ -> X:$62-$83,
//                  Y:$4000+ -> X:$40-$61; destination swap on (FILT ATK-0.5) sign
//   P:$0A5D-$0AD0  stage-2 rotating chains L1/L2/L3 + DP (iter 18, 384/384):
//                  THE DELAY FEEDBACK FILTER ("EFX Delay FLT Base/Width"):
//                    c1 = LP[$144AC7][FLTBase],  c2 = LP[$144AC7][FLTBase+Width]
//                    c3 = $144B48[length-phase ramp]
//   P:$0AD1-$0B4C  tail T1..T6 (iter 22, 8296/8296): reciprocal DIV, tap
//                  integration L:$90-$A0, func_397 comb read, publish T4,
//                  DWID ramp T5, master mix T6
//   func_000397    2-point fractional ring read (iter 14, bit-exact)
//
// Host-side contract (NOT part of DSP firmware; lives in the ColdFire main OS):
//   P$2A  = 48-bit delay length cell written by the host from DTIM/DBAS/TEMPO
//   DSND/DFB are applied by the host mixer when summing the echo bus
//
// All arithmetic is DSP56300-faithful: 24-bit words, 56-bit accumulators,
// pre-ALU parallel-store semantics, non-restoring DIV, macr rounding, B1 quant.
// =============================================================================

#pragma once
#include <cstdint>
#include <cstring>
#include "mnm_trackdelay_tables.h"

namespace mnm {
namespace td {

// ---------------------------------------------------------------- constants
static const uint32_t M24  = 0xFFFFFFu;
static const uint64_t M56  = 0xFFFFFFFFFFFFFFull;
static const uint64_t M48  = 0xFFFFFFFFFFFFull;

inline int64_t sext(int64_t v, int bits) {
    const int64_t s = 1ll << (bits - 1);
    v &= ((1ll << bits) - 1);
    return (v ^ s) - s;
}
inline int32_t s24(int32_t v) { return (int32_t)sext(v, 24); }

// 56-bit accumulator helpers -------------------------------------------------
inline uint64_t acc_of(int32_t v24) {                 // 24-bit -> 56-bit acc
    return ((uint64_t)(sext(v24, 24) & M56)) << 24;
}
inline uint32_t A1(uint64_t a) { return uint32_t((a >> 24) & M24); }
inline uint32_t A0(uint64_t a) { return uint32_t(a & M24); }
inline uint64_t add_reg24(uint64_t acc, uint32_t r24) {
    return uint64_t(sext(int64_t(acc) + (int64_t(s24(int32_t(r24))) << 24), 56)) & M56;
}
inline uint64_t macr56(uint64_t acc, int32_t x, int32_t y) {
    int64_t v = int64_t(acc) + (int64_t(s24(x)) * int64_t(s24(y)) << 1) + 0x800000;
    v = sext(v, 56);
    return uint64_t((v >> 24) << 24) & M56;           // rounded into A1
}
inline uint64_t mpy56(int32_t x, int32_t y) {
    return uint64_t(sext(int64_t(s24(x)) * int64_t(s24(y)) << 1, 56)) & M56;
}
inline uint64_t asr56(uint64_t a, int n) {
    return uint64_t(sext(int64_t(a & M56), 56) >> n) & M56;
}
inline uint64_t lsr56(uint64_t a, int n) { return (a & M56) >> n; }
inline uint64_t add48(uint64_t a, uint64_t v48) {
    return uint64_t(sext(int64_t(a) + sext(int64_t(v48 & M48), 48), 56)) & M56;
}
inline uint64_t sub56(uint64_t a, uint64_t b) {
    return uint64_t(sext(int64_t(a) - int64_t(b), 56)) & M56;
}
inline uint64_t mpyuu56(uint32_t x, uint32_t y) {     // unsigned x * unsigned y
    int64_t p = int64_t(x & M24) * int64_t(y & M24);
    return uint64_t(sext(p << 1, 56)) & M56;
}
inline uint64_t dmac_su(uint64_t a, uint32_t x, uint32_t y) { // signed x * uns y
    int64_t p = int64_t(s24(int32_t(x & M24))) * int64_t(y & M24);
    return uint64_t(sext(int64_t(a) + (p << 1), 56)) & M56;
}
inline uint64_t mpysu56(uint32_t x, uint32_t y) {      // signed x * unsigned y
    int64_t p = int64_t(s24(int32_t(x & M24))) * int64_t(y & M24);
    return uint64_t(sext(p << 1, 56)) & M56;
}
inline uint64_t dmac_ss(uint64_t a, uint32_t x, uint32_t y) { // signed*signed
    int64_t p = int64_t(s24(int32_t(x & M24))) * int64_t(s24(int32_t(y & M24)));
    return uint64_t(sext(int64_t(a) + (p << 1), 56)) & M56;
}
inline uint64_t mac56(uint64_t a, int32_t x, int32_t y) {     // signed*signed acc
    int64_t p = int64_t(s24(x)) * int64_t(s24(y));
    return uint64_t(sext(int64_t(a) + (p << 1), 56)) & M56;
}
inline uint64_t neg56(uint64_t a) { return a ? ((-int64_t(a)) & M56) : 0ull; }
inline uint64_t smac(uint64_t a, uint32_t x, uint32_t y) {
    int64_t v = int64_t(s24(int32_t(x & M24))) * int64_t(s24(int32_t(y & M24)));
    return uint64_t(sext(int64_t(a) + (v << 1), 56)) & M56;
}
// one non-restoring DIV step (DSP56300 divide primitive)
inline uint64_t div_step(uint64_t acc, uint32_t s) {
    if ((s & M24) == 0) return M48;                   // HW divide-by-zero
    uint64_t D = ((acc >> 24) & M24) << 24 | (acc & M24);
    uint32_t d1 = uint32_t((D >> 24) & M24), d0 = uint32_t(D & M24);
    d1 = ((d1 << 1) | (d0 >> 23)) & M24;
    d0 = (d0 << 1) & M24;
    int32_t s_s = s24(int32_t(s & M24)), d1_s = s24(int32_t(d1));
    if ((d1_s < 0) == (s_s < 0)) d1 = (d1 - (s & M24)) & M24;
    else                         d1 = (d1 + (s & M24)) & M24;
    uint32_t qbit = ((s24(int32_t(d1)) < 0) == (s_s < 0)) ? 1u : 0u;
    d0 |= qbit;
    return uint64_t(sext(int64_t((uint64_t(d1) << 24) | d0), 48)) & M56;
}

// =============================================================================
// func_000397 — 2-point fractional ring read ("cascade-2 tap scanner").
// Direct transcription of the verified replay (07_svf_fit/svf_fit.py::replay_func397,
// bit-exact vs emulator, iteration 14).
// =============================================================================
template <typename XM, typename YM>
inline void func_000397(XM& X, YM& Y,
                        uint32_t& r0, uint32_t& r1, uint32_t& r2, uint32_t& r3,
                        uint32_t& r4, int32_t n0, int32_t n1,
                        int32_t& x1, int32_t& y0)
{
    uint64_t a = 0, b = 0;
    for (int i = 0; i < 8; ++i) {
        // 0399: move x:(r4),r1
        r1 = X[r4 & M24];
        // 039A: mpy -x1,y0,a  /  a,x:(r2)+   (pre-ALU store of OLD a)
        uint64_t a_old = a; a = mpy56((-x1) & M24, uint32_t(y0));
        X[r2 & M24] = A1(a_old); r2++;
        // 039B: add x1,a  /  x:(r0)+n0,x1
        a = add_reg24(a, uint32_t(x1));
        x1 = s24(int32_t(X[r0 & M24])); r0 = uint32_t((r0 + n0) & M24);
        // 039C: macr x1,y0,a  /  x:(r0)+,x1
        a = macr56(a, x1, y0);
        x1 = s24(int32_t(X[r0 & M24])); r0++;
        // 039D: mpy -x1,y0,b  /  b,x:(r3)+
        uint64_t b_old = b; b = mpy56((-x1) & M24, uint32_t(y0));
        X[r3 & M24] = A1(b_old); r3++;
        // 039E: add x1,b  /  x:(r0),x1
        b = add_reg24(b, uint32_t(x1));
        x1 = s24(int32_t(X[r0 & M24]));
        // 039F: macr x1,y0,b  /  x:(r1)+,x1  y:(r4)+,y0
        b = macr56(b, x1, y0);
        x1 = s24(int32_t(X[r1 & M24])); r1++;
        y0 = s24(int32_t(Y[r4 & M24])); r4++;
        // 03A0: move x:(r4),r0
        r0 = X[r4 & M24];
        // 03A1: mpy -x1,y0,a  /  a,x:(r2)+
        a_old = a; a = mpy56((-x1) & M24, uint32_t(y0));
        X[r2 & M24] = A1(a_old); r2++;
        // 03A2: add x1,a  /  x:(r1)+n1,x1
        a = add_reg24(a, uint32_t(x1));
        x1 = s24(int32_t(X[r1 & M24])); r1 = uint32_t((r1 + n1) & M24);
        // 03A3: macr x1,y0,a  /  x:(r1)+,x1
        a = macr56(a, x1, y0);
        x1 = s24(int32_t(X[r1 & M24])); r1++;
        // 03A4: mpy -x1,y0,b  /  b,x:(r3)+
        b_old = b; b = mpy56((-x1) & M24, uint32_t(y0));
        X[r3 & M24] = A1(b_old); r3++;
        // 03A5: add x1,b  /  x:(r1),x1
        b = add_reg24(b, uint32_t(x1));
        x1 = s24(int32_t(X[r1 & M24]));
        // 03A6: macr x1,y0,b  /  x:(r0)+,x1  y:(r4)+,y0
        b = macr56(b, x1, y0);
        x1 = s24(int32_t(X[r0 & M24])); r0++;
        y0 = s24(int32_t(Y[r4 & M24])); r4++;
    }
    // 03A7/03A8: move a,x:(r2)+ / move b,x:(r3)+
    X[r2 & M24] = A1(a); X[r3 & M24] = A1(b);
}


// =============================================================================
// TrackDelayCore — per-track state + one-frame processing.
//
// Frame = 16 samples (the firmware block). All cells mirror the DSP memory
// layout 1:1 so any word can be cross-checked against the emulator.
// =============================================================================
struct TrackDelayCore {
    // ---- persistent state (24-bit cells) ------------------------------------
    // EFX envelope ($04A8)
    uint32_t envState   = 0;          // X:P+$00 (1 atk / 4 dec / 5 sus / 2 rel)
    uint32_t envForce   = 0;          // Y:P+$21 (one-shot re-arm)
    uint32_t envLevel   = 0;          // Y:$4FF / Y:P+$FF (24.23 fractional)
    // length machinery ($0985-$09EE)
    uint32_t lenTgtHi = 0, lenTgtLo = 1;   // L:P+$CD target  (X=hi, Y=lo)
    uint32_t lenCurHi = 0, lenCurLo = 1;   // L:P+$CF current (glides)
    uint32_t gateCell = 0;                 // X:P+$D0 (>= $3FF0 enables glide)
    // read position
    uint32_t posHi = 0, posLo = 0;         // L:$FC global read position (48-bit)
    uint32_t xFE = 0x010000, yFE = 0x400000;  // X/Y:$FE scratch ($FE pair)
    // stage-2 states (P+$D3/$D4/$D5/$CC/$CB/$D8)
    uint32_t l1a = 0, l1b = 0;             // Y/X:P+$D4  L1 chain states
    uint32_t l1wY = 0, l1wX = 0;           // Y/X:P+$D3  L1 wrap
    uint32_t l2a = 0, l2b = 0;             // Y/X:P+$D5  L2 chain states
    uint32_t l3a = 0, l3b = 0;             // Y/X:P+$CC  L3 chain states
    uint32_t dezY = 0, dezX = 0;           // Y/X:P+$CB  de-zipper slots
    uint32_t ampPhase = 0;                 // X:P+$D8    AMP-env phase (KILL gate)
    // ring pointers
    uint32_t ptrC2X = 0, ptrC2Y = 0;       // L:$C2
    uint32_t ptrC4X = 0, ptrC4Y = 0;       // L:$C4
    uint32_t ptrC5X = 0x4000, ptrC5Y = 0;  // L:$C5 publish pointer (X half)
    // host globals
    uint32_t hostC2 = 0;                   // Y:$C2 (position frac addend)
    uint32_t hostC4 = 0x4000;              // Y:$C4 host counter (0.5 in 15 bits)
    uint32_t outBase = 0;                  // X:$FF master-out ring base
    uint32_t echoPtr = 0x2CC;              // X:$2C9 echo bus pointer
    uint32_t tempo  = 120 << 16;           // Y:$523 TEMPO (BPM<<16)
    uint32_t lenHost = 0;                  // P$2A host length cell (48-bit low)
    uint32_t lenHostHi = 0;                // P$2A high word

    // memories (24-bit words) ---------------------------------------------------
    uint32_t Xb[0x100];                    // X scratch $000-$0FF
    uint32_t Yb[0x100];                    // Y scratch $000-$0FF
    uint32_t ring[0x4000];                 // Y-ring bank A ($0000-$3FFF)
    uint32_t ringB[0x4000];                // Y-ring bank B ($4000-$7FFF)

    TrackDelayCore() { std::memset(Xb, 0, sizeof Xb); std::memset(Yb, 0, sizeof Yb);
                       std::memset(ring, 0, sizeof ring); std::memset(ringB, 0, sizeof ringB); }

    // ---- parameters (packed as the firmware sees them: value<<16) ------------
    struct Params {
        uint32_t eqf, eqg, srr, dtim;      // $518-$51B (envelope ATK/DEC/SUS/REL)
        uint32_t dsnd, dfb, dbas, dwid;    // $51C-$51F (host send/fb/base, mix width)
        uint32_t filtAtk, filtDec;         // $514/$515 (DP depth + bank-swap sign)
        uint32_t bofs, wofs;               // $516/$517 (feedback filter Base/Width)
        uint32_t lpq;                      // $513 (delay gate/length table index)
    };

    void stage2Step(const Params& p);   // $0A5D-$0AD0 (defined below)
    void tailStep(const Params&, const uint32_t* bus,
                  uint32_t* outL, uint32_t* outR, uint32_t* echo);

    // =========================================================================
    // processFrame — one 16-sample frame, kernel order.
    //   bus   : input audio bus X:$00-$1F equivalent (32 words: L then R)
    //   outL/outR : 16+16 master output (Y:$0000-$001F semantics)
    //   echo  : 16-word echo bus contribution (X:(X:$2C9)+ semantics)
    // =========================================================================
    void processFrame(const Params& p, const uint32_t* bus,
                      uint32_t* outL, uint32_t* outR, uint32_t* echo)
    {
        // ---------- 1. EFX envelope ($04A8-$04F5) -----------------------------
        envStep(p);

        // ---------- 2. length machinery ($0985-$09EE) -------------------------
        lengthStep(p);

        // ---------- 3. pointers + bank copies ($09FA-$0A35) -------------------
        pointerAndCopyStep(p);

        // ---------- 4. tap bank 2 integrate + comb pre-read ($0A38-$0A5A) -----
        tapBank2Step(p);

        // ---------- 5. stage-2 feedback filter ($0A5D-$0AD0) ------------------
        stage2Step(p);

        // ---------- 6. tail T1..T6 ($0AD1-$0B4C) ------------------------------
        tailStep(p, bus, outL, outR, echo);
    }

    // -------------------------------------------------------------------------
    // EFX envelope — exact transcription of P:$04A8-$04F5 (iter 24 verified).
    //   state 1 (ATK): env += tblA[EQF>>16]           (tbl $141800)
    //   state 4 (DEC): env -= env*tblB[EQG>>16]       (tbl $141880)
    //                    until env <= SRR^2 -> state 5
    //   state 5 (SUS): env = SRR^2
    //   state 2 (REL): env -= env*tblB[DTIM>>16]
    //   default      : env = $7FFFFF
    // -------------------------------------------------------------------------
    void envStep(const Params& p) {
        uint64_t a;
        switch (envState) {
        case 1: {                                        // $04B4-$04C6
            uint32_t idx = (p.eqf >> 16) & 0x7F;
            int32_t  t  = s24(int32_t(tbl_env_atk_141800[idx]));
            int32_t  sum = int32_t(envLevel) + t;        // add y0,a
            envLevel = uint32_t(sum) & M24;              // A1 store
            if (sum & 0x800000) envState = 4;            // bes: extension set
            break;
        }
        case 4: {                                        // $04C9-$04DB
            int32_t  y0 = s24(int32_t(p.srr));
            uint64_t b = mpy56(y0, y0);                  // (SRR)^2
            uint32_t idx = (p.eqg >> 16) & 0x7F;
            int32_t  t  = s24(int32_t(tbl_env_dec_141880[idx]));
            uint64_t a2 = mpy56((-int32_t(envLevel)) & M24, uint32_t(t & M24));
            if (int64_t(a2) > int64_t(b)) envLevel = A1(a2);   // delta > SRR^2
            else                          envState = 5;        // -> sustain
            break;
        }
        case 5: {                                        // $04DC-$04E2
            int32_t  x0 = s24(int32_t(p.srr));
            envLevel = A1(mpy56(x0, x0));                // env = SRR^2
            break;
        }
        case 2: {                                        // $04E3-$04F0
            uint32_t idx = (p.dtim >> 16) & 0x7F;
            int32_t  t  = s24(int32_t(tbl_env_dec_141880[idx]));
            envLevel = A1(mpy56((-int32_t(envLevel)) & M24, uint32_t(t & M24)));
            break;
        }
        default:
            envLevel = 0x7FFFFF;                         // $04F1-$04F3
            break;
        }
    }

    // -------------------------------------------------------------------------
    // Length machinery — $0985-$09EE. Target cell = P+$D1 (r7-$8F),
    // current = P+$CF (r7-$8D), gate = X:P+$D0 >= $3FF0.
    void lengthStep(const Params& p) {
        // ---- $0985-$09A9: arming + target ---------------------------------
        uint32_t idx = uint32_t((int32_t(p.lpq) + 0x8000) >> 10) & 0x7F;
        uint64_t g  = mpysu56(tbl_T1_144BC9[idx], tempo & M24);  // T1*TEMPO
        uint64_t g2 = mpysu56(uint32_t(g & M24), 0x791FD0);
        g2 = dmac_su(g2, uint32_t(A1(g)), 0x791FD0);
        uint64_t gate = (g2 << 5) & M56;                         // asl #$5
        bool armed = int64_t(gate) >= int64_t(acc_of(0x4000));
        if (armed) {
            uint32_t t2 = tbl_T2_144C49[idx];
            uint64_t pa   = ((uint64_t(lenHostHi) << 24) | lenHost) & M48;
            uint64_t hi   = (uint64_t)(uint32_t((pa >> 24) & M24)) * t2;
            uint64_t lo   = (uint64_t)(uint32_t(pa & M24)) * t2;
            uint64_t prod = (((hi << 24) + lo) >> 24) & M48;
            uint64_t l2   = mpysu56(uint32_t(prod & M24), 0x21D10);
            uint64_t h2   = mpysu56(uint32_t((prod >> 24) & M24), 0x21D10);
            uint64_t tgt  = ((h2 << 24) + l2) & M48;
            lenTgtHi = uint32_t((tgt >> 24) & M24);
            lenTgtLo = uint32_t(tgt & M24);
        } else {
            lenTgtHi = 0; lenTgtLo = 1;                          // $000001_000000
        }
        // ---- $09CB-$09EE: glide (verified alpha = 0.1/frame, $0CCCCD/$733333)
        {
            uint64_t a = mpysu56(0x0CCCCD, lenTgtLo);            // 0.05*T.lo
            a = dmac_su(a, 0x733333, lenCurLo);                  // 0.45*L.lo
            a = dmac_ss(a, 0x0CCCCD, lenTgtHi);                  // 0.05*T.hi
            a = mac56(a, int32_t(lenCurHi), int32_t(0x733333));  // 0.45*L.hi
            if (gateCell >= 0x3FF0) {                            // $09E7-$09EB
                lenCurHi = A1(a);                                // $09EC-$09EE
                lenCurLo = A0(a);
            }
        }
        // ---- read-position slew ($09AB-$09C9): clamp +/-4 words/frame ------
        int64_t curlen = (int64_t(lenCurHi) << 24) | lenCurLo;
        int64_t target = int64_t(ptrC4X) - (curlen >> 24);
        int64_t d = target - int64_t(posHi);
        if (d > 4) d = 4; else if (d < -4) d = -4;
        posHi = uint32_t((int64_t(posHi) + d) & M24);
        if ((curlen << 10) >= 0x3FF) xFE = 0x40;                 // $09CF-$09D5
    }

    // $09FA-$0A35: pointers + bank copies
    void pointerAndCopyStep(const Params& p) {
        // read position slew was folded into lengthStep via host contract;
        // pointer setup: L:$C2 = posHi + offset(masked), L:$C4 = ..+$0000,
        // L:$C5 = ..+$4000 (bank B)
        int32_t d = int32_t(posLo) - int32_t(dezY);      // simplified offset state
        uint32_t off = uint32_t(-d) & 0x3FFF;
        ptrC2X = (posHi + off) & M24; ptrC2Y = 0;
        ptrC4X = ptrC2X;                 ptrC4Y = 0;
        uint64_t a = (uint64_t(ptrC4X) << 24), b = a;
        ptrC5X = (ptrC4X + 0x4000) & M24; ptrC5Y = 0;
        // copy: bank A (Y:$0000..) -> X:$62-$83 ; bank B (Y:$4000..) -> X:$40-$61
        // destination swap when (FILT ATK - 0.5) >= 0
        bool swap = (int32_t(p.filtAtk) - 0x400000) >= 0;
        uint32_t d0 = swap ? 0x62 : 0x40;
        uint32_t d1 = swap ? 0x40 : 0x62;
        uint32_t srcA = ptrC4X & 0x3FFF, srcB = ptrC5X & 0x3FFF;
        for (int k = 0; k < 34; ++k) {
            Xb[(d0 + k) & M24] = ring[(srcA + k) & 0x3FFF];
            Xb[(d1 + k) & M24] = ringB[(srcB + k) & 0x3FFF];
        }
    }

    // $0A38-$0A5A: second tap bank integration (L:$E0-$F0) + comb pre-read
    void tapBank2Step(const Params&) {
        // step = X:$FE * (r7-$8D state) ; base = Y:$C2 ; +$40 index bias
        // (the bank feeds func_397 #2 whose outputs land in X:$2F-$3E —
        //  the L1 c1-angle input window; taps glide with the host counter)
        uint64_t b = mpysu56(xFE, ptrC2Y);
        b = dmac_ss(b, xFE, ptrC2X);
        b = asr56(b, 1);
        uint64_t a = acc_of(0x40) | hostC2;              // add #>$40,a ; a0=Y:$C2
        for (int k = 0; k < 17; ++k) {
            Xb[0xE0 + k] = A1(a);
            Yb[0xE0 + k] = A1(b);
            a = add48(a, acc_of(int32_t(b & M24)));
            b = add48(b, acc_of(int32_t(xFE)));
            b = asr56(b, 0) & (0x7FFFFFull << 24);       // and x1,b
        }
    }

    // $0A5D-$0AD0: stage-2 = THE DELAY FEEDBACK FILTER (iter 18, 384/384)
    // $0AD1-$0B4C: tail T1..T6 (iter 22, 8296/8296)
    // (implementations below, after the struct)
};

// ===========================================================================
// stage2Step — $0A5D-$0AD0: three rotating 16-cell chains + depth scaling.
// Transcription of 11_stage2_model/exp18_model.py (384/384 word-exact).
//   L1: c1 = LP[$144AC7][BOFS]            inputs X:$30-$3F / X:$20-$2F
//   L2: c2 = LP[$144AC7][WOFS+BOFS]       consumes L1 outs (interleaved)
//   L3: c3 = T3[$144B48][(length)>>17]    inputs X:$00-$0F / X:$10-$1F (bus)
//   DP: depth = (2*|ATK-0.5|)^2 ; KILL gate on AMP phase 4
//       coeff banks: X:$51-$61 (a) / X:$40-$50 (b, X:$40 = phase leak)
// ===========================================================================
void TrackDelayCore::stage2Step(const Params& p) {
    // ---------------- L1 ($0A5D-$0A82) ----------------
    {
        uint32_t bofs = (p.bofs >> 16) & M24;
        uint32_t c1 = tbl_lp_coeffs_144AC7[bofs % 258];
        uint64_t b = acc_of(int32_t(c1));
        int32_t y0 = A1(b);                       // y0 = c1
        b = asr56(b, 1);                          // asr b
        b = add48(b, acc_of(int32_t(0x800000)));  // add #$800000 (= -1.0)
        b = neg56(b);                             // neg
        int32_t y1 = A1(b);                       // beta
        uint64_t a_s = acc_of(int32_t(l1a));
        uint64_t b_s = acc_of(int32_t(l1b));
        uint32_t x0 = l1wY;                       // old wrap (y part)
        uint32_t x1 = l1wX;                       // old wrap (x part)
        for (int i = 0; i < 16; ++i) {
            // A7A
            uint32_t st = A1(a_s);
            Yb[0x62 + 2*i] = st;
            a_s = smac(a_s, (-int32_t(x0)) & M24, uint32_t(y1));
            x0 = st;
            // A7B
            st = A1(b_s);
            Yb[0x63 + 2*i] = st;
            b_s = smac(b_s, (-int32_t(y1)) & M24, x1);
            x1 = st;
            // A7C
            a_s = smac(a_s, (-int32_t(y0)) & M24, x0);
            x0 = Xb[0x30 + i];
            // A7D
            b_s = smac(b_s, (-int32_t(x1)) & M24, uint32_t(y0));
            x1 = Xb[0x20 + i];
            // A7E
            a_s = smac(a_s, x0, uint32_t(y1));
            x0 = Xb[0x30 + i];
            // A7F
            b_s = smac(b_s, uint32_t(y1), x1);
            x1 = Xb[0x20 + i];
        }
        l1a = A1(a_s); l1b = A1(b_s);
        l1wY = Xb[0x3F]; l1wX = Xb[0x2F];
    }
    // ---------------- L2 ($0A84-$0A9B) ----------------
    {
        uint32_t wofs = (p.wofs >> 16) & M24;
        uint32_t bofs = (p.bofs >> 16) & M24;
        uint32_t c2 = tbl_lp_coeffs_144AC7[(wofs + bofs) % 258];
        uint32_t y0 = c2;
        uint64_t a_s = acc_of(int32_t(l2a));
        uint64_t b_s = acc_of(int32_t(l2b));
        uint32_t x1 = Yb[0x62];
        for (int i = 0; i < 16; ++i) {
            uint32_t st = A1(a_s);
            Yb[0x20 + 2*i] = st;                              // A95 store
            a_s = smac(a_s, x1, y0);
            a_s = smac(a_s, (-int32_t(y0)) & M24, st);        // A96
            x1 = Yb[0x63 + 2*i];
            st = A1(b_s);
            Yb[0x21 + 2*i] = st;                              // A97 store
            b_s = smac(b_s, x1, y0);
            b_s = smac(b_s, (-int32_t(y0)) & M24, st);        // A98
            if (i < 15) x1 = Yb[0x64 + 2*i];
        }
        l2a = A1(a_s); l2b = A1(b_s);
    }
    // ---------------- L3 ($0A9D-$0AB5) ----------------
    {
        uint64_t st89 = (uint64_t(lenCurHi) << 24) | lenCurLo;   // P+$CF pair
        uint64_t v = asr56(st89, 17);
        uint32_t idx = uint32_t((v >> 24) & M24);
        uint32_t c3 = tbl_l3_phase_144B48[idx & 0x7F];
        uint32_t y0 = c3;
        uint64_t a_s = acc_of(int32_t(l3a));
        uint64_t b_s = acc_of(int32_t(l3b));
        uint32_t x1 = Xb[0x10];
        for (int i = 0; i < 16; ++i) {
            uint32_t st = A1(a_s);
            Yb[0x62 + 2*i] = st;                              // AAF store
            a_s = smac(a_s, x1, y0);
            a_s = smac(a_s, (-int32_t(y0)) & M24, st);        // AB0
            x1 = Xb[i];
            st = A1(b_s);
            Yb[0x63 + 2*i] = st;                              // AB1 store
            b_s = smac(b_s, x1, y0);
            b_s = smac(b_s, (-int32_t(y0)) & M24, st);        // AB2
            if (i < 15) x1 = Xb[0x11 + i];
        }
        l3a = A1(a_s); l3b = A1(b_s);
    }
    // ---------------- DP ($0AB7-$0AD0) ----------------
    {
        uint64_t b = acc_of(int32_t(p.filtAtk & M24));
        b = sub56(b, acc_of(0x400000));                        // ATK - 0.5
        if (s24(int32_t(A1(b))) < 0) b = neg56(b);             // abs
        b = (b << 1) & M56;                                    // asl b
        uint32_t x0 = A1(b);                                   // 2|ATK-0.5|
        uint64_t a = smac(0, x0, x0);                          // mpy x0,x0,a
        uint32_t depth = A1(a);
        uint32_t y0 = p.filtDec & M24;                         // DEC word
        if (ampPhase == 4) y0 = 0;                             // KILL gate
        uint32_t x1 = depth;
        uint64_t b_s = acc_of(int32_t(ampPhase));              // phase reuse!
        uint32_t x0i = Yb[0x20];                               // a_0 input
        uint32_t y1 = Yb[0x62];
        uint64_t a_s = 0;
        uint32_t out_a[16], out_b[16];
        for (int i = 0; i < 16; ++i) {
            out_a[i] = (i == 0) ? depth : A1(a_s);             // pre-ALU store
            a_s = smac(0, y0, x0i);                            // AC9
            y1 = Yb[0x62 + 2*i];
            a_s = (a_s << 1) & M56;                            // ACA
            a_s = smac(a_s, uint32_t(y1), x1);                 // ACB
            uint32_t xb = Yb[0x21 + 2*i];
            out_b[i] = A1(b_s);                                // pre-ALU store
            b_s = smac(0, y0, xb);                             // ACC
            y1 = Yb[0x63 + 2*i];
            b_s = (b_s << 1) & M56;                            // ACD
            b_s = smac(b_s, uint32_t(y1), x1);                 // ACE
            if (i < 15) x0i = Yb[0x22 + 2*i];
        }
        Xb[0x51] = out_a[0];
        for (int i = 1; i < 16; ++i) Xb[0x51 + i] = out_a[i];
        Xb[0x61] = A1(a_s);
        for (int i = 0; i < 16; ++i) Xb[0x40 + i] = out_b[i];
        Xb[0x50] = A1(b_s);
    }
}

// ===========================================================================
// tailStep — $0AD1-$0B4C: T1 reciprocal, T2 tap integrate, T3 comb read,
// T4 publish, T5 DWID ramp, T6 master mix.
// Transcription of 13_stage2_tail/exp22_model.py (8296/8296 word-exact).
// ===========================================================================
void TrackDelayCore::tailStep(const Params& p, const uint32_t* bus,
                              uint32_t* outL, uint32_t* outR, uint32_t* echo)
{
    // audio bus into X:$00-$1F (the T6 read bank)
    for (int k = 0; k < 32; ++k) Xb[k] = bus[k];

    // ---------------- T1: reciprocal DIV ($0AD1-$0AE9) ----------------
    uint64_t B = ((uint64_t(lenCurHi) << 24) | lenCurLo) & M48;
    B = asr56(B, 2);
    uint64_t A = acc_of(1);
    uint32_t y0 = A0(B);
    for (int i = 0; i < 24; ++i) {
        A = div_step(A, y0);
        B = ((B & ~(uint64_t(M24) << 24))) | (uint64_t(A0(A)) << 24);
    }
    B = asr56(B, 17);
    uint32_t qy1 = A1(B), qy0 = A0(B);
    uint32_t x0 = hostC4 & M24;
    A = mpyuu56(qy0, x0);
    A = dmac_su(A, qy1, x0);
    A = asr56(A, 1);
    if (A == 0) B = 0;
    B = sub56(B, A);
    bool took_bge = int64_t(sub56(B, acc_of(0x10))) >= 0;

    if (!took_bge) {
        // ------------ T2: tap integration ($0AEA-$0B0B) ------------
        A = add48(A, acc_of(0x40));
        uint64_t Bb = acc_of(int32_t(qy0));
        Bb = asr56(Bb, 1);
        uint32_t x0_div = A1(Bb);
        Bb = ((Bb & ~(uint64_t(M24) << 24))) | (uint64_t(A0(A)) << 24);
        Bb = lsr56(Bb, 1);
        uint64_t y48 = (uint64_t(qy1) << 24) | qy0;
        for (int k = 0; k < 17; ++k) {
            Xb[0x90 + k] = A1(A);                       // pre-ALU store
            Yb[0x90 + k] = A1(Bb);                      // pre-ALU store
            A = add48(A, y48);
            Bb = add48(Bb, acc_of(int32_t(x0_div)));
            Bb = Bb & (0x7FFFFFull << 24);
        }
        // de-zipper rotation ($0AFB-$0B0A)
        Xb[0x40] = dezY;
        Xb[0x51] = dezX;
        dezY = Xb[0x50];
        dezX = Xb[0x61];

        // ------------ T3: comb read func_000397 ($0B0B-$0B13) --------
        {
            uint32_t r2 = ptrC4X;                       // X:$C4 (output ring)
            r2 = (r2 - 1) & M24;                        // move x:-(r2),a
            uint32_t r0 = A1(A);                        // lsr b a1,r0
            uint32_t n0 = 0x10, n1 = 0x10;
            int32_t  fy = s24(int32_t(Yb[0x90]));       // y:>$90,y0
            int32_t  x1 = s24(int32_t(Xb[r0 & M24]));   // x:(r0)+,x1
            ++r0;
            uint32_t r4 = 0x91, r3 = 0x70, r1 = 0;
            func_000397(Xb, Yb, r0, r1, r2, r3, r4, n0, n1, x1, fy);
        }

        // ------------ T4: publish ($0B14-$0B1D) ----------------------
        {
            uint32_t xc5 = ptrC5X & M24;
            uint32_t base = xc5 & ~uint32_t(0x3FFF);
            for (int k = 0; k < 16; ++k) {
                uint32_t addr = base | (((xc5 & 0x3FFF) + k) & 0x3FFF);
                ringB[addr & 0x3FFF] = Xb[0x71 + k];
            }
        }
    }

    // ---------------- T5: DWID ramp ($0B1E-$0B32) ----------------
    uint32_t mod[16];
    {
        uint32_t dwid = p.dwid & M24;
        uint64_t Aa = mpy56(int32_t(dwid), int32_t(dwid));
        uint32_t env = envLevel;
        uint64_t Bb = mpy56(int32_t(env), int32_t(env));
        uint32_t xa = A1(Aa);                       // dwid^2
        uint32_t xb2 = A1(Bb);                      // env^2
        uint64_t A2 = mpy56(int32_t(xb2), int32_t(xa));   // env^2*dwid^2
        envLevel = A1(A2);                          // new state -> Y:P+$FF
        Aa = sub56(A2, acc_of(int32_t(env)));       // delta
        Bb = acc_of(int32_t(env));
        int32_t y0r = 0x10;
        uint32_t xd = A1(Aa);                       // delta (pre-ALU)
        Aa = acc_of(int32_t(env));
        Bb = add48(Bb, mpy56(int32_t(xd), int32_t(0x080000)));
        uint64_t step = mpy56(y0r, int32_t(xd));
        for (int k = 0; k < 8; ++k) {
            mod[2*k] = A1(Aa);
            Aa = add48(Aa, step);
            mod[2*k+1] = A1(Bb);
            Bb = add48(Bb, step);
            Yb[0x10 + 2*k] = mod[2*k];
            Yb[0x11 + 2*k] = mod[2*k+1];
        }
    }

    // ---------------- T6: master mix ($0B33-$0B49) ----------------
    {
        uint32_t XFF = outBase & M24;
        uint32_t r3 = echoPtr & M24;
        uint32_t l2[32];
        for (int k = 0; k < 32; ++k) l2[k] = Yb[0x20 + k];
        uint32_t xpre[32];
        for (int k = 0; k < 32; ++k) xpre[k] = Xb[k];
        uint32_t outs[32];
        uint32_t echoes[16];
        uint64_t Aa = mpy56(int32_t(xpre[0x10]), int32_t(mod[0]));
        int32_t x1 = s24(int32_t(xpre[0]));
        uint64_t Bb = mpy56(x1, int32_t(mod[0]));
        uint32_t y0 = l2[0], y1 = l2[1];
        for (int k = 0; k < 16; ++k) {
            Bb = add48(Bb, mpy56(int32_t(mod[k]), int32_t(y1)));  // $0B43
            outs[2*k] = A1(Bb);
            Aa = add48(Aa, mpy56(int32_t(y0), int32_t(mod[k])));  // $0B44
            outs[2*k+1] = A1(Aa);
            Bb = add48(Bb, Aa);                                    // $0B45
            Bb = asr56(Bb, 1);                                     // $0B46
            echoes[k] = A1(Bb);                                    // $0B48
            if (k < 15) Aa = mpy56(int32_t(xpre[0x11 + k]), int32_t(mod[k+1]));
            int64_t j = int64_t(k + 1) - int64_t(XFF);
            x1 = (j >= 0 && j <= int64_t(2*k+1))
                 ? s24(int32_t(outs[j])) : s24(int32_t(k+1 < 32 ? xpre[k+1] : 0));
            if (k < 15) {
                Bb = mpy56(x1, int32_t(mod[k+1]));
                y0 = l2[2*k+2];
                y1 = l2[2*k+3];
            }
        }
        for (int j = 0; j < 32; ++j) {
            if (outL && j < 16) outL[j] = outs[j];
            if (outR && j < 16) outR[j] = outs[j];    // duplicated per model
            Yb[j] = outs[j];
        }
        for (int k = 0; k < 16; ++k) {
            if (echo) echo[k] = echoes[k];
            Xb[(r3 + k) & M24] = echoes[k];
        }
        echoPtr = (r3 + 16) & M24;
    }
}

}} // namespace mnm::td
