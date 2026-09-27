// test_mnm_flanger.cpp — автономная проверка FlaCore (вне сборки плагина).
// Сборка: g++ -std=c++17 -I<path to DSP_New> test_mnm_flanger.cpp -o test && ./test
// Проверки: тишина при INP=0, ненулевой выход при INP>0, границы MIX,
// устойчивость (нет разрыва на 1e6 сэмплах), детерминизм reset.
#include <cmath>
#include <cstdio>
#include <vector>
#include "flanger/MnmFlaExact.hpp"

static int failures = 0;
static void check(bool ok, const char* name) {
    std::printf("%-42s %s\n", name, ok ? "OK" : "FAIL");
    if (!ok) ++failures;
}

int main() {
    monomachine::mnm::FlaCore core;
    const int N = 16;

    // INP=0 -> тишина (после фейд-ина)
    core.reset(44100.0);
    int p0[8] = {64, 64, 64, 64, 64, 64, 0, 0};
    core.setParameters(p0);
    bool silent = true;
    for (int b = 0; b < 200; ++b) {
        std::vector<float> inL(N, 0.5f), inR(N, 0.5f), oL(N), oR(N);
        core.process(inL.data(), inR.data(), oL.data(), oR.data());
        if (b > 140)
            for (int i = 0; i < N; ++i)
                if (std::fabs(oL[i]) > 1e-9f || std::fabs(oR[i]) > 1e-9f) silent = false;
    }
    check(silent, "INP=0 -> silence (после фейд-ина)");

    // INP=64 -> ненулевой выход
    core.reset(44100.0);
    int p1[8] = {64, 64, 64, 64, 64, 64, 0, 64};
    core.setParameters(p1);
    float peak = 0.0f;
    for (int b = 0; b < 200; ++b) {
        std::vector<float> inL(N), inR(N), oL(N), oR(N);
        for (int i = 0; i < N; ++i) { inL[i] = 0.5f * std::sin(0.05f * (b * N + i)); inR[i] = inL[i]; }
        core.process(inL.data(), inR.data(), oL.data(), oR.data());
        for (int i = 0; i < N; ++i) peak = std::max(peak, std::fabs(oL[i]));
    }
    check(peak > 1e-3f, "INP=64 -> nonzero output");

    // MIX=127 -> выход ≈ dry (прошивочный кроссфейд)
    core.reset(44100.0);
    int p2[8] = {64, 64, 64, 127, 64, 64, 0, 64};
    core.setParameters(p2);
    double dryErr = 0.0;
    for (int b = 0; b < 200; ++b) {
        std::vector<float> inL(N), inR(N), oL(N), oR(N);
        for (int i = 0; i < N; ++i) { inL[i] = 0.3f * std::sin(0.11f * (b * N + i)); inR[i] = inL[i]; }
        core.process(inL.data(), inR.data(), oL.data(), oR.data());
        if (b > 150) for (int i = 0; i < N; ++i) dryErr += std::fabs(oL[i] - inL[i]);
    }
    check(dryErr / 800.0 < 0.02, "MIX=127 -> ~dry");

    // устойчивость: 1e6 сэмплов при FB=127 (максимум положительной обратной связи)
    core.reset(44100.0);
    int p3[8] = {64, 64, 64, 64, 127, 32, 0, 64};
    core.setParameters(p3);
    bool stable = true;
    float mx = 0.0f;
    for (int b = 0; b < 62500 && stable; ++b) {
        std::vector<float> inL(N), inR(N), oL(N), oR(N);
        for (int i = 0; i < N; ++i) { inL[i] = 0.4f * std::sin(0.037f * (b * N + i)); inR[i] = inL[i] * 0.8f; }
        core.process(inL.data(), inR.data(), oL.data(), oR.data());
        for (int i = 0; i < N; ++i) {
            mx = std::max(mx, std::fabs(oL[i]));
            if (!(std::fabs(oL[i]) < 8.0f) || std::isnan(oL[i])) { stable = false; break; }
        }
    }
    check(stable, "FB=127 -> stable over 1M samples");

    // детерминизм: reset -> одинаковый выход
    auto runonce = [&]() {
        core.reset(44100.0);
        core.setParameters(p1);
        std::vector<float> oL(N), oR(N), inL(N, 0.5f), inR(N, 0.5f);
        for (int b = 0; b < 10; ++b) core.process(inL.data(), inR.data(), oL.data(), oR.data());
        return oL[0];
    };
    float a = runonce(), b2 = runonce();
    check(a == b2, "reset -> deterministic");

    std::printf(failures ? "\n%d CHECKS FAILED\n" : "\nALL CHECKS PASSED\n", failures);
    return failures ? 1 : 0;
}
