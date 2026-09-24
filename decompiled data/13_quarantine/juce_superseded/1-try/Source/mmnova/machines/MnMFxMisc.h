// ============================================================================
//  MnMFxMisc.h — остальные FX-машины страницы A и B/C
//    m12 FX-THRU     (18 слов кода, только INP)
//    m13 FX-REVERB   DEC DAMP GATE MIX HP LP --- INP   40 02 7F 20 00 7F 00 40
//    m16 FX-DYNAMIX  ATK REL THRS MIX RAT GAIN RMS INP 40 40 40 7F 00 00 00 40
//    m17 FX-RINGMOD  WAVE EXT --- MIX --- --- --- INP
//    m18 FX-PHASER   CNTR DEP SPD MIX FB WID --- INP   40 40 40 7F 2D 00 7F 40
//    m19 FX-FLANGER  DEL DEP SPD MIX FB WID --- INP    40 40 40 7F 2D 00 7F 40
//    m32 FX-DLY      кольцевой буфер X:$114000 (2 x 2048, канал B +$800)
//    m33 FX-EXT      bitcrush/downsample + внешний вход, 4 канала
//
//  Единые правила железа (соблюдены во всех): вход = in*INP^2*4,
//  выход = dry*($7FFFFF-MIX) + wet*MIX, LFO inline (синус $14A000 + SPD),
//  LP по таблице $144AC7, сатурация ALU вместо отдельного дисторшна.
// ============================================================================
#pragma once
#include "MnMOsc.h"
#include <array>

namespace mmnova {

// ------------------------------------------------------------- m12 THRU ----
class MachineFxThru final : public Machine
{
public:
    void init (int) override {}
    void process (s24* out, const BlockContext& c) override
    { for (int i = 0; i < kBlockFrames; ++i) out[i] = fxInput (c.input ? c.input[i] : 0, p[7]); }
    MachineId id() const override { return MachineId::FX_THRU; }
    const char* name() const override { return "FX-THRU"; }
};

// ----------------------------------------------------------- m13 REVERB ----
class MachineFxReverb final : public Machine
{
public:
    enum { DEC = 0, DAMP, GATE, MIX, HP, LP_, UNUSED, INP };
    void init (int) override
    {
        for (int i = 0; i < 4; ++i) { comb[i].fill (0); ci[i] = 0; cz[i] = 0; }
        for (int i = 0; i < 2; ++i) { ap[i].fill (0); ai[i] = 0; }
        hpZ = 0; lpF.reset(); gateEnv = 0;
    }
    void process (s24* out, const BlockContext& c) override
    {
        const s24 dec  = p[DEC];
        const s24 damp = tables::lpFromParam (sat24 (kOne24 - p[DAMP]));
        const s24 lpC  = tables::lpFromParam (p[LP_]);
        const double hpA = 1.0 - paramNorm (p[HP]) * 0.05;
        const int    gate = paramIndex (p[GATE]);
        const s24 fbAmt = fromFloat (0.70 + paramNorm (dec) * 0.28);

        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 dry = fxInput (c.input ? c.input[n] : 0, p[INP]);
            s56 acc = 0;
            for (int k = 0; k < 4; ++k)
            {
                const int len = kCombLen[k];
                s24 v = comb[k][(size_t) ci[k]];
                cz[k] = sat24 ((s56) cz[k] + fmul (damp, sat24 ((s56) v - cz[k])));  // DAMP
                comb[k][(size_t) ci[k]] = sat24 ((s56) dry + fmul (fbAmt, cz[k]));
                ci[k] = (ci[k] + 1) % len;
                acc += (s56) v << 22;
            }
            s24 w = sat24 (acc >> 24);
            for (int k = 0; k < 2; ++k)                       // 2 allpass
            {
                const int len = kApLen[k];
                const s24 bufv = ap[k][(size_t) ai[k]];
                const s24 in2  = sat24 ((s56) w + fmul (fromFloat (0.5), bufv));
                w = sat24 ((s56) bufv - fmul (fromFloat (0.5), in2));
                ap[k][(size_t) ai[k]] = in2;
                ai[k] = (ai[k] + 1) % len;
            }
            w = lpF.process (w, lpC);                          // LP
            const s24 hp = sat24 ((s56) w - hpZ);              // HP (один полюс)
            hpZ = sat24 (hpZ + fromFloat (1.0 - hpA) * 0 + fmul (fromFloat (1.0 - hpA), hp));
            w = paramIndex (p[HP]) ? hp : w;

            if (gate < 127)                                    // GATE: обрыв хвоста
            {
                const s24 target = (std::abs (dry) > (kOne24 >> 6)) ? kOne24 : 0;
                gateEnv = sat24 ((s56) gateEnv + ((target - gateEnv) >> (1 + (127 - gate) / 16)));
                w = fmul (w, gateEnv);
            }
            out[n] = fxMix (dry, w, p[MIX]);
        }
    }
    MachineId id() const override { return MachineId::FX_REVERB; }
    const char* name() const override { return "FX-REVERB"; }
private:
    static constexpr int kCombLen[4] = { 1116, 1188, 1277, 1356 };
    static constexpr int kApLen[2]   = { 225, 556 };
    std::array<s24, 1400> comb[4];
    std::array<s24, 600>  ap[2];
    int ci[4] { 0,0,0,0 }; int ai[2] { 0,0 };
    s24 cz[4] { 0,0,0,0 }; s24 hpZ { 0 }; s24 gateEnv { 0 };
    OnePoleLP lpF;
};

// ------------------------------------------- общая база линий задержки ------
//  X:$114000 + voice*$2000 : 2 x 2048 сэмпла, второй канал +$800
template <int kLen>
struct DelayLine
{
    std::array<s24, kLen> buf {};
    int wr { 0 };
    void clear() { buf.fill (0); wr = 0; }
    inline void write (s24 v) noexcept { buf[(size_t) wr] = v; wr = (wr + 1) & (kLen - 1); }
    inline s24 read (double d) const noexcept
    {
        d = std::clamp (d, 1.0, (double) (kLen - 2));
        const int di = (int) d; const uint32_t fr = (uint32_t) ((d - di) * 65536.0);
        const s24 a = buf[(size_t) ((wr - di) & (kLen - 1))];
        const s24 b = buf[(size_t) ((wr - di - 1) & (kLen - 1))];
        return (s24) (a + (((s56) (b - a) * (s56) fr) >> 16));
    }
};

// ---------------------------------------------------------- m19 FLANGER ----
class MachineFxFlanger final : public Machine
{
public:
    enum { DEL = 0, DEP, SPD, MIX, FB, WID, UNUSED, INP };
    void init (int) override { dl.clear(); lfo.reset(); fade = 0; }
    void process (s24* out, const BlockContext& c) override
    {
        const s24 wetGain = (fade >= 0x80) ? kOne24 : 0;  if (fade < 0x80) ++fade;
        const double d0  = 1.0 + paramNorm (p[DEL]) * 200.0;
        const double dep = paramNorm (p[DEP]) * 180.0;
        const s24    fbA = sat24 ((s56) p[FB] - 0x2D0000);   // дефолт $2D = 0 обратной связи
        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 dry = fxInput (c.input ? c.input[n] : 0, p[INP]);
            const s24 m   = lfo.tick (p[SPD]);
            const s24 wet = fmul (dl.read (d0 + dep * (0.5 + 0.5 * toFloat (m))), wetGain);
            dl.write (sat24 ((s56) dry + fmul (fbA, wet)));
            out[n] = fxMix (dry, wet, p[MIX]);
        }
    }
    MachineId id() const override { return MachineId::FX_FLANGER; }
    const char* name() const override { return "FX-FLANGER"; }
private:
    DelayLine<2048> dl; InlineLfo lfo; int fade { 0 };
};

// ----------------------------------------------------------- m18 PHASER ----
class MachineFxPhaser final : public Machine
{
public:
    enum { CNTR = 0, DEP, SPD, MIX, FB, WID, UNUSED, INP };
    void init (int) override { for (auto& z : ap) z = 0; lfo.reset(); fbZ = 0; }
    void process (s24* out, const BlockContext& c) override
    {
        const double ctr = paramNorm (p[CNTR]);
        const double dep = paramNorm (p[DEP]);
        const s24    fbA = sat24 ((s56) p[FB] - 0x2D0000);
        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 dry = fxInput (c.input ? c.input[n] : 0, p[INP]);
            const double mo = 0.5 + 0.5 * toFloat (lfo.tick (p[SPD]));
            const double g  = std::clamp (0.05 + (ctr + dep * mo) * 0.9, 0.02, 0.97);
            const s24 gc = fromFloat (g);
            s24 x = sat24 ((s56) dry + fmul (fbA, fbZ));
            for (int k = 0; k < 6; ++k)                    // 6 ступеней all-pass
            {
                const s24 y = sat24 ((s56) fmul (gc, x) + ap[k]);
                ap[k] = sat24 ((s56) x - fmul (gc, y));
                x = y;
            }
            fbZ = x;
            out[n] = fxMix (dry, x, p[MIX]);
        }
    }
    MachineId id() const override { return MachineId::FX_PHASER; }
    const char* name() const override { return "FX-PHASER"; }
private:
    s24 ap[6] { 0,0,0,0,0,0 }; s24 fbZ { 0 }; InlineLfo lfo;
};

// ---------------------------------------------------------- m16 DYNAMIX ----
//  ATK REL THRS MIX RAT GAIN RMS INP — компрессор с RMS-детектором
class MachineFxDynamix final : public Machine
{
public:
    enum { ATK = 0, REL, THRS, MIX, RAT, GAIN, RMS, INP };
    void init (int) override { rms = 0; gr = kOne24; }
    void process (s24* out, const BlockContext& c) override
    {
        const s24 atk = tables::rateFromParam (p[ATK]);
        const s24 rel = tables::rateFromParam (p[REL]);
        const double thr  = 0.02 + paramNorm (p[THRS]) * 0.98;
        const double ratio= 1.0 + paramNorm (p[RAT]) * 19.0;
        const double gain = std::pow (10.0, paramNorm (p[GAIN]) * 24.0 / 20.0);
        const s24 rmsC = tables::lpFromParam (p[RMS]);
        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 dry = fxInput (c.input ? c.input[n] : 0, p[INP]);
            const s24 sq  = fmul (dry, dry);
            rms = sat24 ((s56) rms + fmul (rmsC, sat24 ((s56) sq - rms)));
            const double lvl = std::sqrt (std::max (0.0, (double) toFloat (rms)));
            double target = 1.0;
            if (lvl > thr) target = std::pow (thr / lvl, 1.0 - 1.0 / ratio);
            const s24 tg = fromFloat (target);
            gr = sat24 ((s56) gr + fmul (tg < gr ? atk : rel, sat24 ((s56) tg - gr)));
            const s24 wet = sat24 ((s56) fmul (dry, gr) * 0 + fromFloat (
                              std::clamp (toFloat (fmul (dry, gr)) * gain, -1.0f, 1.0f)));
            out[n] = fxMix (dry, wet, p[MIX]);
        }
    }
    MachineId id() const override { return MachineId::FX_DYNAMIX; }
    const char* name() const override { return "FX-DYNAMIX"; }
private:
    s24 rms { 0 }, gr { kOne24 };
};

// ---------------------------------------------------------- m17 RINGMOD ----
//  WAVE EXT --- MIX --- --- --- INP
class MachineFxRingmod final : public Machine
{
public:
    enum { WAVE = 0, EXT, U2, MIX, U4, U5, U6, INP };
    void init (int) override { osc.reset(); }
    void process (s24* out, const BlockContext& c) override
    {
        const int wave = paramIndex (p[WAVE]) * 4 / 128;
        const uint64_t inc = oscIncrement (c, 1.0, paramWord (64),
                                           0.25 + paramNorm (p[EXT]) * 8.0);
        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 dry = fxInput (c.input ? c.input[n] : 0, p[INP]);
            s24 car;
            switch (wave)
            {
                case 1:  car = osc.saw (inc); break;
                case 2:  car = osc.pulse (inc, 0.5); break;
                case 3:  car = rng.next(); break;
                default: car = osc.sine(); break;
            }
            osc.advance (inc);
            out[n] = fxMix (dry, fmul (dry, car), p[MIX]);
        }
    }
    MachineId id() const override { return MachineId::FX_RINGMOD; }
    const char* name() const override { return "FX-RINGMOD"; }
private:
    PhaseOsc osc; Noise rng;
};

// -------------------------------------------------------------- m32 DLY ----
class MachineFxDelay final : public Machine
{
public:
    enum { TIME = 0, FB, U2, MIX, LP_, WID, U6, INP };
    void init (int) override { a.clear(); b.clear(); lpF.reset(); }
    void process (s24* out, const BlockContext& c) override
    {
        const double t = 8.0 + paramNorm (p[TIME]) * 2000.0;
        const s24 fbA  = p[FB];
        const s24 lpC  = tables::lpFromParam (p[LP_]);
        const double wid = paramNorm (p[WID]);
        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 dry = fxInput (c.input ? c.input[n] : 0, p[INP]);
            const s24 wa = a.read (t);
            const s24 wb = b.read (t * (1.0 + 0.5 * wid));   // канал B = +$800
            const s24 w  = sat24 (((s56) wa + wb) >> 1);
            const s24 fl = lpF.process (w, lpC);
            a.write (sat24 ((s56) dry + fmul (fbA, fl)));
            b.write (sat24 ((s56) dry + fmul (fbA, fl)));
            out[n] = fxMix (dry, w, p[MIX]);
        }
    }
    MachineId id() const override { return MachineId::FX_DLY; }
    const char* name() const override { return "FX-DLY"; }
private:
    DelayLine<2048> a, b; OnePoleLP lpF;
};

// -------------------------------------------------------------- m33 EXT ----
//  bitcrush / downsample + внешний вход. Децимация — fractional table lookup
//  (паттерн mpysu+dmac), питч-таблицы $101AFB/$101BFB/$101CFB.
class MachineFxExt final : public Machine
{
public:
    enum { SRR = 0, BITS, U2, MIX, DRIVE, U5, U6, INP };
    void init (int) override { acc = 0.0; hold = 0; }
    void process (s24* out, const BlockContext& c) override
    {
        const double step = std::pow (2.0, -8.0 * paramNorm (p[SRR]));
        const int bits = 24 - (int) (paramNorm (p[BITS]) * 20.0);
        const s24 mask = (s24) ~((1 << (24 - bits)) - 1);
        const double drive = 1.0 + paramNorm (p[DRIVE]) * 15.0;
        for (int n = 0; n < kBlockFrames; ++n)
        {
            const s24 dry = fxInput (c.input ? c.input[n] : 0, p[INP]);
            acc += step;
            if (acc >= 1.0) { acc -= 1.0; hold = dry; }
            s24 w = (s24) (hold & mask);
            w = fromFloat (std::tanh (toFloat (w) * drive));   // сатурация ALU
            out[n] = fxMix (dry, w, p[MIX]);
        }
    }
    MachineId id() const override { return MachineId::FX_EXT; }
    const char* name() const override { return "FX-EXT"; }
private:
    double acc { 0.0 }; s24 hold { 0 };
};

} // namespace mmnova
