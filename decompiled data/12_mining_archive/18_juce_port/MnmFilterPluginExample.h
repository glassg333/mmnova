// ============================================================================
// MnmFilterPluginExample.h — минимальный скелет JUCE-плагина с dual-фильтром
// Monomachine. Это РАБОЧИЙ образец: скопируйте в свой проект (Projucer/CMake),
// подключите Source/mnm_*.h и адаптируйте под свой граф.
//
// Параметры (APVTS): BASE WDTH HPQ LPQ (0..127) + ATK HOLD DEC REL VOL PAN.
// Порядок в processBlock: параметры -> MIDI -> порубить буфер на кадры
// по 16 сэмплов -> voice.process().
// ============================================================================
#pragma once
#include <JuceHeader.h>
#include "Source/mnm_voice_example.h"

class MnmFilterAudioProcessor : public juce::AudioProcessor
{
public:
    MnmFilterAudioProcessor()
        : AudioProcessor (BusesProperties()
              .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
              .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
          apvts (*this, nullptr, "PARAMS", createLayout())
    {
    }

    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout l;
        auto add = [&] (const juce::String& id, const juce::String& name, int def)
        {
            l.add (std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID { id, 1 }, name,
                juce::NormalisableRange<float> (0.f, 127.f, 1.f), (float) def));
        };
        // фильтр (Y[P+$10..$13] оригинала)
        add ("base", "FILT BASE", 64);
        add ("wdth", "FILT WDTH", 0);
        add ("hpq",  "FILT HPQ",  0);
        add ("lpq",  "FILT LPQ",  0);
        // AMP-энвелоп (Y[P+$00..$06])
        add ("atk",  "AMP ATK",   0);
        add ("hold", "AMP HOLD",  0);
        add ("dec",  "AMP DEC",   90);
        add ("rel",  "AMP REL",   40);
        add ("vol",  "AMP VOL",   127);
        add ("pan",  "AMP PAN",   64);
        return l;
    }

    void prepareToPlay (double sampleRate, int /*samplesPerBlock*/) override
    {
        voice.prepare (sampleRate);
    }
    void releaseResources() override {}

    bool isBusesLayoutSupported (const BusesLayout& l) const override
    {
        return l.getMainInputChannelSet()  == juce::AudioChannelSet::stereo()
            && l.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
    }

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override
    {
        juce::ScopedNoDenormals noDenormals;

        // --- MIDI -> события огибающей (TRIG_EVENT 1/2/3) -------------------
        for (const auto meta : midi)
        {
            const auto& m = meta.getMessage();
            if (m.isNoteOn())       voice.noteOn();
            else if (m.isNoteOff()) voice.noteOff();
            else if (m.isAllNotesOff() || m.isAllSoundOff()) voice.kill();
        }

        // --- параметры (слова Q23, как CPU оригинала: knob<<16) -------------
        voice.setFilter ((int) apvts.getRawParameterValue ("base")->load(),
                         (int) apvts.getRawParameterValue ("wdth")->load(),
                         (int) apvts.getRawParameterValue ("hpq")->load(),
                         (int) apvts.getRawParameterValue ("lpq")->load());
        voice.setAmp ((int) apvts.getRawParameterValue ("atk")->load(),
                      (int) apvts.getRawParameterValue ("hold")->load(),
                      (int) apvts.getRawParameterValue ("dec")->load(),
                      (int) apvts.getRawParameterValue ("rel")->load(),
                      (int) apvts.getRawParameterValue ("vol")->load(),
                      (int) apvts.getRawParameterValue ("pan")->load());

        // --- обработка: порубить на кадры по 16 сэмплов ---------------------
        float* L = buffer.getWritePointer (0);
        float* R = buffer.getWritePointer (1);
        const int n = buffer.getNumSamples();

        int pos = 0;
        while (pos < n)
        {
            const int cnt = juce::jmin (16 - frameFill, n - pos);
            for (int i = 0; i < cnt; ++i)
            {
                frameL[frameFill + i] = L[pos + i];
                frameR[frameFill + i] = R[pos + i];
            }
            frameFill += cnt;
            pos       += cnt;
            if (frameFill == 16)
            {
                for (int i = 0; i < 16; ++i)     // в interleaved для voice
                {
                    io[2 * i]     = frameL[i];
                    io[2 * i + 1] = frameR[i];
                }
                voice.process (io, 16);
                const int base = pos - 16;
                for (int i = 0; i < 16; ++i)
                {
                    L[base + i] = io[2 * i];
                    R[base + i] = io[2 * i + 1];
                }
                frameFill = 0;
            }
        }
    }

    // ==== служебное (boilerplate AudioProcessor) ==============================
    const juce::String getName() const override { return "MnmDualFilter"; }
    double getTailLengthSeconds() const override { return 0.0; }
    bool acceptsMidi() const override  { return true; }
    bool producesMidi() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}
    void getStateInformation (juce::MemoryBlock& dest) override
    {
        if (auto st = apvts.copyState().createXml())
            copyXmlToBinary (*st, dest);
    }
    void setStateInformation (const void* data, int size) override
    {
        if (auto st = getXmlFromBinary (data, size))
            apvts.replaceState (juce::ValueTree::fromXml (*st));
    }

private:
    juce::AudioProcessorValueTreeState apvts;
    mnm::MonoMachineTrackVoice voice;
    float frameL[16] {}, frameR[16] {}, io[32] {};
    int   frameFill = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MnmFilterAudioProcessor)
};
