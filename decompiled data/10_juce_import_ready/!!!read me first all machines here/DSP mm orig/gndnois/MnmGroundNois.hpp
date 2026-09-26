// =============================================================================
// MnmGroundNois.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m2 "GND-NOIS" — literal transcription of DSP1 P:$144D9A-$144E79.
// =============================================================================
// Firmware provenance (listing 01_GND-NOIS_full.txt; vectors exp57):
//
//   INIT $144D9A: Y:(r6+$10)=$A, Y:(r6+$11)=$1   (Fibonacci seeds)
//   PROC $144DA2, one 16-sample frame:
//
//   1) Lagged-Fibonacci noise, 24-bit wraparound (do #$10, 2 values/iter):
//        v[2k]   = (B + X) & $FFFFFF         ; add x1,b ; store b1
//        v[2k+1] = (Xold + v[2k]) & $FFFFFF  ; add x1,a ; store a1
//        state: B <- v[2k] (via x0), X <- v[2k+1] (via a1)
//        -> stored to X:$06-$25; after frame Y:($10)=v31, Y:($11)=v30
//   2) Coefficient table copy (r2=$144E7A LINEAR, r4 mod 6, 7 loads with
//      parallel old-store): Y:$00-$05 = [t6,t1,t2,t3,t4,t5] where
//      t0..t6 = $FF7C8F $05E4F1 $F4D218 $05D05A $10B5C7 $BF728F $4FD6AE
//   3) 48-bit state staging: X:$00-$05 = [$14,$13,$12] (X-half then Y-half),
//      X:$26-$2B = [$17,$16,$15]
//   4) 16-tap ladder loop (do #$10): 6 cyclic coefficients (r4 mod 6 over
//      Y:$00-$05), taps walk X:$00+ (r0, +2/iter) and X:$26+ (r1, +2/iter,
//      n1=-5 write-back); outputs asl #2 -> new states written to X:$26-$45
//   5) State restore r6+$12..$17 from the walk-end positions
//   6) Resonator (do #$10): a/b cross-pair over X:$06-$25 noise,
//      a += 4*y1 (x0=2 raw <<1), a += x1*a_prev (x1=$FFFCB9), out X:$4C-$6B
//   7) RED gain law (Y:$5):
//        2R <= $7FFFFF: x1=(($7FFFFF-2R)>>2), y0=2R, y1=0        (dry->ladder)
//        else:          x1=0, y0=$800000*4-2R... exact: b=2R-$800000<<1,
//                       y1=b, y0=$7FFFFF-b                        (ladder->res)
//   8) Matrix mix (do #$10): out = x1*s[k] + y0*ladder[k] (+cross taps)
//      + y1*res[k] -> X:$80-$9F
//   9) ST pan (Y:$4): gain pair g0 = 64 + (ST*64*2>>24), g1 = 64 - (...),
//      STON (Y:$6) >= 0.5 selects the second mix law; 16x2 words to Y:(r7)+
//
//   Knobs (voice page Y:(r6+$04..)): $04 = ST, $05 = RED, $06 = STON.
//   Output: 32 words = 16 samples x (L,R).
// =============================================================================

#pragma once
#include <cstdint>
#include <cstring>

namespace mnm {
namespace gn {

static const uint32_t M24 = 0xFFFFFFu;

inline int64_t s24x(int64_t v) {
    v &= M24;
    return (v ^ 0x800000) - 0x800000;
}

// MAC in 56-bit accumulator semantics: acc + sext(x)*sext(y)*2, wrapped 56-bit.
// ('mac' is SIGNED x SIGNED on the DSP56300 — sign-extend both operands.)
inline int64_t mac56(int64_t acc, int64_t x, int64_t y) {
    const int64_t M56 = (1ll << 56) - 1;
    int64_t v = acc + s24x(x) * s24x(y) * 2;
    v &= M56;
    if (v & (1ll << 55)) v -= (1ll << 56);
    return v;
}
inline int64_t mpy56(int64_t x, int64_t y) { return mac56(0, x, y); }
inline int64_t asl56(int64_t a, int n) {
    const int64_t M56 = (1ll << 56) - 1;
    int64_t v = (a << n) & M56;
    if (v & (1ll << 55)) v -= (1ll << 56);
    return v;
}
inline int64_t asr56(int64_t a, int n) {
    const int64_t M56 = (1ll << 56) - 1;
    int64_t v = (a >> n) & M56;                      // a is sign-extended int64
    if (v & (1ll << 55)) v -= (1ll << 56);
    return v;
}
inline uint32_t A1(int64_t a) { return uint32_t((a >> 24) & M24); }

class GroundNois {
public:
    // knobs as raw Q23 param words (value<<16)
    void setKnobs(uint32_t st, uint32_t red, uint32_t ston) {
        kST = st & M24; kRED = red & M24; kSTON = ston & M24;
    }

    // One 16-sample frame; out32 = 16 x (L,R) words (24-bit).
    void processFrame(uint32_t* out32) {
        // ---- 1. Fibonacci noise (P:$144DA2-$144DB3) -----------------------
        // NOTE the firmware's crossed cell semantics: the state cells are
        // STORED as s10 = x1 (last odd), s11 = b1 (last even), but LOADED as
        // b_acc = s10, x1 = s11 — roles swap at every frame boundary. This
        // makes each odd element lag: v[2k+1] = v[2k] + v[2k-2] at frame
        // starts (verified against the emulator register trace).
        uint32_t bacc24 = fibS10, Xr = fibS11;
        int r1 = 0x06;
        for (int k = 0; k < 16; ++k) {
            // DAA: add x1,b  x1,x0     : b_acc += x1 (24-bit wrap via b1)
            uint32_t bn = (bacc24 + Xr) & M24;
            // DAB: move x0,a1          : a1 = OLD x1
            uint32_t a1 = Xr;
            // DAC: move b1,x1          : x1 = bn
            Xr = bn;
            // DAD: store b1            (v[2k])
            Xm[r1++ & 0xFF] = bn;
            // DAE: add x1,a            : a1 = OLD x1 + bn
            uint32_t an = (a1 + Xr) & M24;
            // DAF: move x0,b1          : b1 = bn
            bacc24 = bn;
            // DB0: move a1,x1          : x1 = an
            Xr = an;
            // DB1: store a1            (v[2k+1])
            Xm[r1++ & 0xFF] = an;
        }
        fibS10 = Xr;                                  // DB2: Y:($10) = x1 (an)
        fibS11 = bacc24;                              // DB3: Y:($11) = b1 (bn)

        // ---- 2. coefficient table copy (P:$144DB4-$144DC5) ----------------
        // m4=$6 => circular buffer length m+1 = 7 (Y:$00-$06); 7 stores fit
        // without wrap (verified against emulator: Y:00-06 = t0..t6 linear).
        static const uint32_t tbl[7] = {
            0xFF7C8F, 0x05E4F1, 0xF4D218, 0x05D05A, 0x10B5C7, 0xBF728F, 0x4FD6AE
        };
        uint32_t areg = tbl[0], breg = tbl[1];
        int r4 = 0;
        auto stY = [&](uint32_t v) { Ym[r4 & 0xFF] = v; r4 = (r4 + 1) % 7; };
        areg = tbl[2]; stY(tbl[0]);
        breg = tbl[3]; stY(tbl[1]);
        areg = tbl[4]; stY(tbl[2]);
        breg = tbl[5]; stY(tbl[3]);
        areg = tbl[6]; stY(tbl[4]);
        stY(tbl[5]);
        stY(tbl[6]);                                  // Y:06 = t6
        // final: Y:00=t0, Y:01=t1, ... Y:06=t6

        // ---- 3. state staging (P:$144DC6-$144DE4) -------------------------
        Xm[0x00] = st14x; Xm[0x01] = st14y;
        Xm[0x02] = st13x; Xm[0x03] = st13y;
        Xm[0x04] = st12x; Xm[0x05] = st12y;
        Xm[0x26] = st17x; Xm[0x27] = st17y;
        Xm[0x28] = st16x; Xm[0x29] = st16y;
        Xm[0x2A] = st15x; Xm[0x2B] = st15y;

        // ---- 4. ladder loop (P:$144DE6-$144DFA) ---------------------------
        int r0 = 0x00, rr1 = 0x26;
        int n0 = -5, n1 = -5;
        uint32_t x0 = Xm[r0++ & 0xFF];
        uint32_t y0 = Ym[r4]; r4 = (r4 + 1) % 7;
        int64_t aacc = 0, bacc = 0;
        for (int k = 0; k < 16; ++k) {
            // DE9: mpy y0,x0,a  x:(r0)+,x0
            aacc = mpy56(y0, x0); x0 = Xm[r0++ & 0xFF];
            // DEA: mpy y0,x0,b  x:(r0)+,x0  y:(r4)+,y0
            bacc = mpy56(y0, x0); x0 = Xm[r0++ & 0xFF];
            y0 = Ym[r4]; r4 = (r4 + 1) % 7;
            // DEB: mac y0,x0,a  x:(r0)+,x0
            aacc = mac56(aacc, y0, x0); x0 = Xm[r0++ & 0xFF];
            // DEC: mac y0,x0,b  x:(r0)+,x0  y:(r4)+,y0
            bacc = mac56(bacc, y0, x0); x0 = Xm[r0++ & 0xFF];
            y0 = Ym[r4]; r4 = (r4 + 1) % 7;
            // DED: mac y0,x0,a  x:(r0)+,x0
            aacc = mac56(aacc, y0, x0); x0 = Xm[r0++ & 0xFF];
            // DEE: mac y0,x0,b  x:(r0)+,x0  y:(r4)+,y0
            bacc = mac56(bacc, y0, x0); x0 = Xm[r0++ & 0xFF];
            y0 = Ym[r4]; r4 = (r4 + 1) % 7;
            // DEF: mac y0,x0,a  x:(r0)+n0,x0
            aacc = mac56(aacc, y0, x0); x0 = Xm[r0 & 0xFF]; r0 += n0;
            // DF0: mac y0,x0,b  x:(r1)+,x0  y:(r4)+,y0
            bacc = mac56(bacc, y0, x0); x0 = Xm[rr1++ & 0xFF];
            y0 = Ym[r4]; r4 = (r4 + 1) % 7;
            // DF1: mac y0,x0,a  x:(r1)+,x0
            aacc = mac56(aacc, y0, x0); x0 = Xm[rr1++ & 0xFF];
            // DF2: mac y0,x0,b  x:(r1)+,x0  y:(r4)+,y0
            bacc = mac56(bacc, y0, x0); x0 = Xm[rr1++ & 0xFF];
            y0 = Ym[r4]; r4 = (r4 + 1) % 7;
            // DF3: mac y0,x0,a  x:(r1)+,x0
            aacc = mac56(aacc, y0, x0); x0 = Xm[rr1++ & 0xFF];
            // DF4: mac y0,x0,b  x:(r1)+,x0  y:(r4)+,y0
            bacc = mac56(bacc, y0, x0); x0 = Xm[rr1++ & 0xFF];
            y0 = Ym[r4]; r4 = (r4 + 1) % 7;
            // DF5: mac y0,x0,a  x:(r1)+,x0
            aacc = mac56(aacc, y0, x0); x0 = Xm[rr1++ & 0xFF];
            // DF6: mac y0,x0,b  x:(r0)+,x0  y:(r4)+,y0
            bacc = mac56(bacc, y0, x0); x0 = Xm[r0++ & 0xFF];
            y0 = Ym[r4]; r4 = (r4 + 1) % 7;
            // DF7/8: asl #$2,a / asl #$2,b
            aacc = asl56(aacc, 2);
            bacc = asl56(bacc, 2);
            // DF9: move a,x:(r1)+    DFA: move b,x:(r1)+n1
            Xm[rr1++ & 0xFF] = A1(aacc);
            Xm[rr1 & 0xFF] = A1(bacc); rr1 += n1;
        }
        // ---- 5. state restore (P:$144DFB-$144E11) --------------------------
        // $14.X = the ladder's final x0 register; $14.Y = X:(r0)+,
        // $13 = the next two reads, $12 = the next two (5 reads, r0 += 5);
        // $17/$16/$15 = three x1/x0 pairs from X:(r1)+ (6 reads).
        uint32_t x1r = Xm[r0++ & 0xFF];
        st14x = x0;            st14y = x1r;
        x1r = Xm[r0++ & 0xFF]; x0 = Xm[r0++ & 0xFF];
        st13x = x1r;           st13y = x0;
        x1r = Xm[r0++ & 0xFF]; x0 = Xm[r0++ & 0xFF];
        st12x = x1r;           st12y = x0;
        x1r = Xm[rr1++ & 0xFF]; x0 = Xm[rr1++ & 0xFF];
        st17x = x1r;         st17y = x0;
        x1r = Xm[rr1++ & 0xFF]; x0 = Xm[rr1++ & 0xFF];
        st16x = x1r;         st16y = x0;
        x1r = Xm[rr1++ & 0xFF]; x0 = Xm[rr1++ & 0xFF];
        st15x = x1r;         st15y = x0;

        // ---- 6. resonator (P:$144E14-$144E25) ------------------------------
        int rq0 = 0x06, rq1 = 0x4C;
        aacc = (int64_t)(resAx) << 24;                // a = X:($18) word
        bacc = (int64_t)(resBy) << 24;                // b = Y:($18) word
        uint32_t rx0 = 0x020000;                      // move #$2,x0: 8-bit imm -> $020000 (0.25)
        uint32_t rx1 = 0xFFFCB9;                      // -0.001026
        uint32_t ry1 = Xm[rq0++ & 0xFF];
        for (int k = 0; k < 16; ++k) {
            // E20: mac x0,y1,a  a,x:(r1)+  a,y0     (stores OLD a)
            uint32_t aOld = A1(aacc);
            aacc = mac56(aacc, rx0, ry1);
            Xm[rq1++ & 0xFF] = aOld;
            y0 = aOld;
            // E21: mac x1,y0,a  x:(r0)+,y1
            aacc = mac56(aacc, rx1, y0);
            ry1 = Xm[rq0++ & 0xFF];
            // E22: mac x0,y1,b  b,x:(r1)+  b,y0
            uint32_t bOld = A1(bacc);
            bacc = mac56(bacc, rx0, ry1);
            Xm[rq1++ & 0xFF] = bOld;
            y0 = bOld;
            // E23: mac x1,y0,b  x:(r0)+,y1
            bacc = mac56(bacc, rx1, y0);
            ry1 = Xm[rq0++ & 0xFF];
        }
        resAx = A1(aacc); resBy = A1(bacc);

        // ---- 7. RED gain law (P:$144E26-$144E3D) ---------------------------
        // b = RED ; a = $7FFFFF - 2*RED ; if a >= 0: x1=a>>2, y0=(2*RED),
        //                                     y1=0
        // else: b = (RED-$400000)<<1 ; y1 = b ; a = $7FFFFF-b ; y0 = a ; x1=0
        uint32_t gX1, gY0, gY1;
        {
            int64_t bb = (int64_t)kRED;
            int64_t aa = 0x7FFFFF - 2 * bb;           // two `sub b,a`
            if (aa >= 0) {
                int64_t a2 = aa >> 2;                 // asr #$2,a,a
                int64_t b2 = asl56(bb << 24, 1);      // asl b (on acc)
                gX1 = A1(a2 << 24);
                gY0 = A1(b2);
                gY1 = 0;
            } else {
                int64_t b2 = (bb - 0x400000) << 24;   // sub #$400000,b
                b2 = asl56(b2, 1);                    // asl b
                int64_t a2 = (int64_t)0x7FFFFF << 24;
                a2 = a2 - b2;                         // sub b,a
                gX1 = 0;
                gY1 = A1(b2);
                gY0 = A1(a2);
            }
        }
        x1g = gX1; y0g = gY0; y1g = gY1;

        // ---- 8. matrix mix (P:$144E3D-$144E50) -----------------------------
        int rm0 = 0x06, rm1 = 0x2C, rm2 = 0x4C, rm3 = 0x80;
        x0 = Xm[rm0++ & 0xFF];
        for (int k = 0; k < 16; ++k) {
            // E48: mpy x1,x0,a  x:(r0)+,x0
            aacc = mpy56(x1g, x0); x0 = Xm[rm0++ & 0xFF];
            // E49: mpy x1,x0,b  x:(r1)+,x0
            bacc = mpy56(x1g, x0); x0 = Xm[rm1++ & 0xFF];
            // E4A: mac y0,x0,a  x:(r1)+,x0
            aacc = mac56(aacc, gY0, x0); x0 = Xm[rm1++ & 0xFF];
            // E4B: mac y0,x0,b  x:(r2)+,x0
            bacc = mac56(bacc, gY0, x0); x0 = Xm[rm2++ & 0xFF];
            // E4C: mac x0,y1,a  x:(r2)+,x0
            aacc = mac56(aacc, x0, gY1); x0 = Xm[rm2++ & 0xFF];
            // E4D: mac x0,y1,b  x:(r0)+,x0
            bacc = mac56(bacc, x0, gY1); x0 = Xm[rm0++ & 0xFF];
            // E4E/E4F: store a,b
            Xm[rm3++ & 0xFF] = A1(aacc);
            Xm[rm3++ & 0xFF] = A1(bacc);
        }

        // ---- 9. ST pan + STON law -> output (P:$144E50-$144E71) ------------
        int ro1 = 0x80;
        uint32_t px0 = 0x400000;                      // move #$40,x0: 8-bit imm -> $400000 (0.5)
        uint32_t py0 = kST;
        aacc = (int64_t)px0 << 24;                    // a = 0.5
        bacc = (int64_t)px0 << 24;                    // b = 0.5
        aacc = mac56(aacc, py0, px0);                 // a = 0.5 + ST*0.5*2 = 0.5 + ST
        bacc = mac56(bacc, (uint32_t)(-(int64_t)py0) & M24, px0);  // b = 0.5 - ST
        uint32_t pg0 = A1(aacc), pg1 = A1(bacc);
        x0 = Xm[ro1++ & 0xFF];
        if (kSTON >= 0x400000) {
            // E5F branch: do #$10 (E61-E66)
            for (int k = 0; k < 16; ++k) {
                // E61: mpy y0,x0,a  x:(r1)+,x1
                aacc = mpy56(pg0, x0); x1r_ = Xm[ro1++ & 0xFF];
                // E62: mac y1,x1,a
                aacc = mac56(aacc, pg1, x1r_);
                // E63: mpy x0,y1,b
                bacc = mpy56(x0, pg1);
                // E64: mac x1,y0,b  x:(r1)+,x0
                bacc = mac56(bacc, x1r_, pg0); x0 = Xm[ro1++ & 0xFF];
                out32[2 * k]     = A1(aacc);
                out32[2 * k + 1] = A1(bacc);
            }
        } else {
            // E69 branch: do #$10 (E6B-E70)
            for (int k = 0; k < 16; ++k) {
                // E6B: mpy y0,x0,a  x:(r1)+,x1
                aacc = mpy56(pg0, x0); x1r_ = Xm[ro1++ & 0xFF];
                // E6C: mac y1,x1,a
                aacc = mac56(aacc, pg1, x1r_);
                // E6D: mpy y0,x0,b
                bacc = mpy56(pg0, x0);
                // E6E: mac y1,x1,b  x:(r1)+,x0
                bacc = mac56(bacc, pg1, x1r_); x0 = Xm[ro1++ & 0xFF];
                out32[2 * k]     = A1(aacc);
                out32[2 * k + 1] = A1(bacc);
            }
        }
    }

    // state (mirrors voice-page cells)
    uint32_t fibS10 = 0x00000A, fibS11 = 0x000001;   // INIT: Y:($10)=$A, Y:($11)=$1
    uint32_t st12x = 0, st12y = 0, st13x = 0, st13y = 0, st14x = 0, st14y = 0;
    uint32_t st15x = 0, st15y = 0, st16x = 0, st16y = 0, st17x = 0, st17y = 0;
    uint32_t resAx = 0, resBy = 0;               // X/Y:($18)
    uint32_t Xm[256] = {0}, Ym[256] = {0};       // scratch mirror

private:
    uint32_t kST = 0, kRED = 0, kSTON = 0;
    uint32_t x1g = 0, y0g = 0, y1g = 0;
    uint32_t x0 = 0, y0 = 0;
    uint32_t x1r_ = 0;
};

} // namespace gn
} // namespace mnm
