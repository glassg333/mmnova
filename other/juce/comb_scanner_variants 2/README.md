# Comb Scanner Variants

This package contains ten selectable JUCE/C++ comb-scanner models. The models are deliberately close variations rather than unrelated effects, so their WAV previews can be compared against the original test material.

## Models

1. Original Hypothesis: baseline ratios and the simplest adjacent scan crossfade.
2. Tight Crossfade: shorter delay ratios with more diffusion.
3. Long Resonator: longer ratios and a darker, slower resonance.
4. Scrub Stretch: curved scan and stronger delay movement.
5. Phase Cloud: dense all-pass diffusion and wide stereo motion.
6. Ping Pong: alternating comb voice positions.
7. Dark Bloom: high feedback, low brightness, and restrained stereo spread.
8. Bright Teeth: shorter diffusion and sharper transients.
9. Unstable Edge: reversed scan and the most aggressive resonance.
10. Balanced Matrix: strong inter-voice cross-mix for a blended scanner.

## Parameters

- Model: one of the ten variants.
- Gain: normalized feedback amount; `1.0` is clamped just below runaway feedback.
- Damp: damping amount.
- Phase: modulation amount.
- Delay 1: `0..2000 ms`.
- Delay 2: `0..2000 ms`.
- Scan: plugin UI range `0..2`, normalized internally to `0..1`.
- Character: `0..1`; low values are cleaner and smoother, high values add cross-mix, diffusion, delay movement, and resonance.

At zero delay the DSP uses a one-sample delay so the plugin remains stable and processable.

## Build

Open this directory with CMake on the target machine. `CMakePresets.json` contains the Visual Studio 2026 x64 preset. JUCE 8.0.6 is fetched automatically unless `JUCE_DIR` is supplied.

The standalone demo target is `combscanner_variants_demo`. It renders the ten WAVs when passed an output directory. The JavaScript renderer in `tools/render_variants.js` is also usable with Node.js when a C++ toolchain is not available.

## Preview files

The ten WAV files are 15-second comparison previews rendered from the supplied `cs example.wav` reference with a swept Scan and the same test settings: Gain `0.99`, Damp `0.90`, Phase `0.75`, Delay 1 `115 ms`, and Delay 2 `500 ms`. The audition mix uses 12% dry, 88% wet, and a fixed `4.5x` output makeup so the scanner movement is easy to hear. The reference audio is intentionally not copied into this package.

The JavaScript renderer requires a real source path and accepts a start offset and optional Character override: `node tools/render_variants.js previews path/to/source.wav 0 0.55`. It accepts 16-bit PCM WAV files at 44.1 or 48 kHz and resamples them to 48 kHz. There is no synthetic fallback.

These are model candidates, not a claim of numerical identity with the unavailable original Max/RNBO internals. The exact original Scan parameter range was not present in the supplied test material.
