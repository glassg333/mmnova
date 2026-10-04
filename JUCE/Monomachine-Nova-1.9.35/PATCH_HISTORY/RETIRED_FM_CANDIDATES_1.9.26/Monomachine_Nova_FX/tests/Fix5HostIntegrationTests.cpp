// Host-level registry/migration regression for the FM+ MODE SYNT cleanup.
// It runs against the actual processor target in both products and verifies
// the six retained choices plus raw-ID 6..8 normalization to MNM FRQ.
// The former imported Fix-5 audio-route helpers remain below as dormant
// historical test material and are intentionally not invoked by main().
#include "PluginProcessor.h"

#include <array>
#include <cmath>
#include <cstdio>
#include <stdexcept>

namespace
{
using Processor = MonomachineNovaAudioProcessor;
constexpr std::array<int, 3> kFix5Machines { 8, 9, 10 };

struct StereoEnergy
{
    double left = 0.0, right = 0.0;
    double total() const noexcept { return left + right; }
};

void require(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

void set(Processor& processor, const char* id, float value)
{
    auto* parameter = processor.parameters.getParameter(id);
    require(parameter != nullptr, id);
    parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
}

void set(Processor& processor, const juce::String& id, float value)
{
    set(processor, id.toRawUTF8(), value);
}

float valueOf(const Processor& processor, const juce::String& id)
{
    const auto* value = processor.parameters.getRawParameterValue(id);
    require(value != nullptr, id.toRawUTF8());
    return value->load();
}

int machineListIndex(int machineId)
{
    for (size_t i = 0; i < nova::machines().size(); ++i)
        if (nova::machines()[i].id == machineId) return static_cast<int>(i);
    throw std::runtime_error("required FM+ machine is absent");
}

juce::String modeId(int machineId)
{
    return monomachine::dspMachineModeParamId(machineId);
}

juce::MemoryBlock binaryOf(const juce::ValueTree& tree)
{
    juce::MemoryBlock state;
    if (auto xml = tree.createXml())
        juce::AudioProcessor::copyXmlToBinary(*xml, state);
    return state;
}

void setTreeValue(juce::ValueTree& state, const juce::String& id, float value)
{
    auto node = state.getChildWithProperty("id", id);
    require(node.isValid(), id.toRawUTF8());
    node.setProperty("value", value, nullptr);
}

void configureFix5(Processor& processor, int machineId, float rawVolume, float rawPan)
{
    set(processor, "machine", static_cast<float>(machineListIndex(machineId)));
    set(processor, modeId(machineId), static_cast<float>(monomachine::dspModeMnmFix5Full));
    // P1 AMP page. These raw values are intentionally passed to the native
    // frame envelope and gain ring, not remapped to a generic float ADSR.
    set(processor, "p0_0", 0.0f);   // AMP ATK
    set(processor, "p0_1", 0.0f);   // AMP HOLD
    set(processor, "p0_2", 90.0f);  // AMP DEC
    set(processor, "p0_3", 40.0f);  // AMP REL
    set(processor, "p0_5", rawVolume);
    set(processor, "p0_6", rawPan);
}

StereoEnergy renderBlock(Processor& processor, juce::MidiBuffer midi)
{
    juce::AudioBuffer<float> audio(2, 128);
    audio.clear();
    processor.processBlock(audio, midi);
    StereoEnergy result;
    for (int sample = 0; sample < audio.getNumSamples(); ++sample) {
        const float left = audio.getSample(0, sample);
        const float right = audio.getSample(1, sample);
        require(std::isfinite(left) && std::isfinite(right), "Fix-5 processor output is non-finite");
        result.left += double(left) * left;
        result.right += double(right) * right;
    }
    return result;
}

StereoEnergy synthEnergyFor(int machineId, float rawVolume, float rawPan)
{
    Processor processor;
    configureFix5(processor, machineId, rawVolume, rawPan);
    processor.prepareToPlay(44100.0, 128);

    StereoEnergy energy;
    for (int block = 0; block < 48; ++block) {
        juce::MidiBuffer midi;
        if (block == 0)
            midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(127)), 0);
        const auto frame = renderBlock(processor, midi);
        if (block >= 12) {
            energy.left += frame.left;
            energy.right += frame.right;
        }
    }
    return energy;
}

void checkSchemaGateForEveryFmMachine()
{
    constexpr std::array<int, 3> kRetiredRaw { monomachine::dspModeMnmFrqEnvFix,
                                                 monomachine::dspModeTry4,
                                                 monomachine::dspModeMnmFix5Full };
    for (const int machineId : kFix5Machines) {
        for (const int schema : { 33, 34, 35, 36 }) {
            for (const int retired : kRetiredRaw) {
                Processor processor;
                const auto id = modeId(machineId);
                auto state = processor.parameters.copyState();
                state.setProperty("schema", schema, nullptr);
                setTreeValue(state, id, static_cast<float>(retired));
                auto binary = binaryOf(state);
                processor.setStateInformation(binary.getData(), static_cast<int>(binary.getSize()));
                require(valueOf(processor, id) == static_cast<float>(monomachine::dspModeMnm),
                        "retired FM raw ID must normalize to MNM FRQ, never to OLD BPM");
            }
        }
    }
}

void checkPerMachineSelectorIsolation()
{
    Processor processor;
    set(processor, modeId(8), static_cast<float>(monomachine::dspModeMnmFix));
    set(processor, modeId(9), static_cast<float>(monomachine::dspModeOldFix));
    set(processor, modeId(10), static_cast<float>(monomachine::dspModeNewFix));
    require(valueOf(processor, modeId(8)) == static_cast<float>(monomachine::dspModeMnmFix)
                && valueOf(processor, modeId(9)) == static_cast<float>(monomachine::dspModeOldFix)
                && valueOf(processor, modeId(10)) == static_cast<float>(monomachine::dspModeNewFix),
            "a retained FM+ BPM selection rewrote another machine's selector");
}

void checkSynthRoutingAndLifecycle()
{
    // Every supported FM+ machine must reach the isolated renderer, rather
    // than merely exposing the choice in the selector UI.
    for (const int machineId : kFix5Machines) {
        const auto energy = synthEnergyFor(machineId, 127.0f, 64.0f);
        require(energy.total() > 1.0e-9, "Fix-5 FM+ machine route is silent");
    }

    // A MIDI note-off is a native TRIG=2 release, not an immediate host ADSR
    // mute. The Pack-7 vector test separately verifies the native TRIG=3 KILL
    // curve word-for-word; here the actual processor's MIDI/panic boundary is
    // covered as well.
    Processor processor;
    configureFix5(processor, 8, 127.0f, 64.0f);
    processor.prepareToPlay(44100.0, 128);
    juce::MidiBuffer noteOn;
    noteOn.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(127)), 0);
    (void) renderBlock(processor, noteOn);
    StereoEnergy sustained;
    for (int i = 0; i < 12; ++i) {
        const auto frame = renderBlock(processor, {});
        sustained.left += frame.left; sustained.right += frame.right;
    }
    juce::MidiBuffer noteOff;
    noteOff.addEvent(juce::MidiMessage::noteOff(1, 60), 0);
    StereoEnergy release = renderBlock(processor, noteOff);
    release.left += renderBlock(processor, {}).left;
    release.right += renderBlock(processor, {}).right;
    require(sustained.total() > 1.0e-10 && release.total() > 1.0e-14,
            "processor MIDI note-off skipped Fix-5's native release tail");
    juce::MidiBuffer allSoundOff;
    allSoundOff.addEvent(juce::MidiMessage::allSoundOff(1), 0);
    const auto cleared = renderBlock(processor, allSoundOff);
    require(cleared.total() < 1.0e-14,
            "processor all-sound-off did not clear an active Fix-5 renderer");

    // Native VOL squared yields about (64/127)^4 in signal energy. Applying
    // the generic host VOL stage as well would add a further ~0.25 factor.
    const double full = synthEnergyFor(8, 127.0f, 64.0f).total();
    const double half = synthEnergyFor(8, 64.0f, 64.0f).total();
    require(full > 1.0e-9 && half > 1.0e-12, "Fix-5 Synth core is silent");
    const double ratio = half / full;
    require(ratio > 0.030 && ratio < 0.110,
            "Fix-5 VOL squared path appears to be double-processed by host VOL");

    // PAN 0/127 belongs to the native sin/cos ring. This route-local check
    // also catches an accidental generic PAN reapplication after the bypass.
    const auto hardLeft = synthEnergyFor(8, 127.0f, 0.0f);
    const auto hardRight = synthEnergyFor(8, 127.0f, 127.0f);
    require(hardLeft.left > hardLeft.right * 8.0 && hardRight.right > hardRight.left * 8.0,
            "Fix-5 native pan ring is not the sole effective VOL/PAN path");
    std::printf("FIX5 HOST SYNTH PASS: m8/m9/m10 routes; VOL^2 ratio %.6f\n", ratio);
}
} // namespace

int main()
{
    try {
        Processor processor;
        for (const int machineId : kFix5Machines) {
            const auto* mode = dynamic_cast<juce::AudioParameterChoice*>(processor.parameters.getParameter(modeId(machineId)));
            require(mode != nullptr && mode->choices.size() == 6,
                    "FM+ selector must expose exactly six retained FRQ/BPM choices");
            require(mode->choices[0] == "mnm frq" && mode->choices[1] == "old frq"
                        && mode->choices[2] == "new frq" && mode->choices[3] == "mnm bpm"
                        && mode->choices[4] == "new bpm" && mode->choices[5] == "old bpm",
                    "FM+ selector labels/order do not match the retained renderer map");
        }
        checkSchemaGateForEveryFmMachine();
        checkPerMachineSelectorIsolation();
        std::puts("FM MODE HOST PASS: six retained FM+ selectors; raw IDs 6..8 normalize to MNM FRQ");
        return 0;
    }
    catch (const std::exception& error) {
        std::fprintf(stderr, "FIX5 HOST INTEGRATION FAIL: %s\n", error.what());
        return 1;
    }
}
