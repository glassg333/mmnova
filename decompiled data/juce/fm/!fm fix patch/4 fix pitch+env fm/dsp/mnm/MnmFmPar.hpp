// =============================================================================
// MnmFmPar.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m9 "FM+ PAR" — bit-exact transcription of DSP1 P:$145EC9 (INIT),
// P:$145ED4 (CONF), P:$145EDB-$14619C (PROC).
//
// Transcribed instruction-by-instruction from
// decompiled data/03_listings/machines_page_A/09_FM-PAR_full.txt and verified
// against scripts/dsp_emu.py running the original firmware
// (research/fm_par_vectors.txt: 14 knob sets x 16 blocks, output + state).
//
// Topology (three parallel modulators through one carrier):
//   mod k: sine($14A000, 8192) -> first difference -> one-pole LP
//          (kTone144AC7[TONE]) -> peak shaper cur + 32*G*prev
//          (G_word = max(0, ENV-64)<<16, gated at 64)
//   levels: wrap-squared ENV word * droop state (CONF resets to $7FFFFF)
//   mix -> LP(kTone144AC7[TONE]) -> carrier phase FM (mix >> 12 per sample)
//   carrier: sine, interpolated; 2 quadrature rotators; pair-sum x0.5 output.
//
// Q23 domain: params are knob words (knob<<16), pitch is the kernel pitch word
// (register A on PROC entry), output = 32 words (16 mono L/R pairs, ±16 range;
// the kernel AMP stage provides the voice gain).
// =============================================================================
#pragma once
#include "MnmFmDsp.hpp"
#include <cstring>

namespace mnmfm {

class MnmFmPar {
public:
    // Optional per-stage debug hook (see research stage-diff tooling).
    void (*stageHook)(void* ud, int stage) = nullptr;
    void* stageUser = nullptr;
    inline void hook(int s) { if (stageHook) stageHook(stageUser, s); }

    // ---- INIT P:$145EC9 --------------------------------------------------------
    // lua (r6+$11),r0 ; move #>$14a000,x0 ; move #$0,x1 ;
    // rep #<$20 move x1,y:(r0)+ ; x0 -> y:(r6+$10/$14/$18/$1c)
    void init() {
        for (int i = 0; i < 0x100; ++i) { mem.X[i] = 0; mem.Y[i] = 0; }
        for (int i = 0; i < 0x40; ++i) mem.st[i] = 0;
        mem.resetRegs();
        mem.R[0] = R6BASE + 0x11;              // lua (r6+$11),r0
        x1 = 0;                                 // move #$0,x1 (short imm -> 0)
        for (int i = 0; i < 32; ++i) {          // rep #<$20 ; move x1,y:(r0)+
            mem.wry(mem.R[0], x1);
            mem.updR(0, +1);
        }
        x0 = 0x14A000u;                         // move #>$14a000,x0
        mem.wry(R6BASE + 0x10, x0);             // move x0,y:(r6+$10)
        mem.wry(R6BASE + 0x14, x0);             // move x0,y:(r6+$14)
        mem.wry(R6BASE + 0x18, x0);             // move x0,y:(r6+$18)
        mem.wry(R6BASE + 0x1C, x0);             // move x0,y:(r6+$1c)
        A = 0; B = 0;
    }

    // ---- CONF P:$145ED4 --------------------------------------------------------
    // move #>$7fffff,a ; a -> y:(r6+$32/$33/$34) ; rts
    void conf(const uint32_t knob[8]) {         // knob 0..127, hardware order
        for (int i = 0; i < 8; ++i)
            mem.st[0x04 + i] = (knob[i] & 0xFFFF) << 16;   // y:(r6+$4..$B) = knob<<16
        mem.st[0x32] = 0x7FFFFF;                // move a,y:(r6+$32)
        mem.st[0x33] = 0x7FFFFF;                // move a,y:(r6+$33)
        mem.st[0x34] = 0x7FFFFF;                // move a,y:(r6+$34)
    }

    // ---- PROC P:$145EDB-$14619C -------------------------------------------------
    // A = pitch word on entry; writes 32 words (16 L/R pairs).
    void proc(int64_t pitchA, uint32_t out[32]) {
        int oidx = 0;
        // ---- pitch -> carrier inc48, stored to L:$5 -----------------------------
        A = pitchA;                              // 48-bit acc as loaded by the kernel call
        x0 = 0x0BE37Cu;                         // $145edb move #>$be37c,x0
        y1 = acc24raw(A);                       // $145edd move a1,y1
        y0 = (uint32_t)(A & M24);               // $145ede move a0,y0
        A = mpsu(x0, y0);                       // $145edf mpysu x0,y0,a
        A = mac_ss(x0, y1, A);                  // $145ee0 dmac ss x0,y1,a
        mem.R[1] = 0x141A80u;                   // $145ee1 move #>$141a80,r1
        // ---- modulator 1 ratio index ------------------------------------------
        B = mem2acc(mem.rdy(R6BASE + 0x04));    // $145ee3 move y:(r6+$4),b   (1FRQ)
        B = alu_add(B, al24(0x8000));           // $145ee4 add #>$8000,b
        x1 = 0x18;                              // $145ee6 move #>$18,x1
        x0 = acc24sat(B);                       // $145ee8 move b,x0
        B = mpy_ss(x1, x0);                     // $145ee9 mpy x1,x0,b
        mem.wrL(5, A & M48);                    // $145eea move a,l:?:>$5
        mem.N[1] = (int32_t)sgn24(acc24raw(B)); // $145eec move b1,n1
        y1 = acc24raw(A);                       // $145eed move a1,y1
        y0 = (uint32_t)(A & M24);               // $145eee move a0,y0
        mem.M[2] = 0x1FFFu;                     // $145eef move #>$1fff,m2
        mem.M[0] = 0x1FFFu;                     // $145ef1 move #>$1fff,m0
        x0 = mem.rdy(mem.R[1] + (uint32_t)mem.N[1]); // $145ef3 move y:(r1+n1),x0
        A = mpsu(x0, y0);                       // $145ef4 mpysu x0,y0,a
        A = mac_ss(x0, y1, A);                  // $145ef5 dmac ss x0,y1,a
        A = acc_asl(A, 3);                      // $145ef6 asl #$3,a,a
        // ---- mod 1 glide ramp (delta = (new_inc - old_inc) >> 5) ---------------
        B = mem2acc(mem.rdy(R6BASE + 0x12));    // $145ef7 move y:(r6+$12),b
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x13); // $145ef8 move y:(r6+$13),b0
        mem.wry(R6BASE + 0x12, acc24raw(A));    // $145ef9 move a1,y:(r6+$12)
        mem.wry(R6BASE + 0x13, (uint32_t)(A & M24)); // $145efa move a0,y:(r6+$13)
        A = alu_sub(A, B);                      // $145efb sub b,a
        A = acc_asr(A, 5);                      // $145efc asr #$5,a,a
        mem.R[1] = 0x20u;                       // $145efd move #>$20,r1
        y1 = acc24raw(A);                       // $145eff move a1,y1
        y0 = (uint32_t)(A & M24);               // $145f00 move a0,y0
        A = B;                                  // $145f01 tfr b,a
        B = mem2acc(mem.rdy(R6BASE + 0x10));    // $145f02 move y:(r6+$10),b
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x11); // $145f03 move y:(r6+$11),b0
        x0 = 0x1FFFu;                           // $145f04 move #>$1fff,x0
        x1 = 0x14A000u;                         // $145f06 move #>$14a000,x1
        // $145f08 do #$20 — body $145f0a-$145f0d (phase ring L:$20-$3F)
        for (int i = 0; i < 32; ++i) {
            A = alu_add(A, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // $145f0a add y,a
            {   int64_t Bold = B;               // $145f0b add a,b  b,l:(r1)+
                mem.wrL(mem.R[1], Bold & M48);
                mem.updR(1, +1);
                B = alu_add(B, A);
            }
            B = acc_and(B, x0);                 // $145f0c and x0,b   (MSW only, frac kept)
            B = alu_add(B, al24(x1));           // $145f0d add x1,b
        }
        hook(1);
        // ---- mod 1 sine interpolation from the ring -> Y:$80-$9F ---------------
        mem.R[1] = 0x20u;                       // $145f0e move #>$20,r1
        mem.R[5] = 0x80u;                       // $145f10 move #>$80,r5
        mem.wry(R6BASE + 0x10, acc24raw(B));    // $145f12 move b1,y:(r6+$10)
        mem.R[4] = mem.R[1];                    // $145f13 move r1,r4
        {   uint32_t t = mem.rdx(mem.R[1]);     // $145f14 move x:(r1)+,r2
            mem.updR(1, +1);
            mem.R[2] = t;
        }
        mem.wry(R6BASE + 0x11, (uint32_t)(B & M24)); // $145f15 move b0,y:(r6+$11)
        y0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $145f16 move y:(r4)+,y0
        {   uint32_t t = mem.rdx(mem.R[1]);     // $145f17 move x:(r1)+,r0
            mem.updR(1, +1);
            mem.R[0] = t;
        }
        x1 = mem.rdx(mem.R[2]); mem.updR(2, +1);// $145f18 move x:(r2)+,x1
        // unrolled pair (samples 0/1)
        A = mpsu_neg(x1, y0);                   // $145f19 mpysu -x1,y0,a
        {   uint32_t t = mem.rdx(mem.R[2]);     // $145f1a add x1,a  x:(r2),x0
            x0 = t;
        }
        A = alu_add(A, al24(x1));
        {   uint32_t t = mem.rdx(mem.R[1]);     // $145f1b add x1,a  x:(r1)+,r2
            mem.updR(1, +1);
            mem.R[2] = t;
        }
        A = alu_add(A, al24(x1));
        A = mcsu(x0, y0, A);                    // $145f1c macsu x0,y0,a
        {   int64_t Aold = A;                   // $145f1d asr a  x:(r0)+,x1  y:(r4)+,y0
            uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1);
            uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
            A = acc_asr(Aold, 1);
            x1 = tx1; y0 = ty0;
        }
        B = mpsu_neg(x1, y0);                   // $145f1e mpysu -x1,y0,b
        {   int64_t Aold = A;                   // $145f1f add x1,b  x:(r0),x0  a,y:(r5)+
            uint32_t tx0 = mem.rdx(mem.R[0]);
            mem.wry(mem.R[5], acc24sat(Aold)); mem.updR(5, +1);
            B = alu_add(B, al24(x1));
            x0 = tx0;
        }
        {   uint32_t t = mem.rdx(mem.R[1]);     // $145f20 add x1,b  x:(r1)+,r0
            mem.updR(1, +1);
            mem.R[0] = t;
        }
        B = alu_add(B, al24(x1));
        B = mcsu(x0, y0, B);                    // $145f21 macsu x0,y0,b
        {   int64_t Bold = B;                   // $145f22 asr b  x:(r2)+,x1  y:(r4)+,y0
            uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1);
            uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
            B = acc_asr(Bold, 1);
            x1 = tx1; y0 = ty0;
        }
        // $145f23 do #$f — body $145f25-$145f2e
        for (int i = 0; i < 15; ++i) {
            A = mpsu_neg(x1, y0);               // $145f25 mpysu -x1,y0,a
            {   int64_t Bold = B;               // $145f26 add x1,a  x:(r2),x0  b,y:(r5)+
                uint32_t tx0 = mem.rdx(mem.R[2]);
                mem.wry(mem.R[5], acc24sat(Bold)); mem.updR(5, +1);
                A = alu_add(A, al24(x1));
                x0 = tx0;
            }
            {   uint32_t t = mem.rdx(mem.R[1]); // $145f27 add x1,a  x:(r1)+,r2
                mem.updR(1, +1);
                mem.R[2] = t;
            }
            A = alu_add(A, al24(x1));
            A = mcsu(x0, y0, A);                // $145f28 macsu x0,y0,a
            {   int64_t Aold = A;               // $145f29 asr a  x:(r0)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = acc_asr(Aold, 1);
                x1 = tx1; y0 = ty0;
            }
            B = mpsu_neg(x1, y0);               // $145f2a mpysu -x1,y0,b
            {   int64_t Aold = A;               // $145f2b add x1,b  x:(r0),x0  a,y:(r5)+
                uint32_t tx0 = mem.rdx(mem.R[0]);
                mem.wry(mem.R[5], acc24sat(Aold)); mem.updR(5, +1);
                B = alu_add(B, al24(x1));
                x0 = tx0;
            }
            {   uint32_t t = mem.rdx(mem.R[1]); // $145f2c add x1,b  x:(r1)+,r0
                mem.updR(1, +1);
                mem.R[0] = t;
            }
            B = alu_add(B, al24(x1));
            B = mcsu(x0, y0, B);                // $145f2d macsu x0,y0,b
            {   int64_t Bold = B;               // $145f2e asr b  x:(r2)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = acc_asr(Bold, 1);
                x1 = tx1; y0 = ty0;
            }
        }
        hook(2);
        x0 = mem.rdy(R6BASE + 0x20);            // $145f2f move y:(r6+$20),x0
        mem.wry(mem.R[5], acc24sat(B)); mem.updR(5, +1); // $145f30 move b,y:(r5)+
        // ---- mod 1 first difference -> X:$80-$9F -------------------------------
        mem.R[5] = 0x80u;                       // $145f31 move #>$80,r5
        mem.R[3] = 0x80u;                       // $145f33 move #>$80,r3
        B = mem2acc(mem.rdy(mem.R[5])); mem.updR(5, +1); // $145f35 move y:(r5)+,b
        A = mem2acc(mem.rdy(mem.R[5])); mem.updR(5, +1); // $145f36 move y:(r5)+,a
        {   uint32_t t = x0;                    // $145f37 sub x0,b  b,x0
            uint32_t tb = acc24sat(B);
            B = alu_sub(B, al24(t));
            x0 = tb;
        }
        {   uint32_t t = x0;                    // $145f38 sub x0,a  a,x0
            uint32_t ta = acc24sat(A);
            A = alu_sub(A, al24(t));
            x0 = ta;
        }
        for (int i = 0; i < 15; ++i) {          // $145f39 do #$f — $145f3b-$145f3e
            {   uint32_t tb = acc24sat(B);      // $145f3b move b,x:(r3)+  y:(r5)+,b
                uint32_t tn = mem.rdy(mem.R[5]); mem.updR(5, +1);
                mem.wrx(mem.R[3], tb); mem.updR(3, +1);
                B = mem2acc(tn);
            }
            {   uint32_t ta = acc24sat(A);      // $145f3c move a,x:(r3)+  y:(r5)+,a
                uint32_t tn = mem.rdy(mem.R[5]); mem.updR(5, +1);
                mem.wrx(mem.R[3], ta); mem.updR(3, +1);
                A = mem2acc(tn);
            }
            {   uint32_t t = x0;                // $145f3d sub x0,b  b,x0
                uint32_t tb = acc24sat(B);
                B = alu_sub(B, al24(t));
                x0 = tb;
            }
            {   uint32_t t = x0;                // $145f3e sub x0,a  a,x0
                uint32_t ta = acc24sat(A);
                A = alu_sub(A, al24(t));
                x0 = ta;
            }
        }
        mem.wrx(mem.R[3], acc24sat(B)); mem.updR(3, +1); // $145f3f move b,x:(r3)+
        mem.wrx(mem.R[3], acc24sat(A)); mem.updR(3, +1); // $145f40 move a,x:(r3)+
        mem.wry(R6BASE + 0x20, x0);             // $145f41 move x0,y:(r6+$20)
        hook(3);
        // ---- TONE one-pole LP on mod 1 (state $2c) -> Y:$80 --------------------
        mem.R[5] = 0x80u;                       // $145f42 move #>$80,r5
        mem.R[2] = 0x144AC7u;                   // $145f44 move #>$144ac7,r2
        B = mem2acc(mem.rdy(R6BASE + 0x0A));    // $145f46 move y:(r6+$a),b  (TONE)
        B = acc_asr(B, 16);                     // $145f47 asr #$10,b,b
        A = mem2acc(mem.rdy(R6BASE + 0x2C));    // $145f48 move y:(r6+$2c),a
        mem.N[2] = (int32_t)sgn24(acc24raw(B)); // $145f49 move b,n2
        mem.R[4] = 0x80u;                       // $145f4a move #>$80,r4
        x0 = mem.rdx(mem.R[5]); mem.updR(5, +1);// $145f4c move x:(r5)+,x0
        y0 = mem.rdy(mem.R[2] + (uint32_t)mem.N[2]); // $145f4d move y:(r2+n2),y0
        for (int i = 0; i < 32; ++i) {          // $145f4e do #$20 — $145f50-$145f51
            {   int64_t Aold = A;               // $145f50 mac y0,x0,a  a,x1  a,y:(r4)+
                mem.wry(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                x1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdx(mem.R[5]); mem.updR(5, +1); // $145f51 mac -x1,y0,a  x:(r5)+,x0
                A = mac_ss_neg(x1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + 0x2C, acc24sat(A));    // $145f52 move a,y:(r6+$2c)
        hook(4);
        // ---- 1ENV gate + shaper gain word --------------------------------------
        B = mem2acc(mem.rdy(R6BASE + 0x05));    // $145f53 move y:(r6+$5),b  (1ENV)
        A = 0;                                  // $145f54 clr a
        if (B >= al24(0x400000)) {              // $145f55 cmp #>$400000,b ; $145f57 blt
            B = alu_sub(B, al24(0x400000));     // $145f59 sub #>$400000,b
            B = alu_add(B, A);                  // $145f5b add a,b
            A = B;                              // $145f5d move b,a
        }
        // ---- mod 1 peak shaper (state $2f), in-place Y:$80 ----------------------
        x0 = mem.rdy(R6BASE + 0x2F);            // $145f5e move y:(r6+$2f),x0
        y1 = acc24sat(A);                       // $145f5f move a,y1
        mem.R[0] = 0x80u;                       // $145f60 move #>$80,r0
        mem.R[1] = 0x80u;                       // $145f62 move #>$80,r1
        for (int i = 0; i < 16; ++i) {          // $145f64 do #$10 — $145f66-$145f6d
            {   int64_t An = mpy_ss(x0, y1);    // $145f66 mpy x0,y1,a  y:(r0)+,x1
                uint32_t tx1 = mem.rdy(mem.R[0]); mem.updR(0, +1);
                A = An; x1 = tx1;
            }
            A = acc_asl(A, 4);                  // $145f67 asl #$4,a,a
            A = alu_add(A, al24(x1)); x0 = x1;  // $145f68 add x1,a  x1,x0
            {   int64_t Bn = mpy_ss(x0, y1);    // $145f69 mpy x0,y1,b  y:(r0)+,x1
                uint32_t tx1 = mem.rdy(mem.R[0]); mem.updR(0, +1);
                B = Bn; x1 = tx1;
            }
            B = acc_asl(B, 4);                  // $145f6a asl #$4,b,b
            B = alu_add(B, al24(x1)); x0 = x1;  // $145f6b add x1,b  x1,x0
            mem.wry(mem.R[1], acc24sat(A)); mem.updR(1, +1); // $145f6c move a,y:(r1)+
            mem.wry(mem.R[1], acc24sat(B)); mem.updR(1, +1); // $145f6d move b,y:(r1)+
        }
        hook(5);
        mem.wry(R6BASE + 0x2F, x0);             // $145f6e move x0,y:(r6+$2f)
        // ================= modulator 2 (2FRQ $6, 2ENV $7) =======================
        modStage(0x06, 0x07, 0x16, 0x17, 0x14, 0x15, 0x40u, 0xC0u,
                 0x21, 0x2D, 0x30);
        // ================= modulator 3 (3FRQ $8, 3ENV $9) =======================
        modStage(0x08, 0x09, 0x1A, 0x1B, 0x18, 0x19, 0x20u, 0xE0u,
                 0x22, 0x2E, 0x31);
        // ================= levels (droop states) =================================
        hook(6);
        mem.R[0] = 0x80u;                       // $146085 move #>$80,r0
        mem.R[1] = 0xC0u;                       // $146087 move #>$c0,r1
        mem.R[2] = 0x80u;                       // $146089 move #>$80,r2
        // 2ENV droop (state $33)
        B = mem2acc(mem.rdy(R6BASE + 0x07));    // $14608b move y:(r6+$7),b
        y0 = mem.rdy(R6BASE + 0x33);            // $14608c move y:(r6+$33),y0
        if (B >= al24(0x400000)) {              // $14608d cmp ; $14608f blt
            B = alu_sub(B, al24(0x400000));     // $146091 sub #>$400000,b
            A = mem2acc(0x7FFFFF);              // $146093 move #>$7fffff,a
            x0 = acc24sat(B);                   // $146095 move b,x0
            B = mpy_ss(x0, x0);                 // $146096 mpy x0,x0,b
            x0 = acc24sat(B);                   // $146097 move b,x0
            A = mac_ss_neg(x0, x0, A);          // $146098 mac -x0,x0,a
            x0 = acc24sat(A);                   // $146099 move a,x0
            A = mpy_ss(y0, x0);                 // $14609a mpy y0,x0,a
            y0 = acc24sat(A);                   // $14609c move a,y0
            mem.wry(R6BASE + 0x33, y0);         // $14609d move y0,y:(r6+$33)
        }
        {   // level 2: y1 = state2 * ((2ENV^2)<<2)
            x0 = mem.rdy(R6BASE + 0x07);        // $14609e move y:(r6+$7),x0
            A = mpy_ss(x0, x0);                 // $14609f mpy x0,x0,a
            A = acc_asl(A, 2);                  // $1460a0 asl #$2,a,a
            x0 = acc24sat(A);                   // $1460a1 move a,x0
            A = mpy_ss(y0, x0);                 // $1460a2 mpy y0,x0,a
            y1 = acc24sat(A);                   // $1460a3 move a,y1
        }
        // 1ENV droop (state $32)
        B = mem2acc(mem.rdy(R6BASE + 0x05));    // $1460a4 move y:(r6+$5),b
        y0 = mem.rdy(R6BASE + 0x32);            // $1460a5 move y:(r6+$32),y0
        if (B >= al24(0x400000)) {              // $1460a6 cmp ; $1460a8 blt
            B = alu_sub(B, al24(0x400000));     // $1460aa sub #>$400000,b
            A = mem2acc(0x7FFFFF);              // $1460ac move #>$7fffff,a
            x0 = acc24sat(B);                   // $1460ae move b,x0
            B = mpy_ss(x0, x0);                 // $1460af mpy x0,x0,b
            x0 = acc24sat(B);                   // $1460b0 move b,x0
            A = mac_ss_neg(x0, x0, A);          // $1460b1 mac -x0,x0,a
            x0 = acc24sat(A);                   // $1460b2 move a,x0
            A = mpy_ss(y0, x0);                 // $1460b3 mpy y0,x0,a
            y0 = acc24sat(A);                   // $1460b5 move a,y0
            mem.wry(R6BASE + 0x32, y0);         // $1460b6 move y0,y:(r6+$32)
        }
        {   // level 1: y0 = state1 * ((1ENV^2)<<2)
            x0 = mem.rdy(R6BASE + 0x05);        // $1460b7 move y:(r6+$5),x0
            A = mpy_ss(x0, x0);                 // $1460b8 mpy x0,x0,a
            A = acc_asl(A, 2);                  // $1460b9 asl #$2,a,a
            x0 = acc24sat(A);                   // $1460ba move a,x0
            A = mpy_ss(y0, x0);                 // $1460bb mpy y0,x0,a
            y0 = acc24sat(A);                   // $1460bc move a,y0
        }
        hook(7);
        // ================= mix: mod1 + mod2 -> X:$80 =============================
        {   uint32_t m1 = mem.rdy(mem.R[0]); mem.updR(0, +1); // $1460bd move y:(r0)+,x0
            x0 = m1;
        }
        {   uint32_t m2v = mem.rdy(mem.R[1]); mem.updR(1, +1); // $1460be move y:(r1)+,x1
            x1 = m2v;
        }
        A = mpy_ss(y0, x0);                     // $1460bf mpy y0,x0,a
        {   uint32_t tx0 = mem.rdy(mem.R[0]); mem.updR(0, +1); // $1460c0 mac y1,x1,a  y:(r0)+,x0
            A = mac_ss(y1, x1, A);
            x0 = tx0;
        }
        {   uint32_t tx1 = mem.rdy(mem.R[1]); mem.updR(1, +1); // $1460c1 move y:(r1)+,x1
            x1 = tx1;
        }
        for (int i = 0; i < 31; ++i) {          // $1460c2 do #$1f — $1460c4-$1460c6
            {   int64_t Aold = A;               // $1460c4 mpy y0,x0,a  a,x:(r2)+
                mem.wrx(mem.R[2], acc24sat(Aold)); mem.updR(2, +1);
                A = mpy_ss(y0, x0);
            }
            {   uint32_t tx0 = mem.rdy(mem.R[0]); mem.updR(0, +1); // $1460c5 mac y1,x1,a  y:(r0)+,x0
                A = mac_ss(y1, x1, A);
                x0 = tx0;
            }
            {   uint32_t tx1 = mem.rdy(mem.R[1]); mem.updR(1, +1); // $1460c6 move y:(r1)+,x1
                x1 = tx1;
            }
        }
        mem.wrx(mem.R[2], acc24sat(A)); mem.updR(2, +1); // $1460c7 move a,x:(r2)+
        hook(8);
        // ================= mod 3 level + mix ====================================
        // 3ENV droop (state $34)
        B = mem2acc(mem.rdy(R6BASE + 0x09));    // $1460ce move y:(r6+$9),b
        y0 = mem.rdy(R6BASE + 0x34);            // $1460cf move y:(r6+$34),y0
        if (B >= al24(0x400000)) {              // $1460d0 cmp ; $1460d2 blt
            B = alu_sub(B, al24(0x400000));     // $1460d4 sub #>$400000,b
            A = mem2acc(0x7FFFFF);              // $1460d6 move #>$7fffff,a
            x0 = acc24sat(B);                   // $1460d8 move b,x0
            B = mpy_ss(x0, x0);                 // $1460d9 mpy x0,x0,b
            x0 = acc24sat(B);                   // $1460da move b,x0
            A = mac_ss_neg(x0, x0, A);          // $1460db mac -x0,x0,a
            x0 = acc24sat(A);                   // $1460dc move a,x0
            A = mpy_ss(y0, x0);                 // $1460dd mpy y0,x0,a
            y0 = acc24sat(A);                   // $1460df move a,y0
            mem.wry(R6BASE + 0x34, y0);         // $1460e0 move y0,y:(r6+$34)
        }
        {   // level 3: y0 = state3 * ((3ENV^2)<<2)
            x0 = mem.rdy(R6BASE + 0x09);        // $1460e1 move y:(r6+$9),x0
            A = mpy_ss(x0, x0);                 // $1460e2 mpy x0,x0,a
            A = acc_asl(A, 2);                  // $1460e3 asl #$2,a,a
            x0 = acc24sat(A);                   // $1460e4 move a,x0
            A = mpy_ss(y0, x0);                 // $1460e5 mpy y0,x0,a
            y0 = acc24sat(A);                   // $1460e6 move a,y0
        }
        mem.R[0] = 0xE0u;                       // $1460c8 move #>$e0,r0
        mem.R[1] = 0x80u;                       // $1460ca move #>$80,r1
        mem.R[4] = 0x80u;                       // $1460cc move #>$80,r4
        {   uint32_t t = mem.rdy(mem.R[0]); mem.updR(0, +1); // $1460e7 move y:(r0)+,y1
            y1 = t;
        }
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $1460e8 move x:(r1)+,a
            A = mem2acc(t);
        }
        {   uint32_t ty1 = mem.rdy(mem.R[0]); mem.updR(0, +1); // $1460e9 mac y1,y0,a  y:(r0)+,y1
            A = mac_ss(y1, y0, A);
            y1 = ty1;
        }
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $1460ea move x:(r1)+,b
            B = mem2acc(t);
        }
        for (int i = 0; i < 15; ++i) {          // $1460eb do #$f — $1460ed-$1460f0
            {   int64_t Aold = A; int64_t Bold = B; // $1460ed mac y1,y0,b  a,x:(r4)+  y:(r0)+,y1
                uint32_t ty1 = mem.rdy(mem.R[0]); mem.updR(0, +1);
                mem.wrx(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                B = mac_ss(y1, y0, Bold);
                y1 = ty1;
            }
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $1460ee move x:(r1)+,a
                A = mem2acc(t);
            }
            {   int64_t Bold = B;               // $1460ef mac y1,y0,a  b,x:(r4)+  y:(r0)+,y1
                uint32_t ty1 = mem.rdy(mem.R[0]); mem.updR(0, +1);
                mem.wrx(mem.R[4], acc24sat(Bold)); mem.updR(4, +1);
                A = mac_ss(y1, y0, A);
                y1 = ty1;
            }
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $1460f0 move x:(r1)+,b
                B = mem2acc(t);
            }
        }
        {   int64_t Aold = A;                   // $1460f1 mac y1,y0,b  a,x:(r4)+  y:(r0)+,y1
            uint32_t ty1 = mem.rdy(mem.R[0]); mem.updR(0, +1);
            mem.wrx(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
            B = mac_ss(y1, y0, B);
            y1 = ty1;
        }
        mem.wrx(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $1460f2 move b,x:(r4)+
        hook(9);
        // ================= TONE LP on the mix (state $2b, in-place X:$80) ========
        mem.R[5] = 0x80u;                       // $1460f3 move #>$80,r5
        mem.R[2] = 0x144AC7u;                   // $1460f5 move #>$144ac7,r2
        B = mem2acc(mem.rdy(R6BASE + 0x0A));    // $1460f7 move y:(r6+$a),b
        B = acc_asr(B, 16);                     // $1460f8 asr #$10,b,b
        A = mem2acc(mem.rdy(R6BASE + 0x2B));    // $1460f9 move y:(r6+$2b),a
        mem.N[2] = (int32_t)sgn24(acc24raw(B)); // $1460fa move b,n2
        mem.R[4] = 0x80u;                       // $1460fb move #>$80,r4
        x0 = mem.rdx(mem.R[5]); mem.updR(5, +1);// $1460fd move x:(r5)+,x0
        y0 = mem.rdy(mem.R[2] + (uint32_t)mem.N[2]); // $1460fe move y:(r2+n2),y0
        for (int i = 0; i < 32; ++i) {          // $1460ff do #$20 — $146101-$146102
            {   int64_t Aold = A;               // $146101 mac y0,x0,a  a,x:(r4)+  a,y1
                mem.wrx(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                y1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdx(mem.R[5]); mem.updR(5, +1); // $146102 mac -y1,y0,a  x:(r5)+,x0
                A = mac_ss_neg(y1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + 0x2B, acc24sat(A));    // $146103 move a,y:(r6+$2b)
        hook(10);
        // ================= carrier phase + post-sample FM ========================
        A = sext48(mem.rdL(5));                 // $146104 move l:?:>$5,a
        B = mem2acc(mem.rdy(R6BASE + 0x1E));    // $146106 move y:(r6+$1e),b
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x1F); // $146107 move y:(r6+$1f),b0
        mem.wry(R6BASE + 0x1E, acc24raw(A));    // $146108 move a1,y:(r6+$1e)
        mem.wry(R6BASE + 0x1F, (uint32_t)(A & M24)); // $146109 move a0,y:(r6+$1f)
        A = alu_sub(A, B);                      // $14610a sub b,a
        A = acc_asr(A, 5);                      // $14610b asr #$5,a,a
        mem.R[1] = 0x20u;                       // $14610c move #>$20,r1
        mem.R[0] = 0x80u;                       // $14610e move #>$80,r0
        y1 = acc24raw(A);                       // $146110 move a1,y1
        y0 = (uint32_t)(A & M24);               // $146111 move a0,y0
        A = B;                                  // $146112 tfr b,a
        B = mem2acc(mem.rdy(R6BASE + 0x1C));    // $146113 move y:(r6+$1c),b
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x1D); // $146114 move y:(r6+$1d),b0
        for (int i = 0; i < 32; ++i) {          // $146115 do #$20 — $146117-$146123
            A = alu_add(A, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // $146117 add y,a
            {   int64_t Bold = B;               // $146118 add a,b  b,l:(r1)+
                mem.wrL(mem.R[1], Bold & M48);
                mem.updR(1, +1);
                B = alu_add(B, A);
            }
            x1 = acc24raw(A);                   // $146119 move a1,x1
            x0 = (uint32_t)(A & M24);           // $14611a move a0,x0
            {   uint32_t t = mem.rdx(mem.R[0]); // $14611b move x:(r0)+,a
                mem.updR(0, +1);
                A = mem2acc(t);
            }
            A = acc_asr(A, 12);                 // $14611c asr #$c,a,a
            B = alu_add(B, A);                  // $14611d add a,b
            B = acc_and(B, 0x1FFFu);            // $14611e and #>$1fff,b
            B = alu_add(B, al24(0x14A000u));    // $146120 add #>$14a000,b
            A = sext48(((int64_t)x1 << 24) | (int64_t)x0); // $146122/$146123 move x1,a ; move x0,a0
        }
        hook(11);
        // ---- carrier interpolation (Y-page LUT) -> X:$E0-$FF, >>5 ---------------
        mem.R[1] = 0x20u;                       // $146124 move #>$20,r1
        mem.R[5] = 0xE0u;                       // $146126 move #>$e0,r5
        mem.wry(R6BASE + 0x1C, acc24raw(B));    // $146128 move b1,y:(r6+$1c)
        mem.R[4] = mem.R[1];                    // $146129 move r1,r4
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $14612a move x:(r1)+,r2
            mem.R[2] = t;
        }
        mem.wry(R6BASE + 0x1D, (uint32_t)(B & M24)); // $14612b move b0,y:(r6+$1d)
        y0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $14612c move y:(r4)+,y0
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $14612d move x:(r1)+,r0
            mem.R[0] = t;
        }
        x1 = mem.rdy(mem.R[2]); mem.updR(2, +1);// $14612e move y:(r2)+,x1
        A = mpsu_neg(x1, y0);                   // $14612f mpysu -x1,y0,a
        {   uint32_t t = mem.rdy(mem.R[2]);     // $146130 add x1,a  y:(r2),y1
            y1 = t;
        }
        A = alu_add(A, al24(x1));
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $146131 add x1,a  x:(r1)+,r2
            mem.R[2] = t;
        }
        A = alu_add(A, al24(x1));
        A = mcsu(y1, y0, A);                    // $146132 macsu y1,y0,a
        {   uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1); // $146133 move x:(r0)+,x1  y:(r4)+,y0
            uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
            x1 = tx1; y0 = ty0;
        }
        A = acc_asr(A, 3);                      // $146134 asr #$3,a,a
        B = mpsu_neg(x1, y0);                   // $146135 mpysu -x1,y0,b
        {   int64_t Aold = A;                   // $146136 add x1,b  a,x:(r5)+  y:(r0),y1
            uint32_t ty1 = mem.rdy(mem.R[0]);
            mem.wrx(mem.R[5], acc24sat(Aold)); mem.updR(5, +1);
            B = alu_add(B, al24(x1));
            y1 = ty1;
        }
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $146137 add x1,b  x:(r1)+,r0
            mem.R[0] = t;
        }
        B = alu_add(B, al24(x1));
        B = mcsu(y1, y0, B);                    // $146138 macsu y1,y0,b
        {   uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1); // $146139 move x:(r2)+,x1  y:(r4)+,y0
            uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
            x1 = tx1; y0 = ty0;
        }
        B = acc_asr(B, 3);                      // $14613a asr #$3,b,b
        for (int i = 0; i < 15; ++i) {          // $14613b do #$f — $14613d-$146148
            A = mpsu_neg(x1, y0);               // $14613d mpysu -x1,y0,a
            {   int64_t Bold = B;               // $14613e add x1,a  b,x:(r5)+  y:(r2),y1
                uint32_t ty1 = mem.rdy(mem.R[2]);
                mem.wrx(mem.R[5], acc24sat(Bold)); mem.updR(5, +1);
                A = alu_add(A, al24(x1));
                y1 = ty1;
            }
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $14613f add x1,a  x:(r1)+,r2
                mem.R[2] = t;
            }
            A = alu_add(A, al24(x1));
            A = mcsu(y1, y0, A);                // $146140 macsu y1,y0,a
            {   uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1); // $146141 move x:(r0)+,x1  y:(r4)+,y0
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                x1 = tx1; y0 = ty0;
            }
            A = acc_asr(A, 3);                  // $146142 asr #$3,a,a
            B = mpsu_neg(x1, y0);               // $146143 mpysu -x1,y0,b
            {   int64_t Aold = A;               // $146144 add x1,b  a,x:(r5)+  y:(r0),y1
                uint32_t ty1 = mem.rdy(mem.R[0]);
                mem.wrx(mem.R[5], acc24sat(Aold)); mem.updR(5, +1);
                B = alu_add(B, al24(x1));
                y1 = ty1;
            }
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); // $146145 add x1,b  x:(r1)+,r0
                mem.R[0] = t;
            }
            B = alu_add(B, al24(x1));
            B = mcsu(y1, y0, B);                // $146146 macsu y1,y0,b
            {   uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1); // $146147 move x:(r2)+,x1  y:(r4)+,y0
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                x1 = tx1; y0 = ty0;
            }
            B = acc_asr(B, 3);                  // $146148 asr #$3,b,b
        }
        mem.wrx(mem.R[5], acc24sat(B)); mem.updR(5, +1); // $146149 move b,x:(r5)+
        hook(12);
        // ================= rotator 1 (states $23/$24, fb $27/$28) ================
        mem.R[0] = 0xDEu;                       // $14614a move #>$de,r0
        mem.R[1] = 0xE0u;                       // $14614c move #>$e0,r1
        x0 = mem.rdy(R6BASE + 0x23);            // $14614e move y:(r6+$23),x0
        mem.wrx(mem.R[0], x0); mem.updR(0, +1); // $14614f move x0,x:(r0)+
        x0 = mem.rdy(R6BASE + 0x24);            // $146150 move y:(r6+$24),x0
        mem.wrx(mem.R[0], x0); mem.updR(0, -1); // $146151 move x0,x:(r0)-
        x1 = 0x0CFCE3u;                         // $146152 move #>$cfce3,x1
        y0 = 0x2BC9CAu;                         // $146154 move #>$2bc9ca,y0
        y1 = mem.rdy(R6BASE + 0x27);            // $146156 move y:(r6+$27),y1
        B = mem2acc(mem.rdy(R6BASE + 0x28));    // $146157 move y:(r6+$28),b
        mem.R[4] = 0x1Eu;                       // $146158 move #>$1e,r4
        x0 = mem.rdx(mem.R[1]); mem.updR(1, +1);// $14615a move x:(r1)+,x0
        A = mem2acc(mem.rdx(mem.R[0])); mem.updR(0, +1); // $14615b move x:(r0)+,a
        for (int i = 0; i < 16; ++i) {          // $14615c do #$10 — $14615e-$146161
            {   int64_t Aold = A;               // $14615e mac y0,x0,a  x:(r1)+,x0  y1,y:(r4)+
                uint32_t tx0 = mem.rdx(mem.R[1]);
                mem.wry(mem.R[4], y1); mem.updR(4, +1);
                A = mac_ss(y0, x0, Aold);
                mem.updR(1, +1);
                x0 = tx0;
            }
            {   int64_t Aold = A; int64_t Bold = B; // $14615f mac -y1,y0,a  x:(r0)+,b  b,y1
                uint32_t tb = mem.rdx(mem.R[0]);
                A = mac_ss_neg(y1, y0, Aold);
                mem.updR(0, +1);
                B = mem2acc(tb);
                y1 = acc24sat(Bold);
            }
            {   int64_t Bold = B;               // $146160 mac x1,x0,b  x:(r1)+,x0  y1,y:(r4)+
                uint32_t tx0 = mem.rdx(mem.R[1]);
                mem.wry(mem.R[4], y1); mem.updR(4, +1);
                B = mac_ss(x1, x0, Bold);
                mem.updR(1, +1);
                x0 = tx0;
            }
            {   int64_t Bold = B; int64_t Aold = A; // $146161 mac -y1,x1,b  x:(r0)+,a  a,y1
                uint32_t ta = mem.rdx(mem.R[0]);
                B = mac_ss_neg(y1, x1, Bold);
                mem.updR(0, +1);
                A = mem2acc(ta);
                y1 = acc24sat(Aold);
            }
        }
        mem.wry(mem.R[4], y1); mem.updR(4, +1); // $146162 move y1,y:(r4)+
        mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $146163 move b,y:(r4)+
        mem.wry(R6BASE + 0x27, y1);             // $146164 move y1,y:(r6+$27)
        mem.wry(R6BASE + 0x28, acc24sat(B));    // $146165 move b,y:(r6+$28)
        mem.wry(R6BASE + 0x23, acc24sat(A));    // $146166 move a,y:(r6+$23)
        x0 = mem.rdx(mem.R[0]); mem.updR(0, +1);// $146167 move x:(r0)+,x0
        mem.wry(R6BASE + 0x24, x0);             // $146168 move x0,y:(r6+$24)
        hook(13);
        // ================= rotator 2 (states $25/$26, fb $29/$2a) ================
        mem.R[4] = 0x1Eu;                       // $146169 move #>$1e,r4
        mem.R[5] = 0x20u;                       // $14616b move #>$20,r5
        x0 = mem.rdy(R6BASE + 0x25);            // $14616d move y:(r6+$25),x0
        mem.wry(mem.R[4], x0); mem.updR(4, +1); // $14616e move x0,y:(r4)+
        x0 = mem.rdy(R6BASE + 0x26);            // $14616f move y:(r6+$26),x0
        mem.wry(mem.R[4], x0); mem.updR(4, -1); // $146170 move x0,y:(r4)-
        y1 = 0x4E63DFu;                         // $146171 move #>$4e63df,y1
        x0 = 0x6F0F12u;                         // $146173 move #>$6f0f12,x0
        x1 = mem.rdy(R6BASE + 0x29);            // $146175 move y:(r6+$29),x1
        B = mem2acc(mem.rdy(R6BASE + 0x2A));    // $146176 move y:(r6+$2a),b
        mem.R[0] = 0x1Eu;                       // $146177 move #>$1e,r0
        y0 = mem.rdy(mem.R[5]); mem.updR(5, +1);// $146179 move y:(r5)+,y0
        A = mem2acc(mem.rdy(mem.R[4])); mem.updR(4, +1); // $14617a move y:(r4)+,a
        for (int i = 0; i < 16; ++i) {          // $14617b do #$10 — $14617d-$146180
            {   int64_t Aold = A;               // $14617d mac y0,x0,a  x1,x:(r0)+  y:(r5)+,y0
                uint32_t ty0 = mem.rdy(mem.R[5]);
                mem.wrx(mem.R[0], x1);
                A = mac_ss(y0, x0, Aold);
                mem.updR(0, +1); mem.updR(5, +1);
                y0 = ty0;
            }
            {   int64_t Aold = A; int64_t Bold = B; // $14617e mac -x1,x0,a  b,x1  y:(r4)+,b
                A = mac_ss_neg(x1, x0, Aold);
                x1 = acc24sat(Bold);
                uint32_t tb = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = mem2acc(tb);
            }
            {   int64_t Bold = B;               // $14617f mac y1,y0,b  x1,x:(r0)+  y:(r5)+,y0
                uint32_t ty0 = mem.rdy(mem.R[5]);
                mem.wrx(mem.R[0], x1);
                B = mac_ss(y1, y0, Bold);
                mem.updR(0, +1); mem.updR(5, +1);
                y0 = ty0;
            }
            {   int64_t Bold = B; int64_t Aold = A; // $146180 mac -y1,x1,b  a,x1  y:(r4)+,a
                B = mac_ss_neg(y1, x1, Bold);
                x1 = acc24sat(Aold);
                uint32_t ta = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = mem2acc(ta);
            }
        }
        mem.wrx(mem.R[0], x1); mem.updR(0, +1); // $146181 move x1,x:(r0)+
        mem.wrx(mem.R[0], acc24sat(B)); mem.updR(0, +1); // $146182 move b,x:(r0)+
        mem.wry(R6BASE + 0x29, x1);             // $146183 move x1,y:(r6+$29)
        mem.wry(R6BASE + 0x2A, acc24sat(B));    // $146184 move b,y:(r6+$2a)
        mem.wry(R6BASE + 0x25, acc24sat(A));    // $146185 move a,y:(r6+$25)
        x0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $146186 move y:(r4)+,x0
        mem.wry(R6BASE + 0x26, x0);             // $146187 move x0,y:(r6+$26)
        hook(14);
        // ================= output: pair sums of rot-2 (X:$20-$3F) ================
        mem.R[0] = 0x20u;                       // $146188 move #>$20,r0
        y0 = 0x400000u;                         // $14618a move #$40,y0 (short imm -> $400000)
        x0 = mem.rdx(mem.R[0]); mem.updR(0, +1);// $14618b move x:(r0)+,x0
        {   int64_t An = mpy_ss(y0, x0);        // $14618c mpy y0,x0,a  x:(r0)+,x0
            uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
            A = An; x0 = tx0;
        }
        {   uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1); // $14618d mac y0,x0,a  x:(r0)+,x0
            A = mac_ss(y0, x0, A);
            x0 = tx0;
        }
        A = acc_asr(A, 1);                      // $14618e asr a
        {   int64_t Bn = mpy_ss(y0, x0);        // $14618f mpy y0,x0,b  x:(r0)+,x0
            uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
            B = Bn; x0 = tx0;
        }
        {   int64_t Aold = A;                   // $146190 mac y0,x0,b  x:(r0)+,x0  a,y:(r7)+
            uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
            B = mac_ss(y0, x0, B);
            x0 = tx0;
            out[oidx++] = acc24sat(Aold);
        }
        {   int64_t Aold = A;                   // $146191 asr b  a,y:(r7)+
            B = acc_asr(B, 1);
            out[oidx++] = acc24sat(Aold);
        }
        for (int i = 0; i < 7; ++i) {           // $146192 do #$7 — $146194-$146199
            {   int64_t An = mpy_ss(y0, x0);    // $146194 mpy y0,x0,a  x:(r0)+,x0
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                A = An; x0 = tx0;
            }
            {   int64_t Bold = B;               // $146195 mac y0,x0,a  x:(r0)+,x0  b,y:(r7)+
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                A = mac_ss(y0, x0, A);
                x0 = tx0;
                out[oidx++] = acc24sat(Bold);
            }
            {   int64_t Bold = B;               // $146196 asr a  b,y:(r7)+
                A = acc_asr(A, 1);
                out[oidx++] = acc24sat(Bold);
            }
            {   int64_t Bn = mpy_ss(y0, x0);    // $146197 mpy y0,x0,b  x:(r0)+,x0
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                B = Bn; x0 = tx0;
            }
            {   int64_t Aold = A;               // $146198 mac y0,x0,b  x:(r0)+,x0  a,y:(r7)+
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                B = mac_ss(y0, x0, B);
                x0 = tx0;
                out[oidx++] = acc24sat(Aold);
            }
            {   int64_t Aold = A;               // $146199 asr b  a,y:(r7)+
                B = acc_asr(B, 1);
                out[oidx++] = acc24sat(Aold);
            }
        }
        out[oidx++] = acc24sat(B);              // $14619a move b,y:(r7)+
        out[oidx++] = acc24sat(B);              // $14619b move b,y:(r7)+
    }

private:
    // One modulator stage: ratio index from knob frqOff, ring L:ringBase,
    // interp -> Y:bufBase, diff (state diffOff) -> X:$80, TONE LP (state lpOff)
    // -> Y:bufBase, shaper (gain from envOff, state shOff).
    // Mirrors the firmware's three copies of the same code (no factoring).
    void modStage(uint8_t frqOff, uint8_t envOff, uint8_t incHi, uint8_t incLo,
                  uint8_t phHi, uint8_t phLo, uint32_t ringBase, uint32_t bufBase,
                  uint8_t diffOff, uint8_t lpOff, uint8_t shOff) {
        // ---- ratio index --------------------------------------------------------
        mem.R[1] = 0x141A80u;                   // move #>$141a80,r1
        B = mem2acc(mem.rdy(R6BASE + frqOff));  // move y:(r6+$n),b
        B = alu_add(B, al24(0x8000));           // add #>$8000,b
        x1 = 0x18;                              // move #>$18,x1
        x0 = acc24sat(B);                       // move b,x0
        B = mpy_ss(x1, x0);                     // mpy x1,x0,b
        {   int64_t t = sext48(mem.rdL(5));     // move l:?:>$5,a
            A = t;
        }
        mem.N[1] = (int32_t)sgn24(acc24raw(B)); // move b1,n1
        y1 = acc24raw(A);                       // move a1,y1
        y0 = (uint32_t)(A & M24);               // move a0,y0
        x0 = mem.rdy(mem.R[1] + (uint32_t)mem.N[1]); // move y:(r1+n1),x0
        A = mpsu(x0, y0);                       // mpysu x0,y0,a
        A = mac_ss(x0, y1, A);                  // dmac ss x0,y1,a
        A = acc_asl(A, 3);                      // asl #$3,a,a
        // ---- glide ramp ---------------------------------------------------------
        B = mem2acc(mem.rdy(R6BASE + incHi));   // move y:(r6+$n),b
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + incLo);
        mem.wry(R6BASE + incHi, acc24raw(A));   // move a1,y:(r6+$n)
        mem.wry(R6BASE + incLo, (uint32_t)(A & M24));
        A = alu_sub(A, B);                      // sub b,a
        A = acc_asr(A, 5);                      // asr #$5,a,a
        mem.R[1] = ringBase;                    // move #>$xx,r1
        y1 = acc24raw(A);                       // move a1,y1
        y0 = (uint32_t)(A & M24);               // move a0,y0
        A = B;                                  // tfr b,a
        B = mem2acc(mem.rdy(R6BASE + phHi));    // move y:(r6+$n),b
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + phLo);
        x0 = 0x1FFFu;                           // move #>$1fff,x0
        x1 = 0x14A000u;                         // move #>$14a000,x1
        for (int i = 0; i < 32; ++i) {          // do #$20
            A = alu_add(A, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // add y,a
            {   int64_t Bold = B;               // add a,b  b,l:(r1)+
                mem.wrL(mem.R[1], Bold & M48);
                mem.updR(1, +1);
                B = alu_add(B, A);
            }
            B = acc_and(B, x0);                 // and x0,b
            B = alu_add(B, al24(x1));           // add x1,b
        }
        // ---- interpolation -> Y:bufBase ------------------------------------------
        mem.R[1] = ringBase;                    // move #>$xx,r1
        mem.R[5] = bufBase;                     // move #>$xx,r5
        mem.wry(R6BASE + phHi, acc24raw(B));    // move b1,y:(r6+$n)
        mem.R[4] = mem.R[1];                    // move r1,r4
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // move x:(r1)+,r2
        mem.wry(R6BASE + phLo, (uint32_t)(B & M24)); // move b0,y:(r6+$n)
        y0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// move y:(r4)+,y0
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // move x:(r1)+,r0
        x1 = mem.rdx(mem.R[2]); mem.updR(2, +1);// move x:(r2)+,x1
        A = mpsu_neg(x1, y0);                   // mpysu -x1,y0,a
        {   uint32_t t = mem.rdx(mem.R[2]); x0 = t; }  // add x1,a  x:(r2),x0
        A = alu_add(A, al24(x1));
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // add x1,a  x:(r1)+,r2
        A = alu_add(A, al24(x1));
        A = mcsu(x0, y0, A);                    // macsu x0,y0,a
        {   int64_t Aold = A;                   // asr a  x:(r0)+,x1  y:(r4)+,y0
            uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1);
            uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
            A = acc_asr(Aold, 1);
            x1 = tx1; y0 = ty0;
        }
        B = mpsu_neg(x1, y0);                   // mpysu -x1,y0,b
        {   int64_t Aold = A;                   // add x1,b  x:(r0),x0  a,y:(r5)+
            uint32_t tx0 = mem.rdx(mem.R[0]);
            mem.wry(mem.R[5], acc24sat(Aold)); mem.updR(5, +1);
            B = alu_add(B, al24(x1));
            x0 = tx0;
        }
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // add x1,b  x:(r1)+,r0
        B = alu_add(B, al24(x1));
        B = mcsu(x0, y0, B);                    // macsu x0,y0,b
        {   int64_t Bold = B;                   // asr b  x:(r2)+,x1  y:(r4)+,y0
            uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1);
            uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
            B = acc_asr(Bold, 1);
            x1 = tx1; y0 = ty0;
        }
        for (int i = 0; i < 15; ++i) {          // do #$f
            A = mpsu_neg(x1, y0);               // mpysu -x1,y0,a
            {   int64_t Bold = B;               // add x1,a  x:(r2),x0  b,y:(r5)+
                uint32_t tx0 = mem.rdx(mem.R[2]);
                mem.wry(mem.R[5], acc24sat(Bold)); mem.updR(5, +1);
                A = alu_add(A, al24(x1));
                x0 = tx0;
            }
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // add x1,a  x:(r1)+,r2
            A = alu_add(A, al24(x1));
            A = mcsu(x0, y0, A);                // macsu x0,y0,a
            {   int64_t Aold = A;               // asr a  x:(r0)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = acc_asr(Aold, 1);
                x1 = tx1; y0 = ty0;
            }
            B = mpsu_neg(x1, y0);               // mpysu -x1,y0,b
            {   int64_t Aold = A;               // add x1,b  x:(r0),x0  a,y:(r5)+
                uint32_t tx0 = mem.rdx(mem.R[0]);
                mem.wry(mem.R[5], acc24sat(Aold)); mem.updR(5, +1);
                B = alu_add(B, al24(x1));
                x0 = tx0;
            }
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // add x1,b  x:(r1)+,r0
            B = alu_add(B, al24(x1));
            B = mcsu(x0, y0, B);                // macsu x0,y0,b
            {   int64_t Bold = B;               // asr b  x:(r2)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = acc_asr(Bold, 1);
                x1 = tx1; y0 = ty0;
            }
        }
        x0 = mem.rdy(R6BASE + diffOff);         // move y:(r6+$n),x0
        mem.wry(mem.R[5], acc24sat(B)); mem.updR(5, +1); // move b,y:(r5)+
        // ---- difference -> X:$80-$9F ---------------------------------------------
        mem.R[5] = bufBase;                     // move #>$xx,r5
        mem.R[3] = 0x80u;                       // move #>$80,r3
        B = mem2acc(mem.rdy(mem.R[5])); mem.updR(5, +1); // move y:(r5)+,b
        A = mem2acc(mem.rdy(mem.R[5])); mem.updR(5, +1); // move y:(r5)+,a
        {   uint32_t t = x0; uint32_t tb = acc24sat(B); // sub x0,b  b,x0
            B = alu_sub(B, al24(t)); x0 = tb;
        }
        {   uint32_t t = x0; uint32_t ta = acc24sat(A); // sub x0,a  a,x0
            A = alu_sub(A, al24(t)); x0 = ta;
        }
        for (int i = 0; i < 15; ++i) {          // do #$f
            {   uint32_t tb = acc24sat(B);      // move b,x:(r3)+  y:(r5)+,b
                uint32_t tn = mem.rdy(mem.R[5]); mem.updR(5, +1);
                mem.wrx(mem.R[3], tb); mem.updR(3, +1);
                B = mem2acc(tn);
            }
            {   uint32_t ta = acc24sat(A);      // move a,x:(r3)+  y:(r5)+,a
                uint32_t tn = mem.rdy(mem.R[5]); mem.updR(5, +1);
                mem.wrx(mem.R[3], ta); mem.updR(3, +1);
                A = mem2acc(tn);
            }
            {   uint32_t t = x0; uint32_t tb = acc24sat(B); // sub x0,b  b,x0
                B = alu_sub(B, al24(t)); x0 = tb;
            }
            {   uint32_t t = x0; uint32_t ta = acc24sat(A); // sub x0,a  a,x0
                A = alu_sub(A, al24(t)); x0 = ta;
            }
        }
        mem.wrx(mem.R[3], acc24sat(B)); mem.updR(3, +1); // move b,x:(r3)+
        mem.wrx(mem.R[3], acc24sat(A)); mem.updR(3, +1); // move a,x:(r3)+
        mem.wry(R6BASE + diffOff, x0);          // move x0,y:(r6+$n)
        // ---- TONE LP (state lpOff): X:$80 -> Y:bufBase ----------------------------
        mem.R[5] = 0x80u;                       // move #>$80,r5
        mem.R[2] = 0x144AC7u;                   // move #>$144ac7,r2
        B = mem2acc(mem.rdy(R6BASE + 0x0A));    // move y:(r6+$a),b  (TONE)
        B = acc_asr(B, 16);                     // asr #$10,b,b
        A = mem2acc(mem.rdy(R6BASE + lpOff));   // move y:(r6+$n),a
        mem.N[2] = (int32_t)sgn24(acc24raw(B)); // move b,n2
        mem.R[4] = bufBase;                     // move #>$xx,r4
        x0 = mem.rdx(mem.R[5]); mem.updR(5, +1);// move x:(r5)+,x0
        y0 = mem.rdy(mem.R[2] + (uint32_t)mem.N[2]); // move y:(r2+n2),y0
        for (int i = 0; i < 32; ++i) {          // do #$20
            {   int64_t Aold = A;               // mac y0,x0,a  a,x1  a,y:(r4)+
                mem.wry(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                x1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdx(mem.R[5]); mem.updR(5, +1); // mac -x1,y0,a  x:(r5)+,x0
                A = mac_ss_neg(x1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + lpOff, acc24sat(A));   // move a,y:(r6+$n)
        // ---- ENV gate + shaper gain -----------------------------------------------
        B = mem2acc(mem.rdy(R6BASE + envOff));  // move y:(r6+$n),b
        A = 0;                                  // clr a
        if (B >= al24(0x400000)) {              // cmp #>$400000,b ; blt
            B = alu_sub(B, al24(0x400000));     // sub #>$400000,b
            B = alu_add(B, A);                  // add a,b
            A = B;                              // move b,a
        }
        // ---- peak shaper (state shOff), in-place Y:bufBase -------------------------
        x0 = mem.rdy(R6BASE + shOff);           // move y:(r6+$n),x0
        y1 = acc24sat(A);                       // move a,y1
        mem.R[0] = bufBase;                     // move #>$xx,r0
        mem.R[1] = bufBase;                     // move #>$xx,r1
        for (int i = 0; i < 16; ++i) {          // do #$10
            {   int64_t An = mpy_ss(x0, y1);    // mpy x0,y1,a  y:(r0)+,x1
                uint32_t tx1 = mem.rdy(mem.R[0]); mem.updR(0, +1);
                A = An; x1 = tx1;
            }
            A = acc_asl(A, 4);                  // asl #$4,a,a
            A = alu_add(A, al24(x1)); x0 = x1;  // add x1,a  x1,x0
            {   int64_t Bn = mpy_ss(x0, y1);    // mpy x0,y1,b  y:(r0)+,x1
                uint32_t tx1 = mem.rdy(mem.R[0]); mem.updR(0, +1);
                B = Bn; x1 = tx1;
            }
            B = acc_asl(B, 4);                  // asl #$4,b,b
            B = alu_add(B, al24(x1)); x0 = x1;  // add x1,b  x1,x0
            mem.wry(mem.R[1], acc24sat(A)); mem.updR(1, +1); // move a,y:(r1)+
            mem.wry(mem.R[1], acc24sat(B)); mem.updR(1, +1); // move b,y:(r1)+
        }
        mem.wry(R6BASE + shOff, x0);            // move x0,y:(r6+$n)
    }

public:
    FmMem mem;
    int64_t A = 0, B = 0;
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;
};

} // namespace mnmfm
