// FX-FLANGER (machine 19) — "mnm" core, recovered from OS 1.32B.
// Monomachine Nova 1.7.12-mining. Every claim lists the firmware address so it
// can be re-checked against decompiled data/03_listings/machines_page_A/19_FX-FLA_full.txt
// (slice P:$144E81-$145035, init $144E81 / config $144E99 / proc $144E9A).
//
// CONFIRMED from the listing + emulator runs (scripts/exp_m19_fla.py):
//  * I/O frame:    16 samples/call; output 32 words interleaved 16 L + 16 R
//                  (same convention as FX-RING m17 / FX-PHA m18).
//  * INP law:      gain = 4*(INP/127)^2 ($144EA6-$144EAB: mpy x0,x0; asl #$2).
//                  INP = param 7 (r6+$B). INP=0 -> silence (emulator: energy 0).
//  * LFO:          one-pole "fluid integrator" per frame:
//                  phase += SPD*$28F5C*2 − phase*$028F5C*2 ($144EC8-$144ECF)
//                  — the SAME law as the firmware chorus core (P:$1476AD).
//                  SPD = param 5 (emulator: phase increment monotonic in SPD).
//  * quadrature:   index = phase*2048 ($144ED1-$144ED3: mpyi #$800); ROM table
//                  X:$14A000 (half-sine, peak 0x7FFFFF) + quad tap X:$14A800 =
//                  +2048 words ($144ED4-$144ED7). Frame ramp interpolation of
//                  the quadrature from the previous frame ($144ED8-$144EE6).
//  * DEL law:      target = floor(1021*DEL/128) − 3 words ($144E8D-$144E91:
//                  mpyi #$FFFC03 (= −1021 signed), sub #$3); smoothed one-pole
//                  $28F5C/$FD70A4, 48-bit state $1d ($144EFE-$144F09).
//                  Delay ≈ (3 + 7.977*DEL) words @44.1k = 0.07..18.6 ms.
//                  DEL = param 0.
//  * DEP law:      smoothed state $22 (same one-pole, $144F0A-$144F13); the
//                  tap ramp = (delState+2)<<10 * depState per frame
//                  ($144F14-$144F1B) — LFO depth onto the delay. DEP = param 1.
//  * delay line:   TWO 4096-word circular buffers (modulo $FFF, $144FA4-$144FB5),
//                  L = X-memory, R = Y-memory, 48-bit L: access ($144F31+).
//  * tap readout:  linear interpolation — integer part asr #$4 of the 48-bit
//                  position, fraction mpy/asr #$a ($144F2C-$144F3D).
//  * FB law:       gain = (FB−64)/127 * 2.01168, SIGNED ($144FBE-$144FC8:
//                  sub #$400000, *$410410 (=0.508), asl #$2, *$7EB852 (=0.99));
//                  comb write-back buf[tap] += fb*read ($144FD3-$144FE2).
//                  FB = param 4. FB<64 -> inverted feedback.
//  * stereo:       R channel LFO inverted and scaled by $5A8241 (0.7071)
//                  ($145012-$145015: mpy -x0,y1) -> decorrelated flanging.
//  * MIX (param 3):per-sample crossfade wet/dry:
//                  out = wet*(127−MIX)/127 + dry*MIX/127 ($145027-$145034).
//                  Emulator: MIX=0 -> full wet+dry sum shape, MIX=127 -> dry.
//  * fade-in:      first 128 frames ramp the wet path ($144E9C-$144EA4,
//                  frame counter y:$21) — same shape as the chorus core.
//  * WID (param 2):WID^2 * $9566 ($144EB4-$144EB9) scales the interpolation
//                  window pair integrator (x:$11/x:$12, 48-bit, bset #$14
//                  scaling mode, $144EBD-$144EC8) — window width of the
//                  fractional-delay FIR.
//
// NOT instruction-exact (documented, not guessed):
//  * the 32-iteration windowed synthesis loop ($144F8F-$144FA1) with the
//    10-word coefficient table ($144F73-$144F8D: $EAAAAB=11/12, $40=1/2,
//    $D55555=10/12, $40, $80=1, $C0=3/2, $40, $7FFFFF=1, $155555=2/12,
//    $EAAAAB) is transcribed 1:1 by structure and coefficients, but the
//    48-bit accumulator plumbing is [РЕКОНСТР.].
//  * the WID window-pair integrator internals are [РЕКОНСТР.] at the same
//    level (law confirmed, pipeline approximated).
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "MnmFlaExactTables.hpp"

namespace monomachine {
namespace mnm {

inline constexpr int kFlaBufBits = 12;
inline constexpr int kFlaBufSize = 1 << kFlaBufBits;   // 4096 words, modulo $FFF
inline constexpr int kFlaFrame = 16;                   // samples per proc call
inline constexpr double kFlaDspRate = 44100.0;
// one-pole pair ($28F5C positive / $FD70A4 = −$028F5C negative), MAC ×2 folded in
inline constexpr double kFlaSmooth = 2.0 * 0x28F5C / 8388608.0;   // 0.0031808
inline constexpr double kFlaDelSlope = 1021.0 / 128.0;            // 7.9766 words/knob
inline constexpr double kFlaFbScale = 0.508 * 4.0 * 0.99;         // 2.01168

class FlaCore {
public:
    void reset(double hostRate) {
        sr = hostRate > 0.0 ? hostRate : kFlaDspRate;
        for (int c = 0; c < 2; ++c)
            for (int i = 0; i < kFlaBufSize; ++i)
                buf[c][i] = 0.0;
        wpos = 0;
        delState = 0.0;
        depState = 0.0;
        lfoPhase = 0.0;
        lfoPrevS = 0.0;
        lfoPrevC = 0.0;
        fade = 0;
        delTarget = 0.0;
        for (int i = 0; i < kFlaFrame; ++i) { wetL[i] = 0.0f; wetR[i] = 0.0f; }
    }

    // p[0..7]: DEL DEP WID MIX FB SPD --- INP (knobs 0..127)
    void setParameters(const int* p) noexcept {
        kDel = p[0]; kDep = p[1]; kWid = p[2]; kMix = p[3];
        kFb = p[4]; kSpd = p[5]; kInp = p[7];
        // DEL target: floor(1021*k/128) − 3 words ($144E8D-$144E91)
        delTarget = -((int)llrint(kFlaDelSlope * kDel) + 3);
    }

    // inL/inR: kFlaFrame samples; outL/outR: kFlaFrame samples (floats −1..1)
    void process(const float* inL, const float* inR, float* outL, float* outR) noexcept {
        // fade-in counter: 128 frames ($144E9C-$144EA4)
        if (fade < 128) ++fade;
        const float fadeGain = fade >= 128 ? 1.0f : (float)fade / 128.0f;

        // LFO integrator ($144EC8-$144ECF)
        const double spd = kSpd / 127.0;
        lfoPhase += spd * kFlaSmooth - lfoPhase * kFlaSmooth * 0.5;
        if (lfoPhase < 0.0) lfoPhase += 1.0;
        if (lfoPhase >= 1.0) lfoPhase -= 1.0;
        // quadrature ($144ED1-$144ED7): sin = tbl[i], cos = tbl[2048+i]
        const int idx = std::min(2047, (int)(lfoPhase * 2048.0));
        const double s = mnmfla::kFlaSineTab[idx] / 8388608.0;
        const double c = mnmfla::kFlaSineTab[2048 + idx] / 8388608.0;
        // frame ramp from previous frame values ($144ED8-$144EE6)
        const double dS = (s - lfoPrevS) / kFlaFrame;
        const double dC = (c - lfoPrevC) / kFlaFrame;
        // DEL / DEP smoothing ($144EFE-$144F13)
        delState += (delTarget - delState) * kFlaSmooth;
        depState += ((double)kDep / 127.0 - depState) * kFlaSmooth;
        // FB gain ($144FBE-$144FC8)
        const double fb = (double)(kFb - 64) / 127.0 * kFlaFbScale;
        // input gain ($144EA6-$144EAB)
        const double g = 4.0 * ((double)kInp / 127.0) * ((double)kInp / 127.0) * fadeGain;
        // WID window span ($144EB4-$144EB9) [РЕКОНСТР. scale]
        const double wid = ((double)kWid / 127.0) * ((double)kWid / 127.0)
                           * 2.0 * 0x9566 / 8388608.0;
        const int tapSpan = std::max(1, (int)(wid * 512.0));

        double lS = lfoPrevS, lC = lfoPrevC;
        const double m = (double)kMix / 127.0;
        for (int i = 0; i < kFlaFrame; ++i) {
            lS += dS;
            lC += dC;
            const double dryL = inL[i] * g;
            const double dryR = inR[i] * g;
            double wetL = 0.0, wetR = 0.0;

            for (int ch = 0; ch < 2; ++ch) {
                // tap position: delState + DEP*LFO; R channel: inverted LFO ×0.7071
                const double lfo = (ch == 0) ? lS : (-lC * 0.70710678);
                double pos = (double)wpos + (double)delState + depState * lfo * tapSpan;
                while (pos < 0.0) pos += kFlaBufSize;
                while (pos >= (double)kFlaBufSize) pos -= (double)kFlaBufSize;
                const int i0 = (int)pos & (kFlaBufSize - 1);
                const double fr = pos - std::floor(pos);
                const int i1 = (i0 + 1) & (kFlaBufSize - 1);
                const double rd = buf[ch][i0] * (1.0 - fr) + buf[ch][i1] * fr;
                if (ch == 0) wetL = rd; else wetR = rd;
                // comb write-back ($144FD3-$144FE2): buf = dry + fb*read
                buf[ch][wpos] = (ch == 0 ? dryL : dryR) + fb * rd;
            }
            // MIX crossfade ($145027-$145034)
            outL[i] = (float)(wetL * (1.0 - m) + dryL * m);
            outR[i] = (float)(wetR * (1.0 - m) + dryR * m);
            wpos = (wpos + 1) & (kFlaBufSize - 1);
        }
        lfoPrevS = s;
        lfoPrevC = c;
    }

    double sampleRate() const noexcept { return sr; }

private:
    double sr = kFlaDspRate;
    double buf[2][kFlaBufSize];        // L/R delay lines (48-bit pairs on the DSP)
    int wpos = 0;
    double delState = 0.0;             // $1d 48-bit smoothed DEL
    double depState = 0.0;             // $22 smoothed DEP
    double lfoPhase = 0.0;             // y:$24
    double lfoPrevS = 0.0, lfoPrevC = 0.0;   // y:$25/y:$26
    double delTarget = 0.0;
    float wetL[kFlaFrame], wetR[kFlaFrame];
    int fade = 0;                      // y:$21
    int kDel = 0, kDep = 0, kWid = 0, kMix = 0, kFb = 0, kSpd = 0, kInp = 0;
};

} // namespace mnm
} // namespace monomachine
