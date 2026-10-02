// OmegaEngine.h — 8-voice stereo synthesis engine for Omega 8 / Omega CODE
// r16: Osc: tri/saw/pulse + sub + noise, XMOD; filters: SEM (LP/BP/HP/BR, TPT
// SVF), MINI (MoogPopDSP 24dB ladder), AUX1 cards (OB-Xa 24 / OB-X 12 / TB-303,
// референс AudioFilterOBXa + Wurtz), AUX2 (CS-80 dual VCF: SVF-HP -> SVF-LP);
// 2-й порядок HP 10Hz на выходе (DC-clean); BLEND живой (enum-байты снап
// + 30 мс кроссфейд); FREQ = raw 0..63 semitones from note.
// 3 envs (A/D/Dk2/S/R), 2 LFOs + per-voice pan LFO; 24-slot mod matrix
// (LFO1 x3, LFO2 x3, ENV3 x3, modwheel/dynamics/bender/pressure/cont1/cont2
// x2 each).
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
    void reset() noexcept { phase = 0.0; }   // r18.7: KEY-sync
    float valueAt (double phaseOffset) const noexcept   // r18.7: POLY mode
    {
        double p = phase + phaseOffset;
        p -= std::floor (p);
        return valueAtRaw (p);
    }
    float value() const noexcept    // bipolar -1..1
    {
        return valueAtRaw (phase);
    }
private:
    float valueAtRaw (double p) const noexcept
    {
        const float ph = (float) p;
        if (square) return ph < 0.5f ? 1.0f : -1.0f;
        return (float) std::sin (ph * 2.0 * M_PI);
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
        // r18.1: tan-форма расходится при f/sr >= 0.30 (проверено сеткой,
        // dspcheck/dbg8: 0.25 стабильно при любом Q, 0.30+ = inf). Кламп 0.24.
        freq = std::clamp (freq, 10.0f, sr * 0.24f);
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

// r17: SEM = «Oberheim 12 dB» (мануал p.21: TYPE = Oberheim 12 db LP/BP/HP/BR,
// Moog 24db minimoog, AUX1, AUX2). 4-полюсная лестница BZT one-pole,
// референс OberhVar.h (omega8_decode_for_agents/external_filters): K-фидбэк
// через суммарный sigma, входной буст (1+K) с нормализацией alpha0 и tanh
// (saturation = 1.0, как в референсе).
// Мануал: «Except for the SEM filter, self-oscillation happily chirps...» —
// SEM не должен уходить в свободный самовозбуд; входной tanh + K<=4
// (референсное отображение r 1..10 -> K 0..4) держат амплитуду.
// ВАЖНО: v9/v10 ставили ЭТУ ЖЕ лестницу в слот MINI (не в SEM!) с K до 6.7 —
// отсюда «громкий шум» при высоком reso. Теперь: MINI = Moog24, SEM = Oberheim12.
// Режимы (тапы, подобраны замером АЧХ на этой лестнице, см. dspcheck/t17f/h):
// LP = s4, BP = s2 - s3 (средний каскад, DC = 0), HP = u - s4 (DC = 0),
// BR = x - s1 + s2 - s3 + s4 = x - (s1-s2) - (s3-s4): вычитание 12dB и 24dB
// band-секций — настоящий 4-pole band-reject, dip на cutoff (DC = x, gain 1).
// Комбинации u+s4-2sN дают пик на cutoff, а не нотч — не использовать.
struct Oberheim12
{
    struct OnePole
    {
        double alpha = 1, beta = 0, z1 = 0;
        double Tick (double s) noexcept
        {
            double vn = (s - z1) * alpha;   // a0 = 1, gamma = 1, fb = 0 (референс)
            double out = vn + z1;
            z1 = vn + out;                  // BZT-хранение (TPT)
            return out;
        }
        double Fb() const noexcept { return beta * z1; }
    };

    OnePole lp[4];
    double K = 0, gamma4 = 1, alpha0 = 1;

    void set (float hz, float res01, float sr) noexcept
    {
        hz = std::clamp (hz, 10.0f, sr * 0.45f);
        K = 4.0 * std::clamp (res01, 0.0f, 1.0f);
        const double T = 1.0 / (double) sr;
        const double g = std::tan (std::clamp ((double) hz, 10.0, (double) sr * 0.45) * T * M_PI);
        const double G = g / (1.0 + g);
        for (auto& o : lp) o.alpha = G;
        lp[0].beta = G * G * G / (1.0 + g);
        lp[1].beta = G * G / (1.0 + g);
        lp[2].beta = G / (1.0 + g);
        lp[3].beta = 1.0 / (1.0 + g);
        gamma4 = G * G * G * G;
        alpha0 = 1.0 / (1.0 + K * gamma4);
    }

    float run (float x, int mode) noexcept
    {
        const double sigma = lp[0].Fb() + lp[1].Fb() + lp[2].Fb() + lp[3].Fb();
        double u = ((double) x * (1.0 + K) - K * sigma) * alpha0;
        u = std::tanh (u);
        const double s1 = lp[0].Tick (u);
        const double s2 = lp[1].Tick (s1);
        const double s3 = lp[2].Tick (s2);
        const double s4 = lp[3].Tick (s3);
        switch (mode)
        {
            case 1:  return (float) (s2 - s3);             // BP — средний каскад
            case 2:  return (float) (u - s4);              // HP — DC точно 0
            case 3:  return (float) ((double) x - s1 + s2 - s3 + s4); // BR — dip на cutoff (DC = x)
            default: return (float) s4;                    // LP (референсный выход)
        }
    }
    void reset() noexcept { for (auto& o : lp) o.z1 = 0; }
};

// r16: MINI (Moog 24 dB minimoog) — MoogPopDSP.h из репо
// («juce random filters for import/!for omega8»): 4x one-pole (bilinear),
// фидбэк x - res*stage3 (коэффициент зависит от cutoff), кубический soft-clip
// на последнем каскаде. Заменяет r9 ObVarLadder (ZDF OberhVar с бустом входа
// 1+K): у него DC/низкочастотный gain до 5x при низком cutoff — основной
// источник «DC offset» и «взрывов» на басовых патчах. Здесь DC gain =
// 1/(1+resGain) <= 1, self-osc ограничен кубическим клипом.
struct Moog24
{
    float s0 = 0, s1 = 0, s2 = 0, s3 = 0;
    float d0 = 0, d1 = 0, d2 = 0, d3 = 0;
    float p = 0, k = 0, resGain = 0;

    void set (float hz, float res01, float sr) noexcept
    {
        hz = std::clamp (hz, 5.0f, sr * 0.48f);
        const float cutoff = 2.0f * hz / sr;
        p  = cutoff * (1.8f - 0.8f * cutoff);
        k  = 2.0f * std::sin (cutoff * (float) M_PI * 0.5f) - 1.0f;
        const float t1 = (1.0f - p) * 1.386249f;
        const float t2 = 12.0f + t1 * t1;
        resGain = std::clamp (res01, 0.0f, 1.0f) * (t2 + 6.0f * t1)
                  / std::max (t2 - 6.0f * t1, 1.0e-3f);
    }
    float run (float x) noexcept
    {
        const float in = x - resGain * s3;
        const float y0 = in * p + d0 * p - k * s0;
        const float y1 = y0 * p + d1 * p - k * s1;
        const float y2 = y1 * p + d2 * p - k * s2;
        float y3 = y2 * p + d3 * p - k * s3;
        y3 -= (y3 * y3 * y3) / 6.0f;                 // band-limited sigmoid
        d0 = in; d1 = y0; d2 = y1; d3 = y2;
        s0 = y0; s1 = y1; s2 = y2; s3 = y3;
        return s3;
    }
    void reset() noexcept { s0 = s1 = s2 = s3 = 0; d0 = d1 = d2 = d3 = 0; }
};

// ===========================================================================
// r16: карты AUX1 — OB-Xa/OB-X (AudioFilterOBXa.cpp из репо, MIT, Oberheim
//     OB-Xf-топология) вместо r9 ObxdFilt (обрезанный 2-полюс из oberhx.h).
//     TPT ZDF, 2-полюс (диодная пара) и 4-полюс 24dB (atan-сатурация первого
//     полюса + компенсация громкости при резонансе). Таблица pole-mix
//     Xpander — дословно из референса (service manual Oberheim Xpander).
//     DC gain = 1 (проверено: в пределе g->0 все режимы проходят DC с gain 1,
//     диодный член в т.рабочем равен 0), s-состояния при смене карты сбрас.
// ---------------------------------------------------------------------------

struct Obxa
{
    float p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    float res2 = 1.0f, res4 = 0.0f;               // 1-r01 / 3.5*r01
    float rc = 970.0f / 44000.0f, rcInv = 44000.0f / 970.0f;
    float fsInv = 1.0f / 44100.0f;

    // Xpander pole-mix {y0(in), y1..y4} — из референса
    static constexpr int kXpanderCount = 15;
    static constexpr float kPoleMix[kXpanderCount][5] = {
        { 0,  0,  0,  0,  1},   //  0: LP4  (24 dB LP)
        { 0,  0,  0,  1,  0},   //  1: LP3
        { 0,  0,  1,  0,  0},   //  2: LP2
        { 0,  1,  0,  0,  0},   //  3: LP1
        { 1, -3,  3, -1,  0},   //  4: HP3
        { 1, -2,  1,  0,  0},   //  5: HP2
        { 1, -1,  0,  0,  0},   //  6: HP1
        { 0,  0,  2, -4,  2},   //  7: BP4
        { 0, -2,  2,  0,  0},   //  8: BP2
        { 1, -2,  2,  0,  0},   //  9: N2   (notch)
        { 1, -3,  6, -4,  0},   // 10: PH3  (phaser)
        { 0, -1,  2, -1,  0},   // 11: HP2+LP1
        { 0, -1,  3, -3,  1},   // 12: HP3+LP1
        { 0, -1,  2, -2,  0},   // 13: N2+LP1
        { 0, -1,  3, -6,  4},   // 14: PH3+LP1
    };

    void setSampleRate (float sr) noexcept
    {
        fsInv = 1.0f / sr;
        rc = (970.0f / 44000.0f) * std::sqrt (44000.0f / sr);
        rcInv = 1.0f / rc;
    }
    void setResonance (float r01) noexcept { res2 = 1.0f - r01; res4 = 3.5f * r01; }

    static float diodePair (float x) noexcept
    {
        // Taylor approx of slightly mismatched diode pair (из референса)
        return (((((0.0103592f) * x + 0.00920833f) * x + 0.185f) * x + 0.05f) * x + 1.0f);
    }

    // 12 dB/oct — OB-X 2-pole с диодной парой (карта «OB-X 12»), LP.
    float run2 (float x, float g) noexcept
    {
        const float tCfb = diodePair (p1 * 0.0876f) - 1.0f;
        const float v = (x - 2.0f * (p1 * (res2 + tCfb)) - g * p1 - p2)
                        / (1.0f + g * (2.0f * (res2 + tCfb) + g));
        const float y1 = v * g + p1;
        p1 = v * g + y1;
        const float y2 = y1 * g + p2;
        p2 = y1 * g + y2;
        return y2;
    }

    // 24 dB/oct — OB-Xa 4-pole (карта «OB-Xa 24»), mode = Xpander 0..14.
    float run4 (float x, float g, int mode) noexcept
    {
        const float lpc = g / (1.0f + g);
        const float ml  = 1.0f / (1.0f + g);
        const float S = (lpc * (lpc * (lpc * p1 + p2) + p3) + p4) * ml;
        const float G = lpc * lpc * lpc * lpc;
        const float y0 = (x - res4 * S) / (1.0f + res4 * G);

        float v = (y0 - p1) * lpc;
        float r = v + p1;
        p1 = r + v;
        p1 = std::atan (p1 * rc) * rcInv;            // сатурация первого полюса
        const float y1 = r;

        v = (y1 - p2) * lpc; r = v + p2; p2 = r + v;
        const float y2 = r;
        v = (y2 - p3) * lpc; r = v + p3; p3 = r + v;
        const float y3 = r;
        v = (y3 - p4) * lpc; r = v + p4; p4 = r + v;
        const float y4 = r;

        const float* m = kPoleMix[std::clamp (mode, 0, kXpanderCount - 1)];
        // half volume comp (из референса): уровень не падает при резонансе
        return (y0 * m[0] + y1 * m[1] + y2 * m[2] + y3 * m[3] + y4 * m[4])
               * (1.0f + res4 * 0.45f);
    }
    void reset() noexcept { p1 = p2 = p3 = p4 = 0; }
};

// TB303.cpp (Dominique Wurtz) — диагональная лестница 303; Lerp/LookupTable из
// «just filt.h» заменены прямым вычислением коэффициентов. Только LP (карта).
// r18.5: ARP2600 — 4-полюсный lowpass-«лестница» (BZT one-pole, без диодного
// драйва оригинала), резонанс = глубина обратной связи. Как в мануале у OMEGACODE:
// карта ARP2600 в слоте AUX1.
struct ArpLadder
{
    float s1 = 0, s2 = 0, s3 = 0, s4 = 0, fc = 0, fb = 0;
    void set (float cutHz, float res, float sr)
    {
        fc = std::tan (std::min (cutHz, sr * 0.24f) * (float) M_PI / sr);
        fb = std::clamp (res, 0.0f, 1.0f) * 0.92f;
    }
    float run (float x)
    {
        s1 += fc * (x - s1 - fb * s4);
        s2 += fc * (s1 - s2 - fb * s4);
        s3 += fc * (s2 - s3 - fb * s4);
        s4 += fc * (s3 - s4 - fb * s4);
        return s4;
    }
    void reset() { s1 = s2 = s3 = s4 = 0; }
};

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
    Patch pg;                             // r11: патч, захваченный на момент ноты
    float vgP = 0.35f;                    // r15: overwritten from this voice's VOLUME on noteOn

    Svf     svf;       // CS80 (AUX2): HP + LP-каскады
    Svf     svfHp;     // r16: HP-каскад CS80 (AUX2)
    Oberheim12 ober;   // r17: SEM LP/BP/HP/BR (Oberheim 12 dB, референс OberhVar.h)
    Moog24  moog;      // r16: MINI (MoogPopDSP из репо)
    Obxa    obxa;      // r16: карты AUX1 «OB-Xa 24» / «OB-X 12»
    Wurtz303 tbf;      // TB303 (Wurtz) — карта AUX1
    ArpLadder arp;     // r18.5: карта AUX1 «ARP2600»
    float ts808 = 0.0f; // r18.5: карта AUX1 «TS-808» — состояние LP

    // per-voice pan LFO
    double panPhase = 0;
    float  panLfoHz = 0.2f;

    float  panL = 1.0f, panR = 1.0f;

    // r18.7: MULTI — голос части (part)
    int   mPart = -1;      // -1 = обычный голос
    float mPan  = 0.0f;    // позиция части (-1..1)
    bool  mPanOn = false;

    // r5: anti-click state
    float cutSm = 1000.0f, resSm = 0.0f;      // smoothed cutoff / resonance
    float panSmL = 0.707f, panSmR = 0.707f;   // smoothed pan
    float lastG = 0.0f, holdVal = 0.0f;       // last output / value held at change
    int   holdXf = 0;                          // remaining crossfade samples
    int   holdXfTotal = 0;                     // r18: длина этого фейда (для t-кривой)

    void reset() noexcept
    {
        active = false; keyDown = false;
        env1.kill(); env2.kill(); env3.kill();
        svf.reset(); svfHp.reset();
        moog.reset(); obxa.reset(); tbf.reset();
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

    std::atomic<int> auxCard { 0 };   // r9: карта AUX1; r18.5: 0=OB-Xa 24, 1=OB-X 12, 2=TB303, 3=ARP2600, 4=CS-80, 5=WAH, 6=TS-808
    // r18: было -1 → на ПЕРВОМ блоке (note-on) срабатывал «сдвиг карты» с
    // кроссфейдом от holdVal=0 — в r16/r17 это был инвертированный бам на каждой
    // атаке (e2e peak 0.826 на тихом патче). Теперь 0 = начальное значение auxCard.
    int lastCard = 0;

    void prepare (double sampleRate) noexcept
    {
        sr = sampleRate;
        const float s = (float) sampleRate;
        aGain = 1.0f - std::exp (-1.0f / (0.006f * s));    // 6 ms: master/patch gain
        aCut  = 1.0f - std::exp (-1.0f / (0.004f * s));    // 4 ms: cutoff/resonance
        aPan  = 1.0f - std::exp (-1.0f / (0.001f * s));    // 1 ms: pan
        dcA   = std::exp (-2.0f * (float) M_PI * 10.0f / s); // r16: HP 10 Hz (2x one-pole)
        xfN   = std::max (16, (int) (0.004 * sampleRate)); // 4 ms change xfade
        xfLongN = std::max (64, (int) (0.150 * sampleRate)); // r18.5: 150 ms — мануал (кроссфейд пресетов в банке и морф 50%)
        mgSm = masterGain;
        dcX1L = dcY1L = dcX2L = dcY2L = 0.0f;
        dcX1R = dcY1R = dcX2R = dcY2R = 0.0f;
        for (auto& v : voices) { v.reset(); v.obxa.setSampleRate ((float) sampleRate); }
    }

    void setPatch (const Patch& p) noexcept
    {
        // r16: BLEND живой (как аппаратный BLEND Dxxx+Dyyy): непрерывные байты
        // сразу идут на активные голоса; enum/флаговые байты в Patch::lerp
        // перелетают скачком на 50% — здесь под скачок сбрасываем состояния
        // фильтров/волн и скрываем кроссфейдом 30 мс (xfLongN). Больше никаких
        // «затычных» латчей: голос играет ТЕКУЩЕЕ значение морфа.
        patch = p;
        for (auto& v : voices)
        {
            if (! v.active) continue;
            bool top = false;
            for (size_t i = 0; i < kPatchSize; ++i)
                if (p.raw[i] != v.pg.raw[i])
                {
                    v.pg.raw[i] = p.raw[i];
                    if (i < kParamBytes && Patch::isEnumByte ((int) i)) top = true;
                }
            if (top)
            {
                v.svf.reset(); v.svfHp.reset(); v.ober.reset();
                v.moog.reset(); v.obxa.reset(); v.tbf.reset();
                v.holdVal = v.lastG; v.holdXf = xfLongN; v.holdXfTotal = xfLongN;
            }
        }
    }

    // r18.2: RESET ALL — убить все голоса и состояния (звук «исчезает» при тестах:
    // застрявшие голоса/арп/лэтчи). Вызывается из GUI под bankLock (как setPatch).
    void clearVoices() noexcept
    {
        for (auto& v : voices)
        {
            v.reset();
            v.ober.reset();
            v.p1 = v.p2 = v.ps = v.panPhase = 0.0;
            v.lastOsc2 = 0.0f;
            v.cutSm = 1000.0f; v.resSm = 0.0f;
            v.panSmL = v.panSmR = 0.707f;
            v.lastG = v.holdVal = 0.0f;
            v.holdXf = v.holdXfTotal = 0;
        }
    }

    // r15: вернуть уровневую кривую r10 — она ближе к оригиналу по тесту пользователя.
    // Громкость всё ещё хранится per-voice, поэтому браузинг пресетов не меняет
    // уровень уже звучащих нот; меняется только прежняя, проверенная кривая.
    static float volGain (int v) noexcept
    {
        const float x = (float) std::clamp (v, 0, 127) / 127.0f;
        return 1.4f * x * x;
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
        v->pg = patch;                     // r11: голос живёт со СВОИМ патчом (нет взрывов при BLEND/смене)
        v->vgP = volGain (v->pg.i (off::VOLUME));
        // r18.7: LFO KEY-trigger (мануал page3): сброс фазы по клавишам
        {
            const int k1 = patch.i (off::LFO1_KEY);
            if (k1 == 1 || k1 == 3) lfo1.reset();
            const int k2 = patch.i (off::LFO2_KEY);
            if (k2 == 1 || k2 == 3) lfo2.reset();
        }
        // r15: вернуться к поведению r10: свежая нота сразу строит заданную высоту,
        // glide не стартует с дефолтных 440 Hz. Bit2 пока не трактуем без надёжного декода.
        v->needGlide = ((v->pg.raw[off::GLIDE_FLAGS] & 0x40) != 0) && ! fresh;
        v->svf.reset(); v->svfHp.reset(); v->moog.reset();
        v->obxa.reset(); v->tbf.reset();
        v->holdXf = 0;
        v->holdXfTotal = 0;
        // r11: ADSR из захваченного патча — активные ноты не перестраиваются
        v->env1.setParams (v->pg.i (off::ENV1_ATK), v->pg.i (off::ENV1_DEC),
                           v->pg.i (off::ENV1_DK2), v->pg.i (off::ENV1_SUS), v->pg.i (off::ENV1_REL),
                           v->pg.i (off::DLY1));
        v->env2.setParams (v->pg.i (off::ENV2_ATK), v->pg.i (off::ENV2_DEC),
                           v->pg.i (off::ENV2_DK2), v->pg.i (off::ENV2_SUS), v->pg.i (off::ENV2_REL),
                           v->pg.i (off::DLY2));
        v->env3.setParams (v->pg.i (off::ENV3_ATK), v->pg.i (off::ENV3_DEC),
                           v->pg.i (off::ENV3_DK2), v->pg.i (off::ENV3_SUS), v->pg.i (off::ENV3_REL),
                           v->pg.i (off::DLY3));
        v->env1.noteOn(); v->env2.noteOn(); v->env3.noteOn();
    }

    void noteOff (int note) noexcept
    {
        for (auto& v : voices)
            if (v.active && v.keyDown && v.note == note)
            {
                const int k1 = v.pg.i (off::LFO1_KEY);
                if (k1 == 2) lfo1.reset();
                const int k2 = v.pg.i (off::LFO2_KEY);
                if (k2 == 2) lfo2.reset();
                v.keyDown = false; v.env1.noteOff(); v.env2.noteOff(); v.env3.noteOff();
            }
    }

    void allNotesOff() noexcept
    {
        for (auto& v : voices)
            if (v.active) { v.keyDown = false; v.env1.noteOff(); v.env2.noteOff(); v.env3.noteOff(); }
    }

    // r18.9: SEQ — release ВСЕХ голосов части (гейт на офф-степах)
    void noteOffPart (int part) noexcept
    {
        for (auto& v : voices)
            if (v.active && v.mPart == part && v.keyDown)
            {
                v.keyDown = false; v.env1.noteOff(); v.env2.noteOff(); v.env3.noteOff();
            }
    }

    // r18.7: MULTI — голос с чужим патчем/уровнем/панорамой (part из 8)
    void noteOnPart (int note, float vel, const Patch& pp, int vol, int pan, int part) noexcept
    {
        Voice* v = nullptr;
        for (auto& x : voices) if (x.active && x.note == note && x.mPart == part && x.keyDown) { v = &x; break; }
        if (v == nullptr) for (auto& x : voices) if (x.active && x.note == note && x.mPart == part && ! x.keyDown) { v = &x; break; }
        if (v == nullptr) for (auto& x : voices) if (! x.active) { v = &x; break; }
        if (v == nullptr)
        {
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
        v->pg = pp;
        v->mPart = part;
        v->mPanOn = true;
        v->mPan = ((float) pan) / 64.0f;
        v->vgP = volGain (vol);
        v->needGlide = ((pp.raw[off::GLIDE_FLAGS] & 0x40) != 0) && ! fresh;
        v->svf.reset(); v->svfHp.reset(); v->moog.reset();
        v->obxa.reset(); v->tbf.reset(); v->arp.reset(); v->ts808 = 0.0f;
        v->holdXf = 0;
        v->holdXfTotal = 0;
        v->env1.setParams (pp.i (off::ENV1_ATK), pp.i (off::ENV1_DEC),
                           pp.i (off::ENV1_DK2), pp.i (off::ENV1_SUS), pp.i (off::ENV1_REL),
                           pp.i (off::DLY1));
        v->env2.setParams (pp.i (off::ENV2_ATK), pp.i (off::ENV2_DEC),
                           pp.i (off::ENV2_DK2), pp.i (off::ENV2_SUS), pp.i (off::ENV2_REL),
                           pp.i (off::DLY2));
        v->env3.setParams (pp.i (off::ENV3_ATK), pp.i (off::ENV3_DEC),
                           pp.i (off::ENV3_DK2), pp.i (off::ENV3_SUS), pp.i (off::ENV3_REL),
                           pp.i (off::DLY3));
        v->env1.noteOn(); v->env2.noteOn(); v->env3.noteOn();
        const int k1 = pp.i (off::LFO1_KEY);
        if (k1 == 1 || k1 == 3) lfo1.reset();
        const int k2 = pp.i (off::LFO2_KEY);
        if (k2 == 1 || k2 == 3) lfo2.reset();
    }

    // -------------------------------------------------------------------------
    void process (float* outL, float* outR, int n,
                  const float* extInL = nullptr, const float* extInR = nullptr) noexcept
    {
        const float dt = (float) (1.0 / sr);
        updateGlobals();

        // r16: смена карты AUX1 — сброс состояний карт + кроссфейд 30 мс (без щелчка)
        const int card = auxCard.load();
        if (card != lastCard)
        {
            lastCard = card;
            for (auto& v : voices)
            {
                v.obxa.reset(); v.tbf.reset(); v.arp.reset(); v.ts808 = 0.0f;   // r18.5
                if (v.active) { v.holdVal = v.lastG; v.holdXf = xfLongN; v.holdXfTotal = xfLongN; }
            }
        }

        for (int i = 0; i < n; ++i)
        {
            // r5: smoothed gains (anti-click on patch/morph/volume changes)
            mgSm += aGain * (masterGain - mgSm);

            // LFOs are global
            // r18.4: LF1R/LF2R — скорость LFO = базовый байт + mod (±64 байта).
            lfo1.rateHz = 0.02f * std::pow (500.0f, std::clamp (rate1Byte + 64.0f * lf1Rmod, 0.0f, 127.0f) / 127.0f);
            lfo2.rateHz = 0.02f * std::pow (500.0f, std::clamp (rate2Byte + 64.0f * lf2Rmod, 0.0f, 127.0f) / 127.0f);
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
                const float env3v = v.env3.value() * ((float) v.pg.i (off::ENV3_AMT) / 127.0f);

                float mod[D_COUNT] = { 0 };
                auto add = [&] (int dest, float src, int amt) noexcept
                {
                    if (dest > D_OFF && dest < D_COUNT && amt != 0)
                        mod[dest] += src * ((float) amt / 127.0f);
                };

                // r18.4: controllers идут ПЕРВЫМИ — чтобы их mod на EA1/EA3/XMOD/
                // LF1R/LF2R/LF1D/LF2D/PAND/PANR/PAN/EXT применялся к LFO/ENV3/pan
                // в ЭТОМ же блоке. В r18.3 15 из 24 dest (RESO кроме) применялись,
                // а XMOD/EA1/EA3/EXT/LF1R/LF2R/LF1D/LF2D/PAND/PANR молча отбрасывались
                // — отсюда «modwheel не реагирует» на большинстве фабричных пресетов.
                add (v.pg.i (off::MODW_D1), modWheel,     v.pg.i (off::MODW_A1));
                add (v.pg.i (off::MODW_D2), modWheel,     v.pg.i (off::MODW_A2));
                add (v.pg.i (off::DYN_D1),  v.velocity,   v.pg.i (off::DYN_A1));
                add (v.pg.i (off::DYN_D2),  v.velocity,   v.pg.i (off::DYN_A2));
                add (v.pg.i (off::BEND_D1), bend,         v.pg.i (off::BEND_A1));
                add (v.pg.i (off::BEND_D2), bend,         v.pg.i (off::BEND_A2));
                add (v.pg.i (off::PRES_D1), pressure,     v.pg.i (off::PRES_A1));
                add (v.pg.i (off::PRES_D2), pressure,     v.pg.i (off::PRES_A2));
                add (v.pg.i (off::C1_D1),   cont1,        v.pg.i (off::C1_A1));
                add (v.pg.i (off::C1_D2),   cont1,        v.pg.i (off::C1_A2));
                // CONT2 amplitudes are signed (stored = display + 64)
                auto sgn = [&] (int o) noexcept { return ((int) v.pg.raw[(size_t) o] - 64) / 64.0f; };
                int c2d1 = v.pg.i (off::C2_D1), c2d2 = v.pg.i (off::C2_D2);
                if (c2d1 > 0 && c2d1 < D_COUNT) mod[c2d1] += cont2 * sgn (off::C2_A1);
                if (c2d2 > 0 && c2d2 < D_COUNT) mod[c2d2] += cont2 * sgn (off::C2_A2);

                // r18.4: LF1R/LF2R = mod скорости LFO (LFO глобальный — берём от голоса,
                // все голоса несут один патч; применяется к LFO в следующем такте).
                lf1Rmod = mod[D_LF1R];
                lf2Rmod = mod[D_LF2R];

                // r18.4: LF1D/LF2D = mod глубины LFO (сигнал масштабируется на голосе).
                // r18.7: LFO page2/3: MODE=POLY -> per-voice фаза; QUAN -> глубина шагами 1/12
                const int viP = (int) (&v - voices);
                float l1v = l1, l2v = l2;
                if (v.pg.i (off::LFO1_MODE) != 0) l1v = lfo1.valueAt (((float) viP + 0.25f) / (float) kNumVoices);
                if (v.pg.i (off::LFO2_MODE) != 0) l2v = lfo2.valueAt (((float) viP + 0.25f) / (float) kNumVoices);
                if (v.pg.i (off::LFO1_QUAN) != 0) l1v = std::round (l1v * 12.0f) / 12.0f;
                if (v.pg.i (off::LFO2_QUAN) != 0) l2v = std::round (l2v * 12.0f) / 12.0f;
                const float l1d = l1v * (1.0f + mod[D_LF1D]);
                const float l2d = l2v * (1.0f + mod[D_LF2D]);
                // LFO1: 3 slots
                add (v.pg.i (off::LFO1_DEST1), l1d, v.pg.i (off::LFO1_DEPTH1));
                add (v.pg.i (off::LFO1_DEST2), l1d, v.pg.i (off::LFO1_DEPTH2));
                add (v.pg.i (off::LFO1_DEST3), l1d, v.pg.i (off::LFO1_DEPTH3));
                // LFO2: 3 slots
                add (v.pg.i (off::LFO2_DEST1), l2d, v.pg.i (off::LFO2_DEPTH1));
                add (v.pg.i (off::LFO2_DEST2), l2d, v.pg.i (off::LFO2_DEPTH2));
                add (v.pg.i (off::LFO2_DEST3), l2d, v.pg.i (off::LFO2_DEPTH3));
                // ENV3: 3 slots (unipolar) + r18.4: EA3 = mod глубины ENV3
                {
                    auto addf = [&] (int dest, float src, float amt) noexcept
                    {
                        if (dest > D_OFF && dest < D_COUNT && amt != 0.0f)
                            mod[dest] += src * amt;
                    };
                    addf (v.pg.i (off::ENV3_DEST1), env3v, (float) v.pg.i (off::ENV3_AMT1) / 127.0f + mod[D_EA3]);
                    addf (v.pg.i (off::ENV3_DEST2), env3v, (float) v.pg.i (off::ENV3_AMT2) / 127.0f + mod[D_EA3]);
                    addf (v.pg.i (off::ENV3_DEST3), env3v, (float) v.pg.i (off::ENV3_AMT3) / 127.0f + mod[D_EA3]);
                }
                // fixed routing: ENV1 -> filter + r18.4: EA1 = mod глубины ENV1
                mod[D_FILT] += env1v * ((float) v.pg.i (off::ENV1_AMT) / 127.0f + mod[D_EA1]);

                // ---- pitch ----
                // r18.1: ВОЗВРАТ к r10 (пользовательское решение): OSC FREQ 0..63,
                // центр 32 = нота клавиатуры (semi = FREQ - 32). В r15/r16 сдвиг
                // сняли по фабричной статистике, но живой тест: без -32 звук на
                // 32 полутона выше (тонкий, «октавы сбиты», «слишком фильтрован»),
                // с -32 — как в r10, по которому пользователь отбрал патчи.
                // Остаточный «detune 2-3 ноты» r10 — отдельная мелкая правка r19.
                const float fine2 = ((int) v.pg.raw[off::OSC2_FINE] - 64) / 32.0f;
                float semi1 = (float) v.pg.i (off::OSC1_FREQ) - 32.0f;
                float semi2 = (float) v.pg.i (off::OSC2_FREQ) - 32.0f + fine2;
                const int ob = v.pg.i (off::OCTAVE);
                const int hi = (ob >> 4) & 0x0F;
                // r18.4: нижний ниббл OCTAVE больше НЕ детюнит шагами 10 центов —
                // в фабричных банках он различается (0x3D/0x40/0x41/0x42/0x43...) и
                // давал «плавание» ±20-30 центов от пресета к пресету (жалоба).
                const float octSemis = (float) ((hi - 4) * 12);
                semi1 += 24.0f * (mod[D_FRE1] + mod[D_12F]);
                semi2 += 24.0f * (mod[D_FRE2] + mod[D_12F]);
                semi1 += 12.0f * mod[D_12D];  semi2 += 12.0f * mod[D_12D];
                semi2 += 6.0f * mod[D_12R];
                semi1 += bend * 2.0f;         // bender: ±2 semis base

                double base = 440.0 * std::pow (2.0, (v.note - 69) / 12.0);
                // r18.4: MASTER_TUNE (byte 77, raw-64 центов) — в r10..r18.3 игнорировался
                // (в оригинале это панельный параметр «TUNE: 64%»).
                const double tuneMul = std::pow (2.0, ((double) v.pg.raw[off::MASTER_TUNE] - 64.0) / 1200.0);
                double want1 = base * std::pow (2.0, (semi1 + octSemis) / 12.0) * tuneMul;
                double want2 = base * std::pow (2.0, (semi2 + octSemis) / 12.0) * tuneMul;

                // glide
                if (v.needGlide && (v.pg.raw[off::GLIDE_FLAGS] & 0x40) != 0)
                {
                    float g = std::clamp ((float) v.pg.i (off::GLIDE_TIME) / 127.0f, 0.0f, 1.0f);
                    double k = std::pow (0.001, (double) dt / std::max (0.0005 + 0.6 * g, 0.0005));
                    v.f1 = want1 + (v.f1 - want1) * k;
                    v.f2 = want2 + (v.f2 - want2) * k;
                }
                else { v.f1 = want1; v.f2 = want2; v.needGlide = false; }

                // ---- oscillator generation ----
                double d1 = v.f1 / sr, d2 = v.f2 / sr;
                v.p1 += d1; if (v.p1 >= 1.0) v.p1 -= std::floor (v.p1);
                v.p2 += d2; if (v.p2 >= 1.0) v.p2 -= std::floor (v.p2);

                float pw1 = std::clamp ((float) v.pg.i (off::PWM1) / 127.0f
                                        + 0.5f * (mod[D_PW1] + mod[D_12P]), 0.02f, 0.98f);
                float pw2 = std::clamp ((float) v.pg.i (off::PWM2) / 127.0f
                                        + 0.5f * (mod[D_PW2] + mod[D_12P]), 0.02f, 0.98f);

                // XMOD: osc2 signal -> osc1 freq / osc1 pw / filter cutoff (1-sample tap)
                // r18.4: XMOD-dest = mod глубины XMOD
                const float xDepth = std::clamp ((float) v.pg.i (off::XMOD_DPTH) / 127.0f + mod[D_XMOD], 0.0f, 2.0f);
                const float xd = xDepth * v.lastOsc2;
                float cutMod = 0.0f;
                switch (v.pg.i (off::XMOD_DEST))
                {
                    case 0:  v.p1 += xd * 0.03; if (v.p1 >= 1.0 || v.p1 < 0.0) v.p1 -= std::floor (v.p1); break;
                    case 1:  pw1 = std::clamp (pw1 + xd * 0.35f, 0.02f, 0.98f); break;
                    default: cutMod = xd * 0.6f; break;
                }
                (void) 0;

                // OSC1 waves: bitmask tri=1, saw=2, pulse=4 of byte 74 (value 0 = default SAW)
                const uint8_t wb = v.pg.raw[off::WAVE_BITS];
                int m1 = (wb & 7) != 0 ? (wb & 7) : 2;
                float o1 = 0.0f, o2 = 0.0f;
                if (m1 & 1) o1 += tri  ((float) v.p1);
                if (m1 & 2) o1 += saw  ((float) v.p1);
                if (m1 & 4) o1 += pulse((float) v.p1, pw1);
                o1 *= 0.45f;
                // sub
                int sw = v.pg.i (off::SUB_WAVE);
                if (sw > 0) o1 += 0.4f * subOsc ((float) v.p1, sw);

                // OSC2 wave: enum byte 75 {0 = pulse, 1 = tri (836 patches), 2 = saw}
                int w2 = v.pg.i (off::WAVE2);
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

                float lvl1 = std::clamp ((float) v.pg.i (off::OSC1_LEVEL) / 127.0f + mod[D_LEV1], 0.0f, 1.3f);
                float lvl2 = std::clamp ((float) v.pg.i (off::OSC2_LEVEL) / 127.0f + mod[D_LEV2], 0.0f, 1.3f);
                float lvlN = std::clamp ((float) v.pg.i (off::NOISE_LEVEL) / 127.0f + mod[D_LEVN], 0.0f, 1.3f);

                float vca = env2v;
                // velocity dynamics on VCA (DYN2) & filter env (DYN1)
                float kVelVca = (float) v.pg.i (off::DYN2) / 127.0f;
                vca *= (1.0f - kVelVca) + kVelVca * v.velocity;

                // r18.4: EXT-dest = mod уровня внешнего входа
                float drive = lvl1 * o1 + lvl2 * o2 + lvlN * noise + ext * (1.0f + mod[D_EXT]);

                // ---- filter ----
                float cut = (float) v.pg.i (off::CUTOFF) / 127.0f;
                cut += mod[D_FILT];
                cut += cutMod;                                    // XMOD->filter
                float cutHz = 25.0f * std::pow (700.0f, std::clamp (cut, 0.0f, 1.0f));
                // key tracking
                float trk = (float) v.pg.i (off::TRACKING) / 127.0f;
                if (trk > 0)
                {
                    double ratio = base / 261.626;
                    cutHz *= (float) std::pow (ratio, trk);
                }
                float res = std::clamp ((float) v.pg.i (off::RESO) / 127.0f + mod[D_RESO], 0.0f, 1.0f);

                // r5: smooth cutoff/resonance (kills zipper on morph & preset jumps)
                v.cutSm += aCut * (cutHz - v.cutSm);
                cutHz = v.cutSm;
                v.resSm += aCut * (res - v.resSm);
                res = v.resSm;
                // r14: громкость голоса из ЕГО патча (VOLUME не «плавает» при смене пресета)
                v.vgP += aGain * (volGain (v.pg.i (off::VOLUME)) - v.vgP);

                int ftype = v.pg.i (off::FILT_TYPE);
                float fOut = 0;
                switch (ftype)
                {
                    // r17: SEM = Oberheim 12 dB (лестница, референс OberhVar.h).
                    // Был biquad Svf (Q до 12) — жёсткий резонансный «звон»
                    // без характера Oberheim («LP пердит»).
                    // r18.4: DEBUG-переключатель (меню сверху-справа):
                    // semFilterNew = true  -> r17 Oberheim12 (текущий),
                    // semFilterNew = false -> pre-r17 / v10 Svf SEM (как было до r17).
                    case F_SEM_LP:
                        if (semFilterNew) { v.ober.set (cutHz, res, (float) sr); fOut = v.ober.run (drive, 0); }
                        else              { v.svf.set (cutHz, 0.7f + res * 12.0f, (float) sr); fOut = v.svf.run (drive, 0); }
                        break;
                    case F_SEM_BP:
                        if (semFilterNew) { v.ober.set (cutHz, res, (float) sr); fOut = v.ober.run (drive, 1); }
                        else              { v.svf.set (cutHz, 0.7f + res * 10.0f, (float) sr); fOut = v.svf.run (drive, 1); }
                        break;
                    case F_SEM_HP:
                        if (semFilterNew) { v.ober.set (cutHz, res, (float) sr); fOut = v.ober.run (drive, 2); }
                        else              { v.svf.set (cutHz, 0.7f + res * 10.0f, (float) sr); fOut = v.svf.run (drive, 2); }
                        break;
                    case F_SEM_BR:
                        if (semFilterNew) { v.ober.set (cutHz, res, (float) sr); fOut = v.ober.run (drive, 3); }
                        else              { v.svf.set (cutHz, 0.7f + res * 10.0f, (float) sr); fOut = v.svf.run (drive, 3); }
                        break;
                    case F_MINI:   v.moog.set (cutHz, res, (float) sr);             fOut = v.moog.run (drive); break;
                    case F_AUX1:
                    {
                        // r16: карты AUX1 — OB-Xa (репо AudioFilterOBXa, MIT):
                        //  0 = «OB-Xa 24» (4-полюс 24dB, atan-сатурация),
                        //  1 = «OB-X 12» (2-полюс, диодная пара),
                        //  2 = «TB-303» (Wurtz, без изменений).
                        v.obxa.setResonance (res);
                        float og = std::tan (std::min (cutHz, (float) sr * 0.24f) * (float) M_PI / (float) sr);
                        switch (lastCard)
                        {
                            case 1: fOut = v.obxa.run2 (drive, og); break;          // OB-X 12
                            case 2: v.tbf.init (sr, cutHz, res); fOut = (float) v.tbf.eval (drive); break; // TB303
                            case 3:                                                 // r18.5: ARP2600
                                v.arp.set (cutHz, res, (float) sr); fOut = v.arp.run (drive); break;
                            case 4:                                                 // r18.5: CS-80 (тот же каскад, что в AUX2)
                            {
                                // r18.10: Q ограничен (HP≤3, LP≤6) + мягкий лимитер —
                                // на HPR/RESO каскад больше не «взрывается» (factory
                                // AUX2-пресеты: HPR до 107, RESO до 87)
                                const float qHp = 0.5f + (float) v.pg.i (off::HPR) / 127.0f * 2.5f;
                                v.svfHp.set (20.0f * std::pow (600.0f, (float) v.pg.i (off::HPF) / 127.0f),
                                             qHp, (float) sr);
                                const float hpOut = v.svfHp.run (drive, 2);
                                v.svf.set (cutHz, 0.5f + res * 5.5f, (float) sr);
                                fOut = v.svf.run (hpOut, 0);
                                fOut *= 1.0f / (1.0f + 0.9f * res * res);
                                { const float pa = std::fabs (fOut);
                                  if (pa > 0.7f) fOut *= 0.7f / pa; }
                                break;
                            }
                            case 5:                                                 // r18.5: WAH — auto-wah от LFO1
                            {
                                const float w = 0.5f + 0.5f * std::clamp (l1, -1.0f, 1.0f);
                                const float f = std::exp (std::log (200.0f) + w * std::log (1400.0f / 200.0f));
                                v.svf.set (f, 6.0f + res * 10.0f, (float) sr);     // BP, высокий Q
                                fOut = v.svf.run (drive, 1);
                                break;
                            }
                            case 6:                                                 // r18.5: TS-808 — транзисторный overdrive
                            {
                                const float g  = 2.0f + res * 10.0f;                // RESO = гейн драйва
                                const float t  = drive * g;
                                const float odd = std::tanh (t * 1.4f);
                                const float evn = 0.45f * (1.0f - std::exp (-std::fabs (t) * 1.2f))
                                                  * (t >= 0.0f ? 1.0f : -1.0f);
                                const float d  = 0.8f * odd + 0.5f * evn;
                                v.ts808 += 0.35f * (d - v.ts808);                   // one-pole LP (tone)
                                fOut = v.ts808;
                                break;
                            }
                            default: fOut = v.obxa.run4 (drive, og, 0); break;      // OB-Xa 24 (LP4)
                        }
                        break;
                    }
                    case F_AUX2:
                    {
                        // CS-80 (мануал: всегда в AUX2) = dual VCF: HP -> LP.
                        // HP freq = HPF-байт (20..120 Hz), HP reso = HPR-байт
                        // (r16: наконец используется, в r15 было (void)hpr),
                        // LP = CUTOFF/RESO. У SVF-HP DC -> ровно 0
                        // (один-полюсный hack r10 a*(hpS1+x-hpS2) пропускал DC).
                        // r18.10: Q ограничен (HP≤3, LP≤6) + мягкий лимитер —
                        // «взрывы» на HPR/RESO убраны (factory AUX2-пресеты:
                        // HPR до 107, RESO до 87); AGC r18.1 остался
                        const float qHp = 0.5f + (float) v.pg.i (off::HPR) / 127.0f * 2.5f;
                        v.svfHp.set (20.0f * std::pow (600.0f, (float) v.pg.i (off::HPF) / 127.0f),
                                     qHp, (float) sr);
                        const float hpOut = v.svfHp.run (drive, 2);
                        v.svf.set (cutHz, 0.5f + res * 5.5f, (float) sr);
                        fOut = v.svf.run (hpOut, 0);
                        fOut *= 1.0f / (1.0f + 0.9f * res * res);
                        { const float pa = std::fabs (fOut);
                          if (pa > 0.7f) fOut *= 0.7f / pa; }
                        break;
                    }
                    default: fOut = drive; break;
                }

                // r15: статический gain cut r11/r14 остаётся снятым (чистота);
                // r16: «взрывы» лечит не gain, а DC-чистые фильтры выше.
                float out = fOut * vca;
                if (! std::isfinite (out))                 // r5: NaN/Inf recovery
                {
                    v.svf.reset(); v.svfHp.reset(); v.moog.reset();
                    v.ober.reset(); v.obxa.reset(); v.tbf.reset(); v.arp.reset(); v.ts808 = 0.0f;
                    out = 0.0f;
                }

                // ---- per-voice flying pan ----
                int vi = (int) (&v - voices);
                if (vi >= 0 && vi < kNumVoices)
                {
                    // r18.7: pan-таблица = 8 записей (kNumVoices=32 для MULTI);
                    // r18.4: PANR-dest = mod скорости pan, PAND-dest = mod глубины pan
                    const int v8 = vi % 8;
                    float rate = std::clamp ((float) v.pg.raw[kArrRate + v8] + 64.0f * mod[D_PANR], 0.0f, 127.0f);
                    float pos  = v.mPanOn ? v.mPan : (((float) v.pg.raw[kArrPos + v8] - 64.0f) / 64.0f);
                    float dep  = v.mPanOn ? 1.0f
                                          : std::clamp ((float) v.pg.raw[kArrDepth + v8] / 127.0f + mod[D_PAND], 0.0f, 2.0f);
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

                float g = out * v.vgP * mgSm;

                // r5: 4 ms crossfade from last output on topology change (no clicks)
                if (v.holdXf > 0)
                {
                    // r18: делитель = длина ЭТОГО фейда. Раньше было xfN (4 мс):
                    // при 30-мс фейде (enum-прыжок морфа, смена карты) t на первых
                    // 26 мс был в −6..0 = инвертированный усиленный сигнал — слышался
                    // как «ретриг ноты» при движении ручки BLEND.
                    const int tot = v.holdXfTotal > 0 ? v.holdXfTotal : xfN;
                    float t = 1.0f - (float) v.holdXf / (float) tot;
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

            // r16: HP ~10 Hz, 2-го порядка (каскад 2x one-pole) на канал.
            // Один 12 Гц one-pole из r5 пропускал низкочастотный «гул»
            // (пик резонанса фильтров в 12..30 Гц проходил почти без
            // затухания). При 10 Гц: DC gain точно 0, 24 дБ/окт,
            // музыкальный низ не тронут (<0.6 дБ на 30 Гц, <0.3 дБ на 40 Гц).
            float yl = 0.0f, yr = 0.0f;
            {
                float o1 = mixL - dcX1L + dcA * dcY1L; dcX1L = mixL; dcY1L = o1;
                yl = o1 - dcX2L + dcA * dcY2L;          dcX2L = o1; dcY2L = yl;
                o1 = mixR - dcX1R + dcA * dcY1R; dcX1R = mixR; dcY1R = o1;
                yr = o1 - dcX2R + dcA * dcY2R;          dcX2R = o1; dcY2R = yr;
            }
            if (! std::isfinite (yl)) { yl = 0.0f; dcX1L = dcY1L = dcX2L = dcY2L = 0.0f; }
            if (! std::isfinite (yr)) { yr = 0.0f; dcX1R = dcY1R = dcX2R = dcY2R = 0.0f; }
            // r8: soft-clip — резонансные/перегруженные патчи не «взрываются»
            outL[i] = softClip (yl);
            outR[i] = softClip (yr);
        }
    }

    // -------------------------------------------------------------------------
    // simple built-in arpeggiator feed (called from processor's timer/MIDI logic)
    void recomputeGlobals() noexcept { updateGlobals(); }
    // r18.4: debug-переключатель поколения фильтра SEM (меню справа сверху)
    void setSemFilterNew (bool on) noexcept { semFilterNew = on; }
    bool getSemFilterNew() const noexcept { return semFilterNew; }

    std::vector<int> heldNotes;

private:
    Voice voices[kNumVoices];
    Lfo   lfo1, lfo2;
    int   ageCounter = 0;

    // Cached globals which are genuinely shared (external input and LFO setup).
    float extExtIn = 0.0f;
    // r18.4: debug-переключатель поколения фильтра SEM (меню сверху-справа):
    // true = r17 Oberheim12 (текущий звук), false = pre-r17 Svf (v10).
    bool   semFilterNew = true;
    // r18.4: LFO-rate mod (заполняется в голосовом цикле, применяется к глобальным LFO)
    float  lf1Rmod = 0.0f, lf2Rmod = 0.0f;
    int    rate1Byte = 64, rate2Byte = 64;

    // r5: smoothing / anti-click state
    float aGain = 0.003f, aCut = 0.004f, aPan = 0.01f;
    float dcA = 0.9993f;    // r16: coeff one-pole HP 10 Hz (каскад ×2 = 2-й порядок)
    float mgSm = 1.0f;
    float dcX1L = 0, dcY1L = 0, dcX2L = 0, dcY2L = 0;
    float dcX1R = 0, dcY1R = 0, dcX2R = 0, dcY2R = 0;
    int   xfN = 176, xfLongN = 1323;

    void updateGlobals() noexcept
    {
        extExtIn = (float) patch.i (off::EXT_IN);

        // LFO setup
        lfo1.setRate (patch.i (off::LFO1_RATE));
        lfo2.setRate (patch.i (off::LFO2_RATE));
        rate1Byte = patch.i (off::LFO1_RATE);   // r18.4: база для LFO-rate mod
        rate2Byte = patch.i (off::LFO2_RATE);
        int w1 = patch.i (off::LFO1_WAVSYNC), w2 = patch.i (off::LFO2_WAVSYNC);
        lfo1.square = (w1 != 0);
        lfo2.square = (w2 != 0);

        // r11: ADSR берутся в noteOn из захваченного патча (активные ноты не трогаются).
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
