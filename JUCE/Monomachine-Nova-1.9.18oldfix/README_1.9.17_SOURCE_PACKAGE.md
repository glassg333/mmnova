# Monomachine Nova 1.9.17 source package

This source-only package contains matching Synth and FX trees. The exact visible MODE L/H lists are included in `CURRENT_MODE_L_MODE_H_1.9.17.md`.

> **FM level / OLD FIX repair note (2026-09-28):** `Monomachine-Nova-1.9.17.zip` and `Monomachine-Nova-1.9.17-FM-NEW-candidate.zip` remain preserved. The earlier `POST-FM-FIX-source`, `POST-FM-FIX-UI-ISOLATION-source`, `FM-LEGACY-ISOLATION-source`, and incorrect `FM-LEVEL-OLD-FIX-source` archives are evidence only and are **not delivery candidates**. The retained `MnmFm.hpp`, `FmExactNew.hpp`, and `FmExactDsp.hpp` source bytes remain restored from the candidate baseline. `mnm fix` uses a separate `MnmFixCore`; `new fix` uses a separately namespaced `FmExactFixCore`; schema-31 appends `old fix=5` with separate copies of the OLD m9/m10 renderer topology plus measured controls, without renumbering the already serialized `new fix=4` or touching retained `old`. Measured LCD FREQ/TUNE labels apply only to `mnm fix`, `new fix`, and `old fix` for m8/m9/m10. `new` and `new fix` now receive the user-requested explicit +10 dB host bridge after their raw exact cores; the raw exact headers are not edited. Recovered FIX raw profiles remain available only through `LOAD FIX FACTORY RAW VALUES (explicit)` while a FIX mode is selected; they are never global defaults or automatic mode-switch writes. The delivery candidate is named `Monomachine-Nova-1.9.17-FM-OLD-TOPOLOGY-LEVEL-source-2026-09-28.zip`; it does not claim a full JUCE/DAW/listening-validated release. `new fix` remains the corrected-DYN-`2FRQ` listening candidate; the retained approximate MNM DYN route does not audibly mix its generated `mod2` path. `FM_NEW_AUDIT_2026-09-28.md` states the scope and validation boundary. `SYNTH_FX_SIDE_BY_SIDE_FM_OLD_TOPOLOGY_HANDOFF_2026-09-28.md` provides the paired source-route handoff. A separate pre-FIX, route-labelled agent-handoff archive accompanies this delivery for delegated review.

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
