// ============================================================================
//  MnMVoice.cpp
// ============================================================================
#include "MnMVoice.h"

namespace mmnova {

// =============================================================== AMP env ====
s24 AmpEnvelope::tick (int atk, int dec, int sus, int rel)
{
    const s24* rt = tables::rate();
    const s24  susLevel = fmul (paramWord (sus), paramWord (sus));   // (V-$0E)^2

    switch (state)
    {
        case Attack:                                     // состояние 1
            level = sat24 ((s56) level + rt[atk & 0x7F]);
            if (level >= kOne24) { level = kOne24; state = ahdMode ? Decay : Decay; }
            break;

        case Decay:                                      // состояние 4
            level = sat24 ((s56) level - fmul (rt[dec & 0x7F], level));
            if (level <= susLevel) { level = susLevel; state = ahdMode ? Release : Sustain; }
            break;

        case Sustain:                                    // состояние 5
            level = susLevel;
            break;

        case Release:                                    // состояние 2
            level = sat24 ((s56) level - rt[rel & 0x7F]);
            if (level <= 0) { level = 0; state = Idle; }
            break;

        default:                                         // default: полный уровень
            level = 0;
            break;
    }
    return level;
}

// =================================================================== LFO ====
s24 KernelLfo::tick (int speed7, int mult7, int shape, bool freeRun)
{
    const s24* rt = tables::rate();
    const uint32_t inc = (uint32_t) rt[speed7 & 0x7F] * (uint32_t) ((mult7 >> 4) + 1);
    const uint32_t prev = phase;
    phase += inc;
    wrapped = phase < prev;
    if (wrapped && ! freeRun) phase = 0;

    const uint64_t ph48 = (uint64_t) phase << 24;
    switch (shape)
    {
        case Sine:   return lerpTable<tables::kSineBits> (tables::sine(), ph48);
        case Saw:    return (s24) ((int32_t) (phase >> 8) - 0x800000);
        case Ramp:   return (s24) (0x7FFFFF - (int32_t) (phase >> 8));
        case Square: return (phase & 0x80000000u) ? kOne24 : kMin24;
        case Exp:    { const double x = phase / 4294967296.0;
                       return fromFloat (std::exp (-4.0 * x) * 2.0 - 1.0); }
        case Random: if (wrapped) { rng = rng * 1664525u + 1013904223u;
                                    sh = (s24) ((int32_t) (rng >> 8) - 0x800000); }
                     return sh;
        default:     { // Tri
                       const int32_t s = (int32_t) (phase >> 7) - 0x1000000;
                       return sat24 (s < 0 ? -s - 0x800000 : 0x800000 - s); }
    }
}

// =================================================================== SVF ====
void SVF2::setCutoff (int base7, int width7, double sampleRate)
{
    // Base = частота среза, Width = полоса. Прошивочные коэффициенты служат
    // границами нормировки (P:$5D3: $F528BD / $4A4DF0).
    constexpr double kCoefA = -0.0847570;   // $F528BD
    constexpr double kCoefB =  0.5805054;   // $4A4DF0
    const double n  = base7 / 127.0;
    const double fc = 20.0 * std::pow (1000.0, n);
    double ff = 2.0 * std::sin (3.14159265358979 * std::min (fc, sampleRate * 0.45) / sampleRate);
    ff = std::clamp (ff, 0.0, kCoefB * 2.0);
    f  = fromFloat (ff);
    const double wnorm = width7 / 127.0;
    q  = fromFloat (std::clamp (1.0 - wnorm * (1.0 + kCoefA), 0.02, 0.999));
    bandpass = width7 < 127;
}

s24 SVF2::process (s24 in)
{
    // stage 1
    s24 lpo = sat24 (lp1 >> 24);
    s24 bpo = sat24 (bp1 >> 24);
    s24 hp  = sat24 ((s56) in - lpo - fmul (q, bpo));
    bp1 += mpy (f, hp);  bpo = sat24 (bp1 >> 24);
    lp1 += mpy (f, bpo); lpo = sat24 (lp1 >> 24);
    s24 y = bandpass ? bpo : lpo;
    // stage 2 (вторая одинаковая петля прошивки)
    s24 lpo2 = sat24 (lp2 >> 24);
    s24 bpo2 = sat24 (bp2 >> 24);
    s24 hp2  = sat24 ((s56) y - lpo2 - fmul (q, bpo2));
    bp2 += mpy (f, hp2);  bpo2 = sat24 (bp2 >> 24);
    lp2 += mpy (f, bpo2); lpo2 = sat24 (lp2 >> 24);
    return bandpass ? bpo2 : lpo2;
}

// ================================================================= Voice ====
void Voice::prepare (double sampleRate, int voiceIndex)
{
    rate = sampleRate; vIndex = voiceIndex;
    tables::init();
    env.reset(); svf.reset();
    for (auto& l : lfo) l.reset();
    if (mach) mach->init (voiceIndex);
}

void Voice::setMachine (std::unique_ptr<Machine> m)
{
    mach = std::move (m);
    if (mach) { mach->init (vIndex); mach->tickConfig(); }
}

void Voice::noteOn (int midiNote, int velocity)
{
    note = midiNote; vel = velocity; trig = true;
    // target питча в единицах таблицы: 2048 ступеней на октаву, 11 октав
    target1 = (int32_t) std::lrint ((midiNote - 12) * (2048.0 / 12.0));
    target1 = std::clamp<int32_t> (target1, 0, tables::kPitchAccMax);
    if (page.portamento == 0) accum1 = target1;
    env.noteOn();
    for (auto& l : lfo) l.reset();
    svf.setCutoff (page.base, page.width, rate);
}

void Voice::noteOff() { env.noteOff(); }

// pre-voice P:$262 -------------------------------------------------------------
s56 Voice::preVoice()
{
    // 2 slew-лимитера (портаменто). Скорость из той же rate-таблицы.
    const s24 slew = tables::rate()[page.portamento & 0x7F];
    const int32_t step = std::max<int32_t> (1, (int32_t) (slew >> 14));
    if (accum1 < target1) accum1 = std::min (target1, accum1 + step);
    if (accum1 > target1) accum1 = std::max (target1, accum1 - step);

    int32_t acc = accum1 + accum2;

    // LFO -> питч (применяется ДО машины, это и есть смысл pre-voice)
    for (int i = 0; i < 3; ++i)
        if (page.lfoDestParam[i] < 0 && page.lfoDepth[i] > 0)
        {
            const s24 v = lfo[i].tick (page.lfoSpeed[i], page.lfoMult[i], page.lfoShape[i], true);
            acc += (int32_t) ((int64_t) v * page.lfoDepth[i] / (127 * 2048));
        }

    acc = std::clamp<int32_t> (acc, 0, tables::kPitchAccMax);

    // P:$2D8-$2EA: idx = acc >> 11 (октава), frac = acc & $7FF
    const int oct  = acc >> 11;
    const int frac = acc & 0x7FF;
    const s24 w    = tables::pitch()[frac];              // 0.5 * 2^(frac/2048)
    s56 a = mpy (w, tables::kPitchScale);                // * $1D22A
    a <<= oct;                                           // сдвиг октавы (asl)
    return a >> 3;                                       // ...>>3 (масштаб kernel)
}

void Voice::renderBlock (s24* outL, s24* outR, const s24* fxIn)
{
    if (! mach) return;
    mach->tickConfig();

    BlockContext ctx;
    ctx.pitchInc   = preVoice();
    ctx.input      = fxIn;
    ctx.voiceIndex = vIndex;
    ctx.trigger    = trig;
    ctx.rate       = rate;
    trig = false;

    mach->process (blockBuf, ctx);          // jsr (r1) — 16 кадров

    // ---- post-voice P:$4A8 ---------------------------------------------------
    const s24 panW  = paramWord (page.pan);
    const double pn = paramIndex (panW) / 127.0;
    const s24 gL = fromFloat (std::cos (pn * 1.5707963267948966));
    const s24 gR = fromFloat (std::sin (pn * 1.5707963267948966));
    const s24 vol = fmul (paramWord (page.volume), paramWord (vel > 0 ? vel : 100));

    for (int i = 0; i < kBlockFrames; ++i)
    {
        s24 s = svf.process (blockBuf[i]);
        const s24 e = env.tick (page.amp[0], page.amp[1], page.amp[2], page.amp[3]);
        s = fmul (s, e);
        s = fmul (s, vol);
        outL[i] = sat24 ((s56) outL[i] + fmul (s, gL));
        outR[i] = sat24 ((s56) outR[i] + fmul (s, gR));
    }
}

} // namespace mmnova
