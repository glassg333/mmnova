// ============================================================================
// mnm_chain_a.hpp — секции кадра, часть A.
// Канонический перенос OS 1.32B (P:$0132-$0142, $04A8-$0536) в читаемый C++.
// Каждая строка = 1-2 инструкции листинга (PC в комментарии). ВСЕ записи в
// регистры (x0/x1/y0/y1/rN/nN/A/B) идут в Regs — они видны на границах секций.
// Верификация: оракул work/chain_oracle + stage_dump.bin (границы секций).
// ============================================================================
#pragma once
#include "mnm_dsp_fix.hpp"

namespace mnmchain {

using mnmfix::Mem;
using mnmfix::Regs;
using mnmfix::Rom;
using mnmfix::M24;
using mnmfix::s24;
using mnmfix::sat56;
using mnmfix::sext56;
using mnmfix::acc_to24;

// 24-бит слово памяти → аккумулятор (заносится в A1 со знаком)
inline int64_t ld_acc(uint32_t w) noexcept { return (int64_t)s24(w) << 24; }

// add/sub слова, выровненного на A1 (<<24): точная реплика alu_add/alu_sub
// эталона (C = бит55 сырой суммы для add; V по знакам; без сатурации)
inline int64_t acc_add(Regs& g, int64_t x, uint32_t wop) noexcept {
    const int64_t y = (int64_t)s24(wop) << 24;
    const int64_t a = x + y;
    const int sx = x < 0, sy = y < 0, sr = sext56(a) < 0;
    g.fv = (sx == sy && sr != sx) ? 1 : 0;
    g.fc = (int)((a >> 55) & 1);
    g.fn = sr; g.fz = (sext56(a) == 0);
    {   // E: A2-байт не является знак-расширением A1 (flags_from эталона)
        const int64_t r = sext56(a);
        const int a1msb = (int)((r >> 47) & 1);
        const int a2 = (int)((r >> 48) & 0xFF);
        g.fe = (a2 != (a1msb ? 0xFF : 0x00)) ? 1 : 0;
    }
    return sext56(a);
}
inline int64_t acc_sub(Regs& g, int64_t x, uint32_t wop) noexcept {
    const int64_t y = (int64_t)s24(wop) << 24;
    const int64_t a = x - y;
    const int sx = x < 0, sy = y < 0, sr = sext56(a) < 0;
    g.fv = (sx != sy && sr != sx) ? 1 : 0;
    g.fc = (a < 0) ? 0 : 1;
    g.fn = sr; g.fz = (sext56(a) == 0);
    {
        const int64_t r = sext56(a);
        const int a1msb = (int)((r >> 47) & 1);
        const int a2 = (int)((r >> 48) & 0xFF);
        g.fe = (a2 != (a1msb ? 0xFF : 0x00)) ? 1 : 0;
    }
    return sext56(a);
}
// cmp b,a — полное 56-бит сравнение аккумуляторов (флаги a - b)
inline int64_t acc_sub56(Regs& g, int64_t x, int64_t y) noexcept {
    const int64_t a = x - y;
    const int sx = x < 0, sy = y < 0, sr = sext56(a) < 0;
    g.fv = (sx != sy && sr != sx) ? 1 : 0;
    g.fc = (a < 0) ? 0 : 1;
    g.fn = sr; g.fz = (sext56(a) == 0);
    {
        const int64_t r = sext56(a);
        const int a1msb = (int)((r >> 47) & 1);
        const int a2 = (int)((r >> 48) & 0xFF);
        g.fe = (a2 != (a1msb ? 0xFF : 0x00)) ? 1 : 0;
    }
    return sext56(a);
}
// mpy: s24*s24<<1 с сатурацией
inline int64_t acc_mpy(int64_t a1, int64_t a2, bool neg1 = false) noexcept {
    int64_t p1 = s24((uint32_t)a1 & M24);
    if (neg1) p1 = -p1;
    return sat56((p1 * s24((uint32_t)a2 & M24)) << 1);
}
// ветвления (Motorola): blt = N!=V; ble = (N!=V)||Z; bec = !C
inline bool blt(const Regs& g) { return g.fn != g.fv; }
inline bool ble(const Regs& g) { return (g.fn != g.fv) || g.fz; }

// -- константы-хосты ОС (однотрековая модель) --------------------------------
static constexpr uint32_t R6BASE = 0x400;      // база страницы трека (P)
static constexpr uint32_t PAGE   = 0x428;      // Y:$123 = P + $28

// ---------------------------------------------------------------------------
// ВХОД КАДРА $02EC: jmp func_000132 ($0132-$0142).
// ---------------------------------------------------------------------------
inline void sec_entry(Mem& M, Regs& g) {
    const uint32_t m0 = M24;
    g.m[0] = m0;                       // 0132
    g.m[6] = m0; g.m[1] = m0;          // 0134-0135
    g.m[2] = m0; g.m[3] = m0;          // 0136-0137
    g.r[6] = M.rd('y', 0x123);         // 0138  (Y:$123 = P+$28)
    g.m[4] = m0; g.m[5] = m0; g.m[7] = m0;   // 013A-013C
    g.r[6] = (uint32_t)((int64_t)g.r[6] - 0x28) & M24;  // 013D lua (r6-$28),r6
    g.r[7] = 0x100;                    // 013E
    g.r[5] = M.rd('x', 0x2C3);         // 0140  (хост: стек треков)
}

// ---------------------------------------------------------------------------
// $04A8-$04F4 — МАШИННЫЙ ADSR-АВТОМАТ (фаза X:(r6+$0), триггер Y:(r6+$21)).
// Накопитель Y:$4FF, кривые tblA[$141800] / tblB[$141880].
//   фаза 1 = ATK:  Y4FF += tblA[ATK>>16];  signed-wrap → фаза 4
//   фаза 4 = DEC1: Y4FF = -Y4FF * tblB[DEC>>16], пока > SUS²; ниже → фаза 5
//   фаза 5 = SUS:  Y4FF = SUS²
//   фаза 2 = REL:  Y4FF = -Y4FF * tblB[REL>>16]
//   прочее:        Y4FF = $7FFFFF
// ---------------------------------------------------------------------------
inline void sec_adsr(Mem& M, const Rom& rom, Regs& g) {
    const uint32_t r6 = g.r[6];
    int64_t& a = g.A;   // аккумуляторы персистентны (как в DSP)
    int64_t& b = g.B;
    a = ld_acc(M.rd('y', 0x124));                       // 04A8
    if (a != 0) return;                                 // 04AA-04AB bne $04F5
    a = ld_acc(M.rd('x', r6 + 0x00));                   // 04AC фаза
    b = ld_acc(M.rd('y', r6 + 0x21));                   // 04AD триггер-событие
    g.x0 = 0;                                           // 04AE #$0,x0
    if (b != 0) a = b;                                  // 04AF tfr b,a ifne
    M.wr('y', r6 + 0x21, g.x0);                         // 04B0 сброс триггера
    M.wr('x', r6 + 0x00, acc_to24(a));                  // 04B1 фаза → X:(r6+$0)

    {   // 04B2 cmp #<$1,a ; bne $04C7
        if (acc_sub(g, a, 1) != 0) goto PH_NOT1;
    }
    {   // === фаза 1: ATTACK ===
        a = ld_acc(M.rd('y', r6 + 0x18));               // 04B4 ATK-слово
        a = sext56(a >> 16);                            // 04B5 asr #$10
        g.r[4] = 0x141800;                              // 04B6
        g.n[4] = (int32_t)s24(acc_to24(a));             // 04B8 move a,n4
        a = ld_acc(M.rd('y', 0x4FF));                   // 04B9 накопитель
        g.y0 = rom.rd('y', 0x141800 + (uint32_t)g.n[4]); // 04BB tblA[ATK]
        a = acc_add(g, a, g.y0);                        // 04BC add y0,a
        M.wr('y', 0x4FF, acc_to24(a));                  // 04BE/04C1 (обе ветки)
        if (g.fe) {                                     // 04BD bes $04C1 (E=1)
            g.x0 = 4;                                   // 04C3 #>$4,x0
            M.wr('x', r6 + 0x00, g.x0);                 // 04C5 фаза=4
        }
        return;                                         // 04C0/04C6
    }
PH_NOT1:
    {   // 04C7 cmp #<$4,a ; bne $04DC
        if (acc_sub(g, a, 4) != 0) goto PH_NOT4;
    }
    {   // === фаза 4: DECAY-1 (рампа вниз до SUS²) ===
        g.y0 = M.rd('y', r6 + 0x1A);                    // 04C9 SUS-слово
        b = acc_mpy(g.y0, g.y0);                        // 04CA mpy y0,y0,b = SUS²
        g.r[4] = 0x141880;                              // 04CA (параллельно)
        a = ld_acc(M.rd('y', r6 + 0x19));               // 04CC DEC-слово
        a = sext56(a >> 16);                            // 04CD asr #$10
        g.n[4] = (int32_t)s24(acc_to24(a));             // 04CE
        g.x1 = M.rd('y', 0x4FF);                        // 04CF drive → x1
        g.y0 = rom.rd('y', 0x141880 + (uint32_t)g.n[4]); // 04D1 tblB[DEC]
        a = acc_mpy(g.x1, g.y0, /*neg1=*/true);         // 04D2 mpy -x1,y0,a
        {   // 04D3 cmp b,a ; ble $04D8
            acc_sub56(g, a, b);
            if (ble(g)) {                               // 04D4 a <= SUS² → SUS
                g.x0 = 5;                               // 04D8 #>$5,x0
                M.wr('x', r6 + 0x00, g.x0);             // 04DA фаза=5
                return;                                 // 04DB
            }
        }
        M.wr('y', 0x4FF, acc_to24(a));                  // 04D5 рампа вниз
        return;                                         // 04D7
    }
PH_NOT4:
    {   // 04DC cmp #<$5,a ; bne $04E3
        if (acc_sub(g, a, 5) != 0) goto PH_NOT5;
    }
    {   // === фаза 5: SUSTAIN — драйв = SUS² ===
        g.x0 = M.rd('y', r6 + 0x1A);                    // 04DE SUS
        const int64_t acc = acc_mpy(g.x0, g.x0);        // 04DF mpy x0,x0,a
        a = acc;
        M.wr('y', 0x4FF, acc_to24(a));                  // 04E0
        return;                                         // 04E2
    }
PH_NOT5:
    {   // 04E3 cmp #<$2,a ; bne $04F1
        if (acc_sub(g, a, 2) != 0) goto PH_NOT2;
    }
    {   // === фаза 2: RELEASE ===
        a = ld_acc(M.rd('y', r6 + 0x1B));               // 04E5 REL-слово
        a = sext56(a >> 16);                            // 04E6 asr #$10
        g.r[4] = 0x141880;                              // 04E7
        g.n[4] = (int32_t)s24(acc_to24(a));             // 04E9
        g.x1 = M.rd('y', 0x4FF);                        // 04EA
        g.y0 = rom.rd('y', 0x141880 + (uint32_t)g.n[4]); // 04EC tblB[REL]
        a = acc_mpy(g.x1, g.y0, /*neg1=*/true);         // 04ED mpy -x1,y0,a
        M.wr('y', 0x4FF, acc_to24(a));                  // 04EE
        return;                                         // 04F0
    }
PH_NOT2:
    {   // === прочее: драйв = 0.9999999 ===
        g.x0 = 0x7FFFFF;                                // 04F1 #>$7fffff,x0
        M.wr('y', 0x4FF, g.x0);                         // 04F3
    }
}

// ---------------------------------------------------------------------------
// $04F5-$04FE — КОПИЯ ВЫХОДА МАШИНЫ: Y:$100-$121 → L-плоскость X:$97-$A7,
// R-плоскость X:$D7-$E7. r7 → r6+$DC (ячейки дист-энвелопа).
// ---------------------------------------------------------------------------
inline void sec_machcopy(Mem& M, Regs& g) {
    g.n[6] = 0xDC;                                     // 04F5
    M.wr('x', 0xFF, g.r[5]);                           // 04F6
    g.r[2] = 0x97;                                     // 04F8
    g.r[3] = 0xD7;                                     // 04F9
    for (int i = 0; i < 17; ++i) {                     // 04FA do #<$11
        M.wr('x', g.ppost(2), acc_to24(g.A));          // 04FC a → x:(r2)+
        g.A = ld_acc(M.rd('y', g.ppost(7)));           // 04FC y:(r7)+ → a
        M.wr('x', g.ppost(3), acc_to24(g.B));          // 04FD b → x:(r3)+
        g.B = ld_acc(M.rd('y', g.ppost(7)));           // 04FD y:(r7)+ → b
    }
    g.r[7] = (uint32_t)((int64_t)g.r[6] + g.n[6]) & M24;  // 04FE lua (r6)+n6,r7
}

// ---------------------------------------------------------------------------
// $04FF-$0536 — DIST ДРАЙВ-ЭНВЕЛОП (фаза Y:(r7-$1), накопитель X:(r7-$1),
// счётчик X:(r7-$2); r7 = r6+$DC).
//   фаза 0: drive += tblA[$141800][DIST]; signed-wrap (C=1) → фаза 1 + arm
//   фаза 1: счётчик++; b = 0 → a>0 → blt НЕ берётся → фаза = 2 (сразу)
//   фаза 2: drive -= tblE[$141A00][VOL], снизу нулём (clr b ifmi)
// ---------------------------------------------------------------------------
inline void sec_distenv(Mem& M, const Rom& rom, Regs& g) {
    const uint32_t r6 = g.r[6];
    const uint32_t r7 = g.r[7];                        // = r6+$DC

    int64_t& a = g.A;   // аккумуляторы персистентны (как в DSP)
    int64_t& b = g.B;
    a = ld_acc(M.rd('y', r6 + 0x20));                  // 04FF armed-флаг
    if (a == 0x1000000ll) {                            // 0500-0501
        g.x0 = 0;                                      // 0502
        M.wr('y', r7 - 1, g.x0);                       // 0503 фаза = 0
        M.wr('y', r6 + 0x20, g.x0);                    // 0504 сброс armed
        M.wr('x', r7 - 1, g.x0);                       // 0505 накопитель = 0
    }
    a = ld_acc(M.rd('y', r7 - 1));                     // 0506 фаза
    if (a == 0) {                                      // 0507-0508 bne $0517
        // === фаза 0: атака драйва ===
        a = ld_acc(M.rd('y', r6 + 0x0C));              // 0509 DIST-слово
        a = sext56(a >> 16);                           // 050A asr #$10
        b = ld_acc(M.rd('x', r7 - 1));                 // 050B накопитель
        g.r[4] = acc_to24(a);                          // 050C move a,r4
        g.y0 = rom.rd('y', 0x141800 + g.r[4]);         // 050D tblA[DIST]
        b = acc_add(g, b, g.y0);                       // 050F add y0,b
        if (g.fe) {                                    // 0510 bec НЕ взят (E=1 → wrap) → arm
            g.x0 = 1;                                  // 0511 #>$1,x0
            M.wr('y', r7 - 1, g.x0);                   // 0513 фаза = 1
            M.wr('x', r7 - 2, g.x0);                   // 0514 счётчик = 1
        }
        M.wr('x', r7 - 1, acc_to24(b));                // 0515 накопитель
        return;                                        // 0516 bra $0537
    }
    {   // 0517 cmp #<$1,a ; bne $052B
        if (acc_sub(g, a, 1) == 0) {
            // === фаза 1 ===
            a = 0;                                     // 0519 clr a
            g.y0 = M.rd('y', r6 + 0x23);               // 051A
            b = acc_mpy(g.y0, 0);                      // 051B-051C mpy y0,x0,b
            g.x0 = acc_to24(a);                        // 051B move a,x0 (=0)
            a = ld_acc(M.rd('x', r7 - 2));             // 051D счётчик
            g.x1 = acc_to24(b);                        // 051E move b,x1
            b = acc_mpy(0x791FD0, g.x1);               // 051F mpyi #>$791fd0,x1,b
            b = sext56(b << 1);                        // 0521 asl b
            a = sext56(a + 0x1000000ll);               // 0522 add #<$1,a
            {   // 0523 cmp b,a ; blt $0529
                acc_sub56(g, a, b);
                if (blt(g)) {                          // 0524
                    M.wr('x', r7 - 2, acc_to24(a));    // 0529 счётчик
                    return;                            // 052A
                }
                g.x0 = 2;                              // 0525 #>$2,x0
                M.wr('y', r7 - 1, g.x0);               // 0527 фаза = 2
                // 0528 bra $052B — провал в фазу 2
            }
        }
    }
    {   // === фаза 2 (и провал из фазы 1): спад по VOL ===
        a = ld_acc(M.rd('y', r6 + 0x0D));              // 052B VOL-слово
        a = acc_add(g, a, 0x7FFF);                     // 052C-052D add #>$7fff
        a = sext56(a >> 16);                           // 052E asr #$10
        mnmfix::rnd_acc(a);                            // 052F rnd a
        g.r[4] = acc_to24(a);                          // 0530 move a,r4
        b = ld_acc(M.rd('x', r7 - 1));                 // 0531 накопитель
        g.y0 = rom.rd('y', 0x141A00 + g.r[4]);         // 0532 tblE[VOL]
        b = acc_sub(g, b, g.y0);                       // 0534 sub y0,b
        if (b < 0) b = 0;                              // 0535 clr b ifmi
        M.wr('x', r7 - 1, acc_to24(b));                // 0536
    }
}

} // namespace mnmchain
