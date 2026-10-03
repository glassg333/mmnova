// ============================================================================
// test_sanity.cpp — САМОПРОВЕРКА модуля без эмулятора (компилировать у себя):
//   g++ -std=c++17 -I../source -o test_sanity test_sanity.cpp && ./test_sanity
// ----------------------------------------------------------------------------
// Проверяются ФАКТЫ декомпиляции (значения из прошивки), а не звук:
//   1. Таблицы-якоря (kDrive, kHpqDamp, kDiv1) совпадают с дампами;
//   2. Закон DIST: точки измеренной отдачи 4k (MnMAmpDistCORE_VERIFIED.h);
//   3. Кольцо: BASE=64 → div-индекс 599 (конвенция верифицированного прока);
//   4. Фильтровая огибающая: ATK=0 → фаза растёт ~0.5/блок (P:$0511 bec);
//   5. envTerm: максимум $700=1792 при |BOFS−0.5|=0.5, env=1.
// ============================================================================
#include <cstdio>
#include <cmath>
#include "MnmVoiceFilterDist.h"

using namespace mnm;

static int fails = 0;
static void check(const char* name, bool ok)
{
    std::printf("[%s] %s\n", ok ? " OK " : "FAIL", name);
    if (!ok) ++fails;
}

int main()
{
    // 1. якоря таблиц -------------------------------------------------------
    check("kDrive[0] == 1.0",        std::abs(q23ToF(kDrive[0]) - 1.0f) < 1e-6f);
    check("kDrive[128] == sqrt(2)/4",std::abs(q23ToF(kDrive[128]) - 0.3535534f) < 1e-5f);
    check("kHpqDamp[0] == 0.5",      std::abs(q23ToF(kHpqDamp[0]) - 0.5f) < 1e-6f);
    check("kDiv1[0] == 9645",        (kDiv1[0] & 0xFFFFFFu) == 9645u);
    check("kDiv1[0] frac ~0.00115",  std::abs(q23ToF(kDiv1[0]) - 0.0011498f) < 1e-5f);

    // 2. закон DIST ----------------------------------------------------------
    MnmDist d;
    d.setDistWord(0x400000u);                       // D = 0.5 (центр ручки)
    check("DIST D=0.5: x0 == 0",        std::abs(d.x0()) < 1e-6f);
    check("DIST D=0.5: k == 0.0162109", std::abs(d.k() - 0.0162109f) < 1e-5f);
    check("DIST D=0.5: c == 1.0",       std::abs(d.c() - 1.0f) < 1e-6f);
    // измеренная точка: вход 0.5 → выход 0.032 (0.5·4k)
    {
        const float out = d.process(0.5f);
        check("DIST D=0.5: 0.5·4k == 0.032", std::abs(out - 0.0324f) < 2e-3f);
    }
    d.setDistWord(0x7FFFFFu);                       // D = 1.0
    check("DIST D=1.0: k == 1.0",       std::abs(d.k() - 1.0f) < 1e-4f);
    check("DIST D=1.0: c == sqrt(2)/4", std::abs(d.c() - 0.3535534f) < 1e-5f);
    {   // жёсткий клип: 0.5·4.0 = 2.0 → 1.0
        const float out = d.process(0.5f);
        check("DIST D=1.0: клип на 1.0", std::abs(out - 1.0f) < 1e-4f);
    }
    d.setDistWord(0x500000u);                       // D = 0.625
    {   // измеренная точка: x0=0.25, k=0.0775, 0.5·4k = 0.155
        const float out = d.process(0.5f);
        check("DIST D=0.625: 0.5·4k ≈ 0.155", std::abs(out - 0.155f) < 5e-3f);
    }

    // 3. кольцо: BASE=64 → индекс div1 599 -----------------------------------
    {
        const int idx = (int)(((uint64_t)(64u << 16) * 1199ull) >> 23);
        check("кольцо: BASE=64 → idx==599", idx == 599);
        const FilterRing r = computeRing(64u << 16, 127u << 16);
        check("кольцо: c[2]=width1[widx=126]≈0.187", std::abs(r.c[2] - q23ToF(kWidth1[126])) < 1e-5f);
    }

    // 4. фильтровая огибающая -------------------------------------------------
    {
        MnmFilterEnv e;
        e.setParams(0u, 64u << 16);     // ATK=0: шаг kEnvShapeA[0]
        e.trigger();
        e.tickBlock();                  // первый шаг: phase += kEnvShapeA[0]
        // kEnvShapeA[0] = 0.5 → после 2 блоков фаза должна насытиться (bec)
        e.tickBlock();
        check("FILT env: ATK=0, фаза насытилась за 2 блока",
              std::abs(e.phase() - 0.99999988f) < 1e-3f || e.phase() > 0.99f);
    }

    // 5. envTerm --------------------------------------------------------------
    {
        // Максимум: |BOFS−0.5|=0.5, env=1 → 0.25·4·$700·2/2^24·2^23 ≈ 447 слов
        // (точное значение — из цепочки mpy-сдвигов P:$0545-054B)
        const int32_t t = envTerm(0u, 1.0f);
        check("envTerm: полный ход ~447 слов (0.25·4·$700>>23)", t > 440 && t <= 448);
        // BOFS=64 (центр, |0|) → член = 0
        const int32_t t0 = envTerm(64u << 16, 1.0f);
        check("envTerm: 0 в центре", t0 == 0);
    }

    // 6. кадр целиком не падает и даёт конечные значения ----------------------
    {
        MnmVoiceFilterDist fx;
        fx.setFiltWords(wordFromKnob(32), wordFromKnob(80), wordFromKnob(84),
                        wordFromKnob(31), wordFromKnob(10), wordFromKnob(40),
                        wordFromKnob(58), wordFromKnob(80));
        fx.setDistKnob(40);
        fx.trigger();
        bool finite = true;
        for (int i = 0; i < 4800; ++i)
        {
            const float in = 0.5f * std::sin(2.0f * 3.14159265f * 220.0f * i / 44100.0f);
            const float oL = fx.processL(in);
            const float oR = fx.processR(in);
            if (!std::isfinite(oL) || !std::isfinite(oR)) { finite = false; break; }
        }
        check("цепь: 4800 сэмплов, выход конечен", finite);
    }

    std::printf("\n%s (%d провалов)\n", fails == 0 ? "ALL OK" : "HAS FAILURES", fails);
    return fails == 0 ? 0 : 1;
}
