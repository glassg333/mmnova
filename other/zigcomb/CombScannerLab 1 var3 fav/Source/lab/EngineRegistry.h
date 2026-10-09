#pragma once

#include "Engine.h"

// Единый список страниц-слотов (порядок = порядок вкладок в GUI).
// Чтобы убрать или добавить вариант — достаточно поправить EngineRegistry.cpp,
// интерфейс и параметры генерируются автоматически.
namespace lab
{
inline constexpr int kNumSlots = 5;

const char* slotParamId   (int slot, const char* key);   // "s0_gain"
const char* slotModelId   (int slot);                    // "s0_model"
const char* slotTrimId    (int slot);                    // "s0_trim"

// Описания страниц без создания рабочего движка (для GUI и построения параметров).
int              engineParamCount (int slot);
const ParamInfo* engineParams     (int slot);
EngineInfo       engineInfo       (int slot);
int              engineModelCount (int slot);
const char*      engineModelName  (int slot, int modelIndex);
}
