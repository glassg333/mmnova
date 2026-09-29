# Monomachine Nova 1.9.13 — selected physical-filter sound and control smoothing

## Reason for the follow-up

A listening report against 1.9.11 found permanent clip-like compression around
resonant low-BASE LP use and audible crackle while moving `BASE`. The reporter
confirmed that the clip-like character was present continuously and the crackle
was most evident during movement, across physical filter families.

This is treated as a regression report, not as intended analogue character.
The 1.9.11 archive remains unchanged and is not reissued. Version 1.9.12's
separate route is retained as history; this is a new patch version.

## Diagnosed paths

1. The former selected-family carrier path used `RealFilterCore::process()`.
   That method applied the port-only native MNM `softClip()` after every output,
   including K35/MOOG/ladder/R outputs. It can colour a resonance peak even
   when the user has not enabled `FILT SAT`.
2. Physical adapters used a fixed 16-sample coefficient ramp. At 48 kHz that
   is only about 0.33 ms. In addition, K35 and ladder compared incoming targets
   with their moving current control, so an unchanged snapshot could re-arm the
   ramp while it was still progressing.

## Implemented correction

- `IndependentPhysicalFilterCore` calls the same side-specific physical
  primitives through `processIndependentPhysical()`, which retains DC removal
  and a finite-value emergency recovery but does **not** apply the generic MNM
  output soft limiter.
- `FilterCore::process()` still retains the native MNM safety law for the
  explicit NATIVE/NATIVE `DSP FILT=mnm` renderer. OLD and saved IDs/schema are
  untouched.
- K35, ladder, R Classic, and R Import 2 adapters now use a sample-rate-aware
  10 ms control ramp (`441/480/960` samples at `44.1/48/96 kHz`).
- K35 and ladder now compare a changed value with the previous **target** and
  only begin a new ramp for a real target/mode change. Repeated host snapshots
  no longer restart the ramp.
- Explicit `FILT SAT` remains the only common post-FILT saturation stage.
  Its `SAT=0` bypass remains exact. Individual model equations may still carry
  their documented analogue/nonlinear behaviour; this release removes only the
  unrelated common carrier clip.

## Regressions added/strengthened

- `FilterRouteTests` drives a selected K35 physical core above the old native
  soft-clip knee and proves that the independent result is un-clipped while a
  separate native carrier still follows its own retained safety law.
- `HybridDspTests` verifies the 10 ms sample-rate mapping, proves that repeated
  K35/ladder snapshots are bit-identical to uninterrupted smoothing, and bounds
  high-Q transition peaks after a large live cutoff change.
- Existing route, `TrackFILT.inl`, R-family, finite/stereo, callback-cadence,
  schema, and no-JUCE test coverage remains in place.

## Compatibility

No parameter ID, schema number, MODE L/H choice ID/order, BASE/WDTH physical
side law, R menu structure, delay mode, or saved-state migration changes in
1.9.13. The change is intentionally limited to selected physical-filter output
policy and their control de-zipper.
