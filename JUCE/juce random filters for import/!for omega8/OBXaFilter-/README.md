# AudioFilterOBXa — Oberheim OB-Xf Style Multimode Filter

A TPT (Topology-Preserving Transform) zero-delay-feedback ladder filter modelled
after the Oberheim OB-X / OB-Xa / OB-Xf topology, implemented as a Teensy Audio
Library object.

## Features

- **2-pole** (12 dB/oct) and **4-pole** (24 dB/oct) operation
- **15 Xpander pole-mix modes** (LP, HP, BP, notch, phaser, and combinations)
- **Multimode crossfading** — continuous sweep from lowpass through bandpass to highpass
- **Audio-rate modulation** of cutoff and resonance via standard AudioConnection wiring
- **Control-rate** key tracking and envelope modulation (call from sketch)
- **State guard** — auto-resets on NaN/Inf to prevent speaker-damaging output
- ~0.5% CPU per voice at 44.1 kHz on Teensy 4.x (block-rate mod enabled)


## Installation

Copy these two files into your Arduino libraries folder or project directory:

```
AudioFilterOBXa.h
AudioFilterOBXa.cpp
```

No external dependencies beyond the standard Teensy Audio Library.


## Quick Start

No hardware required — just Teensy 4.x and a USB cable.

1. Set **Tools → USB Type** to `Serial + MIDI + Audio`
2. Upload the example sketch
3. Open any DAW or virtual MIDI keyboard
4. Play notes on MIDI channel 1, twist CCs 1–17

```cpp
#include <Audio.h>
#include "AudioFilterOBXa.h"

AudioSynthWaveform   osc;
AudioFilterOBXa      filter;
AudioOutputUSB       usbOut;

AudioConnection c0(osc,    0, filter, 0);  // audio → filter
AudioConnection c1(filter, 0, usbOut, 0);  // filter → USB left
AudioConnection c2(filter, 0, usbOut, 1);  // filter → USB right

void setup() {
    AudioMemory(12);
    osc.begin(0.5, 220.0, WAVEFORM_SAWTOOTH);
    filter.frequency(2000.0);   // cutoff in Hz
    filter.resonance(0.3);      // 0.0 – 1.0
}

void loop() {
    usbMIDI.read();  // process incoming MIDI
}
```


## Wiring (AudioConnection inputs)

| Input | Signal              | Range       | Purpose                         |
|-------|---------------------|-------------|----------------------------------|
| 0     | Audio signal        | -1.0..+1.0  | The signal to be filtered        |
| 1     | Cutoff mod bus      | -1.0..+1.0  | LFO/envelope → cutoff (optional) |
| 2     | Resonance mod bus   | -1.0..+1.0  | LFO → resonance (optional)       |

| Output | Signal             |
|--------|--------------------|
| 0      | Filtered audio     |


## API Reference

### Core Controls

```cpp
void frequency(float hz);       // Set cutoff (clamped to 5 Hz – 0.24×fs)
void resonance(float r01);      // Set resonance 0.0–1.0
void multimode(float m01);      // Set multimode position 0.0–1.0
```

### Topology Selection

```cpp
void setTwoPole(bool enabled);          // true = 12 dB/oct, false = 24 dB/oct
void setXpander4Pole(bool enabled);     // Enable Xpander mode (4-pole only)
void setXpanderMode(uint8_t mode);      // Select Xpander mode 0–14
void setBPBlend2Pole(bool enabled);     // Enable LP→BP→HP blend (2-pole)
void setPush2Pole(bool enabled);        // Enable OB-Xf feedback push (2-pole)
```

### Xpander Mode Table (4-pole only)

| Mode | Type      | Response             |
|------|-----------|----------------------|
| 0    | LP4       | 24 dB lowpass        |
| 1    | LP3       | 18 dB lowpass        |
| 2    | LP2       | 12 dB lowpass        |
| 3    | LP1       | 6 dB lowpass         |
| 4    | HP3       | 18 dB highpass       |
| 5    | HP2       | 12 dB highpass       |
| 6    | HP1       | 6 dB highpass        |
| 7    | BP4       | 24 dB bandpass       |
| 8    | BP2       | 12 dB bandpass       |
| 9    | N2        | 12 dB notch          |
| 10   | PH3       | 18 dB phaser         |
| 11   | HP2+LP1   | Highpass + lowpass    |
| 12   | HP3+LP1   | Highpass + lowpass    |
| 13   | N2+LP1    | Notch + lowpass      |
| 14   | PH3+LP1   | Phaser + lowpass     |

### Modulation Scaling (audio-rate via AudioConnection)

```cpp
void setCutoffModOctaves(float oct);    // Octaves per +1.0 on input 1
void setResonanceModDepth(float d01);   // Resonance units per +1.0 on input 2
```

### Control-Rate Modulation (call from sketch)

```cpp
void setKeyTrack(float amount01);       // 0.0 = off, 1.0 = 1:1 pitch tracking
void setEnvModOctaves(float oct);       // Envelope depth in octaves
void setMidiNote(float note);           // Current MIDI note (0–127)
void setEnvValue(float env01);          // Current envelope value (0.0–1.0)
```


## Compile-Time Options

Define these before `#include "AudioFilterOBXa.h"`:

```cpp
#define OBXA_BLOCKRATE_MOD 1    // (default) Block-rate mod — saves ~15k cycles/voice
#define OBXA_BLOCKRATE_MOD 0    // Per-sample mod — true audio-rate filter FM

#define OBXA_STATE_GUARD 1      // (default) Auto-reset on NaN/runaway
#define OBXA_STATE_GUARD 0      // Disable for benchmarking (not recommended)

#define OBXA_HUGE_THRESHOLD 1e6 // (default) Runaway detection threshold
```


## Modulation Wiring Example (LFO → cutoff)

```cpp
AudioSynthWaveform       osc;
AudioSynthWaveform       lfo;
AudioFilterOBXa          filter;
AudioOutputUSB           usbOut;

AudioConnection c0(osc,    0, filter, 0);   // audio → filter
AudioConnection c1(lfo,    0, filter, 1);   // LFO → cutoff mod bus
AudioConnection c2(filter, 0, usbOut, 0);

void setup() {
    AudioMemory(12);

    osc.begin(0.5, 220.0, WAVEFORM_SAWTOOTH);
    lfo.begin(1.0, 2.0, WAVEFORM_SINE);   // 2 Hz, full scale

    filter.frequency(800.0);
    filter.resonance(0.6);
    filter.setCutoffModOctaves(3.0);  // ±3 octaves of sweep
}
```


## Example Sketch CC Map

The included `OBXaFilter.ino` exposes every parameter via USB MIDI (channel 1):

| CC  | Parameter              | Range                                     |
|-----|------------------------|--------------------------------------------|
| 1   | Cutoff frequency       | 20 Hz – 10 000 Hz (exponential)            |
| 2   | Resonance              | 0.0 – 1.0                                  |
| 3   | Multimode              | 0 = LP, 64 = BP, 127 = HP                  |
| 4   | 2-pole / 4-pole        | 0 = 4-pole, ≥64 = 2-pole                   |
| 5   | Xpander enable         | 0 = off, ≥64 = on                          |
| 6   | Xpander mode           | 0–14 (mapped from 0–127)                   |
| 7   | Output volume          | 0 = silent, 127 = full                     |
| 8   | BP blend (2-pole)      | 0 = off, ≥64 = on                          |
| 9   | Push (2-pole)          | 0 = off, ≥64 = on                          |
| 10  | Key tracking           | 0.0 – 1.0                                  |
| 11  | Env mod depth          | 0 – 8 octaves                               |
| 12  | Cutoff LFO depth       | 0 – 8 octaves                               |
| 13  | Resonance LFO depth    | 0.0 – 1.0                                  |
| 14  | Cutoff LFO rate        | 0.1 – 20 Hz (exponential)                  |
| 15  | Cutoff LFO waveform    | sine / tri / saw / square                  |
| 16  | Resonance LFO rate     | 0.1 – 20 Hz (exponential)                  |
| 17  | Resonance LFO waveform | sine / tri / saw / square                  |


## Adding to the Audio Design Tool

The Audio Design Tool uses a JavaScript object definition for each audio object.
Add the following to the `AudioFilterOBXa` entry in the design tool's data file.

### GUI Definition (for Audio System Design Tool `index.html`)

Add this to the `AudioFilterOBXa` section in the nodes array:

```javascript
{
  type: "AudioFilterOBXa",
  data: {
    category: "filter",
    color: "#E6E0F8",
    icon: "filter_vintage",
    shortName: "obxa",
    inputs: 3,
    inputLabels: ["audio in", "cutoff mod", "resonance mod"],
    outputs: 1,
    outputLabels: ["filtered"],
    description: "OB-Xa/OB-Xf style multimode filter. 2-pole (12dB) or 4-pole (24dB) with 15 Xpander modes.",
    params: [
      { name: "frequency",          type: "float",  default: 1000.0, min: 5.0, max: 10584.0,
        description: "Cutoff frequency in Hz" },
      { name: "resonance",          type: "float",  default: 0.0,    min: 0.0, max: 1.0,
        description: "Resonance amount (0=none, 1=self-oscillation)" },
      { name: "multimode",          type: "float",  default: 0.0,    min: 0.0, max: 1.0,
        description: "Mode position (0=LP, 0.5=BP, 1=HP)" },
      { name: "setTwoPole",         type: "bool",   default: false,
        description: "true=12dB/oct, false=24dB/oct" },
      { name: "setXpander4Pole",    type: "bool",   default: false,
        description: "Enable Xpander pole-mix modes" },
      { name: "setXpanderMode",     type: "int",    default: 0, min: 0, max: 14,
        description: "Xpander mode selector (0-14)" },
      { name: "setCutoffModOctaves", type: "float", default: 0.0, min: 0.0, max: 8.0,
        description: "Cutoff mod depth in octaves" },
      { name: "setResonanceModDepth", type: "float", default: 0.0, min: 0.0, max: 1.0,
        description: "Resonance mod depth" },
      { name: "setKeyTrack",        type: "float",  default: 0.0, min: 0.0, max: 1.0,
        description: "Key tracking amount" },
      { name: "setEnvModOctaves",   type: "float",  default: 0.0, min: 0.0, max: 8.0,
        description: "Envelope mod depth in octaves" }
    ]
  }
}
```

### `keywords.txt` (for Arduino IDE autocompletion)

```
AudioFilterOBXa	KEYWORD1
frequency	KEYWORD2
resonance	KEYWORD2
multimode	KEYWORD2
setTwoPole	KEYWORD2
setXpander4Pole	KEYWORD2
setXpanderMode	KEYWORD2
setBPBlend2Pole	KEYWORD2
setPush2Pole	KEYWORD2
setCutoffModOctaves	KEYWORD2
setResonanceModDepth	KEYWORD2
setKeyTrack	KEYWORD2
setEnvModOctaves	KEYWORD2
setMidiNote	KEYWORD2
setEnvValue	KEYWORD2
```


## Performance

Measured on Teensy 4.1 at 44 100 Hz, single instance:

| Configuration              | CPU per block |
|----------------------------|---------------|
| 4-pole LP, block-rate mod  | ~0.5%         |
| 4-pole LP, per-sample mod  | ~1.8%         |
| 2-pole LP, block-rate mod  | ~0.3%         |
| Xpander mode, block-rate   | ~0.6%         |

Block-rate modulation (`OBXA_BLOCKRATE_MOD=1`) saves approximately 15 000 cycles
per voice per 128-sample block by:
- Replacing 128 `tanf()` calls with 1 (precomputed g/lpc)
- Replacing 128 `powf()` calls with 1 `obxa_fast_pow2()` call


## Design Notes

**Why 0.24 × fs cutoff limit?**
The bilinear transform computes `g = tan(π·fc/fs)`.  Above ~0.25 × fs, the
tangent diverges to infinity, causing coefficient overflow and filter instability.
Clamping to 0.24 × fs (≈10 584 Hz at 44 100 Hz) provides a safe margin.

**Why atan() saturation on the first pole only?**
The Oberheim topology places the nonlinear element at the input of the ladder.
This models the diode/transistor saturation that gives the OB-X its characteristic
warmth under resonance.  Saturating all four poles would over-darken the sound
and waste CPU.

**Block-rate vs per-sample modulation:**
For LFO and envelope modulation (typical synth use), block-rate is perceptually
identical — the 344 Hz update rate is well above the modulation frequencies
involved.  Per-sample mode is provided for audio-rate FM experiments.


## File Listing

```
AudioFilterOBXa.h                     - Header (public API)
AudioFilterOBXa.cpp                   - Implementation
examples/OBXaFilter/OBXaFilter.ino    - Minimal example sketch
README.md                             - This file
```


## License

MIT License — see file headers for full text.
