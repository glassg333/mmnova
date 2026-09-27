// ============================================================================
//  MnMGnd.h — m1 GND-GND, m2 GND-SIN (P:$144CD0), m3 GND-NOIS
// ============================================================================
#pragma once
#include "MnMOsc.h"

namespace mmnova {

// --- m1 : GND-GND — тишина (слот-заглушка диспетчера) -----------------------
class MachineGnd final : public Machine
{
public:
    void init (int) override {}
    void process (s24* out, const BlockContext&) override
    { for (int i = 0; i < kBlockFrames; ++i) out[i] = 0; }
    MachineId id() const override { return MachineId::GND_GND; }
    const char* name() const override { return "GND-GND"; }
};

// --- m2 : GND-SIN -----------------------------------------------------------
//  P:$144CD0 : A48 * $17C6F9, фаза 48 бит, чтение синуса $14A000, маска 13 бит.
//  Параметры: (—) x7, TUNE
class MachineGndSin final : public Machine
{
public:
    void init (int) override { osc.reset(); }
    void process (s24* out, const BlockContext& c) override
    {
        const uint64_t inc = oscIncrement (c, kGndSinRel, p[7]);
        for (int i = 0; i < kBlockFrames; ++i)
        {
            out[i] = osc.sine();
            osc.advance (inc);
        }
    }
    MachineId id() const override { return MachineId::GND_SIN; }
    const char* name() const override { return "GND-SIN"; }
private:
    PhaseOsc osc;
};

// --- m3 : GND-NOIS ----------------------------------------------------------
//  Параметры: ST RED STON --- --- --- --- TUNE
//    ST   — «stereo»/ширина шума (две декоррелированные копии)
//    RED  — red/brown окраска (интегратор), в железе — LP по таблице $144AC7
//    STON — «stone»: децимация сэмпл-рейта (fractional lookup, как в FX-EXT)
class MachineGndNoise final : public Machine
{
public:
    void init (int) override { lp.reset(); hold = 0; cnt = 0.0; }
    void process (s24* out, const BlockContext& c) override
    {
        const double stone = paramNorm (p[2]);
        const double step  = std::pow (2.0, -8.0 * stone);     // децимация
        const s24    red   = tables::lpFromParam (sat24 (kOne24 - p[1]));
        const s24    width = p[0];
        const uint64_t inc = oscIncrement (c, 1.0, p[7]);      // TUNE двигает stone-клок
        (void) inc;

        for (int i = 0; i < kBlockFrames; ++i)
        {
            cnt += step;
            if (cnt >= 1.0) { cnt -= 1.0; hold = rng.next(); }
            s24 s = lp.process (hold, red);                    // RED
            if (width) s = sat24 ((s56) s - fmul (width, rng.next() >> 2));
            out[i] = s;
        }
    }
    MachineId id() const override { return MachineId::GND_NOIS; }
    const char* name() const override { return "GND-NOIS"; }
private:
    Noise rng; OnePoleLP lp; s24 hold { 0 }; double cnt { 0.0 };
};

} // namespace mmnova
