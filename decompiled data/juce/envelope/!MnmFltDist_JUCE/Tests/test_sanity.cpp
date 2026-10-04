// ============================================================================
// test_sanity.cpp — самопроверка фактов порта (без JUCE): g++ -std=c++17
// ----------------------------------------------------------------------------
// Проверяются: якоря таблиц прошивки (извлечены из dsp1_pmem.bin), репликация
// DIV эталонного эмулятора, математика ветки панча (P:$0537–$0556), законы
// верифицированных паков (панч 13, AHDR 7, env2 14), ограниченность кольца,
// стабильность каскада, латентность 1 кадр, закрытие гейтов.
// mnm_16: резонатор func_000397 (8 тапов) исключён из тракта — это
// делей-машина, отдельный модуль (правка владельца).
// ============================================================================
#include <cstdio>
#include <cmath>
#include <array>
#include <algorithm>
#include "mnm_dsp56300_math.h"
#include "mnm_firmware_tables.h"
#include "mnm_filter_ring.h"
#include "mnm_cascade340.h"
#include "mnm_dist_stage.h"       // ступень DIST $079F-$07D1 (mnm_18)
#include "mnm_tone_index.h"       // индекс тона $0537-$056C (mnm_18)
#include "MnmDistDrive.hpp"        // панч DIST (пак 13)
#include "MnmAmpEnv.hpp"           // AHDR (пак 7)
#include "MnmEnv2.hpp"             // env2-гейт (пак 14)
#include "MnmFltDistVoice.h"

using namespace mnm;

static int gFail = 0;
static void check(bool ok, const char* what, const char* info = "")
{
    std::printf("  [%s] %s %s\n", ok ? "OK " : "FAIL", what, info);
    if (!ok) ++gFail;
}

static void testTables()
{
    std::printf("— таблицы прошивки (якоря, извлечение из dsp1_pmem.bin):\n");
    check(kDrive[0] == 0x7FFFFF, "kDrive[0] == 1.0");
    check(std::abs(q23ToF(kDrive[128]) - 0.3535534f) < 1e-6f, "kDrive[128] == 0.3535534");
    check(kHpqDamp[0] == 0x400000, "kHpqDamp[0] == 0.5");
    check(std::abs(q23ToF(kHpqDamp[127]) - 0.0315f) < 2e-3f, "kHpqDamp[127] ~ 0.0315");
    check(kDiv1[0] == 9645, "kDiv1[0] == 9645 (0.0011498)");
    check(sext24(kEnvShapeB[0]) < 0, "kEnvShapeB — знакоотрицательная (DEC/REL)");
    check(kEnvShapeE[0] == 0x020000, "kEnvShapeE[0] == 0.015625 (вычитаемый спад)");
    check(kCutoff[1699] != 0, "kCutoff[1699] существует (кламп $6A3)");
}

static void testDiv24()
{
    std::printf("— div24 (24-шаговое DIV DSP56300, делимое 1<<24 — доказано эмулятором):\n");
    // Трасса diag_ring_0592.py 2026-10-04: move #$1,a даёт A = 1<<24
    // (левое выравнивание imm8), после 24 div A = 00005E9C000075 →
    // q0 = 0x75 = 2^24/143188 (avg kDiv1[0]+kDiv2[0]). Бит-в-бит с ядром.
    {
        const int32_t avg0 = (sext24(kDiv1[0]) + sext24(kDiv2[0])) >> 1;
        const uint64_t D = div24(1ull << 24, avg0);
        char buf[64]; std::snprintf(buf, 64, "(avg=%d q0=%06X)", avg0, (unsigned)(D & 0xFFFFFF));
        check(avg0 == 143188 && (D & 0xFFFFFF) == 0x75,
              "div24(1<<24, 143188) == 0x75 — пин ядра (трасса $0588/$0589)", buf);
    }
    bool deterministic = true;
    for (int t = 0; t < 32; ++t)
    {
        const int idx = (t * 97) % 1201, widx = (t * 31) % 128;
        const int32_t avgS = (sext24(kDiv1[idx]) + sext24(kDiv2[widx])) >> 1;
        const uint64_t D1 = div24(1ull << 24, avgS);
        const uint64_t D2 = div24(1ull << 24, avgS);
        if (D1 != D2) deterministic = false;
    }
    check(deterministic, "div24 детерминирован");
}

static void testEnvTerm()
{
    std::printf("— математика ветки панча (P:$0537–$0556, MnmDistDrive::toneIndex):\n");
    // ветка 4·$700·env·|p−0.5|² — член панча (вердикт §2.2), не «BOFS-огибающая»
    check(envTerm(64u << 16, 1.0f) == 0, "|p−0.5| = 0 → член 0");
    const int32_t t = envTerm(0u, 1.0f);
    char buf[64]; std::snprintf(buf, 64, "(max = %d слов)", t);
    check(t > 1700 && t <= 1792, "макс члена = $700 = 1792 слов", buf);
    int32_t a = mpyA1(0x7FFFFF, 0x800) - 0x80;
    a += envTerm(0u, 1.0f);
    a = std::clamp(a, -0x800000, 0x700);
    check(a == 0x700, "кламп суммы на $700 (P:$054D-054E)");
}

static void testPunch()
{
    std::printf("— панч DIST (P:$04FF–$0556, пак 13, векторы exp69):\n");
    MnmDistDrive p;
    MnmDistDrive::Params pp;         // $40C/$40D/$408/$40E — дыра страницы, нули
    p.retrig();                       // $420 := 1
    // атака: +0.5/кадр → сатурация на 2-м кадре (~0.7 мс)
    const int32_t f0 = p.tickRaw(pp);
    const int32_t f1 = p.tickRaw(pp);
    check(f0 == 0x400000, "кадр 0: level = +0.5 (kDistAtk[0])");
    check(f1 == 0x7FFFFF, "кадр 1: дата-лимитер $7FFFFF (+1.0, не −1.0)");
    // hold 1 кадр → спад 1/64/кадр → ровно 64 кадра
    int frames = 0; int32_t lv = f1;
    while (lv > 0 && frames < 4096) { lv = p.tickRaw(pp); ++frames; }
    char buf[64]; std::snprintf(buf, 64, "(спад %d кадров)", frames);
    check(frames == 64, "спад ровно 64 кадра = 23.2 мс (1/64/кадр)", buf);
    // ретриг сбрасывает
    p.retrig();
    check(p.tickRaw(pp) == 0x400000, "ретриг $420: уровень с нуля");
}

static void testAhdr()
{
    std::printf("— AHDR (P:$088E–$08D7, пак 7):\n");
    mnmfm::MnmAmpEnv e;
    e.reset();
    e.trig(1);                        // $428 := 1 (note-on)
    float peak = 0; int frames = 0;
    while (frames++ < 256 && e.level < 0x700000) {
        e.tick(0, 0, 64u << 16, 64u << 16, 120);   // ATK=0: быстрый подъём
        peak = std::max(peak, (float)e.level);
    }
    check(peak > 0.5f * 8388608.f, "атака поднимает уровень (kEnvShapeA)");
    e.trig(2);                        // $428 := 2 (note-off → REL)
    const int32_t l0 = e.level;
    e.tick(0, 0, 64u << 16, 0, 120);  // REL=0: быстрый мультипликативный спад
    check(e.level < l0, "REL мультипликативно спадает");
    check(e.phase == mnmfm::MnmAmpEnv::kRel, "фаза REL");
}

static void testEnv2()
{
    std::printf("— env2-гейт (P:$04A8–$04F5, пак 14):\n");
    mnmenv2::MnmEnv2 e;
    e.atk = 0; e.dec = 64u << 16; e.sust = 64u << 16; e.rel = 0;
    e.gate = 1; e.slot = 0;
    for (int i = 0; i < 8; ++i) e.frame();
    check(e.level > 0x400000, "атака аддитивная до насыщения (kEnvShapeA)");
    check(e.phase == 4, "переполнение → фаза 4 (DEC, P:$04C1)");
    for (int i = 0; i < 4096 && e.level > 0x210000; ++i) e.frame();
    char buf[64]; std::snprintf(buf, 64, "(level=%.4f)", e.level / 8388608.0f);
    check(std::abs(e.level / 8388608.0f - 0.25f) < 0.03f,
          "DEC мультипликативный до SUS² (sust=64 → 0.25)", buf);
}

static void testRing()
{
    std::printf("— кольцо коэффициентов (P:$0576-$059A, пины эмулятора):\n");
    {
        const int i0 = (int)((0ull * 0x4AF) >> 23);
        const int iMax = (int)(((uint64_t)0x7FFFFF * 0x4AF) >> 23);
        char buf[64]; std::snprintf(buf, 64, "(0..%d из 1201)", iMax);
        check(i0 == 0 && iMax >= 1190 && iMax <= 1200, "индекс kDiv1 покрывает таблицу", buf);
    }
    // Пины: снимены с эмулятора ядра (sweep_ring_grid.py, 30/30 бит-в-бит).
    // Формула: epsW = limit24((q0·dif·2)<<6) [P:$0591, ЛИМИТЕР FM §5.4.1.2];
    //          Y04 = limit24((−c2·eps·2 − c2·2^24) >> 1) [P:$0592-0594, asr a!]
    struct Pin { unsigned bk, wk, y04, y05; };
    const Pin pins[] = {
        {  0,   0, 0xBFFFB3, 0x0000EE },
        {  0,  64, 0x800055, 0x7FFFFF },   // зона лимитера eps
        {  0, 127, 0x800055, 0x7FFFFF },
        {  64,  64, 0x80D9E7, 0x7FFFFF },
        { 115,  66, 0xFB8401, 0x7FFFFF },
        { 127,   0, 0x3CA5E1, 0xFFFF28 },
        { 127, 127, 0x71E202, 0x705922 },
    };
    bool pinsOK = true;
    for (const Pin& p : pins)
    {
        const FilterRing r = computeRing(p.bk << 16, p.wk << 16);
        if (fToQ23(r.c[0]) != p.y04 || fToQ23(r.c[1]) != p.y05)
            pinsOK = false;
    }
    check(pinsOK, "7 пинов эмулятора Y04/Y05 бит-в-бит (вкл. зону лимитера)");
    {
        const FilterRing r = computeRing(0x400000u, 0x400000u);
        check(std::isfinite(r.c[0]) && std::isfinite(r.c[1]) &&
              std::isfinite(r.c[2]) && std::isfinite(r.c[3]), "коэффициенты конечны");
        check(r.c[1] >= -1.0f && r.c[1] <= 1.0f, "eps в домене лимитера: eps ∈ [−1, +1]");
        const FilterRing r2 = computeRing(0x7FFFFFu, 0x400000u);
        check(r.c[0] != r2.c[0], "кольцо зависит от BASE");
    }
    {
        // полная сетка 128×128: словная формула с asr (P:$0594) и лимитером
        bool exact = true;
        for (int k = 0; k < 128 && exact; ++k)
            for (int w = 0; w < 128 && exact; ++w)
            {
                const uint32_t bw = (uint32_t)k << 16, ww = (uint32_t)w << 16;
                const FilterRing r = computeRing(bw, ww);
                const int idx = std::clamp((int)(((uint64_t)bw * 0x4AFull) >> 23), 0, 1200);
                const int widx = std::clamp((int)(((uint64_t)ww * 128ull) >> 23), 0, 127);
                const int32_t avgS = (sext24(kDiv1[(size_t)idx]) + sext24(kDiv2[(size_t)widx])) >> 1;
                const uint64_t D = div24(1ull << 24, avgS);
                const int32_t q0 = (int32_t)(D & 0xFFFFFFu);
                const int32_t difW = sext24((uint32_t)(kDiv2[(size_t)widx] - kDiv1[(size_t)idx]));
                int64_t b = (int64_t)sext24((uint32_t)q0) * (int64_t)difW * 2;
                b <<= 6;
                const uint32_t epsW = limit24(b);
                const int32_t c2s = sext24(kCoeff2[(size_t)idx]);
                int64_t A = (int64_t)-c2s * (int64_t)sext24(epsW) * 2;
                A -= (int64_t)c2s << 24;
                A >>= 1;
                const uint32_t y04 = limit24(A);
                if (fToQ23(r.c[0]) != y04 || fToQ23(r.c[1]) != epsW ||
                    fToQ23(r.c[2]) != kWidth1[(size_t)widx] ||
                    fToQ23(r.c[3]) != kWidth2[(size_t)widx])
                    exact = false;
            }
        check(exact, "сетка 128×128: словная формула P:$0581-059A бит-в-бит (asr + лимитер)");
    }
}

static void testCascade()
{
    std::printf("— каскад func_000340 (P:$0340-$0364):\n");
    Cascade340 c; c.reset();
    const FilterRing ring = computeRing(0x400000u, 0x400000u);
    std::array<float, kFrame> in{}, out{};
    in.fill(0.0f);
    c.processFrame(in.data(), out.data(), ring.c);
    bool silent = true;
    for (float v : out) if (std::abs(v) > 1e-6f) silent = false;
    check(silent, "нулев вход → нулевой выход");
    in[0] = 1.0f;
    c.processFrame(in.data(), out.data(), ring.c);
    bool finite = true; float peak = 0;
    for (float v : out) { if (!std::isfinite(v)) finite = false; peak = std::max(peak, std::abs(v)); }
    check(finite, "импульс → конечный выход");
    char buf[64]; std::snprintf(buf, 64, "(peak=%.4f)", peak);
    check(peak > 0.01f && peak < 8.0f, "импульс виден на выходе", buf);
    bool stable = true;
    for (int f = 0; f < 200 && stable; ++f)
    {
        for (int i = 0; i < kFrame; ++i) in[i] = (float)((i * 37 + f * 91) % 97) / 97.0f - 0.5f;
        c.processFrame(in.data(), out.data(), ring.c);
        for (float v : out) if (!std::isfinite(v) || std::abs(v) > 4.0f) { stable = false; break; }
    }
    check(stable, "200 кадров шума — стабильно, |out| < 4");
}

static void testVoice()
{
    std::printf("— голос целиком (кадр 16, тракт FLT→DIST→AHDR→env2):\n");
    MnmFltDistVoice v;
    v.reset();
    v.setFiltWords(0u, 127u << 16, 0u, 0u, 8u << 16, 40u << 16,
                   64u << 16, 64u << 16);           // BASE=0, WDTH=127 — открытый THRU
    v.setAmpWords(0u, 0u, 64u << 16, 0u);           // REL=0 — быстрое закрытие
    v.setEnv2Words(0u, 64u << 16, 127u << 16, 0u);  // гейт открыт, быстрый REL
    v.trigger();
    {
        bool silent = true;
        for (int i = 0; i < 16; ++i)
            if (v.processL(1.0f) != 0.0f) silent = false;
        check(silent, "первые 16 сэмплов — тишина (латентность 1 кадр)");
    }
    bool finite = true; float peak = 0;
    for (int i = 0; i < 44100; ++i)
    {
        const float s = 0.5f * (float)std::sin(2.0 * 3.14159265358979 * 220.0 * i / 44100.0);
        const float o = v.processL(s);
        if (!std::isfinite(o)) finite = false;
        peak = std::max(peak, std::abs(o));
    }
    check(finite, "44100 сэмплов — без NaN");
    char buf[64]; std::snprintf(buf, 64, "(peak=%.4f)", peak);
    check(peak > 0.05f && peak <= 1.0f, "сигнал проходит, |out| ≤ 1.0", buf);
    // крайние слова страницы FLT: HPQ/LPQ в тракт НЕ входят (их потребитель
    // — делей-машина), узкое кольцо/каскад (BASE=24, WDTH=20) не разносятся
    v.setFiltWords(24u << 16, 20u << 16, 127u << 16, 127u << 16,
                   0u, 64u << 16, 64u << 16, 64u << 16);
    finite = true; peak = 0;
    for (int i = 0; i < 88200; ++i)
    {
        const float s = 0.5f * (float)std::sin(2.0 * 3.14159265358979 * 110.0 * i / 44100.0);
        const float o = v.processR(s);
        if (!std::isfinite(o)) finite = false;
        peak = std::max(peak, std::abs(o));
    }
    check(finite, "крайние слова FLT (BASE=24, WDTH=20) — без разноса");
    check(peak <= 1.0f + 1e-4f, "клип ступени держит |out| ≤ 1.0");
    // note-off: AHDR (REL=0) + env2 (REL=0) закрывают тракт
    v.release();
    float tail = 0;
    for (int i = 0; i < 16 * 400; ++i) tail = v.processL(0.5f);
    char buf2[64]; std::snprintf(buf2, 64, "(tail=%.4f)", tail);
    check(tail < 0.05f, "note-off → гейты закрывают тракт", buf2);
}

// ---------------------------------------------------------------------------
// mnm_18: пины ступени DIST $079F–$07D1 и тона-индекса $0537–$056C
// (оракул: эмулятор dsp_emu.py, work/exp72d_pins.json + work/exp72e_tone.json)
// ---------------------------------------------------------------------------
static void testDistStage()
{
    std::printf("— ступень DIST $079F–$07D1 (пины оракула exp72d, D = X:$40B = 8):\n");
    MnmDistStage st;
    MnmDistStage::In par;
    par.divWord = 8;                       // D = 8 → y1 = frac(8/8) = $FFFFFF
    par.wofsWord = 0;

    struct Case { int v; uint32_t x0, r2, kDrive, curveW, K1; };
    // D = 8 → y1 = frac(8/8) = $FFFFFF (знаково −1) → K1 = limit24(−2·kDrive)
    // = $FFFFFF (|−2·kDrive| < 2^24 → B1 = $FFFFFF) — пины банков exp72d
    // (p1[0] = $FFFFFF при любом v). curve[0] = $7FFFFF (v ≤ 63 → x0 = 0).
    const Case cases[] = {
        { -64, 0x000000u, 0x000000u, 0x021333u, 0x7FFFFFu, 0xFFFFFFu },
        {   0, 0x000000u, 0x000000u, 0x021333u, 0x7FFFFFu, 0xFFFFFFu },
        {  32, 0x000000u, 0x000000u, 0x021333u, 0x7FFFFFu, 0xFFFFFFu },
        { 127, 0x7E0000u, 0x0000FCu, 0x7C1878u, 0x2D413Du, 0xFFFFFFu },
    };
    for (const Case& c : cases)
    {
        MnmDistStage::Out o;
        par.distWord = (uint32_t)(c.v << 16) & 0xFFFFFFu;
        st.frame({}, {}, par, o);
        char buf[96];
        std::snprintf(buf, 96, "v=%d x0=%06X r2=%04X kD=%06X cur=%06X K1=%06X",
                      c.v, o.x0, o.r2, o.kDrive, o.curveW, o.K1);
        const bool ok = o.x0 == c.x0 && o.r2 == c.r2 && o.kDrive == c.kDrive &&
                        o.curveW == c.curveW && o.K1 == c.K1 && o.y1 == 0xFFFFFFu;
        check(ok, "пины закона диста (x0 = max(0,2w−1), пила, kDrive, K1)", buf);
    }
    // K2 = limit24(curve·D): v=127, curve = $2D413D, D = 8
    {
        MnmDistStage::Out o;
        par.distWord = (127u << 16) & 0xFFFFFFu;
        st.frame({}, {}, par, o);
        char buf[48];
        std::snprintf(buf, 48, "K2=%06X (curve·8, пин оракула)", o.K2);
        check(o.K2 == 0x000002u, "K2 = limit24(curve·D) — пин exp72d", buf);
    }
    // Конвейер банков: синтетическая рампа (оракул exp72d bank_pins, v=32)
    {
        std::array<uint32_t, MnmDistStage::kBank> inA{}, inB{};
        for (int i = 0; i < MnmDistStage::kBank; ++i)
        {
            const int32_t w = (int32_t)(0.9 * (1.0 - 2.0 * i / 33.0) * 8388608.0);
            inA[(size_t)i] = (uint32_t)w & 0xFFFFFFu;
            inB[(size_t)i] = inA[(size_t)i];
        }
        MnmDistStage::Out o;
        par.distWord = (32u << 16) & 0xFFFFFFu;
        par.divWord = 8;
        st.frame(inA, inB, par, o);
        // оракул exp72d (v=32, K1 = $FFFFFF): после pass2 шапки банков =
        // limit24(хвост pass1) = $0000E6 в ОБОИХ банках (acc-переток);
        // (окно p2_X72 в exp72d-капчере смещено на 2 — банк B пишется
        //  с X:$70, капчер читал с X:$72; выверено по указателям r0/r1)
        const bool okHead = o.bankA[0] == 0x0000E6u && o.bankB[0] == 0x0000E6u;
        char buf[64];
        std::snprintf(buf, 64, "p2[0]=%06X p2b[0]=%06X", o.bankA[0], o.bankB[0]);
        check(okHead, "конвейер банков pass1→pass2 (acc-переток) — пин exp72d", buf);
    }
    // Стоковая дыра: divWord = 0 → y1 = $FFFFFF, K1-acc = −2·kDrive → K1 = $FFFFFF
    {
        MnmDistStage::Out o;
        par.distWord = (127u << 16) & 0xFFFFFFu;
        par.divWord = 0;
        st.frame({}, {}, par, o);
        check(o.y1 == 0xFFFFFFu && o.K1 == 0xFFFFFFu,
              "дыра X:$40B: 8/0 → y1=$FFFFFF (знак. −1) → K1 = $FFFFFF (−ε)");
    }
    // Фактор среза $07D2–$07DB: clamp(Y:$4DA, $6A3) → kCutoff
    {
        MnmDistStage::Out o;
        par.distWord = (127u << 16) & 0xFFFFFFu;
        par.divWord = 8;
        st.frame({}, {}, par, o);
        // Y:$4DA = 0 → c = 0 → kCut = limit24(K2·kCutoff[0])
        const uint32_t kCut0 = MnmDistStage::cutoffFactor(o, 0);
        const int64_t expect = mpy56(sext24(o.K2), sext24(kCutoff[0]));
        check(kCut0 == limit24(expect), "фактор среза $07DB = limit24(K2·kCutoff[c])");
    }
}

static void testToneIndex()
{
    std::printf("— индекс тона $0537–$056C (пины оракула exp72e):\n");
    MnmToneIndex::In tin;
    tin.param4 = 0; tin.w40E = 0; tin.w40F = 0;
    tin.hpqWord = 0; tin.x401 = 0; tin.bits425 = 0;

    // level 0.5 (панч кадр 1) → X:$4D9 = FFFC00 (−1024), Y:$4DA = 0 — пин пака 12
    {
        tin.levelWord = 0x400000u;
        const auto o = MnmToneIndex::compute(tin);
        check(o.toneIdx == 0xFFFC00u && o.wofsWord == 0x000000u,
              "level=0.5: idx = FFFC00 (−1024, замер пака 12), Y:$4DA = 0");
    }
    // level слово $800000 (−1.0 — знаковое!) → idx = $00067F, Y:$4DA = $000D7F
    {
        tin.levelWord = 0x800000u;
        const auto o = MnmToneIndex::compute(tin);
        check(o.toneIdx == 0x00067Fu && o.wofsWord == 0x000D7Fu,
              "level=$800000 (знак. −1): idx = 00067F, Y:$4DA = 000D7F (WOFS-ветка)");
    }
    // param4 = 0.25 → idx = FFFE00 (−512) — пин exp72e
    {
        tin.levelWord = 0x400000u;
        tin.param4 = 0x200000u;
        const auto o = MnmToneIndex::compute(tin);
        check(o.toneIdx == 0xFFFE00u, "param4=0.25: idx = FFFE00 (−512)");
    }
    // Гейн EQ $0857: clamp(idx, 0, $63F), g = $7FFFFF − kCutoff[128+idx]
    {
        const uint32_t g0 = MnmToneIndex::eqGain(0xFFFC00u);  // −1024 → clamp 0
        check(g0 == 0x7FFFFFu - kCutoff[128], "eqGain(idx<0) = 1 − tbl[0]");
        const uint32_t gM = MnmToneIndex::eqGain(0x700000u);  // > $63F → clamp
        check(gM == 0x7FFFFFu - kCutoff[128 + 0x63F], "eqGain(idx>$63F) = 1 − tbl[$63F]");
    }
    // Биты $425: bit11 → X:$4D9 = B1(A + 2048·X:$401·2) — при X:$401 = 3
    // добавка (+48) сидит в A0 (дробь acc), B1 НЕ меняется — пин оракула
    // (exp72-прогон 2026-10-04: X4D9 = FFFC00, Y4DA = 000000)
    {
        tin.param4 = 0; tin.levelWord = 0x400000u;
        tin.x401 = 3; tin.bits425 = 0x800u;   // bit11
        const auto o = MnmToneIndex::compute(tin);
        check(o.toneIdx == 0xFFFC00u && o.wofsWord == 0x000000u,
              "bit11($425)+X:$401=3: добавка в A0, B1 = FFFC00 (пин оракула)");
    }
}

// ---------------------------------------------------------------------------
// mnm_19: стерео-кадр (pushStereo, один счётчик на пару L/R) + проведение
// слова DIST Y:$404 в ступень $079F–$07D1 (критика импорта Nova, 2026-10-04)
// ---------------------------------------------------------------------------
static void testStereoAndDistWord()
{
    std::printf("— стерео-кадр pushStereo и слово DIST (фиксы mnm_19):\n");

    // 1) pushStereo: кадр без дыр — при DC-входе выход в установившемся
    //    режиме ПОСТОЯНЕН от кадра к кадру (дыры в буфере дали бы пилообразную
    //    рябь). Стадия 2 асимметрична по дизайну прошивки (банки a/b с
    //    раздельным стейтом) — outL == outR НЕ ожидается.
    {
        MnmFltDistVoice v;
        v.reset();
        v.setFiltWords(0u, 127u << 16, 0u, 0u, 8u << 16, 40u << 16,
                       64u << 16, 64u << 16);
        v.trigger();
        // прогрев 100 кадров DC
        for (int i = 0; i < 100 * 16; ++i)
        {
            float ol, or_;
            v.pushStereo(0.25f, 0.25f, &ol, &or_);
        }
        bool steady = true, finite = true;
        float pl = 0.f, pr = 0.f;
        for (int i = 0; i < 100 * 16; ++i)
        {
            float ol, or_;
            v.pushStereo(0.25f, 0.25f, &ol, &or_);
            if (i > 0 && (ol != pl || or_ != pr)) steady = false;
            pl = ol; pr = or_;
            if (!std::isfinite(ol) || !std::isfinite(or_)) finite = false;
        }
        check(steady, "pushStereo DC → установившийся режим покадрово постоянен (кадр без дыр)");
        check(finite, "pushStereo — без NaN");
    }

    // 2) канал-в-канал: стерео-L должен совпасть с моно-трактом на том же
    //    сигнале (заполнение L-буфера кадра не сбивается R-вызовами)
    {
        MnmFltDistVoice vs, vm;
        vs.reset(); vm.reset();
        const uint32_t flt[8] = { 24u << 16, 20u << 16, 0u, 0u,
                                  8u << 16, 40u << 16, 64u << 16, 64u << 16 };
        vs.setFiltWords(flt[0], flt[1], flt[2], flt[3], flt[4], flt[5], flt[6], flt[7]);
        vm.setFiltWords(flt[0], flt[1], flt[2], flt[3], flt[4], flt[5], flt[6], flt[7]);
        vs.trigger(); vm.trigger();
        bool match = true;
        for (int i = 0; i < 22050; ++i)
        {
            const float s = 0.5f * (float)std::sin(2.0 * 3.14159265358979 * 197.0 * i / 44100.0);
            float ol = 0.f, or_ = 0.f;
            vs.pushStereo(s, -s, &ol, &or_);
            if (ol != vm.processL(s)) match = false;
        }
        check(match, "pushStereo(L) == processL(L) канал-в-канал (та же прошивка кадра)");
    }

    // 3) слово ручки DIST Y:$404 доходит до ступени: v = 127 → x0 = $7E0000
    //    (пин оракула exp72d); делитель X:$40B (частное DIV — формат A0,
    //    в знаковом mpy читается как signed Q23 — квирк железа):
    //    D = 32 → y1 = $400000 (0.5) → K1 = kDrive/2;
    //    D = 16 → y1 = $800000 = −1.0 знаковое → K1 = −kDrive
    //    (закон реплики div24+mpy56, см. шапку mnm_dist_stage.h, mnm_19)
    {
        MnmFltDistVoice v;
        v.reset();
        v.setFiltWords(0u, 127u << 16, 0u, 0u, 0u, 64u << 16, 64u << 16, 64u << 16);
        v.setDistWord(127u << 16);                       // Y:$404 = $7F0000
        for (int i = 0; i < 3 * 16; ++i) v.processL(0.0f);
        check(v.distOutRaw().x0 == 0x7E0000u,
              "setDistWord(127<<16) → x0 = 7E0000 (пин exp72d, слово проведено)");
        check(v.distWordRaw() == 0x7F0000u, "distWordRaw() == $7F0000");

        v.setDistDivWord(32u);                           // X:$40B = 32 (сток: 0/дыра)
        for (int i = 0; i < 16; ++i) v.processL(0.0f);
        check(v.distOutRaw().y1 == 0x400000u, "divWord=32 → y1 = $400000 (частное DIV, формат A0)");
        check(v.distOutRaw().K1 == (v.distOutRaw().kDrive >> 1),
              "divWord=32 → K1 = kDrive/2 (знаковый mpy Q23·Q23)");

        v.setDistDivWord(16u);
        for (int i = 0; i < 16; ++i) v.processL(0.0f);
        check(v.distOutRaw().y1 == 0x800000u,
              "divWord=16 → y1 = $800000 (−1.0 знаковое — квирк формата A0)");
        check(v.distOutRaw().K1 == (uint32_t)((-v.distOutRaw().kDrive) & 0xFFFFFFu),
              "divWord=16 → K1 = −kDrive (инверсный гейн)");
    }
}


int main()
{
    std::printf("=== MnmFltDist: самопроверка (карта паков 12-14, mnm_19) ===\n");
    testTables();
    testDiv24();
    testEnvTerm();
    testPunch();
    testAhdr();
    testEnv2();
    testRing();
    testCascade();
    testDistStage();
    testToneIndex();
    testVoice();
    testStereoAndDistWord();
    std::printf("=== %s (%d провалов) ===\n", gFail ? "ПРОВАЛ" : "ВСЕ ПРОВЕРКИ ПРОЙДЕНЫ", gFail);
    return gFail ? 1 : 0;
}
