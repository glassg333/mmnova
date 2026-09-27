# Monomachine Nova FX 1.9.3

See [`../RELEASE_1.9.3_FILT_EXTRAS_ENV.md`](../RELEASE_1.9.3_FILT_EXTRAS_ENV.md).

This source-only FX revision adds independent P1/P2 FILT extras, one opt-in
post-FILT Korg/Odin `SAT` stage, additive FIL ENV through `BASE/WIDTH`, and
four MOD ENV matrix sources. `SAT=0` and `ENV FIL=0` are neutral; MNM and OLD
remain separate references. Schema 20 safely fills neutral values for older
projects.
