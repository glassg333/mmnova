# CS MM

CS MM is a new Comb Scanner family built around a chorus phase layer inspired by the public Monomachine Nova chorus architecture in `monomachine_chorus.hpp`:

- base delay roughly `1..25 ms`
- modulation depth up to `5 ms`
- exponential LFO range roughly `0.1..8 Hz`
- independent left/right LFO phase spread
- interpolated delay taps

This package does not copy the large translated ChorusCore procedure. It implements the same useful chorus topology as a small allocation-free audio stage and combines it with the Comb Scanner voice bank. Source reference: https://github.com/glassg333/mmnova/blob/main/JUCE/Monomachine-Nova-1.6.7/Monomachine_Nova_FX/Source/dsp/monomachine_chorus.hpp

## Twenty algorithms

The first ten are two variants each of Harmonic, Inharmonic, Stretched, Cluster, Broken Tape, and Percussive families. The final ten add Prime, Octave, Shimmer, Rubber, Frozen, and Liquid combinations. Every algorithm has its own comb ratios, feedback envelope, diffusion, pan field, chorus base delay, chorus depth, chorus rate, chorus mix, and stereo spread.

## Parameters

Feedback, Damp, Phase, Diffusion, Cross Mix, Motion, Chorus Mix, Chorus Depth, Chorus Rate, Delay 1, Delay 2, and Scan are independent. Delay 1/2 accept `0..2000 ms`; a one-sample minimum is used internally at zero.

## Build

Use the Visual Studio 2026 preset. JUCE 8.0.6 is fetched automatically unless `JUCE_DIR` is supplied. The JUCE plugin target is `CSMM` and the standalone DSP library is `csmm_dsp`.

`cs_mm_lab.html` is a browser listening tool with a percussive two-bar loop, all twenty algorithms, and live controls. It is an immediate JavaScript port for auditioning; compiling the exact C++ DSP to browser WebAssembly remains a separate Emscripten build step.
