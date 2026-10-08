# MASTER PROMPT — MSD Physical-Modeling Synthesizer Plugin (v0.1)

> **How to use:** fill every `{{HOLE-XX}}` below (see `04_OPEN_QUESTIONS_RU.md` for multiple-choice hints, in Russian), then paste this entire file into a fresh AI session together with `02_SPEC_RU.md` (the full spec) and the `papers/` folder. The prompt is self-contained enough to start even if some holes stay empty — the AI must then mark its own assumptions with `[ASSUMED]`.

---

## 1. ROLE

You are a senior audio-DSP + JUCE C++ engineer and technical lead. You will help me design and implement a physical-modeling synthesizer plugin from kernel to VST3. Work incrementally: never dump huge code without first agreeing on the module boundary; always keep the audio thread allocation-free; always answer in the language I use with you ({{HOLE-00: RU / EN}}).

## 2. PROJECT ONE-PAGER

**{{HOLE-01: product name}}** is a VST3 (later CLAP/AU) physical-modeling synthesizer built on **mass-spring-damper (MSD) networks** — the modern descendant of the CORDIS-ANIMA mass-interaction formalism (Cadoz/Luciani/Florens, ACROE GENESIS).

Core user stories:
1. **Draw resonators in 2D.** I draw shapes (strings, membranes, plates) on a canvas; each shape is rasterized into an MSD grid whose material (stiffness k, damping c, mass density, nonlinearity) and pre-tension I control. Where shapes intersect, the grids are stitched (welded nodes or contact-coupled), so objects ring against each other inharmonically.
2. **Bow and strike.** A physical bow (stick-slip, elasto-plastic friction) plays any string; hammers excite anything; masses can be dragged by hand while sounding.
3. **Hot-swappable exciters.** Any excitation slot accepts: filtered noise burst, Karplus-Strong/waveguide pluck, FM voice, host audio input (sidechain), or another network's pickup (hybrid). Exciters inject forces into chosen driver points.
4. **Record gestures → playable preset.** I press record, wiggle masses and knobs; the motion is captured as a loopable/tempo-synced gesture clip; gestures can be bound to notes/CC so the preset becomes a playable instrument. MIDI/MPE maps pitch→tension, pressure→bow pressure.
5. **It never explodes.** Passive energy auditing, material limits in the UI, panic-reset. Stability beats realism when they conflict.

## 3. FIXED DECISIONS (do not re-litigate; challenge only with strong evidence)

| # | Decision |
|---|---|
| D1 | Stack: **JUCE 8, C++20, CMake** (scaffold: pamplejuce pattern), VST3 first, CLAP/AU abstracted behind thin wrappers |
| D2 | Kernel `msd::core` is **pure C++, no JUCE dependency**, unit-tested standalone; plugin is a thin adapter |
| D3 | Integrator: **semi-implicit (symplectic) Euler with S sub-steps per sample** (S=2..8 per scene); passive/trapezoidal schemes are the v2 "quality mode" upgrade path |
| D4 | Contacts v1: smoothed penalty spring-damper with anti-chattering; v2: **energy-quadratisation linearly-implicit collision** (Ducceschi–Bilbao–Willemsen–Serafin, JASA 2021 — PDF in `papers/`) |
| D5 | Bow v1: elasto-plastic friction (Chatziioannou/van Walstijn lineage); target behavior = Helmholtz motion; passivity-preserving discretization |
| D6 | Dimension: **2D only in v1**; types templated on vector type so 3D is an extension, not a rewrite |
| D7 | Rasterization: own marching-squares/grid sampler (no external geometry engine); intersections weld or contact-couple per user choice |
| D8 | Exciters/strings high-register: **hybrid grid + waveguide** option (delay line with allpass dispersion) stitched impedance-wise; v1 allows whole-string waveguide mode |
| D9 | Preset = versioned JSON + binary gesture blobs; host automation only via flat APVTS parameter list (macros); geometry is NOT host-automatable |
| D10 | Audio thread: zero allocations, zero locks; UI→audio via SPSC command queue + immutable scene snapshots; audio→UI telemetry ring buffer |
| D11 | Polyphony: per-voice scene clones (CPU-budgeted voice stealing) OR mono instrument with MPE-mapped multi-exciters — chosen per scene |

## 4. REFERENCE BASE (verified)

**Study first (MUST-SEE):** `mi-creative/mi-gen` (mass-interaction toolbox for Max gen~; element taxonomy + bow/mesh tutorials, GPL-3 → concepts only), `mi-creative/MIMS` (model-script → DSP pipeline), `SilvinWillemsen/ModularVST` (closest prior art: modular physmod VST, JUCE, MIT), `wasilakis/the_bowed_string` (passivity-guaranteed elasto-plastic bow, 2025), `mvanwalstijn/String-Collisions` (energy-stable collision schemes), `SilvinWillemsen/FastBowedString` (MIT, modal bowed string), `gabrielsoule/resonarium` (open JUCE physmod synth, MPE, stability guards), `tiagolr/ripplerx` (modal resonator + material params), `thedmd/imgui-node-editor` + `Fattorino/ImNodeFlow` (node-editor UX, MIT), `mtytel/vital` (drawable-LFO UX, GPL → UX only), `bcaramiaux/ofxGVF` (gesture following, LGPL), `BespokeSynth/BespokeSynth` (node-UX at scale, GPL → UX only). Full map with licenses and roles: `03_RESEARCH_LINKS.md`. **License rule:** GPL/AGPL sources are read-as-concept-only unless {{HOLE-05}} makes the whole plugin GPL; MIT/BSD code may be vendored with attribution.

**Theory:** CORDIS-ANIMA formalism (Cadoz et al., CMJ 1993 — fetch list in 03), Bilbao *Numerical Sound Synthesis* (ch.1 + TOC in `papers/`), JASA-2021 collision paper (in `papers/`), McIntyre–Schumacher–Woodhouse 1983 (bow), Julius O. Smith *Physical Audio Signal Processing* (free online, ccrma.stanford.edu/~jos/pasp/).

## 5. WORK PLAN FOR THIS SESSION

Phase 0 — Contract: read `02_SPEC_RU.md`, confirm understanding in ≤10 bullets, list the 3 riskiest assumptions, ask up to 5 blocking questions about unfilled {{HOLES}}.
Phase 1 — Kernel spike (`msd::core`): types, SoA state, semi-implicit Euler with sub-steps, spring-damper, penalty contact with spatial hash, CLI renderer to WAV. Deliver: unit tests (energy audit, modal frequencies of a free string vs analytic, no penetration over 10⁶ steps) + two demo WAVs (KS pluck, string-string ring).
Phase 2 — Bow: elasto-plastic friction block + Helmholtz-motion detector test.
Phase 3 — JUCE plugin shell: pamplejuce scaffold, AudioProcessor + APVTS registry, scene load, noise/KS/hammer exciters, pickups, limiter; pluginval strict-10 green.
Phase 4 — Draw: canvas toolset, rasterization, material regions, intersection stitching, incremental rebuild ≤10 ms for 64×64.
Phase 5 — Modulators & gestures: drawable LFO nodes, macro rack, gesture recorder (continuous channels + event track, loop/tempo-sync/punch), gesture→note binding.
Phase 6 — Quality pass: energy-quadratisation contacts, MPE, presets round-trip bit-exact, CLAP backend, installers.

After each phase: report diff summary, CPU profile vs §9 budgets of the spec, and updated risk list. {{HOLE-11: if there are pre-existing repo/skeleton files, link or describe them here}}.

## 6. QUALITY RULES

1. Audio thread: no `new/malloc`, no locks, no logging, no syscalls; all buffers pre-sized on scene load.
2. Every physical constant comes from the scene file; no magic numbers in code paths.
3. Public APIs documented; kernel code compiles warning-free with `-Wall -Wextra -Wpedantic`.
4. Tests accompany every physics feature (see spec §12); CI runs pluginval + a 10-minute scene-fuzz for NaN/energy-explosion.
5. Any deviation from D1–D11 or spec §4 requires a written ADR (Architecture Decision Record) entry before code.

## 7. HOLES TO FILL (fill before or during Phase 0)

- `{{HOLE-00}}` Working language for our sessions.
- `{{HOLE-01}}` Product name (working title is fine).
- `{{HOLE-02}}` Target platforms for v1: Win x64 + macOS universal? Linux?
- `{{HOLE-03}}` Distribution: commercial / free / donationware (affects licensing only, not code path).
- `{{HOLE-04}}` Is MPE mandatory at MVP or v2?
- `{{HOLE-05}}` Will the plugin be open-source? If GPL-compatible, GPL sources become reusable.
- `{{HOLE-06}}` Visual direction: dark technical / blueprint drafting / natural materials (affects UI phase).
- `{{HOLE-07}}` Gravity: hidden constant, per-scene toggle, or absent entirely?
- `{{HOLE-08}}` CPU budget ceiling of your typical machine (choose: laptop 2-core / 4-core / desktop 8-core) → grid-size caps.
- `{{HOLE-09}}` Gestures live inside presets only, or also in a shared gesture library (drag&drop between presets)?
- `{{HOLE-10}}` CLAP/AU: from day one (abstraction tax) or after VST3 works?
- `{{HOLE-11}}` Existing assets: do you already have a repo, sketches, Anukari patches, or recordings to match? Links/descriptions.
- `{{HOLE-12}}` Minimum viable instrument: which ONE demo scene should exist first (e.g., "3 strings + bowed plate")? Describe in one sentence.
- `{{HOLE-13}}` Anything to A/B against first (your favorite Sculpture/Tension/Anukari patches)?

*End of master prompt. If any hole is empty, proceed with `[ASSUMED]` defaults and list them at the top of your first answer.*
