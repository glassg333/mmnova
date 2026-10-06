# Sources, attribution and license boundaries

This package is source-only. It does not contain a compiled plugin or the full firmware image.

## Firmware and disassembly

Elektron Monomachine SFX6/60 OS 1.32B firmware and associated instruction/data
translations remain subject to their original rights. No new open-source license
is claimed for Elektron firmware, its coefficient tables, or its translated routines.
This includes `FirmwareTables.hpp`, the firmware-derived content of
`FirmwareFilterDist.hpp`, `evidence/memory.json` and `evidence/selected_dsp.asm`.
The presence of a public firmware download is not a license grant from this package.

Source snapshot: [glassg333/mmnova, 602d71b043ab831b9c98811fae84e3cc82edf6cf](https://github.com/glassg333/mmnova/tree/602d71b043ab831b9c98811fae84e3cc82edf6cf).

- [BIN](https://github.com/glassg333/mmnova/blob/602d71b043ab831b9c98811fae84e3cc82edf6cf/decompiled%20data/01_firmware/elektron_sfx6-60_os1.32b.bin)
- [SYX](https://github.com/glassg333/mmnova/blob/602d71b043ab831b9c98811fae84e3cc82edf6cf/decompiled%20data/01_firmware/Elektron_SFX6-60_OS1.32B.syx)
- [Kernel disassembly](https://github.com/glassg333/mmnova/blob/602d71b043ab831b9c98811fae84e3cc82edf6cf/decompiled%20data/04_listings/dispatch/dsp1_kernel_P0000-0B4D.txt)
- [User manual OS 1.32](https://github.com/glassg333/mmnova/blob/602d71b043ab831b9c98811fae84e3cc82edf6cf/decompiled%20data/01_firmware/monomachine_manual_OS1.32.pdf)

The mnemonics come from that listing; machine words were checked against the
decoded firmware. Existing hand-decompiled “Exact” filter/distortion headers
in that repository were not used as implementations in this package.

## Monomodule

[shnolk/monomodule, 214d4b95d6147fffdd99a02ca8d4dc3a45c83fd1](https://github.com/shnolk/monomodule/tree/214d4b95d6147fffdd99a02ca8d4dc3a45c83fd1)


Relevant source files: `src/core/firmware/Firmware.cpp`,
`src/core/host/HostModel.cpp`, `HostModel.h`, `Machines.h`.

## DSP56300 reference

[dsp56300/dsp56300, c051afad31612c2d2c7a81a7ab23e1c5ac9e61af](https://github.com/dsp56300/dsp56300/tree/c051afad31612c2d2c7a81a7ab23e1c5ac9e61af)
was read to check arithmetic and transfer semantics. In particular:
`source/dsp56kEmu/dsp_ops_alu.inl`, `dsp_decode.inl`, `dsp_ops_move.inl`,
`types.h`, `dsp.h` 

`DspArithmetic.hpp` is a small separately written semantic translation for
the selected instructions, not a copy of the full emulator. It omits CPU features
which the selected code does not consume. The Python model follows the same
reference, so agreement between it and the intended C++ semantics is not an
independent hardware oracle.

## Emulator requested for reference

[joelanders/gearmulator-md-mm](https://github.com/joelanders/gearmulator-md-mm)
is a suitable integration reference named in the task. It was not built or run
for these results. No emulator-output comparison is claimed.
