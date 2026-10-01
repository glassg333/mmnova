// test_juce_fix_smoke.cpp — smoke-проверка juce/MnmEnvLegacyFix.h (пак 11).
// Проверяем инварианты эталонных фиксов (без JUCE — хедер автономный):
//   * KILL-гейт: фаза 4 → множитель 0, иначе K/128
//   * атака с data-limiter: сатурация $7FFFFF (не -1.0), фаза HOLD
//   * decay idx: raw $20 → 1 (KILL), DEC=127 → 127 (×1.0 заморозка)
//   * T5-рампа: точная арифметика совпадает с MnmTail.hpp на контрольном наборе
// Сборка: g++ -O2 -std=c++17 -I juce -I dsp/mnm -o /tmp/fix_smoke tools/test_juce_fix_smoke.cpp
#include "MnmEnvLegacyFix.h"
#include "MnmTail.hpp"
#include <cstdio>
#include <cstdlib>

static int g_fail = 0;
#define CHECK(cond, msg) do { if (!(cond)) { printf("FAIL: %s\n", msg); ++g_fail; } \
                              else printf("ok: %s\n", msg); } while (0)

int main() {
    using namespace mnmfix;
    using namespace mnmtail;

    // ФИКС 1: KILL-гейт
    CHECK(stage2DecayMultiplier(4, 64 << 16) == 0, "KILL: фаза 4 -> множитель 0");
    CHECK(stage2DecayMultiplier(0, 64 << 16) == 64, "не-KILL: множитель K/128");

    // ФИКС 2: атака — инкремент SHAPE_A[0] = 0x400000 (+0.5/кадр)
    static const int32_t shapeA[128] = { 0x400000 };
    int level = 0x600000;
    int ph = ampAttackStep(level, 0, shapeA);          // 0x600000+0x400000 > 24 бит
    CHECK(level == 0x7FFFFF && ph == 1, "атака: сатурация $7FFFFF и фаза HOLD");
    level = 0x100000;
    ph = ampAttackStep(level, 0, shapeA);
    CHECK(level == 0x500000 && ph == 0, "атака: обычный кадр без перехода");

    // ФИКС 3: decay idx
    CHECK(decayIdxFromRateWord(0x20) == 1, "KILL: raw $20 -> idx 1");
    CHECK(decayIdxFromRateWord(127 << 16) == 127, "DEC=127 -> idx 127 (заморозка)");
    static const int32_t tblB[128] = { -7530345, -7583434 };
    CHECK(envDecayStep(0x400000, tblB, 1) ==
          int32_t((int64_t(-0x400000) * -7583434) >> 23), "decay: арифметика >>23");

    // ФИКС 5: T5-рампа — байт-в-байт против MnmTail.hpp (доказан 8296/0)
    {
        uint32_t b7z[16] = { 0 };
        long long envv = 7919;
        for (int i = 0; i < 200; ++i) {                 // конечная сетка env
            uint32_t env = (uint32_t)(envv & 0xFFFFFF);
            long long dwv = 104729;
            for (int j = 0; j < 200; ++j) {             // конечная сетка dwid
                uint32_t dwid = (uint32_t)(dwv & 0xFFFFFF);
                int32_t e1 = (int32_t)env, e2 = (int32_t)env;
                int32_t r1[16], r2[16];
                dwidRamp16(e1, (int32_t)dwid, r1);
                // через MnmTail (T5): прогоняем model_tail с нулевыми входами,
                // меняя только YPFF/YP1F; b7 — нулевой (не-skip путь читает его в T4)
                TailIn0 in{}; in.YPFF = env; in.YP1F = dwid;
                TailB8 b8{}; TailResult res;
                model_tail(in, b7z, b8, res);
                for (int k = 0; k < 16; ++k) r2[k] = (int32_t)res.mod[k];
                bool same = (e1 == (int32_t)res.out.at("Y:P+FF"));
                for (int k = 0; k < 16 && same; ++k) same = (r1[k] == r2[k]);
                if (!same) { printf("FAIL: T5 mismatch env=%X dwid=%X\n", env, dwid);
                             ++g_fail; return 1; }
                dwv = dwv * 3 + 104729;
            }
            envv = envv * 2 + 7919;
        }
    }
    printf("ok: T5-рампа совпадает с MnmTail.hpp на сетке env/dwid (200x200)\n");

    printf(g_fail ? "SMOKE: FAIL (%d)\n" : "SMOKE: ALL OK (%d failures)\n", g_fail);
    return g_fail ? 1 : 0;
}
