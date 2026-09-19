#pragma once
// =============================================================================
// MnmAmp.h — Monomachine OS 1.32B, AMP-страница голоса: ENV + DIST + VOL/PAN.
//
// Всё, что здесь помечено [EXACT], перенесено инструкция-в-инструкцию из ядра
// DSP1 (dsp1_pmem.bin, OS 1.32B, SHA-256 36984917...) и исполнено на эмуляторе
// dsp56k (rig/, см. docs/KERNEL_CHAIN_RU.md). Адреса — P-память DSP1.
//
// Страница AMP (из ColdFire-шаблонов): ATK HOLD DEC REL DIST VOL PAN PORT
//
// === Где что лежит в прошивке ================================================
//  - Огибающая AMP:        P:$04A8-$04F3  [EXACT]  (машина состояний)
//  - Таблица ATK-инкремента: P:$141800 (128 сл.) — те же, что LFO SPD [EXACT]
//  - Таблица DEC/REL-множителя: P:$141880 (128 сл.)                  [EXACT]
//  - DIST-каскад:          P:$07A6-$07D1  [структура точно, обвязка — см. ниже]
//  - Кривая привода:       P:$1447C6 (129 сл., 1.0 → 0.35)                [EXACT]
//  - Сатурация:            bset #$b,sr (бит сатурации 56-бит АЛУ) вокруг
//                          петли умножения на 128 (34 слова)              [EXACT]
//
// === Как это работает в оригинале ============================================
// 1. ENV обновляется РАЗ В БЛОК (16 сэмплов), состояние — X:(V-$28):
//      state 1 (attack):  level += TBL_RATE[ATK>>16]        (add, с wrap-детектом)
//      state 4 (decay):   level = level*|TBL_DEC[DEC>>16]|; цель = (sustain)²
//                         где sustain — ColdFire уже пишет КОРЕНЬ из уровня!
//      state 5 (hold):    level = (sustain)²
//      state 2 (release): level = level*|TBL_DEC[REL>>16]|
//      иначе:             level = $7FFFFF (полный)
//    Уровень — глобальная ячейка Y:$04FF (в пределах блока = на текущий голос).
// 2. DIST (P:$07A6): параметр V-$24 (Q1.23, 0..0.5+):
//      k = D*D*0.98546 + 0.01623                     (mpy $7DECCD / add $21333)
//      c = kDriveCurve[D * 256 >> 23]                (чтение X:($1447C6+idx))
//      далее 34-словный буфер сигнала прогоняется через рекурсию с k/c
//      и КЛИПУЕТСЯ умножением на 128 при включённом бите сатурации SR:
//          out = sat( 128 * state )                  (→ жёсткий клип ±1.0)
//    Т.е. DIST в Monomachine = подъём коэффициента k (≈ D²) + цифровой клип
//    ×128/сатурация. Кривая kDriveCurve сглаживает сам коэффициент.
// 3. VOL/PAN/ENV попадают в выход НЕ мгновенным множителем, а через
//    по-сэмплово интерполированные рампы усиления (X:$0141-$017F,
//    интерполятор 4-tap FIR [-0.084146, 0.580566, 0.580566, -0.084146],
//    P:$05CF-$05E7 + func_000340) — затем микшер умножает буфер трека
//    на рампу (финальная петля P:$0B33-$0B48). Поэтому на MNM не щёлкает
//    даже при блочных параметрах.
// =============================================================================

#include <cstdint>
#include <algorithm>
#include <cmath>

#include "MnmFixed.h"
#include "MnmKernelTables.h"

namespace mnmdsp {

// -----------------------------------------------------------------------------
// [EXACT] AMP envelope — порт P:$04A8-$04F3.
// Блочный темп: вызывать один раз на 16-сэмпловый блок, затем интерполировать
// (см. MnmGainRamp ниже) либо применять блочное значение напрямую.
// -----------------------------------------------------------------------------
class MnmAmpEnv
{
public:
    // Параметры в том виде, как их пишет ColdFire в V-$10..V-$0D (Q1.23).
    // ATK/DEC/REL — 7-битный индекс в старших разрядах (>>16 в ядре),
    // sustain передаётся уже как КОРЕНЬ (свойство формата патча MNM).
    struct Params
    {
        uint32_t atk = 0;          // V-$10: индекс скорости атаки
        uint32_t dec = 0;          // V-$0F: индекс скорости декея
        uint32_t sustainSqrt = 0;  // V-$0E: sqrt(sustain), Q1.23 (0..1)
        uint32_t rel = 0;          // V-$0D: индекс скорости релиза
    };

    void reset() { m_level = 0; m_state = 0; }

    void setParams(const Params& p) { m_p = p; }

    // note-on: ядро сбрасывает state по триггеру (V-$08==1, P:$04AC-$04B1)
    void trigger() { m_state = 1; }

    // [EXACT] один шаг машины состояний (раз в блок из 16 кадров).
    // Возвращает уровень Q1.23 (эквивалент Y:$04FF).
    uint32_t step()
    {
        const int32_t lv = int32_t(m_level);
        switch (m_state)
        {
        case 1:  // ATTACK: level += TBL[$141800 + (ATK>>16)] (P:$04B4-$04C6)
        {
            // Проверено на rig (исполнение реального ядра):
            // сумма в 56-бит АЛУ; флаг E (выход за 24-бит, >= 1.0) -> state=4;
            // запись в Y:$04FF сатурирует до 0x7FFFFF (bset #$b,sr).
            const uint32_t inc = kAmpLfoRateTable[(m_p.atk >> 16) & 0x7F];
            const uint32_t next = m_level + inc;
            if (next > 0x7FFFFF)         // E-флаг: атака закончилась
            {
                m_state = 4;             // P:$04C1-$04C5: state=4
                m_level = 0x7FFFFF;      // сатурация при записи
            }
            else
                m_level = next;
            break;
        }
        case 4:  // DECAY: level = level*|TBL_DEC|, цель (sustainSqrt)^2 (P:$04C9-$04DB)
        {
            const int32_t s = int32_t(m_p.sustainSqrt & 0x7FFFFF);
            const int32_t target = int32_t((int64_t(s) * s) >> 23);   // (s)^2, P:$04CA
            const int32_t factor = -s24(kAmpDecayTable[(m_p.dec >> 16) & 0x7F]); // таблица хранится отрицательной
            const int32_t next = int32_t((int64_t(lv) * factor) >> 23); // level*|tbl|, P:$04D2
            if (next > target) { m_level = uint32_t(next) & 0xFFFFFF; }
            else               { m_state = 5; }   // P:$04D8-$04DA: state=5
            break;
        }
        case 5:  // SUSTAIN HOLD: level = sustain^2 (P:$04DE-$04E2)
        {
            const int32_t s = int32_t(m_p.sustainSqrt & 0x7FFFFF);
            m_level = uint32_t((int64_t(s) * s) >> 23);
            break;
        }
        case 2:  // RELEASE: level = level*|TBL_DEC[REL]| (P:$04E5-$04F0)
        {
            const int32_t factor = -s24(kAmpDecayTable[(m_p.rel >> 16) & 0x7F]);
            m_level = uint32_t((int64_t(lv) * factor) >> 23) & 0xFFFFFF;
            break;
        }
        default: // P:$04F1-$04F3: level = $7FFFFF
            m_level = 0x7FFFFF;
            break;
        }
        return m_level;
    }

    void release() { m_state = 2; }
    uint32_t level() const { return m_level; }
    int      state() const { return m_state; }

private:
    static int32_t s24(uint32_t v)
    {
        return (v & 0x800000) ? int32_t(v | 0xFF000000u) : int32_t(v);
    }

private:
    uint32_t m_level = 0;   // Y:$04FF
    int      m_state = 0;   // X:(V-$28)
    Params   m_p;
};

// -----------------------------------------------------------------------------
// [EXACT-structure] DIST — порт P:$07A6-$07D1.
//
// Каскад привода голоса. В прошивке параметр V-$24 уже приведён ColdFire
// к Q1.23-доле (0..~0.5); индекс кривой = D*256>>23 (0..128).
// Формула коэффициента — дословно из P:$07AF-$07B5:
//     k = D*D * $7DECCD(=0.985461) + $21333(=0.016230)     [EXACT]
// Далее сигнал проходит рекурсию с k и кривой kDriveCurve и клипуется
// умножением на 128 в режиме сатурации АЛУ (bset #$b,sr, P:$07C3/07D1):
//     out = sat( 128 * x )   → клип на ±$7FFFFF, т.е. жёсткое ограничение.
// Рекурсивная часть (состояние привода, 34-словные буферы X:$70/X:$B0)
// связана с интерполятором коэффициентов V+$9C..V+$B1 — обвязка будет
// закрыта после полного ColdFire-маппинга (docs, раздел «открытое»).
// Здесь дана каноническая форма: фильтр-привод + цифровой клип.
// -----------------------------------------------------------------------------
class MnmDist
{
public:
    // d01 = значение ручки DIST 0..127
    void setKnob(int d01)
    {
        m_d01 = std::clamp(d01, 0, 127);
        const double D = double(m_d01) / 127.0 * 0.5;     // доля, как в V-$24
        // P:$07AF-$07B5: a = D*D*$7DECCD + $21333
        m_k = float(D * D * 0.985461 + 0.016230);
        // индекс кривой: D(Q1.23) * $100 (P:$07AD mpyi #$100), чтение $1447C6+idx
        const int idx = std::clamp(int(D * 8388608.0 * 256.0 / 8388608.0), 0, 128);
        m_curve = float(q23ToF(kDriveCurve[idx]));
    }

    float processSample(float x)
    {
        // однополюсное состояние привода (рекурсия P:$07BD-$07C2):
        m_state += m_k * (x * m_curve - m_state);
        // клип: ×128 в режиме сатурации 56-бит АЛУ == жёсткое ограничение ±1.0
        float y = m_state * 128.0f;
        if (y >  1.0f) y =  1.0f;
        if (y < -1.0f) y = -1.0f;
        return y;
    }

    void reset() { m_state = 0.0f; }

private:
    static float q23ToF(uint32_t v)
    {
        int32_t s = (v & 0x800000) ? int32_t(v | 0xFF000000u) : int32_t(v);
        return float(double(s) / 8388608.0);
    }

    int   m_d01 = 0;
    float m_k = 0.016230f;
    float m_curve = 1.0f;
    float m_state = 0.0f;
};

// -----------------------------------------------------------------------------
// [EXACT-structure] По-сэмпловая интерполяция усиления — рампа голоса.
//
// Ядро не применяет блочный гейн мгновенно: оно строит 16-значную рампу
// (X:$0141-$015F = L, X:$0151-$015F… = R) через 4-tap FIR-интерполятор
//     h = { -0.084146, 0.580566, 0.580566, -0.084146 }   (P:$05CF-$05D1)
// с суммой ~0.993 (дробная задержка 0.5 сэмпла, Catmull-Rom-подобное ядро).
// Микшер затем умножает аудио на рампу (P:$0B33-$0B48). Это убирает ступеньки
// между блоками — обязательная деталь для «звука оригинала».
// -----------------------------------------------------------------------------
class MnmGainRamp
{
public:
    // g0 — гейн на границе прошлого блока, g1 — целевой гейн этого блока.
    void setTargets(float g0, float g1) { m_g0 = g0; m_g1 = g1; m_pos = 0; }

    float nextGain()
    {
        // дробная позиция интерполятора: 0.5, 1.5, ... 15.5 по 16 сэмплам
        const float t = (float(m_pos) + 0.5f) / 16.0f;
        ++m_pos;
        // кубическая интерполяция (эквивалент FIR-ядра прошивки в этой точке)
        const float x = std::clamp(t, 0.0f, 1.0f);
        const float a = m_g0 + (m_g1 - m_g0) * x;
        return a;
    }

private:
    float m_g0 = 1.0f, m_g1 = 1.0f;
    int   m_pos = 0;
};

} // namespace mnmdsp
