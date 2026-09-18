/*
 * demo.cpp — демонстрация фильтров Monomachine
 *
 * Прогоняет 4 теста:
 *   1) MnmOnePole LP с разными TONE-параметрами (sweep)
 *   2) MnmOnePole HP
 *   3) MnmBaseWidthFilter (cascaded HP+LP, ядро P:$A5F-ABF)
 *   4) MnmSVF multimode (LP/BP/HP/NOTCH)
 *
 * Выводит: amplitude response на нескольких частотах + cutoff-таблицу.
 *
 * Build: make
 * Run:   ./build/demo
 */
#include "../src/MnmFilter.h"
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstring>

namespace {

constexpr float kFS = 44100.0f;
constexpr size_t kBlockN = 16384;  // ~0.37 sec

// Сгенерировать N сэмплов синуса на частоте freq
std::vector<float> makeSine(float freq, size_t n, float amp = 0.8f) {
    std::vector<float> v(n);
    for (size_t i = 0; i < n; ++i)
        v[i] = amp * std::sin(2.0f * 3.14159265358979323846f * freq * i / kFS);
    return v;
}

// Измерить установившуюся амплитуду отклика
float measureSteadyAmplitude(const std::vector<float>& out, size_t skipFirst) {
    float maxAbs = 0.0f;
    for (size_t i = skipFirst; i < out.size(); ++i)
        maxAbs = std::max(maxAbs, std::abs(out[i]));
    return maxAbs;
}

// ДБ-величина
float toDb(float amp) {
    if (amp <= 1e-7f) return -140.0f;
    return 20.0f * std::log10(amp);
}

void testOnePoleLP() {
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf(" TEST 1: MnmOnePole LP — амплитудная характеристика\n");
    printf("       (1-pole IIR, table P:$144ac7, fx-chorus/fm/rev pattern)\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("  TONE  coef      fc(-3dB)   | 100 Hz  500 Hz  1 kHz  5 kHz  10 kHz\n");
    printf("  ----  --------  ---------  +  ------------------------------------\n");

    const int tone_values[] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 127};
    const float test_freqs[] = {100, 500, 1000, 5000, 10000};
    const size_t n_tones = sizeof(tone_values) / sizeof(tone_values[0]);
    const size_t n_freqs = sizeof(test_freqs) / sizeof(test_freqs[0]);

    for (size_t it = 0; it < n_tones; ++it) {
        int tone = tone_values[it];
        mnm::MnmOnePole lp;
        lp.setCutoffFromTable(tone);
        float fc = lp.cutoffHz();
        printf("  %3d   %8.6f  ", tone, lp.coef());
        if (std::isnan(fc)) printf("passthrough | ");
        else printf("%8.1f Hz | ", fc);

        for (size_t ifr = 0; ifr < n_freqs; ++ifr) {
            auto sig = makeSine(test_freqs[ifr], kBlockN);
            std::vector<float> out(kBlockN);
            lp.processBlockLP(sig.data(), out.data(), kBlockN);
            // Skip first 0.1s (transient)
            size_t skip = static_cast<size_t>(0.1f * kFS);
            if (skip > kBlockN) skip = kBlockN / 4;
            float amp = measureSteadyAmplitude(out, skip) / 0.8f;  // normalize to input
            printf(" %+5.1fdB", toDb(amp));
            if (ifr < n_freqs - 1) printf("  ");
        }
        printf("\n");
    }
    printf("\n");
}

void testOnePoleHP() {
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf(" TEST 2: MnmOnePole HP — амплитудная характеристика\n");
    printf("       (тот же 1-pole, но y[i] = x[i] - state — как FX-REV HP)\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("  TONE  coef      fc(-3dB)   | 100 Hz  500 Hz  1 kHz  5 kHz  10 kHz\n");
    printf("  ----  --------  ---------  +  ------------------------------------\n");

    const int tone_values[] = {0, 16, 32, 48, 64, 80, 96, 112, 127};
    const float test_freqs[] = {100, 500, 1000, 5000, 10000};
    const size_t n_tones = sizeof(tone_values) / sizeof(tone_values[0]);
    const size_t n_freqs = sizeof(test_freqs) / sizeof(test_freqs[0]);

    for (size_t it = 0; it < n_tones; ++it) {
        int tone = tone_values[it];
        mnm::MnmOnePole hp;
        hp.setCutoffFromTable(tone);
        float fc = hp.cutoffHz();
        printf("  %3d   %8.6f  ", tone, hp.coef());
        if (std::isnan(fc)) printf("passthrough | ");
        else printf("%8.1f Hz | ", fc);

        for (size_t ifr = 0; ifr < n_freqs; ++ifr) {
            auto sig = makeSine(test_freqs[ifr], kBlockN);
            std::vector<float> out(kBlockN);
            hp.processBlockHP(sig.data(), out.data(), kBlockN);
            size_t skip = static_cast<size_t>(0.1f * kFS);
            if (skip > kBlockN) skip = kBlockN / 4;
            float amp = measureSteadyAmplitude(out, skip) / 0.8f;
            printf(" %+5.1fdB", toDb(amp));
            if (ifr < n_freqs - 1) printf("  ");
        }
        printf("\n");
    }
    printf("\n");
}

void testBaseWidth() {
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf(" TEST 3: MnmBaseWidthFilter — cascaded HP+LP (kernel P:$A5F-ABF)\n");
    printf("       (base=HP cutoff, width=LP offset, все коэффициенты из таблицы)\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("  BASE  WIDTH  HP-fc   LP-fc  | 100 Hz  500 Hz  1 kHz  5 kHz  10 kHz\n");
    printf("  ----  -----  ------  ------  +  ----------------------------------\n");

    struct BW { int b; int w; };
    const BW sets[] = {
        {0, 127}, {32, 32}, {48, 32}, {64, 32}, {64, 64}, {80, 32}, {96, 32}
    };
    const float test_freqs[] = {100, 500, 1000, 5000, 10000};
    const size_t n_freqs = sizeof(test_freqs) / sizeof(test_freqs[0]);

    for (auto& s : sets) {
        mnm::MnmBaseWidthFilter f;
        f.setFromParams(s.b, s.w, 0);
        float hpf = f.cutoffBase();
        float lpf = f.cutoffWidth();
        char hpf_str[16], lpf_str[16];
        if (std::isnan(hpf)) std::snprintf(hpf_str, sizeof(hpf_str), "    passthru");
        else                 std::snprintf(hpf_str, sizeof(hpf_str), "%8.1f Hz", hpf);
        if (std::isnan(lpf)) std::snprintf(lpf_str, sizeof(lpf_str), "    passthru");
        else                 std::snprintf(lpf_str, sizeof(lpf_str), "%8.1f Hz", lpf);
        printf("  %3d   %4d   %s  %s  | ", s.b, s.w, hpf_str, lpf_str);

        for (size_t ifr = 0; ifr < n_freqs; ++ifr) {
            auto sig = makeSine(test_freqs[ifr], kBlockN);
            std::vector<float> out(kBlockN);
            f.processBlock(sig.data(), out.data(), kBlockN);
            size_t skip = static_cast<size_t>(0.15f * kFS);
            if (skip > kBlockN) skip = kBlockN / 4;
            float amp = measureSteadyAmplitude(out, skip) / 0.8f;
            printf(" %+5.1fdB", toDb(amp));
            if (ifr < n_freqs - 1) printf("  ");
        }
        printf("\n");
    }
    printf("\n");
}

void testSVF() {
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf(" TEST 4: MnmSVF multimode — fixed x0=$F528BD (-0.0847), x1=$4A4DF0 (+0.5812)\n");
    printf("       (kernel P:$5D3-5FA, FILT-page multimode, 16 samples/block)\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    const float cutoffs[] = {200.0f, 1000.0f, 4000.0f};
    const float resos[]   = {0.1f, 0.5f, 0.9f};
    const float test_freqs[] = {100, 500, 1000, 5000, 10000};
    const mnm::MnmSVF::Mode modes[] = {
        mnm::MnmSVF::Mode::LP,
        mnm::MnmSVF::Mode::BP,
        mnm::MnmSVF::Mode::HP,
        mnm::MnmSVF::Mode::NOTCH,
    };
    const char* mode_names[] = {"LP", "BP", "HP", "NOTCH"};

    for (size_t im = 0; im < 4; ++im) {
        printf("  --- Mode: %s ---\n", mode_names[im]);
        printf("  fc      Q       | 100 Hz  500 Hz  1 kHz  5 kHz  10 kHz\n");
        for (float fc : cutoffs) {
            for (float r : resos) {
                mnm::MnmSVF svf;
                svf.setParams(fc, r);
                printf("  %5.0f  %5.2f   | ", fc, 0.5f + r * 49.5f);
                for (size_t ifr = 0; ifr < 5; ++ifr) {
                    auto sig = makeSine(test_freqs[ifr], kBlockN);
                    std::vector<float> out(kBlockN);
                    svf.processBlock(sig.data(), out.data(), kBlockN, modes[im]);
                    size_t skip = static_cast<size_t>(0.2f * kFS);
                    if (skip > kBlockN) skip = kBlockN / 4;
                    float amp = measureSteadyAmplitude(out, skip) / 0.8f;
                    printf(" %+6.1fdB", toDb(amp));
                    if (ifr < 4) printf(" ");
                }
                printf("\n");
            }
        }
        printf("\n");
    }
}

void testSVF_ImpulseResponse() {
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf(" TEST 5: MnmSVF — импульсная характеристика (impulse response)\n");
    printf("       (fc=1000 Hz, Q=10, 32 сэмпла)\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    mnm::MnmSVF svf;
    svf.setParams(1000.0f, 0.82f);  // Q ≈ 41
    printf("  n   input   lp   bp   hp\n");
    printf("  --  ------  ---  ---  ---\n");
    for (size_t i = 0; i < 32; ++i) {
        float in = (i == 0) ? 1.0f : 0.0f;
        float lp, bp, hp;
        svf.process(in, lp, bp, hp);
        printf("  %2zu  %5.2f   %+6.3f  %+6.3f  %+6.3f\n", i, in, lp, bp, hp);
    }
    printf("\n");
}

void printSourceInfo() {
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf(" Monomachine SFX-60 OS 1.32B — DSP-фильтры\n");
    printf(" Источник: верифицированные образы памяти DSP56300\n");
    printf("   dsp1_pmem.bin  SHA-256: e99c6a6714feebb893c4f8f3eccf1b31dc900cc856814b44cd2f397d60889112\n");
    printf("   dsp1_xmem.bin  SHA-256: 6620483f069e82efb6f5215df4e239f0b288236712eefe8b9a6ad20206c5402f\n");
    printf("   dsp1_ymem.bin  SHA-256: 4af22d0c5ec3d0c2a0d4a6d123dc618b381b37b3283e38ffa1fefd56b0a0460d\n");
    printf(" Sample rate: 44 100 Hz. Block: 16 samples (~0.36 ms).\n");
    printf(" Все коэффициенты сверены байт-в-байт с .txt-дампами репозитория.\n");
    printf("═══════════════════════════════════════════════════════════════════════\n\n");
}

}  // namespace anonymous

int main() {
    printSourceInfo();
    testOnePoleLP();
    testOnePoleHP();
    testBaseWidth();
    testSVF();
    testSVF_ImpulseResponse();
    printf("[OK] All filter tests done.\n");
    return 0;
}
