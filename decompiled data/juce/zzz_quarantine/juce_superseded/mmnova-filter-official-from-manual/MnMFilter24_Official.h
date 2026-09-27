#pragma once
#include "MnMFixed.h"
#include <cmath>
#include <algorithm>

// ============================================================================
//  MnMFilter24_Official.h — ЭТАЛОННЫЙ ФИЛЬТР MONOMACHINE OS 1.32B
//  Математика и параметры строго по официальному мануалу Elektron (стр. 29-31):
//
//  1. Закон изменения среза: ровно 8 шагов ручки = 1 октава (2.0x частоты)!
//  2. Привязка к ноте: при BASE=0 срез HP равен Note_Freq / 4 (-2 октавы).
//  3. WDTH задаёт расстояние от HP до LP среза (LP = BASE + WDTH).
//  4. HPQ и LPQ — раздельные резонансы для каждой секции фильтра.
//  5. Собственная огибающая фильтра (ATK, DEC) модулирует BASE и WDTH
//     через коэффициенты BOFS (Base Offset) и WOFS (Width Offset).
// ============================================================================

namespace mmnova {

class MonomachineFilterOfficial
{
public:
    struct Params
    {
        int base = 0;   // 0..127: HP Cutoff (8 шагов = 1 октава)
        int wdth = 127; // 0..127: Расстояние до LP Cutoff
        int hpq  = 0;   // 0..127: Резонанс High-Pass
        int lpq  = 0;   // 0..127: Резонанс Low-Pass
        int atk  = 0;   // 0..127: Filter Envelope Attack
        int dec  = 64;  // 0..127: Filter Envelope Decay
        int bofs = 64;  // 0..127: Base Offset (-64..+63, 64=0)
        int wofs = 64;  // 0..127: Width Offset (-64..+63, 64=0)
    };

    void reset() noexcept
    {
        hpLo = hpBp = lpLo = lpBp = 0.0;
        envVal = 0.0;
        attacking = true;
    }

    // Триггерится от FILTER-TRIG (нота или триг-трек секвенсора)
    void trigger() noexcept
    {
        envVal = 0.0;
        attacking = true;
    }

    // Обработка блока из 16 сэмплов (аппаратный квант DSP56303)
    void processBlock16 (s24* monoBlock, const Params& p, double noteFreq, double sampleRate = 44100.0) noexcept
    {
        // 1. Тик огибающей фильтра (Filter Envelope, 2-фазная AD)
        if (attacking) {
            envVal += 1.0 / (1.0 + p.atk * 12.0);
            if (envVal >= 1.0) { envVal = 1.0; attacking = false; }
        } else {
            envVal -= 1.0 / (1.0 + p.dec * 16.0);
            if (envVal < 0.0) envVal = 0.0;
        }

        // 2. Модуляция BASE и WDTH через BOFS и WOFS
        const double bofsShift = (p.bofs - 64) * 2.0; // сдвиг в шагах ручки
        const double wofsShift = (p.wofs - 64) * 2.0;
        const double effBase = std::clamp (p.base + envVal * bofsShift, 0.0, 127.0);
        const double effWdth = std::clamp (p.wdth + envVal * wofsShift, 0.0, 127.0);

        // 3. Официальная формула частот среза Elektron:
        //    BASE=0 -> на 2 октавы ниже ноты (noteFreq * 0.25)
        //    Каждые 8 шагов = +1 октава (фактор 2.0):
        const double fHpHz = (noteFreq * 0.25) * std::pow (2.0, effBase / 8.0);
        const double fLpHz = fHpHz * std::pow (2.0, effWdth / 8.0);

        // Нормализация коэффициентов интегратора Chamberlin SVF
        const double fHp = std::clamp (2.0 * std::sin (3.1415926535 * std::min (fHpHz, sampleRate * 0.45) / sampleRate), 0.001, 0.98);
        const double fLp = std::clamp (2.0 * std::sin (3.1415926535 * std::min (fLpHz, sampleRate * 0.45) / sampleRate), 0.001, 0.98);

        // 4. Демпфирование (резонанс) с учётом аппаратной константы $F528BD (-0.084694564):
        const double kHwFb = -0.084694564;
        const double qHp = std::clamp (2.0 + kHwFb - (p.hpq / 127.0) * 1.88, 0.05, 2.0);
        const double qLp = std::clamp (2.0 + kHwFb - (p.lpq / 127.0) * 1.88, 0.05, 2.0);

        // 5. Каскадная фильтрация 16 сэмплов (HP секция -> LP секция = 24dB multi-mode)
        for (int i = 0; i < 16; ++i)
        {
            double x = toFloat (monoBlock[i]);

            // Секция 1: High-Pass (отсекает всё ниже точки BASE)
            hpLo += fHp * hpBp;
            const double hpHi = x - hpLo - qHp * hpBp;
            hpBp += fHp * hpHi;
            hpBp = std::clamp (hpBp, -1.8, 1.8);
            x = hpHi; // выход HP

            // Секция 2: Low-Pass (отсекает всё выше точки BASE + WDTH)
            lpLo += fLp * lpBp;
            const double lpHi = x - lpLo - qLp * lpBp;
            lpBp += fLp * lpHi;
            lpBp = std::clamp (lpBp, -1.8, 1.8);
            x = lpLo; // выход LP

            monoBlock[i] = fromFloat (std::clamp (x, -1.0, 1.0));
        }
    }

private:
    double hpLo { 0.0 }, hpBp { 0.0 };
    double lpLo { 0.0 }, lpBp { 0.0 };
    double envVal { 0.0 };
    bool   attacking { true };
};

} // namespace mmnova
