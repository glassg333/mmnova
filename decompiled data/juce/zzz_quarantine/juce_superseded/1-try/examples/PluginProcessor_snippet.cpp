// ============================================================================
//  Пример стыковки с вашим PluginProcessor (Monomachine-Nova-1.6.0).
//  Копировать целиком не нужно — берите три отмеченных куска.
// ============================================================================
#include "mmnova/MnMEngine.h"

// --- 1) поле процессора ------------------------------------------------------
//  mmnova::Engine mnm;

// --- 2) prepareToPlay --------------------------------------------------------
void MonomachineNovaAudioProcessor::prepareToPlay (double sr, int /*blockSize*/)
{
    mnm.prepare (sr);

    // машины по трекам (как в патче Monomachine)
    mnm.setTrackMachine (0, mmnova::MachineId::SWAVE_SAW);
    mnm.setTrackMachine (1, mmnova::MachineId::FM_STAT);
    mnm.setTrackMachine (2, mmnova::MachineId::SID_6581);
    mnm.setTrackMachine (3, mmnova::MachineId::VO_6);
    mnm.setTrackMachine (4, mmnova::MachineId::DPRO_WAVE);
    mnm.setTrackMachine (5, mmnova::MachineId::FX_CHORUS);   // FX-трек

    mnm.setFxSource (5, 0);          // хорус слушает трек 1 (эквивалент INP-роутинга)
}

// --- 3) processBlock ---------------------------------------------------------
void MonomachineNovaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                  juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    mnm.process (buffer, midi);      // MIDI-канал 1..6 = трек 1..6, CC16..CC23 = 8 параметров
}

// --- Построение параметров APVTS прямо из дескрипторов ColdFire --------------
juce::AudioProcessorValueTreeState::ParameterLayout makeLayout()
{
    using namespace juce;
    AudioProcessorValueTreeState::ParameterLayout layout;

    for (int t = 0; t < mmnova::kNumTracks; ++t)
        for (int i = 0; i < mmnova::kNumParams; ++i)
        {
            const auto* d = mmnova::descriptorFor (mmnova::MachineId::SWAVE_SAW);
            const String id   = "t" + String (t) + "p" + String (i);
            const String name = "T" + String (t + 1) + " " + String (d->param[i]);
            layout.add (std::make_unique<AudioParameterInt> (
                ParameterID { id, 1 }, name, 0, 127, d->def[i]));
        }
    return layout;
}
