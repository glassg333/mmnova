// ============================================================================
// mnm_amp_env.h — AMP-энвелоп трека Monomachine (P:$088E–$08D7), точный порт.
//
// Реконструкция бит-точная (iter.13, 06_amp_env/README_amp_envelope.md):
// все временные карты совпали с эмулятором (ATK=64 -> 419 кадров и т.д.).
//
// ВАЖНО (ответ на «фильтр берёт энвелоуп от ампа?»):
//  - в САМ фильтр (BASE/WDTH/HPQ/LPQ) уровень энвелопа НЕ входит — доказано
//    бит-точной эмуляцией (08_env_verdict/, iter.15);
//  - но уровень энвелопа = ГЕЙН, применяемый по-сэмплово В РЕЗОНАНСНОЙ
//    секции (P:$0904–$0910: gain = |level|*VOL^2*pan, де-зиппер ~3/32,
//    внутриблочный рамп) — т.е. громкость огибающей живёт именно там;
//  - ФАЗА энвелопа (X[P+$D8]) гейтит обратную связь интегратора
//    delay-модуляции: фаза 4 (KILL) -> fb = 0 (P:$0ABF–$0AC4, листинг
//    снят в iter.16). Это единственное «проникновение» AMP-env в хвост тракта.
//
// Использование: tick() раз в кадр (16 сэмплов @44.1к = 0.3629 мс),
// trig(1)=note-on, trig(2)=note-off, trig(3)=kill.
// ============================================================================
#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>

#include "mnm_tables.h"

namespace mnm {

class MonoMachineAmpEnv
{
public:
    // Параметры страницы AMP (raw 0..127, как рукоятки машины)
    struct Params
    {
        int atk  = 0;      // AMP ATK  ($00)
        int hold = 0;      // AMP HOLD ($01), темп-зависимый
        int dec  = 90;     // AMP DEC  ($02)
        int rel  = 40;     // AMP REL  ($03)
        int vol  = 127;    // AMP VOL  ($05) — гейн ~ level*VOL^2
        int pan  = 64;     // AMP PAN  ($06) — 64 = центр
        int tempo = 120;   // хост-темп (Y[P+$23])
    };

    // Фазы — как в железе (X[P+$D8])
    enum Phase : int { kAtk = 0, kHold = 1, kDec = 2, kRel = 3, kKill = 4 };

    void reset() noexcept { phase = kAtk; level = 0.f; counter = 0; lastTrig = 0; }

    void trig (int t) noexcept        // 1 = note-on, 2 = note-off, 3 = kill
    {
        lastTrig = t;
        if (t == 1)      phase = kAtk;                    // уровень НЕ сбрасывается!
        else if (t == 2) phase = kRel;
        else if (t >= 3) phase = kKill;
    }

    // Один кадр (16 сэмплов). Возвращает |level| (гейн до VOL^2/панорамы).
    float tick (const Params& p) noexcept
    {
        switch (phase)
        {
        case kAtk:
            level += kEnvAtk[(size_t) p.atk];            // inc = tblA[ATK]
            if (level >= 1.f)
            {
                // 24-битный wrap атаки: уровень проскакивает в -1.0 (знак меняется!)
                level -= 2.f;
                phase = kHold;
                counter = 1;
            }
            break;

        case kHold: {
            // t0 = max(0, HOLD*2^16 - $10000); порог = t0*TEMPO*$791FD0*2/2^24
            const double t0  = std::max (0.0, (double) ((p.hold << 16) - 0x10000) / 65536.0);
            const double thr = t0 * (double) p.tempo * 0.945464;   // $791FD0*2/2^24
            if (++counter > (int) thr) phase = kDec;
            break; }

        case kDec:  level = -level * kEnvDec[(size_t) p.dec]; break;   // kEnvDec < 0!
        case kRel:  level = -level * kEnvDec[(size_t) p.rel]; break;
        case kKill: level = -level * kEnvDec[1]; break;   // $20 -> rnd -> idx 1
        }
        return std::fabs (level);
    }

    // Гейн по-сэмплово: |level| * VOL^2 * панорама + внутрикадровый рамп.
    // Точный путь P:$08DF–$08F9 (гейн = level*VOL^2; рамп шагом x8 на сэмпл).
    void gainFrame (const Params& p, float* io, /*interleaved*/ bool /*stereo*/ = true) noexcept
    {
        const float lvl = tick (p);
        const float v2  = (float) (p.vol * p.vol) / (127.f * 127.f);
        // панорама: центр = единица; края по синус-закону узла P:$08E5
        const float pb = (float) p.pan / 127.f;
        const float gl = v2 * std::cos (pb * 1.178097f);   // (1-pan)*pi/2*0.75
        const float gr = v2 * std::sin (pb * 1.178097f + 0.3926991f);
        // де-зиппер оригинала ~3/32 за кадр:
        smoothL += 0.094f * (lvl * gl - smoothL);
        smoothR += 0.094f * (lvl * gr - smoothR);
        const float stL = (smoothL - prevL) / (float) kFrame / 1.0f; // внутрикадровый рамп
        const float stR = (smoothR - prevR) / (float) kFrame;
        for (int i = 0; i < kFrame; ++i)
        {
            io[2 * i]     *= prevL + stL * (float) i;
            io[2 * i + 1] *= prevR + stR * (float) i;
        }
        prevL = smoothL; prevR = smoothR;
    }

    int   phase = kAtk;
    float level = 0.f;      // ЗНАКОВЫЙ уровень (wrap в -1.0 — родное поведение)
    int   phaseForDelayMod() const noexcept { return phase; }  // X[P+$D8]: KILL-гейт

private:
    static constexpr int kFrame = 16;
    int   counter = 0;
    int   lastTrig = 0;
    float smoothL = 0.f, smoothR = 0.f, prevL = 0.f, prevR = 0.f;
};

} // namespace mnm
