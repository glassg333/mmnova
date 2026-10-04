// =============================================================================
// MnmFmDyn.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m10 "FM+ DYN" — bit-exact transcription of DSP1 P:$14619D (INIT),
// P:$1461A8 (CONF), P:$1461C1-$14636E (PROC).
//
// Transcribed instruction-by-instruction from
// decompiled data/03_listings/machines_page_A/10_FM-DYN_full.txt
// (the real m10 range ends at $14636E; $14636F+ is machine m3 SID — dispatch
// table $10016B/$10018D/$1001AF, verified in worklog iterations 27-28) and
// verified against scripts/dsp_emu.py running the original firmware
// (research/fm_dyn_vectors.txt: 16 knob sets x 16 blocks, output + state).
//
// Topology (two modulators, "dynamic" continuous ratios):
//   mod 1: sine($14A000) -> interp -> diff -> LP(fixed $400000) ->
//          peak shaper cur + 32*G*prev (G from gated 1VOL + $2C recursion);
//          its phase increment = 4*ratio1*(carrier_inc + ($2E>>12)),
//          ratio1 = 1FRQ word clamped to $7FFFFF, $2E = 1FEN table recursion.
//   mod 2: NO interpolation — raw LUT reads; the 2FB feedback wobbles the
//          read address (one-sample FM term, state $1E/$1F); then diff ->
//          LP($400000) -> level2 (2ENV droop state $2D) into the mix.
//   mix (level1*mod1 + level2*mod2) -> LP($400000, state $29) ->
//          carrier phase FM (mix >> 12 per sample).
//   carrier inc = L:$5 = 2*$0BE37C*pitch (NO 1FEN offset — that goes into
//   mod 1's increment only). Interp >> 5 -> X:$E0-$FF; 2 quadrature rotators;
//   output = 16 pair-sums of rot-2 (x0.5) each >>1, duplicated a,a/b,b.
// =============================================================================
#pragma once
#include "Import4FmDsp.hpp"

namespace mnmfrqenvfm {

class MnmFmDyn {
public:
    // Optional per-stage debug hook (see research stage-diff tooling).
    void (*stageHook)(void* ud, int stage) = nullptr;
    void* stageUser = nullptr;
    inline void hook(int s) { if (stageHook) stageHook(stageUser, s); }
    // ---- INIT P:$14619D --------------------------------------------------------
    // lua (r6+$11),r0 ; move #>$14a000,x0 ; move #$0,x1 ;
    // do #<$1a move x1,y:(r0)+ ; x0 -> y:(r6+$10/$14/$18)
    void init() {
        for (int i = 0; i < 0x100; ++i) { mem.X[i] = 0; mem.Y[i] = 0; }
        for (int i = 0; i < 0x40; ++i) mem.st[i] = 0;
        mem.resetRegs();
        mem.R[0] = R6BASE + 0x11;              // lua (r6+$11),r0
        x1 = 0;                                 // move #$0,x1
        for (int i = 0; i < 26; ++i) {          // do #<$1a ; move x1,y:(r0)+
            mem.wry(mem.R[0], x1);
            mem.updR(0, +1);
        }
        x0 = 0x14A000u;                         // move #>$14a000,x0
        mem.wry(R6BASE + 0x10, x0);             // move x0,y:(r6+$10)
        mem.wry(R6BASE + 0x14, x0);             // move x0,y:(r6+$14)
        mem.wry(R6BASE + 0x18, x0);             // move x0,y:(r6+$18)
        A = 0; B = 0;
    }

    // ---- CONF P:$1461A8-$1461C0 -------------------------------------------------
    void conf(const uint32_t knob[8]) {
        for (int i = 0; i < 8; ++i)
            mem.st[0x04 + i] = (knob[i] & 0xFFFF) << 16;
        mem.st[0x2D] = 0x7FFFFF;                // $1461a8/$1461aa: a -> y:(r6+$2d)
        // $1461ab: a = y:(r6+$7) (1VEN) ; $1461ac: sub #>$400000,a
        A = alu_sub(mem2acc(mem.st[0x07]), al24(0x400000));
        uint32_t x0 = mem.st[0x06];             // $1461ae move y:(r6+$6),x0 (1VOL)
        uint32_t x1 = acc24sat(A);              // $1461af move a,x1
        int64_t B = mpy_ss(x1, x0);             // $1461b0 mpy x1,x0,b
        bool mi = (A < 0);                      // $1461b1 tst a (n flag)
        x0 = acc24sat(B);                       // $1461b2 move b,x0
        if (mi) A = al24(x0);                   // $1461b3 tfr x0,a  ifmi
        A = acc_asl(A, 1);                      // $1461b4 asl a
        A = acc_asl(A, 1);                      // $1461b5 asl a
        B = mem2acc(mem.st[0x05]);              // $1461b6 move y:(r6+$5),b (1FEN)
        mem.st[0x2C] = acc24sat(A);             // $1461b7 move a,y:(r6+$2c)
        B = alu_sub(B, al24(0x400000));         // $1461b8 sub #>$400000,b
        if (B < 0)                              // $1461ba asr b  ifmi
            B = acc_asr(B, 1);
        uint32_t y0v = acc24sat(B);             // $1461bb move b,y0
        B = acc_abs(B);                         // $1461bc abs b
        uint32_t y1v = acc24sat(B);             // $1461bd move b,y1
        B = mpy_ss(y1v, y0v);                   // $1461be mpy y1,y0,b
        mem.st[0x2E] = acc24sat(B);             // $1461bf move b,y:(r6+$2e)
    }

    // ---- PROC P:$1461C1-$14636E -------------------------------------------------
    void proc(int64_t pitchA, uint32_t out[32]) {
        int oidx = 0;
        A = pitchA;                              // 48-bit acc (kernel convention)
        // ---- carrier inc48 = 2 * $0BE37C * A -> L:$5 ----------------------------
        x0 = 0x0BE37Cu;                         // $1461c1
        y1 = acc24raw(A);                       // $1461c3
        y0 = (uint32_t)(A & M24);               // $1461c4
        A = mpsu(x0, y0);                       // $1461c5
        A = mac_ss(x0, y1, A);                  // $1461c6
        mem.wrL(5, A & M48);                    // $1461c8 move a,l:?:>$5
        mem.M[2] = 0x1FFFu;                     // $1461ca
        mem.M[0] = 0x1FFFu;                     // $1461cc
        mem.R[1] = 0x141880u;                   // $1461ce move #>$141880,r1
        // ---- 1FEN recursion ($2E) + FM offset into mod-1 increment --------------
        B = mem2acc(mem.rdy(R6BASE + 0x05));    // $1461d0 move y:(r6+$5),b (1FEN)
        A = mem2acc(0x80);                      // $1461d1 move #>$80,a (long: raw $80)
        B = alu_sub(B, al24(0x400000));         // $1461d3 sub #>$400000,b
        B = acc_asr(B, 15);                     // $1461d5 asr #$f,b,b
        B = acc_abs(B);                         // $1461d6 abs b
        A = alu_sub(A, B);                      // $1461d7 sub b,a
        mem.N[1] = (int32_t)sgn24(acc24raw(A)); // $1461d8 move a,n1
        x0 = mem.rdy(R6BASE + 0x2E);            // $1461d9 move y:(r6+$2e),x0
        x1 = mem.rdy(mem.R[1] + (uint32_t)mem.N[1]); // $1461da move y:(r1+n1),x1
        B = mpy_ss_neg(x1, x0);                 // $1461db mpy -x1,x0,b
        mem.wry(R6BASE + 0x2E, acc24sat(B));    // $1461dc move b,y:(r6+$2e)
        B = acc_asr(B, 12);                     // $1461dd asr #$c,b,b
        A = sext48(mem.rdL(5));                 // $1461de move l:?:>$5,a
        A = alu_add(A, B);                      // $1461e0 add b,a
        // ---- mod-1 ratio (continuous, clamped) ----------------------------------
        x0 = 0x7FFFFFu;                         // $1461e1 move #>$7fffff,x0
        B = mem2acc(mem.rdy(R6BASE + 0x04));    // $1461e3 move y:(r6+$4),b (1FRQ)
        if (B > al24(0x7EFFFF))                 // $1461e4 cmp #>$7effff,b ; $1461e6 tfr x0,b ifgt
            B = al24(x0);
        x0 = acc24sat(B);                       // $1461e7 move b,x0
        y1 = acc24raw(A);                       // $1461e8 move a1,y1
        y0 = (uint32_t)(A & M24);               // $1461e9 move a0,y0
        A = mpsu(x0, y0);                       // $1461ea mpysu x0,y0,a
        A = mac_ss(x0, y1, A);                  // $1461eb dmac ss x0,y1,a
        A = acc_asl(A, 1);                      // $1461ec asl #$1,a,a
        // ---- mod-1 glide ramp ----------------------------------------------------
        B = mem2acc(mem.rdy(R6BASE + 0x12));    // $1461ed
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x13); // $1461ee
        mem.wry(R6BASE + 0x12, acc24raw(A));    // $1461ef
        mem.wry(R6BASE + 0x13, (uint32_t)(A & M24)); // $1461f0
        A = alu_sub(A, B);                      // $1461f1
        A = acc_asr(A, 5);                      // $1461f2
        mem.R[1] = 0x20u;                       // $1461f3
        y1 = acc24raw(A);                       // $1461f5
        y0 = (uint32_t)(A & M24);               // $1461f6
        A = B;                                  // $1461f7 tfr b,a
        B = mem2acc(mem.rdy(R6BASE + 0x10));    // $1461f8
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x11); // $1461f9
        x0 = 0x1FFFu;                           // $1461fa
        x1 = 0x14A000u;                         // $1461fc
        for (int i = 0; i < 32; ++i) {          // $1461fe do #$20 — $146200-$146203
            A = alu_add(A, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // add y,a
            {   int64_t Bold = B;               // add a,b  b,l:(r1)+
                mem.wrL(mem.R[1], Bold & M48);
                mem.updR(1, +1);
                B = alu_add(B, A);
            }
            B = acc_and(B, x0);                 // and x0,b
            B = alu_add(B, al24(x1));           // add x1,b
        }
        mem.wry(R6BASE + 0x10, acc24raw(B));    // $146205 move b1,y:(r6+$10)
        mem.wry(R6BASE + 0x11, (uint32_t)(B & M24)); // $146206 move b0,y:(r6+$11)
        hook(1);
        // ---- mod-1 interpolation -> Y:$80-$9F ------------------------------------
        mem.R[1] = 0x20u;                       // $146207
        mem.R[5] = 0x80u;                       // $146209
        mem.R[4] = mem.R[1];                    // $14620c
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // $14620f
        y0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $146210
        x1 = mem.rdx(mem.R[2]); mem.updR(2, +1);// $146213
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // $146214
        for (int i = 0; i < 16; ++i) {          // $146215 do #$10 — $146217-$146222
            A = mpsu_neg(x1, y0);               // $146217
            {   uint32_t t = mem.rdx(mem.R[2]); // $146218 add x1,a  x:(r2),x0
                x0 = t;
            }
            A = alu_add(A, al24(x1));
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // $146219
            A = alu_add(A, al24(x1));
            A = mcsu(x0, y0, A);                // $14621a
            {   int64_t Aold = A;               // $14621b asr a  x:(r0)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = acc_asr(Aold, 1);
                x1 = tx1; y0 = ty0;
            }
            B = mpsu_neg(x1, y0);               // $14621c
            {   uint32_t t = mem.rdx(mem.R[0]); // $14621d add x1,b  x:(r0),x0
                x0 = t;
            }
            B = alu_add(B, al24(x1));
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // $14621e
            B = alu_add(B, al24(x1));
            B = mcsu(x0, y0, B);                // $14621f
            {   int64_t Bold = B;               // $146220 asr b  x:(r2)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = acc_asr(Bold, 1);
                x1 = tx1; y0 = ty0;
            }
            mem.wry(mem.R[5], acc24sat(A)); mem.updR(5, +1); // $146221
            mem.wry(mem.R[5], acc24sat(B)); mem.updR(5, +1); // $146222
        }
        hook(2);
        // ---- mod-1 difference (in-place Y:$80, state $1c) ------------------------
        mem.R[4] = 0x80u;                       // $146223
        mem.R[5] = 0x80u;                       // $146225
        x0 = mem.rdy(R6BASE + 0x1C);            // $146227
        for (int i = 0; i < 32; ++i) {          // $146228 do #$20 — $14622a-$14622c
            {   uint32_t t = mem.rdy(mem.R[5]); mem.updR(5, +1); // $14622a move y:(r5)+,b
                B = mem2acc(t);
            }
            {   uint32_t tb = acc24sat(B);      // $14622b sub x0,b  b,x0
                B = alu_sub(B, al24(x0));
                x0 = tb;
            }
            mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $14622c move b,y:(r4)+
        }
        mem.wry(R6BASE + 0x1C, x0);             // $14622d
        hook(3);
        // ---- mod-1 LP (fixed coeff $400000, state $2a) ---------------------------
        A = mem2acc(mem.rdy(R6BASE + 0x2A));    // $14622e
        mem.R[5] = 0x80u;                       // $14622f
        mem.R[4] = 0x80u;                       // $146231
        y0 = 0x400000u;                         // $146233 move #$40,y0
        x0 = mem.rdy(mem.R[5]); mem.updR(5, +1);// $146234 move y:(r5)+,x0
        for (int i = 0; i < 32; ++i) {          // $146235 do #$20 — $146237-$146238
            {   int64_t Aold = A;               // $146237 mac y0,x0,a  a,x1  a,y:(r4)+
                mem.wry(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                x1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdy(mem.R[5]); mem.updR(5, +1); // $146238 mac -x1,y0,a  y:(r5)+,x0
                A = mac_ss_neg(x1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + 0x2A, acc24sat(A));    // $146239
        hook(4);
        // ---- 1VEN recursion ($2c) -------------------------------------------------
        mem.R[1] = 0x141880u;                   // $14623a
        B = mem2acc(mem.rdy(R6BASE + 0x07));    // $14623c (1VEN)
        A = mem2acc(0x80);                      // $14623d
        B = alu_sub(B, al24(0x400000));         // $14623f
        B = acc_asr(B, 15);                     // $146241
        B = acc_abs(B);                         // $146242
        A = alu_sub(A, B);                      // $146243
        mem.N[1] = (int32_t)sgn24(acc24raw(A)); // $146244
        x0 = mem.rdy(R6BASE + 0x2C);            // $146245
        x1 = mem.rdy(mem.R[1] + (uint32_t)mem.N[1]); // $146246
        A = mpy_ss_neg(x1, x0);                 // $146247 mpy -x1,x0,a
        mem.wry(R6BASE + 0x2C, acc24sat(A));    // $146248
        // ---- mod-1 shaper gain: gate(1VOL + $2C - 64) -----------------------------
        B = mem2acc(mem.rdy(R6BASE + 0x06));    // $146249 (1VOL)
        B = alu_add(B, A);                      // $14624a add a,b
        A = 0;                                  // $14624b clr a
        B = alu_sub(B, al24(0x400000));         // $14624c sub #>$400000,b
        if (B >= 0)                             // $14624e tst b ; $14624f tfr b,a ifpl
            A = B;
        y1 = acc24sat(A);                       // $146250 move a,y1
        // ---- mod-1 peak shaper (state $20, in-place Y:$80) ------------------------
        mem.R[0] = 0x80u;                       // $146251
        x0 = mem.rdy(R6BASE + 0x20);            // $146253
        for (int i = 0; i < 32; ++i) {          // $146254 do #$20 — $146256-$146259
            {   int64_t An = mpy_ss(x0, y1);    // $146256 mpy x0,y1,a  y:(r0),x1
                uint32_t tx1 = mem.rdy(mem.R[0]);
                A = An; x1 = tx1;
            }
            A = acc_asl(A, 4);                  // $146257 asl #$4,a,a
            A = alu_add(A, al24(x1)); x0 = x1;  // $146258 add x1,a  x1,x0
            mem.wry(mem.R[0], acc24sat(A)); mem.updR(0, +1); // $146259 move a,y:(r0)+
        }
        mem.wry(R6BASE + 0x20, x0);             // $14625a
        hook(5);
        // ---- mod-2 ratio: quadratic 2FRQ ------------------------------------------
        x0 = 0x7FFFFFu;                         // $14625b
        B = mem2acc(mem.rdy(R6BASE + 0x08));    // $14625d (2FRQ)
        if (B > al24(0x7EFFFF))                 // $14625e cmp ; $146260 tfr x0,b ifgt
            B = al24(x0);
        x0 = acc24sat(B);                       // $146261
        A = sext48(mem.rdL(5));                 // $146262 move l:?:>$5,a
        B = mpy_ss(x0, x0);                     // $146264 mpy x0,x0,b
        y1 = acc24raw(A);                       // $146265
        y0 = (uint32_t)(A & M24);               // $146266
        x0 = acc24sat(B);                       // $146267 move b,x0
        A = mpsu(x0, y0);                       // $146268
        A = mac_ss(x0, y1, A);                  // $146269
        A = acc_asl(A, 2);                      // $14626a asl #$2,a,a
        // ---- mod-2 phase loop with 2FB address wobble -----------------------------
        y1 = acc24raw(A);                       // $14626c
        y0 = (uint32_t)(A & M24);               // $14626d
        B = mem2acc(mem.rdy(R6BASE + 0x14));    // $14626e (prev phase $14/$15)
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x15); // $14626f
        {   int64_t t = mem2acc(mem.rdy(R6BASE + 0x0A)); // $146270 (2FB)
            A = t;
        }
        A = acc_asr(A, 11);                     // $146271 asr #$b,a,a
        mem.R[5] = 0xC0u;                       // $146272
        x0 = acc24sat(A);                       // $146274 move a,x0
        A = mem2acc(mem.rdy(R6BASE + 0x1E));    // $146275 (FM state $1e/$1f)
        A = (A & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x1F); // $146276
        for (int i = 0; i < 32; ++i) {          // $146277 do #$20 — $146279-$146282
            B = alu_add(B, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // $146279 add y,b
            B = alu_add(B, A);                  // $14627a add a,b
            B = acc_and(B, 0x1FFFu);            // $14627b and #>$1fff,b
            B = alu_add(B, al24(0x14A000u));    // $14627d add #>$14a000,b
            mem.R[3] = acc24raw(B);             // $14627f move b1,r3
            B = alu_sub(B, A);                  // $146280 sub a,b
            x1 = mem.rdx(mem.R[3]);             // $146281 move x:(r3),x1
            {   int64_t An = mpy_ss(x1, x0);    // $146282 mpy x1,x0,a  x1,y:(r5)+
                mem.wry(mem.R[5], x1); mem.updR(5, +1);
                A = An;
            }
        }
        mem.wry(R6BASE + 0x14, acc24raw(B));    // $146283
        mem.wry(R6BASE + 0x15, (uint32_t)(B & M24)); // $146284
        mem.wry(R6BASE + 0x1E, acc24raw(A));    // $146285
        mem.wry(R6BASE + 0x1F, (uint32_t)(A & M24)); // $146286
        // ---- mod-2 difference (in-place Y:$C0, state $1d) -------------------------
        mem.R[5] = 0xC0u;                       // $146287
        mem.R[4] = 0xC0u;                       // $146289
        x0 = mem.rdy(R6BASE + 0x1D);            // $14628b
        for (int i = 0; i < 32; ++i) {          // $14628c do #$20 — $14628e-$146290
            {   uint32_t t = mem.rdy(mem.R[5]); mem.updR(5, +1); // $14628e
                B = mem2acc(t);
            }
            {   uint32_t tb = acc24sat(B);      // $14628f sub x0,b  b,x0
                B = alu_sub(B, al24(x0));
                x0 = tb;
            }
            mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $146290
        }
        mem.wry(R6BASE + 0x1D, x0);             // $146291
        // ---- mod-2 LP (fixed coeff, state $2b) -------------------------------------
        A = mem2acc(mem.rdy(R6BASE + 0x2B));    // $146292
        mem.R[5] = 0xC0u;                       // $146293
        mem.R[4] = 0xC0u;                       // $146295
        y0 = 0x400000u;                         // $146297
        x0 = mem.rdy(mem.R[5]); mem.updR(5, +1);// $146298
        for (int i = 0; i < 32; ++i) {          // $146299 do #$20 — $14629b-$14629c
            {   int64_t Aold = A;               // $14629b
                mem.wry(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                x1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdy(mem.R[5]); mem.updR(5, +1); // $14629c
                A = mac_ss_neg(x1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + 0x2B, acc24sat(A));    // $14629d
        hook(6);
        // ---- level 1: (1VOL^2 + $2c) << 2 -> y0 ------------------------------------
        mem.R[0] = 0x80u;                       // $14629e
        mem.R[1] = 0xC0u;                       // $1462a0
        mem.R[2] = 0x80u;                       // $1462a2
        x0 = mem.rdy(R6BASE + 0x06);            // $1462a4 (1VOL)
        A = mpy_ss(x0, x0);                     // $1462a5
        B = mem2acc(mem.rdy(R6BASE + 0x2C));    // $1462a6
        A = alu_add(A, B);                      // $1462a7 add b,a
        A = acc_asl(A, 2);                      // $1462a8
        y0 = acc24sat(A);                       // $1462a9
        // ---- 2ENV droop (state $2d) + level 2 -> y1 --------------------------------
        B = mem2acc(mem.rdy(R6BASE + 0x09));    // $1462aa (2ENV)
        y1 = mem.rdy(R6BASE + 0x2D);            // $1462ab
        if (B >= al24(0x400000)) {              // $1462ac cmp ; $1462ae blt
            B = alu_sub(B, al24(0x400000));     // $1462b0
            A = mem2acc(0x7FFFFF);              // $1462b2
            x0 = acc24sat(B);                   // $1462b4
            B = mpy_ss(x0, x0);                 // $1462b5
            x0 = acc24sat(B);                   // $1462b6
            A = mac_ss_neg(x0, x0, A);          // $1462b7
            x0 = acc24sat(A);                   // $1462b8
            A = mpy_ss(x0, y1);                 // $1462b9 mpy x0,y1,a
            y1 = acc24sat(A);                   // $1462bb
            mem.wry(R6BASE + 0x2D, y1);         // $1462bc
        }
        {   // level 2: y1 = state2 * ((2ENV^2)<<2)
            x0 = mem.rdy(R6BASE + 0x09);        // $1462bd
            A = mpy_ss(x0, x0);                 // $1462be
            A = acc_asl(A, 2);                  // $1462bf
            x0 = acc24sat(A);                   // $1462c0
            A = mpy_ss(x0, y1);                 // $1462c1
            y1 = acc24sat(A);                   // $1462c2
        }
        hook(7);
        // ---- mix: level1*mod1 + level2*mod2 -> X:$80 -------------------------------
        {   uint32_t t = mem.rdy(mem.R[0]); mem.updR(0, +1); // $1462c3
            x0 = t;
        }
        {   uint32_t t = mem.rdy(mem.R[1]); mem.updR(1, +1); // $1462c4
            x1 = t;
        }
        A = mpy_ss(y0, x0);                     // $1462c5
        {   int64_t Bn = mpy_ss(y1, x1);        // $1462c6 mpy y1,x1,b  y:(r0)+,x0
            uint32_t tx0 = mem.rdy(mem.R[0]); mem.updR(0, +1);
            B = Bn; x0 = tx0;
        }
        {   uint32_t tx1 = mem.rdy(mem.R[1]); mem.updR(1, +1); // $1462c7 add a,b  y:(r1)+,x1
            B = alu_add(B, A);
            x1 = tx1;
        }
        for (int i = 0; i < 31; ++i) {          // $1462c8 do #$1f — $1462ca-$1462cc
            {   int64_t Bold = B;               // $1462ca mpy y0,x0,a  b,x:(r2)+
                mem.wrx(mem.R[2], acc24sat(Bold)); mem.updR(2, +1);
                A = mpy_ss(y0, x0);
            }
            {   int64_t Bn = mpy_ss(y1, x1);    // $1462cb mpy y1,x1,b  y:(r0)+,x0
                uint32_t tx0 = mem.rdy(mem.R[0]); mem.updR(0, +1);
                B = Bn; x0 = tx0;
            }
            {   uint32_t tx1 = mem.rdy(mem.R[1]); mem.updR(1, +1); // $1462cc add a,b  y:(r1)+,x1
                B = alu_add(B, A);
                x1 = tx1;
            }
        }
        mem.wrx(mem.R[2], acc24sat(B)); mem.updR(2, +1); // $1462cd
        hook(8);
        // ---- mix LP (fixed coeff, state $29, in-place X:$80) -----------------------
        A = mem2acc(mem.rdy(R6BASE + 0x29));    // $1462ce
        mem.R[5] = 0x80u;                       // $1462cf
        mem.R[4] = 0x80u;                       // $1462d1
        y0 = 0x400000u;                         // $1462d3
        x0 = mem.rdx(mem.R[5]); mem.updR(5, +1);// $1462d4
        for (int i = 0; i < 32; ++i) {          // $1462d5 do #$20 — $1462d7-$1462d8
            {   int64_t Aold = A;               // $1462d7 mac y0,x0,a  a,x:(r4)+  a,y1
                mem.wrx(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                y1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdx(mem.R[5]); mem.updR(5, +1); // $1462d8 mac -y1,y0,a  x:(r5)+,x0
                A = mac_ss_neg(y1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + 0x29, acc24sat(A));    // $1462d9
        hook(9);
        // ---- carrier phase + post-sample FM -----------------------------------------
        A = sext48(mem.rdL(5));                 // $1462da
        B = mem2acc(mem.rdy(R6BASE + 0x1A));    // $1462dc
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x1B); // $1462dd
        mem.wry(R6BASE + 0x1A, acc24raw(A));    // $1462de
        mem.wry(R6BASE + 0x1B, (uint32_t)(A & M24)); // $1462df
        A = alu_sub(A, B);                      // $1462e0
        A = acc_asr(A, 5);                      // $1462e1
        mem.R[1] = 0x20u;                       // $1462e2
        mem.R[0] = 0x80u;                       // $1462e4
        y1 = acc24raw(A);                       // $1462e6
        y0 = (uint32_t)(A & M24);               // $1462e7
        A = B;                                  // $1462e8
        B = mem2acc(mem.rdy(R6BASE + 0x18));    // $1462e9
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x19); // $1462ea
        for (int i = 0; i < 32; ++i) {          // $1462eb do #$20 — $1462ed-$1462f9
            A = alu_add(A, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // $1462ed
            {   int64_t Bold = B;               // $1462ee
                mem.wrL(mem.R[1], Bold & M48);
                mem.updR(1, +1);
                B = alu_add(B, A);
            }
            x1 = acc24raw(A);                   // $1462ef
            x0 = (uint32_t)(A & M24);           // $1462f0
            {   uint32_t t = mem.rdx(mem.R[0]); mem.updR(0, +1); // $1462f1
                A = mem2acc(t);
            }
            A = acc_asr(A, 12);                 // $1462f2
            B = alu_add(B, A);                  // $1462f3
            B = acc_and(B, 0x1FFFu);            // $1462f4
            B = alu_add(B, al24(0x14A000u));    // $1462f6
            A = sext48(((int64_t)x1 << 24) | (int64_t)x0); // $1462f8/$1462f9
        }
        mem.wry(R6BASE + 0x18, acc24raw(B));    // $1462fa
        mem.wry(R6BASE + 0x19, (uint32_t)(B & M24)); // $1462fb
        hook(10);
        // ---- carrier interpolation -> X:$E0-$FF, >>5 --------------------------------
        mem.R[1] = 0x20u;                       // $1462fc
        mem.R[3] = 0xE0u;                       // $1462fe
        mem.R[4] = mem.R[1];                    // $146300
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // $146301
        y0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $146302
        x1 = mem.rdx(mem.R[2]); mem.updR(2, +1);// $146303
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // $146304
        for (int i = 0; i < 16; ++i) {          // $146305 do #$10 — $146307-$146316
            A = mpsu_neg(x1, y0);               // $146307
            {   uint32_t t = mem.rdx(mem.R[2]); // $146308
                x0 = t;
            }
            A = alu_add(A, al24(x1));
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // $146309
            A = alu_add(A, al24(x1));
            A = mcsu(x0, y0, A);                // $14630a
            {   int64_t Aold = A;               // $14630b asr a  x:(r0)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = acc_asr(Aold, 1);
                x1 = tx1; y0 = ty0;
            }
            A = acc_asr(A, 1);                  // $14630c
            A = acc_asr(A, 1);                  // $14630d
            B = mpsu_neg(x1, y0);               // $14630e
            {   uint32_t t = mem.rdx(mem.R[0]); // $14630f
                x0 = t;
            }
            B = alu_add(B, al24(x1));
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // $146310
            B = alu_add(B, al24(x1));
            B = mcsu(x0, y0, B);                // $146311
            {   int64_t Bold = B;               // $146312 asr b  x:(r2)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = acc_asr(Bold, 1);
                x1 = tx1; y0 = ty0;
            }
            B = acc_asr(B, 1);                  // $146313
            B = acc_asr(B, 1);                  // $146314
            mem.wrx(mem.R[3], acc24sat(A)); mem.updR(3, +1); // $146315
            mem.wrx(mem.R[3], acc24sat(B)); mem.updR(3, +1); // $146316
        }
        hook(11);
        // ---- rotator 1 (states $21/$22, fb $25/$26) ----------------------------------
        mem.R[0] = 0xDEu;                       // $146317
        mem.R[1] = 0xE0u;                       // $146319
        x0 = mem.rdy(R6BASE + 0x21);            // $14631b
        mem.wrx(mem.R[0], x0); mem.updR(0, +1); // $14631c
        x0 = mem.rdy(R6BASE + 0x22);            // $14631d
        mem.wrx(mem.R[0], x0); mem.updR(0, -1); // $14631e
        x1 = 0x0CFCE3u;                         // $14631f
        y0 = 0x2BC9CAu;                         // $146321
        y1 = mem.rdy(R6BASE + 0x25);            // $146323
        B = mem2acc(mem.rdy(R6BASE + 0x26));    // $146324
        mem.R[4] = 0x1Eu;                       // $146325
        x0 = mem.rdx(mem.R[1]); mem.updR(1, +1);// $146327
        A = mem2acc(mem.rdx(mem.R[0])); mem.updR(0, +1); // $146328
        for (int i = 0; i < 16; ++i) {          // $146329 do #$10 — $14632b-$14632e
            {   int64_t Aold = A;               // $14632b
                uint32_t tx0 = mem.rdx(mem.R[1]);
                mem.wry(mem.R[4], y1); mem.updR(4, +1);
                A = mac_ss(y0, x0, Aold);
                mem.updR(1, +1);
                x0 = tx0;
            }
            {   int64_t Aold = A; int64_t Bold = B; // $14632c
                uint32_t tb = mem.rdx(mem.R[0]);
                A = mac_ss_neg(y1, y0, Aold);
                mem.updR(0, +1);
                B = mem2acc(tb);
                y1 = acc24sat(Bold);
            }
            {   int64_t Bold = B;               // $14632d
                uint32_t tx0 = mem.rdx(mem.R[1]);
                mem.wry(mem.R[4], y1); mem.updR(4, +1);
                B = mac_ss(x1, x0, Bold);
                mem.updR(1, +1);
                x0 = tx0;
            }
            {   int64_t Bold = B; int64_t Aold = A; // $14632e
                uint32_t ta = mem.rdx(mem.R[0]);
                B = mac_ss_neg(y1, x1, Bold);
                mem.updR(0, +1);
                A = mem2acc(ta);
                y1 = acc24sat(Aold);
            }
        }
        mem.wry(mem.R[4], y1); mem.updR(4, +1); // $14632f
        mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $146330
        mem.wry(R6BASE + 0x25, y1);             // $146331
        mem.wry(R6BASE + 0x26, acc24sat(B));    // $146332
        mem.wry(R6BASE + 0x21, acc24sat(A));    // $146333
        x0 = mem.rdx(mem.R[0]); mem.updR(0, +1);// $146334
        mem.wry(R6BASE + 0x22, x0);             // $146335
        hook(12);
        // ---- rotator 2 (states $23/$24, fb $27/$28) ----------------------------------
        mem.R[4] = 0x1Eu;                       // $146336
        mem.R[5] = 0x20u;                       // $146338
        x0 = mem.rdy(R6BASE + 0x23);            // $14633a
        mem.wry(mem.R[4], x0); mem.updR(4, +1); // $14633b
        x0 = mem.rdy(R6BASE + 0x24);            // $14633c
        mem.wry(mem.R[4], x0); mem.updR(4, -1); // $14633d
        y1 = 0x4E63DFu;                         // $14633e
        x0 = 0x6F0F12u;                         // $146340
        x1 = mem.rdy(R6BASE + 0x27);            // $146342
        B = mem2acc(mem.rdy(R6BASE + 0x28));    // $146343
        mem.R[0] = 0x1Eu;                       // $146344
        y0 = mem.rdy(mem.R[5]); mem.updR(5, +1);// $146346
        A = mem2acc(mem.rdy(mem.R[4])); mem.updR(4, +1); // $146347
        for (int i = 0; i < 16; ++i) {          // $146348 do #$10 — $14634a-$14634d
            {   int64_t Aold = A;               // $14634a
                uint32_t ty0 = mem.rdy(mem.R[5]);
                mem.wrx(mem.R[0], x1);
                A = mac_ss(y0, x0, Aold);
                mem.updR(0, +1); mem.updR(5, +1);
                y0 = ty0;
            }
            {   int64_t Aold = A; int64_t Bold = B; // $14634b
                A = mac_ss_neg(x1, x0, Aold);
                x1 = acc24sat(Bold);
                uint32_t tb = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = mem2acc(tb);
            }
            {   int64_t Bold = B;               // $14634c
                uint32_t ty0 = mem.rdy(mem.R[5]);
                mem.wrx(mem.R[0], x1);
                B = mac_ss(y1, y0, Bold);
                mem.updR(0, +1); mem.updR(5, +1);
                y0 = ty0;
            }
            {   int64_t Bold = B; int64_t Aold = A; // $14634d
                B = mac_ss_neg(y1, x1, Bold);
                x1 = acc24sat(Aold);
                uint32_t ta = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = mem2acc(ta);
            }
        }
        mem.wrx(mem.R[0], x1); mem.updR(0, +1); // $14634e
        mem.wrx(mem.R[0], acc24sat(B)); mem.updR(0, +1); // $14634f
        mem.wry(R6BASE + 0x27, x1);             // $146350
        mem.wry(R6BASE + 0x28, acc24sat(B));    // $146351
        mem.wry(R6BASE + 0x23, acc24sat(A));    // $146352
        x0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $146353
        mem.wry(R6BASE + 0x24, x0);             // $146354
        hook(13);
        // ---- output: pair sums of rot-2 (X:$20-$3F) -> Y:$1F-$2F, then >>1 ----------
        mem.R[0] = 0x20u;                       // $146355
        mem.R[4] = 0x1Fu;                       // $146357
        y0 = 0x400000u;                         // $146359
        x0 = mem.rdx(mem.R[0]); mem.updR(0, +1);// $14635a
        for (int i = 0; i < 8; ++i) {           // $14635b do #$8 — $14635d-$146360
            {   int64_t An = mpy_ss(y0, x0);    // $14635d
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                A = An; x0 = tx0;
            }
            {   int64_t Bold = B;               // $14635e
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                A = mac_ss(y0, x0, A);
                x0 = tx0;
                mem.wry(mem.R[4], acc24sat(Bold)); mem.updR(4, +1);
            }
            {   int64_t Bn = mpy_ss(y0, x0);    // $14635f
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                B = Bn; x0 = tx0;
            }
            {   int64_t Aold = A;               // $146360
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                B = mac_ss(y0, x0, B);
                x0 = tx0;
                mem.wry(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
            }
        }
        mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $146361
        mem.R[4] = 0x20u;                       // $146362
        for (int i = 0; i < 8; ++i) {           // $146364 do #$8 — $146366-$14636d
            A = mem2acc(mem.rdy(mem.R[4])); mem.updR(4, +1); // $146366
            B = mem2acc(mem.rdy(mem.R[4])); mem.updR(4, +1); // $146367
            A = acc_asr(A, 1);                  // $146368
            B = acc_asr(B, 1);                  // $146369
            out[oidx++] = acc24sat(A);          // $14636a
            out[oidx++] = acc24sat(A);          // $14636b
            out[oidx++] = acc24sat(B);          // $14636c
            out[oidx++] = acc24sat(B);          // $14636d
        }
    }

public:
    FmMem mem;
    int64_t A = 0, B = 0;
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;
};

} // namespace mnmfrqenvfm
