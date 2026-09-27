# Monomachine Nova Synth 1.9.9

Source-only patch release. This Synth package retains 1.9.8's `DLY mnm|old|new`
feedback-filter work and adds the following hybrid physical-filter choices on
both P1 and P2:

- `R 303` — LP, BP, HP;
- `R MS20` — LP, BP, HP;
- `R MOOG` — LP12/LP24, BP12/BP24, HP12/HP24.

The old hybrid IDs `0..8` are untouched. The new responses append as IDs
`9..20` under schema 24. MODE L orders each filter family as HP → BP → LP;
MODE H orders it as LP → BP → HP, matching the lower/upper physical edges.
The popup is grouped and screen-aware so all choices remain reachable.

The source equations are an attributed, self-contained adaptation of the
user-supplied TB303/MS20/Moog files, not a claim that the incomplete upstream
`Filter.h` framework was compiled verbatim.

See [`../RELEASE_1.9.9_R_CLASSIC_FILTERS.md`](../RELEASE_1.9.9_R_CLASSIC_FILTERS.md)
for exact mapping, provenance, migration, safety behavior, and validation.
