# Monomachine Nova 1.9.17 source package

This source-only package contains matching Synth and FX trees. The exact visible MODE L/H lists are included in `CURRENT_MODE_L_MODE_H_1.9.17.md`.

> **Legacy-isolation repair note (2026-09-28):** `Monomachine-Nova-1.9.17.zip` and `Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` remain preserved. The earlier `POST-FM-FIX-source` and `POST-FM-FIX-UI-ISOLATION-source` archives are preserved for evidence but are **not delivery candidates**: the latter accidentally made recovered FIX words global descriptor defaults, changing fresh/reset retained-mode knobs and sound. This worktree restores the retained `MnmFm.hpp`, `FmExactNew.hpp`, and `FmExactDsp.hpp` source bytes from the candidate baseline. `mnm fix` now uses `MnmFixCore`; `new fix` uses a separately namespaced `FmExactFixCore`; neither shares a core/state object with retained `mnm` or `new`. Measured LCD FREQ/TUNE labels apply only to `mnm fix` / `new fix` for m8/m9/m10. Recovered FIX raw profiles are available only through the explicit `LOAD FIX FACTORY RAW VALUES (explicit)` action while a FIX mode is selected; they are never global defaults or automatic mode-switch writes. The delivery candidate is named `Monomachine-Nova-1.9.17-FM-LEGACY-ISOLATION-source-2026-09-28.zip`; it does not claim a full JUCE/DAW/listening-validated release. `new fix` remains the corrected-DYN-`2FRQ` listening candidate; the retained approximate MNM DYN route does not audibly mix its generated `mod2` path. `FM_NEW_AUDIT_2026-09-28.md` states the scope and validation boundary. A separate pre-FIX, route-labelled agent-handoff archive accompanies this delivery for delegated review.

## Included UI/menu layout

- MODE L exposes the normal low-cut/HP family; MODE H exposes the normal high-cut/LP family.
- `R HYPER` modes are deliberately deferred to the end of their HP/LP/BP response folders.
- All band-pass modes are grouped in a single `BAND PASS` folder.
- The six LP-derived, direct-input HP diagnostics are explicit only in the final MODE L `DRY` folder:
  `R HUV HP4`, `R KRAJ HP4`, `R MICRO HP4`, `R MUSIC HP4`, `R OBERHEIM HP4`, and `R DVAL HP4`.
  Five use `dry - LP`; DVAL uses `dry + LP` because its LP output polarity is inverted. Removing dry would not create an honest HP response.
- Popup, wheel, and drag use the same visible-choice gate. Opposite response types do not appear in normal MODE L/H selection.

The underlying 0..57 algorithm IDs remain available for state/diagnostics; this UI package does not claim a native Monomachine filter repair.

## Validation performed

The following JUCE-free C++17 tests passed separately for Synth and FX:

- `NovaChorusTests`, `MnmCoreTests`, `DelayFeedbackDspTests`, `DistVariantTests`, and `TrackDistVariantIntegrationTests`
- `MnmRealFilterTests`, `HybridDspTests`, `FilterRouteTests`, and `TrackFilterRouteIntegrationTests`
- `Import2FiltersTests`, `FilterExtrasDspTests`, `ModEnvMatrixTests`, and `ArpWindowTests`
- `FmNewModeTests`, `FmFixModeTests`, and `ModeRollbackTests`

Static integrity checks also passed for both products after the manifests were regenerated. No full JUCE/editor/VST3/DAW build or subjective listening validation was performed.

`FM_FIX_UI_VALUE_MAP_2026-09-28.md` is the explicit m8/m9/m10 FIX-only LCD/list mapping for delegated UI review. `FM_RETAINED_READOUT_ROUTES_2026-09-28.md` records the literal original m8/m9/m10 formatter branches for retained `mnm`/`old`/`new`, side by side with their final locations. `VALIDATION_1.9.17_FM_LEGACY_ISOLATION.md` records the separate-core/default/source-hash evidence for this repair.

No JUCE checkout, binary, build directory, or cache is included.
