// FX-RINGMOD (machine 17) -- "mnm" core, recovered from OS 1.32B.
// Monomachine Nova 1.7.12-mining. Every claim lists the firmware address so it
// can be re-checked against decompiled data/03_listings/machines_page_A/17_FX-RING_full.txt
// (slice P:$14789E-$147B37, init $14789E / config $1478C6 / proc $1478C7).
//
// CONFIRMED from the listing + emulator runs (scripts/exp_m17_ring.py):
//  * carrier table:  X:$14A000, 8192 words, 13-bit phase mask $1FFF, linear
//    interpolation ($147907-$147936: base + (0x1FFF - ((p-$800)&$1FFF)),
//    mpysu/add/macsu/asr interp). The kernel SVF reads the same region at
//    +2048 for its quadrature tap (P:$0008E5 x:(r2+$14a801)) -> the region is
//    one unfolded sine. Same conclusion as the FM cores (see MnmFm.hpp).
//  * carrier pitch:  the OS passes the carrier frequency in A (Hz scale).
//    Empirical slope 0.18565 idx/sample per Hz = 8192/44100 (0.1 % off).
//    Hard clamp at 0x4074 = 16500 Hz ($1478CF-$1478D2: cmp #>$4074 / tge).
//    -> carrier is KEYTRACKED to the track note; there is no TUNE param
//    (raw descriptor fx_descriptors_region.bin @0x370: slots are
//    WAVE EXT --- MIX --- --- --- INP; slot 2 of machine_definitions.hpp was
//    an invention and is fixed in this patch).
//  * INP law:        three scaled copies of the input; the main copy carries
//    gain 4*(INP/127)^2 -- unity at INP=64 ($147A2A-$147A3E: mpy x0,x0 then
//    asl #2; emulator: in*0.25 for INP=64, *1.0 in the x4 copy).
//  * EXT/WAVE weights: $147A04-$147A0E computes
//        b = (127-EXT)*2W,  a = (127-EXT)*b        (x1 = 127-EXT, y0 = 2W)
//    and $147A74-$147A76 clamps the (127-2W) weight at zero ("clr a ifmi").
//    The pair (127-2W, 2W) sums to 127 -> WAVE is a crossfade between the two
//    carrier sources; EXT is a master fade of the internal pair toward the
//    external carrier path.
//  * two carrier sources: the shared OS oscillator helper func_0003D3
//    (bsr at $14796D, quadrature seeds 0x5AE148/-0x5AE148 = +-0.707 written
//    at $14797C-$147981) and the table sine read with the -$800 base offset
//    ($147909). -$800 of 0x2000 = -90 degrees -> the alternate source is the
//    quadrature (-cos) of the main sine. WAVE morphs sine -> quadrature.
//  * stereo:         the L/R product buses carry the carrier in quadrature
//    (the +-0.707 seeds); output sums the buses 50/50 ($147AA9-$147AB5:
//    mpy x1,x0 + mac y1,y0 with x1=y1=0x40).
//  * MIX:            one-pole smoothed before the dry/wet crossfade
//    (state cell (r6+$54), $147A5B-$147A72). Coefficient not recovered --
//    5 ms one-pole used, flagged below.
//
// NOT instruction-exact (documented, not guessed):
//  * the exact 48-bit accumulator pipeline of the pitch->increment division
//    ($1478CF-$1478DF, clb/normf/div x0 chain over the constant $2B1100);
//    the closed form inc = f*8192/sr reproduces the measured slope.
//  * the external carrier tap point: the firmware mixes a 64-sample frame
//    history (4x16 rotating slots, x:(r6+$58)..(r6+$97)) written by the OS.
//    In the plugin the external carrier is the opposite channel's input.
//  * the MIX smoothing coefficient.
#pragma once

#include <algorithm>
#include <cmath>

namespace monomachine {
namespace mnm {

inline constexpr int kRingTableBits = 13;                    // $1FFF phase mask
inline constexpr int kRingTableSize = 1 << kRingTableBits;   // 8192, X:$14A000
inline constexpr int kRingQuadOffset = 0x800;                // -$800 read offset
inline constexpr float kRingMaxCarrierHz = 16500.0f;         // 0x4074 clamp
inline constexpr double kRingDspRate = 44100.0;              // internal DSP1 rate

class RingCore {
public:
    void reset(double hostRate) {
        sr = hostRate > 0.0 ? hostRate : kRingDspRate;
        phase = 0.0;
        mixSmooth = 0.0f;
        carrierHz = 261.6f;
    }

    // p[0]=WAVE, p[1]=EXT, p[3]=MIX, p[7]=INP (slot 2 is unused on hardware).
    void setParameters(float wave, float ext, float mix, float inp) {
        // Quadrature-set weight: (127-2W)/127 clamped at 0 ($147A74-$147A76).
        wSin = std::clamp((127.0f - 2.0f * wave) / 127.0f, 0.0f, 1.0f);
        wQuad = 1.0f - wSin;
        // EXT master fade of the internal pair ($147A0A-$147A0E).
        wExt = std::clamp((127.0f - ext) / 127.0f, 0.0f, 1.0f);
        // INP: gain = 4*(INP/127)^2, unity at 64 ($147A2A-$147A3E).
        const float g = std::clamp(inp / 127.0f, 0.0f, 1.0f);
        inpGain = 4.0f * g * g;
        // MIX smoothing target (one-pole state (r6+$54)).
        mixTarget = std::clamp(mix / 127.0f, 0.0f, 1.0f);
    }

    // Carrier is keytracked: the OS passes the track note frequency in A.
    void setCarrierFrequency(float hzValue) {
        carrierHz = std::clamp(hzValue, 0.0f, kRingMaxCarrierHz);
    }

    void processStereo(const float* inL, const float* inR,
                       float* outL, float* outR, int numFrames) {
        if (numFrames <= 0) return;
        // Per-sample table advance at the host rate (firmware law:
        // f * 8192 / 44100; measured slope 0.18565 vs 0.18578 theoretical).
        const double inc = static_cast<double>(carrierHz) *
                           static_cast<double>(kRingTableSize) / sr;
        // MIX one-pole (coefficient not recovered -- 5 ms, flagged).
        const float mixCoeff = 1.0f - std::exp(-1.0f / (0.005f * static_cast<float>(sr)));
        const double dt = 2.0 * 3.14159265358979323846 / static_cast<double>(kRingTableSize);

        for (int i = 0; i < numFrames; ++i) {
            const float xl = inL ? inL[i] : 0.0f;
            const float xr = inR ? inR[i] : 0.0f;
            mixSmooth += mixCoeff * (mixTarget - mixSmooth);

            // Two carrier sources: main sine and the -$800 (quadrature) read
            // ($147909 sub #>$800 -> -90 degrees).
            const double p0 = phase * dt;
            const float srcSin = static_cast<float>(std::sin(p0));
            const float srcQuad = static_cast<float>(-std::cos(p0));  // sin(p - 90 deg)
            const float internal = wSin * srcSin + wQuad * srcQuad;
            // L and R carry the carrier in quadrature (+-0.707 seeds).
            const float internalR = wSin * srcQuad + wQuad * srcSin;

            // EXT blends the internal pair toward the opposite channel input
            // (external carrier tap; see header note).
            const float carrierL = wExt * internal + (1.0f - wExt) * xr;
            const float carrierR = wExt * internalR + (1.0f - wExt) * xl;

            const float ringL = xl * inpGain * carrierL;
            const float ringR = xr * inpGain * carrierR;

            outL[i] = xl + mixSmooth * (ringL - xl);
            outR[i] = xr + mixSmooth * (ringR - xr);

            phase += inc;
            if (phase >= static_cast<double>(kRingTableSize))
                phase -= static_cast<double>(kRingTableSize) *
                         std::floor(phase / static_cast<double>(kRingTableSize));
        }
    }

private:
    double sr = kRingDspRate;
    double phase = 0.0;
    float wSin = 1.0f, wQuad = 0.0f, wExt = 1.0f;
    float inpGain = 1.0f;
    float mixSmooth = 0.0f, mixTarget = 1.0f;
    float carrierHz = 261.6f;
};

} // namespace mnm
} // namespace monomachine
