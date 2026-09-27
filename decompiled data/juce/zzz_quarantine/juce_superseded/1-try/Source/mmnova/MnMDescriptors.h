// ============================================================================
//  MnMDescriptors.h — дескрипторы параметров машин.
//  Источник: распакованный ColdFire-образ (coldfire_main.bin, 425 858 байт),
//  таблица @0x57FC5, шаг 0xB0 (176 байт). Имена и заводские дефолты — точные.
// ============================================================================
#pragma once
#include "MnMMachine.h"

namespace mmnova {

struct MachineDescriptor
{
    MachineId   id;
    const char* fullName;      // как в дескрипторе ColdFire
    const char* shortName;
    uint32_t    coldfireOffset;
    const char* param[kNumParams];   // "---" = параметр не используется
    uint8_t     def[kNumParams];     // заводские значения
    bool        isFx;                // FX-машина (берёт вход от соседнего трека)
};

const MachineDescriptor* descriptors();     // массив
int                      descriptorCount();
const MachineDescriptor* descriptorFor (MachineId id);

} // namespace mmnova
