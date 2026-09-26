// test_mnm_fm_exact.cpp — knob-law vectors measured on the bit-precise emulator
// (iteration 27). g++ -std=c++17 test_mnm_fm_exact.cpp && ./a.out
#include "mnm_fm_exact.hpp"
#include <cstdio>
#include <cmath>
static int fails = 0;
#define CHECK(cond, msg) do { if(!(cond)) { printf("FAIL: %s\n", msg); fails++; } } while(0)
int main() {
    using namespace mnm_fm;
    // ratio table: raw[7]=0x040000 -> 0.5 ; raw[23]=0x400000 -> 8.0
    CHECK(fabs(ratioOf(7) - 0.5f) < 1e-6f, "ratioOf(7)==0.5");
    CHECK(fabs(ratioOf(23) - 8.0f) < 1e-5f, "ratioOf(23)==8.0");
    CHECK(fabs(ratioOf(0) - 1.0f/32.0f) < 1e-6f, "ratioOf(0)==1/32");
    // index edges measured on the emulator: knobs 0..10 -> 0, 11..20 -> 1
    CHECK(ratioIndex(10) == 0, "ratioIndex(10)==0");
    CHECK(ratioIndex(11) == 1, "ratioIndex(11)==1");
    CHECK(ratioIndex(64) == 6, "ratioIndex(64)==6");
    CHECK(ratioIndex(127) == 11, "ratioIndex(127)==11");
    // fin multiplier
    CHECK(fabs(finMultiplier(0) - 0.75f) < 1e-5f, "fin(0)==0.75");
    CHECK(fabs(finMultiplier(127) - 1.24609375f) < 1e-4f, "fin(127)");
    // wrap level: measured mix_max/(0.826) at ENV=80 -> -0.4375*droop
    CHECK(fabs(wrapLevelSq(64) + 1.0f) < 1e-5f, "wrap(64)==-1.0 (sign flips already at 64!)");
    CHECK(fabs(wrapLevelSq(16) - 0.0625f) < 1e-4f, "wrap(16)==0.0625");
    CHECK(fabs(wrapLevelSq(80) + 0.4375f) < 1e-3f, "wrap(80)==-0.4375 (sign wrap)");
    CHECK(fabs(wrapLevelSq(127) + 0.0625f) < 1e-3f, "wrap(127)==-0.0625");
    // droop measured: 96 -> 0.99609, 127 -> 0.94132
    CHECK(fabs(envDroop(96) - 0.99609375f) < 1e-4f, "droop(96)");
    CHECK(fabs(envDroop(127) - 0.94140625f) < 1e-3f, "droop(127)");
    // shaper gain: 2VOL=80 -> G = 1.0
    CHECK(fabs(shaperGain(80) - 1.0f) < 1e-5f, "shaper(80)==1.0");
    CHECK(fabs(shaperGain(127) - 3.9375f) < 1e-3f, "shaper(127)");
    // fb: 1FB=32 -> 1024 table units
    CHECK(fabs(fbPhaseUnits(32) - 1024.0f) < 1e-3f, "fb(32)");
    // TUNE: 64 -> +200 cents (measured pitch shift 341/2048 oct)
    CHECK(fabs(tuneCents(64) - 200.0f) < 1.0f, "tune(64)==+200c");
    CHECK(tuneCents(0) == 0.0f, "tune(0)==0");
    // DYN ratio laws
    CHECK(fabs(dynRatio2(64) - 1.0f) < 1e-5f, "dynRatio2(64)==1");
    CHECK(fabs(dynRatio2(127) - 3.9375f) < 1e-3f, "dynRatio2(127)");
    CHECK(fabs(dynRatio1(64) - 1.0f) < 1e-5f, "dynRatio1(64)==1");
    // DYN 1VEN (iter-28): CONF-инициализация рекурсии $2C, не селектор шейперов
    CHECK(dynVenInit(64, 127) == 0.0f && dynVenInit(127, 127) == 0.0f, "venInit >= 64 == 0");
    CHECK(fabs(dynVenInit(63, 127) - (-4.0f * (1.0f / 128.0f) * (127.0f / 128.0f))) < 1e-4f, "venInit(63)");
    CHECK(dynVenInit(0, 0) == 0.0f, "venInit vol=0");
    CHECK(fabs(dynVenInit(0, 127) - (4.0f * (-64.0f / 128.0f) * (127.0f / 128.0f))) < 1e-4f, "venInit(0,127)");
    const std::array<int, 8> zp{0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(fabs(dynLaws(zp).lpCoeff - (64.0f / 8388608.0f)) < 1e-12f, "DYN fixed LP 64/2^23");
    printf(fails ? "%d FAILS\n" : "all tests passed\n", fails);
    return fails != 0;
}
