// OmegaEngine.h — 8-voice stereo synthesis engine for Omega 8 / Omega CODE
// Osc: tri/saw/pulse + sub + noise, XMOD; filters: SEM (LP/BP/HP/BR), MINI
// (ladder), AUX1 (Oberheim SVF), AUX2 (CS80 HP+LP); 3 envs (A/D/Dk2/S/R),
// 2 LFOs + per-voice pan LFO; 24-slot mod matrix (LFO1 x3, LFO2 x3, ENV3 x3,
// modwheel/dynamics/bender/pressure/cont1/cont2 x2 each).
#pragma once
#include "OmegaPatch.h"
#include <cmath>
#include <algorithm>
#include <atomic>

#ifndef M_PI
 #define M_PI 3.14159265358979323846
#endif

namespace omega8
{

// ---------------------------------------------------------------------------
struct Env
{
    enum Stage { Idle, Delay, Attack, Decay, Sustain, Release };
    Stage stage = Idle;
    float level = 0, out = 0;
    float aT = 0.001f, dT = 0.001f, rT = 0.1f, sus = 1.0f;
    float dlyT = 0.0f, dlyLeft = 0.0f;

    static float time (int v) noexcept   // 0..127 -> seconds
    {
        if (v <= 0) return 0.0002f;
        return 0.0002f * std::pow (50000.0f, (float) v / 127.0f);
    }
    void setParams (int a, int d, int /*dk2*/, int s, int r, int dly = 0) noexcept
    {
        aT = time (a); dT = time (d); rT = time (r);
        sus = (float) s / 127.0f;
        dlyT = (dly <= 0 ? 0.0f : time (dly));   // r6: DLY перед ATTACK (как в оригинале)
    }
    void noteOn() noexcept
    {
        dlyLeft = dlyT;
        stage = (dlyT > 1.0e-3f ? Delay : Attack);
    }
    void noteOff() noexcept { if (stage != Idle) stage = Release; }
    void kill() noexcept { stage = Idle; level = 0; out = 0; }

    void process (float dt) noexcept
    {
        switch (stage)
        {
            case Attack:
                level += dt / std::max (aT, 1.0e-5f);
                if (level >= 1.0f) { level = 1.0f; stage = Decay; }
                break;
            case Decay:
            {
                float rate = dt / std::max (dT, 1.0e-5f);
                if (level > sus) { level -= rate; if (level <= sus) { level = sus; stage = Sustain; } }
                else             { level += rate; if (level >= sus) { level = sus; stage = Sustain; } }
                break;
            }
            case Sustain: level = sus; break;
            case Release:
                level -= (level / std::max (rT, 1.0e-5f)) * dt * 4.0f;   // exponential
                if (level < 1.0e-5f) { stage = Idle; level = 0; }
                break;
            case Idle:  level = 0; break;
            case Delay:
                dlyLeft -= dt;
                if (dlyLeft <= 0.0f) stage = Attack;
                break;
        }
        out = level;
    }
    float value() const noexcept { return out; }
};

// ---------------------------------------------------------------------------
struct Lfo
{
    double phase = 0.0;
    float  rateHz = 1.0f;
    bool   square = false;

    void setRate (int v) noexcept
    {
        rateHz = 0.02f * std::pow (500.0f, (float) v / 127.0f);
    }
    void process (double dt) noexcept
    {
        phase += rateHz * dt;
        if (phase >= 1.0) phase -= std::floor (phase);
    }
    float value() const noexcept    // bipolar -1..1
    {
        if (square) return phase < 0.5f ? 1.0f : -1.0f;
        return (float) std::sin (phase * 2.0 * M_PI);
    }
};

// ---------------------------------------------------------------------------
// TPT/Zavalishin state-variable filter — SEM family core
struct Svf
{
    float ic1 = 0, ic2 = 0;
    float g = 0.1f, k = 1.0f;
    void set (float freq, float q, float sr) noexcept
    {
        freq = std::clamp (freq, 10.0f, sr * 0.45f);
        g = std::tan ((float) M_PI * freq / sr);
        k = 1.0f / std::max (q, 0.5f);
    }
    // mode: 0 lp, 1 bp, 2 hp, 3 notch
    void process (float x, float& lp, float& bp, float& hp) noexcept
    {
        float v3 = x - ic2;
        float v1 = (g * v3 - g * ic1 + ic1) / (1.0f + g * (g + k));
        float v2 = ic2 + g * v1;
        ic1 = 2.0f * v1 - ic1;
        ic2 = 2.0f * v2 - ic2;
        lp = v2; bp = v1; hp = x - k * v1 - v2;
    }
    float run (float x, int mode) noexcept
    {
        float lp, bp, hp; process (x, lp, bp, hp);
        switch (mode) { case 1: return bp; case 2: return hp; case 3: return lp + hp; default: return lp; }
    }
    void reset() noexcept { ic1 = ic2 = 0; }
};

// 4-pole Moog-style ladder (MINI)
struct Ladder
{
    float s1 = 0, s2 = 0, s3 = 0, s4 = 0, fb = 0;
    float g = 0.1f, res = 0.0f;
    void set (float freq, float resonance, float sr) noexcept
    {
        freq = std::clamp (freq, 10.0f, sr * 0.4f);
        g = 2.0f * std::sin ((float) M_PI * freq / (2.0f * sr));
        g = std::min (g, 1.0f);
        res = std::clamp (resonance, 0.0f, 1.0f) * 1.6f;
    }
    float run (float x) noexcept
    {
        float in = std::tanh (x - res * std::tanh (fb));
        float y1 = s1 + g * (in  - s1);   if (y1 >  4.f) y1 =  4.f; if (y1 < -4.f) y1 = -4.f;
        float y2 = s2 + g * (y1 - s2);    if (y2 >  4.f) y2 =  4.f; if (y2 < -4.f) y2 = -4.f;
        float y3 = s3 + g * (y2 - s3);    if (y3 >  4.f) y3 =  4.f; if (y3 < -4.f) y3 = -4.f;
        float y4 = s4 + g * (y3 - s4);    if (y4 >  4.f) y4 =  4.f; if (y4 < -4.f) y4 = -4.f;
        s1 = y1; s2 = y2; s3 = y3; s4 = y4; fb = y4;
        return y4;
    }
    void reset() noexcept { s1 = s2 = s3 = s4 = fb = 0; }
};

// ===========================================================================
// r9: адаптированные фильтры из репо «juce random filters for import/!for omega8»
//     — OB-Xd/oberhx.h (Vadim), TB303 (Dominique Wurtz, blaukraut.info),
//     OberheimVariationMoog. Внешние заголовки (ObxdVoice, LadderFilterBase,
//     MoogUtils, just filt.h) заменены локальными эквивалентами.
// ---------------------------------------------------------------------------

// OB-Xd / oberhx.h — 2-полюсный ZDF SVF с диодной обратной связью (семейство
// SEM/OB-X): LP/BP/HP morph + band-reject. Работает в роли карты AUX1.
struct ObxdFilt
{
    float s1 = 0, s2 = 0;
    float R = 1;
    float mm = 0;
    bool  bandPassSw = false, brSw = false;
    float srInv = 1.0f / 44100.0f;

    void setSampleRate (float sr) noexcept { srInv = 1.0f / sr; }
    // res 0..1 -> R = 1-res (как в OB-Xd: R=0 в self-osc точке)
    void setResonance (float res) noexcept { R = 1.0f - res; }
    // mode: 0 lp, 1 bp, 2 hp, 3 br (семьи TYPE = карточки)
    void setMode (int mode) noexcept
    {
        brSw       = (mode == 3);
        bandPassSw = (mode == 1);
        mm         = mode == 2 ? 1.0f : (mode == 1 ? 0.5f : 0.0f);
    }
    static float diodePairResistanceApprox (float x) noexcept
    {
        return (((((0.0103592f) * x + 0.00920833f) * x + 0.185f) * x + 0.05f) * x + 1.0f);
    }
    // 0-delay feedback (диодная пара — «едкость» OB-Xd)
    float NR (float sample, float g) const noexcept
    {
        float tCfb = diodePairResistanceApprox (s1 * 0.0876f) - 1.0f;
        return (sample - 2.0f * (s1 * (R + tCfb)) - g * s1 - s2)
               / (1.0f + g * (2.0f * (R + tCfb) + g));
    }
    float Apply (float sample, float freqHz) noexcept
    {
        float g  = std::tan (freqHz * srInv * (float) M_PI);
        float v  = NR (sample, g);
        float y1 = v * g + s1;  s1 = v * g + y1;
        float y2 = y1 * g + s2; s2 = y1 * g + y2;
        if (brSw) return y2 + v;   // notch = LP + HP
        if (! bandPassSw) return (1.0f - mm) * y2 + mm * v;   // mm: 0=LP .. 1=HP
        return 2.0f * (mm < 0.5f ? ((0.5f - mm) * y2 + mm * y1)
                                 : ((1.0f - mm) * y1 + (mm - 0.5f) * v));
    }
    void reset() noexcept { s1 = s2 = 0; }
};

// OberhVar.h (OberheimVariationMoog) — TPT/ZDF 4-полюсник с tanh-сатюратором
// на входе; вместо «Oberheim variations» — taps {0,0,0,0,1} = чистый LP24.
// Заменяет старый эмпирический Ladder в роли MINI (Moog 24dB).
struct ObVarOne
{
    double z1 = 0, alpha = 1, beta = 0;
    double tick (double s) noexcept
    {
        double vn = (s - z1) * alpha;
        double out = vn + z1;
        z1 = vn + out;
        return out;
    }
    double gfo() const noexcept { return beta * z1; }
    void reset() noexcept { z1 = 0; }
};

struct ObVarLadder
{
    ObVarOne l1, l2, l3, l4;
    double K = 0, gamma = 0, alpha0 = 1;

    void set (float freq, float res01, float sr) noexcept
    {
        freq = std::clamp (freq, 10.0f, sr * 0.45f);
        const double T  = 1.0 / (double) sr;
        const double wd = 2.0 * M_PI * (double) freq;
        const double wa = (2.0 / T) * std::tan (wd * T * 0.5);   // BZT prewarp
        const double g  = wa * T * 0.5;
        const double G  = g / (1.0 + g);
        l1.alpha = l2.alpha = l3.alpha = l4.alpha = G;
        l1.beta = G * G * G / (1.0 + g);
        l2.beta = G * G     / (1.0 + g);
        l3.beta = G         / (1.0 + g);
        l4.beta = 1.0       / (1.0 + g);
        gamma  = G * G * G * G;
        K      = 4.0 * std::clamp (res01, 0.0f, 1.0f);   // как r in [1..10] -> K [0..4]
        alpha0 = 1.0 / (1.0 + K * gamma);
    }
    float run (float x) noexcept
    {
        double sigma = l1.gfo() + l2.gfo() + l3.gfo() + l4.gfo();
        double input = (double) x * (1.0 + K);
        double u = std::tanh ((input - K * sigma) * alpha0);
        double o1 = l1.tick (u);
        double o2 = l2.tick (o1);
        double o3 = l3.tick (o2);
        double o4 = l4.tick (o3);
        return (float) o4;
    }
    void reset() noexcept { l1.reset(); l2.reset(); l3.reset(); l4.reset(); }
};

// TB303.cpp (Dominique Wurtz) — диагональная лестница 303; Lerp/LookupTable из
// «just filt.h» заменены прямым вычислением коэффициентов. Только LP (карта).
struct Wurtz303
{
    double wc = 0, wc2 = 0, wc3 = 0, wc4 = 0;
    double b = 0, g = 0, k = 0, A = 1;
    double z0 = 0, z1 = 0, z2 = 0, z3 = 0;
    double y1 = 0, y2 = 0, y3 = 0, y4 = 0;
    double b0 = 0, a0 = 0, a1 = 0, a2 = 0, a3 = 0;
    double b10 = 0, a10 = 0, a11 = 0, a12 = 0, a13 = 0;
    double b20 = 0, a20 = 0, a21 = 0, a22 = 0, a23 = 0;
    double c2 = 0, c3 = 0;

    void init (double srate, double freq, double q) noexcept
    {
        // coeffLUT(ratio) из just filt.h = tan (min (0.499π, ratio·π))
        double ratio = 0.45 * M_PI * freq / srate * 0.5;
        wc  = std::tan (std::min (0.499 * M_PI, ratio * M_PI));
        wc2 = wc * wc;  wc3 = wc2 * wc;  wc4 = wc3 * wc;

        b = 1.0 / (1.0 + 8.0 * wc + 20.0 * wc2 + 16.0 * wc3 + 2.0 * wc4);
        g = 2.0 * wc4 * b;
        k = 16.95 * q;
        A = 1 + 0.5 * k;

        double dwc = 2 * wc, dwc2 = 2 * wc2, qwc2 = 4 * wc2;
        double dwc3 = 2 * wc3, qwc3 = 4 * wc3;
        b0  = dwc + 12 * wc2 + 20 * wc3 + 8 * wc4;
        a0  = 1 + 6 * wc + 10 * wc2 + qwc3;
        a1  = dwc + 8 * wc2 + 6 * wc3;
        a2  = dwc2 + wc3;
        a3  = dwc3;
        b10 = dwc2 + 8 * wc3 + 6 * wc4;
        a10 = wc + 4 * wc2 + 3 * wc3;
        a11 = 1 + 6 * wc + 11 * wc2 + 6 * wc3;
        a12 = wc + qwc2 + qwc3;
        a13 = wc2 + dwc3;
        b20 = dwc3 + 4 * wc4;
        a20 = a13;
        a21 = wc + qwc2 + 4 * wc3;
        a22 = 1 + 6 * wc + 10 * wc2 + qwc3;
        a23 = wc + qwc2 + dwc3;
        c2  = a21 - a3;
        c3  = 1 + 6 * wc + 9 * wc2 + dwc3;
    }

    double eval (double sample) noexcept
    {
        double s  = (z0 * wc3 + z1 * a20 + z2 * c2 + z3 * c3) * b;
        y4 = (g * sample + s) / (1.0 + g * k);          // предпросмотр фидбэка
        double fb = sample - k * y4;
        double y0 = std::clamp (fb, -1.0, 1.0);         // диодная клиппинг-петля
        y1 = b * (y0 * b0  + z0 * a0  + z1 * a1  + z2 * a2  + z3 * a3);
        y2 = b * (y0 * b10 + z0 * a10 + z1 * a11 + z2 * a12 + z3 * a13);
        y3 = b * (y0 * b20 + z0 * a20 + z1 * a21 + z2 * a22 + z3 * a23);
        y4 = g * y0 + s;
        z0 += 4 * wc * (y0 - y1 + y2);
        z1 += 2 * wc * (y1 - 2 * y2 + y3);
        z2 += 2 * wc * (y2 - 2 * y3 + y4);
        z3 += 2 * wc * (y3 - 2 * y4);
        return A * y4;
    }
    void reset (double sample = 0.0) noexcept
    {
        z1 = z2 = z3 = 0;
        y1 = y2 = y3 = y4 = sample;
    }
};

// ---------------------------------------------------------------------------
struct Voice
{
    bool  active = false, keyDown = false;
    int   note = 60, age = 0;
    float velocity = 1.0f;

    // oscillators
    double p1 = 0, p2 = 0, ps = 0;
    float  lastOsc2 = 0;
    double f1 = 440, f2 = 440;           // smoothed current frequencies
    double glideFrom1 = 440, glideFrom2 = 440;
    bool   needGlide = false;
    uint32_t lcg = 0x1234567u;

    Env env1, env2, env3;                 // filter / VCA / mod

    Svf     svf;
    Ladder  ladder;
    ObxdFilt    obx;    // r9: OB-Xd (oberhx) — карта AUX1
    ObVarLadder obv;    // r9: ZDF-лестница для MINI (OberheimVariationMoog)
    Wurtz303    tbf;    // r9: TB303 (Wurtz) — карта AUX1

    // per-voice pan LFO
    double panPhase = 0;
    float  panLfoHz = 0.2f;

    float  panL = 1.0f, panR = 1.0f;

    // CS80 HP stage state (per voice)
    float hpS1 = 0, hpS2 = 0;

    // r5: anti-click state
    float cutSm = 1000.0f, resSm = 0.0f;      // smoothed cutoff / resonance
    float panSmL = 0.707f, panSmR = 0.707f;   // smoothed pan
    float lastG = 0.0f, holdVal = 0.0f;       // last output / value held at change
    int   holdXf = 0;                          // remaining crossfade samples

    void reset() noexcept
    {
        active = false; keyDown = false;
        env1.kill(); env2.kill(); env3.kill();
        svf.reset(); ladder.reset();
        obx.reset(); obv.reset(); tbf.reset();
    }
};

// ---------------------------------------------------------------------------
class Engine
{
public:
    double sr = 44100.0;

    Patch  patch;                         // current (morphed) patch

    // performance inputs
    float  modWheel = 0, bend = 0, pressure = 0, cont1 = 0, cont2 = 0;
    float  masterGain = 1.0f;
    bool   arpOn = false;

    std::atomic<int> auxCard { 0 };   // r9: карта AUX1: 0=OB SVF, 1=OB-X, 2=TB303
    int lastCard = -1;

    void prepare (double sampleRate) noexcept
    {
        sr = sampleRate;
        const float s = (float) sampleRate;
        aGain = 1.0f - std::exp (-1.0f / (0.006f * s));    // 6 ms: master/patch gain
        aCut  = 1.0f - std::exp (-1.0f / (0.004f * s));    // 4 ms: cutoff/resonance
        aPan  = 1.0f - std::exp (-1.0f / (0.001f * s));    // 1 ms: pan
        dcR   = std::exp (-2.0f * (float) M_PI * 12.0f / s); // DC blocker ~12 Hz
        xfN   = std::max (16, (int) (0.004 * sampleRate)); // 4 ms change xfade
        mgSm = masterGain;
        vgSm = -1.0f;                                      // впервые = target в updateGlobals
        dcXLp = dcXRp = dcYL = dcYR = 0.0f;
        for (auto& v : voices) { v.reset(); v.obx.setSampleRate ((float) sampleRate); }
    }

    void setPatch (const Patch& p) noexcept
    {
        // Смена «топологии» (волна/фильтр/XMOD-назначение) — микро-кроссфейд
        // на xfN сэмплов, чтобы переключение и морф не щёлкали.
        static constexpr int sigOffs[] = {
            off::WAVE_BITS, off::WAVE2, off::SUB_WAVE, off::FILT_TYPE, off::XMOD_DEST
        };
        bool changed = false;
        for (int o : sigOffs)
            if (p.raw[(size_t) o] != patch.raw[(size_t) o]) { changed = true; break; }
        patch = p;
        if (changed)
            for (auto& v : voices)
                if (v.active) { v.holdVal = v.lastG; v.holdXf = xfN; }
    }

    void noteOn (int note, float vel) noexcept
    {
        // retrigger same note
        Voice* v = nullptr;
        for (auto& x : voices) if (x.active && x.note == note && ! x.keyDown) { v = &x; break; }
        if (v == nullptr) for (auto& x : voices) if (x.active && x.note == note && x.keyDown) { v = &x; break; }
        if (v == nullptr) for (auto& x : voices) if (! x.active) { v = &x; break; }
        if (v == nullptr)
        {
            // steal: prefer releasing, then oldest
            Voice* best = &voices[0];
            for (auto& x : voices) if (! x.keyDown && x.env2.value() < best->env2.value()) best = &x;
            if (best->keyDown) { best = &voices[0]; for (auto& x : voices) if (x.age < best->age) best = &x; }
            v = best;
        }
        v->note = note;
        v->velocity = std::clamp (vel, 0.0f, 1.0f);
        bool fresh = ! v->active;
        v->keyDown = true;
        v->active = true;
        v->age = ageCounter++;
        v->needGlide = glideOn && ! fresh;
        v->hpS1 = v->hpS2 = 0;
        v->holdXf = 0;
        v->env1.noteOn(); v->env2.noteOn(); v->env3.noteOn();
    }

    void noteOff (int note) noexcept
    {
        for (auto& v : voices)
            if (v.active && v.keyDown && v.note == note)
            { v.keyDown = false; v.env1.noteOff(); v.env2.noteOff(); v.env3.noteOff(); }
    }

    void allNotesOff() noexcept
    {
        for (auto& v : voices)
            if (v.active) { v.keyDown = false; v.env1.noteOff(); v.env2.noteOff(); v.env3.noteOff(); }
    }

    // -------------------------------------------------------------------------
    void process (float* outL, float* outR, int n,
                  const float* extInL = nullptr, const float* extInR = nullptr) noexcept
    {
        const float dt = (float) (1.0 / sr);
        updateGlobals();

        // r9: смена карты AUX1 — сброс состояний карт + микро-кроссфейд (без щелчка)
        const int card = auxCard.load();
        if (card != lastCard)
        {
            lastCard = card;
            for (auto& v : voices)
            {
                v.obx.reset(); v.tbf.reset();
                if (v.active) { v.holdVal = v.lastG; v.holdXf = xfN; }
            }
        }

        for (int i = 0; i < n; ++i)
        {
            // r5: smoothed gains (anti-click on patch/morph/volume changes)
            mgSm += aGain * (masterGain - mgSm);
            vgSm += aGain * (voiceGain - vgSm);

            // LFOs are global
            lfo1.process (1.0 / sr);
            lfo2.process (1.0 / sr);
            const float l1 = lfo1.value();
            const float l2 = lfo2.value();

            float mixL = 0, mixR = 0;
            float ext = 0.0f;
            if (extInL != nullptr)
                ext = (extExtIn / 127.0f) * 0.5f * (extInL[i] + (extInR != nullptr ? extInR[i] : extInL[i]));

            for (auto& v : voices)
            {
                if (! v.active) { continue; }

                // ---- modulation matrix (per sample; 8 voices * few slots ok) ----
                const float env1v = v.env1.value();
                const float env2v = v.env2.value();
                const float env3v = v.env3.value() * (extEnv3Amt / 127.0f);

                float mod[D_COUNT] = { 0 };
                auto add = [&] (int dest, float src, int amt) noexcept
                {
                    if (dest > D_OFF && dest < D_COUNT && amt != 0)
                        mod[dest] += src * ((float) amt / 127.0f);
                };

                // LFO1: 3 slots
                add (patch.i (off::LFO1_DEST1), l1, patch.i (off::LFO1_DEPTH1));
                add (patch.i (off::LFO1_DEST2), l1, patch.i (off::LFO1_DEPTH2));
                add (patch.i (off::LFO1_DEST3), l1, patch.i (off::LFO1_DEPTH3));
                // LFO2: 3 slots
                add (patch.i (off::LFO2_DEST1), l2, patch.i (off::LFO2_DEPTH1));
                add (patch.i (off::LFO2_DEST2), l2, patch.i (off::LFO2_DEPTH2));
                add (patch.i (off::LFO2_DEST3), l2, patch.i (off::LFO2_DEPTH3));
                // ENV3: 3 slots (unipolar)
                add (patch.i (off::ENV3_DEST1), env3v, patch.i (off::ENV3_AMT1));
                add (patch.i (off::ENV3_DEST2), env3v, patch.i (off::ENV3_AMT2));
                add (patch.i (off::ENV3_DEST3), env3v, patch.i (off::ENV3_AMT3));
                // controllers (bipolar where signed)
                add (patch.i (off::MODW_D1), modWheel,     patch.i (off::MODW_A1));
                add (patch.i (off::MODW_D2), modWheel,     patch.i (off::MODW_A2));
                add (patch.i (off::DYN_D1),  v.velocity,   patch.i (off::DYN_A1));
                add (patch.i (off::DYN_D2),  v.velocity,   patch.i (off::DYN_A2));
                add (patch.i (off::BEND_D1), bend,         patch.i (off::BEND_A1));
                add (patch.i (off::BEND_D2), bend,         patch.i (off::BEND_A2));
                add (patch.i (off::PRES_D1), pressure,     patch.i (off::PRES_A1));
                add (patch.i (off::PRES_D2), pressure,     patch.i (off::PRES_A2));
                add (patch.i (off::C1_D1),   cont1,        patch.i (off::C1_A1));
                add (patch.i (off::C1_D2),   cont1,        patch.i (off::C1_A2));
                // CONT2 amplitudes are signed (stored = display + 64)
                auto sgn = [&] (int o) noexcept { return ((int) patch.raw[(size_t) o] - 64) / 64.0f; };
                int c2d1 = patch.i (off::C2_D1), c2d2 = patch.i (off::C2_D2);
                if (c2d1 > 0 && c2d1 < D_COUNT) mod[c2d1] += cont2 * sgn (off::C2_A1);
                if (c2d2 > 0 && c2d2 < D_COUNT) mod[c2d2] += cont2 * sgn (off::C2_A2);
                // fixed routing: ENV1 -> filter
                mod[D_FILT] += env1v * ((float) patch.i (off::ENV1_AMT) / 127.0f);

                // ---- pitch ----
                float semi1 = (float) patch.i (off::OSC1_FREQ) - 32.0f;
                float semi2 = (float) patch.i (off::OSC2_FREQ) - 32.0f + osc2Fine;
                semi1 += 24.0f * (mod[D_FRE1] + mod[D_12F]);
                semi2 += 24.0f * (mod[D_FRE2] + mod[D_12F]);
                semi1 += 12.0f * mod[D_12D];  semi2 += 12.0f * mod[D_12D];
                semi2 += 6.0f * mod[D_12R];
                semi1 += bend * 2.0f;         // bender: ±2 semis base

                double base = 440.0 * std::pow (2.0, (v.note - 69) / 12.0);
                double want1 = base * std::pow (2.0, (semi1 + vOctSemis) / 12.0);
                double want2 = base * std::pow (2.0, (semi2 + vOctSemis) / 12.0);

                // glide
                if (glideOn && v.needGlide)
                {
                    float g = std::clamp (extGlideTime / 127.0f, 0.0f, 1.0f);
                    double k = std::pow (0.001, (double) dt / std::max (0.0005 + 0.6 * g, 0.0005));
                    v.f1 = want1 + (v.f1 - want1) * k;
                    v.f2 = want2 + (v.f2 - want2) * k;
                }
                else { v.f1 = want1; v.f2 = want2; v.needGlide = false; }

                // ---- oscillator generation ----
                double d1 = v.f1 / sr, d2 = v.f2 / sr;
                v.p1 += d1; if (v.p1 >= 1.0) v.p1 -= std::floor (v.p1);
                v.p2 += d2; if (v.p2 >= 1.0) v.p2 -= std::floor (v.p2);

                float pw1 = std::clamp ((float) patch.i (off::PWM1) / 127.0f
                                        + 0.5f * (mod[D_PW1] + mod[D_12P]), 0.02f, 0.98f);
                float pw2 = std::clamp ((float) patch.i (off::PWM2) / 127.0f
                                        + 0.5f * (mod[D_PW2] + mod[D_12P]), 0.02f, 0.98f);

                // XMOD: osc2 signal -> osc1 freq / osc1 pw / filter cutoff (1-sample tap)
                const float xDepth = (float) patch.i (off::XMOD_DPTH) / 127.0f;
                const float xd = xDepth * v.lastOsc2;
                float cutMod = 0.0f;
                switch (patch.i (off::XMOD_DEST))
                {
                    case 0:  v.p1 += xd * 0.03; if (v.p1 >= 1.0 || v.p1 < 0.0) v.p1 -= std::floor (v.p1); break;
                    case 1:  pw1 = std::clamp (pw1 + xd * 0.35f, 0.02f, 0.98f); break;
                    default: cutMod = xd * 0.6f; break;
                }
                (void) 0;

                // OSC1 waves: bitmask tri=1, saw=2, pulse=4 of byte 74 (value 0 = default SAW)
                const uint8_t wb = patch.raw[off::WAVE_BITS];
                int m1 = (wb & 7) != 0 ? (wb & 7) : 2;
                float o1 = 0.0f, o2 = 0.0f;
                if (m1 & 1) o1 += tri  ((float) v.p1);
                if (m1 & 2) o1 += saw  ((float) v.p1);
                if (m1 & 4) o1 += pulse((float) v.p1, pw1);
                o1 *= 0.45f;
                // sub
                int sw = patch.i (off::SUB_WAVE);
                if (sw > 0) o1 += 0.4f * subOsc ((float) v.p1, sw);

                // OSC2 wave: enum byte 75 {0 = pulse, 1 = tri (836 patches), 2 = saw}
                int w2 = patch.i (off::WAVE2);
                switch (w2)
                {
                    case 0:  o2 = pulse ((float) v.p2, pw2); break;
                    case 2:  o2 = saw   ((float) v.p2); break;
                    default: o2 = tri   ((float) v.p2); break;
                }
                o2 *= 0.45f;

                v.lcg = v.lcg * 1103515245u + 12345u;
                float noise = 0.5f * ((float) (v.lcg >> 24) / 128.0f - 1.0f);

                v.lastOsc2 = o2;

                float lvl1 = std::clamp ((float) patch.i (off::OSC1_LEVEL) / 127.0f + mod[D_LEV1], 0.0f, 1.3f);
                float lvl2 = std::clamp ((float) patch.i (off::OSC2_LEVEL) / 127.0f + mod[D_LEV2], 0.0f, 1.3f);
                float lvlN = std::clamp ((float) patch.i (off::NOISE_LEVEL) / 127.0f + mod[D_LEVN], 0.0f, 1.3f);

                float vca = env2v;
                // velocity dynamics on VCA (DYN2) & filter env (DYN1)
                float kVelVca = extDyn2 / 127.0f;
                vca *= (1.0f - kVelVca) + kVelVca * v.velocity;

                float drive = lvl1 * o1 + lvl2 * o2 + lvlN * noise + ext;

                // ---- filter ----
                float cut = (float) patch.i (off::CUTOFF) / 127.0f;
                cut += mod[D_FILT];
                cut += cutMod;                                    // XMOD->filter
                float cutHz = 25.0f * std::pow (700.0f, std::clamp (cut, 0.0f, 1.0f));
                // key tracking
                float trk = (float) patch.i (off::TRACKING) / 127.0f;
                if (trk > 0)
                {
                    double ratio = base / 261.626;
                    cutHz *= (float) std::pow (ratio, trk);
                }
                float res = std::clamp ((float) patch.i (off::RESO) / 127.0f + mod[D_RESO], 0.0f, 1.0f);

                // r5: smooth cutoff/resonance (kills zipper on morph & preset jumps)
                v.cutSm += aCut * (cutHz - v.cutSm);
                cutHz = v.cutSm;
                v.resSm += aCut * (res - v.resSm);
                res = v.resSm;

                int ftype = patch.i (off::FILT_TYPE);
                float fOut = 0;
                switch (ftype)
                {
                    case F_SEM_LP: v.svf.set (cutHz, 0.7f + res * 12.0f, (float) sr); fOut = v.svf.run (drive, 0); break;
                    case F_SEM_BP: v.svf.set (cutHz, 0.7f + res * 10.0f, (float) sr); fOut = v.svf.run (drive, 1); break;
                    case F_SEM_HP: v.svf.set (cutHz, 0.7f + res * 10.0f, (float) sr); fOut = v.svf.run (drive, 2); break;
                    case F_SEM_BR: v.svf.set (cutHz, 0.7f + res * 10.0f, (float) sr); fOut = v.svf.run (drive, 3); break;
                    case F_MINI:   v.obv.set (cutHz, res, (float) sr);               fOut = v.obv.run (drive); break;
                    case F_AUX1:
                        switch (lastCard)
                        {
                            case 1: v.obx.setResonance (res); fOut = v.obx.Apply (drive, cutHz); break;         // OB-Xd
                            case 2: v.tbf.init (sr, cutHz, res); fOut = (float) v.tbf.eval (drive); break;      // TB303
                            default: v.svf.set (cutHz, 0.5f + res * 8.0f, (float) sr);                          // OB SVF (сток)
                                     fOut = v.svf.run (drive, 0); break;
                        }
                        break;
                    case F_AUX2:
                    {
                        // CS80: dedicated HP stage (hpf/hpr) then LP
                        v.svf.set (cutHz, 0.5f + res * 9.0f, (float) sr);
                        float hpFreq = 20.0f * std::pow (600.0f, (float) patch.i (off::HPF) / 127.0f);
                        float hpr = (float) patch.i (off::HPR) / 127.0f;
                        // simple one-pole-cascade HP approximation
                        float w = (float) (2.0 * M_PI * hpFreq / sr);
                        float a = w / (w + 1.0f);
                        (void) hpr;
                        v.hpS2 = 0.985f * v.hpS2 + 0.015f * drive;
                        float hpOut = a * (v.hpS1 + drive - v.hpS2);
                        v.hpS1 = 0.999f * v.hpS1 + 0.001f * drive;
                        fOut = v.svf.run (hpOut, 0);
                        break;
                    }
                    default: fOut = drive; break;
                }

                float out = fOut * vca;
                if (! std::isfinite (out))                 // r5: NaN/Inf recovery
                {
                    v.svf.reset(); v.ladder.reset(); v.hpS1 = v.hpS2 = 0;
                    out = 0.0f;
                }

                // ---- per-voice flying pan ----
                int vi = (int) (&v - voices);
                if (vi >= 0 && vi < kNumVoices)
                {
                    float rate = (float) patch.raw[kArrRate + vi];
                    float pos  = ((float) patch.raw[kArrPos + vi] - 64.0f) / 64.0f;
                    float dep  = (float) patch.raw[kArrDepth + vi] / 127.0f;
                    v.panLfoHz = 0.02f * std::pow (400.0f, rate / 127.0f);
                    v.panPhase += v.panLfoHz / sr;
                    if (v.panPhase >= 1.0) v.panPhase -= 1.0;
                    float p = std::clamp (pos + dep * (float) std::sin (v.panPhase * 2.0 * M_PI)
                                          + 0.5f * mod[D_PAN], -1.0f, 1.0f);
                    float pl = std::cos ((p + 1.0f) * (float) M_PI / 4.0f);
                    float pr = std::sin ((p + 1.0f) * (float) M_PI / 4.0f);
                    v.panL = pl; v.panR = pr;
                    v.panSmL += aPan * (pl - v.panSmL);   // r5: anti-click pan
                    v.panSmR += aPan * (pr - v.panSmR);
                }

                float g = out * vgSm * mgSm;

                // r5: 4 ms crossfade from last output on topology change (no clicks)
                if (v.holdXf > 0)
                {
                    float t = 1.0f - (float) v.holdXf / (float) xfN;
                    g = v.holdVal + (g - v.holdVal) * t;
                    --v.holdXf;
                }
                if (! std::isfinite (g)) g = 0.0f;
                v.lastG = g;

                mixL += g * v.panSmL;
                mixR += g * v.panSmR;

                // env update once per sample
                v.env1.process (dt); v.env2.process (dt); v.env3.process (dt);
                if (! std::isfinite (v.env1.level + v.env2.level + v.env3.level))
                { v.env1.kill(); v.env2.kill(); v.env3.kill(); }   // r5: NaN recovery

                if (! v.keyDown && v.env2.value() < 1.0e-5f && v.env1.stage == Env::Idle)
                    v.active = false;
            }

            // r5: DC blocker (выходная связь как по конденсатору в железе) + защита
            float yl = mixL - dcXLp + dcR * dcYL;
            float yr = mixR - dcXRp + dcR * dcYR;
            dcXLp = mixL;
            dcXRp = mixR;
            if (std::isfinite (yl)) dcYL = yl; else { yl = 0.0f; dcYL = 0.0f; dcXLp = 0.0f; }
            if (std::isfinite (yr)) dcYR = yr; else { yr = 0.0f; dcYR = 0.0f; dcXRp = 0.0f; }
            // r8: soft-clip — резонансные/перегруженные патчи не «взрываются»
            outL[i] = softClip (yl);
            outR[i] = softClip (yr);
        }
    }

    // -------------------------------------------------------------------------
    // simple built-in arpeggiator feed (called from processor's timer/MIDI logic)
    void recomputeGlobals() noexcept { updateGlobals(); }

    std::vector<int> heldNotes;

private:
    Voice voices[kNumVoices];
    Lfo   lfo1, lfo2;
    int   ageCounter = 0;

    // cached globals from patch
    float voiceGain = 1.0f, extEnv3Amt = 0, extGlideTime = 0, extDyn2 = 127,
          extExtIn = 0, osc2Fine = 0, vOctSemis = 0;
    bool  glideOn = false;

    // r5: smoothing / anti-click state
    float aGain = 0.003f, aCut = 0.004f, aPan = 0.01f;
    float dcR = 0.9983f;
    float mgSm = 1.0f, vgSm = 1.0f;
    float dcXLp = 0, dcXRp = 0, dcYL = 0, dcYR = 0;
    int   xfN = 176;

    void updateGlobals() noexcept
    {
        float vol = (float) patch.i (off::VOLUME) / 127.0f;
        voiceGain = vol * vol * 1.4f;
        if (vgSm < 0.0f) vgSm = voiceGain;   // первая инициализация
        extEnv3Amt = (float) patch.i (off::ENV3_AMT);
        extGlideTime = (float) patch.i (off::GLIDE_TIME);
        glideOn = (patch.raw[off::GLIDE_FLAGS] & 0x40) != 0;
        extDyn2 = (float) patch.i (off::DYN2);
        extExtIn = (float) patch.i (off::EXT_IN);

        // octave nibble: hi = octave (4 = MID), lo = fine 0..15
        int ob = patch.i (off::OCTAVE);
        int hi = (ob >> 4) & 0x0F, lo = ob & 0x0F;
        vOctSemis = (hi - 4) * 12 + (lo > 8 ? lo - 16 : lo) * 0.1f;

        // osc2 fine (64 centre) in semitones ±2
        osc2Fine = ((int) patch.raw[off::OSC2_FINE] - 64) / 32.0f;

        // LFO setup
        lfo1.setRate (patch.i (off::LFO1_RATE));
        lfo2.setRate (patch.i (off::LFO2_RATE));
        int w1 = patch.i (off::LFO1_WAVSYNC), w2 = patch.i (off::LFO2_WAVSYNC);
        lfo1.square = (w1 != 0);
        lfo2.square = (w2 != 0);

        for (auto& v : voices)
        {
            v.env1.setParams (patch.i (off::ENV1_ATK), patch.i (off::ENV1_DEC),
                              patch.i (off::ENV1_DK2), patch.i (off::ENV1_SUS), patch.i (off::ENV1_REL),
                              patch.i (off::DLY1));
            v.env2.setParams (patch.i (off::ENV2_ATK), patch.i (off::ENV2_DEC),
                              patch.i (off::ENV2_DK2), patch.i (off::ENV2_SUS), patch.i (off::ENV2_REL),
                              patch.i (off::DLY2));
            v.env3.setParams (patch.i (off::ENV3_ATK), patch.i (off::ENV3_DEC),
                              patch.i (off::ENV3_DK2), patch.i (off::ENV3_SUS), patch.i (off::ENV3_REL),
                              patch.i (off::DLY3));
        }
    }

    static float softClip (float x) noexcept
    {
        if (x >  0.85f) return  0.85f + 0.15f * std::tanh ((x  - 0.85f) * 4.0f);
        if (x < -0.85f) return -0.85f - 0.15f * std::tanh ((-x - 0.85f) * 4.0f);
        return x;
    }

    static float tri (float p) noexcept  { return 4.0f * std::abs (p - 0.5f) - 1.0f; }
    static float saw (float p) noexcept  { return 2.0f * p - 1.0f; }
    static float pulse (float p, float w) noexcept { return p < w ? 1.0f : -1.0f; }
    static float subOsc (float p, int wave) noexcept
    {
        switch (wave)
        {
            case 1: return pulse (p, 0.5f);
            case 2: return std::sin (p * 2.0f * (float) M_PI);
            case 3: return tri (p);
            case 4: return pulse (p, 0.5f);
            case 5: return saw (p);
            default: return saw (p);
        }
    }
};

} // namespace omega8
