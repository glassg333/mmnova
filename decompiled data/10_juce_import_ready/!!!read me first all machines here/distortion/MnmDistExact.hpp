// ============================================================================
// MnmDistExact.hpp — DIST трека Monomachine, режим "ex" (exact)
// Порт уровня инструкций (OS 1.32B, ядро DSP1).
// ============================================================================
// ИСТОЧНИК: pack filter_phaser_pack, итерации 21–23 (03_voice_page/
// DIST_STATUS.md, 15_dist_element/):
//   - драйв-энвелоп P:$04FF–$0536 снят БИТ-В-БИТ: атака = tblA[$141800][DIST]
//     для DIST ∈ {16,32,64,96,127} (0x10CFEA/0x46A98/0x4E05/0x560/0x65);
//     wrap -> -1.0; HOLD 1 кадр с fallthrough в DEC; DEC шаг = tblE[$141A00]
//     [rnd((VOL²+$7FFF)>>16)], кламп clr b ifmi -> 0; ретриг сбрасывает.
//   - тимбр-индекс P:$0537–$0556 бит-в-бит (включая знаковые a1-срезы):
//     при нулевом param4 idx = -$80 - drive*1792; проверено эмулятором
//     (DIST=64, кадр 70: drive=1418083 -> idx=-431 = 0xFFFE51).
//
// ГЛАВНОЕ ОТЛИЧИЕ от текущего режима "mnm" (Saturator в MnmKernel.hpp):
//   текущий mnm-дист — СТАТИЧЕСКИЙ клип от положения ручки. В прошивке DIST —
//   это ЭНВЕЛОП ДРАЙВА, перезапускаемый нотой: ручка DIST задаёт ВРЕМЯ АТАКИ
//   драйва (DIST=0 -> 0.5/кадр, DIST=127 -> 0x65/кадр ≈ 30 с до полного
//   драйва), драйв взлетает до 1.0, wrap'ается и мгновенно спадает в 0
//   (VOL²-индекс всегда 0 при VOL<=127 -> шаг tblE[0] = 1/64). Слышимый
//   характер — «набегающий» овердрайв на атаке ноты, а не постоянный клип.
//
// ЧЕСТНЫЕ ГРАНИЦЫ (см. DIST_STATUS.md «Статус»):
//   по-сэмпловое тело машинного обработчика $145C48 (SWAVE-семейство:
//   вращатель/кольцо 1024/билинейные веса 16 тапов) НЕ декодировано — это
//   per-machine семейство (полный список «машина -> обработчик» живёт у
//   ColdFire-хоста, искать по эпилогу jmp $000981). Здесь:
//     БИТ-ТОЧНО: стейт-машина драйв-энвелопа + тимбр-индекс;
//     [РЕКОНСТРУКЦИЯ]: по-сэмпловая стадия y = sat24(x*(1+drive*kDriveGain))
//     — сатурация 24-битного слова DSP (bset #$14,sr), масштаб kDriveGain
//     подобран равным потолку текущего Saturator'а (сравнимая громкость A/B).
//   Тимбр-индекс доступен геттером timbreIndex() для будущей связки с
//   таблицей тембра $143546 (машино-специфичной).
//
// СЕТКА: кадр = 16 сэмплов; process() тикает энвелоп раз в кадр сам.
// Зависимости: только MnmDistExactTables.hpp (тот же каталог).
// ============================================================================
#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>

#include "MnmDistExactTables.hpp"

namespace monomachine {
namespace mnm {

namespace exact_raw {
// 24-битная signed-арифметика Q1.23 — как аккумулятор DSP (a1 = биты 23..0)
inline int32_t wrap24(int32_t v) noexcept
{
    v &= 0xFFFFFF;
    return (v & 0x800000) ? (v - 0x1000000) : v;
}
inline int32_t q23(float x) noexcept { return (int32_t)std::lrint(x * 8388608.0f); }
} // namespace exact_raw

// ---------------------------------------------------------------------------
// DistDriveEnv — драйв-энвелоп DIST (бит-точный порт P:$04FF–$0536).
//
// Фазы (ячейка y:(page+$DB)):
//   0 = ATK : level += tblA[DIST]        (add y0,b; EC-переход = 24-битный wrap)
//   1 = HOLD: порог = 0 (x0 = 0 от clr a) -> 1 кадр, bra $052B — ПАДЕНИЕ В DEC
//             В ТОМ ЖЕ КАДРЕ (замер iter.23: wrap -> -1.0 -> 0 за один шаг)
//   2 = DEC : level -= tblE[rnd((VOL²+$7FFF)>>16)]; clr b ifmi -> кламп 0
//             (поэтому wrap'нутый -1.0 умирает мгновенно — замер подтверждает)
//
// Ретриг: y:(page+$20)==1 -> ядро обнуляет уровень/фазу/счётчик ($0500–$0505).
//
// Уровень ПОСЛЕ wrap'а отрицательный — это не ошибка, так в железе (знак —
// «внутренняя кухня», гейн = |level|; тимбр-индекс использует знак как есть).
// ---------------------------------------------------------------------------
class DistDriveEnv
{
public:
    struct Params
    {
        int dist = 0;    // DIST-ручка 0..127 (ячейка y:(page+$0C), биты 23..16)
        int vol = 127;   // VOL 0..127 -> ядро пишет VOL² в y:(page+$0D) (замер)
    };

    enum Phase : int { kAtk = 0, kHold = 1, kDec = 2 };

    void reset() noexcept { phase = kAtk; level = 0; counter = 0; }
    void retrig() noexcept { phase = kAtk; level = 0; counter = 0; }

    // Один кадр (16 сэмплов). Возвращает СЫРОЙ уровень Q1.23 (может быть < 0
    // после wrap'а — родное поведение железа).
    int32_t tickRaw(const Params& p) noexcept
    {
        using namespace exact_raw;
        switch (phase)
        {
        case kAtk: {
            const int32_t inc = (int32_t)std::lrint(exact_tables::dist::kDistAtk[(size_t)(p.dist & 127)] * 8388608.0f);
            const int32_t sum = level + inc;           // add y0,b
            if (sum > 0x7FFFFF) {                      // EC set -> wrap
                level = wrap24(sum);                   // -1.0-зона
                phase = kHold;
                counter = q23(1.0f);
            } else {
                level = sum;                           // bec -> обычный сторе
            }
            break;
        }
        case kHold:                                    // P:$0517–$052A
            // порог = 0·tempo·… = 0 -> фаза := 2 и bra $052B — ПАДЕНИЕ В DEC
            // В ТОМ ЖЕ КАДРЕ (замер итер. 23)
            phase = kDec;
            [[fallthrough]];
        case kDec: default: {                          // P:$052B–$0536
            const int32_t cell = (p.vol & 127) * (p.vol & 127);   // VOL² (замер)
            const int idx = (cell + 0x7FFF) >> 16;                // +rnd
            int32_t b = level - (int32_t)std::lrint(exact_tables::dist::kDistRel[(size_t)(idx & 127)] * 8388608.0f);
            if (b < 0) b = 0;                           // clr b ifmi
            level = b;
            break;
        }
        }
        return level;
    }

    int     getPhase() const noexcept { return phase; }
    int32_t getLevelRaw() const noexcept { return level; }

private:
    int     phase = 0;
    int32_t level = 0;      // X:(page+$DB), signed Q1.23
    int32_t counter = 0;    // X:(page+$DA)
};

// ---------------------------------------------------------------------------
// Тимбр-индекс секции P:$0537–$0556 — БИТ-В-БИТ (iter.23).
// Все промежуточные результаты идут через 24-битный a1-срез аккумулятора —
// включая |term|: (panTerm+$C00000) считается в 56-битном акк., abs — там же,
// но в множитель попадает a1 = младшие 24 бита СО ЗНАКОМ. При panTerm = 0:
// |0+$C00000| = 0.75 -> a1 = 0xC00000 = ЗНАКОВОЙ -0.5! Квадрат 0.25, asl#2 ->
// 1.0 -> a1 = 0x800000 = -1.0 — ЕЩЁ ОДИН переворот знака. Итог: при нулевом
// param4 индекс = -$80 - drive*1792 (ОТРИЦАТЕЛЬНЫЙ, скользит вниз с драйвом).
// Верхний кламп $700 подтверждён (tfr x1,a ifge), нижнего нет — отрицательные
// индексы уходят в таблицу тембра как есть (родное поведение железа).
// ---------------------------------------------------------------------------
inline int exactTimbreIndex(int32_t driveRaw, int32_t param4Raw,
                            int32_t panTermRaw) noexcept
{
    using namespace exact_raw;
    const int64_t base = (int64_t)param4Raw * 0x800 - 0x80;  // mpyi/sub: 56-бит
    int32_t s = panTermRaw + 0xC00000;                       // add: 56-бит
    if (s < 0) s = -s;                                       // abs (56-бит)
    const int32_t t = wrap24(s);                             // a1 -> y0 (знаковый срез!)
    const int32_t v1 = wrap24((((int64_t)t * t) >> 23) << 2); // mpy; asl#2; a1
    const int32_t v2 = wrap24((int64_t)driveRaw * v1 >> 23);  // mpy y1,y0; a1 -> y1
    const int32_t v3 = wrap24((int64_t)v2 * 0x700 >> 23);     // mpy y1,x1; a1
    const int64_t acc = v3 + base;                            // add b,a
    if (acc >= 0x700) return 0x700;                           // tfr x1,a ifge
    return wrap24((int32_t)(acc & 0xFFFFFF));                 // сторе a1
}

// ---------------------------------------------------------------------------
// DistExactCore — звуковой узел режима "ex" для плагина.
// ---------------------------------------------------------------------------
class DistExactCore
{
public:
    void reset() noexcept
    {
        drive_.reset();
        cnt_ = 0;
    }

    // DIST-ручка 0..127 (AMP-страница, CC 60): ВРЕМЯ атаки драйва (tblA[DIST]).
    void setKnob(float dist) noexcept
    {
        dp_.dist = (int)std::clamp(dist + 0.5f, 0.0f, 127.0f);
    }
    // VOL 0..127: в прошивке ядро пишет VOL² в ячейку DEC-индекса (замер
    // iter.23); при VOL<=127 индекс всегда 0 -> спад tblE[0] = 1/64/кадр.
    void setVol(float vol) noexcept
    {
        dp_.vol = (int)std::clamp(vol + 0.5f, 0.0f, 127.0f);
    }
    // Ретриг по ноте (y:(page+$20)==1): сброс уровня/фазы/счётчика.
    void retrig() noexcept { drive_.retrig(); }

    // Один сэмпл. Энвелоп тикает раз в кадр (16 сэмплов) — сетка прошивки.
    // [РЕКОНСТРУКЦИЯ по-сэмпловой стадии — см. шапку: тело машинного
    // обработчика $145C48 не декодировано, здесь сатурация слова DSP с
    // драйвом от бит-точного энвелопа.]
    inline float process(float x) noexcept
    {
        if (cnt_ == 0)
            drive_.tickRaw(dp_);                     // 1 кадр = 16 сэмплов
        if (++cnt_ >= 16) cnt_ = 0;

        const int32_t lvl = drive_.getLevelRaw();
        const float drive = lvl > 0 ? (float)lvl * (1.0f / 8388608.0f) : 0.0f;
        const float gain = 1.0f + drive * kDriveGain;
        return sat24(x * gain);
    }

    // Стерео-пара за вызов: энвелоп ОДИН на трек (как в прошивке — страница
    // AMP общая), счётчик кадра тикает по ПАРАМ, а не по каналам. Используйте
    // этот метод в цепочке плагина: processStereo(l[i], r[i]).
    inline void processStereo(float& l, float& r) noexcept
    {
        if (cnt_ == 0)
            drive_.tickRaw(dp_);
        if (++cnt_ >= 16) cnt_ = 0;

        const int32_t lvl = drive_.getLevelRaw();
        const float drive = lvl > 0 ? (float)lvl * (1.0f / 8388608.0f) : 0.0f;
        const float gain = 1.0f + drive * kDriveGain;
        l = sat24(l * gain);
        r = sat24(r * gain);
    }

    // Диагностика/тесты
    int     drivePhase() const noexcept { return drive_.getPhase(); }
    int32_t driveRaw() const noexcept { return drive_.getLevelRaw(); }
    // Тимбр-индекс текущего кадра (param4=0, panTerm=0): -$80 - drive*1792.
    int     timbreIndex() const noexcept
    {
        return exactTimbreIndex(drive_.getLevelRaw(), 0, 0);
    }

    // Масштаб драйва по-сэмпловой стадии [РЕКОНСТРУКЦИЯ, TUNABLE]: потолок
    // равен потолку текущего Saturator'а (drive = 1+15*amount) для сравнимой
    // громкости A/B. Меняется одной строкой.
    static constexpr float kDriveGain = 15.0f;

private:
    // Сатурация 24-битного слова DSP56300 (bset #$14,sr): жёсткий клип на
    // границах слова ±(1-2^-23). Родной эквивалент выхода ALU за пределы ±1.
    static float sat24(float v) noexcept
    {
        const float lim = 1.0f - 1.1920929e-7f;      // 1 - 2^-23
        return std::clamp(v, -lim, lim);
    }

    DistDriveEnv drive_ {};
    DistDriveEnv::Params dp_ {};
    int cnt_ = 0;
};

} // namespace mnm
} // namespace monomachine
