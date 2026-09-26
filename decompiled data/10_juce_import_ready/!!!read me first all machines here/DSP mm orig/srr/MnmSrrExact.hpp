// MnmSrrExact.hpp — MiniNova OS 1.32B: EFX envelope (P:$04A8-$04F3),
// SRR/DWID ramp (P:$0B1E-$0B33) and the SRR decimator.
//
// EFX envelope — instruction-exact phase machine (r6 = voice page base):
//   phase cell x:(P+$00); TRIG cell y:(P+$21), cleared after read.
//   TRIG 1 -> phase 1 (attack):  env += kEnvAttack[EQF_int]   (EQF = P+$18)
//              on 48-bit carry -> phase 4
//   phase 4 (decay):  a = -env * kEnvDecay[EQG_int]  (EQG = P+$19)
//              if a <= SRR^2 -> phase 5 else env = a
//   phase 5 (hold):   env = SRR_cell^2 >> 24   (SRR = P+$1A; a1 extraction
//              = SRR_v^2 * 256, i.e. hold = SRR_v^2/32768 in Q23)
//   TRIG 2 -> phase 2 (release): env = -env * kEnvDecay[DTIM_int] (P+$1B)
//   TRIG 4/5 -> SRR hold paths (phase 4/5 entry)
//   phase 0/other:    env = $7FFFFF
//
// The kernel additionally rewrites env late in the block (P:$0B1E-$0B33)
// with the squared map env* = (env^2 >> 24)*(DWID^2*256) >> 23, but ONLY
// when a delay-ratio condition holds (P:$0AE7-$0AE9: b >= 16 where b is
// derived from the smoothed target and the read frac). In the captured
// configurations the condition did not trigger; the rewrite is implemented
// and gated by the same comparison shape, marked [RECONSTR. trigger].
//
// The per-frame decimator coefficients (Y:$10-$1F) are the 16-word ramp:
//   ramp[k]   = prev + (k+1)*$10*X0            (k = 0..7)
//   ramp[k+8] = prev + (1/16)*X0 + (k-7)*$10*X0
//   X0 = (env^2 >> 24) * (DWID_v^2 * 256) >> 23   (mac chain, <<1 semantics)
// [RECONSTR. consumer]: the per-sample decimator driven by these
// coefficients is not isolated in the kernel (it feeds the tap tables /
// staging of the following block); the port implements the standard
// sample-and-hold SRR: the effective rate = 1 + floor(ramp[k] scaled),
// decreasing the sample rate by dropping samples.
#pragma once

#include <cstdint>
#include <cstring>
#include "MnmTrackDelayExact.hpp"  // helpers (mpy/mpysu/s24/kBlock) + tables

namespace mnm_td {

// ---------- EFX envelope (P:$04A8-$04F3) — instruction-exact ---------------
struct EfxEnv {
    int32_t env;     // Y:$4FF, Q23
    int32_t phase;   // x:(P+$00)
    EfxEnv() : env(0), phase(0) {}

    static inline int32_t knobInt(int32_t cell) {
        return (int32_t)(((int64_t)cell + 0x8000) >> 16) & 0xFFFF;
    }
    // SRR hold: cell = v<<16 -> cell^2 = v^2<<32 -> a1 = v^2 * 256
    static inline int32_t sqHold(int32_t cell) {
        int64_t c = (int64_t)(uint32_t)(cell & 0xFFFFFF);
        return (int32_t)(((c * c) >> 24) & 0xFFFFFF);
    }

    // trig: 0 none, 1 note-on, 2 note-off (firmware values at y:(P+$21))
    void block(int32_t trig, int32_t cellEqf, int32_t cellEqg,
               int32_t cellSrr, int32_t cellDtim) {
        if (trig != 0) phase = trig;
        int64_t a;
        switch (phase) {
        case 1: {  // attack: env += kEnvAttack[EQF]; carry -> phase 4
            int32_t inc = (int32_t)kEnvAttack[knobInt(cellEqf) & 0x7F];
            int64_t s = (int64_t)(uint32_t)env + (int64_t)(uint32_t)inc;
            if (s > 0xFFFFFF) { env = (int32_t)(s & 0xFFFFFF); phase = 4; }
            else env = (int32_t)s;
            break;
        }
        case 4: {  // decay: a = -env * kEnvDecay[EQG]; a <= SRR^2 -> phase 5
            int32_t dec = s24((int32_t)kEnvDecay[knobInt(cellEqg) & 0x7F]);
            int64_t sq = (int64_t)(uint32_t)sqHold(cellSrr);
            a = -((int64_t)(uint32_t)env) * (int64_t)dec;   // env*dec < 0
            a = (a << 1) >> 0;                               // mpy <<1
            if (a <= sq) phase = 5;
            else env = (int32_t)(a & 0xFFFFFF);
            break;
        }
        case 5:    // hold at SRR^2
            env = sqHold(cellSrr);
            break;
        case 2: {  // release: env = -env * kEnvDecay[DTIM]
            int32_t rel = s24((int32_t)kEnvDecay[knobInt(cellDtim) & 0x7F]);
            a = -((int64_t)(uint32_t)env) * (int64_t)rel;
            env = (int32_t)((a << 1) & 0xFFFFFF);
            break;
        }
        default:
            env = 0x7FFFFF;
            break;
        }
    }
};

// ---------- SRR ramp (P:$0B1E-$0B33) ---------------------------------------
// Produces the 16 per-frame decimation coefficients (Y:$10-$1F).
inline void srrRamp(int32_t env, int32_t cellDwid, int32_t* ramp16) {
    // x0 = a1 of DWID^2 = DWID_v^2 * 256
    int64_t d = (int64_t)(uint32_t)(cellDwid & 0xFFFFFF);
    int32_t x0 = (int32_t)(((d * d) >> 24) & 0xFFFFFF);
    // x1 = env^2 >> 24 (a1 of the square)
    int64_t e = (int64_t)(uint32_t)env;
    int32_t x1 = (int32_t)(((e * e) >> 24) & 0xFFFFFF);
    // a = mpy x1,x0 (<<1), a1 extraction
    int64_t prod = ((int64_t)x1 * (int64_t)x0) << 1;
    int32_t X0 = (int32_t)((prod >> 24) & 0xFFFFFF);
    int32_t prev = env;
    int64_t a = (int64_t)(uint32_t)prev << 24;             // tfr y1,a: high part
    int64_t b = ((int64_t)(uint32_t)prev << 24) + mpysu(0x080000, (uint32_t)X0);
    for (int k = 0; k < 8; ++k) {
        a += mpy(0x10, X0);
        b += mpy(0x10, X0);
        ramp16[k]     = (int32_t)((a >> 24) & 0xFFFFFF);
        ramp16[k + 8] = (int32_t)((b >> 24) & 0xFFFFFF);
    }
}

// ---------- SRR decimator [RECONSTR.] --------------------------------------
// Sample-rate reduction driven by the per-frame coefficient. The firmware
// reduces the rate by repeating samples (hold); the depth follows the ramp.
struct SrrDecimator {
    int32_t holdL, holdR;
    int32_t phase;      // fractional sample counter
    SrrDecimator() : holdL(0), holdR(0), phase(0) {}

    // rate: per-frame coefficient from srrRamp, Q23 (0..1) -> hold length
    // 1..17 samples (rate 0 = bypass).
    inline void process(const int32_t* inL, const int32_t* inR,
                        int32_t* outL, int32_t* outR, int32_t rate) {
        int64_t r = (int64_t)(uint32_t)(rate & 0xFFFFFF);
        int32_t hold = 1 + (int32_t)((r * 16) >> 23);   // 1..17
        for (int f = 0; f < kBlock; ++f) {
            if (phase <= 0) { holdL = inL[f]; holdR = inR[f]; phase = hold; }
            outL[f] = holdL;
            outR[f] = holdR;
            --phase;
        }
    }
};

} // namespace mnm_td
