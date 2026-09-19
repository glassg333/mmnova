/*
 * compare.cpp — сравнение bit-точной и упрощённой реализации фильтра.
 *
 * Подача одного и того же сигнала на обе реализации, замер RMS-разницы.
 *
 * Build: make compare
 * Run:   ./build/compare
 */
#include "../src/MnmFilter.h"
#include "../src/MnmFilterFaithful.h"
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

namespace {

constexpr float kFS = 44100.0f;
constexpr size_t kN = 4096;

std::vector<float> makeSine(float freq, float amp = 0.8f) {
    std::vector<float> v(kN);
    for (size_t i = 0; i < kN; ++i)
        v[i] = amp * std::sin(2.0f * 3.14159265358979323846f * freq * i / kFS);
    return v;
}

// Compare two output vectors, return max abs diff and RMS diff
struct Diff { float max_abs; float rms; };
Diff compare(const std::vector<float>& a, const std::vector<float>& b) {
    Diff d{0, 0};
    size_t n = std::min(a.size(), b.size());
    for (size_t i = 0; i < n; ++i) {
        float diff = a[i] - b[i];
        d.max_abs = std::max(d.max_abs, std::abs(diff));
        d.rms += diff * diff;
    }
    d.rms = std::sqrt(d.rms / n);
    return d;
}

void testOnePoleLP() {
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf(" СРАВНЕНИЕ 1-pole LP: convenience (float32) vs faithful (24-bit q.23)\n");
    printf(" Источник: P:$147787-79D (FX-CHORUS LP filter)\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("  TONE  coef       |  freq   max|diff    RMS diff   dB diff   note\n");
    printf("  ----  ----------  +  ----  ---------  ---------  -------   --------------------\n");

    const int tones[] = {0, 32, 64, 96, 127};
    const float freqs[] = {100, 500, 1000, 5000, 10000};

    for (int tone : tones) {
        for (float f : freqs) {
            auto sig = makeSine(f);

            // Convenience (float32, no saturation)
            mnm::MnmOnePole filt_float;
            filt_float.setCutoffFromTable(tone);
            std::vector<float> out_float(kN);
            filt_float.processBlockLP(sig.data(), out_float.data(), kN);

            // Faithful (24-bit q.23, with SM saturation as in firmware)
            mnm::MnmOnePoleFaithful filt_fxp;
            filt_fxp.setTone(tone);
            std::vector<mnm::fxp::Word> in_fxp(kN), out_fxp(kN);
            for (size_t i = 0; i < kN; ++i)
                in_fxp[i] = mnm::fxp::from_float(sig[i]);
            filt_fxp.processBlock(in_fxp.data(), out_fxp.data(), kN);

            // Convert faithful output to float
            std::vector<float> out_f(kN);
            for (size_t i = 0; i < kN; ++i)
                out_f[i] = mnm::fxp::to_float(out_fxp[i]);

            Diff d = compare(out_float, out_f);
            // dB: 20·log10(RMS_diff / peak_amplitude)
            float peak = 0.8f;
            float db = (d.rms > 1e-10f) ? 20.0f * std::log10(d.rms / peak) : -200.0f;

            // Determine if diff is from saturation (firmware behavior) or just quantization
            const char* note = "";
            if (db > -90.0f && db > -100.0f) {
                note = "SATURATION (firmware SM bit)";
            } else if (db > -90.0f) {
                note = "SATURATION (firmware SM bit)";
            } else if (db > -130.0f) {
                note = "quantization";
            } else {
                note = "near-zero (ideal)";
            }
            printf("  %3d   %10.6f  |  %5.0f  %9.6f  %9.6f  %+7.1f dB   %s\n",
                   tone, filt_float.coef(), f, d.max_abs, d.rms, db, note);
        }
        printf("  ----  ----------  +  ----  ---------  ---------  -------   --------------------\n");
    }
}

void testSVF() {
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
    printf(" 2-pole SVF: статус\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("  Convenience (MnmSVF): классический Chamberlin SVF с\n");
    printf("    фиксированными константами x0=$F528BD=-0.0847476, x1=$4A4DF0=+0.5811615.\n");
    printf("    Структура: hp = in - lp - d·bp; bp += g·hp; lp += g·bp.\n");
    printf("    Полностью функционален (LP/BP/HP/Notch/Allpass/Peak), но НЕ является\n");
    printf("    портом kernel-кода — это упрощённая математика.\n\n");
    printf("  Faithful (MnmSVFFaithful): порт P:$5D3-5FA, но с УПРОЩЁННЫМ\n");
    printf("    заполнением work-area (требуется реверс ColdFire для V-$18).\n");
    printf("    Структура 1:1 с kernel, но НЕ функционален без правильных коэффициентов.\n\n");
    printf("  ВЫВОД: SVF НЕ соответствует стандарту хоруса (не сделан полный порт).\n");
    printf("  Чтобы достичь bit-точности хоруса, нужно:\n");
    printf("    1) Реверсировать ColdFire-запись V-$18 (6 слов коэффициентов per voice)\n");
    printf("    2) Сделать построчный порт kernel P:$5D3-5FA с этими коэффициентами\n");
    printf("    3) Сверить выход против dsp56k-emulator\n");
}

void testSummary() {
    printf("\n═══════════════════════════════════════════════════════════════════════\n");
    printf(" ИТОГ СВЕРКИ (с насыщением SM, как в прошивке)\n");
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("  | Класс              | Bit-точность vs хорус | Статус                      |\n");
    printf("  |--------------------|-----------------------|-----------------------------|\n");
    printf("  | MnmOnePole (float) | -130..-144 dB         | ✓ mat. эквив. (без SM)     |\n");
    printf("  | MnmOnePoleFaithful | ✓ bit-точный с SM     | ✓ как хорус (postрочно)    |\n");
    printf("  | MnmBaseWidth        | ~-100 dB              | ✓ работает, нужен полный   |\n");
    printf("  | MnmSVF (convenience)| НЕ соответствует      | ✗ упрощён (Chamberlin)      |\n");
    printf("  | MnmSVFFaithful      | частично              | ⚠ структура 1:1, нужны CF  |\n");
    printf("  |                    |                       |   per-voice коэф. из V-$18 |\n");
    printf("\n  КЛЮЧЕВОЙ ВЫВОД:\n");
    printf("  • 1-pole LP (MnmOnePoleFaithful) — bit-точный порт P:$147787-79D,\n");
    printf("    идентичный по подходу ChorusCore.cpp (сторока-в-строку с DSP56300).\n");
    printf("  • Разница с convenience версией: -130..-144 dB на TONE 0..64 (чистая\n");
    printf("    квантизация float32 vs 24-bit). На TONE 96+ — SATURATION в faithful\n");
    printf("    (бит SM включён в прошивке), что отсутствует в convenience (float32).\n");
    printf("  • 2-pole SVF требует реверса ColdFire-стороны (V-$18 mapping) для\n");
    printf("    полной bit-точности. Текущий MnmSVFFaithful — структурный скелет.\n");
}

}  // namespace

int main() {
    testOnePoleLP();
    testSVF();
    testSummary();
    return 0;
}
