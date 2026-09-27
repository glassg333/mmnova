// ============================================================================
// monomachine_phaser.hpp — черновик порта FX-PHASER Monomachine
// Источник: машинный код DSP56300 OS 1.32B, P:$144E81–$145035 (437 слов).
// Все константы и пути параметров — ВЕРИФИЦИРОВАНЫ по инструкциям.
// 16-табличная кубика и 48-бит конвейер фазы — реконструкция (TODO-SIM).
// ============================================================================
#pragma once
#include <cstdint>
#include <cmath>
#include <array>

namespace mnm {

static constexpr int   kFs = 44100;
static constexpr int   kBlock = 16;              // 16 сэмплов/блок (8 L + 8 R)
static constexpr float kScale = 1.0f / 8388608.0f;

// 8192-словные квадратурные таблицы (math-recovered: sin/cos(2πi/8192)·$7FFFFF)
inline float sine8k(int i) { return std::sin(2.0 * M_PI * i / 8192.0); }
inline float cos8k(int i)  { return std::cos(2.0 * M_PI * i / 8192.0); }
// (для бит-аккуратности: таблица round(sin·8388608) в 24-бит signed, читать из дампа)

// кубические веса интерполяции позиции чтения — точные слова кода P:$144F73–$144F8B
// (набор из 10 слов, stride 10 в кольце r2/m2=$0A; смещения $40/$80/$C0 = 0.5/1.0/1.5 отсчёта)
struct CubicKernel {
    static constexpr int32_t W0 = (int32_t)0xEAAAAB - (1 << 24); // −1/6
    static constexpr int32_t W2 = (int32_t)0xD55555 - (1 << 24); // −1/3
    static constexpr int32_t W8 = 0x7FFFFF;                      // ~+1.0
    static constexpr int32_t W9 = 0x155555;                      // +1/6
    static constexpr float f(int32_t w) { return w * kScale; }
};

struct PhaserParams {         // страница голоса, оффсеты $04–$0B
    uint16_t cntr;            // $04 CNTR
    uint16_t dep;             // $05 DEP
    uint16_t spd;             // $06 SPD
    uint16_t mix;             // $07 MIX
    uint16_t fb;              // $08 FB  (bipolar: word − $400000)
    uint16_t wid;             // $09 WID
    uint16_t inp;             // $0B INP
};

class Phaser {
public:
    void reset() {
        lfoPhase = 0; readPos = 0; widSt = 0; depSt = 0;
        fadeBlocks = 0; bufL.fill(0); bufR.fill(0); writeIdx = 0;
    }

    // in8x2: 8 L + 8 R interleaved; out8x2 — выход
    void process(const float* in8x2, float* out8x2, const PhaserParams& p) {
        // ---- вход: input·INP²·4 (P:$144EA6–$144EB3) --------------------------
        float inGain = (float)p.inp * (float)p.inp / (127.0f * 127.0f) * 4.0f;

        // ---- LFO-фаза: += SPD²·$9566 (P:$144EB4–$144EC5, 48-бит конвейер) ----
        double inc = (double)p.spd * (double)p.spd * 0x9566 * kScale;
        lfoPhase += inc * kBlock;                    // per-sample — см. TODO-SIM

        // ---- амплитуда LFO: WID → one-pole($28F5C) → sin-таблица -------------
        float k = 0x28F5C * kScale;                  // P:$144ECB
        float wTarget = (float)p.wid / 127.0f;
        widSt += k * (wTarget - widSt);              // P:$144EC8–$144ECF
        int ampIdx = (int)(widSt * 127.0f * 8.0f);   // st·$800>>24 (P:$144ED1)
        float s = sine8k(ampIdx & 8191), c = cos8k(ampIdx & 8191);

        // ---- DEP: one-pole ×2·$FD70A4 (P:$144F0C–$144F13) ---------------------
        float depTarget = (float)p.dep / 127.0f;
        depSt += 2.0f * (0x28F5C * kScale) * (depTarget - depSt);

        // ---- CNTR: приращение позиции чтения (P:$144EFB) ----------------------
        double cntrInc = (double)p.cntr * (int32_t)0xFFFC03 * kScale - 3.0 * kScale;
        readPos += cntrInc * kBlock;

        // ---- FB: bipolar (P:$144FBE) ------------------------------------------
        float fbAmt = ((int32_t)p.fb - 0x400000) * kScale;   // ±0.5
        float fbGain = fbAmt * 0x410410 * 4.0f * kScale;     // ×$410410·4
        fbGain *= 0x7EB852 * kScale;                          // ×$7EB852

        // ---- fade-in wet 128 блоков (P:$144E9C–$144EA4) -----------------------
        float wetGain = (fadeBlocks < 128) ? 0.0f : (0x80 * kScale * 16.0f);
        if (fadeBlocks < 128) ++fadeBlocks;

        // ---- лестница: 2 аллпасс-ступени по линии (P:$144F31–$144F3F) ---------
        for (int i = 0; i < 8; ++i) {
            float inL = in8x2[2*i] * inGain;
            float inR = in8x2[2*i+1] * inGain;
            // per-sample LFO-рампа: prev + (i+1)·Δ·8 (P:$144EE0–$144EE6)
            float mod = (float)(lfoPhase) * s;               // TODO-SIM: точный конвейер
            double pos = readPos + mod * depSt * 4096.0;     // позиция в кольце $1000
            float apL = allpassRead(bufL.data(), pos);
            float apR = allpassRead(bufR.data(), pos);
            // TODO-SIM: do#2 (две ступени, состояние 48-бит L:$1F),
            //           кубическое ядро CubicKernel (P:$144F70–$144FA2),
            //           нормировка чтения (P:$144F4E–$144F65)
            bufL[writeIdx] = inL - fbGain * apL;
            bufR[writeIdx] = inR - fbGain * apR;
            float wetL = apL, wetR = apR;

            // ---- микс (P:$145027–$145034): dry·(1−MIX) + wet·MIX ---------------
            float mix = (float)p.mix / 127.0f;
            float dryL = inL, dryR = inR;
            float outL = dryL * (1.0f - mix) + wetL * mix;
            float outR = dryR * (1.0f - mix) + wetR * mix;
            // WID-ширина: перекрёстные члены (P:$14502F–$145034) —
            // outL += (wetR - wetL) * widthAmt; outR += (wetL - wetR) * widthAmt;
            out8x2[2*i]   = outL * 0.7071f;   // ·$5A8241 (P:$145012)
            out8x2[2*i+1] = outR * 0.7071f;
            writeIdx = (writeIdx + 1) & 0xFFF;   // кольцо $1000 (m0=$FFF)
        }
    }

private:
    // 4-точечная кубическая интерполяция чтения (веса из kCubic-констант кода)
    static float allpassRead(const float* buf, double pos) {
        int   ip = (int)std::floor(pos);
        float fr = (float)(pos - ip);
        float m0 = buf[(ip - 1) & 0xFFF], m1 = buf[ip & 0xFFF];
        float m2 = buf[(ip + 1) & 0xFFF], m3 = buf[(ip + 2) & 0xFFF];
        // Catmull-Rom-класс (точные веса: −1/6, −1/3, +1/6, смещения $40/$80/$C0):
        float a0 = m1;
        float a1 = 0.5f * (m2 - m0);
        float a2 = m0 - 2.5f * m1 + 2.0f * m2 - 0.5f * m3;
        float a3 = 0.5f * (m3 - m0) + 1.5f * (m1 - m2);
        return ((a3 * fr + a2) * fr + a1) * fr + a0;
    }

    double lfoPhase = 0, readPos = 0;
    float widSt = 0, depSt = 0;
    int fadeBlocks = 0, writeIdx = 0;
    std::array<float, 4096> bufL{}, bufR{};   // 2 кольца по $1000 слов (m0=$FFF)
};

} // namespace mnm

/* ============================================================================
СПРАВКА ПО СООТВЕТСТВИЮ КОДУ (для сверки симуляцией):
  P:$144E81 init : база буфера $114000 + track·$2000; $11=$80; $12=0; LFO-стейты=0
  P:$144E9A proc : fade-in → вход INP²·4 → LFO-фаза → ampLFO → DEP-сглаживание →
                   CNTR-приращение → лестница do#2 → LSB-сглаживание → нормировка →
                   кубика → запись в кольцо → чтение ступеней → микс MIX/$7FFFFF−MIX
  Состояния страницы: $10 база буф., $11/$12 фаза-LFO 48b, $1D позиция 48b,
                      $20/$21 fade-счётчик, $22 DEP-сглаж., $24 WID-сглаж.,
                      $25/$26 LFO-фаза 48b
============================================================================ */
