// MnmFmExact.hpp — Monomachine FM+ machines (m8 STAT, m9 PAR, m10 DYN)
// Bit-faithful port of the OS 1.32B DSP56300 process code.
//
// Source of truth:
//   * listings  decompiled data/03_listings/machines_page_A/08_FM-STAT_full.txt
//               (P:$145D21-$145EC8), 09_FM-PAR_full.txt (P:$145EDB-$14619C),
//               10_FM-DYN_full.txt (P:$1461C1-...)
//   * tables    Y:$141A80 (24 listed ratios), Y:$144AC7 (128 TONE one-pole coeffs),
//               X:$140000 (2048 steps/octave pitch table) — dumped from dsp1_pmem.bin
//   * oracle    scripts/dsp_emu.py runs the REAL listing; this port is diffed
//               against it word-for-word (see work/fm_fix/ and worklog.md).
//
// Verified facts (emulator oracle, this session):
//   * knob index law:  n = A1( 2*24*(Kword+$8000) ) = (24*(K<<16+$8000))>>23
//     (fractional-mode mpy doubles the product; the older mnm_fm_exact.hpp
//      used >>24 and lost the top half of the ratio table)
//   * mod-1 ratio = table[n] * 2 * (1 + (FIN-64)/256);  mod-2 ratio = table[n]
//   * carrier runs at the raw pitch word (L5 = 2*$BE37C*pitch), phase-modulated
//     by (mod1*w1 + mod2*w2) >> 12 per sample
//   * 1FB: one-sample phase feedback on mod-1 readout, depth = 1FB*32 table units
//   * 1ENV: weight 8*K^2>>24 * level, level decays per block when 1ENV>=64
//     (config $145D1D sets level=$7FFFFF at note-on = the FM+ envelope)
//   * 2VOL: weight 8*K^2>>24 + the gated shaper in[i]+in[i-1]*(K-64)/256
//   * TUNE never enters the process (kernel adds it to the pitch word)
//   * both mod paths: sine -> first difference -> TONE one-pole (Y:$144AC7)
//   * output: 16 mono samples per 32-sample block (half rate), L=R duplicated
//   * TONE knob index = K (the raw knob value, Kword>>16)
//
// The sine table at X:$14A000 is NOT present in any available dump (SRAM init
// region); a perfect 8192-point S1.23 sine is used identically in the oracle
// and here, so all comparisons remain exact.  If the real table ever surfaces,
// drop it into setSineTable().
//
// Q23 fixed point throughout, DSP56300 fractional MACs (product<<1), 56-bit
// accumulators. No JUCE dependency: plain C++17, drop-in for the plugin.
#pragma once
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <cstdio>

namespace mnmfm {

// verification hook: called at section boundaries when set (test only)
struct FmStatVoice;
extern void (*gCkpt)(int id, void* voice);

static constexpr int64_t M24 = 0xFFFFFFll;
static constexpr int64_t M56 = (1ll << 56) - 1;

static inline int32_t s24(int64_t v) {
    v &= M24;
    return (int32_t)(v - ((v >> 23) & 1 ? (1ll << 24) : 0));
}
static inline int64_t AL(int64_t v) {            // 24-bit reg -> acc alignment
    return ((int64_t)s24(v) << 24) & M56;
}
static inline int32_t A1s(int64_t acc) {         // acc -> 24-bit (A1 field) WITH
    // DSP56300 data-limit checking (FM 5.4.1.2): A/B transfers to memory or
    // X0/Y0-class registers saturate; verified against emulator dumps.
    int64_t v = (acc & (1ll << 55)) ? (acc - (1ll << 56)) : acc;
    if (v > 0x007FFFFFFFFFFFll) return 0x7FFFFF;
    if (v < -0x00800000000000ll) return (int32_t)0x800000;
    return (int32_t)((acc >> 24) & M24);
}
static inline int32_t A1(int64_t acc) {          // alias: whole-acc = saturating
    return A1s(acc);
}
static inline int32_t A1r(int64_t acc) {         // explicit a1/b1 part move: NO limit check
    return (int32_t)((acc >> 24) & M24);
}
static inline int32_t A0(int64_t acc) { return (int32_t)(acc & M24); }
static inline int64_t asr56(int64_t acc, int n) {
    int64_t v = (acc & (1ll << 55)) ? (acc - (1ll << 56)) : acc;
    return (v >> n) & M56;
}
static inline int64_t mpy_ss(int64_t a, int64_t b) { return (2 * (int64_t)s24(a) * s24(b)) & M56; }
static inline int64_t mac_ss(int64_t acc, int64_t a, int64_t b) {
    return (acc + 2 * (int64_t)s24(a) * s24(b)) & M56;
}
// mpysu x0,y0,a ; dmac ss x0,y1,a  (y48 = 48-bit acc source: A1:A0)
static inline int64_t mpysu_dmac(int64_t x0, int64_t y48) {
    int64_t lo = y48 & M24;
    int64_t hi = (y48 >> 24) & M24;
    int64_t a = (2 * (int64_t)s24(x0) * lo) & M56;          // mpysu: s*u
    return (a + 2 * (int64_t)s24(x0) * s24(hi)) & M56;      // dmac ss
}

// ---- P:$141880: 128-word curve table (1FEN/1VEN recursion, raw dump) ----
static const int32_t kDynCurve[128] = {
    0x8D1897,0x8C4936,0x8B8603,0x8ACE5A,0x8A21A0,0x897F3D,0x88E6A2,0x885744,
    0x87D09F,0x875234,0x86DB8C,0x866C33,0x8603BC,0x85A1C2,0x8545E0,0x84EFBC,
    0x849EFC,0x84534D,0x840C62,0x83C9EF,0x838BB0,0x835161,0x831AC5,0x82E7A2,
    0x82B7C0,0x828AEB,0x8260F2,0x8239A8,0x8214E2,0x81F277,0x81D240,0x81B41B,
    0x8197E6,0x817D81,0x8164CF,0x814DB4,0x813816,0x8123DD,0x8110F3,0x80FF40,
    0x80EEB3,0x80DF37,0x80D0BC,0x80C330,0x80B685,0x80AAAB,0x809F97,0x809539,
    0x808B88,0x808278,0x8079FE,0x807211,0x806AA7,0x8063B9,0x805D3E,0x80572E,
    0x805183,0x804C37,0x804742,0x8042A0,0x803E4B,0x803A3E,0x803674,0x8032EA,
    0x802F9A,0x802C81,0x80299C,0x8026E7,0x80245F,0x802202,0x801FCB,0x801DBA,
    0x801BCB,0x8019FC,0x80184B,0x8016B6,0x80153C,0x8013DA,0x801290,0x80115A,
    0x801039,0x800F2B,0x800E2E,0x800D42,0x800C65,0x800B97,0x800AD6,0x800A21,
    0x800978,0x8008DB,0x800847,0x8007BD,0x80073C,0x8006C4,0x800653,0x8005EA,
    0x800587,0x80052B,0x8004D5,0x800484,0x800439,0x8003F3,0x8003B1,0x800374,
    0x80033A,0x800304,0x8002D2,0x8002A3,0x800277,0x80024E,0x800228,0x800204,
    0x8001E2,0x8001C3,0x8001A5,0x80018A,0x800170,0x800158,0x800142,0x80012D,
    0x800119,0x800107,0x8000F6,0x8000E6,0x8000D7,0x8000C9,0x8000BC,0x800000
};
// ---- Y:$141A80: 24 listed-frequency words (raw dump) ------------------------
static const int32_t kRatioRaw[24] = {
    0x004000,0x008000,0x010000,0x018000,0x020000,0x028000,0x030000,0x040000,
    0x050000,0x060000,0x070000,0x080000,0x0A0000,0x0C0000,0x0E0000,0x100000,
    0x140000,0x180000,0x1C0000,0x200000,0x280000,0x300000,0x380000,0x400000
};
// ---- Y:$144AC7: 128 TONE one-pole coefficients (raw dump) ------------------
static const int32_t kToneCoeff[128] = {
    0x004876,0x004CE3,0x005196,0x005692,0x005BDC,0x006179,0x00676D,0x006DBE,
    0x007472,0x007B8E,0x008319,0x008B1A,0x009398,0x009C9A,0x00A629,0x00B04C,
    0x00BB0E,0x00C676,0x00D291,0x00DF67,0x00ED06,0x00FB78,0x010ACA,0x011B0A,
    0x012C47,0x013E8F,0x0151F2,0x016682,0x017C51,0x019370,0x01ABF5,0x01C5F5,
    0x01E187,0x01FEC1,0x021DBE,0x023E98,0x02616B,0x028654,0x02AD74,0x02D6EB,
    0x0302DD,0x03316E,0x0362C5,0x03970B,0x03CE6B,0x040914,0x044735,0x048900,
    0x04CEAC,0x05186E,0x056684,0x05B928,0x06109E,0x066D27,0x06CF0C,0x073696,
    0x07A413,0x0817D4,0x08922E,0x091378,0x099C0E,0x0A2C4E,0x0AC49D,0x0B655E,
    0x0C0EFD,0x0CC1E5,0x0D7E86,0x0E4553,0x0F16C0,0x0FF345,0x10DB5B,0x11CF7C,
    0x12D023,0x13DDCC,0x14F8F2,0x16220C,0x175993,0x189FF7,0x19F5A8,0x1B5B0B,
    0x1CD081,0x1E565E,0x1FECEC,0x219466,0x234CF9,0x2516BD,0x26F1B5,0x28DDCF,
    0x2ADADC,0x2CE891,0x2F0683,0x313423,0x3370BD,0x35BB79,0x381350,0x3A7714,
    0x3CE56A,0x3F5CCB,0x41DB85,0x445FBA,0x46E76A,0x49706C,0x4BF87C,0x4E7D40,
    0x50FC49,0x537323,0x55DF5D,0x583E8D,0x5A8E63,0x5CCCAB,0x5EF75C,0x610C9C,
    0x630ACC,0x64F089,0x66BCB0,0x686E62,0x6A04FD,0x6B801F,0x6CDF98,0x6E236B,
    0x6F4BBA,0x7058C1,0x714AC1,0x7221F3,0x72DE71,0x73801D,0x74067D,0x747093
};
// X:$140000 pitch table: 2048 steps/octave, entry = 2^(i/2048 - 1) in S1.23.
// Only the shape matters here; the port computes the pitch word directly.
static inline int32_t pitchWordForNote(float note) {
    // word = 2^(note/12 - 1) * 2^23, i.e. table[(note*2048/12) mod 2048] << octave
    double semis = note / 12.0;
    double oct = std::floor(semis);
    double frac = semis - oct;                       // 0..1 within the octave
    int idx = (int)(frac * 2048.0 + 0.5) & 0x7FF;
    double v = std::pow(2.0, frac) * 0.5 * std::pow(2.0, oct);
    (void)idx;
    int64_t w = (int64_t)(v * 8388608.0 + 0.5);
    if (w > 0x7FFFFF) w = 0x7FFFFF;
    if (w < 0) w = 0;
    return (int32_t)w;
}

static std::array<int32_t, 8192> gSine;
static inline void initSineTable() {
    for (int i = 0; i < 8192; ++i)
        gSine[i] = (int32_t)(std::lround(std::sin(2.0 * M_PI * i / 8192.0) * 8388607.0) & M24);
}
static inline int32_t sineRead(int addr) {         // x:(r) with m2=$1fff modulo
    return gSine[(addr - 0x14A000) & 0x1FFF];
}

// ============================================================================
// FM+ STAT (m8) — instruction-faithful mirror of P:$145D21-$145EC8
// ============================================================================
void (*gCkpt)(int id, void* voice) = nullptr;

struct FmStatVoice {
    // page words y:(r6+$10..$2A)
    int32_t w[0x2B];
    // scratch
    int32_t L20X[32], L20Y[32];
    int32_t X80[32], Y80[32], YC0[32], XE0[32];
    int32_t Y1E[40], X1E[40];   // rotator scratch (exposed for verification)

    FmStatVoice() { init(); }
    void init() {                                   // $145D12 + $145D1D
        for (int i = 0; i < 0x2B; ++i) w[i] = 0;
        w[0x10] = 0x14A000; w[0x14] = 0x14A000; w[0x16] = 0x14A000;
        w[0x29] = 0x061080;                         // NOT covered by init's zero range
                                                    // ($11..$28): SRAM residue at page+$29
                                                    // in the reference emulator run
        w[0x2A] = 0x7FFFFF;                         // config: FM+ envelope level
    }
    // knobs: 8 raw values 0..127 (1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE)
    void process(const int knobs[8], int32_t pitch, int32_t out[32]) {
        int64_t Kw[8];
        for (int i = 0; i < 8; ++i) {
            Kw[i] = (int64_t)(knobs[i] & 0xFF) << 16;
            w[4 + i] = (int32_t)Kw[i];           // kernel writes knob words into the page
        }

        // ---- $145d21: L5 = 2*$BE37C*pitch ----
        int64_t L5 = mpysu_dmac(0x0BE37C, AL(pitch));

        // ---- mod-2 (2FRQ) increment: $145d2e-$145d3f ----
        int64_t b = mpy_ss(0x18, (Kw[4] + 0x8000) & M24);
        int n1 = A1r(b);
        int64_t raw2 = (n1 >= 0 && n1 < 24) ? kRatioRaw[n1] : 0;
        int64_t a = mpysu_dmac(raw2, L5 & ((1ll << 48) - 1));
        a = (a << 3) & M56;                          // asl #$3

        // ---- phase loop mod-2 -> L:$20 ($145d40-$145d58) ----
        b = (AL(w[0x12]) | w[0x13]) & M56;
        w[0x12] = A1r(a); w[0x13] = A0(a);
        int64_t delta = asr56((a - b) & M56, 5);
        int64_t y48 = delta;
        a = b;                                       // tfr b,a
        b = (AL(w[0x10]) | w[0x11]) & M56;
        for (int i = 0; i < 32; ++i) {
            L20X[i] = A1r(b); L20Y[i] = A0(b);       // b,l:(r1)+ latched PRE-ALU
            a = (a + y48) & M56;                     // add y,a
            b = (b + a) & M56;                       // add a,b
            b = (AL(A1r(b) & 0x1FFF) | A0(b)) & M56;  // and #$1fff,b (A1 only)
            b = (b + AL(0x14A000)) & M56;            // add #$14a000,b
        }
        w[0x10] = A1r(b); w[0x11] = A0(b);
        if (gCkpt) gCkpt(1, this);

        // ---- interpolation -> Y:$80 ($145d59-$145d70) ----
        for (int i = 0; i < 32; ++i) {
            int32_t s0 = sineRead(L20X[i]);
            int32_t s1 = sineRead(L20X[i] + 1);
            int64_t acc = (2 * ((int64_t)s24(s0) << 24)) & M56;   // mpysu -x1 + 2 add x1
            acc = (acc + 2 * (int64_t)s24(s1) * L20Y[i]) & M56;   // macsu x0,y0
            acc = (acc - 2 * (int64_t)s24(s0) * L20Y[i]) & M56;
            Y80[i] = A1(asr56(acc, 1));
        }

        if (gCkpt) gCkpt(2, this);
        // ---- first-difference cascade, state $1A ($145d70-$145d7a) ----
        {
            int64_t x0 = w[0x1A];
            for (int i = 0; i < 32; ++i) {
                int64_t bb = AL(Y80[i]);
                int32_t nx = A1(bb);                 // b,x0 latched pre-ALU
                bb = (bb - AL(x0)) & M56;            // sub x0,b
                Y80[i] = A1(bb);
                x0 = nx;
            }
            w[0x1A] = (int32_t)x0;
        }

        if (gCkpt) gCkpt(3, this);
        // ---- TONE one-pole, state $28 ($145d7b-$145d8b) ----
        int n2 = (int)((w[0x0A] >> 16) & 0x7F);      // asr #$10 -> knob 0..127
        int64_t y0c = kToneCoeff[n2];
        {
            int64_t aa = AL(w[0x28]);
            int64_t x0 = Y80[0];
            for (int i = 0; i < 32; ++i) {
                int32_t x1l = A1(aa);                // a,x1 ; a,y:(r4)+ latched
                Y80[i] = x1l;
                aa = mac_ss(aa, y0c, x0);            // mac y0,x0,a
                aa = (aa - 2 * (int64_t)s24(x1l) * s24(y0c)) & M56;  // mac -x1,y0,a
                if (i < 31) x0 = Y80[i + 1];
            }
            w[0x28] = A1(aa);
        }

        if (gCkpt) gCkpt(4, this);
        // ---- 2VOL gated shaper, state $1E ($145d8c-$145da0) ----
        {
            int64_t y1 = (Kw[5] >= 0x400000) ? (Kw[5] - 0x400000) & M24 : 0;
            int64_t x0 = w[0x1E];
            for (int i = 0; i < 32; ++i) {
                int64_t aa = mpy_ss(x0, y1);         // mpy x0,y1,a
                aa = (aa << 4) & M56;                // asl #$4
                int32_t x1 = Y80[i];                 // y:(r0),x1
                aa = (aa + AL(x1)) & M56;            // add x1,a
                int32_t x0n = x1;                    // x1,x0 latched pre-ALU
                Y80[i] = A1(aa);                     // move a,y:(r0)+
                x0 = x0n;
            }
            w[0x1E] = (int32_t)x0;
        }

        if (gCkpt) gCkpt(5, this);
        // ---- mod-1 (1FRQ/1FIN) increment ($145da1-$145dbc) ----
        b = mpy_ss(0x18, (Kw[0] + 0x8000) & M24);
        n1 = A1(b);
        int64_t raw1 = (n1 >= 0 && n1 < 24) ? kRatioRaw[n1] : 0;
        b = (AL(Kw[1]) - AL(0x400000)) & M56;        // sub #$400000
        b = asr56(b, 2);                             // asr #$2
        b = (b + AL(0x400000)) & M56;                // add #$400000
        int64_t x1f = A1(b);
        b = mpy_ss(x1f, raw1);                       // mpy x1,x0,b
        b = (b << 1) & M56;                          // asl b
        int64_t x0r = A1(b);
        a = mpysu_dmac(x0r, L5 & ((1ll << 48) - 1));
        a = (a << 3) & M56;
        int64_t inc1 = a;

        // ---- warp loop: mod-1 sine -> YC0, 1FB one-sample feedback ($145dbf) ----
        {
            b = (AL(w[0x14]) | w[0x15]) & M56;       // phM
            int64_t x0d = A1(asr56(AL(Kw[3]), 11));  // 1FB*32 (13-bit word)
            a = (AL(w[0x1C]) | w[0x1D]) & M56;       // fb1 state
            for (int i = 0; i < 32; ++i) {
                b = (b + inc1) & M56;                // add y,b
                b = (b + a) & M56;                   // add a,b
                b = (AL(A1r(b) & 0x1FFF) | A0(b)) & M56;
                b = (b + AL(0x14A000)) & M56;
                int32_t r3 = A1r(b);
                b = (b - a) & M56;                   // sub a,b
                int32_t x1 = sineRead(r3);           // move x:(r3),x1
                YC0[i] = x1;                         // x1,y:(r5)+
                a = mpy_ss(x1, x0d);                 // mpy x1,x0,a
            }
            w[0x14] = A1r(b); w[0x15] = A0(b);
            w[0x1C] = A1r(a); w[0x1D] = A0(a);
        }

        if (gCkpt) gCkpt(6, this);
        // ---- diff cascade YC0, state $1B ($145dd8-$145de2) ----
        {
            int64_t x0 = w[0x1B];
            for (int i = 0; i < 32; ++i) {
                int64_t bb = AL(YC0[i]);
                int32_t nx = A1(bb);
                bb = (bb - AL(x0)) & M56;
                YC0[i] = A1(bb);
                x0 = nx;
            }
            w[0x1B] = (int32_t)x0;
        }

        if (gCkpt) gCkpt(7, this);
        // ---- TONE one-pole YC0, state $29 ($145de3-$145df3) ----
        {
            int64_t aa = AL(w[0x29]);
            int64_t x0 = YC0[0];
            for (int i = 0; i < 32; ++i) {
                int32_t x1l = A1(aa);
                YC0[i] = x1l;
                aa = mac_ss(aa, y0c, x0);
                aa = (aa - 2 * (int64_t)s24(x1l) * s24(y0c)) & M56;
                if (i < 31) x0 = YC0[i + 1];
            }
            w[0x29] = A1(aa);
        }

        if (gCkpt) gCkpt(12, this);
        // ---- FM+ envelope + mix weights ($145df4-$145e17) ----
        int64_t y0w, y1w;
        {
            int64_t x0 = Kw[5];
            int64_t aa = mpy_ss(x0, x0);
            aa = (aa << 2) & M56;
            y0w = A1(aa);                            // w2
            int64_t bb = AL(Kw[2]);
            int64_t y1 = w[0x2A];
            if (Kw[2] >= 0x400000) {
                bb = (bb - AL(0x400000)) & M56;
                int64_t aa2 = AL(0x7FFFFF);
                int64_t x0b = A1(bb);
                int64_t b2 = mpy_ss(x0b, x0b);
                x0b = A1(b2);
                aa2 = (aa2 - 2 * (int64_t)s24(x0b) * s24(x0b)) & M56;
                x0b = A1(aa2);
                aa2 = mpy_ss(x0b, y1);
                y1 = A1(aa2);
                w[0x2A] = (int32_t)y1;
            }
            int64_t x0d = Kw[2];
            int64_t aa3 = mpy_ss(x0d, x0d);
            aa3 = (aa3 << 2) & M56;
            int64_t x0c = A1(aa3);
            aa3 = mpy_ss(x0c, y1);
            y1w = A1(aa3);                           // w1
        }

        // ---- mix -> X:$80 ($145e18-$145e23) ----
        for (int i = 0; i < 32; ++i) {
            int64_t aa = mpy_ss(y0w, Y80[i]);
            int64_t bb = mpy_ss(y1w, YC0[i]);
            bb = (bb + aa) & M56;
            X80[i] = A1(bb);
        }

        if (gCkpt) gCkpt(8, this);
        // ---- TONE one-pole on the mix, IN PLACE in X:$80, state $27 ----
        // ($145e23-$145e33): output store is a,x:(r4)+ (X space!) and the
        // feedback latch is a,y1 (whole-acc -> y1, saturating).
        {
            int64_t aa = AL(w[0x27]);
            int64_t x0 = X80[0];
            for (int i = 0; i < 32; ++i) {
                int32_t y1l = A1(aa);            // a,y1 ; a,x:(r4)+ latched pre-ALU
                X80[i] = y1l;                    // output overwrites the mix
                aa = mac_ss(aa, y0c, x0);        // mac y0,x0,a
                aa = (aa - 2 * (int64_t)s24(y1l) * s24(y0c)) & M56;  // mac -y1,y0,a
                if (i < 31) x0 = X80[i + 1];
            }
            w[0x27] = A1(aa);
        }

        if (gCkpt) gCkpt(13, this);
        // ---- carrier loop: phC += ramped L5 + mix>>12 ($145e34-$145e55) ----
        {
            a = L5;
            b = (AL(w[0x18]) | w[0x19]) & M56;
            w[0x18] = A1r(a); w[0x19] = A0(a);
            int64_t delta2 = asr56((a - b) & M56, 5);
            int64_t y48 = delta2;
            a = b;
            b = (AL(w[0x16]) | w[0x17]) & M56;
            for (int i = 0; i < 32; ++i) {
                L20X[i] = A1r(b); L20Y[i] = A0(b);   // b,l:(r1)+ latched PRE-ALU
                a = (a + y48) & M56;                 // add y,a
                b = (b + a) & M56;                   // add a,b
                int64_t a_save = a;                  // move a1,x1 / a0,x0
                int64_t am = asr56(AL(X80[i]), 12);  // move x:(r0)+,a ; asr #$c
                b = (b + am) & M56;                  // add a,b
                b = (AL(A1r(b) & 0x1FFF) | A0(b)) & M56;
                b = (b + AL(0x14A000)) & M56;
                a = a_save;                          // move x1,a / x0,a0
            }
            w[0x16] = A1r(b); w[0x17] = A0(b);
        }

        if (gCkpt) gCkpt(9, this);
        // ---- carrier interpolation -> X:$E0 ($145e56-$145e70), 3 asr ----
        for (int i = 0; i < 32; ++i) {
            int32_t s0 = sineRead(L20X[i]);
            int32_t s1 = sineRead(L20X[i] + 1);
            int64_t acc = (2 * ((int64_t)s24(s0) << 24)) & M56;
            acc = (acc + 2 * (int64_t)s24(s1) * L20Y[i]) & M56;
            acc = (acc - 2 * (int64_t)s24(s0) * L20Y[i]) & M56;
            XE0[i] = A1(asr56(acc, 3));
        }

        // ---- rotator 1 ($145e71-$145e8f) ----
        // X:$DE/$DF preloaded with states $1F/$20, then the carrier stream:
        // XDE[0]=st$1F, XDE[1]=st$20, XDE[2+i]=XE0[i].
        // mac x0 stream: XDE[2],XDE[3],XDE[4]... (consecutive);
        // 145e86 b-reads: XDE[1],XDE[3],XDE[5]... ; 145e88 a-reads: XDE[2],XDE[4],...
        int32_t XDE[40] = {0};
        XDE[0] = w[0x1F]; XDE[1] = w[0x20];
        for (int i = 0; i < 32; ++i) XDE[2 + i] = XE0[i];
        int32_t (&Y1E)[40] = this->Y1E;              // y:(r4)+ chain, Y:$1E..
        {
            const int64_t x1c = 0x0CFCE3, y0c2 = 0x2BC9CA;
            int64_t y1 = w[0x23];
            int64_t bb = AL(w[0x24]);
            int64_t aa = AL(XDE[0]);                 // move x:(r0)+,a  (X:$DE)
            int64_t x0 = XDE[2];                     // move x:(r1)+,x0 (X:$E0)
            int chain = 0;
            for (int k = 0; k < 16; ++k) {
#ifdef MNMFM_DBG_ROT
                if (k < 2) printf("k=%d pre: aa=%014llx bb=%014llx x0=%06llX y1=%06llX\n", k,
                    (unsigned long long)aa, (unsigned long long)bb,
                    (unsigned long long)x0, (unsigned long long)y1);
#endif
                // 145e85: mac y0,x0,a ; x:(r1)+,x0 ; y1,y:(r4)+
                int64_t y1l = y1;                    // latched pre-ALU
                aa = mac_ss(aa, y0c2, x0);
                x0 = XDE[3 + 2 * k];
                Y1E[chain++] = (int32_t)y1l;
                // 145e86: mac -y1,y0,a ; x:(r0)+,b ; b,y1
                aa = (aa - 2 * (int64_t)s24(y1) * s24(y0c2)) & M56;
                int32_t bn = XDE[1 + 2 * k];
                y1 = A1(bb);                         // b,y1 latched pre-ALU
                bb = AL(bn);
                // 145e87: mac x1,x0,b ; x:(r1)+,x0 ; y1,y:(r4)+
                int64_t y1l2 = y1;
                bb = mac_ss(bb, x1c, x0);
                x0 = XDE[4 + 2 * k];
                Y1E[chain++] = (int32_t)y1l2;
                // 145e88: mac -y1,x1,b ; x:(r0)+,a ; a,y1
                bb = (bb - 2 * (int64_t)s24(y1) * s24(x1c)) & M56;
                int32_t an = XDE[2 + 2 * k];
                y1 = A1(aa);                         // a,y1 latched pre-ALU
                aa = AL(an);
#ifdef MNMFM_DBG_ROT
                if (k < 2) printf("   post: aa=%014llx bb=%014llx y1=%06llX chain=%06X/%06X\n",
                    (unsigned long long)aa, (unsigned long long)bb,
                    (unsigned long long)y1,
                    (unsigned long long)(int64_t)Y1E[chain-2], (unsigned long long)(int64_t)Y1E[chain-1]);
#endif
            }
            Y1E[32] = (int32_t)y1;                   // 145e89
            Y1E[33] = A1s(bb);                       // 145e8a
            w[0x23] = (int32_t)y1;                   // 145e8b (reg)
            w[0x24] = A1s(bb);                       // 145e8c (whole acc)
            w[0x1F] = A1s(aa);                       // 145e8d (whole acc)
            w[0x20] = XDE[33];                       // 145e8e/f (scratch read X:$FF)
        }
        if (gCkpt) gCkpt(14, this);

        if (gCkpt) gCkpt(10, this);
        // ---- rotator 2 ($145e90-$145eae) -> X:$1E.. ----
        // Ymem[$1E..$3F] = [st$21, st$22, Y1E[2..33]] (states overwrite chain[0..1])
        {
            const int64_t y1c = 0x4E63DF, x0c = 0x6F0F12;
            int32_t Ym[40] = {0};
            Ym[0] = w[0x21]; Ym[1] = w[0x22];
            for (int i = 2; i < 40; ++i) Ym[i] = Y1E[i];
            int64_t y1 = y1c;                        // const (register y1)
            int64_t x1 = w[0x25];
            int64_t bb = AL(w[0x26]);
            int64_t x0 = x0c;
            int64_t y0 = Ym[2];                      // move y:(r5)+,y0 (Y:$20)
            int64_t aa = AL(Ym[0]);                  // move y:(r4)+,a  (Y:$1E)
            int32_t (&X1E)[40] = this->X1E;
            for (int k = 0; k < 16; ++k) {
                // 145ea4: mac y0,x0,a ; x1,x:(r0)+ ; y:(r5)+,y0
                int32_t x1l = (int32_t)x1;
                aa = mac_ss(aa, y0, x0);
                X1E[2 * k] = x1l;
                y0 = Ym[3 + 2 * k];
                // 145ea5: mac -x1,x0,a ; b,x1 ; y:(r4)+,b
                aa = (aa - 2 * (int64_t)s24(x1) * s24(x0c)) & M56;
                int64_t x1n = x1;
                x1 = A1(bb);                         // b,x1 latched pre-ALU
                bb = AL(Ym[1 + 2 * k]);
                // 145ea6: mac y1,y0,b ; x1,x:(r0)+ ; y:(r5)+,y0
                int32_t x1l2 = (int32_t)x1;
                bb = mac_ss(bb, y0, y1c);
                X1E[1 + 2 * k] = x1l2;
                y0 = Ym[4 + 2 * k];
                // 145ea7: mac -y1,x1,b ; a,x1 ; y:(r4)+,a
                bb = (bb - 2 * (int64_t)s24(y1c) * s24(x1)) & M56;
                x1 = A1(aa);                         // a,x1 latched pre-ALU
                aa = AL(Ym[2 + 2 * k]);
                (void)x1n;
            }
            X1E[32] = (int32_t)x1;                   // 145ea8
            X1E[33] = A1s(bb);                       // 145ea9
            w[0x25] = (int32_t)x1;                   // 145eaa (reg)
            w[0x26] = A1s(bb);                       // 145eab (whole acc)
            w[0x21] = A1s(aa);                       // 145eac (whole acc)
            w[0x22] = Ym[33];                        // 145ead/ae (Y:$3F)
            if (gCkpt) gCkpt(15, this);

            // ---- output ($145eaf-$145ec8): pair-sum of X:$20.. ×64 ----
            int32_t Yout[18];                        // Y:$1F.. (r4=$1F)
            {
                const int64_t y0g = 0x400000;   // "move #$40,y0": 8-bit imm $40 -> $400000 = 0.5
                int r0i2 = 0;                        // X:$20.. = X1E[2..]
                int r4i2 = 0;
                int64_t aa2 = 0, bb2 = 0;
                int64_t x0g = X1E[2 + r0i2]; r0i2++;
                for (int k = 0; k < 8; ++k) {
                    aa2 = mpy_ss(y0g, x0g);          // 145eb7
                    x0g = X1E[2 + r0i2]; r0i2++;
                    aa2 = mac_ss(aa2, y0g, x0g);     // 145eb8
                    x0g = X1E[2 + r0i2]; r0i2++;
                    Yout[r4i2++] = A1(bb2);          // b,y:(r4)+
                    bb2 = mpy_ss(y0g, x0g);          // 145eb9
                    x0g = X1E[2 + r0i2]; r0i2++;
                    bb2 = mac_ss(bb2, y0g, x0g);     // 145eba
                    x0g = X1E[2 + r0i2]; r0i2++;
                    Yout[r4i2++] = A1(aa2);          // a,y:(r4)+
                }
                Yout[r4i2++] = A1(bb2);              // 145ebb (Y:$2F)
            }
            // 145ebe-$145ec7: read Y:$20..$2F (= Yout[1..16]), asr, duplicate L/R
            for (int k = 0; k < 8; ++k) {
                int64_t aa2 = asr56(AL(Yout[1 + 2 * k]), 1);
                int64_t bb2 = asr56(AL(Yout[2 + 2 * k]), 1);
                out[4 * k + 0] = A1(aa2);
                out[4 * k + 1] = A1(aa2);
                out[4 * k + 2] = A1(bb2);
                out[4 * k + 3] = A1(bb2);
            }
        }
    }
};

#include "MnmFmPar.hpp"
#include "MnmFmDyn.hpp"

} // namespace mnmfm
