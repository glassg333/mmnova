
#pragma once

// =============================================================================
// AudioFilterOBXa — Oberheim OB-Xf style multimode filter
// =============================================================================
//
// A TPT (Topology-Preserving Transform) ZDF ladder filter modelled after the
// Oberheim OB-X / OB-Xa / OB-Xf topology.  Provides 2-pole and 4-pole modes,
// 15 Xpander pole-mix modes, multimode crossfading (LP → BP → HP), and
// optional audio-rate cutoff + resonance modulation.
//
// DSP core:
//   - 1-pole TPT integrators (bilinear / trapezoidal)
//   - Diode-pair resistance approximation for nonlinear feedback (2-pole)
//   - Resonance-corrected atan() saturation on first pole (4-pole)
//   - Xpander modes via 5-coefficient pole-mix table
//
// Wiring (3 inputs, matching Teensy Audio Library convention):
//   input 0 : audio signal
//   input 1 : cutoff modulation bus   (-1..+1 normalised)
//   input 2 : resonance modulation bus (-1..+1 normalised)
//
// Safe operating ranges:
//   Cutoff    : 5 Hz – 0.24 × fs  (≈10 584 Hz at 44 100 Hz)
//   Resonance : 0.0 – 1.0
//
// Why 0.24 × fs?
//   The bilinear transform uses tan(π·fc/fs).  Above ~0.25 × fs the tangent
//   diverges rapidly, causing coefficient overflow and numerical instability.
//   Clamping to 0.24 × fs provides a safe margin while preserving the
//   musically useful range.
//
// Compile-time options (define before including this header):
//
//   OBXA_BLOCKRATE_MOD  (default 1)
//     When 1, cutoff and resonance modulation buses are sampled once per
//     128-sample block (at the midpoint, sample 64).  This replaces 128
//     powf() + tanf() calls with one each, saving ~15 000 cycles per voice
//     per block on Cortex-M7.  Perceptually identical for LFO/envelope
//     modulation.  Set to 0 for true audio-rate FM of the cutoff.
//
//   OBXA_STATE_GUARD  (default 1)
//     When 1, each output sample is checked for NaN / Inf / runaway values.
//     If detected, the filter poles are zeroed and output is muted for 2
//     blocks to allow transients to settle.  Recommended to leave enabled.
//
//   OBXA_HUGE_THRESHOLD  (default 1e6)
//     Absolute value above which a pole or output sample is considered
//     runaway.  Only used when OBXA_STATE_GUARD is enabled.
//
// =============================================================================

#include <Arduino.h>
#include <math.h>
#include "AudioStream.h"

// ---------------------------------------------------------------------------
// Compile-time configuration — override by defining before this header
// ---------------------------------------------------------------------------

// Block-rate modulation: sample mod buses once per block instead of per-sample.
// Saves ~15k cycles/voice/block.  Set to 0 for true audio-rate filter FM.
#ifndef OBXA_BLOCKRATE_MOD
#define OBXA_BLOCKRATE_MOD 1
#endif

// Auto-reset poles on NaN / Inf / runaway — prevents speaker-damaging output.
#ifndef OBXA_STATE_GUARD
#define OBXA_STATE_GUARD 1
#endif

// Absolute value threshold for runaway detection (only if STATE_GUARD is on).
#ifndef OBXA_HUGE_THRESHOLD
#define OBXA_HUGE_THRESHOLD 1.0e6f
#endif

// =============================================================================
// AudioFilterOBXa
// =============================================================================
class AudioFilterOBXa : public AudioStream
{
public:
    AudioFilterOBXa();

    // ---- Core controls (Teensy Audio Library style) -------------------------

    // Set cutoff frequency in Hz.  Clamped to 5 Hz – 0.24 × fs.
    void frequency(float hz);

    // Set resonance amount, 0.0 (none) to 1.0 (self-oscillation onset).
    void resonance(float r01);

    // Set multimode position, 0.0 (LP) through 1.0 (HP).
    // Interpolates pole outputs: LP → BP → HP in standard mode.
    // Ignored when Xpander 4-pole mode is active (use setXpanderMode instead).
    void multimode(float m01);

    // ---- Topology selection -------------------------------------------------

    // Switch between 2-pole (12 dB/oct) and 4-pole (24 dB/oct) operation.
    void setTwoPole(bool enabled);
    bool getTwoPole() const { return _useTwoPole; }

    // Enable Xpander pole-mix mode (4-pole only).  When enabled, the 15
    // Xpander modes override the normal multimode crossfade.
    void setXpander4Pole(bool enabled);
    bool getXpander4Pole() const { return _xpander4Pole; }

    // Select Xpander mode 0–14.  Only active when Xpander 4-pole is enabled.
    //   0: LP4   1: LP3   2: LP2   3: LP1   4: HP3   5: HP2   6: HP1
    //   7: BP4   8: BP2   9: N2   10: PH3  11: HP2+LP1  12: HP3+LP1
    //  13: N2+LP1  14: PH3+LP1
    void setXpanderMode(uint8_t mode);
    uint8_t getXpanderMode() const { return _xpanderMode; }

    // ---- 2-pole sub-modes ---------------------------------------------------

    // When true, multimode knob crossfades LP → BP → HP across 0..1 range.
    // When false (default), multimode crossfades LP → HP directly.
    void setBPBlend2Pole(bool enabled);
    bool getBPBlend2Pole() const { return _bpBlend2Pole; }

    // Subtle feedback push for extra growl in 2-pole mode (OB-Xf behaviour).
    void setPush2Pole(bool enabled);
    bool getPush2Pole() const { return _push2Pole; }

    // ---- Modulation scaling (audio-rate inputs 1 and 2) ---------------------

    // How many octaves of cutoff shift per +1.0 on input 1.
    // Example: setCutoffModOctaves(4.0) → full-scale LFO sweeps ±4 octaves.
    void setCutoffModOctaves(float oct);
    float getCutoffModOctaves() const { return _cutoffModOct; }

    // How much resonance changes per +1.0 on input 2 (in 0..1 units).
    // Example: setResonanceModDepth(0.5) → full-scale LFO adds ±0.5 to r01.
    void setResonanceModDepth(float depth01);
    float getResonanceModDepth() const { return _resModDepth; }

    // ---- Control-rate modulation (call from sketch, not audio graph) ---------

    // Key tracking: how much cutoff follows pitch.
    // 0.0 = no tracking, 1.0 = cutoff follows pitch 1:1.
    void setKeyTrack(float amount01);
    float getKeyTrack() const { return _keyTrack; }

    // Envelope modulation depth in octaves.  Multiplied by the current
    // envelope value to produce octave shift.
    void setEnvModOctaves(float oct);
    float getEnvModOctaves() const { return _envModOct; }

    // Current MIDI note number (0–127).  Used by key tracking to compute
    // octave offset from middle C (note 60).
    void setMidiNote(float note);
    float getMidiNote() const { return _midiNote; }

    // Current filter envelope value (0.0–1.0).  Call this at control rate
    // from your envelope generator.
    void setEnvValue(float env01);
    float getEnvValue() const { return _envValue; }

    // ---- AudioStream interface ----------------------------------------------
    virtual void update(void) override;

private:
    audio_block_t *_inQ[3]{};

    // Target parameters (set by API, applied in update)
    float _cutoffHzTarget = 1000.0f;
    float _res01Target    = 0.0f;
    float _multimode01    = 0.0f;

    // Topology flags
    bool    _useTwoPole   = false;
    bool    _xpander4Pole = false;
    uint8_t _xpanderMode  = 0;
    bool    _bpBlend2Pole = false;
    bool    _push2Pole    = false;

    // Modulation scaling
    float _cutoffModOct = 0.0f;
    float _resModDepth  = 0.0f;

    // Control-rate modulation state
    float _keyTrack  = 0.0f;
    float _envModOct = 0.0f;
    float _midiNote  = 60.0f;
    float _envValue  = 0.0f;

    // State guard: mute output for N blocks after a pole reset
    uint16_t _cooldownBlocks = 0;

    // Filter core — forward-declared, defined in .cpp to keep header clean
    struct Core;
    Core *_core = nullptr;
};
