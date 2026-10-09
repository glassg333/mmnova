#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>

namespace
{
constexpr float kFadeOutSeconds = 0.006f; // уход со старой страницы
constexpr float kFadeInSeconds  = 0.025f; // приход на новую
constexpr float kTwoPi          = 6.283185307179586f;
}

CombScannerLabAudioProcessor::CombScannerLabAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      engines (makeEngines()),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout (engines))
{
    cacheSlotPointers (engines);

    for (int slot = 0; slot < numSlots; ++slot)
        if (engines[(size_t) slot] != nullptr)
            runtime[(size_t) slot].lastModel = -1;

    currentSlot = juce::jlimit (0, numSlots - 1, getActiveSlot());
}

CombScannerLabAudioProcessor::~CombScannerLabAudioProcessor() = default;

CombScannerLabAudioProcessor::EngineArray CombScannerLabAudioProcessor::makeEngines()
{
    EngineArray result;

    for (int i = 0; i < numSlots; ++i)
        result[(size_t) i].reset (lab::createEngine (i));

    return result;
}

juce::AudioProcessorValueTreeState::ParameterLayout
CombScannerLabAudioProcessor::createParameterLayout (const EngineArray& enginesInOrder)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto addFloat = [&layout] (const juce::String& id, const juce::String& name,
                               float minimum, float maximum, float step, float defaultValue,
                               const juce::String& unit)
    {
        layout.add (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { id, 1 },
            name,
            juce::NormalisableRange<float> (minimum, maximum, step),
            defaultValue,
            juce::AudioParameterFloatAttributes().withLabel (unit)));
    };

    for (int slot = 0; slot < numSlots; ++slot)
    {
        const auto* engine = enginesInOrder[(size_t) slot].get();

        if (engine == nullptr)
            continue;

        const auto* table = engine->params();
        const int count = engine->numParams();
        const juce::String prefix = "S" + juce::String (slot + 1) + " ";

        for (int i = 0; i < count; ++i)
        {
            const auto& info = table[i];
            addFloat (lab::slotParamId (slot, info.key),
                      prefix + info.name,
                      info.min, info.max, info.step, info.def,
                      info.unit != nullptr ? juce::String (info.unit) : juce::String());
        }

        if (engine->numModels() > 0)
        {
            juce::StringArray items;

            for (int m = 0; m < engine->numModels(); ++m)
                items.add (engine->modelName (m));

            layout.add (std::make_unique<juce::AudioParameterChoice> (
                juce::ParameterID { lab::slotModelId (slot), 1 },
                prefix + "Модель",
                items,
                0));
        }

        addFloat (lab::slotTrimId (slot), prefix + "Trim", -24.0f, 24.0f, 0.1f, 0.0f, "dB");
    }

    layout.add (std::make_unique<juce::AudioParameterInt> (
        juce::ParameterID { activeSlotId, 1 }, "Активная страница", 0, numSlots - 1, 0));

    addFloat (mixId,       "Dry/Wet",     0.0f, 1.0f, 0.001f, 1.0f,  "");
    addFloat (outputId,    "Output",     -24.0f, 24.0f, 0.1f,  0.0f, "dB");
    addFloat (scanRateId,  "Scan Rate",   0.0f, 2.0f, 0.001f, 0.09f, "Hz");
    addFloat (scanDepthId, "Scan Depth",  0.0f, 1.0f, 0.001f, 0.55f, "");

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { resetSwitchId, 1 }, "Сброс при переключении", true));

    return layout;
}

void CombScannerLabAudioProcessor::cacheSlotPointers (const EngineArray& enginesInOrder)
{
    pSlot = apvts.getRawParameterValue (activeSlotId);
    pMix = apvts.getRawParameterValue (mixId);
    pOutput = apvts.getRawParameterValue (outputId);
    pScanRate = apvts.getRawParameterValue (scanRateId);
    pScanDepth = apvts.getRawParameterValue (scanDepthId);
    pResetOnSwitch = apvts.getRawParameterValue (resetSwitchId);

    for (int slot = 0; slot < numSlots; ++slot)
    {
        auto* engine = enginesInOrder[(size_t) slot].get();

        if (engine == nullptr)
            continue;

        auto& rt = runtime[(size_t) slot];
        rt.values.clear();

        for (int i = 0; i < engine->numParams(); ++i)
        {
            rt.values.push_back (apvts.getRawParameterValue (lab::slotParamId (slot, engine->params()[i].key)));

            if (juce::String (engine->params()[i].key) == "scan")
                rt.scanIndex = i;
        }

        if (engine->numModels() > 0)
            rt.model = apvts.getRawParameterValue (lab::slotModelId (slot));

        rt.trim = apvts.getRawParameterValue (lab::slotTrimId (slot));
    }
}

float CombScannerLabAudioProcessor::readParam (const std::atomic<float>* value, float fallback) const
{
    return value != nullptr ? value->load() : fallback;
}

void CombScannerLabAudioProcessor::applySlotParameters (int slot, float scanOverride)
{
    auto* engine = engines[(size_t) slot].get();

    if (engine == nullptr)
        return;

    auto& rt = runtime[(size_t) slot];
    const auto* table = engine->params();

    for (int i = 0; i < engine->numParams(); ++i)
    {
        const auto* ptr = (i < (int) rt.values.size()) ? rt.values[(size_t) i] : nullptr;
        float value = readParam (ptr, table[i].def);

        if (i == rt.scanIndex && scanOverride >= 0.0f)
            value = scanOverride;

        engine->setParam (i, juce::jlimit (table[i].min, table[i].max, value));
    }

    if (rt.model != nullptr)
    {
        const int model = juce::jlimit (0, engine->numModels() - 1,
                                        (int) std::lround (rt.model->load()));

        if (model != rt.lastModel || rt.lastModel < 0)
        {
            engine->setModel (model);
            rt.lastModel = model;
        }
    }
}

void CombScannerLabAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 48000.0;

    for (auto& prepared : slotPrepared)
        prepared = false;

    preparedBlockSize = juce::jmax (samplesPerBlock, 1);
    dryBuffer.setSize (2, preparedBlockSize, false, false, true);
    dryBuffer.clear();

    currentSlot = juce::jlimit (0, numSlots - 1, getActiveSlot());
    fadeState = FadeState::steady;
    fadeGain = 1.0f;
    lastLoudnessGain = 1.0f;
    scanPhase = 0.0;

    if (auto* engine = engines[(size_t) currentSlot].get())
    {
        engine->prepare (currentSampleRate, samplesPerBlock);
        engine->reset();
        slotPrepared[(size_t) currentSlot] = true;
        applySlotParameters (currentSlot, -1.0f);
    }
}

void CombScannerLabAudioProcessor::releaseResources()
{
    for (int slot = 0; slot < numSlots; ++slot)
    {
        if (engines[(size_t) slot] != nullptr)
            engines[(size_t) slot]->reset();

        slotPrepared[(size_t) slot] = false;
    }
}

bool CombScannerLabAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    const auto output = layouts.getMainOutputChannelSet();

    if (input != output)
        return false;

    return input == juce::AudioChannelSet::mono() || input == juce::AudioChannelSet::stereo();
}

void CombScannerLabAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = juce::jmin (2, buffer.getNumChannels());

    if (numSamples <= 0 || numChannels <= 0)
        return;

    for (int ch = numChannels; ch < buffer.getNumChannels(); ++ch)
        buffer.clear (ch, 0, numSamples);

    const int requestedSlot = juce::jlimit (0, numSlots - 1, getActiveSlot());

    // --- переключение страниц с коротким фейдом ---------------------------------
    const float fadeStart = fadeGain;

    if (fadeState == FadeState::steady && requestedSlot != currentSlot)
        fadeState = FadeState::out;

    if (fadeState == FadeState::out)
    {
        fadeGain -= (float) numSamples / (float) (kFadeOutSeconds * currentSampleRate);

        if (fadeGain <= 0.0f)
        {
            fadeGain = 0.0f;
            currentSlot = requestedSlot;

            if (readParam (pResetOnSwitch, 1.0f) > 0.5f)
                if (auto* engine = engines[(size_t) currentSlot].get())
                    engine->reset();

            fadeState = FadeState::in;
        }
    }
    else if (fadeState == FadeState::in)
    {
        fadeGain += (float) numSamples / (float) (kFadeInSeconds * currentSampleRate);

        if (fadeGain >= 1.0f)
        {
            fadeGain = 1.0f;
            fadeState = FadeState::steady;
        }
    }

    auto* engine = engines[(size_t) currentSlot].get();

    if (engine == nullptr)
        return;

    if (! slotPrepared[(size_t) currentSlot])
    {
        engine->prepare (currentSampleRate, juce::jmax (numSamples, preparedBlockSize));
        engine->reset();
        slotPrepared[(size_t) currentSlot] = true;
        runtime[(size_t) currentSlot].lastModel = -1;
    }

    // --- Scan + LFO --------------------------------------------------------------
    const auto& rt = runtime[(size_t) currentSlot];
    float scanOverride = -1.0f;

    if (rt.scanIndex >= 0 && rt.scanIndex < (int) rt.values.size())
    {
        const float base = readParam (rt.values[(size_t) rt.scanIndex], 0.0f);
        const float depth = juce::jlimit (0.0f, 1.0f, readParam (pScanDepth, 0.0f));
        const float rate = juce::jmax (0.0f, readParam (pScanRate, 0.0f));

        scanPhase += (double) rate * (double) numSamples / currentSampleRate;

        if (scanPhase >= 1.0)
            scanPhase -= std::floor (scanPhase);

        const float lfo = std::sin ((float) scanPhase * kTwoPi);
        const float centre = 0.5f;
        scanOverride = juce::jlimit (0.0f, 1.0f, centre + (base - centre) * (1.0f - depth) + 0.5f * depth * lfo);
    }

    applySlotParameters (currentSlot, scanOverride);

    // --- аудио --------------------------------------------------------------------
    const bool needDry = readParam (pMix, 1.0f) < 0.999f;

    if (needDry)
    {
        if (dryBuffer.getNumChannels() < numChannels || dryBuffer.getNumSamples() < numSamples)
        {
            preparedBlockSize = juce::jmax (preparedBlockSize, numSamples);
            dryBuffer.setSize (numChannels, preparedBlockSize, false, false, true);
        }

        for (int ch = 0; ch < numChannels; ++ch)
            dryBuffer.copyFrom (ch, 0, buffer, ch, 0, numSamples);
    }

    engine->process (buffer.getArrayOfWritePointers(), numChannels, numSamples);

    const float mix = juce::jlimit (0.0f, 1.0f, readParam (pMix, 1.0f));

    if (needDry)
    {
        buffer.applyGain (0, numSamples, mix);

        for (int ch = 0; ch < numChannels; ++ch)
            buffer.addFrom (ch, 0, dryBuffer, ch, 0, numSamples, 1.0f - mix);
    }

    // фейд переключения страниц
    if (fadeStart < 1.0f || fadeGain < 1.0f)
        buffer.applyGainRamp (0, numSamples, fadeStart, fadeGain);

    // громкость: Trim страницы + общий Output
    const float trimDb = readParam (rt.trim, 0.0f);
    const float outputDb = readParam (pOutput, 0.0f);
    const float targetGain = juce::Decibels::decibelsToGain (trimDb + outputDb);

    buffer.applyGainRamp (0, numSamples, lastLoudnessGain, targetGain);
    lastLoudnessGain = targetGain;
}

int CombScannerLabAudioProcessor::getActiveSlot() const
{
    return juce::jlimit (0, numSlots - 1,
                         (int) std::lround (readParam (pSlot, 0.0f)));
}

void CombScannerLabAudioProcessor::setActiveSlot (int slot)
{
    slot = juce::jlimit (0, numSlots - 1, slot);

    if (auto* param = apvts.getParameter (activeSlotId))
        param->setValueNotifyingHost (param->convertTo0to1 ((float) slot));
}

void CombScannerLabAudioProcessor::copySlotToAll (int slot)
{
    slot = juce::jlimit (0, numSlots - 1, slot);
    auto* source = engines[(size_t) slot].get();

    if (source == nullptr)
        return;

    const auto& sourceRuntime = runtime[(size_t) slot];

    for (int i = 0; i < source->numParams(); ++i)
    {
        const auto& info = source->params()[i];
        const float value = readParam (i < (int) sourceRuntime.values.size() ? sourceRuntime.values[(size_t) i] : nullptr,
                                       info.def);

        for (int target = 0; target < numSlots; ++target)
        {
            if (target == slot)
                continue;

            auto* destination = engines[(size_t) target].get();

            if (destination == nullptr)
                continue;

            for (int j = 0; j < destination->numParams(); ++j)
            {
                const auto& targetInfo = destination->params()[j];

                if (juce::String (targetInfo.key) != juce::String (info.key))
                    continue;

                if (auto* param = apvts.getParameter (lab::slotParamId (target, targetInfo.key)))
                    param->setValueNotifyingHost (param->convertTo0to1 (
                        juce::jlimit (targetInfo.min, targetInfo.max, value)));
            }
        }
    }

    const int sourceModels = source->numModels();

    for (int target = 0; target < numSlots; ++target)
    {
        if (target == slot || engines[(size_t) target] == nullptr)
            continue;

        if (sourceModels > 0 && engines[(size_t) target]->numModels() > 0)
        {
            const int model = juce::jlimit (0, engines[(size_t) target]->numModels() - 1,
                                            (int) std::lround (readParam (runtime[(size_t) slot].model, 0.0f)));

            if (auto* param = apvts.getParameter (lab::slotModelId (target)))
                param->setValueNotifyingHost (param->convertTo0to1 ((float) model));
        }

        if (auto* param = apvts.getParameter (lab::slotTrimId (target)))
        {
            const float trim = readParam (runtime[(size_t) slot].trim, 0.0f);
            param->setValueNotifyingHost (param->convertTo0to1 (juce::jlimit (-24.0f, 24.0f, trim)));
        }
    }
}

void CombScannerLabAudioProcessor::resetSlotToDefaults (int slot)
{
    slot = juce::jlimit (0, numSlots - 1, slot);
    auto* engine = engines[(size_t) slot].get();

    if (engine == nullptr)
        return;

    for (int i = 0; i < engine->numParams(); ++i)
    {
        const auto& info = engine->params()[i];

        if (auto* param = apvts.getParameter (lab::slotParamId (slot, info.key)))
            param->setValueNotifyingHost (param->convertTo0to1 (info.def));
    }

    if (engine->numModels() > 0)
        if (auto* param = apvts.getParameter (lab::slotModelId (slot)))
            param->setValueNotifyingHost (0.0f);

    if (auto* param = apvts.getParameter (lab::slotTrimId (slot)))
        param->setValueNotifyingHost (param->convertTo0to1 (0.0f));
}

void CombScannerLabAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void CombScannerLabAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessorEditor* CombScannerLabAudioProcessor::createEditor()
{
    return new CombScannerLabAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CombScannerLabAudioProcessor();
}
