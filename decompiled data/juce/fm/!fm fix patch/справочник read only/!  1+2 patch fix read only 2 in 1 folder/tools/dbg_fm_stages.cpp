// dbg_fm_stages.cpp — dump intermediate stages from the C++ transcription in
// the same JSON shape as exp62_fm_stage_dump.py, then diff.
//   usage: dbg_fm_stages 9|10
#include "MnmFmPar.hpp"
#include "MnmFmDyn.hpp"
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>

using namespace mnmfm;

static MnmFmPar* gpar = nullptr;
static MnmFmDyn* gdyn = nullptr;
static int gmaxStage = 99;

static int32_t s24of(uint32_t p) { return (int32_t)(p & 0x800000u ? p - (1u << 24) : p); }

static void hookFn(void*, int stage) {
    printf("{\n");
    auto rngx = [&](const char* k, int a0, int a1, bool sign) {
        printf("  \"%s\": [", k);
        for (int a = a0; a < a1; ++a) {
            uint32_t v = gpar ? gpar->mem.X[a] : gdyn->mem.X[a];
            uint32_t vy = gpar ? gpar->mem.Y[a] : gdyn->mem.Y[a];
            (void)vy;
            printf("%u%s", v, a + 1 < a1 ? ", " : "");
        }
        printf("],\n");
    };
    auto rngy = [&](const char* k, int a0, int a1, bool sign) {
        printf("  \"%s\": [", k);
        for (int a = a0; a < a1; ++a) {
            uint32_t v = gpar ? gpar->mem.Y[a] : gdyn->mem.Y[a];
            printf("%u%s", v, a + 1 < a1 ? ", " : "");
        }
        printf("],\n");
    };
    auto stv = [&](int off) -> int32_t {
        uint32_t v = gpar ? gpar->mem.st[off] : gdyn->mem.st[off];
        return (int32_t)v;   // raw pattern (emulator dumps raw too)
    };
    auto regv = [&](const char* k, int32_t v) {
        printf("  \"%s\": %d,\n", k, v);
    };
    if (gpar) {
        if (stage >= 1) { rngx("ring1_x", 0x20, 0x40, false); rngy("ring1_y", 0x20, 0x40, false);
            regv("st10", stv(0x10)); regv("st11", stv(0x11)); regv("st12", stv(0x12));
            regv("st13", stv(0x13)); regv("x5", (int32_t)gpar->mem.X[5]); regv("y5", (int32_t)gpar->mem.Y[5]); }
        if (stage >= 2) rngy("mod1_sine", 0x80, 0xA0, true);
        if (stage >= 3) { rngx("mod1_diff", 0x80, 0xA0, true); regv("st20", stv(0x20));
            rngy("mod1_diff_y", 0x80, 0xA0, true); }
        if (stage >= 4) { rngy("mod1_lp", 0x80, 0xA0, true); regv("st2c", stv(0x2C)); }
        if (stage >= 5) { rngy("mod1_sh", 0x80, 0xA0, true); regv("st2f", stv(0x2F)); }
        if (stage >= 6) { rngy("mod2_sh", 0xC0, 0xE0, true); rngy("mod3_sh", 0xE0, 0x100, true);
            regv("st2d", stv(0x2D)); regv("st30", stv(0x30)); regv("st2e", stv(0x2E));
            regv("st31", stv(0x31)); regv("st21", stv(0x21)); regv("st22", stv(0x22)); }
        if (stage >= 7) { regv("y0", (int32_t)gpar->y0); regv("y1", (int32_t)gpar->y1);
            regv("st32", stv(0x32)); regv("st33", stv(0x33)); regv("st34", stv(0x34)); }
        if (stage >= 8) rngx("mix12", 0x80, 0xA0, true);
        if (stage >= 9) rngx("mix", 0x80, 0xA0, true);
        if (stage >= 10) { rngx("mixlp", 0x80, 0xA0, true); regv("st2b", stv(0x2B)); }
        if (stage >= 11) { rngx("ringc_x", 0x20, 0x40, false); rngy("ringc_y", 0x20, 0x40, false);
            regv("st1c", stv(0x1C)); regv("st1d", stv(0x1D)); regv("st1e", stv(0x1E)); regv("st1f", stv(0x1F)); }
        if (stage >= 12) rngx("car", 0xE0, 0x100, true);
        if (stage >= 13) { rngy("rot1", 0x1E, 0x40, true);
            regv("st23", stv(0x23)); regv("st24", stv(0x24)); regv("st27", stv(0x27)); regv("st28", stv(0x28)); }
        if (stage >= 14) { rngx("rot2", 0x1E, 0x40, true);
            regv("st25", stv(0x25)); regv("st26", stv(0x26)); regv("st29", stv(0x29)); regv("st2a", stv(0x2A)); }
        if (stage >= 15) {
            // outputs were produced; re-derive by running proc is done by caller
        }
        printf("  \"_stage\": %d\n}\n", stage);
        if (stage >= gmaxStage) return;
        return;
    }
    if (gdyn) {
        if (stage >= 1) { rngx("ring1_x", 0x20, 0x40, false); rngy("ring1_y", 0x20, 0x40, false);
            regv("st10", stv(0x10)); regv("st11", stv(0x11)); regv("st12", stv(0x12));
            regv("st13", stv(0x13)); regv("x5", (int32_t)gdyn->mem.X[5]); regv("y5", (int32_t)gdyn->mem.Y[5]); }
        if (stage >= 2) rngy("mod1_sine", 0x80, 0xA0, true);
        if (stage >= 3) { rngx("mod1_diff", 0x80, 0xA0, true); regv("st20", stv(0x20));
            rngy("mod1_diff_y", 0x80, 0xA0, true); }
        if (stage >= 4) { rngy("mod1_lp", 0x80, 0xA0, true); regv("st2a", stv(0x2A)); regv("st2c", stv(0x2C)); }
        if (stage >= 5) { rngy("mod1_sh", 0x80, 0xA0, true); regv("st2f", stv(0x2F)); regv("st20b", stv(0x20)); }
        if (stage >= 6) { rngy("mod2_sh", 0xC0, 0xE0, true); rngy("mod3_sh", 0xE0, 0x100, true);
            regv("st2b", stv(0x2B)); regv("st1d", stv(0x1D)); regv("st2c", stv(0x2C)); regv("st2d", stv(0x2D));
            regv("st30", stv(0x30)); regv("st2e", stv(0x2E)); regv("st31", stv(0x31));
            regv("st21", stv(0x21)); regv("st22", stv(0x22)); }
        if (stage >= 7) { regv("y0", (int32_t)gdyn->y0); regv("y1", (int32_t)gdyn->y1);
            regv("st29", stv(0x29)); regv("st32", stv(0x32)); regv("st33", stv(0x33)); regv("st34", stv(0x34)); }
        if (stage >= 8) { rngx("mix12", 0x80, 0xA0, true); rngx("mix", 0x80, 0xA0, true); }
        if (stage >= 9) { rngx("mixlp", 0x80, 0xA0, true); regv("st29b", stv(0x29)); regv("st2b2", stv(0x2B)); }
        if (stage >= 10) { rngx("ringc_x", 0x20, 0x40, false); rngy("ringc_y", 0x20, 0x40, false);
            regv("st18", stv(0x18)); regv("st19", stv(0x19)); regv("st1a", stv(0x1A)); regv("st1b", stv(0x1B));
            regv("mixlp2", 0); regv("st2b", stv(0x2B));
            regv("st1c", stv(0x1C)); regv("st1d2", stv(0x1D)); regv("st1e", stv(0x1E)); regv("st1f", stv(0x1F)); }
        if (stage >= 11) rngx("car", 0xE0, 0x100, true);
        if (stage >= 12) { rngy("rot1", 0x1E, 0x40, true);
            regv("st21", stv(0x21)); regv("st22", stv(0x22)); regv("st25", stv(0x25)); regv("st26", stv(0x26));
            regv("st23", stv(0x23)); regv("st24", stv(0x24)); regv("st27", stv(0x27)); regv("st28", stv(0x28)); }
        if (stage >= 13) { rngx("rot2", 0x1E, 0x40, true);
            regv("st23", stv(0x23)); regv("st24", stv(0x24)); regv("st27", stv(0x27)); regv("st28", stv(0x28));
            regv("st25b", stv(0x25)); regv("st26b", stv(0x26)); regv("st29b", stv(0x29)); regv("st2ab", stv(0x2A)); }
        printf("  \"_stage\": %d\n}\n", stage);
        return;
    }
}

int main(int argc, char** argv) {
    int which = argc > 1 ? atoi(argv[1]) : 9;
    if (which == 9) {
        static MnmFmPar m;
        gpar = &m;
        m.stageHook = hookFn;
        m.init();
        uint32_t knob[8] = {64, 64, 64, 64, 64, 64, 64, 64};
        uint32_t out[32];
        m.conf(knob);
        m.proc(11776u, out);
        printf("[");
        for (int i = 0; i < 32; ++i) printf("%d%s", s24of(out[i]), i < 31 ? ", " : "");
        printf("]\n");
    } else {
        static MnmFmDyn m;
        gdyn = &m;
        m.stageHook = hookFn;
        m.init();
        uint32_t knob[8] = {64, 64, 64, 64, 64, 64, 64, 64};
        uint32_t out[32];
        m.conf(knob);
        m.proc(11776u, out);
        printf("[");
        for (int i = 0; i < 32; ++i) printf("%d%s", s24of(out[i]), i < 31 ? ", " : "");
        printf("]\n");
    }
    return 0;
}
