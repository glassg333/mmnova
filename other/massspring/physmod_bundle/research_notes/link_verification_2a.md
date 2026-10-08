# Link Verification — Task 2-a (6 GitHub repos + 1 HAL paper)

Verified 2026-10-07. GitHub metadata via api.github.com (search endpoint, unauthenticated — the
core REST endpoint was rate-limited for this IP; search returned identical repo objects).
READMEs via raw.githubusercontent.com. HAL metadata via api.archives-ouvertes.fr (the hal.science
site itself is behind the "Anubis" anti-bot JS challenge and blocks curl).

## Repos

### 1. mi-creative/miPhysics_Processing — RELEVANCE: HIGH
- URL: https://github.com/mi-creative/miPhysics_Processing
- Lang: Java (Processing library). License: GPL-3.0. Stars 52, forks 7.
- Created 2018-12-20, last push 2020-05-05. Not archived. Branch `master`. Topics: haptics, physics-simulation-library, processing, sound-synthesis. Homepage http://mi-creative.eu
- Purpose: mass-interaction physical modeling library (ACROE lineage) for visuals + audio + haptics in Processing. Authors: James Leonard, Jérôme Villeneuve.
- API style: `PhysicalModel` container; `addMass(...)` (Mass3D, Ground3D, ...), `addInteraction(...)` (SpringDamper3D ...), `addInOut(...)` (Driver3D); `mdl.compute()` per tick; model can run in audio thread (Minim backend for audio examples); haptics via Haply.
- Examples split: 0_Basics / 1_Visuals (dynamic topology changes, grouped param modification) / 2_Audio / 3_Haptics.
- Build: IntelliJ + Ant (Processing library template).
- Borrow: the element taxonomy (mass/ground/spring-damper/driver/plucked-string/contact), `compute()` audio-callback pattern, dynamic topology changes concept. GPL-3.0 → do NOT copy code into a closed/other-licensed plugin; concepts only.
- README saved: research/readmes/miPhysics_Processing.md (6119 B)

### 2. SilvinWillemsen/ModularVST — RELEVANCE: HIGH (closest to our target project)
- URL: https://github.com/SilvinWillemsen/ModularVST
- Lang: C++ (JUCE). License: MIT. Stars 10, forks 1.
- Created 2021-08-31, last push 2022-07-14. Not archived. Branch `main`; app source on branch `SMCconf` (verified: raw Source/PluginProcessor.cpp 200 OK, ~44 KB). Releases: vSMC(mac), vSMC(win).
- Purpose: modular mass-interaction physical modeling VST; accompanies SMC-2021 paper "Modular Physical Modeling in a Real-Time Interactive Application" (README is just a pointer; 355 B).
- Borrow: JUCE plugin architecture, module-routing UI approach, MIT license = code can be reused with attribution.
- README saved: research/readmes/ModularVST.md (355 B)

### 3. pooya-shams/massspring — RELEVANCE: LOW-MEDIUM
- URL: https://github.com/pooya-shams/massspring
- Lang: Python (pygame). License: MIT. Stars 19, forks 1.
- Created 2020-02-04, last push 2020-09-04. Not archived. Branch `master`. Topics: mass-spring, pygame, physics-3d. Homepage: https://pooya-shams.github.io/massspring
- Top-level README.md is a symlink (20 B → "massspring/README.md"); real README is inside the package (saved: research/readmes/massspring_package_README.md, 12668 B).
- pip-installable package; mass objects (position/velocity/radius/charge, solid/moveable/bound flags) + force classes: spring (Hooke), gravity, electricity (Coulomb), collision (elastic, momentum+KE conservation), air_resistance (drag); mainloop; v1.2 adds networklib (server/client position streaming).
- Integrator not documented; fixed-step velocity/force accumulation pygame loop. Render: 2D/3D projections in pygame.
- Borrow: force-class design (spring/collision/air drag as separate force objects), elastic-collision handling idea. Python → reference only; rewrite in C++.

### 4. davrempe/2d-mass-spring-sim — RELEVANCE: MEDIUM (direct 2D draw-to-spawn reference)
- URL: https://github.com/davrempe/2d-mass-spring-sim
- Lang: C (OpenGL + GLUT). License: none. Stars 4, forks 1.
- Created 2016-06-20, last push 2016-09-02. Not archived. Branch `master`. Core: SpringMassSim/SpringMassSim/SpringMassSim.cpp. Build: Visual Studio solution (NuGet nupengl) or GLUT+GLEW.
- Interactive: left-click-hold = dynamic mass (hold longer = bigger), right-click-hold = fixed mass, click-drag between masses = spring (DEFAULT_KS/KD/R), arrows start/pause, drag masses while running.
- Integrator: forward Euler (stated). Collision: simple elastic boundary-touch handling (admits "wonky" multi-collision behavior). Inspired by Baraff & Witkin SIGGRAPH'99 phys_model course notes.
- Borrow: the draw-to-spawn interaction grammar (exactly our "draw shapes → spawn resonators"), mass-by-hold-duration affordance, drag-to-link springs. No license → treat as ideas-only, no code reuse.
- README saved: research/readmes/2d-mass-spring-sim.md (2251 B)

### 5. arasgungore/mass-spring-damper-system — RELEVANCE: LOW
- URL: https://github.com/arasgungore/mass-spring-damper-system
- Lang: MATLAB / Simulink. License: MIT. Stars 16, forks 3.
- Created 2022-04-18, last push 2022-08-08. Not archived. Branch `main`. ~19 topics.
- Single & double mass-spring-damper Simulink models; plots position-time; run headless via `matlab -nodisplay ... param.m`. No audio, no UI interactivity.
- Borrow: nothing beyond textbook reference equations (small educational value; useful only as a sanity-check example of MSD math).
- README saved: research/readmes/mass-spring-damper-system.md (1120 B)

### 6. lucasw/tao_synth — RELEVANCE: HIGH (conceptually closest classic)
- URL: https://github.com/lucasw/tao_synth — FORK of MindBuffer/tao (confirmed from repo HTML).
- Lang: C++. License: GPL-2.0. Stars 11, forks 2.
- Created 2018-08-20 (fork), last push 2020-06-13. Not archived. Branch `master`. Topics: music, sound, sound-synthesis. Upstream project: taopm (http://taopm.sourceforge.net, Mark Pearson).
- Tao: virtual acoustic material = point masses + springs; instruments built from it; "devices": Bows, Hammers, Connectors, Outputs for exciting/coupling/instrument output; real-time 3D OpenGL visualisation of wave propagation; usable standalone or as C++ library.
- Requires X11/OpenGL/GLUT + audiofile (SGI port) for WAV output; CMake build; example `examples/strand -g`.
- Borrow: bow/hammer/connect devices (our bow excitation + collisions), mass-spring material → instrument mapping, C++ library embedding idea. GPL-2.0 → no code copy into our plugin unless we accept GPL; concepts only.
- README saved: research/readmes/tao_synth.md (5011 B)

## HAL paper (link 7) — METADATA OK, PDF BLOCKED FOR CURL
- hal-01262144 v1: "Visual Representation in GENESIS as a tool for Physical Modeling, Sound Synthesis and Musical Composition"
- Authors: Jerome Villeneuve, Claude Cadoz, Nicolas Castagné. Date: 2015-05-31 (NIME 2015 context; filename VCC15_Conf_NIME.pdf). No DOI registered on HAL record.
- URI: https://hal.science/hal-01262144v1 ; official file URL per HAL API: https://hal.science/hal-01262144/file/VCC15_Conf_NIME.pdf
- Download result: `file` says HTML, 10404 bytes — hal.science (and hal.archives-ouvertes.fr, and UGA mirror) serve an "Anubis" anti-bot JS proof-of-work page to non-browser clients (HTTP 200 text/html). Browser access works; curl/wget cannot. Fallbacks tried: http:// variant, old domain, UGA mirror, browser UA, archive.org (connection timeout — blocked in sandbox), nime.org (unreachable), CORE (403), Semantic Scholar (429 rate limit).
- Abstract (via HAL API): importance of visual representations for artists modeling mass-interaction physical networks; survey of GENESIS's static/dynamic visualization tools; case studies of artists using GENESIS from musical idea to finished piece.
- Artifacts: papers/hal_api_meta.json, papers/hal_api_meta2.json, papers/2015_GENESIS_VCC15_NIME_download_blocked_anubis.html (the challenge page, kept as evidence). pdftotext not possible (no PDF). Recommend manual browser download of the PDF by the user.

## Sibling repos of interest
- mi-creative org (5 repos): mi-gen (149★, Max/gen~ mass-interaction toolbox, pushed 2024-10 — ACTIVE), MIMS (33★, PyQt model editor compiling to Faust/gen~), FaustPM_2021_examples (31★), miPhysics_Processing, website.
- SilvinWillemsen (81 repos; relevant subset): BowedStringJUCE (11★, C++/JUCE), FastBowedString (5★, "super fast implementation of the bowed stiff string using modal synthesis", pushed 2022-07), NonlinearMassSpring_DAFx23 (3★, paper "Nonlinear Strings Based on Masses and Springs" DAFx-2023 — directly relevant to string-string collision/inharmonicity), SimpleStringApp (2★, simplest JUCE stiff string), RealTimeFDTD (2★, JUCE FDTD lecture code), Dynamic_Grid_JAES (1★, time-varying FDS params), ModularPhysModVR (1★, C#), FDSSolver, BowedStringModel (MATLAB), PMS (MATLAB class material).
