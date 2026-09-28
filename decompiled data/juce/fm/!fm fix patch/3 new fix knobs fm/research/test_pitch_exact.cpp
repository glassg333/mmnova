// test_pitch_exact.cpp — проверка питч-закона пака 7:
// 1) accForNote(n) == kFmGoldenAcc[n] бит-в-бит для всех 128 нот (тюн 440);
// 2) тюн 400 Гц: сверка с золотыми acc ОС (допуск 2 цента — пол LUT ОС);
// 3) кламп низа (тюн 400, низкие ноты -> kFmAccFloor, бит-в-бит);
// 4) смоук рендера всех 3 машин на ноте 69.
#include <cstdio>
#include <cstdint>
#include <cmath>
#include "MnmFm.hpp"

using monomachine::mnm::FmCore;
using monomachine::mnm::FmKind;

// золотые acc с ОС при тюне 400 Гц (полный захват, 128 нот)
static const int64_t kGolden400[] = {
#include "golden400.inc"
};

static double centsErr(int64_t got, int64_t want) {
    if (got <= 0 || want <= 0) return 1e9;
    return 1200.0 * std::log2((double)got / (double)want);
}

int main() {
    int fails = 0;
    // --- 1: бит-в-бит при тюне 440
    {
        FmCore c; c.reset(44100.0);
        int bad = 0;
        for (int n = 0; n < 128; ++n) {
            const int64_t got = c.accForNote((double)n);
            if (got != monomachine::mnm::kFmGoldenAcc[n]) {
                if (bad < 8) std::printf("FAIL n%d: got %lld want %lld\n", n, (long long)got, (long long)monomachine::mnm::kFmGoldenAcc[n]);
                ++bad;
            }
        }
        std::printf("тюн 440, 128 нот: %s\n", bad ? "ЕСТЬ РАСХОЖДЕНИЯ" : "128/128 БИТ-В-БИТ");
        fails += bad;
    }
    // --- 2: тюн 400 (допуск 2 цента на пол LUT ОС)
    {
        FmCore c; c.reset(44100.0);
        c.setMasterTuneHz(400.0);
        int bad = 0; double worst = 0;
        for (int n = 0; n < 128; ++n) {
            const int64_t got = c.accForNote((double)n);
            const double ce = centsErr(got, kGolden400[n]);
            worst = (ce > worst && ce < 1e8) ? ce : worst;
            if (ce > 2.0) { ++bad; if (bad < 6) std::printf("FAIL 400 n%d: got %lld want %lld (%.2f ц)\n", n, (long long)got, (long long)kGolden400[n], ce); }
        }
        std::printf("тюн 400: %s (худш ошибка %.3f цента)\n", bad ? "ЕСТЬ" : "OK", worst);
        fails += bad;
    }
    // --- 3: кламп низа бит-в-бит
    {
        FmCore c; c.reset(44100.0);
        c.setMasterTuneHz(400.0);
        int bad = 0;
        for (int n = 0; n <= 30; ++n) {
            const int64_t got = c.accForNote((double)n);
            if (got != monomachine::mnm::kFmAccFloor) { ++bad; if (bad < 4) std::printf("FAIL clamp n%d: got %lld want floor %lld\n", n, (long long)got, (long long)monomachine::mnm::kFmAccFloor); }
        }
        std::printf("кламп низа (тюн 400, ноты 0..30): %s\n", bad ? "ЕСТЬ" : "OK");
        fails += bad;
    }
    // --- 4: смоук рендера
    {
        FmCore c; c.reset(44100.0);
        const FmKind kinds[3] = { FmKind::Stat, FmKind::Par, FmKind::Dyn };
        const char* names[3] = { "STAT", "PAR", "DYN" };
        for (int k = 0; k < 3; ++k) {
            c.noteOn(69.0f);
            c.setParameters(kinds[k], {64,64,64,64,64,64,64,64});
            float buf[512];
            double energy = 0.0;
            for (int b = 0; b < 8; ++b) { c.processBlock(buf, 512); for (float v : buf) energy += v*v; }
            std::printf("%s: энергия = %.3e (%s)\n", names[k], energy, energy > 1e-6 ? "рендерит" : "тишина!");
            if (energy <= 1e-6) ++fails;
        }
    }
    std::printf(fails ? "ИТОГ: %d FAIL\n" : "ИТОГ: ВСЕ OK\n", fails);
    return fails ? 1 : 0;
}
