// =============================================================================
// FmExactDsp.hpp — DSP56300 primitives for the bit-exact FM machine ports
// (m9 FM+PAR, m10 FM+DYN), Elektron Monomachine SFX-60/MkII OS 1.32B.
//
// Every primitive mirrors scripts/dsp_emu.py (the emulator that validated
// m1/m2/m3/reverb bit-in-bit). Semantics implemented here:
//   * 56-bit accumulators A/B (int64_t, sext56 after every ALU op);
//   * fractional MAC: the 24x24 product is LEFT-SHIFTED 1 bit (FM §5.5.4);
//   * mpysu/macsu: S*U; dmac ss: S*S; negation inside the operand list
//     ("-x1") negates the SIGNED value then re-interprets 24-bit
//     (so -$800000 stays -$800000);
//   * logical ops on accumulators touch ONLY the MSW (A1/B1), the LSW
//     (fraction) is preserved — critical for the phase accumulators;
//   * acc -> memory / x0/x1/y0/y1 transfers SATURATE to $7FFFFF/$800000;
//     acc part transfers (a0/a1/b0/b1) and acc -> R/N bypass the limiter;
//   * acc -> L: pair stores the full 48 bits (no saturation);
//   * parallel moves: sources latched before the ALU, writes committed
//     after it (the classic "old accumulator" store), pointer post-update
//     last;
//   * R post-increment/decrement with M-register modulo (m0/m2 = $1FFF
//     wraps the 8192-word sine window; $14A000 is 8192-aligned so the
//     modulo reduces to (idx+1) & $1FFF).
// =============================================================================
#pragma once
// Selectively imported for the opt-in FM NEW engine.  Original source:
// glassg333/mmnova, 1_NEW_code_FM_2026-09-28/dsp/mnm.
// Names are isolated under fmnew so legacy monomachine::mnm code remains untouched.
#include <cstdint>
#include "FmExactSineTable.h"

// Imported arithmetic originally shifted negative signed values left.  That is
// undefined in C++, despite representing a simple DSP fractional x2 operation.
// This isolated copy uses overflow-safe signed multiplication instead; every
// possible signed-24 product remains comfortably inside int64_t.  The supplied
// 52,800-word STAT vectors still match word-for-word after this portability fix.

namespace fmnew {

static const uint32_t M24 = 0xFFFFFFu;
static const int64_t  M48 = 0xFFFFFFFFFFFFll;
static const uint32_t R6BASE = 0x428u;   // machine page (y:(r6+..))

// ---- ratio table P:$141A80 (24 words), ratio = raw / 2^19 -------------------
static const uint32_t kRatio141A80[24] = {
    0x004000u,0x008000u,0x010000u,0x018000u,0x020000u,0x028000u,0x030000u,0x040000u,
    0x050000u,0x060000u,0x070000u,0x080000u,0x0A0000u,0x0C0000u,0x0E0000u,0x100000u,
    0x140000u,0x180000u,0x1C0000u,0x200000u,0x280000u,0x300000u,0x380000u,0x400000u
};
// ---- TONE one-pole coefficient table P:$144AC7 (128 words) ------------------
static const uint32_t kTone144AC7[128] = {
    0x004876u,0x004CE3u,0x005196u,0x005692u,0x005BDCu,0x006179u,0x00676Du,0x006DBEu,
    0x007472u,0x007B8Eu,0x008319u,0x008B1Au,0x009398u,0x009C9Au,0x00A629u,0x00B04Cu,
    0x00BB0Eu,0x00C676u,0x00D291u,0x00DF67u,0x00ED06u,0x00FB78u,0x010ACAu,0x011B0Au,
    0x012C47u,0x013E8Fu,0x0151F2u,0x016682u,0x017C51u,0x019370u,0x01ABF5u,0x01C5F5u,
    0x01E187u,0x01FEC1u,0x021DBEu,0x023E98u,0x02616Bu,0x028654u,0x02AD74u,0x02D6EBu,
    0x0302DDu,0x03316Eu,0x0362C5u,0x03970Bu,0x03CE6Bu,0x040914u,0x044735u,0x048900u,
    0x04CEACu,0x051868u,0x056684u,0x05B928u,0x06109Eu,0x066D27u,0x06CF0Cu,0x073696u,
    0x07A413u,0x0817D4u,0x08922Eu,0x091378u,0x099C0Eu,0x0A2C4Eu,0x0AC49Du,0x0B655Eu,
    0x0C0EFDu,0x0CC1E5u,0x0D7E86u,0x0E4553u,0x0F16C0u,0x0FF345u,0x10DB5Bu,0x11CF7Cu,
    0x12D023u,0x13DDCCu,0x14F8F2u,0x16220Cu,0x175993u,0x189FF7u,0x19F5A8u,0x1B5B0Bu,
    0x1CD081u,0x1E565Eu,0x1FECECu,0x219466u,0x234CF9u,0x2516BDu,0x26F1B5u,0x28DDCFu,
    0x2ADADCu,0x2CE891u,0x2F0683u,0x313423u,0x3370BDu,0x35BB79u,0x381350u,0x3A7714u,
    0x3CE56Au,0x3F5CCBu,0x41DB85u,0x445FBAu,0x46E76Au,0x49706Cu,0x4BF87Cu,0x4E7D40u,
    0x50FC49u,0x537323u,0x55DF5Du,0x583E8Du,0x5A8E63u,0x5CCCABu,0x5EF75Cu,0x610C9Cu,
    0x630ACCu,0x64F089u,0x66BCB0u,0x686E62u,0x6A04FDu,0x6B801Fu,0x6CDF98u,0x6E236Bu,
    0x6F4BBAu,0x7058C1u,0x714AC1u,0x7221F3u,0x72DE71u,0x73801Du,0x74067Du,0x747093u
};
// ---- P:$141880 (128 words + 8 tail words: n1 may index 128..135) ------------
// The same table the kernel AMP decay uses. Entries 128+ read as $000001.
static const uint32_t kTbl141880[136] = {
    0x8D1897u,0x8C4936u,0x8B8603u,0x8ACE5Au,0x8A21A0u,0x897F3Du,0x88E6A2u,0x885744u,
    0x87D09Fu,0x875234u,0x86DB8Cu,0x866C33u,0x8603BCu,0x85A1C2u,0x8545E0u,0x84EFBCu,
    0x849EFCu,0x84534Du,0x840C62u,0x83C9EFu,0x838BB0u,0x835161u,0x831AC5u,0x82E7A2u,
    0x82B7C0u,0x828AEBu,0x8260F2u,0x8239A8u,0x8214E2u,0x81F277u,0x81D240u,0x81B41Bu,
    0x8197E6u,0x817D81u,0x8164CFu,0x814DB4u,0x813816u,0x8123DDu,0x8110F3u,0x80FF40u,
    0x80EEB3u,0x80DF37u,0x80D0BCu,0x80C330u,0x80B685u,0x80AAABu,0x809F97u,0x809539u,
    0x808B88u,0x808278u,0x8079FEu,0x807211u,0x806AA7u,0x8063B9u,0x805D3Eu,0x80572Eu,
    0x805183u,0x804C37u,0x804742u,0x8042A0u,0x803E4Bu,0x803A3Eu,0x803674u,0x8032EAu,
    0x802F9Au,0x802C81u,0x80299Cu,0x8026E7u,0x80245Fu,0x802202u,0x801FCBu,0x801DBAu,
    0x801BCBu,0x8019FCu,0x80184Bu,0x8016B6u,0x80153Cu,0x8013DAu,0x801290u,0x80115Au,
    0x801039u,0x800F2Bu,0x800E2Eu,0x800D42u,0x800C65u,0x800B97u,0x800AD6u,0x800A21u,
    0x800978u,0x8008DBu,0x800847u,0x8007BDu,0x80073Cu,0x8006C4u,0x800653u,0x8005EAu,
    0x800587u,0x80052Bu,0x8004D5u,0x800484u,0x800439u,0x8003F3u,0x8003B1u,0x800374u,
    0x80033Au,0x800304u,0x8002D2u,0x8002A3u,0x800277u,0x80024Eu,0x800228u,0x800204u,
    0x8001E2u,0x8001C3u,0x8001A5u,0x80018Au,0x800170u,0x800158u,0x800142u,0x80012Du,
    0x800119u,0x800107u,0x8000F6u,0x8000E6u,0x8000D7u,0x8000C9u,0x8000BCu,0x800000u,
    0x000001u,0x000001u,0x000001u,0x000001u,0x000001u,0x000001u,0x000001u,0x000001u
};

// ---- value helpers -----------------------------------------------------------
static inline int64_t sext56(int64_t v) {
    v &= (1ll << 56) - 1;
    return (v ^ (1ll << 55)) - (1ll << 55);
}
static inline int64_t sext48(int64_t v) {
    v &= (1ll << 48) - 1;
    return (v ^ (1ll << 47)) - (1ll << 47);
}
static inline int32_t sgn24(uint32_t v) {
    return (int32_t)(v & M24) - ((v & 0x800000u) ? (1 << 24) : 0);
}
static inline int64_t al24(uint32_t v) {            // ALU source: sext24 << 24
    return (int64_t)sgn24(v) * (1ll << 24);
}
static inline int64_t mem2acc(uint32_t v) {         // memory -> accumulator
    return (int64_t)sgn24(v) * (1ll << 24);
}
// acc -> 24-bit with data-limit checking (FM §5.4.1.2)
static inline uint32_t acc24sat(int64_t v) {
    if (v > 0x007FFFFFFFFFFFll) return 0x7FFFFFu;
    if (v < -0x00800000000000ll) return 0x800000u;
    return (uint32_t)((v >> 24) & M24);
}
static inline uint32_t acc24raw(int64_t v) {        // part transfer (a1/b1)
    return (uint32_t)((v >> 24) & M24);
}

// ---- MAC family (all products << 1) ------------------------------------------
static inline int64_t mpy_ss(uint32_t a, uint32_t b) {          // mpy S,S
    return sext56(((int64_t)sgn24(a) * (int64_t)sgn24(b)) * 2ll);
}
static inline int64_t mac_ss(uint32_t a, uint32_t b, int64_t acc) {
    return sext56(acc + (((int64_t)sgn24(a) * (int64_t)sgn24(b)) * 2ll));
}
static inline int64_t mpsu(uint32_t a, uint32_t b) {            // mpysu S,U
    return sext56(((int64_t)sgn24(a) * (int64_t)(b & M24)) * 2ll);
}
static inline int64_t mcsu(uint32_t a, uint32_t b, int64_t acc) { // macsu S,U
    return sext56(acc + (((int64_t)sgn24(a) * (int64_t)(b & M24)) * 2ll));
}
// negated signed operand ("-x1"): v1 = (-sgn24(x)) & $FFFFFF, signed again
static inline uint32_t neg24(uint32_t x) {
    return (uint32_t)(-(int64_t)sgn24(x)) & M24;
}
static inline int64_t mpsu_neg(uint32_t a, uint32_t b) {        // mpysu -x1,y0,a
    uint32_t v = neg24(a);
    return sext56(((int64_t)sgn24(v) * (int64_t)(b & M24)) * 2ll);
}
static inline int64_t mcsu_neg(uint32_t a, uint32_t b, int64_t acc) {
    uint32_t v = neg24(a);
    return sext56(acc + (((int64_t)sgn24(v) * (int64_t)(b & M24)) * 2ll));
}
// "mac -s1,s2,acc" — negated SIGNED x SIGNED (rotators, droop)
static inline int64_t mac_ss_neg(uint32_t a, uint32_t b, int64_t acc) {
    uint32_t v = neg24(a);
    return sext56(acc + (((int64_t)sgn24(v) * (int64_t)sgn24(b)) * 2ll));
}
// "mpy -s1,s2" — negated SIGNED x SIGNED (DYN $2E/$2C recursions)
static inline int64_t mpy_ss_neg(uint32_t a, uint32_t b) {
    uint32_t v = neg24(a);
    return sext56(((int64_t)sgn24(v) * (int64_t)sgn24(b)) * 2ll);
}

// ---- simple ALU ----------------------------------------------------------------
static inline int64_t alu_add(int64_t a, int64_t b) { return sext56(a + b); }
static inline int64_t alu_sub(int64_t a, int64_t b) { return sext56(a - b); }
static inline int64_t acc_asr(int64_t v, int n) { return sext56(v >> n); }
static inline int64_t acc_asl(int64_t v, int n) { return sext56((int64_t)((uint64_t)v << n)); }
// logical op on the accumulator: MSW only, LSW (fraction) preserved
static inline int64_t acc_and(int64_t acc, uint32_t m) {
    uint32_t msw = (uint32_t)((acc >> 24) & M24) & m;
    return sext56(((int64_t)msw << 24) | (acc & M24));
}
static inline int64_t acc_abs(int64_t v) { return v < 0 ? sext56(-v) : v; }

// ---- memory model ---------------------------------------------------------------
// Scratch windows X/Y:$000-$0FF, machine page y:$428-$467, tables, sine LUT.
struct FmMem {
    uint32_t X[0x100];
    uint32_t Y[0x100];
    uint32_t st[0x40];            // y:(r6+$00..$3F)
    uint32_t R[8];
    int32_t  N[8];
    uint32_t M[8];

    void resetRegs() {
        for (int i = 0; i < 8; ++i) { R[i] = 0; N[i] = 0; M[i] = M24; }
    }
    // pointer update with M-register modulo (mirrors upd_r)
    inline void updR(int n, int delta) {
        uint32_t m = M[n];
        if (m == M24) { R[n] = (R[n] + (uint32_t)delta) & M24; return; }
        uint32_t base = R[n] & ~m;
        int32_t off = (int32_t)(R[n] - base) + delta;
        if (off > (int32_t)m) off -= (int32_t)m + 1;
        else if (off < 0) off += (int32_t)m + 1;
        R[n] = base + (uint32_t)off;
    }
    // All firmware tables are present in BOTH X and Y spaces (verified against
    // the emulator memory), and the sine LUT is injected into both.
    inline uint32_t tables(uint32_t a) const {
        if (a >= 0x14A000u) return kSine8192[a & 0x1FFFu];
        if (a >= 0x144AC7u && a < 0x144B47u) return kTone144AC7[a - 0x144AC7u];
        if (a >= 0x141A80u && a < 0x141A98u) return kRatio141A80[a - 0x141A80u];
        if (a >= 0x141880u && a < 0x141908u) return kTbl141880[a - 0x141880u];
        return 0;
    }
    inline uint32_t rdx(uint32_t a) const {
        if (a < 0x100) return X[a];
        return tables(a);
    }
    inline uint32_t rdy(uint32_t a) const {
        if (a < 0x100) return Y[a];
        if (a >= R6BASE && a < R6BASE + 0x40) return st[a - R6BASE];
        return tables(a);
    }
    inline void wrx(uint32_t a, uint32_t v) { if (a < 0x100) X[a] = v & M24; }
    inline void wry(uint32_t a, uint32_t v) {
        v &= M24;
        if (a < 0x100) Y[a] = v;
        else if (a >= R6BASE && a < R6BASE + 0x40) st[a - R6BASE] = v;
    }
    inline int64_t rdL(uint32_t a) const { return ((int64_t)rdx(a) << 24) | (int64_t)rdy(a); }
    inline void wrL(uint32_t a, int64_t v48) {
        wrx(a, (uint32_t)((v48 >> 24) & M24));
        wry(a, (uint32_t)(v48 & M24));
    }
};

} // namespace fmnew
