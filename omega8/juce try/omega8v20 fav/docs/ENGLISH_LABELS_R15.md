# English control names — r15

The faceplate uses the original Omega/CODE section order and common short labels. Hovering a control shows its full English meaning. All visible control strings in the editor source are plain ASCII; factory patch names are sanitized to printable ASCII before display.

## Faceplate sections

| UI label | Full meaning |
| --- | --- |
| MULTI / MIDI | Multi mode and MIDI-related controls |
| MODULATION | LFOs and modulation routing |
| PROGRAMMER | Patch selection, bank navigation, compare/save/global functions |
| OSCILLATORS | Oscillator pitch, waveform, pulse width, level and noise |
| FILTER | Cutoff, resonance, tracking, filter type and AUX1 card selection |
| ENVELOPES | ENV1 filter contour, ENV2 amplifier contour, ENV3 assignable contour |
| VOLUME | Master output level |
| GLIDE | Portamento time |
| OCT: MID | Neutral octave; `+1` / `-1` show octave offsets |
| TUNE | Patch Tune field. The original editor shows raw value 64 as 100%. This build stores/displays the value; the exact random-variation depth is not yet modeled in DSP. |

## Oscillator and filter abbreviations

- `OSC1`, `OSC2`: oscillator 1 and oscillator 2.
- `FREQ`: oscillator pitch/tuning. r15 restores the r10 centered mapping: this engine treats byte 32 as neutral; values above/below it transpose upward/downward. The REVELATION editor screenshot has 24 and 36, a 12-semitone difference. The absolute MIDI-note mapping still needs a tuner A/B check on a neutral patch.
- `FINE`: oscillator 2 fine tuning.
- `TRI`, `SAW`, `PULSE`: triangle, sawtooth and pulse waveforms.
- `PWM`: pulse width.
- `LEV`: oscillator mix level; `NOISE`: white-noise level; `SUB`: oscillator 1 sub-waveform, values 0–6.
- `CUTOFF`: filter cutoff/centre frequency; `RESO`: resonance/Q; `TRACKING`: keyboard tracking.
- `HPF`, `HPR`: AUX2/CS-80 high-pass frequency and resonance controls.
- Filter types: `SEM LP` (SEM low-pass), `SEM BP` (band-pass), `SEM HP` (high-pass), `SEM BR` (band-reject/notch), `MINI LP` (Minimoog-style 24 dB low-pass), `AUX1`, `AUX2`.

## Envelope names

- `A`: Attack time; `D`: Decay time; `S`: Sustain level; `R`: Release time.
- `DKY2`: Decay 2, an extra held-note decay stage (shown in GLOBAL so the main panel can match the original four-knob ADSR layout).
- `ENV1`: filter contour / VCF. `ENV2`: amplifier contour / VCA. `ENV3`: assignable contour.
- `DLY1–3`: delay before the matching envelope starts. `DYN1–3`: dynamics/velocity sensitivity for that envelope.
- `ENV1 AMT`: ENV1 depth to filter frequency. `ENV3 AMT`: ENV3 amount for destination 1.

## Modulation and controller abbreviations

- `LFO1`, `LFO2`: low-frequency oscillators; `RATE`: speed; `D1`, `D2`, `D3`: amount for destination 1, 2 or 3.
- `XMOD`: oscillator 2 audio-rate cross-modulation depth.
- Destination codes: `FRE1/FRE2` = oscillator frequency; `1&2F` = both oscillator frequencies; `LEV1/LEV2` = oscillator levels; `PW1/PW2` = pulse width; `1&2P` = both pulse widths; `FILT` = filter frequency; `RESO` = resonance; `LEVN` = noise level; `EA1/EA3` = ENV1/ENV3 amount; `EXT` = external input; `LF1R/LF2R` = LFO rate; `LF1D/LF2D` = LFO depth; `1&2D` = both LFO depths; `1&2R` = both LFO rates; `PAND/PANR/PAN` = pan depth/rate/position.
- `MW`: mod wheel; `DY`: dynamics/velocity; `BN`: pitch bend; `PR`: pressure/aftertouch; `C1/C2`: continuous controller 1/2; `A1/A2`: amount for destination slot 1/2.
- `P1–P4 POS/RATE/DPTH`: pan position/rate/depth for each of four pan lanes.

## AUX card scope (important)

The three choices `OB SVF`, `OB-X`, and `TB-303` are the **three filter DSP models currently implemented in this VST for AUX1**; they are not a claim that the original instrument had only three cards. The CODE/Omega manual describes the built-in SEM/MINI modes plus AUX slots and names 303, 2600, Juno/Jupiter and CS-80 variants; it says CS-80 is in AUX2. Exact installed cards depend on the hardware configuration. The present OB-SVF, OB-X and TB-303 entries are explicitly marked as approximations. No unimplemented card model is silently presented as original.

## Clickability and panel geometry

- The seven front-panel filter rows are active selection controls. They update the patch byte and the host-automatable `Filter Type` parameter.
- The GLOBAL dropdown is an alternate selector for the same value.
- GLOBAL-only controls are hidden when the lower extension is closed; reopening it restores the controller matrix, additional envelope stages, pan lanes and other decoded controls.
- The panel follows the section proportions/order of the supplied OmegaCODEeditor screenshot. Controls start below the section-title strip, knob diameters are limited by each row height, and labels no longer spill into the next row.
