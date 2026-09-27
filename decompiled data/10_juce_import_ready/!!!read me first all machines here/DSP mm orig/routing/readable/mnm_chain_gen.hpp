// ============================================================================
// mnm_chain_gen.hpp — АВТОГЕНЕРАЦИЯ из листинга OS 1.32B
// (scripts/transpile_full.py; источник: chain_program.lst + machines dump).
// Семантика инструкций = бит-точный эталон (dsp_emu.py / MnmDspCore.hpp).
// Каждая строка C++ = одна инструкция DSP56300 (PC в комментарии $XXXX).
// НЕ редактировать руками — перегенерировать скриптом.
// ============================================================================
#pragma once
#include "mnm_chain_b.hpp"

namespace mnmchain {

// CHK: контрольная точка границы секции (подключается тестом)
#ifdef MNM_CHAIN_CHK
#define CHK(pc) mnmchain::g_chk(pc)
extern void (*g_chk)(uint32_t);
#else
#define CHK(pc) ((void)0)
#endif

// ---- рантайм-хелперы сгенерированного кода (семантика = эталон) -----------
#ifdef MNM_GEN_TRACE
#include <cstdio>
static void mnm_trc(uint32_t pc, const Regs& g) {
    fprintf(stderr, "%04X %014llX %014llX %06X %06X %06X %06X r0=%06X r1=%06X r2=%06X r4=%06X\n",
        pc, (unsigned long long)g.A, (unsigned long long)g.B, g.x0, g.x1, g.y0, g.y1,
        g.r[0], g.r[1], g.r[2], g.r[4]);
}
#define GEN_TRC(pc) mnm_trc((pc), g)
#else
#define GEN_TRC(pc) ((void)0)
#endif
inline void gm_flags56(Regs& g, int64_t r) {
    r = sext56(r);
    g.fn = (r < 0) ? 1 : 0;
    g.fz = (r == 0) ? 1 : 0;
    const int a1msb = (int)((r >> 47) & 1);
    const int a2 = (int)((r >> 48) & 0xFF);
    g.fe = (a2 != (a1msb ? 0xFF : 0x00)) ? 1 : 0;
}
inline void gm_test56(Regs& g, int64_t r) {
    r = sext56(r);
    g.fn = (r < 0) ? 1 : 0;
    g.fz = (r == 0) ? 1 : 0;
    g.fe = 0;
}
inline uint32_t acc_to24n(int64_t acc) noexcept { return (uint32_t)((acc >> 24) & M24); }
inline void gm_mac(Regs& g, int64_t& dst, int64_t s1, int64_t s2, bool acc) {
    // DSP56300 MAC НЕ сатурирует (sat только при записи acc в память)
    const int64_t prod = (s1 * s2) << 1;
    const int64_t res = sext56(acc ? dst + prod : prod);
    gm_flags56(g, res);
    dst = res;
}
inline void gm_macr(Regs& g, int64_t& dst, int64_t s1, int64_t s2, bool acc) {
    gm_mac(g, dst, s1, s2, acc);
    mnmfix::rnd_acc(dst);
    gm_flags56(g, dst);
}
inline void gm_mpy(Regs& g, int64_t& dst, int64_t s1, int64_t s2) { gm_mac(g, dst, s1, s2, false); }
inline void gm_mpyr(Regs& g, int64_t& dst, int64_t s1, int64_t s2) { gm_macr(g, dst, s1, s2, false); }
inline void gm_macsu(Regs& g, int64_t& dst, int64_t s1, int64_t s2, bool acc) {
    const int64_t prod = (s1 * (int64_t)(s2 & 0xFFFFFFll)) << 1;
    const int64_t res = sext56(acc ? dst + prod : prod);
    gm_flags56(g, res);
    dst = res;
}
inline void gm_macuu(Regs& g, int64_t& dst, int64_t s1, int64_t s2, bool acc) {
    const int64_t prod = ((int64_t)(s1 & 0xFFFFFFll) * (int64_t)(s2 & 0xFFFFFFll)) << 1;
    const int64_t res = sext56(acc ? dst + prod : prod);
    gm_flags56(g, res);
    dst = res;
}
inline void gm_cmp(Regs& g, int64_t x, int64_t y) {
    // cmp: полное 56-бит вычитание, только флаги (sem = acc_sub56 без записи)
    const int64_t a = sext56(x) - sext56(y);
    const int64_t r = sext56(a);
    g.fn = (r < 0) ? 1 : 0;
    g.fz = (r == 0) ? 1 : 0;
    g.fc = (a < 0) ? 0 : 1;
    const int a1msb = (int)((r >> 47) & 1);
    const int a2 = (int)((r >> 48) & 0xFF);
    g.fe = (a2 != (a1msb ? 0xFF : 0x00)) ? 1 : 0;
}
inline void gm_logic(Regs& g, char op, const char* dstn, int64_t w) {
    int64_t& acc = (dstn[0] == 'a') ? g.A : g.B;
    uint32_t a1 = (uint32_t)((acc >> 24) & M24);
    uint32_t b = (uint32_t)(w & M24);
    uint32_t r = (op == 'a') ? (a1 & b) : (op == 'o') ? (a1 | b) : (a1 ^ b);
    int64_t r56 = ((int64_t)r << 24) | (acc & M24);
    if (r & 0x800000) r56 |= (int64_t)0xFF << 48;
    g.fn = (r & 0x800000) ? 1 : 0;
    g.fz = (r == 0) ? 1 : 0;
    acc = r56;
}

void func_000350(Mem& M, const Rom& rom, Regs& g) {
// === func_000350 (автогенерация из листинга) ===
int _do0354;
  { // $0350: move     x:(r0)+,x0      y:(r4)+,y1
  GEN_TRC(0x0350);
  const uint32_t _v1 = M.rd('x', g.r[0]);
  const uint32_t _v2 = M.rd('y', g.r[4]);
  g.x0 = (uint32_t)(_v1 & M24);
  g.y1 = (uint32_t)(_v2 & M24);
  g.pstep(0,1);
  g.pstep(4,1);
  }
  { // $0351: move     #>$fffffe,n4
  GEN_TRC(0x0351);
  g.n[4] = (uint32_t)(0xFFFFFEu);
  }
  { // $0353: move     b,x1
  GEN_TRC(0x0353);
  g.x1 = acc_to24(g.B);
  }
  _do0354 = 16; // $0354: do
  Ldo0354_top:;
    { // $0356: asr      #$4,b,b
    GEN_TRC(0x0356);
    const int64_t _v3 = g.B;
    g.fc = (int)((_v3 >> 3) & 1);
    g.B = sext56(_v3 >> 4);
    gm_flags56(g, g.B);
    }
    { // $0357: mac      x0,y1,b y:(r4)+,y1
    GEN_TRC(0x0357);
    const uint32_t _v4 = M.rd('y', g.r[4]);
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.y1 = (uint32_t)(_v4 & M24);
    g.pstep(4,1);
    }
    { // $0358: asl      #$4,b,b
    GEN_TRC(0x0358);
    const int64_t _v5 = g.B;
    g.fc = (int)((_v5 >> 52) & 1);
    g.B = sext56(_v5 << 4);
    gm_flags56(g, g.B);
    }
    { // $0359: mac      y1,x1,b a,x0 y:(r4)+n4,y0
    GEN_TRC(0x0359);
    const uint32_t _v6 = M.rd('y', g.r[4]);
    const uint32_t _v7 = acc_to24(g.A);
    const uint32_t _v8 = acc_to24(g.B);
    const int64_t _v9 = g.A;
    const int64_t _v10 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x0 = _v7;
    g.y0 = (uint32_t)(_v6 & M24);
    g.pstep(4,g.n[4]);
    }
    { // $035A: mac      -y0,x0,b a,l:(r1)+
    GEN_TRC(0x035A);
    const uint32_t _v11 = acc_to24(g.A);
    const uint32_t _v12 = acc_to24(g.B);
    const int64_t _v13 = g.A;
    const int64_t _v14 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v15 = g.r[1];
    M.wrL(_v15, (uint64_t)_v13 & 0xFFFFFFFFFFll);
    g.pstep(1,1);
    }
    { // $035B: mac      x0,y1,a x:(r0)+,x0 y:(r4)+,y1
    GEN_TRC(0x035B);
    const uint32_t _v16 = M.rd('x', g.r[0]);
    const uint32_t _v17 = M.rd('y', g.r[4]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v16 & M24);
    g.y1 = (uint32_t)(_v17 & M24);
    g.pstep(0,1);
    g.pstep(4,1);
    }
    { // $035C: mac      x1,y0,a b,x1
    GEN_TRC(0x035C);
    const uint32_t _v18 = acc_to24(g.A);
    const uint32_t _v19 = acc_to24(g.B);
    const int64_t _v20 = g.A;
    const int64_t _v21 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = _v19;
    }
    { // $035D: asr      #$4,b,b
    GEN_TRC(0x035D);
    const int64_t _v22 = g.B;
    g.fc = (int)((_v22 >> 3) & 1);
    g.B = sext56(_v22 >> 4);
    gm_flags56(g, g.B);
    }
    { // $035E: mac      x0,y1,b y:(r4)+,y1
    GEN_TRC(0x035E);
    const uint32_t _v23 = M.rd('y', g.r[4]);
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.y1 = (uint32_t)(_v23 & M24);
    g.pstep(4,1);
    }
    { // $035F: asl      #$4,b,b
    GEN_TRC(0x035F);
    const int64_t _v24 = g.B;
    g.fc = (int)((_v24 >> 52) & 1);
    g.B = sext56(_v24 << 4);
    gm_flags56(g, g.B);
    }
    { // $0360: mac      y1,x1,b a,x0 y:(r4)+,y0
    GEN_TRC(0x0360);
    const uint32_t _v25 = M.rd('y', g.r[4]);
    const uint32_t _v26 = acc_to24(g.A);
    const uint32_t _v27 = acc_to24(g.B);
    const int64_t _v28 = g.A;
    const int64_t _v29 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x0 = _v26;
    g.y0 = (uint32_t)(_v25 & M24);
    g.pstep(4,1);
    }
    { // $0361: mac      -y0,x0,b a,l:(r1)+
    GEN_TRC(0x0361);
    const uint32_t _v30 = acc_to24(g.A);
    const uint32_t _v31 = acc_to24(g.B);
    const int64_t _v32 = g.A;
    const int64_t _v33 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v34 = g.r[1];
    M.wrL(_v34, (uint64_t)_v32 & 0xFFFFFFFFFFll);
    g.pstep(1,1);
    }
    { // $0362: mac      x0,y1,a x:(r0)+,x0 y:(r4)+,y1
    GEN_TRC(0x0362);
    const uint32_t _v35 = M.rd('x', g.r[0]);
    const uint32_t _v36 = M.rd('y', g.r[4]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v35 & M24);
    g.y1 = (uint32_t)(_v36 & M24);
    g.pstep(0,1);
    g.pstep(4,1);
    }
    { // $0363: mac      x1,y0,a b,x1
    GEN_TRC(0x0363);
    const uint32_t _v37 = acc_to24(g.A);
    const uint32_t _v38 = acc_to24(g.B);
    const int64_t _v39 = g.A;
    const int64_t _v40 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = _v38;
    }
  if (--_do0354 > 0) goto Ldo0354_top; // конец do
    { // $0364: rts
    GEN_TRC(0x0364);
      return;
    }
}

void func_000365(Mem& M, const Rom& rom, Regs& g) {
// === func_000365 (автогенерация из листинга) ===
int _do0369;
  { // $0365: move     x:(r0)+,x1 y:(r4)+,y1
  GEN_TRC(0x0365);
  const uint32_t _v1 = M.rd('x', g.r[0]);
  const uint32_t _v2 = M.rd('y', g.r[4]);
  g.x1 = (uint32_t)(_v1 & M24);
  g.y1 = (uint32_t)(_v2 & M24);
  g.pstep(0,1);
  g.pstep(4,1);
  }
  { // $0366: move     x:(r1),x0 y:(r5),y0
  GEN_TRC(0x0366);
  const uint32_t _v3 = M.rd('x', g.r[1]);
  const uint32_t _v4 = M.rd('y', g.r[5]);
  g.x0 = (uint32_t)(_v3 & M24);
  g.y0 = (uint32_t)(_v4 & M24);
  }
  { // $0367: move     #>$fffffe,n4
  GEN_TRC(0x0367);
  g.n[4] = (uint32_t)(0xFFFFFEu);
  }
  _do0369 = 16; // $0369: do
  Ldo0369_top:;
    { // $036B: mac      -y1,x1,b b,x1 y:(r4)+,y1
    GEN_TRC(0x036B);
    const uint32_t _v5 = M.rd('y', g.r[4]);
    const uint32_t _v6 = acc_to24(g.A);
    const uint32_t _v7 = acc_to24(g.B);
    const int64_t _v8 = g.A;
    const int64_t _v9 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x1 = _v7;
    g.y1 = (uint32_t)(_v5 & M24);
    g.pstep(4,1);
    }
    { // $036C: mac      y0,x0,a a,l:(r2)
    GEN_TRC(0x036C);
    const uint32_t _v10 = acc_to24(g.A);
    const uint32_t _v11 = acc_to24(g.B);
    const int64_t _v12 = g.A;
    const int64_t _v13 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v14 = g.r[2];
    M.wrL(_v14, (uint64_t)_v12 & 0xFFFFFFFFFFll);
    }
    { // $036D: asl      #$7,a,a
    GEN_TRC(0x036D);
    const int64_t _v15 = g.A;
    g.fc = (int)((_v15 >> 49) & 1);
    g.A = sext56(_v15 << 7);
    gm_flags56(g, g.A);
    }
    { // $036E: mac      y1,x1,b x:(r2),x0 y:(r4)+n4,y0
    GEN_TRC(0x036E);
    const uint32_t _v16 = M.rd('x', g.r[2]);
    const uint32_t _v17 = M.rd('y', g.r[4]);
    gm_mac(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x0 = (uint32_t)(_v16 & M24);
    g.y0 = (uint32_t)(_v17 & M24);
    g.pstep(4,g.n[4]);
    }
    { // $036F: mac      -y0,x0,b a,l:(r1)+
    GEN_TRC(0x036F);
    const uint32_t _v18 = acc_to24(g.A);
    const uint32_t _v19 = acc_to24(g.B);
    const int64_t _v20 = g.A;
    const int64_t _v21 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v22 = g.r[1];
    M.wrL(_v22, (uint64_t)_v20 & 0xFFFFFFFFFFll);
    g.pstep(1,1);
    }
    { // $0370: move     l:(r2),a
    GEN_TRC(0x0370);
    const uint64_t _v23 = M.rdL(g.r[2]);
    g.A = sext56((int64_t)(_v23 & 0xFFFFFFFFFFll));
    }
    { // $0371: mac      x0,y1,a x:(r1),x0 y:(r4)+,y1
    GEN_TRC(0x0371);
    const uint32_t _v24 = M.rd('x', g.r[1]);
    const uint32_t _v25 = M.rd('y', g.r[4]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v24 & M24);
    g.y1 = (uint32_t)(_v25 & M24);
    g.pstep(4,1);
    }
    { // $0372: mac      x1,y0,a x:(r0)+,x1 y:(r5),y0
    GEN_TRC(0x0372);
    const uint32_t _v26 = M.rd('x', g.r[0]);
    const uint32_t _v27 = M.rd('y', g.r[5]);
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v26 & M24);
    g.y0 = (uint32_t)(_v27 & M24);
    g.pstep(0,1);
    }
    { // $0373: mac      -y1,x1,b b,x1 y:(r4)+,y1
    GEN_TRC(0x0373);
    const uint32_t _v28 = M.rd('y', g.r[4]);
    const uint32_t _v29 = acc_to24(g.A);
    const uint32_t _v30 = acc_to24(g.B);
    const int64_t _v31 = g.A;
    const int64_t _v32 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x1 = _v30;
    g.y1 = (uint32_t)(_v28 & M24);
    g.pstep(4,1);
    }
    { // $0374: mac      y0,x0,a a,l:(r2)
    GEN_TRC(0x0374);
    const uint32_t _v33 = acc_to24(g.A);
    const uint32_t _v34 = acc_to24(g.B);
    const int64_t _v35 = g.A;
    const int64_t _v36 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v37 = g.r[2];
    M.wrL(_v37, (uint64_t)_v35 & 0xFFFFFFFFFFll);
    }
    { // $0375: asl      #$7,a,a
    GEN_TRC(0x0375);
    const int64_t _v38 = g.A;
    g.fc = (int)((_v38 >> 49) & 1);
    g.A = sext56(_v38 << 7);
    gm_flags56(g, g.A);
    }
    { // $0376: mac      y1,x1,b x:(r2),x0 y:(r4)+,y0
    GEN_TRC(0x0376);
    const uint32_t _v39 = M.rd('x', g.r[2]);
    const uint32_t _v40 = M.rd('y', g.r[4]);
    gm_mac(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x0 = (uint32_t)(_v39 & M24);
    g.y0 = (uint32_t)(_v40 & M24);
    g.pstep(4,1);
    }
    { // $0377: mac      -y0,x0,b a,l:(r1)+
    GEN_TRC(0x0377);
    const uint32_t _v41 = acc_to24(g.A);
    const uint32_t _v42 = acc_to24(g.B);
    const int64_t _v43 = g.A;
    const int64_t _v44 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v45 = g.r[1];
    M.wrL(_v45, (uint64_t)_v43 & 0xFFFFFFFFFFll);
    g.pstep(1,1);
    }
    { // $0378: move     l:(r2),a
    GEN_TRC(0x0378);
    const uint64_t _v46 = M.rdL(g.r[2]);
    g.A = sext56((int64_t)(_v46 & 0xFFFFFFFFFFll));
    }
    { // $0379: mac      x0,y1,a x:(r1),x0 y:(r4)+,y1
    GEN_TRC(0x0379);
    const uint32_t _v47 = M.rd('x', g.r[1]);
    const uint32_t _v48 = M.rd('y', g.r[4]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v47 & M24);
    g.y1 = (uint32_t)(_v48 & M24);
    g.pstep(4,1);
    }
    { // $037A: mac      x1,y0,a x:(r0)+,x1 y:(r5),y0
    GEN_TRC(0x037A);
    const uint32_t _v49 = M.rd('x', g.r[0]);
    const uint32_t _v50 = M.rd('y', g.r[5]);
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v49 & M24);
    g.y0 = (uint32_t)(_v50 & M24);
    g.pstep(0,1);
    }
  if (--_do0369 > 0) goto Ldo0369_top; // конец do
    { // $037B: rts
    GEN_TRC(0x037B);
      return;
    }
}

void func_00037c(Mem& M, const Rom& rom, Regs& g) {
// === func_00037c (автогенерация из листинга) ===
int _do037d;
  { // $037C: move     x:(r0)+,x0 y:(r4)+n4,y0
  GEN_TRC(0x037C);
  const uint32_t _v1 = M.rd('x', g.r[0]);
  const uint32_t _v2 = M.rd('y', g.r[4]);
  g.x0 = (uint32_t)(_v1 & M24);
  g.y0 = (uint32_t)(_v2 & M24);
  g.pstep(0,1);
  g.pstep(4,g.n[4]);
  }
  _do037d = 16; // $037D: do
  Ldo037d_top:;
    { // $037F: mac      y0,x0,a a,x:(r1)+ a,y1
    GEN_TRC(0x037F);
    const uint32_t _v3 = acc_to24(g.A);
    const uint32_t _v4 = acc_to24(g.B);
    const int64_t _v5 = g.A;
    const int64_t _v6 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v7 = g.r[1];
    M.wr('x', _v7, _v3);
    g.pstep(1,1);
    g.y1 = _v3;
    }
    { // $0380: mac      -y1,y0,a x:(r2)+,x0
    GEN_TRC(0x0380);
    const uint32_t _v8 = M.rd('x', g.r[2]);
    gm_mac(g, g.A, -(int64_t)s24(g.y1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v8 & M24);
    g.pstep(2,1);
    }
    { // $0381: mac      y0,x0,b b,x:(r3)+ b,y1
    GEN_TRC(0x0381);
    const uint32_t _v9 = acc_to24(g.A);
    const uint32_t _v10 = acc_to24(g.B);
    const int64_t _v11 = g.A;
    const int64_t _v12 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v13 = g.r[3];
    M.wr('x', _v13, _v10);
    g.pstep(3,1);
    g.y1 = _v10;
    }
    { // $0382: mac      -y1,y0,b x:(r0)+,x0
    GEN_TRC(0x0382);
    const uint32_t _v14 = M.rd('x', g.r[0]);
    gm_mac(g, g.B, -(int64_t)s24(g.y1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v14 & M24);
    g.pstep(0,1);
    }
    { // $0383: mac      y0,x0,a a,x:(r1)+ a,y1
    GEN_TRC(0x0383);
    const uint32_t _v15 = acc_to24(g.A);
    const uint32_t _v16 = acc_to24(g.B);
    const int64_t _v17 = g.A;
    const int64_t _v18 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v19 = g.r[1];
    M.wr('x', _v19, _v15);
    g.pstep(1,1);
    g.y1 = _v15;
    }
    { // $0384: mac      -y1,y0,a x:(r2)+,x0
    GEN_TRC(0x0384);
    const uint32_t _v20 = M.rd('x', g.r[2]);
    gm_mac(g, g.A, -(int64_t)s24(g.y1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v20 & M24);
    g.pstep(2,1);
    }
    { // $0385: mac      y0,x0,b b,x:(r3)+ b,y1
    GEN_TRC(0x0385);
    const uint32_t _v21 = acc_to24(g.A);
    const uint32_t _v22 = acc_to24(g.B);
    const int64_t _v23 = g.A;
    const int64_t _v24 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v25 = g.r[3];
    M.wr('x', _v25, _v22);
    g.pstep(3,1);
    g.y1 = _v22;
    }
    { // $0386: mac      -y1,y0,b x:(r0)+,x0 y:(r4)+n4,y0
    GEN_TRC(0x0386);
    const uint32_t _v26 = M.rd('x', g.r[0]);
    const uint32_t _v27 = M.rd('y', g.r[4]);
    gm_mac(g, g.B, -(int64_t)s24(g.y1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v26 & M24);
    g.y0 = (uint32_t)(_v27 & M24);
    g.pstep(0,1);
    g.pstep(4,g.n[4]);
    }
  if (--_do037d > 0) goto Ldo037d_top; // конец do
    { // $0387: move     a,x:(r1)+
    GEN_TRC(0x0387);
    const uint32_t _v28 = g.r[1];
    M.wr('x', _v28, acc_to24(g.A));
    g.pstep(1,1);
    }
  { // $0388: move     b,x:(r3)+
  GEN_TRC(0x0388);
  const uint32_t _v29 = g.r[3];
  M.wr('x', _v29, acc_to24(g.B));
  g.pstep(3,1);
  }
  { // $0389: rts
  GEN_TRC(0x0389);
    return;
  }
}

void func_00038a(Mem& M, const Rom& rom, Regs& g) {
// === func_00038a (автогенерация из листинга) ===
int _do038b;
  { // $038A: move     x:(r0)+,y0
  GEN_TRC(0x038A);
  const uint32_t _v1 = M.rd('x', g.r[0]);
  g.y0 = (uint32_t)(_v1 & M24);
  g.pstep(0,1);
  }
  _do038b = 16; // $038B: do
  Ldo038b_top:;
    { // $038D: move     l:(r4)+,x
    GEN_TRC(0x038D);
    const uint64_t _v2 = M.rdL(g.r[4]);
    g.x1 = (uint32_t)((_v2 >> 24) & M24); g.x0 = (uint32_t)(_v2 & M24);
    g.pstep(4,1);
    }
    { // $038E: mac      -y0,x0,a a,x:(r1)+ a,y0
    GEN_TRC(0x038E);
    const uint32_t _v3 = acc_to24(g.A);
    const uint32_t _v4 = acc_to24(g.B);
    const int64_t _v5 = g.A;
    const int64_t _v6 = g.B;
    gm_mac(g, g.A, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v7 = g.r[1];
    M.wr('x', _v7, _v3);
    g.pstep(1,1);
    g.y0 = _v3;
    }
    { // $038F: mac      -x1,y0,a x:(r0),y0
    GEN_TRC(0x038F);
    const uint32_t _v8 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, -(int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.y0 = (uint32_t)(_v8 & M24);
    }
    { // $0390: mac      y0,x0,a x:(r2)+,y0
    GEN_TRC(0x0390);
    const uint32_t _v9 = M.rd('x', g.r[2]);
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.y0 = (uint32_t)(_v9 & M24);
    g.pstep(2,1);
    }
    { // $0391: mac      -y0,x0,b b,x:(r3)+ b,y0
    GEN_TRC(0x0391);
    const uint32_t _v10 = acc_to24(g.A);
    const uint32_t _v11 = acc_to24(g.B);
    const int64_t _v12 = g.A;
    const int64_t _v13 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v14 = g.r[3];
    M.wr('x', _v14, _v11);
    g.pstep(3,1);
    g.y0 = _v11;
    }
    { // $0392: mac      -x1,y0,b x:(r2),y0
    GEN_TRC(0x0392);
    const uint32_t _v15 = M.rd('x', g.r[2]);
    gm_mac(g, g.B, -(int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.y0 = (uint32_t)(_v15 & M24);
    }
    { // $0393: mac      y0,x0,b x:(r0)+,y0
    GEN_TRC(0x0393);
    const uint32_t _v16 = M.rd('x', g.r[0]);
    gm_mac(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.y0 = (uint32_t)(_v16 & M24);
    g.pstep(0,1);
    }
  if (--_do038b > 0) goto Ldo038b_top; // конец do
    { // $0394: move     a,x:(r1)+
    GEN_TRC(0x0394);
    const uint32_t _v17 = g.r[1];
    M.wr('x', _v17, acc_to24(g.A));
    g.pstep(1,1);
    }
  { // $0395: move     b,x:(r3)+
  GEN_TRC(0x0395);
  const uint32_t _v18 = g.r[3];
  M.wr('x', _v18, acc_to24(g.B));
  g.pstep(3,1);
  }
  { // $0396: rts
  GEN_TRC(0x0396);
    return;
  }
}

void func_000397(Mem& M, const Rom& rom, Regs& g) {
// === func_000397 (автогенерация из листинга) ===
int _do0397;
  _do0397 = 8; // $0397: do
  Ldo0397_top:;
    { // $0399: move     x:(r4),r1
    GEN_TRC(0x0399);
    const uint32_t _v1 = M.rd('x', g.r[4]);
    g.r[1] = (uint32_t)(_v1);
    }
    { // $039A: mpy      -x1,y0,a a,x:(r2)+
    GEN_TRC(0x039A);
    const uint32_t _v2 = acc_to24(g.A);
    const uint32_t _v3 = acc_to24(g.B);
    const int64_t _v4 = g.A;
    const int64_t _v5 = g.B;
    gm_mpy(g, g.A, -(int64_t)s24(g.x1), (int64_t)s24(g.y0));
    const uint32_t _v6 = g.r[2];
    M.wr('x', _v6, _v2);
    g.pstep(2,1);
    }
    { // $039B: add      x1,a x:(r0)+n0,x1
    GEN_TRC(0x039B);
    const uint32_t _v7 = M.rd('x', g.r[0]);
    g.A = acc_add56(g, g.A, (int64_t)s24(g.x1) << 24);
    g.x1 = (uint32_t)(_v7 & M24);
    g.pstep(0,g.n[0]);
    }
    { // $039C: macr     x1,y0,a x:(r0)+,x1
    GEN_TRC(0x039C);
    const uint32_t _v8 = M.rd('x', g.r[0]);
    gm_macr(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v8 & M24);
    g.pstep(0,1);
    }
    { // $039D: mpy      -x1,y0,b b,x:(r3)+
    GEN_TRC(0x039D);
    const uint32_t _v9 = acc_to24(g.A);
    const uint32_t _v10 = acc_to24(g.B);
    const int64_t _v11 = g.A;
    const int64_t _v12 = g.B;
    gm_mpy(g, g.B, -(int64_t)s24(g.x1), (int64_t)s24(g.y0));
    const uint32_t _v13 = g.r[3];
    M.wr('x', _v13, _v10);
    g.pstep(3,1);
    }
    { // $039E: add      x1,b x:(r0),x1
    GEN_TRC(0x039E);
    const uint32_t _v14 = M.rd('x', g.r[0]);
    g.B = acc_add56(g, g.B, (int64_t)s24(g.x1) << 24);
    g.x1 = (uint32_t)(_v14 & M24);
    }
    { // $039F: macr     x1,y0,b x:(r1)+,x1 y:(r4)+,y0
    GEN_TRC(0x039F);
    const uint32_t _v15 = M.rd('x', g.r[1]);
    const uint32_t _v16 = M.rd('y', g.r[4]);
    gm_macr(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v15 & M24);
    g.y0 = (uint32_t)(_v16 & M24);
    g.pstep(1,1);
    g.pstep(4,1);
    }
    { // $03A0: move     x:(r4),r0
    GEN_TRC(0x03A0);
    const uint32_t _v17 = M.rd('x', g.r[4]);
    g.r[0] = (uint32_t)(_v17);
    }
    { // $03A1: mpy      -x1,y0,a a,x:(r2)+
    GEN_TRC(0x03A1);
    const uint32_t _v18 = acc_to24(g.A);
    const uint32_t _v19 = acc_to24(g.B);
    const int64_t _v20 = g.A;
    const int64_t _v21 = g.B;
    gm_mpy(g, g.A, -(int64_t)s24(g.x1), (int64_t)s24(g.y0));
    const uint32_t _v22 = g.r[2];
    M.wr('x', _v22, _v18);
    g.pstep(2,1);
    }
    { // $03A2: add      x1,a x:(r1)+n1,x1
    GEN_TRC(0x03A2);
    const uint32_t _v23 = M.rd('x', g.r[1]);
    g.A = acc_add56(g, g.A, (int64_t)s24(g.x1) << 24);
    g.x1 = (uint32_t)(_v23 & M24);
    g.pstep(1,g.n[1]);
    }
    { // $03A3: macr     x1,y0,a x:(r1)+,x1
    GEN_TRC(0x03A3);
    const uint32_t _v24 = M.rd('x', g.r[1]);
    gm_macr(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v24 & M24);
    g.pstep(1,1);
    }
    { // $03A4: mpy      -x1,y0,b b,x:(r3)+
    GEN_TRC(0x03A4);
    const uint32_t _v25 = acc_to24(g.A);
    const uint32_t _v26 = acc_to24(g.B);
    const int64_t _v27 = g.A;
    const int64_t _v28 = g.B;
    gm_mpy(g, g.B, -(int64_t)s24(g.x1), (int64_t)s24(g.y0));
    const uint32_t _v29 = g.r[3];
    M.wr('x', _v29, _v26);
    g.pstep(3,1);
    }
    { // $03A5: add      x1,b x:(r1),x1
    GEN_TRC(0x03A5);
    const uint32_t _v30 = M.rd('x', g.r[1]);
    g.B = acc_add56(g, g.B, (int64_t)s24(g.x1) << 24);
    g.x1 = (uint32_t)(_v30 & M24);
    }
    { // $03A6: macr     x1,y0,b x:(r0)+,x1 y:(r4)+,y0
    GEN_TRC(0x03A6);
    const uint32_t _v31 = M.rd('x', g.r[0]);
    const uint32_t _v32 = M.rd('y', g.r[4]);
    gm_macr(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v31 & M24);
    g.y0 = (uint32_t)(_v32 & M24);
    g.pstep(0,1);
    g.pstep(4,1);
    }
  if (--_do0397 > 0) goto Ldo0397_top; // конец do
    { // $03A7: move     a,x:(r2)+
    GEN_TRC(0x03A7);
    const uint32_t _v33 = g.r[2];
    M.wr('x', _v33, acc_to24(g.A));
    g.pstep(2,1);
    }
  { // $03A8: move     b,x:(r3)+
  GEN_TRC(0x03A8);
  const uint32_t _v34 = g.r[3];
  M.wr('x', _v34, acc_to24(g.B));
  g.pstep(3,1);
  }
  { // $03A9: rts
  GEN_TRC(0x03A9);
    return;
  }
}

void m14_handler(Mem& M, const Rom& rom, Regs& g) {
// === m14_handler: обработчик слота $145C48-$145CCD (автогенерация) ===
int _do145c77;
int _do145c93;
int _do145c95;
int _do145cac;
int _do145cc2;
  { // $145C48: move     #>$7fffff,b
  GEN_TRC(0x145C48);
  g.B = sext56(((int64_t)s24((uint32_t)(0x7FFFFFu))) << 24);
  }
  { // $145C4A: move     y:(r6+$34),a
  GEN_TRC(0x145C4A);
  const uint32_t _v1 = M.rd('y', (g.r[6] + 0x34u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v1))) << 24);
  }
  { // $145C4B: cmp      #>$40,a
  GEN_TRC(0x145C4B);
  gm_cmp(g, g.A, (int64_t)s24(0x000040u) << 24);
  }
  { // $145C4D: bge      func_145c51
  GEN_TRC(0x145C4D);
    if ((g.fn==g.fv)) goto L_145c51;
  }
  { // $145C4E: add      #>$1,a
  GEN_TRC(0x145C4E);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x000001u) << 24);
  }
  { // $145C50: clr      b
  GEN_TRC(0x145C50);
  g.B = 0;
  gm_flags56(g, g.B);
  }
L_145c51:;
  { // $145C51: move     a,y:(r6+$34)
  GEN_TRC(0x145C51);
  const uint32_t _v2 = (g.r[6] + 0x34u) & M24;
  M.wr('y', _v2, acc_to24(g.A));
  }
  { // $145C52: move     b,y:(r6+$33)
  GEN_TRC(0x145C52);
  const uint32_t _v3 = (g.r[6] + 0x33u) & M24;
  M.wr('y', _v3, acc_to24(g.B));
  }
  { // $145C53: move     #>$7fffff,a
  GEN_TRC(0x145C53);
  g.A = sext56(((int64_t)s24((uint32_t)(0x7FFFFFu))) << 24);
  }
  { // $145C55: move     y:(r6+$9),x1
  GEN_TRC(0x145C55);
  const uint32_t _v4 = M.rd('y', (g.r[6] + 0x9u) & M24);
  g.x1 = (uint32_t)(_v4 & M24);
  }
  { // $145C56: mpyi     #>$66666,x1,b
  GEN_TRC(0x145C56);
  gm_mpy(g, g.B, (int64_t)s24(0x066666u), (int64_t)s24(g.x1));
  }
  { // $145C58: move     y:(r6+$32),x1
  GEN_TRC(0x145C58);
  const uint32_t _v5 = M.rd('y', (g.r[6] + 0x32u) & M24);
  g.x1 = (uint32_t)(_v5 & M24);
  }
  { // $145C59: maci     #>$79999a,x1,b
  GEN_TRC(0x145C59);
  gm_mac(g, g.B, (int64_t)s24(0x79999Au), (int64_t)s24(g.x1), true);
  }
  { // $145C5B: move     #$c0,r5
  GEN_TRC(0x145C5B);
  g.r[5] = (uint32_t)(0x0000C0u);
  }
  { // $145C5C: move     b,y:(r6+$32)
  GEN_TRC(0x145C5C);
  const uint32_t _v6 = (g.r[6] + 0x32u) & M24;
  M.wr('y', _v6, acc_to24(g.B));
  }
  { // $145C5D: sub      x1,a            a,b
  GEN_TRC(0x145C5D);
  const uint32_t _v7 = acc_to24(g.A);
  const uint32_t _v8 = acc_to24(g.B);
  const int64_t _v9 = g.A;
  const int64_t _v10 = g.B;
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x1) << 24);
  g.B = _v9;
  }
  { // $145C5E: move     y:(r6+$a),x0
  GEN_TRC(0x145C5E);
  const uint32_t _v11 = M.rd('y', (g.r[6] + 0xAu) & M24);
  g.x0 = (uint32_t)(_v11 & M24);
  }
  { // $145C5F: move     a,y:>$c4
  GEN_TRC(0x145C5F);
  const uint32_t _v12 = 0x0000c4u;
  M.wr('y', _v12, acc_to24(g.A));
  }
  { // $145C61: tfr      b,a             b,y1
  GEN_TRC(0x145C61);
  const uint32_t _v13 = acc_to24(g.A);
  const uint32_t _v14 = acc_to24(g.B);
  const int64_t _v15 = g.A;
  const int64_t _v16 = g.B;
  g.A = g.B;
  g.y1 = _v14;
  }
  { // $145C62: maci     #>$c00000,x0,a
  GEN_TRC(0x145C62);
  gm_mac(g, g.A, (int64_t)s24(0xC00000u), (int64_t)s24(g.x0), true);
  }
  { // $145C64: mpy      y1,x1,b         y:$1,y1
  GEN_TRC(0x145C64);
  const uint32_t _v17 = M.rd('y', 0x000001u);
  gm_mpy(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1));
  g.y1 = (uint32_t)(_v17 & M24);
  }
  { // $145C65: move     a,y0
  GEN_TRC(0x145C65);
  g.y0 = acc_to24(g.A);
  }
  { // $145C66: mpy      x1,y0,a         y:$0,y0
  GEN_TRC(0x145C66);
  const uint32_t _v18 = M.rd('y', 0x000000u);
  gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0));
  g.y0 = (uint32_t)(_v18 & M24);
  }
  { // $145C67: move     y:(r6+$33),x0
  GEN_TRC(0x145C67);
  const uint32_t _v19 = M.rd('y', (g.r[6] + 0x33u) & M24);
  g.x0 = (uint32_t)(_v19 & M24);
  }
  { // $145C68: mpy      y0,x0,b         b,x1
  GEN_TRC(0x145C68);
  const uint32_t _v20 = acc_to24(g.A);
  const uint32_t _v21 = acc_to24(g.B);
  const int64_t _v22 = g.A;
  const int64_t _v23 = g.B;
  gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  g.x1 = _v21;
  }
  { // $145C69: mpy      x0,y1,a         a,x0
  GEN_TRC(0x145C69);
  const uint32_t _v24 = acc_to24(g.A);
  const uint32_t _v25 = acc_to24(g.B);
  const int64_t _v26 = g.A;
  const int64_t _v27 = g.B;
  gm_mpy(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  g.x0 = _v24;
  }
  { // $145C6A: move     b,y0
  GEN_TRC(0x145C6A);
  g.y0 = acc_to24(g.B);
  }
  { // $145C6B: mpy      y0,x0,b         a,y1
  GEN_TRC(0x145C6B);
  const uint32_t _v28 = acc_to24(g.A);
  const uint32_t _v29 = acc_to24(g.B);
  const int64_t _v30 = g.A;
  const int64_t _v31 = g.B;
  gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  g.y1 = _v28;
  }
  { // $145C6C: mpy      y1,x1,a         #$20,r1
  GEN_TRC(0x145C6C);
  gm_mpy(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x1));
  g.r[1] = (uint32_t)(0x000020u);
  }
  { // $145C6D: move     #$30,r0
  GEN_TRC(0x145C6D);
  g.r[0] = (uint32_t)(0x000030u);
  }
  { // $145C6E: mpy      x0,y1,a         a,y:(r5)+
  GEN_TRC(0x145C6E);
  const uint32_t _v32 = acc_to24(g.A);
  const uint32_t _v33 = acc_to24(g.B);
  const int64_t _v34 = g.A;
  const int64_t _v35 = g.B;
  gm_mpy(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  const uint32_t _v36 = g.r[5];
  M.wr('y', _v36, _v32);
  g.pstep(5,1);
  }
  { // $145C6F: mpy      x1,y0,b         b,y:(r5)+
  GEN_TRC(0x145C6F);
  const uint32_t _v37 = acc_to24(g.A);
  const uint32_t _v38 = acc_to24(g.B);
  const int64_t _v39 = g.A;
  const int64_t _v40 = g.B;
  gm_mpy(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0));
  const uint32_t _v41 = g.r[5];
  M.wr('y', _v41, _v38);
  g.pstep(5,1);
  }
  { // $145C70: move     a,y:(r5)+
  GEN_TRC(0x145C70);
  const uint32_t _v42 = g.r[5];
  M.wr('y', _v42, acc_to24(g.A));
  g.pstep(5,1);
  }
  { // $145C71: move     b,y:(r5)+
  GEN_TRC(0x145C71);
  const uint32_t _v43 = g.r[5];
  M.wr('y', _v43, acc_to24(g.B));
  g.pstep(5,1);
  }
  { // $145C72: move     x:(r6+$30),y1
  GEN_TRC(0x145C72);
  const uint32_t _v44 = M.rd('x', (g.r[6] + 0x30u) & M24);
  g.y1 = (uint32_t)(_v44 & M24);
  }
  { // $145C73: move     x:(r6+$31),b
  GEN_TRC(0x145C73);
  const uint32_t _v45 = M.rd('x', (g.r[6] + 0x31u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v45))) << 24);
  }
  { // $145C74: tfr      y1,a            #>$ef,x0
  GEN_TRC(0x145C74);
  g.A = (int64_t)s24(g.y1) << 24;
  g.x0 = (uint32_t)(0x0000EFu);
  }
  { // $145C76: bset     #$14,sr
  GEN_TRC(0x145C76);
    /* bset #$14,sr: флаги-модификатор (для div-последовательностей) */
  }
  _do145c77 = 16; // $145C77: do
  Ldo145c77_top:;
    { // $145C79: mac      -x0,y1,b        b,x:(r0)+       b,y0
    GEN_TRC(0x145C79);
    const uint32_t _v46 = acc_to24(g.A);
    const uint32_t _v47 = acc_to24(g.B);
    const int64_t _v48 = g.A;
    const int64_t _v49 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v50 = g.r[0];
    M.wr('x', _v50, _v47);
    g.pstep(0,1);
    g.y0 = _v47;
    }
    { // $145C7A: mac      y0,x0,a a,x:(r1)+ a,y1
    GEN_TRC(0x145C7A);
    const uint32_t _v51 = acc_to24(g.A);
    const uint32_t _v52 = acc_to24(g.B);
    const int64_t _v53 = g.A;
    const int64_t _v54 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v55 = g.r[1];
    M.wr('x', _v55, _v51);
    g.pstep(1,1);
    g.y1 = _v51;
    }
  if (--_do145c77 > 0) goto Ldo145c77_top; // конец do
    { // $145C7B: bclr     #$14,sr
    GEN_TRC(0x145C7B);
      /* bclr #$14,sr: флаги-модификатор (для div-последовательностей) */
    }
  { // $145C7C: move     #>$fffe70,n0
  GEN_TRC(0x145C7C);
  g.n[0] = (uint32_t)(0xFFFE70u);
  }
  { // $145C7E: move     y:(r6+$36),r0
  GEN_TRC(0x145C7E);
  const uint32_t _v56 = M.rd('y', (g.r[6] + 0x36u) & M24);
  g.r[0] = (uint32_t)(_v56);
  }
  { // $145C80: move     #>$3ff,m0
  GEN_TRC(0x145C80);
  g.m[0] = (uint32_t)(0x0003FFu);
  }
  { // $145C82: move     m0,m4
  GEN_TRC(0x145C82);
  g.m[4] = (uint32_t)(g.m[0]);
  }
  { // $145C83: move     b,x:(r6+$31)
  GEN_TRC(0x145C83);
  const uint32_t _v57 = (g.r[6] + 0x31u) & M24;
  M.wr('x', _v57, acc_to24(g.B));
  }
  { // $145C84: lua      (r0)+n0,r0
  GEN_TRC(0x145C84);
    g.r[0] = (uint32_t)((int64_t)g.r[0] + 1 * g.n[0]) & M24;
  }
  { // $145C85: move     #$20,r2
  GEN_TRC(0x145C85);
  g.r[2] = (uint32_t)(0x000020u);
  }
  { // $145C86: move     r0,r1
  GEN_TRC(0x145C86);
  g.r[1] = (uint32_t)(g.r[0]);
  }
  { // $145C87: move     r0,r4
  GEN_TRC(0x145C87);
  g.r[4] = (uint32_t)(g.r[0]);
  }
  { // $145C88: move     #>$154,x0
  GEN_TRC(0x145C88);
  g.x0 = (uint32_t)(0x000154u);
  }
  { // $145C8A: move     x:(r2)+,x1
  GEN_TRC(0x145C8A);
  const uint32_t _v58 = M.rd('x', g.r[2]);
  g.x1 = (uint32_t)(_v58 & M24);
  g.pstep(2,1);
  }
  { // $145C8B: mpy      x1,x0,b         x:(r2)+,x1
  GEN_TRC(0x145C8B);
  const uint32_t _v59 = M.rd('x', g.r[2]);
  gm_mpy(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
  g.x1 = (uint32_t)(_v59 & M24);
  g.pstep(2,1);
  }
  { // $145C8C: move     a,x:(r6+$30)
  GEN_TRC(0x145C8C);
  const uint32_t _v60 = (g.r[6] + 0x30u) & M24;
  M.wr('x', _v60, acc_to24(g.A));
  }
  { // $145C8D: move     b1,n4
  GEN_TRC(0x145C8D);
  g.n[4] = (int32_t)s24(acc_to24n(g.B));
  }
  { // $145C8E: move     (r0)+
  GEN_TRC(0x145C8E);
  g.pstep(0,1);
  }
  { // $145C8F: move     #$7f,r3
  GEN_TRC(0x145C8F);
  g.r[3] = (uint32_t)(0x00007Fu);
  }
  { // $145C90: move     #$f,n0
  GEN_TRC(0x145C90);
  g.n[0] = (uint32_t)(0x00000Fu);
  }
  { // $145C91: move     (r4)+n4
  GEN_TRC(0x145C91);
  g.pstep(4,g.n[4]);
  }
  { // $145C92: move     y:(r4)+,y0
  GEN_TRC(0x145C92);
  const uint32_t _v61 = M.rd('y', g.r[4]);
  g.y0 = (uint32_t)(_v61 & M24);
  g.pstep(4,1);
  }
  _do145c93 = 2; // $145C93: do
  Ldo145c93_top:;
    _do145c95 = (int)(g.n[0] & 0xFFFFFF); // $145C95: do
    Ldo145c95_top:;
      { // $145C97: mpy      x1,x0,b         b0,x1
      GEN_TRC(0x145C97);
      const uint32_t _v62 = acc_to24(g.A);
      const uint32_t _v63 = acc_to24(g.B);
      const int64_t _v64 = g.A;
      const int64_t _v65 = g.B;
      gm_mpy(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
      g.x1 = (uint32_t)(((_v65 >> 0) & M24));
      }
      { // $145C98: move     a,x:(r3)+       y:(r4)+,y1
      GEN_TRC(0x145C98);
      const uint32_t _v66 = M.rd('y', g.r[4]);
      const uint32_t _v67 = g.r[3];
      M.wr('x', _v67, acc_to24(g.A));
      g.pstep(3,1);
      g.y1 = (uint32_t)(_v66 & M24);
      g.pstep(4,1);
      }
      { // $145C99: move     r0,r4
      GEN_TRC(0x145C99);
      g.r[4] = (uint32_t)(g.r[0]);
      }
      { // $145C9A: move     b1,n4
      GEN_TRC(0x145C9A);
      g.n[4] = (int32_t)s24(acc_to24n(g.B));
      }
      { // $145C9B: move     (r0)+
      GEN_TRC(0x145C9B);
      g.pstep(0,1);
      }
      { // $145C9C: mpysu    y1,x1,a
      GEN_TRC(0x145C9C);
      gm_macsu(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x1), false);
      }
      { // $145C9D: macsu    -y0,x1,a
      GEN_TRC(0x145C9D);
      gm_macsu(g, g.A, -(int64_t)s24(g.y0), (int64_t)s24(g.x1), true);
      }
      { // $145C9E: asr      a               (r4)+n4
      GEN_TRC(0x145C9E);
      const int64_t _v68 = g.A;
      g.fc = (int)((_v68 >> 0) & 1);
      g.A = sext56(_v68 >> 1);
      gm_flags56(g, g.A);
      g.pstep(4,g.n[4]);
      }
      { // $145C9F: add      y0,a            x:(r2)+,x1      y:(r4)+,y0
      GEN_TRC(0x145C9F);
      const uint32_t _v69 = M.rd('x', g.r[2]);
      const uint32_t _v70 = M.rd('y', g.r[4]);
      g.A = acc_add56(g, g.A, (int64_t)s24(g.y0) << 24);
      g.x1 = (uint32_t)(_v69 & M24);
      g.y0 = (uint32_t)(_v70 & M24);
      g.pstep(2,1);
      g.pstep(4,1);
      }
    if (--_do145c95 > 0) goto Ldo145c95_top; // конец do
      { // $145CA0: move     #$11,n0
      GEN_TRC(0x145CA0);
      g.n[0] = (uint32_t)(0x000011u);
      }
    { // $145CA1: move     r1,r0
    GEN_TRC(0x145CA1);
    g.r[0] = (uint32_t)(g.r[1]);
    }
  if (--_do145c93 > 0) goto Ldo145c93_top; // конец do
    { // $145CA2: move     m1,m0
    GEN_TRC(0x145CA2);
    g.m[0] = (uint32_t)(g.m[1]);
    }
  { // $145CA3: move     a,x:(r3)+
  GEN_TRC(0x145CA3);
  const uint32_t _v71 = g.r[3];
  M.wr('x', _v71, acc_to24(g.A));
  g.pstep(3,1);
  }
  { // $145CA4: move     #$0,r0
  GEN_TRC(0x145CA4);
  g.r[0] = (uint32_t)(0x000000u);
  }
  { // $145CA5: move     #$10,r1
  GEN_TRC(0x145CA5);
  g.r[1] = (uint32_t)(0x000010u);
  }
  { // $145CA6: move     y:(r6+$36),r4
  GEN_TRC(0x145CA6);
  const uint32_t _v72 = M.rd('y', (g.r[6] + 0x36u) & M24);
  g.r[4] = (uint32_t)(_v72);
  }
  { // $145CA8: move     #$40,y0
  GEN_TRC(0x145CA8);
  g.y0 = (uint32_t)(0x400000u);
  }
  { // $145CA9: move     x:(r0)+,x0
  GEN_TRC(0x145CA9);
  const uint32_t _v73 = M.rd('x', g.r[0]);
  g.x0 = (uint32_t)(_v73 & M24);
  g.pstep(0,1);
  }
  { // $145CAA: mpy      y0,x0,a         x:(r1)+,x0
  GEN_TRC(0x145CAA);
  const uint32_t _v74 = M.rd('x', g.r[1]);
  gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  g.x0 = (uint32_t)(_v74 & M24);
  g.pstep(1,1);
  }
  { // $145CAB: mac      y0,x0,a         x:(r0)+,x0
  GEN_TRC(0x145CAB);
  const uint32_t _v75 = M.rd('x', g.r[0]);
  gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
  g.x0 = (uint32_t)(_v75 & M24);
  g.pstep(0,1);
  }
  _do145cac = 8; // $145CAC: do
  Ldo145cac_top:;
    { // $145CAE: mpy      y0,x0,b         x:(r1)+,x0
    GEN_TRC(0x145CAE);
    const uint32_t _v76 = M.rd('x', g.r[1]);
    gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
    g.x0 = (uint32_t)(_v76 & M24);
    g.pstep(1,1);
    }
    { // $145CAF: mac      y0,x0,b         x:(r0)+,x0      a,y:(r4)+
    GEN_TRC(0x145CAF);
    const uint32_t _v77 = M.rd('x', g.r[0]);
    const uint32_t _v78 = acc_to24(g.A);
    const uint32_t _v79 = acc_to24(g.B);
    const int64_t _v80 = g.A;
    const int64_t _v81 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x0 = (uint32_t)(_v77 & M24);
    const uint32_t _v82 = g.r[4];
    M.wr('y', _v82, _v78);
    g.pstep(4,1);
    g.pstep(0,1);
    }
    { // $145CB0: mpy      y0,x0,a         x:(r1)+,x0
    GEN_TRC(0x145CB0);
    const uint32_t _v83 = M.rd('x', g.r[1]);
    gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
    g.x0 = (uint32_t)(_v83 & M24);
    g.pstep(1,1);
    }
    { // $145CB1: mac      y0,x0,a         x:(r0)+,x0      b,y:(r4)+
    GEN_TRC(0x145CB1);
    const uint32_t _v84 = M.rd('x', g.r[0]);
    const uint32_t _v85 = acc_to24(g.A);
    const uint32_t _v86 = acc_to24(g.B);
    const int64_t _v87 = g.A;
    const int64_t _v88 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x0 = (uint32_t)(_v84 & M24);
    const uint32_t _v89 = g.r[4];
    M.wr('y', _v89, _v86);
    g.pstep(4,1);
    g.pstep(0,1);
    }
  if (--_do145cac > 0) goto Ldo145cac_top; // конец do
    { // $145CB2: move     m0,m4
    GEN_TRC(0x145CB2);
    g.m[4] = (uint32_t)(g.m[0]);
    }
  { // $145CB3: move     r4,y:(r6+$36)
  GEN_TRC(0x145CB3);
  const uint32_t _v90 = (g.r[6] + 0x36u) & M24;
  M.wr('y', _v90, (uint32_t)(g.r[4] & M24));
  }
  { // $145CB5: move     #$0,r0
  GEN_TRC(0x145CB5);
  g.r[0] = (uint32_t)(0x000000u);
  }
  { // $145CB6: move     #$f,r3
  GEN_TRC(0x145CB6);
  g.r[3] = (uint32_t)(0x00000Fu);
  }
  { // $145CB7: move     r0,r4
  GEN_TRC(0x145CB7);
  g.r[4] = (uint32_t)(g.r[0]);
  }
  { // $145CB8: move     #$80,r1
  GEN_TRC(0x145CB8);
  g.r[1] = (uint32_t)(0x000080u);
  }
  { // $145CB9: move     #$90,r2
  GEN_TRC(0x145CB9);
  g.r[2] = (uint32_t)(0x000090u);
  }
  { // $145CBA: move     #$c0,r5
  GEN_TRC(0x145CBA);
  g.r[5] = (uint32_t)(0x0000C0u);
  }
  { // $145CBB: move     #$3,m5
  GEN_TRC(0x145CBB);
  g.m[5] = (uint32_t)(0x000003u);
  }
  { // $145CBC: move     y:>$c4,y1
  GEN_TRC(0x145CBC);
  const uint32_t _v91 = M.rd('y', 0x0000c4u);
  g.y1 = (uint32_t)(_v91 & M24);
  }
  { // $145CBE: move     x:(r0)+,x1
  GEN_TRC(0x145CBE);
  const uint32_t _v92 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v92 & M24);
  g.pstep(0,1);
  }
  { // $145CBF: move     x:(r3)+,b
  GEN_TRC(0x145CBF);
  const uint32_t _v93 = M.rd('x', g.r[3]);
  g.B = sext56(((int64_t)s24((uint32_t)(_v93))) << 24);
  g.pstep(3,1);
  }
  { // $145CC0: move     x:(r3)-,x0
  GEN_TRC(0x145CC0);
  const uint32_t _v94 = M.rd('x', g.r[3]);
  g.x0 = (uint32_t)(_v94 & M24);
  g.pstep(3,-1);
  }
  { // $145CC1: move     #$2,n3
  GEN_TRC(0x145CC1);
  g.n[3] = (uint32_t)(0x000002u);
  }
  _do145cc2 = 16; // $145CC2: do
  Ldo145cc2_top:;
    { // $145CC4: mpy      y1,x1,a         b,x:(r3)+n3
    GEN_TRC(0x145CC4);
    const uint32_t _v95 = acc_to24(g.A);
    const uint32_t _v96 = acc_to24(g.B);
    const int64_t _v97 = g.A;
    const int64_t _v98 = g.B;
    gm_mpy(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x1));
    const uint32_t _v99 = g.r[3];
    M.wr('x', _v99, _v96);
    g.pstep(3,g.n[3]);
    }
    { // $145CC5: mpy      x0,y1,b         x:(r1)+,x1      y:(r5)+,y0
    GEN_TRC(0x145CC5);
    const uint32_t _v100 = M.rd('x', g.r[1]);
    const uint32_t _v101 = M.rd('y', g.r[5]);
    gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
    g.x1 = (uint32_t)(_v100 & M24);
    g.y0 = (uint32_t)(_v101 & M24);
    g.pstep(1,1);
    g.pstep(5,1);
    }
    { // $145CC6: mac      x1,y0,a         y:(r5)+,y0
    GEN_TRC(0x145CC6);
    const uint32_t _v102 = M.rd('y', g.r[5]);
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.y0 = (uint32_t)(_v102 & M24);
    g.pstep(5,1);
    }
    { // $145CC7: mac      x1,y0,b         x:(r2)+,x1      y:(r5)+,y0
    GEN_TRC(0x145CC7);
    const uint32_t _v103 = M.rd('x', g.r[2]);
    const uint32_t _v104 = M.rd('y', g.r[5]);
    gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v103 & M24);
    g.y0 = (uint32_t)(_v104 & M24);
    g.pstep(2,1);
    g.pstep(5,1);
    }
    { // $145CC8: mac      x1,y0,a         x:(r3)-,x0      y:(r5)+,y0
    GEN_TRC(0x145CC8);
    const uint32_t _v105 = M.rd('x', g.r[3]);
    const uint32_t _v106 = M.rd('y', g.r[5]);
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v105 & M24);
    g.y0 = (uint32_t)(_v106 & M24);
    g.pstep(3,-1);
    g.pstep(5,1);
    }
    { // $145CC9: mac      x1,y0,b         x:(r0)+,x1
    GEN_TRC(0x145CC9);
    const uint32_t _v107 = M.rd('x', g.r[0]);
    gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v107 & M24);
    g.pstep(0,1);
    }
    { // $145CCA: move     a,x:(r4)+
    GEN_TRC(0x145CCA);
    const uint32_t _v108 = g.r[4];
    M.wr('x', _v108, acc_to24(g.A));
    g.pstep(4,1);
    }
  if (--_do145cc2 > 0) goto Ldo145cc2_top; // конец do
    { // $145CCB: move     b,x:(r3)+
    GEN_TRC(0x145CCB);
    const uint32_t _v109 = g.r[3];
    M.wr('x', _v109, acc_to24(g.B));
    g.pstep(3,1);
    }
  { // $145CCC: move     m4,m5
  GEN_TRC(0x145CCC);
  g.m[5] = (uint32_t)(g.m[4]);
  }
  { // $145CCD: jmp      func_000981  (выход из обработчика в эпилог диспетчера)
    return;
  }
}

void chain_tail(Mem& M, const Rom& rom, Regs& g) {
// === ХВОСТ ЦЕПИ $05FF-$0B4C (автогенерация) ===
int _do0651;
int _do067b;
int _do069c;
int _do06a7;
int _do06b6;
int _do0721;
int _do0742;
int _do075c;
int _do0767;
int _do0776;
int _do07a2;
int _do07bd;
int _do07cb;
int _do07e3;
int _do07ef;
int _do081f;
int _do0844;
int _do0869;
int _do08f6;
int _do0904;
int _do0929;
int _do096e;
int _do0a2b;
int _do0a31;
int _do0a4e;
int _do0a78;
int _do0a93;
int _do0aad;
int _do0ac7;
int _do0ad9;
int _do0af6;
int _do0b1a;
int _do0b2f;
int _do0b41;
  CHK(0x05FF);
  { // $05FF: move     y:(r7-$6),x1
  GEN_TRC(0x05FF);
  const uint32_t _v1 = M.rd('y', (g.r[7] - 0x6u) & M24);
  g.x1 = (uint32_t)(_v1 & M24);
  }
  { // $0600: tfr      x1,a
  GEN_TRC(0x0600);
  g.A = (int64_t)s24(g.x1) << 24;
  }
  { // $0601: sub      #>$5bf,a
  GEN_TRC(0x0601);
  g.A = acc_sub56(g, g.A, (int64_t)s24(0x0005BFu) << 24);
  }
  { // $0603: clr      a               ifmi
  GEN_TRC(0x0603);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $0604: move     a,y0
  GEN_TRC(0x0604);
  g.y0 = acc_to24(g.A);
  }
  { // $0605: sub      #>$80,a
  GEN_TRC(0x0605);
  g.A = acc_sub56(g, g.A, (int64_t)s24(0x000080u) << 24);
  }
  { // $0607: clr      a               ifmi
  GEN_TRC(0x0607);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $0608: move     a,y1
  GEN_TRC(0x0608);
  g.y1 = acc_to24(g.A);
  }
  { // $0609: move     #>$63f,x0
  GEN_TRC(0x0609);
  g.x0 = (uint32_t)(0x00063Fu);
  }
  { // $060B: tfr      x1,a
  GEN_TRC(0x060B);
  g.A = (int64_t)s24(g.x1) << 24;
  }
  { // $060C: cmp      x0,a
  GEN_TRC(0x060C);
  gm_cmp(g, g.A, (int64_t)s24(g.x0) << 24);
  }
  { // $060D: tfr      x0,a            ifgt
  GEN_TRC(0x060D);
  if (((g.fn==g.fv)&&!g.fz)) g.A = (int64_t)s24(g.x0) << 24;
  }
  { // $060E: move     a,x1
  GEN_TRC(0x060E);
  g.x1 = acc_to24(g.A);
  }
  { // $060F: move     x1,r0
  GEN_TRC(0x060F);
  g.r[0] = (uint32_t)(g.x1);
  }
  { // $0610: move     y:(r6+$b),a
  GEN_TRC(0x0610);
  const uint32_t _v2 = M.rd('y', (g.r[6] + 0xBu) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v2))) << 24);
  }
  { // $0611: mpyi     #>$fffdf3,x1,b
  GEN_TRC(0x0611);
  gm_mpy(g, g.B, (int64_t)s24(0xFFFDF3u), (int64_t)s24(g.x1));
  }
  { // $0613: maci     #>$ffe666,y0,b
  GEN_TRC(0x0613);
  gm_mac(g, g.B, (int64_t)s24(0xFFE666u), (int64_t)s24(g.y0), true);
  }
  { // $0615: maci     #>$ffa666,y1,b
  GEN_TRC(0x0615);
  gm_mac(g, g.B, (int64_t)s24(0xFFA666u), (int64_t)s24(g.y1), true);
  }
  { // $0617: asl      #$18,b,b
  GEN_TRC(0x0617);
  const int64_t _v3 = g.B;
  g.fc = (int)((_v3 >> 32) & 1);
  g.B = sext56(_v3 << 24);
  gm_flags56(g, g.B);
  }
  { // $0618: tfr      x1,b b,y0
  GEN_TRC(0x0618);
  const uint32_t _v4 = acc_to24(g.A);
  const uint32_t _v5 = acc_to24(g.B);
  const int64_t _v6 = g.A;
  const int64_t _v7 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y0 = _v5;
  }
  { // $0619: move     a,x0
  GEN_TRC(0x0619);
  g.x0 = acc_to24(g.A);
  }
  { // $061A: mac      y0,x0,a
  GEN_TRC(0x061A);
  gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
  }
  { // $061B: clr      a               ifmi
  GEN_TRC(0x061B);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $061C: move     a,x0
  GEN_TRC(0x061C);
  g.x0 = acc_to24(g.A);
  }
  { // $061D: maci     #>$fff912,x0,b
  GEN_TRC(0x061D);
  gm_mac(g, g.B, (int64_t)s24(0xFFF912u), (int64_t)s24(g.x0), true);
  }
  { // $061F: move     x:(r7-$1e),x0
  GEN_TRC(0x061F);
  const uint32_t _v8 = M.rd('x', (g.r[7] - 0x1Eu) & M24);
  g.x0 = (uint32_t)(_v8 & M24);
  }
  { // $0620: move     y:(r7-$1e),a
  GEN_TRC(0x0620);
  const uint32_t _v9 = M.rd('y', (g.r[7] - 0x1Eu) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v9))) << 24);
  }
  { // $0621: move     b1,x:(r7-$1e)
  GEN_TRC(0x0621);
  const uint32_t _v10 = (g.r[7] - 0x1Eu) & M24;
  M.wr('x', _v10, (uint32_t)((g.B >> 24) & M24));
  }
  { // $0622: move     x1,y:(r7-$1e)
  GEN_TRC(0x0622);
  const uint32_t _v11 = (g.r[7] - 0x1Eu) & M24;
  M.wr('y', _v11, (uint32_t)(g.x1 & M24));
  }
  { // $0623: add      x1,a b1,r2
  GEN_TRC(0x0623);
  const uint32_t _v12 = acc_to24(g.A);
  const uint32_t _v13 = acc_to24(g.B);
  const int64_t _v14 = g.A;
  const int64_t _v15 = g.B;
  g.A = acc_add56(g, g.A, (int64_t)s24(g.x1) << 24);
  g.r[2] = (uint32_t)(((_v15 >> 24) & M24));
  }
  { // $0624: asr      a
  GEN_TRC(0x0624);
  const int64_t _v16 = g.A;
  g.fc = (int)((_v16 >> 0) & 1);
  g.A = sext56(_v16 >> 1);
  gm_flags56(g, g.A);
  }
  { // $0625: add      x0,b
  GEN_TRC(0x0625);
  g.B = acc_add56(g, g.B, (int64_t)s24(g.x0) << 24);
  }
  { // $0626: asr      b a1,r1
  GEN_TRC(0x0626);
  const uint32_t _v17 = acc_to24(g.A);
  const uint32_t _v18 = acc_to24(g.B);
  const int64_t _v19 = g.A;
  const int64_t _v20 = g.B;
  const int64_t _v21 = g.B;
  g.fc = (int)((_v21 >> 0) & 1);
  g.B = sext56(_v21 >> 1);
  gm_flags56(g, g.B);
  g.r[1] = (uint32_t)(((_v19 >> 24) & M24));
  }
  { // $0627: move     a0,x0
  GEN_TRC(0x0627);
  g.x0 = (uint32_t)((g.A >> 0) & M24);
  }
  { // $0628: move     b0,x1
  GEN_TRC(0x0628);
  g.x1 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0629: move     x:(r1+$141a98),y0
  GEN_TRC(0x0629);
  const uint32_t _v22 = rom.rd('x', (g.r[1] + 0x141A98u) & M24);
  g.y0 = (uint32_t)(_v22 & M24);
  }
  { // $062B: move     x:(r1+$141a99),y1
  GEN_TRC(0x062B);
  const uint32_t _v23 = rom.rd('x', (g.r[1] + 0x141A99u) & M24);
  g.y1 = (uint32_t)(_v23 & M24);
  }
  { // $062D: tfr      y0,a b1,r3
  GEN_TRC(0x062D);
  const uint32_t _v24 = acc_to24(g.A);
  const uint32_t _v25 = acc_to24(g.B);
  const int64_t _v26 = g.A;
  const int64_t _v27 = g.B;
  g.A = (int64_t)s24(g.y0) << 24;
  g.r[3] = (uint32_t)(((_v27 >> 24) & M24));
  }
  { // $062E: macsu    -y0,x0,a
  GEN_TRC(0x062E);
  gm_macsu(g, g.A, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
  }
  { // $062F: macsu    y1,x0,a
  GEN_TRC(0x062F);
  gm_macsu(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x0), true);
  }
  { // $0630: move     x:(r1+$142158),y0
  GEN_TRC(0x0630);
  const uint32_t _v28 = rom.rd('x', (g.r[1] + 0x142158u) & M24);
  g.y0 = (uint32_t)(_v28 & M24);
  }
  { // $0632: move     x:(r1+$142159),y1
  GEN_TRC(0x0632);
  const uint32_t _v29 = rom.rd('x', (g.r[1] + 0x142159u) & M24);
  g.y1 = (uint32_t)(_v29 & M24);
  }
  { // $0634: mpysu    -y0,x0,b
  GEN_TRC(0x0634);
  gm_macsu(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), false);
  }
  { // $0635: add      y0,b a,y0
  GEN_TRC(0x0635);
  const uint32_t _v30 = acc_to24(g.A);
  const uint32_t _v31 = acc_to24(g.B);
  const int64_t _v32 = g.A;
  const int64_t _v33 = g.B;
  g.B = acc_add56(g, g.B, (int64_t)s24(g.y0) << 24);
  g.y0 = _v30;
  }
  { // $0636: macsu    y1,x0,b
  GEN_TRC(0x0636);
  gm_macsu(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x0), true);
  }
  { // $0637: move     x:(r3+$142f06),x0
  GEN_TRC(0x0637);
  const uint32_t _v34 = rom.rd('x', (g.r[3] + 0x142F06u) & M24);
  g.x0 = (uint32_t)(_v34 & M24);
  }
  { // $0639: move     x:(r3+$142f07),y1
  GEN_TRC(0x0639);
  const uint32_t _v35 = rom.rd('x', (g.r[3] + 0x142F07u) & M24);
  g.y1 = (uint32_t)(_v35 & M24);
  }
  { // $063B: mpysu    y1,x1,a
  GEN_TRC(0x063B);
  gm_macsu(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x1), false);
  }
  { // $063C: add      x0,a b,y1
  GEN_TRC(0x063C);
  const uint32_t _v36 = acc_to24(g.A);
  const uint32_t _v37 = acc_to24(g.B);
  const int64_t _v38 = g.A;
  const int64_t _v39 = g.B;
  g.A = acc_add56(g, g.A, (int64_t)s24(g.x0) << 24);
  g.y1 = _v37;
  }
  { // $063D: macsu    -x0,x1,a
  GEN_TRC(0x063D);
  gm_macsu(g, g.A, -(int64_t)s24(g.x0), (int64_t)s24(g.x1), true);
  }
  { // $063E: move     #>$7fffa4,x0
  GEN_TRC(0x063E);
  g.x0 = (uint32_t)(0x7FFFA4u);
  }
  { // $0640: cmp      x0,a
  GEN_TRC(0x0640);
  gm_cmp(g, g.A, (int64_t)s24(g.x0) << 24);
  }
  { // $0641: tgt      x0,a
  GEN_TRC(0x0641);
  if (((g.fn==g.fv)&&!g.fz)) g.A = (int64_t)s24(g.x0);
  }
  { // $0642: move     a,x0
  GEN_TRC(0x0642);
  g.x0 = acc_to24(g.A);
  }
  { // $0643: mpy      x0,y1,b
  GEN_TRC(0x0643);
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  }
  { // $0644: mpy      y0,x0,a
  GEN_TRC(0x0644);
  gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  }
  { // $0645: move     b,y0
  GEN_TRC(0x0645);
  g.y0 = acc_to24(g.B);
  }
  { // $0646: add      #>$800000,a
  GEN_TRC(0x0646);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x800000u) << 24);
  }
  { // $0648: move     b,y:$1e
  GEN_TRC(0x0648);
  const uint32_t _v40 = 0x00001eu;
  M.wr('y', _v40, acc_to24(g.B));
  }
  { // $0649: move     a,y:$1d
  GEN_TRC(0x0649);
  const uint32_t _v41 = 0x00001du;
  M.wr('y', _v41, acc_to24(g.A));
  }
  { // $064A: mpy      x0,x0,b
  GEN_TRC(0x064A);
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.x0));
  }
  { // $064B: subr     a,b
  GEN_TRC(0x064B);
  g.B = acc_sub56(g, g.A, g.B);
  }
  { // $064C: sub      #>$400000,b
  GEN_TRC(0x064C);
  g.B = acc_sub56(g, g.B, (int64_t)s24(0x400000u) << 24);
  }
  { // $064E: andi     #$fe,ccr
  GEN_TRC(0x064E);
    if ((0xFE & 1) == 0) g.fc = 0; // andi ccr
  }
  { // $064F: move     #>$800,a
  GEN_TRC(0x064F);
  g.A = sext56(((int64_t)s24((uint32_t)(0x000800u))) << 24);
  }
  _do0651 = 24; // $0651: do
  Ldo0651_top:;
    { // $0653: div      y0,a
    GEN_TRC(0x0653);
      div_step(g, g.A, g.y0);
    }
  if (--_do0651 > 0) goto Ldo0651_top; // конец do
    { // $0654: move     b1,y1
    GEN_TRC(0x0654);
    g.y1 = (uint32_t)((g.B >> 24) & M24);
    }
  { // $0655: move     b0,y0
  GEN_TRC(0x0655);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0656: move     a0,x1
  GEN_TRC(0x0656);
  g.x1 = (uint32_t)((g.A >> 0) & M24);
  }
  { // $0657: mpysu    x1,y0,b
  GEN_TRC(0x0657);
  gm_macsu(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), false);
  }
  { // $0658: dmac     ss x1,y1,b
  GEN_TRC(0x0658);
  gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y1), true);
  }
  { // $0659: asl      #$a,b,b
  GEN_TRC(0x0659);
  const int64_t _v42 = g.B;
  g.fc = (int)((_v42 >> 46) & 1);
  g.B = sext56(_v42 << 10);
  gm_flags56(g, g.B);
  }
  { // $065A: move     y:(r6+$4),a
  GEN_TRC(0x065A);
  const uint32_t _v43 = M.rd('y', (g.r[6] + 0x4u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v43))) << 24);
  }
  { // $065B: asl      a b,y1
  GEN_TRC(0x065B);
  const uint32_t _v44 = acc_to24(g.A);
  const uint32_t _v45 = acc_to24(g.B);
  const int64_t _v46 = g.A;
  const int64_t _v47 = g.B;
  const int64_t _v48 = g.A;
  g.fc = (int)((_v48 >> 55) & 1);
  g.A = sext56(_v48 << 1);
  gm_flags56(g, g.A);
  g.y1 = _v45;
  }
  { // $065C: move     a,x0
  GEN_TRC(0x065C);
  g.x0 = acc_to24(g.A);
  }
  { // $065D: mpyi     #>$100,x0,a
  GEN_TRC(0x065D);
  gm_mpy(g, g.A, (int64_t)s24(0x000100u), (int64_t)s24(g.x0));
  }
  { // $065F: move     a,r1
  GEN_TRC(0x065F);
  g.r[1] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $0660: move     x:(r1+$143c06),x0
  GEN_TRC(0x0660);
  const uint32_t _v49 = rom.rd('x', (g.r[1] + 0x143C06u) & M24);
  g.x0 = (uint32_t)(_v49 & M24);
  }
  { // $0662: mpy      x0,y1,b
  GEN_TRC(0x0662);
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  }
  { // $0663: move     x:(r0+$141a98),y0
  GEN_TRC(0x0663);
  const uint32_t _v50 = rom.rd('x', (g.r[0] + 0x141A98u) & M24);
  g.y0 = (uint32_t)(_v50 & M24);
  }
  { // $0665: move     b,y:$1c
  GEN_TRC(0x0665);
  const uint32_t _v51 = 0x00001cu;
  M.wr('y', _v51, acc_to24(g.B));
  }
  { // $0666: move     x:(r0+$142158),y1
  GEN_TRC(0x0666);
  const uint32_t _v52 = rom.rd('x', (g.r[0] + 0x142158u) & M24);
  g.y1 = (uint32_t)(_v52 & M24);
  }
  { // $0668: move     x:(r2+$142f06),a
  GEN_TRC(0x0668);
  const uint32_t _v53 = rom.rd('x', (g.r[2] + 0x142F06u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v53))) << 24);
  }
  { // $066A: move     #>$7fffa4,x0
  GEN_TRC(0x066A);
  g.x0 = (uint32_t)(0x7FFFA4u);
  }
  { // $066C: cmp      x0,a
  GEN_TRC(0x066C);
  gm_cmp(g, g.A, (int64_t)s24(g.x0) << 24);
  }
  { // $066D: tfr      x0,a            ifgt
  GEN_TRC(0x066D);
  if (((g.fn==g.fv)&&!g.fz)) g.A = (int64_t)s24(g.x0) << 24;
  }
  { // $066E: move     #$5,r4
  GEN_TRC(0x066E);
  g.r[4] = (uint32_t)(0x000005u);
  }
  { // $066F: move     a,x0
  GEN_TRC(0x066F);
  g.x0 = acc_to24(g.A);
  }
  { // $0670: mpy      y0,x0,a
  GEN_TRC(0x0670);
  gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  }
  { // $0671: mpy      x0,y1,b
  GEN_TRC(0x0671);
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  }
  { // $0672: move     a,y:(r4)+
  GEN_TRC(0x0672);
  const uint32_t _v54 = g.r[4];
  M.wr('y', _v54, acc_to24(g.A));
  g.pstep(4,1);
  }
  { // $0673: move     b,y:(r4)+
  GEN_TRC(0x0673);
  const uint32_t _v55 = g.r[4];
  M.wr('y', _v55, acc_to24(g.B));
  g.pstep(4,1);
  }
  { // $0674: mpy      x0,x0,b b,y0
  GEN_TRC(0x0674);
  const uint32_t _v56 = acc_to24(g.A);
  const uint32_t _v57 = acc_to24(g.B);
  const int64_t _v58 = g.A;
  const int64_t _v59 = g.B;
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.x0));
  g.y0 = _v57;
  }
  { // $0675: subr     a,b
  GEN_TRC(0x0675);
  g.B = acc_sub56(g, g.A, g.B);
  }
  { // $0676: add      #>$400000,b
  GEN_TRC(0x0676);
  g.B = acc_add56(g, g.B, (int64_t)s24(0x400000u) << 24);
  }
  { // $0678: andi     #$fe,ccr
  GEN_TRC(0x0678);
    if ((0xFE & 1) == 0) g.fc = 0; // andi ccr
  }
  { // $0679: move     #>$800,a
  GEN_TRC(0x0679);
  g.A = sext56(((int64_t)s24((uint32_t)(0x000800u))) << 24);
  }
  _do067b = 24; // $067B: do
  Ldo067b_top:;
    { // $067D: div      y0,a
    GEN_TRC(0x067D);
      div_step(g, g.A, g.y0);
    }
  if (--_do067b > 0) goto Ldo067b_top; // конец do
    { // $067E: move     b1,y1
    GEN_TRC(0x067E);
    g.y1 = (uint32_t)((g.B >> 24) & M24);
    }
  { // $067F: move     b0,y0
  GEN_TRC(0x067F);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0680: move     a0,x1
  GEN_TRC(0x0680);
  g.x1 = (uint32_t)((g.A >> 0) & M24);
  }
  { // $0681: mpysu    x1,y0,b
  GEN_TRC(0x0681);
  gm_macsu(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), false);
  }
  { // $0682: dmac     ss x1,y1,b
  GEN_TRC(0x0682);
  gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y1), true);
  }
  { // $0683: move     #$2,n0
  GEN_TRC(0x0683);
  g.n[0] = (uint32_t)(0x000002u);
  }
  { // $0684: asl      #$a,b,b
  GEN_TRC(0x0684);
  const int64_t _v60 = g.B;
  g.fc = (int)((_v60 >> 46) & 1);
  g.B = sext56(_v60 << 10);
  gm_flags56(g, g.B);
  }
  { // $0685: move     y:(r6+$4),a
  GEN_TRC(0x0685);
  const uint32_t _v61 = M.rd('y', (g.r[6] + 0x4u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v61))) << 24);
  }
  { // $0686: asl      a b,y1
  GEN_TRC(0x0686);
  const uint32_t _v62 = acc_to24(g.A);
  const uint32_t _v63 = acc_to24(g.B);
  const int64_t _v64 = g.A;
  const int64_t _v65 = g.B;
  const int64_t _v66 = g.A;
  g.fc = (int)((_v66 >> 55) & 1);
  g.A = sext56(_v66 << 1);
  gm_flags56(g, g.A);
  g.y1 = _v63;
  }
  { // $0687: move     #$5,r0
  GEN_TRC(0x0687);
  g.r[0] = (uint32_t)(0x000005u);
  }
  { // $0688: move     a,x0
  GEN_TRC(0x0688);
  g.x0 = acc_to24(g.A);
  }
  { // $0689: mpyi     #>$100,x0,a
  GEN_TRC(0x0689);
  gm_mpy(g, g.A, (int64_t)s24(0x000100u), (int64_t)s24(g.x0));
  }
  { // $068B: move     a,r1
  GEN_TRC(0x068B);
  g.r[1] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $068C: move     x:(r1+$143c06),x0
  GEN_TRC(0x068C);
  const uint32_t _v67 = rom.rd('x', (g.r[1] + 0x143C06u) & M24);
  g.x0 = (uint32_t)(_v67 & M24);
  }
  { // $068E: move     #$4,r1
  GEN_TRC(0x068E);
  g.r[1] = (uint32_t)(0x000004u);
  }
  { // $068F: mpy      x0,y1,b y:(r0)+,a
  GEN_TRC(0x068F);
  const uint32_t _v68 = M.rd('y', g.r[0]);
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  g.A = sext56(((int64_t)s24((uint32_t)(_v68))) << 24);
  g.pstep(0,1);
  }
  { // $0690: add      #>$800000,a
  GEN_TRC(0x0690);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x800000u) << 24);
  }
  { // $0692: move     b,y:$4
  GEN_TRC(0x0692);
  const uint32_t _v69 = 0x000004u;
  M.wr('y', _v69, acc_to24(g.B));
  }
  { // $0693: move     l:(r7),x
  GEN_TRC(0x0693);
  const uint64_t _v70 = M.rdL(g.r[7]);
  g.x1 = (uint32_t)((_v70 >> 24) & M24); g.x0 = (uint32_t)(_v70 & M24);
  }
  { // $0694: move     a,y:(r7)
  GEN_TRC(0x0694);
  const uint32_t _v71 = g.r[7];
  M.wr('y', _v71, acc_to24(g.A));
  }
  { // $0695: move     y:$1d,a
  GEN_TRC(0x0695);
  const uint32_t _v72 = M.rd('y', 0x00001du);
  g.A = sext56(((int64_t)s24((uint32_t)(_v72))) << 24);
  }
  { // $0696: sub      x0,a y:(r0)-,y1
  GEN_TRC(0x0696);
  const uint32_t _v73 = M.rd('y', g.r[0]);
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x0) << 24);
  g.y1 = (uint32_t)(_v73 & M24);
  g.pstep(0,-1);
  }
  { // $0697: move     y1,x:(r7)
  GEN_TRC(0x0697);
  const uint32_t _v74 = g.r[7];
  M.wr('x', _v74, (uint32_t)(g.y1 & M24));
  }
  { // $0698: move     y:$1e,b
  GEN_TRC(0x0698);
  const uint32_t _v75 = M.rd('y', 0x00001eu);
  g.B = sext56(((int64_t)s24((uint32_t)(_v75))) << 24);
  }
  { // $0699: sub      x1,b a,y0
  GEN_TRC(0x0699);
  const uint32_t _v76 = acc_to24(g.A);
  const uint32_t _v77 = acc_to24(g.B);
  const int64_t _v78 = g.A;
  const int64_t _v79 = g.B;
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.x1) << 24);
  g.y0 = _v76;
  }
  { // $069A: tfr      x0,a #$10,x0
  GEN_TRC(0x069A);
  g.A = (int64_t)s24(g.x0) << 24;
  g.x0 = (uint32_t)(0x100000u);
  }
  { // $069B: tfr      x1,b            b,y1
  GEN_TRC(0x069B);
  const uint32_t _v80 = acc_to24(g.A);
  const uint32_t _v81 = acc_to24(g.B);
  const int64_t _v82 = g.A;
  const int64_t _v83 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y1 = _v81;
  }
  _do069c = 8; // $069C: do
  Ldo069c_top:;
    { // $069E: mac      y0,x0,a a,y:(r0)+
    GEN_TRC(0x069E);
    const uint32_t _v84 = acc_to24(g.A);
    const uint32_t _v85 = acc_to24(g.B);
    const int64_t _v86 = g.A;
    const int64_t _v87 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v88 = g.r[0];
    M.wr('y', _v88, _v84);
    g.pstep(0,1);
    }
    { // $069F: mac      x0,y1,b b,y:(r0)+n0
    GEN_TRC(0x069F);
    const uint32_t _v89 = acc_to24(g.A);
    const uint32_t _v90 = acc_to24(g.B);
    const int64_t _v91 = g.A;
    const int64_t _v92 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v93 = g.r[0];
    M.wr('y', _v93, _v90);
    g.pstep(0,g.n[0]);
    }
  if (--_do069c > 0) goto Ldo069c_top; // конец do
    { // $06A0: move     y:(r0)+,x0
    GEN_TRC(0x06A0);
    const uint32_t _v94 = M.rd('y', g.r[0]);
    g.x0 = (uint32_t)(_v94 & M24);
    g.pstep(0,1);
    }
  { // $06A1: move     l:(r7)+,ba
  GEN_TRC(0x06A1);
  const uint64_t _v95 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)s24((uint32_t)((_v95 >> 24) & M24)) << 24); g.A = sext56(s24((uint32_t)((_v95) & M24)) << 24);
  g.B = sext56((int64_t)s24((uint32_t)((_v95 >> 24) & M24)) << 24); g.A = sext56((int64_t)s24((uint32_t)((_v95) & M24)) << 24);
  g.pstep(7,1);
  }
  { // $06A2: sub      x0,a y:(r0)-,x1
  GEN_TRC(0x06A2);
  const uint32_t _v96 = M.rd('y', g.r[0]);
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x0) << 24);
  g.x1 = (uint32_t)(_v96 & M24);
  g.pstep(0,-1);
  }
  { // $06A3: sub      x1,b
  GEN_TRC(0x06A3);
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.x1) << 24);
  }
  { // $06A4: tfr      x0,a            a,y0
  GEN_TRC(0x06A4);
  const uint32_t _v97 = acc_to24(g.A);
  const uint32_t _v98 = acc_to24(g.B);
  const int64_t _v99 = g.A;
  const int64_t _v100 = g.B;
  g.A = (int64_t)s24(g.x0) << 24;
  g.y0 = _v97;
  }
  { // $06A5: tfr      x1,b            b,y1
  GEN_TRC(0x06A5);
  const uint32_t _v101 = acc_to24(g.A);
  const uint32_t _v102 = acc_to24(g.B);
  const int64_t _v103 = g.A;
  const int64_t _v104 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y1 = _v102;
  }
  { // $06A6: move     #$10,x0
  GEN_TRC(0x06A6);
  g.x0 = (uint32_t)(0x100000u);
  }
  _do06a7 = 8; // $06A7: do
  Ldo06a7_top:;
    { // $06A9: mac      y0,x0,a a,y:(r0)+
    GEN_TRC(0x06A9);
    const uint32_t _v105 = acc_to24(g.A);
    const uint32_t _v106 = acc_to24(g.B);
    const int64_t _v107 = g.A;
    const int64_t _v108 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v109 = g.r[0];
    M.wr('y', _v109, _v105);
    g.pstep(0,1);
    }
    { // $06AA: mac      x0,y1,b b,y:(r0)+n0
    GEN_TRC(0x06AA);
    const uint32_t _v110 = acc_to24(g.A);
    const uint32_t _v111 = acc_to24(g.B);
    const int64_t _v112 = g.A;
    const int64_t _v113 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v114 = g.r[0];
    M.wr('y', _v114, _v111);
    g.pstep(0,g.n[0]);
    }
  if (--_do06a7 > 0) goto Ldo06a7_top; // конец do
    { // $06AB: move     #$3,n0
    GEN_TRC(0x06AB);
    g.n[0] = (uint32_t)(0x000003u);
    }
  { // $06AC: move     n0,n1
  GEN_TRC(0x06AC);
  g.n[1] = (uint32_t)(g.n[0]);
  }
  { // $06AD: move     #$1c,r0
  GEN_TRC(0x06AD);
  g.r[0] = (uint32_t)(0x00001Cu);
  }
  { // $06AE: move     y:$1c,x1
  GEN_TRC(0x06AE);
  const uint32_t _v115 = M.rd('y', 0x00001cu);
  g.x1 = (uint32_t)(_v115 & M24);
  }
  { // $06AF: tfr      x1,a y:(r1),b
  GEN_TRC(0x06AF);
  const uint32_t _v116 = M.rd('y', g.r[1]);
  g.A = (int64_t)s24(g.x1) << 24;
  g.B = sext56(((int64_t)s24((uint32_t)(_v116))) << 24);
  }
  { // $06B0: move     y:(r7),x0
  GEN_TRC(0x06B0);
  const uint32_t _v117 = M.rd('y', g.r[7]);
  g.x0 = (uint32_t)(_v117 & M24);
  }
  { // $06B1: sub      x0,a b,y:(r7)+
  GEN_TRC(0x06B1);
  const uint32_t _v118 = acc_to24(g.A);
  const uint32_t _v119 = acc_to24(g.B);
  const int64_t _v120 = g.A;
  const int64_t _v121 = g.B;
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x0) << 24);
  const uint32_t _v122 = g.r[7];
  M.wr('y', _v122, _v119);
  g.pstep(7,1);
  }
  { // $06B2: sub      x1,b
  GEN_TRC(0x06B2);
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.x1) << 24);
  }
  { // $06B3: tfr      x0,a            a,y0
  GEN_TRC(0x06B3);
  const uint32_t _v123 = acc_to24(g.A);
  const uint32_t _v124 = acc_to24(g.B);
  const int64_t _v125 = g.A;
  const int64_t _v126 = g.B;
  g.A = (int64_t)s24(g.x0) << 24;
  g.y0 = _v123;
  }
  { // $06B4: tfr      x1,b            b,y1
  GEN_TRC(0x06B4);
  const uint32_t _v127 = acc_to24(g.A);
  const uint32_t _v128 = acc_to24(g.B);
  const int64_t _v129 = g.A;
  const int64_t _v130 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y1 = _v128;
  }
  { // $06B5: move     #$10,x0
  GEN_TRC(0x06B5);
  g.x0 = (uint32_t)(0x100000u);
  }
  _do06b6 = 8; // $06B6: do
  Ldo06b6_top:;
    { // $06B8: mac      y0,x0,a a,y:(r1)+n1
    GEN_TRC(0x06B8);
    const uint32_t _v131 = acc_to24(g.A);
    const uint32_t _v132 = acc_to24(g.B);
    const int64_t _v133 = g.A;
    const int64_t _v134 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v135 = g.r[1];
    M.wr('y', _v135, _v131);
    g.pstep(1,g.n[1]);
    }
    { // $06B9: mac      x0,y1,b b,y:(r0)+n0
    GEN_TRC(0x06B9);
    const uint32_t _v136 = acc_to24(g.A);
    const uint32_t _v137 = acc_to24(g.B);
    const int64_t _v138 = g.A;
    const int64_t _v139 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v140 = g.r[0];
    M.wr('y', _v140, _v137);
    g.pstep(0,g.n[0]);
    }
  if (--_do06b6 > 0) goto Ldo06b6_top; // конец do
    { // $06BA: move     #$74,r0
    GEN_TRC(0x06BA);
    g.r[0] = (uint32_t)(0x000074u);
    }
  { // $06BB: move     r0,r1
  GEN_TRC(0x06BB);
  g.r[1] = (uint32_t)(g.r[0]);
  }
  { // $06BC: move     #$4,r4
  GEN_TRC(0x06BC);
  g.r[4] = (uint32_t)(0x000004u);
  }
  { // $06BD: move     l:(r7)+,a
  GEN_TRC(0x06BD);
  const uint64_t _v141 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v141 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $06BE: move     l:(r7)-,b
  GEN_TRC(0x06BE);
  const uint64_t _v142 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)(_v142 & 0xFFFFFFFFFFll));
  g.pstep(7,-1);
  }
  { // $06BF: jsr      func_000350
  GEN_TRC(0x06BF);
    func_000350(M, rom, g);
  }
  { // $06C0: move     a,l:(r7)+
  GEN_TRC(0x06C0);
  const uint32_t _v143 = g.r[7];
  M.wrL(_v143, (uint64_t)g.A & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $06C1: move     b,l:(r7)+
  GEN_TRC(0x06C1);
  const uint32_t _v144 = g.r[7];
  M.wrL(_v144, (uint64_t)g.B & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  CHK(0x06C2);
  { // $06C2: move     x:(r6+$f),x0
  GEN_TRC(0x06C2);
  const uint32_t _v145 = M.rd('x', (g.r[6] + 0xFu) & M24);
  g.x0 = (uint32_t)(_v145 & M24);
  }
  { // $06C3: jset     #$0,x0,func_0006cb
  GEN_TRC(0x06C3);
    if (((g.x0 >> 0) & 1)) goto L_0006cb;
  }
  { // $06C5: move     #$b4,r0
  GEN_TRC(0x06C5);
  g.r[0] = (uint32_t)(0x0000B4u);
  }
  { // $06C6: move     r0,r1
  GEN_TRC(0x06C6);
  g.r[1] = (uint32_t)(g.r[0]);
  }
  { // $06C7: move     #$4,r4
  GEN_TRC(0x06C7);
  g.r[4] = (uint32_t)(0x000004u);
  }
  { // $06C8: move     l:(r7)+,a
  GEN_TRC(0x06C8);
  const uint64_t _v146 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v146 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $06C9: move     l:(r7)-,b
  GEN_TRC(0x06C9);
  const uint64_t _v147 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)(_v147 & 0xFFFFFFFFFFll));
  g.pstep(7,-1);
  }
  { // $06CA: jsr      func_000350
  GEN_TRC(0x06CA);
    func_000350(M, rom, g);
  }
L_0006cb:;
  { // $06CB: move     a,l:(r7)+
  GEN_TRC(0x06CB);
  const uint32_t _v148 = g.r[7];
  M.wrL(_v148, (uint64_t)g.A & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $06CC: move     b,l:(r7)+
  GEN_TRC(0x06CC);
  const uint32_t _v149 = g.r[7];
  M.wrL(_v149, (uint64_t)g.B & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $06CD: move     x:(r7-$d),x1
  GEN_TRC(0x06CD);
  const uint32_t _v150 = M.rd('x', (g.r[7] - 0xDu) & M24);
  g.x1 = (uint32_t)(_v150 & M24);
  }
  { // $06CE: tfr      x1,a
  GEN_TRC(0x06CE);
  g.A = (int64_t)s24(g.x1) << 24;
  }
  { // $06CF: sub      #>$5bf,a
  GEN_TRC(0x06CF);
  g.A = acc_sub56(g, g.A, (int64_t)s24(0x0005BFu) << 24);
  }
  { // $06D1: clr      a               ifmi
  GEN_TRC(0x06D1);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $06D2: move     a,y0
  GEN_TRC(0x06D2);
  g.y0 = acc_to24(g.A);
  }
  { // $06D3: sub      #>$80,a
  GEN_TRC(0x06D3);
  g.A = acc_sub56(g, g.A, (int64_t)s24(0x000080u) << 24);
  }
  { // $06D5: clr      a               ifmi
  GEN_TRC(0x06D5);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $06D6: move     a,y1
  GEN_TRC(0x06D6);
  g.y1 = acc_to24(g.A);
  }
  { // $06D7: move     #>$63f,x0
  GEN_TRC(0x06D7);
  g.x0 = (uint32_t)(0x00063Fu);
  }
  { // $06D9: tfr      x1,a
  GEN_TRC(0x06D9);
  g.A = (int64_t)s24(g.x1) << 24;
  }
  { // $06DA: cmp      x0,a
  GEN_TRC(0x06DA);
  gm_cmp(g, g.A, (int64_t)s24(g.x0) << 24);
  }
  { // $06DB: tfr      x0,a            ifgt
  GEN_TRC(0x06DB);
  if (((g.fn==g.fv)&&!g.fz)) g.A = (int64_t)s24(g.x0) << 24;
  }
  { // $06DC: tst      a
  GEN_TRC(0x06DC);
  gm_test56(g, g.A);
  }
  { // $06DD: clr      a               ifmi
  GEN_TRC(0x06DD);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $06DE: move     a,x1
  GEN_TRC(0x06DE);
  g.x1 = acc_to24(g.A);
  }
  { // $06DF: move     x1,r0
  GEN_TRC(0x06DF);
  g.r[0] = (uint32_t)(g.x1);
  }
  { // $06E0: move     y:(r6+$a),a
  GEN_TRC(0x06E0);
  const uint32_t _v151 = M.rd('y', (g.r[6] + 0xAu) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v151))) << 24);
  }
  { // $06E1: mpyi     #>$fffdf3,x1,b
  GEN_TRC(0x06E1);
  gm_mpy(g, g.B, (int64_t)s24(0xFFFDF3u), (int64_t)s24(g.x1));
  }
  { // $06E3: maci     #>$ffe666,y0,b
  GEN_TRC(0x06E3);
  gm_mac(g, g.B, (int64_t)s24(0xFFE666u), (int64_t)s24(g.y0), true);
  }
  { // $06E5: maci     #>$ffd99a,y1,b
  GEN_TRC(0x06E5);
  gm_mac(g, g.B, (int64_t)s24(0xFFD99Au), (int64_t)s24(g.y1), true);
  }
  { // $06E7: asl      #$18,b,b
  GEN_TRC(0x06E7);
  const int64_t _v152 = g.B;
  g.fc = (int)((_v152 >> 32) & 1);
  g.B = sext56(_v152 << 24);
  gm_flags56(g, g.B);
  }
  { // $06E8: tfr      x1,b b,y0
  GEN_TRC(0x06E8);
  const uint32_t _v153 = acc_to24(g.A);
  const uint32_t _v154 = acc_to24(g.B);
  const int64_t _v155 = g.A;
  const int64_t _v156 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y0 = _v154;
  }
  { // $06E9: move     a,x0
  GEN_TRC(0x06E9);
  g.x0 = acc_to24(g.A);
  }
  { // $06EA: mac      y0,x0,a
  GEN_TRC(0x06EA);
  gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
  }
  { // $06EB: clr      a               ifmi
  GEN_TRC(0x06EB);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $06EC: move     a,x0
  GEN_TRC(0x06EC);
  g.x0 = acc_to24(g.A);
  }
  { // $06ED: maci     #>$fff912,x0,b
  GEN_TRC(0x06ED);
  gm_mac(g, g.B, (int64_t)s24(0xFFF912u), (int64_t)s24(g.x0), true);
  }
  { // $06EF: move     x:(r7-$25),x0
  GEN_TRC(0x06EF);
  const uint32_t _v157 = M.rd('x', (g.r[7] - 0x25u) & M24);
  g.x0 = (uint32_t)(_v157 & M24);
  }
  { // $06F0: move     y:(r7-$25),a
  GEN_TRC(0x06F0);
  const uint32_t _v158 = M.rd('y', (g.r[7] - 0x25u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v158))) << 24);
  }
  { // $06F1: move     b1,x:(r7-$25)
  GEN_TRC(0x06F1);
  const uint32_t _v159 = (g.r[7] - 0x25u) & M24;
  M.wr('x', _v159, (uint32_t)((g.B >> 24) & M24));
  }
  { // $06F2: move     x1,y:(r7-$25)
  GEN_TRC(0x06F2);
  const uint32_t _v160 = (g.r[7] - 0x25u) & M24;
  M.wr('y', _v160, (uint32_t)(g.x1 & M24));
  }
  { // $06F3: add      x1,a b1,r2
  GEN_TRC(0x06F3);
  const uint32_t _v161 = acc_to24(g.A);
  const uint32_t _v162 = acc_to24(g.B);
  const int64_t _v163 = g.A;
  const int64_t _v164 = g.B;
  g.A = acc_add56(g, g.A, (int64_t)s24(g.x1) << 24);
  g.r[2] = (uint32_t)(((_v164 >> 24) & M24));
  }
  { // $06F4: asr      a
  GEN_TRC(0x06F4);
  const int64_t _v165 = g.A;
  g.fc = (int)((_v165 >> 0) & 1);
  g.A = sext56(_v165 >> 1);
  gm_flags56(g, g.A);
  }
  { // $06F5: add      x0,b
  GEN_TRC(0x06F5);
  g.B = acc_add56(g, g.B, (int64_t)s24(g.x0) << 24);
  }
  { // $06F6: asr      b a,r1
  GEN_TRC(0x06F6);
  const uint32_t _v166 = acc_to24(g.A);
  const uint32_t _v167 = acc_to24(g.B);
  const int64_t _v168 = g.A;
  const int64_t _v169 = g.B;
  const int64_t _v170 = g.B;
  g.fc = (int)((_v170 >> 0) & 1);
  g.B = sext56(_v170 >> 1);
  gm_flags56(g, g.B);
  g.r[1] = (uint32_t)(_v166);
  }
  { // $06F7: move     a0,x0
  GEN_TRC(0x06F7);
  g.x0 = (uint32_t)((g.A >> 0) & M24);
  }
  { // $06F8: move     b0,x1
  GEN_TRC(0x06F8);
  g.x1 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $06F9: move     x:(r1+$141a98),y0
  GEN_TRC(0x06F9);
  const uint32_t _v171 = rom.rd('x', (g.r[1] + 0x141A98u) & M24);
  g.y0 = (uint32_t)(_v171 & M24);
  }
  { // $06FB: move     x:(r1+$141a99),y1
  GEN_TRC(0x06FB);
  const uint32_t _v172 = rom.rd('x', (g.r[1] + 0x141A99u) & M24);
  g.y1 = (uint32_t)(_v172 & M24);
  }
  { // $06FD: tfr      y0,a b1,r3
  GEN_TRC(0x06FD);
  const uint32_t _v173 = acc_to24(g.A);
  const uint32_t _v174 = acc_to24(g.B);
  const int64_t _v175 = g.A;
  const int64_t _v176 = g.B;
  g.A = (int64_t)s24(g.y0) << 24;
  g.r[3] = (uint32_t)(((_v176 >> 24) & M24));
  }
  { // $06FE: macsu    -y0,x0,a
  GEN_TRC(0x06FE);
  gm_macsu(g, g.A, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
  }
  { // $06FF: macsu    y1,x0,a
  GEN_TRC(0x06FF);
  gm_macsu(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x0), true);
  }
  { // $0700: move     x:(r1+$142158),y0
  GEN_TRC(0x0700);
  const uint32_t _v177 = rom.rd('x', (g.r[1] + 0x142158u) & M24);
  g.y0 = (uint32_t)(_v177 & M24);
  }
  { // $0702: move     x:(r1+$142159),y1
  GEN_TRC(0x0702);
  const uint32_t _v178 = rom.rd('x', (g.r[1] + 0x142159u) & M24);
  g.y1 = (uint32_t)(_v178 & M24);
  }
  { // $0704: mpysu    -y0,x0,b
  GEN_TRC(0x0704);
  gm_macsu(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), false);
  }
  { // $0705: add      y0,b a,y0
  GEN_TRC(0x0705);
  const uint32_t _v179 = acc_to24(g.A);
  const uint32_t _v180 = acc_to24(g.B);
  const int64_t _v181 = g.A;
  const int64_t _v182 = g.B;
  g.B = acc_add56(g, g.B, (int64_t)s24(g.y0) << 24);
  g.y0 = _v179;
  }
  { // $0706: macsu    y1,x0,b
  GEN_TRC(0x0706);
  gm_macsu(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x0), true);
  }
  { // $0707: move     x:(r3+$142f07),y1
  GEN_TRC(0x0707);
  const uint32_t _v183 = rom.rd('x', (g.r[3] + 0x142F07u) & M24);
  g.y1 = (uint32_t)(_v183 & M24);
  }
  { // $0709: move     x:(r3+$142f06),x0
  GEN_TRC(0x0709);
  const uint32_t _v184 = rom.rd('x', (g.r[3] + 0x142F06u) & M24);
  g.x0 = (uint32_t)(_v184 & M24);
  }
  { // $070B: mpysu    y1,x1,a
  GEN_TRC(0x070B);
  gm_macsu(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x1), false);
  }
  { // $070C: add      x0,a b,y1
  GEN_TRC(0x070C);
  const uint32_t _v185 = acc_to24(g.A);
  const uint32_t _v186 = acc_to24(g.B);
  const int64_t _v187 = g.A;
  const int64_t _v188 = g.B;
  g.A = acc_add56(g, g.A, (int64_t)s24(g.x0) << 24);
  g.y1 = _v186;
  }
  { // $070D: macsu    -x0,x1,a
  GEN_TRC(0x070D);
  gm_macsu(g, g.A, -(int64_t)s24(g.x0), (int64_t)s24(g.x1), true);
  }
  { // $070E: move     #>$7fffa4,x0
  GEN_TRC(0x070E);
  g.x0 = (uint32_t)(0x7FFFA4u);
  }
  { // $0710: cmp      x0,a
  GEN_TRC(0x0710);
  gm_cmp(g, g.A, (int64_t)s24(g.x0) << 24);
  }
  { // $0711: tgt      x0,a
  GEN_TRC(0x0711);
  if (((g.fn==g.fv)&&!g.fz)) g.A = (int64_t)s24(g.x0);
  }
  { // $0712: move     a,x0
  GEN_TRC(0x0712);
  g.x0 = acc_to24(g.A);
  }
  { // $0713: mpy      x0,y1,b
  GEN_TRC(0x0713);
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  }
  { // $0714: mpy      y0,x0,a
  GEN_TRC(0x0714);
  gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  }
  { // $0715: move     b,y0
  GEN_TRC(0x0715);
  g.y0 = acc_to24(g.B);
  }
  { // $0716: add      #>$800000,a
  GEN_TRC(0x0716);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x800000u) << 24);
  }
  { // $0718: move     b,y:$1e
  GEN_TRC(0x0718);
  const uint32_t _v189 = 0x00001eu;
  M.wr('y', _v189, acc_to24(g.B));
  }
  { // $0719: move     a,y:$1d
  GEN_TRC(0x0719);
  const uint32_t _v190 = 0x00001du;
  M.wr('y', _v190, acc_to24(g.A));
  }
  { // $071A: mpy      x0,x0,b
  GEN_TRC(0x071A);
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.x0));
  }
  { // $071B: subr     a,b
  GEN_TRC(0x071B);
  g.B = acc_sub56(g, g.A, g.B);
  }
  { // $071C: sub      #>$400000,b
  GEN_TRC(0x071C);
  g.B = acc_sub56(g, g.B, (int64_t)s24(0x400000u) << 24);
  }
  { // $071E: andi     #$fe,ccr
  GEN_TRC(0x071E);
    if ((0xFE & 1) == 0) g.fc = 0; // andi ccr
  }
  { // $071F: move     #>$800,a
  GEN_TRC(0x071F);
  g.A = sext56(((int64_t)s24((uint32_t)(0x000800u))) << 24);
  }
  _do0721 = 24; // $0721: do
  Ldo0721_top:;
    { // $0723: div      y0,a
    GEN_TRC(0x0723);
      div_step(g, g.A, g.y0);
    }
  if (--_do0721 > 0) goto Ldo0721_top; // конец do
    { // $0724: move     b1,y1
    GEN_TRC(0x0724);
    g.y1 = (uint32_t)((g.B >> 24) & M24);
    }
  { // $0725: move     b0,y0
  GEN_TRC(0x0725);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0726: move     a0,x1
  GEN_TRC(0x0726);
  g.x1 = (uint32_t)((g.A >> 0) & M24);
  }
  { // $0727: mpysu    x1,y0,b
  GEN_TRC(0x0727);
  gm_macsu(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), false);
  }
  { // $0728: dmac     ss x1,y1,b
  GEN_TRC(0x0728);
  gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y1), true);
  }
  { // $0729: asl      #$a,b,b
  GEN_TRC(0x0729);
  const int64_t _v191 = g.B;
  g.fc = (int)((_v191 >> 46) & 1);
  g.B = sext56(_v191 << 10);
  gm_flags56(g, g.B);
  }
  { // $072A: move     x:(r0+$141a98),y0
  GEN_TRC(0x072A);
  const uint32_t _v192 = rom.rd('x', (g.r[0] + 0x141A98u) & M24);
  g.y0 = (uint32_t)(_v192 & M24);
  }
  { // $072C: move     b,y:$1c
  GEN_TRC(0x072C);
  const uint32_t _v193 = 0x00001cu;
  M.wr('y', _v193, acc_to24(g.B));
  }
  { // $072D: move     x:(r0+$142158),y1
  GEN_TRC(0x072D);
  const uint32_t _v194 = rom.rd('x', (g.r[0] + 0x142158u) & M24);
  g.y1 = (uint32_t)(_v194 & M24);
  }
  { // $072F: move     x:(r2+$142f06),a
  GEN_TRC(0x072F);
  const uint32_t _v195 = rom.rd('x', (g.r[2] + 0x142F06u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v195))) << 24);
  }
  { // $0731: move     #>$7fffa4,x0
  GEN_TRC(0x0731);
  g.x0 = (uint32_t)(0x7FFFA4u);
  }
  { // $0733: cmp      x0,a
  GEN_TRC(0x0733);
  gm_cmp(g, g.A, (int64_t)s24(g.x0) << 24);
  }
  { // $0734: tgt      x0,a
  GEN_TRC(0x0734);
  if (((g.fn==g.fv)&&!g.fz)) g.A = (int64_t)s24(g.x0);
  }
  { // $0735: move     #$5,r4
  GEN_TRC(0x0735);
  g.r[4] = (uint32_t)(0x000005u);
  }
  { // $0736: move     a,x0
  GEN_TRC(0x0736);
  g.x0 = acc_to24(g.A);
  }
  { // $0737: mpy      y0,x0,a
  GEN_TRC(0x0737);
  gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  }
  { // $0738: mpy      x0,y1,b
  GEN_TRC(0x0738);
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  }
  { // $0739: move     a,y:(r4)+
  GEN_TRC(0x0739);
  const uint32_t _v196 = g.r[4];
  M.wr('y', _v196, acc_to24(g.A));
  g.pstep(4,1);
  }
  { // $073A: move     b,y0
  GEN_TRC(0x073A);
  g.y0 = acc_to24(g.B);
  }
  { // $073B: mpy      x0,x0,b b,y:(r4)+
  GEN_TRC(0x073B);
  const uint32_t _v197 = acc_to24(g.A);
  const uint32_t _v198 = acc_to24(g.B);
  const int64_t _v199 = g.A;
  const int64_t _v200 = g.B;
  gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.x0));
  const uint32_t _v201 = g.r[4];
  M.wr('y', _v201, _v198);
  g.pstep(4,1);
  }
  { // $073C: subr     a,b
  GEN_TRC(0x073C);
  g.B = acc_sub56(g, g.A, g.B);
  }
  { // $073D: add      #>$400000,b
  GEN_TRC(0x073D);
  g.B = acc_add56(g, g.B, (int64_t)s24(0x400000u) << 24);
  }
  { // $073F: andi     #$fe,ccr
  GEN_TRC(0x073F);
    if ((0xFE & 1) == 0) g.fc = 0; // andi ccr
  }
  { // $0740: move     #>$800,a
  GEN_TRC(0x0740);
  g.A = sext56(((int64_t)s24((uint32_t)(0x000800u))) << 24);
  }
  _do0742 = 24; // $0742: do
  Ldo0742_top:;
    { // $0744: div      y0,a
    GEN_TRC(0x0744);
      div_step(g, g.A, g.y0);
    }
  if (--_do0742 > 0) goto Ldo0742_top; // конец do
    { // $0745: move     b1,y1
    GEN_TRC(0x0745);
    g.y1 = (uint32_t)((g.B >> 24) & M24);
    }
  { // $0746: move     b0,y0
  GEN_TRC(0x0746);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0747: move     a0,x1
  GEN_TRC(0x0747);
  g.x1 = (uint32_t)((g.A >> 0) & M24);
  }
  { // $0748: mpysu    x1,y0,b
  GEN_TRC(0x0748);
  gm_macsu(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), false);
  }
  { // $0749: dmac     ss x1,y1,b
  GEN_TRC(0x0749);
  gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y1), true);
  }
  { // $074A: move     #$2,n0
  GEN_TRC(0x074A);
  g.n[0] = (uint32_t)(0x000002u);
  }
  { // $074B: asl      #$a,b,b
  GEN_TRC(0x074B);
  const int64_t _v202 = g.B;
  g.fc = (int)((_v202 >> 46) & 1);
  g.B = sext56(_v202 << 10);
  gm_flags56(g, g.B);
  }
  { // $074C: move     b,y1
  GEN_TRC(0x074C);
  g.y1 = acc_to24(g.B);
  }
  { // $074D: move     #$5,r0
  GEN_TRC(0x074D);
  g.r[0] = (uint32_t)(0x000005u);
  }
  { // $074E: move     #$4,r1
  GEN_TRC(0x074E);
  g.r[1] = (uint32_t)(0x000004u);
  }
  { // $074F: move     y:(r0)+,a
  GEN_TRC(0x074F);
  const uint32_t _v203 = M.rd('y', g.r[0]);
  g.A = sext56(((int64_t)s24((uint32_t)(_v203))) << 24);
  g.pstep(0,1);
  }
  { // $0750: add      #>$800000,a
  GEN_TRC(0x0750);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x800000u) << 24);
  }
  { // $0752: move     b,y:$4
  GEN_TRC(0x0752);
  const uint32_t _v204 = 0x000004u;
  M.wr('y', _v204, acc_to24(g.B));
  }
  { // $0753: move     l:(r7),x
  GEN_TRC(0x0753);
  const uint64_t _v205 = M.rdL(g.r[7]);
  g.x1 = (uint32_t)((_v205 >> 24) & M24); g.x0 = (uint32_t)(_v205 & M24);
  }
  { // $0754: move     a,y:(r7)
  GEN_TRC(0x0754);
  const uint32_t _v206 = g.r[7];
  M.wr('y', _v206, acc_to24(g.A));
  }
  { // $0755: move     y:$1d,a
  GEN_TRC(0x0755);
  const uint32_t _v207 = M.rd('y', 0x00001du);
  g.A = sext56(((int64_t)s24((uint32_t)(_v207))) << 24);
  }
  { // $0756: sub      x0,a y:(r0)-,y1
  GEN_TRC(0x0756);
  const uint32_t _v208 = M.rd('y', g.r[0]);
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x0) << 24);
  g.y1 = (uint32_t)(_v208 & M24);
  g.pstep(0,-1);
  }
  { // $0757: move     y1,x:(r7)
  GEN_TRC(0x0757);
  const uint32_t _v209 = g.r[7];
  M.wr('x', _v209, (uint32_t)(g.y1 & M24));
  }
  { // $0758: move     y:$1e,b
  GEN_TRC(0x0758);
  const uint32_t _v210 = M.rd('y', 0x00001eu);
  g.B = sext56(((int64_t)s24((uint32_t)(_v210))) << 24);
  }
  { // $0759: sub      x1,b a,y0
  GEN_TRC(0x0759);
  const uint32_t _v211 = acc_to24(g.A);
  const uint32_t _v212 = acc_to24(g.B);
  const int64_t _v213 = g.A;
  const int64_t _v214 = g.B;
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.x1) << 24);
  g.y0 = _v211;
  }
  { // $075A: tfr      x0,a #$10,x0
  GEN_TRC(0x075A);
  g.A = (int64_t)s24(g.x0) << 24;
  g.x0 = (uint32_t)(0x100000u);
  }
  { // $075B: tfr      x1,b            b,y1
  GEN_TRC(0x075B);
  const uint32_t _v215 = acc_to24(g.A);
  const uint32_t _v216 = acc_to24(g.B);
  const int64_t _v217 = g.A;
  const int64_t _v218 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y1 = _v216;
  }
  _do075c = 8; // $075C: do
  Ldo075c_top:;
    { // $075E: mac      y0,x0,a a,y:(r0)+
    GEN_TRC(0x075E);
    const uint32_t _v219 = acc_to24(g.A);
    const uint32_t _v220 = acc_to24(g.B);
    const int64_t _v221 = g.A;
    const int64_t _v222 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v223 = g.r[0];
    M.wr('y', _v223, _v219);
    g.pstep(0,1);
    }
    { // $075F: mac      x0,y1,b b,y:(r0)+n0
    GEN_TRC(0x075F);
    const uint32_t _v224 = acc_to24(g.A);
    const uint32_t _v225 = acc_to24(g.B);
    const int64_t _v226 = g.A;
    const int64_t _v227 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v228 = g.r[0];
    M.wr('y', _v228, _v225);
    g.pstep(0,g.n[0]);
    }
  if (--_do075c > 0) goto Ldo075c_top; // конец do
    { // $0760: move     y:(r0)+,x0
    GEN_TRC(0x0760);
    const uint32_t _v229 = M.rd('y', g.r[0]);
    g.x0 = (uint32_t)(_v229 & M24);
    g.pstep(0,1);
    }
  { // $0761: move     l:(r7)+,ba
  GEN_TRC(0x0761);
  const uint64_t _v230 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)s24((uint32_t)((_v230 >> 24) & M24)) << 24); g.A = sext56(s24((uint32_t)((_v230) & M24)) << 24);
  g.B = sext56((int64_t)s24((uint32_t)((_v230 >> 24) & M24)) << 24); g.A = sext56((int64_t)s24((uint32_t)((_v230) & M24)) << 24);
  g.pstep(7,1);
  }
  { // $0762: sub      x0,a y:(r0)-,x1
  GEN_TRC(0x0762);
  const uint32_t _v231 = M.rd('y', g.r[0]);
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x0) << 24);
  g.x1 = (uint32_t)(_v231 & M24);
  g.pstep(0,-1);
  }
  { // $0763: sub      x1,b
  GEN_TRC(0x0763);
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.x1) << 24);
  }
  { // $0764: tfr      x0,a            a,y0
  GEN_TRC(0x0764);
  const uint32_t _v232 = acc_to24(g.A);
  const uint32_t _v233 = acc_to24(g.B);
  const int64_t _v234 = g.A;
  const int64_t _v235 = g.B;
  g.A = (int64_t)s24(g.x0) << 24;
  g.y0 = _v232;
  }
  { // $0765: tfr      x1,b            b,y1
  GEN_TRC(0x0765);
  const uint32_t _v236 = acc_to24(g.A);
  const uint32_t _v237 = acc_to24(g.B);
  const int64_t _v238 = g.A;
  const int64_t _v239 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y1 = _v237;
  }
  { // $0766: move     #$10,x0
  GEN_TRC(0x0766);
  g.x0 = (uint32_t)(0x100000u);
  }
  _do0767 = 8; // $0767: do
  Ldo0767_top:;
    { // $0769: mac      y0,x0,a a,y:(r0)+
    GEN_TRC(0x0769);
    const uint32_t _v240 = acc_to24(g.A);
    const uint32_t _v241 = acc_to24(g.B);
    const int64_t _v242 = g.A;
    const int64_t _v243 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v244 = g.r[0];
    M.wr('y', _v244, _v240);
    g.pstep(0,1);
    }
    { // $076A: mac      x0,y1,b b,y:(r0)+n0
    GEN_TRC(0x076A);
    const uint32_t _v245 = acc_to24(g.A);
    const uint32_t _v246 = acc_to24(g.B);
    const int64_t _v247 = g.A;
    const int64_t _v248 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v249 = g.r[0];
    M.wr('y', _v249, _v246);
    g.pstep(0,g.n[0]);
    }
  if (--_do0767 > 0) goto Ldo0767_top; // конец do
    { // $076B: move     #$3,n0
    GEN_TRC(0x076B);
    g.n[0] = (uint32_t)(0x000003u);
    }
  { // $076C: move     n0,n1
  GEN_TRC(0x076C);
  g.n[1] = (uint32_t)(g.n[0]);
  }
  { // $076D: move     #$1c,r0
  GEN_TRC(0x076D);
  g.r[0] = (uint32_t)(0x00001Cu);
  }
  { // $076E: move     y:$1c,x1
  GEN_TRC(0x076E);
  const uint32_t _v250 = M.rd('y', 0x00001cu);
  g.x1 = (uint32_t)(_v250 & M24);
  }
  { // $076F: tfr      x1,a y:(r1),b
  GEN_TRC(0x076F);
  const uint32_t _v251 = M.rd('y', g.r[1]);
  g.A = (int64_t)s24(g.x1) << 24;
  g.B = sext56(((int64_t)s24((uint32_t)(_v251))) << 24);
  }
  { // $0770: move     y:(r7),x0
  GEN_TRC(0x0770);
  const uint32_t _v252 = M.rd('y', g.r[7]);
  g.x0 = (uint32_t)(_v252 & M24);
  }
  { // $0771: sub      x0,a b,y:(r7)+
  GEN_TRC(0x0771);
  const uint32_t _v253 = acc_to24(g.A);
  const uint32_t _v254 = acc_to24(g.B);
  const int64_t _v255 = g.A;
  const int64_t _v256 = g.B;
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x0) << 24);
  const uint32_t _v257 = g.r[7];
  M.wr('y', _v257, _v254);
  g.pstep(7,1);
  }
  { // $0772: sub      x1,b
  GEN_TRC(0x0772);
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.x1) << 24);
  }
  { // $0773: tfr      x0,a            a,y0
  GEN_TRC(0x0773);
  const uint32_t _v258 = acc_to24(g.A);
  const uint32_t _v259 = acc_to24(g.B);
  const int64_t _v260 = g.A;
  const int64_t _v261 = g.B;
  g.A = (int64_t)s24(g.x0) << 24;
  g.y0 = _v258;
  }
  { // $0774: tfr      x1,b            b,y1
  GEN_TRC(0x0774);
  const uint32_t _v262 = acc_to24(g.A);
  const uint32_t _v263 = acc_to24(g.B);
  const int64_t _v264 = g.A;
  const int64_t _v265 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y1 = _v263;
  }
  { // $0775: move     #$10,x0
  GEN_TRC(0x0775);
  g.x0 = (uint32_t)(0x100000u);
  }
  _do0776 = 8; // $0776: do
  Ldo0776_top:;
    { // $0778: mac      y0,x0,a a,y:(r1)+n1
    GEN_TRC(0x0778);
    const uint32_t _v266 = acc_to24(g.A);
    const uint32_t _v267 = acc_to24(g.B);
    const int64_t _v268 = g.A;
    const int64_t _v269 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v270 = g.r[1];
    M.wr('y', _v270, _v266);
    g.pstep(1,g.n[1]);
    }
    { // $0779: mac      x0,y1,b b,y:(r0)+n0
    GEN_TRC(0x0779);
    const uint32_t _v271 = acc_to24(g.A);
    const uint32_t _v272 = acc_to24(g.B);
    const int64_t _v273 = g.A;
    const int64_t _v274 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v275 = g.r[0];
    M.wr('y', _v275, _v272);
    g.pstep(0,g.n[0]);
    }
  if (--_do0776 > 0) goto Ldo0776_top; // конец do
    { // $077A: move     #$0,r5
    GEN_TRC(0x077A);
    g.r[5] = (uint32_t)(0x000000u);
    }
  { // $077B: move     #$1,r2
  GEN_TRC(0x077B);
  g.r[2] = (uint32_t)(0x000001u);
  }
  { // $077C: move     #>$fffffe,n7
  GEN_TRC(0x077C);
  g.n[7] = (uint32_t)(0xFFFFFEu);
  }
  { // $077E: move     #$74,r0
  GEN_TRC(0x077E);
  g.r[0] = (uint32_t)(0x000074u);
  }
  { // $077F: move     #$72,r1
  GEN_TRC(0x077F);
  g.r[1] = (uint32_t)(0x000072u);
  }
  { // $0780: move     #$4,r4
  GEN_TRC(0x0780);
  g.r[4] = (uint32_t)(0x000004u);
  }
  { // $0781: move     #$10,x0
  GEN_TRC(0x0781);
  g.x0 = (uint32_t)(0x100000u);
  }
  { // $0782: move     x0,y:(r5)
  GEN_TRC(0x0782);
  const uint32_t _v276 = g.r[5];
  M.wr('y', _v276, (uint32_t)(g.x0 & M24));
  }
  { // $0783: move     l:(r7)+,y
  GEN_TRC(0x0783);
  const uint64_t _v277 = M.rdL(g.r[7]);
  g.y1 = (uint32_t)((_v277 >> 24) & M24); g.y0 = (uint32_t)(_v277 & M24);
  g.pstep(7,1);
  }
  { // $0784: move     l:(r7)+,a
  GEN_TRC(0x0784);
  const uint64_t _v278 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v278 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $0785: move     l:(r7)+n7,b
  GEN_TRC(0x0785);
  const uint64_t _v279 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)(_v279 & 0xFFFFFFFFFFll));
  g.pstep(7,g.n[7]);
  }
  { // $0786: move     y0,x:(r1)+
  GEN_TRC(0x0786);
  const uint32_t _v280 = g.r[1];
  M.wr('x', _v280, (uint32_t)(g.y0 & M24));
  g.pstep(1,1);
  }
  { // $0787: move     y1,x:(r1)-
  GEN_TRC(0x0787);
  const uint32_t _v281 = g.r[1];
  M.wr('x', _v281, (uint32_t)(g.y1 & M24));
  g.pstep(1,-1);
  }
  { // $0788: jsr      func_000365
  GEN_TRC(0x0788);
    func_000365(M, rom, g);
  }
  CHK(0x0789);
  { // $0789: move     x:(r1)+,x0
  GEN_TRC(0x0789);
  const uint32_t _v282 = M.rd('x', g.r[1]);
  g.x0 = (uint32_t)(_v282 & M24);
  g.pstep(1,1);
  }
  { // $078A: move     x:(r1),x1
  GEN_TRC(0x078A);
  const uint32_t _v283 = M.rd('x', g.r[1]);
  g.x1 = (uint32_t)(_v283 & M24);
  }
  { // $078B: move     x,l:(r7)+
  GEN_TRC(0x078B);
  const uint32_t _v284 = g.r[7];
  M.wr('x', _v284, g.x1); M.wr('y', _v284, g.x0);
  g.pstep(7,1);
  }
  { // $078C: move     a,l:(r7)+
  GEN_TRC(0x078C);
  const uint32_t _v285 = g.r[7];
  M.wrL(_v285, (uint64_t)g.A & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $078D: move     b,l:(r7)+
  GEN_TRC(0x078D);
  const uint32_t _v286 = g.r[7];
  M.wrL(_v286, (uint64_t)g.B & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $078E: move     x:(r6+$f),x0
  GEN_TRC(0x078E);
  const uint32_t _v287 = M.rd('x', (g.r[6] + 0xFu) & M24);
  g.x0 = (uint32_t)(_v287 & M24);
  }
  { // $078F: jset     #$0,x0,func_00079c
  GEN_TRC(0x078F);
    if (((g.x0 >> 0) & 1)) goto L_00079c;
  }
  { // $0791: move     #$b4,r0
  GEN_TRC(0x0791);
  g.r[0] = (uint32_t)(0x0000B4u);
  }
  { // $0792: move     #$b2,r1
  GEN_TRC(0x0792);
  g.r[1] = (uint32_t)(0x0000B2u);
  }
  { // $0793: move     #$4,r4
  GEN_TRC(0x0793);
  g.r[4] = (uint32_t)(0x000004u);
  }
  { // $0794: move     l:(r7)+,y
  GEN_TRC(0x0794);
  const uint64_t _v288 = M.rdL(g.r[7]);
  g.y1 = (uint32_t)((_v288 >> 24) & M24); g.y0 = (uint32_t)(_v288 & M24);
  g.pstep(7,1);
  }
  { // $0795: move     l:(r7)+,a
  GEN_TRC(0x0795);
  const uint64_t _v289 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v289 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $0796: move     l:(r7)+n7,b
  GEN_TRC(0x0796);
  const uint64_t _v290 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)(_v290 & 0xFFFFFFFFFFll));
  g.pstep(7,g.n[7]);
  }
  { // $0797: move     y0,x:(r1)+
  GEN_TRC(0x0797);
  const uint32_t _v291 = g.r[1];
  M.wr('x', _v291, (uint32_t)(g.y0 & M24));
  g.pstep(1,1);
  }
  { // $0798: move     y1,x:(r1)-
  GEN_TRC(0x0798);
  const uint32_t _v292 = g.r[1];
  M.wr('x', _v292, (uint32_t)(g.y1 & M24));
  g.pstep(1,-1);
  }
  { // $0799: jsr      func_000365
  GEN_TRC(0x0799);
    func_000365(M, rom, g);
  }
  { // $079A: move     x:(r1)+,x0
  GEN_TRC(0x079A);
  const uint32_t _v293 = M.rd('x', g.r[1]);
  g.x0 = (uint32_t)(_v293 & M24);
  g.pstep(1,1);
  }
  { // $079B: move     x:(r1),x1
  GEN_TRC(0x079B);
  const uint32_t _v294 = M.rd('x', g.r[1]);
  g.x1 = (uint32_t)(_v294 & M24);
  }
L_00079c:;
  { // $079C: move     x,l:(r7)+
  GEN_TRC(0x079C);
  const uint32_t _v295 = g.r[7];
  M.wr('x', _v295, g.x1); M.wr('y', _v295, g.x0);
  g.pstep(7,1);
  }
  { // $079D: move     a,l:(r7)+
  GEN_TRC(0x079D);
  const uint32_t _v296 = g.r[7];
  M.wrL(_v296, (uint64_t)g.A & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $079E: move     b,l:(r7)+
  GEN_TRC(0x079E);
  const uint32_t _v297 = g.r[7];
  M.wrL(_v297, (uint64_t)g.B & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $079F: move     x:(r6+$b),y0
  GEN_TRC(0x079F);
  const uint32_t _v298 = M.rd('x', (g.r[6] + 0xBu) & M24);
  g.y0 = (uint32_t)(_v298 & M24);
  }
  { // $07A0: move     #$8,a
  GEN_TRC(0x07A0);
  g.A = sext56(((int64_t)s24((uint32_t)(0x000008u))) << 24);
  }
  { // $07A1: andi     #$fe,ccr
  GEN_TRC(0x07A1);
    if ((0xFE & 1) == 0) g.fc = 0; // andi ccr
  }
  _do07a2 = 24; // $07A2: do
  Ldo07a2_top:;
    { // $07A4: div      y0,a
    GEN_TRC(0x07A4);
      div_step(g, g.A, g.y0);
    }
  if (--_do07a2 > 0) goto Ldo07a2_top; // конец do
    { // $07A5: move     a0,y1
    GEN_TRC(0x07A5);
    g.y1 = (uint32_t)((g.A >> 0) & M24);
    }
  { // $07A6: move     y:(r6+$4),a
  GEN_TRC(0x07A6);
  const uint32_t _v299 = M.rd('y', (g.r[6] + 0x4u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v299))) << 24);
  }
  { // $07A7: asl      a #$b2,r0
  GEN_TRC(0x07A7);
  const int64_t _v300 = g.A;
  g.fc = (int)((_v300 >> 55) & 1);
  g.A = sext56(_v300 << 1);
  gm_flags56(g, g.A);
  g.r[0] = (uint32_t)(0x0000B2u);
  }
  { // $07A8: add      #>$800000,a
  GEN_TRC(0x07A8);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x800000u) << 24);
  }
  { // $07AA: clr      a               ifmi
  GEN_TRC(0x07AA);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $07AB: move     #$72,r1
  GEN_TRC(0x07AB);
  g.r[1] = (uint32_t)(0x000072u);
  }
  { // $07AC: move     a,x0
  GEN_TRC(0x07AC);
  g.x0 = acc_to24(g.A);
  }
  { // $07AD: mpyi     #>$100,x0,b
  GEN_TRC(0x07AD);
  gm_mpy(g, g.B, (int64_t)s24(0x000100u), (int64_t)s24(g.x0));
  }
  { // $07AF: mpy      x0,x0,a #$70,r5
  GEN_TRC(0x07AF);
  gm_mpy(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.x0));
  g.r[5] = (uint32_t)(0x000070u);
  }
  { // $07B0: move     #$b0,r4
  GEN_TRC(0x07B0);
  g.r[4] = (uint32_t)(0x0000B0u);
  }
  { // $07B1: move     b,r2
  GEN_TRC(0x07B1);
  g.r[2] = (int32_t)s24(acc_to24n(g.B));
  }
  { // $07B2: move     a,x1
  GEN_TRC(0x07B2);
  g.x1 = acc_to24(g.A);
  }
  { // $07B3: mpyi     #>$7deccd,x1,a
  GEN_TRC(0x07B3);
  gm_mpy(g, g.A, (int64_t)s24(0x7DECCDu), (int64_t)s24(g.x1));
  }
  { // $07B5: add      #>$21333,a
  GEN_TRC(0x07B5);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x021333u) << 24);
  }
  { // $07B7: move     x:(r2+$1447c6),x1
  GEN_TRC(0x07B7);
  const uint32_t _v301 = rom.rd('x', (g.r[2] + 0x1447C6u) & M24);
  g.x1 = (uint32_t)(_v301 & M24);
  }
  { // $07B9: mpy      x1,y0,b a,y0
  GEN_TRC(0x07B9);
  const uint32_t _v302 = acc_to24(g.A);
  const uint32_t _v303 = acc_to24(g.B);
  const int64_t _v304 = g.A;
  const int64_t _v305 = g.B;
  gm_mpy(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0));
  g.y0 = _v302;
  }
  { // $07BA: mpy      y1,y0,a
  GEN_TRC(0x07BA);
  gm_mpy(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.y0));
  }
  { // $07BB: move     b,x1
  GEN_TRC(0x07BB);
  g.x1 = acc_to24(g.B);
  }
  { // $07BC: move     x:(r0)+,x0 a,y0
  GEN_TRC(0x07BC);
  const uint32_t _v306 = M.rd('x', g.r[0]);
  g.x0 = (uint32_t)(_v306 & M24);
  g.y0 = acc_to24(g.A);
  g.pstep(0,1);
  }
  _do07bd = 34; // $07BD: do
  Ldo07bd_top:;
    { // $07BF: mpy      y0,x0,a x:(r1)+,x0 a,y:(r4)+
    GEN_TRC(0x07BF);
    const uint32_t _v307 = M.rd('x', g.r[1]);
    const uint32_t _v308 = acc_to24(g.A);
    const uint32_t _v309 = acc_to24(g.B);
    const int64_t _v310 = g.A;
    const int64_t _v311 = g.B;
    gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
    g.x0 = (uint32_t)(_v307 & M24);
    const uint32_t _v312 = g.r[4];
    M.wr('y', _v312, _v308);
    g.pstep(4,1);
    g.pstep(1,1);
    }
    { // $07C0: asl      #$8,a,a
    GEN_TRC(0x07C0);
    const int64_t _v313 = g.A;
    g.fc = (int)((_v313 >> 48) & 1);
    g.A = sext56(_v313 << 8);
    gm_flags56(g, g.A);
    }
    { // $07C1: mpy      y0,x0,b x:(r0)+,x0 b,y:(r5)+
    GEN_TRC(0x07C1);
    const uint32_t _v314 = M.rd('x', g.r[0]);
    const uint32_t _v315 = acc_to24(g.A);
    const uint32_t _v316 = acc_to24(g.B);
    const int64_t _v317 = g.A;
    const int64_t _v318 = g.B;
    gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
    g.x0 = (uint32_t)(_v314 & M24);
    const uint32_t _v319 = g.r[5];
    M.wr('y', _v319, _v316);
    g.pstep(5,1);
    g.pstep(0,1);
    }
    { // $07C2: asl      #$8,b,b
    GEN_TRC(0x07C2);
    const int64_t _v320 = g.B;
    g.fc = (int)((_v320 >> 48) & 1);
    g.B = sext56(_v320 << 8);
    gm_flags56(g, g.B);
    }
  if (--_do07bd > 0) goto Ldo07bd_top; // конец do
    { // $07C3: bset     #$b,sr
    GEN_TRC(0x07C3);
      /* bset #$B,sr: флаги-модификатор (для div-последовательностей) */
    }
  { // $07C4: move     #$70,r5
  GEN_TRC(0x07C4);
  g.r[5] = (uint32_t)(0x000070u);
  }
  { // $07C5: move     #$b0,r4
  GEN_TRC(0x07C5);
  g.r[4] = (uint32_t)(0x0000B0u);
  }
  { // $07C6: move     r5,r1
  GEN_TRC(0x07C6);
  g.r[1] = (uint32_t)(g.r[5]);
  }
  { // $07C7: move     r4,r0
  GEN_TRC(0x07C7);
  g.r[0] = (uint32_t)(g.r[4]);
  }
  { // $07C8: move     #$80,x0
  GEN_TRC(0x07C8);
  g.x0 = (uint32_t)(0x800000u);
  }
  { // $07C9: move     y:(r4)+,y0
  GEN_TRC(0x07C9);
  const uint32_t _v321 = M.rd('y', g.r[4]);
  g.y0 = (uint32_t)(_v321 & M24);
  g.pstep(4,1);
  }
  { // $07CA: move     y:(r5)+,y1
  GEN_TRC(0x07CA);
  const uint32_t _v322 = M.rd('y', g.r[5]);
  g.y1 = (uint32_t)(_v322 & M24);
  g.pstep(5,1);
  }
  _do07cb = 34; // $07CB: do
  Ldo07cb_top:;
    { // $07CD: mpy      y0,x0,a a,x:(r0)+ y:(r4)+,y0
    GEN_TRC(0x07CD);
    const uint32_t _v323 = M.rd('y', g.r[4]);
    const uint32_t _v324 = acc_to24(g.A);
    const uint32_t _v325 = acc_to24(g.B);
    const int64_t _v326 = g.A;
    const int64_t _v327 = g.B;
    gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
    const uint32_t _v328 = g.r[0];
    M.wr('x', _v328, _v324);
    g.pstep(0,1);
    g.y0 = (uint32_t)(_v323 & M24);
    g.pstep(4,1);
    }
    { // $07CE: mpy      x0,y1,b b,x:(r1)+ y:(r5)+,y1
    GEN_TRC(0x07CE);
    const uint32_t _v329 = M.rd('y', g.r[5]);
    const uint32_t _v330 = acc_to24(g.A);
    const uint32_t _v331 = acc_to24(g.B);
    const int64_t _v332 = g.A;
    const int64_t _v333 = g.B;
    gm_mpy(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
    const uint32_t _v334 = g.r[1];
    M.wr('x', _v334, _v331);
    g.pstep(1,1);
    g.y1 = (uint32_t)(_v329 & M24);
    g.pstep(5,1);
    }
  if (--_do07cb > 0) goto Ldo07cb_top; // конец do
    { // $07CF: move     a,x:(r0)+
    GEN_TRC(0x07CF);
    const uint32_t _v335 = g.r[0];
    M.wr('x', _v335, acc_to24(g.A));
    g.pstep(0,1);
    }
  { // $07D0: move     b,x:(r1)+
  GEN_TRC(0x07D0);
  const uint32_t _v336 = g.r[1];
  M.wr('x', _v336, acc_to24(g.B));
  g.pstep(1,1);
  }
  { // $07D1: bclr     #$b,sr
  GEN_TRC(0x07D1);
    /* bclr #$B,sr: флаги-модификатор (для div-последовательностей) */
  }
  CHK(0x07D2);
  { // $07D2: move     #>$6a3,x0
  GEN_TRC(0x07D2);
  g.x0 = (uint32_t)(0x0006A3u);
  }
  { // $07D4: move     y:(r7-$14),a
  GEN_TRC(0x07D4);
  const uint32_t _v337 = M.rd('y', (g.r[7] - 0x14u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v337))) << 24);
  }
  { // $07D5: cmp      x0,a x1,y1
  GEN_TRC(0x07D5);
  gm_cmp(g, g.A, (int64_t)s24(g.x0) << 24);
  g.y1 = (uint32_t)(g.x1);
  }
  { // $07D6: tfr      x0,a ifge
  GEN_TRC(0x07D6);
  if ((g.fn==g.fv)) g.A = (int64_t)s24(g.x0) << 24;
  }
  { // $07D7: move     l:(r7),x
  GEN_TRC(0x07D7);
  const uint64_t _v338 = M.rdL(g.r[7]);
  g.x1 = (uint32_t)((_v338 >> 24) & M24); g.x0 = (uint32_t)(_v338 & M24);
  }
  { // $07D8: move     a,r0
  GEN_TRC(0x07D8);
  g.r[0] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $07D9: move     x:(r0+$143546),y0
  GEN_TRC(0x07D9);
  const uint32_t _v339 = rom.rd('x', (g.r[0] + 0x143546u) & M24);
  g.y0 = (uint32_t)(_v339 & M24);
  }
  { // $07DB: mpy      y1,y0,b y0,a
  GEN_TRC(0x07DB);
  gm_mpy(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.y0));
  g.A = sext56(((int64_t)s24((uint32_t)(g.y0))) << 24);
  }
  { // $07DC: move     #$72,r0
  GEN_TRC(0x07DC);
  g.r[0] = (uint32_t)(0x000072u);
  }
  { // $07DD: move     #$4,r4
  GEN_TRC(0x07DD);
  g.r[4] = (uint32_t)(0x000004u);
  }
  { // $07DE: sub      x0,a ba,l:(r7)+
  GEN_TRC(0x07DE);
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x0) << 24);
  const uint32_t _v340 = g.r[7];
  M.wr('x', _v340, (uint32_t)(g.B >> 24)); M.wr('y', _v340, (uint32_t)(g.A >> 24));
  g.pstep(7,1);
  }
  { // $07DF: sub      x1,b
  GEN_TRC(0x07DF);
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.x1) << 24);
  }
  { // $07E0: move     a,y0
  GEN_TRC(0x07E0);
  g.y0 = acc_to24(g.A);
  }
  { // $07E1: tfr      x1,b            b,y1
  GEN_TRC(0x07E1);
  const uint32_t _v341 = acc_to24(g.A);
  const uint32_t _v342 = acc_to24(g.B);
  const int64_t _v343 = g.A;
  const int64_t _v344 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y1 = _v342;
  }
  { // $07E2: tfr      x0,a #$8,x0
  GEN_TRC(0x07E2);
  g.A = (int64_t)s24(g.x0) << 24;
  g.x0 = (uint32_t)(0x080000u);
  }
  _do07e3 = 16; // $07E3: do
  Ldo07e3_top:;
    { // $07E5: mac      y0,x0,a         a,y:(r4)+
    GEN_TRC(0x07E5);
    const uint32_t _v345 = acc_to24(g.A);
    const uint32_t _v346 = acc_to24(g.B);
    const int64_t _v347 = g.A;
    const int64_t _v348 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v349 = g.r[4];
    M.wr('y', _v349, _v345);
    g.pstep(4,1);
    }
    { // $07E6: mac      x0,y1,b         b,y:(r4)+
    GEN_TRC(0x07E6);
    const uint32_t _v350 = acc_to24(g.A);
    const uint32_t _v351 = acc_to24(g.B);
    const int64_t _v352 = g.A;
    const int64_t _v353 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v354 = g.r[4];
    M.wr('y', _v354, _v351);
    g.pstep(4,1);
    }
  if (--_do07e3 > 0) goto Ldo07e3_top; // конец do
    { // $07E7: move     #$4,r4
    GEN_TRC(0x07E7);
    g.r[4] = (uint32_t)(0x000004u);
    }
  { // $07E8: move     r0,r1
  GEN_TRC(0x07E8);
  g.r[1] = (uint32_t)(g.r[0]);
  }
  { // $07E9: move     #$b2,r2
  GEN_TRC(0x07E9);
  g.r[2] = (uint32_t)(0x0000B2u);
  }
  { // $07EA: move     r2,r3
  GEN_TRC(0x07EA);
  g.r[3] = (uint32_t)(g.r[2]);
  }
  { // $07EB: move     l:(r7)+,a
  GEN_TRC(0x07EB);
  const uint64_t _v355 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v355 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $07EC: move     l:(r7)-,b
  GEN_TRC(0x07EC);
  const uint64_t _v356 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)(_v356 & 0xFFFFFFFFFFll));
  g.pstep(7,-1);
  }
  { // $07ED: move     x:(r0)+,x0      y:(r4)+,y0
  GEN_TRC(0x07ED);
  const uint32_t _v357 = M.rd('x', g.r[0]);
  const uint32_t _v358 = M.rd('y', g.r[4]);
  g.x0 = (uint32_t)(_v357 & M24);
  g.y0 = (uint32_t)(_v358 & M24);
  g.pstep(0,1);
  g.pstep(4,1);
  }
  { // $07EE: move     y:(r4)+,x1
  GEN_TRC(0x07EE);
  const uint32_t _v359 = M.rd('y', g.r[4]);
  g.x1 = (uint32_t)(_v359 & M24);
  g.pstep(4,1);
  }
  _do07ef = 16; // $07EF: do
  Ldo07ef_top:;
    { // $07F1: mac      x1,x0,a a,x:(r1)+ a,y1
    GEN_TRC(0x07F1);
    const uint32_t _v360 = acc_to24(g.A);
    const uint32_t _v361 = acc_to24(g.B);
    const int64_t _v362 = g.A;
    const int64_t _v363 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0), true);
    const uint32_t _v364 = g.r[1];
    M.wr('x', _v364, _v360);
    g.pstep(1,1);
    g.y1 = _v360;
    }
    { // $07F2: mac      -y1,y0,a x:(r2)+,x0
    GEN_TRC(0x07F2);
    const uint32_t _v365 = M.rd('x', g.r[2]);
    gm_mac(g, g.A, -(int64_t)s24(g.y1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v365 & M24);
    g.pstep(2,1);
    }
    { // $07F3: mac      x1,x0,b b,x:(r3)+ b,y1
    GEN_TRC(0x07F3);
    const uint32_t _v366 = acc_to24(g.A);
    const uint32_t _v367 = acc_to24(g.B);
    const int64_t _v368 = g.A;
    const int64_t _v369 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.x0), true);
    const uint32_t _v370 = g.r[3];
    M.wr('x', _v370, _v367);
    g.pstep(3,1);
    g.y1 = _v367;
    }
    { // $07F4: mac      -y1,y0,b x:(r0)+,x0
    GEN_TRC(0x07F4);
    const uint32_t _v371 = M.rd('x', g.r[0]);
    gm_mac(g, g.B, -(int64_t)s24(g.y1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v371 & M24);
    g.pstep(0,1);
    }
    { // $07F5: mac      x1,x0,a a,x:(r1)+ a,y1
    GEN_TRC(0x07F5);
    const uint32_t _v372 = acc_to24(g.A);
    const uint32_t _v373 = acc_to24(g.B);
    const int64_t _v374 = g.A;
    const int64_t _v375 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0), true);
    const uint32_t _v376 = g.r[1];
    M.wr('x', _v376, _v372);
    g.pstep(1,1);
    g.y1 = _v372;
    }
    { // $07F6: mac      -y1,y0,a x:(r2)+,x0
    GEN_TRC(0x07F6);
    const uint32_t _v377 = M.rd('x', g.r[2]);
    gm_mac(g, g.A, -(int64_t)s24(g.y1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v377 & M24);
    g.pstep(2,1);
    }
    { // $07F7: mac      x1,x0,b b,x:(r3)+ b,y1
    GEN_TRC(0x07F7);
    const uint32_t _v378 = acc_to24(g.A);
    const uint32_t _v379 = acc_to24(g.B);
    const int64_t _v380 = g.A;
    const int64_t _v381 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.x0), true);
    const uint32_t _v382 = g.r[3];
    M.wr('x', _v382, _v379);
    g.pstep(3,1);
    g.y1 = _v379;
    }
    { // $07F8: mac      -y1,y0,b x:(r0)+,x0 y:(r4)+,y0
    GEN_TRC(0x07F8);
    const uint32_t _v383 = M.rd('x', g.r[0]);
    const uint32_t _v384 = M.rd('y', g.r[4]);
    gm_mac(g, g.B, -(int64_t)s24(g.y1), (int64_t)s24(g.y0), true);
    g.x0 = (uint32_t)(_v383 & M24);
    g.y0 = (uint32_t)(_v384 & M24);
    g.pstep(0,1);
    g.pstep(4,1);
    }
    { // $07F9: move     y:(r4)+,x1
    GEN_TRC(0x07F9);
    const uint32_t _v385 = M.rd('y', g.r[4]);
    g.x1 = (uint32_t)(_v385 & M24);
    g.pstep(4,1);
    }
  if (--_do07ef > 0) goto Ldo07ef_top; // конец do
    { // $07FA: move     a,x:(r1)+
    GEN_TRC(0x07FA);
    const uint32_t _v386 = g.r[1];
    M.wr('x', _v386, acc_to24(g.A));
    g.pstep(1,1);
    }
  { // $07FB: move     b,x:(r3)+
  GEN_TRC(0x07FB);
  const uint32_t _v387 = g.r[3];
  M.wr('x', _v387, acc_to24(g.B));
  g.pstep(3,1);
  }
  { // $07FC: move     a,l:(r7)+
  GEN_TRC(0x07FC);
  const uint32_t _v388 = g.r[7];
  M.wrL(_v388, (uint64_t)g.A & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $07FD: move     b,l:(r7)+
  GEN_TRC(0x07FD);
  const uint32_t _v389 = g.r[7];
  M.wrL(_v389, (uint64_t)g.B & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $07FE: move     #$4,r4
  GEN_TRC(0x07FE);
  g.r[4] = (uint32_t)(0x000004u);
  }
  { // $07FF: move     #$2,n4
  GEN_TRC(0x07FF);
  g.n[4] = (uint32_t)(0x000002u);
  }
  { // $0800: move     #$72,r0
  GEN_TRC(0x0800);
  g.r[0] = (uint32_t)(0x000072u);
  }
  { // $0801: move     r0,r1
  GEN_TRC(0x0801);
  g.r[1] = (uint32_t)(g.r[0]);
  }
  { // $0802: move     #$b2,r2
  GEN_TRC(0x0802);
  g.r[2] = (uint32_t)(0x0000B2u);
  }
  { // $0803: move     r2,r3
  GEN_TRC(0x0803);
  g.r[3] = (uint32_t)(g.r[2]);
  }
  { // $0804: move     l:(r7)+,a
  GEN_TRC(0x0804);
  const uint64_t _v390 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v390 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $0805: move     l:(r7)-,b
  GEN_TRC(0x0805);
  const uint64_t _v391 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)(_v391 & 0xFFFFFFFFFFll));
  g.pstep(7,-1);
  }
  { // $0806: jsr      func_00037c
  GEN_TRC(0x0806);
    func_00037c(M, rom, g);
  }
  { // $0807: move     a,l:(r7)+
  GEN_TRC(0x0807);
  const uint32_t _v392 = g.r[7];
  M.wrL(_v392, (uint64_t)g.A & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $0808: move     b,l:(r7)+
  GEN_TRC(0x0808);
  const uint32_t _v393 = g.r[7];
  M.wrL(_v393, (uint64_t)g.B & 0xFFFFFFFFFFll);
  g.pstep(7,1);
  }
  { // $0809: move     #>$eeb88,y0
  GEN_TRC(0x0809);
  g.y0 = (uint32_t)(0x0EEB88u);
  }
  { // $080B: move     #>$39c201,y1
  GEN_TRC(0x080B);
  g.y1 = (uint32_t)(0x39C201u);
  }
  { // $080D: lua      (r7-$30),r3
  GEN_TRC(0x080D);
    g.r[3] = (uint32_t)((int64_t)g.r[7] + -1 * 0x30ll) & M24;
  }
  { // $080E: move     r3,r4
  GEN_TRC(0x080E);
  g.r[4] = (uint32_t)(g.r[3]);
  }
  { // $080F: move     #$6d,r0
  GEN_TRC(0x080F);
  g.r[0] = (uint32_t)(0x00006Du);
  }
  { // $0810: move     #$6c,r2
  GEN_TRC(0x0810);
  g.r[2] = (uint32_t)(0x00006Cu);
  }
  { // $0811: move     l:(r3)+,x
  GEN_TRC(0x0811);
  const uint64_t _v394 = M.rdL(g.r[3]);
  g.x1 = (uint32_t)((_v394 >> 24) & M24); g.x0 = (uint32_t)(_v394 & M24);
  g.pstep(3,1);
  }
  { // $0812: move     x0,x:(r0)+
  GEN_TRC(0x0812);
  const uint32_t _v395 = g.r[0];
  M.wr('x', _v395, (uint32_t)(g.x0 & M24));
  g.pstep(0,1);
  }
  { // $0813: move     x1,x:(r0)+
  GEN_TRC(0x0813);
  const uint32_t _v396 = g.r[0];
  M.wr('x', _v396, (uint32_t)(g.x1 & M24));
  g.pstep(0,1);
  }
  { // $0814: move     l:(r3)+,x
  GEN_TRC(0x0814);
  const uint64_t _v397 = M.rdL(g.r[3]);
  g.x1 = (uint32_t)((_v397 >> 24) & M24); g.x0 = (uint32_t)(_v397 & M24);
  g.pstep(3,1);
  }
  { // $0815: move     x0,x:(r0)+
  GEN_TRC(0x0815);
  const uint32_t _v398 = g.r[0];
  M.wr('x', _v398, (uint32_t)(g.x0 & M24));
  g.pstep(0,1);
  }
  { // $0816: move     x1,x:(r0)+
  GEN_TRC(0x0816);
  const uint32_t _v399 = g.r[0];
  M.wr('x', _v399, (uint32_t)(g.x1 & M24));
  g.pstep(0,1);
  }
  { // $0817: move     x:(r3),x1
  GEN_TRC(0x0817);
  const uint32_t _v400 = M.rd('x', g.r[3]);
  g.x1 = (uint32_t)(_v400 & M24);
  }
  { // $0818: move     x1,x:(r0)+
  GEN_TRC(0x0818);
  const uint32_t _v401 = g.r[0];
  M.wr('x', _v401, (uint32_t)(g.x1 & M24));
  g.pstep(0,1);
  }
  { // $0819: move     #$6d,r0
  GEN_TRC(0x0819);
  g.r[0] = (uint32_t)(0x00006Du);
  }
  { // $081A: move     #>$f75277,x1
  GEN_TRC(0x081A);
  g.x1 = (uint32_t)(0xF75277u);
  }
  { // $081C: move     #>$fffffd,n0
  GEN_TRC(0x081C);
  g.n[0] = (uint32_t)(0xFFFFFDu);
  }
  { // $081E: move     x:(r0)+,x0
  GEN_TRC(0x081E);
  const uint32_t _v402 = M.rd('x', g.r[0]);
  g.x0 = (uint32_t)(_v402 & M24);
  g.pstep(0,1);
  }
  _do081f = 16; // $081F: do
  Ldo081f_top:;
    { // $0821: mpy      x1,x0,a x:(r0)+,x0
    GEN_TRC(0x0821);
    const uint32_t _v403 = M.rd('x', g.r[0]);
    gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
    g.x0 = (uint32_t)(_v403 & M24);
    g.pstep(0,1);
    }
    { // $0822: mac      y0,x0,a         x:(r0)+,x0
    GEN_TRC(0x0822);
    const uint32_t _v404 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x0 = (uint32_t)(_v404 & M24);
    g.pstep(0,1);
    }
    { // $0823: mac      x0,y1,a x:(r0)+,x0
    GEN_TRC(0x0823);
    const uint32_t _v405 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v405 & M24);
    g.pstep(0,1);
    }
    { // $0824: mac      x0,y1,a x:(r0)+,x0
    GEN_TRC(0x0824);
    const uint32_t _v406 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v406 & M24);
    g.pstep(0,1);
    }
    { // $0825: mac      y0,x0,a         x:(r0)+n0,x0
    GEN_TRC(0x0825);
    const uint32_t _v407 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x0 = (uint32_t)(_v407 & M24);
    g.pstep(0,g.n[0]);
    }
    { // $0826: mac      x1,x0,a b,x:(r2)+
    GEN_TRC(0x0826);
    const uint32_t _v408 = acc_to24(g.A);
    const uint32_t _v409 = acc_to24(g.B);
    const int64_t _v410 = g.A;
    const int64_t _v411 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0), true);
    const uint32_t _v412 = g.r[2];
    M.wr('x', _v412, _v409);
    g.pstep(2,1);
    }
    { // $0827: tfr      a,b x:(r0)+,x0
    GEN_TRC(0x0827);
    const uint32_t _v413 = M.rd('x', g.r[0]);
    g.B = g.A;
    g.x0 = (uint32_t)(_v413 & M24);
    g.pstep(0,1);
    }
  if (--_do081f > 0) goto Ldo081f_top; // конец do
    { // $0828: move     a,x:(r2)+
    GEN_TRC(0x0828);
    const uint32_t _v414 = g.r[2];
    M.wr('x', _v414, acc_to24(g.A));
    g.pstep(2,1);
    }
  { // $0829: tfr      x0,a r0,r1
  GEN_TRC(0x0829);
  g.A = (int64_t)s24(g.x0) << 24;
  g.r[1] = (uint32_t)(g.r[0]);
  }
  { // $082A: move     x:(r0)+,x1
  GEN_TRC(0x082A);
  const uint32_t _v415 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v415 & M24);
  g.pstep(0,1);
  }
  { // $082B: move     x,l:(r4)+
  GEN_TRC(0x082B);
  const uint32_t _v416 = g.r[4];
  M.wr('x', _v416, g.x1); M.wr('y', _v416, g.x0);
  g.pstep(4,1);
  }
  { // $082C: move     x:(r0)+,x0
  GEN_TRC(0x082C);
  const uint32_t _v417 = M.rd('x', g.r[0]);
  g.x0 = (uint32_t)(_v417 & M24);
  g.pstep(0,1);
  }
  { // $082D: move     x:(r0)+,x1
  GEN_TRC(0x082D);
  const uint32_t _v418 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v418 & M24);
  g.pstep(0,1);
  }
  { // $082E: move     x,l:(r4)+
  GEN_TRC(0x082E);
  const uint32_t _v419 = g.r[4];
  M.wr('x', _v419, g.x1); M.wr('y', _v419, g.x0);
  g.pstep(4,1);
  }
  { // $082F: move     x:(r0)+,x1
  GEN_TRC(0x082F);
  const uint32_t _v420 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v420 & M24);
  g.pstep(0,1);
  }
  { // $0830: move     x1,x:(r4)
  GEN_TRC(0x0830);
  const uint32_t _v421 = g.r[4];
  M.wr('x', _v421, (uint32_t)(g.x1 & M24));
  }
  { // $0831: move     a,x0
  GEN_TRC(0x0831);
  g.x0 = acc_to24(g.A);
  }
  { // $0832: move     r1,r0
  GEN_TRC(0x0832);
  g.r[0] = (uint32_t)(g.r[1]);
  }
  { // $0833: move     x:(r6+$f),x1
  GEN_TRC(0x0833);
  const uint32_t _v422 = M.rd('x', (g.r[6] + 0xFu) & M24);
  g.x1 = (uint32_t)(_v422 & M24);
  }
  { // $0834: jset     #$0,x1,func_00084e
  GEN_TRC(0x0834);
    if (((g.x1 >> 0) & 1)) goto L_00084e;
  }
  { // $0836: move     #$ad,r0
  GEN_TRC(0x0836);
  g.r[0] = (uint32_t)(0x0000ADu);
  }
  { // $0837: move     #$ac,r2
  GEN_TRC(0x0837);
  g.r[2] = (uint32_t)(0x0000ACu);
  }
  { // $0838: move     y:(r3)+,x0
  GEN_TRC(0x0838);
  const uint32_t _v423 = M.rd('y', g.r[3]);
  g.x0 = (uint32_t)(_v423 & M24);
  g.pstep(3,1);
  }
  { // $0839: move     x0,x:(r0)+
  GEN_TRC(0x0839);
  const uint32_t _v424 = g.r[0];
  M.wr('x', _v424, (uint32_t)(g.x0 & M24));
  g.pstep(0,1);
  }
  { // $083A: move     l:(r3)+,x
  GEN_TRC(0x083A);
  const uint64_t _v425 = M.rdL(g.r[3]);
  g.x1 = (uint32_t)((_v425 >> 24) & M24); g.x0 = (uint32_t)(_v425 & M24);
  g.pstep(3,1);
  }
  { // $083B: move     x0,x:(r0)+
  GEN_TRC(0x083B);
  const uint32_t _v426 = g.r[0];
  M.wr('x', _v426, (uint32_t)(g.x0 & M24));
  g.pstep(0,1);
  }
  { // $083C: move     x1,x:(r0)+
  GEN_TRC(0x083C);
  const uint32_t _v427 = g.r[0];
  M.wr('x', _v427, (uint32_t)(g.x1 & M24));
  g.pstep(0,1);
  }
  { // $083D: move     l:(r3)+,x
  GEN_TRC(0x083D);
  const uint64_t _v428 = M.rdL(g.r[3]);
  g.x1 = (uint32_t)((_v428 >> 24) & M24); g.x0 = (uint32_t)(_v428 & M24);
  g.pstep(3,1);
  }
  { // $083E: move     x0,x:(r0)+
  GEN_TRC(0x083E);
  const uint32_t _v429 = g.r[0];
  M.wr('x', _v429, (uint32_t)(g.x0 & M24));
  g.pstep(0,1);
  }
  { // $083F: move     x1,x:(r0)+
  GEN_TRC(0x083F);
  const uint32_t _v430 = g.r[0];
  M.wr('x', _v430, (uint32_t)(g.x1 & M24));
  g.pstep(0,1);
  }
  { // $0840: move     #$ad,r0
  GEN_TRC(0x0840);
  g.r[0] = (uint32_t)(0x0000ADu);
  }
  { // $0841: move     #>$f75277,x1
  GEN_TRC(0x0841);
  g.x1 = (uint32_t)(0xF75277u);
  }
  { // $0843: move     x:(r0)+,x0
  GEN_TRC(0x0843);
  const uint32_t _v431 = M.rd('x', g.r[0]);
  g.x0 = (uint32_t)(_v431 & M24);
  g.pstep(0,1);
  }
  _do0844 = 16; // $0844: do
  Ldo0844_top:;
    { // $0846: mpy      x1,x0,a x:(r0)+,x0
    GEN_TRC(0x0846);
    const uint32_t _v432 = M.rd('x', g.r[0]);
    gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
    g.x0 = (uint32_t)(_v432 & M24);
    g.pstep(0,1);
    }
    { // $0847: mac      y0,x0,a         x:(r0)+,x0
    GEN_TRC(0x0847);
    const uint32_t _v433 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x0 = (uint32_t)(_v433 & M24);
    g.pstep(0,1);
    }
    { // $0848: mac      x0,y1,a x:(r0)+,x0
    GEN_TRC(0x0848);
    const uint32_t _v434 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v434 & M24);
    g.pstep(0,1);
    }
    { // $0849: mac      x0,y1,a x:(r0)+,x0
    GEN_TRC(0x0849);
    const uint32_t _v435 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v435 & M24);
    g.pstep(0,1);
    }
    { // $084A: mac      y0,x0,a         x:(r0)+n0,x0
    GEN_TRC(0x084A);
    const uint32_t _v436 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x0 = (uint32_t)(_v436 & M24);
    g.pstep(0,g.n[0]);
    }
    { // $084B: mac      x1,x0,a b,x:(r2)+
    GEN_TRC(0x084B);
    const uint32_t _v437 = acc_to24(g.A);
    const uint32_t _v438 = acc_to24(g.B);
    const int64_t _v439 = g.A;
    const int64_t _v440 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0), true);
    const uint32_t _v441 = g.r[2];
    M.wr('x', _v441, _v438);
    g.pstep(2,1);
    }
    { // $084C: tfr      a,b x:(r0)+,x0
    GEN_TRC(0x084C);
    const uint32_t _v442 = M.rd('x', g.r[0]);
    g.B = g.A;
    g.x0 = (uint32_t)(_v442 & M24);
    g.pstep(0,1);
    }
  if (--_do0844 > 0) goto Ldo0844_top; // конец do
    { // $084D: move     a,x:(r2)+
    GEN_TRC(0x084D);
    const uint32_t _v443 = g.r[2];
    M.wr('x', _v443, acc_to24(g.A));
    g.pstep(2,1);
    }
L_00084e:;
  { // $084E: move     x0,y:(r4)+
  GEN_TRC(0x084E);
  const uint32_t _v444 = g.r[4];
  M.wr('y', _v444, (uint32_t)(g.x0 & M24));
  g.pstep(4,1);
  }
  { // $084F: move     x:(r0)+,x0
  GEN_TRC(0x084F);
  const uint32_t _v445 = M.rd('x', g.r[0]);
  g.x0 = (uint32_t)(_v445 & M24);
  g.pstep(0,1);
  }
  { // $0850: move     x:(r0)+,x1
  GEN_TRC(0x0850);
  const uint32_t _v446 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v446 & M24);
  g.pstep(0,1);
  }
  { // $0851: move     x,l:(r4)+
  GEN_TRC(0x0851);
  const uint32_t _v447 = g.r[4];
  M.wr('x', _v447, g.x1); M.wr('y', _v447, g.x0);
  g.pstep(4,1);
  }
  { // $0852: move     x:(r0)+,x0
  GEN_TRC(0x0852);
  const uint32_t _v448 = M.rd('x', g.r[0]);
  g.x0 = (uint32_t)(_v448 & M24);
  g.pstep(0,1);
  }
  { // $0853: move     x:(r0)+,x1
  GEN_TRC(0x0853);
  const uint32_t _v449 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v449 & M24);
  g.pstep(0,1);
  }
  { // $0854: move     x,l:(r4)+
  GEN_TRC(0x0854);
  const uint32_t _v450 = g.r[4];
  M.wr('x', _v450, g.x1); M.wr('y', _v450, g.x0);
  g.pstep(4,1);
  }
  { // $0855: move     #>$63f,x0
  GEN_TRC(0x0855);
  g.x0 = (uint32_t)(0x00063Fu);
  }
  { // $0857: move     x:(r7-$1a),a
  GEN_TRC(0x0857);
  const uint32_t _v451 = M.rd('x', (g.r[7] - 0x1Au) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v451))) << 24);
  }
  { // $0858: tst      a
  GEN_TRC(0x0858);
  gm_test56(g, g.A);
  }
  CHK(0x0859);
  { // $0859: clr      a               ifmi
  GEN_TRC(0x0859);
  if (g.fn) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $085A: cmp      x0,a
  GEN_TRC(0x085A);
  gm_cmp(g, g.A, (int64_t)s24(g.x0) << 24);
  }
  { // $085B: tfr      x0,a            ifgt
  GEN_TRC(0x085B);
  if (((g.fn==g.fv)&&!g.fz)) g.A = (int64_t)s24(g.x0) << 24;
  }
  { // $085C: move     a,r0
  GEN_TRC(0x085C);
  g.r[0] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $085D: move     #>$7fffff,a
  GEN_TRC(0x085D);
  g.A = sext56(((int64_t)s24((uint32_t)(0x7FFFFFu))) << 24);
  }
  { // $085F: move     l:(r7),x
  GEN_TRC(0x085F);
  const uint64_t _v452 = M.rdL(g.r[7]);
  g.x1 = (uint32_t)((_v452 >> 24) & M24); g.x0 = (uint32_t)(_v452 & M24);
  }
  { // $0860: move     x:(r0+$1435c6),b
  GEN_TRC(0x0860);
  const uint32_t _v453 = rom.rd('x', (g.r[0] + 0x1435C6u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v453))) << 24);
  }
  { // $0862: sub      b,a a,y0
  GEN_TRC(0x0862);
  const uint32_t _v454 = acc_to24(g.A);
  const uint32_t _v455 = acc_to24(g.B);
  const int64_t _v456 = g.A;
  const int64_t _v457 = g.B;
  g.A = acc_sub56(g, g.A, g.B);
  g.y0 = _v454;
  }
  { // $0863: add      y0,a #$6c,r0
  GEN_TRC(0x0863);
  g.A = acc_add56(g, g.A, (int64_t)s24(g.y0) << 24);
  g.r[0] = (uint32_t)(0x00006Cu);
  }
  { // $0864: asr      a #$4,r4
  GEN_TRC(0x0864);
  const int64_t _v458 = g.A;
  g.fc = (int)((_v458 >> 0) & 1);
  g.A = sext56(_v458 >> 1);
  gm_flags56(g, g.A);
  g.r[4] = (uint32_t)(0x000004u);
  }
  { // $0865: sub      x0,a ba,l:(r7)+
  GEN_TRC(0x0865);
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.x0) << 24);
  const uint32_t _v459 = g.r[7];
  M.wr('x', _v459, (uint32_t)(g.B >> 24)); M.wr('y', _v459, (uint32_t)(g.A >> 24));
  g.pstep(7,1);
  }
  { // $0866: sub      x1,b a,y0
  GEN_TRC(0x0866);
  const uint32_t _v460 = acc_to24(g.A);
  const uint32_t _v461 = acc_to24(g.B);
  const int64_t _v462 = g.A;
  const int64_t _v463 = g.B;
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.x1) << 24);
  g.y0 = _v460;
  }
  { // $0867: tfr      x0,a #$8,x0
  GEN_TRC(0x0867);
  g.A = (int64_t)s24(g.x0) << 24;
  g.x0 = (uint32_t)(0x080000u);
  }
  { // $0868: tfr      x1,b            b,y1
  GEN_TRC(0x0868);
  const uint32_t _v464 = acc_to24(g.A);
  const uint32_t _v465 = acc_to24(g.B);
  const int64_t _v466 = g.A;
  const int64_t _v467 = g.B;
  g.B = (int64_t)s24(g.x1) << 24;
  g.y1 = _v465;
  }
  _do0869 = 16; // $0869: do
  Ldo0869_top:;
    { // $086B: mac      y0,x0,a a,y:(r4)
    GEN_TRC(0x086B);
    const uint32_t _v468 = acc_to24(g.A);
    const uint32_t _v469 = acc_to24(g.B);
    const int64_t _v470 = g.A;
    const int64_t _v471 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v472 = g.r[4];
    M.wr('y', _v472, _v468);
    }
    { // $086C: mac      x0,y1,b b,x:(r4)+
    GEN_TRC(0x086C);
    const uint32_t _v473 = acc_to24(g.A);
    const uint32_t _v474 = acc_to24(g.B);
    const int64_t _v475 = g.A;
    const int64_t _v476 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    const uint32_t _v477 = g.r[4];
    M.wr('x', _v477, _v474);
    g.pstep(4,1);
    }
  if (--_do0869 > 0) goto Ldo0869_top; // конец do
    { // $086D: move     #$4,r4
    GEN_TRC(0x086D);
    g.r[4] = (uint32_t)(0x000004u);
    }
  { // $086E: move     r0,r1
  GEN_TRC(0x086E);
  g.r[1] = (uint32_t)(g.r[0]);
  }
  { // $086F: move     #$ac,r2
  GEN_TRC(0x086F);
  g.r[2] = (uint32_t)(0x0000ACu);
  }
  { // $0870: move     r2,r3
  GEN_TRC(0x0870);
  g.r[3] = (uint32_t)(g.r[2]);
  }
  { // $0871: move     l:(r7)+,a
  GEN_TRC(0x0871);
  const uint64_t _v478 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v478 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $0872: move     l:(r7)+,b
  GEN_TRC(0x0872);
  const uint64_t _v479 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)(_v479 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $0873: move     l:(r7),x
  GEN_TRC(0x0873);
  const uint64_t _v480 = M.rdL(g.r[7]);
  g.x1 = (uint32_t)((_v480 >> 24) & M24); g.x0 = (uint32_t)(_v480 & M24);
  }
  { // $0874: move     x0,x:(r0)
  GEN_TRC(0x0874);
  const uint32_t _v481 = g.r[0];
  M.wr('x', _v481, (uint32_t)(g.x0 & M24));
  }
  { // $0875: move     x1,x:(r2)
  GEN_TRC(0x0875);
  const uint32_t _v482 = g.r[2];
  M.wr('x', _v482, (uint32_t)(g.x1 & M24));
  }
  { // $0876: move     x:(r0+$10),x0
  GEN_TRC(0x0876);
  const uint32_t _v483 = M.rd('x', (g.r[0] + 0x10u) & M24);
  g.x0 = (uint32_t)(_v483 & M24);
  }
  { // $0877: move     x:(r2+$10),x1
  GEN_TRC(0x0877);
  const uint32_t _v484 = M.rd('x', (g.r[2] + 0x10u) & M24);
  g.x1 = (uint32_t)(_v484 & M24);
  }
  { // $0878: move     x,l:(r7)-
  GEN_TRC(0x0878);
  const uint32_t _v485 = g.r[7];
  M.wr('x', _v485, g.x1); M.wr('y', _v485, g.x0);
  g.pstep(7,-1);
  }
  { // $0879: jsr      func_00038a
  GEN_TRC(0x0879);
    func_00038a(M, rom, g);
  }
  { // $087A: move     b,l:(r7)-
  GEN_TRC(0x087A);
  const uint32_t _v486 = g.r[7];
  M.wrL(_v486, (uint64_t)g.B & 0xFFFFFFFFFFll);
  g.pstep(7,-1);
  }
  { // $087B: move     a,l:(r7)
  GEN_TRC(0x087B);
  const uint32_t _v487 = g.r[7];
  M.wrL(_v487, (uint64_t)g.A & 0xFFFFFFFFFFll);
  }
  { // $087C: lua      (r7+$3),r7
  GEN_TRC(0x087C);
    g.r[7] = (uint32_t)((int64_t)g.r[7] + 1 * 0x3ll) & M24;
  }
  { // $087D: move     #$4,r4
  GEN_TRC(0x087D);
  g.r[4] = (uint32_t)(0x000004u);
  }
  { // $087E: move     #$6c,r0
  GEN_TRC(0x087E);
  g.r[0] = (uint32_t)(0x00006Cu);
  }
  { // $087F: move     r0,r1
  GEN_TRC(0x087F);
  g.r[1] = (uint32_t)(g.r[0]);
  }
  { // $0880: move     #$ac,r2
  GEN_TRC(0x0880);
  g.r[2] = (uint32_t)(0x0000ACu);
  }
  { // $0881: move     r2,r3
  GEN_TRC(0x0881);
  g.r[3] = (uint32_t)(g.r[2]);
  }
  { // $0882: move     l:(r7)+,a
  GEN_TRC(0x0882);
  const uint64_t _v488 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v488 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $0883: move     l:(r7)+,b
  GEN_TRC(0x0883);
  const uint64_t _v489 = M.rdL(g.r[7]);
  g.B = sext56((int64_t)(_v489 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $0884: move     l:(r7),x
  GEN_TRC(0x0884);
  const uint64_t _v490 = M.rdL(g.r[7]);
  g.x1 = (uint32_t)((_v490 >> 24) & M24); g.x0 = (uint32_t)(_v490 & M24);
  }
  { // $0885: move     x0,x:(r0)
  GEN_TRC(0x0885);
  const uint32_t _v491 = g.r[0];
  M.wr('x', _v491, (uint32_t)(g.x0 & M24));
  }
  { // $0886: move     x1,x:(r2)
  GEN_TRC(0x0886);
  const uint32_t _v492 = g.r[2];
  M.wr('x', _v492, (uint32_t)(g.x1 & M24));
  }
  { // $0887: move     x:(r0+$10),x0
  GEN_TRC(0x0887);
  const uint32_t _v493 = M.rd('x', (g.r[0] + 0x10u) & M24);
  g.x0 = (uint32_t)(_v493 & M24);
  }
  { // $0888: move     x:(r2+$10),x1
  GEN_TRC(0x0888);
  const uint32_t _v494 = M.rd('x', (g.r[2] + 0x10u) & M24);
  g.x1 = (uint32_t)(_v494 & M24);
  }
  { // $0889: move     x,l:(r7)-
  GEN_TRC(0x0889);
  const uint32_t _v495 = g.r[7];
  M.wr('x', _v495, g.x1); M.wr('y', _v495, g.x0);
  g.pstep(7,-1);
  }
  { // $088A: jsr      func_00038a
  GEN_TRC(0x088A);
    func_00038a(M, rom, g);
  }
  { // $088B: move     b,l:(r7)-
  GEN_TRC(0x088B);
  const uint32_t _v496 = g.r[7];
  M.wrL(_v496, (uint64_t)g.B & 0xFFFFFFFFFFll);
  g.pstep(7,-1);
  }
  { // $088C: move     a,l:(r7)
  GEN_TRC(0x088C);
  const uint32_t _v497 = g.r[7];
  M.wrL(_v497, (uint64_t)g.A & 0xFFFFFFFFFFll);
  }
  { // $088D: lua      (r7+$3),r7
  GEN_TRC(0x088D);
    g.r[7] = (uint32_t)((int64_t)g.r[7] + 1 * 0x3ll) & M24;
  }
  CHK(0x088E);
  { // $088E: move     y:(r6+$28),a
  GEN_TRC(0x088E);
  const uint32_t _v498 = M.rd('y', (g.r[6] + 0x28u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v498))) << 24);
  }
  { // $088F: move     y:(r7-$23),b
  GEN_TRC(0x088F);
  const uint32_t _v499 = M.rd('y', (g.r[7] - 0x23u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v499))) << 24);
  }
  { // $0890: cmp      #<$1,a
  GEN_TRC(0x0890);
  gm_cmp(g, g.A, (int64_t)s24(0x000001u) << 24);
  }
  { // $0891: bne      func_000894
  GEN_TRC(0x0891);
    if (!g.fz) goto L_000894;
  }
  { // $0892: move     #$0,x0
  GEN_TRC(0x0892);
  g.x0 = (uint32_t)(0x000000u);
  }
  { // $0893: move     x0,x:(r7-$22)
  GEN_TRC(0x0893);
  const uint32_t _v500 = (g.r[7] - 0x22u) & M24;
  M.wr('x', _v500, (uint32_t)(g.x0 & M24));
  }
L_000894:;
  { // $0894: cmp      #<$2,a
  GEN_TRC(0x0894);
  gm_cmp(g, g.A, (int64_t)s24(0x000002u) << 24);
  }
  { // $0895: blt      func_00089d
  GEN_TRC(0x0895);
    if ((g.fn!=g.fv)) goto L_00089d;
  }
  { // $0896: move     #>$3,x0
  GEN_TRC(0x0896);
  g.x0 = (uint32_t)(0x000003u);
  }
  { // $0898: cmp      #<$3,a
  GEN_TRC(0x0898);
  gm_cmp(g, g.A, (int64_t)s24(0x000003u) << 24);
  }
  { // $0899: bne      func_00089c
  GEN_TRC(0x0899);
    if (!g.fz) goto L_00089c;
  }
  { // $089A: move     #>$4,x0
  GEN_TRC(0x089A);
  g.x0 = (uint32_t)(0x000004u);
  }
L_00089c:;
  { // $089C: move     x0,x:(r7-$22)
  GEN_TRC(0x089C);
  const uint32_t _v501 = (g.r[7] - 0x22u) & M24;
  M.wr('x', _v501, (uint32_t)(g.x0 & M24));
  }
L_00089d:;
  { // $089D: move     x:(r7-$22),a
  GEN_TRC(0x089D);
  const uint32_t _v502 = M.rd('x', (g.r[7] - 0x22u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v502))) << 24);
  }
  { // $089E: tst      a #$40,r0
  GEN_TRC(0x089E);
  gm_test56(g, g.A);
  g.r[0] = (uint32_t)(0x000040u);
  }
  { // $089F: bne      func_0008ae
  GEN_TRC(0x089F);
    if (!g.fz) goto L_0008ae;
  }
  { // $08A0: move     y:(r6+$0),a
  GEN_TRC(0x08A0);
  const uint32_t _v503 = M.rd('y', (g.r[6] + 0x0u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v503))) << 24);
  }
  { // $08A1: asr      #$10,a,a
  GEN_TRC(0x08A1);
  const int64_t _v504 = g.A;
  g.fc = (int)((_v504 >> 15) & 1);
  g.A = sext56(_v504 >> 16);
  gm_flags56(g, g.A);
  }
  { // $08A2: move     a,r4
  GEN_TRC(0x08A2);
  g.r[4] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $08A3: move     y:(r7-$23),a
  GEN_TRC(0x08A3);
  const uint32_t _v505 = M.rd('y', (g.r[7] - 0x23u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v505))) << 24);
  }
  { // $08A4: move     y:(r4+$141800),y0
  GEN_TRC(0x08A4);
  const uint32_t _v506 = rom.rd('y', (g.r[4] + 0x141800u) & M24);
  g.y0 = (uint32_t)(_v506 & M24);
  }
  { // $08A6: add      y0,a
  GEN_TRC(0x08A6);
  g.A = acc_add56(g, g.A, (int64_t)s24(g.y0) << 24);
  }
  { // $08A7: move     a,y:(r7-$23)
  GEN_TRC(0x08A7);
  const uint32_t _v507 = (g.r[7] - 0x23u) & M24;
  M.wr('y', _v507, acc_to24(g.A));
  }
  { // $08A8: bec      func_0008d8
  GEN_TRC(0x08A8);
    if (!g.fe) goto L_0008d8;
  }
  { // $08A9: move     #>$1,x0
  GEN_TRC(0x08A9);
  g.x0 = (uint32_t)(0x000001u);
  }
  { // $08AB: move     x0,x:(r7-$22)
  GEN_TRC(0x08AB);
  const uint32_t _v508 = (g.r[7] - 0x22u) & M24;
  M.wr('x', _v508, (uint32_t)(g.x0 & M24));
  }
  { // $08AC: move     x0,y:(r7-$22)
  GEN_TRC(0x08AC);
  const uint32_t _v509 = (g.r[7] - 0x22u) & M24;
  M.wr('y', _v509, (uint32_t)(g.x0 & M24));
  }
  { // $08AD: bra      func_0008d8
  GEN_TRC(0x08AD);
    if (true) goto L_0008d8;
  }
L_0008ae:;
  { // $08AE: cmp      #<$1,a
  GEN_TRC(0x08AE);
  gm_cmp(g, g.A, (int64_t)s24(0x000001u) << 24);
  }
  { // $08AF: bne      func_0008c6
  GEN_TRC(0x08AF);
    if (!g.fz) goto L_0008c6;
  }
  { // $08B0: move     y:(r6+$1),a
  GEN_TRC(0x08B0);
  const uint32_t _v510 = M.rd('y', (g.r[6] + 0x1u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v510))) << 24);
  }
  { // $08B1: cmp      #>$10000,a
  GEN_TRC(0x08B1);
  gm_cmp(g, g.A, (int64_t)s24(0x010000u) << 24);
  }
  { // $08B3: clr      a               iflt
  GEN_TRC(0x08B3);
  if ((g.fn!=g.fv)) {
    g.A = 0;
    gm_flags56(g, g.A);
  }
  }
  { // $08B4: move     y:(r6+$23),y0
  GEN_TRC(0x08B4);
  const uint32_t _v511 = M.rd('y', (g.r[6] + 0x23u) & M24);
  g.y0 = (uint32_t)(_v511 & M24);
  }
  { // $08B5: move     a,x0
  GEN_TRC(0x08B5);
  g.x0 = acc_to24(g.A);
  }
  { // $08B6: mpy      y0,x0,b
  GEN_TRC(0x08B6);
  gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  }
  { // $08B7: move     y:(r7-$22),a
  GEN_TRC(0x08B7);
  const uint32_t _v512 = M.rd('y', (g.r[7] - 0x22u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v512))) << 24);
  }
  { // $08B8: move     b,x1
  GEN_TRC(0x08B8);
  g.x1 = acc_to24(g.B);
  }
  { // $08B9: mpyi     #>$791fd0,x1,b
  GEN_TRC(0x08B9);
  gm_mpy(g, g.B, (int64_t)s24(0x791FD0u), (int64_t)s24(g.x1));
  }
  { // $08BB: asl      b
  GEN_TRC(0x08BB);
  const int64_t _v513 = g.B;
  g.fc = (int)((_v513 >> 55) & 1);
  g.B = sext56(_v513 << 1);
  gm_flags56(g, g.B);
  }
  { // $08BC: add      #<$1,a
  GEN_TRC(0x08BC);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x000001u) << 24);
  }
  { // $08BD: cmp      b,a
  GEN_TRC(0x08BD);
  gm_cmp(g, g.A, g.B);
  }
  { // $08BE: bgt      func_0008c1
  GEN_TRC(0x08BE);
    if (((g.fn==g.fv)&&!g.fz)) goto L_0008c1;
  }
  { // $08BF: move     a,y:(r7-$22)
  GEN_TRC(0x08BF);
  const uint32_t _v514 = (g.r[7] - 0x22u) & M24;
  M.wr('y', _v514, acc_to24(g.A));
  }
  { // $08C0: bra      func_0008d8
  GEN_TRC(0x08C0);
    if (true) goto L_0008d8;
  }
L_0008c1:;
  { // $08C1: move     #>$2,x0
  GEN_TRC(0x08C1);
  g.x0 = (uint32_t)(0x000002u);
  }
  { // $08C3: move     x0,x:(r7-$22)
  GEN_TRC(0x08C3);
  const uint32_t _v515 = (g.r[7] - 0x22u) & M24;
  M.wr('x', _v515, (uint32_t)(g.x0 & M24));
  }
  { // $08C4: move     x0,a
  GEN_TRC(0x08C4);
  g.A = sext56(((int64_t)s24((uint32_t)(g.x0))) << 24);
  }
  { // $08C5: bra      func_0008d8
  GEN_TRC(0x08C5);
    if (true) goto L_0008d8;
  }
L_0008c6:;
  { // $08C6: cmp      #<$2,a
  GEN_TRC(0x08C6);
  gm_cmp(g, g.A, (int64_t)s24(0x000002u) << 24);
  }
  { // $08C7: bne      func_0008ca
  GEN_TRC(0x08C7);
    if (!g.fz) goto L_0008ca;
  }
  { // $08C8: move     y:(r6+$2),b
  GEN_TRC(0x08C8);
  const uint32_t _v516 = M.rd('y', (g.r[6] + 0x2u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v516))) << 24);
  }
  { // $08C9: bra      func_0008ce
  GEN_TRC(0x08C9);
    if (true) goto L_0008ce;
  }
L_0008ca:;
  { // $08CA: move     y:(r6+$3),b
  GEN_TRC(0x08CA);
  const uint32_t _v517 = M.rd('y', (g.r[6] + 0x3u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v517))) << 24);
  }
  { // $08CB: cmp      #<$4,a
  GEN_TRC(0x08CB);
  gm_cmp(g, g.A, (int64_t)s24(0x000004u) << 24);
  }
  { // $08CC: bne      func_0008ce
  GEN_TRC(0x08CC);
    if (!g.fz) goto L_0008ce;
  }
  { // $08CD: move     #$20,b
  GEN_TRC(0x08CD);
  g.B = sext56(((int64_t)s24((uint32_t)(0x000020u))) << 24);
  }
L_0008ce:;
  { // $08CE: add      #>$7fff,b
  GEN_TRC(0x08CE);
  g.B = acc_add56(g, g.B, (int64_t)s24(0x007FFFu) << 24);
  }
  { // $08D0: asr      #$10,b,b
  GEN_TRC(0x08D0);
  const int64_t _v518 = g.B;
  g.fc = (int)((_v518 >> 15) & 1);
  g.B = sext56(_v518 >> 16);
  gm_flags56(g, g.B);
  }
  { // $08D1: rnd      b
  GEN_TRC(0x08D1);
  mnmfix::rnd_acc(g.B); gm_flags56(g, g.B);
  }
  { // $08D2: move     b,r4
  GEN_TRC(0x08D2);
  g.r[4] = (int32_t)s24(acc_to24n(g.B));
  }
  { // $08D3: move     y:(r7-$23),x1
  GEN_TRC(0x08D3);
  const uint32_t _v519 = M.rd('y', (g.r[7] - 0x23u) & M24);
  g.x1 = (uint32_t)(_v519 & M24);
  }
  { // $08D4: move     y:(r4+$141880),y0
  GEN_TRC(0x08D4);
  const uint32_t _v520 = rom.rd('y', (g.r[4] + 0x141880u) & M24);
  g.y0 = (uint32_t)(_v520 & M24);
  }
  { // $08D6: mpy      -x1,y0,a
  GEN_TRC(0x08D6);
  gm_mpy(g, g.A, -(int64_t)s24(g.x1), (int64_t)s24(g.y0));
  }
  { // $08D7: move     a,y:(r7-$23)
  GEN_TRC(0x08D7);
  const uint32_t _v521 = (g.r[7] - 0x23u) & M24;
  M.wr('y', _v521, acc_to24(g.A));
  }
  CHK(0x08D8);
L_0008d8:;
  { // $08D8: move     y:(r6+$6),x0
  GEN_TRC(0x08D8);
  const uint32_t _v522 = M.rd('y', (g.r[6] + 0x6u) & M24);
  g.x0 = (uint32_t)(_v522 & M24);
  }
  { // $08D9: move     #>$408e05,y0
  GEN_TRC(0x08D9);
  g.y0 = (uint32_t)(0x408E05u);
  }
  { // $08DB: mpy      y0,x0,b #$11,r3
  GEN_TRC(0x08DB);
  gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  g.r[3] = (uint32_t)(0x000011u);
  }
  { // $08DC: asl      b #$10,n0
  GEN_TRC(0x08DC);
  const int64_t _v523 = g.B;
  g.fc = (int)((_v523 >> 55) & 1);
  g.B = sext56(_v523 << 1);
  gm_flags56(g, g.B);
  g.n[0] = (uint32_t)(0x000010u);
  }
  { // $08DD: move     b,a
  GEN_TRC(0x08DD);
  g.A = g.B;
  }
  { // $08DE: asr      #$c,a,a
  GEN_TRC(0x08DE);
  const int64_t _v524 = g.A;
  g.fc = (int)((_v524 >> 11) & 1);
  g.A = sext56(_v524 >> 12);
  gm_flags56(g, g.A);
  }
  { // $08DF: move     y:(r6+$5),x0
  GEN_TRC(0x08DF);
  const uint32_t _v525 = M.rd('y', (g.r[6] + 0x5u) & M24);
  g.x0 = (uint32_t)(_v525 & M24);
  }
  { // $08E0: mpy      x0,x0,a a,r2
  GEN_TRC(0x08E0);
  const uint32_t _v526 = acc_to24(g.A);
  const uint32_t _v527 = acc_to24(g.B);
  const int64_t _v528 = g.A;
  const int64_t _v529 = g.B;
  gm_mpy(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.x0));
  g.r[2] = (uint32_t)(_v526);
  }
  { // $08E1: move     a,x0
  GEN_TRC(0x08E1);
  g.x0 = acc_to24(g.A);
  }
  { // $08E2: move     y:(r7-$23),x1
  GEN_TRC(0x08E2);
  const uint32_t _v530 = M.rd('y', (g.r[7] - 0x23u) & M24);
  g.x1 = (uint32_t)(_v530 & M24);
  }
  { // $08E3: mpy      x1,x0,a
  GEN_TRC(0x08E3);
  gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
  }
  { // $08E4: mpy      x1,x0,a
  GEN_TRC(0x08E4);
  gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
  }
  { // $08E5: move     x:(r2+$14a000),x1
  GEN_TRC(0x08E5);
  const uint32_t _v531 = rom.rd('x', (g.r[2] + 0x14A000u) & M24);
  g.x1 = (uint32_t)(_v531 & M24);
  }
  { // $08E7: move     x1,y:$0
  GEN_TRC(0x08E7);
  const uint32_t _v532 = 0x000000u;
  M.wr('y', _v532, (uint32_t)(g.x1 & M24));
  }
  { // $08E8: move     a,x0
  GEN_TRC(0x08E8);
  g.x0 = acc_to24(g.A);
  }
  { // $08E9: move     n0,n1
  GEN_TRC(0x08E9);
  g.n[1] = (uint32_t)(g.n[0]);
  }
  { // $08EA: move     #$6d,r1
  GEN_TRC(0x08EA);
  g.r[1] = (uint32_t)(0x00006Du);
  }
  { // $08EB: move     x:(r2+$14a801),y1
  GEN_TRC(0x08EB);
  const uint32_t _v533 = rom.rd('x', (g.r[2] + 0x14A801u) & M24);
  g.y1 = (uint32_t)(_v533 & M24);
  }
  { // $08ED: move     y1,y:$1
  GEN_TRC(0x08ED);
  const uint32_t _v534 = 0x000001u;
  M.wr('y', _v534, (uint32_t)(g.y1 & M24));
  }
  { // $08EE: mpy      x1,x0,b #$20,r5
  GEN_TRC(0x08EE);
  gm_mpy(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
  g.r[5] = (uint32_t)(0x000020u);
  }
  { // $08EF: mpy      x0,y1,a l:(r7),y
  GEN_TRC(0x08EF);
  const uint64_t _v535 = M.rdL(g.r[7]);
  gm_mpy(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1));
  g.y1 = (uint32_t)((_v535 >> 24) & M24); g.y0 = (uint32_t)(_v535 & M24);
  }
  { // $08F0: move     ab,l:(r7)+
  GEN_TRC(0x08F0);
  const uint32_t _v536 = g.r[7];
  M.wr('x', _v536, (uint32_t)(g.B >> 24)); M.wr('y', _v536, (uint32_t)(g.A >> 24));
  g.pstep(7,1);
  }
  { // $08F1: sub      y0,b #$ad,r0
  GEN_TRC(0x08F1);
  g.B = acc_sub56(g, g.B, (int64_t)s24(g.y0) << 24);
  g.r[0] = (uint32_t)(0x0000ADu);
  }
  { // $08F2: sub      y1,a r5,r4
  GEN_TRC(0x08F2);
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.y1) << 24);
  g.r[4] = (uint32_t)(g.r[5]);
  }
  { // $08F3: tfr      y0,a a,x1
  GEN_TRC(0x08F3);
  const uint32_t _v537 = acc_to24(g.A);
  const uint32_t _v538 = acc_to24(g.B);
  const int64_t _v539 = g.A;
  const int64_t _v540 = g.B;
  g.A = (int64_t)s24(g.y0) << 24;
  g.x1 = _v537;
  }
  { // $08F4: tfr      y1,b b,x0
  GEN_TRC(0x08F4);
  const uint32_t _v541 = acc_to24(g.A);
  const uint32_t _v542 = acc_to24(g.B);
  const int64_t _v543 = g.A;
  const int64_t _v544 = g.B;
  g.B = (int64_t)s24(g.y1) << 24;
  g.x0 = _v542;
  }
  { // $08F5: move     #$8,y0
  GEN_TRC(0x08F5);
  g.y0 = (uint32_t)(0x080000u);
  }
  _do08f6 = 16; // $08F6: do
  Ldo08f6_top:;
    { // $08F8: mac      y0,x0,a a,y:(r5)+
    GEN_TRC(0x08F8);
    const uint32_t _v545 = acc_to24(g.A);
    const uint32_t _v546 = acc_to24(g.B);
    const int64_t _v547 = g.A;
    const int64_t _v548 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v549 = g.r[5];
    M.wr('y', _v549, _v545);
    g.pstep(5,1);
    }
    { // $08F9: mac      x1,y0,b b,y:(r5)+
    GEN_TRC(0x08F9);
    const uint32_t _v550 = acc_to24(g.A);
    const uint32_t _v551 = acc_to24(g.B);
    const int64_t _v552 = g.A;
    const int64_t _v553 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    const uint32_t _v554 = g.r[5];
    M.wr('y', _v554, _v551);
    g.pstep(5,1);
    }
  if (--_do08f6 > 0) goto Ldo08f6_top; // конец do
    { // $08FA: move     y:(r6+$12),x0
    GEN_TRC(0x08FA);
    const uint32_t _v555 = M.rd('y', (g.r[6] + 0x12u) & M24);
    g.x0 = (uint32_t)(_v555 & M24);
    }
  { // $08FB: mpyri    #>$80,x0,a
  GEN_TRC(0x08FB);
  gm_mpyr(g, g.A, (int64_t)s24(0x000080u), (int64_t)s24(g.x0));
  }
  { // $08FD: move     x:(r6+$f),x0
  GEN_TRC(0x08FD);
  const uint32_t _v556 = M.rd('x', (g.r[6] + 0xFu) & M24);
  g.x0 = (uint32_t)(_v556 & M24);
  }
  { // $08FE: jclr     #$0,x0,func_000901
  GEN_TRC(0x08FE);
    if (!((g.x0 >> 0) & 1)) goto L_000901;
  }
  { // $0900: move     r1,r0
  GEN_TRC(0x0900);
  g.r[0] = (uint32_t)(g.r[1]);
  }
L_000901:;
  { // $0901: move     #$0,r2
  GEN_TRC(0x0901);
  g.r[2] = (uint32_t)(0x000000u);
  }
  { // $0902: move     a1,r5
  GEN_TRC(0x0902);
  g.r[5] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $0903: move     l:(r7),ab
  GEN_TRC(0x0903);
  const uint64_t _v557 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)s24((uint32_t)((_v557 >> 24) & M24)) << 24); g.B = sext56(s24((uint32_t)((_v557) & M24)) << 24);
  g.A = sext56((int64_t)s24((uint32_t)((_v557 >> 24) & M24)) << 24); g.B = sext56((int64_t)s24((uint32_t)((_v557) & M24)) << 24);
  }
  CHK(0x0904);
  _do0904 = 16; // $0904: do
  Ldo0904_top:;
    { // $0906: move     a,x:(r2)+ y:(r4)+,y1
    GEN_TRC(0x0906);
    const uint32_t _v558 = M.rd('y', g.r[4]);
    const uint32_t _v559 = g.r[2];
    M.wr('x', _v559, acc_to24(g.A));
    g.pstep(2,1);
    g.y1 = (uint32_t)(_v558 & M24);
    g.pstep(4,1);
    }
    { // $0907: move     b,x:(r3)+ y:(r4)+,y0
    GEN_TRC(0x0907);
    const uint32_t _v560 = M.rd('y', g.r[4]);
    const uint32_t _v561 = g.r[3];
    M.wr('x', _v561, acc_to24(g.B));
    g.pstep(3,1);
    g.y0 = (uint32_t)(_v560 & M24);
    g.pstep(4,1);
    }
    { // $0908: move     l:(r1)+,x
    GEN_TRC(0x0908);
    const uint64_t _v562 = M.rdL(g.r[1]);
    g.x1 = (uint32_t)((_v562 >> 24) & M24); g.x0 = (uint32_t)(_v562 & M24);
    g.pstep(1,1);
    }
    { // $0909: mpysu    y0,x0,a
    GEN_TRC(0x0909);
    gm_macsu(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), false);
    }
    { // $090A: dmac     ss y0,x1,a
    GEN_TRC(0x090A);
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x1), true);
    }
    { // $090B: move     l:(r0)+,x
    GEN_TRC(0x090B);
    const uint64_t _v563 = M.rdL(g.r[0]);
    g.x1 = (uint32_t)((_v563 >> 24) & M24); g.x0 = (uint32_t)(_v563 & M24);
    g.pstep(0,1);
    }
    { // $090C: mpysu    y1,x0,b
    GEN_TRC(0x090C);
    gm_macsu(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x0), false);
    }
    { // $090D: dmac     ss y1,x1,b
    GEN_TRC(0x090D);
    gm_mac(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    }
    { // $090E: asl      #$4,a,a
    GEN_TRC(0x090E);
    const int64_t _v564 = g.A;
    g.fc = (int)((_v564 >> 52) & 1);
    g.A = sext56(_v564 << 4);
    gm_flags56(g, g.A);
    }
    { // $090F: asl      #$4,b,b
    GEN_TRC(0x090F);
    const int64_t _v565 = g.B;
    g.fc = (int)((_v565 >> 52) & 1);
    g.B = sext56(_v565 << 4);
    gm_flags56(g, g.B);
    }
  if (--_do0904 > 0) goto Ldo0904_top; // конец do
    { // $0910: move     a,x:(r2)+
    GEN_TRC(0x0910);
    const uint32_t _v566 = g.r[2];
    M.wr('x', _v566, acc_to24(g.A));
    g.pstep(2,1);
    }
  { // $0911: move     ab,l:(r7)+
  GEN_TRC(0x0911);
  const uint32_t _v567 = g.r[7];
  M.wr('x', _v567, (uint32_t)(g.B >> 24)); M.wr('y', _v567, (uint32_t)(g.A >> 24));
  g.pstep(7,1);
  }
  CHK(0x0912);
  { // $0912: move     y:(r5+$1448c6),y0
  GEN_TRC(0x0912);
  const uint32_t _v568 = rom.rd('y', (g.r[5] + 0x1448C6u) & M24);
  g.y0 = (uint32_t)(_v568 & M24);
  }
  { // $0914: move     y:(r5+$144946),y1
  GEN_TRC(0x0914);
  const uint32_t _v569 = rom.rd('y', (g.r[5] + 0x144946u) & M24);
  g.y1 = (uint32_t)(_v569 & M24);
  }
  { // $0916: tfr      y0,b b,x:(r3)+
  GEN_TRC(0x0916);
  const uint32_t _v570 = acc_to24(g.A);
  const uint32_t _v571 = acc_to24(g.B);
  const int64_t _v572 = g.A;
  const int64_t _v573 = g.B;
  g.B = (int64_t)s24(g.y0) << 24;
  const uint32_t _v574 = g.r[3];
  M.wr('x', _v574, _v571);
  g.pstep(3,1);
  }
  { // $0917: cmp      #>$400000,b
  GEN_TRC(0x0917);
  gm_cmp(g, g.B, (int64_t)s24(0x400000u) << 24);
  }
  { // $0919: move     l:(r7),a
  GEN_TRC(0x0919);
  const uint64_t _v575 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v575 & 0xFFFFFFFFFFll));
  }
  { // $091A: asr      a ifeq
  GEN_TRC(0x091A);
  const int64_t _v576 = g.A;
  g.fc = (int)((_v576 >> 0) & 1);
  g.A = sext56(_v576 >> 1);
  gm_flags56(g, g.A);
  }
  { // $091B: move     a,l:(r7)
  GEN_TRC(0x091B);
  const uint32_t _v577 = g.r[7];
  M.wrL(_v577, (uint64_t)g.A & 0xFFFFFFFFFFll);
  }
  { // $091C: tfr      y1,b #>$7fffff,x1
  GEN_TRC(0x091C);
  g.B = (int64_t)s24(g.y1) << 24;
  g.x1 = (uint32_t)(0x7FFFFFu);
  }
  { // $091E: asr      #$10,b,b
  GEN_TRC(0x091E);
  const int64_t _v578 = g.B;
  g.fc = (int)((_v578 >> 15) & 1);
  g.B = sext56(_v578 >> 16);
  gm_flags56(g, g.B);
  }
  { // $091F: move     y,l:?:>$fe
  GEN_TRC(0x091F);
  const uint32_t _v579 = 0x0000feu;
  M.wr('x', _v579, g.y1); M.wr('y', _v579, g.y0);
  }
  { // $0921: move     b0,a
  GEN_TRC(0x0921);
  g.A = (g.A & ~(int64_t)0xFFFFFF000000ll) | ((int64_t)(g.B & M24) << 24);
  }
  { // $0922: lsr      a b1,y1
  GEN_TRC(0x0922);
  const uint32_t _v580 = acc_to24(g.A);
  const uint32_t _v581 = acc_to24(g.B);
  const int64_t _v582 = g.A;
  const int64_t _v583 = g.B;
  const int64_t _v584 = g.A;
  g.fc = (int)((_v584 >> 0) & 1);
  g.A = (int64_t)(((uint64_t)_v584 & 0xFFFFFFFFFFFFFFll) >> 1);
  gm_flags56(g, g.A);
  g.y1 = (uint32_t)(((_v583 >> 24) & M24));
  }
  { // $0923: move     b0,y0
  GEN_TRC(0x0923);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0924: move     a1,x0
  GEN_TRC(0x0924);
  g.x0 = (uint32_t)((g.A >> 24) & M24);
  }
  { // $0925: move     l:(r7)+,a
  GEN_TRC(0x0925);
  const uint64_t _v585 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)(_v585 & 0xFFFFFFFFFFll));
  g.pstep(7,1);
  }
  { // $0926: move     a0,b
  GEN_TRC(0x0926);
  g.B = (g.B & ~(int64_t)0xFFFFFF000000ll) | ((int64_t)(g.A & M24) << 24);
  }
  { // $0927: lsr      b #$80,r1
  GEN_TRC(0x0927);
  const int64_t _v586 = g.B;
  g.fc = (int)((_v586 >> 0) & 1);
  g.B = (int64_t)(((uint64_t)_v586 & 0xFFFFFFFFFFFFFFll) >> 1);
  gm_flags56(g, g.B);
  g.r[1] = (uint32_t)(0x000080u);
  }
  { // $0928: move     a1,r0
  GEN_TRC(0x0928);
  g.r[0] = (int32_t)s24(acc_to24n(g.A));
  }
  _do0929 = 17; // $0929: do
  Ldo0929_top:;
    { // $092B: add      y,a a,l:(r1)
    GEN_TRC(0x092B);
    const uint32_t _v587 = acc_to24(g.A);
    const uint32_t _v588 = acc_to24(g.B);
    const int64_t _v589 = g.A;
    const int64_t _v590 = g.B;
    g.A = acc_add56(g, g.A, sext56(((int64_t)g.y1 << 24) | g.y0));
    const uint32_t _v591 = g.r[1];
    M.wrL(_v591, (uint64_t)_v589 & 0xFFFFFFFFFFll);
    }
    { // $092C: add      x0,b b1,y:(r1)+
    GEN_TRC(0x092C);
    const uint32_t _v592 = acc_to24(g.A);
    const uint32_t _v593 = acc_to24(g.B);
    const int64_t _v594 = g.A;
    const int64_t _v595 = g.B;
    g.B = acc_add56(g, g.B, (int64_t)s24(g.x0) << 24);
    const uint32_t _v596 = g.r[1];
    M.wr('y', _v596, (uint32_t)(((_v595 >> 24) & M24)));
    g.pstep(1,1);
    }
    { // $092D: and      x1,b
    GEN_TRC(0x092D);
    gm_logic(g, 'a', "b", s24(g.x1));
    }
  if (--_do0929 > 0) goto Ldo0929_top; // конец do
    { // $092E: move     y:>$80,y0
    GEN_TRC(0x092E);
    const uint32_t _v597 = M.rd('y', 0x000080u);
    g.y0 = (uint32_t)(_v597 & M24);
    }
  { // $0930: move     #$81,r4
  GEN_TRC(0x0930);
  g.r[4] = (uint32_t)(0x000081u);
  }
  { // $0931: move     #$22,r2
  GEN_TRC(0x0931);
  g.r[2] = (uint32_t)(0x000022u);
  }
  { // $0932: move     #$34,r3
  GEN_TRC(0x0932);
  g.r[3] = (uint32_t)(0x000034u);
  }
  { // $0933: move     l:(r7)+,ab
  GEN_TRC(0x0933);
  const uint64_t _v598 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)s24((uint32_t)((_v598 >> 24) & M24)) << 24); g.B = sext56(s24((uint32_t)((_v598) & M24)) << 24);
  g.A = sext56((int64_t)s24((uint32_t)((_v598 >> 24) & M24)) << 24); g.B = sext56((int64_t)s24((uint32_t)((_v598) & M24)) << 24);
  g.pstep(7,1);
  }
  { // $0934: move     x:(r0)+,x1
  GEN_TRC(0x0934);
  const uint32_t _v599 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v599 & M24);
  g.pstep(0,1);
  }
  { // $0935: move     a,x:(r2)+
  GEN_TRC(0x0935);
  const uint32_t _v600 = g.r[2];
  M.wr('x', _v600, acc_to24(g.A));
  g.pstep(2,1);
  }
  { // $0936: move     b,x:(r3)+
  GEN_TRC(0x0936);
  const uint32_t _v601 = g.r[3];
  M.wr('x', _v601, acc_to24(g.B));
  g.pstep(3,1);
  }
  { // $0937: move     l:(r7)-,ab
  GEN_TRC(0x0937);
  const uint64_t _v602 = M.rdL(g.r[7]);
  g.A = sext56((int64_t)s24((uint32_t)((_v602 >> 24) & M24)) << 24); g.B = sext56(s24((uint32_t)((_v602) & M24)) << 24);
  g.A = sext56((int64_t)s24((uint32_t)((_v602 >> 24) & M24)) << 24); g.B = sext56((int64_t)s24((uint32_t)((_v602) & M24)) << 24);
  g.pstep(7,-1);
  }
  { // $0938: jsr      func_000397
  GEN_TRC(0x0938);
    func_000397(M, rom, g);
  }
  CHK(0x0939);
  { // $0939: move     #>$10,b
  GEN_TRC(0x0939);
  g.B = sext56(((int64_t)s24((uint32_t)(0x000010u))) << 24);
  }
  { // $093B: move     l:-(r7),a
  GEN_TRC(0x093B);
  g.pstep(7,-1);
  const uint64_t _v603 = M.rdL((g.r[7] - 1) & M24);
  g.A = sext56((int64_t)(_v603 & 0xFFFFFFFFFFll));
  }
  { // $093C: sub      a,b #$11,n0
  GEN_TRC(0x093C);
  g.B = acc_sub56(g, g.B, g.A);
  g.n[0] = (uint32_t)(0x000011u);
  }
  { // $093D: move     y:>$fe,y0
  GEN_TRC(0x093D);
  const uint32_t _v604 = M.rd('y', 0x0000feu);
  g.y0 = (uint32_t)(_v604 & M24);
  }
  { // $093F: move     b1,x1
  GEN_TRC(0x093F);
  g.x1 = (uint32_t)((g.B >> 24) & M24);
  }
  { // $0940: move     b0,x0
  GEN_TRC(0x0940);
  g.x0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0941: mpysu    y0,x0,b
  GEN_TRC(0x0941);
  gm_macsu(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0), false);
  }
  { // $0942: dmac     ss y0,x1,b
  GEN_TRC(0x0942);
  gm_mac(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x1), true);
  }
  { // $0943: asl      b #>$22,x1
  GEN_TRC(0x0943);
  const int64_t _v605 = g.B;
  g.fc = (int)((_v605 >> 55) & 1);
  g.B = sext56(_v605 << 1);
  gm_flags56(g, g.B);
  g.x1 = (uint32_t)(0x000022u);
  }
  { // $0945: move     #>$7fffff,x0
  GEN_TRC(0x0945);
  g.x0 = (uint32_t)(0x7FFFFFu);
  }
  { // $0947: add      x,b n0,n1
  GEN_TRC(0x0947);
  g.B = acc_add56(g, g.B, sext56(((int64_t)g.x1 << 24) | g.x0));
  g.n[1] = (uint32_t)(g.n[0]);
  }
  { // $0948: rnd      b y:(r7)+,y0
  GEN_TRC(0x0948);
  const uint32_t _v606 = M.rd('y', g.r[7]);
  mnmfix::rnd_acc(g.B); gm_flags56(g, g.B);
  g.y0 = (uint32_t)(_v606 & M24);
  g.pstep(7,1);
  }
  { // $0949: move     b,r0
  GEN_TRC(0x0949);
  g.r[0] = (int32_t)s24(acc_to24n(g.B));
  }
  { // $094A: move     x:>$fe,a
  GEN_TRC(0x094A);
  const uint32_t _v607 = M.rd('x', 0x0000feu);
  g.A = sext56(((int64_t)s24((uint32_t)(_v607))) << 24);
  }
  { // $094C: asr      #$10,a,a
  GEN_TRC(0x094C);
  const int64_t _v608 = g.A;
  g.fc = (int)((_v608 >> 15) & 1);
  g.A = sext56(_v608 >> 16);
  gm_flags56(g, g.A);
  }
  { // $094D: move     x:(r0)+,x1
  GEN_TRC(0x094D);
  const uint32_t _v609 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v609 & M24);
  g.pstep(0,1);
  }
  { // $094E: move     x:(r0)+n0,y1
  GEN_TRC(0x094E);
  const uint32_t _v610 = M.rd('x', g.r[0]);
  g.y1 = (uint32_t)(_v610 & M24);
  g.pstep(0,g.n[0]);
  }
  { // $094F: move     x:(r0)+,x0
  GEN_TRC(0x094F);
  const uint32_t _v611 = M.rd('x', g.r[0]);
  g.x0 = (uint32_t)(_v611 & M24);
  g.pstep(0,1);
  }
  { // $0950: move     x:(r0),y0
  GEN_TRC(0x0950);
  const uint32_t _v612 = M.rd('x', g.r[0]);
  g.y0 = (uint32_t)(_v612 & M24);
  }
  { // $0951: move     x,l:(r7)+
  GEN_TRC(0x0951);
  const uint32_t _v613 = g.r[7];
  M.wr('x', _v613, g.x1); M.wr('y', _v613, g.x0);
  g.pstep(7,1);
  }
  { // $0952: move     y,l:(r7)-
  GEN_TRC(0x0952);
  const uint32_t _v614 = g.r[7];
  M.wr('x', _v614, g.y1); M.wr('y', _v614, g.y0);
  g.pstep(7,-1);
  }
  { // $0953: move     x:(r0+$4b),b
  GEN_TRC(0x0953);
  const uint32_t _v615 = M.rd('x', (g.r[0] + 0x4Bu) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v615))) << 24);
  }
  { // $0955: move     y:(r0+$4b),b0
  GEN_TRC(0x0955);
  const uint32_t _v616 = M.rd('y', (g.r[0] + 0x4Bu) & M24);
  g.B = (g.B & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v616) & M24) << 0);
  }
  { // $0957: sub      #<$10,b
  GEN_TRC(0x0957);
  g.B = acc_sub56(g, g.B, (int64_t)s24(0x000010u) << 24);
  }
  { // $0958: cmp      a,b y:>$fe,y0
  GEN_TRC(0x0958);
  const uint32_t _v617 = M.rd('y', 0x0000feu);
  gm_cmp(g, g.B, g.A);
  g.y0 = (uint32_t)(_v617 & M24);
  }
  { // $095A: tfr      a,b             ifgt
  GEN_TRC(0x095A);
  if (((g.fn==g.fv)&&!g.fz)) g.B = g.A;
  }
  { // $095B: move     #>$23,a
  GEN_TRC(0x095B);
  g.A = sext56(((int64_t)s24((uint32_t)(0x000023u))) << 24);
  }
  { // $095D: move     l:-(r7),x
  GEN_TRC(0x095D);
  g.pstep(7,-1);
  const uint64_t _v618 = M.rdL((g.r[7] - 1) & M24);
  g.x1 = (uint32_t)((_v618 >> 24) & M24); g.x0 = (uint32_t)(_v618 & M24);
  }
  { // $095E: move     b,l:(r7)
  GEN_TRC(0x095E);
  const uint32_t _v619 = g.r[7];
  M.wrL(_v619, (uint64_t)g.B & 0xFFFFFFFFFFll);
  }
  { // $095F: mpysu    y0,x0,b
  GEN_TRC(0x095F);
  gm_macsu(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0), false);
  }
  { // $0960: dmac     ss y0,x1,b
  GEN_TRC(0x0960);
  gm_mac(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x1), true);
  }
  { // $0961: asl      b #$f,r2
  GEN_TRC(0x0961);
  const int64_t _v620 = g.B;
  g.fc = (int)((_v620 >> 55) & 1);
  g.B = sext56(_v620 << 1);
  gm_flags56(g, g.B);
  g.r[2] = (uint32_t)(0x00000Fu);
  }
  { // $0962: sub      b,a r2,r3
  GEN_TRC(0x0962);
  g.A = acc_sub56(g, g.A, g.B);
  g.r[3] = (uint32_t)(g.r[2]);
  }
  { // $0963: move     y:>$fe,b
  GEN_TRC(0x0963);
  const uint32_t _v621 = M.rd('y', 0x0000feu);
  g.B = sext56(((int64_t)s24((uint32_t)(_v621))) << 24);
  }
  { // $0965: asr      #$16,b,b
  GEN_TRC(0x0965);
  const int64_t _v622 = g.B;
  g.fc = (int)((_v622 >> 21) & 1);
  g.B = sext56(_v622 >> 22);
  gm_flags56(g, g.B);
  }
  { // $0966: move     b1,y1
  GEN_TRC(0x0966);
  g.y1 = (uint32_t)((g.B >> 24) & M24);
  }
  { // $0967: move     b0,y0
  GEN_TRC(0x0967);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0968: asr      b #>$7fffff,x1
  GEN_TRC(0x0968);
  const int64_t _v623 = g.B;
  g.fc = (int)((_v623 >> 0) & 1);
  g.B = sext56(_v623 >> 1);
  gm_flags56(g, g.B);
  g.x1 = (uint32_t)(0x7FFFFFu);
  }
  { // $096A: move     b0,x0
  GEN_TRC(0x096A);
  g.x0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $096B: move     a0,b
  GEN_TRC(0x096B);
  g.B = (g.B & ~(int64_t)0xFFFFFF000000ll) | ((int64_t)(g.A & M24) << 24);
  }
  { // $096C: lsr      b #$80,r1
  GEN_TRC(0x096C);
  const int64_t _v624 = g.B;
  g.fc = (int)((_v624 >> 0) & 1);
  g.B = (int64_t)(((uint64_t)_v624 & 0xFFFFFFFFFFFFFFll) >> 1);
  gm_flags56(g, g.B);
  g.r[1] = (uint32_t)(0x000080u);
  }
  { // $096D: move     a1,r0
  GEN_TRC(0x096D);
  g.r[0] = (int32_t)s24(acc_to24n(g.A));
  }
  _do096e = 16; // $096E: do
  Ldo096e_top:;
    { // $0970: add      y,a a,l:(r1)
    GEN_TRC(0x0970);
    const uint32_t _v625 = acc_to24(g.A);
    const uint32_t _v626 = acc_to24(g.B);
    const int64_t _v627 = g.A;
    const int64_t _v628 = g.B;
    g.A = acc_add56(g, g.A, sext56(((int64_t)g.y1 << 24) | g.y0));
    const uint32_t _v629 = g.r[1];
    M.wrL(_v629, (uint64_t)_v627 & 0xFFFFFFFFFFll);
    }
    { // $0971: add      x0,b b1,y:(r1)+
    GEN_TRC(0x0971);
    const uint32_t _v630 = acc_to24(g.A);
    const uint32_t _v631 = acc_to24(g.B);
    const int64_t _v632 = g.A;
    const int64_t _v633 = g.B;
    g.B = acc_add56(g, g.B, (int64_t)s24(g.x0) << 24);
    const uint32_t _v634 = g.r[1];
    M.wr('y', _v634, (uint32_t)(((_v633 >> 24) & M24)));
    g.pstep(1,1);
    }
    { // $0972: and      x1,b
    GEN_TRC(0x0972);
    gm_logic(g, 'a', "b", s24(g.x1));
    }
  if (--_do096e > 0) goto Ldo096e_top; // конец do
    { // $0973: move     y:>$80,y0
    GEN_TRC(0x0973);
    const uint32_t _v635 = M.rd('y', 0x000080u);
    g.y0 = (uint32_t)(_v635 & M24);
    }
  { // $0975: move     #$81,r4
  GEN_TRC(0x0975);
  g.r[4] = (uint32_t)(0x000081u);
  }
  { // $0976: move     x:(r0)+,x1
  GEN_TRC(0x0976);
  const uint32_t _v636 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v636 & M24);
  g.pstep(0,1);
  }
  { // $0977: move     #$f,m2
  GEN_TRC(0x0977);
  g.m[2] = (uint32_t)(0x00000Fu);
  }
  { // $0978: jsr      func_000397
  GEN_TRC(0x0978);
    func_000397(M, rom, g);
  }
  { // $0979: move     #>$ffffff,m2
  GEN_TRC(0x0979);
  g.m[2] = (uint32_t)(0xFFFFFFu);
  }
  { // $097B: lua      (r6+$28),r6
  GEN_TRC(0x097B);
    g.r[6] = (uint32_t)((int64_t)g.r[6] + 1 * 0x28ll) & M24;
  }
  { // $097C: move     x:(r6-$1c),a
  GEN_TRC(0x097C);
  const uint32_t _v637 = M.rd('x', (g.r[6] - 0x1Cu) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v637))) << 24);
  }
  { // $097D: tst      a
  GEN_TRC(0x097D);
  gm_test56(g, g.A);
  }
  { // $097E: beq      func_000981
  GEN_TRC(0x097E);
    if (g.fz) goto L_000981;
  }
  { // $097F: move     a,r0
  GEN_TRC(0x097F);
  g.r[0] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $0980: jmp      (r0)
  GEN_TRC(0x0980);
    if (g.r[0] != 0) {
      if (g.r[0] == 0x145C48) m14_handler(M, rom, g);
    }
    goto L_000981;
  }
L_000981:;
  { // $0981: lua      (r6-$28),r6
  GEN_TRC(0x0981);
    g.r[6] = (uint32_t)((int64_t)g.r[6] + -1 * 0x28ll) & M24;
  }
  CHK(0x0982);
  { // $0982: move     #>$15c,n6
  GEN_TRC(0x0982);
  g.n[6] = (uint32_t)(0x00015Cu);
  }
  { // $0984: lua      (r6)+n6,r7
  GEN_TRC(0x0984);
    g.r[7] = (uint32_t)((int64_t)g.r[6] + 1 * g.n[6]) & M24;
  }
  { // $0985: move     y:(r6+$13),a
  GEN_TRC(0x0985);
  const uint32_t _v638 = M.rd('y', (g.r[6] + 0x13u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v638))) << 24);
  }
  { // $0986: add      #>$8000,a
  GEN_TRC(0x0986);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x008000u) << 24);
  }
  { // $0988: asr      #$10,a,a
  GEN_TRC(0x0988);
  const int64_t _v639 = g.A;
  g.fc = (int)((_v639 >> 15) & 1);
  g.A = sext56(_v639 >> 16);
  gm_flags56(g, g.A);
  }
  { // $0989: move     #>$1,b
  GEN_TRC(0x0989);
  g.B = sext56(((int64_t)s24((uint32_t)(0x000001u))) << 24);
  }
  { // $098B: move     a,r0
  GEN_TRC(0x098B);
  g.r[0] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $098C: move     b1,x:(r7-$8f)
  GEN_TRC(0x098C);
  const uint32_t _v640 = (g.r[7] - 0x8Fu) & M24;
  M.wr('x', _v640, (uint32_t)((g.B >> 24) & M24));
  }
  { // $098E: move     b0,y:(r7-$8f)
  GEN_TRC(0x098E);
  const uint32_t _v641 = (g.r[7] - 0x8Fu) & M24;
  M.wr('y', _v641, (uint32_t)(g.B & M24));
  }
  { // $0990: move     x:(r0+$144bc9),x1
  GEN_TRC(0x0990);
  const uint32_t _v642 = rom.rd('x', (g.r[0] + 0x144BC9u) & M24);
  g.x1 = (uint32_t)(_v642 & M24);
  }
  { // $0992: move     x:(r0+$144c49),x0
  GEN_TRC(0x0992);
  const uint32_t _v643 = rom.rd('x', (g.r[0] + 0x144C49u) & M24);
  g.x0 = (uint32_t)(_v643 & M24);
  }
  { // $0994: move     y:(r6+$23),y0
  GEN_TRC(0x0994);
  const uint32_t _v644 = M.rd('y', (g.r[6] + 0x23u) & M24);
  g.y0 = (uint32_t)(_v644 & M24);
  }
  { // $0995: mpyi     #>$791fd0,y0,a
  GEN_TRC(0x0995);
  gm_mpy(g, g.A, (int64_t)s24(0x791FD0u), (int64_t)s24(g.y0));
  }
  { // $0997: asl      #$5,a,a
  GEN_TRC(0x0997);
  const int64_t _v645 = g.A;
  g.fc = (int)((_v645 >> 51) & 1);
  g.A = sext56(_v645 << 5);
  gm_flags56(g, g.A);
  }
  { // $0998: move     a,y0
  GEN_TRC(0x0998);
  g.y0 = acc_to24(g.A);
  }
  { // $0999: mpy      x1,y0,a
  GEN_TRC(0x0999);
  gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0));
  }
  { // $099A: cmp      #>$4000,a
  GEN_TRC(0x099A);
  gm_cmp(g, g.A, (int64_t)s24(0x004000u) << 24);
  }
  { // $099C: blt      func_0009ab
  GEN_TRC(0x099C);
    if ((g.fn!=g.fv)) goto L_0009ab;
  }
  { // $099D: move     y:(r6+$2a),y0
  GEN_TRC(0x099D);
  const uint32_t _v646 = M.rd('y', (g.r[6] + 0x2Au) & M24);
  g.y0 = (uint32_t)(_v646 & M24);
  }
  { // $099E: mpy      y0,x0,b
  GEN_TRC(0x099E);
  gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
  }
  { // $099F: move     #>$21d10,x1
  GEN_TRC(0x099F);
  g.x1 = (uint32_t)(0x021D10u);
  }
  { // $09A1: move     b1,y1
  GEN_TRC(0x09A1);
  g.y1 = (uint32_t)((g.B >> 24) & M24);
  }
  { // $09A2: move     b0,y0
  GEN_TRC(0x09A2);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $09A3: mpysu    x1,y0,b
  GEN_TRC(0x09A3);
  gm_macsu(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), false);
  }
  { // $09A4: dmac     ss x1,y1,b
  GEN_TRC(0x09A4);
  gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y1), true);
  }
  { // $09A5: move     #>$3ff0,a
  GEN_TRC(0x09A5);
  g.A = sext56(((int64_t)s24((uint32_t)(0x003FF0u))) << 24);
  }
  { // $09A7: move     b1,x:(r7-$8f)
  GEN_TRC(0x09A7);
  const uint32_t _v647 = (g.r[7] - 0x8Fu) & M24;
  M.wr('x', _v647, (uint32_t)((g.B >> 24) & M24));
  }
  { // $09A9: move     b0,y:(r7-$8f)
  GEN_TRC(0x09A9);
  const uint32_t _v648 = (g.r[7] - 0x8Fu) & M24;
  M.wr('y', _v648, (uint32_t)(g.B & M24));
  }
L_0009ab:;
  { // $09AB: move     a1,x:(r7-$8e)
  GEN_TRC(0x09AB);
  const uint32_t _v649 = (g.r[7] - 0x8Eu) & M24;
  M.wr('x', _v649, (uint32_t)((g.A >> 24) & M24));
  }
  { // $09AD: move     a0,y:(r7-$8e)
  GEN_TRC(0x09AD);
  const uint32_t _v650 = (g.r[7] - 0x8Eu) & M24;
  M.wr('y', _v650, (uint32_t)(g.A & M24));
  }
  { // $09AF: move     x:(r7-$8e),b
  GEN_TRC(0x09AF);
  const uint32_t _v651 = M.rd('x', (g.r[7] - 0x8Eu) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v651))) << 24);
  }
  { // $09B1: move     y:(r7-$8e),b0
  GEN_TRC(0x09B1);
  const uint32_t _v652 = M.rd('y', (g.r[7] - 0x8Eu) & M24);
  g.B = (g.B & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v652) & M24) << 0);
  }
  { // $09B3: move     x:(r7-$8c),a
  GEN_TRC(0x09B3);
  const uint32_t _v653 = M.rd('x', (g.r[7] - 0x8Cu) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v653))) << 24);
  }
  { // $09B5: move     y:(r7-$8c),a0
  GEN_TRC(0x09B5);
  const uint32_t _v654 = M.rd('y', (g.r[7] - 0x8Cu) & M24);
  g.A = (g.A & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v654) & M24) << 0);
  }
  { // $09B7: move     #$40,y1
  GEN_TRC(0x09B7);
  g.y1 = (uint32_t)(0x400000u);
  }
  { // $09B8: move     a,l:?:>$fc
  GEN_TRC(0x09B8);
  const uint32_t _v655 = 0x0000fcu;
  M.wrL(_v655, (uint64_t)g.A & 0xFFFFFFFFFFll);
  }
  { // $09BA: move     y1,x:>$fe
  GEN_TRC(0x09BA);
  const uint32_t _v656 = 0x0000feu;
  M.wr('x', _v656, (uint32_t)(g.y1 & M24));
  }
  { // $09BC: cmp      b,a
  GEN_TRC(0x09BC);
  gm_cmp(g, g.A, g.B);
  }
  { // $09BD: beq      func_0009cb
  GEN_TRC(0x09BD);
    if (g.fz) goto L_0009cb;
  }
  { // $09BE: bgt      func_0009c5
  GEN_TRC(0x09BE);
    if (((g.fn==g.fv)&&!g.fz)) goto L_0009c5;
  }
  { // $09BF: sub      a,b #>$4,y0
  GEN_TRC(0x09BF);
  g.B = acc_sub56(g, g.B, g.A);
  g.y0 = (uint32_t)(0x000004u);
  }
  { // $09C1: cmpm     y0,b
  GEN_TRC(0x09C1);
  gm_cmp(g, g.B, (int64_t)s24(g.y0) << 24);
  }
  { // $09C2: tfr      y0,b ifgt
  GEN_TRC(0x09C2);
  if (((g.fn==g.fv)&&!g.fz)) g.B = (int64_t)s24(g.y0) << 24;
  }
  { // $09C3: add      b,a #$30,y1
  GEN_TRC(0x09C3);
  g.A = acc_add56(g, g.A, g.B);
  g.y1 = (uint32_t)(0x300000u);
  }
  { // $09C4: bra      func_0009cb
  GEN_TRC(0x09C4);
    if (true) goto L_0009cb;
  }
L_0009c5:;
  { // $09C5: sub      a,b #>$fffffc,y0
  GEN_TRC(0x09C5);
  g.B = acc_sub56(g, g.B, g.A);
  g.y0 = (uint32_t)(0xFFFFFCu);
  }
  { // $09C7: cmp      y0,b
  GEN_TRC(0x09C7);
  gm_cmp(g, g.B, (int64_t)s24(g.y0) << 24);
  }
  { // $09C8: tfr      y0,b            iflt
  GEN_TRC(0x09C8);
  if ((g.fn!=g.fv)) g.B = (int64_t)s24(g.y0) << 24;
  }
  { // $09C9: add      b,a #>$500000,y1
  GEN_TRC(0x09C9);
  g.A = acc_add56(g, g.A, g.B);
  g.y1 = (uint32_t)(0x500000u);
  }
L_0009cb:;
  { // $09CB: move     x:(r7-$8d),b
  GEN_TRC(0x09CB);
  const uint32_t _v657 = M.rd('x', (g.r[7] - 0x8Du) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v657))) << 24);
  }
  { // $09CD: move     y:(r7-$8d),b0
  GEN_TRC(0x09CD);
  const uint32_t _v658 = M.rd('y', (g.r[7] - 0x8Du) & M24);
  g.B = (g.B & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v658) & M24) << 0);
  }
  { // $09CF: asl      #$a,b,b
  GEN_TRC(0x09CF);
  const int64_t _v659 = g.B;
  g.fc = (int)((_v659 >> 46) & 1);
  g.B = sext56(_v659 << 10);
  gm_flags56(g, g.B);
  }
  { // $09D0: cmp      #>$3ff,b
  GEN_TRC(0x09D0);
  gm_cmp(g, g.B, (int64_t)s24(0x0003FFu) << 24);
  }
  { // $09D2: blt      func_0009d7
  GEN_TRC(0x09D2);
    if ((g.fn!=g.fv)) goto L_0009d7;
  }
  { // $09D3: move     y1,x:>$fe
  GEN_TRC(0x09D3);
  const uint32_t _v660 = 0x0000feu;
  M.wr('x', _v660, (uint32_t)(g.y1 & M24));
  }
  { // $09D5: move     a,l:?:>$fc
  GEN_TRC(0x09D5);
  const uint32_t _v661 = 0x0000fcu;
  M.wrL(_v661, (uint64_t)g.A & 0xFFFFFFFFFFll);
  }
L_0009d7:;
  { // $09D7: move     y:(r7-$8f),y0
  GEN_TRC(0x09D7);
  const uint32_t _v662 = M.rd('y', (g.r[7] - 0x8Fu) & M24);
  g.y0 = (uint32_t)(_v662 & M24);
  }
  { // $09D9: move     y:(r7-$8d),y1
  GEN_TRC(0x09D9);
  const uint32_t _v663 = M.rd('y', (g.r[7] - 0x8Du) & M24);
  g.y1 = (uint32_t)(_v663 & M24);
  }
  { // $09DB: move     #>$ccccd,x0
  GEN_TRC(0x09DB);
  g.x0 = (uint32_t)(0x0CCCCDu);
  }
  { // $09DD: move     #>$733333,x1
  GEN_TRC(0x09DD);
  g.x1 = (uint32_t)(0x733333u);
  }
  { // $09DF: mpysu    x0,y0,a
  GEN_TRC(0x09DF);
  gm_macsu(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y0), false);
  }
  { // $09E0: macsu    x1,y1,a
  GEN_TRC(0x09E0);
  gm_macsu(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y1), true);
  }
  { // $09E1: move     x:(r7-$8f),y0
  GEN_TRC(0x09E1);
  const uint32_t _v664 = M.rd('x', (g.r[7] - 0x8Fu) & M24);
  g.y0 = (uint32_t)(_v664 & M24);
  }
  { // $09E3: move     x:(r7-$8d),y1
  GEN_TRC(0x09E3);
  const uint32_t _v665 = M.rd('x', (g.r[7] - 0x8Du) & M24);
  g.y1 = (uint32_t)(_v665 & M24);
  }
  { // $09E5: dmac     ss x0,y0,a
  GEN_TRC(0x09E5);
  gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y0), true);
  }
  { // $09E6: mac      y1,x1,a
  GEN_TRC(0x09E6);
  gm_mac(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
  }
  { // $09E7: move     x:(r7-$8c),b
  GEN_TRC(0x09E7);
  const uint32_t _v666 = M.rd('x', (g.r[7] - 0x8Cu) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v666))) << 24);
  }
  { // $09E9: cmp      #>$3ff0,b
  GEN_TRC(0x09E9);
  gm_cmp(g, g.B, (int64_t)s24(0x003FF0u) << 24);
  }
  { // $09EB: blt      func_0009f0
  GEN_TRC(0x09EB);
    if ((g.fn!=g.fv)) goto L_0009f0;
  }
  { // $09EC: move     a1,x:(r7-$8d)
  GEN_TRC(0x09EC);
  const uint32_t _v667 = (g.r[7] - 0x8Du) & M24;
  M.wr('x', _v667, (uint32_t)((g.A >> 24) & M24));
  }
  { // $09EE: move     a0,y:(r7-$8d)
  GEN_TRC(0x09EE);
  const uint32_t _v668 = (g.r[7] - 0x8Du) & M24;
  M.wr('y', _v668, (uint32_t)(g.A & M24));
  }
L_0009f0:;
  { // $09F0: move     #>$3fff,x0
  GEN_TRC(0x09F0);
  g.x0 = (uint32_t)(0x003FFFu);
  }
  { // $09F2: move     y:(r7-$8b),x1
  GEN_TRC(0x09F2);
  const uint32_t _v669 = M.rd('y', (g.r[7] - 0x8Bu) & M24);
  g.x1 = (uint32_t)(_v669 & M24);
  }
  { // $09F4: move     x:(r7-$86),b
  GEN_TRC(0x09F4);
  const uint32_t _v670 = M.rd('x', (g.r[7] - 0x86u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v670))) << 24);
  }
  { // $09F6: move     y:(r7-$86),b0
  GEN_TRC(0x09F6);
  const uint32_t _v671 = M.rd('y', (g.r[7] - 0x86u) & M24);
  g.B = (g.B & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v671) & M24) << 0);
  }
  { // $09F8: move     l:?:>$fc,y
  GEN_TRC(0x09F8);
  const uint64_t _v672 = M.rdL(0x0000fcu);
  g.y1 = (uint32_t)((_v672 >> 24) & M24); g.y0 = (uint32_t)(_v672 & M24);
  }
  { // $09FA: move     y1,x:(r7-$8c)
  GEN_TRC(0x09FA);
  const uint32_t _v673 = (g.r[7] - 0x8Cu) & M24;
  M.wr('x', _v673, (uint32_t)(g.y1 & M24));
  }
  { // $09FC: move     y0,y:(r7-$8c)
  GEN_TRC(0x09FC);
  const uint32_t _v674 = (g.r[7] - 0x8Cu) & M24;
  M.wr('y', _v674, (uint32_t)(g.y0 & M24));
  }
  { // $09FE: move     y1,a
  GEN_TRC(0x09FE);
  g.A = sext56(((int64_t)s24((uint32_t)(g.y1))) << 24);
  }
  { // $09FF: sub      b,a
  GEN_TRC(0x09FF);
  g.A = acc_sub56(g, g.A, g.B);
  }
  { // $0A00: neg      a
  GEN_TRC(0x0A00);
  g.A = sat56(-g.A);
  gm_flags56(g, g.A);
  }
  { // $0A01: and      x0,a
  GEN_TRC(0x0A01);
  gm_logic(g, 'a', "a", s24(g.x0));
  }
  { // $0A02: move     #$0,a2
  GEN_TRC(0x0A02);
  g.A = (g.A & ~(((int64_t)M24) << 48)) | ((int64_t)(0x000000u & M24) << 48);
  }
  { // $0A03: add      x1,a a,l:?:>$c2
  GEN_TRC(0x0A03);
  const uint32_t _v675 = acc_to24(g.A);
  const uint32_t _v676 = acc_to24(g.B);
  const int64_t _v677 = g.A;
  const int64_t _v678 = g.B;
  g.A = acc_add56(g, g.A, (int64_t)s24(g.x1) << 24);
  const uint32_t _v679 = 0x0000c2u;
  M.wrL(_v679, (uint64_t)_v677 & 0xFFFFFFFFFFll);
  }
  { // $0A05: add      x1,b
  GEN_TRC(0x0A05);
  g.B = acc_add56(g, g.B, (int64_t)s24(g.x1) << 24);
  }
  { // $0A06: move     a1,r4
  GEN_TRC(0x0A06);
  g.r[4] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $0A07: move     b,l:?:>$c4
  GEN_TRC(0x0A07);
  const uint32_t _v680 = 0x0000c4u;
  M.wrL(_v680, (uint64_t)g.B & 0xFFFFFFFFFFll);
  }
  { // $0A09: add      #>$4000,a
  GEN_TRC(0x0A09);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x004000u) << 24);
  }
  { // $0A0B: add      #>$4000,b
  GEN_TRC(0x0A0B);
  g.B = acc_add56(g, g.B, (int64_t)s24(0x004000u) << 24);
  }
  { // $0A0D: move     a1,r5
  GEN_TRC(0x0A0D);
  g.r[5] = (int32_t)s24(acc_to24n(g.A));
  }
  { // $0A0E: move     b,l:?:>$c5
  GEN_TRC(0x0A0E);
  const uint32_t _v681 = 0x0000c5u;
  M.wrL(_v681, (uint64_t)g.B & 0xFFFFFFFFFFll);
  }
  { // $0A10: move     x:(r7-$8d),a
  GEN_TRC(0x0A10);
  const uint32_t _v682 = M.rd('x', (g.r[7] - 0x8Du) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v682))) << 24);
  }
  { // $0A12: move     y:(r7-$8d),a0
  GEN_TRC(0x0A12);
  const uint32_t _v683 = M.rd('y', (g.r[7] - 0x8Du) & M24);
  g.A = (g.A & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v683) & M24) << 0);
  }
  { // $0A14: asl      #$4,a,a
  GEN_TRC(0x0A14);
  const int64_t _v684 = g.A;
  g.fc = (int)((_v684 >> 52) & 1);
  g.A = sext56(_v684 << 4);
  gm_flags56(g, g.A);
  }
  { // $0A15: move     x:(r7-$86),b
  GEN_TRC(0x0A15);
  const uint32_t _v685 = M.rd('x', (g.r[7] - 0x86u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v685))) << 24);
  }
  { // $0A17: move     y:(r7-$86),b0
  GEN_TRC(0x0A17);
  const uint32_t _v686 = M.rd('y', (g.r[7] - 0x86u) & M24);
  g.B = (g.B & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v686) & M24) << 0);
  }
  { // $0A19: add      a,b #$40,r0
  GEN_TRC(0x0A19);
  g.B = acc_add56(g, g.B, g.A);
  g.r[0] = (uint32_t)(0x000040u);
  }
  { // $0A1A: and      x0,b #$62,r1
  GEN_TRC(0x0A1A);
  gm_logic(g, 'a', "b", s24(g.x0));
  g.r[1] = (uint32_t)(0x000062u);
  }
  { // $0A1B: move     #>$3fff,m4
  GEN_TRC(0x0A1B);
  g.m[4] = (uint32_t)(0x003FFFu);
  }
  { // $0A1D: move     b1,x:(r7-$86)
  GEN_TRC(0x0A1D);
  const uint32_t _v687 = (g.r[7] - 0x86u) & M24;
  M.wr('x', _v687, (uint32_t)((g.B >> 24) & M24));
  }
  { // $0A1F: move     b0,y:(r7-$86)
  GEN_TRC(0x0A1F);
  const uint32_t _v688 = (g.r[7] - 0x86u) & M24;
  M.wr('y', _v688, (uint32_t)(g.B & M24));
  }
  { // $0A21: move     m4,m5
  GEN_TRC(0x0A21);
  g.m[5] = (uint32_t)(g.m[4]);
  }
  { // $0A22: move     y:(r6+$14),a
  GEN_TRC(0x0A22);
  const uint32_t _v689 = M.rd('y', (g.r[6] + 0x14u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v689))) << 24);
  }
  { // $0A23: sub      #>$400000,a
  GEN_TRC(0x0A23);
  g.A = acc_sub56(g, g.A, (int64_t)s24(0x400000u) << 24);
  }
  { // $0A25: bmi      func_000a29
  GEN_TRC(0x0A25);
    if (g.fn) goto L_000a29;
  }
  { // $0A27: move     #$62,r0
  GEN_TRC(0x0A27);
  g.r[0] = (uint32_t)(0x000062u);
  }
  { // $0A28: move     #$40,r1
  GEN_TRC(0x0A28);
  g.r[1] = (uint32_t)(0x000040u);
  }
L_000a29:;
  { // $0A29: move     y:(r4)+,a
  GEN_TRC(0x0A29);
  const uint32_t _v690 = M.rd('y', g.r[4]);
  g.A = sext56(((int64_t)s24((uint32_t)(_v690))) << 24);
  g.pstep(4,1);
  }
  { // $0A2A: move     y:(r4)+,b
  GEN_TRC(0x0A2A);
  const uint32_t _v691 = M.rd('y', g.r[4]);
  g.B = sext56(((int64_t)s24((uint32_t)(_v691))) << 24);
  g.pstep(4,1);
  }
  _do0a2b = 16; // $0A2B: do
  Ldo0a2b_top:;
    { // $0A2D: move     a,x:(r0)+ y:(r4)+,a
    GEN_TRC(0x0A2D);
    const uint32_t _v692 = M.rd('y', g.r[4]);
    const uint32_t _v693 = g.r[0];
    M.wr('x', _v693, acc_to24(g.A));
    g.pstep(0,1);
    g.A = sext56(((int64_t)s24((uint32_t)(_v692))) << 24);
    g.pstep(4,1);
    }
    { // $0A2E: move     b,x:(r0)+ y:(r4)+,b
    GEN_TRC(0x0A2E);
    const uint32_t _v694 = M.rd('y', g.r[4]);
    const uint32_t _v695 = g.r[0];
    M.wr('x', _v695, acc_to24(g.B));
    g.pstep(0,1);
    g.B = sext56(((int64_t)s24((uint32_t)(_v694))) << 24);
    g.pstep(4,1);
    }
  if (--_do0a2b > 0) goto Ldo0a2b_top; // конец do
    { // $0A2F: move     a,x:(r0)+ y:(r5)+,a
    GEN_TRC(0x0A2F);
    const uint32_t _v696 = M.rd('y', g.r[5]);
    const uint32_t _v697 = g.r[0];
    M.wr('x', _v697, acc_to24(g.A));
    g.pstep(0,1);
    g.A = sext56(((int64_t)s24((uint32_t)(_v696))) << 24);
    g.pstep(5,1);
    }
  { // $0A30: move     b,x:(r0)+ y:(r5)+,b
  GEN_TRC(0x0A30);
  const uint32_t _v698 = M.rd('y', g.r[5]);
  const uint32_t _v699 = g.r[0];
  M.wr('x', _v699, acc_to24(g.B));
  g.pstep(0,1);
  g.B = sext56(((int64_t)s24((uint32_t)(_v698))) << 24);
  g.pstep(5,1);
  }
  _do0a31 = 17; // $0A31: do
  Ldo0a31_top:;
    { // $0A33: move     a,x:(r1)+ y:(r5)+,a
    GEN_TRC(0x0A33);
    const uint32_t _v700 = M.rd('y', g.r[5]);
    const uint32_t _v701 = g.r[1];
    M.wr('x', _v701, acc_to24(g.A));
    g.pstep(1,1);
    g.A = sext56(((int64_t)s24((uint32_t)(_v700))) << 24);
    g.pstep(5,1);
    }
    { // $0A34: move     b,x:(r1)+ y:(r5)+,b
    GEN_TRC(0x0A34);
    const uint32_t _v702 = M.rd('y', g.r[5]);
    const uint32_t _v703 = g.r[1];
    M.wr('x', _v703, acc_to24(g.B));
    g.pstep(1,1);
    g.B = sext56(((int64_t)s24((uint32_t)(_v702))) << 24);
    g.pstep(5,1);
    }
  if (--_do0a31 > 0) goto Ldo0a31_top; // конец do
    { // $0A35: move     #>$ffffff,m4
    GEN_TRC(0x0A35);
    g.m[4] = (uint32_t)(0xFFFFFFu);
    }
  { // $0A37: move     m4,m5
  GEN_TRC(0x0A37);
  g.m[5] = (uint32_t)(g.m[4]);
  }
  { // $0A38: move     x:>$fe,y1
  GEN_TRC(0x0A38);
  const uint32_t _v704 = M.rd('x', 0x0000feu);
  g.y1 = (uint32_t)(_v704 & M24);
  }
  { // $0A3A: move     x:(r7-$8d),x1
  GEN_TRC(0x0A3A);
  const uint32_t _v705 = M.rd('x', (g.r[7] - 0x8Du) & M24);
  g.x1 = (uint32_t)(_v705 & M24);
  }
  { // $0A3C: move     y:(r7-$8d),x0
  GEN_TRC(0x0A3C);
  const uint32_t _v706 = M.rd('y', (g.r[7] - 0x8Du) & M24);
  g.x0 = (uint32_t)(_v706 & M24);
  }
  { // $0A3E: mpysu    y1,x0,b
  GEN_TRC(0x0A3E);
  gm_macsu(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x0), false);
  }
  { // $0A3F: dmac     ss y1,x1,b
  GEN_TRC(0x0A3F);
  gm_mac(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
  }
  { // $0A40: asl      b #>$40,a
  GEN_TRC(0x0A40);
  const int64_t _v707 = g.B;
  g.fc = (int)((_v707 >> 55) & 1);
  g.B = sext56(_v707 << 1);
  gm_flags56(g, g.B);
  g.A = sext56(((int64_t)s24((uint32_t)(0x000040u))) << 24);
  }
  { // $0A42: move     y:>$c2,a0
  GEN_TRC(0x0A42);
  const uint32_t _v708 = M.rd('y', 0x0000c2u);
  g.A = (g.A & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v708) & M24) << 0);
  }
  { // $0A44: move     b1,y1
  GEN_TRC(0x0A44);
  g.y1 = (uint32_t)((g.B >> 24) & M24);
  }
  { // $0A45: move     b0,y0
  GEN_TRC(0x0A45);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0A46: asr      b #>$7fffff,x1
  GEN_TRC(0x0A46);
  const int64_t _v709 = g.B;
  g.fc = (int)((_v709 >> 0) & 1);
  g.B = sext56(_v709 >> 1);
  gm_flags56(g, g.B);
  g.x1 = (uint32_t)(0x7FFFFFu);
  }
  { // $0A48: move     #$22,n0
  GEN_TRC(0x0A48);
  g.n[0] = (uint32_t)(0x000022u);
  }
  { // $0A49: move     n0,n1
  GEN_TRC(0x0A49);
  g.n[1] = (uint32_t)(g.n[0]);
  }
  { // $0A4A: move     b0,x0
  GEN_TRC(0x0A4A);
  g.x0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0A4B: move     a0,b
  GEN_TRC(0x0A4B);
  g.B = (g.B & ~(int64_t)0xFFFFFF000000ll) | ((int64_t)(g.A & M24) << 24);
  }
  { // $0A4C: lsr      b a1,r0
  GEN_TRC(0x0A4C);
  const uint32_t _v710 = acc_to24(g.A);
  const uint32_t _v711 = acc_to24(g.B);
  const int64_t _v712 = g.A;
  const int64_t _v713 = g.B;
  const int64_t _v714 = g.B;
  g.fc = (int)((_v714 >> 0) & 1);
  g.B = (int64_t)(((uint64_t)_v714 & 0xFFFFFFFFFFFFFFll) >> 1);
  gm_flags56(g, g.B);
  g.r[0] = (uint32_t)(((_v712 >> 24) & M24));
  }
  { // $0A4D: move     #$e0,r1
  GEN_TRC(0x0A4D);
  g.r[1] = (uint32_t)(0x0000E0u);
  }
  _do0a4e = 17; // $0A4E: do
  Ldo0a4e_top:;
    { // $0A50: add      y,a a,l:(r1)
    GEN_TRC(0x0A50);
    const uint32_t _v715 = acc_to24(g.A);
    const uint32_t _v716 = acc_to24(g.B);
    const int64_t _v717 = g.A;
    const int64_t _v718 = g.B;
    g.A = acc_add56(g, g.A, sext56(((int64_t)g.y1 << 24) | g.y0));
    const uint32_t _v719 = g.r[1];
    M.wrL(_v719, (uint64_t)_v717 & 0xFFFFFFFFFFll);
    }
    { // $0A51: add      x0,b b1,y:(r1)+
    GEN_TRC(0x0A51);
    const uint32_t _v720 = acc_to24(g.A);
    const uint32_t _v721 = acc_to24(g.B);
    const int64_t _v722 = g.A;
    const int64_t _v723 = g.B;
    g.B = acc_add56(g, g.B, (int64_t)s24(g.x0) << 24);
    const uint32_t _v724 = g.r[1];
    M.wr('y', _v724, (uint32_t)(((_v723 >> 24) & M24)));
    g.pstep(1,1);
    }
    { // $0A52: and      x1,b
    GEN_TRC(0x0A52);
    gm_logic(g, 'a', "b", s24(g.x1));
    }
  if (--_do0a4e > 0) goto Ldo0a4e_top; // конец do
    { // $0A53: move     #$2f,r2
    GEN_TRC(0x0A53);
    g.r[2] = (uint32_t)(0x00002Fu);
    }
  { // $0A54: move     r2,r3
  GEN_TRC(0x0A54);
  g.r[3] = (uint32_t)(g.r[2]);
  }
  { // $0A55: move     y:>$e0,y0
  GEN_TRC(0x0A55);
  const uint32_t _v725 = M.rd('y', 0x0000e0u);
  g.y0 = (uint32_t)(_v725 & M24);
  }
  { // $0A57: move     #$e1,r4
  GEN_TRC(0x0A57);
  g.r[4] = (uint32_t)(0x0000E1u);
  }
  { // $0A58: move     x:(r0)+,x1
  GEN_TRC(0x0A58);
  const uint32_t _v726 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v726 & M24);
  g.pstep(0,1);
  }
  { // $0A59: move     #$f,m2
  GEN_TRC(0x0A59);
  g.m[2] = (uint32_t)(0x00000Fu);
  }
  { // $0A5A: jsr      func_000397
  GEN_TRC(0x0A5A);
    func_000397(M, rom, g);
  }
  { // $0A5B: move     #>$ffffff,m2
  GEN_TRC(0x0A5B);
  g.m[2] = (uint32_t)(0xFFFFFFu);
  }
  CHK(0x0A5D);
  { // $0A5D: move     y:(r6+$16),b
  GEN_TRC(0x0A5D);
  const uint32_t _v727 = M.rd('y', (g.r[6] + 0x16u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v727))) << 24);
  }
  { // $0A5E: asr      #$10,b,b
  GEN_TRC(0x0A5E);
  const int64_t _v728 = g.B;
  g.fc = (int)((_v728 >> 15) & 1);
  g.B = sext56(_v728 >> 16);
  gm_flags56(g, g.B);
  }
  { // $0A5F: move     #>$144ac7,r3
  GEN_TRC(0x0A5F);
  g.r[3] = (uint32_t)(0x144AC7u);
  }
  { // $0A61: move     b,n3
  GEN_TRC(0x0A61);
  g.n[3] = (int32_t)s24(acc_to24n(g.B));
  }
  { // $0A62: move     #$20,r1
  GEN_TRC(0x0A62);
  g.r[1] = (uint32_t)(0x000020u);
  }
  { // $0A63: move     x:(r3+n3),b
  GEN_TRC(0x0A63);
  const uint32_t _v729 = M.rd('x', (g.r[3] + g.n[3]) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v729))) << 24);
  }
  { // $0A64: asr      b b,y0
  GEN_TRC(0x0A64);
  const uint32_t _v730 = acc_to24(g.A);
  const uint32_t _v731 = acc_to24(g.B);
  const int64_t _v732 = g.A;
  const int64_t _v733 = g.B;
  const int64_t _v734 = g.B;
  g.fc = (int)((_v734 >> 0) & 1);
  g.B = sext56(_v734 >> 1);
  gm_flags56(g, g.B);
  g.y0 = _v731;
  }
  { // $0A65: add      #>$800000,b
  GEN_TRC(0x0A65);
  g.B = acc_add56(g, g.B, (int64_t)s24(0x800000u) << 24);
  }
  { // $0A67: move     #$62,r4
  GEN_TRC(0x0A67);
  g.r[4] = (uint32_t)(0x000062u);
  }
  { // $0A68: neg      b #$30,r0
  GEN_TRC(0x0A68);
  g.B = sat56(-g.B);
  gm_flags56(g, g.B);
  g.r[0] = (uint32_t)(0x000030u);
  }
  { // $0A69: move     y:(r7-$88),a
  GEN_TRC(0x0A69);
  const uint32_t _v735 = M.rd('y', (g.r[7] - 0x88u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v735))) << 24);
  }
  { // $0A6B: move     y:(r7-$89),x0
  GEN_TRC(0x0A6B);
  const uint32_t _v736 = M.rd('y', (g.r[7] - 0x89u) & M24);
  g.x0 = (uint32_t)(_v736 & M24);
  }
  { // $0A6D: move     x:(r7-$89),x1
  GEN_TRC(0x0A6D);
  const uint32_t _v737 = M.rd('x', (g.r[7] - 0x89u) & M24);
  g.x1 = (uint32_t)(_v737 & M24);
  }
  { // $0A6F: move     x:(r1+$1f),y1
  GEN_TRC(0x0A6F);
  const uint32_t _v738 = M.rd('x', (g.r[1] + 0x1Fu) & M24);
  g.y1 = (uint32_t)(_v738 & M24);
  }
  { // $0A70: move     y1,y:(r7-$89)
  GEN_TRC(0x0A70);
  const uint32_t _v739 = (g.r[7] - 0x89u) & M24;
  M.wr('y', _v739, (uint32_t)(g.y1 & M24));
  }
  { // $0A72: move     x:(r1+$f),y1
  GEN_TRC(0x0A72);
  const uint32_t _v740 = M.rd('x', (g.r[1] + 0xFu) & M24);
  g.y1 = (uint32_t)(_v740 & M24);
  }
  { // $0A73: move     y1,x:(r7-$89)
  GEN_TRC(0x0A73);
  const uint32_t _v741 = (g.r[7] - 0x89u) & M24;
  M.wr('x', _v741, (uint32_t)(g.y1 & M24));
  }
  { // $0A75: move     b,y1
  GEN_TRC(0x0A75);
  g.y1 = acc_to24(g.B);
  }
  { // $0A76: move     x:(r7-$88),b
  GEN_TRC(0x0A76);
  const uint32_t _v742 = M.rd('x', (g.r[7] - 0x88u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v742))) << 24);
  }
  _do0a78 = 16; // $0A78: do
  Ldo0a78_top:;
    { // $0A7A: mac      -x0,y1,a a,x0 a,y:(r4)+
    GEN_TRC(0x0A7A);
    const uint32_t _v743 = acc_to24(g.A);
    const uint32_t _v744 = acc_to24(g.B);
    const int64_t _v745 = g.A;
    const int64_t _v746 = g.B;
    gm_mac(g, g.A, -(int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = _v743;
    const uint32_t _v747 = g.r[4];
    M.wr('y', _v747, _v743);
    g.pstep(4,1);
    }
    { // $0A7B: mac      -y1,x1,b b,x1 b,y:(r4)+
    GEN_TRC(0x0A7B);
    const uint32_t _v748 = acc_to24(g.A);
    const uint32_t _v749 = acc_to24(g.B);
    const int64_t _v750 = g.A;
    const int64_t _v751 = g.B;
    gm_mac(g, g.B, -(int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x1 = _v749;
    const uint32_t _v752 = g.r[4];
    M.wr('y', _v752, _v749);
    g.pstep(4,1);
    }
    { // $0A7C: mac      -y0,x0,a x:(r0),x0
    GEN_TRC(0x0A7C);
    const uint32_t _v753 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x0 = (uint32_t)(_v753 & M24);
    }
    { // $0A7D: mac      -x1,y0,b x:(r1),x1
    GEN_TRC(0x0A7D);
    const uint32_t _v754 = M.rd('x', g.r[1]);
    gm_mac(g, g.B, -(int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x1 = (uint32_t)(_v754 & M24);
    }
    { // $0A7E: mac      x0,y1,a x:(r0)+,x0
    GEN_TRC(0x0A7E);
    const uint32_t _v755 = M.rd('x', g.r[0]);
    gm_mac(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    g.x0 = (uint32_t)(_v755 & M24);
    g.pstep(0,1);
    }
    { // $0A7F: mac      y1,x1,b x:(r1)+,x1
    GEN_TRC(0x0A7F);
    const uint32_t _v756 = M.rd('x', g.r[1]);
    gm_mac(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x1 = (uint32_t)(_v756 & M24);
    g.pstep(1,1);
    }
  if (--_do0a78 > 0) goto Ldo0a78_top; // конец do
    { // $0A80: move     a,y:(r7-$88)
    GEN_TRC(0x0A80);
    const uint32_t _v757 = (g.r[7] - 0x88u) & M24;
    M.wr('y', _v757, acc_to24(g.A));
    }
  { // $0A82: move     b,x:(r7-$88)
  GEN_TRC(0x0A82);
  const uint32_t _v758 = (g.r[7] - 0x88u) & M24;
  M.wr('x', _v758, acc_to24(g.B));
  }
  { // $0A84: move     y:(r6+$17),a
  GEN_TRC(0x0A84);
  const uint32_t _v759 = M.rd('y', (g.r[6] + 0x17u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v759))) << 24);
  }
  { // $0A85: move     y:(r6+$16),x0
  GEN_TRC(0x0A85);
  const uint32_t _v760 = M.rd('y', (g.r[6] + 0x16u) & M24);
  g.x0 = (uint32_t)(_v760 & M24);
  }
  { // $0A86: add      x0,a #>$144ac7,r2
  GEN_TRC(0x0A86);
  g.A = acc_add56(g, g.A, (int64_t)s24(g.x0) << 24);
  g.r[2] = (uint32_t)(0x144AC7u);
  }
  { // $0A88: move     #$20,r4
  GEN_TRC(0x0A88);
  g.r[4] = (uint32_t)(0x000020u);
  }
  { // $0A89: move     #$62,r5
  GEN_TRC(0x0A89);
  g.r[5] = (uint32_t)(0x000062u);
  }
  { // $0A8A: move     a,b
  GEN_TRC(0x0A8A);
  g.B = g.A;
  }
  { // $0A8B: asr      #$10,b,b
  GEN_TRC(0x0A8B);
  const int64_t _v761 = g.B;
  g.fc = (int)((_v761 >> 15) & 1);
  g.B = sext56(_v761 >> 16);
  gm_flags56(g, g.B);
  }
  { // $0A8C: move     y:(r7-$87),a
  GEN_TRC(0x0A8C);
  const uint32_t _v762 = M.rd('y', (g.r[7] - 0x87u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v762))) << 24);
  }
  { // $0A8E: move     b,n2
  GEN_TRC(0x0A8E);
  g.n[2] = (int32_t)s24(acc_to24n(g.B));
  }
  { // $0A8F: move     x:(r7-$87),b
  GEN_TRC(0x0A8F);
  const uint32_t _v763 = M.rd('x', (g.r[7] - 0x87u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v763))) << 24);
  }
  { // $0A91: move     y:(r5)+,x1
  GEN_TRC(0x0A91);
  const uint32_t _v764 = M.rd('y', g.r[5]);
  g.x1 = (uint32_t)(_v764 & M24);
  g.pstep(5,1);
  }
  { // $0A92: move     x:(r2+n2),y0
  GEN_TRC(0x0A92);
  const uint32_t _v765 = M.rd('x', (g.r[2] + g.n[2]) & M24);
  g.y0 = (uint32_t)(_v765 & M24);
  }
  _do0a93 = 16; // $0A93: do
  Ldo0a93_top:;
    { // $0A95: mac      x1,y0,a a,x0 a,y:(r4)+
    GEN_TRC(0x0A95);
    const uint32_t _v766 = acc_to24(g.A);
    const uint32_t _v767 = acc_to24(g.B);
    const int64_t _v768 = g.A;
    const int64_t _v769 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x0 = _v766;
    const uint32_t _v770 = g.r[4];
    M.wr('y', _v770, _v766);
    g.pstep(4,1);
    }
    { // $0A96: mac      -y0,x0,a y:(r5)+,x1
    GEN_TRC(0x0A96);
    const uint32_t _v771 = M.rd('y', g.r[5]);
    gm_mac(g, g.A, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x1 = (uint32_t)(_v771 & M24);
    g.pstep(5,1);
    }
    { // $0A97: mac      x1,y0,b b,x0 b,y:(r4)+
    GEN_TRC(0x0A97);
    const uint32_t _v772 = acc_to24(g.A);
    const uint32_t _v773 = acc_to24(g.B);
    const int64_t _v774 = g.A;
    const int64_t _v775 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x0 = _v773;
    const uint32_t _v776 = g.r[4];
    M.wr('y', _v776, _v773);
    g.pstep(4,1);
    }
    { // $0A98: mac      -y0,x0,b y:(r5)+,x1
    GEN_TRC(0x0A98);
    const uint32_t _v777 = M.rd('y', g.r[5]);
    gm_mac(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x1 = (uint32_t)(_v777 & M24);
    g.pstep(5,1);
    }
  if (--_do0a93 > 0) goto Ldo0a93_top; // конец do
    { // $0A99: move     a,y:(r7-$87)
    GEN_TRC(0x0A99);
    const uint32_t _v778 = (g.r[7] - 0x87u) & M24;
    M.wr('y', _v778, acc_to24(g.A));
    }
  { // $0A9B: move     b,x:(r7-$87)
  GEN_TRC(0x0A9B);
  const uint32_t _v779 = (g.r[7] - 0x87u) & M24;
  M.wr('x', _v779, acc_to24(g.B));
  }
  { // $0A9D: move     #$62,r4
  GEN_TRC(0x0A9D);
  g.r[4] = (uint32_t)(0x000062u);
  }
  { // $0A9E: move     #$0,r1
  GEN_TRC(0x0A9E);
  g.r[1] = (uint32_t)(0x000000u);
  }
  { // $0A9F: move     x:(r7-$8d),b
  GEN_TRC(0x0A9F);
  const uint32_t _v780 = M.rd('x', (g.r[7] - 0x8Du) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v780))) << 24);
  }
  { // $0AA1: move     y:(r7-$8d),b0
  GEN_TRC(0x0AA1);
  const uint32_t _v781 = M.rd('y', (g.r[7] - 0x8Du) & M24);
  g.B = (g.B & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v781) & M24) << 0);
  }
  { // $0AA3: asr      #$11,b,b
  GEN_TRC(0x0AA3);
  const int64_t _v782 = g.B;
  g.fc = (int)((_v782 >> 16) & 1);
  g.B = sext56(_v782 >> 17);
  gm_flags56(g, g.B);
  }
  { // $0AA4: move     y:(r7-$90),a
  GEN_TRC(0x0AA4);
  const uint32_t _v783 = M.rd('y', (g.r[7] - 0x90u) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v783))) << 24);
  }
  { // $0AA6: move     b0,r2
  GEN_TRC(0x0AA6);
  g.r[2] = (int32_t)s24(acc_to24n(g.B));
  }
  { // $0AA7: move     x:(r7-$90),b
  GEN_TRC(0x0AA7);
  const uint32_t _v784 = M.rd('x', (g.r[7] - 0x90u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v784))) << 24);
  }
  { // $0AA9: move     x:(r2+$144b48),y0
  GEN_TRC(0x0AA9);
  const uint32_t _v785 = rom.rd('x', (g.r[2] + 0x144B48u) & M24);
  g.y0 = (uint32_t)(_v785 & M24);
  }
  { // $0AAB: move     #$10,r3
  GEN_TRC(0x0AAB);
  g.r[3] = (uint32_t)(0x000010u);
  }
  { // $0AAC: move     x:(r3)+,x1
  GEN_TRC(0x0AAC);
  const uint32_t _v786 = M.rd('x', g.r[3]);
  g.x1 = (uint32_t)(_v786 & M24);
  g.pstep(3,1);
  }
  _do0aad = 16; // $0AAD: do
  Ldo0aad_top:;
    { // $0AAF: mac      x1,y0,a a,x0 a,y:(r4)+
    GEN_TRC(0x0AAF);
    const uint32_t _v787 = acc_to24(g.A);
    const uint32_t _v788 = acc_to24(g.B);
    const int64_t _v789 = g.A;
    const int64_t _v790 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x0 = _v787;
    const uint32_t _v791 = g.r[4];
    M.wr('y', _v791, _v787);
    g.pstep(4,1);
    }
    { // $0AB0: mac      -y0,x0,a x:(r1)+,x1
    GEN_TRC(0x0AB0);
    const uint32_t _v792 = M.rd('x', g.r[1]);
    gm_mac(g, g.A, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x1 = (uint32_t)(_v792 & M24);
    g.pstep(1,1);
    }
    { // $0AB1: mac      x1,y0,b b,x0 b,y:(r4)+
    GEN_TRC(0x0AB1);
    const uint32_t _v793 = acc_to24(g.A);
    const uint32_t _v794 = acc_to24(g.B);
    const int64_t _v795 = g.A;
    const int64_t _v796 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.y0), true);
    g.x0 = _v794;
    const uint32_t _v797 = g.r[4];
    M.wr('y', _v797, _v794);
    g.pstep(4,1);
    }
    { // $0AB2: mac      -y0,x0,b x:(r3)+,x1
    GEN_TRC(0x0AB2);
    const uint32_t _v798 = M.rd('x', g.r[3]);
    gm_mac(g, g.B, -(int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x1 = (uint32_t)(_v798 & M24);
    g.pstep(3,1);
    }
  if (--_do0aad > 0) goto Ldo0aad_top; // конец do
    { // $0AB3: move     a,y:(r7-$90)
    GEN_TRC(0x0AB3);
    const uint32_t _v799 = (g.r[7] - 0x90u) & M24;
    M.wr('y', _v799, acc_to24(g.A));
    }
  { // $0AB5: move     b,x:(r7-$90)
  GEN_TRC(0x0AB5);
  const uint32_t _v800 = (g.r[7] - 0x90u) & M24;
  M.wr('x', _v800, acc_to24(g.B));
  }
  { // $0AB7: move     y:(r6+$14),b
  GEN_TRC(0x0AB7);
  const uint32_t _v801 = M.rd('y', (g.r[6] + 0x14u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v801))) << 24);
  }
  { // $0AB8: sub      #>$400000,b
  GEN_TRC(0x0AB8);
  g.B = acc_sub56(g, g.B, (int64_t)s24(0x400000u) << 24);
  }
  { // $0ABA: abs      b #$62,r4
  GEN_TRC(0x0ABA);
  g.B = (g.B < 0) ? sat56(-g.B) : g.B;
  gm_flags56(g, g.B);
  g.r[4] = (uint32_t)(0x000062u);
  }
  { // $0ABB: asl      b #$20,r1
  GEN_TRC(0x0ABB);
  const int64_t _v802 = g.B;
  g.fc = (int)((_v802 >> 55) & 1);
  g.B = sext56(_v802 << 1);
  gm_flags56(g, g.B);
  g.r[1] = (uint32_t)(0x000020u);
  }
  { // $0ABC: move     #$51,r2
  GEN_TRC(0x0ABC);
  g.r[2] = (uint32_t)(0x000051u);
  }
  { // $0ABD: move     b,x0
  GEN_TRC(0x0ABD);
  g.x0 = acc_to24(g.B);
  }
  { // $0ABE: mpy      x0,x0,a #$40,r3
  GEN_TRC(0x0ABE);
  gm_mpy(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.x0));
  g.r[3] = (uint32_t)(0x000040u);
  }
  { // $0ABF: move     y:(r6+$15),y0
  GEN_TRC(0x0ABF);
  const uint32_t _v803 = M.rd('y', (g.r[6] + 0x15u) & M24);
  g.y0 = (uint32_t)(_v803 & M24);
  }
  { // $0AC0: move     x:(r7-$84),b
  GEN_TRC(0x0AC0);
  const uint32_t _v804 = M.rd('x', (g.r[7] - 0x84u) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v804))) << 24);
  }
  { // $0AC2: cmp      #<$4,b
  GEN_TRC(0x0AC2);
  gm_cmp(g, g.B, (int64_t)s24(0x000004u) << 24);
  }
  { // $0AC3: bne      func_000ac5
  GEN_TRC(0x0AC3);
    if (!g.fz) goto L_000ac5;
  }
  { // $0AC4: move     #$0,y0
  GEN_TRC(0x0AC4);
  g.y0 = (uint32_t)(0x000000u);
  }
L_000ac5:;
  { // $0AC5: move     a,x1
  GEN_TRC(0x0AC5);
  g.x1 = acc_to24(g.A);
  }
  { // $0AC6: move     y:(r1)+,x0
  GEN_TRC(0x0AC6);
  const uint32_t _v805 = M.rd('y', g.r[1]);
  g.x0 = (uint32_t)(_v805 & M24);
  g.pstep(1,1);
  }
  _do0ac7 = 16; // $0AC7: do
  Ldo0ac7_top:;
    { // $0AC9: mpy      y0,x0,a a,x:(r2)+ y:(r4)+,y1
    GEN_TRC(0x0AC9);
    const uint32_t _v806 = M.rd('y', g.r[4]);
    const uint32_t _v807 = acc_to24(g.A);
    const uint32_t _v808 = acc_to24(g.B);
    const int64_t _v809 = g.A;
    const int64_t _v810 = g.B;
    gm_mpy(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
    const uint32_t _v811 = g.r[2];
    M.wr('x', _v811, _v807);
    g.pstep(2,1);
    g.y1 = (uint32_t)(_v806 & M24);
    g.pstep(4,1);
    }
    { // $0ACA: asl      #$1,a,a
    GEN_TRC(0x0ACA);
    const int64_t _v812 = g.A;
    g.fc = (int)((_v812 >> 55) & 1);
    g.A = sext56(_v812 << 1);
    gm_flags56(g, g.A);
    }
    { // $0ACB: mac      y1,x1,a y:(r1)+,x0
    GEN_TRC(0x0ACB);
    const uint32_t _v813 = M.rd('y', g.r[1]);
    gm_mac(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x0 = (uint32_t)(_v813 & M24);
    g.pstep(1,1);
    }
    { // $0ACC: mpy      y0,x0,b b,x:(r3)+ y:(r4)+,y1
    GEN_TRC(0x0ACC);
    const uint32_t _v814 = M.rd('y', g.r[4]);
    const uint32_t _v815 = acc_to24(g.A);
    const uint32_t _v816 = acc_to24(g.B);
    const int64_t _v817 = g.A;
    const int64_t _v818 = g.B;
    gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0));
    const uint32_t _v819 = g.r[3];
    M.wr('x', _v819, _v816);
    g.pstep(3,1);
    g.y1 = (uint32_t)(_v814 & M24);
    g.pstep(4,1);
    }
    { // $0ACD: asl      #$1,b,b
    GEN_TRC(0x0ACD);
    const int64_t _v820 = g.B;
    g.fc = (int)((_v820 >> 55) & 1);
    g.B = sext56(_v820 << 1);
    gm_flags56(g, g.B);
    }
    { // $0ACE: mac      y1,x1,b y:(r1)+,x0
    GEN_TRC(0x0ACE);
    const uint32_t _v821 = M.rd('y', g.r[1]);
    gm_mac(g, g.B, (int64_t)s24(g.y1), (int64_t)s24(g.x1), true);
    g.x0 = (uint32_t)(_v821 & M24);
    g.pstep(1,1);
    }
  if (--_do0ac7 > 0) goto Ldo0ac7_top; // конец do
    { // $0ACF: move     a,x:(r2)+
    GEN_TRC(0x0ACF);
    const uint32_t _v822 = g.r[2];
    M.wr('x', _v822, acc_to24(g.A));
    g.pstep(2,1);
    }
  { // $0AD0: move     b,x:(r3)+
  GEN_TRC(0x0AD0);
  const uint32_t _v823 = g.r[3];
  M.wr('x', _v823, acc_to24(g.B));
  g.pstep(3,1);
  }
  CHK(0x0AD1);
  { // $0AD1: move     x:(r7-$8d),b
  GEN_TRC(0x0AD1);
  const uint32_t _v824 = M.rd('x', (g.r[7] - 0x8Du) & M24);
  g.B = sext56(((int64_t)s24((uint32_t)(_v824))) << 24);
  }
  { // $0AD3: move     y:(r7-$8d),b0
  GEN_TRC(0x0AD3);
  const uint32_t _v825 = M.rd('y', (g.r[7] - 0x8Du) & M24);
  g.B = (g.B & ~(((int64_t)M24) << 0)) | ((int64_t)((uint32_t)(_v825) & M24) << 0);
  }
  { // $0AD5: asr      #$2,b,b
  GEN_TRC(0x0AD5);
  const int64_t _v826 = g.B;
  g.fc = (int)((_v826 >> 1) & 1);
  g.B = sext56(_v826 >> 2);
  gm_flags56(g, g.B);
  }
  { // $0AD6: move     #$1,a
  GEN_TRC(0x0AD6);
  g.A = sext56(((int64_t)s24((uint32_t)(0x000001u))) << 24);
  }
  { // $0AD7: move     b0,y0
  GEN_TRC(0x0AD7);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0AD8: andi     #$fe,ccr
  GEN_TRC(0x0AD8);
    if ((0xFE & 1) == 0) g.fc = 0; // andi ccr
  }
  _do0ad9 = 24; // $0AD9: do
  Ldo0ad9_top:;
    { // $0ADB: div      y0,a
    GEN_TRC(0x0ADB);
      div_step(g, g.A, g.y0);
    }
  if (--_do0ad9 > 0) goto Ldo0ad9_top; // конец do
    { // $0ADC: move     a0,b
    GEN_TRC(0x0ADC);
    g.B = (g.B & ~(int64_t)0xFFFFFF000000ll) | ((int64_t)(g.A & M24) << 24);
    }
  { // $0ADD: asr      #$11,b,b
  GEN_TRC(0x0ADD);
  const int64_t _v827 = g.B;
  g.fc = (int)((_v827 >> 16) & 1);
  g.B = sext56(_v827 >> 17);
  gm_flags56(g, g.B);
  }
  { // $0ADE: move     y:>$c4,x0
  GEN_TRC(0x0ADE);
  const uint32_t _v828 = M.rd('y', 0x0000c4u);
  g.x0 = (uint32_t)(_v828 & M24);
  }
  { // $0AE0: move     b1,y1
  GEN_TRC(0x0AE0);
  g.y1 = (uint32_t)((g.B >> 24) & M24);
  }
  { // $0AE1: move     b0,y0
  GEN_TRC(0x0AE1);
  g.y0 = (uint32_t)((g.B >> 0) & M24);
  }
  { // $0AE2: mpyuu    y0,x0,a
  GEN_TRC(0x0AE2);
  gm_macuu(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), false);
  }
  { // $0AE3: dmac     su y1,x0,a
  GEN_TRC(0x0AE3);
  gm_mac(g, g.A, (int64_t)s24(g.y1), (int64_t)s24(g.x0), true);
  }
  { // $0AE4: asr      a
  GEN_TRC(0x0AE4);
  const int64_t _v829 = g.A;
  g.fc = (int)((_v829 >> 0) & 1);
  g.A = sext56(_v829 >> 1);
  gm_flags56(g, g.A);
  }
  { // $0AE5: clr      b               ifeq
  GEN_TRC(0x0AE5);
  if (g.fz) {
    g.B = 0;
    gm_flags56(g, g.B);
  }
  }
  { // $0AE6: sub      a,b
  GEN_TRC(0x0AE6);
  g.B = acc_sub56(g, g.B, g.A);
  }
  { // $0AE7: tfr      b,a
  GEN_TRC(0x0AE7);
  g.A = g.B;
  }
  { // $0AE8: cmp      #<$10,b
  GEN_TRC(0x0AE8);
  gm_cmp(g, g.B, (int64_t)s24(0x000010u) << 24);
  }
  { // $0AE9: bge      func_000b1e
  GEN_TRC(0x0AE9);
    if ((g.fn==g.fv)) goto L_000b1e;
  }
  { // $0AEA: move     #>$3fff,m2
  GEN_TRC(0x0AEA);
  g.m[2] = (uint32_t)(0x003FFFu);
  }
  { // $0AEC: add      #>$40,a
  GEN_TRC(0x0AEC);
  g.A = acc_add56(g, g.A, (int64_t)s24(0x000040u) << 24);
  }
  { // $0AEE: move     #$90,r1
  GEN_TRC(0x0AEE);
  g.r[1] = (uint32_t)(0x000090u);
  }
  { // $0AEF: tfr      y0,b x:>$c4,r2
  GEN_TRC(0x0AEF);
  const uint32_t _v830 = M.rd('x', 0x0000c4u);
  g.B = (int64_t)s24(g.y0) << 24;
  g.r[2] = (uint32_t)(_v830);
  }
  { // $0AF1: asr      b #>$7fffff,x1
  GEN_TRC(0x0AF1);
  const int64_t _v831 = g.B;
  g.fc = (int)((_v831 >> 0) & 1);
  g.B = sext56(_v831 >> 1);
  gm_flags56(g, g.B);
  g.x1 = (uint32_t)(0x7FFFFFu);
  }
  { // $0AF3: move     b1,x0
  GEN_TRC(0x0AF3);
  g.x0 = (uint32_t)((g.B >> 24) & M24);
  }
  { // $0AF4: move     a0,b
  GEN_TRC(0x0AF4);
  g.B = (g.B & ~(int64_t)0xFFFFFF000000ll) | ((int64_t)(g.A & M24) << 24);
  }
  { // $0AF5: lsr      b a1,r0
  GEN_TRC(0x0AF5);
  const uint32_t _v832 = acc_to24(g.A);
  const uint32_t _v833 = acc_to24(g.B);
  const int64_t _v834 = g.A;
  const int64_t _v835 = g.B;
  const int64_t _v836 = g.B;
  g.fc = (int)((_v836 >> 0) & 1);
  g.B = (int64_t)(((uint64_t)_v836 & 0xFFFFFFFFFFFFFFll) >> 1);
  gm_flags56(g, g.B);
  g.r[0] = (uint32_t)(((_v834 >> 24) & M24));
  }
  _do0af6 = 17; // $0AF6: do
  Ldo0af6_top:;
    { // $0AF8: add      y,a a,l:(r1)
    GEN_TRC(0x0AF8);
    const uint32_t _v837 = acc_to24(g.A);
    const uint32_t _v838 = acc_to24(g.B);
    const int64_t _v839 = g.A;
    const int64_t _v840 = g.B;
    g.A = acc_add56(g, g.A, sext56(((int64_t)g.y1 << 24) | g.y0));
    const uint32_t _v841 = g.r[1];
    M.wrL(_v841, (uint64_t)_v839 & 0xFFFFFFFFFFll);
    }
    { // $0AF9: add      x0,b b1,y:(r1)+
    GEN_TRC(0x0AF9);
    const uint32_t _v842 = acc_to24(g.A);
    const uint32_t _v843 = acc_to24(g.B);
    const int64_t _v844 = g.A;
    const int64_t _v845 = g.B;
    g.B = acc_add56(g, g.B, (int64_t)s24(g.x0) << 24);
    const uint32_t _v846 = g.r[1];
    M.wr('y', _v846, (uint32_t)(((_v845 >> 24) & M24)));
    g.pstep(1,1);
    }
    { // $0AFA: and      x1,b
    GEN_TRC(0x0AFA);
    gm_logic(g, 'a', "b", s24(g.x1));
    }
  if (--_do0af6 > 0) goto Ldo0af6_top; // конец do
    { // $0AFB: move     y:(r7-$91),x0
    GEN_TRC(0x0AFB);
    const uint32_t _v847 = M.rd('y', (g.r[7] - 0x91u) & M24);
    g.x0 = (uint32_t)(_v847 & M24);
    }
  { // $0AFD: move     x0,x:>$40
  GEN_TRC(0x0AFD);
  const uint32_t _v848 = 0x000040u;
  M.wr('x', _v848, (uint32_t)(g.x0 & M24));
  }
  { // $0AFF: move     x:(r7-$91),x0
  GEN_TRC(0x0AFF);
  const uint32_t _v849 = M.rd('x', (g.r[7] - 0x91u) & M24);
  g.x0 = (uint32_t)(_v849 & M24);
  }
  { // $0B01: move     x0,x:>$51
  GEN_TRC(0x0B01);
  const uint32_t _v850 = 0x000051u;
  M.wr('x', _v850, (uint32_t)(g.x0 & M24));
  }
  { // $0B03: move     x:>$50,x0
  GEN_TRC(0x0B03);
  const uint32_t _v851 = M.rd('x', 0x000050u);
  g.x0 = (uint32_t)(_v851 & M24);
  }
  { // $0B05: move     x0,y:(r7-$91)
  GEN_TRC(0x0B05);
  const uint32_t _v852 = (g.r[7] - 0x91u) & M24;
  M.wr('y', _v852, (uint32_t)(g.x0 & M24));
  }
  { // $0B07: move     x:>$61,x0
  GEN_TRC(0x0B07);
  const uint32_t _v853 = M.rd('x', 0x000061u);
  g.x0 = (uint32_t)(_v853 & M24);
  }
  { // $0B09: move     x0,x:(r7-$91)
  GEN_TRC(0x0B09);
  const uint32_t _v854 = (g.r[7] - 0x91u) & M24;
  M.wr('x', _v854, (uint32_t)(g.x0 & M24));
  }
  { // $0B0B: move     x:-(r2),a
  GEN_TRC(0x0B0B);
  g.pstep(2,-1);
  const uint32_t _v855 = M.rd('x', (g.r[2] - 1) & M24);
  g.A = sext56(((int64_t)s24((uint32_t)(_v855))) << 24);
  }
  { // $0B0C: move     #$10,n0
  GEN_TRC(0x0B0C);
  g.n[0] = (uint32_t)(0x000010u);
  }
  { // $0B0D: move     n0,n1
  GEN_TRC(0x0B0D);
  g.n[1] = (uint32_t)(g.n[0]);
  }
  { // $0B0E: move     y:>$90,y0
  GEN_TRC(0x0B0E);
  const uint32_t _v856 = M.rd('y', 0x000090u);
  g.y0 = (uint32_t)(_v856 & M24);
  }
  { // $0B10: move     x:(r0)+,x1
  GEN_TRC(0x0B10);
  const uint32_t _v857 = M.rd('x', g.r[0]);
  g.x1 = (uint32_t)(_v857 & M24);
  g.pstep(0,1);
  }
  { // $0B11: move     #$91,r4
  GEN_TRC(0x0B11);
  g.r[4] = (uint32_t)(0x000091u);
  }
  { // $0B12: move     #$70,r3
  GEN_TRC(0x0B12);
  g.r[3] = (uint32_t)(0x000070u);
  }
  { // $0B13: jsr      func_000397
  GEN_TRC(0x0B13);
    func_000397(M, rom, g);
  }
  { // $0B14: move     #$71,r0
  GEN_TRC(0x0B14);
  g.r[0] = (uint32_t)(0x000071u);
  }
  { // $0B15: move     x:>$c5,r4
  GEN_TRC(0x0B15);
  const uint32_t _v858 = M.rd('x', 0x0000c5u);
  g.r[4] = (uint32_t)(_v858);
  }
  { // $0B17: move     m2,m4
  GEN_TRC(0x0B17);
  g.m[4] = (uint32_t)(g.m[2]);
  }
  { // $0B18: move     x:(r0)+,a
  GEN_TRC(0x0B18);
  const uint32_t _v859 = M.rd('x', g.r[0]);
  g.A = sext56(((int64_t)s24((uint32_t)(_v859))) << 24);
  g.pstep(0,1);
  }
  { // $0B19: move     x:(r0)+,b
  GEN_TRC(0x0B19);
  const uint32_t _v860 = M.rd('x', g.r[0]);
  g.B = sext56(((int64_t)s24((uint32_t)(_v860))) << 24);
  g.pstep(0,1);
  }
  _do0b1a = 8; // $0B1A: do
  Ldo0b1a_top:;
    { // $0B1C: move     x:(r0)+,a       a,y:(r4)+
    GEN_TRC(0x0B1C);
    const uint32_t _v861 = M.rd('x', g.r[0]);
    g.A = sext56(((int64_t)s24((uint32_t)(_v861))) << 24);
    const uint32_t _v862 = g.r[4];
    M.wr('y', _v862, acc_to24(g.A));
    g.pstep(4,1);
    g.pstep(0,1);
    }
    { // $0B1D: move     x:(r0)+,b       b,y:(r4)+
    GEN_TRC(0x0B1D);
    const uint32_t _v863 = M.rd('x', g.r[0]);
    g.B = sext56(((int64_t)s24((uint32_t)(_v863))) << 24);
    const uint32_t _v864 = g.r[4];
    M.wr('y', _v864, acc_to24(g.B));
    g.pstep(4,1);
    g.pstep(0,1);
    }
L_000b1e:;
  if (--_do0b1a > 0) goto Ldo0b1a_top; // конец do
    { // $0B1E: move     y:(r6+$1f),x0
    GEN_TRC(0x0B1E);
    const uint32_t _v865 = M.rd('y', (g.r[6] + 0x1Fu) & M24);
    g.x0 = (uint32_t)(_v865 & M24);
    }
  { // $0B1F: mpy      x0,x0,a
  GEN_TRC(0x0B1F);
  gm_mpy(g, g.A, (int64_t)s24(g.x0), (int64_t)s24(g.x0));
  }
  { // $0B20: move     y:>$4ff,y0
  GEN_TRC(0x0B20);
  const uint32_t _v866 = M.rd('y', 0x0004ffu);
  g.y0 = (uint32_t)(_v866 & M24);
  }
  { // $0B22: mpy      y0,y0,b a,x0
  GEN_TRC(0x0B22);
  const uint32_t _v867 = acc_to24(g.A);
  const uint32_t _v868 = acc_to24(g.B);
  const int64_t _v869 = g.A;
  const int64_t _v870 = g.B;
  gm_mpy(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.y0));
  g.x0 = _v867;
  }
  { // $0B23: move     y:(r7-$5d),y1
  GEN_TRC(0x0B23);
  const uint32_t _v871 = M.rd('y', (g.r[7] - 0x5Du) & M24);
  g.y1 = (uint32_t)(_v871 & M24);
  }
  { // $0B25: move     b,x1
  GEN_TRC(0x0B25);
  g.x1 = acc_to24(g.B);
  }
  { // $0B26: mpy      x1,x0,a
  GEN_TRC(0x0B26);
  gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
  }
  { // $0B27: move     #$10,r5
  GEN_TRC(0x0B27);
  g.r[5] = (uint32_t)(0x000010u);
  }
  { // $0B28: move     a,y:(r7-$5d)
  GEN_TRC(0x0B28);
  const uint32_t _v872 = (g.r[7] - 0x5Du) & M24;
  M.wr('y', _v872, acc_to24(g.A));
  }
  { // $0B2A: sub      y1,a
  GEN_TRC(0x0B2A);
  g.A = acc_sub56(g, g.A, (int64_t)s24(g.y1) << 24);
  }
  { // $0B2B: tfr      y1,b #$10,y0
  GEN_TRC(0x0B2B);
  g.B = (int64_t)s24(g.y1) << 24;
  g.y0 = (uint32_t)(0x100000u);
  }
  { // $0B2C: tfr      y1,a a,x0
  GEN_TRC(0x0B2C);
  const uint32_t _v873 = acc_to24(g.A);
  const uint32_t _v874 = acc_to24(g.B);
  const int64_t _v875 = g.A;
  const int64_t _v876 = g.B;
  g.A = (int64_t)s24(g.y1) << 24;
  g.x0 = _v873;
  }
  { // $0B2D: maci     #>$80000,x0,b
  GEN_TRC(0x0B2D);
  gm_mac(g, g.B, (int64_t)s24(0x080000u), (int64_t)s24(g.x0), true);
  }
  _do0b2f = 8; // $0B2F: do
  Ldo0b2f_top:;
    { // $0B31: mac      y0,x0,a a,y:(r5)+
    GEN_TRC(0x0B31);
    const uint32_t _v877 = acc_to24(g.A);
    const uint32_t _v878 = acc_to24(g.B);
    const int64_t _v879 = g.A;
    const int64_t _v880 = g.B;
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v881 = g.r[5];
    M.wr('y', _v881, _v877);
    g.pstep(5,1);
    }
    { // $0B32: mac      y0,x0,b b,y:(r5)+
    GEN_TRC(0x0B32);
    const uint32_t _v882 = acc_to24(g.A);
    const uint32_t _v883 = acc_to24(g.B);
    const int64_t _v884 = g.A;
    const int64_t _v885 = g.B;
    gm_mac(g, g.B, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    const uint32_t _v886 = g.r[5];
    M.wr('y', _v886, _v883);
    g.pstep(5,1);
    }
  if (--_do0b2f > 0) goto Ldo0b2f_top; // конец do
    { // $0B33: move     #$20,r4
    GEN_TRC(0x0B33);
    g.r[4] = (uint32_t)(0x000020u);
    }
  { // $0B34: move     #$10,r5
  GEN_TRC(0x0B34);
  g.r[5] = (uint32_t)(0x000010u);
  }
  { // $0B35: move     #$0,r2
  GEN_TRC(0x0B35);
  g.r[2] = (uint32_t)(0x000000u);
  }
  { // $0B36: move     x:>$2c9,r3
  GEN_TRC(0x0B36);
  const uint32_t _v887 = M.rd('x', 0x0002c9u);
  g.r[3] = (uint32_t)(_v887);
  }
  { // $0B38: move     x:>$ff,r1
  GEN_TRC(0x0B38);
  const uint32_t _v888 = M.rd('x', 0x0000ffu);
  g.r[1] = (uint32_t)(_v888);
  }
  { // $0B3A: move     #>$ffffff,m2
  GEN_TRC(0x0B3A);
  g.m[2] = (uint32_t)(0xFFFFFFu);
  }
  { // $0B3C: move     m2,m4
  GEN_TRC(0x0B3C);
  g.m[4] = (uint32_t)(g.m[2]);
  }
  CHK(0x0B3D);
  { // $0B3D: move     #$0,r6
  GEN_TRC(0x0B3D);
  g.r[6] = (uint32_t)(0x000000u);
  }
  { // $0B3E: move     l:(r5)+,x
  GEN_TRC(0x0B3E);
  const uint64_t _v889 = M.rdL(g.r[5]);
  g.x1 = (uint32_t)((_v889 >> 24) & M24); g.x0 = (uint32_t)(_v889 & M24);
  g.pstep(5,1);
  }
  { // $0B3F: mpy      x1,x0,a x:(r2)+,x1 y:(r4)+,y0
  GEN_TRC(0x0B3F);
  const uint32_t _v890 = M.rd('x', g.r[2]);
  const uint32_t _v891 = M.rd('y', g.r[4]);
  gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
  g.x1 = (uint32_t)(_v890 & M24);
  g.y0 = (uint32_t)(_v891 & M24);
  g.pstep(2,1);
  g.pstep(4,1);
  }
  { // $0B40: mpy      x1,x0,b y:(r4)+,y1
  GEN_TRC(0x0B40);
  const uint32_t _v892 = M.rd('y', g.r[4]);
  gm_mpy(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
  g.y1 = (uint32_t)(_v892 & M24);
  g.pstep(4,1);
  }
  _do0b41 = 16; // $0B41: do
  Ldo0b41_top:;
    { // $0B43: mac      x0,y1,b
    GEN_TRC(0x0B43);
    gm_mac(g, g.B, (int64_t)s24(g.x0), (int64_t)s24(g.y1), true);
    }
    { // $0B44: mac      y0,x0,a l:(r5)+,x
    GEN_TRC(0x0B44);
    const uint64_t _v893 = M.rdL(g.r[5]);
    gm_mac(g, g.A, (int64_t)s24(g.y0), (int64_t)s24(g.x0), true);
    g.x1 = (uint32_t)((_v893 >> 24) & M24); g.x0 = (uint32_t)(_v893 & M24);
    g.pstep(5,1);
    }
    { // $0B45: add      a,b b,x:(r1)+ b,y:(r6)+
    GEN_TRC(0x0B45);
    const uint32_t _v894 = acc_to24(g.A);
    const uint32_t _v895 = acc_to24(g.B);
    const int64_t _v896 = g.A;
    const int64_t _v897 = g.B;
    g.B = acc_add56(g, g.B, g.A);
    const uint32_t _v898 = g.r[1];
    M.wr('x', _v898, _v895);
    g.pstep(1,1);
    const uint32_t _v899 = g.r[6];
    M.wr('y', _v899, _v895);
    g.pstep(6,1);
    }
    { // $0B46: asr      b a,x:(r1)+ a,y:(r6)+
    GEN_TRC(0x0B46);
    const uint32_t _v900 = acc_to24(g.A);
    const uint32_t _v901 = acc_to24(g.B);
    const int64_t _v902 = g.A;
    const int64_t _v903 = g.B;
    const int64_t _v904 = g.B;
    g.fc = (int)((_v904 >> 0) & 1);
    g.B = sext56(_v904 >> 1);
    gm_flags56(g, g.B);
    const uint32_t _v905 = g.r[1];
    M.wr('x', _v905, _v900);
    g.pstep(1,1);
    const uint32_t _v906 = g.r[6];
    M.wr('y', _v906, _v900);
    g.pstep(6,1);
    }
    { // $0B47: mpy      x1,x0,a x:(r2)+,x1 y:(r4)+,y0
    GEN_TRC(0x0B47);
    const uint32_t _v907 = M.rd('x', g.r[2]);
    const uint32_t _v908 = M.rd('y', g.r[4]);
    gm_mpy(g, g.A, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
    g.x1 = (uint32_t)(_v907 & M24);
    g.y0 = (uint32_t)(_v908 & M24);
    g.pstep(2,1);
    g.pstep(4,1);
    }
    { // $0B48: mpy      x1,x0,b b,x:(r3)+ y:(r4)+,y1
    GEN_TRC(0x0B48);
    const uint32_t _v909 = M.rd('y', g.r[4]);
    const uint32_t _v910 = acc_to24(g.A);
    const uint32_t _v911 = acc_to24(g.B);
    const int64_t _v912 = g.A;
    const int64_t _v913 = g.B;
    gm_mpy(g, g.B, (int64_t)s24(g.x1), (int64_t)s24(g.x0));
    const uint32_t _v914 = g.r[3];
    M.wr('x', _v914, _v911);
    g.pstep(3,1);
    g.y1 = (uint32_t)(_v909 & M24);
    g.pstep(4,1);
    }
  if (--_do0b41 > 0) goto Ldo0b41_top; // конец do
    { // $0B49: move     r3,x:>$2c9
    GEN_TRC(0x0B49);
    const uint32_t _v915 = 0x0002c9u;
    M.wr('x', _v915, (uint32_t)(g.r[3] & M24));
    }
  { // $0B4B: pflush
  GEN_TRC(0x0B4B);
    /* pflush: нет видимого эффекта */
  }
}

} // namespace mnmchain
