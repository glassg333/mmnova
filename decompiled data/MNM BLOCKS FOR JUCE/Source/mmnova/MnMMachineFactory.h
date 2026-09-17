// ============================================================================
//  MnMMachineFactory.h — эквивалент таблицы диспетчеризации
//  X:$10016B (init) / X:$10018D (config) / X:$1001AF (process), по 34 входа.
// ============================================================================
#pragma once
#include "machines/MnMGnd.h"
#include "machines/MnMSid.h"
#include "machines/MnMSwave.h"
#include "machines/MnMDpro.h"
#include "machines/MnMFm.h"
#include "machines/MnMVo6.h"
#include "machines/MnMFxChorus.h"
#include "machines/MnMFxMisc.h"
#include "MnMDescriptors.h"
#include <memory>

namespace mmnova {

inline std::unique_ptr<Machine> createMachine (MachineId id)
{
    switch (id)
    {
        case MachineId::GND_SIN:    return std::make_unique<MachineGndSin>();
        case MachineId::GND_NOIS:   return std::make_unique<MachineGndNoise>();
        case MachineId::SID_6581:   return std::make_unique<MachineSid>();
        case MachineId::SWAVE_SAW:  return std::make_unique<MachineSwaveSaw>();
        case MachineId::SWAVE_PULS: return std::make_unique<MachineSwavePuls>();
        case MachineId::SWAVE_ENS:  return std::make_unique<MachineSwaveEns>();
        case MachineId::DPRO_WAVE:  return std::make_unique<MachineDproWave>();
        case MachineId::DPRO_BBOX:  return std::make_unique<MachineDproBbox>();
        case MachineId::DPRO_DDRW:  return std::make_unique<MachineDproDdrw>();
        case MachineId::DPRO_DENS:  return std::make_unique<MachineDproDens>();
        case MachineId::FM_STAT:    return std::make_unique<MachineFmStat>();
        case MachineId::FM_PAR:     return std::make_unique<MachineFmPar>();
        case MachineId::FM_DYN:     return std::make_unique<MachineFmDyn>();
        case MachineId::VO_6:       return std::make_unique<MachineVo6>();
        case MachineId::FX_THRU:    return std::make_unique<MachineFxThru>();
        case MachineId::FX_REVERB:  return std::make_unique<MachineFxReverb>();
        case MachineId::FX_CHORUS:  return std::make_unique<MachineFxChorus>();
        case MachineId::FX_DYNAMIX: return std::make_unique<MachineFxDynamix>();
        case MachineId::FX_RINGMOD: return std::make_unique<MachineFxRingmod>();
        case MachineId::FX_PHASER:  return std::make_unique<MachineFxPhaser>();
        case MachineId::FX_FLANGER: return std::make_unique<MachineFxFlanger>();
        case MachineId::FX_DLY:     return std::make_unique<MachineFxDelay>();
        case MachineId::FX_EXT:     return std::make_unique<MachineFxExt>();
        default:                    return std::make_unique<MachineGnd>();
    }
}

// Создать машину с её заводскими дефолтами из дескриптора ColdFire
inline std::unique_ptr<Machine> createMachineWithDefaults (MachineId id)
{
    auto m = createMachine (id);
    if (const auto* d = descriptorFor (id))
    {
        int v[kNumParams];
        for (int i = 0; i < kNumParams; ++i) v[i] = d->def[i];
        m->setAllParams (v);
    }
    m->tickConfig();
    return m;
}

} // namespace mmnova
