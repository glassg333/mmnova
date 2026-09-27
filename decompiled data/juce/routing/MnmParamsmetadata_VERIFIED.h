// ============================================================================
// MnmParams_VERIFIED.h — проверенные поля дескрипторов, entries 0..21.
// Сверено с all_machine_descriptors.json entries 0..21 и raw ColdFire table @0x57FC5, stride 0xB0.
// Layout блока: short@+0, display@+9, params 8x6 байт @+0x12, defaults @+0x42.
// Порядок ручек = порядок параметров в voice-блоке DSP: param_i -> V+$04+i
// (доказано чтениями y:(r6+$4..$b) в листингах машин).
// ВАЖНО: это только имена/дефолты/порядок; кривые UI->DSP здесь НЕ подтверждаются.
// ============================================================================
#pragma once
#include <cstdint>

namespace mmnova_verified {

struct MachineParams {
    int         slot;
    const char* family;    // short name (GND/SID/SWAVE/DPRO/FM+/VO/FX)
    const char* variant;   // display name (SIN/BBOX/CHORUS/...)
    const char* knobs[8];
    uint8_t     defaults[8];
};

static constexpr MachineParams kVerifiedDescriptorEntries[] = {
    { 0, "GND", "GND", { "---", "---", "---", "---", "---", "---", "---", "---" }, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 1, "GND", "SIN", { "---", "---", "---", "---", "---", "---", "---", "TUNE" }, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 } },
    { 2, "GND", "NOIS", { "ST", "RED", "STON", "---", "---", "---", "---", "TUNE" }, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 } },
    { 3, "SID", "6581", { "PW", "PWAD", "PWRS", "WAVE", "MOD", "MSRC", "MFRQ", "TUNE" }, { 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x40, 0x40 } },
    { 4, "SWAVE", "SAW", { "UNIL", "UNIW", "UNIX", "---", "SUBX", "SUB1", "SUB2", "TUNE" }, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 } },
    { 5, "SWAVE", "PULS", { "UNIL", "UNIW", "SUB1", "SUB2", "PW", "PWAD", "PWRS", "TUNE" }, { 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40 } },
    { 6, "DPRO", "WAVE", { "WAVE", "WP", "WPM", "WPRS", "SYNC", "SFRQ", "---", "TUNE" }, { 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x40 } },
    { 7, "DPRO", "BBOX", { "PTCH", "STRT", "---", "---", "RTRG", "RTIM", "---", "---" }, { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 8, "FM+", "STAT", { "1FRQ", "1FIN", "1ENV", "1FB", "2FRQ", "2VOL", "TONE", "TUNE" }, { 0x3C, 0x40, 0x50, 0x1E, 0x50, 0x40, 0x62, 0x40 } },
    { 9, "FM+", "PAR", { "1FRQ", "1ENV", "2FRQ", "2ENV", "3FRQ", "3ENV", "TONE", "TUNE" }, { 0x3C, 0x40, 0x50, 0x40, 0x66, 0x50, 0x62, 0x40 } },
    { 10, "FM+", "DYN", { "1FRQ", "1FEN", "1VOL", "1VEN", "2FRQ", "2ENV", "2FB", "TUNE" }, { 0x40, 0x40, 0x40, 0x40, 0x4A, 0x50, 0x1E, 0x40 } },
    { 11, "VO", "VO-6", { "VOC1", "VOC2", "V-SW", "VOIC", "CONS", "CLEN", "CVOL", "TUNE" }, { 0x40, 0x40, 0x60, 0x00, 0x00, 0x40, 0x40, 0x40 } },
    { 12, "FX", "THRU", { "---", "---", "---", "---", "---", "---", "---", "INP" }, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 } },
    { 13, "FX", "REVERB", { "DEC", "DAMP", "GATE", "MIX", "HP", "LP", "---", "INP" }, { 0x40, 0x02, 0x7F, 0x20, 0x00, 0x7F, 0x00, 0x40 } },
    { 14, "SWAVE", "ENS", { "PCH2", "PCH3", "PCH4", "WAVE", "PW", "CHRL", "CHRW", "TUNE" }, { 0x3F, 0x3F, 0x3F, 0x00, 0x40, 0x00, 0x7F, 0x40 } },
    { 15, "FX", "CHORUS", { "DEL", "DEP", "SPD", "MIX", "FB", "WID", "LP", "INP" }, { 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x7F, 0x40 } },
    { 16, "FX", "DYNAMIX", { "ATK", "REL", "THRS", "MIX", "RAT", "GAIN", "RMS", "INP" }, { 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x00, 0x40 } },
    { 17, "FX", "RINGMOD", { "WAVE", "EXT", "---", "MIX", "---", "---", "---", "INP" }, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 } },
    { 18, "FX", "PHASER", { "CNTR", "DEP", "SPD", "MIX", "FB", "WID", "---", "INP" }, { 0x40, 0x40, 0x40, 0x7F, 0x2D, 0x00, 0x7F, 0x40 } },
    { 19, "FX", "FLANGER", { "DEL", "DEP", "SPD", "MIX", "FB", "WID", "---", "INP" }, { 0x40, 0x40, 0x40, 0x7F, 0x2D, 0x00, 0x7F, 0x40 } },
    { 20, "DPRO", "DDRW", { "WAV1", "MIX", "WAV2", "TIME", "BR1", "WID", "BR2", "TUNE" }, { 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 } },
    { 21, "DPRO", "DENS", { "PCH2", "PCH3", "PCH4", "WAVE", "---", "CHRL", "CHRW", "TUNE" }, { 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x40 } },
};

static constexpr int kVerifiedDescriptorEntryCount = sizeof(kVerifiedDescriptorEntries)/sizeof(kVerifiedDescriptorEntries[0]);

} // namespace mmnova_verified
