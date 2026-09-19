#pragma once
#include "MnMFixed.h"
#include <array>

// ============================================================================
//  Страница голоса Y:$500..$533 (52 слова).  V = Y:$528.
//  Перенесено 1-в-1 из core/monomachine_engine.cpp (Jick238/monomachine-lab).
//  Все формулы ниже — ТИР A (работающий эмулятор на настоящей прошивке).
// ============================================================================

namespace mmnova {

static constexpr uint32_t kVoicePageY  = 0x000500;
static constexpr uint32_t kVoiceBaseY  = 0x000528;   // V = kVoicePageY + 40
static constexpr size_t   kVoicePageWords = 52;

// Канонический порядок параметров (индексы 0..32)
enum class Param : uint8_t
{
    // AMP (Y:$500..$507 = V-$28..V-$21)
    AmpAttack = 0, AmpHold, AmpDecay, AmpRelease,
    AmpDistortion, AmpVolume, AmpPan, AmpPortamento,

    // FILTER (Y:$508..$50F = V-$20..V-$19)
    FilterBase = 8, FilterWidth, FilterHighPassQ, FilterLowPassQ,
    FilterAttack, FilterDecay, FilterBaseOffset, FilterWidthOffset,

    // EFFECTS (Y:$510..$517 = V-$18..V-$11)
    EffectEqFrequency = 16, EffectEqGain, EffectSampleRateReduction,
    EffectDelayTime, EffectDelaySend, EffectDelayFeedback,
    EffectDelayBase, EffectDelayWidth,

    // SYNTH machine knobs (Y:$52C..$533 = V+$04..V+$0B = R6+$4..$B)
    SynthA = 24, SynthB, SynthC, SynthD, SynthE, SynthF, SynthG, SynthH,

    TrackLevel = 32,   // Y:$51F = V-$09
    Count = 33
};

// Смещение в 52-словной странице Y:$500
constexpr size_t pageOffset (Param p) noexcept
{
    const size_t i = static_cast<size_t>(p);
    if (i < 24) return i;
    if (i < 32) return 44 + (i - 24);
    return 31;
}

// Заводские дефолты трека (kLass3Track4Defaults)
static constexpr std::array<uint8_t, 33> kTrackDefaults = {
    0, 0, 64, 64, 64, 64, 64, 0,        // AMP  (DIST=64, PAN=64 -> нейтраль)
    0, 127, 0, 0, 0, 32, 64, 64,        // FILT (BASE=0, WDTH=127, BOFS/WOFS=64)
    64, 64, 0, 64, 64, 28, 0, 127,      // EFFX (EQG=64 -> 0 дБ)
    0, 0, 0, 0, 0, 0, 0, 64,            // SYNT (TUNE=64)
    90                                  // LEVEL
};

// --- Питч ноты (слот 41 = Y:$529 = V+$01) -----------------------------------
// 0x800 (2048) шагов на октаву, преднастройка 0x5800 (11 октав).
// Даёт 261.6 Гц на MIDI 60 для GND-SIN.
constexpr uint32_t midiNoteWord (uint8_t midiNote) noexcept
{
    return 0x5800u + (static_cast<uint32_t>(midiNote) * 0x800u + 6u) / 12u;
}

// --- Слово темпа (слот 35 = Y:$523 = V-$05) ---------------------------------
// Побитовый перевод MAIN OS 0x24DFF6..0x24E058. bpmTenths = BPM * 10 (1200 = 120.0).
constexpr uint32_t tempoWordFromTenths (uint16_t bpmTenths) noexcept
{
    const uint32_t b = bpmTenths < 300 ? 300 : (bpmTenths > 3000 ? 3000 : bpmTenths);
    const uint32_t whole = b / 10, frac = b % 10;
    const uint32_t ppqn24 = whole * 24 + (frac * 23 + 4) / 9;
    return 0x800000u / ppqn24;
}

// --- TUNE (SynthH, 0..127, центр 64) ----------------------------------------
// Считается хостом, а не машиной: асимметричное /64 вниз и /63 вверх,
// диапазон -1..+1 полутон. Умножает 56-битный инкремент питча A перед машиной.
inline double tuneFactor (uint8_t tune7) noexcept
{
    const double semi = (tune7 < 64)
        ? -static_cast<double>(64 - tune7) / 64.0
        :  static_cast<double>(tune7 - 64) / 63.0;
    return std::exp2 (semi / 12.0);
}

// --- PAN (AmpPan, 0..127, центр 64) -----------------------------------------
// Линейный баланс с полкой в центре (на pan=64 оба канала = 1.0).
// Делители разные: 63 слева и 64 справа — так в оригинальном микшере платы.
inline void applyPan (s24& left, s24& right, uint8_t pan7) noexcept
{
    const uint32_t lNum = (pan7 <= 64) ? 63u : (127u - pan7);
    const uint32_t rNum = (pan7 >= 64) ? 64u : pan7;
    left  = static_cast<s24>((static_cast<int64_t>(left)  * lNum) / 63);
    right = static_cast<s24>((static_cast<int64_t>(right) * rNum) / 64);
}

} // namespace mmnova
