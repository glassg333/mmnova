// =============================================================================
// MnmEnvLegacyFix.h — эталонные куски конвертов Monomachine OS 1.32B для
// починки СТАРЫХ проектов (пак 11, JUCE-адаптация).
//
// ЗАЧЕМ: в старых портах/пресетах конверты были сломаны типовыми способами
// (полный список — JUCE_ADAPT_AND_FIX_RU.md, раздел «8 типов поломок»).
// Здесь — верифицированные замены. Адреса прошивки — комментарии.
//
// ТЕРМИНОЛОГИЯ: пишем «резонанс» / «резонансный хвост», НЕ «резонатор».
// =============================================================================
#pragma once
#include <cstdint>

namespace mnmfix {

// кадр конвертов = 16 сэмплов @ 44.1 кГц
inline constexpr double kFrameMs = 16.0 / 44100.0 * 1000.0;   // 0.3628 мс

// Таблицы шагов $141800 (атака) / $141880 (спад/релиз) — общие для AMP-конверта,
// env2, DIST drive-env и FM-конвертов. В портах: pack7 MnmAmpEnvTables.h
// (kMnmEnvAttack/kMnmEnvDecay) или pack10 MnmEnv2Tables.h (SHAPE_A/SHAPE_B).

// ---------------------------------------------------------------------------
// ФИКС 1 (B5): KILL-гейт блока резонанса.
// ПРОЛОМ: резонансный хвост продолжает звенеть после снятия ноты.
// ПРАВИЛЬНО: при слове фазы AMP-конверта == 4 множитель затухания резонанса
// обнуляется ($0AC0–$0AC4) — резонансный хвост глохнет МГНОВЕННО.
// Это гейт-вход от AMP-конверта, НЕ отдельная ручка и НЕ «энвелоуп фильтра».
// ---------------------------------------------------------------------------
inline int32_t stage2DecayMultiplier(int ampEnvPhase, int32_t decRaw) {
    // decRaw — сырое слово DEC (K<<16); нормальный множитель = K/128
    if (ampEnvPhase == 4) return 0;                  // $0AC0–$0AC4: cmp #4 -> 0
    return decRaw >> 16;                             // стадия 2: mult = K/128
}

// ---------------------------------------------------------------------------
// ФИКС 2 (B6): атака AMP-конверта с data-limiter.
// ПРОЛОМ: level += inc с обычным переполнением int32 → заворот в отрицательные
//         (слышимый «клац»/инверсия огибающей на длинной атаке).
// ПРАВИЛЬНО (P:$088E–$08D7): сумма вышла за 24 бита (флаг E) → уровень
//         сатурируется $7FFFFF (НЕ $800000!), фаза := HOLD, счётчик := 1.
// Возврат: новая фаза (0=ATK остаёмся, 1=HOLD).
// ---------------------------------------------------------------------------
inline int ampAttackStep(int& level24, int atkIdx, const int32_t* atkIncTable /*[128]*/) {
    const int64_t sum = int64_t(level24) + atkIncTable[atkIdx & 0x7F];
    if (sum >= 0x800000) {                           // E-флаг → data limiter
        level24 = 0x7FFFFF;                          // сатурация, не -1.0!
        return 1;                                    // -> HOLD, counter := 1
    }
    level24 = int32_t(sum);
    return 0;
}

// ---------------------------------------------------------------------------
// ФИКС 3 (B7): релиз/DEC/kill — разные скорости.
// ПРОЛОМ: REL использует таблицу DEC (или наоборот), KILL вообще не реализован.
// ПРАВИЛЬНО (P:$08CE–$08D7): idx = (rateWord + $FFFF) >> 16 (округление);
//         level = (−level * tblB[idx]) >> 23; KILL — фиксированное слово $20
//         → idx 1 → ×0.904016/кадр; DEC=127 → ×1.0 (заморозка уровня).
// ---------------------------------------------------------------------------
inline int decayIdxFromRateWord(int32_t rateWord) {
    int idx = (rateWord == 0x20) ? 1                 // KILL: raw $20 → idx 1
                                 : (rateWord + 0xFFFF) >> 16;
    if (idx < 0) idx = 0;
    if (idx > 127) idx = 127;
    return idx;
}
inline int32_t envDecayStep(int32_t level24, const int32_t* tblB /*[128]*/, int idx) {
    const int64_t prod = int64_t(-level24) * tblB[idx];
    int32_t out = int32_t(prod >> 23);
    if (out > 0x7FFFFF) out = 0x7FFFFF;
    return out;
}

// ---------------------------------------------------------------------------
// ФИКС 4 (B4): env2 — куда подключать и чему равен по умолчанию.
// ПРОЛОМ: env2 подключали к фильтру («второй фильтр», «энвелоуп среза»).
// ПРАВИЛЬНО: env2 (P:$04A8–$04F4) — конверт глубины модуляции тапов ДЕЛЭЯ.
//         Единственный потребитель выхода Y:$4FF — рампа $0B1E–$0B22:
//         рампы тапов Y:$10–$1F ← (env·DWID)². Ячейки $418–$41B скрыты от
//         UI/MIDI; по умолчанию уровень = 1.0 → модуляция = просто DWID².
//         В Second-фильтре Monomachine НЕТ — фильтр один, срез env-free.
// ---------------------------------------------------------------------------
inline constexpr int32_t kEnv2DefaultLevel = 0x7FFFFF;   // 1.0 (полный)

// ---------------------------------------------------------------------------
// ФИКС 5 (B8): DWID-рампа тапов с анти-зиппером (T5, $0B1E–$0B32).
// ПРОЛОМ: делэйные тапы модулировали мгновенным значением (зиппер-шум) или
//         вешали на DBAS/DWID отдельную ADSR (в прошивке её НЕТ).
// ПРАВИЛЬНО: 16 значений на кадр: delta = (env·DWID)² − env_prev;
//         чётные = env + k·step, нечётные = env + delta/16 + k·step,
//         step = 16·delta (Q23-арифметика mpy даёт delta/16·16 = step).
//         env_new = (env·DWID)² пишется в состояние (Y:$4FF).
// ---------------------------------------------------------------------------
inline void dwidRamp16(int32_t& envState, int32_t dwidWord /*K<<16*/,
                       int32_t out16[16]) {
    // Точная транскрипция T5 — та же арифметика, что в dsp/mnm/MnmTail.hpp
    // (пак 11, векторы 8296/0). 56-битная аккумуляторная семантика DSP56300.
    struct Ops {
        static uint64_t mpy(uint32_t x, uint32_t y) {
            const uint64_t M24 = 0xFFFFFFull, M56 = (1ull << 56) - 1;
            auto sx = [](uint32_t v) -> int64_t {
                v &= M24; return (int64_t)v - ((int64_t)(v >> 23) << 24);
            };
            return ((uint64_t)((sx(x) * sx(y)) << 1)) & M56;
        }
        static uint64_t add56(uint64_t a, int64_t v) {
            const uint64_t M56 = (1ull << 56) - 1;
            auto sx56 = [](uint64_t v) -> int64_t {
                v &= M56; return (int64_t)v - ((int64_t)(v >> 55) << 56);
            };
            return ((uint64_t)sx56((uint64_t)((int64_t)a + v))) & M56;
        }
        static uint64_t compose(uint32_t v) {
            const uint64_t M24 = 0xFFFFFFull, M56 = (1ull << 56) - 1;
            auto sx = [](uint32_t t) -> int64_t {
                t &= M24; return (int64_t)t - ((int64_t)(t >> 23) << 24);
            };
            return ((uint64_t)((uint64_t)sx(v) & M56)) << 24;
        }
        static uint32_t a1(uint64_t acc) { return (uint32_t)((acc >> 24) & 0xFFFFFFull); }
    };
    uint64_t A = Ops::mpy((uint32_t)dwidWord, (uint32_t)dwidWord);   // DWID²
    uint64_t B = Ops::mpy((uint32_t)envState, (uint32_t)envState);   // env²
    uint32_t x0 = Ops::a1(A), x1 = Ops::a1(B);
    A = Ops::mpy(x1, x0);                                            // (env·DWID)²
    const uint32_t newState = Ops::a1(A);
    A = Ops::add56(A, -(int64_t)Ops::compose((uint32_t)envState));   // delta
    B = Ops::compose((uint32_t)envState);
    uint32_t delta = Ops::a1(A);
    A = Ops::compose((uint32_t)envState);
    B = Ops::add56(B, (int64_t)Ops::mpy(delta, 0x080000u));          // delta/16
    uint64_t step = Ops::mpy(0x10u, delta);                          // 16·delta (Q)
    for (int k = 0; k < 8; ++k) {
        out16[2 * k]     = (int32_t)Ops::a1(A);
        A = Ops::add56(A, (int64_t)step);
        out16[2 * k + 1] = (int32_t)Ops::a1(B);
        B = Ops::add56(B, (int64_t)step);
    }
    envState = (int32_t)newState;
}

// ---------------------------------------------------------------------------
// ФИКС 6 (B3): BOFS/WOFS vs DBAS/DWID — разные ячейки.
// ПРОЛОМ: смешение $416/$417 (оффсеты ФИЛЬТРА, CC78/79) с $41E/$41F
//         (фильтр ДЕЛЭЯ, CC86/87).
// ПРАВИЛЬНО: BOFS/WOFS = стартовые углы блока резонанса фильтра (стадия 2),
//         DBAS/DWID = база/глубина модуляции тапов делэя. Не перепутывать.
// ---------------------------------------------------------------------------
inline constexpr int kCellFilterBOFS = 0x416;   // CC78
inline constexpr int kCellFilterWOFS = 0x417;   // CC79
inline constexpr int kCellDelayDBAS  = 0x41E;   // CC86
inline constexpr int kCellDelayDWID  = 0x41F;   // CC87

// ---------------------------------------------------------------------------
// ФИКС 7 (B9): DSND/пинг-понг — это хост-уровень.
// ПРОЛОМ: пинг-понг эха реализован внутри DSP-кода.
// ПРАВИЛЬНО: DSND (P+$1C, CC84) читается в прологе кадра ($0284), знак
//         переключает ветвь клампировки ($029C–$02A8) = «positive = ping-pong,
//         negative = stereo preserved» (мануал стр. 33). Посыл в эхо — МОНО
//         (T6, $0B33–$0B49: echo[k] = (outs[2k]+outs[2k+1])>>1).
// ---------------------------------------------------------------------------

}  // namespace mnmfix
