// JUCE-free contract tests for schema-32 MODE SYNT=oracle.
// This target intentionally includes only the isolated oracle implementation
// and the serial-mode registry: it must compile without any retained FM core or
// shared FIX table header.
#include "dsp/fm_oracle/OracleFm.hpp"
#include "models/DspModes.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdint>

namespace {
int failures = 0;

void require(bool condition, const char* message)
{
    std::printf("%-82s %s\n", message, condition ? "OK" : "FAIL");
    if (!condition) ++failures;
}

bool closeEnough(float actual, float expected, float tolerance = 1.0e-6f)
{
    return std::fabs(actual - expected) <= tolerance;
}

uint64_t hashWords(const float* samples, size_t count)
{
    uint64_t result = 1469598103934665603ull;
    for (size_t i = 0; i < count; ++i) {
        union { float value; uint32_t word; } bits{samples[i]};
        for (int byte = 0; byte < 4; ++byte) {
            result ^= (bits.word >> (byte * 8)) & 0xffu;
            result *= 1099511628211ull;
        }
    }
    return result;
}

template <typename Core>
uint64_t render(Core& core, const std::array<uint8_t, 8>& raw, int note)
{
    float left[256]{};
    float right[256]{};
    core.reset(44100.0);
    core.setParameters(raw[0], raw[1], raw[2], raw[3], raw[4], raw[5], raw[6], raw[7]);
    core.noteOn(static_cast<uint8_t>(note));
    core.setPitchBend(0.0f);
    core.processStereo(left, right, 256);
    for (size_t i = 0; i < 256; ++i) {
        if (!std::isfinite(left[i]) || !std::isfinite(right[i])) return 0;
        if (left[i] != right[i]) return 0;
    }
    return hashWords(left, 256);
}
} // namespace

int main()
{
    using namespace monomachine;
    using namespace monomachine::fm_oracle;

    require(kDspModeSchemaVersion == 32
                && dspModeMnm == 0 && dspModeOld == 1 && dspModeNew == 2
                && dspModeMnmFix == 3 && dspModeNewFix == 4 && dspModeOldFix == 5
                && dspModeOracle == 6 && dspModeCount == 7,
            "schema 32 appends oracle=6 while all serialized IDs 0..5 remain frozen");
    require(std::strcmp(dspSyntModeChoicesForMachine(8), "mnm|old|new|mnm fix|new fix|old fix|oracle") == 0
                && dspSyntModeCountForMachine(8) == 7
                && dspSyntModeAllowedForMachine(8, dspModeOracle)
                && dspSyntModeAllowedForMachine(9, dspModeOracle)
                && dspSyntModeAllowedForMachine(10, dspModeOracle)
                && !dspSyntModeAllowedForMachine(7, dspModeOracle)
                && !dspSyntModeUsesMeasuredFix(dspModeOracle)
                && dspSyntModeUsesOracle(dspModeOracle),
            "oracle is FM+-only and cannot fall through the pre-existing measured-FIX profile");

    require(supportsMachine(8) && supportsMachine(9) && supportsMachine(10) && !supportsMachine(7)
                && factoryRawForMachine(7) == nullptr
                && (*factoryRawForMachine(8)) == kOracleStatFactoryRaw
                && (*factoryRawForMachine(9)) == kOracleParFactoryRaw
                && (*factoryRawForMachine(10)) == kOracleDynFactoryRaw,
            "private oracle namespace owns the three independent reference factory raw profiles");
    require(std::strcmp(machineLabelForMachine(8), "FM+ STAT") == 0
                && std::strcmp(machineLabelForMachine(9), "FM+ PAR") == 0
                && std::strcmp(machineLabelForMachine(10), "FM+ DYN") == 0
                && std::strcmp(labelForMachine(8, 0), "1FRQ") == 0
                && std::strcmp(labelForMachine(9, 4), "3FRQ") == 0
                && std::strcmp(labelForMachine(10, 6), "2FB") == 0
                && std::strcmp(labelForMachine(7, 0), "---") == 0,
            "private oracle namespace owns visible STAT/PAR/DYN machine/control labels");
    require(statParListIndexForRaw(0) == 0 && statParListIndexForRaw(48) == 9
                && statParListIndexForRaw(60) == 11 && statParListIndexForRaw(80) == 15
                && statParListIndexForRaw(127) == 23
                && std::strcmp(statParRatioLabelForRaw(48), "5/32") == 0
                && closeEnough(statParRatioForRaw(60), 0.5f)
                && closeEnough(statParRatioForRaw(80), 1.0f),
            "Oracle STAT/PAR owns the local 24-entry raw-list mapping and label law");
    require(closeEnough(dynRatio1ForRaw(64), 1.0f)
                && closeEnough(dynRatio2ForRaw(32), 0.25f)
                && closeEnough(tuneSemitonesForRaw(0), -2.0f)
                && closeEnough(tuneSemitonesForRaw(64), 0.0f)
                && closeEnough(tuneSemitonesForRaw(127), 63.0f / 32.0f),
            "Oracle DYN direct laws and private tune mapping are deterministic at endpoints");

    OracleStatCore statA, statB;
    OracleParallelCore parA, parB;
    OracleDynamicCore dynA, dynB;
    const auto statHashA = render(statA, kOracleStatFactoryRaw, 69);
    const auto statHashB = render(statB, kOracleStatFactoryRaw, 69);
    const auto parHashA = render(parA, kOracleParFactoryRaw, 69);
    const auto parHashB = render(parB, kOracleParFactoryRaw, 69);
    const auto dynHashA = render(dynA, kOracleDynFactoryRaw, 69);
    const auto dynHashB = render(dynB, kOracleDynFactoryRaw, 69);
    require(statHashA != 0 && parHashA != 0 && dynHashA != 0
                && statHashA == statHashB && parHashA == parHashB && dynHashA == dynHashB,
            "private STAT/PAR/DYN cores are finite stereo-deterministic after their own reset/note/pitch lifecycle");
    require(statHashA != parHashA && statHashA != dynHashA && parHashA != dynHashA,
            "private STAT/PAR/DYN cores have distinct native topology/state outputs");

    if (failures != 0) {
        std::fprintf(stderr, "ORACLE_MODE_TESTS FAIL: %d check(s)\n", failures);
        return 1;
    }
    std::puts("ORACLE_MODE_TESTS PASS");
    return 0;
}
