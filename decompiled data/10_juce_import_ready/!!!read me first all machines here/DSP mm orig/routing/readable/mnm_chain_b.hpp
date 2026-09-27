// ============================================================================
// mnm_chain_b.hpp — секции кадра, часть B: ФИЛЬТР.
//   $0537-$05A0  кольцо коэффициентов (BASE/WDTH/HPQ/LPQ + DIST-драйвер)
//   $0340-$034F  func_000340 — каскад 1 (гибрид FIR+SVF, 16 итераций)
//   $05A1-$05BA  вызов каскада L (+ каскад R при стерео), перестановка состояний
//   $05BB-$05FE  halfband 2× интерполятор ($F528BD/$4A4DF0), L и R
// ============================================================================
#pragma once
#include "mnm_chain_a.hpp"

namespace mnmchain {

// mac с отрицанием операнда 1 (mac -x0,y1,a)
inline void mac_n1(Regs& g, int64_t& dst, uint32_t s1, uint32_t s2, bool accumulate) noexcept {
    const int64_t prod = ((int64_t)-s24(s1) * (int64_t)s24(s2)) << 1;
    const int64_t res = sat56(accumulate ? dst + prod : prod);
    g.fv = (-(1ll << 47) <= res && res < (1ll << 47)) ? 0 : 1;
    dst = res;
    const int64_t r = sext56(res);
    const int a1msb = (int)((r >> 47) & 1);
    const int a2 = (int)((r >> 48) & 0xFF);
    g.fe = (a2 != (a1msb ? 0xFF : 0x00)) ? 1 : 0;
    g.fn = (r < 0); g.fz = (r == 0);
}
inline void mac_std(Regs& g, int64_t& dst, uint32_t s1, uint32_t s2, bool accumulate) noexcept {
    const int64_t prod = ((int64_t)s24(s1) * (int64_t)s24(s2)) << 1;
    const int64_t res = sat56(accumulate ? dst + prod : prod);
    g.fv = (-(1ll << 47) <= res && res < (1ll << 47)) ? 0 : 1;
    dst = res;
    const int64_t r = sext56(res);
    const int a1msb = (int)((r >> 47) & 1);
    const int a2 = (int)((r >> 48) & 0xFF);
    g.fe = (a2 != (a1msb ? 0xFF : 0x00)) ? 1 : 0;
    g.fn = (r < 0); g.fz = (r == 0);
}
// один шаг невосстанавливающего деления DSP56300 (div S,D над A1:A0)
inline void div_step(Regs& g, int64_t& acc, uint32_t s) noexcept {
    uint32_t d1 = (uint32_t)((acc >> 24) & M24);
    uint32_t d0 = (uint32_t)(acc & M24);
    d1 = ((d1 << 1) | (d0 >> 23)) & M24;
    d0 = (d0 << 1) & M24;
    const int32_t ss = s24(s);
    if ((s24(d1) < 0) == (ss < 0)) d1 = (d1 - s) & M24;
    else d1 = (d1 + s) & M24;
    const uint32_t qbit = ((s24(d1) < 0) == (ss < 0)) ? 1u : 0u;
    d0 |= qbit;
    acc = sext56((int64_t)(((uint64_t)d1 << 24) | d0));
    g.fc = qbit ^ 1;
}

// ---------------------------------------------------------------------------
// func_000340 — КАСКАД 1. Вход: r0 = X:$96 (окно истории L), r1 = X:$93
// (состояния), r4 = кольцо коэффициентов Y:$04-$07 (m4 = $3, установлен
// вызывающим). Выход: b (b_old шина), состояния пишутся в X/Y:$93-$A2.
// Параллельные источники читаются ДО ALU-операции (латч).
// ---------------------------------------------------------------------------
inline void func_000340(Mem& M, Regs& g) {
    int64_t& a = g.A;
    int64_t& b = g.B;
    g.x0 = M.rd('x', g.ppost(0));                      // 0340 x:(r0)+,x0
    g.n[1] = 2;                                        // 0341
    {   // 0342: tfr x0,a | x:(r0)+,x0 | y:(r4)+,y0
        a = sext56((int64_t)s24(g.x0) << 24);          // tfr x0,a (латч x0)
        g.x0 = M.rd('x', g.ppost(0));
        g.y0 = M.rd('y', g.ppost(4));
    }
    for (int it = 0; it < 16; ++it) {                  // 0343 do #<$10
        uint32_t x1v, y1v;
        {   // 0345: mac y0,x0,a | x:(r0)-,x1 | y:(r4)+,y1
            mac_std(g, a, g.y0, g.x0, true);
            x1v = M.rd('x', g.ppostdec(0));
            y1v = M.rd('y', g.ppost(4));
        }
        mac_std(g, a, g.y0, g.x0, true);               // 0346 mac y0,x0,a (×2)
        {   // 0347: mac y1,x1,a | x:(r1)+,x0
            mac_std(g, a, y1v, x1v, true);
            g.x0 = M.rd('x', g.ppost(1));
        }
        {   // 0348: mac -x0,y1,a | x:(r1)-,x0
            mac_n1(g, a, g.x0, y1v, true);
            g.x0 = M.rd('x', g.ppostdec(1));
        }
        {   // 0349: mac -y0,x0,a | b,y:(r1)+n1
            mac_n1(g, a, g.y0, g.x0, true);
            M.wr('y', g.pnext(1), acc_to24(b));
        }
        {   // 034A: mac -y0,x0,a | x:(r0)+,x0 | y:(r4)+,y0
            mac_n1(g, a, g.y0, g.x0, true);
            g.x0 = M.rd('x', g.ppost(0));
            g.y0 = M.rd('y', g.ppost(4));
        }
        {   // 034B: mpy x1,y0,b | y:(r4)+,y1
            mac_std(g, b, x1v, g.y0, false);
            y1v = M.rd('y', g.ppost(4));
        }
        {   // 034C: tfr x0,a | a,x:(r1)- | a,y0
            const int64_t aOld = a;
            a = sext56((int64_t)s24(g.x0) << 24);
            const uint32_t aOldW = acc_to24(aOld);
            M.wr('x', g.ppostdec(1), aOldW);
            g.y0 = aOldW;
        }
        {   // 034D: mac y1,y0,b | x:(r0)+,x0 | y:(r4)+,y0
            mac_std(g, b, y1v, g.y0, true);
            g.x0 = M.rd('x', g.ppost(0));
            g.y0 = M.rd('y', g.ppost(4));
        }
        b = sext56(b << 2);                            // 034E asl #$2,b,b
    }                                                  // 034F rts
}

// вспомогательные 56-бит add/sub с флагами (для секции B)
inline int64_t acc_sub56r2(Regs& g, int64_t x, int64_t y) noexcept {
    const int64_t a = x - y;
    const int sx = x < 0, sy = y < 0, sr = sext56(a) < 0;
    g.fv = (sx != sy && sr != sx) ? 1 : 0;
    g.fc = (a < 0) ? 0 : 1;
    g.fn = sr; g.fz = (sext56(a) == 0);
    {   const int64_t r = sext56(a); const int msb = (int)((r >> 47) & 1);
        const int a2 = (int)((r >> 48) & 0xFF);
        g.fe = (a2 != (msb ? 0xFF : 0x00)) ? 1 : 0; }
    return sext56(a);
}
inline int64_t acc_add56(Regs& g, int64_t x, int64_t y) noexcept {
    const int64_t a = x + y;
    const int sx = x < 0, sy = y < 0, sr = sext56(a) < 0;
    g.fv = (sx == sy && sr != sx) ? 1 : 0;
    g.fc = (int)((a >> 55) & 1);
    g.fn = sr; g.fz = (sext56(a) == 0);
    {   const int64_t r = sext56(a); const int msb = (int)((r >> 47) & 1);
        const int a2 = (int)((r >> 48) & 0xFF);
        g.fe = (a2 != (msb ? 0xFF : 0x00)) ? 1 : 0; }
    return sext56(a);
}


// ---------------------------------------------------------------------------
// $0537-$05A0 — КОЛЬЦО КОЭФФИЦИЕНТОВ ФИЛЬТРА.
//   Индексы: idx = $4AF × тимбр (BASE+DIST-драйвер+…)>>8; ring Y:$04-$07:
//   ring0 = −c2×(0.5+eps), ring1 = eps, ring2 = width1[widx], ring3 = width2[widx].
//   Таблицы: $143F95 (div1), $144446 (div2), $141CA7 (coeff2),
//            $1444C6 (width1), $144546 (width2) — все в X-пространстве.
// ---------------------------------------------------------------------------
inline void sec_coefring(Mem& M, const Rom& rom, Regs& g) {
    const uint32_t r6 = g.r[6];
    const uint32_t r7 = g.r[7];
    int64_t& a = g.A;
    int64_t& b = g.B;

    g.x0 = M.rd('y', r6 + 0x08);                       // 0537 BASE-слово
    a = sat56(((int64_t)s24(0x800) * (int64_t)s24(g.x0)) << 1);  // 0538 mpyi
    a = acc_sub(g, a, 0x80);                           // 053A sub #>$80,a
    mnmfix::rnd_acc(a);                                // 053C rnd a
    b = a;                                             // 053D tfr a,b

    a = ld_acc(M.rd('y', r6 + 0x0E));                  // 053E (FILT-ENV L-офсет)
    a = acc_add(g, a, 0xC00000);                       // 053F add #>$c00000,a
    {   // 0541: abs a | a,y0  (y0 = латч ДО abs)
        g.y0 = acc_to24(a);
        a = (a < 0) ? sat56(-a) : a;
    }
    g.x1 = 0x700;                                      // 0542
    g.y1 = acc_to24(a);                                // 0544 move a,y1
    a = acc_mpy(g.y1, g.y0);                           // 0545 mpy y1,y0,a
    a = sext56(a << 2);                                // 0546 asl #$2,a,a
    g.y1 = M.rd('x', r7 - 1);                          // 0547 DIST-драйв (X:$4DB)
    g.y0 = acc_to24(a);                                // 0548 move a,y0
    a = acc_mpy(g.y1, g.y0);                           // 0549 mpy y1,y0,a
    g.y1 = acc_to24(a);                                // 054A move a,y1
    a = acc_mpy(g.y1, g.x1);                           // 054B mpy y1,x1,a
    {   // 054C add b,a (полное 56-бит)
        const int64_t sum = a + b;
        g.fc = (int)((sum >> 55) & 1);
        g.fn = sext56(sum) < 0; g.fz = (sext56(sum) == 0);
        {   const int64_t r = sext56(sum); const int msb = (int)((r >> 47) & 1);
            const int a2 = (int)((r >> 48) & 0xFF);
            g.fe = (a2 != (msb ? 0xFF : 0x00)) ? 1 : 0; }
        a = sext56(sum);
    }
    {   // 054D cmp x1,a ; 054E tfr x1,a ifge
        acc_sub(g, a, g.x1);
        if (g.fn == g.fv) a = sext56((int64_t)s24(g.x1) << 24);
    }
    g.y0 = 0x8;                                        // 054F
    g.x1 = M.rd('x', r6 + 0x01);                       // 0550
    b = ld_acc(M.rd('y', r6 + 0x25));                  // 0551
    M.wr('x', r7 - 3, acc_to24(a));                    // 0552 X:$4D9 (тимбр L)
    a = sat56(a + (((int64_t)s24(g.x1) * (int64_t)s24(g.y0)) << 1));  // 0553 mac x1,y0,a
    g.fc = (int)((((uint64_t)M.rd('y', r6 + 0x25)) >> 11) & 1);  // 0554 btst #$b,b
    const bool bit11 = g.fc != 0;
    if (!bit11) {                                      // 0555 bcc $0557
        /* 0556 не исполняется (bcc взят) */
    } else {
        M.wr('x', r7 - 3, acc_to24(a));                // 0556 (повторная запись)
    }
    g.x0 = M.rd('y', r6 + 0x09);                       // 0557 WDTH-слово
    g.fc = (int)((((uint64_t)M.rd('y', r6 + 0x25)) >> 9) & 1);   // 0558 btst #$9,b
    if (!g.fc) {                                       // 0559 mac -x1,y0,a ifcc
        const int64_t prod = ((int64_t)-s24(g.x1) * (int64_t)s24(g.y0)) << 1;
        a = sat56(a + prod);
    }
    a = sat56(a + (((int64_t)s24(0x800) * (int64_t)s24(g.x0)) << 1));  // 055A maci #$800,x0,a

    // --- симметричный блок для R-канала ($055C-$056C) ---
    b = ld_acc(M.rd('y', r6 + 0x0F));                  // 055C (R-офсет)
    b = acc_add(g, b, 0xC00000);                       // 055D
    {   // 055F: abs b | b,y0 (y0 = латч ДО abs)
        g.y0 = acc_to24(b);
        b = (b < 0) ? sat56(-b) : b;
    }
    g.x1 = 0x700;                                      // 0560
    g.y1 = acc_to24(b);                                // 0562 move b,y1
    b = acc_mpy(g.y1, g.y0);                           // 0563 mpy y1,y0,b
    b = sext56(b << 2);                                // 0564 asl #$2,b,b
    g.y1 = M.rd('x', r7 - 1);                          // 0565 DIST-драйв
    {   // 0566: tfr a,b | b,y0  (y0 = старый b, b = a)
        g.y0 = acc_to24(b);
        b = a;
    }
    a = acc_mpy(g.y1, g.y0);                           // 0567 mpy y1,y0,a
    g.y1 = acc_to24(a);                                // 0568 move a,y1
    b = sat56(b + (((int64_t)s24(g.y1) * (int64_t)s24(g.x1)) << 1));  // 0569 mac y1,x1,b
    if (b < 0) b = 0;                                  // 056A clr b ifmi
    a = ld_acc(M.rd('y', r7 - 3));                     // 056B Y:$4D9 (L-пара с X:$4D9)
    M.wr('y', r7 - 2, acc_to24(b));                    // 056C Y:$4DA (тимбр R)

    b = ld_acc(M.rd('y', r6 + 0x10));                  // 056D
    b = acc_sub56r2(g, b, a);                       // 056E sub a,b (56-бит)
    b = sext56(b << 8);                                // 056F asl #$8
    mnmfix::rnd_acc(b);                                // 0570 rnd b
    b = sext56(b >> 8);                                // 0571 asr #$8
    a = acc_add56(g, a, b);                              // 0572 add b,a

    g.m[4] = 0x3;                                      // 0573 (кольцо mod 4)
    g.x0 = acc_to24(a);                                // 0574 move a,x0
    M.wr('y', r7 - 3, g.x0);                           // 0575 Y:$4D9
    a = sat56(((int64_t)s24(0x4AF) * (int64_t)s24(g.x0)) << 1);  // 0576 mpyi #$4af
    g.y0 = M.rd('y', r6 + 0x11);                       // 0578
    g.r[0] = acc_to24(a);                              // 0579 move a,r0 (индекс!)
    b = sat56(((int64_t)s24(0x80) * (int64_t)s24(g.y0)) << 1);   // 057A mpyi #$80
    g.r[2] = acc_to24(b);                              // 057C move b,r2 (индекс!)

    g.x0 = rom.rd('x', 0x143F95 + g.r[0]);             // 057D div1[idx]
    g.y1 = rom.rd('x', 0x144446 + g.r[2]);             // 057F div2[widx]
    b = ld_acc(g.x0);                                  // 0581
    b = acc_add(g, b, g.y1);                           // 0582 add y1,b
    {   // 0583: asr b #$1,a  — эмулятор: B >>= 1 и a = 1<<24 (параллельно)
        g.fc = (int)(((uint64_t)b >> 0) & 1);
        b = sext56(b >> 1);
        a = 0x1000000ll;
        g.fn = (b < 0); g.fz = (b == 0);
        {   const int64_t r = sext56(b); const int msb = (int)((r >> 47) & 1);
            const int a2 = (int)((r >> 48) & 0xFF);
            g.fe = (a2 != (msb ? 0xFF : 0x00)) ? 1 : 0; }
    }
    g.fc = 0;                                          // 0584 andi #$fe,ccr
    g.x1 = acc_to24(b);                                // 0585 move b,x1
    for (int i = 0; i < 24; ++i) div_step(g, a, g.x1); // 0586-0588 do 24 × div

    b = ld_acc(g.y1);                                  // 0589 move y1,b
    {   // 058A: sub x0,b | a0,x1
        b = acc_sub(g, b, g.x0);
        g.x1 = (uint32_t)(a & M24);                    // частное (a0)
    }
    g.y0 = rom.rd('x', 0x141CA7 + g.r[0]);             // 058B coeff2[idx]
    g.x0 = acc_to24(b);                                // 058D move b,x0
    b = acc_mpy(g.x1, g.x0);                           // 058E mpy x1,x0,b
    b = sext56(b << 6);                                // 058F asl #$6
    g.r[4] = 0x4;                                      // 0590 (кольцо Y:$04-$07)
    g.x0 = acc_to24(b);                                // 0591 move b,x0 (eps)
    {   // 0592: mpy -y0,x0,a
        a = sat56(((int64_t)-s24(g.y0) * (int64_t)s24(g.x0)) << 1);
    }
    {   // 0593: sub y0,a | #$96,r0
        a = acc_sub(g, a, g.y0);
        g.r[0] = 0x96;
    }
    {   // 0594: asr a #$93,r1
        g.fc = (int)(((uint64_t)a >> 0) & 1);
        a = sext56(a >> 1);
        g.r[1] = 0x93;
        g.fn = (a < 0); g.fz = (a == 0);
        {   const int64_t r = sext56(a); const int msb = (int)((r >> 47) & 1);
            const int a2 = (int)((r >> 48) & 0xFF);
            g.fe = (a2 != (msb ? 0xFF : 0x00)) ? 1 : 0; }
    }
    g.y0 = rom.rd('x', 0x1444C6 + g.r[2]);             // 0595 width1[widx]
    g.y1 = rom.rd('x', 0x144546 + g.r[2]);             // 0597 width2[widx]
    M.wr('y', g.ppost(4), acc_to24(a));                // 0599 ring[0] Y:$04
    M.wr('y', g.ppost(4), g.x0);                       // 059A ring[1] Y:$05 (eps)
    {   // 059B: l:(r7)+,x  → x1 = X:(ea), x0 = Y:(ea)
        const uint32_t ea = g.ppost(7);
        g.x1 = M.rd('x', ea);
        g.x0 = M.rd('y', ea);
    }
    {   // 059C: x1,x:(r0)+ | y0,y:(r4)+
        M.wr('x', g.ppost(0), g.x1);
        M.wr('y', g.ppost(4), g.y0);
    }
    {   // 059D: x0,x:(r0)- | y1,y:(r4)+
        M.wr('x', g.ppostdec(0), g.x0);
        M.wr('y', g.ppost(4), g.y1);
    }
    {   // 059E: l:(r7)-,x
        const uint32_t ea = g.ppostdec(7);
        g.x1 = M.rd('x', ea);
        g.x0 = M.rd('y', ea);
    }
    M.wr('x', g.ppost(1), g.x1);                       // 059F x1,x:(r1)+
    M.wr('x', g.ppostdec(1), g.x0);                    // 05A0 x0,x:(r1)-
}
// ---------------------------------------------------------------------------
// $05A1-$05BA — вызов каскада-1 (L; R только при стерео), перестановка
// состояний каскада через L-память $4DC/$4DD.
// ---------------------------------------------------------------------------
inline void sec_cascade(Mem& M, Regs& g) {
    func_000340(M, g);                                 // 05A1 jsr func_000340
    {   // 05A2-05A5: x:-(r0),y0 | x:-(r0),y1 | x:(r1)+,x1 | x:(r1)-,x0
        g.y0 = M.rd('x', g.ppre(0));
        g.y1 = M.rd('x', g.ppre(0));
        g.x1 = M.rd('x', g.ppost(1));
        g.x0 = M.rd('x', g.ppostdec(1));                   // 05A5 (r1)- пост-декремент
    }
    M.wr('y', g.r[1], acc_to24(g.B));                  // 05A6 b,y:(r1)
    {   // 05A7: y,l:(r7)+  → X:(ea)=y1, Y:(ea)=y0
        const uint32_t ea = g.ppost(7);
        M.wr('x', ea, g.y1);
        M.wr('y', ea, g.y0);
    }
    {   // 05A8: x,l:(r7)+  → X:(ea)=x1, Y:(ea)=x0
        const uint32_t ea = g.ppost(7);
        M.wr('x', ea, g.x1);
        M.wr('y', ea, g.x0);
    }
    g.x0 = M.rd('x', g.r[6] + 0x0F);                   // 05A9 mono-флаг
    if (((g.x0 >> 0) & 1) != 0) return;                // 05AA jset #$0 → $05BB
    // --- стерео: каскад R ($05AC-$05BA) ---
    g.r[4] = 0x4;                                      // 05AC
    g.r[0] = 0xD6;                                     // 05AD
    g.r[1] = 0xD3;                                     // 05AE
    {   // 05AF: l:(r7)+,x
        const uint32_t ea = g.ppost(7);
        g.x1 = M.rd('x', ea);
        g.x0 = M.rd('y', ea);
    }
    M.wr('x', g.ppost(0), g.x1);                       // 05B0
    M.wr('x', g.ppostdec(0), g.x0);                    // 05B1
    {   // 05B2: l:(r7)-,x
        const uint32_t ea = g.ppre(7);
        g.x1 = M.rd('x', ea);
        g.x0 = M.rd('y', ea);
    }
    M.wr('x', g.ppost(1), g.x1);                       // 05B3
    M.wr('x', g.ppostdec(1), g.x0);                    // 05B4
    func_000340(M, g);                                 // 05B5 jsr func_000340
    g.y0 = M.rd('x', g.ppre(0));                       // 05B6
    g.y1 = M.rd('x', g.ppre(0));                       // 05B7
    g.x1 = M.rd('x', g.ppost(1));                      // 05B8
    g.x0 = M.rd('x', g.ppostdec(1));                   // 05B9
    M.wr('y', g.r[1], acc_to24(g.B));                  // 05BA b,y:(r1)
}

// ---------------------------------------------------------------------------
// $05BB-$05FE — HALFBAND 2× интерполятор (полифаза 2 тапа × 8 групп),
// история Y:$91-$B0 (L) / Y:$D1-$F0 (R), выходы X:$71+ / X:$B1+.
// ---------------------------------------------------------------------------
inline void sec_halfband_tail(Mem& M, Regs& g);

inline void sec_halfband(Mem& M, Regs& g) {
    {   // 05BB-05BC: y,l:(r7)+ | x,l:(r7)+  (продолжение сохранения состояний)
        const uint32_t ea = g.ppost(7);
        M.wr('x', ea, g.y1);
        M.wr('y', ea, g.y0);
        const uint32_t ea2 = g.ppost(7);
        M.wr('x', ea2, g.x1);
        M.wr('y', ea2, g.x0);
    }
    g.m[4] = M24;                                      // 05BD
    g.r[4] = (uint32_t)((int64_t)g.r[7] - 0x18) & M24; // 05BF lua (r7-$18),r4
    g.n[4] = (int32_t)s24(0xFFFFFE);                   // 05C0 (= -2)
    g.r[5] = g.r[4];                                   // 05C2
    g.r[0] = 0x91;                                     // 05C3
    g.r[2] = 0xD1;                                     // 05C4
    {   // 05C5-05CD: раскладка L-пар в Y:$91+/Y:$D1+
        {   const uint32_t ea = g.ppost(4);
            g.x1 = M.rd('x', ea); g.x0 = M.rd('y', ea); }
        M.wr('y', g.ppost(0), g.x0);                   // 05C6
        M.wr('y', g.ppost(0), g.x1);                   // 05C7
        {   const uint32_t ea = g.ppost(4);
            g.x1 = M.rd('x', ea); g.x0 = M.rd('y', ea); }
        M.wr('y', g.ppost(0), g.x0);                   // 05C9
        M.wr('y', g.ppost(2), g.x1);                   // 05CA
        {   const uint32_t ea = g.ppost(4);
            g.x1 = M.rd('x', ea); g.x0 = M.rd('y', ea); }
        M.wr('y', g.ppost(2), g.x0);                   // 05CC
        M.wr('y', g.ppost(2), g.x1);                   // 05CD
    }
    // --- L: halfband ---
    g.r[4] = 0x91;                                     // 05CE
    g.x0 = 0xF528BD;                                   // 05CF
    g.x1 = 0x4A4DF0;                                   // 05D1
    g.r[1] = 0x71;                                     // 05D3
    g.r[2] = 0x73;                                     // 05D4
    g.y0 = M.rd('y', g.ppost(4));                      // 05D5
    g.n[1] = 3;                                        // 05D6
    g.n[2] = g.n[1];                                   // 05D7
    for (int i = 0; i < 8; ++i) {                      // 05D8 do #<$8
        {   // 05DA: mpy y0,x0,a | a,x:(r1)+n1 | y:(r4)+,y1
            const uint32_t aL = acc_to24(g.A);      // латч ДО ALU
            mac_std(g, g.A, g.y0, g.x0, false);
            M.wr('x', g.pnext(1), aL);
            g.y1 = M.rd('y', g.ppost(4));
        }
        {   // 05DB: mpy x0,y1,b | b,x:(r2)+n2
            const uint32_t bL = acc_to24(g.B);
            mac_std(g, g.B, g.x0, g.y1, false);
            M.wr('x', g.pnext(2), bL);
        }
        {   // 05DC: mac y1,x1,a | y:(r4)+,y0
            mac_std(g, g.A, g.y1, g.x1, true);
            g.y0 = M.rd('y', g.ppost(4));
        }
        {   // 05DD: mac x1,y0,b | y1,x:(r1)+
            mac_std(g, g.B, g.x1, g.y0, true);
            M.wr('x', g.ppost(1), g.y1);
        }
        {   // 05DE: mac x1,y0,a | y:(r4)+,y1
            mac_std(g, g.A, g.x1, g.y0, true);
            g.y1 = M.rd('y', g.ppost(4));
        }
        {   // 05DF: mac y1,x1,b | y0,x:(r2)+
            mac_std(g, g.B, g.y1, g.x1, true);
            M.wr('x', g.ppost(2), g.y0);
        }
        {   // 05E0: mac x0,y1,a | y:(r4)+n4,y0
            mac_std(g, g.A, g.x0, g.y1, true);
            g.y0 = M.rd('y', g.pnext(4));
        }
        {   // 05E1: mac y0,x0,b | y:(r4)+,y0
            mac_std(g, g.B, g.y0, g.x0, true);
            g.y0 = M.rd('y', g.ppost(4));
        }
    }
    M.wr('x', g.pnext(1), acc_to24(g.A));              // 05E2 a,x:(r1)+n1
    M.wr('x', g.pnext(2), acc_to24(g.B));              // 05E3 b,x:(r2)+n2
    g.y1 = M.rd('y', g.ppost(4));                      // 05E4
    {   // 05E5: y,l:(r5)+ → X:(ea)=y1, Y:(ea)=y0
        const uint32_t ea = g.ppost(5);
        M.wr('x', ea, g.y1);
        M.wr('y', ea, g.y0);
    }
    g.y1 = M.rd('y', g.ppostdec(4));                   // 05E6
    M.wr('y', g.r[5], g.y1);                           // 05E7
    g.y1 = M.rd('x', g.r[6] + 0x0F);                   // 05E8 mono-флаг
    if ((((g.y1) >> 0) & 1) != 0) {                    // 05E9 brset #$0 → $05FB
        // моно: R-halfband пропускается; хвост $05FB-$05FE
        sec_halfband_tail(M, g);
        return;
    }
    // --- R: halfband ($05EB-$05FA) ---
    g.r[4] = 0xD1;                                     // 05EB
    g.r[1] = 0xB1;                                     // 05EC
    g.r[2] = 0xB3;                                     // 05ED
    g.y0 = M.rd('y', g.ppost(4));                      // 05EE
    for (int i = 0; i < 8; ++i) {                      // 05EF do #<$8
        {   const uint32_t aL = acc_to24(g.A);         // 05F1 (латч)
            mac_std(g, g.A, g.y0, g.x0, false);
            M.wr('x', g.pnext(1), aL);
            g.y1 = M.rd('y', g.ppost(4)); }
        {   const uint32_t bL = acc_to24(g.B);         // 05F2 (латч)
            mac_std(g, g.B, g.x0, g.y1, false);
            M.wr('x', g.pnext(2), bL); }
        {   mac_std(g, g.A, g.y1, g.x1, true);         // 05F3
            g.y0 = M.rd('y', g.ppost(4)); }
        {   mac_std(g, g.B, g.x1, g.y0, true);         // 05F4
            M.wr('x', g.ppost(1), g.y1); }
        {   mac_std(g, g.A, g.x1, g.y0, true);         // 05F5
            g.y1 = M.rd('y', g.ppost(4)); }
        {   mac_std(g, g.B, g.y1, g.x1, true);         // 05F6
            M.wr('x', g.ppost(2), g.y0); }
        {   mac_std(g, g.A, g.x0, g.y1, true);         // 05F7
            g.y0 = M.rd('y', g.pnext(4)); }
        {   mac_std(g, g.B, g.y0, g.x0, true);         // 05F8
            g.y0 = M.rd('y', g.ppost(4)); }
    }
    M.wr('x', g.pnext(1), acc_to24(g.A));              // 05F9
    M.wr('x', g.pnext(2), acc_to24(g.B));              // 05FA
    sec_halfband_tail(M, g);
}
// $05FB-$05FE — хвост halfband (склейка состояний)
inline void sec_halfband_tail(Mem& M, Regs& g) {
    M.wr('x', g.ppost(5), g.y0);                       // 05FB y0,x:(r5)+
    g.y0 = M.rd('y', g.ppost(4));                      // 05FC
    g.y1 = M.rd('y', g.ppost(4));                      // 05FD
    {   // 05FE: y,l:(r5) → X:(r5)=y1, Y:(r5)=y0
        M.wr('x', g.r[5], g.y1);
        M.wr('y', g.r[5], g.y0);
    }
}

} // namespace mnmchain
