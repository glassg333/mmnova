// ============================================================================
// MnmFixed.h — арифметика DSP56300 в терминах C++ (Q1.23 / 24-бит слова)
// Часть референс-пака mnmdsp (Elektron Monomachine SFX-60 MKII OS 1.32B)
//
// Зачем: весь нативный DSP Monomachine работает на 24-бит словах с
// 56-бит аккумулятором и сатурацией (bset #$14,sr). Чтобы порт «звучал как
// оригинал», арифметика должна повторять эти правила, а не float.
//
// Соответствие железу:
//   слово      = int32_t в диапазоне [-$800000, $7FFFFF], 1.0 == $800000
//   MAC        = int64_t аккумулятор, результат saturate -> слово
//   mpy x0,x1  = слово×слово >> 23 (дробный режим DSP56300)
//   asl/asr    = сдвиги с 56-бит точностью до записи
// ============================================================================
#pragma once
#include <cstdint>

namespace mnmdsp {

static constexpr int32_t  kOne = 0x800000;   // 1.0 в Q1.23
static constexpr int32_t  kMax = 0x7FFFFF;
static constexpr int32_t  kMin = -0x800000;

// Сатурация слова (эквивалент bset #$14,sr — ограничение при записи)
static inline int32_t sat24(int64_t v) {
    if (v >  0x7FFFFF) return  0x7FFFFF;
    if (v < -0x800000) return -0x800000;
    return (int32_t)v;
}

// Умножение слов: mpy x0,x1,a -> a1 = x0*x1 >> 23
static inline int32_t mpy(int32_t a, int32_t b) {
    return (int32_t)(((int64_t)a * (int64_t)b) >> 23);
}

// MAC в 56-бит аккумуляторе: держим Q46 до записи
struct Acc {
    int64_t v = 0;
    void    mac(int32_t a, int32_t b) { v += (int64_t)a * (int64_t)b; }
    void    add(int32_t w)            { v += (int64_t)w << 23; }
    void    sub(int32_t w)            { v -= (int64_t)w << 23; }
    void    addRaw(int64_t x)         { v += x; }
    int32_t word() const              { return sat24(v >> 23); }  // запись в память
};

// Однополюсный сглаживатель прошивки: y += c*(x-y)  (LP хоруса, DEP-смузер)
struct OnePole {
    int32_t y = 0;
    int32_t tick(int32_t x, int32_t c) {          // c = Q1.23
        y = sat24((int64_t)y + mpy(c, sat24((int64_t)x - y)));
        return y;
    }
};

// Конвертация питча -> фазовый инкремент через таблицу X:$140000.
// wt[i] = 0.5*2^(i/2048); 2048 ступеней/октава, 11 октав (acc 0..$5800).
// Формула kernel func_000262 (P:$2C6-$2EA), та же в BBOX/осцилляторах.
static inline int32_t pitchIncrement(const int32_t* pitchTable /*2048 слов*/, int acc) {
    if (acc < 0) acc = 0;
    if (acc > 0x5800) acc = 0x5800;
    int oct  = acc >> 11;
    int frac = acc & 0x7FF;
    int32_t w = pitchTable[frac] >> 10;                        // asr #$a
    int32_t s = (int32_t)((int64_t)w << oct);                  // asl x0 (октавы)
    return (int32_t)(((int64_t)s * 0x1D22A) >> 26);            // mpy $1D22A; asr #3; в слово
}

} // namespace mnmdsp
