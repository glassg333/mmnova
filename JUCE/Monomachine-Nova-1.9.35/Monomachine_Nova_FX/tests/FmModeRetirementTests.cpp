// Host-level regression for the retained six-mode FM+ MODE SYNT contract.
// Historical raw state IDs 6..8 must normalize to MNM FRQ; no imported
// experimental renderer is compiled or selected by this test.
#include "PluginProcessor.h"

#include <array>
#include <cstdio>
#include <stdexcept>

namespace {
using Processor = MonomachineNovaAudioProcessor;
constexpr std::array<int, 3> kFmMachines { 8, 9, 10 };
constexpr std::array<int, 3> kRetiredRaw {
    monomachine::kRetiredSyntRawId6,
    monomachine::kRetiredSyntRawId7,
    monomachine::kRetiredSyntRawId8
};

void require(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

juce::String modeId(int machineId)
{
    return monomachine::dspMachineModeParamId(machineId);
}

float valueOf(const Processor& processor, const juce::String& id)
{
    const auto* value = processor.parameters.getRawParameterValue(id);
    require(value != nullptr, "FM+ mode parameter is absent");
    return value->load();
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
    require(node.isValid(), "state node for FM+ mode is absent");
    node.setProperty("value", value, nullptr);
}

void checkSixLabelsAndLegacyNormalization()
{
    for (const int machineId : kFmMachines) {
        Processor processor;
        const auto id = modeId(machineId);
        const auto* selector = dynamic_cast<juce::AudioParameterChoice*>(processor.parameters.getParameter(id));
        require(selector != nullptr && selector->choices.size() == 6,
                "FM+ selector must expose exactly six retained choices");
        require(selector->choices[0] == "mnm frq" && selector->choices[1] == "old frq"
                    && selector->choices[2] == "new frq" && selector->choices[3] == "mnm bpm"
                    && selector->choices[4] == "new bpm" && selector->choices[5] == "old bpm",
                "FM+ selector labels/order differ from the retained contract");

        for (const int schema : { 33, 34, 35, 36 }) {
            for (const int raw : kRetiredRaw) {
                auto state = processor.parameters.copyState();
                state.setProperty("schema", schema, nullptr);
                setTreeValue(state, id, static_cast<float>(raw));
                const auto binary = binaryOf(state);
                processor.setStateInformation(binary.getData(), static_cast<int>(binary.getSize()));
                require(valueOf(processor, id) == static_cast<float>(monomachine::dspModeMnm),
                        "historical raw ID must normalize to MNM FRQ");
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
            "one retained FM+ selection rewrote another machine selector");
}
} // namespace

int main()
{
    try {
        checkSixLabelsAndLegacyNormalization();
        checkPerMachineSelectorIsolation();
        std::puts("FM MODE RETIREMENT PASS: six retained selectors; raw IDs 6..8 normalize to MNM FRQ");
        return 0;
    }
    catch (const std::exception& error) {
        std::fprintf(stderr, "FM MODE RETIREMENT FAIL: %s\n", error.what());
        return 1;
    }
}
