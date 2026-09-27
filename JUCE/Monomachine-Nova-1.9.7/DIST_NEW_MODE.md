# MODE S / DIST status — schema 19

Date: **2026-09-26**.

MODE S is the only user-facing selector for the existing post-filter DIST block:

| Stored value | Mode |
|---:|---|
| 0 | `MNM` — retained local MNM reference |
| 1 | `OLD` — untouched bipolar reference |
| 2 | `MNM FIX` — experimental 3.5 kHz notch-compensation A/B candidate |
| 3 | `FOLD` — imported character under MNM control progression |
| 4 | `ZERO` — imported character under MNM control progression |
| 5 | `CLAMP` — imported character under MNM control progression |

`MNM FIX` uses a +3 dB Q=1.0 peak at 3.5 kHz after the retained MNM result,
blended only above DIST=64. It was added at the user's request as a separate
comparison mode; it is not claimed to reproduce the unresolved per-machine
firmware handler.

MNM and OLD remain unmodified. FOLD/ZERO/CLAMP retain the MNM-compatible
control law: native result through DIST=64, then retained positive drive/output
terms with a continuous character blend.

Schema 19 shifts an existing schema-18 FOLD/ZERO/CLAMP value from 2/3/4 to
3/4/5, preserving its sound after inserting MNM FIX. The former `mode_dist`
state remains migration-only. Legacy `mode_dist=old` still migrates to explicit
MODE S OLD because OLD won over private selection in the prior renderer.

The block order remains:

```text
EQ → FILT → DIST → ENV → VOL/PAN → SRR → DELAY
```

See `RELEASE_1.9.2_MNM_FIX_AND_IMPORTED_DECLICK.md` for the test definition
and `HYBRID_FILTER_AUDIT_2026-09-26.md` for evidence boundaries.
