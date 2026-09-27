// APVTS migration coverage for the removal of FMA/source-derived DSP modes.
#include "PluginProcessor.h"

#include <cmath>
#include <cstdio>
#include <stdexcept>

namespace
{
void require(bool value, const char* message)
{
    if (! value)
        throw std::runtime_error(message);
}

float parameterValue(const juce::AudioProcessorValueTreeState& state, const char* id)
{
    const auto* value = state.getRawParameterValue(id);
    require(value != nullptr, id);
    return value->load();
}

juce::MemoryBlock stateOf(MonomachineNovaAudioProcessor& processor)
{
    juce::MemoryBlock state;
    processor.getStateInformation(state);
    return state;
}

std::unique_ptr<juce::XmlElement> xmlOf(const juce::MemoryBlock& state)
{
    auto xml = juce::AudioProcessor::getXmlFromBinary(state.getData(), static_cast<int>(state.getSize()));
    require(xml != nullptr, "could not parse state XML");
    return xml;
}

void setSerializedValue(juce::XmlElement& xml, const char* id, float value)
{
    for (auto* child : xml.getChildIterator()) {
        if (child->getStringAttribute("id") == id) {
            child->setAttribute("value", value);
            return;
        }
    }
    auto* child = xml.createNewChildElement("PARAM");
    child->setAttribute("id", id);
    child->setAttribute("value", value);
}

bool containsSerializedId(const juce::XmlElement& xml, const char* id)
{
    for (auto* child : xml.getChildIterator())
        if (child->getStringAttribute("id") == id)
            return true;
    return false;
}

void removeSerializedId(juce::XmlElement& xml, const char* id)
{
    for (auto* child : xml.getChildIterator()) {
        if (child->getStringAttribute("id") == id) {
            xml.removeChildElement(child, true);
            return;
        }
    }
}

juce::MemoryBlock binaryOf(const juce::XmlElement& xml)
{
    juce::MemoryBlock state;
    juce::AudioProcessor::copyXmlToBinary(xml, state);
    return state;
}

void requireNear(float actual, float expected, const char* message)
{
    require(std::abs(actual - expected) < 1.0e-6f, message);
}
}

int main()
{
    try {
        MonomachineNovaAudioProcessor source;
        require(source.parameters.getParameter("p2_fx_mode") == nullptr,
                "withdrawn P2 selector is still registered");
        requireNear(parameterValue(source.parameters, "mode_dist"), 0.0f,
                    "P1 DIST no longer defaults to native mnm");
        requireNear(parameterValue(source.parameters, "p2_mode_dist"), 0.0f,
                    "P2 DIST no longer defaults to native mnm");
        requireNear(parameterValue(source.parameters, "amp_mode"), 1.0f,
                    "AMP no longer defaults to mnm");

        // Emulate a state saved by the withdrawn selector schema. Values 2/3
        // must never land on a surviving alternate index after the list shrinks.
        auto legacy = xmlOf(stateOf(source));
        legacy->setAttribute("schema", 16);
        for (const char* id : { "mode_synt", "mode_filt", "mode_dist", "mode_dly",
                                "p2_mode_filt", "p2_mode_dist", "p2_mode_dly" })
            setSerializedValue(*legacy, id, 2.0f);
        for (const auto& machine : nova::machines())
            setSerializedValue(*legacy,
                               monomachine::dspMachineModeParamId(machine.id).c_str(), 3.0f);
        setSerializedValue(*legacy, "amp_mode", 3.0f);
        setSerializedValue(*legacy, "p2_amp_mode", 3.0f);
        setSerializedValue(*legacy, "p2_fx_mode", 1.0f);

        auto retiredState = binaryOf(*legacy);
        MonomachineNovaAudioProcessor restored;
        restored.setStateInformation(retiredState.getData(), static_cast<int>(retiredState.getSize()));

        for (const char* id : { "mode_filt", "mode_dist", "mode_dly",
                                "p2_mode_filt", "p2_mode_dist", "p2_mode_dly" })
            requireNear(parameterValue(restored.parameters, id), 0.0f,
                        "withdrawn selector did not migrate to mnm");
        for (const auto& machine : nova::machines())
            requireNear(parameterValue(restored.parameters,
                                       monomachine::dspMachineModeParamId(machine.id).c_str()), 0.0f,
                        "withdrawn per-machine selector did not migrate to mnm");
        requireNear(parameterValue(restored.parameters, "amp_mode"), 1.0f,
                    "withdrawn AMP value did not migrate to mnm");
        requireNear(parameterValue(restored.parameters, "p2_amp_mode"), 1.0f,
                    "withdrawn P2 AMP value did not migrate to mnm");

        // Schema 23 appends DLY NEW at index two.  A schema-22 value two was
        // a withdrawn branch and must still become MNM, while the four newly
        // appended feedback-Q values start at their neutral zero defaults.
        auto schema22Delay = xmlOf(stateOf(source));
        schema22Delay->setAttribute("schema", 22);
        setSerializedValue(*schema22Delay, "mode_dly", 2.0f);
        setSerializedValue(*schema22Delay, "p2_mode_dly", 2.0f);
        for (const char* id : { "dly_dbas_q", "dly_dwid_q", "p2_dly_dbas_q", "p2_dly_dwid_q" })
            removeSerializedId(*schema22Delay, id);
        auto schema22DelayState = binaryOf(*schema22Delay);
        MonomachineNovaAudioProcessor migratedDelay;
        migratedDelay.setStateInformation(schema22DelayState.getData(), static_cast<int>(schema22DelayState.getSize()));
        requireNear(parameterValue(migratedDelay.parameters, "mode_dly"), 0.0f,
                    "schema-22 withdrawn P1 DLY selector did not migrate to MNM");
        requireNear(parameterValue(migratedDelay.parameters, "p2_mode_dly"), 0.0f,
                    "schema-22 withdrawn P2 DLY selector did not migrate to MNM");
        for (const char* id : { "dly_dbas_q", "dly_dwid_q", "p2_dly_dbas_q", "p2_dly_dwid_q" })
            requireNear(parameterValue(migratedDelay.parameters, id), 0.0f,
                        "schema-22 delay feedback Q did not migrate to neutral");

        // A schema-23 state may intentionally select appended NEW and preserve
        // independent P1/P2 Q values; no old index is repurposed.
        auto schema23Delay = xmlOf(stateOf(source));
        schema23Delay->setAttribute("schema", 23);
        setSerializedValue(*schema23Delay, "mode_dly", 2.0f);
        setSerializedValue(*schema23Delay, "p2_mode_dly", 2.0f);
        setSerializedValue(*schema23Delay, "dly_dbas_q", 31.0f);
        setSerializedValue(*schema23Delay, "dly_dwid_q", 47.0f);
        setSerializedValue(*schema23Delay, "p2_dly_dbas_q", 63.0f);
        setSerializedValue(*schema23Delay, "p2_dly_dwid_q", 79.0f);
        auto schema23DelayState = binaryOf(*schema23Delay);
        MonomachineNovaAudioProcessor preservedDelay;
        preservedDelay.setStateInformation(schema23DelayState.getData(), static_cast<int>(schema23DelayState.getSize()));
        requireNear(parameterValue(preservedDelay.parameters, "mode_dly"), 2.0f,
                    "schema-23 P1 DLY NEW selector was not retained");
        requireNear(parameterValue(preservedDelay.parameters, "p2_mode_dly"), 2.0f,
                    "schema-23 P2 DLY NEW selector was not retained");
        requireNear(parameterValue(preservedDelay.parameters, "dly_dbas_q"), 31.0f,
                    "schema-23 P1 DBAS Q was not retained");
        requireNear(parameterValue(preservedDelay.parameters, "dly_dwid_q"), 47.0f,
                    "schema-23 P1 DWID Q was not retained");
        requireNear(parameterValue(preservedDelay.parameters, "p2_dly_dbas_q"), 63.0f,
                    "schema-23 P2 DBAS Q was not retained");
        requireNear(parameterValue(preservedDelay.parameters, "p2_dly_dwid_q"), 79.0f,
                    "schema-23 P2 DWID Q was not retained");

        // Schema 17 used canonical FilterCore IDs in both menus and delegated
        // MODE S=NATIVE to mode_dist. Schema 18 presents physical-side choices
        // and makes MODE S authoritative without losing an OLD project.
        auto hybrid17 = xmlOf(stateOf(source));
        hybrid17->setAttribute("schema", 17);
        setSerializedValue(*hybrid17, "hybrid_p1_mode_l", 1.0f); // K35 LP -> lower list slot 6
        setSerializedValue(*hybrid17, "hybrid_p1_mode_h", 2.0f); // K35 HP -> upper list slot 6
        setSerializedValue(*hybrid17, "mode_dist", 1.0f);        // old reference wins in schema 17
        setSerializedValue(*hybrid17, "hybrid_p1_mode_s", 1.0f); // private FOLD was not audible over old
        setSerializedValue(*hybrid17, "hybrid_p2_mode_l", 7.0f); // HP24 -> lower list slot 2
        setSerializedValue(*hybrid17, "hybrid_p2_mode_h", 3.0f); // LP24 -> upper list slot 2
        setSerializedValue(*hybrid17, "p2_mode_dist", 0.0f);
        setSerializedValue(*hybrid17, "hybrid_p2_mode_s", 3.0f); // CLAMP -> new slot 4
        auto hybrid17State = binaryOf(*hybrid17);
        MonomachineNovaAudioProcessor hybrid18;
        hybrid18.setStateInformation(hybrid17State.getData(), static_cast<int>(hybrid17State.getSize()));
        requireNear(parameterValue(hybrid18.parameters, "hybrid_p1_mode_l"), 6.0f,
                    "legacy MODE L K35 LP did not migrate to lower physical order");
        requireNear(parameterValue(hybrid18.parameters, "hybrid_p1_mode_h"), 6.0f,
                    "legacy MODE H K35 HP did not migrate to upper physical order");
        requireNear(parameterValue(hybrid18.parameters, "hybrid_p1_mode_s"), 1.0f,
                    "legacy OLD precedence over a private MODE S selection was not preserved");
        requireNear(parameterValue(hybrid18.parameters, "hybrid_p2_mode_l"), 2.0f,
                    "legacy MODE L HP24 did not migrate to lower physical order");
        requireNear(parameterValue(hybrid18.parameters, "hybrid_p2_mode_h"), 2.0f,
                    "legacy MODE H LP24 did not migrate to upper physical order");
        requireNear(parameterValue(hybrid18.parameters, "hybrid_p2_mode_s"), 5.0f,
                    "legacy MODE S CLAMP did not retain its imported character");

        // Schema 18 already used physical MODE L/H and MODE S values
        // MNM|OLD|FOLD|ZERO|CLAMP. Schema 19 inserts MNM FIX without moving
        // those saved imported selections.
        auto schema18 = xmlOf(stateOf(source));
        schema18->setAttribute("schema", 18);
        setSerializedValue(*schema18, "hybrid_p1_mode_s", 2.0f); // FOLD
        setSerializedValue(*schema18, "hybrid_p2_mode_s", 4.0f); // CLAMP
        auto schema18State = binaryOf(*schema18);
        MonomachineNovaAudioProcessor schema19;
        schema19.setStateInformation(schema18State.getData(), static_cast<int>(schema18State.getSize()));
        requireNear(parameterValue(schema19.parameters, "hybrid_p1_mode_s"), 3.0f,
                    "schema-18 MODE S FOLD did not retain its sound slot");
        requireNear(parameterValue(schema19.parameters, "hybrid_p2_mode_s"), 5.0f,
                    "schema-18 MODE S CLAMP did not retain its sound slot");

        // Schema 20 adds only opt-in FILT extras and four MOD ENV sources.
        // A schema-19 state lacking all of these fields must receive exact
        // neutral values, rather than inheriting arbitrary APVTS memory.
        auto legacy19 = xmlOf(stateOf(source));
        legacy19->setAttribute("schema", 19);
        const char* filtFields[]{"vel_l","vel_h","kt_l","kt_h","sat","env_atk","env_hold","env_dec","env_rel","env_mix","env_base_depth","env_width_depth"};
        const float filtDefaults[]{0,0,0,0,0,0,0,127,127,0,0,0};
        for (const char* prefix : { "filt_", "p2_filt_" })
            for (const char* field : filtFields)
                removeSerializedId(*legacy19, (juce::String(prefix) + field).toRawUTF8());
        for (int env = 1; env <= 4; ++env)
            for (const char* field : { "atk", "hold", "dec", "rel" })
                removeSerializedId(*legacy19, ("modenv" + juce::String(env) + "_" + field).toRawUTF8());
        auto legacy19State = binaryOf(*legacy19);
        MonomachineNovaAudioProcessor schema20;
        schema20.setStateInformation(legacy19State.getData(), static_cast<int>(legacy19State.getSize()));
        for (const char* prefix : { "filt_", "p2_filt_" })
            for (size_t i = 0; i < sizeof(filtFields) / sizeof(filtFields[0]); ++i)
                requireNear(parameterValue(schema20.parameters, (juce::String(prefix) + filtFields[i]).toRawUTF8()), filtDefaults[i],
                            "schema-19 FILT extra did not migrate to neutral");
        for (int env = 1; env <= 4; ++env) {
            const auto prefix = "modenv" + juce::String(env) + "_";
            requireNear(parameterValue(schema20.parameters, (prefix + "atk").toRawUTF8()), 0.0f, "MOD ENV attack default missing");
            requireNear(parameterValue(schema20.parameters, (prefix + "hold").toRawUTF8()), 0.0f, "MOD ENV hold default missing");
            requireNear(parameterValue(schema20.parameters, (prefix + "dec").toRawUTF8()), 127.0f, "MOD ENV decay default missing");
            requireNear(parameterValue(schema20.parameters, (prefix + "rel").toRawUTF8()), 127.0f, "MOD ENV release default missing");
        }
        const int modEnvSlot = schema20.addRouteFromSource(35, 0);
        requireNear(parameterValue(schema20.parameters, ("r" + juce::String(modEnvSlot) + "_src").toRawUTF8()), 35.0f,
                    "MOD ENV4 source ID was not retained by route creation");

        auto written = xmlOf(stateOf(restored));
        require(written->getIntAttribute("schema") == monomachine::kDspModeSchemaVersion,
                "rollback schema version was not saved");
        require(! containsSerializedId(*written, "p2_fx_mode"),
                "withdrawn P2 selector was written back out");

        std::printf("ROLLBACKSTATE schema=%d native=P1/P2 PASS\n",
                    monomachine::kDspModeSchemaVersion);
        return 0;
    }
    catch (const std::exception& error) {
        std::fprintf(stderr, "ROLLBACKSTATE FAIL: %s\n", error.what());
        return 1;
    }
}
