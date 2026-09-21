// ============================================================================
// test_mnm_filter.cpp — автономная проверка JUCE-порта против якорей,
// измеренных на бит-точном эмуляторе DSP56300 (без JUCE; plain C++17).
//
// Якоря (scripts/probe_eps.py, probe_resonance.py, iter.16):
//   ring[0] @BASE=64  = -0.496676   ring[1] = +1.7e-6
//   ring[0] @BASE=0   = -0.500009   ring[0] @BASE=127 = +0.473812
//   ring[2/3] = width1/width2[WDTH]
//   AMP: ATK=64 -> ~420 кадров до 1.0; DEC=0 -> -60 дБ за 62 кадра
// Сборка:  g++ -std=c++17 -O2 -I../Source test_mnm_filter.cpp -o test && ./test
// ============================================================================
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include "mnm_dual_filter.h"
#include "mnm_amp_env.h"

static int failures = 0;

static void expectNear (const char* what, double got, double want, double tol)
{
    const bool ok = std::fabs (got - want) <= tol;
    if (! ok) ++failures;
    printf ("  [%s] %-34s got %+.9f  want %+.9f  (tol %.1e)\n",
            ok ? " OK " : "FAIL", what, got, want, tol);
}

static void expectTrue (const char* what, bool ok)
{
    if (! ok) ++failures;
    printf ("  [%s] %s\n", ok ? " OK " : "FAIL", what);
}

int main()
{
    using namespace mnm;

    printf ("== 1. кольцо коэффициентов против эмулятора ==\n");
    {
        CoeffRing r = computeRing (64u << 16, 0u);
        expectNear ("ring0 @BASE=64",  r.c[0], -0.496676, 2.5e-5);
        expectNear ("ring1 @BASE=64",  r.c[1], +1.7e-6,   1.0e-6);
        expectNear ("ring2 = width1[0]", r.c[2], kWidth1[0], 1e-9);

        r = computeRing (0u, 0u);
        expectNear ("ring0 @BASE=0",   r.c[0], -0.500009, 2.5e-5);

        r = computeRing (127u << 16, 0u);
        expectNear ("ring0 @BASE=127", r.c[0], +0.473812, 2.5e-5);

        r = computeRing (64u << 16, 64u << 16);
        expectNear ("ring2 = width1[64]",  r.c[2], kWidth1[64], 1e-9);
        expectNear ("ring3 = width2[64]",  r.c[3], kWidth2[64], 1e-9);

        r = computeRing (127u << 16, 127u << 16);
        expectNear ("ring2 = width1[127]", r.c[2], kWidth1[127], 1e-9);
        expectNear ("ring3 = width2[127]", r.c[3], kWidth2[127], 1e-9);
    }

    printf ("== 2. каскад 1: синус проходит, стабильность ==\n");
    {
        DualFilter f;
        FilterParams p;                       // BASE=64, WDTH=0, HPQ=LPQ=0
        float in[kFrame * 2] {}, out[kFrame * 2] {};
        double peak = 0.0;
        for (int fr = 0; fr < 400; ++fr)
        {
            for (int i = 0; i < kFrame; ++i)
            {
                const float t = (float) (fr * kFrame + i);
                const float s = 0.5f * std::sin (2.f * 3.14159265f * t * 7.f / 44100.f);
                in[2 * i] = in[2 * i + 1] = s;
            }
            f.processFrame (in, out, p);
            for (int i = 0; i < kFrame * 2; ++i)
            {
                if (! (std::fabs (out[i]) < 10.f))
                { expectTrue ("стабильность: |out|<10", false); goto done; }
                peak = std::fmax (peak, (double) std::fabs (out[i]));
            }
        }
        expectTrue ("стабильность: 400 кадров без NaN/взрыва", true);
        expectTrue ("сигнал проходит (peak > 0.01)", peak > 0.01);
        printf ("      peak = %.4f\n", peak);
    done:;
    }

    printf ("== 3. полный узел с резонансом: стабильность на максимуме ==\n");
    {
        MonoMachineDualFilter node;
        node.prepare (44100.0);
        FilterParams p;
        p.baseWord = wordFromKnob (80);
        p.wdthWord = wordFromKnob (100);
        p.hpqWord  = wordFromKnob (127);       // максимум резонанса
        p.lpqWord  = wordFromKnob (127);
        node.setParams (p);
        float in[kFrame * 2] {}, out[kFrame * 2] {};
        bool ok = true;
        double peak = 0.0;
        for (int fr = 0; fr < 600; ++fr)
        {
            for (int i = 0; i < kFrame; ++i)
            {
                const float t = (float) (fr * kFrame + i);
                in[2 * i] = in[2 * i + 1] =
                    0.25f * std::sin (2.f * 3.14159265f * t * 13.f / 44100.f);
            }
            node.processFrame (in, out);
            for (int i = 0; i < kFrame * 2; ++i)
            {
                if (! (std::fabs (out[i]) < 50.f)) { ok = false; break; }
                peak = std::fmax (peak, (double) std::fabs (out[i]));
            }
            if (! ok) break;
        }
        expectTrue ("резонанс @max: 600 кадров стабильно", ok);
        printf ("      peak = %.4f\n", peak);
    }

    printf ("== 4. AMP-энвелоп: временные карты ==\n");
    {
        MonoMachineAmpEnv env;
        MonoMachineAmpEnv::Params ep;
        ep.atk = 64; ep.dec = 90; ep.rel = 40;
        env.reset(); env.trig (1);
        int frames = 0;
        while (frames < 100000)
        {
            (void) env.tick (ep);
            ++frames;
            if (env.phase != MonoMachineAmpEnv::kAtk) break;   // конец атаки
        }
        // эмулятор: 419 кадров до phase=1 (+1 кадр перехода) => 419..421
        expectNear ("ATK=64: кадров до конца атаки", frames, 420, 2);

        // DEC=0: множитель 0.897687/кадр; -60 дБ = ln(0.001)/ln(0.897687) = 61.9
        ep.atk = 0; ep.dec = 0;
        env.reset(); env.trig (1);
        while (env.phase == MonoMachineAmpEnv::kAtk) (void) env.tick (ep);
        const double startLevel = std::fabs (env.level);
        int decFrames = 0;
        while (decFrames < 100000)
        {
            (void) env.tick (ep);
            ++decFrames;
            if (std::fabs (env.level) < 0.001 * startLevel) break;
        }
        expectNear ("DEC=0: кадров до -60 дБ", decFrames, 62, 4);

        // KILL: множитель 0.904016/кадр -> ln(0.001)/|ln(0.904016)| = 68.4
        env.reset(); env.trig (1);
        while (env.phase == MonoMachineAmpEnv::kAtk) (void) env.tick (ep);
        env.trig (3);
        int killFrames = 0;
        const double killStart = std::fabs (env.level);
        while (killFrames < 10000)
        {
            (void) env.tick (ep);
            ++killFrames;
            if (std::fabs (env.level) < 0.001 * killStart) break;
        }
        expectNear ("KILL: кадров до -60 дБ", killFrames, 68, 4);
    }

    printf ("\n%s: %d провал(ов)\n", failures ? "ПРОВАЛ" : "ВСЁ ЗЕЛЁНОЕ", failures);
    return failures ? 1 : 0;
}
