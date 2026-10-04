// ============================================================================
// mnm_fixed.h — точная семантика целочисленной арифметики DSP56300 (DSP56303)
// ----------------------------------------------------------------------------
// Источник фактов:
//   * mmnova "decompiled data/_docs/monomachine_os132b_reverse_engineering_status.txt",
//     раздел "4. DSP56300 ISA FACTS";
//   * mmnova decompiled data/juce/filter/mnm_filter_stage2_juce_port.h
//     ("bit-exactness notes": parallel-move stores latch PRE-ALU accumulator,
//     "add #$800000" adds -1.0, B1-quantization часть алгоритма);
//   * ядро OS 1.32B: dsp1_pmem.bin (общий код DSP1/DSP2 P:$0328-$0B24).
//
// Формат: 24-битные слова Q1.23 (дробь = signed/2^23), аккумулятор 56 бит
// (A2:A1:A0 = 8:24:24), MPY/MAC дают произведение СО СДВИГОМ ВЛЕВО на 1
// (компенсация знакового бита: Q1.23*Q1.23 -> Q1.47 в аккумуляторе).
// Насыщение слова: $7FFFFF / $800000 (флаг насыщения ALU; в прошивке включается
// bset #$14,sr / bset #$b,sr на критичных участках, например P:$07C3).
// ============================================================================
#pragma once
#include <cstdint>
#include <cstring>
#include <cmath>

namespace mnm {

// --- базовые константы ------------------------------------------------------
static constexpr int64_t  Q23_ONE   = 0x7FFFFF;      // +0.99999988
static constexpr int64_t  Q23_MINUS = -0x800000;     // -1.0
static constexpr uint64_t ACC56_MASK = 0xFFFFFFFFFFFFFFull; // 56 бит

// знаковое расширение 24-бит слова
static inline int32_t sext24(uint32_t w) noexcept
{
    w &= 0xFFFFFFu;
    return (w & 0x800000u) ? (int32_t)(w - 0x1000000u) : (int32_t)w;
}

// извлечение A1/B1 из 56-бит аккумулятора (биты 47..24), с заворотом знака
static inline int32_t accB1(int64_t acc) noexcept
{
    return (int32_t)((uint64_t(acc) >> 24) & 0xFFFFFFu);
}

// дробное произведение как его кладёт MPY в 56-бит аккумулятор:
//   acc = sext24(x) * sext24(y) << 1, заворот по модулю 2^56
static inline int64_t mpy56(int32_t x, int32_t y) noexcept
{
    int64_t p = int64_t(x) * int64_t(y);
    uint64_t r = (uint64_t(p << 1)) & ACC56_MASK;
    return (int64_t)r;
}

// MAC: acc = acc + x*y<<1 (56-бит заворот)
static inline int64_t mac56(int64_t acc, int32_t x, int32_t y) noexcept
{
    return (int64_t)(((uint64_t)acc + (uint64_t)mpy56(x, y)) & ACC56_MASK);
}

// MACR: как mac56, но с округлением (+2^23 к A0) — используется в
// func_000397 ($039B macr) для интерполяции дробных тапов
static inline int64_t macr56(int64_t acc, int32_t x, int32_t y) noexcept
{
    int64_t a = mac56(acc, x, y);
    a = (int64_t)(((uint64_t)a + (1ull << 23)) & ACC56_MASK);
    return a;
}

// насыщающее извлечение 24-бит слова из аккумулятора (A1 c учётом A0)
static inline int32_t sat24acc(int64_t acc) noexcept
{
    int32_t v = accB1(acc);
    uint32_t low = (uint32_t)(uint64_t(acc) & 0xFFFFFFull);
    if (v == -1 && low != 0) return -0x800000;
    if (v == 0x7FFFFF && low != 0) return 0x7FFFFF;
    return sext24((uint32_t)v);
}

// насыщение произвольного int64 до 24-бит слова (клип $7FFFFF/$800000)
static inline int32_t sat24(int64_t v) noexcept
{
    if (v >  0x7FFFFF) return  0x7FFFFF;
    if (v < -0x800000) return -0x800000;
    return (int32_t)v;
}

// запись A1 в память с заворотом ±1.0 (родное поведение экстракции
// аккумулятора: за пределами единицы слово заворачивается — это
// стабилизирует фильтр прошивки и является частью алгоритма;
// формула 1:1 из верифицированного порта MnmFilterExact.hpp)
static inline float wrap1(float v) noexcept
{
    return v - 2.0f * std::round(v * 0.5f);
}

// 24-шаговое невосстанавливающее деление DSP56300 (инструкция DIV).
// Порядок операций — точная копия верифицированной репликации из пака
// filter_phaser_pack (probe_eps.py, 9/9 конфигураций бит-в-бит; см.
// MnmFilterExact.hpp::computeExactRing в исходном репозитории mmnova).
// dhi_dlo — 48-бит пара (DHI:DLO), divisor — 24-бит слово.
// Возвращает 48-бит пару после 24 итераций (частное — младшие 24 бита).
static inline uint64_t div24(uint64_t dhi_dlo, int32_t divisor) noexcept
{
    uint64_t D = dhi_dlo & 0xFFFFFFFFFFFFull;
    const int32_t ss = divisor;
    for (int i = 0; i < 24; ++i)
    {
        uint32_t dhi = (uint32_t)((D >> 24) & 0xFFFFFFull);
        uint32_t dlo = (uint32_t)(D & 0xFFFFFFull);
        dhi = ((dhi << 1) | (dlo >> 23)) & 0xFFFFFFu;   // сдвиг влево на 1
        dlo = (dlo << 1) & 0xFFFFFFu;
        const int32_t ds = sext24(dhi);
        dhi = ((ds < 0) == (ss < 0)) ? (uint32_t)((dhi - (uint32_t)divisor) & 0xFFFFFFu)
                                     : (uint32_t)((dhi + (uint32_t)divisor) & 0xFFFFFFu);
        const uint32_t qbit = ((sext24(dhi) < 0) == (ss < 0)) ? 1u : 0u;
        dlo |= qbit;
        D = ((uint64_t)dhi << 24) | (uint64_t)dlo;
    }
    return D;
}

// Q1.23 слово -> float (точное)
static inline float q23ToF(uint32_t w) noexcept
{
    return float(sext24(w)) / 8388608.0f;
}

// float -> Q1.23 слово (насыщение)
static inline uint32_t fToQ23(float v) noexcept
{
    if (v >= 0.99999988f) return 0x7FFFFFu;
    if (v <= -1.0f)       return 0x800000u;
    return (uint32_t)(int32_t)(v * 8388608.0f) & 0xFFFFFFu;
}

} // namespace mnm
