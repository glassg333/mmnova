// Processor-state regression for the seven-stage documented P1/P2 topology.
// It proves the exact six-stage release default maps to the documented reset
// order, while an actually rearranged legacy state retains its old relative
// ordering and gains VOL/PAN immediately before SRR.
#include "PluginProcessor.h"
#include <array>
#include <cstdio>
#include <stdexcept>

namespace
{
using Processor = MonomachineNovaAudioProcessor;

void require(bool value, const char* message)
{
    if (! value)
        throw std::runtime_error(message);
}

void requireOrder(const std::array<int, Processor::classicStagesCount>& actual,
                  const std::array<int, Processor::classicStagesCount>& expected,
                  const char* message)
{
    for (int i = 0; i < Processor::classicStagesCount; ++i)
        if (actual[static_cast<size_t>(i)] != expected[static_cast<size_t>(i)])
            throw std::runtime_error(message);
}

void requireEnabled(const std::array<bool, Processor::classicStagesCount>& actual,
                    bool expected, const char* message)
{
    for (const bool enabled : actual)
        if (enabled != expected)
            throw std::runtime_error(message);
}

juce::MemoryBlock stateOf(Processor& processor)
{
    juce::MemoryBlock result;
    processor.getStateInformation(result);
    return result;
}
}

int main()
{
    try
    {
        constexpr std::array<int, Processor::classicStagesCount> canonical{{
            Processor::classicEq, Processor::classicFilt, Processor::classicDist,
            Processor::classicEnv, Processor::classicVolPan, Processor::classicSrr,
            Processor::classicDelay
        }};

        Processor fresh;
        const auto freshRoute = fresh.classicRouteState();
        requireOrder(freshRoute.p1, canonical, "new P1 default is not documented topology");
        requireOrder(freshRoute.p2, canonical, "new P2 default is not documented topology");
        requireEnabled(freshRoute.p1Enabled, true, "new P1 blocks must default ON");
        requireEnabled(freshRoute.p2Enabled, true, "new P2 blocks must default ON");

        // A bypass belongs to the stable stage ID rather than its route position
        // and must survive state serialization without moving any physical slot.
        fresh.classicRouteSetEnabled(0, Processor::classicDist, false);
        fresh.classicRouteSetEnabled(1, Processor::classicDelay, false);
        auto bypassBlock = stateOf(fresh);
        Processor bypassRestored;
        bypassRestored.setStateInformation(bypassBlock.getData(), static_cast<int>(bypassBlock.getSize()));
        const auto bypassRoute = bypassRestored.classicRouteState();
        require(! bypassRoute.p1Enabled[static_cast<size_t>(Processor::classicDist)],
                "P1 DIST bypass did not persist");
        require(! bypassRoute.p2Enabled[static_cast<size_t>(Processor::classicDelay)],
                "P2 DELAY bypass did not persist");
        fresh.classicRouteReset(0);fresh.classicRouteReset(1);
        requireEnabled(fresh.classicRouteState().p1Enabled, true, "P1 reset must restore all blocks ON");
        requireEnabled(fresh.classicRouteState().p2Enabled, true, "P2 reset must restore all blocks ON");

        // Serialize, then emulate a six-block saved state. P1 is the exact old
        // product default; P2 is explicitly rearranged and must retain that
        // user-selected relative order.
        auto block = stateOf(fresh);
        auto xml = juce::AudioProcessor::getXmlFromBinary(block.getData(), static_cast<int>(block.getSize()));
        require(xml != nullptr, "could not decode state");
        auto* route = xml->getChildByName("CLASSICROUTE");
        require(route != nullptr, "CLASSICROUTE was not serialized");
        route->removeAttribute("format");
        route->removeAttribute("p1_6");
        route->removeAttribute("p2_6");
        for (int i = 0; i < Processor::classicStagesCount; ++i)
        {
            route->removeAttribute("p1_on_" + juce::String(i));
            route->removeAttribute("p2_on_" + juce::String(i));
        }

        // Legacy IDs: DIST=0, SRR=1, FILT=2, EQ=3, ENV=4, DELAY=5.
        // This P1 sequence is the exact old release default. It must *not*
        // take the generic insert-before-SRR migration path.
        const int oldP1[]{Processor::classicDist, Processor::classicSrr, Processor::classicFilt,
                          Processor::classicEq, Processor::classicEnv, Processor::classicDelay};
        // This P2 sequence is genuinely customised, so it uses the generic
        // insertion rule and preserves the six old stage positions.
        const int oldP2[]{Processor::classicDelay, Processor::classicDist, Processor::classicFilt,
                          Processor::classicEq, Processor::classicEnv, Processor::classicSrr};
        for (int i = 0; i < 6; ++i)
        {
            route->setAttribute("p1_" + juce::String(i), oldP1[i]);
            route->setAttribute("p2_" + juce::String(i), oldP2[i]);
        }

        juce::MemoryBlock legacy;
        juce::AudioProcessor::copyXmlToBinary(*xml, legacy);
        Processor restored;
        restored.setStateInformation(legacy.getData(), static_cast<int>(legacy.getSize()));
        const auto migrated = restored.classicRouteState();
        constexpr std::array<int, Processor::classicStagesCount> migratedP1{{
            Processor::classicEq, Processor::classicFilt, Processor::classicDist,
            Processor::classicEnv, Processor::classicVolPan, Processor::classicSrr,
            Processor::classicDelay
        }};
        constexpr std::array<int, Processor::classicStagesCount> migratedP2{{
            Processor::classicDelay, Processor::classicDist, Processor::classicFilt,
            Processor::classicEq, Processor::classicEnv, Processor::classicVolPan,
            Processor::classicSrr
        }};
        requireOrder(migrated.p1, migratedP1,
                     "exact legacy release default did not migrate to documented reset topology");
        requireOrder(migrated.p2, migratedP2,
                     "custom legacy P2 order did not gain VOL/PAN before SRR");
        requireEnabled(migrated.p1Enabled, true, "legacy P1 without bypass flags must default ON");
        requireEnabled(migrated.p2Enabled, true, "legacy P2 without bypass flags must default ON");

        auto current = stateOf(restored);
        auto currentXml = juce::AudioProcessor::getXmlFromBinary(current.getData(), static_cast<int>(current.getSize()));
        require(currentXml != nullptr && currentXml->getIntAttribute("schema") == monomachine::kDspModeSchemaVersion,
                "topology route migration changed DSP mode schema");
        auto* currentRoute = currentXml->getChildByName("CLASSICROUTE");
        require(currentRoute != nullptr && currentRoute->hasAttribute("p1_6") && currentRoute->hasAttribute("p2_6")
                    && currentRoute->hasAttribute("p1_on_0") && currentRoute->hasAttribute("p2_on_6")
                    && currentRoute->getIntAttribute("format") == 4,
                "migrated seven-stage route did not serialize in current format");

        // format=2 had the short-lived wrong seven-block reset order with
        // DIST before FILT. The exact former default is remapped, but a real
        // user permutation remains untouched.
        auto priorBlock = stateOf(fresh);
        auto priorXml = juce::AudioProcessor::getXmlFromBinary(priorBlock.getData(), static_cast<int>(priorBlock.getSize()));
        require(priorXml != nullptr, "could not decode format-2 state");
        auto* priorRoute = priorXml->getChildByName("CLASSICROUTE");
        require(priorRoute != nullptr, "format-2 CLASSICROUTE was not serialized");
        priorRoute->setAttribute("format", 2);
        const int formerFormat2Default[]{Processor::classicDist, Processor::classicEq, Processor::classicFilt,
                                         Processor::classicEnv, Processor::classicVolPan, Processor::classicSrr,
                                         Processor::classicDelay};
        const int formerFormat2Custom[]{Processor::classicDelay, Processor::classicDist, Processor::classicEq,
                                        Processor::classicFilt, Processor::classicEnv, Processor::classicVolPan,
                                        Processor::classicSrr};
        for (int i = 0; i < Processor::classicStagesCount; ++i)
        {
            priorRoute->setAttribute("p1_" + juce::String(i), formerFormat2Default[i]);
            priorRoute->setAttribute("p2_" + juce::String(i), formerFormat2Custom[i]);
        }
        juce::MemoryBlock prior;
        juce::AudioProcessor::copyXmlToBinary(*priorXml, prior);
        Processor priorRestored;
        priorRestored.setStateInformation(prior.getData(), static_cast<int>(prior.getSize()));
        const auto priorMigrated = priorRestored.classicRouteState();
        requireOrder(priorMigrated.p1, canonical,
                     "format-2 default did not migrate DIST behind FILT");
        constexpr std::array<int, Processor::classicStagesCount> preservedFormat2Custom{{
            Processor::classicDelay, Processor::classicDist, Processor::classicEq,
            Processor::classicFilt, Processor::classicEnv, Processor::classicVolPan,
            Processor::classicSrr
        }};
        requireOrder(priorMigrated.p2, preservedFormat2Custom,
                     "format-2 custom order was not preserved");

        std::printf("CLASSICTOPOLOGY stages=%d schema=%d PASS\n",
                    Processor::classicStagesCount, monomachine::kDspModeSchemaVersion);
        return 0;
    }
    catch (const std::exception& error)
    {
        std::fprintf(stderr, "CLASSICTOPOLOGY FAIL: %s\n", error.what());
        return 1;
    }
}
