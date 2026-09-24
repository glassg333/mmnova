#pragma once
#include "MnMFixed.h"

// ============================================================================
//  Страница FILTER: BASE WIDTH HPQ LPQ ATK DEC BOFS HOFS
//  CC трека 1:      72   73    74  75  76  77  78   79
//
//  Мануал: "resonant 24dB low/high/band-pass filter with envelope and
//           individual Q-control"
//
//  Ядро (KERNEL_VOICE_CHAIN.md §4.6):
//    две одинаковые петли  do #<$8  (8 итераций x 2 аккумулятора = 16 смп)
//      петля 1: P:$05D3 - $05E8
//      петля 2: P:$05EB - $05FA
//    x0 = $F528BD, x1 = $4A4DF0
//    состояния X:$71+n*3 (банк a), X:$73+n*3 (банк b), n=0..7
//
//  Репо §9 п.3 спрашивало: две петли это L/R или LP/HP?
//  Вывод: LP/HP. На панели раздельные HPQ и LPQ — раздельный резонанс
//  требует двух независимых секций. Мануал: "individual Q-control".
// ============================================================================

namespace mmnova {

class Filter24
{
public:
    // ---- [ТИР A] точные коэффициенты, пересчитанные от hex ----------------
    //  $F528BD = 16066749 - 2^24 = -710467 ;  -710467 / 2^23 = -0.084694564
    //  $4A4DF0 = 4869616          ;  4869616 / 2^23 = +0.580503464
    //  (в доке репо стоят округлённые -0.08415 / 0.58056 — не бери их)
    static constexpr s24    kHwA_F528BD = -0x0AD743;      // сырое 24-бит
    static constexpr s24    kHwB_4A4DF0 =  0x4A4DF0;
    static constexpr double kA = -0.084694564;
    static constexpr double kB =  0.580503464;

    void reset() noexcept
    {
        hpLo = hpBp = 0;
        lpLo = lpBp = 0;
    }

    // ------------------------------------------------------------------
    //  [ТИР C] Пересчёт BASE/WIDTH в коэффициент среза — ЗАГЛУШКА.
    //  В железе это НЕ tan(), а 24-битное деление (§4.5):
    //    P:$586-$588  do #<$18 ; div
    //    делимое  = TBL[X:$143F95 + x0*$4AF] + TBL[X:$144446 + y0*$80]
    //    делитель = (a >> 1)
    //  Обе таблицы в репо не выгружены. Пока стоит аналитика.
    //  Когда задампишь — заменяется только эта функция.
    // ------------------------------------------------------------------
    static double cutoffCoeff (int knob7, double sr) noexcept
    {
        const double n = std::clamp (knob7, 0, 127) / 127.0;
        const double hz = 20.0 * std::pow (1000.0, n);              // ЗАГЛУШКА
        return std::clamp (2.0 * std::sin (3.14159265358979 * std::min (hz, sr * 0.45) / sr),
                           0.002, 0.98);
    }

    // BASE = нижний край полосы, WIDTH = ширина до верхнего края.
    // HPQ и LPQ независимы — это и есть смысл двух секций.
    void setParams (int base7, int width7, int hpq7, int lpq7, double sr) noexcept
    {
        fHp = cutoffCoeff (base7, sr);
        fLp = cutoffCoeff (std::min (127, base7 + width7), sr);

        // демпфирование = 1/Q. Чем больше Q-ручка, тем меньше демпфирование.
        // [ТИР B] в железе демпфирование приходит из Y:$91+ уже готовым;
        // здесь коэффициент kA использован как нижняя граница, как в ядре.
        qHp = std::clamp (2.0 + kA - (std::clamp (hpq7,0,127) / 127.0) * 1.85, 0.05, 2.0);
        qLp = std::clamp (2.0 + kA - (std::clamp (lpq7,0,127) / 127.0) * 1.85, 0.05, 2.0);
    }

    // ------------------------------------------------------------------
    //  Блок 16 сэмплов. В железе это 8 итераций по 2 аккумулятора (A и B
    //  идут параллельно), поэтому порядок обновления состояния здесь
    //  воспроизведён парами, а не по одному сэмплу.
    // ------------------------------------------------------------------
    void processBlock16 (s24* buf) noexcept
    {
        for (int i = 0; i < 8; ++i)          // do #<$8
        {
            buf[i * 2 + 0] = one (buf[i * 2 + 0]);
            buf[i * 2 + 1] = one (buf[i * 2 + 1]);
        }
    }

private:
    inline s24 one (s24 in) noexcept
    {
        double x = toFloat (in);

        // --- секция 1: HP (петля P:$05D3-$05E8) ---
        hpLo += fHp * hpBp;
        const double hpHi = x - hpLo - qHp * hpBp;
        hpBp += fHp * hpHi;
        hpBp  = std::clamp (hpBp, -1.6, 1.6);
        x = hpHi;                            // берём highpass-выход

        // --- секция 2: LP (петля P:$05EB-$05FA) ---
        lpLo += fLp * lpBp;
        const double lpHi = x - lpLo - qLp * lpBp;
        lpBp += fLp * lpHi;
        lpBp  = std::clamp (lpBp, -1.6, 1.6);
        x = lpLo;                            // берём lowpass-выход

        return fromFloat (std::clamp (x, -1.0, 1.0));
    }

    double fHp { 0.2 }, fLp { 0.5 };
    double qHp { 1.2 }, qLp { 1.2 };
    double hpLo { 0 }, hpBp { 0 };
    double lpLo { 0 }, lpBp { 0 };
};

} // namespace mmnova
