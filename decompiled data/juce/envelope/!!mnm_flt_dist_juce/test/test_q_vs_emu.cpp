// ============================================================================
// test_q_vs_emu.cpp — диагностический реплей векторов svf_trace.py E1 через
//                     изолированный mnmq::QInterpolator (mnm_filter_q.h).
//                     НЕ гейт: сравнение с допуском + печать расхождений;
//                     бит-точная сверка (word-домен полинома/масштаба f) —
//                     по результатам прогона владелец решает о включении
//                     MNM_ENABLE_EXACT_Q.
//
// Сборка (вне workspace репозитория, по правилам mmnova):
//   g++ -std=c++17 -I../source -o test_q_vs_emu test_q_vs_emu.cpp
// Векторы: reference/svf_trace_results.json (E1, прогон эмулятора 2026-10-05).
// ============================================================================
#include <cstdio>
#include <cmath>
#include "mnm_filter_q.h"

using mnmq::QInterpolator;
using mnmq::QFrameInputs;
using mnmq::QFrameResult;

static int g_fail = 0;
static int g_warn = 0;

// слова q23 из float-значений векторов E1
static uint32_t w23(double v)
{
    const double s = v * 8388608.0;
    return (uint32_t)(int32_t)(s >= 0 ? s + 0.5 : s - 0.5);
}
static double f23(uint32_t w)
{
    int32_t s = (int32_t)(w & 0xFFFFFFu);
    if (s & 0x800000) s -= 0x1000000;
    return double(s) / 8388608.0;
}

struct Case
{
    const char* name;
    double base, wdth, qToA, qToB, param4;   // ручки (0..1)
    uint32_t idxWord;                        // измеренный y:(r6+$DA) (E1)
    double qhpMeasA, qlpMeasA;               // q после фильтра A (E1)
    double qhpMeasB, qlpMeasB;               // q после фильтра B (E1, y:$1d/$1e на $071A)
};

// векторы E1 (svf_trace.py, прогон 3 блока, дамп на $06CD/$071A)
static const Case kCases[] = {
    { "all_zero",          0.5, 0.5, 0.0, 0.0, 0.0, 1920, -0.98047, +0.25621, -0.03027, +0.03221 },
    { "BASE@408=0.7",      0.7, 0.5, 0.0, 0.0, 0.0, 2333, -0.98047, +0.25621, -0.27787, +0.22801 },
    { "WDTH@409=0.7",      0.5, 0.7, 0.0, 0.0, 0.0, 2333, -0.98047, +0.25621, -0.03027, +0.03221 },
    { "QA@40B=0.8",        0.5, 0.5, 0.8, 0.0, 0.0, 1920, -0.93107, +0.90444, -0.03027, +0.03221 },
    { "QB@40A=0.8",        0.5, 0.5, 0.0, 0.8, 0.0, 1920, -0.98047, +0.25621, -0.00058, +0.03320 },
};

static void runCase(const Case& c)
{
    // индекс по формуле Builder: BASE·$800 − $80 + WDTH·$800 (ENV=0, кейтрек=0)
    const uint32_t idx = w23(c.base) * 0x800 - 0x80 + w23(c.wdth) * 0x800;
    std::printf("case %-14s idx(calc)=%u idx(E1)=%u ", c.name, (unsigned)idx, c.idxWord);
    if (idx != c.idxWord) { std::printf("[IDX DIFF] "); ++g_warn; }

    // фильтр A: qWord = ячейка $40B = qToA (кросс, E1)
    QInterpolator qa(false);
    QFrameInputs ia{ idx, w23(c.qToA), w23(c.param4) };
    QFrameResult ra{};
    for (int blk = 0; blk < 3; ++blk) ra = qa.processFrame(ia);

    // фильтр B: qWord = ячейка $40A = qToB; индекс B = кейтрек-копия (x:$4D9);
    // в E1 кейтрек выключен (Y:$425=0) → x:(r6+$D9) = частичный индекс без WDTH
    const uint32_t idxB = w23(c.base) * 0x800 - 0x80;
    QInterpolator qb(true);
    QFrameInputs ib{ idxB, w23(c.qToB), w23(c.param4) };
    QFrameResult rb{};
    for (int blk = 0; blk < 3; ++blk) rb = qb.processFrame(ib);

    const double qhpA = f23((uint32_t)ra.qHp & 0xFFFFFFu);
    const double qlpA = f23((uint32_t)ra.qLp & 0xFFFFFFu);
    const double qhpB = f23((uint32_t)rb.qHp & 0xFFFFFFu);
    const double qlpB = f23((uint32_t)rb.qLp & 0xFFFFFFu);
    std::printf("A(qhp %+.5f / E1 %+.5f, qlp %+.5f / E1 %+.5f) ",
                qhpA, c.qhpMeasA, qlpA, c.qlpMeasA);
    std::printf("B(qhp %+.5f / E1 %+.5f, qlp %+.5f / E1 %+.5f)\n",
                qhpB, c.qhpMeasB, qlpB, c.qlpMeasB);

    const double tol = 0.02;
    if (std::fabs(qhpA - c.qhpMeasA) > tol || std::fabs(qlpA - c.qlpMeasA) > tol) ++g_warn;
    if (std::fabs(qhpB - c.qhpMeasB) > tol || std::fabs(qlpB - c.qlpMeasB) > tol) ++g_warn;
    // грубый отказ: знак/порядок
    if (std::fabs(qhpA - c.qhpMeasA) > 0.5 || std::fabs(qlpA - c.qlpMeasA) > 0.5 ||
        std::fabs(qhpB - c.qhpMeasB) > 0.5 || std::fabs(qlpB - c.qlpMeasB) > 0.5) ++g_fail;
}

int main()
{
    std::printf("test_q_vs_emu: реплей векторов E1 (svf_trace.py 2026-10-05)\n");
    for (const Case& c : kCases) runCase(c);
    std::printf("\nитог: FAIL=%d WARN=%d\n", g_fail, g_warn);
    std::printf("WARN = расхождение > 0.02 (ожидаемо до сверки ТРАНСКР-частей:\n");
    std::printf("экстракция полинома asl#$18, знак SUBR $064B, масштаб div-ветки f).\n");
    std::printf("FAIL = грубое расхождение > 0.5 — структуру чинить до регрессии.\n");
    return g_fail != 0;
}
