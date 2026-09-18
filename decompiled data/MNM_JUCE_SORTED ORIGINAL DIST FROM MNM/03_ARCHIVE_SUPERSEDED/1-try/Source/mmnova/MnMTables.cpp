// ============================================================================
//  MnMTables.cpp
// ============================================================================
#include "MnMTables.h"
#if defined (MMNOVA_HAVE_FW_TABLES)
 #include "MnMFirmwareTables.h"      // сгенерировано tools/extract_tables_fixed.py
#endif
#include <vector>

namespace mmnova { namespace tables {

// P:$141A80 — 24 отношения (значения из аудита, 1.23 fixed-point)
const double kFmRatioF[kFmRatioCount] = {
    1.0/32, 1.0/16, 1.0/8, 3.0/16, 1.0/4, 5.0/16, 3.0/8, 1.0/2,
    5.0/8,  3.0/4,  7.0/8, 1.0,    1.25,  1.5,    1.75,  2.0,
    2.5,    3.0,    3.5,   4.0,    5.0,   6.0,    7.0,   8.0
};
const s24 kFmRatios[kFmRatioCount] = {
    0x004000, 0x008000, 0x010000, 0x018000, 0x020000, 0x028000, 0x030000, 0x040000,
    0x050000, 0x060000, 0x070000, 0x080000, 0x0A0000, 0x0C0000, 0x0E0000, 0x100000,
    0x140000, 0x180000, 0x1C0000, 0x200000, 0x280000, 0x300000, 0x380000, 0x400000
};

namespace {
    bool  gInit = false;
    std::vector<s24> gSine, gPitch, gLp, gRate, gDyn, gNoise;

    // fallback-кривая LP: один полюс, fc растёт экспоненциально с индексом.
    void buildLpFallback (std::vector<s24>& t)
    {
        t.resize (258);
        for (int i = 0; i < 258; ++i)
        {
            const double x  = i / 257.0;
            const double fc = 20.0 * std::pow (1000.0, x);        // 20 Гц .. 20 кГц
            double c = 1.0 - std::exp (-6.2831853071795864 * fc / 44100.0);
            c = std::min (c, 0.999999);
            t[(size_t) i] = fromFloat (c);
        }
    }
    // fallback rate: экспонента, покрывает ~0.02 Гц .. ~50 Гц LFO / env
    void buildRateFallback (std::vector<s24>& t)
    {
        t.resize (128);
        for (int i = 0; i < 128; ++i)
        {
            const double v = std::pow (2.0, (i - 127) / 12.0);     // 0..1
            t[(size_t) i] = fromFloat (std::min (v, 0.9999999));
        }
    }
    // fallback FM-DYN wavetable: нисходящий спектральный ряд (описание аудита:
    // "нисходящая спектральная таблица, сканируется 2ENV")
    void buildDynFallback (std::vector<s24>& t)
    {
        t.resize (1024);
        for (int i = 0; i < 1024; ++i)
        {
            const double p = i / 1024.0;
            double s = 0.0, norm = 0.0;
            for (int h = 1; h <= 8; ++h)
            {
                const double amp = 1.0 / h;
                s    += amp * std::sin (6.2831853071795864 * h * p);
                norm += amp;
            }
            t[(size_t) i] = fromFloat (s / norm * 0.98);
        }
    }
}

void init()
{
    if (gInit) return;
    gInit = true;

    // --- sine 8192 (X:$14A000) ------------------------------------------------
    gSine.resize (kSineSize);
    for (int i = 0; i < kSineSize; ++i)
        gSine[(size_t) i] = fromFloat (std::sin (6.2831853071795864 * i / kSineSize));

    // --- pitch 2048 (X:$140000) : wt[i] = 0.5 * 2^(i/2048) --------------------
    gPitch.resize (kPitchTableSize);
    for (int i = 0; i < kPitchTableSize; ++i)
        gPitch[(size_t) i] = fromFloat (0.5 * std::pow (2.0, (double) i / kPitchTableSize));

#if defined (MMNOVA_HAVE_FW_TABLES)
    gLp.assign   (fw::kLp144AC7,  fw::kLp144AC7  + fw::kLp144AC7Size);
    gRate.assign (fw::kRate141800, fw::kRate141800 + fw::kRate141800Size);
    gDyn.assign  (fw::kDynWave141880, fw::kDynWave141880 + fw::kDynWave141880Size);
    gNoise.assign(fw::kNoiseTable, fw::kNoiseTable + fw::kNoiseTableSize);
#else
 #if defined (_MSC_VER)
  #pragma message ("MMNova: firmware tables absent - LP/rate/FM-DYN are analytic fallbacks")
 #else
  #warning "MMNova: firmware tables absent - LP/rate/FM-DYN are analytic fallbacks (run tools/extract_tables_fixed.py)"
 #endif
    buildLpFallback (gLp);
    buildRateFallback (gRate);
    buildDynFallback (gDyn);
    gNoise.assign (1, 0);
#endif
}

const s24* sine()       { init(); return gSine.data();  }
const s24* pitch()      { init(); return gPitch.data(); }
const s24* lpCoeff()    { init(); return gLp.data();    }
const s24* rate()       { init(); return gRate.data();  }
const s24* fmDynWave()  { init(); return gDyn.data();   }
const s24* noiseTable() { init(); return gNoise.data(); }

bool firmwareTablesPresent()
{
#if defined (MMNOVA_HAVE_FW_TABLES)
    return true;
#else
    return false;
#endif
}

}} // namespace
