// ============================================================================
// mnm_filter_ring.h — КОЛЬЦО КОЭФФИЦИЕНТОВ ФИЛЬТРА + TONE-ПУТЬ СРЕЗА
//                      (FLT BASE/WDTH/HPQ/LPQ + фильтровая огибающая)
//                      Monomachine OS 1.32B, ядро DSP1
// ----------------------------------------------------------------------------
// Две декомпилированные части:
//
// ОПРЕДЕЛЕНИЕ (окончательное, владелец, mnm_16): «кольцо» здесь = КОЛЬЦО
// КОЭФФИЦИЕНТОВ ФИЛЬТРА — ступень P:$0573–$05A2, которая из BASE/WDTH через
// kDiv1/kDiv2 и 24-шаговое деление считает пары срезов; выход — модуль-4
// кольцо регистров Y:$04–$07. Название совпало с FX-машиной RINGMOD, к ней
// блок отношения НЕ имеет; RINGMOD (эффекты) не трогается вообще.
//
// [A] КОЛЬЦО (P:$0573-$059A, ступень целиком до $05A2) — питает каскад 1 (func_000340):
//       r0 = (baseWord·$4AF·2)>>24   [P:$0576 mpyi #>$4af]  0..1199
//       r2 = (widthWord·$80·2)>>24   [P:$057A mpyi #>$80]   0..127
//       avg  = (s24(kDiv1[r0]) + s24(kDiv2[r2])) >> 1           [P:$0581-0585]
//       div24: q0 = 1.0 / avg (24 шага)                         [P:$0583-0588]
//       eps56 = (q0·dif·2) << 6,  dif = слово (kDiv2−kDiv1)     [P:$0589-058F]
//       Y:$05 = limit24(eps56)  — ЛИМИТЕР FM §5.4.1.2!          [P:$0591]
//       Y:$04 = limit24( (−c2·Y05·2 − c2·2^24) >> 1 )           [P:$0592-0594/0599]
//             = −c2·(0.5 + Y05/2)  — asr a (P:$0594) = защитное деление
//       Y:$06 = kWidth1[r2], Y:$07 = kWidth2[r2]                [P:$0595/0597]
//     P:$0594 (asr a #$93,r1) — арифметический сдвиг аккумулятора вправо
//     на 1 ПЕРЕД store; без него Y04 ровно ×2 (поймано трассировкой
//     diag_ring_0592.py 2026-10-04: A=-800098FF61F4 → asr → -40004C7FB0FA
//     → a1 = BFFFB3).
//     ЛИМИТЕР (FM §5.4.1.2): перенос аккумулятора в x0/память насыщает
//     слово при выходе 56-бит значения за 24-битный домен. В кольце он
//     срабатывает ДВАЖДЫ: (1) eps-слово P:$0591 — при больших WDTH разница
//     div2−div1 ≈ 8·10^6 и eps56 ≈ 5·10^8 ≫ 2^24 → Y05 зажимается в
//     $7FFFFF, Y04 ≈ −1.0 (зона максимального расхождения пары SVF);
//     (2) store Y04 P:$0599 — страховка на границе домена.
//     Без лимитера сетка BASE×WDTH расходилась с ядром в 15/30 точках
//     (sweep_ring_grid.py), с ним — бит-в-бит.
//     Репликация верифицирована: probe_eps.py 9/9 (WDTH=0, малые eps) +
//     sweep_ring_grid.py 30/30 точек эмулятора на сетке BASE×WDTH.
//     Попутно ядро публикует q0/eps в X:$93/$94/$96/$97 (скретч r1/r0,
//     P:$059C-05A0) — вне потока данных порта, не моделируется.
//
// [B] TONE-ПУТЬ (P:$0537-$0572 + P:$05FF-$0649 + P:$07D2-$07DB) — индекс среза:
//       idx = (BASEword·$800·2>>24) − $80                       [P:$0538-053A]
//           + envTerm(BOFS) = 4·$700·env·|BOFS−0.5|² (макс ≈447 слов)
//           + (WDTHword·$800·2>>24)                             [P:$055A]
//           + envTerm(WOFS)                                     [P:$055C-056A] (*)
//           ± трекинг (X:(V-$27), флаги bit11/bit9 Y:(V-$03))   [P:$054F-055A](**)
//       кламп СУММЫ (BASE+BOFS-часть) на $700                   [P:$054D-054E]
//       q8-сглаживание: idx += q8(state − idx)                  [P:$056B-0572]
//       clamp 0..$6A3 (1699) → kCutoff[$143546]                 [P:$07D2-07D9]
//     Домен A1 (24-бит слов) подтверждается согласованностью клампов:
//     $700 (1792) — кламп ветки BASE+BOFS, $63F (1599) — kTbl2, $6A3 (1699) — kCutoff.
//
// (*) В листинге результат WOFS-ветки (P:$0569 mac y1,x1,b) после `clr b ifmi`
//     перезаписывается загрузкой состояния (P:$056B move y:(r6+$10),b) — точная
//     регистровая разводка WOFS в индекс LP остаётся OPEN; здесь WOFS входит в
//     idx той же математикой, что и BOFS, что соответствует ERRATA
//     (|V-$1A + $C00000|·$700 для BOFS/WOFS, P:$053E-054B), мануалу
//     (WOFS = env-добавка к WDTH) и описанию поведения реальной машины.
//
// (**) Трекинг: mac x1,y0/a с x1 = X:(V-$27) (0..$5800), y0 = 8 [P:$054F].
//      Вклад в домен A1 = (pitchAcc·16)>>24 — мал при сырых словах; шкала
//      питч-аккумулятора на входе этой ветки — OPEN. Листинг ветки — в
//      reference/kernel_P0537-05A2_coeff_ring.txt.
//
// СВЯЗЬ ENV С КОЛЬЦОМ: в ядре env-члены входят в индекс среза ДО кольца; в
// порту они прибавляются к словам BASE/WDTH до отображения ·$4AF/·$80
// (эквивалентное доменное преобразование, помечено как PORT WIRING).
// Само кольцо и каскад — бит-точные.
// ============================================================================
#pragma once
#include "mnm_dsp56300_math.h"
#include "mnm_firmware_tables.h"
#include <cstdlib>

namespace mnm {

static constexpr int kFrame = 16;   // родная сетка прошивки: 16 сэмплов/блок

// ---------------------------------------------------------------------------
// [A] Кольцо коэффициентов каскада 1 — бит-точная репликация P:$0573-$059A
// ---------------------------------------------------------------------------
struct FilterRing { float c[4]; };   // c[0]=Y:$04 (f), c[1]=Y:$05 (eps), c[2..3]=Y:$06/$07

inline FilterRing computeRing(uint32_t baseWord, uint32_t wdthWord) noexcept
{
    int idx  = (int)(((uint64_t)baseWord * 1199ull) >> 23);           // P:$0576
    idx = idx > 1200 ? 1200 : (idx < 0 ? 0 : idx);
    int widx = (int)(((uint64_t)wdthWord * 128ull) >> 23);            // P:$057A
    widx = widx > 127 ? 127 : (widx < 0 ? 0 : widx);

    // avg = (div1+div2)>>1 — ЗНАКОВАЯ сумма, арифметический сдвиг
    // [P:$0581-0585: 56-бит add/asr, слово в x1]
    const int32_t avgS = (sext24(kDiv1[(size_t)idx] & 0xFFFFFFu)
                        + sext24(kDiv2[(size_t)widx] & 0xFFFFFFu)) >> 1;

    // 24-шаговое DIV: 1.0 / avg  [P:$0583 move #>1,a … P:$0588 div x1,a]
    const uint64_t D = div24(1ull << 24, avgS);
    const int32_t q0 = (int32_t)(D & 0xFFFFFFu);                      // a0 = частное

    // eps: 56-бит (q0·dif·2)<<6; слово — ЧЕРЕЗ ЛИМИТЕР [P:$0591 move b,x0]
    // разница — как ЗНАКОВОЕ 24-битное слово (56-бит sub, слово в x0, $058D)
    const int32_t difW = sext24((uint32_t)((kDiv2[(size_t)widx] - kDiv1[(size_t)idx]) & 0xFFFFFFu));
    int64_t b = (int64_t)sext24((uint32_t)q0) * (int64_t)difW * 2;    // mpy <<1
    b <<= 6;                                                          // asl #$6 (|b| < 2^54, без заворота 56 бит)
    const uint32_t epsW = limit24(b);
    const float eps = q23ToF(epsW);

    FilterRing r;
    // Y:$04 [P:$0592-0594/0599]: A = (−c2)·Y05·2 (mpy -y0,x0,a) − c2·2^24
    // (sub y0,a), затем ASR A >> 1 (P:$0594!) — store ЧЕРЕЗ ЛИМИТЕР ($0599).
    const int32_t c2s = sext24(kCoeff2[(size_t)idx] & 0xFFFFFFu);
    int64_t A = (int64_t)-c2s * (int64_t)sext24(epsW) * 2;            // mpy -y0,x0,a
    A -= (int64_t)c2s << 24;                                          // sub y0,a
    A >>= 1;                                                          // P:$0594 asr a
    r.c[0] = q23ToF(limit24(A));                                      // 0599 move a,y:(r4)+ — лимитер
    r.c[1] = eps;                                     // Y:$05 [P:$059A move x0,y:(r4)+]
    r.c[2] = q23ToF(kWidth1[(size_t)widx]);           // Y:$06 [P:$0595 x:(r2+$1444c6)]
    r.c[3] = q23ToF(kWidth2[(size_t)widx]);           // Y:$07 [P:$0597 x:(r2+$144546)]
    return r;
}

// ---------------------------------------------------------------------------
// [B] TONE-ПУТЬ: индекс среза с огибающей — P:$0537-$0572
// ---------------------------------------------------------------------------

// env-член: 4·$700·env·|p−0.5|² — точные A1-единицы P:$053E-$054B.
//   $053F add #>$c00000 (−0.5 в Q1.23); $0541 abs; $0545 mpy (·2>>24);
//   $0546 asl #$2; $0549 mpy env (x:(r7-$1)); $054B mpy #$700.
// Максимум (env=1, |p−0.5|=0.5) ≈ 447 слов. Кламп $700 (P:$054D-054E)
// применяется ВЫЗЫВАЮЩИМ к СУММЕ 16·BASE−128 + член (см. computeToneIndex).
inline int32_t envTerm(uint32_t paramWord, float envPhase) noexcept
{
    // p + (−0.5) в словах Q1.23 (заворот по 24 бита — как add в ядре)
    const int32_t aW = sext24((uint32_t)(((int32_t)(paramWord & 0xFFFFFFu)) + (int32_t)0xC00000));
    const int32_t absW = aW < 0 ? -aW : aW;                       // $0541 abs
    int32_t sq = mpyA1(absW, absW);                               // $0545
    sq = sq << 2;                                                 // $0546 asl #$2
    int32_t envW = (int32_t)(envPhase * 8388608.0f);              // x:(r7-$1) фаза
    if (envW > 0x7FFFFF) envW = 0x7FFFFF;                         // насыщение Q1.23
    if (envW < -0x800000) envW = -0x800000;
    sq = mpyA1(sq, envW);                                         // $0549
    sq = mpyA1(sq, 0x700);                                        // $054B
    return sq;
}

// полный индекс среза (A1-единицы) — P:$0537-$0572
struct TonePathState
{
    int32_t slewState = 0;    // Y:(V-$18) — q8-состояние [P:$056B]
    int32_t pitchAcc  = 0;    // X:(V-$27) — сырые слова 0..$5800 (2048 шаг/октава)
};
struct TrackingFlags { bool hpTrack = true; bool lpTrack = true; };  // bit11/bit9 Y:(V-$03)

inline int32_t computeToneIndex(uint32_t baseWord, uint32_t wdthWord,
                                uint32_t bofsWord, uint32_t wofsWord,
                                float envPhase, const TrackingFlags& tf,
                                TonePathState& st) noexcept
{
    // $0538-053A: (BASE·$800·2)>>24 − $80
    int32_t a = mpyA1((int32_t)(baseWord & 0xFFFFFFu), 0x800) - 0x80;
    a += envTerm(bofsWord, envPhase);                       // $054C add b,a
    if (a > 0x700) a = 0x700;                               // $054D-054E (ветка BASE+BOFS)
    // $054F-0556: трекинг HP (bit11): mac x1,y0,a → вклад (pitchAcc·16)>>24 в A1
    if (tf.hpTrack) a += mpyA1(st.pitchAcc, 8);
    a += mpyA1((int32_t)(wdthWord & 0xFFFFFFu), 0x800);     // $55A +WDTH·$800
    // $558-559: mac -x1,y0,a ifcc — bit9==0 → трекинг вычитается (LP-ветка)
    if (!tf.lpTrack) a -= mpyA1(st.pitchAcc, 8);
    a += envTerm(wofsWord, envPhase);                       // $055C-0569 (*)
    if (a < 0) a = 0;                                       // $056A clr b ifmi
    // $056B-0572: idx += q8(state − idx); q8 = округление дельты до 1/256
    const int32_t d = st.slewState - a;
    const int32_t q = (d >= 0) ? (((d + 128) >> 8) << 8) : -(((-d + 128) >> 8) << 8);
    a += q;
    if (a < 0) a = 0;
    st.slewState = a;
    return a;
}

// ---------------------------------------------------------------------------
// Связка с DIST: c-кривая умножает kCutoff[idx] (P:$07B9: mpy x1,y0,b;
// P:$07D2-07DB: clamp $6A3 → x:(r0+$143546) → mpy y1,y0)
// ---------------------------------------------------------------------------
inline float cutoffCoefficient(int32_t toneIdx, float driveC) noexcept
{
    int32_t idx = toneIdx;
    if (idx < 0)    idx = 0;
    if (idx > 0x6A3) idx = 0x6A3;                        // P:$07D2-07D6 (clamp 1699)
    const float base = q23ToF(kCutoff[(size_t)idx]);     // P:$07D9 x:(r0+$143546)
    return base * driveC;                                // P:$07DB mpy y1,y0
}

} // namespace mnm
