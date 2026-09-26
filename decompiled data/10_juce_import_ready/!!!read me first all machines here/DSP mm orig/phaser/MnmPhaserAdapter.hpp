// MnmPhaserAdapter.hpp — режим "ex" для FX-PHASER (m18) в Monomachine Nova.
// Обёртка над mnm_phaser.h — бит-в-бит портом машины m18 (DSP56300 PROC
// $145045-$14513F), верифицированным слово-в-слово на эмуляторе:
//   grid 8 конфигов x 5 сигналов x 30 блоков -> 38400/38400 слов OK
//   stress 8 случайных конфигов x 60 блоков  -> 15360/15360 слов OK
// (см. tests/test_mnm_phaser.cpp + tests/test_vectors.h).
//
// Слоты параметров (05_descriptors/fx_descriptors_region.bin, блок PHASER):
//   p0=CNTR, p1=DEP, p2=SPD, p3=MIX, p4=FB, p5=WID, p6=---, p7=INP.
// Вход/выход ядра: 32 слова, interleaved (in[2j]=L, in[2j+1]=R; out так же).
// INP² ×4 и MIX применяются ВНУТРИ ядра (прошивочные законы), поэтому
// адаптер не делает никакой внешней обработки уровня.
//
// Латентность: обработка идёт 16-сэмпловыми блоками (как на DSP), поэтому
// выход отстаёт от входа до 15 сэмплов; reportLatency наружу не выставляется
// (у старого generic-фейзера её тоже нет — A/B-сравнение честное).
#pragma once

#include "mnm_phaser.h"
#include "MnmPhaserExactTables.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace monomachine {

class MnmPhaserExact {
public:
    void reset() noexcept {
        st = mnm::PhaserState{};
        fill = 0;
        inArr.fill(0);
    }

    // p[0..7] — значения ручек 0..127 (synth page A..H)
    void setParameters(const float* p) noexcept {
        for (int i = 0; i < 8; ++i) {
            const int k = std::clamp(int(std::lround(i == 6 ? 0.0f : p[i])), 0, 127);
            prm[static_cast<size_t>(i)] = k << 16;   // raw v<<16, как пишет ColdFire
        }
    }

    void processStereo(float* l, float* r, std::size_t n) noexcept {
        for (std::size_t i = 0; i < n; ++i) {
            inArr[static_cast<size_t>(2 * fill)] = to24(l[i]);
            inArr[static_cast<size_t>(2 * fill + 1)] = to24(r[i]);
            ++fill;
            if (fill == 16) {
                int32_t out[32];
                mnm::phaser_block(st, {mnmpha::kSinTab, mnmpha::kCosTab}, prm,
                                  inArr.data(), out);
                const std::size_t base = i + 1 - 16;
                for (int j = 0; j < 16; ++j) {
                    l[base + static_cast<std::size_t>(j)] = from24(out[2 * j]);
                    r[base + static_cast<std::size_t>(j)] = from24(out[2 * j + 1]);
                }
                fill = 0;
            }
        }
    }

private:
    static std::int32_t to24(float v) noexcept {
        if (!std::isfinite(v)) return 0;
        const double s = std::clamp(double(v) * 8388608.0, -8388608.0, 8388607.0);
        return std::int32_t(std::lround(s));
    }
    static float from24(std::int32_t w) noexcept {
        return float(static_cast<int>(w & 0xFFFFFF) | ((w & 0x800000) ? 0xFF000000 : 0x00000000)) / 8388608.0f;
    }

    mnm::PhaserState st{};
    mnm::PhaserParams prm{};
    std::array<std::int32_t, 34> inArr{};   // ядро читает 34 слова (32 используются)
    int fill = 0;
};

} // namespace monomachine
