// ============================================================================
//  MnMFm.h — m8 FM+STAT ($145D21), m9 FM+PAR ($145EDB), m10 FM+DYN ($1461C1)
//
//  Раскладка состояния (из листингов, R6-относительно):
//    +$4..+$B  параметры;  +$10/$11 фаза несущей (48 бит);  +$12/$13 фаза
//    модулятора;  +$1A состояние FB;  +$28 огибающая;  +$2C/$2E усиления.
//  Таблицы: отношения P:$141A80 (24 шт.), LP P:$144AC7, вейвтейбл P:$141880,
//  синус $14A000. Питч: engine_pitch * $0BE37C -> 48-бит аккумулятор L:$5.
// ============================================================================
#pragma once
#include "MnMOsc.h"

namespace mmnova {

// Огибающая оператора: параметр xENV = скорость спада (таблица Y:$141800),
// значение хранится в +$28, читается как множитель девиации.
struct OpEnv
{
    s24 v { 0 };
    inline void trigger() noexcept { v = kOne24; }
    inline s24  tick (int rate7) noexcept
    {
        const s24 r = tables::rate()[rate7 & 0x7F];
        v = sat24 ((s56) v - fmul (r, v));
        return v;
    }
};

inline int ratioIndex (s24 paramWordV) noexcept          // 2FRQ/3FRQ -> 0..23
{
    const int i = paramIndex (paramWordV) * tables::kFmRatioCount / 128;
    return std::clamp (i, 0, tables::kFmRatioCount - 1);
}

// --------------------------------------------------------------- FM+STAT ---
//  1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE   дефолт 3C 40 50 1E 50 40 62 40
class MachineFmStat final : public Machine
{
public:
    void init (int) override { car.reset(); mod.reset(); fb = 0; env.v = 0; lp.reset(); }
    void process (s24* out, const BlockContext& c) override
    {
        const double rC   = tables::kFmRatioF[ratioIndex (p[0])];      // 1FRQ
        const double fine = 1.0 + (paramIndex (p[1]) - 64) * 0.0008;   // 1FIN
        const int    e1   = paramIndex (p[2]);                         // 1ENV
        const s24    fbA  = p[3];                                      // 1FB
        const double rM   = tables::kFmRatioF[ratioIndex (p[4])];      // 2FRQ
        const s24    vol  = p[5];                                      // 2VOL
        const s24    tone = tables::lpFromParam (p[6]);                // TONE
        const uint64_t base = oscIncrement (c, 1.0, p[7]);
        const uint64_t iC = (uint64_t) ((double) base * rC * fine);
        const uint64_t iM = (uint64_t) ((double) base * rM);
        if (c.trigger) env.trigger();

        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 e = env.tick (e1);

            // модулятор с рекурсивным фидбеком (bset #$14,sr — сатурация ALU)
            uint64_t mp = mod.phase + ((uint64_t) (int64_t) fmul (fbA, fb) << 24);
            const s24 m = lerpTable<tables::kSineBits> (tables::sine(), mp);
            fb = sat24 (((s56) fb + m) >> 1);
            mod.advance (iM);

            // несущая: девиация = m * 1ENV(env) * 2VOL
            const s24 dev = fmul (fmul (m, e), vol);
            uint64_t cp = car.phase + ((uint64_t) (int64_t) dev << 25);
            s24 s = lerpTable<tables::kSineBits> (tables::sine(), cp);
            car.advance (iC);

            out[n] = lp.process (s, tone);                              // TONE
        }
    }
    MachineId id() const override { return MachineId::FM_STAT; }
    const char* name() const override { return "FM-STAT"; }
private:
    PhaseOsc car, mod; s24 fb { 0 }; OpEnv env; OnePoleLP lp;
};

// ---------------------------------------------------------------- FM+PAR ---
//  1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE   дефолт 3C 40 50 40 66 50 62 40
//  Три параллельных модулятора на одну несущую.
class MachineFmPar final : public Machine
{
public:
    void init (int) override
    { car.reset(); for (auto& m : mod) m.reset(); for (auto& e : env) e.v = 0; lp.reset(); }

    void process (s24* out, const BlockContext& c) override
    {
        const double r[3] = { tables::kFmRatioF[ratioIndex (p[0])],
                              tables::kFmRatioF[ratioIndex (p[2])],
                              tables::kFmRatioF[ratioIndex (p[4])] };
        const int    e7[3] = { paramIndex (p[1]), paramIndex (p[3]), paramIndex (p[5]) };
        const s24    tone  = tables::lpFromParam (p[6]);
        const uint64_t base = oscIncrement (c, 1.0, p[7]);
        uint64_t inc[3];
        for (int k = 0; k < 3; ++k) inc[k] = (uint64_t) ((double) base * r[k]);
        if (c.trigger) for (auto& e : env) e.trigger();

        for (int n = 0; n < kBlockFrames; ++n)
        {
            s56 dev = 0;
            for (int k = 0; k < 3; ++k)
            {
                const s24 e = env[k].tick (e7[k]);
                dev += mpy (mod[k].sine(), e);            // сумма модуляций
                mod[k].advance (inc[k]);
            }
            const s24 d = sat24 (dev >> 25);
            uint64_t cp = car.phase + ((uint64_t) (int64_t) d << 25);
            const s24 s = lerpTable<tables::kSineBits> (tables::sine(), cp);
            car.advance (base);
            out[n] = lp.process (s, tone);
        }
    }
    MachineId id() const override { return MachineId::FM_PAR; }
    const char* name() const override { return "FM-PAR"; }
private:
    PhaseOsc car, mod[3]; OpEnv env[3]; OnePoleLP lp;
};

// ---------------------------------------------------------------- FM+DYN ---
//  1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB TUNE   дефолт 40 40 40 40 4A 50 1E 40
//  Динамический скан вейвтейбла P:$141880 параметром 2ENV:
//     n1 = |2ENV*scale - $400000| >> 15
//  Условный знак фидбека (tfr x0,a ifmi).
class MachineFmDyn final : public Machine
{
public:
    void init (int) override { car.reset(); mod.reset(); fb = 0; fEnv.v = vEnv.v = dEnv.v = 0; }
    void process (s24* out, const BlockContext& c) override
    {
        const double rC = tables::kFmRatioF[ratioIndex (p[0])];   // 1FRQ
        const int    fe = paramIndex (p[1]);                      // 1FEN
        const s24    vol= p[2];                                   // 1VOL
        const int    ve = paramIndex (p[3]);                      // 1VEN
        const double rM = tables::kFmRatioF[ratioIndex (p[4])];   // 2FRQ
        const s24    d2 = p[5];                                   // 2ENV (скан)
        const s24    fbA= p[6];                                   // 2FB
        const uint64_t base = oscIncrement (c, 1.0, p[7]);
        const uint64_t iM = (uint64_t) ((double) base * rM);
        if (c.trigger) { fEnv.trigger(); vEnv.trigger(); dEnv.trigger(); }

        const s24* wt = tables::fmDynWave();

        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 fv = fEnv.tick (fe);                        // огибающая частоты
            const s24 av = vEnv.tick (ve);                        // огибающая громкости

            // точка скана волны: n1 = |2ENV*scale - $400000| >> 15
            const int32_t scan = (int32_t) std::abs ((int32_t) fmul (d2, dEnv.tick (paramIndex (d2))) - 0x400000) >> 15;
            const int wIdx = std::clamp ((int) scan, 0, 1023);

            uint64_t mp = mod.phase + ((uint64_t) (int64_t) fmul (fbA, fb) << 24);
            s24 m = lerpTable<tables::kSineBits> (tables::sine(), mp);
            m = sat24 (((s56) m + wt[wIdx]) >> 1);                // подмес волны из таблицы
            fb = (fb < 0) ? sat24 (-(s56) m) : m;                 // условный знак (ifmi)
            mod.advance (iM);

            const uint64_t iC = (uint64_t) ((double) base * rC
                                * (1.0 + toFloat (fv) * 0.25));   // 1FEN двигает частоту
            uint64_t cp = car.phase + ((uint64_t) (int64_t) m << 25);
            s24 s = lerpTable<tables::kSineBits> (tables::sine(), cp);
            car.advance (iC);

            out[n] = fmul (fmul (s, vol), av);
        }
    }
    MachineId id() const override { return MachineId::FM_DYN; }
    const char* name() const override { return "FM-DYN"; }
private:
    PhaseOsc car, mod; s24 fb { 0 }; OpEnv fEnv, vEnv, dEnv;
};

} // namespace mmnova
