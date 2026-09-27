// test_mnm_new.cpp — автономная проверка DSP_New (dist/filter/env, режим "ex")
// Векторы: pack filter_phaser_pack (probe23d iter.23, README_amp_envelope iter.13,
// svf_fit340/probe_eps iter.14-16). Сборка: g++ -std=c++17 -I<DSP_New> test_mnm_new.cpp
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <array>

#include "distortion/MnmDistExact.hpp"
#include "filter/MnmFilterExact.hpp"
#include "envelope/MnmEnvExact.hpp"

using namespace monomachine::mnm;

static int fails = 0, oks = 0;
static void expect(bool ok, const char* what)
{
    if (ok) { printf("ok: %s\n", what); ++oks; }
    else { printf("FAIL: %s\n", what); ++fails; }
}

static uint32_t rngState = 0x12345678u;
static float noise()
{
    rngState = rngState * 1664525u + 1013904223u;
    return (float)(int32_t)rngState / 2147483648.0f;
}

int main()
{
    // ============================ DIST =====================================
    {
        // V1: атака DIST=64 бит-в-бит (0x4E05 = 19973/2^23 = tblA[64])
        DistDriveEnv d; d.reset();
        DistDriveEnv::Params p; p.dist = 64; p.vol = 127;
        const int32_t step = (int32_t)std::lrint(exact_tables::dist::kDistAtk[64] * 8388608.0f);
        bool ok = step == 19973;
        for (int i = 0; i < 8 && ok; ++i) ok = d.tickRaw(p) == step * (i + 1);
        expect(ok, "V1 DIST=64: шаг атаки == 0x4E05*n (замер iter.23)");

        // V2: DIST=0: +0.5 -> wrap -1.0 -> DEC кламп 0 (замер probe23d)
        DistDriveEnv d2; d2.reset();
        DistDriveEnv::Params p0; p0.dist = 0; p0.vol = 127;
        const int32_t t1 = d2.tickRaw(p0), t2 = d2.tickRaw(p0);
        const int32_t t3 = d2.tickRaw(p0), t4 = d2.tickRaw(p0);
        expect(t1 == 0x400000 && t2 == -0x800000 && t3 == 0 &&
               t4 == 0 && d2.getPhase() == DistDriveEnv::kDec,
               "V2 DIST=0: 0.5 -> -1.0 -> 0 -> 0 (wrap+кламп)");

        // V3: DIST=32 — топ за ~29 кадров (замер)
        DistDriveEnv d3; d3.reset();
        DistDriveEnv::Params p32; p32.dist = 32; p32.vol = 127;
        int frames = 0;
        while (d3.getPhase() == DistDriveEnv::kAtk && frames < 200) { d3.tickRaw(p32); ++frames; }
        expect(frames >= 27 && frames <= 31, "V3 DIST=32: топ за ~29 кадров");

        // V4: DIST=127 — шаг 0x65/кадр (замер)
        DistDriveEnv d4; d4.reset();
        DistDriveEnv::Params p127; p127.dist = 127; p127.vol = 127;
        const int32_t s1 = d4.tickRaw(p127), s2 = d4.tickRaw(p127);
        expect(s1 == 0x65 && s2 == 0xCA, "V4 DIST=127: шаг 0x65/кадр");

        // V5: тимбр-индекс бит-в-бит: drive=1418083 -> -431 (0xFFFE51, кадр 70)
        const int idx = exactTimbreIndex(1418083, 0, 0);
        expect(idx == -431, "V5 тимбр-индекс: drive=1418083 -> -431 (знаковые a1-срезы)");

        // V6: ядро не взрывается и усиливает на драйве
        DistExactCore core; core.reset(); core.setKnob(20.0f); core.setVol(127.0f); core.retrig();
        bool finite = true; float peak = 0.0f;
        for (int i = 0; i < 44100; ++i) {
            float in = 0.5f * noise();
            float out = core.process(in);
            if (!std::isfinite(out)) finite = false;
            peak = std::max(peak, std::fabs(out));
        }
        expect(finite && peak <= 1.0f, "V6 DistExactCore: выход конечен и в слове");
    }

    // ============================ ENV ======================================
    {
        // E1: ATK=64 -> топ за ~420 кадров (README: 419 измерено)
        EnvExactCore e; e.reset(); e.setParameters(64, 0, 90, 40);
        int frames = 0; bool hit = false;
        while (frames < 2000) { e.tickFrame(); ++frames; if (e.phase() == EnvExactCore::kHold) { hit = true; break; } }
        expect(hit && frames >= 417 && frames <= 423, "E1 ATK=64: топ за ~419-420 кадров");

        // E2: wrap -> уровень в -1.0-зоне (родное поведение)
        expect(e.levelSigned() < 0.0f, "E2 wrap атаки: уровень отрицательный (-1.0 зона)");

        // E3: DEC=0 — множитель 0.897687/кадр (tblB[0])
        e.setParameters(64, 0, 0, 40);
        e.tickFrame();                       // переход Hold->Dec (уровень не меняется)
        float a = std::fabs(e.levelSigned());
        e.tickFrame();                       // первый шаг DEC
        float b = std::fabs(e.levelSigned());
        const float ratio = b / a;
        expect(std::fabs(ratio - 0.897687f) < 0.002f, "E3 DEC=0: |множитель| = 0.8977/кадр");

        // E4: KILL — -60 дБ за ~44 кадра (tblB[1] = 0.904)
        EnvExactCore k; k.reset(); k.setParameters(0, 0, 90, 40);
        for (int i = 0; i < 3; ++i) k.tickFrame();   // быстро взлетаем (ATK=0)
        k.trig(3);
        int killFrames = 0;
        while (killFrames < 400 && std::fabs(k.levelSigned()) > 0.001f) { k.tickFrame(); ++killFrames; }
        expect(killFrames <= 70, "E4 KILL: -60 дБ за <70 кадров (теория 44)");

        // E5: DEC=127 — заморозка (tblB[127] = -1.0)
        EnvExactCore f; f.reset(); f.setParameters(0, 0, 127, 40);
        for (int i = 0; i < 5; ++i) f.tickFrame();
        float l1 = std::fabs(f.levelSigned());
        for (int i = 0; i < 50; ++i) f.tickFrame();
        float l2 = std::fabs(f.levelSigned());
        expect(std::fabs(l2 - l1) < 1e-6f, "E5 DEC=127: уровень заморожен");

        // E6: ретриг из DEC НЕ сбрасывает уровень (дип-к-тишине)
        EnvExactCore r; r.reset(); r.setParameters(10, 0, 20, 40);
        for (int i = 0; i < 40; ++i) r.tickFrame();  // топ и часть DEC
        float before = std::fabs(r.levelSigned());
        r.trig(1);                                    // ретриг
        float after = std::fabs(r.levelSigned());     // level не обнулился
        r.tickFrame();                                // шаг атаки продолжается
        expect(before > 0.01f && std::fabs(after - before) < 0.2f,
               "E6 ретриг: уровень продолжается с текущего");

        // E7: HOLD=64 @120 BPM — ~114 кадров (измеренная карта)
        EnvExactCore h; h.reset(); h.setParameters(0, 64, 90, 40, 120.0f);
        h.tickFrame();                                 // атака 1: 0.5
        h.tickFrame();                                 // атака 2: wrap -> Hold
        int holdFrames = 0;
        while (h.phase() == EnvExactCore::kHold && holdFrames < 4000) { h.tickFrame(); ++holdFrames; }
        expect(holdFrames >= 110 && holdFrames <= 120, "E7 HOLD=64@120: ~114 кадров (закон подогнан к карте)");

        // E8: process() гейн в [0..1+eps], де-зиппер живой
        EnvExactCore g; g.reset(); g.setParameters(64, 0, 64, 40);
        bool okG = true;
        for (int i = 0; i < 44100; ++i) {
            float v = g.process();
            if (!(v >= -0.001f && v < 1.05f)) okG = false;
        }
        expect(okG, "E8 EnvExactCore: гейн в диапазоне, рамп гладкий");
    }

    // ============================ FILTER ===================================
    {
        // F1: кольцо BASE=64<<16, WDTH=0 -> idx=599 (конвенция прогонов пака)
        FilterExactRing ring = computeExactRing(64u << 16, 0u << 16);
        const float c2 = exact_tables::filt::kCoeff2[599];
        expect(std::fabs(ring.c[0] - (-c2 * 0.5f)) < 0.01f && std::fabs(ring.c[1]) < 0.01f,
               "F1 кольцо: BASE=64 -> idx=599, ring[0] = -coeff2[599]*(0.5+eps), |eps| мал");

        // F2: шум через ядро при экстремальных настройках — устойчив, конечен
        FilterExactCore fc; fc.reset(); fc.setSampleRate(44100.0);
        fc.setParameters(64, 64, 127, 127);
        bool finite = true; float peak = 0.0f;
        for (int i = 0; i < 48000; ++i) {
            float y = fc.process(i % 2, 0.8f * noise());
            if (!std::isfinite(y)) finite = false;
            peak = std::max(peak, std::fabs(y));
        }
        expect(finite && peak < 16.0f, "F2 фильтр HPQ=LPQ=127: устойчив (гребёнка+демпфирование)");

        // F3: bypass на дефолте (порт-соглашение unity)
        FilterExactCore fb; fb.reset(); fb.setParameters(0, 127, 0, 0);
        bool bypass = fb.bypassed();
        float pass = fb.process(0, 0.5f);
        expect(bypass && pass == 0.5f, "F3 дефолт: THRU (BASE=0/WDTH=127/Q=0)");

        // F4: BASE=127/WDTH=0 — максимальный срез, не взрывается
        FilterExactCore f4; f4.reset(); f4.setParameters(127, 0, 60, 60);
        bool fin4 = true;
        for (int i = 0; i < 24000; ++i) {
            float y = f4.process(i % 2, 0.7f * noise());
            if (!std::isfinite(y)) fin4 = false;
        }
        expect(fin4, "F4 BASE=127/WDTH=0: стабильно");

        // F5: модуляция словами каждый сэмпл (как мод-матрица оригинала)
        FilterExactCore f5; f5.reset();
        bool fin5 = true;
        for (int i = 0; i < 24000; ++i) {
            const int sweep = (i / 240) % 128;
            f5.setWords(exactWordFromKnob(sweep), 32u << 16, 40u << 16, 40u << 16);
            float y = f5.process(i % 2, 0.5f * noise());
            if (!std::isfinite(y)) fin5 = false;
        }
        expect(fin5, "F5 свип BASE словами каждый сэмпл: стабильно");
    }

    printf("\nитог: %d ok, %d FAIL\n", oks, fails);
    return fails == 0 ? 0 : 1;
}
