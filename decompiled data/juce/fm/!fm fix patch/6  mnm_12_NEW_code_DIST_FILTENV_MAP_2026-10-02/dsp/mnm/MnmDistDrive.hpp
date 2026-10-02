// ============================================================================
// MnmDistDrive.hpp — DIST дист Monomachine: драйв-энвелоп («наскок») + индекс
// тона. Бит-точный порт P:$04FF–$0556 ядра DSP1, OS 1.32B.
//
// Исправление относительно mnm_dist.h (пак 11 / итерация 23): точка перелома
// атаки. В реальном кадре активен дата-лимитер DSP56300 (порог 2^47), поэтому
// суммирование level+inc при достижении 1.0 пишется как $7FFFFF (+1.0), а НЕ
// как a1-срез (−1.0). Измерено на бит-точном эмуляторе (exp69, харнес пака 7/8):
//   atk=0:  f0 lvl=0.5, f1 lvl=$7FFFFF фаза1, f2 lvl=$7FFFFF−$080000 фаза2,
//           ноль на f65 — ровно 64 кадра спада по 1/64.
//   idx(f0) = 0xFFFC00 = −1024 = −128 − 0.5·1792  (формула индекса бит-в-бит).
//
// Закон (все адреса — обоснование в листинге dsp1_kernel_P0000-0B4D.txt):
//   АТАКА  $0506–$0516: level += kDistAtk[$40C>>16]; переполнение -> level=
//          $7FFFFF (дата-лимитер), фаза:=1.
//   HOLD   $0517–$052A: 1 кадр, безусловно падает в DEC того же кадра
//          (множитель темпа = 0).
//   СПАД   $052B–$0536: level −= kDistRel[(VOL²+$7FFF)>>16]; VOL≤127 -> индекс
//          0 -> шаг 1/64/кадр; кламп ifmi -> 0. Итого спад 1.0->0 = 64 кадра
//          = 23.2 мс (кадр = 16 сэмплов @44.1 кГц).
//   РЕТРИГ $04FF–$0505: ячейка $420==1 -> level/фаза/счётчик := 0.
//   ИНДЕКС $0537–$0556: idx = wrap24( param4·$800 − $80 + panFactor −
//          level·1792·panFactor ); читатели $06CD/$07D4/$0857: гейн EQ =
//          1 − tbl[$1435C6][clamp(idx,0,1599)] — аттенюация/тон.
//
// Верификация: vectors_dist_exp69.txt (450 кадров x уровень/фаза/индекс,
// 5 случаев атаки), оракул = бит-точный эмулятор exp64/пак-7/8.
// ============================================================================
#pragma once
#include <cstdint>
#include <cstdlib>
#include "MnmDistTables.h"

namespace mnm {

namespace raw {
inline int32_t wrap24(int32_t v) noexcept
{
    v &= 0xFFFFFF;
    return (v & 0x800000) ? (v - 0x1000000) : v;
}
} // namespace raw

class MnmDistDrive
{
public:
    enum Phase : int { kAtk = 0, kHold = 1, kDec = 2 };

    struct Params
    {
        int   atkIdx = 0;   // ячейка $40C, старшая половина (0..127)
        int   vol = 100;    // ячейка $405 (0..127); ядро пишет VOL² в $40D
        int32_t param4 = 0; // ячейка $408 (хост-слово тона), Q1.23
        int32_t panTerm = 0;// ячейка $40E (PAN-L производная), Q1.23
    };

    void reset() noexcept { phase = kAtk; level = 0; }
    void retrig() noexcept { phase = kAtk; level = 0; }   // $420==1

    // Один кадр. Возвращает уровень (сырой Q1.23, 0..$7FFFFF).
    int32_t tickRaw(const Params& p) noexcept
    {
        using namespace raw;
        switch (phase)
        {
        case kAtk: {
            const int32_t inc = (int32_t) kDistAtk[(size_t)(p.atkIdx & 127)];
            const int64_t sum = (int64_t) level + inc;
            if (sum >= 0x800000) {                 // переполнение Q1.23
                level = 0x7FFFFF;                  // ДАТА-ЛИМИТЕР: +1.0 (не −1.0!)
                phase = kHold;
            } else {
                level = (int32_t) sum;             // bec -> обычный сторе
            }
            break;
        }
        case kHold:
            phase = kDec;                          // $0525-$0528: выход безусловный
            [[fallthrough]];
        case kDec: default: {
            const int32_t cell = (p.vol & 127) * (p.vol & 127);   // VOL² ($40D)
            const int idx = (cell + 0x7FFF) >> 16;                // = 0 для VOL<=127
            int32_t b = level - (int32_t) kDistRel[(size_t)(idx & 127)];
            if (b < 0) b = 0;                      // clr b ifmi ($0535)
            level = b;
            break;
        }
        }
        return level;
    }

    // Индекс тона x:$4D9 ($0537–$0556) — вход читателей аттенюации EQ.
    int32_t toneIndex(const Params& p) const noexcept
    {
        using namespace raw;
        const int64_t base = (int64_t) p.param4 * 0x800 - 0x80;  // mpyi/sub
        int32_t s = p.panTerm + 0xC00000;                        // 56-бит, без wrap
        if (s < 0) s = -s;                                       // abs
        const int32_t t = wrap24(s);                             // a1-срез (знаковый!)
        const int32_t v1 = wrap24((((int64_t) t * t) >> 23) << 2);
        const int32_t v2 = wrap24((int64_t) level * v1 >> 23);
        const int32_t v3 = wrap24((int64_t) v2 * 0x700 >> 23);   // ·1792
        const int64_t acc = v3 + base;
        if (acc >= 0x700) return 0x700;                          // верхний кламп
        return wrap24((int32_t)(acc & 0xFFFFFF));                // низа нет —
    }                                                            // отрицательный как есть

    int     getPhase()    const noexcept { return phase; }
    int32_t getLevelRaw() const noexcept { return level; }

private:
    int     phase = kAtk;
    int32_t level = 0;   // x:$4DB
};

} // namespace mnm
