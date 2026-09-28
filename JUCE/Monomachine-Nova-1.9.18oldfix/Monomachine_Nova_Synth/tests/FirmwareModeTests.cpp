// JUCE-free schema-33 firmware boundary tests.
// The test deliberately contains no OS image: its tiny packet is transport
// framing only and cannot be used as firmware.
#include "models/DspModes.hpp"
#include "dsp/firmware/FirmwareControl.hpp"
#include "dsp/firmware/FirmwareSysex.hpp"

#include <array>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {
int failures = 0;
void require(bool condition, const char* message)
{
    if (!condition) {
        std::fprintf(stderr, "FirmwareModeTests: %s\n", message);
        ++failures;
    }
}
}

int main()
{
    using namespace monomachine;
    using namespace nova::firmware;

    require(kDspModeSchemaVersion == 33
                && dspModeMnm == 0 && dspModeOld == 1 && dspModeNew == 2
                && dspModeMnmFix == 3 && dspModeNewFix == 4
                && dspModeOldFix == 5 && dspModeFirmware == 6
                && dspModeCount == 7,
            "schema 33 preserves retained SYNT IDs 0..5 and appends firmware=6");
    require(std::strcmp(dspSyntModeChoicesForMachine(8),
                        "mnm|old|new|mnm fix|new fix|old fix|firmware") == 0
                && dspSyntModeAllowedForMachine(8, dspModeFirmware)
                && dspSyntModeAllowedForMachine(9, dspModeFirmware)
                && dspSyntModeAllowedForMachine(10, dspModeFirmware)
                && !dspSyntModeAllowedForMachine(7, dspModeFirmware),
            "firmware SYNT selection is restricted to FM+ m8/m9/m10");
    require(!dspSyntModeUsesMeasuredFix(dspModeFirmware)
                && dspSyntModeUsesFirmware(dspModeFirmware),
            "firmware does not borrow a measured FIX profile");

    require(dspSectionFirmwareMode(DspAmp) == 1
                && dspSectionFirmwareMode(DspFilter) == 2
                && dspSectionFirmwareMode(DspDist) == 2
                && dspSectionFirmwareMode(DspDelay) == 3
                && dspSectionFirmwareMode(DspSrr) == 1
                && dspSectionFirmwareMode(DspVolPan) == 1
                && std::strcmp(dspSectionModeChoices(DspDelay), "mnm|old|new|firmware") == 0,
            "every requested non-SYNT stage has its own append-only firmware selection");
    require(dspModeAllowedForSection(DspFilter, 1)
                && dspModeAllowedForSection(DspFilter, 2)
                && !dspModeAllowedForSection(DspFilter, 3)
                && !dspModeAllowedForSection(DspDelay, 6),
            "section firmware raw IDs do not alias SYNT IDs");

    std::array<float, 32> pages{};
    for (std::size_t i = 0; i < pages.size(); ++i)
        pages[i] = static_cast<float>(i * 8 - 10);
    const auto frame = ControlFrame::fromModulatedPages(
        pages.data(), 9, 64, 1.25f, 0.75f, 133.0, 42,
        kSynth | kFilter | kEcho | kSampleRate);
    require(frame.synth[0] == 0 && frame.synth[1] == 0
                && frame.amp[0] == 54 && frame.filter[0] == 118
                && frame.effect[0] == 127 && frame.machineId == 9
                && frame.midiNote == 64 && frame.sequence == 42
                && selects(frame.selected, Section::Synth)
                && selects(frame.selected, Section::Filter)
                && selects(frame.selected, Section::Echo)
                && selects(frame.selected, Section::SampleRate)
                && !selects(frame.selected, Section::Amp),
            "post-modulation frame preserves current public page controls and selected sections");
    require(!canRenderWholeTrack(RuntimeAvailability::AdapterUnavailable)
                && !canRenderSelectedSections(RuntimeAvailability::WholeTrackReady)
                && canRenderSelectedSections(RuntimeAvailability::StageHarnessReady),
            "runtime availability cannot overclaim an unavailable or whole-track-only adapter");

    const std::vector<std::uint8_t> transportOnly = {
        0xf0, 0x00, 0x20, 0x3c, 0x03, 0x00, 0x7e,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x02, 0x03, 0xf7
    };
    const auto inspected = inspectMonomachineOsSysex(transportOnly);
    require(inspected.valid && inspected.dataPackets == 1
                && inspected.decodedTransportBytes == 2
                && inspected.firstAddress == 0 && inspected.endAddress == 2,
            "bounded SysEx inspector accepts only transport framing metadata");
    require(sha256Hex(reinterpret_cast<const std::uint8_t*>("abc"), 3)
                == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
            "SHA-256 reference hash is stable");

    SourceReference ref;
    ref.path = "/user/local/Monomachine_OS.syx";
    ref.sha256 = "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";
    require(!ref.empty(), "source reference requires only path plus SHA-256");

    return failures == 0 ? 0 : 1;
}
