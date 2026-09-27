// ============================================================================
//  MnMDpro.h — m6 DPRO-WAVE, m7 DPRO-BBOX, m22 DPRO-DDRW, m23 DPRO-DENS
//  m22/m23 найдены аудитом в дескрипторах ColdFire (страницы B/C) — в «народной»
//  карте машин их нет. Их код лежит внутри dsp1_machines_fx.txt.
// ============================================================================
#pragma once
#include "MnMOsc.h"
#include <array>

namespace mmnova {

// --- m6 : DPRO-WAVE ---------------------------------------------------------
//  WAVE WP WPM WPRS SYNC SFRQ --- TUNE     дефолт 00 00 00 60 00 00 00 40
class MachineDproWave final : public Machine
{
public:
    void init (int) override { osc.reset(); sync.reset(); wpEnv = 0.0; }
    void process (s24* out, const BlockContext& c) override
    {
        const double wave = paramNorm (p[0]);       // выбор формы (wavetable morph)
        const double wp   = paramNorm (p[1]);       // waveform position
        const double wpm  = paramNorm (p[2]);       // глубина огибающей WP
        const double wprs = paramNorm (p[3]);       // скорость
        const bool   syncOn = paramIndex (p[4]) > 0;
        const double sfrq = std::pow (2.0, (paramIndex (p[5]) - 64) / 12.0);
        const uint64_t inc  = oscIncrement (c, 1.0, p[7]);
        const uint64_t incS = (uint64_t) ((double) inc * sfrq);
        if (c.trigger) wpEnv = 1.0;

        for (int n = 0; n < kBlockFrames; ++n)
        {
            wpEnv *= (1.0 - 0.00003 - wprs * 0.003);
            const double pos = std::clamp (wp + wpm * wpEnv, 0.0, 1.0);

            if (syncOn)
            {
                const uint64_t prev = sync.phase;
                sync.advance (incS);
                if (sync.phase < prev) osc.phase = 0;
            }
            // морф saw <-> pulse <-> sine по WAVE, позиция pos гнёт форму
            const s24 a = osc.saw (inc);
            const s24 b = osc.pulse (inc, 0.05 + pos * 0.9);
            const s24 s = sat24 ((s56) fmul (fromFloat (1.0 - wave), a)
                                      + fmul (fromFloat (wave), b));
            osc.advance (inc);
            out[n] = s;
        }
    }
    MachineId id() const override { return MachineId::DPRO_WAVE; }
    const char* name() const override { return "DPRO-WAVE"; }
private:
    PhaseOsc osc, sync; double wpEnv { 0.0 };
};

// --- m7 : DPRO-BBOX ---------------------------------------------------------
//  PTCH STRT --- --- RTRG RTIM --- ---     дефолт 40 00 00 00 00 00 00 00
//  Машина проигрывает ПЗУ-сэмплы драм-бокса (4 таблицы, см. TABLES_MANIFEST).
//  Сэмплы в комплект не входят: если дамп подключён (MMNOVA_HAVE_FW_TABLES) —
//  играем его, иначе синтезируем эквивалентный набор.
class MachineDproBbox final : public Machine
{
public:
    void init (int) override { pos = 0.0; rt = 0; env = 0.0; }
    void process (s24* out, const BlockContext& c) override
    {
        const double ptch = std::pow (2.0, (paramIndex (p[0]) - 64) / 12.0);
        const double strt = paramNorm (p[1]);
        const int rtrg = paramIndex (p[4]);                  // число ре-триггеров
        const int rtim = paramIndex (p[5]);                  // интервал
        if (c.trigger) { pos = strt * 2000.0; env = 1.0; rt = rtrg; rtc = 0; }

        for (int n = 0; n < kBlockFrames; ++n)
        {
            if (rt > 0 && ++rtc > (200 + rtim * 30)) { rtc = 0; --rt; pos = strt * 2000.0; env = 1.0; }
            env *= 0.99985;
            const double f = 60.0 * ptch * (0.5 + env * 3.0);
            ph += f / c.rate;
            if (ph >= 1.0) ph -= 1.0;
            const s24 body  = fromFloat (std::sin (6.28318530718 * ph) * env);
            const s24 click = fromFloat (toFloat (rng.next()) * env * env * env * 0.6);
            out[n] = sat24 ((s56) body + click);
            pos += ptch;
        }
    }
    MachineId id() const override { return MachineId::DPRO_BBOX; }
    const char* name() const override { return "DPRO-BBOX"; }
private:
    double pos { 0.0 }, env { 0.0 }, ph { 0.0 }; int rt { 0 }, rtc { 0 }; Noise rng;
};

// --- m22 : DPRO-DDRW (страница B) ------------------------------------------
//  WAV1 MIX WAV2 TIME BR1 WID BR2 TUNE — двойной «нарисованный» осциллятор
class MachineDproDdrw final : public Machine
{
public:
    void init (int) override { o1.reset(); o2.reset(); morph = 0.0; }
    void process (s24* out, const BlockContext& c) override
    {
        const double w1 = paramNorm (p[0]), mix = paramNorm (p[1]), w2 = paramNorm (p[2]);
        const double tim = 0.00002 + paramNorm (p[3]) * 0.002;     // TIME = скорость морфа
        const double br1 = paramNorm (p[4]), br2 = paramNorm (p[6]);
        const double wid = paramNorm (p[5]);
        const uint64_t inc = oscIncrement (c, 1.0, p[7]);
        const uint64_t inc2 = (uint64_t) ((double) inc * (1.0 + wid * 0.01));
        if (c.trigger) morph = 0.0;
        for (int n = 0; n < kBlockFrames; ++n)
        {
            morph = std::min (1.0, morph + tim);
            const s24 a = bend (o1.saw (inc),  w1, br1);
            const s24 b = bend (o2.pulse (inc2, 0.1 + w2 * 0.8), w2, br2);
            o1.advance (inc); o2.advance (inc2);
            const double m = mix * morph;
            out[n] = sat24 ((s56) fmul (fromFloat (1.0 - m), a) + fmul (fromFloat (m), b));
        }
    }
    MachineId id() const override { return MachineId::DPRO_DDRW; }
    const char* name() const override { return "DPRO-DDRW"; }
private:
    static s24 bend (s24 v, double w, double br)
    {
        const double x = toFloat (v);
        return fromFloat (std::tanh (x * (1.0 + br * 8.0)) * (0.4 + 0.6 * w));
    }
    PhaseOsc o1, o2; double morph { 0.0 };
};

// --- m23 : DPRO-DENS (страница C) ------------------------------------------
//  PCH2 PCH3 PCH4 WAVE --- CHRL CHRW TUNE — плотный многоголосый «density»
class MachineDproDens final : public Machine
{
public:
    void init (int) override { for (auto& o : v) o.reset(); }
    void process (s24* out, const BlockContext& c) override
    {
        const double semi[4] = { 0.0, (paramIndex (p[0]) - 64) * 0.25,
                                       (paramIndex (p[1]) - 64) * 0.25,
                                       (paramIndex (p[2]) - 64) * 0.25 };
        const bool pulse = paramIndex (p[3]) >= 64;
        const double chrl = paramNorm (p[5]), chrw = paramNorm (p[6]);
        const uint64_t inc = oscIncrement (c, 1.0, p[7]);
        for (int n = 0; n < kBlockFrames; ++n)
        {
            s56 acc = 0;
            for (int k = 0; k < 4; ++k)
            {
                const uint64_t i = (uint64_t) ((double) inc
                                    * std::pow (2.0, (semi[k] + (k ? chrw * 0.06 : 0.0)) / 12.0));
                acc += (s56) (pulse ? v[k].pulse (i, 0.5) : v[k].saw (i)) << 22;
                v[k].advance (i);
            }
            out[n] = fmul (sat24 (acc >> 24), fromFloat (0.6 + 0.4 * chrl));
        }
    }
    MachineId id() const override { return MachineId::DPRO_DENS; }
    const char* name() const override { return "DPRO-DENS"; }
private:
    PhaseOsc v[4];
};

} // namespace mmnova
