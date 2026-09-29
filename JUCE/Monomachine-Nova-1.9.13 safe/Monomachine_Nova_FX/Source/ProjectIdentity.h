#pragma once
#include "NovaConfig.h"
// Included after JuceHeader.h: validates Projucer/AppConfig and CMake metadata alike.
#if NOVA_SYNTH != 0 && NOVA_SYNTH != 1
#error "NOVA_SYNTH must be 0 (FX) or 1 (Synth)."
#endif
#if defined(JucePlugin_IsSynth) && JucePlugin_IsSynth != NOVA_SYNTH
#error "Wrong project/source pairing: JucePlugin_IsSynth differs from NovaConfig.h."
#endif
#if defined(JucePlugin_WantsMidiInput) && !JucePlugin_WantsMidiInput
#error "Nova requires MIDI input in both projects."
#endif
#if defined(JucePlugin_ProducesMidiOutput) && JucePlugin_ProducesMidiOutput != (NOVA_SYNTH == 0)
#error "Wrong MIDI output flag: FX must output MIDI, Synth must not."
#endif
#ifdef JucePlugin_PluginCode
static_assert(JucePlugin_PluginCode == (NOVA_SYNTH ? 0x4e765379 : 0x4e764678),
              "Wrong plugin code: expected NvSy for Synth or NvFx for FX.");
#endif
#ifdef JucePlugin_ManufacturerCode
static_assert(JucePlugin_ManufacturerCode == 0x4d6e4f6e, "Manufacturer code must remain MnOn.");
#endif
#ifdef JucePlugin_Name
namespace nova_build_check {
constexpr bool equal(const char* a,const char* b){while(*a&&*a==*b){++a;++b;}return *a==*b;}
static_assert(equal(JucePlugin_Name,NOVA_SYNTH ? "Monomachine Nova Synth" : "Monomachine Nova FX"),
              "Wrong product name. Re-save the correct .jucer and rebuild its generated solution.");
}
#endif
