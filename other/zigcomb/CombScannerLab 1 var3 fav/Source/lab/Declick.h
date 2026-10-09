#pragma once

// Общие утилиты движков: де-кликер прыжков времени задержки и мягкий лимитер.
//
// Идея де-кликера: позиция чтения меняется МГНОВЕННО (рваный характер, как в
// оригинальном патче), но разрыв волны маскируется коротким кроссфейдом (~5 мс)
// между старой и новой позицией. Никакого сглаживания времени во времени —
// именно оно превращало задержку в "тейп-делей" с питч-глиссандо.
//
// Мягкий лимитер повторяет роль omx.peaklim~ из оригинальной схемы: комб может
// заводиться (это и нужно), но не может уйти в разнос и "громко зашуметь".
// Ниже порога лимитер прозрачен, выше — C1-непрерывно ограничивает.

#include <algorithm>
#include <cmath>

namespace lab
{
struct DeclickTap
{
    float pos  = 1.0f;   // текущая (целевая) позиция чтения, в сэмплах
    float prev = 1.0f;   // предыдущая позиция (для кроссфейда)
    float xf   = 1.0f;   // 0..1; 1 = кроссфейд завершён

    void reset (float position) noexcept
    {
        pos = prev = position;
        xf = 1.0f;
    }

    // Вызывается КАЖДЫЙ сэмпл с текущей целью (в сэмплах).
    //  * плавное движение (медленнее minJump за сэмпл) — позиция едет за целью точно,
    //    без задержки и без кроссфейда: это и есть «живое» модулирование оригинала;
    //  * скачок (быстрее minJump за сэмпл: крутёжка ручки, смена модели) — позиция
    //    становится новой СРАЗУ, а разрыв волны прячет кроссфейд ~5 мс.
    // Возвращает true, если начался новый кроссфейд.
    bool retarget (float target, float minJump) noexcept
    {
        const float delta = target - pos;

        if (std::fabs (delta) <= minJump)
        {
            pos += delta;      // == target; следуем точно
            prev += delta;     // если идёт кроссфейд — оба его конца едут вместе с целью
            return false;
        }

        const float rendered = prev + (pos - prev) * xf;   // что реально звучит сейчас
        prev = rendered;
        pos = target;
        xf = std::fabs (target - rendered) > minJump ? 0.0f : 1.0f;
        return xf < 1.0f;
    }

    void advance (float step) noexcept { xf = std::min (1.0f, xf + step); }
};

// C1-непрерывный мягкий лимитер: ниже threshold — прозрачен, выше — плавно
// подходит к threshold + 1 (то есть порог 0.9 даёт максимум ~1.9).
inline float softLimit (float x, float threshold) noexcept
{
    const float ax = std::fabs (x);

    if (ax <= threshold)
        return x;

    const float sign = x < 0.0f ? -1.0f : 1.0f;
    return sign * (threshold + (1.0f - std::exp (-(ax - threshold))));
}
}
