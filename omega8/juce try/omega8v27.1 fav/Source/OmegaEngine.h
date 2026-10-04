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
    float aT = 0.001f, dT = 0.001f, dk2T = 0.0f, rT = 0.1f, sus = 1.0f;
    float dlyT = 0.0f, dlyLeft = 0.0f;
    bool  hasDk2 = false;

    static float time (int v) noexcept   // 0..127 -> seconds
    {
        if (v <= 0) return 0.0002f;
        return 0.0002f * std::pow (50000.0f, (float) v / 127.0f);
    }
    void setParams (int a, int d, int dk2, int s, int r, int dly = 0) noexcept
    {
        aT = time (a); dT = time (d); rT = time (r);
        sus = (float) std::clamp (s, 0, 127) / 127.0f;
        // v27: мануал стр.24: DKY2 активен при 0 < dk2 < 127 (0 и 127 = бесконечный сустейн)
        hasDk2 = (dk2 > 0 && dk2 < 127);
        dk2T   = hasDk2 ? time (dk2) : 0.0f;
        dlyT   = (dly <= 0 ? 0.0f : time (dly));   // r6: DLY перед ATTACK (как в оригинале)
    }
    void noteOn (bool resetLevel = false) noexcept
    {
        if (resetLevel) level = 0.0f;
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
            case Sustain:
                if (hasDk2)
                {
                    level -= (level / std::max (dk2T, 1.0e-3f)) * dt * 2.5f;
                    if (level < 1.0e-5f) level = 0.0f;
                }
                else
                {
                    // плавное следование за ручкой SUS / морфом без щелчков
                    level += (sus - level) * std::min (1.0f, dt * 60.0f);
                }
                break;
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
    double   phase = 0.0;
    float    rateHz = 1.0f;
    bool     square = false;
    int      wave = 0;       // v27: 0=TRI/SIN, 1=SAW, 2=SQR, 3=RND (S&H)
    uint32_t rng = 0x2468ACE1u;
    float    rndVal = 0.0f;

    void setRate (int v) noexcept
    {
        rateHz = 0.02f * std::pow (500.0f, (float) v / 127.0f);
    }
    void process (double dt) noexcept
    {
        phase += rateHz * dt;
        if (phase >= 1.0)
        {
            phase -= std::floor (phase);
            rng = rng * 1664525u + 1013904223u;
            rndVal = ((float) (rng >> 16) / 32768.0f) - 1.0f;
        }
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
        if (wave == 1) return 1.0f - 2.0f * ph;                  // SAW
        if (wave == 2 || square) return ph < 0.5f ? 1.0f : -1.0f; // SQR
        if (wave == 3) return rndVal;                            // RND (Sample & Hold)
        return (float) std::sin (ph * 2.0 * M_PI);               // TRI/SIN
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
// v26: ARP2600 — СТАБИЛЬНАЯ TPT-лестница (прежняя была forward-Euler: fc→0.94,
// предел устойчивости ~0.5 → лимит-циклы и «взрывы»). Топология как у Oberheim12,
// но с более «злым» входным tanh — характер ARP.
struct ArpLadder
{
    struct OnePole
    {
        double alpha = 0, beta = 0, z1 = 0;
        double Tick (double s) noexcept
        {
            double vn = (s - z1) * alpha;
            double out = vn + z1;
            z1 = vn + out;
            return out;
        }
        double Fb() const noexcept { return beta * z1; }
    };
    OnePole lp[4];
    double K = 0, gamma4 = 1, alpha0 = 1;
    void set (float cutHz, float res, float sr) noexcept
    {
        cutHz = std::clamp (cutHz, 10.0f, sr * 0.45f);
        K = 4.0 * std::clamp (res, 0.0f, 1.0f);
        const double T = 1.0 / (double) sr;
        const double g = std::tan ((double) cutHz * T * M_PI);
        const double G = g / (1.0 + g);
        for (auto& o : lp) o.alpha = G;
        lp[0].beta = G * G * G / (1.0 + g);
        lp[1].beta = G * G / (1.0 + g);
        lp[2].beta = G / (1.0 + g);
        lp[3].beta = 1.0 / (1.0 + g);
        gamma4 = G * G * G * G;
        alpha0 = 1.0 / (1.0 + K * gamma4);
    }
    float run (float x) noexcept
    {
        const double sigma = lp[0].Fb() + lp[1].Fb() + lp[2].Fb() + lp[3].Fb();
        double u = ((double) x * (1.0 + K) - K * sigma) * alpha0;
        u = std::tanh (u * 1.3);
        const double s1 = lp[0].Tick (u);
        const double s2 = lp[1].Tick (s1);
        const double s3 = lp[2].Tick (s2);
        const double s4 = lp[3].Tick (s3);
        return (float) s4;
    }
    void reset() noexcept { for (auto& o : lp) o.z1 = 0; }
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
    bool   snapPitch = true;             // v27: на свежей ноте частота ставится сразу, далее сглаживается
    float  unisonDetune = 0.0f;          // v27: UNISON расстройка голоса (в полутонах)
    float  unisonGain   = 1.0f;          // v27: компенсация громкости стека UNISON
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
    double hostBpm = 0.0;   // v26: темп хоста для PAN [CLK] (midi clock sync панорамы)
    float  masterGain = 1.0f;
    bool   arpOn = false;

    std::atomic<int> auxCard  { 0 };   // r9: карта AUX1; r18.5: 0=OB-Xa 24, 1=OB-X 12, 2=TB303, 3=ARP2600, 4=CS-80, 5=WAH, 6=TS-808
    std::atomic<int> auxCard2 { 4 };   // v27: карта AUX2 (по мануалу по умолчанию 4 = CS-80, но переключаемая)
    // r18: было -1 → на ПЕРВОМ блоке (note-on) срабатывал «сдвиг карты» с
    // кроссфейдом от holdVal=0 — в r16/r17 это был инвертированный бам на каждой
    // атаке (e2e peak 0.826 на тихом патче). Теперь 0 = начальное значение auxCard.
    int lastCard  = 0;
    int lastCard2 = 4;

    void prepare (double sampleRate) noexcept
    {
        sr = sampleRate;
        const float s = (float) sampleRate;
        aGain  = 1.0f - std::exp (-1.0f / (0.006f * s));    // 6 ms: master/patch gain
        aCut   = 1.0f - std::exp (-1.0f / (0.004f * s));    // 4 ms: cutoff/resonance
        aPan   = 1.0f - std::exp (-1.0f / (0.001f * s));    // 1 ms: pan
        aPitch = 1.0f - std::exp (-1.0f / (0.0025f * s));   // v27: 2.5 ms anti-zipper pitch при морфе/ручках
        dcA    = std::exp (-2.0f * (float) M_PI * 10.0f / s); // r16: HP 10 Hz (2x one-pole)
        xfN    = std::max (16, (int) (0.005 * sampleRate)); // 5 ms change xfade
        xfLongN = std::max (32, (int) (0.012 * sampleRate)); // v27: короткий фейд при смене топологии фильтра (без 150-мс DC-щелчка v26)
        mgSm = masterGain;
        dcX1L = dcY1L = dcX2L = dcY2L = 0.0f;
        dcX1R = dcY1R = dcX2R = dcY2R = 0.0f;
        for (auto& v : voices) { v.reset(); v.obxa.setSampleRate ((float) sampleRate); }
    }

    void setPatch (const Patch& p) noexcept
    {
        // v27: живой BLEND и отклик ручек без щелчков:
        // 1) состояния фильтров сбрасываются ТОЛЬКО если реально сменился FILT_TYPE
        //    (в v26 сброс всех фильтров и 150-мс DC hold срабатывали при изменении ЛЮБОГО enum-байта на 50%,
        //    что давало громкий щелчок/провал даже когда оба патча использовали один и тот же фильтр!).
        // 2) ADSR-параметры активных обычных голосов плавно обновляются при кручении ручек/морфе.
        patch = p;
        for (auto& v : voices)
        {
            if (! v.active || v.mPart >= 0) continue;
            const bool filtChanged = (p.raw[off::FILT_TYPE] != v.pg.raw[off::FILT_TYPE]);
            v.pg = p;
            v.env1.setParams (v.pg.i (off::ENV1_ATK), v.pg.i (off::ENV1_DEC),
                              v.pg.i (off::ENV1_DK2), v.pg.i (off::ENV1_SUS), v.pg.i (off::ENV1_REL),
                              v.pg.i (off::DLY1));
            v.env2.setParams (v.pg.i (off::ENV2_ATK), v.pg.i (off::ENV2_DEC),
                              v.pg.i (off::ENV2_DK2), v.pg.i (off::ENV2_SUS), v.pg.i (off::ENV2_REL),
                              v.pg.i (off::DLY2));
            v.env3.setParams (v.pg.i (off::ENV3_ATK), v.pg.i (off::ENV3_DEC),
                              v.pg.i (off::ENV3_DK2), v.pg.i (off::ENV3_SUS), v.pg.i (off::ENV3_REL),
                              v.pg.i (off::DLY3));
            if (filtChanged)
            {
                v.svf.reset(); v.svfHp.reset(); v.ober.reset();
                v.moog.reset(); v.obxa.reset(); v.tbf.reset(); v.arp.reset(); v.ts808 = 0.0f;
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
        // v27: проверяем, удерживается ли уже клавиша (для Legato Glide и MTRG),
        // и запоминаем последнюю звучащую частоту для плавного полифонического портаменто
        bool anyKeyHeld = false;
        double lastF1 = 0.0, lastF2 = 0.0;
        int bestAge = -1;
        for (const auto& x : voices)
        {
            if (x.active && x.mPart < 0)
            {
                if (x.keyDown) anyKeyHeld = true;
                if (x.age > bestAge && x.f1 > 10.0)
                {
                    bestAge = x.age;
                    lastF1 = x.f1;
                    lastF2 = x.f2;
                }
            }
        }

        // v27: UNISON (byte 16) + VOICE_QTY (byte 17: 0=FOUR, 1=TWO, 16=EIGH)
        const bool uniOn = (patch.i (off::UNISON) != 0);
        int numStack = 1;
        if (uniOn)
        {
            const int vq = patch.i (off::VOICE_QTY);
            numStack = (vq == 1 ? 2 : (vq >= 8 ? 8 : 4));
        }
        const float stackGain = 1.0f / std::sqrt ((float) numStack);

        // r18.7: LFO KEY-trigger (мануал page3): сброс фазы по клавишам
        {
            const int k1 = patch.i (off::LFO1_KEY);
            if (k1 == 1 || k1 == 3) lfo1.reset();
            const int k2 = patch.i (off::LFO2_KEY);
            if (k2 == 1 || k2 == 3) lfo2.reset();
        }

        const int vmode = patch.i (off::VMODE);   // 0=FIRST, 1=NEXT, 2=CYCL, 3=LAST
        const int mtrg  = patch.i (off::MTRG);    // 0=OFF, 1=ENV1, 2=ENV2, 3=ENV3

        for (int s = 0; s < numStack; ++s)
        {
            Voice* v = nullptr;
            if (! uniOn)
            {
                for (auto& x : voices) if (x.active && x.mPart < 0 && x.note == note && ! x.keyDown) { v = &x; break; }
                if (v == nullptr) for (auto& x : voices) if (x.active && x.mPart < 0 && x.note == note && x.keyDown) { v = &x; break; }
            }
            if (v == nullptr)
            {
                if (vmode == 0) // FIRST: всегда с первого свободного из 8
                {
                    for (int vi = 0; vi < 8; ++vi)
                        if (! voices[vi].active) { v = &voices[vi]; break; }
                }
                else // NEXT / CYCL: циклический обход 8 аналоговых голосов
                {
                    for (int step = 0; step < 8; ++step)
                    {
                        int idx = (rrVoiceIdx + step) & 7;
                        if (! voices[idx].active)
                        {
                            v = &voices[idx];
                            rrVoiceIdx = (idx + 1) & 7;
                            break;
                        }
                    }
                }
                if (v == nullptr)
                    for (auto& x : voices) if (! x.active) { v = &x; break; }
            }
            if (v == nullptr)
            {
                // steal: prefer releasing, then oldest
                Voice* best = &voices[0];
                for (auto& x : voices) if (! x.keyDown && x.env2.value() < best->env2.value()) best = &x;
                if (best->keyDown) { best = &voices[0]; for (auto& x : voices) if (x.age < best->age) best = &x; }
                v = best;
            }

            const bool wasActive = v->active;
            v->note = note;
            v->velocity = std::clamp (vel, 0.0f, 1.0f);
            v->keyDown = true;
            v->active = true;
            v->mPart = -1;
            v->mPanOn = false;
            v->age = ageCounter++;
            v->pg = patch;
            v->vgP = volGain (v->pg.i (off::VOLUME));

            if (numStack > 1)
            {
                const float pos = ((float) s - 0.5f * (float) (numStack - 1)) / (float) std::max (1, numStack - 1);
                v->unisonDetune = pos * 0.18f;   // ±9 центов жирного аналогового стека
                v->unisonGain   = stackGain;
            }
            else
            {
                v->unisonDetune = 0.0f;
                v->unisonGain   = 1.0f;
            }

            // v27: GLIDE (bit6 = ON, bit2 = LEGATO mode: скользит только при удержании другой ноты)
            const uint8_t gFlags = v->pg.raw[off::GLIDE_FLAGS];
            const bool glideOn   = (gFlags & 0x40) != 0 && v->pg.i (off::GLIDE_TIME) > 0;
            const bool legatoMod = (gFlags & 0x04) != 0;
            const bool canGlide  = legatoMod ? anyKeyHeld : (lastF1 > 10.0);
            if (glideOn && canGlide && lastF1 > 10.0)
            {
                v->f1 = lastF1;
                v->f2 = lastF2;
                v->needGlide = true;
                v->snapPitch = false;
            }
            else
            {
                v->needGlide = false;
                v->snapPitch = true;
            }

            if (v->pg.i (off::PAN_KEY) != 0)
                v->panPhase = 0.0;

            if (! wasActive)
            {
                v->svf.reset(); v->svfHp.reset(); v->moog.reset();
                v->obxa.reset(); v->tbf.reset(); v->arp.reset(); v->ts808 = 0.0f;
            }
            v->holdXf = 0;
            v->holdXfTotal = 0;

            v->env1.setParams (v->pg.i (off::ENV1_ATK), v->pg.i (off::ENV1_DEC),
                               v->pg.i (off::ENV1_DK2), v->pg.i (off::ENV1_SUS), v->pg.i (off::ENV1_REL),
                               v->pg.i (off::DLY1));
            v->env2.setParams (v->pg.i (off::ENV2_ATK), v->pg.i (off::ENV2_DEC),
                               v->pg.i (off::ENV2_DK2), v->pg.i (off::ENV2_SUS), v->pg.i (off::ENV2_REL),
                               v->pg.i (off::DLY2));
            v->env3.setParams (v->pg.i (off::ENV3_ATK), v->pg.i (off::ENV3_DEC),
                               v->pg.i (off::ENV3_DK2), v->pg.i (off::ENV3_SUS), v->pg.i (off::ENV3_REL),
                               v->pg.i (off::DLY3));
            v->env1.noteOn (mtrg == 1);
            v->env2.noteOn (mtrg == 2);
            v->env3.noteOn (mtrg == 3);
        }
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
        v->unisonDetune = 0.0f;
        v->unisonGain   = 1.0f;
        v->needGlide = ((pp.raw[off::GLIDE_FLAGS] & 0x40) != 0) && ! fresh;
        v->snapPitch = ! v->needGlide;
        if (pp.i (off::PAN_KEY) != 0) v->panPhase = 0.0;
        if (fresh)
        {
            v->svf.reset(); v->svfHp.reset(); v->moog.reset();
            v->obxa.reset(); v->tbf.reset(); v->arp.reset(); v->ts808 = 0.0f;
        }
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

        // v27: смена карты AUX1 / AUX2 — сброс состояний карт + короткий кроссфейд (без щелчка)
        const int card1 = auxCard.load();
        const int card2 = auxCard2.load();
        if (card1 != lastCard || card2 != lastCard2)
        {
            lastCard  = card1;
            lastCard2 = card2;
            for (auto& v : voices)
            {
                v.obxa.reset(); v.tbf.reset(); v.arp.reset(); v.svf.reset(); v.svfHp.reset(); v.ts808 = 0.0f;
                if (v.active) { v.holdVal = v.lastG; v.holdXf = xfLongN; v.holdXfTotal = xfLongN; }
            }
        }

        for (int i = 0; i < n; ++i)
        {
            // r5: smoothed gains (anti-click on patch/morph/volume changes)
            mgSm += aGain * (masterGain - mgSm);

            // LFOs are global
            // v27: LFO1_SYNC (byte 121) & LFO2_SYNC (byte 125) — MIDI clock sync к темпу хоста (1/1, 1/2, 1/4, 1/8)
            static const double kLfoSyncDiv[5] = { 0.0, 0.25, 0.5, 1.0, 2.0 };
            const int ls1 = patch.i (off::LFO1_SYNC);
            const int ls2 = patch.i (off::LFO2_SYNC);
            if (ls1 > 0 && ls1 <= 4 && hostBpm > 1.0)
                lfo1.rateHz = (float) ((hostBpm / 60.0) * kLfoSyncDiv[ls1]) * std::pow (2.0f, lf1Rmod);
            else
                lfo1.rateHz = 0.02f * std::pow (500.0f, std::clamp (rate1Byte + 64.0f * lf1Rmod, 0.0f, 127.0f) / 127.0f);

            if (ls2 > 0 && ls2 <= 4 && hostBpm > 1.0)
                lfo2.rateHz = (float) ((hostBpm / 60.0) * kLfoSyncDiv[ls2]) * std::pow (2.0f, lf2Rmod);
            else
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
                // v27: DYN1 (byte 112) и DYN3 (byte 114) — чувствительность к велосити для ENV1 и ENV3
                //      INVT (byte 132: 0=OFF, 1=ENV1, 2=E1+3, 3=ENV3) — инверсия огибающих 1 и 3
                const float kVel1 = (float) v.pg.i (off::DYN1) / 127.0f;
                const float kVel3 = (float) v.pg.i (off::DYN3) / 127.0f;
                float env1v = v.env1.value() * ((1.0f - kVel1) + kVel1 * v.velocity);
                float env2v = v.env2.value();
                float env3v = v.env3.value() * ((float) v.pg.i (off::ENV3_AMT) / 127.0f)
                              * ((1.0f - kVel3) + kVel3 * v.velocity);
                const int invt = v.pg.i (off::INVT);
                if (invt == 1 || invt == 2) env1v = -env1v;
                if (invt == 2 || invt == 3) env3v = -env3v;

                float mod[D_COUNT] = { 0 };
                auto add = [&] (int dest, float src, int amt) noexcept
                {
                    if (dest > D_OFF && dest < D_COUNT && amt != 0)
                        mod[dest] += src * ((float) amt / 127.0f);
                };

                // v27: источники CONT1 (byte 83) и CONT2 (byte 84): 0=TRAK, 1=DYNA, 2=MODW, 3=PEDL, 4=PRES
                auto evalContSrc = [&] (int srcCode, float ccFallback) noexcept -> float
                {
                    switch (srcCode)
                    {
                        case 0:  return std::clamp ((float) (v.note - 60) / 48.0f, -1.0f, 1.0f); // TRAK
                        case 1:  return v.velocity;                                              // DYNA
                        case 2:  return modWheel;                                                // MODW
                        case 4:  return pressure;                                                // PRES
                        default: return ccFallback;                                              // PEDL / CC16-17
                    }
                };
                // CONT1/CONT2 are source selectors: return exactly one chosen source.
                // The earlier extra +cont1/+cont2 contaminated TRAK/DYNA/MODW/PRES modes.
                const float c1Src = evalContSrc (v.pg.i (off::CONT1_SRC), cont1);
                const float c2Src = evalContSrc (v.pg.i (off::CONT2_SRC), cont2);

                add (v.pg.i (off::MODW_D1), modWheel,     v.pg.i (off::MODW_A1));
                add (v.pg.i (off::MODW_D2), modWheel,     v.pg.i (off::MODW_A2));
                add (v.pg.i (off::DYN_D1),  v.velocity,   v.pg.i (off::DYN_A1));
                add (v.pg.i (off::DYN_D2),  v.velocity,   v.pg.i (off::DYN_A2));
                add (v.pg.i (off::BEND_D1), bend,         v.pg.i (off::BEND_A1));
                add (v.pg.i (off::BEND_D2), bend,         v.pg.i (off::BEND_A2));
                add (v.pg.i (off::PRES_D1), pressure,     v.pg.i (off::PRES_A1));
                add (v.pg.i (off::PRES_D2), pressure,     v.pg.i (off::PRES_A2));
                add (v.pg.i (off::C1_D1),   c1Src,        v.pg.i (off::C1_A1));
                add (v.pg.i (off::C1_D2),   c1Src,        v.pg.i (off::C1_A2));
                // CONT2 amplitudes are signed (stored = display + 64)
                auto sgn = [&] (int o) noexcept { return ((int) v.pg.raw[(size_t) o] - 64) / 64.0f; };
                int c2d1 = v.pg.i (off::C2_D1), c2d2 = v.pg.i (off::C2_D2);
                if (c2d1 > 0 && c2d1 < D_COUNT) mod[c2d1] += c2Src * sgn (off::C2_A1);
                if (c2d2 > 0 && c2d2 < D_COUNT) mod[c2d2] += c2Src * sgn (off::C2_A2);

                // r18.4: LF1R/LF2R = mod скорости LFO
                lf1Rmod = mod[D_LF1R];
                lf2Rmod = mod[D_LF2R];

                // r18.4: LF1D/LF2D = mod глубины LFO
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
                // v27: OSC2_FINE (byte 78) в диапазоне ±50 центов (±0.5 полутона: (raw - 64) / 128.0f),
                // чтобы фабричный detune=40 (-24) давал музыкальный хорус -18.75 центов, а не диссонанс -75 центов!
                const float fine2 = ((int) v.pg.raw[off::OSC2_FINE] - 64) / 128.0f;
                float semi1 = (float) v.pg.i (off::OSC1_FREQ) - 36.0f + v.unisonDetune;
                float semi2 = (float) v.pg.i (off::OSC2_FREQ) - 36.0f + fine2 - v.unisonDetune * 0.5f;

                // v27: OSC2_MODE (byte 115: 0=HALF, 1=OCT, 2=SUB, 3=TRK, 4=NORM)
                switch (v.pg.i (off::OSC2_MODE))
                {
                    case 0: semi2 -= 0.5f * (float) (v.note - 60); break; // HALF tracking
                    case 1: semi2 += 12.0f; break;                        // OCT (+1 octave)
                    case 2: semi2 -= 12.0f; break;                        // SUB (-1 octave)
                    default: break;                                       // TRK / NORM
                }
                // v27: OSC2_ENV1 (byte 131: ENV1 -> OSC2 pitch, мануал стр.20)
                if (v.pg.i (off::OSC2_ENV1) > 0)
                    semi2 += env1v * ((float) v.pg.i (off::OSC2_ENV1) / 127.0f) * 36.0f;

                // v27: OCTAVE (byte 2): учёт знакового заёма младшего ниббла ((ob + 8) >> 4),
                // чтобы 0x3D (61), 0x3E (62), 0x3F (63 = 64-1) оставались в октаве 4 (MID),
                // а не падали на октаву вниз в 42 фабричных патчах!
                const int ob = v.pg.i (off::OCTAVE);
                const int hi = ((ob + 8) >> 4) & 0x07;
                const float octSemis = (float) ((hi - 4) * 12);

                semi1 += 24.0f * (mod[D_FRE1] + mod[D_12F]);
                semi2 += 24.0f * (mod[D_FRE2] + mod[D_12F]);
                semi1 += 12.0f * mod[D_12D];  semi2 += 12.0f * mod[D_12D];
                semi2 += 6.0f * mod[D_12R];
                semi1 += bend * 2.0f;
                semi2 += bend * 2.0f;

                double base = 440.0 * std::pow (2.0, (v.note - 69) / 12.0);
                // v27: MASTER_TUNE (byte 77, мануал стр.16: «[TUNE] - Tune percentage: 100% is full strength;
                // lesser settings randomize tuning or intentional imperfection»):
                // - при 64 (100%): 0 дрейфа, идеальный строй Accu-Tune;
                // - при < 64 (0..99%): симметричный аналоговый разброс между 8 голосами (до ±20 центов при 0%) без сдвига центра;
                // - при > 64 (65..127): плавная подстройка вверх (+1..+63 цента).
                static const double kVoiceDrift[8] = { -0.95, +0.85, -0.60, +0.55, -0.35, +0.40, -0.15, +0.25 };
                const int mtRaw = v.pg.i (off::MASTER_TUNE);
                double tuneCents = 0.0;
                if (mtRaw < 64)
                    tuneCents = kVoiceDrift[viP & 7] * ((64 - mtRaw) / 64.0) * 20.0;
                else if (mtRaw > 64)
                    tuneCents = (double) (mtRaw - 64);
                const double tuneMul = std::pow (2.0, tuneCents / 1200.0);

                double want1 = base * std::pow (2.0, (semi1 + octSemis) / 12.0) * tuneMul;
                double want2 = base * std::pow (2.0, (semi2 + octSemis) / 12.0) * tuneMul;

                // glide & v27 per-sample anti-zipper pitch smoothing
                if (v.needGlide && (v.pg.raw[off::GLIDE_FLAGS] & 0x40) != 0)
                {
                    float g = std::clamp ((float) v.pg.i (off::GLIDE_TIME) / 127.0f, 0.0f, 1.0f);
                    double k = std::pow (0.001, (double) dt / std::max (0.0005 + 0.6 * g, 0.0005));
                    v.f1 = want1 + (v.f1 - want1) * k;
                    v.f2 = want2 + (v.f2 - want2) * k;
                }
                else if (v.snapPitch)
                {
                    v.f1 = want1; v.f2 = want2;
                    v.snapPitch = false;
                }
                else
                {
                    // v27: плавное (2.5 мс) сглаживание частоты на каждом сэмпле убирает
                    // ступенчатый «треск» (zipper) при морфе OSC1_FREQ / OSC2_FREQ / OCTAVE
                    v.f1 += aPitch * (want1 - v.f1);
                    v.f2 += aPitch * (want2 - v.f2);
                }

                // ---- oscillator generation ----
                double d1 = v.f1 / sr, d2 = v.f2 / sr;
                v.p1 += d1;
                bool osc1Wrap = false;                       // v26: OSC2 SYNC (byte 118)
                if (v.p1 >= 1.0) { v.p1 -= std::floor (v.p1); osc1Wrap = true; }
                v.p2 += d2; if (v.p2 >= 1.0) v.p2 -= std::floor (v.p2);
                // v26: OSC2 SYNC ON/OFF (мануал стр.508/1126): OSC1 hard-syncs OSC2 —
                // на каждом периоде OSC1 фаза OSC2 сбрасывается (та же базовая частота).
                if (osc1Wrap && v.pg.i (off::OSC2_SYNC) != 0)
                    v.p2 = 0.0;

                float pw1 = std::clamp ((float) v.pg.i (off::PWM1) / 127.0f
                                        + 0.5f * (mod[D_PW1] + mod[D_12P]), 0.02f, 0.98f);
                float pw2 = std::clamp ((float) v.pg.i (off::PWM2) / 127.0f
                                        + 0.5f * (mod[D_PW2] + mod[D_12P]), 0.02f, 0.98f);

                // XMOD: osc2 signal -> osc1 freq / osc1 pw / filter cutoff / dual O+V (1-sample tap)
                const float xDepth = std::clamp ((float) v.pg.i (off::XMOD_DPTH) / 127.0f + mod[D_XMOD], 0.0f, 2.0f);
                const float xd = xDepth * v.lastOsc2;
                float cutMod = 0.0f;
                switch (v.pg.i (off::XMOD_DEST))
                {
                    case 0:  v.p1 += xd * 0.03; if (v.p1 >= 1.0 || v.p1 < 0.0) v.p1 -= std::floor (v.p1); break;
                    case 1:  pw1 = std::clamp (pw1 + xd * 0.35f, 0.02f, 0.98f); break;
                    case 3:  v.p1 += xd * 0.03; if (v.p1 >= 1.0 || v.p1 < 0.0) v.p1 -= std::floor (v.p1);
                             cutMod = xd * 0.6f; break;
                    default: cutMod = xd * 0.6f; break;
                }

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
                    case F_AUX1:   fOut = runAuxCard (lastCard,  v, drive, cutHz, res, l1); break;
                    case F_AUX2:   fOut = runAuxCard (lastCard2, v, drive, cutHz, res, l1); break;
                    default:       fOut = drive; break;
                }

                // v26/v27: страховка от резонансного разхождения:
                if (std::fabs (fOut) > 8.0f)
                {
                    v.svf.reset(); v.svfHp.reset(); v.moog.reset();
                    v.ober.reset(); v.obxa.reset(); v.tbf.reset(); v.arp.reset(); v.ts808 = 0.0f;
                    fOut = 0.0f;
                }
                else if (std::fabs (fOut) > 1.5f)
                {
                    fOut = 1.5f * std::tanh (fOut / 1.5f);
                }
                float out = fOut * vca * v.unisonGain;
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
                    const int v8 = vi % 8;
                    float rate = std::clamp ((float) v.pg.raw[kArrRate + v8] + 64.0f * mod[D_PANR], 0.0f, 127.0f);
                    float pos  = v.mPanOn ? v.mPan : (((float) v.pg.raw[kArrPos + v8] - 64.0f) / 64.0f);
                    if (v.unisonGain < 0.99f) pos = std::clamp (pos + v.unisonDetune * 2.5f, -1.0f, 1.0f);
                    float dep  = v.mPanOn ? 1.0f
                                          : std::clamp ((float) v.pg.raw[kArrDepth + v8] / 127.0f + mod[D_PAND], 0.0f, 2.0f);
                    // v27: PAN [CLK] (byte 129) или PAN_SYNC (byte 116) — синк пан-LFO к темпу хоста (1/1, 1/2, 1/4, 1/8)
                    int pclk = v.pg.i (off::PAN_CLK);
                    if (pclk == 0) pclk = v.pg.i (off::PAN_SYNC);
                    if (pclk > 0 && pclk <= 4 && hostBpm > 1.0)
                    {
                        static const double pdiv[5] = { 0.0, 0.25, 0.5, 1.0, 2.0 };
                        v.panLfoHz = (float) ((hostBpm / 60.0) * pdiv[pclk]);
                    }
                    else
                        v.panLfoHz = 0.02f * std::pow (400.0f, rate / 127.0f);
                    v.panPhase += v.panLfoHz / sr;
                    if (v.panPhase >= 1.0) v.panPhase -= 1.0;

                    // v27: PAN_WAVE (byte 120: 0=TRI, 1=SAW, 2=SIN, 3=SQR)
                    float panWaveVal = 0.0f;
                    const float pph = (float) v.panPhase;
                    switch (v.pg.i (off::PAN_WAVE) & 3)
                    {
                        case 0:  panWaveVal = 4.0f * std::abs (pph - 0.5f) - 1.0f; break; // TRI
                        case 1:  panWaveVal = 2.0f * pph - 1.0f; break;                   // SAW
                        case 3:  panWaveVal = (pph < 0.5f ? 1.0f : -1.0f); break;         // SQR
                        default: panWaveVal = (float) std::sin (pph * 2.0 * M_PI); break; // SIN
                    }
                    float p = std::clamp (pos + dep * panWaveVal + 0.5f * mod[D_PAN], -1.0f, 1.0f);
                    float pl = std::cos ((p + 1.0f) * (float) M_PI / 4.0f);
                    float pr = std::sin ((p + 1.0f) * (float) M_PI / 4.0f);
                    v.panL = pl; v.panR = pr;
                    v.panSmL += aPan * (pl - v.panSmL);   // r5: anti-click pan
                    v.panSmR += aPan * (pr - v.panSmR);
                }

                float g = out * v.vgP * mgSm;

                // r5: short crossfade from last output on filter topology change (no clicks)
                if (v.holdXf > 0)
                {
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
            float yl = 0.0f, yr = 0.0f;
            {
                float o1 = mixL - dcX1L + dcA * dcY1L; dcX1L = mixL; dcY1L = o1;
                yl = o1 - dcX2L + dcA * dcY2L;          dcX2L = o1; dcY2L = yl;
                o1 = mixR - dcX1R + dcA * dcY1R; dcX1R = mixR; dcY1R = o1;
                yr = o1 - dcX2R + dcA * dcY2R;          dcX2R = o1; dcY2R = yr;
            }
            if (! std::isfinite (yl)) { yl = 0.0f; dcX1L = dcY1L = dcX2L = dcY2L = 0.0f; }
            if (! std::isfinite (yr)) { yr = 0.0f; dcX1R = dcY1R = dcX2R = dcY2R = 0.0f; }
            outL[i] = softClip (yl);
            outR[i] = softClip (yr);
        }
    }

    // -------------------------------------------------------------------------
    // simple built-in arpeggiator feed (called from processor's timer/MIDI logic)
    void recomputeGlobals() noexcept { updateGlobals(); }
    void setSemFilterNew (bool on) noexcept { semFilterNew = on; }
    bool getSemFilterNew() const noexcept { return semFilterNew; }

    std::vector<int> heldNotes;

private:
    Voice voices[kNumVoices];
    Lfo   lfo1, lfo2;
    int   ageCounter = 0;
    int   rrVoiceIdx = 0;

    float extExtIn = 0.0f;
    bool   semFilterNew = true;
    float  lf1Rmod = 0.0f, lf2Rmod = 0.0f;
    int    rate1Byte = 64, rate2Byte = 64;

    // r5/v27: smoothing / anti-click state
    float aGain = 0.003f, aCut = 0.004f, aPan = 0.01f, aPitch = 0.01f;
    float dcA = 0.9993f;
    float mgSm = 1.0f;
    float dcX1L = 0, dcY1L = 0, dcX2L = 0, dcY2L = 0;
    float dcX1R = 0, dcY1R = 0, dcX2R = 0, dcY2R = 0;
    int   xfN = 176, xfLongN = 512;

    float runAuxCard (int cardId, Voice& v, float drive, float cutHz, float res, float l1) noexcept
    {
        v.obxa.setResonance (res);
        const float og = std::tan (std::min (cutHz, (float) sr * 0.24f) * (float) M_PI / (float) sr);
        switch (cardId)
        {
            case 1: // OB-X 12 (2-pole diode pair)
                return v.obxa.run2 (drive, og);
            case 2: // TB-303 (Wurtz diode ladder)
                v.tbf.init (sr, cutHz, res);
                return (float) v.tbf.eval (drive);
            case 3: // ARP2600 (4-pole TPT ladder)
                v.arp.set (cutHz, res, (float) sr);
                return v.arp.run (drive);
            case 4: // CS-80 (dual VCF: HP -> LP, мануал стр.21)
            {
                // v27: HPF 20..1200 Hz (в v26 было 20*600 = 12 кГц, что глушило звук!)
                // и мягкая tanh-компенсация вместо жёсткого обрезания 0.7f
                const float qHp  = 0.5f + (float) v.pg.i (off::HPR) / 127.0f * 2.5f;
                const float hpHz = 20.0f * std::pow (60.0f, (float) v.pg.i (off::HPF) / 127.0f);
                v.svfHp.set (hpHz, qHp, (float) sr);
                const float hpOut = v.svfHp.run (drive, 2);
                v.svf.set (cutHz, 0.55f + res * 6.0f, (float) sr);
                float lpOut = v.svf.run (hpOut, 0);
                return 1.15f * std::tanh (lpOut * 0.95f);
            }
            case 5: // WAH (bandpass wah: CUTOFF = педаль/центр 180..2600 Гц + LFO1 auto-wah, RESO = добротность Q)
            {
                const float w = 0.5f * std::clamp (l1, -1.0f, 1.0f);
                const float baseWah = std::clamp (cutHz, 180.0f, 2600.0f);
                const float f = std::clamp (baseWah * std::pow (2.0f, w * 0.85f), 150.0f, 3400.0f);
                v.svf.set (f, 2.5f + res * 9.5f, (float) sr);
                return 1.55f * v.svf.run (drive, 1);
            }
            case 6: // TS-808 (Overdrive + Tone LP: RESO = DRIVE/перегруз, CUTOFF = TONE/частота среза)
            {
                const float g   = 1.5f + res * 12.0f;               // RESO (REZONATE) = DRIVE
                const float t   = drive * g;
                const float odd = std::tanh (t * 1.4f);
                const float evn = 0.45f * (1.0f - std::exp (-std::fabs (t) * 1.2f))
                                  * (t >= 0.0f ? 1.0f : -1.0f);
                const float clipped = (0.8f * odd + 0.5f * evn) / (1.0f + 0.22f * res);
                // CUTOFF (FREQUENCY) = TONE фильтр (150 Гц .. 12 кГц) с лёгким mid-focus
                const float toneHz = std::clamp (cutHz, 150.0f, 12000.0f);
                v.svf.set (toneHz, 0.707f + 0.45f * res, (float) sr);
                return v.svf.run (clipped, 0);
            }
            default: // 0: OB-Xa 24 (4-pole LP)
                return v.obxa.run4 (drive, og, 0);
        }
    }

    void updateGlobals() noexcept
    {
        extExtIn = (float) patch.i (off::EXT_IN);

        // LFO setup
        lfo1.setRate (patch.i (off::LFO1_RATE));
        lfo2.setRate (patch.i (off::LFO2_RATE));
        rate1Byte = patch.i (off::LFO1_RATE);
        rate2Byte = patch.i (off::LFO2_RATE);
        int w1 = patch.i (off::LFO1_WAVSYNC), w2 = patch.i (off::LFO2_WAVSYNC);
        lfo1.wave   = (w1 == 8 ? 2 : (w1 & 3));
        lfo2.wave   = (w2 == 8 ? 2 : (w2 & 3));
        lfo1.square = (lfo1.wave == 2);
        lfo2.square = (lfo2.wave == 2);
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
