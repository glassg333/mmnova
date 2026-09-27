// ============================================================================
// monomachine_track_filter.hpp — черновик порта трекового фильтра Monomachine
// Источник: машинный код DSP56300 OS 1.32B, ядро P:$04A8–$0B49 + func_000340/397.
// Данные таблиц: 01_filter/tables/ (24-бит BE, signed fraction /2^23).
// Статус: каркас с ВЕРИФИЦИРОВАННЫМИ путями параметров и таблиц;
//         рекурсия SVF — реконструкция (Chamberlin), помечена TODO-SIM.
// ============================================================================
#pragma once
#include <cstdint>
#include <cmath>
#include <vector>
#include <array>

namespace mnm {

static constexpr int kFs = 44100;
static constexpr int kBlock = 16;          // 16 сэмплов/блок (8 L + 8 R)
static constexpr float kScale = 1.0f / 8388608.0f;  // 24-bit signed fraction

// --- загрузчик таблиц (файлы из tables/, 3 байта/слово big-endian) -----------
std::vector<float> load24(const char* path);

// --- таблицы, добытые из образа ---------------------------------------------
// div1      P:$143F95 1201w  f-коэфф. от BASE
// div2      P:$144446  128w  f-коэфф. от WDTH (сатурация 1.0)
// width1    P:$1444C6  128w  +Δf ширины
// width2    P:$144546  128w  −Δf ширины
// hpqDamp   P:$1448C6  128w  демпфирование от HPQ (индекс HPQ>>1)
// hpqFb     P:$144946  128w  фидбек от HPQ        (индекс HPQ>>1)
// lpqFb     P:$144BC9  128w  фидбек от LPQ        (индекс LPQ)
// lpRamp    P:$144AC7  258w  скорость однополюсных рамп BOFS/WOFS + delay-тон
// envCurve  P:$144B48  128w  амплитудная кривая env фильтра
// envShapeA P:$141800  128w  шаг атаки (ATK>>10)
// envShapeB P:$141880  128w  множитель затухания (DEC/REL>>10), отрицательные
extern std::vector<float> div1, div2, width1, width2, hpqDamp, hpqFb, lpqFb,
                          lpRamp, envCurve, envShapeA, envShapeB;

// halfband FIR каскада 1: константы из кода P:$05CF–$05D1 ($F528BD/$4A4DF0)
static constexpr std::array<float,4> kHalfband = {
    -723267.0f * kScale, 4870640.0f * kScale,
     4870640.0f * kScale, -723267.0f * kScale };  // −0.08623, +0.58066

struct FilterParams {       // страница голоса, оффсеты $10–$17 (слова param<<16)
    uint16_t base, wdth, hpq, lpq;   // 0..127
    uint16_t atk, dec;               // фильтр-энвелоп: скорость/глубина
    int16_t  bofs, wofs;             // bipolar −64..+63 (глубина env к BASE/WDTH)
};

class TrackFilter {
public:
    void reset() {
        hpLp = {0,0,0,0}; svfL = {0,0,0,0}; svfR = {0,0,0,0};
        envPhase = 0; envVal = 0; bofsRamp = 0; wofsRamp = 0;
        blockCount = 0;
    }
    void trigger()   { envStage = 0; envPhase = 0; }   // note on
    void release()   { envStage = 3; }                 // note off (стадия 3)

    // обработка блока: 8 L + 8 R interleaved (как в железе)
    void process(const float* in8x2, float* out8x2, const FilterParams& p) {
        // ---- [A] env фильтра (P:$0960–$0B49) -------------------------------
        float target = (p.bofs - 64) / 64.0f;           // BOFS−$400000, bipolar
        int rIdx = std::min(257, std::abs(p.bofs) * 2); // индекс LP-таблицы (P:$0A5F)
        float k = lpRamp[rIdx];                          // однополюсник за блок
        bofsRamp += k * (target - bofsRamp);
        float wTarget = (p.wofs - 64) / 64.0f;
        wofsRamp += lpRamp[std::min(257, std::abs(p.wofs) * 2)] * (wTarget - wofsRamp);
        // фаза/спад env: DEC ($15) — спад глубины, кривая envCurve
        // (стейт-машина стадий — см. README, раздел «FILTER-энвелоп»)
        float envAmt = envVal * bofsRamp;                // модуляция к BASE
        float envAmtW = envVal * wofsRamp;               // модуляция к WDTH

        // ---- [B] каскад 1: коэффициенты (P:$0537–$05A1) ---------------------
        float baseEnv = (p.base / 127.0f) + envAmt * 0.5f;   // ← масштаб env уточнить симуляцией
        float wdthEnv = (p.wdth / 127.0f) + envAmtW * 0.5f;
        float x = baseEnv * 1199.0f;
        int   idx = std::min(1199, (int)x);
        float frac = x - idx;
        float c1 = div1[idx] + (idx < 1200 ? (div1[idx + 1] - div1[idx]) * frac : 0.f);
        int   widx = std::min(127, (int)(wdthEnv * 128.0f));
        float c2 = div2[widx];
        float f  = 0.5f * (c1 + c2);                     // P:$0583 (div1+div2)/2
        f += frac * (c2 - c1) * 64.0f * kScale;          // P:$058E–$0591
        f -= width1[widx] * (1.0f + f);                  // P:$0592–$0593

        // ---- [C] резонанс (P:$08FA–$092D) -----------------------------------
        float damp = hpqDamp[p.hpq >> 1];
        float fbHP = hpqFb[p.hpq >> 1];
        float fbLP = lpqFb[p.lpq];

        // ---- [D] процесс -----------------------------------------------------
        // каскад 1: 2× oversample halfband + нерезонансная пара (func_000340)
        // каскад 2: резонансный SVF (func_000397) — Chamberlin-реконструкция:
        for (int i = 0; i < 8; ++i) {
            float inL = in8x2[2*i], inR = in8x2[2*i+1];
            // TODO-SIM: halfband-апсемплинг ×2 (чётные/нечётные фазы P:$05D8/$05EF),
            //           SVF-каскад 1 на f, затем каскад 2 (func_000397-паттерн):
            // LP += f * BP;  BP += f * LP - damp * BP + in;  HP = in - LP - bpDamp*BP
            // фидбеки: fbHP (HPQ), fbLP (LPQ); гейн-компенсация P:$0916–$092D
            float lp1L = svfL[0], bp1L = svfL[1];
            lp1L += f * bp1L;
            bp1L += f * lp1L - damp * bp1L + inL;
            float hp1L = inL - lp1L - damp * bp1L;
            // каскад 2 (резонансный):
            float lp2L = svfL[2] + f * svfL[3];
            float bp2L = svfL[3] + f * lp2L - (damp - fbHP - fbLP) * svfL[3] + hp1L;
            float outL = lp2L;
            svfL = {lp1L, bp1L, lp2L, bp2L};
            // R-канал — симметрично (в коде — параллельные a/b аккумуляторы)
            float lp1R = svfR[0], bp1R = svfR[1];
            lp1R += f * bp1R;
            bp1R += f * lp1R - damp * bp1R + inR;
            float hp1R = inR - lp1R - damp * bp1R;
            float lp2R = svfR[2] + f * svfR[3];
            float bp2R = svfR[3] + f * lp2R - (damp - fbHP - fbLP) * svfR[3] + hp1R;
            float outR = lp2R;
            svfR = {lp1R, bp1R, lp2R, bp2R};

            out8x2[2*i]   = outL;   // TODO: гейн-компенсация резонанса (P:$0916+)
            out8x2[2*i+1] = outR;   // TODO: halfband-децимация 2→1
        }
        ++blockCount;
    }

private:
    std::array<float,4> hpLp{}, svfL{}, svfR{};
    float envPhase = 0, envVal = 0, bofsRamp = 0, wofsRamp = 0;
    int envStage = -1;  // -1 idle, 0 atk, 1 hold, 2 dec, 3 rel (P:$088E-машина)
    int blockCount = 0;
};

} // namespace mnm
