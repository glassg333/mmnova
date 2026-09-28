// test_amp_env.cpp — PACK 8: точная AMP-энвелопа трека (MnmKernel.hpp ->
// EnvExactCore). Сверки с измеренными фактами итерации 13 (06_amp_env/
// README_amp_envelope.md, amp_env_sweep.json) — БЕЗ гаданий:
//   1) атака: ATK=64 -> wrap через ~419 кадров (измерено 419, предсказание 420);
//      шаг tblA[32] = 0x46A98/2^23 за кадр (снят с эмулятора бит-в-бит);
//   2) wrap атаки: levelSigned < 0 сразу после wrap (24-битный wrap в -1.0);
//   3) карта HOLD (TEMPO=120): HOLD=4->6, 16->29, 32->57, 64->114 кадров
//      (допуск +-2 кадра, как в документе итерации 13);
//   4) DEC=127 (tblB=-1.0) — уровень заморожен;
//   5) первый шаг DEC из -1.0: level = -level*tblB -> отрицательный (|level| падает);
//   6) ретриг не сбрасывает уровень (дип-к-тишине);
//   7) гейн в [0, 1+eps] каждый сэмпл (де-зиппер не даёт выбросов);
//   8) nova-путь: mnm::AmpEnvelope::setExact + tick() == EnvExactCore::process();
//   9) релизный хвост достигает -96 дБ -> isDone (хост перестаёт рендерить).
// Exit code 0 только при 100% прохождении.
#include "MnmKernel.hpp"
#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace monomachine;
using Env = mnm::EnvExactCore;

static int fails = 0;
static void check(bool ok, const char* what) {
    printf("   [%s] %s\n", ok ? "OK" : "FAIL", what);
    if (!ok) ++fails;
}

static int framesToDec(int hold, int tempo) {
    Env e; e.reset();
    e.setParameters(0.f, (float)hold, 90.f, 40.f, (float)tempo);
    int frames = 0;
    while (e.phase() != Env::kDec) { e.tickFrame(); if (++frames > 100000) return -1; }
    return frames; // кадры до DEC (HOLD-фаза)
}

int main() {
    printf("== PACK 8 AMP env: exact state machine vs measured facts ==\n");

    // --- 1) attack increment bit-exact: tblA[32] = 0x46A98
    {
        const float expect = (float)0x46A98 / 8388608.0f;
        check(mnm::exact_tables::env::kEnvAtk[(size_t)32] == expect, "tblA[32] == 0x46A98/2^23");
    }

    // --- 2) ATK=64: wrap после ~419 кадров, level уходит в минус
    {
        Env e; e.reset();
        e.setParameters(64.f, 0.f, 90.f, 40.f, 120.f);
        int wrap = -1;
        for (int i = 1; i <= 2000; ++i) {
            e.tickFrame();
            if (e.phase() != Env::kAtk) { wrap = i; break; }
        }
        bool ok = wrap >= 418 && wrap <= 420 && e.levelSigned() < 0.0f;
        char buf[128]; snprintf(buf, sizeof buf, "ATK=64 wrap @ %d frames (419 measured), sign<0", wrap);
        check(ok, buf);
    }

    // --- 3) HOLD map (tempo 120), +-2 frames tolerance
    {
        const int want[4] = {6, 29, 57, 114};
        const int hs[4] = {4, 16, 32, 64};
        for (int i = 0; i < 4; ++i) {
            const int got = framesToDec(hs[i], 120);
            char buf[128];
            snprintf(buf, sizeof buf, "HOLD=%d @120: %d frames (measured %d, tol +-2)", hs[i], got, want[i]);
            check(got > 0 && std::abs(got - want[i]) <= 2, buf);
        }
    }

    // --- 4) DEC=127 freezes
    {
        Env e; e.reset();
        e.setParameters(0.f, 0.f, 127.f, 40.f, 120.f);
        for (int i = 0; i < 500; ++i) e.tickFrame();     // finish attack + wrap
        const float frozen = std::fabs(e.levelSigned());
        bool ok = frozen > 0.9f;
        for (int i = 0; i < 200 && ok; ++i) { e.tickFrame(); if (std::fabs(std::fabs(e.levelSigned()) - frozen) > 1e-6f) ok = false; }
        check(ok, "DEC=127 freezes |level| (tblB[127] = -1.0)");
    }

    // --- 5) first DEC step from wrap: level negative, |level| decays
    {
        Env e; e.reset();
        e.setParameters(0.f, 0.f, 90.f, 40.f, 120.f);
        for (int i = 0; i < 600; ++i) e.tickFrame();     // into DEC
        check(e.levelSigned() < 0.0f, "in DEC after wrap level is negative (sign law)");
        const float m1 = std::fabs(e.levelSigned());
        e.tickFrame();
        const float m2 = std::fabs(e.levelSigned());
        check(m2 < m1 && m2 > 0.5f, "|level| decays monotonically in DEC");
    }

    // --- 6) retrigger does NOT reset the level
    {
        Env e; e.reset();
        e.setParameters(0.f, 0.f, 90.f, 40.f, 120.f);
        for (int i = 0; i < 700; ++i) e.tickFrame();     // deep in DEC
        const float before = std::fabs(e.levelSigned());
        e.noteOn();                                       // retrigger
        check(e.phase() == Env::kAtk && std::fabs(e.levelSigned()) > 0.0f
              && std::fabs(e.levelSigned()) < before + 1e-6f,
              "retrig: phase ATK, level continues (dip-to-silence, not reset)");
    }

    // --- 7/8) nova path: mnm::AmpEnvelope == EnvExactCore, gain sane
    {
        Env ref; ref.reset();
        ref.setParameters(20.f, 0.f, 64.f, 64.f, 120.f);
        mnm::AmpEnvelope env;
        env.reset();
        env.setExact(20.f, 0.f, 64.f, 64.f, 120.f);
        env.trigger();
        bool same = true, sane = true;
        for (int i = 0; i < 48000 * 2; ++i) {             // 2 s
            const float a = env.tick();
            const float b = ref.process();
            if (std::fabs(a - b) > 1e-7f) { same = false; break; }
            if (a < -1e-6f || a > 1.0000001f) sane = false;
        }
        check(same, "nova mnm::AmpEnvelope::tick == EnvExactCore::process (2 s)");
        check(sane, "gain in [0, 1+eps] every sample (de-zipper, no overshoot)");
    }

    // --- 9) release floor: done after note-off tail, retrigger revives
    {
        mnm::AmpEnvelope env; env.reset();
        env.setExact(0.f, 0.f, 64.f, 40.f, 120.f);
        env.trigger();
        for (int i = 0; i < 48000; ++i) env.tick();
        env.release();
        int guard = 0;
        while (!env.isDone() && guard < 48000 * 30) { env.tick(); ++guard; }
        check(env.isDone(), "release tail reaches -96 dB floor -> isDone (host stops render)");
        env.trigger();
        check(!env.isDone(), "retrigger after floor revives the voice");
    }

    printf("== %s (%d failures) ==\n", fails ? "FAILED" : "ALL PASSED", fails);
    return fails ? 1 : 0;
}
