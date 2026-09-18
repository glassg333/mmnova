#pragma once
#include "MnMFixed.h"

// ============================================================================
//  Страница AMPLIFICATION: ATK HOLD DEC REL DIST VOL PAN PORT
//  CC трека 1: 56  57   58  59  60   61  62  63
//
//  DIST это НЕ drive-секция. Мануал OS 1.32:
//    "increase your headroom by decreasing the DIST parameter"
//  То есть это регулятор запаса по головке в фиксированной точке.
//  Само искажение делает ALU, когда аккумулятор упирается в 24-битный
//  потолок при SR.SM (bset #$14,sr). Клип ЖЁСТКИЙ, не tanh.
// ============================================================================

namespace mmnova {

class AmpDist
{
public:
    // ------------------------------------------------------------------
    //  [ТИР C] КРИВАЯ НЕ ПРОВЕРЕНА.
    //  Знаю только якоря: display 0 -> множитель 1.0 (нейтраль),
    //  минус -> тише (больше headroom), плюс -> громче (клип).
    //  Форма между якорями НЕ извлечена: её считает ColdFire, а в DSP
    //  приходит уже готовое fixed-point число (репо §9 п.1).
    //
    //  Здесь стоит экспонента как ЗАГЛУШКА. Когда вскроешь ColdFire —
    //  меняется только это тело, остальной тракт не трогается.
    // ------------------------------------------------------------------
    static s24 distGainFromDisplay (int display /* -64..+63 */) noexcept
    {
        const int d = std::clamp (display, -64, 63);
        if (d == 0) return kOne24;                      // нейтраль = x1.0 [якорь A]
        const double g = std::pow (2.0, (double) d / 16.0);  // ЗАГЛУШКА [C]
        return fromFloat (std::clamp (g, 0.0, 7.999));
    }

    // Внутреннее представление 0..127, где 64 = display 0 (как у PAN)
    static s24 distGainFromRaw (int raw7 /* 0..127 */) noexcept
    {
        return distGainFromDisplay (std::clamp (raw7, 0, 127) - 64);
    }

    // ------------------------------------------------------------------
    //  [ТИР A] Сам клип. Это и есть "дисторшн" Monomachine.
    //  1476a5: bset #$14,sr  -> ALU клипует на $7FFFFF / $800000
    //  Никакого сглаживания в железе нет.
    // ------------------------------------------------------------------
    static inline s24 applyDistortion (s24 in, s24 gain) noexcept
    {
        // умножение в 56-битном аккумуляторе, затем sat24 = SR.SM
        const s56 acc = ((s56) in * (s56) gain) >> 23;
        return sat24 (acc);
    }

    // Готовый шаг: сигнал -> DIST -> VOL
    // [ТИР C] порядок DIST относительно фильтра НЕ установлен.
    // Ставится там, где тебе нужно, пока не вскрыты опкоды $04A8-$05FA.
    static inline s24 stage (s24 in, int distDisplay, int vol7) noexcept
    {
        const s24 g = distGainFromDisplay (distDisplay);
        const s24 d = applyDistortion (in, g);
        return fmul (d, paramWord (vol7));
    }
};

// ============================================================================
//  AMP-огибающая AHDR. [ТИР B] — состояния и таблицы из KERNEL_VOICE_CHAIN.md
//  §4.1, но САМИ ОПКОДЫ $04A8-$04F5 я не читал. Переходы достроены.
//
//   состояние 1 (attack) : Y:$4FF += TBL[Y:$141800 + (V-$10)>>16]
//   состояние 4 (decay)  : с участием (V-$0E)^2 и TBL[$141880+idx]
//   состояние 5          : Y:$4FF = (V-$0E)^2
//   состояние 2 (release): Y:$4FF -= TBL[$141880 + (V-$0D)>>16]
//   default              : Y:$4FF = $7FFFFF
//
//  Уровень Y:$04FF ГЛОБАЛЬНЫЙ на все голоса в данном сэмпл-блоке.
//  Monomachine это AHDR, а не ADSR: sustain заменён на HOLD.
// ============================================================================
class AmpEnvAHDR
{
public:
    enum State { Idle = 0, Attack = 1, Release = 2, Decay = 4, Hold = 5 };

    void noteOn () noexcept { st = Attack; held = 0; }
    void noteOff() noexcept { st = Release; }
    void reset  () noexcept { st = Idle; lvl = 0; held = 0; }
    bool active () const noexcept { return st != Idle || lvl > 0; }

    // rateTbl = Y:$141800 (128 слов), decTbl = Y:$141880 (128 слов).
    // Обе НЕ выгружены в репо — дампить mmnova_slice.py tables.
    s24 tick (const s24* rateTbl, const s24* decTbl,
              int atk, int hold, int dec, int rel) noexcept
    {
        switch (st)
        {
            case Attack:
                lvl = sat24 ((s56) lvl + rateTbl[atk & 0x7F]);
                if (lvl >= kOne24) { lvl = kOne24; st = Hold; held = 0; }
                break;

            case Hold:                       // HOLD вместо sustain
                lvl = kOne24;
                if (++held >= holdBlocks (hold)) st = Decay;
                break;

            case Decay:
                lvl = sat24 ((s56) lvl - fmul (decTbl[dec & 0x7F], lvl));
                if (lvl <= 0x000400) { lvl = 0; st = Idle; }
                break;

            case Release:
                lvl = sat24 ((s56) lvl - decTbl[rel & 0x7F]);
                if (lvl <= 0) { lvl = 0; st = Idle; }
                break;

            default:
                lvl = 0;
                break;
        }
        return lvl;
    }

    s24 level() const noexcept { return lvl; }

private:
    // [ТИР C] пересчёт HOLD 0..127 в число блоков не извлечён
    static int holdBlocks (int h) noexcept { return (h & 0x7F) * 8; }

    State st { Idle };
    s24   lvl { 0 };
    int   held { 0 };
};

} // namespace mmnova
