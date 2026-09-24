// Smoke test for mnm_dist.h against probe23d emulator vectors (iteration 23).
// Build: g++ -std=c++17 -I<path to Source> test_mnm_dist.cpp -o test_mnm_dist
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "mnm_dist.h"

static int fails = 0;
static void expect(bool ok, const char* what)
{
    if (! ok) { printf("FAIL: %s\n", what); ++fails; }
    else printf("ok: %s\n", what);
}

int main()
{
    using namespace mnm;

    // --- V1: attack steps bit-exact vs tblA (emulator probe23d) ---
    {
        MonoMachineDistDrive d;
        d.reset();
        MonoMachineDistDrive::Params p; p.dist = 64; p.vol = 127;
        const int32_t step = (int32_t) lrintf(kEnvAtk[64] * 8388608.0f);
        bool ok = true;
        for (int i = 0; i < 8; ++i)
            if (d.tickRaw(p) != step * (i + 1)) ok = false;
        expect(ok, "V1 DIST=64 attack steps == kEnvAtk[64]*n (0x4E05*n)");
    }
    // --- V2: DIST=0 wrap sequence (emulator: 0.5, -1.0, 0, 0) ---
    {
        MonoMachineDistDrive d; d.reset();
        MonoMachineDistDrive::Params p; p.dist = 0; p.vol = 127;
        const int32_t t1 = d.tickRaw(p);
        const int32_t t2 = d.tickRaw(p);
        const int32_t t3 = d.tickRaw(p);
        const int32_t t4 = d.tickRaw(p);
        expect(t1 == 0x400000, "V2a first tick = +0.5 (0x400000)");
        expect(t2 == -0x800000, "V2b wrap -> -1.0 (-0x800000)");
        expect(t3 == 0, "V2c DEC from -1.0 clamps to 0 (clr b ifmi)");
        expect(t4 == 0 && d.getPhase() == MonoMachineDistDrive::kDec,
               "V2d stays 0 in DEC phase");
    }
    // --- V3: attack-rate ladder vs measured frames-to-top (README_amp map) ---
    {
        // DIST=32: emulator showed 0.0345/frame -> ~29 frames to top
        MonoMachineDistDrive d; d.reset();
        MonoMachineDistDrive::Params p; p.dist = 32; p.vol = 127;
        int frames = 0;
        while (d.getPhase() == MonoMachineDistDrive::kAtk && frames < 200) { d.tickRaw(p); ++frames; }
        expect(frames >= 27 && frames <= 31, "V3 DIST=32 reaches top in ~29 frames");
    }
    // --- V4: DIST=127 attack is 0x65/frame (emulator) ---
    {
        MonoMachineDistDrive d; d.reset();
        MonoMachineDistDrive::Params p; p.dist = 127; p.vol = 127;
        const int32_t t1 = d.tickRaw(p);
        const int32_t t2 = d.tickRaw(p);
        expect(t1 == 0x65 && t2 == 0xCA, "V4 DIST=127 step = 0x65/frame");
    }
    // --- V5: fade-in contract of the slot ---
    {
        struct Nul : MonoMachineDistSlot { void process() noexcept override {} } n;
        n.arm();
        bool allZero = true;
        for (int i = 0; i < 64; ++i) { if (n.fade() != 0.0f) allZero = false; n.run(); }
        expect(allZero, "V5a first 64 frames gain = 0");
        n.run();
        expect(n.fade() == 1.0f, "V5b gain = 1.0 after 64 frames");
    }
    // --- V6: timbreIndex (a1-срезы, знак через wrap) ---
    {
        const int idx0 = timbreIndex(0, 0, 0);            // база = −$80, драйв 0 гасит знак
        expect(idx0 == -0x80, "V6a zero drive/param4/pan -> база −$80");
        const int idxc = timbreIndex(0x7FFFFF, 0x7FFFFF, 0); // база 0x3FFFFF800 ≥ $700
        expect(idxc == 0x700, "V6b большой param4 клампится в $700");
    }
    // --- V7: бит-точная сверка индекса с эмулятором (probe23d, DIST=64, кадр 70) ---
    {
        // drive = 1418083 (уровень кадра 70), param4 = 0, panTerm = 0:
        // |0+$C00000| → a1 = −0.5 → кв. 0.25 → asl#2 → a1 −1.0 →
        // v2 = −1418083 → v3 = −303 → акк = −303−128 = −431 = 0xFFFE51 (замер)
        const int idx = timbreIndex(1418083, 0, 0);
        expect(idx == -431, "V7 индекс = −431 ровно как в эмуляторе (0xFFFE51)");
    }

    printf(fails ? "\n%d TEST(S) FAILED\n" : "\nALL TESTS PASSED\n", fails);
    return fails ? 1 : 0;
}
