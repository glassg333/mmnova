// ============================================================================
//  MnMTables.h — таблицы прошивки OS 1.32B
//  A  = воспроизводятся аналитически с точностью прошивки
//  C  = требуют дампа: tools/extract_tables_fixed.py -> MnMFirmwareTables.h
// ============================================================================
#pragma once
#include "MnMFixed.h"

namespace mmnova { namespace tables {

// --- питч ---------------------------------------------------------------------
// X:$140000 : wt[i] = 0.5 * 2^(i/2048), 2048 слов, диапазон acc 0..$5800 (11 октав)
constexpr int    kPitchTableSize = 2048;
constexpr int    kPitchAccMax    = 0x5800;
constexpr s24    kPitchScale     = 0x01D22A;   // масштаб инкремента (kernel P:$2D7..)
constexpr s24    kGndSinScale    = 0x17C6F9;   // *0.18625 — собственный масштаб GND-SIN
constexpr s24    kFmPitchScale   = 0x0BE37C;   // engine_pitch -> 48-бит фаза (FM)

// --- синус ---------------------------------------------------------------------
// X:$14A000, 8192 записи, маска фазы $1FFF, линейная интерполяция.
// В прошивке считается ColdFire'ом в runtime -> генерируем идентично.
constexpr int kSineBits = 13;
constexpr int kSineSize = 1 << kSineBits;

// --- FM ---------------------------------------------------------------------
// P:$141A80 : 24 музыкальных отношения, 1.23 fixed point (raw 0x004000..0x400000)
constexpr int kFmRatioCount = 24;
extern const s24 kFmRatios[kFmRatioCount];
extern const double kFmRatioF[kFmRatioCount];

// --- доступ к таблицам --------------------------------------------------------
void init();                       // один раз перед звуком (idempotent)

const s24* sine();                 // [8192]  A
const s24* pitch();                // [2048]  A
const s24* lpCoeff();              // [258]   P:$144AC7  C
const s24* rate();                 // [128]   Y:$141800  C  (AMP env + LFO SPD)
const s24* fmDynWave();            // [1024]  P:$141880  C
const s24* noiseTable();           // GND-NOIS                      C
bool  firmwareTablesPresent();     // true, если собрано с MMNOVA_HAVE_FW_TABLES

// удобные обёртки --------------------------------------------------------------
inline s24 lpFromParam (s24 paramWord) noexcept          // TONE/LP: asr #$10 -> idx
{
    const int idx = paramIndex (paramWord) * 2;          // 258 слов = 129 пар
    return lpCoeff()[idx < 257 ? idx : 257];
}
inline s24 rateFromParam (s24 paramWord) noexcept        // SPD / ATK / REL / DEC
{
    return rate()[paramIndex (paramWord)];
}

}} // namespace mmnova::tables
