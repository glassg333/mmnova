#!/usr/bin/env python3
"""Run the native-DSP rollback static checks for both release products."""

from pathlib import Path
import subprocess
import sys


def main() -> int:
    root = Path(__file__).resolve().parent
    result = 0
    for product in ("Monomachine_Nova_Synth", "Monomachine_Nova_FX"):
        script = root / product / "verify_dsp_mode_patch.py"
        source = root / product / "Source"
        print(f"\n===== {product} =====", flush=True)
        completed = subprocess.run([sys.executable, str(script), str(source)], check=False)
        result |= completed.returncode
    return result


if __name__ == "__main__":
    raise SystemExit(main())
