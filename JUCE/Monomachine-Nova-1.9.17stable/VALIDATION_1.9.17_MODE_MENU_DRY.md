# 1.9.17 validation — MODE L/H, DRY and Hyperion ordering

## UI contract

- MODE L normal selection consists of HP/low-cut, NOTCH, BAND PASS, then final DRY.
- MODE H normal selection consists of LP/high-cut, NOTCH, then final BAND PASS.
- Each response folder puts ordinary families first and Hyperion last.
- DRY contains only the six explicitly direct-input LP-derived HP diagnostic modes and is MODE L-only.
- Popup, wheel, and drag share the visible-item gate, so hidden inverse response types cannot be reached through ordinary browsing.

## Performed validation

- Static integrity checker passed for Synth and FX.
- `Import2FiltersTests`, `HybridDspTests`, and `FilterRouteTests` compiled and passed in each tree.
- Synth/FX modified source/test/verifier copies are byte-identical.
- Product `SOURCE_BUILD.json` manifests were regenerated and verified.

## Not performed

No complete JUCE UI build, plug-in render, host/DAW state load, or listening test was performed here.
