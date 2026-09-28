# 1.9.15 source validation

Validated on both final source trees:

- `Monomachine_Nova_Synth`
- `Monomachine_Nova_FX`

## Static and source-audit validation

1. `verify_dsp_mode_patch.py` passed in both trees.
2. Each refreshed `SOURCE_BUILD.json` SHA-256 inventory was verified after all
   edits: 162 entries in Synth and 162 entries in FX.
3. Mirrored production DSP/UI/schema files and new standalone test sources were
   compared byte-for-byte between Synth and FX where their product identity
   does not intentionally differ.
4. Source/tests were checked for removed generic native clip symbols:
   no `real_detail::softClip` or `kOutCeiling` remains.
5. CMake, `.jucer`, generated JUCE metadata, and visible build markers identify
   the source candidate as `1.9.15`.

## Standalone C++17 DSP targets

The following targets were compiled with `g++ -std=c++17 -O2 -I Source` and run
successfully in **both** source trees:

1. `MnmCoreTests`
2. `DelayFeedbackDspTests`
3. `DistVariantTests`
4. `TrackDistVariantIntegrationTests`
5. `MnmRealFilterTests`
6. `HybridDspTests`
7. `FilterRouteTests`
8. `TrackFilterRouteIntegrationTests`
9. `Import2FiltersTests`
10. `FilterExtrasDspTests`
11. `ModEnvMatrixTests`
12. `ArpWindowTests`

`DistVariantTests` verifies exact zero identity, preservation of the old
reference law, candidate endpoint/curve behavior, and finite sweeps.
`TrackDistVariantIntegrationTests` compiles the production `TrackDIST.inl`
body with its real MODE S enum and verifies exact dispatch for IDs 6–8.

## Not performed

No full JUCE plug-in/editor build, VST3 load, DAW session, interactive GUI
check, or final listening judgment was performed in this environment. The
package is source plus source-level validation; it is not evidence of a
compiled binary or a DAW CPU percentage.
