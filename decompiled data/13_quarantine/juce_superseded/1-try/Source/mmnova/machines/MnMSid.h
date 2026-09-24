// ============================================================================
//  MnMSid.h — m3(слот 4) SID-6581
//  Дескриптор @0x581D5: PW PWAD PWRS WAVE MOD MSRC MFRQ TUNE
//  дефолты 00 00 60 00 00 00 40 40
//  Кривые дескриптора: PWRS/WAVE/MOD/MSRC — ступенчатые (тип 02/05/04/02),
//  поэтому WAVE/MSRC читаются как дискретные селекторы.
// ============================================================================
#pragma once
#include "MnMOsc.h"

namespace mmnova {

class MachineSid final : public Machine
{
public:
    enum Wave { Tri = 0, Saw, Pulse, Noise, TriSaw, TriPulse, SawPulse, All };

    void init (int) override { a.reset(); b.reset(); pwEnv = 0.0; lastA = 0; }

    void process (s24* out, const BlockContext& c) override
    {
        const double pw0  = 0.02 + paramNorm (p[0]) * 0.96;     // PW
        const double pwad = paramNorm (p[1]);                   // PWAD глубина
        const double pwrs = paramNorm (p[2]);                   // PWRS скорость
        const int    wave = paramIndex (p[3]) * 8 / 128;        // WAVE 8 комбинаций
        const int    mod  = paramIndex (p[4]);                  // MOD: 0=off 1=ring 2=sync
        const int    msrc = paramIndex (p[5]);                  // MSRC источник модуляции
        const double mfrq = std::pow (2.0, (paramIndex (p[6]) - 64) / 12.0);
        const uint64_t inc = oscIncrement (c, 1.0, p[7]);
        const uint64_t incM = (uint64_t) ((double) inc * mfrq);
        if (c.trigger) pwEnv = 1.0;

        const int modType = mod == 0 ? 0 : (msrc < 64 ? 1 : 2);

        for (int n = 0; n < kBlockFrames; ++n)
        {
            pwEnv *= (1.0 - 0.00005 - pwrs * 0.004);
            const double pw = std::clamp (pw0 + pwad * pwEnv * 0.45, 0.02, 0.98);

            // осциллятор B = модулятор (ring / sync)
            const uint64_t prevB = b.phase;
            b.advance (incM);
            const bool bWrapped = b.phase < prevB;

            if (modType == 2 && bWrapped) a.phase = 0;          // hard sync
            s24 s = renderWave (a, inc, pw, wave);
            if (modType == 1)                                    // ring mod
                s = fmul (s, b.sine());
            a.advance (inc);

            // SID 6581 имел ярко выраженную нелинейность выходного каскада
            s = sat24 ((s56) s - (s >> 5) + (lastA >> 6));
            lastA = s;
            out[n] = s;
        }
    }

    MachineId id() const override { return MachineId::SID_6581; }
    const char* name() const override { return "SID-6581"; }

private:
    s24 renderWave (PhaseOsc& o, uint64_t inc, double pw, int w)
    {
        switch (w)
        {
            case Tri:      { const double t = o.norm();
                             return fromFloat (4.0 * std::fabs (t - 0.5) - 1.0); }
            case Saw:      return o.saw (inc);
            case Pulse:    return o.pulse (inc, pw);
            case Noise:    return rng.next() >> 1;
            case TriSaw:   { const double t = o.norm();
                             return sat24 (((s56) fromFloat (4.0 * std::fabs (t - 0.5) - 1.0)
                                           + o.saw (inc)) >> 1); }
            case TriPulse: { const double t = o.norm();
                             return fmul (fromFloat (4.0 * std::fabs (t - 0.5) - 1.0),
                                          o.pulse (inc, pw)); }
            case SawPulse: return fmul (o.saw (inc), o.pulse (inc, pw));
            default:       return sat24 (((s56) o.saw (inc) + o.pulse (inc, pw)
                                          + (rng.next() >> 2)) >> 1);
        }
    }
    PhaseOsc a, b; Noise rng; double pwEnv { 0.0 }; s24 lastA { 0 };
};

} // namespace mmnova
