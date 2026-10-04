// =============================================================================
// MnmFmStat.hpp — Elektron Monomachine SFX-60/MkII OS 1.32B
// Machine m8 "FM+ STAT" — bit-exact transcription of DSP1 P:$145D12 (INIT),
// P:$145D1D (CONF), P:$145D21-$145EC8 (PROC).
//
// Transcribed instruction-by-instruction from
// decompiled data/03_listings/machines_page_A/08_FM-STAT_full.txt
// (INIT/CONF from machines_full_141000.txt $145D12-$145D20) and verified
// against scripts/dsp_emu.py running the original firmware
// (fm_stat_vectors.txt: 25 knob sets x 16 blocks, output + state).
//
// Topology (two operators + carrier, table ratios):
//   op2 (mod): ratio2 = table Y:$141A80[n], n = floor(((2FRQ<<16)+$8000)*48/2^24)
//          (all 24 entries 1/32..8 reachable; the <<1 of the fractional MPY
//          doubles the *24, hence *48 — the old port missed it and could
//          only reach 12 entries). inc2 = ratio2 * L:5 << 3, GLIDED from the
//          previous block's inc (state $12/$13, ramp >>5). Phase -> X:$20-$3F
//          addresses / Y:$20-$3F fractions, sine LUT interpolation -> Y:$80,
//          first difference (state $1a), TONE one-pole (coef = $144AC7[TONE],
//          state $28), 2VOL gate (silent below 64, $145D8C) with a one-pole
//          feedback smear (gain = gated word, state $1e).
//   op1 (mod): ratio1 = same table (1FRQ), fine multiplier 1FIN =
//          ((1FIN<<16 - $400000) >> 2 + $400000) -> *4 (mpy + asl), then
//          inc1 = ratio1*fin*4 * L:5 << 3, NO glide. Phase state $14/$15,
//          one-sample feedback FM on the read address: wobble = prev sine *
//          (1FB >> 11) (states $1c/$1d, depth from 1FB word >> 11).
//          Sine -> Y:$C0, first difference (state $1b), TONE LP (state $29).
//   levels: 1ENV gated at 64 with a quartic droop recursion (state $2a,
//          CONF inits $7FFFFF); level1 = 1ENV^2<<2 * droop.
//          2VOL gated at 64; level2 = 2VOL^2<<2.
//   mix (X:$80) = level1*op1 + level2*op2 -> TONE LP (state $27) ->
//          carrier phase (state $16/$17, ramped inc from state $18/$19,
//          FM = mix >> 12 per sample).
//   carrier sine interp -> X:$E0-$FF (>>3); quadrature rotator 1 on the
//          interp (consts $0CFCE3/$2BC9CA, states $1f/$20, fb $23/$24) and
//          rotator 2 on the carrier phase ramp (consts $4E63DF/$6F0F12,
//          states $21/$22, fb $25/$26); output = 0.5*(pair sums of rot-2),
//          duplicated, >>1, 32 words.
//   TUNE (r6+$b) is NOT read by PROC (applied by the kernel to the pitch).
// =============================================================================
#pragma once
#include "Import4FmDsp.hpp"

namespace mnmfrqenvfm {

class MnmFmStat {
public:
    // Optional per-stage debug hook (see research stage-diff tooling).
    void (*stageHook)(void* ud, int stage) = nullptr;
    void* stageUser = nullptr;
    inline void hook(int s) { if (stageHook) stageHook(stageUser, s); }

    // ---- INIT P:$145D12-$145D1C ------------------------------------------------
    // lua (r6+$11),r0 ; move #>$14a000,x0 ; move #$0,x1 ;
    // do #<$18 move x1,y:(r0)+ ; x0 -> y:(r6+$10/$14/$16)
    void init() {
        for (int i = 0; i < 0x100; ++i) { mem.X[i] = 0; mem.Y[i] = 0; }
        for (int i = 0; i < 0x40; ++i) mem.st[i] = 0;
        mem.resetRegs();
        mem.R[0] = R6BASE + 0x11;              // lua (r6+$11),r0
        x1 = 0;                                 // move #$0,x1
        for (int i = 0; i < 24; ++i) {          // do #<$18 ; move x1,y:(r0)+
            mem.wry(mem.R[0], x1);
            mem.updR(0, +1);
        }
        x0 = 0x14A000u;                         // move #>$14a000,x0
        mem.wry(R6BASE + 0x10, x0);             // move x0,y:(r6+$10)
        mem.wry(R6BASE + 0x14, x0);             // move x0,y:(r6+$14)
        mem.wry(R6BASE + 0x16, x0);             // move x0,y:(r6+$16)
        A = 0; B = 0;
    }

    // ---- CONF P:$145D1D-$145D20 -------------------------------------------------
    // move #>$7fffff,a ; move a,y:(r6+$2a) — 1ENV droop state = max
    void conf(const uint32_t knob[8]) {
        for (int i = 0; i < 8; ++i)
            mem.st[0x04 + i] = (knob[i] & 0xFFFF) << 16;
        mem.st[0x2A] = 0x7FFFFF;                // $145d1d/$145d1f
    }

    // ---- PROC P:$145D21-$145EC8 -------------------------------------------------
    void proc(int64_t pitchA, uint32_t out[32]) {
        int oidx = 0;
        A = pitchA;                              // 48-bit acc (kernel convention)
        // ---- carrier inc48 = $0BE37C * A -> L:$5 (NO <<1 — unlike PAR/DYN) ------
        x0 = 0x0BE37Cu;                         // $145d21
        y1 = acc24raw(A);                       // $145d23
        y0 = (uint32_t)(A & M24);               // $145d24
        A = mpsu(x0, y0);                       // $145d25
        A = mac_ss(x0, y1, A);                  // $145d26
        mem.wrL(5, A & M48);                    // $145d28
        mem.M[2] = 0x1FFFu;                     // $145d2a
        mem.M[0] = 0x1FFFu;                     // $145d2c
        // ---- op2 ratio: n = ((2FRQ<<16 + $8000) * 24) << 1 >> 24 -----------------
        mem.R[1] = 0x141A80u;                   // $145d2e
        B = mem2acc(mem.rdy(R6BASE + 0x08));    // $145d30 (2FRQ)
        B = alu_add(B, al24(0x8000u));          // $145d31
        x0 = acc24sat(B);                       // $145d33
        x1 = 0x18u;                             // $145d34
        B = mpy_ss(x1, x0);                     // $145d36 (the <<1 doubles 24 -> 48)
        mem.N[1] = (int32_t)sgn24(acc24raw(B)); // $145d37
        x0 = mem.rdy(mem.R[1] + (uint32_t)mem.N[1]); // $145d38 ratio2 word
        // ---- op2 inc = ratio2 * L:5 << 3 ----------------------------------------
        A = sext48(mem.rdL(5));                 // $145d39
        y1 = acc24raw(A);                       // $145d3b
        y0 = (uint32_t)(A & M24);               // $145d3c
        A = mpsu(x0, y0);                       // $145d3d
        A = mac_ss(x0, y1, A);                  // $145d3e
        A = acc_asl(A, 3);                      // $145d3f
        // ---- op2 glide: delta = (new_inc - old_inc $12/$13) >> 5 -----------------
        B = mem2acc(mem.rdy(R6BASE + 0x12));    // $145d40
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x13); // $145d41
        mem.wry(R6BASE + 0x12, acc24raw(A));    // $145d42
        mem.wry(R6BASE + 0x13, (uint32_t)(A & M24)); // $145d43
        A = alu_sub(A, B);                      // $145d44
        A = acc_asr(A, 5);                      // $145d45
        // ---- op2 phase ramp -> X:$20-$3F addresses, Y:$20-$3F fractions ----------
        mem.R[1] = 0x20u;                       // $145d46
        y1 = acc24raw(A);                       // $145d48
        y0 = (uint32_t)(A & M24);               // $145d49
        A = B;                                  // $145d4a tfr b,a (start from old inc)
        B = mem2acc(mem.rdy(R6BASE + 0x10));    // $145d4b (phase $10/$11)
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x11); // $145d4c
        x0 = 0x1FFFu;                           // $145d4d
        x1 = 0x14A000u;                         // $145d4f
        for (int i = 0; i < 32; ++i) {          // $145d51 do #$20 — $145d53-$145d56
            A = alu_add(A, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // $145d53 add y,a
            {   int64_t Bold = B;               // $145d54 add a,b  b,l:(r1)+
                mem.wrL(mem.R[1], Bold & M48);
                mem.updR(1, +1);
                B = alu_add(B, A);
            }
            B = acc_and(B, x0);                 // $145d55
            B = alu_add(B, al24(x1));           // $145d56
        }
        mem.wry(R6BASE + 0x10, acc24raw(B));    // $145d57
        mem.wry(R6BASE + 0x11, (uint32_t)(B & M24)); // $145d58
        hook(0);
        // ---- op2 sine interpolation -> Y:$80-$9F ---------------------------------
        mem.R[1] = 0x20u;                       // $145d59
        mem.R[5] = 0x80u;                       // $145d5b
        mem.R[4] = mem.R[1];                    // $145d5d
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // $145d5e
        y0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $145d5f
        x1 = mem.rdx(mem.R[2]); mem.updR(2, +1);// $145d60
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // $145d61
        for (int i = 0; i < 16; ++i) {          // $145d62 do #$10 — $145d64-$145d6f
            A = mpsu_neg(x1, y0);               // $145d64
            {   uint32_t t = mem.rdx(mem.R[2]); // $145d65 add x1,a  x:(r2),x0
                x0 = t;
            }
            A = alu_add(A, al24(x1));
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // $145d66
            A = alu_add(A, al24(x1));
            A = mcsu(x0, y0, A);                // $145d67
            {   int64_t Aold = A;               // $145d68 asr a  x:(r0)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = acc_asr(Aold, 1);
                x1 = tx1; y0 = ty0;
            }
            B = mpsu_neg(x1, y0);               // $145d69
            {   uint32_t t = mem.rdx(mem.R[0]); // $145d6a add x1,b  x:(r0),x0
                x0 = t;
            }
            B = alu_add(B, al24(x1));
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // $145d6b
            B = alu_add(B, al24(x1));
            B = mcsu(x0, y0, B);                // $145d6c
            {   int64_t Bold = B;               // $145d6d asr b  x:(r2)+,x1  y:(r4)+,y0
                uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = acc_asr(Bold, 1);
                x1 = tx1; y0 = ty0;
            }
            mem.wry(mem.R[5], acc24sat(A)); mem.updR(5, +1); // $145d6e
            mem.wry(mem.R[5], acc24sat(B)); mem.updR(5, +1); // $145d6f
        }
        hook(1);
        // ---- op2 first difference (in-place Y:$80, state $1a) --------------------
        mem.R[4] = 0x80u;                       // $145d70
        mem.R[5] = 0x80u;                       // $145d72
        x0 = mem.rdy(R6BASE + 0x1A);            // $145d74
        for (int i = 0; i < 32; ++i) {          // $145d75 do #$20 — $145d77-$145d79
            {   uint32_t t = mem.rdy(mem.R[5]); mem.updR(5, +1); // $145d77
                B = mem2acc(t);
            }
            {   uint32_t tb = acc24sat(B);      // $145d78 sub x0,b  b,x0
                B = alu_sub(B, al24(x0));
                x0 = tb;
            }
            mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $145d79
        }
        mem.wry(R6BASE + 0x1A, x0);             // $145d7a
        // ---- op2 TONE one-pole (coef $144AC7[TONE], state $28) -------------------
        A = mem2acc(mem.rdy(R6BASE + 0x28));    // $145d7b
        mem.R[5] = 0x80u;                       // $145d7c
        mem.R[4] = 0x80u;                       // $145d7e
        mem.R[2] = 0x144AC7u;                   // $145d80
        B = mem2acc(mem.rdy(R6BASE + 0x0A));    // $145d82 (TONE)
        B = acc_asr(B, 16);                     // $145d83
        mem.N[2] = (int32_t)sgn24(acc24raw(B)); // $145d84 (n2 = TONE knob 0..127)
        x0 = mem.rdy(mem.R[5]); mem.updR(5, +1);// $145d85
        y0 = mem.rdy(mem.R[2] + (uint32_t)mem.N[2]); // $145d86 coef
        for (int i = 0; i < 32; ++i) {          // $145d87 do #$20 — $145d89-$145d8a
            {   int64_t Aold = A;               // $145d89 mac y0,x0,a  a,x1  a,y:(r4)+
                mem.wry(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                x1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdy(mem.R[5]); mem.updR(5, +1); // $145d8a
                A = mac_ss_neg(x1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + 0x28, acc24sat(A));    // $145d8b
        hook(2);
        // ---- 2VOL gate: y1 = max(0, 2VOL - $400000) ------------------------------
        B = mem2acc(mem.rdy(R6BASE + 0x09));    // $145d8c (2VOL)
        A = 0;                                  // $145d8d clr a
        if (B >= al24(0x400000u)) {             // $145d8e cmp ; $145d90 blt
            B = alu_sub(B, al24(0x400000u));    // $145d92
            B = alu_add(A, B);                  // $145d94 add a,b
            A = B;                              // $145d95 move b,a
        }
        y1 = acc24sat(A);                       // $145d96
        // ---- op2 gated feedback smear (state $1e, in-place Y:$80) ----------------
        x0 = mem.rdy(R6BASE + 0x1E);            // $145d97
        mem.R[0] = 0x80u;                       // $145d98
        for (int i = 0; i < 32; ++i) {          // $145d9a do #$20 — $145d9c-$145d9f
            {   int64_t An = mpy_ss(x0, y1);    // $145d9c mpy x0,y1,a  y:(r0),x1
                uint32_t tx1 = mem.rdy(mem.R[0]);
                A = An; x1 = tx1;
            }
            A = acc_asl(A, 4);                  // $145d9d
            A = alu_add(A, al24(x1)); x0 = x1;  // $145d9e add x1,a  x1,x0
            mem.wry(mem.R[0], acc24sat(A)); mem.updR(0, +1); // $145d9f
        }
        mem.wry(R6BASE + 0x1E, x0);             // $145da0
        hook(3);
        // ---- op1 ratio: same table, 1FRQ; 1FIN word multiplier (*4 total) --------
        A = sext48(mem.rdL(5));                 // $145da1
        mem.R[1] = 0x141A80u;                   // $145da3
        B = mem2acc(mem.rdy(R6BASE + 0x04));    // $145da5 (1FRQ)
        B = alu_add(B, al24(0x8000u));          // $145da6
        x0 = acc24sat(B);                       // $145da8
        x1 = 0x18u;                             // $145da9
        B = mpy_ss(x1, x0);                     // $145dab
        mem.N[1] = (int32_t)sgn24(acc24raw(B)); // $145dac
        x0 = mem.rdy(mem.R[1] + (uint32_t)mem.N[1]); // $145dad ratio1 word
        B = mem2acc(mem.rdy(R6BASE + 0x05));    // $145dae (1FIN)
        B = alu_sub(B, al24(0x400000u));        // $145daf
        B = acc_asr(B, 2);                      // $145db1
        B = alu_add(B, al24(0x400000u));        // $145db2
        x1 = acc24sat(B);                       // $145db4
        B = mpy_ss(x1, x0);                     // $145db5 (fin * ratio)
        B = acc_asl(B, 1);                      // $145db6 asl b (total *4)
        x0 = acc24sat(B);                       // $145db7
        // ---- op1 inc = ratio1' * L:5 << 3 ----------------------------------------
        y1 = acc24raw(A);                       // $145db8
        y0 = (uint32_t)(A & M24);               // $145db9
        A = mpsu(x0, y0);                       // $145dba
        A = mac_ss(x0, y1, A);                  // $145dbb
        A = acc_asl(A, 3);                      // $145dbc
        // ---- op1 phase + one-sample 1FB wobble -> raw sine Y:$C0 -----------------
        y1 = acc24raw(A);                       // $145dbd (new inc, NO glide)
        y0 = (uint32_t)(A & M24);               // $145dbe
        B = mem2acc(mem.rdy(R6BASE + 0x14));    // $145dbf (phase $14/$15)
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x15); // $145dc0
        A = mem2acc(mem.rdy(R6BASE + 0x07));    // $145dc1 (1FB)
        A = acc_asr(A, 11);                     // $145dc2
        mem.R[5] = 0xC0u;                       // $145dc3
        x0 = acc24sat(A);                       // $145dc5 (wobble depth)
        A = mem2acc(mem.rdy(R6BASE + 0x1C));    // $145dc6 (wobble state $1c/$1d)
        A = (A & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x1D); // $145dc7
        for (int i = 0; i < 32; ++i) {          // $145dc8 do #$20 — $145dca-$145dd3
            B = alu_add(B, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // $145dca add y,b
            B = alu_add(B, A);                  // $145dcb add a,b
            B = acc_and(B, 0x1FFFu);            // $145dcc
            B = alu_add(B, al24(0x14A000u));    // $145dce
            mem.R[3] = acc24raw(B);             // $145dd0 move b1,r3
            B = alu_sub(B, A);                  // $145dd1 sub a,b
            x1 = mem.rdx(mem.R[3]);             // $145dd2
            {   int64_t An = mpy_ss(x1, x0);    // $145dd3 mpy x1,x0,a  x1,y:(r5)+
                mem.wry(mem.R[5], x1); mem.updR(5, +1);
                A = An;
            }
        }
        mem.wry(R6BASE + 0x14, acc24raw(B));    // $145dd4
        mem.wry(R6BASE + 0x15, (uint32_t)(B & M24)); // $145dd5
        mem.wry(R6BASE + 0x1C, acc24raw(A));    // $145dd6
        mem.wry(R6BASE + 0x1D, (uint32_t)(A & M24)); // $145dd7
        hook(4);
        // ---- op1 first difference (in-place Y:$C0, state $1b) --------------------
        mem.R[5] = 0xC0u;                       // $145dd8
        mem.R[4] = 0xC0u;                       // $145dda
        x0 = mem.rdy(R6BASE + 0x1B);            // $145ddc
        for (int i = 0; i < 32; ++i) {          // $145ddd do #$20 — $145ddf-$145de1
            {   uint32_t t = mem.rdy(mem.R[5]); mem.updR(5, +1); // $145ddf
                B = mem2acc(t);
            }
            {   uint32_t tb = acc24sat(B);      // $145de0
                B = alu_sub(B, al24(x0));
                x0 = tb;
            }
            mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $145de1
        }
        mem.wry(R6BASE + 0x1B, x0);             // $145de2
        // ---- op1 TONE one-pole (coef $144AC7[TONE], state $29) -------------------
        A = mem2acc(mem.rdy(R6BASE + 0x29));    // $145de3
        mem.R[5] = 0xC0u;                       // $145de4
        mem.R[4] = 0xC0u;                       // $145de6
        mem.R[2] = 0x144AC7u;                   // $145de8
        B = mem2acc(mem.rdy(R6BASE + 0x0A));    // $145dea
        B = acc_asr(B, 16);                     // $145deb
        mem.N[2] = (int32_t)sgn24(acc24raw(B)); // $145dec
        x0 = mem.rdy(mem.R[5]); mem.updR(5, +1);// $145ded
        y0 = mem.rdy(mem.R[2] + (uint32_t)mem.N[2]); // $145dee
        for (int i = 0; i < 32; ++i) {          // $145def do #$20 — $145df1-$145df2
            {   int64_t Aold = A;               // $145df1
                mem.wry(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                x1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdy(mem.R[5]); mem.updR(5, +1); // $145df2
                A = mac_ss_neg(x1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + 0x29, acc24sat(A));    // $145df3
        hook(5);
        // ---- level2 = 2VOL^2 << 2 -> y0 ------------------------------------------
        mem.R[0] = 0x80u;                       // $145df4
        mem.R[1] = 0xC0u;                       // $145df6 (op1 buffer, Y:$C0)
        mem.R[2] = 0x80u;                       // $145df8 (mix out, X:$80)
        x0 = mem.rdy(R6BASE + 0x09);            // $145dfa (2VOL)
        A = mpy_ss(x0, x0);                     // $145dfb
        A = acc_asl(A, 2);                      // $145dfc
        y0 = acc24sat(A);                       // $145dfe
        // ---- level1: 1ENV gate + quartic droop (state $2a) -> y1 -----------------
        B = mem2acc(mem.rdy(R6BASE + 0x06));    // $145dff (1ENV)
        y1 = mem.rdy(R6BASE + 0x2A);            // $145e00 (droop state)
        if (B >= al24(0x400000u)) {             // $145e01 cmp ; $145e03 blt
            B = alu_sub(B, al24(0x400000u));    // $145e05
            A = mem2acc(0x7FFFFFu);             // $145e07
            x0 = acc24sat(B);                   // $145e09
            B = mpy_ss(x0, x0);                 // $145e0a
            x0 = acc24sat(B);                   // $145e0b
            A = mac_ss_neg(x0, x0, A);          // $145e0c
            x0 = acc24sat(A);                   // $145e0d
            A = mpy_ss(x0, y1);                 // $145e0e
            y1 = acc24sat(A);                   // $145e10
            mem.wry(R6BASE + 0x2A, y1);         // $145e11
        }
        {   // level1 = 1ENV^2 << 2 * droop ($145e12-$145e17, both paths)
            x0 = mem.rdy(R6BASE + 0x06);        // $145e12
            A = mpy_ss(x0, x0);                 // $145e13
            A = acc_asl(A, 2);                  // $145e14
            x0 = acc24sat(A);                   // $145e15
            A = mpy_ss(x0, y1);                 // $145e16
            y1 = acc24sat(A);                   // $145e17
        }
        hook(6);
        // ---- mix -> X:$80: mix[i] = level1*op1[i] + level2*op2[i] ----------------
        {   uint32_t t = mem.rdy(mem.R[0]); mem.updR(0, +1); // $145e18
            x0 = t;
        }
        {   uint32_t t = mem.rdy(mem.R[1]); mem.updR(1, +1); // $145e19
            x1 = t;
        }
        A = mpy_ss(y0, x0);                     // $145e1a
        {   int64_t Bn = mpy_ss(y1, x1);        // $145e1b mpy y1,x1,b  y:(r0)+,x0
            uint32_t tx0 = mem.rdy(mem.R[0]); mem.updR(0, +1);
            B = Bn; x0 = tx0;
        }
        {   uint32_t tx1 = mem.rdy(mem.R[1]); mem.updR(1, +1); // $145e1c
            B = alu_add(B, A);
            x1 = tx1;
        }
        for (int i = 0; i < 31; ++i) {          // $145e1d do #$1f — $145e1f-$145e21
            {   int64_t Bold = B;               // $145e1f mpy y0,x0,a  b,x:(r2)+
                mem.wrx(mem.R[2], acc24sat(Bold)); mem.updR(2, +1);
                A = mpy_ss(y0, x0);
            }
            {   int64_t Bn = mpy_ss(y1, x1);    // $145e20 mpy y1,x1,b  y:(r0)+,x0
                uint32_t tx0 = mem.rdy(mem.R[0]); mem.updR(0, +1);
                B = Bn; x0 = tx0;
            }
            {   uint32_t tx1 = mem.rdy(mem.R[1]); mem.updR(1, +1); // $145e21
                B = alu_add(B, A);
                x1 = tx1;
            }
        }
        mem.wrx(mem.R[2], acc24sat(B)); mem.updR(2, +1); // $145e22
        hook(7);
        // ---- mix TONE LP (state $27, in-place X:$80) -----------------------------
        A = mem2acc(mem.rdy(R6BASE + 0x27));    // $145e23
        mem.R[5] = 0x80u;                       // $145e24
        mem.R[4] = 0x80u;                       // $145e26
        mem.R[2] = 0x144AC7u;                   // $145e28
        B = mem2acc(mem.rdy(R6BASE + 0x0A));    // $145e2a
        B = acc_asr(B, 16);                     // $145e2b
        mem.N[2] = (int32_t)sgn24(acc24raw(B)); // $145e2c
        x0 = mem.rdx(mem.R[5]); mem.updR(5, +1);// $145e2d (X space!)
        y0 = mem.rdy(mem.R[2] + (uint32_t)mem.N[2]); // $145e2e
        for (int i = 0; i < 32; ++i) {          // $145e2f do #$20 — $145e31-$145e32
            {   int64_t Aold = A;               // $145e31 mac y0,x0,a  a,x:(r4)+  a,y1
                mem.wrx(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
                A = mac_ss(y0, x0, A);
                y1 = acc24sat(Aold);
            }
            {   uint32_t tx0 = mem.rdx(mem.R[5]); mem.updR(5, +1); // $145e32
                A = mac_ss_neg(y1, y0, A);
                x0 = tx0;
            }
        }
        mem.wry(R6BASE + 0x27, acc24sat(A));    // $145e33
        hook(8);
        // ---- carrier phase: ramped inc + FM(mix >> 12) -> X/Y:$20-$3F ------------
        A = sext48(mem.rdL(5));                 // $145e34 (carrier inc)
        B = mem2acc(mem.rdy(R6BASE + 0x18));    // $145e36 (old inc $18/$19)
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x19); // $145e37
        mem.wry(R6BASE + 0x18, acc24raw(A));    // $145e38
        mem.wry(R6BASE + 0x19, (uint32_t)(A & M24)); // $145e39
        A = alu_sub(A, B);                      // $145e3a
        A = acc_asr(A, 5);                      // $145e3b
        mem.R[1] = 0x20u;                       // $145e3c
        mem.R[0] = 0x80u;                       // $145e3e (mix, X space)
        y1 = acc24raw(A);                       // $145e40
        y0 = (uint32_t)(A & M24);               // $145e41
        A = B;                                  // $145e42 tfr b,a (ramp start)
        B = mem2acc(mem.rdy(R6BASE + 0x16));    // $145e43 (carrier phase $16/$17)
        B = (B & ~(int64_t)M24) | (int64_t)mem.rdy(R6BASE + 0x17); // $145e44
        for (int i = 0; i < 32; ++i) {          // $145e45 do #$20 — $145e47-$145e53
            A = alu_add(A, sext48(((int64_t)y1 << 24) | (int64_t)y0)); // $145e47
            {   int64_t Bold = B;               // $145e48 add a,b  b,l:(r1)+
                mem.wrL(mem.R[1], Bold & M48);
                mem.updR(1, +1);
                B = alu_add(B, A);
            }
            x1 = acc24raw(A);                   // $145e49
            x0 = (uint32_t)(A & M24);           // $145e4a
            {   uint32_t t = mem.rdx(mem.R[0]); mem.updR(0, +1); // $145e4b
                A = mem2acc(t);
            }
            A = acc_asr(A, 12);                 // $145e4c
            B = alu_add(B, A);                  // $145e4d (FM by mix)
            B = acc_and(B, 0x1FFFu);            // $145e4e
            B = alu_add(B, al24(0x14A000u));    // $145e50
            A = sext48(((int64_t)x1 << 24) | (int64_t)x0); // $145e52/$145e53
        }
        mem.wry(R6BASE + 0x16, acc24raw(B));    // $145e54
        mem.wry(R6BASE + 0x17, (uint32_t)(B & M24)); // $145e55
        hook(9);
        // ---- carrier sine interpolation -> X:$E0-$FF (>>3) ------------------------
        mem.R[1] = 0x20u;                       // $145e56
        mem.R[3] = 0xE0u;                       // $145e58
        mem.R[4] = mem.R[1];                    // $145e5a
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // $145e5b
        y0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $145e5c
        x1 = mem.rdx(mem.R[2]); mem.updR(2, +1);// $145e5d
        {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // $145e5e
        for (int i = 0; i < 16; ++i) {          // $145e5f do #$10 — $145e61-$145e70
            A = mpsu_neg(x1, y0);               // $145e61
            {   uint32_t t = mem.rdx(mem.R[2]); // $145e62
                x0 = t;
            }
            A = alu_add(A, al24(x1));
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[2] = t; } // $145e63
            A = alu_add(A, al24(x1));
            A = mcsu(x0, y0, A);                // $145e64
            {   int64_t Aold = A;               // $145e65
                uint32_t tx1 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = acc_asr(Aold, 1);
                x1 = tx1; y0 = ty0;
            }
            A = acc_asr(A, 1);                  // $145e66
            A = acc_asr(A, 1);                  // $145e67
            B = mpsu_neg(x1, y0);               // $145e68
            {   uint32_t t = mem.rdx(mem.R[0]); // $145e69
                x0 = t;
            }
            B = alu_add(B, al24(x1));
            {   uint32_t t = mem.rdx(mem.R[1]); mem.updR(1, +1); mem.R[0] = t; } // $145e6a
            B = alu_add(B, al24(x1));
            B = mcsu(x0, y0, B);                // $145e6b
            {   int64_t Bold = B;               // $145e6c
                uint32_t tx1 = mem.rdx(mem.R[2]); mem.updR(2, +1);
                uint32_t ty0 = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = acc_asr(Bold, 1);
                x1 = tx1; y0 = ty0;
            }
            B = acc_asr(B, 1);                  // $145e6d
            B = acc_asr(B, 1);                  // $145e6e
            mem.wrx(mem.R[3], acc24sat(A)); mem.updR(3, +1); // $145e6f
            mem.wrx(mem.R[3], acc24sat(B)); mem.updR(3, +1); // $145e70
        }
        hook(10);
        // ---- rotator 1 (states $1f/$20 prepended to X:$E0, fb $23/$24, out Y:$1E) -
        mem.R[0] = 0xDEu;                       // $145e71
        mem.R[1] = 0xE0u;                       // $145e73
        x0 = mem.rdy(R6BASE + 0x1F);            // $145e75
        mem.wrx(mem.R[0], x0); mem.updR(0, +1); // $145e76
        x0 = mem.rdy(R6BASE + 0x20);            // $145e77
        mem.wrx(mem.R[0], x0); mem.updR(0, -1); // $145e78
        x1 = 0x0CFCE3u;                         // $145e79
        y0 = 0x2BC9CAu;                         // $145e7b
        y1 = mem.rdy(R6BASE + 0x23);            // $145e7d
        B = mem2acc(mem.rdy(R6BASE + 0x24));    // $145e7e
        mem.R[4] = 0x1Eu;                       // $145e7f
        x0 = mem.rdx(mem.R[1]); mem.updR(1, +1);// $145e81
        A = mem2acc(mem.rdx(mem.R[0])); mem.updR(0, +1); // $145e82
        for (int i = 0; i < 16; ++i) {          // $145e83 do #$10 — $145e85-$145e88
            {   int64_t Aold = A;               // $145e85
                uint32_t tx0 = mem.rdx(mem.R[1]);
                mem.wry(mem.R[4], y1); mem.updR(4, +1);
                A = mac_ss(y0, x0, Aold);
                mem.updR(1, +1);
                x0 = tx0;
            }
            {   int64_t Aold = A; int64_t Bold = B; // $145e86
                uint32_t tb = mem.rdx(mem.R[0]);
                A = mac_ss_neg(y1, y0, Aold);
                mem.updR(0, +1);
                B = mem2acc(tb);
                y1 = acc24sat(Bold);
            }
            {   int64_t Bold = B;               // $145e87
                uint32_t tx0 = mem.rdx(mem.R[1]);
                mem.wry(mem.R[4], y1); mem.updR(4, +1);
                B = mac_ss(x1, x0, Bold);
                mem.updR(1, +1);
                x0 = tx0;
            }
            {   int64_t Bold = B; int64_t Aold = A; // $145e88
                uint32_t ta = mem.rdx(mem.R[0]);
                B = mac_ss_neg(y1, x1, Bold);
                mem.updR(0, +1);
                A = mem2acc(ta);
                y1 = acc24sat(Aold);
            }
        }
        mem.wry(mem.R[4], y1); mem.updR(4, +1); // $145e89
        mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $145e8a
        mem.wry(R6BASE + 0x23, y1);             // $145e8b
        mem.wry(R6BASE + 0x24, acc24sat(B));    // $145e8c
        mem.wry(R6BASE + 0x1F, acc24sat(A));    // $145e8d
        x0 = mem.rdx(mem.R[0]); mem.updR(0, +1);// $145e8e
        mem.wry(R6BASE + 0x20, x0);             // $145e8f
        hook(11);
        // ---- rotator 2 (states $21/$22 prepended to Y:$1E, fb $25/$26, out X:$1E) -
        mem.R[4] = 0x1Eu;                       // $145e90
        mem.R[5] = 0x20u;                       // $145e92
        x0 = mem.rdy(R6BASE + 0x21);            // $145e94
        mem.wry(mem.R[4], x0); mem.updR(4, +1); // $145e95
        x0 = mem.rdy(R6BASE + 0x22);            // $145e96
        mem.wry(mem.R[4], x0); mem.updR(4, -1); // $145e97
        y1 = 0x4E63DFu;                         // $145e98
        x0 = 0x6F0F12u;                         // $145e9a
        x1 = mem.rdy(R6BASE + 0x25);            // $145e9c
        B = mem2acc(mem.rdy(R6BASE + 0x26));    // $145e9d
        mem.R[0] = 0x1Eu;                       // $145e9e
        y0 = mem.rdy(mem.R[5]); mem.updR(5, +1);// $145ea0
        A = mem2acc(mem.rdy(mem.R[4])); mem.updR(4, +1); // $145ea1
        for (int i = 0; i < 16; ++i) {          // $145ea2 do #$10 — $145ea4-$145ea7
            {   int64_t Aold = A;               // $145ea4
                uint32_t ty0 = mem.rdy(mem.R[5]);
                mem.wrx(mem.R[0], x1);
                A = mac_ss(y0, x0, Aold);
                mem.updR(0, +1); mem.updR(5, +1);
                y0 = ty0;
            }
            {   int64_t Aold = A; int64_t Bold = B; // $145ea5
                A = mac_ss_neg(x1, x0, Aold);
                x1 = acc24sat(Bold);
                uint32_t tb = mem.rdy(mem.R[4]); mem.updR(4, +1);
                B = mem2acc(tb);
            }
            {   int64_t Bold = B;               // $145ea6
                uint32_t ty0 = mem.rdy(mem.R[5]);
                mem.wrx(mem.R[0], x1);
                B = mac_ss(y1, y0, Bold);
                mem.updR(0, +1); mem.updR(5, +1);
                y0 = ty0;
            }
            {   int64_t Bold = B; int64_t Aold = A; // $145ea7
                B = mac_ss_neg(y1, x1, Bold);
                x1 = acc24sat(Aold);
                uint32_t ta = mem.rdy(mem.R[4]); mem.updR(4, +1);
                A = mem2acc(ta);
            }
        }
        mem.wrx(mem.R[0], x1); mem.updR(0, +1); // $145ea8
        mem.wrx(mem.R[0], acc24sat(B)); mem.updR(0, +1); // $145ea9
        mem.wry(R6BASE + 0x25, x1);             // $145eaa
        mem.wry(R6BASE + 0x26, acc24sat(B));    // $145eab
        mem.wry(R6BASE + 0x21, acc24sat(A));    // $145eac
        x0 = mem.rdy(mem.R[4]); mem.updR(4, +1);// $145ead
        mem.wry(R6BASE + 0x22, x0);             // $145eae
        hook(12);
        // ---- output: 0.5 * pair sums of rot-2 (X:$20-$3F) -> Y:$1F.., dup ---------
        mem.R[0] = 0x20u;                       // $145eaf
        mem.R[4] = 0x1Fu;                       // $145eb1
        y0 = 0x400000u;                         // $145eb3 (0.5)
        x0 = mem.rdx(mem.R[0]); mem.updR(0, +1);// $145eb4
        for (int i = 0; i < 8; ++i) {           // $145eb5 do #$8 — $145eb7-$145eba
            {   int64_t An = mpy_ss(y0, x0);    // $145eb7
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                A = An; x0 = tx0;
            }
            {   int64_t Bold = B;               // $145eb8
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                A = mac_ss(y0, x0, A);
                x0 = tx0;
                mem.wry(mem.R[4], acc24sat(Bold)); mem.updR(4, +1);
            }
            {   int64_t Bn = mpy_ss(y0, x0);    // $145eb9
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                B = Bn; x0 = tx0;
            }
            {   int64_t Aold = A;               // $145eba
                uint32_t tx0 = mem.rdx(mem.R[0]); mem.updR(0, +1);
                B = mac_ss(y0, x0, B);
                x0 = tx0;
                mem.wry(mem.R[4], acc24sat(Aold)); mem.updR(4, +1);
            }
        }
        mem.wry(mem.R[4], acc24sat(B)); mem.updR(4, +1); // $145ebb (AFTER the loop —
        // the do opcode end field is $145EBA; the disassembler label is off by one)
        // ---- output words: read Y:$20.., >>1, duplicate a,a/b,b -------------------
        mem.R[4] = 0x20u;                       // $145ebc
        for (int i = 0; i < 8; ++i) {           // $145ebe do #$8 — $145ec0-$145ec7
            A = mem2acc(mem.rdy(mem.R[4])); mem.updR(4, +1); // $145ec0
            B = mem2acc(mem.rdy(mem.R[4])); mem.updR(4, +1); // $145ec1
            A = acc_asr(A, 1);                  // $145ec2
            B = acc_asr(B, 1);                  // $145ec3
            out[oidx++] = acc24sat(A);          // $145ec4
            out[oidx++] = acc24sat(A);          // $145ec5
            out[oidx++] = acc24sat(B);          // $145ec6
            out[oidx++] = acc24sat(B);          // $145ec7
        }
    }

public:
    FmMem mem;
    int64_t A = 0, B = 0;
    uint32_t x0 = 0, x1 = 0, y0 = 0, y1 = 0;
};

} // namespace mnmfrqenvfm
