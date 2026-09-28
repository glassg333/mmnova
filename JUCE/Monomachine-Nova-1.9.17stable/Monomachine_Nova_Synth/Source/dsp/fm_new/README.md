# FM NEW source provenance and isolation

`MODE SYNT = new` for FM+ STAT (m8), FM+ PAR (m9), and FM+ DYN (m10) owns
the isolated raw cores in this directory. `MODE SYNT = new fix` uses those
same independent cores plus the explicit measured control-law table under
`Source/dsp/fm_fix/`. Neither choice is a replacement for `Source/dsp/mnm/`
or calls the local `old` FM implementations.

## Reviewed import

These five raw cores were selectively imported from:

`glassg333/mmnova`,
`decompiled data/juce/fm/!fm fix patch/1_NEW_code_FM_2026-09-28/dsp/mnm/`

| original | isolated file | upstream SHA-256 |
| --- | --- | --- |
| `MnmFmSineTable.h` | `FmExactSineTable.h` | `e43b0dabae626d8a6c1ac34d2dcaea997bea4df3fd352dbac7ca6f7a9bee4445` |
| `MnmFmDsp.hpp` | `FmExactDsp.hpp` | `0ab1251a3053eb81f67da370cd7c318e035134a1fc41cd059c4937295f938df0` |
| `MnmFmStat.hpp` | `FmExactStat.hpp` | `935d436da340a2034413406fa346620fc66dfded2668876adcdbc1604b614568` |
| `MnmFmPar.hpp` | `FmExactPar.hpp` | `b790e0602347974e0ba5bc2557ec3a382134b9de81b115cc1f9e580a4a9d3dac` |
| `MnmFmDyn.hpp` | `FmExactDyn.hpp` | `6498815fb603fa0b27aca313d2b2e231d1e15895edf7593e68147c5b5730ec83` |

The raw namespace changed from `mnmfm` to `fmnew`, include names changed to
match this directory, and `FmExactDsp.hpp` replaces signed left shifts of
negative operands with range-safe multiplication by two.  This is a C++
undefined-behaviour repair, not a DSP-law change: the supplied STAT vector
harness passed 52,800/52,800 words after the repair.

`FmExactNew.hpp` is a deliberately small host wrapper.  It has independent
state and an exact-core FIFO; it does not depend on `MnmKernel.hpp`, the
current `MnmFm.hpp`, or `monomachine_fm_par/dynamic.hpp`.

## Scope and validation boundary

The upstream package supplies a complete STAT vector corpus and harness; its
PAR/DYN vector corpus is referenced by the upstream proof but was not shipped
inside this package.  Local regression tests therefore prove STAT's supplied
vectors, wrapper FIFO consistency, deterministic PAR/DYN smoke behaviour,
mode isolation, and legacy-core fingerprints.  They do **not** establish
hardware/DAW/listening equivalence for every PAR/DYN setting.

The firmware machine PROC does not consume the `TUNE` knob itself; the kernel
applies output pitch before dispatch. The retained `new` bridge preserves its
previous approximately +/-1-semitone mapping for A/B comparison. The separate
`new fix` bridge uses the user-measured hardware endpoints: raw `0`/`64`/`127`
map to `-2`/`0`/`+1.96875` semitones. The exact native kernel pitch-word
quantisation still needs a dedicated reference vector before either bridge can
be described as bit-exact end-to-end.
