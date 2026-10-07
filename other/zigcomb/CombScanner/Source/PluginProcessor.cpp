#include "PluginProcessor.h"

#include "PluginEditor.h"

#include <cmath>

namespace
{
float safeLoad(const std::atomic<float>* value, float fallback) noexcept
{
    return value != nullptr ? value->load(std::memory_order_relaxed) : fallback;
}
int asIndex(float value) noexcept
{
    return static_cast<int>(std::lround(value));
}
}

CombScannerAudioProcessor::CombScannerAudioProcessor()
    : juce::AudioProcessor(BusesProperties()
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "COMBSCANNER_STATE", combscanner::createParameterLayout())
{
    cacheParameterPointers();
}

void CombScannerAudioProcessor::cacheParameterPointers()
{
    const auto get = [this](const juce::String& id) { return parameters.getRawParameterValue(id); };
    raw.mode = get("global_mode");
    raw.model = get("global_model");
    raw.bypass = get("global_bypass");
    raw.classicFeedback = get("global_feedback");
    raw.classicDamp = get("global_damp");
    raw.classicPhase = get("global_phase");
    raw.classicDelay1 = get("global_delay1");
    raw.classicDelay2 = get("global_delay2");
    raw.scan = get("global_scan");
    raw.scanWidth = get("global_scan_width");
    raw.scanRate = get("global_scan_rate");
    raw.scanShape = get("global_scan_shape");
    raw.diffusion = get("global_diffusion");
    raw.crossMix = get("global_crossmix");
    raw.motion = get("global_motion");
    raw.dryWet = get("global_mix");
    raw.output = get("global_output");
    raw.stereoWidth = get("global_width");

    for (int i = 1; i <= combscanner::kNumCombs; ++i)
    {
        auto& c = raw.combs[static_cast<std::size_t>(i - 1)];
        c.enabled = get(combscanner::ParamIDs::comb(i, "on"));
        c.ratio = get(combscanner::ParamIDs::comb(i, "ratio"));
        c.ratio2 = get(combscanner::ParamIDs::comb(i, "ratio2"));
        c.delay1 = get(combscanner::ParamIDs::comb(i, "delay1"));
        c.delay2 = get(combscanner::ParamIDs::comb(i, "delay2"));
        c.feedback = get(combscanner::ParamIDs::comb(i, "feedback"));
        c.damp = get(combscanner::ParamIDs::comb(i, "damp"));
        c.phase = get(combscanner::ParamIDs::comb(i, "phase"));
        c.diffusion = get(combscanner::ParamIDs::comb(i, "diffusion"));
        c.crossMix = get(combscanner::ParamIDs::comb(i, "crossmix"));
        c.drive = get(combscanner::ParamIDs::comb(i, "drive"));
        c.level = get(combscanner::ParamIDs::comb(i, "level"));
        c.pan = get(combscanner::ParamIDs::comb(i, "pan"));
        c.scanWeight = get(combscanner::ParamIDs::comb(i, "scanweight"));
    }

    for (int i = 1; i <= combscanner::kNumFxSlots; ++i)
    {
        auto& fx = raw.fxSlots[static_cast<std::size_t>(i - 1)];
        fx.type = get(combscanner::ParamIDs::fx(i, "type"));
        fx.position = get(combscanner::ParamIDs::fx(i, "position"));
        fx.mix = get(combscanner::ParamIDs::fx(i, "mix"));
        fx.size = get(combscanner::ParamIDs::fx(i, "size"));
        fx.pitch = get(combscanner::ParamIDs::fx(i, "pitch"));
        fx.blur = get(combscanner::ParamIDs::fx(i, "blur"));
        fx.freeze = get(combscanner::ParamIDs::fx(i, "freeze"));
    }

    for (int i = 1; i <= combscanner::kNumLfos; ++i)
    {
        auto& lfo = raw.lfos[static_cast<std::size_t>(i - 1)];
        lfo.rate = get(combscanner::ParamIDs::lfo(i, "rate"));
        lfo.depth = get(combscanner::ParamIDs::lfo(i, "depth"));
        lfo.shape = get(combscanner::ParamIDs::lfo(i, "shape"));
        lfo.sync = get(combscanner::ParamIDs::lfo(i, "sync"));
        lfo.division = get(combscanner::ParamIDs::lfo(i, "division"));
    }

    for (int i = 1; i <= combscanner::kNumSequences; ++i)
    {
        auto& seq = raw.sequences[static_cast<std::size_t>(i - 1)];
        seq.rate = get(combscanner::ParamIDs::sequence(i, "rate"));
        seq.slew = get(combscanner::ParamIDs::sequence(i, "slew"));
        seq.length = get(combscanner::ParamIDs::sequence(i, "length"));
        seq.mode = get(combscanner::ParamIDs::sequence(i, "mode"));
        seq.sync = get(combscanner::ParamIDs::sequence(i, "sync"));
        seq.division = get(combscanner::ParamIDs::sequence(i, "division"));
        for (int step = 1; step <= combscanner::kSequenceSteps; ++step)
            seq.steps[static_cast<std::size_t>(step - 1)] = get(combscanner::ParamIDs::sequence(i, "step" + juce::String(step)));
    }

    for (int i = 1; i <= combscanner::kNumModRoutes; ++i)
    {
        auto& route = raw.routes[static_cast<std::size_t>(i - 1)];
        route.source = get(combscanner::ParamIDs::route(i, "source"));
        route.destination = get(combscanner::ParamIDs::route(i, "destination"));
        route.amount = get(combscanner::ParamIDs::route(i, "amount"));
    }
}

float CombScannerAudioProcessor::load(const std::atomic<float>* parameter, float fallback) const noexcept
{
    return safeLoad(parameter, fallback);
}

combscanner::EngineParameters CombScannerAudioProcessor::readEngineParameters() const noexcept
{
    auto p = combscanner::makeDefaultEngineParameters();
    p.mode = asIndex(load(raw.mode, 0.0f));
    p.model = asIndex(load(raw.model, 0.0f));
    p.bypass = load(raw.bypass) > 0.5f;
    p.classicFeedback = load(raw.classicFeedback, 0.80f);
    p.classicDamp = load(raw.classicDamp, 0.90f);
    p.classicPhase = load(raw.classicPhase, 0.75f);
    p.classicDelay1Ms = load(raw.classicDelay1, 115.0f);
    p.classicDelay2Ms = load(raw.classicDelay2, 500.0f);
    p.scan = load(raw.scan, 0.0f);
    p.scanWidth = load(raw.scanWidth, 0.22f);
    p.scanRateHz = load(raw.scanRate, 0.0f);
    p.scanShape = asIndex(load(raw.scanShape, 0.0f));
    p.globalDiffusion = load(raw.diffusion, 0.55f);
    p.globalCrossMix = load(raw.crossMix, 0.50f);
    p.motion = load(raw.motion, 0.45f);
    p.dryWet = load(raw.dryWet, 0.82f);
    p.outputDb = load(raw.output, 0.0f);
    p.stereoWidth = load(raw.stereoWidth, 1.0f);
    p.midiWheel = midiWheel;
    p.midiVelocity = midiVelocity;

    for (int i = 0; i < combscanner::kNumCombs; ++i)
    {
        const auto& source = raw.combs[static_cast<std::size_t>(i)];
        auto& destination = p.combs[static_cast<std::size_t>(i)];
        destination.enabled = load(source.enabled, 1.0f) > 0.5f;
        destination.ratio = load(source.ratio, destination.ratio);
        destination.ratio2 = load(source.ratio2, destination.ratio2);
        destination.delay1Ms = load(source.delay1, destination.delay1Ms);
        destination.delay2Ms = load(source.delay2, destination.delay2Ms);
        destination.feedback = load(source.feedback, destination.feedback);
        destination.damp = load(source.damp, destination.damp);
        destination.phase = load(source.phase, destination.phase);
        destination.diffusion = load(source.diffusion, destination.diffusion);
        destination.crossMix = load(source.crossMix, destination.crossMix);
        destination.driveDb = load(source.drive, destination.driveDb);
        destination.levelDb = load(source.level, destination.levelDb);
        destination.pan = load(source.pan, destination.pan);
        destination.scanWeight = load(source.scanWeight, destination.scanWeight);
    }

    for (int i = 0; i < combscanner::kNumFxSlots; ++i)
    {
        const auto& source = raw.fxSlots[static_cast<std::size_t>(i)];
        auto& destination = p.fxSlots[static_cast<std::size_t>(i)];
        destination.type = asIndex(load(source.type, 0.0f));
        destination.afterComb = asIndex(load(source.position, static_cast<float>(i))) + 1;
        destination.mix = load(source.mix, destination.mix);
        destination.size = load(source.size, destination.size);
        destination.pitch = load(source.pitch, destination.pitch);
        destination.blur = load(source.blur, destination.blur);
        destination.freeze = load(source.freeze) > 0.5f;
    }

    for (int i = 0; i < combscanner::kNumLfos; ++i)
    {
        const auto& source = raw.lfos[static_cast<std::size_t>(i)];
        auto& destination = p.lfos[static_cast<std::size_t>(i)];
        destination.rateHz = load(source.rate, destination.rateHz);
        destination.depth = load(source.depth, destination.depth);
        destination.shape = asIndex(load(source.shape, 0.0f));
        destination.tempoSync = load(source.sync) > 0.5f;
        destination.division = asIndex(load(source.division, 2.0f));
    }

    for (int i = 0; i < combscanner::kNumSequences; ++i)
    {
        const auto& source = raw.sequences[static_cast<std::size_t>(i)];
        auto& destination = p.sequences[static_cast<std::size_t>(i)];
        destination.stepRateHz = load(source.rate, destination.stepRateHz);
        destination.slew = load(source.slew, destination.slew);
        destination.length = asIndex(load(source.length, static_cast<float>(combscanner::kSequenceSteps)));
        destination.mode = asIndex(load(source.mode, 0.0f));
        destination.tempoSync = load(source.sync) > 0.5f;
        destination.division = asIndex(load(source.division, 0.0f));
        for (int step = 0; step < combscanner::kSequenceSteps; ++step)
            destination.steps[static_cast<std::size_t>(step)] = load(source.steps[static_cast<std::size_t>(step)], 0.0f);
    }

    for (int i = 0; i < combscanner::kNumModRoutes; ++i)
    {
        const auto& source = raw.routes[static_cast<std::size_t>(i)];
        auto& destination = p.routes[static_cast<std::size_t>(i)];
        destination.source = asIndex(load(source.source, 0.0f));
        destination.destination = asIndex(load(source.destination, 0.0f));
        destination.amount = load(source.amount, 0.0f);
    }
    return p;
}

void CombScannerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    engine.prepare(sampleRate, samplesPerBlock);
    engine.setParameters(readEngineParameters());
    lastReportedLatency = engine.getLatencySamples();
    setLatencySamples(lastReportedLatency);
}

void CombScannerAudioProcessor::releaseResources()
{
    engine.reset();
}

bool CombScannerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    const auto output = layouts.getMainOutputChannelSet();
    if (input != output)
        return false;
    return input == juce::AudioChannelSet::mono() || input == juce::AudioChannelSet::stereo();
}

void CombScannerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    for (const auto metadata : midiMessages)
    {
        const auto message = metadata.getMessage();
        if (message.isController() && message.getControllerNumber() == 1)
            midiWheel = static_cast<float>(message.getControllerValue()) / 127.0f;
        else if (message.isNoteOn())
            midiVelocity = message.getFloatVelocity();
        else if (message.isNoteOff())
            midiVelocity = 0.0f;
    }

    if (auto* playHead = getPlayHead())
    {
        if (const auto position = playHead->getPosition())
            if (const auto bpm = position->getBpm())
                currentTempoBpm = *bpm;
    }

    const auto currentParameters = readEngineParameters();
    engine.setParameters(currentParameters);
    const int latency = engine.getLatencySamples();
    if (latency != lastReportedLatency)
    {
        lastReportedLatency = latency;
        setLatencySamples(lastReportedLatency);
    }

    const int channels = buffer.getNumChannels();
    if (channels <= 0)
        return;
    auto* left = buffer.getWritePointer(0);
    auto* right = channels > 1 ? buffer.getWritePointer(1) : nullptr;
    engine.processBlock(left, right, buffer.getNumSamples(), currentTempoBpm);
    for (int channel = 2; channel < channels; ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());
}

double CombScannerAudioProcessor::getTailLengthSeconds() const
{
    return engine.getTailLengthSeconds();
}

juce::AudioProcessorEditor* CombScannerAudioProcessor::createEditor()
{
    return new CombScannerAudioProcessorEditor(*this);
}

int CombScannerAudioProcessor::getCurrentProgram()
{
    const auto mode = asIndex(load(raw.mode, 0.0f));
    return mode == static_cast<int>(combscanner::DspMode::moreControl)
        ? 10 : std::clamp(asIndex(load(raw.model, 0.0f)), 0, 9);
}

const juce::String CombScannerAudioProcessor::getProgramName(int index)
{
    if (index == 10)
        return "More Control - Manual Ratios";
    const auto names = combscanner::getModelNames();
    return names[std::clamp(index, 0, names.size() - 1)];
}

void CombScannerAudioProcessor::setCurrentProgram(int index)
{
    loadFactoryPreset(std::clamp(index, 0, getNumPrograms() - 1));
}

void CombScannerAudioProcessor::setParameterActual(const juce::String& parameterID, float actualValue)
{
    if (auto* parameter = parameters.getParameter(parameterID))
        parameter->setValueNotifyingHost(parameter->convertTo0to1(actualValue));
}

void CombScannerAudioProcessor::loadFactoryPreset(int index)
{
    auto p = combscanner::makeDefaultEngineParameters();
    p.mode = index == 10 ? static_cast<int>(combscanner::DspMode::moreControl)
                         : static_cast<int>(combscanner::DspMode::classicFav);
    p.model = index == 10 ? 0 : index;
    if (index == 3) // Scrub Stretch
    {
        p.scanWidth = 0.34f;
        p.motion = 0.78f;
        p.scanRateHz = 0.08f;
    }
    else if (index == 4) // Phase Cloud
    {
        p.globalDiffusion = 0.82f;
        p.globalCrossMix = 0.58f;
        p.stereoWidth = 1.0f;
    }
    else if (index == 5) // Ping Pong
    {
        p.scanWidth = 0.45f;
        for (int i = 0; i < combscanner::kNumCombs; ++i)
            p.combs[static_cast<std::size_t>(i)].pan = (i % 2 == 0) ? -0.8f : 0.8f;
    }
    else if (index == 6) // Dark Bloom
    {
        p.classicFeedback = 0.92f;
        p.classicDamp = 0.97f;
        p.dryWet = 0.90f;
    }
    else if (index == 8) // Unstable Edge, kept below runaway feedback.
    {
        p.classicFeedback = 0.98f;
        p.scanWidth = 0.12f;
    }
    else if (index == 10) // More Control manual ratios.
    {
        p.dryWet = 0.82f;
        p.scanWidth = 0.30f;
        p.motion = 0.55f;
    }

    setParameterActual("global_mode", static_cast<float>(p.mode));
    setParameterActual("global_model", static_cast<float>(p.model));
    setParameterActual("global_bypass", p.bypass ? 1.0f : 0.0f);
    setParameterActual("global_feedback", p.classicFeedback);
    setParameterActual("global_damp", p.classicDamp);
    setParameterActual("global_phase", p.classicPhase);
    setParameterActual("global_delay1", p.classicDelay1Ms);
    setParameterActual("global_delay2", p.classicDelay2Ms);
    setParameterActual("global_scan", p.scan);
    setParameterActual("global_scan_width", p.scanWidth);
    setParameterActual("global_scan_rate", p.scanRateHz);
    setParameterActual("global_scan_shape", static_cast<float>(p.scanShape));
    setParameterActual("global_diffusion", p.globalDiffusion);
    setParameterActual("global_crossmix", p.globalCrossMix);
    setParameterActual("global_motion", p.motion);
    setParameterActual("global_mix", p.dryWet);
    setParameterActual("global_output", p.outputDb);
    setParameterActual("global_width", p.stereoWidth);

    for (int i = 1; i <= combscanner::kNumCombs; ++i)
    {
        const auto& c = p.combs[static_cast<std::size_t>(i - 1)];
        setParameterActual(combscanner::ParamIDs::comb(i, "on"), c.enabled ? 1.0f : 0.0f);
        setParameterActual(combscanner::ParamIDs::comb(i, "ratio"), c.ratio);
        setParameterActual(combscanner::ParamIDs::comb(i, "ratio2"), c.ratio2);
        setParameterActual(combscanner::ParamIDs::comb(i, "delay1"), c.delay1Ms);
        setParameterActual(combscanner::ParamIDs::comb(i, "delay2"), c.delay2Ms);
        setParameterActual(combscanner::ParamIDs::comb(i, "feedback"), c.feedback);
        setParameterActual(combscanner::ParamIDs::comb(i, "damp"), c.damp);
        setParameterActual(combscanner::ParamIDs::comb(i, "phase"), c.phase);
        setParameterActual(combscanner::ParamIDs::comb(i, "diffusion"), c.diffusion);
        setParameterActual(combscanner::ParamIDs::comb(i, "crossmix"), c.crossMix);
        setParameterActual(combscanner::ParamIDs::comb(i, "drive"), c.driveDb);
        setParameterActual(combscanner::ParamIDs::comb(i, "level"), c.levelDb);
        setParameterActual(combscanner::ParamIDs::comb(i, "pan"), c.pan);
        setParameterActual(combscanner::ParamIDs::comb(i, "scanweight"), c.scanWeight);
    }

    for (int i = 1; i <= combscanner::kNumFxSlots; ++i)
    {
        const auto& fx = p.fxSlots[static_cast<std::size_t>(i - 1)];
        setParameterActual(combscanner::ParamIDs::fx(i, "type"), static_cast<float>(fx.type));
        setParameterActual(combscanner::ParamIDs::fx(i, "position"), static_cast<float>(fx.afterComb - 1));
        setParameterActual(combscanner::ParamIDs::fx(i, "mix"), fx.mix);
        setParameterActual(combscanner::ParamIDs::fx(i, "size"), fx.size);
        setParameterActual(combscanner::ParamIDs::fx(i, "pitch"), fx.pitch);
        setParameterActual(combscanner::ParamIDs::fx(i, "blur"), fx.blur);
        setParameterActual(combscanner::ParamIDs::fx(i, "freeze"), fx.freeze ? 1.0f : 0.0f);
    }

    for (int i = 1; i <= combscanner::kNumLfos; ++i)
    {
        const auto& lfo = p.lfos[static_cast<std::size_t>(i - 1)];
        setParameterActual(combscanner::ParamIDs::lfo(i, "rate"), lfo.rateHz);
        setParameterActual(combscanner::ParamIDs::lfo(i, "depth"), lfo.depth);
        setParameterActual(combscanner::ParamIDs::lfo(i, "shape"), static_cast<float>(lfo.shape));
        setParameterActual(combscanner::ParamIDs::lfo(i, "sync"), lfo.tempoSync ? 1.0f : 0.0f);
        setParameterActual(combscanner::ParamIDs::lfo(i, "division"), static_cast<float>(lfo.division));
    }

    for (int i = 1; i <= combscanner::kNumSequences; ++i)
    {
        const auto& seq = p.sequences[static_cast<std::size_t>(i - 1)];
        setParameterActual(combscanner::ParamIDs::sequence(i, "rate"), seq.stepRateHz);
        setParameterActual(combscanner::ParamIDs::sequence(i, "slew"), seq.slew);
        setParameterActual(combscanner::ParamIDs::sequence(i, "length"), static_cast<float>(seq.length));
        setParameterActual(combscanner::ParamIDs::sequence(i, "mode"), static_cast<float>(seq.mode));
        setParameterActual(combscanner::ParamIDs::sequence(i, "sync"), seq.tempoSync ? 1.0f : 0.0f);
        setParameterActual(combscanner::ParamIDs::sequence(i, "division"), static_cast<float>(seq.division));
        for (int step = 1; step <= combscanner::kSequenceSteps; ++step)
            setParameterActual(combscanner::ParamIDs::sequence(i, "step" + juce::String(step)),
                               seq.steps[static_cast<std::size_t>(step - 1)]);
    }

    for (int i = 1; i <= combscanner::kNumModRoutes; ++i)
    {
        const auto& route = p.routes[static_cast<std::size_t>(i - 1)];
        setParameterActual(combscanner::ParamIDs::route(i, "source"), static_cast<float>(route.source));
        setParameterActual(combscanner::ParamIDs::route(i, "destination"), static_cast<float>(route.destination));
        setParameterActual(combscanner::ParamIDs::route(i, "amount"), route.amount);
    }
}

void CombScannerAudioProcessor::getStateInformation(juce::MemoryBlock& destinationData)
{
    if (auto xml = parameters.copyState().createXml())
        copyXmlToBinary(*xml, destinationData);
}

void CombScannerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CombScannerAudioProcessor();
}
