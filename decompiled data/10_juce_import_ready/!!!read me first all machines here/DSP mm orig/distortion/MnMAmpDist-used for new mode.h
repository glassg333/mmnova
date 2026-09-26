// ============================================================================
// MnMAmpDist.h — РЕАЛЬНАЯ AMP-страница: DIST-каскад + огибающая AHDR
//                Monomachine SFX-60 MKII OS 1.32B
// ============================================================================
// Источники (всё исполнено на эмуляторе dsp56300, сверено с листингом ядра):
//  * DIST       — P:$079F-$07D1 (post-voice func_0004A8)
//  * огибающая  — P:$04A8-$04F5 (состояния 1=ATK, 4=DEC, 5=SUS, 2=REL)
//  * таблицы    — kDriveCurve129 (P:$1447C6), kAmpRateTable128 (Y:$141800),
//                 kAmpDecayTable128 (Y:$141880) — дословные слова прошивки.
//
// ТОЧНЫЙ ЗАКОН DIST (P:$07A6-$07B5, байт-в-байт):
//    D  = Y:(V-$24)                ; параметр DIST, Q1.23
//    a  = 2*D                      ; P:$07A7 asl a
//    a += -1.0                     ; P:$07A8 add #>$800000  ($800000 = -1.0!)
//    x0 = max(0, a)                ; P:$07AA clr a ifmi
//    => DIST АКТИВЕН ТОЛЬКО при D > 0.5. При скейле ручки 0..0.5 схема МЕРТВА.
//    idx = floor(x0*256) & 0xFF    ; P:$07AD mpyi #$100, P:$07B1 move b,r2
//    c   = kDriveCurve129[idx]     ; P:$07B7 move x:(r2+$1447C6),x1
//                                  ;   1.0 -> 0.3535534 (=sqrt(2)/4) за 129,
//                                  ;   дальше flat-паддинг до 255
//    k   = x0*x0*$7DECCD + $21333  ; P:$07AF/07B3/07B5
//      = 0.9837891*x0^2 + 0.0162109;   k(0)=0.0162, k(1)=1.0000 (ровно)
//
// ИЗМЕРЕНО НА ЭМУЛЯТОРЕ (stop@P:$07D2, синус 3/16, ампл. 0.5):
//    статическое усиление = 4.000 * k   (3 точки сошлись ровно),
//    жёсткий клип ±1.0 (sat24) при 4k >= 2 (D >= ~0.75).
//    Пиковые значения выхода: D=0.5/0.626/0.752/0.878 -> 0.032/0.157/0.532/1.0.
//
// СВЯЗКА С ФИЛЬТРОМ: значение c (кривая привода) подаётся в путь КОЭФФИЦИЕНТА
// среза (P:$07B9: b = c * y1, где y1 — частное деления X:$50B; далее
// P:$07D5-07DB: умножение с TBL[$143546]). Больше drive -> меньше c ->
// темнее срез. Это НЕ отдельный тон-стейдж, а множитель в коэффициенте.
//    Также c умножается на частное div(X:$50B): при X:$50B=0 оно даёт ~1.0
//    (проверено свипом H=0.03125..0.5 — на усиление не влияет). X:$50B —
//    ColdFire-сторона, статус OPEN.
//
// ЧЕГО ДЕЛАТЬ НЕ НАДО:
//  * НЕ моделировать дисторшн как tanh/фолд/"клип с drive 1..8" — в прошивке
//    этого нет; характер = k-зависимое усиление + сатурация 24-битного слова.
//  * НЕ подавать D как 0..0.5 — мёртвая зона (x0=0 всегда).
//  * НЕ убирать промежуточный каскад насыщения (см. processBlock16).
// ============================================================================
#pragma once
#include "MnmFixed.h"
#include "MnmKernelTables.h"
#include <cstring>

namespace mnmsys {

// ----------------------------------------------------------------------------
// DIST-каскад AMP-страницы
// ----------------------------------------------------------------------------
class MnMDist
{
public:
    // d: значение параметра DIST в терминах прошивки, Q1.23 как float (0..1).
    // Рабочая зона прошивки: 0.5..1.0. Если источник — ручка knob 0..127,
    // используйте distFromKnob() ниже (закон ColdFire-стороны — OPEN,
    // консервативное отображение 0.5 + 0.5*knob/127).
    void setDist(float d) noexcept { d_ = std::clamp(d, 0.0f, 1.0f); recompute(); }

    // knob: 0..127 (значение ручки). ВНИМАНИЕ: точный закон ColdFire -> V-$24
    // не вскрыт (нужен дизасм coldfire_main.bin); здесь принят линейный
    // half-range, что подтверждено рабочей зоной D>0.5 и дефолтом D=0.5.
    static float distFromKnob(int knob) noexcept
    {
        return 0.5f + 0.5f * (std::clamp(knob, 0, 127) / 127.0f);
    }

    void reset() noexcept {}

    // Блок 16 сэмплов (аппаратный кадр прошивки). in/out — float -1..+1.
    // Возвращает c — кривую привода для связки с коэффициентом фильтра
    // (подайте в MnMFilter::setDriveCurve).
    float processBlock16(const float* in, float* out, int n) noexcept
    {
        // усиление 4*k в Q1.23-совместимом виде: gainQ = 4k (может быть >1,
        // поэтому множитель в int: x*(4k) = x*(4k*256)>>8 с сатурацией)
        const int32_t gainX256 = int32_t(__builtin_lroundf(gain_ * 256.0f)); // 4k*256
        for (int i = 0; i < n; ++i)
        {
            const s24 x = fToQ23(in[i]);
            // каскад 1: произведение в 56-бит -> сдвиг -> НАСЫЩЕНИЕ слова (sat24)
            const s24 y = sat24((int64_t(x) * int64_t(gainX256)) >> 8);
            // каскад 2: к насыщенному слову применяем тот же 24-битный клип
            // (в прошивке между каскадами АЛУ дважды ограничивает на ±1.0)
            out[i] = q23ToF(uint32_t(y) & 0xFFFFFF);
        }
        return c_;
    }

    float k()  const noexcept { return k_;  }   // 0.9837891*x0^2 + 0.0162109
    float c()  const noexcept { return c_;  }   // кривая привода (для фильтра)
    float x0() const noexcept { return x0_; }   // max(0, 2D-1)

private:
    void recompute() noexcept
    {
        // P:$07A7-07AA: x0 = max(0, 2D - 1)
        x0_ = std::clamp(2.0f * d_ - 1.0f, 0.0f, 1.0f);
        // P:$07AD-07B1: idx = floor(x0*256), 0..255; кривая 129 уникальных,
        // дальше flat 0.3535534 — это уже зашито в kDriveCurve129+паддинг.
        const int idx = std::min(int(x0_ * 256.0f), 128);  // таблица 129 записей,
        // дальше в прошивке flat-паддинг 0x2D413D — эквивалент индекса 128
        // P:$07B7: c = DRIVE[idx]
        c_ = q23ToF(kDriveCurve129[idx]);
        // P:$07B3/07B5: k = x0^2 * $7DECCD + $21333 (точно 0.9837891 / 0.0162109)
        k_ = 0.9837891f * x0_ * x0_ + 0.0162109f;
        // измеренный статический закон: gain = 4.000*k, клип при 4k>2
        gain_ = 4.0f * k_;
    }

    float d_ = 0.0f, x0_ = 0.0f, k_ = 0.0162109f, c_ = 1.0f, gain_ = 0.0648438f;
};

// ----------------------------------------------------------------------------
// AMP-огибающая AHDR — ТОЧНОЕ воспроизведение P:$04A8-$04F5
// (исполнено на риге: attack "+0.5/блок при ATK=0", сатурация на 1.0,
//  сустейн = (V-$0E)^2, decay/release МУЛЬТИПЛИКАТИВНЫЕ таблицей Y:$141880)
// ----------------------------------------------------------------------------
class MnMAmpEnv
{
public:
    // параметры страницы AMP: 0..127 ручки; индексы таблиц = (param >> 16),
    // т.е. старшие 7 бит значения страницы (P:$04B5/04CD/04E6 asr #$10).
    void setParams(int atk, int hold, int dec, int rel) noexcept
    {
        atk_  = std::clamp(atk, 0, 127);
        hold_ = std::clamp(hold, 0, 127);
        dec_  = std::clamp(dec, 0, 127);
        rel_  = std::clamp(rel, 0, 127);
    }

    void trigger() noexcept
    {
        state_ = 1;                    // ATTACK
        level_ = 0;
        holdCnt_ = 0;
    }

    bool active() const noexcept { return state_ != 0; }
    float level() const noexcept { return q23ToF(uint32_t(level_) & 0xFFFFFF); }

    // Тик раз в блок 16 кадров (как в прошивке: огибающая блочная).
    void tickBlock() noexcept
    {
        switch (state_)
        {
        case 1:  // ATTACK: level += TBL_ATK[atk]; стоп+сатурация на 1.0 (E-flag)
        {
            const s24 inc = s24(kAmpRateTable128[atk_] & 0xFFFFFF);
            const int64_t sum = int64_t(level_) + inc;
            if (sum >= Q23_ONE) { level_ = Q23_ONE; state_ = 3; break; } // 3 -> HOLD-вход
            level_ = s24(sum);
            break;
        }
        case 3:  // HOLD (после атаки): holdCnt = V-$0F счётчик блоков
            if (++holdCnt_ >= holdBlocks_) { state_ = 4; }
            break;
        case 4:  // DECAY: level = level * |TBL_DEC[dec]|, к sustain=(susQ)^2
        {
            const s24 m = s24(kAmpDecayTable128[dec_] & 0xFFFFFF);
            const int64_t p = int64_t(level_) * int64_t(m < 0 ? -m : m);
            s24 next = sat24(p >> 22);
            // пол: не опускаться ниже сустейна (переход в state 5 при пересечении)
            if (next <= sus_) { level_ = sus_; state_ = 5; }
            else level_ = next;
            break;
        }
        case 5:  // SUSTAIN: level = (V-$0E)^2, константа
            level_ = sus_;
            break;
        case 2:  // RELEASE: level = level * |TBL_DEC[rel]| (мультипликативно!)
        {
            const s24 m = s24(kAmpDecayTable128[rel_] & 0xFFFFFF);
            const int64_t p = int64_t(level_) * int64_t(m < 0 ? -m : m);
            level_ = sat24(p >> 22);
            break;
        }
        default: break;
        }
    }

    void release() noexcept { if (state_ != 0) state_ = 2; }

    void setSustainQ23(s24 susSqrtQ23) noexcept   // V-$0E = sqrt(sustain)
    {
        sus_ = fmpy(susSqrtQ23, susSqrtQ23);      // квадрат в коде: mpy y0,y0
    }
    void setHoldBlocks(int blocks) noexcept { holdBlocks_ = blocks; }

private:
    int    state_ = 0;      // 0=idle 1=ATK 2=REL 3=HOLD 4=DEC 5=SUS (нумерация ядра)
    s24    level_ = 0;
    s24    sus_   = 0;
    int    holdCnt_ = 0, holdBlocks_ = 0;
    int    atk_ = 0, hold_ = 0, dec_ = 0, rel_ = 0;
};

} // namespace mnmsys
