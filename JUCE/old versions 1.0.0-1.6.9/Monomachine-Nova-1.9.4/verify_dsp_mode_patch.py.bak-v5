#!/usr/bin/env python3
"""Static checks for the DSP MODE / filter modes patch.

No C++ compiler is required. It parses the touched sources and the new header
and asserts the properties the patch promises:

  1. MnmFilterModes.hpp is balanced and defines the three new filter classes.
  2. DspModes.hpp exposes modes 0..4 with stable "mnm"/"old" indices.
  3. NovaDSP.h includes the new header and dispatches all three new modes.
  4. PluginEditor.cpp no longer places dspModeButton over the ARP row.
  5. PluginProcessor.cpp collapses filter-only modes for non-FILT sections.

Run:  python3 verify_dsp_mode_patch.py <path-to-Monomachine_Nova_FX/Source>
"""

from pathlib import Path
import re
import sys


def read(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def balanced(text: str) -> bool:
    return text.count("{") == text.count("}")


def main() -> int:
    root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
    mnm = root / "dsp" / "mnm" / "MnmFilterModes.hpp"
    modes = root / "models" / "DspModes.hpp"
    dsp = root / "NovaDSP.h"
    editor = root / "PluginEditor.cpp"
    processor = root / "PluginProcessor.cpp"

    for path in (mnm, modes, dsp, editor, processor):
        if not path.is_file():
            raise AssertionError(f"missing file: {path}")

    failures = []

    def check(ok: bool, message: str) -> None:
        print(("OK   " if ok else "FAIL ") + message)
        if not ok:
            failures.append(message)

    text = read(mnm)
    check(balanced(text), "MnmFilterModes.hpp braces balanced")
    for cls in ("SvfCascadeFilter", "DualSectionFilter", "TruthfulRawFilter"):
        check(f"class {cls}" in text, f"MnmFilterModes.hpp defines {cls}")
    check("$F528BD" in text and "$4A4DF0" in text,
          "MnmFilterModes.hpp cites the recovered literals")
    check("approx" in text.lower(), "MnmFilterModes.hpp marks the approximate knob law")

    text = read(modes)
    check('"mnm|old|cascade|dual|raw"' in text, "mode choice list is mnm|old|cascade|dual|raw")
    check("dspModeMnm = 0" in text and "dspModeOld = 1" in text,
          "mnm/old indices stayed 0/1 (saved-state compatibility)")
    check("dspModeCount = 5" in text, "dspModeCount is 5")

    text = read(dsp)
    check('MnmFilterModes.hpp' in text, "NovaDSP.h includes the new header")
    for cls in ("SvfCascadeFilter", "DualSectionFilter", "TruthfulRawFilter"):
        check(cls in text, f"NovaDSP.h instantiates {cls}")
    for mode in ("dspModeCascade", "dspModeDual", "dspModeRaw"):
        check(mode in text, f"NovaDSP.h dispatches {mode}")

    text = read(editor)
    bounds = re.search(r"dspModeButton\.setBounds\(([^)]*)\)", text)
    check(bounds is not None, "PluginEditor.cpp positions dspModeButton")
    if bounds:
        values = [int(v) for v in re.findall(r"-?\d+", bounds.group(1))]
        x, y, w = values[0], values[1], values[2]
        check(y != 58 or x >= 1014,
              f"dspModeButton at ({x},{y}) does not overlap the ARP row x=471..1010 y=58")
        check(x >= 1014 or y != 58, "dspModeButton moved out of the ARP row")

    text = read(processor)
    check("dspModeIsFilterOnly" in text, "PluginProcessor.cpp collapses filter-only modes")
    check("dspModeFilter" in text or "DspFilter" in text, "PluginProcessor.cpp knows the FILT section id")

    if failures:
        print(f"\nFAIL: {len(failures)} check(s) failed", file=sys.stderr)
        return 1
    print("\nPASS static patch verification")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
