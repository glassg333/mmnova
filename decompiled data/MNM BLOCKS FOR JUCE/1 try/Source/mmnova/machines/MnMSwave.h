// ============================================================================
//  MnMSwave.h — m4 SWAVE-SAW, m5 SWAVE-PULS, m14 SWAVE-ENS
//  ВНИМАНИЕ (аудит, Часть 2): m14 — это SWAVE-ENS, а НЕ FX-EQ.
//  Дескриптор ColdFire @0x58965: PCH2 PCH3 PCH4 WAVE PW CHRL CHRW TUNE.
// ============================================================================
#pragma once
#include "MnMOsc.h"

namespace mmnova {

// общий унисонный блок: UNIL (уровень) / UNIW (расстройка) / UNIX (разброс фаз)
struct UnisonBank
{
    static constexpr int kN = 7;
    PhaseOsc o[kN];
    void reset (bool spread)
    {
        for (int i = 0; i < kN; ++i)
            o[i].phase = spread ? (uint64_t) i * 0x2492492492ULL * 4096ULL : 0;
    }
};

// --- m4 : SWAVE-SAW ---------------------------------------------------------
//  UNIL UNIW UNIX --- SUBX SUB1 SUB2 TUNE
class MachineSwaveSaw final : public Machine
{
public:
    void init (int) override { bank.reset (true); sub1.reset(); sub2.reset(); }
    void process (s24* out, const BlockContext& c) override
    {
        const double lvl   = paramNorm (p[0]);
        const double det   = paramNorm (p[1]) * 0.035;      // UNIW -> расстройка
        const bool   spread= paramIndex (p[2]) > 0;         // UNIX
        const double subX  = paramNorm (p[4]);              // SUBX = вид субы
        const double s1    = paramNorm (p[5]);
        const double s2    = paramNorm (p[6]);
        const uint64_t inc = oscIncrement (c, 1.0, p[7]);
        if (spread != lastSpread) { bank.reset (spread); lastSpread = spread; }

        uint64_t incs[UnisonBank::kN];
        for (int v = 0; v < UnisonBank::kN; ++v)
        {
            const double d = (v - 3) * det / 3.0;
            incs[v] = (uint64_t) ((double) inc * std::pow (2.0, d));
        }
        const uint64_t i1 = inc >> 1, i2 = inc >> 2;

        for (int n = 0; n < kBlockFrames; ++n)
        {
            s56 acc = 0;
            acc += (s56) bank.o[0].saw (incs[0]) << 24;
            for (int v = 1; v < UnisonBank::kN; ++v)
                acc += (s56) fromFloat (lvl) * bank.o[v].saw (incs[v]) >> 0;
            for (int v = 0; v < UnisonBank::kN; ++v) bank.o[v].advance (incs[v]);

            s24 s = sat24 (acc >> 24 >> 1);
            if (s1 > 0.0) s = sat24 ((s56) s + fmul (fromFloat (s1),
                             subX > 0.5 ? sub1.pulse (i1, 0.5) : sub1.saw (i1)));
            if (s2 > 0.0) s = sat24 ((s56) s + fmul (fromFloat (s2),
                             subX > 0.5 ? sub2.pulse (i2, 0.5) : sub2.saw (i2)));
            sub1.advance (i1); sub2.advance (i2);
            out[n] = s;
        }
    }
    MachineId id() const override { return MachineId::SWAVE_SAW; }
    const char* name() const override { return "SWAVE-SAW"; }
private:
    UnisonBank bank; PhaseOsc sub1, sub2; bool lastSpread { true };
};

// --- m5 : SWAVE-PULS --------------------------------------------------------
//  UNIL UNIW SUB1 SUB2 PW PWAD PWRS TUNE   (дефолт PW = $40)
class MachineSwavePuls final : public Machine
{
public:
    void init (int) override { bank.reset (true); sub1.reset(); sub2.reset(); pwEnv = 0.0; }
    void process (s24* out, const BlockContext& c) override
    {
        const double lvl = paramNorm (p[0]);
        const double det = paramNorm (p[1]) * 0.035;
        const double s1  = paramNorm (p[2]);
        const double s2  = paramNorm (p[3]);
        const double pw0 = 0.02 + paramNorm (p[4]) * 0.96;
        const double ad  = paramNorm (p[5]);                 // PWAD: глубина ADSR по PW
        const double rs  = paramNorm (p[6]);                 // PWRS: скорость
        const uint64_t inc = oscIncrement (c, 1.0, p[7]);
        if (c.trigger) pwEnv = 1.0;

        uint64_t incs[UnisonBank::kN];
        for (int v = 0; v < UnisonBank::kN; ++v)
            incs[v] = (uint64_t) ((double) inc * std::pow (2.0, (v - 3) * det / 3.0));
        const uint64_t i1 = inc >> 1, i2 = inc >> 2;

        for (int n = 0; n < kBlockFrames; ++n)
        {
            pwEnv *= (1.0 - 0.0002 - rs * 0.01);
            const double pw = std::clamp (pw0 + ad * pwEnv * 0.45, 0.02, 0.98);
            s56 acc = (s56) bank.o[0].pulse (incs[0], pw) << 24;
            for (int v = 1; v < UnisonBank::kN; ++v)
                acc += (s56) fromFloat (lvl) * bank.o[v].pulse (incs[v], pw);
            for (int v = 0; v < UnisonBank::kN; ++v) bank.o[v].advance (incs[v]);

            s24 s = sat24 (acc >> 24 >> 1);
            if (s1 > 0.0) s = sat24 ((s56) s + fmul (fromFloat (s1), sub1.pulse (i1, 0.5)));
            if (s2 > 0.0) s = sat24 ((s56) s + fmul (fromFloat (s2), sub2.pulse (i2, 0.5)));
            sub1.advance (i1); sub2.advance (i2);
            out[n] = s;
        }
    }
    MachineId id() const override { return MachineId::SWAVE_PULS; }
    const char* name() const override { return "SWAVE-PULS"; }
private:
    UnisonBank bank; PhaseOsc sub1, sub2; double pwEnv { 0.0 };
};

// --- m14 : SWAVE-ENS (ensemble) --------------------------------------------
//  PCH2 PCH3 PCH4 WAVE PW CHRL CHRW TUNE   дефолты 3F 3F 3F 00 40 00 7F 40
class MachineSwaveEns final : public Machine
{
public:
    void init (int) override
    {
        for (auto& o : v) o.reset();
        for (auto& l : chl) l.reset();
        for (auto& d : line) d.fill (0);
        wr = 0;
    }
    void process (s24* out, const BlockContext& c) override
    {
        // PCH2/3/4 = $3F..$41 -> расстройка в полутонах относительно голоса 1
        const double semis[4] = { 0.0,
            (paramIndex (p[0]) - 63) * 0.5, (paramIndex (p[1]) - 63) * 0.5,
            (paramIndex (p[2]) - 63) * 0.5 };
        const bool   isPulse = paramIndex (p[3]) >= 64;      // WAVE
        const double pw   = 0.02 + paramNorm (p[4]) * 0.96;
        const double chrL = paramNorm (p[5]);                // CHRL — уровень хора
        const double chrW = paramNorm (p[6]);                // CHRW — ширина
        const uint64_t inc = oscIncrement (c, 1.0, p[7]);

        for (int n = 0; n < kBlockFrames; ++n)
        {
            s56 acc = 0;
            for (int k = 0; k < 4; ++k)
            {
                const uint64_t i = (uint64_t) ((double) inc * std::pow (2.0, semis[k] / 12.0));
                acc += (s56) (isPulse ? v[k].pulse (i, pw) : v[k].saw (i)) << 22;
                v[k].advance (i);
            }
            s24 dry = sat24 (acc >> 24);

            // ансамблевый хор: 3 линии с LFO-модуляцией, матрица +-0.866
            s24 wet = 0;
            for (int k = 0; k < 3; ++k)
            {
                const s24 m = chl[k].tick (paramWord (28 + k * 3));
                const double d = 220.0 + 200.0 * chrW * (1.0 + toFloat (m));
                wet = sat24 ((s56) wet + (read (k, d) / 3));
                line[k][(size_t) wr] = dry;
            }
            wr = (wr + 1) & (kLen - 1);
            out[n] = sat24 ((s56) dry + fmul (fromFloat (chrL), wet));
        }
    }
    MachineId id() const override { return MachineId::SWAVE_ENS; }
    const char* name() const override { return "SWAVE-ENS"; }
private:
    static constexpr int kLen = 2048;
    PhaseOsc v[4];
    InlineLfo chl[3];
    std::array<s24, kLen> line[3];
    int wr { 0 };
    s24 read (int k, double d) const
    {
        const int di = (int) d; const double fr = d - di;
        const int a = (wr - di) & (kLen - 1), b = (a - 1) & (kLen - 1);
        return sat24 ((s56) (line[k][(size_t) a] * (1.0 - fr) + line[k][(size_t) b] * fr));
    }
};

} // namespace mmnova
