// ============================================================================
// MnmEnvExact.hpp — AMP-энвелоп трека Monomachine, режим "ex" (exact)
// Бит-точный порт стейт-машины P:$088E–$08D7 (OS 1.32B, ядро DSP1).
// ============================================================================
// ИСТОЧНИК: pack filter_phaser_pack, итерация 13 (README_amp_envelope.md):
// исполнено на бит-точном эмуляторе на РЕАЛЬНОМ коде ядра; приращения атаки
// совпали с таблицей бит-в-бит (ATK=32: шаг 0x46A98 = tblA[32] точно);
// свипы ATK×10/DEC×11/REL×5/HOLD×5/KILL/RETRIG — amp_env_sweep.json.
//
// ФАЗЫ (X[P+$D8]): 0=ATK 1=HOLD 2=DEC 3=REL 4=KILL. "Делей" = HOLD:
// порог счётчика масштабируется темпом (t0 = max(0, HOLD*2^16-$10000),
// thr = t0*TEMPO*1.8909 — коэффициент $791FD0*2*2/2^24; при TEMPO=120:
// HOLD=64 -> 114 кадров = 41 мс, HOLD=0/1 -> выкл. Соответствует измеренной
// карте кадров (5 точек эмуляции, ±2 кадра). ПРИМЕЧАНИЕ: в порте пака
// (mnm_amp_env.h) t0 нормировался на /65536 вместо /2^23 — HOLD длился
// в 128 раз дольше измеренного; здесь закон подогнан к измеренной карте.
//
// ДОКАЗАННЫЕ ТОНКОСТИ (все в коде ниже):
//   * 24-битный wrap атаки: уровень проходит через -1.0 (знак меняется) —
//     это НЕ ошибка, так в железе; гейн = |level|, щелчка нет (де-зиппер).
//   * первый шаг затухания: -level*-tblB при level=-1.0 -> +0.999 (знак
//     снова положительный, |level| монотонно падает дальше).
//   * KILL: индекс $20 -> rnd -> 1 -> tblB[1] = 0.904/кадр (-60 дБ за ~44
//     кадра = 16 мс).
//   * DEC=127 (tblB[127] = -1.0): уровень заморожен навсегда.
//   * РЕТРИГ (note-on во время DEC/REL): уровень НЕ сбрасывается — атака
//     продолжается с текущего уровня; |level| при этом падает к нулю и снова
//     растёт (дип-к-тишине) — родное поведение оригинала.
//   * гейн-путь: де-зиппер 3/32 за кадр + внутрикадровый линейный рамп
//     (16 значений на блок, P:$08F6–$08F9) — ступенчатых артефактов нет.
//
// ЧТО ОСТАВЛЕНО ХОСТУ (как в плагине): VOL^2 и панорама — отдельные ручки
// плагина (params VOL/PAN), здесь возвращается только огибающая |level|
// с фирменным сглаживанием. Полный закон прошивки: gain = |level|*VOL^2*pan.
//
// СЕТКА: кадр = 16 сэмплов (0.3629 мс @44.1к); process() буферизует сам.
// Зависимости: только MnmEnvExactTables.hpp (тот же каталог).
// ============================================================================
#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>

#include "MnmEnvExactTables.hpp"

namespace monomachine {
namespace mnm {

// ---------------------------------------------------------------------------
// EnvExactCore — режим "ex" редактора огибающей (кнопка MODE: old|mnm|vital|ex)
// ---------------------------------------------------------------------------
class EnvExactCore
{
public:
    // Фазы — как в железе (X[P+$D8])
    enum Phase : int { kAtk = 0, kHold = 1, kDec = 2, kRel = 3, kKill = 4 };

    // Параметры страницы AMP (raw 0..127, как рукоятки машины)
    struct Params
    {
        int atk   = 0;      // AMP ATK  ($00)
        int hold  = 0;      // AMP HOLD ($01) — «делей», темп-зависимый
        int dec   = 90;     // AMP DEC  ($02)
        int rel   = 40;     // AMP REL  ($03)
        int tempo = 120;    // хост-темп BPM (Y[P+$23])
    };

    void reset() noexcept
    {
        phase_ = kAtk;
        level_ = 0.0f;
        counter_ = 0;
        smooth_ = 0.0f;
        frameStart_ = 0.0f;
        frameTarget_ = 0.0f;
        cnt_ = 0;
    }

    // TRIG_EVENT (Y[P+$28]): 1 = note-on, 2 = note-off, 3 = kill.
    // ВАЖНО: trig=1 НЕ сбрасывает уровень — ретриг из DEC/REL продолжает
    // атаку с текущего уровня (дип-к-тишине, родное поведение).
    void trig(int t) noexcept
    {
        if (t == 1)      phase_ = kAtk;
        else if (t == 2) phase_ = kRel;
        else if (t >= 3) phase_ = kKill;
    }
    void noteOn()  noexcept { trig(1); }
    void noteOff() noexcept { trig(2); }
    void kill()    noexcept { trig(3); }

    void setParameters(float atk, float hold, float dec, float rel,
                       float tempo = 120.0f) noexcept
    {
        p_.atk   = (int)std::clamp(atk + 0.5f, 0.0f, 127.0f);
        p_.hold  = (int)std::clamp(hold + 0.5f, 0.0f, 127.0f);
        p_.dec   = (int)std::clamp(dec + 0.5f, 0.0f, 127.0f);
        p_.rel   = (int)std::clamp(rel + 0.5f, 0.0f, 127.0f);
        p_.tempo = (int)std::clamp(tempo, 30.0f, 300.0f);
    }

    // Один кадр стейт-машины (вызывается раз в 16 сэмплов, см. process).
    // Возвращает |level| — цель гейна этого кадра.
    float tickFrame() noexcept
    {
        switch (phase_)
        {
        case kAtk:
            level_ += exact_tables::env::kEnvAtk[(size_t)p_.atk];   // inc = tblA[ATK]
            if (level_ >= 1.0f)
            {
                level_ -= 2.0f;             // 24-битный wrap: уровень -> -1.0!
                phase_ = kHold;
                counter_ = 1;               // счётчик HOLD сеется в 1
            }
            break;

        case kHold: {
            // t0 = max(0, HOLD*2^16 - $10000)/2^23; thr = t0*TEMPO*1.8909
            // ($791FD0*2*2/2^24); закон подогнан к измеренной карте кадров:
            // TEMPO=120: HOLD=4 -> 6, 16 -> 29, 32 -> 57, 64 -> 114 кадров.
            const double t0  = std::max(0.0, (double)((p_.hold << 16) - 0x10000)) / 8388608.0;
            const double thr = t0 * (double)p_.tempo * 1.8909;
            if (++counter_ > (int)thr) phase_ = kDec;
            break; }

        case kDec:  level_ = -level_ * exact_tables::env::kEnvDec[(size_t)p_.dec]; break;  // tblB < 0
        case kRel:  level_ = -level_ * exact_tables::env::kEnvDec[(size_t)p_.rel]; break;
        case kKill: level_ = -level_ * exact_tables::env::kEnvDec[1]; break;  // $20 -> rnd -> 1
        }
        return std::fabs(level_);
    }

    // Один сэмпл. Возвращает мгновенный гейн огибающей (|level|·де-зиппер·рамп).
    // Кладётся в amp[] плагина (VOL^2/панорама применяются своими стадиями).
    inline float process() noexcept
    {
        if (cnt_ == 0)
        {
            frameStart_ = smooth_;
            frameTarget_ = tickFrame();
            smooth_ += 0.094f * (frameTarget_ - smooth_);   // де-зиппер 3/32
        }
        const float g = frameStart_ +
                        (frameTarget_ - frameStart_) * ((float)cnt_ / 16.0f);
        if (++cnt_ >= 16) cnt_ = 0;
        return g;
    }

    // Диагностика/интеграция: фаза для KILL-гейта delay-модуляции
    // (фаза 4 -> fb интегратора = 0, P:$0ABF-$0AC4) — в плагине можно
    // использовать так же, как в прошивке.
    int  phase() const noexcept { return phase_; }
    float levelSigned() const noexcept { return level_; }

private:
    Params p_ {};
    int   phase_ = kAtk;
    float level_ = 0.0f;     // ЗНАКОВЫЙ уровень (wrap в -1.0 — родное поведение)
    int   counter_ = 0;
    float smooth_ = 0.0f;    // де-зиппер (3/32 за кадр)
    float frameStart_ = 0.0f;
    float frameTarget_ = 0.0f;
    int   cnt_ = 0;
};

} // namespace mnm
} // namespace monomachine
