# Truthful filter — proof boundary

Date: 2026-09-18. Repository commit audited:
`9dd0e6aa0c48126df1ec1cfa9a0a808a98d00307`.

## Directly verified

* The kernel reads the primary cutoff table at `P:$143546` and clamps the
  primary index to `$6A3` before the table read (`P:$07D2..$07D9`). The complete
  1700-word table is stored in `data/filt_cutoff_P143546_1700.bin`.
* The second path reads `P:$1435C6` and clamps its index to `$63F` (`P:$0855..$0860`).
  The complete 1600-word table is stored in
  `data/filt_cutoff_P1435C6_1600.bin`.
* The coefficient preparation contains the 24-step `div` loop at
  `P:$0583..$0588`.
* The two filter loops use the fixed literals `$F528BD` and `$4A4DF0` at
  `P:$05CF..$05D1` and run over 8 iterations each (`P:$05D3..$05E7` and
  `P:$05EB..$05FB`).
* The coefficient update at `P:$056D..$0572` is performed once before the
  per-sample filter loops and has the shift/round/shift shape represented by
  `roundDelta256()`.
* The raw table bytes in this package were compared against
  `decompiled data/02_memory_images/dsp1_pmem.bin`, whose SHA-256 is
  `e99c6a6714feebb893c4f8f3eccf1b31dc900cc856814b44cd2f397d60889112`.

## Deliberately not claimed

The available evidence does not close:

* UI value 0..127 to ColdFire-scaled `V-$20`, `V-$17`, `V-$0E`, `V-$0F` words;
* exact HPQ/LPQ-to-k mapping and which of the coefficient tables is used for
  each control;
* whether the two loops are serial HP/LP sections or channel/voice paths;
* exact instruction ordering and 56-bit accumulator behavior in every update;
* full post-voice routing around the filter.

Therefore `MnmFilterTruthfulCore` accepts **raw kernel inputs** and is marked
`MODEL` in the recurrence. It must not be renamed to `Exact`, `Official`, or
`BitPerfect` without a new acceptance trace against the original kernel.
