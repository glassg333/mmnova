// =============================================================================
// OBXaFilter.ino — Showcase / Testbed for AudioFilterOBXa
// =============================================================================
//
// Zero-hardware demo: USB MIDI in + USB Audio out.
// Plug Teensy into any computer, open a DAW or virtual keyboard, and play.
//
// Features:
//   - Monophonic sawtooth oscillator responding to USB MIDI notes
//   - AudioFilterOBXa with every parameter exposed via MIDI CC
//   - Two built-in LFOs routed to cutoff and resonance mod inputs
//   - LFO rate, depth, and waveform controllable via CC
//   - USB Audio output (stereo) — no audio shield required
//   - CPU usage printed to Serial every second
//
// USB Type: set to "Serial + MIDI + Audio" in Arduino IDE → Tools → USB Type
//
// =============================================================================
//
// MIDI CC Map (channel 1)
// -----------------------
//   CC 1   Cutoff frequency          0 = 20 Hz, 127 = 10 000 Hz (exponential)
//   CC 2   Resonance                 0 = 0.0, 127 = 1.0
//   CC 3   Multimode                 0 = LP, 64 = BP, 127 = HP
//   CC 4   Filter mode               0 = 4-pole, 127 = 2-pole
//   CC 5   Xpander enable            0 = off, 127 = on (4-pole only)
//   CC 6   Xpander mode select       0–14 mapped from 0–127
//   CC 7   Output volume             0 = silent, 127 = full
//   CC 8   BP blend (2-pole)         0 = off, 127 = on
//   CC 9   Push (2-pole)             0 = off, 127 = on
//   CC 10  Key tracking              0 = off, 127 = full
//   CC 11  Env mod depth (octaves)   0 = 0, 127 = 8 octaves
//   CC 12  Cutoff mod depth          0 = 0 oct, 127 = 8 oct
//   CC 13  Resonance mod depth       0 = 0.0, 127 = 1.0
//   CC 14  Cutoff LFO rate           0 = 0.1 Hz, 127 = 20 Hz (exponential)
//   CC 15  Cutoff LFO waveform       0 = sine, 43 = tri, 86 = saw, 127 = square
//   CC 16  Resonance LFO rate        0 = 0.1 Hz, 127 = 20 Hz (exponential)
//   CC 17  Resonance LFO waveform    0 = sine, 43 = tri, 86 = saw, 127 = square
//
// =============================================================================

#include <Audio.h>
#include "AudioFilterOBXa.h"

// =============================================================================
// Audio objects
// =============================================================================
AudioSynthWaveform       osc;          // main oscillator
AudioSynthWaveform       lfoCutoff;    // LFO → filter cutoff mod bus
AudioSynthWaveform       lfoRes;       // LFO → filter resonance mod bus
AudioFilterOBXa          filter;       // the filter under test
AudioAmplifier           volume;       // output level control
AudioOutputUSB           usbOut;       // USB audio output (stereo)

// =============================================================================
// Audio connections
// =============================================================================
AudioConnection c0(osc,       0, filter,  0);   // oscillator → filter audio in
AudioConnection c1(lfoCutoff, 0, filter,  1);   // LFO → filter cutoff mod bus
AudioConnection c2(lfoRes,    0, filter,  2);   // LFO → filter resonance mod bus
AudioConnection c3(filter,    0, volume,  0);   // filter → volume control
AudioConnection c4(volume,    0, usbOut,  0);   // volume → USB left
AudioConnection c5(volume,    0, usbOut,  1);   // volume → USB right

// =============================================================================
// State
// =============================================================================
static int8_t currentNote = -1;    // active MIDI note (-1 = none)
static float  envValue    = 0.0f;  // simple envelope value (0.0–1.0)
static bool   envGateOn   = false; // true while note is held
static float  lfoCutFreq  = 2.0f;  // current cutoff LFO rate in Hz
static float  lfoResFreq  = 0.5f;  // current resonance LFO rate in Hz

// =============================================================================
// CC → parameter helpers
// =============================================================================

// CC (0–127) → normalised float (0.0–1.0)
static inline float ccToNorm(uint8_t val)
{
    return (float)val * (1.0f / 127.0f);
}

// CC → exponential Hz range:  low × (high/low) ^ (cc/127)
static inline float ccToExpHz(uint8_t val, float low, float high)
{
    return low * powf(high / low, ccToNorm(val));
}

// CC → boolean toggle (>= 64 = on)
static inline bool ccToBool(uint8_t val)
{
    return val >= 64;
}

// CC → waveform selector (4 equal zones)
static inline int16_t ccToWaveform(uint8_t val)
{
    if (val < 32)  return WAVEFORM_SINE;
    if (val < 64)  return WAVEFORM_TRIANGLE;
    if (val < 96)  return WAVEFORM_SAWTOOTH;
    return WAVEFORM_SQUARE;
}

// =============================================================================
// MIDI handlers
// =============================================================================
static void handleNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    if (channel != 1) return;
    if (velocity == 0) { handleNoteOff(channel, note, 0); return; }

    currentNote = note;

    // MIDI note → frequency
    float freq = 440.0f * powf(2.0f, ((float)note - 69.0f) / 12.0f);
    osc.frequency(freq);
    osc.amplitude(0.7f);

    // Tell the filter which note we're playing (for key tracking)
    filter.setMidiNote((float)note);

    envGateOn = true;
}

static void handleNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
    (void)velocity;
    if (channel != 1) return;
    if (note != currentNote) return;

    osc.amplitude(0.0f);
    currentNote = -1;
    envGateOn   = false;
}

static void handleControlChange(uint8_t channel, uint8_t cc, uint8_t val)
{
    if (channel != 1) return;

    switch (cc)
    {
    // ---- Filter core --------------------------------------------------------
    case 1:  filter.frequency(ccToExpHz(val, 20.0f, 10000.0f));     break;
    case 2:  filter.resonance(ccToNorm(val));                       break;
    case 3:  filter.multimode(ccToNorm(val));                       break;

    // ---- Topology -----------------------------------------------------------
    case 4:  filter.setTwoPole(ccToBool(val));                      break;
    case 5:  filter.setXpander4Pole(ccToBool(val));                 break;
    case 6:  filter.setXpanderMode((uint8_t)(val * 14 / 127));     break;

    // ---- Output -------------------------------------------------------------
    case 7:  volume.gain(ccToNorm(val));                            break;

    // ---- 2-pole options -----------------------------------------------------
    case 8:  filter.setBPBlend2Pole(ccToBool(val));                 break;
    case 9:  filter.setPush2Pole(ccToBool(val));                    break;

    // ---- Modulation routing -------------------------------------------------
    case 10: filter.setKeyTrack(ccToNorm(val));                     break;
    case 11: filter.setEnvModOctaves(ccToNorm(val) * 8.0f);        break;
    case 12: filter.setCutoffModOctaves(ccToNorm(val) * 8.0f);     break;
    case 13: filter.setResonanceModDepth(ccToNorm(val));            break;

    // ---- Cutoff LFO ---------------------------------------------------------
    case 14: lfoCutFreq = ccToExpHz(val, 0.1f, 20.0f);
             lfoCutoff.frequency(lfoCutFreq);                      break;
    case 15: lfoCutoff.begin(1.0f, lfoCutFreq, ccToWaveform(val));  break;

    // ---- Resonance LFO ------------------------------------------------------
    case 16: lfoResFreq = ccToExpHz(val, 0.1f, 20.0f);
             lfoRes.frequency(lfoResFreq);                          break;
    case 17: lfoRes.begin(1.0f, lfoResFreq, ccToWaveform(val));    break;

    default: break;
    }
}

// =============================================================================
// setup
// =============================================================================
void setup()
{
    Serial.begin(115200);
    AudioMemory(20);

    // ---- USB MIDI callbacks -------------------------------------------------
    usbMIDI.setHandleNoteOn(handleNoteOn);
    usbMIDI.setHandleNoteOff(handleNoteOff);
    usbMIDI.setHandleControlChange(handleControlChange);

    // ---- Oscillator: sawtooth, silent until first noteOn --------------------
    osc.begin(0.0f, 440.0f, WAVEFORM_SAWTOOTH);

    // ---- Filter: warm defaults — mid cutoff, gentle resonance, 4-pole LP ----
    filter.frequency(2000.0f);
    filter.resonance(0.3f);
    filter.multimode(0.0f);            // full lowpass
    filter.setCutoffModOctaves(0.0f);  // LFO has no effect until CC 12 > 0
    filter.setResonanceModDepth(0.0f); // LFO has no effect until CC 13 > 0

    // ---- LFOs: always running at full amplitude -----------------------------
    // Mod depth is controlled by the filter (setCutoffModOctaves /
    // setResonanceModDepth), so the LFOs output full-scale ±1.0 and the
    // filter scales the result internally.
    lfoCutoff.begin(1.0f, 2.0f, WAVEFORM_SINE);   // 2 Hz sine
    lfoRes.begin(1.0f, 0.5f, WAVEFORM_SINE);      // 0.5 Hz sine

    // ---- Output volume ------------------------------------------------------
    volume.gain(0.7f);

    Serial.println("=== AudioFilterOBXa Testbed ===");
    Serial.println("USB Type: Serial + MIDI + Audio");
    Serial.println("MIDI ch 1 — notes to play, CCs 1-17 for parameters");
    Serial.println("See sketch header for full CC map");
}

// =============================================================================
// loop
// =============================================================================
void loop()
{
    // ---- Read all pending USB MIDI messages ----------------------------------
    usbMIDI.read();

    // ---- Simple envelope: fast attack (~10 ms), slow release (~200 ms) ------
    // Gives the envelope mod parameter (CC 11) something to work with.
    // A real synth would use AudioEffectEnvelope; this is just for testing.
    if (envGateOn)
    {
        if (envValue < 1.0f) envValue += 0.05f;
        if (envValue > 1.0f) envValue = 1.0f;
    }
    else
    {
        if (envValue > 0.0f) envValue -= 0.005f;
        if (envValue < 0.0f) envValue = 0.0f;
    }
    filter.setEnvValue(envValue);

    // ---- CPU / memory usage report (once per second) ------------------------
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 1000)
    {
        lastPrint = millis();
        Serial.printf("CPU: %.1f%%  max: %.1f%%  mem: %d/%d",
                      AudioProcessorUsage(),
                      AudioProcessorUsageMax(),
                      AudioMemoryUsage(),
                      AudioMemoryUsageMax());
        if (currentNote >= 0)
            Serial.printf("  note: %d", currentNote);
        Serial.println();

        AudioProcessorUsageMaxReset();
        AudioMemoryUsageMaxReset();
    }
}
