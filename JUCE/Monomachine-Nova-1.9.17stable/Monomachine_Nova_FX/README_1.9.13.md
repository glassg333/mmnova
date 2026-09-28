# Monomachine Nova FX 1.9.13

Source-only physical-filter sound/control follow-up.

When a MODE L/H side selects K35, MOOG/ladder, R CLASSIC, or R Import 2, it
uses the separate `IndependentPhysicalFilterCore` route from 1.9.12. In 1.9.13
that selected route no longer receives the native MNM carrier's generic output
soft clip: resonance is not silently made into a common saturation stage.
`FILT SAT=0` remains an exact bypass; a selected model's own documented
nonlinear behaviour is not replaced by a global limiter.

Physical cutoff/resonance targets now ramp for 10 ms at the real sample rate.
K35 and ladder ignore repeated identical host targets while a ramp is running,
so a live BASE/WDTH move is not rearmed at the filter-envelope cadence.

NATIVE/NATIVE `DSP FILT=old|mnm`, schema 25, IDs 0..51, physical side semantics,
R CLASSIC menu, R HUV/R DVAL repairs, and DLY `mnm|old|new` / DBAS/DWID/Q are
retained. See
[`../RELEASE_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md`](../RELEASE_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md)
and
[`../VALIDATION_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md`](../VALIDATION_1.9.13_PHYSICAL_FILTER_SOUND_AND_SMOOTHING.md).
