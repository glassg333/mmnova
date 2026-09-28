# 1.9.14 source validation

Validated on the final source trees for both `Monomachine_Nova_Synth` and
`Monomachine_Nova_FX`:

1. `verify_dsp_mode_patch.py Source` passed in each tree. The checker now
   verifies schema 26, both tracking parameters, state migration, TrackChain
   wiring, BASE/WDTH RMB ownership, absence of the FILT-category RMB action,
   and compact FIL/MOD envelope symbols.
2. Every `SOURCE_BUILD.json` inventory hash was refreshed and verified:
   159 entries in each variant.
3. The following standalone source tests were compiled and run successfully for
   Synth: `MnmCoreTests`, `DelayFeedbackDspTests`, `MnmRealFilterTests`,
   `HybridDspTests`, `FilterRouteTests`, `TrackFilterRouteIntegrationTests`,
   `Import2FiltersTests`, `FilterExtrasDspTests`, `ModEnvMatrixTests`,
   `ArpWindowTests`, and `ModeRollbackTests`.
4. `MnmRealFilterTests` now includes manual-law tracking assertions for
   `BASE=0`, `+8` octave steps, LPF `BASE+WDTH`, played-note octave movement,
   and independent HPF-off / LPF-on behavior.
5. A legacy-filter octave-equivalence smoke check passed. Targeting C4 with
   `BASE=8` matched C5 with `BASE=0` under key tracking.
6. The key filter/route tests (`DelayFeedbackDspTests`, `MnmRealFilterTests`,
   `TrackFilterRouteIntegrationTests`, `FilterExtrasDspTests`, and
   `ModeRollbackTests`) were also compiled and run from the FX tree.
7. The final ZIP was tested with `unzip -t` after creation.

## Not performed

This is not a full plug-in build validation. No JUCE checkout was available in
the source package, and no VST3/DAW load or interactive GUI session was run.
No claim is made that a DAW has exercised the new controls; the delivered
artifact is full source plus the source-level validation above.
