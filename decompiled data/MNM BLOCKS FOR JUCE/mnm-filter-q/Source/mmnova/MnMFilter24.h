#pragma once
#include "MnMFixed.h"

// ============================================================================
//  FILT page: BASE WDTH HPQ LPQ ATK DEC BOFS WOFS       (0..127 каждый)
//  Имена канонические — enum Parameter из monomachine_engine.h:
//    FilterBase, FilterWidth, FilterHighPassQ, FilterLowPassQ,
//    FilterAttack, FilterDecay, FilterBaseOffset, FilterWidthOffset
//
//  Страничный блок голоса: Y:$500..$527   (kVoicePageYAddress = 0x000500,
//  V = Y:$528, параметры в V-$28..V-$01 — сходится точно)
//
//  Ядро (KERNEL_VOICE_CHAIN.md §4.6):
//    две петли do #<$8 (8 итераций x 2 аккумулятора = 16 сэмплов)
//      HP: P:$05D3-$05E8      LP: P:$05EB-$05FA
//    x0 = $F528BD = -0.084694564    x1 = $4A4DF0 = +0.580503464
//    состояния X:$71+n*3 / X:$73+n*3,  коэффициенты из Y:$91+
//
//  Почему две петли = HP и LP, а не L/R: на панели РАЗДЕЛЬНЫЕ HPQ и LPQ,
//  мануал — "individual Q-control". Раздельный резонанс требует двух
//  независимых секций. Это закрывает открытый вопрос §9 п.3 доки репо.
// ============================================================================

namespace mmnova {

class Filter24
{
public:
    // ---- [A] точные константы петли, пересчитаны от hex -------------------
    //  $F528BD = 16066749 - 2^24 = -710467 ; -710467 / 2^23 = -0.084694564
    //  $4A4DF0 = 4869616                   ;  4869616 / 2^23 = +0.580503464
    //  ВНИМАНИЕ: это НЕ ручки Q. Это зашитые в опкоды структурные
    //  константы SVF. Сами Q приходят из Y:$91+, куда их кладёт
    //  блок P:$537-$5D2 ДО петли.
    static constexpr double kA = -0.084694564;   // $F528BD
    static constexpr double kB =  0.580503464;   // $4A4DF0

    void reset() noexcept { hpLo = hpBp = lpLo = lpBp = 0; envPhase = 0; envVal = 0; }

    // ------------------------------------------------------------------
    //  [C] ЗАГЛУШКА. Реальная кривая 0..127 -> q НЕ извлечена.
    //  Снимается измерением: см. PLAN_MEASURE.md шаг 2.
    //  Когда снимешь 128 значений —替 заменить тело на табличный lookup.
    // ------------------------------------------------------------------
    static double qFromKnob (int knob7) noexcept
    {
        const double n = std::clamp (knob7, 0, 127) / 127.0;
        // нижняя граница взята как 2.0 + kA, чтобы при knob=0 демпфирование
        // совпадало со структурной константой петли
        return std::clamp (2.0 + kA - n * 1.85, 0.05, 2.0);
    }

    // ------------------------------------------------------------------
    //  [C] ЗАГЛУШКА. В железе срез НЕ через tan(), а 24-битным делением:
    //    P:$586-$588  do #<$18 ; div
    //    делимое  = TBL[X:$143F95 + x0*$4AF] + TBL[X:$144446 + y0*$80]
    //    делитель = (a >> 1)
    //  Обе таблицы в репо не выгружены (107 и 128 слов).
    //  Альтернатива дампу: снять готовые f прямо из Y:$91+ (шаг 3).
    // ------------------------------------------------------------------
    static double cutoffCoeff (int knob7, double sr) noexcept
    {
        const double n = std::clamp (knob7, 0, 127) / 127.0;
        const double hz = 20.0 * std::pow (1000.0, n);
        return std::clamp (2.0 * std::sin (3.14159265358979
                          * std::min (hz, sr * 0.45) / sr), 0.002, 0.98);
    }

    // ------------------------------------------------------------------
    //  [B] Модулятор. Гипотеза: блок P:$506-$536, который док называет
    //  "LFO-фаза", на самом деле огибающая фильтра. Док сам помечает
    //  назначение этих веток открытым вопросом (§9 п.2).
    //  BOFS/WOFS биполярные: формула |x + $C00000| = |x - 0.5| в 24 битах.
    //  Форма огибающей [C] не установлена, здесь простая AD.
    // ------------------------------------------------------------------
    void trigger() noexcept { envPhase = 0.0; envVal = 0.0; attacking = true; }

    void tickEnvBlock (int atk7, int dec7, double sr) noexcept
    {
        const double blocksPerSec = sr / 16.0;        // огибающая идёт на блок
        if (attacking)
        {
            const double rate = 1.0 / (1.0 + std::clamp (atk7,0,127) * 8.0);
            envVal += rate;
            if (envVal >= 1.0) { envVal = 1.0; attacking = false; }
        }
        else
        {
            const double rate = 1.0 / (1.0 + std::clamp (dec7,0,127) * 8.0);
            envVal -= rate;
            if (envVal < 0.0) envVal = 0.0;
        }
        (void) blocksPerSec;
    }

    // base7/width7 0..127 ; bofs7/wofs7 0..127 где 64 = ноль (биполярные)
    void setParams (int base7, int width7, int hpq7, int lpq7,
                    int bofs7, int wofs7, double sr) noexcept
    {
        const double bofs = (std::clamp (bofs7,0,127) - 64) / 64.0;   // -1..+1
        const double wofs = (std::clamp (wofs7,0,127) - 64) / 64.0;

        const int baseMod  = (int) std::lround (std::clamp (
                                base7  + envVal * bofs * 127.0, 0.0, 127.0));
        const int widthMod = (int) std::lround (std::clamp (
                                width7 + envVal * wofs * 127.0, 0.0, 127.0));

        fHp = cutoffCoeff (baseMod, sr);
        fLp = cutoffCoeff (std::min (127, baseMod + widthMod), sr);
        qHp = qFromKnob (hpq7);      // отдельный Q для HP-секции
        qLp = qFromKnob (lpq7);      // отдельный Q для LP-секции
    }

    // 16 сэмплов = 8 итераций x 2 аккумулятора, как do #<$8 в железе
    void processBlock16 (s24* buf) noexcept
    {
        for (int i = 0; i < 8; ++i)
        {
            buf[i*2+0] = one (buf[i*2+0]);
            buf[i*2+1] = one (buf[i*2+1]);
        }
    }

private:
    inline s24 one (s24 in) noexcept
    {
        double x = toFloat (in);

        // HP-секция, петля P:$05D3-$05E8
        hpLo += fHp * hpBp;
        const double hpHi = x - hpLo - qHp * hpBp;
        hpBp += fHp * hpHi;
        hpBp  = std::clamp (hpBp, -1.6, 1.6);
        x = hpHi;

        // LP-секция, петля P:$05EB-$05FA
        lpLo += fLp * lpBp;
        const double lpHi = x - lpLo - qLp * lpBp;
        lpBp += fLp * lpHi;
        lpBp  = std::clamp (lpBp, -1.6, 1.6);
        x = lpLo;

        return fromFloat (std::clamp (x, -1.0, 1.0));
    }

    double fHp{0.2}, fLp{0.5}, qHp{1.2}, qLp{1.2};
    double hpLo{0}, hpBp{0}, lpLo{0}, lpBp{0};
    double envPhase{0}, envVal{0};
    bool   attacking{false};
};

} // namespace mmnova
