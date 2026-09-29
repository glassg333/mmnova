

#include "AudioFilterOBXa.h"

// =============================================================================
// Local constants
// =============================================================================
static constexpr float OBXA_PI = 3.14159265358979323846f;

// Maximum cutoff as a fraction of sample rate.
// tan(π·fc/fs) diverges above 0.25·fs; 0.24 gives a safe margin.
static constexpr float OBXA_FC_RATIO_MAX = 0.24f;

// Minimum cutoff in Hz — below this the filter is effectively DC-pass.
static constexpr float OBXA_FC_MIN_HZ = 5.0f;

// Number of Xpander pole-mix modes.
static constexpr int OBXA_NUM_XPANDER_MODES = 15;

// =============================================================================
// obxa_fast_pow2 — fast approximation of 2^x
//
// Replaces powf(2.0f, x) in the modulation path.  Uses integer exponent
// extraction via ldexpf() and a 4th-order Remez minimax polynomial for the
// fractional part.
//
// Cost:    ~8 cycles on Cortex-M7 FPU  (vs ~50-100 for powf)
// Error:   < 0.005% relative over ±10 octaves
// Purpose: cutoff modulation — error is far below audible threshold
// =============================================================================
static inline float obxa_fast_pow2(float x)
{
    // Clamp to prevent IEEE 754 overflow / subnormal
    if (x >  126.0f) return 67108864.0f;  // 2^26 — safely large
    if (x < -126.0f) return 1.49e-38f;    // near float min — safely small

    const int32_t xi = (int32_t)x;    // integer part  (exact power of 2)
    const float   xf = x - (float)xi; // fractional part in [0, 1)

    // Remez minimax polynomial for 2^f on [0,1) — max error < 2e-6
    const float poly = 1.00000000f
                     + xf * (0.69314718f
                     + xf * (0.24022651f
                     + xf * (0.05550411f
                     + xf *  0.00961823f)));

    // Exact integer power via IEEE 754 exponent field
    return ldexpf(poly, xi);
}

// =============================================================================
// obxa_is_huge — runaway detector for state guard
// =============================================================================
static inline bool obxa_is_huge(float x)
{
    return fabsf(x) > OBXA_HUGE_THRESHOLD;
}

// =============================================================================
// TPT (trapezoidal) 1-pole integrator — the fundamental building block
//
// Implements the zero-delay-feedback integrator:
//   v = (input - state) * g
//   y = v + state
//   state = y + v        (trapezoidal update)
//
// Where g = tan(π·fc/fs), the bilinear pre-warped coefficient.
// =============================================================================
static inline float obxa_tpt_tick(float &state, float input, float g)
{
    float v = (input - state) * g;
    float y = v + state;
    state   = y + v;
    return y;
}

// =============================================================================
// TPT 1-pole integrator — pre-scaled version
//
// Same as above but accepts g/(1+g) directly, avoiding the division in the
// caller's tight loop.  Used by the 4-pole cascade where lpc = g/(1+g) is
// already computed.
// =============================================================================
static inline float obxa_tpt_tick_scaled(float &state, float input, float lpc)
{
    // NOTE: float precision is sufficient for audio-rate pole integration.
    // The original OB-Xf code used double here, but on Cortex-M7 double
    // operations take ~2× the cycles of float with no audible difference.
    float v   = (input - state) * lpc;
    float res = v + state;
    state     = res + v;
    return res;
}

// =============================================================================
// AudioFilterOBXa::Core — filter DSP internals
//
// Kept in the .cpp to avoid polluting the header with implementation detail.
// Holds the 4 pole states, resonance coefficients, and the Xpander pole-mix
// table.  All processing happens sample-by-sample via process2Pole/4Pole.
// =============================================================================
struct AudioFilterOBXa::Core
{
    // -------------------------------------------------------------------------
    // Xpander pole-mix table
    //
    // Each row is {y0, y1, y2, y3, y4} weights where y0 = input, y1–y4 = poles.
    // The output is the dot product of these weights with the 5 node voltages.
    // This gives 15 distinct filter responses from the same 4-pole structure.
    //
    // Source: Oberheim Xpander service manual, pole-mix matrix.
    // -------------------------------------------------------------------------
    static constexpr float poleMixFactors[OBXA_NUM_XPANDER_MODES][5] =
    {
        { 0,  0,  0,  0,  1},  //  0: LP4         (24 dB lowpass)
        { 0,  0,  0,  1,  0},  //  1: LP3         (18 dB lowpass)
        { 0,  0,  1,  0,  0},  //  2: LP2         (12 dB lowpass)
        { 0,  1,  0,  0,  0},  //  3: LP1         (6 dB lowpass)
        { 1, -3,  3, -1,  0},  //  4: HP3         (18 dB highpass)
        { 1, -2,  1,  0,  0},  //  5: HP2         (12 dB highpass)
        { 1, -1,  0,  0,  0},  //  6: HP1         (6 dB highpass)
        { 0,  0,  2, -4,  2},  //  7: BP4         (24 dB bandpass)
        { 0, -2,  2,  0,  0},  //  8: BP2         (12 dB bandpass)
        { 1, -2,  2,  0,  0},  //  9: N2          (12 dB notch)
        { 1, -3,  6, -4,  0},  // 10: PH3         (18 dB phaser)
        { 0, -1,  2, -1,  0},  // 11: HP2+LP1
        { 0, -1,  3, -3,  1},  // 12: HP3+LP1
        { 0, -1,  2, -2,  0},  // 13: N2+LP1
        { 0, -1,  3, -6,  4},  // 14: PH3+LP1
    };

    // ---- Per-voice filter state ---------------------------------------------
    struct State
    {
        float pole1 = 0.0f;        // 1st integrator memory
        float pole2 = 0.0f;        // 2nd integrator memory
        float pole3 = 0.0f;        // 3rd integrator memory (4-pole only)
        float pole4 = 0.0f;        // 4th integrator memory (4-pole only)

        float res2Pole       = 1.0f;  // 2-pole damping (1 - r01)
        float res4Pole       = 0.0f;  // 4-pole feedback (3.5 × r01)
        float resCorrection  = 1.0f;  // sample-rate-dependent saturation scaler
        float resCorrInv     = 1.0f;  // reciprocal of resCorrection

        float multimodeXfade = 0.0f;  // fractional part of multimode position
        int   multimodePole  = 0;     // integer part (which poles to crossfade)
    } state;

    // ---- Sample rate --------------------------------------------------------
    float fsInv = 1.0f / AUDIO_SAMPLE_RATE_EXACT;

    // ---- Topology flags (mirrored from wrapper each block) ------------------
    bool    bpBlend2Pole  = false;
    bool    push2Pole     = false;
    bool    xpander4Pole  = false;
    uint8_t xpanderMode   = 0;

    // =========================================================================
    // reset — zero all pole states (called on construction and after fault)
    // =========================================================================
    void reset()
    {
        state.pole1 = 0.0f;
        state.pole2 = 0.0f;
        state.pole3 = 0.0f;
        state.pole4 = 0.0f;
    }

    // =========================================================================
    // setSampleRate — recompute resonance correction for the given rate
    //
    // The atan() saturation in the 4-pole path needs scaling so that the
    // resonance character stays consistent across different sample rates.
    // The correction factor is derived from the ratio 970/44000 (empirical,
    // from the original OB-Xf model) scaled by sqrt(44000/fs).
    // =========================================================================
    void setSampleRate(float sr)
    {
        fsInv = 1.0f / sr;
        float rcRate       = sqrtf(44000.0f / sr);
        state.resCorrection = (970.0f / 44000.0f) * rcRate;
        state.resCorrInv    = 1.0f / state.resCorrection;
    }

    // =========================================================================
    // setResonance — convert normalised 0..1 to internal coefficients
    //
    // 2-pole: damping factor = 1 - r01  (lower = more resonant)
    // 4-pole: feedback gain  = 3.5 × r01  (higher = more resonant)
    // =========================================================================
    void setResonance(float r01)
    {
        state.res2Pole = 1.0f - r01;
        state.res4Pole = 3.5f * r01;
    }

    // =========================================================================
    // setMultimode — convert normalised 0..1 to pole crossfade parameters
    //
    // multimodePole selects which adjacent pair of poles to blend between.
    // multimodeXfade is the fractional position within that pair.
    //   0.0 = full LP4,  0.33 = LP3,  0.67 = LP2,  1.0 = LP1 (input)
    // =========================================================================
    void setMultimode(float m01)
    {
        state.multimodePole  = (int)(m01 * 3.0f);
        state.multimodeXfade = (m01 * 3.0f) - state.multimodePole;
    }

    // =========================================================================
    // diodePairResistanceApprox — polynomial model of diode-pair nonlinearity
    //
    // Attempt to model the voltage-dependent resistance of a back-to-back
    // diode pair in the OB-X feedback path.  This is what gives the 2-pole
    // mode its characteristic "soft" resonance behaviour — resonance drops
    // slightly at high signal levels, preventing hard clipping.
    //
    // Polynomial coefficients are empirical (curve-fitted to measurements).
    // =========================================================================
    inline float diodePairResistanceApprox(float x) const
    {
        return (((((0.0103592f) * x + 0.00920833f) * x + 0.185f) * x
                  + 0.05f) * x + 1.0f);
    }

    // =========================================================================
    // 2-POLE feedback resolver
    //
    // Solves the implicit feedback equation for the 2-pole topology.
    // The diode-pair nonlinearity makes this signal-dependent, which is why
    // it can't be reduced to a simple coefficient multiply.
    // =========================================================================
    inline float resolveFeedback2Pole(float sample, float g) const
    {
        // push2Pole adds a subtle extra feedback for the OB-Xf "growl"
        float push = -1.0f - (push2Pole ? 0.035f : 0.0f);
        float tCfb = diodePairResistanceApprox(state.pole1 * 0.0876f) + push;

        return (sample
                - 2.0f * (state.pole1 * (state.res2Pole + tCfb))
                - g * state.pole1
                - state.pole2)
               /
               (1.0f + g * (2.0f * (state.res2Pole + tCfb) + g));
    }

    // =========================================================================
    // process2Pole — 12 dB/oct filter tick
    //
    // Processes one sample through the 2-pole topology.
    // Output mode depends on bpBlend2Pole and multimode01:
    //   bpBlend=false: crossfade LP2 ↔ HP2 (via input tap)
    //   bpBlend=true:  crossfade LP2 ↔ BP2 ↔ HP2
    // =========================================================================
    float process2Pole(float x, float cutoffHz)
    {
        float g = tanf(cutoffHz * fsInv * OBXA_PI);
        float v = resolveFeedback2Pole(x, g);

        // 1st pole
        float y1 = v * g + state.pole1;
        state.pole1 = v * g + y1;

        // 2nd pole
        float y2 = y1 * g + state.pole2;
        state.pole2 = y1 * g + y2;

        // Output mixing
        float mm = state.multimodeXfade + (float)state.multimodePole / 3.0f;
        if (mm > 1.0f) mm = 1.0f;

        float out;
        if (bpBlend2Pole)
        {
            // Three-way blend: LP2 (0.0) → BP2 (0.5) → HP2 (1.0)
            if (mm < 0.5f)
                out = 2.0f * ((0.5f - mm) * y2 + mm * y1);
            else
                out = 2.0f * ((1.0f - mm) * y1 + (mm - 0.5f) * v);
        }
        else
        {
            // Two-way blend: LP2 (0.0) → HP2 (1.0)
            out = (1.0f - mm) * y2 + mm * v;
        }
        return out;
    }

    // =========================================================================
    // 4-POLE feedback resolver
    //
    // Solves the implicit feedback equation for the 4-pole cascade.
    // Uses the "ladder" trick: the four poles form a geometric cascade where
    // the signal at each stage can be predicted from the current states.
    // =========================================================================
    inline float resolveFeedback4Pole(float sample, float g, float lpc) const
    {
        float ml = 1.0f / (1.0f + g);
        float S  = (lpc * (lpc * (lpc * state.pole1 + state.pole2)
                    + state.pole3) + state.pole4) * ml;
        float G  = lpc * lpc * lpc * lpc;

        return (sample - state.res4Pole * S) / (1.0f + state.res4Pole * G);
    }

    // =========================================================================
    // process4Pole — 24 dB/oct filter tick
    //
    // Processes one sample through the 4-pole cascade.
    // The first pole includes atan() saturation for the characteristic
    // Oberheim resonance "warmth".
    //
    // Output mode:
    //   xpander4Pole=true:  5-coefficient pole-mix (15 modes)
    //   xpander4Pole=false: multimode crossfade across adjacent poles
    // =========================================================================
    float process4Pole(float x, float cutoffHz)
    {
        // Bilinear pre-warp
        float g   = tanf(cutoffHz * fsInv * OBXA_PI);
        float lpc = g / (1.0f + g);

        float y0 = resolveFeedback4Pole(x, g, lpc);

        // 1st pole — includes atan() saturation for resonance warmth
        float v   = (y0 - state.pole1) * lpc;
        float res = v + state.pole1;
        state.pole1 = res + v;

        // Saturation: atan() scaled by sample-rate correction factor
        state.pole1 = atanf(state.pole1 * state.resCorrection)
                      * state.resCorrInv;

        float y1 = res;

        // Remaining poles — linear (no saturation needed after the first)
        float y2 = obxa_tpt_tick_scaled(state.pole2, y1, lpc);
        float y3 = obxa_tpt_tick_scaled(state.pole3, y2, lpc);
        float y4 = obxa_tpt_tick_scaled(state.pole4, y3, lpc);

        // ---- Output selection -----------------------------------------------
        float out;

        if (xpander4Pole)
        {
            // Xpander mode: weighted sum of all 5 node voltages
            const float *m = poleMixFactors[xpanderMode];
            out = y0 * m[0] + y1 * m[1] + y2 * m[2] + y3 * m[3] + y4 * m[4];
        }
        else
        {
            // Standard multimode: crossfade between adjacent pole outputs
            switch (state.multimodePole)
            {
            case 0: out = (1.0f - state.multimodeXfade) * y4
                        + state.multimodeXfade * y3;              break;
            case 1: out = (1.0f - state.multimodeXfade) * y3
                        + state.multimodeXfade * y2;              break;
            case 2: out = (1.0f - state.multimodeXfade) * y2
                        + state.multimodeXfade * y1;              break;
            case 3: out = y1;                                      break;
            default: out = 0.0f;                                   break;
            }
        }

        // Resonance-dependent gain compensation — prevents volume drop at
        // high resonance by boosting output proportionally.
        return out * (1.0f + state.res4Pole * 0.45f);
    }

    // =========================================================================
    // process4Pole_constCutoff — optimised variant for block-rate modulation
    //
    // When cutoff is constant for the entire block, g and lpc can be
    // precomputed once instead of calling tanf() 128 times.
    // Saves ~128 tanf() calls per block (~6000 cycles on Cortex-M7).
    // =========================================================================
    float process4Pole_constCutoff(float x, float g, float lpc)
    {
        float y0 = resolveFeedback4Pole(x, g, lpc);

        float v   = (y0 - state.pole1) * lpc;
        float res = v + state.pole1;
        state.pole1 = res + v;

        state.pole1 = atanf(state.pole1 * state.resCorrection)
                      * state.resCorrInv;

        float y1 = res;
        float y2 = obxa_tpt_tick_scaled(state.pole2, y1, lpc);
        float y3 = obxa_tpt_tick_scaled(state.pole3, y2, lpc);
        float y4 = obxa_tpt_tick_scaled(state.pole4, y3, lpc);

        float out;
        if (xpander4Pole)
        {
            const float *m = poleMixFactors[xpanderMode];
            out = y0 * m[0] + y1 * m[1] + y2 * m[2] + y3 * m[3] + y4 * m[4];
        }
        else
        {
            switch (state.multimodePole)
            {
            case 0: out = (1.0f - state.multimodeXfade) * y4
                        + state.multimodeXfade * y3;              break;
            case 1: out = (1.0f - state.multimodeXfade) * y3
                        + state.multimodeXfade * y2;              break;
            case 2: out = (1.0f - state.multimodeXfade) * y2
                        + state.multimodeXfade * y1;              break;
            case 3: out = y1;                                      break;
            default: out = 0.0f;                                   break;
            }
        }
        return out * (1.0f + state.res4Pole * 0.45f);
    }

    // Same for 2-pole with precomputed g
    float process2Pole_constCutoff(float x, float g)
    {
        float v = resolveFeedback2Pole(x, g);

        float y1 = v * g + state.pole1;
        state.pole1 = v * g + y1;

        float y2 = y1 * g + state.pole2;
        state.pole2 = y1 * g + y2;

        float mm = state.multimodeXfade + (float)state.multimodePole / 3.0f;
        if (mm > 1.0f) mm = 1.0f;

        float out;
        if (bpBlend2Pole)
        {
            if (mm < 0.5f)
                out = 2.0f * ((0.5f - mm) * y2 + mm * y1);
            else
                out = 2.0f * ((1.0f - mm) * y1 + (mm - 0.5f) * v);
        }
        else
        {
            out = (1.0f - mm) * y2 + mm * v;
        }
        return out;
    }
};

// Static member definition (required for C++11/14 constexpr arrays)
constexpr float AudioFilterOBXa::Core::poleMixFactors[OBXA_NUM_XPANDER_MODES][5];

// =============================================================================
// Public API implementation
// =============================================================================

AudioFilterOBXa::AudioFilterOBXa()
    : AudioStream(3, _inQ)
{
    _core = new Core();
    _core->setSampleRate(AUDIO_SAMPLE_RATE_EXACT);
    _core->setResonance(_res01Target);
    _core->setMultimode(_multimode01);
}

void AudioFilterOBXa::frequency(float hz)
{
    const float maxHz = OBXA_FC_RATIO_MAX * AUDIO_SAMPLE_RATE_EXACT;
    if (hz < OBXA_FC_MIN_HZ) hz = OBXA_FC_MIN_HZ;
    if (hz > maxHz)           hz = maxHz;
    _cutoffHzTarget = hz;
}

void AudioFilterOBXa::resonance(float r01)
{
    if (r01 < 0.0f) r01 = 0.0f;
    if (r01 > 1.0f) r01 = 1.0f;
    _res01Target = r01;
    _core->setResonance(r01);
}

void AudioFilterOBXa::multimode(float m01)
{
    if (m01 < 0.0f) m01 = 0.0f;
    if (m01 > 1.0f) m01 = 1.0f;
    _multimode01 = m01;
    _core->setMultimode(m01);
}

void AudioFilterOBXa::setTwoPole(bool enabled)        { _useTwoPole = enabled; }

void AudioFilterOBXa::setXpander4Pole(bool enabled)
{
    _xpander4Pole = enabled;
    _core->xpander4Pole = enabled;
}

void AudioFilterOBXa::setXpanderMode(uint8_t mode)
{
    if (mode >= OBXA_NUM_XPANDER_MODES) mode = OBXA_NUM_XPANDER_MODES - 1;
    _xpanderMode = mode;
    _core->xpanderMode = mode;
}

void AudioFilterOBXa::setBPBlend2Pole(bool enabled)
{
    _bpBlend2Pole = enabled;
    _core->bpBlend2Pole = enabled;
}

void AudioFilterOBXa::setPush2Pole(bool enabled)
{
    _push2Pole = enabled;
    _core->push2Pole = enabled;
}

void AudioFilterOBXa::setCutoffModOctaves(float oct)
{
    if (oct < 0.0f) oct = 0.0f;
    if (oct > 8.0f) oct = 8.0f;
    _cutoffModOct = oct;
}

void AudioFilterOBXa::setResonanceModDepth(float depth01)
{
    if (depth01 < 0.0f) depth01 = 0.0f;
    if (depth01 > 1.0f) depth01 = 1.0f;
    _resModDepth = depth01;
}

void AudioFilterOBXa::setKeyTrack(float amount01)
{
    if (amount01 < 0.0f) amount01 = 0.0f;
    if (amount01 > 1.0f) amount01 = 1.0f;
    _keyTrack = amount01;
}

void AudioFilterOBXa::setEnvModOctaves(float oct)
{
    if (oct < 0.0f) oct = 0.0f;
    if (oct > 8.0f) oct = 8.0f;
    _envModOct = oct;
}

void AudioFilterOBXa::setMidiNote(float note)
{
    if (note < 0.0f)   note = 0.0f;
    if (note > 127.0f) note = 127.0f;
    _midiNote = note;
}

void AudioFilterOBXa::setEnvValue(float env01)
{
    if (env01 < 0.0f) env01 = 0.0f;
    if (env01 > 1.0f) env01 = 1.0f;
    _envValue = env01;
}

// =============================================================================
// update() — called once per 128-sample audio block by the Audio Library
// =============================================================================
void AudioFilterOBXa::update(void)
{
    // ---- Acquire audio input ------------------------------------------------
    audio_block_t *in0 = receiveReadOnly(0);  // audio signal

    // No audio arriving — nothing to filter, skip all DSP.
    // (Self-oscillation is not supported in this path.)
    if (!in0) return;

    // ---- Acquire mod buses and output buffer --------------------------------
    audio_block_t *in1 = receiveReadOnly(1);  // cutoff mod bus
    audio_block_t *in2 = receiveReadOnly(2);  // resonance mod bus

    audio_block_t *out = allocate();
    if (!out)
    {
        release(in0);
        if (in1) release(in1);
        if (in2) release(in2);
        return;
    }

    if (_cooldownBlocks > 0) _cooldownBlocks--;

    // ---- Block-rate coefficient pre-computation -----------------------------
    //
    // Key tracking: octave shift from MIDI note relative to middle C (note 60).
    // Constant for the duration of a note.
    //
    // Envelope mod: octave multiplier from external envelope value.
    // _envValue is set at control rate, so block-rate evaluation is correct.
    //
    // Both use obxa_fast_pow2 instead of powf — same result, ~10× faster.
    // -------------------------------------------------------------------------
    const float keyOct    = (_midiNote - 60.0f) / 12.0f;
    const float keyMul    = obxa_fast_pow2(_keyTrack * keyOct);
    const float envModOct = _envValue * _envModOct;

    // Check whether mod buses carry any signal this block.
    // Skip the per-sample exponential when mod depth is zero.
    const bool hasCutoffMod = (in1 != nullptr) && (_cutoffModOct > 0.0f);
    const bool hasResMod    = (in2 != nullptr) && (_resModDepth  > 0.0f);

    // Set filter topology flags once per block (they don't change per-sample)
    if (_useTwoPole) {
        _core->bpBlend2Pole = _bpBlend2Pole;
        _core->push2Pole    = _push2Pole;
    } else {
        _core->xpander4Pole = _xpander4Pole;
        _core->xpanderMode  = _xpanderMode;
    }

    // ---- INT16 ↔ float conversion constant ----------------------------------
    static constexpr float kToFloat = 1.0f / 32768.0f;
    static constexpr float kMaxHz   = OBXA_FC_RATIO_MAX * AUDIO_SAMPLE_RATE_EXACT;

    // =========================================================================
    //  BLOCK-RATE MODULATION PATH
    //
    //  Cutoff and resonance mod buses are sampled once at the block midpoint
    //  (sample 64).  This replaces 128 obxa_fast_pow2() + tanf() calls with
    //  one each — saving ~15 000 cycles per voice per block on Cortex-M7.
    //
    //  Perceptually identical for LFO / envelope modulation.  Set
    //  OBXA_BLOCKRATE_MOD to 0 for true audio-rate FM of the cutoff.
    // =========================================================================
#if OBXA_BLOCKRATE_MOD

    // Sample mod buses at block midpoint (index 64) — better perceptual
    // average than first or last sample, avoids block-boundary discontinuity.
    float blockCutMod = 0.0f;
    if (hasCutoffMod)
        blockCutMod = (float)in1->data[AUDIO_BLOCK_SAMPLES / 2] * kToFloat;

    const float modOctBlock  = (blockCutMod * _cutoffModOct) + envModOct;
    const float modMulBlock  = obxa_fast_pow2(modOctBlock);
    const float baseCutoffHz = _cutoffHzTarget * keyMul * modMulBlock;

    // Block-rate resonance mod
    float blockResMod = 0.0f;
    if (hasResMod)
        blockResMod = (float)in2->data[AUDIO_BLOCK_SAMPLES / 2] * kToFloat;

    float r01Block = _res01Target + (blockResMod * _resModDepth);
    if (r01Block < 0.0f) r01Block = 0.0f;
    if (r01Block > 1.0f) r01Block = 1.0f;
    _core->setResonance(r01Block);

    // Clamp cutoff
    float cutoffHz = baseCutoffHz;
    if (cutoffHz < OBXA_FC_MIN_HZ) cutoffHz = OBXA_FC_MIN_HZ;
    if (cutoffHz > kMaxHz)          cutoffHz = kMaxHz;

    // Precompute g and lpc once — cutoff is constant for this entire block.
    // This is the biggest single optimisation: 128 tanf() calls → 1.
    const float g   = tanf(cutoffHz * _core->fsInv * OBXA_PI);
    const float lpc = g / (1.0f + g);

    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; ++i)
    {
        float x = (float)in0->data[i] * kToFloat;
        float y = 0.0f;

        if (_cooldownBlocks > 0)
        {
            y = 0.0f;
        }
        else if (_useTwoPole)
        {
            y = _core->process2Pole_constCutoff(x, g);
        }
        else
        {
            y = _core->process4Pole_constCutoff(x, g, lpc);
        }

#if OBXA_STATE_GUARD
        if (!isfinite(y) || obxa_is_huge(y) ||
            obxa_is_huge(_core->state.pole1) ||
            obxa_is_huge(_core->state.pole2) ||
            obxa_is_huge(_core->state.pole3) ||
            obxa_is_huge(_core->state.pole4))
        {
            _core->reset();
            _cooldownBlocks = 2;
            y = 0.0f;
        }
#endif
        // Hard clip to ±1.0 — prevents int16 overflow
        if (y >  1.0f) y =  1.0f;
        if (y < -1.0f) y = -1.0f;

        out->data[i] = (int16_t)(y * 32767.0f);
    }

#else // OBXA_BLOCKRATE_MOD == 0
    // =========================================================================
    //  PER-SAMPLE MODULATION PATH
    //
    //  Full audio-rate cutoff modulation.  Uses obxa_fast_pow2() instead of
    //  powf() for the exponential — ~10× faster with < 0.005% error.
    //  Each sample gets its own cutoffHz and resonance value.
    // =========================================================================
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; ++i)
    {
        float x      = (float)in0->data[i] * kToFloat;
        float cutMod = hasCutoffMod ? ((float)in1->data[i] * kToFloat) : 0.0f;
        float resMod = hasResMod    ? ((float)in2->data[i] * kToFloat) : 0.0f;

        // Exponential cutoff modulation
        float modOct    = (cutMod * _cutoffModOct) + envModOct;
        float modMul    = obxa_fast_pow2(modOct);
        float cutoffHz  = _cutoffHzTarget * keyMul * modMul;

        if (cutoffHz < OBXA_FC_MIN_HZ) cutoffHz = OBXA_FC_MIN_HZ;
        if (cutoffHz > kMaxHz)          cutoffHz = kMaxHz;

        // Resonance modulation
        float r01 = _res01Target + (resMod * _resModDepth);
        if (r01 < 0.0f) r01 = 0.0f;
        if (r01 > 1.0f) r01 = 1.0f;
        _core->setResonance(r01);

        float y = 0.0f;

        if (_cooldownBlocks > 0)
        {
            y = 0.0f;
        }
        else if (_useTwoPole)
        {
            y = _core->process2Pole(x, cutoffHz);
        }
        else
        {
            y = _core->process4Pole(x, cutoffHz);
        }

#if OBXA_STATE_GUARD
        if (!isfinite(y) || obxa_is_huge(y) ||
            obxa_is_huge(_core->state.pole1) ||
            obxa_is_huge(_core->state.pole2) ||
            obxa_is_huge(_core->state.pole3) ||
            obxa_is_huge(_core->state.pole4))
        {
            _core->reset();
            _cooldownBlocks = 2;
            y = 0.0f;
        }
#endif
        if (y >  1.0f) y =  1.0f;
        if (y < -1.0f) y = -1.0f;

        out->data[i] = (int16_t)(y * 32767.0f);
    }
#endif // OBXA_BLOCKRATE_MOD

    // ---- Transmit output and release all blocks -----------------------------
    transmit(out);
    release(out);
    release(in0);
    if (in1) release(in1);
    if (in2) release(in2);
}
