# Monomachine Nova 1.9.4 — LFO / Matrix repair and 64-step ARP / P-LOCK

Date: 2026-09-27

## LFO, Matrix, and MSEG destinations

- Direct LFO `PAGE`/`DEST` selection is driven by one canonical mapping used by DSP, face-panel UI, popup menus, target text, and routing menus.
- Each P1 LFO has the same hierarchy: `PITCH`; P1 `SYNT`, `AMP`, `FILT`, `EFFX`; `P1 LFO` 1–6; `P2 PARAM` (`SYNT`, `AMP`, `FILT`, `EFFX`); and `P2 LFO` 1–6. P2 is the corresponding mirror.
- All six LFO pages per side bind the same eight configuration controls. Legacy raw PAGE values remain stable for host automation and existing states.
- Direct LFO destinations show the target they actually drive. Decorative grouping separators are not destinations; categorical BBOX slot/random/chromatic controls stay excluded from direct LFO modulation.
- A direct PAGE/DEST edit leaves P1/P2 and the displayed LFO page unchanged. Its visible target acknowledgement is a single short fading frame; each new edit replaces the previous frame rather than accumulating a trail. Starting a cord drag and completing a direct patchcord clear this feedback immediately.
- P1/P2 prefixes remain unchanged in parameter values and popup menus. Only the PAGE/DEST faceplate renderer may shorten a leading `P1 ` or `P2 ` and draw a small side badge, keeping the full canonical name available everywhere else.
- Crossing the P1/P2 icon during an active modulation-cord drag is the sole automatic side-navigation path. It toggles once per icon entry and gives the icon short feedback; the source LFO page is captured before the drag so rollback always restores the correct P1/P2 source.
- Cord navigation across LFO 1–6 requires intent: the pointer must dwell briefly inside the inner part of another LFO button. Fast crossings do not change the visible page; a deliberate release on a button remains a direct target-selection action.
- Matrix target IDs are `0…247`. Existing IDs remain stable; appended IDs `244…247` are `ARP START`, `ARP END`, `P-LOCK START`, and `P-LOCK END`. MSEG RATE remains `236…243`; raw APVTS destination zero remains `OFF`.
- `MODE DLY` is positioned above `DTIM`, not `SRR`. New P1/P2 instances default FILT and DLY to `old` while retaining the stable `mnm|old` value order and all existing saved mode values.

## FILT state continuity repair

- The P1/P2 DSP-mode snapshot runs once for each host callback. The former `setModes()` path called `mnmFilter.setSampleRate(sr)` on every snapshot. For the native BASE/WIDTH path this was redundant; for an imported Korg-35 or ladder/Odin side it called adapter `prepare()` and cleared its integrator state at the host-buffer cadence. That made an otherwise isolated P1 FILT sound blocky, clicky, or envelope-like, most noticeably with BASE/Q movement and high resonance.
- Sample-rate setup now belongs to `prepare()` and `RealFilterCore::setSampleRate()` is explicitly idempotent at an unchanged rate. A real sample-rate change still prepares the adapters normally; an unchanged callback no longer touches filter state or coefficients.
- This does not change the stable `mnm|old` mode values, selected hybrid algorithm, saved parameter values, P1/P2 routing, or the separate `SAT=0` exact bypass / one opt-in post-FILT SAT stage.
- `MnmRealFilterTests` is registered as a JUCE-free CTest target. Its continuity regression runs all eight imported side algorithms against a reference while deliberately repeating the same 48 kHz setup at irregular callback boundaries; the output must remain bit-exact. The unfixed core fails that regression with measurable discontinuities.

## ARP and P-LOCK

- The normal ARP uses one inclusive absolute `START…END` window over steps 1–64. The default is 1–16; `START == END` is a one-step loop.
- The canonical ARP layout uses normal lane `[21,97,1129,197]`, P-LOCK lane `[21,341,1129,96]`, and a compact P-LOCK header immediately above it: aim `[399,312,40,26]`, slot `[439,312,159,26]`, polarity `[598,312,74,26]`. The former stray hardcoded `P-LOCK` caption is removed.
- Normal `START` and `END`: double-click resets the pair to `1…16`; RMB opens a black themed numeric-entry dialog rather than the default light JUCE dialog.
- `P-LOCK SYNC` defaults ON and retains legacy behavior: every ARP note uses its current absolute ARP step for P-LOCK lookup. With SYNC OFF, `P-LOCK START…END` is an independent 1–64 circular cursor advanced once per ARP note, so P-LOCK rhythms can be offset or truncated independently.
- P-LOCK START/END are APVTS parameters, are host-automatable, and can be addressed through Matrix targets `246` / `247`; normal ARP boundaries are matrix targets `244` / `245`.
- Two compact shift rows move the entire normal 64-step sequence or the current P-LOCK slot's 64-step values/slide flags left or right by one cyclic step. Their `N`/`P` labels and the P-LOCK START/END captions follow the actual component bounds, so saved layout edits do not leave text at obsolete rows.
- The ARP and P-LOCK lanes compress or expand to their active window. P-LOCK no longer reserves a visible left utility well: its value and slide cells derive their canvas, cell width, and columns from the normal ARP track. This remains correct with the saved layout override (including the negative-x P-LOCK lane), while the separate shift controls retain their own layout positions. Stored step IDs remain `arp_s<page>_<step>_*` for host/state compatibility.
- STEP RND shuffles the whole active ARP window. Its shuffle bag is invalidated whenever a live boundary changes. PAGE RND remains a committed normal-step writer.
- P-LOCK slide lookup is restricted to its actual P-LOCK window end. Matrix route-depth IDs `67…130` remain internal matrix destinations and are deliberately not selectable through the P-LOCK destination UI.
- Schema 21 migrates old page/page-limit state into the first 64 contiguous stored steps. Schema 22 adds P-LOCK SYNC and independent defaults while forcing migrated projects to SYNC ON.

## Validation boundary

`ArpWindowTests` is a JUCE-free regression test for 64-step traversal, a one-step window, live window modulation, and shuffled-window trimming. `MnmRealFilterTests` covers native table behavior plus imported Korg/Odin state continuity across redundant callback-rate setup; `ModEnvMatrixTests` covers the appended Matrix outputs `244…247`; `ModeRollbackTests` checks the schema-22 selector migration. The source verifier also checks the imported-filter setup guard, transient direct-LFO feedback, faceplate-only P1/P2 compaction, dwell-gated cord navigation, and normal/P-LOCK track alignment. The shared implementation, tests, and verifier are mirrored byte-for-byte in Synth and FX.

The workspace did not include CMake or JUCE module sources, so a full plugin/UI build and DAW runtime test must be run in a JUCE/CMake environment before binary release.
