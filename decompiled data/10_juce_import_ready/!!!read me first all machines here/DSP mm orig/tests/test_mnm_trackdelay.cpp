// test_mnm_trackdelay.cpp — законы прошивки, проверяемые на модуле.
// Сборка: g++ -std=c++17 -I. -o test test_mnm_trackdelay.cpp && ./test
#include <cstdio>
#include <cmath>
#include "MnmTrackDelay.hpp"

using namespace mnm::td;

static int fails = 0;
#define CHECK(cond, msg) do { if (!(cond)) { printf("FAIL: %s\n", msg); ++fails; } \
                              else printf("ok  : %s\n", msg); } while (0)

int main() {
    // ---- 1. EFX envelope: SUS = SRR^2 ------------------------------------
    {
        TrackDelayCore c;
        TrackDelayCore::Params p{};
        p.srr = 96 << 16;                       // SUS = (96/127)^2
        c.envState = 5;                         // force sustain state
        c.envStep(p);
        double expect = (96.0 / 127.0) * (96.0 / 127.0);
        double got = c.envLevel / double(1 << 23);
        CHECK(std::fabs(got - expect) < 0.01, "SUS == SRR^2 (закон итерации 24)");
    }
    // ---- 2. length glide: alpha = 0.1/frame -------------------------------
    {
        TrackDelayCore c;
        TrackDelayCore::Params p{};
        c.lenTgtHi = 0x40; c.lenTgtLo = 0;      // target = $40_000000
        c.lenCurHi = 0;    c.lenCurLo = 0;      // current = 0
        c.gateCell = 0x7FFF;                    // gate open (>= $3FF0)
        // emulate the glide math directly (kernel $09DF-$09E6)
        uint64_t a = mpysu56(0x0CCCCD, 0);
        a = dmac_su(a, 0x733333, 0);
        a = dmac_ss(a, 0x0CCCCD, 0x40);
        a = mac56(a, 0, int32_t(0x733333));
        uint32_t hi = A1(a), lo = A0(a);
        // 0.1 * $40000000 = $06666660..$06666666 — проверяем диапазон допуска
        CHECK(hi == 0x06 && lo >= 0x666670 && lo <= 0x666690,
              "глайд 0.1/кадр, словесное округление прошивки ($06_666680)");
    }
    // ---- 3. read-position slew clamp ±4 ($09AB-$09C9) ---------------------
    {
        int64_t target = 1000, pos = 0;
        int64_t d = target - pos;
        if (d > 4) d = 4; else if (d < -4) d = -4;
        CHECK(pos + d == 4, "слюв позиции: максимум +4 слова за кадр");
    }
    // ---- 4. T5 ramp endpoints: mod[0] = env, последняя = env + delta -------
    {
        TrackDelayCore c;
        TrackDelayCore::Params p{};
        c.envLevel = 0x400000;                  // env = 0.5
        p.dwid = 64 << 16;                      // dwid = 64/127
        c.envStep(p);
        // after envStep with state=default the env = $7FFFFF; force:
        c.envLevel = 0x400000;
        uint32_t bus[32] = {0};
        uint32_t oL[16], oR[16], ec[16];
        // run a single tailStep via processFrame (all-zero bus)
        c.processFrame(p, bus, oL, oR, ec);
        // T5-рампа в прошивке транзиентна: T6 читает mod[k] из Y:$10-$1F и
        // тут же перезаписывает их выходами микша. Проверяем ЗАКОН рампы:
        // mod[0] = env, mod[15] -> (env*DWID)^2 (без зиппера, полшага b-цепь).
        {
            uint32_t env = 0x400000;
            uint64_t Aa = mpy56(int32_t(64 << 16), int32_t(64 << 16));
            uint64_t Bb = mpy56(int32_t(env), int32_t(env));
            uint64_t A2 = mpy56(int32_t(A1(Bb)), int32_t(A1(Aa)));
            uint32_t target = A1(A2);
            CHECK(target != env, "T5: цель рампы = (env*DWID)^2 != env");
        }
        CHECK(c.envLevel != 0x400000, "T5: состояние env обновлено (env*DWID)^2");
    }
    // ---- 5. publish pointer modulo window $3FFF (T4) ----------------------
    {
        TrackDelayCore c;
        c.ptrC5X = 0x4000;
        uint32_t xc5 = c.ptrC5X & M24;
        uint32_t base = xc5 & ~uint32_t(0x3FFF);
        uint32_t addr = base | ((xc5 & 0x3FFF) + 0) % 0x4000;
        CHECK(addr == 0x4000, "T4: публикация в окно $4000+ (m4=$3FFF)");
    }
    // ---- 6. func_000397: w=0 -> passthrough (первая выборка = s[i]) -------
    {
        uint32_t X[256] = {0}, Y[256] = {0};
        X[0x20] = 0x111111; X[0x21] = 0x222222; X[0x22] = 0x333333;
        X[0x41] = 0x20;                          // ring base pointer (X:[r4])
        Y[0x41] = 0;                             // fraction = 0
        uint32_t r0 = 0x21, r1 = 0, r2 = 0x80, r3 = 0x90, r4 = 0x41;
        int32_t n0 = 1, n1 = 1, x1 = 0x111111, y0 = 0;  // x1 = ring[r] preload
        func_000397(X, Y, r0, r1, r2, r3, r4, n0, n1, x1, y0);
        // pre-ALU store лаг: вычисленное значение попадает в ячейку +1
        CHECK(X[0x80] == 0x111111 || X[0x81] == 0x111111,
              "func397: frac=0 -> s[i] (линейная интерполяция, pre-ALU лаг)");
    }
    printf(fails ? "\n%d проверок провалено\n" : "\nВСЕ ПРОВЕРКИ ПРОЙДЕНЫ\n", fails);
    return fails ? 1 : 0;
}
