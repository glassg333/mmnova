# Historical filter-route audit — cloud 1.9.8 and 1.9.9

Audit date: **2026-09-27**. This note records the source trace made before the
1.9.12 route correction. It deliberately distinguishes the historical code
from the corrected contract.

## Reviewed cloud baselines

| Baseline | Cloud location reviewed | `TrackFILT.inl` result |
|---|---|---|
| 1.9.8 | commit `788744bb242aeede1a7da219612c19bd0ff24483`, `JUCE/Monomachine-Nova-1.9.8/...` | 2,938 bytes, SHA-256 `c7beca2592527f40b482a3073fd13ac2bd6ef6ada03a86182c248caf3dcbd384` |
| 1.9.9 | main commit `507f03fa2036076ff0802366fb8e0e097774ba1c`, `JUCE/Monomachine-Nova-1.9.9 good or no idk stable/...` | byte-identical 2,938-byte file, same SHA-256 |

The present cloud tree also contains a renamed 1.9.8 copy (`Monomachine-Nova-1.9.8 good maybe`); its `TrackFILT.inl` is byte-identical to the 1.9.8 file above.

## What 1.9.8 / 1.9.9 actually did

1. `TrackChain::stageFILT()` selected **only one outer renderer**:
   - `filterMode == dspModeMnm` called `mnmFilter.process()`;
   - every other FILT mode called the retained legacy `filter.processStereo()`.
2. `setHybridTestModes()` forwarded MODE L/H choices only to `mnmFilter`.
   Consequently, a non-`NATIVE` K35, ladder, or R choice was **ignored** while
   FILT DSP was `old`; it was not literally serialised after OLD.
3. Within `MnmRealFilter.hpp`, an active K35/ladder/R choice **replaced the
   native stage on that same physical side**:
   - lower MODE L: selected family instead of the native lower/BASE stage;
   - upper MODE H: selected family instead of the native upper/BASE+WDTH stage.
   The opposite side continued to use its own explicitly selected MODE choice.
   Thus a `NATIVE` opposite side is a historic physical-side companion, not an
   OLD or full-MNM outer pass added before/after the selected family.
4. 1.9.9 appended R 303 / R MS20 / R MOOG IDs `9..20`; it retained exactly
   this side-replacement topology. 1.9.8 IDs `0..8` were unchanged.

The historical defect was therefore **gating**: special MODE selections needed
FILT DSP=`mnm` to be audible. It was not evidence that OLD was meant to be
stacked with K35/R.

## 1.9.12 correction derived from the trace

1.9.12 makes the renderer decision explicit:

```text
MODE L=NATIVE and MODE H=NATIVE:
    FILT DSP=old  -> retained OLD native renderer
    FILT DSP=mnm  -> retained MNM native renderer

Either MODE side is non-NATIVE:
    -> IndependentPhysicalFilterCore only
       (OLD renderer: zero calls; MNM native renderer: zero calls)
```

The independent core has separate state from `mnmFilter`, retains the verified
historical lower/upper side-replacement law, and is not selected by FILT DSP.
A selected K35/R side runs once. The only possible native side in that path is
the other MODE control when it itself remains explicitly `NATIVE`.

`FilterRouteTests` executes the same `dispatchFilterRenderRoute()` helper used
by `TrackFILT.inl`. `TrackFilterRouteIntegrationTests` also includes the
production `TrackFILT.inl` unchanged with counted renderer stand-ins. Together
they prove OLD and MNM callbacks are both zero while a selected K35/R callback
is exactly one per channel/sample, for either FILT DSP value. They also verify
unchanged selected snapshots do not reset the independent state.

This is source-level/standalone evidence, not a DAW-host claim.
