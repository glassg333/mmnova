// =============================================================================
// MnmSid.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m3 "SID-6581" — transcription of DSP1 P:$14636F-$1464FB.
// *** STATUS: WORK-IN-PROGRESS, NOT YET VALIDATED AGAINST EMULATOR VECTORS ***
// *** (research/m3_vectors.txt + exp58_m3_harness.py are ready; the wavetable
// ***  branch, the LFSR branch and the filter wheel-step switch still need the
// ***  test-driven fix pass).  Do NOT treat as firmware-identical yet.
// =============================================================================
// Block map (listing 03_SID_full.txt; vectors exp58, wavetable = saw default):
//
//   INIT  $14636F: zero $19,$1B-$1F; Y:($1A) = $19B5 (LFSR seed)
//   CFG   $14637A: PWRS(Y:$6)>=0.5 -> Y:($19) = 0
//   PROC  $146382 (one 16-sample frame):
//     A. freq step   $146382-$146390: A>>15 -> a0; macuu $FD9FB3; lsr#5; asl#4
//                     -> new step; if WAVE(Y:$7) < $666666 -> Y:($16) = step
//     B. wave-mod    $146391-$1463C2: idx = (MFRQ>>6 + $400) >> 11 & $7FF reads
//                     X:$140000+idx (runtime machine-wave page); fract scaling
//                     by $20B40, mpyuu $FD9FB3, lsr#2 -> y0 contribution
//     C. phase run   $1463C3-$1463E4: two 48-bit phase chains Y:($17)/($18)
//                     advanced 191 (MOD>=0.5, with carry-kill bclr#0,b2 /
//                     clr a ifcs) or 192 (MOD<0.5, chains swapped) steps per
//                     frame; intermediate phases written to X:$00-$BF / Y:$00-$BF
//     D. waveform    $1463E5-$1464DD: MOD bit21 -> $146493 (wave-mix); else by
//                     WAVE(Y:$7): <0.1 -> $1464C0 LFSR (rol/eor x1=$658100,
//                     seeds $1A/$24); >=0.1 -> $14647D triangle (abs/$40/shift);
//                     dead ranges $146474/$1464A1/$1464DE (unreachable:
//                     bcs chain is monotonic); 192 words to Y:(r4)+
//     E. filter      $1463FB-$14646B: 10-word coeff table (const), 5 states
//                     $1B-$1F mirrored to X:$F0-$F6, polyphase macr structure
//                     x1=$6186, 16 iterations -> 32 output words Y:(r7)+
//     F. restore     $146461-$14646A: $1B-$1F from X:$F0-$F6
// =============================================================================

#pragma once
#include <cstdint>
#include <cstring>

namespace mnm {
namespace sid {

static const uint32_t M24 = 0xFFFFFFu;
static const uint32_t M56lo = 0xFFFFFFFFFFFFFFull;

inline int64_t sgn24(int64_t v) {
    v &= M24;
    return (v ^ 0x800000) - 0x800000;
}
inline uint32_t A1(int64_t a) { return uint32_t((a >> 24) & M24); }
inline int64_t wrap56(int64_t v) {
    v &= (1ll << 56) - 1;
    if (v & (1ll << 55)) v -= (1ll << 56);
    return v;
}
// mac (signed x signed): acc + sx*sy*2
inline int64_t mac_s(int64_t acc, uint32_t x, uint32_t y) {
    return wrap56(acc + sgn24(x) * sgn24(y) * 2);
}
// mpy (signed x signed)
inline int64_t mpy_s(uint32_t x, uint32_t y) { return mac_s(0, x, y); }
// macuu/mpyuu (unsigned x unsigned)
inline int64_t mac_u(int64_t acc, uint32_t x, uint32_t y) {
    return wrap56(acc + (x & M24) * (y & M24) * 2);
}
inline int64_t asr56(int64_t a, int n) {
    int64_t v = (a >> n) & M56lo;
    if (v & (1ll << 55)) v -= (1ll << 56);
    return v;
}
inline int64_t asl56(int64_t a, int n) {
    int64_t v = (a << n) & M56lo;
    if (v & (1ll << 55)) v -= (1ll << 56);
    return v;
}
// lsr on the 56-bit accumulator (logical, zero-fills the top)
inline int64_t lsr56(int64_t a, int n) { return (int64_t)((uint64_t)(a & M56lo) >> n); }

class Sid {
public:
    // knobs (raw q23 words, value<<16)
    void setKnobs(uint32_t pw, uint32_t pwad, uint32_t pwrs, uint32_t wave,
                  uint32_t mod, uint32_t msrc, uint32_t mfrq) {
        kPW = pw & M24; kPWAD = pwad & M24; kPWRS = pwrs & M24;
        kWAVE = wave & M24; kMOD = mod & M24; kMSRC = msrc & M24; kMFRQ = mfrq & M24;
    }
    void setPitchQ23(int32_t a1) { pitchA1 = a1 & (int32_t)M24; }

    // runtime machine-wave page (X:$140000, 2048 words)
    void setWavePage(const uint32_t* t2048) {
        for (int i = 0; i < 2048; ++i) wavePage[i] = t2048[i] & M24;
    }

    // one 16-sample frame -> 32 words (16 x L,R)
    void processFrame(uint32_t* out32) {
        // ---- A. frequency step ($146382-$146390) --------------------------
        {
            int64_t a = asr56((int64_t)pitchA1 << 24, 15);   // asr #$f,a,a
            uint32_t a0v = (uint32_t)(a & M24);              // move a0,x0
            a = mac_u(a, a0v, 0xFD9FB3);                     // macuu x0,y0,a
            a = lsr56(a, 5);                                 // lsr #$5,a
            int64_t x0 = asl56(a, 4);                        // asl #$4,a,a
            // move y:($7),b; cmp $666666,b; tfr x0,a iflt; move a,y:($16)
            if (sgn24(kWAVE) < (int64_t)0x666666) {
                st16 = A1(x0 << 24);                          // move a,y:($16)
            }
        }

        // ---- B. wavetable contribution ($146391-$1463C2) ------------------
        int64_t y0acc;
        {
            int64_t b = wrap56((int64_t)kMFRQ << 24);
            b = asr56(b, 6);                                  // asr #$6,b,b
            b = wrap56(b + ((int64_t)0x400 << 24));           // add #$400,b
            uint32_t y0w = A1(b << 0) ;                        // move b,y0 (b1)
            // r0=r4=0, m0=m4=$bf — 192-word modulo buffers
            int64_t a;
            if (sgn24(kMSRC) < (int64_t)0x400000) {
                // aux cell X/Y:(r6-$1): not initialised by the machine slice;
                // the kernel zeroes the page -> treat as 0
                int64_t xv = 0;                                // x:(r6-$1)
                a = xv ? xv : 0;                               // tst/bne/bra
            } else {
                a = mac_u(0, 0x2000, kMFRQ);                   // mpy $2000,x0,a
                a = wrap56(a + ((int64_t)0x800 << 24));        // add #$800,a
            }
            int64_t bb = asr56(a, 11);                         // asr #$b,a,b
            int64_t aa = wrap56((a & M56lo) & ((int64_t)0x7FF << 24) | (a & M24));
            // and #$7ff,a — logical AND applies to A1 only (b0 preserved)
            aa = (a & ~(0x1FFFFFFll << 24)) & ~(0xFFll << 48);
            aa = (aa & ~((int64_t)0x7FF << 24)) | (((a >> 24) & 0x7FF) << 24);
            uint32_t idx = (uint32_t)((aa >> 24) & 0x7FF);
            uint32_t r2v = idx;
            (void)r2v;
            int64_t wv = (int64_t)(wavePage[idx & 0x7FF]) << 24;  // x:(r2+$140000)
            wv = asr56(wv, 10);                                    // asr #$a,a,a
            wv = asl56(wv, (int)((bb >> 24) & M24));               // asl x0,a,a
            wv = mac_s(0, A1(wv), 0x20B40);                        // mpy $20b40
            wv = asr56(wv, 18);                                    // asr #$12
            uint32_t x0w = (uint32_t)(wv & M24);                   // move a0,x0
            y0acc = lsr56(mac_u(0, x0w, 0xFD9FB3), 2);             // mpyuu + lsr#2
            y0w_ = y0w;
        }

        // ---- C. phase run ($1463C3-$1463E4) -------------------------------
        {
            uint32_t x0 = st16;                                    // move y:($16),x0
            if (sgn24(kMOD) >= (int64_t)0x400000) {
                int64_t a = (int64_t)st17 << 24;                   // move y:($17),a1
                int64_t b = (int64_t)st18 << 24;                   // move y:($18),b1
                b = wrap56(b + (int64_t)x0 << 24);                 // add x0,b
                a = wrap56(a + (y0acc & M56lo));                   // add y0,a (48-bit)
                // bclr #0,b2 + clr a ifcs: the carry-kill per iteration
                int r0 = 0, r4 = 0;
                for (int k = 0; k < 191; ++k) {
                    b = wrap56(b + (int64_t)x0 << 24);             // add x0,b
                    XmYm.phaseB[k] = A1(b);                        // b1,y:(r4)+
                    a = wrap56(a + (y0acc & M56lo));               // add y0,a
                    XmYm.phaseA[k] = A1(a);                        // a1,x:(r0)+
                    // bclr #0,b2 (clear bit 0 of the b2 extension byte)
                    b &= ~((int64_t)1 << 48);
                    // clr a ifcs — carried-out accumulator cleared on carry;
                    // the emu's V/C semantics: ifcs = carry set (extension)
                    if (a & ((int64_t)1 << 48)) { a = 0; }
                }
                XmYm.phaseB[191] = A1(b);                          // move b1,y:(r4)+
                XmYm.phaseA[191] = A1(a);                          // move a1,x:(r0)+
                st18 = A1(b);                                      // move b1,y:($18)
                st17 = A1(a);                                      // move a1,y:($17)
            } else {
                int64_t a = (int64_t)st17 << 24;
                int64_t b = (int64_t)st18 << 24;
                int r0 = 0, r4 = 0;
                for (int k = 0; k < 192; ++k) {
                    b = wrap56(b + (y0acc & M56lo));               // add y0,b
                    XmYm.phaseB[k] = A1(b);                        // b1,y:(r4)+
                    a = wrap56(a + (int64_t)x0 << 24);             // add x0,a
                    XmYm.phaseA[k] = A1(a);                        // a1,x:(r0)+
                }
                st18 = A1(b);
                st17 = A1(a);
            }
        }

        // ---- D. waveform select ($1463E5-$1464DD) -------------------------
        // the 192-word waveform buffer = Y:(r4)+ — reuse XmYm.phaseB area:
        // the branches write Y:(r4)+ = 192 words (the SAME modulo buffer as
        // the phase run's Y:(r4)+ writes — the waveform REPLACES the phases).
        if (kMOD & 0x200000u) {
            // $146493: the wave-mix branch (mpyr against Y:(r4) phase data)
            int r0 = 0, r4 = 0;
            uint32_t x0 = Xm[r0++]; uint32_t y0 = Ym[r4];
            int64_t b = mpy_s(y0, x0) >> 0;                        // mpyr y0,x0,b
            b = wrap56(b + 0x800000);                              // macr rounding
            b = wrap56(b - 0x800000);
            b = mpy_s(y0, Xm[r0++]); r0++;
            uint32_t y0b = Ym[r4];
            b = asl56(abs56(b), 0);
            for (int k = 0; k < 95; ++k) {
                int64_t a = mpy_s(Ym[r4], Xm[r0++]); r4++;
                uint32_t y0c = Ym[r4];
                a = abs56(a);
                Ym[(r4 - 1) & 0xFF] = A1(b);
                b = mpy_s(y0c, Xm[r0++]); r4++;
                b = abs56(b);
            }
            int64_t a = mpy_s(Ym[r4], Xm[r0]);
            a = abs56(a);
            Ym[95 & 0xFF] = A1(b);
            Ym[96 & 0xFF] = A1(a);
        } else if (sgn24(kWAVE) >= (int64_t)0x199999) {
            // $14647D: triangle fold (abs; sub $40; asl) — 96 pairs
            int r0 = 0, r4 = 0;
            uint32_t x0 = 0x400000;                                 // move #$40,x0 (imm8<<16)
            int64_t a = (int64_t)Xm[r0++] << 24;
            int64_t b = (int64_t)Xm[r0++] << 24;
            for (int k = 0; k < 95; ++k) {
                a = abs56(a); b = abs56(b);
                a = wrap56(a - ((int64_t)x0 << 24));
                b = wrap56(b - ((int64_t)x0 << 24));
                a = asl56(a, 1); b = asl56(b, 1);
                Ym[r4++] = A1(a);
                a = (int64_t)Xm[r0++] << 24;
                Ym[r4++] = A1(b);
                b = (int64_t)Xm[r0++] << 24;
            }
            a = abs56(a); b = abs56(b);
            a = wrap56(a - ((int64_t)x0 << 24));
            b = wrap56(b - ((int64_t)x0 << 24));
            a = asl56(a, 1); b = asl56(b, 1);
            Ym[r4++] = A1(a);
            Ym[r4++] = A1(b);
        } else {
            // $1464C0: LFSR noise (WAVE < 0.1): rol/eor x1=$658100
            int r0 = 0, r4 = 0;
            uint32_t n0 = 4, n4 = 4;
            int64_t a = (int64_t)st1A << 24;                        // move y:($1a),a
            int64_t b = (int64_t)st24 << 24;                        // move y:($24),b
            uint32_t x1 = 0x658100;
            for (int k = 0; k < 48; ++k) {
                // rol a  a1,y:(r4)+n4 ; eor x1,a ifcs ; rol b  b1,y:(r4)+n4 ; eor x1,b ifcs
                uint8_t c = (uint8_t)((a >> 47) & 1);
                a = ((a & ~(0xFFll << 48)) & ~(0xFFFFFFll << 24)) |
                    ((((a >> 24) & 0xFFFFFF) << 1 | (a >> 47 & 1)) & 0xFFFFFF) << 24;
                if (a & (1ll << 47)) a |= 0xFFll << 48;
                Ym[(r4 + 3) & 0xFF] = A1(a); r4 += 4;
                if (c) a = a ^ ((int64_t)x1 << 24);
                c = (uint8_t)((b >> 47) & 1);
                b = ((b & ~(0xFFll << 48)) & ~(0xFFFFFFll << 24)) |
                    ((((b >> 24) & 0xFFFFFF) << 1 | (b >> 47 & 1)) & 0xFFFFFF) << 24;
                if (b & (1ll << 47)) b |= 0xFFll << 48;
                Ym[(r4 + 3) & 0xFF] = A1(b); r4 += 4;
                if (c) b = b ^ ((int64_t)x1 << 24);
                (void)n0; (void)n4;
            }
            st1A = A1(a);                                           // move a1,y:($1a)
            st24 = A1(b);                                           // move b1,y:($24)
            // the 48 LFSR words fill Y:(r4) — 192 words expected: the loop
            // writes every 4th slot (n4=4): the remaining slots hold the old
            // phase data — replicate by NOT touching them here.
        }

        // ---- E. the 10-tap filter ($1463FB-$14646B) -----------------------
        filterBlock(out32);

        // ---- F. state restore ($146461-$14646A) ---------------------------
        st1B = Xm[0xF0]; st1C = Xm[0xF1];
        st1D = Xm[0xF2]; st1E = Xm[0xF3];
        st1F = Xm[0xF4];
    }

    // state (voice-page mirrors)
    uint32_t st16 = 0, st17 = 0, st18 = 0, st19 = 0;
    uint32_t st1A = 0x0019B5, st1B = 0, st1C = 0, st1D = 0, st1E = 0, st1F = 0;
    uint32_t st22 = 0, st23 = 0, st24 = 0;
    uint32_t auxX = 0, auxY = 0;            // X/Y:(r6-$1)
    uint32_t Xm[256] = {0}, Ym[256] = {0};
    uint32_t wavePage[2048] = {0};

private:
    struct PhaseBufs { uint32_t phaseA[192], phaseB[192]; } XmYm;
    uint32_t y0w_ = 0;
    uint32_t kPW = 0, kPWAD = 0, kPWRS = 0, kWAVE = 0, kMOD = 0, kMSRC = 0, kMFRQ = 0;
    int32_t pitchA1 = 0;


    // ---------------------------------------------------------------------------
    // filterBlock — $1463FB-$14646B, verbatim.
    // 10-word coefficient wheel on the Y side at $F0 (m5=$9 -> ring length 10,
    // step n5=+2), state mirror X:$F0-$F6 (r1=$F0/r2=$F2/r3=$F4, linear m).
    // The listing's parallel moves are transcribed 1:1: every mac's operand
    // values are the registers BEFORE the instruction's own parallel loads.
    // ---------------------------------------------------------------------------
    void filterBlock(uint32_t* out32) {
        Xm[0xF0] = st1B; Xm[0xF1] = st1C;
        Xm[0xF2] = st1D; Xm[0xF3] = st1E;
        Xm[0xF4] = st1F;
        static const uint32_t coeffs[10] = {
            0x3EF3C2, 0xFE01EB, 0x82F338, 0x01D9BC, 0x3CD5C5,
            0x0650D5, 0x8400A4, 0xF9AF2B, 0xC22E5F, 0x04866C
        };
        // r5 = $F0, m5 = $9 (ring length 10), n5 = +2: 10 stores at +2 steps
        int r5o = 0;
        for (int i = 0; i < 10; ++i) {
            Ym[(0xF0 + r5o) & 0xFF] = coeffs[i];
            r5o = (r5o + 2) % 10;
        }
        auto yld = [&]() {                       // y:(r5)+n5
            uint32_t v = Ym[(0xF0 + r5o) & 0xFF];
            r5o = (r5o + 2) % 10;
            return v;
        };
        // NEG(y) as a 24-bit word (the -y0 operand form)
        auto neg24 = [](uint32_t v) { return (uint32_t)(-(int64_t)(v & M24)) & M24; };
        // macr = mac + rounding (add 0x800000 into A0, A0 zeroed)
        auto macr = [&](int64_t acc, uint32_t x, uint32_t y) {
            int64_t v = wrap56(acc + sgn24(x) * sgn24(y) * 2 + 0x800000);
            return v & ~(int64_t)M24;            // A0 = 0, keep A1
        };

        uint32_t x1 = 0x6186;
        int r1 = 0xF0, r2 = 0xF2, r3 = 0xF4;
        int r4 = 0;                              // Y-side waveform buffer index
        for (int k = 0; k < 16; ++k) {
            uint32_t x0 = Xm[r1++ & 0xFF];                 // $146431 x:(r1)+
            uint32_t y0 = yld();                           //        y:(r5)+n5
            int64_t a = mpy_s(x0, neg24(y0));              // $146432 mpy -y0,x0,a
            x0 = Xm[r1-- & 0xFF];                          //        x:(r1)-
            y0 = yld();                                    //        y:(r5)+n5
            a = mac_s(a, x0, neg24(y0));                   // $146433 mac -y0,x0,a
            Xm[r1++ & 0xFF] = x0;                          //        x0,x:(r1)+
            uint32_t y1 = Ym[r4++ & 0xFF];                 //        y:(r4)+
            a = asl56(a, 1);                               // $146434 asl a
            a = macr(a, x1, y1);                           // $146435 macr y1,x1,a
            x0 = Xm[r2++ & 0xFF];                          //        x:(r2)+
            y0 = yld();                                    //        y:(r5)+n5
            int64_t b = 0; (void)b;
            for (int j = 0; j < 10; ++j) {
                b = mpy_s(x0, neg24(y0));                  // $146438 mpy -y0,x0,b
                x0 = Xm[r2-- & 0xFF];                      //        x:(r2)-
                y0 = yld();                                //        y:(r5)+n5
                b = mac_s(b, x0, neg24(y0));               // $146439 mac -y0,x0,b
                Xm[r2++ & 0xFF] = x0;                      //        x0,x:(r2)+
                b = asl56(b, 1);                           // $14643a asl b
                Xm[r1-- & 0xFF] = A1(a);                   //        a,x:(r1)-
                b = macr(b, x1, y1);                       // $14643b macr y1,x1,b
                x0 = Xm[r3 & 0xFF];                        //        x:(r3)
                y0 = yld();                                //        y:(r5)+n5
                b = mpy_s(x0, neg24(y0));                  // $14643d mpy -y0,x0,b
                Xm[r2-- & 0xFF] = A1(b);                   //        b,x:(r2)-
                b = asl56(b, 1);                           // $14643e asl b
                b = macr(b, x1, y1);                       // $14643f macr y1,x1,b
                x0 = Xm[r1++ & 0xFF];                      //        x:(r1)+
                y0 = yld();                                //        y:(r5)+n5
                a = mpy_s(x0, neg24(y0));                  // $146440 mpy -y0,x0,a
                x0 = Xm[r1-- & 0xFF];                      //        x:(r1)-
                y0 = yld();                                //        y:(r5)+n5
                a = mac_s(a, x0, neg24(y0));               // $146441 mac -y0,x0,a
                Xm[r1++ & 0xFF] = x0;                      //        x0,x:(r1)+
                y1 = Ym[r4++ & 0xFF];                      //        y:(r4)+
                a = asl56(a, 1);                           // $146442 asl a
                Xm[r3 & 0xFF] = A1(b);                     //        b,x:(r3)
                a = macr(a, x1, y1);                       // $146443 macr y1,x1,a
                x0 = Xm[r2++ & 0xFF];                      //        x:(r2)+
                y0 = yld();                                //        y:(r5)+n5
                // ---- inner iterations 1..9 ($146444-$146458, unrolled) ----
                for (int j2 = 0; j2 < 9; ++j2) {
                    b = mpy_s(x0, neg24(y0));              // $146444 mpy -y0,x0,b
                    x0 = Xm[r2-- & 0xFF];                  //        x:(r2)-
                    y0 = yld();                            //        y:(r5)+n5
                    b = mac_s(b, x0, neg24(y0));           // $146445 mac -y0,x0,b
                    Xm[r2++ & 0xFF] = x0;                  //        x0,x:(r2)+
                    b = asl56(b, 1);                       // $146446 asl b
                    Xm[r1-- & 0xFF] = A1(a);               //        a,x:(r1)-
                    b = macr(b, x1, y1);                   // $146447 macr y1,x1,b
                    x0 = Xm[r3 & 0xFF];                    //        x:(r3)
                    y0 = yld();                            //        y:(r5)+n5 (last 4 iters: +n5=+1 per listing $14644b+)
                    // NOTE: from $14644b on the wheel step becomes +1
                    // (y:(r5)+ without n5).  Track the step switch:
                    b = mpy_s(x0, neg24(y0));              // $146449 mpy -y0,x0,b
                    Xm[r2-- & 0xFF] = A1(b);               //        b,x:(r2)-
                    b = asl56(b, 1);                       // $14644a asl b
                    b = macr(b, x1, y1);                   // $14644b macr y1,x1,b
                    x0 = Xm[r1++ & 0xFF];                  //        x:(r1)+
                    y0 = yld();
                    a = mpy_s(x0, neg24(y0));              // $14644d/450 chain
                    x0 = Xm[r1-- & 0xFF];
                    y0 = yld();
                    a = mac_s(a, x0, neg24(y0));
                    Xm[r1++ & 0xFF] = x0;
                    y1 = Ym[r4++ & 0xFF];
                    a = asl56(a, 1);
                    a = macr(a, x1, y1);
                    x0 = Xm[r2++ & 0xFF];
                    y0 = yld();
                    b = mac_s(b, x0, y0);                  // $146452 mac y0,x0,b
                    a = mpy_s(x0, neg24(y0));              // $146453 mpy -y0,x0,a
                    Xm[r1-- & 0xFF] = A1(a);               //        a,x:(r1)-
                    y0 = yld();
                    b = mac_s(b, x0, y0);                  // $146454 mac y0,x0,b
                    x0 = Xm[r2-- & 0xFF];
                    y0 = yld();
                    a = mac_s(a, x0, neg24(y0));           // $146455 mac -y0,x0,a
                    Xm[r2++ & 0xFF] = x0;                  //        x0,x:(r2)+
                    a = asl56(a, 1);                       // $146456 asl a
                    a = macr(a, x1, y1);                   // $146457 macr y1,x1,a
                    b = mac_s(b, x0, y0);                  // $146458 mac y0,x0,b
                    x0 = Xm[r3 & 0xFF];                    //        x:(r3)
                    y0 = yld();
                    a = mpy_s(x0, neg24(y0));              // $146459 mpy -y0,x0,a
                    Xm[r2-- & 0xFF] = A1(a);               //        a,x:(r2)-
                    a = asl56(a, 1);                       // $14645a asl a
                    a = macr(a, x1, y1);                   // $14645b macr y1,x1,a
                    b = mac_s(b, x0, y0);                  // $14645c mac y0,x0,b
                    (void)j2;
                }
                b = asl56(b, 6);                           // $14645d asl #$6,b
                Xm[r3 & 0xFF] = A1(a);                     // $14645f a,x:(r3)
                out32[2 * k] = A1(b);                      //        b,y:(r7)+
                out32[2 * k + 1] = A1(b);                  //        b,y:(r7)+
            }
        }
    }

    static int64_t abs56(int64_t a) {
        if (a & (1ll << 55)) a = wrap56(-a);
        return a;
    }


};

} // namespace sid
} // namespace mnm

