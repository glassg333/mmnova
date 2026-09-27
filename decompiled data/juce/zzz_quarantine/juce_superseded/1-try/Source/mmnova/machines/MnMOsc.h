// ============================================================================
//  MnMOsc.h — общий осцилляторный слой машин (48-битные фазовые аккумуляторы,
//  как L:$5 = пара X:$5/Y:$5 в прошивке).
// ============================================================================
#pragma once
#include "../MnMMachine.h"

namespace mmnova {

// pre-voice (P:$2C6-$2EA) отдаёт A48 = (0.5*2^(frac/2048) * $1D22A) << oct >> 3.
// Перевод этой величины в 48-битный инкремент фазы при 44.1 кГц:
//   для A4 (acc=$2600) A48 = 3.3696e12  ->  440 Гц
//   inc48 = A48 * 2^48 / (44100 * 3.3696e12 / 440) = A48 * 0.833545e0 * 1e-0
constexpr double kIncPerUnit = 8.33545e-1 * 1e-0;
constexpr double kGndSinRel  = 1.0;                 // $17C6F9 принят за 1.0

inline uint64_t oscIncrement (const BlockContext& c,
                              double machineRelScale = kGndSinRel,
                              s24    tuneWord        = 0,
                              double ratio           = 1.0) noexcept
{
    const int    t     = tuneWord ? paramIndex (tuneWord) : 64;
    const double tune  = std::pow (2.0, (t - 64) / 12.0);          // TUNE $40 = 0
    const double srAdj = kEngineRate / c.rate;
    const double v = (double) c.pitchInc * kIncPerUnit * machineRelScale * tune * ratio * srAdj;
    return (uint64_t) (v < 0.0 ? 0.0 : v);
}

// Дешёвый LCG — на железе GND-NOIS читает шумовую таблицу, здесь эквивалент по
// спектру (таблица дампится extract_tables_fixed.py, см. tables::noiseTable()).
struct Noise
{
    uint32_t s { 0x2545F491u };
    inline s24 next() noexcept
    {
        s ^= s << 13; s ^= s >> 17; s ^= s << 5;
        return (s24) ((int32_t) (s >> 8) - 0x800000);
    }
};

// Полоса-ограниченные примитивы (PolyBLEP) — используются там, где железо
// читает вейвтейблы SWAVE/DPRO. При наличии дампов таблиц ветка переключается
// на табличный путь (MMNOVA_HAVE_FW_TABLES).
inline double polyBlep (double t, double dt) noexcept
{
    if (t < dt)            { t /= dt;        return t + t - t * t - 1.0; }
    if (t > 1.0 - dt)      { t = (t - 1.0) / dt; return t * t + t + t + 1.0; }
    return 0.0;
}

struct PhaseOsc
{
    uint64_t phase { 0 };
    inline void  reset() noexcept { phase = 0; }
    inline void  advance (uint64_t inc) noexcept { phase += inc; }
    inline double norm() const noexcept { return (double) phase / 281474976710656.0; }
    inline s24   sine() const noexcept { return lerpTable<tables::kSineBits> (tables::sine(), phase); }
    inline s24   saw (uint64_t inc) const noexcept
    {
        const double t = norm(), dt = (double) inc / 281474976710656.0;
        return fromFloat ((2.0 * t - 1.0) - polyBlep (t, dt));
    }
    inline s24   pulse (uint64_t inc, double pw) const noexcept
    {
        const double t = norm(), dt = (double) inc / 281474976710656.0;
        double t2 = t + (1.0 - pw); if (t2 >= 1.0) t2 -= 1.0;
        double v = (t < pw ? 1.0 : -1.0) + polyBlep (t, dt) - polyBlep (t2, dt);
        return fromFloat (v * 0.9);
    }
};

} // namespace mmnova
