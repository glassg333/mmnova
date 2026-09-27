# Monomachine Nova 1.9.4 — source package

This is a source-only delivery containing the complete paired project tree:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

Read [`RELEASE_1.9.4_LFO_ARP.md`](RELEASE_1.9.4_LFO_ARP.md) first for the LFO / Matrix / ARP implementation, state migration, compatibility details, and validation boundary.

Each product directory includes its own CMake and Projucer metadata, `SOURCE_BUILD.json`, `Check-Build.ps1`, and `verify_dsp_mode_patch.py`. No VST3 binary is included.

The release archive root uses a dash-separated release name; the original product directory names with underscores are preserved unchanged.
