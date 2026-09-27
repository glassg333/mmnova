# Monomachine Nova 1.9.4 — LFO / Matrix repair and 64-step ARP

Date: 2026-09-27

## LFO, Matrix, and MSEG destinations

- Direct LFO `PAGE`/`DEST` selection is now driven by one canonical mapping used by DSP, face-panel UI, popup menus, target text, and cross-surface navigation.
- Each P1 LFO has the same hierarchy: `PITCH`; P1 `SYNT`, `AMP`, `FILT`, `EFFX`; `P1 LFO` 1–6; `P2 PARAM` (`SYNT`, `AMP`, `FILT`, `EFFX`); and `P2 LFO` 1–6.
- Each P2 LFO is the mirror: P2 parameters, P2 LFO 1–6, P1 parameters including `PITCH`, then P1 LFO 1–6.
- All six LFO pages per side bind the same eight configuration controls. The legacy raw PAGE values remain stable for host automation and existing states.
- Direct LFO destinations show the target they actually drive. Decorative grouping separators are not destinations.
- Categorical BBOX slot/random/chromatic controls remain readable from old state/matrix data but are excluded from direct LFO modulation.
- Selecting or hovering a direct target on the other surface switches to that P1/P2 surface and flashes the target, including LFO-to-LFO targets.
- Matrix target IDs are 0–243. MSEG RATE uses 236–243; raw APVTS destination zero remains `OFF`.

## ARP

- The fixed eight-step/page playback model is replaced by one inclusive absolute `START…END` window over steps 1–64. The default is 1–16; `START == END` is a one-step loop.
- The ARP and P-LOCK lanes compress or expand to the active window. Stored step IDs remain `arp_s<page>_<step>_*` for host/state compatibility.
- STEP RND shuffles the whole active window. Its shuffle bag is invalidated whenever either window boundary changes.
- PAGE RND is a one-shot normal-step writer: enabling it queues message-thread, host-notified writes of HOLD / TRANSPOSE / VELOCITY into the active range. It no longer uses temporary grey values, and disabling it retains the generated pattern.
- P-LOCK slide lookup is restricted to the live active window end. Matrix route-depth IDs 67–130 remain internal matrix destinations and are deliberately not selectable through the P-LOCK destination UI.
- Schema 21 migrates old page/page-limit state into the first 64 contiguous stored steps, preserves old parameter IDs, and writes the schema marker after loading.

## Validation boundary

`ArpWindowTests` is a JUCE-free regression test for 64-step traversal, a one-step window, and a live STEP RND trim. `LfoMappingTests` checks the canonical P1/P2 mapping when JUCE is available. The shared implementation and both tests are mirrored byte-for-byte in Synth and FX.

The workspace did not include CMake or JUCE module sources, so a full plugin/UI build and DAW runtime test must be run in a JUCE/CMake environment before binary release.
