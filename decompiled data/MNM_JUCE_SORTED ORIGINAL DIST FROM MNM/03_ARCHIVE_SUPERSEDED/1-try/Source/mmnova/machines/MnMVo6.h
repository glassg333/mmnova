// ============================================================================
//  MnMVo6.h — m11 VO-6 ($1464FC)
//  VOC1 VOC2 V-SW VOIC CONS CLEN CVOL TUNE   дефолт 40 40 60 00 00 40 40 40
//  Формантный «вокальный» синтезатор: два набора гласных (VOC1/VOC2),
//  V-SW = морф между ними, VOIC = выбор голоса, CONS/CLEN/CVOL = согласная
//  (шумовой burst заданной длины и громкости).
// ============================================================================
#pragma once
#include "MnMOsc.h"

namespace mmnova {

class MachineVo6 final : public Machine
{
public:
    void init (int) override
    { osc.reset(); for (auto& f : bp) f = BP{}; consEnv = 0.0; }

    void process (s24* out, const BlockContext& c) override
    {
        const double v1 = paramNorm (p[0]), v2 = paramNorm (p[1]);
        const double sw = paramNorm (p[2]);
        const int    voi = paramIndex (p[3]) * 6 / 128;         // VOIC: 6 «голосов»
        const int    cons = paramIndex (p[4]);
        const double clen = 0.002 + paramNorm (p[5]) * 0.3;
        const double cvol = paramNorm (p[6]);
        const uint64_t inc = oscIncrement (c, 1.0, p[7]);
        if (c.trigger) consEnv = 1.0;

        // форманты: интерполяция между двумя гласными (таблица гласных A E I O U Y)
        static constexpr double F[6][3] = {
            { 730, 1090, 2440 }, { 530, 1840, 2480 }, { 270, 2290, 3010 },
            { 570,  840, 2410 }, { 300,  870, 2240 }, { 440, 1020, 2240 } };
        const int ia = std::clamp ((int) (v1 * 5.0), 0, 5);
        const int ib = std::clamp ((int) (v2 * 5.0), 0, 5);
        const double sc = std::pow (2.0, (voi - 3) * 0.12);      // VOIC = сдвиг формант

        for (int k = 0; k < 3; ++k)
        {
            const double f = (F[ia][k] * (1.0 - sw) + F[ib][k] * sw) * sc;
            bp[k].set (f, 12.0, c.rate);
        }

        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 glottal = osc.saw (inc);                   // голосовой источник
            osc.advance (inc);
            s56 acc = 0;
            for (int k = 0; k < 3; ++k)
                acc += (s56) bp[k].process (glottal) << (23 - k);

            s24 s = sat24 (acc >> 24);
            if (cons > 0 && consEnv > 0.0)                       // согласная
            {
                consEnv -= 1.0 / (clen * c.rate);
                if (consEnv < 0.0) consEnv = 0.0;
                s = sat24 ((s56) s + fromFloat (toFloat (rng.next()) * consEnv * cvol));
            }
            out[n] = s;
        }
    }
    MachineId id() const override { return MachineId::VO_6; }
    const char* name() const override { return "VO-6"; }

private:
    struct BP
    {
        double b0 { 0 }, a1 { 0 }, a2 { 0 }, z1 { 0 }, z2 { 0 };
        void set (double f, double q, double sr)
        {
            const double w = 6.28318530718 * std::min (f, sr * 0.45) / sr;
            const double r = std::exp (-w / (2.0 * q));
            a1 = 2.0 * r * std::cos (w); a2 = -r * r; b0 = (1.0 - r) * 0.9;
        }
        s24 process (s24 in)
        {
            const double x = toFloat (in);
            const double y = b0 * x + a1 * z1 + a2 * z2;
            z2 = z1; z1 = y;
            return fromFloat (std::clamp (y, -1.0, 1.0));
        }
    };
    PhaseOsc osc; BP bp[3]; Noise rng; double consEnv { 0.0 };
};

} // namespace mmnova
