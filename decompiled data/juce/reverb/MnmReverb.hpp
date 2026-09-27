// =============================================================================
// MnmReverb.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m13 "FX-REVERB", bit-exact port of DSP1 PROC P:$1453DC-$14561B.
// =============================================================================
// Verified against a bit-exact DSP56300 emulator running the original firmware
// (emulator fixes required & applied this session: non-power-of-2 modulo
// addressing, Data Limit Checking on accumulator stores, 8-bit immediate
// MOVE left-justification). Impulse/decay/DAMP sweeps match ROM behaviour.
//
// Signal flow (all stages verified on ROM):
//   1. input   Y:0-1F[k]  = 2*INP^2*(bus[2k]+bus[2k+1])
//   2. 5 allpasses (47/67/101/149/223), feedback k_i = fade*(0.35+0.05*DEC)*coef_i
//   3. 6 comb taps: 1-4 one-pole damping (DAMP table), 5-6 LFO-interpolated
//   4. matrix  (DEC table coefficients, L/R +/- pairs):
//        L[k] = 2*( S0[k]*c3 - S1[k]*c6 - S2[k]*c4 + S3[k]*c2 )
//        R[k] = 2*( S0[k]*(c3+c1) - S1[k]*c6 - S2[k]*c4 )
//      S0..S3 = rotation of (DEC0,DEC5,DEC3,DEC1) by k mod 4
//   5. comb write: line += matrix_bank + allpass_out*$155556
//   6. gate: leaky peak -> ramp (attack $147AE*16, decay *$7EB852), wet *= ramp
//   7. HP / LP sections (coefficient LP[$144AC7][HP/LP])
//   8. final: out = 4*(MIX*wet + (1-MIX)*INP^2*dry)
// =============================================================================

#pragma once
#include <cstdint>
#include <cstring>
#include <cmath>
#include "mnm_reverb_tables.h"
#include "mnm_trackdelay_tables.h"   // tbl_lp_coeffs_144AC7

namespace mnm {
namespace rv {

static const uint32_t M24 = 0xFFFFFFu;
static const uint64_t M56 = 0xFFFFFFFFFFFFFFull;

inline int64_t sext(int64_t v, int bits) {
    const int64_t s = 1ll << (bits - 1);
    v &= ((1ll << bits) - 1);
    return (v ^ s) - s;
}
inline int32_t s24(int32_t v) { return (int32_t)sext(v, 24); }

inline uint64_t acc_of(int32_t v24) { return ((uint64_t)(sext(v24, 24) & M56)) << 24; }
inline uint32_t A1(uint64_t a) { return uint32_t((a >> 24) & M24); }
inline uint64_t acc56(int64_t v) { return uint64_t(sext(v, 56)) & M56; }
inline uint64_t mac56(uint64_t a, int32_t x, int32_t y) {
    return acc56(int64_t(a) + (int64_t(s24(x)) * int64_t(s24(y)) << 1));
}
inline uint64_t mpy56(int32_t x, int32_t y) {
    return acc56(int64_t(s24(x)) * int64_t(s24(y)) << 1);
}
inline uint64_t add_reg24(uint64_t acc, uint32_t r24) {
    return acc56(int64_t(acc) + (int64_t(s24(int32_t(r24))) << 24));
}
inline uint64_t sub56(uint64_t a, uint64_t b) { return acc56(int64_t(a) - int64_t(b)); }
inline uint64_t asl56(uint64_t a) { return acc56(int64_t(a & M56) << 1); }
inline uint64_t asr56(uint64_t a, int n) {
    return uint64_t(sext(int64_t(a & M56), 56) >> n) & M56;
}
inline uint32_t neg24(int32_t v) { return (uint32_t)(-s24(v)) & M24; }
// Data Limit Checking (FM 5.4.1.2): 24-bit accumulator store saturates
inline uint32_t limit24(uint64_t acc) {
    int64_t v = sext(int64_t(acc & M56), 56);
    if (v > 0x007FFFFFFFFFFFll) return 0x7FFFFF;
    if (v < -0x00800000000000ll) return 0x800000;
    return A1(acc);
}

struct TrackReverbCore {
    static const int COMB_LEN[6];   // masks $A76 $99E $722 $664 $7FF $3FF
    static const int COMB_DLY[6];   // INIT write-pointer offsets = comb delays
    static const int AP_LEN[5];     // masks $2E  $42  $64  $94  $DE

    int32_t comb[6][2679];
    int     combRd[6], combWr[6];
    uint64_t combSt[6];
    int32_t ap[5][223];
    int     apPtr[5];
    int32_t apCarry[5];
    int32_t apOutSave[16];          // Y:$0-$1F at the comb-write time

    uint64_t hpStA, hpStB;          // r6+$18/$19, $1A/$1B
    int32_t  hpStX0L, hpStX0R;      // r6+$1C/$1D
    uint64_t lpStA, lpStB;          // r6+$14/$15, $16/$17

    int32_t  envSlow, envFast;      // r6+$1e/$1f
    int32_t  quietCnt, gateRate;    // r6+$20/$25
    uint64_t gateRamp;              // r6+$21
    int32_t  fadeCnt, fadeGain;     // r6+$48/$47
    uint32_t lfoPhase;              // r6+$32

    int32_t pDec, pDamp, pGate, pMix, pHp, pLp, pInp;
    // stage snapshots (last processed frame) — for differential debugging
    int32_t snapRamp[16], snapMtxL[16], snapMtxR[16], snapHpL[16], snapLpL[16];
    int32_t snapAp[16], snapComb[6][16], snapWet[32];
    int32_t hpPrev[32];             // X:$20-$3F: the previous frame's HP outs (comb1/2 banks)
    int32_t dbgN1, dbgFrac, dbgWE, dbgWO, dbgCD;
    int32_t dbgRd5, dbgRd6, dbgWr5, dbgWr6;
    uint64_t dbgSt5, dbgSt6;
    bool dbgPrint = false; uint64_t dbgAcc[3]; uint32_t dbgOut[3];
    int32_t dbgX[3], dbgY[3];
    // debug snapshot (last frame)

    TrackReverbCore() { reset(); }

    void reset() {
        memset(comb, 0, sizeof(comb)); memset(ap, 0, sizeof(ap));
        for (int i = 0; i < 6; i++) { combRd[i] = 0; combWr[i] = COMB_DLY[i]; combSt[i] = 0; }
        for (int i = 0; i < 5; i++) { apPtr[i] = 0; apCarry[i] = 0; }
        memset(apOutSave, 0, sizeof(apOutSave));
        memset(hpPrev, 0, sizeof(hpPrev));
        hpStA = hpStB = lpStA = lpStB = 0;
        hpStX0L = hpStX0R = 0;
        envSlow = envFast = quietCnt = 0; gateRate = 0;
        gateRamp = 0; fadeCnt = 0; fadeGain = 0; lfoPhase = 0;
        pDec = pDamp = pGate = pMix = pHp = pLp = pInp = 0;
    }

    static int32_t sinLUT(int idx) {
        static int32_t tbl[8192]; static bool init = false;
        if (!init) {
            for (int i = 0; i < 8192; i++)
                tbl[i] = int32_t(std::lround(0x7FFFFF * std::sin(2.0 * M_PI * i / 8192.0)));
            init = true;
        }
        return tbl[idx & 8191];
    }

    void setParams(int32_t dec, int32_t damp, int32_t gate, int32_t mix,
                   int32_t hp, int32_t lp, int32_t inp) {
        pDec = dec; pDamp = damp; pGate = gate; pMix = mix;
        pHp = hp; pLp = lp; pInp = inp;
        uint64_t b = asr56(mpy56(gate, gate), 11);
        // cmp #$fa0,b — ALU 8-bit immediate is RAW (4000), not A1-aligned
        gateRate = (sext((int64_t)b, 56) > 0x000FA0) ? 0x0F4240 : A1(b);
    }

    void processFrame(const int32_t* bus32, int32_t* out16) {
        // firmware 1453fa-145405 / 1453f9-1453fc: maci #>$3,x0,a => a0 = 2*3*(param>>17)
        // (MPY result is shifted left 1 in the accumulator) — offset step is 6, not 3
        int decOff = 6 * ((uint32_t)pDec >> 17);
        int dampOff = 6 * ((uint32_t)pDamp >> 17);

        // ---- fade ($1453E2-$1453EF) ----
        if (fadeCnt < 0x100) { fadeCnt += 1; fadeGain = 0; }
        else fadeGain = 0x7FFFFF;

        // ---- allpass feedback gain ($145421-$14542C) ----
        int32_t gb = A1(add_reg24(mpy56(pDec, 0x066666), 0x59999A));
        int32_t y1g = A1(mpy56(gb, fadeGain));

        // ---- input stage ($145413-$145420) ----
        int32_t inBuf[16];
        {
            // pushes = 2*INP^2*(bus[2k]+bus[2k+1]), limit-checked stores
            int32_t y0 = A1(mpy56(pInp, pInp));
            for (int k = 0; k < 16; k++) {
                uint64_t a = mpy56(y0, bus32[2*k]);
                a = mac56(a, y0, bus32[2*k + 1]);
                inBuf[k] = (int32_t)limit24(asl56(a));
            }
        }

        // ---- allpass cascade ($14542D-$145441) ----
        for (int i = 0; i < 5; i++) {
            int32_t coef = (int32_t)tbl_reverb_ap_coef_14561c[i];
            int32_t x1 = A1(mpy56(y1g, coef));
            int len = AP_LEN[i];
            int32_t* ring = ap[i];
            int ptr = apPtr[i];
            int32_t carry = apCarry[i];
            int32_t buf = ring[ptr];
            for (int k = 0; k < 16; k++) {
                ring[ptr] = carry;
                ptr = (ptr + 1) % len;
                uint64_t a = sub56(acc_of(buf), mpy56(x1, carry));
                int32_t outk = limit24(a);
                int32_t nb = ring[ptr];
                // firmware 14543d/14543f: b <- in[k] (acc load), then mac x1,x0,b —
                // a plain 56-bit accumulator add of the product (NOT a 24-bit
                // register add — the old add_reg24() call truncated the product)
                carry = A1(acc56(int64_t(acc_of(inBuf[k])) + int64_t(mpy56(x1, nb))));
                inBuf[k] = outk;
                buf = nb;
            }
            apPtr[i] = ptr; apCarry[i] = carry;
        }
        memcpy(apOutSave, inBuf, sizeof(apOutSave));   // allpass out (comb write uses it)

        // ---- comb taps ($145449-$1454D8) ----
        int32_t combOut[6][16];
        memcpy(snapAp, apOutSave, sizeof snapAp);
        for (int c = 0; c < 4; c++) {
            int32_t coef = (int32_t)tbl_reverb_damp_1457b2[dampOff + c];
            int len = COMB_LEN[c];
            int32_t* line = comb[c];
            int ptr = combRd[c];
            uint64_t a = combSt[c];
            int32_t y0 = line[ptr]; ptr = (ptr + 1) % len;
            for (int k = 0; k < 16; k++) {
                combOut[c][k] = limit24(a);
                int32_t x1 = A1(a);
                a = mac56(a, y0, coef);
                int32_t nx = line[ptr]; ptr = (ptr + 1) % len;
                a = mac56(a, x1, neg24(coef));
                y0 = nx;
            }
            combRd[c] = ptr - 1; combSt[c] = a;   // trailing (r1)- read

        }
        {
            int32_t sb = sinLUT((int)lfoPhase);
            lfoPhase = (lfoPhase + 16) & 8191;
            int32_t n1 = (int32_t)(sext((uint32_t)sb, 24)) >> 21;      // integer tap
            // firmware decomposition (1454a0-1454a6): b = sine<<24; asr #21 (b=sine<<3);
            // n1 = b1; clear b1 (drop bits 47..24, keep sign b2); asr b; y1 = b0.
            // => frac = ((sine<<3) & 0xFFFFFF) >> 1  ==  (sine<<2) & 0x7FFFFF
            // (the old (sine<<2)&0xFFFFFF kept bit 23 and doubled frac for sine>0.25)
            int64_t lb = ((int64_t)sb << 24) >> 21;
            lb &= ~(0xFFFFFFll << 24);
            lb >>= 1;
            int32_t frac = (int32_t)(lb & M24);
            int32_t cD = (int32_t)tbl_reverb_damp_1457b2[dampOff + 4];
            dbgN1 = n1; dbgFrac = frac; dbgCD = cD;
            int32_t wE = A1(mpy56(cD, A1(sub56(acc_of(0x7FFFFF), acc_of(frac)))));
            int32_t wO = A1(mpy56(cD, frac));
            dbgWE = wE; dbgWO = wO;
            dbgRd5 = combRd[4]; dbgRd6 = combRd[5]; dbgWr5 = combWr[4]; dbgWr6 = combWr[5];
            dbgSt5 = combSt[4]; dbgSt6 = combSt[5];
            for (int c = 4; c < 6; c++) {
                int len = COMB_LEN[c];
                int32_t* line = comb[c];
                int ptr = ((combRd[c] + n1) % len + len) % len;
                uint64_t a = combSt[c];
                int32_t x1 = line[ptr]; ptr = (ptr + 1) % len;
                for (int k = 0; k < 16; k++) {
                    combOut[c][k] = limit24(a);
                    a = mac56(a, x1, wE);
                    int32_t nx = line[ptr]; ptr = (ptr + 1) % len;
                    a = mac56(a, nx, wO);
                    int32_t y0 = combOut[c][k];
                    a = mac56(a, y0, neg24(cD));   // mac -y0,x0,a
                    x1 = nx;
                }
                ptr = (ptr - 1 + len) % len;      // trailing (r1)- read
                combRd[c] = ((ptr - n1) % len + len) % len;
                combSt[c] = a;
            }
        }

        for (int c = 0; c < 6; c++) memcpy(snapComb[c], combOut[c], sizeof snapComb[c]);

        // ---- matrix ($1454DC-$145546) — literal transcription ----
        // sec1 coef field X:$20-$24 = FIVE words (m3 = 4 -> length 5), written by
        // the preload in order DEC[2],DEC[5],DEC[3],DEC[1],DEC[0]; the coefficient
        // pointer (r3) cycles the same 5 addresses every iteration (5 loads/iter).
        // Per sample k the MACs use (verified by single-step at f38):
        //   a  = F0*c3[k] - F1*c6[k] - F2*c4[k] + F3*c2[k]   (F0=DEC[2] for k>=1,
        //      the k=0 first term uses the setup x0 = DEC[0])
        //   b  = a_without_c2_term + F4*c1[k]
        //   both <<1, stores: mtxL = A1(a), mtxR = A1(b)
        int32_t mtxL[16], mtxR[16];
        int32_t bankC3[16], bankC5[16], bankC6[16];
        {
            const int32_t f0 = (int32_t)tbl_reverb_dec_14562c[decOff + 2];  // X:$20
            const int32_t f1 = (int32_t)tbl_reverb_dec_14562c[decOff + 5];  // X:$21
            const int32_t f2 = (int32_t)tbl_reverb_dec_14562c[decOff + 3];  // X:$22
            const int32_t f3 = (int32_t)tbl_reverb_dec_14562c[decOff + 1];  // X:$23
            const int32_t f4 = (int32_t)tbl_reverb_dec_14562c[decOff + 0];  // X:$24
            // setup (1454EA): x0 = y:(r0) = DEC[0]; (1454F8): y1 = c3[0]
            int32_t x0 = (int32_t)tbl_reverb_dec_14562c[decOff + 0];
            int32_t y1 = combOut[2][0];
            uint64_t a = mpy56(x0, y1);                    // 1454fa mpy (DEC[0]*c3[0])
            x0 = f1; y1 = combOut[5][0];                   // 1454fa loads
            for (int k = 0; k < 16; k++) {
                a = mac56(a, x0, neg24(y1));               // 1454fd: -F1*c6[k]
                int32_t xn = f2; int32_t yn = combOut[3][k];
                a = mac56(a, xn, neg24(yn));               // 1454fe: -F2*c4[k]
                xn = f3; yn = combOut[1][k];
                a = mac56(a, xn, yn);                      // 145500: +F3*c2[k]
                uint64_t b = a; int32_t x1 = f4;           // 1454ff
                int32_t yc1 = combOut[0][k];
                a = asl56(a); xn = f0;                     // 145501
                b = mac56(b, yc1, x1);                     // 145502: +F4*c1[k]
                yn = (k < 15) ? combOut[2][k + 1] : combOut[3][0];  // y:(r0)+ (Y:$50 at k=15)
                b = asl56(b);                              // 145503
                mtxL[k] = limit24(a);                      // 145503 store
                a = mpy56(xn, yn);                         // 145504: F0*c3[k+1]
                xn = f1; yn = (k < 15) ? combOut[5][k + 1] : 0;  // 145504 loads (Y:$80 at k=15)
                mtxR[k] = limit24(b);                      // 145505 store
                y1 = yn;                                   // the c6[k+1] feeds the next 1454fd mac
            }
            // sec2 ($145509-$14551F): only the b-stream survives (X:$40) = comb3 bank;
            // the a-stream (X:$30) is overwritten by HP outputs before the comb write
            {
                int32_t x0 = (int32_t)tbl_reverb_dec_14562c[decOff + 0];   // DEC[0]
                int32_t x1 = (int32_t)tbl_reverb_dec_14562c[decOff + 3];   // DEC[3]
                int32_t y0 = combOut[0][0];                                // comb1 outs
                int32_t yy1 = combOut[3][0];                               // comb4 outs
                uint64_t b = mpy56(y0, x0);
                for (int k = 0; k < 16; k++) {
                    b = mac56(b, yy1, neg24(x1));
                    yy1 = (k < 15) ? combOut[3][k + 1] : combOut[4][0];   // Y:$60 overread
                    int32_t y0n = (k < 15) ? combOut[0][k + 1] : combOut[1][0];  // Y:$30 overread
                    bankC3[k] = limit24(b);                                // 14551e store
                    b = mpy56(y0n, x0);                                    // 14551e mpy
                }
            }
            // sec3 ($145533-$145546): a -> X:$60 = comb5 bank; b -> X:$70 = comb6 bank
            {
                int32_t x0 = (int32_t)tbl_reverb_dec_14562c[decOff + 1];   // DEC[1]
                int32_t x1 = (int32_t)tbl_reverb_dec_14562c[decOff + 2];   // DEC[2]
                int32_t y0 = combOut[1][0];                                // comb2 outs
                int32_t yy1 = combOut[2][0];                               // comb3 outs
                uint64_t a = mpy56(y0, x0), b = mpy56(y0, x0);
                for (int k = 0; k < 16; k++) {
                    a = mac56(a, yy1, x1);
                    b = mac56(b, yy1, neg24(x1));
                    y0 = (k < 15) ? combOut[1][k + 1] : combOut[2][0];     // Y:$40 overread
                    yy1 = (k < 15) ? combOut[2][k + 1] : combOut[2][1];    // Y:$41 overread
                    bankC5[k] = limit24(a);                                // 145545 store
                    bankC6[k] = limit24(b);                                // 145546 store
                    a = mpy56(y0, x0); b = mpy56(y0, x0);                  // 145545/546 mpy
                }
            }
        }

        // ---- comb write-back ($145547-$145562) ----
        // banks read sequentially from X:$20: comb1 <- prev HP outs[0..15],
        // comb2 <- prev HP outs[16..31], comb3 <- sec2 b-stream (X:$40),
        // comb4 <- X:$50 (never written by this machine = device SRAM state,
        // modelled as zero), comb5/6 <- sec3 a/b streams (X:$60/$70)
        {
            int32_t bankC4[16];
            memset(bankC4, 0, sizeof bankC4);
            const int32_t* banks[6] = { hpPrev, hpPrev + 16, bankC3, bankC4, bankC5, bankC6 };
            for (int c = 0; c < 6; c++) {
                int len = COMB_LEN[c];
                int32_t* line = comb[c];
                int ptr = combWr[c];
                for (int k = 0; k < 16; k++) {
                    uint64_t b = acc_of(banks[c][k]);
                    b = mac56(b, apOutSave[k], 0x155556);
                    line[ptr] = limit24(b);
                    ptr = (ptr + 1) % len;
                }
                combWr[c] = ptr;
            }
        }

        // ---- gate/env ($145566-$1455A7) ----
        int32_t rampVals[16];
        {
            uint64_t a = acc_of(envSlow);
            uint64_t bmax = 0;
            for (int k = 0; k < 16; k++) {
                int32_t x1 = A1(a);
                uint32_t mag = (uint32_t)apOutSave[k] & M24;
                if (s24((int32_t)mag) < 0)
                    mag = (uint32_t)(-(int64_t)s24((int32_t)mag)) & M24;
                a = mpy56(x1, 0x7FEF9E);
                a = mac56(a, (int32_t)mag, 0x001062);
                if (sext((int64_t)a, 56) > sext((int64_t)bmax, 56)) bmax = a;
            }
            envSlow = A1(a);
            int32_t rising = (int32_t)((uint32_t)A1(bmax) - (uint32_t)envFast) & M24;
            envFast = A1(bmax);
            if (s24(rising) >= 0xA8) quietCnt = 0;
            else if (quietCnt < gateRate) quietCnt += 1;
            else quietCnt = 0x7FFFF0;
            bool decay = (quietCnt >= gateRate) && (s24(rising) < 0xA8);
            if (!decay) {
                uint64_t r = gateRamp;
                for (int k = 0; k < 16; k++) {
                    r = add_reg24(r, 0x0147AE);
                    rampVals[k] = limit24(r);
                }
                gateRamp = r;
            } else {
                uint64_t r = gateRamp;
                for (int k = 0; k < 16; k++) {
                    r = mpy56(A1(r), 0x7EB852);
                    rampVals[k] = limit24(r);
                }
                gateRamp = r;
            }
            memcpy(snapRamp, rampVals, sizeof snapRamp);
            for (int k = 0; k < 16; k++)
                mtxL[k] = limit24(mpy56(rampVals[k], mtxL[k]));
            memcpy(snapMtxL, mtxL, sizeof snapMtxL);
            memcpy(snapMtxR, mtxR, sizeof snapMtxR);
        }

        // ---- HP section ($1455B4-$1455E6) ----
        // per sample: out = A1(acc) (the stored old acc); acc -= x_prev*y1;
        //             acc -= c*out; acc += in*y1   (y1 = 0.5*(1-c))
        // x_prev chain: x(0) = the saved X:$1E/$1F = the previous frame's wet[15];
        //               x(k) = the wet[k-1]
        int32_t hpL[16], hpR[16];
        {
            int32_t c = (int32_t)mnm::td::tbl_lp_coeffs_144AC7[((uint32_t)pHp >> 16) & 0xFFFF];
            int32_t y0 = A1(add_reg24(acc_of(c), 0x800000));       // c - 1.0
            int32_t y1 = A1(mpy56((uint32_t)(-0x400000) & M24, y0)); // 0.5*(1-c)
            uint64_t a = hpStA, bch = hpStB;
            int32_t x0L = hpStX0L, x0R = hpStX0R;
            if (dbgPrint) printf("[hp] c=%06X y0=%06X y1=%06X a0=%014X x0L=%06X\n",
                                 c&M24, y0&M24, y1&M24, a, x0L&M24);
            for (int k = 0; k < 16; k++) {
                int32_t inL = mtxL[k], inR = mtxR[k];
                int32_t outL = limit24(a);              // a,x:(r2)+ — the old acc stored
                a = mac56(a, x0L, neg24(y1));           // mac -x0,y1,a
                a = mac56(a, c, neg24(outL));           // mac -x1,y0,a
                a = mac56(a, inL, y1);                  // mac x0,y1,a
                int32_t outR = limit24(bch);
                bch = mac56(bch, x0R, neg24(y1));
                bch = mac56(bch, c, neg24(outR));
                bch = mac56(bch, inR, y1);
                hpL[k] = outL; hpR[k] = outR;
                x0L = inL; x0R = inR;                   // the x-loads: X:(r0)+n0 / X:(r1)+n1
            }
            hpStA = a; hpStB = bch;
            hpStX0L = mtxL[15]; hpStX0R = mtxR[15];     // X:$1E/$1F <- wet[30]/[31]
            for (int k = 0; k < 16; k++) { hpPrev[2*k] = hpL[k]; hpPrev[2*k+1] = hpR[k]; }
            memcpy(snapHpL, hpL, sizeof snapHpL);
        }

        // ---- LP section ($1455E7-$1455FF) ----
        // per sample: out = A1(acc) (the old acc); acc += hp[k]*c; acc -= out*c
        int32_t lpL[16], lpR[16];
        {
            int32_t c = (int32_t)mnm::td::tbl_lp_coeffs_144AC7[((uint32_t)pLp >> 16) & 0xFFFF];
            uint64_t a = lpStA, bch = lpStB;
            for (int k = 0; k < 16; k++) {
                int32_t outL = limit24(a);
                a = mac56(a, hpL[k], c);                // mac x1,y0,a
                a = mac56(a, outL, neg24(c));           // mac -y1,y0,a
                int32_t outR = limit24(bch);
                bch = mac56(bch, hpR[k], c);
                bch = mac56(bch, outR, neg24(c));
                lpL[k] = outL; lpR[k] = outR;
            }
            lpStA = a; lpStB = bch;
            memcpy(snapLpL, lpL, sizeof snapLpL);
        }

        // ---- final mix ($145600-$14561B) ----
        {
            int32_t x1 = A1(sub56(acc_of(0x7FFFFF), acc_of(pMix)));
            int32_t y1 = A1(mpy56(pInp, pInp));
            int32_t dg = A1(mpy56(x1, y1));
            for (int k = 0; k < 16; k++) {
                // wet = X:$0-$1F stream (L/R interleaved); dry = bus[k] sequential
                int32_t w = (k & 1) ? lpR[k >> 1] : lpL[k >> 1];
                uint64_t a = mpy56(pMix, w);
                a = mac56(a, dg, bus32[k]);
                a = asl56(a); a = asl56(a);
                out16[k] = limit24(a);
            }
            for (int k = 0; k < 16; k++) { snapWet[2*k] = lpL[k]; snapWet[2*k+1] = lpR[k]; }
        }
    }
};

const int TrackReverbCore::COMB_LEN[6] = {2679, 2447, 1827, 1637, 2048, 1024};
const int TrackReverbCore::COMB_DLY[6] = {2663, 2447, 1811, 1621, 1399, 613};
const int TrackReverbCore::AP_LEN[5] = {47, 67, 101, 149, 223};

}} // namespace
