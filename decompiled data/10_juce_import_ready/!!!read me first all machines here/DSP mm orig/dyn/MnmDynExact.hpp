// FX-DYNAMIX (machine 16) — "mnm" core, recovered from OS 1.32B.
// Monomachine Nova 1.7.12-mining. Every claim lists the firmware address so it
// can be re-checked against decompiled data/03_listings/machines_page_A/16_FX-DYN_full.txt
// (slice P:$1477DE-$14789D, init $1477DE / config $1477E5 / proc $1477E6).
//
// CONFIRMED from the listing + emulator runs (scripts/exp_m16_dyn.py):
//  * I/O frame:      16 samples/call; output 32 words interleaved 16 L + 16 R
//                    (same convention as m17/m18/m19).
//  * INP (p7):       gain 4*(INP/127)^2 ($1477EA-$1477F1; same idiom as
//                    FLA/PHA). INP = param 7 (r6+$b).
//  * peak follower: per-sample running max of squares -> X:$61..$70
//                    ($1477FA-$147802: mpy y0,y0 + maxm a,b).
//  * ENV (p6):       envelope one-pole on the 48-bit state $15; coefficient
//                    table X:$1446C6 indexed p6*128 (+r0 walk)
//                    ($147803-$147816). p6 = follower speed (k=0 fastest).
//  * log domain:     clb/normf normalize the env; mantissa*512 indexes the
//                    512-word log curve X:$1444C6; the exponent indexes the
//                    same-region table for the integer log part
//                    ($14781D-$14782B). (Real ROM tables, extracted.)
//  * THRESH (p2):    thr = p2*$D00000*2 + $320000*2 in log units
//                    ($14782C-$147831). Below threshold: linear passthrough
//                    (emulator: out = 0.52*amp for amp<=0.7 at defaults).
//  * RATIO (p4):     slope = ($40 + p4*$C04000*2)<<1 — the log-gain slope
//                    ($147833-$14783A); emulator: higher p4 -> stronger
//                    reduction at amp=0.9 (gain $0177B3 -> $000433 for
//                    p4 0->127).
//  * gain exponent:  gain_log -> clamp, asr #$7, rnd; integer part -> dynamic
//                    asr of the exp table X:$1407FF (the 2048-word 2^x ROM)
//                    ($14783B-$147849).
//  * ATK (p0)/REL(p1): coefficient tables X:$1446C6 (attack, 1581119->9125)
//                    and X:$144746 (release, 173380->3014), indexed *128; the
//                    one-pole on the gain state $11 selects attack vs release
//                    by cmp of target vs current ($147862-$147871).
//  * MIX (p3):       out = wet*(127-MIX)/127 + dry*MIX/127 ($147883-$147889,
//                    $147890-$147892). MIX = param 3.
//  * OUT (p5):       dry-path gain = OUT^2 * $7F8000*2 + $8000*2
//                    ($147887-$14788F) — quadratic output knob.
//
// NOT instruction-exact (documented, not guessed):
//  * the exact 48-bit pairing of the two state cells (x:$11/y:$11, x:$15/y:$15)
//    is reconstructed from the move order; the emulator cross-check reproduces
//    the laws above but the per-word state layout is [РЕКОНСТР.].
//  * the sign wrap of the log curve for over-threshold hot input (the
//    mantissa table crosses zero at index 256) is firmware behavior; the core
//    preserves it by using the real extracted table.
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "MnmDynExactTables.hpp"

namespace monomachine {
namespace mnm {

inline constexpr int kDynFrame = 16;
inline constexpr double kDynDspRate = 44100.0;

class DynCore {
public:
    void reset(double hostRate) {
        sr = hostRate > 0.0 ? hostRate : kDynDspRate;
        env = 0.0;            // $15 48-bit envelope state
        gain = 1.0;           // $11 48-bit ballistics state
        prevWet = 0.0;
        for (int i = 0; i < kDynFrame; ++i) { wetL[i] = 0.0f; wetR[i] = 0.0f; }
    }

    // p[0..7]: ATK REL THRESH MIX RATIO OUT ENV INP (knobs 0..127)
    void setParameters(const int* p) noexcept {
        kAtk = p[0]; kRel = p[1]; kThr = p[2]; kMix = p[3];
        kRat = p[4]; kOut = p[5]; kEnv = p[6]; kInp = p[7];
        // THRESH in log units ($14782C-$147831): p2*$D00000*2 + $320000*2
        thrLog = (double)kThr * (2.0 * 0xD00000 / 8388608.0)
                 + 2.0 * 0x320000 / 8388608.0;
        // RATIO slope ($147833-$14783A): a = $40 + p4*$C04000 (maci x2, signed:
        // $C04000 = -$3FC000 = -0.49829), then asl a (x2) -> NEGATIVE slope:
        // gain drops as (envlog - thr) rises — the compression law.
        slope = ((64.0 / 8388608.0)
                 + (double)kRat * (2.0 * (-(double)(int32_t)0x3FC000) / 8388608.0)) * 2.0;
        // ATK/REL one-pole coefficients from the ROM tables ($14785A-$147861)
        cAtk = mnmfla2::kDynCoefTab[std::min(127, kAtk)] / 8388608.0;
        cRel = mnmfla2::kDynRelTab[std::min(127, kRel)] / 8388608.0;
        cEnv = mnmfla2::kDynCoefTab[std::min(127, kEnv)] / 8388608.0;
        // MIX / OUT ($147883-$14788F)
        mixWet = (127.0 - kMix) / 127.0;
        mixDry = (double)kMix / 127.0;
        outGain = ((double)kOut / 127.0) * ((double)kOut / 127.0)
                  * (2.0 * 0x7F8000 / 8388608.0) + 2.0 * 0x8000 / 8388608.0;
    }

    void process(const float* inL, const float* inR, float* outL, float* outR) noexcept {
        const double gin = 4.0 * ((double)kInp / 127.0) * ((double)kInp / 127.0);
        for (int i = 0; i < kDynFrame; ++i) {
            const double dl = inL[i] * gin;
            const double dr = inR[i] * gin;
            // peak follower ($1477FA-$147802): running max of squares
            const double p2 = std::max(dl * dl, dr * dr);
            env = std::max(p2, env * (1.0 - cEnv) );
            // log-domain gain ($14781D-$147849)
            const double a = std::max(env, 1e-12);
            int ex = 0;
            double mant = a;
            while (mant >= 1.0) { mant *= 0.5; ++ex; }
            while (mant < 0.5)  { mant *= 2.0; --ex; }
            int li = (int)(mant * 512.0);
            li = std::clamp(li, 0, 511);
            const double logEnv = (double)ex + mnmfla2::kDynLogTab[li] / 8388608.0;
            double gLog = slope * (logEnv - thrLog);
            // clamp ($14783D) + gain = 2^gLog via the exp table ($147840-$147848)
            double gl = std::clamp(gLog, -15.0, 15.0);
            const double gi = std::floor(gl);
            const double gf = gl - gi;
            const int eIdx = std::clamp((int)(gf * 2048.0), 0, 2047);
            double lin = mnmfla2::kDynExpTab[eIdx] / 8388608.0 * std::pow(2.0, gi);
            lin = std::clamp(lin, 0.0, 4.0);
            // ballistics ($147862-$147871): attack vs release by comparison
            const double c = (lin > gain) ? cAtk : cRel;
            gain += (lin - gain) * c;
            // wet/dry ($147872-$14789C)
            wetL[i] = (float)dl;
            wetR[i] = (float)dr;
            outL[i] = (float)((dl * gain) * mixWet + dl * outGain * mixDry);
            outR[i] = (float)((dr * gain) * mixWet + dr * outGain * mixDry);
        }
        prevWet = wetL[kDynFrame - 1];
    }

    double sampleRate() const noexcept { return sr; }

private:
    double sr = kDynDspRate;
    double env = 0.0;
    double gain = 1.0;
    double thrLog = 0.0, slope = 0.0;
    double cAtk = 0.0, cRel = 0.0, cEnv = 0.0;
    double mixWet = 1.0, mixDry = 0.0, outGain = 0.25;
    float wetL[kDynFrame], wetR[kDynFrame];
    double prevWet = 0.0;
    int kAtk = 0, kRel = 0, kThr = 0, kMix = 0, kRat = 0, kOut = 0, kEnv = 0, kInp = 0;
};

} // namespace mnm
} // namespace monomachine
