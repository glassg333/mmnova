// ============================================================================
// mnm_tone_index.h — «индекс тона» Monomachine OS 1.32B: пословная транскрипция
//                     P:$0537–$056C ядра DSP1 + читатель гейна $0857.
// ----------------------------------------------------------------------------
// Закрытие OPEN-пунктов §7: «WOFS-ветка P:$0569–$056B», «трекинг питча
// P:$054F–$055A», «домены tone↔кольцо» (exp72b/c/d/e, worklog Task 6).
//
// ФАКТЫ (оракул — dsp_emu.py; пины в Tests/test_sanity.cpp):
//   КОЛЬЦО КОЭФФИЦИЕНТОВ ЭТОГО НЕ КАСАЕТСЯ: цель кольца = слово $410 точно
//   (P:$056D–$0572 сводит acc к $410 безусловно); mnm_17 верен.
//   Модуль производит ДВА слова:
//     X:$4D9 — индекс тона (без HPQ/WOFS-добавок): читатели $06CD (гребень
//             коэффициентов EQ) и $0857 (гейн EQ = 1 − tbl).
//     Y:$4DA — max(0, idx + imm·HPQ + WOFS-член): читатели $05FF (комбайнер
//             Q-интерполятора EQ) и $07D4 (фактор среза ступени DIST, clamp
//             $6A3, таблица $143546).
//
//   ЗАКОН (ЛИМИТЕР на каждом acc→reg/mem перемещении — FM §5.4.1.2):
//     base  = param4($408)·$800 − $80                        [$0537–$053D]
//     pf    = 4·sat(w40E − 0.5)·sat|w40E − 0.5|              [$053E–$0546]
//             — «знаковый квадрат»: $0541 abs a a,y0 кладёт y0 = ПРЕД-abs
//               (знаковое), $0544 move a,y1 = ПОСЛЕ-abs; обе пересылки —
//               с лимитером (±1.5 сатурируются в ±1.0!);
//     idx   = base + sat(level·sat(pf))·$700·2               [$0547–$054B]
//             — $054A move a,y1 С ЛИМИТЕРОМ: произведение level·pf жёстко
//               сатурируется в ±1.0 → при |level·pf| ≥ 0.5 член фиксирован
//               (потому разные $40E дают одинаковый idx — измерено);
//     A     = min(A, $700)                                   [$054C–$054E]
//     X:$4D9 = B1(A)  [a1-часть, БЕЗ лимитера]               [$0552]
//     A    += 2048·X:$401·2 (delay-база DBAS!)               [$0553 mac x1,y0]
//     if bit11(Y:$425): X:$4D9 = B1(A)  (обновлённый)        [$0556]
//     if bit9(Y:$425) clear: A −= 2048·X:$401·2 (откат)      [$0559 mac -x1]
//     A    += 2048·HPQ($409)·2                               [$055A maci #$800]
//     Y:$4DA = max(0, B_idx + sat(level·sat(4(w40F−0.5)|w40F−0.5|))·$700·2)
//                                                            [$055C–$056C]
//             — $0566 tfr a,b ПЕРЕНОСИТ idx-acc в B; $0569 mac y1,x1,b;
//               $056A clr ifmi; $056C move b1 — B1 БЕЗ лимитера.
//     $408/$40E/$40F — дыра страницы (0 на стоковой OS) → pf = −1.0,
//     X:$401/$425 — дыра → добавки взаимно уничтожаются.
//     ПИНЫ: level 0.5 → X:$4D9 = FFFC00 (−1024); level 1.0 → Y:$4DA = 000D7F
//     (3455); param4 0.25 → X:$4D9 = FFFE00 (−512) — пины пака 12 + exp72e.
//   ГЕЙН EQ $0857–$0862: g = $7FFFFF − kCutoff[128 + clamp(X:$4D9, 0, $63F)]
//     (таблица $1435C6 = kCutoff+128).
// ============================================================================
#pragma once
#include <cstdint>
#include "mnm_dsp56300_math.h"
#include "mnm_firmware_tables.h"   // kCutoff[1728] ($1435C6 = +128)

namespace mnm {

class MnmToneIndex
{
public:
    struct In
    {
        uint32_t param4;     // Y:$408 (дыра: 0)
        uint32_t w40E;       // Y:$40E (дыра: 0)
        uint32_t w40F;       // Y:$40F (дыра: 0)
        uint32_t hpqWord;    // Y:$409 — слово HPQ (FLT-ручка, <<16)
        uint32_t x401;       // X:$401 — delay-база DBAS (дыра: 0)
        uint32_t bits425;    // Y:$425 — биты 9/11 (дыра: 0)
        uint32_t levelWord;  // X:$4DB — уровень панча DIST (MnmDistDrive)
    };

    struct Out
    {
        uint32_t toneIdx;    // X:$4D9 (B1, без лимитера)
        uint32_t wofsWord;   // Y:$4DA (B1, без лимитера)
    };

    static Out compute(const In& p) noexcept
    {
        constexpr uint64_t M56 = 0xFFFFFFFFFFFFFFull;
        auto acc = [](uint32_t w) -> int64_t {          // слово -> acc (sext<<24)
            return int64_t(sext24(w)) << 24;
        };
        auto b1raw = [](int64_t a) -> uint32_t {        // a1-часть БЕЗ лимитера
            return (uint32_t)(uint64_t(a) >> 24) & 0xFFFFFFu;
        };

        // $0537–$053D: base = param4·$800 − $80, rnd
        int64_t A = mpy56(sext24(p.param4 & 0xFFFFFFu), sext24(0x800u));
        A = sext56((uint64_t(A) - (uint64_t(128) << 24)) & M56);
        A = rnd56(A);
        int64_t B = A;                                              // $053D tfr a,b

        // $053E–$0546: pf = 4·sat(w−0.5)·sat|w−0.5| (знаковый квадрат)
        int64_t W = acc(p.w40E) + acc(0xC00000u);                   // $053F add (−0.5)
        W = sext56((uint64_t)W & M56);
        uint32_t y0 = limit24(W);                                   // $0541 parallel PRE-abs
        int64_t Ab = (W < 0) ? -W : W;                              // $0541 abs (знак acc!)
        uint32_t y1 = limit24(Ab);                                  // $0544 move a,y1
        int64_t pf = mpy56(sext24(y1), sext24(y0));                 // $0545 mpy y1,y0,a
        pf = sext56((uint64_t(pf) << 2) & M56);                     // $0546 asl #$2

        // $0547–$054B: idx = base + sat(level·sat(pf))·$700·2
        const uint32_t lvl = p.levelWord & 0xFFFFFFu;               // $0547 raw load
        uint32_t pfW = limit24(pf);                                 // $0548 move a,y0
        int64_t prod = mpy56(sext24(lvl), sext24(pfW));             // $0549 mpy y1,y0,a
        uint32_t prodW = limit24(prod);                             // $054A move a,y1 ЛИМИТЕР
        A = mpy56(sext24(prodW), sext24(0x700u));                   // $054B mpy y1,x1,a
        A = sext56((uint64_t(A) + (uint64_t)B) & M56);              // $054C add b,a
        {                                                           // $054D–$054E
            const int64_t lim = int64_t(0x700) << 24;
            if (A >= lim) A = lim;                                  // tfr x1,a ifge
        }

        // $054F–$055A: добавки X:$401 / HPQ (условные по битам $425)
        const uint32_t x401 = p.x401 & 0xFFFFFFu;
        const uint32_t bits = p.bits425 & 0xFFFFFFu;
        const bool bit11 = (bits >> 11) & 1u;
        const bool bit9 = (bits >> 9) & 1u;
        uint32_t toneIdx = b1raw(A);                                // $0552 a1 raw
        A = mac56(A, sext24(x401), sext24(8));                      // $0553 mac x1,y0,a
        if (bit11)
            toneIdx = b1raw(A);                                     // $0556 a1 raw
        if (!bit9)
            A = mac56(A, -sext24(x401), sext24(8));                 // $0559 mac -x1,y0,a
        A = mac56(A, sext24(0x800u), sext24(p.hpqWord & 0xFFFFFFu)); // $055A maci #$800,x0

        // $055C–$056C: WOFS-ветка → Y:$4DA
        int64_t Wf = acc(p.w40F) + acc(0xC00000u);                  // $055D add (−0.5)
        Wf = sext56((uint64_t)Wf & M56);
        uint32_t w0 = limit24(Wf);                                  // $055F parallel PRE-abs
        int64_t Bb = (Wf < 0) ? -Wf : Wf;                           // $055F abs (знак acc!)
        uint32_t w1 = limit24(Bb);                                  // $0562 move b,y1
        int64_t wf = mpy56(sext24(w1), sext24(w0));                 // $0563 mpy y1,y0,b
        wf = sext56((uint64_t(wf) << 2) & M56);                     // $0564 asl #$2
        uint32_t wfW = limit24(wf);                                 // $0566 tfr a,b b,y0
        B = A;                                                      // $0566 tfr a,b (idx-acc!)
        A = mpy56(sext24(lvl), sext24(wfW));                        // $0567 mpy y1,y0,a
        uint32_t aW = limit24(A);                                   // $0568 move a,y1
        B = mac56(B, sext24(aW), sext24(0x700u));                   // $0569 mac y1,x1,b
        if (B < 0) B = 0;                                           // $056A clr b ifmi
        // $056B: A = Y:$4D9 — гасится шагом $056D–$0572 (кольцо берёт $410
        //         безусловно; см. mnm_filter_ring.h — mnm_17)
        const uint32_t wofsWord = b1raw(B);                         // $056C b1 raw

        Out o;
        o.toneIdx = toneIdx;
        o.wofsWord = wofsWord;
        return o;
    }

    // Гейн EQ $0857–$0862: g = $7FFFFF − kCutoff[128 + clamp(idx, 0, $63F)]
    static uint32_t eqGain(uint32_t toneIdx) noexcept
    {
        uint32_t i = toneIdx & 0xFFFFFFu;
        if (sext24(i) < 0) i = 0;                    // $0858–$0859 clr ifmi
        if (sext24(i) > sext24(0x63Fu)) i = 0x63Fu;  // $085A–$085B clamp $63F
        const uint32_t w = (128u + i < 1728u) ? kCutoff[128u + i] : kCutoff[1727u];
        return 0x7FFFFFu - (w & 0xFFFFFFu);          // $0862 sub b,a
    }

private:
    static constexpr uint64_t M56v = 0xFFFFFFFFFFFFFFull;
    static int64_t sext56(uint64_t v) noexcept
    {
        v &= M56v;
        return (v & (1ull << 55)) ? (int64_t)(v - (1ull << 56)) : (int64_t)v;
    }
    // rnd: округление аккумулятора до 24-бит границы (A1 += A0>>23, A0 = 0)
    static int64_t rnd56(int64_t a) noexcept
    {
        int64_t r = sext56((uint64_t(a) + (1ull << 23)) & M56v);
        return r & ~(int64_t)0xFFFFFF;   // A0 := 0
    }
};

} // namespace mnm
