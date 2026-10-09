#include "EngineRegistry.h"

#include "../engines/CombScannerVariants1Fav.h"
#include "../engines/CombScannerVariants2.h"
#include "../engines/CombScannerVariants3.h"
#include "../engines/CombScannerVariants4.h"
#include "../engines/CsMmVariants.h"
#include "../engines/ZigZag.h"

#include <array>
#include <memory>
#include <string>

namespace lab
{
namespace
{

inline int countOf (const ParamInfo* table)
{
    int n = 0;
    while (table[n].key != nullptr)
        ++n;
    return n;
}

// Модели без 02 Tight Crossfade / 03 Long Resonator / 06 Ping Pong.
static constexpr int kCombModelMap[] = { 0, 1, 2, 3, 4, 5, 6 };
static constexpr int kCombModelCount = 7;

static int clampCombModel (int m) noexcept
{
    return m < 0 ? 0 : (m >= kCombModelCount ? kCombModelCount - 1 : m);
}

#define P_END { nullptr, nullptr, 0, 0, 0, 0, nullptr, 0, nullptr }

// --------------------------- variants 1 fav --------------------------------
class Variants1FavEngine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "var_1fav", "variants 1 (fav)",
                 "other/zigcomb/comb_scanner_variants 1 fav",
                 "BASE: mux-scan (selects a comb, does not stretch time). Feedback knob like var3. Damp=0 / Phase=0 do not eat the tail. Delay max 1000 ms." };
    }

    int numModels() const override { return kCombModelCount; }
    const char* modelName (int modelIndex) const override
    {
        return var1::CombScannerVariantsDSP::getVariantName (kCombModelMap[clampCombModel (modelIndex)]);
    }

    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }

    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        applyCombParams (index, value, p);
        dsp.setParameters (p);
    }

    void setModel (int modelIndex) override
    {
        p.variant = kCombModelMap[clampCombModel (modelIndex)];
        dsp.setParameters (p);
    }

    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

    static void applyCombParams (int index, float value, var1::CombScannerVariantsDSP::Parameters& p)
    {
        switch (index)
        {
            case 0: p.gain = value; break;
            case 1: p.feedback = value; break;
            case 2: p.damp = value; break;
            case 3: p.phase = value; break;
            case 4: p.delay1Ms = value; break;
            case 5: p.delay2Ms = value; break;
            case 6: p.scan = value; break;
            case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14:
                p.ratios[index - 7] = value; break;
            case 15: p.manualRatios = value; break;
            case 16: p.scanCurve = value; break;
            case 17: p.modX = value; break;
            case 18: p.apBaseX = value; break;
            case 19: p.apPhaseX = value; break;
            case 20: p.fbFloorX = value; break;
            case 21: p.fbRangeX = value; break;
            case 22: p.wetX = value; break;
            case 23: p.dry = value; break;
            case 24: p.outGain = value; break;
            case 25: p.spread = value; break;
            case 26: p.scanFade = value; break;
            case 27: p.scanMode = value; break;
            default: break;
        }
    }

    static constexpr ParamInfo table[] = {
        { "gain",     "Gain",      0.0f, 2.0f,    0.99f, 0.001f, "",   0, "Bank drive into the comb loop" },
        { "feedback", "Feedback",  0.0f, 0.999f,  0.99f, 0.001f, "",   0, "Loop gain — tail length" },
        { "damp",     "Damp",      0.0f, 0.999f,  0.0f,  0.001f, "",   0, "One-pole in the loop. 0 = bypass, does not touch the tail" },
        { "phase",    "Phase",     0.0f, 1.0f,    0.0f,  0.001f, "",   0, "All-pass in the loop (|H|=1). Colours modes, does not eat feedback" },
        { "delay1",   "Delay 1",   0.0f, 1000.0f, 32.6f, 0.01f,  "ms", 0, "Primary comb time (ms). Max 1000" },
        { "delay2",   "Delay 2",   0.0f, 1000.0f, 13.1f, 0.01f,  "ms", 0, "Parallel tap of the same loop (ms)" },
        { "scan",     "Scan",      0.0f, 1.0f,    0.0f,  0.001f, "",   0, "Selects which comb is heard (mux by default)" },
        { "ratio0", "Ratio 1", 0.05f, 8.0f, 0.33f, 0.01f, "", 1, "Voice 1 delay multiplier (when Manual Ratios is on)" },
        { "ratio1", "Ratio 2", 0.05f, 8.0f, 0.50f, 0.01f, "", 1, "Voice 2 delay multiplier" },
        { "ratio2", "Ratio 3", 0.05f, 8.0f, 0.66f, 0.01f, "", 1, "Voice 3 delay multiplier" },
        { "ratio3", "Ratio 4", 0.05f, 8.0f, 1.00f, 0.01f, "", 1, "Voice 4 delay multiplier" },
        { "ratio4", "Ratio 5", 0.05f, 8.0f, 1.11f, 0.01f, "", 1, "Voice 5 delay multiplier" },
        { "ratio5", "Ratio 6", 0.05f, 8.0f, 1.45f, 0.01f, "", 1, "Voice 6 delay multiplier" },
        { "ratio6", "Ratio 7", 0.05f, 8.0f, 2.22f, 0.01f, "", 1, "Voice 7 delay multiplier" },
        { "ratio7", "Ratio 8", 0.05f, 8.0f, 3.33f, 0.01f, "", 1, "Voice 8 delay multiplier" },
        { "manual",   "Manual Ratios", 0.0f, 1.0f, 0.0f,  1.0f,   "", 1, "1 = use the eight Ratio knobs instead of the model bank" },
        { "scancurve","Scan Curve",    0.2f, 3.0f, 1.0f,  0.01f,  "", 1, "Power curve on Scan position" },
        { "modx",     "Mod X",         0.0f, 3.0f, 1.0f,  0.01f,  "", 1, "Scales the model's LFO depth on delay times" },
        { "apbasex",  "AP Base X",     0.0f, 3.0f, 1.0f,  0.01f,  "", 1, "Scales the model's all-pass base coefficient" },
        { "apphasex", "AP Phase X",    0.0f, 3.0f, 1.0f,  0.01f,  "", 1, "Scales how far Phase turns the all-pass" },
        { "fbfloorx", "FB Floor X",    0.0f, 3.0f, 1.0f,  0.01f,  "", 1, "Scales the model's minimum feedback" },
        { "fbrangex", "FB Range X",    0.0f, 3.0f, 1.0f,  0.01f,  "", 1, "Scales the model's feedback range" },
        { "wetx",     "Wet X",         0.0f, 3.0f, 1.0f,  0.01f,  "", 1, "Scales the model's wet level" },
        { "dry",      "Dry",           0.0f, 1.0f, 0.12f, 0.001f, "", 1, "Dry signal mixed after the engine" },
        { "outgain",  "Out Gain",      0.0f, 8.0f, 4.5f,  0.01f,  "", 1, "Gain into the output tanh" },
        { "spread",   "Spread",        0.0f, 0.6f, 0.12f, 0.001f, "", 1, "How different each comb's character is" },
        { "scanfade", "Scan Fade",     0.002f, 0.25f, 0.02f, 0.001f, "s", 1, "Crossfade time when Scan jumps" },
        { "scanmode", "Scan Mode",     0.0f, 1.0f, 1.0f,  1.0f,   "", 1, "0 = neighbour sweep, 1 = multiplexer (original)" },
        P_END
    };

    var1::CombScannerVariantsDSP dsp;
    var1::CombScannerVariantsDSP::Parameters p;
};

// --------------------------- variants 2 ------------------------------------
class Variants2Engine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "var_2", "variants 2",
                 "other/zigcomb/comb_scanner_variants 2",
                 "Same loop as var1 + slight detune on delay 2. Mux-scan. Feedback knob added. No Crossfade/Resonator/PingPong models." };
    }

    int numModels() const override { return kCombModelCount; }
    const char* modelName (int modelIndex) const override
    {
        return var2::CombScannerVariantsDSP::getVariantName (kCombModelMap[clampCombModel (modelIndex)]);
    }

    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }
    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        switch (index)
        {
            case 0: p.gain = value; break;
            case 1: p.feedback = value; break;
            case 2: p.damp = value; break;
            case 3: p.phase = value; break;
            case 4: p.delay1Ms = value; break;
            case 5: p.delay2Ms = value; break;
            case 6: p.scan = value; break;
            case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14:
                p.ratios[index - 7] = value; break;
            case 15: p.manualRatios = value; break;
            case 16: p.scanCurve = value; break;
            case 17: p.modX = value; break;
            case 18: p.apBaseX = value; break;
            case 19: p.apPhaseX = value; break;
            case 20: p.fbFloorX = value; break;
            case 21: p.fbRangeX = value; break;
            case 22: p.wetX = value; break;
            case 23: p.dry = value; break;
            case 24: p.outGain = value; break;
            case 25: p.spread = value; break;
            case 26: p.scanFade = value; break;
            case 27: p.scanMode = value; break;
            case 28: p.detune2 = value; break;
            default: break;
        }
        dsp.setParameters (p);
    }

    void setModel (int modelIndex) override
    {
        p.variant = kCombModelMap[clampCombModel (modelIndex)];
        dsp.setParameters (p);
    }

    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

    static constexpr ParamInfo table[] = {
        { "gain",     "Gain",      0.0f, 2.0f,    0.99f, 0.001f, "",   0, "Bank drive into the comb loop" },
        { "feedback", "Feedback",  0.0f, 0.999f,  0.99f, 0.001f, "",   0, "Loop gain — tail length" },
        { "damp",     "Damp",      0.0f, 0.999f,  0.0f,  0.001f, "",   0, "One-pole in the loop. 0 = bypass" },
        { "phase",    "Phase",     0.0f, 1.0f,    0.0f,  0.001f, "",   0, "All-pass in the loop (|H|=1)" },
        { "delay1",   "Delay 1",   0.0f, 1000.0f, 32.6f, 0.01f,  "ms", 0, "Primary comb time (ms)" },
        { "delay2",   "Delay 2",   0.0f, 1000.0f, 13.1f, 0.01f,  "ms", 0, "Parallel tap (ms)" },
        { "scan",     "Scan",      0.0f, 1.0f,    0.0f,  0.001f, "",   0, "Selects which comb is heard" },
        { "ratio0", "Ratio 1", 0.05f, 8.0f, 0.33f, 0.01f, "", 1, "Voice 1 delay multiplier" },
        { "ratio1", "Ratio 2", 0.05f, 8.0f, 0.50f, 0.01f, "", 1, "Voice 2 delay multiplier" },
        { "ratio2", "Ratio 3", 0.05f, 8.0f, 0.66f, 0.01f, "", 1, "Voice 3 delay multiplier" },
        { "ratio3", "Ratio 4", 0.05f, 8.0f, 1.00f, 0.01f, "", 1, "Voice 4 delay multiplier" },
        { "ratio4", "Ratio 5", 0.05f, 8.0f, 1.11f, 0.01f, "", 1, "Voice 5 delay multiplier" },
        { "ratio5", "Ratio 6", 0.05f, 8.0f, 1.45f, 0.01f, "", 1, "Voice 6 delay multiplier" },
        { "ratio6", "Ratio 7", 0.05f, 8.0f, 2.22f, 0.01f, "", 1, "Voice 7 delay multiplier" },
        { "ratio7", "Ratio 8", 0.05f, 8.0f, 3.33f, 0.01f, "", 1, "Voice 8 delay multiplier" },
        { "manual",   "Manual Ratios", 0.0f, 1.0f, 0.0f, 1.0f, "", 1, "1 = use Ratio knobs" },
        { "scancurve","Scan Curve", 0.2f, 3.0f, 1.0f, 0.01f, "", 1, "Power curve on Scan" },
        { "modx",     "Mod X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "LFO depth scale" },
        { "apbasex",  "AP Base X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "All-pass base scale" },
        { "apphasex", "AP Phase X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Phase depth scale" },
        { "fbfloorx", "FB Floor X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Min feedback scale" },
        { "fbrangex", "FB Range X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Feedback range scale" },
        { "wetx",     "Wet X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Wet level scale" },
        { "dry",      "Dry", 0.0f, 1.0f, 0.12f, 0.001f, "", 1, "Dry mix after engine" },
        { "outgain",  "Out Gain", 0.0f, 8.0f, 4.5f, 0.01f, "", 1, "Output tanh gain" },
        { "spread",   "Spread", 0.0f, 0.6f, 0.12f, 0.001f, "", 1, "Per-comb character spread" },
        { "scanfade", "Scan Fade", 0.002f, 0.25f, 0.02f, 0.001f, "s", 1, "Mux crossfade time" },
        { "scanmode", "Scan Mode", 0.0f, 1.0f, 1.0f, 1.0f, "", 1, "0 = sweep, 1 = mux" },
        { "detune2",  "Detune 2", 0.90f, 1.10f, 1.007f, 0.001f, "", 1, "Slight scale of delay-2 tap (var2 character)" },
        P_END
    };

    var2::CombScannerVariantsDSP dsp;
    var2::CombScannerVariantsDSP::Parameters p;
};

class Variants3Engine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "var_3ctl", "variants 3 (ctrl)",
                 "other/zigcomb/comb_scanner_variants 3 more control",
                 "Current favourite tail. Sweep-scan. Two all-passes + Diffusion for a metallic decay. Cross Mix / Motion removed (dead). Damp=0 does not touch the tail." };
    }

    int numModels() const override { return kCombModelCount; }
    const char* modelName (int modelIndex) const override
    {
        return var3::CombScannerVariantsDSP::getVariantName (kCombModelMap[clampCombModel (modelIndex)]);
    }

    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }
    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        apply (index, value, p);
        dsp.setParameters (p);
    }

    void setModel (int modelIndex) override
    {
        p.variant = kCombModelMap[clampCombModel (modelIndex)];
        dsp.setParameters (p);
    }

    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

    template <typename P>
    static void apply (int index, float value, P& p)
    {
        switch (index)
        {
            case 0: p.gain = value; break;
            case 1: p.feedback = value; break;
            case 2: p.damp = value; break;
            case 3: p.phase = value; break;
            case 4: p.diffusion = value; break;
            case 5: p.delay1Ms = value; break;
            case 6: p.delay2Ms = value; break;
            case 7: p.scan = value; break;
            case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
                p.ratios[index - 8] = value; break;
            case 16: p.manualRatios = value; break;
            case 17: p.scanCurve = value; break;
            case 18: p.modX = value; break;
            case 19: p.apBaseX = value; break;
            case 20: p.apPhaseX = value; break;
            case 21: p.fbFloorX = value; break;
            case 22: p.fbRangeX = value; break;
            case 23: p.wetX = value; break;
            case 24: p.dry = value; break;
            case 25: p.outGain = value; break;
            case 26: p.spread = value; break;
            case 27: p.scanFade = value; break;
            case 28: p.scanMode = value; break;
            default: break;
        }
    }

    static constexpr ParamInfo table[] = {
        { "gain",      "Gain",      0.0f, 2.0f,    0.99f, 0.001f, "",   0, "Bank drive" },
        { "feedback",  "Feedback",  0.0f, 0.999f,  0.99f, 0.001f, "",   0, "Loop gain — tail length" },
        { "damp",      "Damp",      0.0f, 0.999f,  0.0f,  0.001f, "",   0, "0 = bypass, does not touch the tail" },
        { "phase",     "Phase",     0.0f, 1.0f,    0.0f,  0.001f, "",   0, "First all-pass in the loop (|H|=1)" },
        { "diffusion", "Diffusion", 0.0f, 1.0f,    0.35f, 0.001f, "",   0, "Second all-pass — metallic tail. 0 = off" },
        { "delay1",    "Delay 1",   0.0f, 1000.0f, 32.6f, 0.01f,  "ms", 0, "Primary comb time (ms)" },
        { "delay2",    "Delay 2",   0.0f, 1000.0f, 13.1f, 0.01f,  "ms", 0, "Parallel tap (ms)" },
        { "scan",      "Scan",      0.0f, 1.0f,    0.0f,  0.001f, "",   0, "Neighbour sweep between combs (this page)" },
        { "ratio0", "Ratio 1", 0.05f, 8.0f, 0.33f, 0.01f, "", 1, "Voice 1 delay multiplier" },
        { "ratio1", "Ratio 2", 0.05f, 8.0f, 0.50f, 0.01f, "", 1, "Voice 2 delay multiplier" },
        { "ratio2", "Ratio 3", 0.05f, 8.0f, 0.66f, 0.01f, "", 1, "Voice 3 delay multiplier" },
        { "ratio3", "Ratio 4", 0.05f, 8.0f, 1.00f, 0.01f, "", 1, "Voice 4 delay multiplier" },
        { "ratio4", "Ratio 5", 0.05f, 8.0f, 1.11f, 0.01f, "", 1, "Voice 5 delay multiplier" },
        { "ratio5", "Ratio 6", 0.05f, 8.0f, 1.45f, 0.01f, "", 1, "Voice 6 delay multiplier" },
        { "ratio6", "Ratio 7", 0.05f, 8.0f, 2.22f, 0.01f, "", 1, "Voice 7 delay multiplier" },
        { "ratio7", "Ratio 8", 0.05f, 8.0f, 3.33f, 0.01f, "", 1, "Voice 8 delay multiplier" },
        { "manual",   "Manual Ratios", 0.0f, 1.0f, 0.0f, 1.0f, "", 1, "1 = use Ratio knobs" },
        { "scancurve","Scan Curve", 0.2f, 3.0f, 1.0f, 0.01f, "", 1, "Power curve on Scan" },
        { "modx",     "Mod X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "LFO depth scale" },
        { "apbasex",  "AP Base X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "All-pass base scale" },
        { "apphasex", "AP Phase X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Phase depth scale" },
        { "fbfloorx", "FB Floor X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Min feedback scale" },
        { "fbrangex", "FB Range X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Feedback range scale" },
        { "wetx",     "Wet X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Wet level scale" },
        { "dry",      "Dry", 0.0f, 1.0f, 0.12f, 0.001f, "", 1, "Dry mix after engine" },
        { "outgain",  "Out Gain", 0.0f, 8.0f, 4.5f, 0.01f, "", 1, "Output tanh gain" },
        { "spread",   "Spread", 0.0f, 0.6f, 0.12f, 0.001f, "", 1, "Per-comb character spread" },
        { "scanfade", "Scan Fade", 0.002f, 0.25f, 0.02f, 0.001f, "s", 1, "Scan smoothing time" },
        { "scanmode", "Scan Mode", 0.0f, 1.0f, 0.0f, 1.0f, "", 1, "0 = sweep (this page), 1 = mux" },
        P_END
    };

    var3::CombScannerVariantsDSP dsp;
    var3::CombScannerVariantsDSP::Parameters p;
};

class Variants4Engine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "var_4mux", "variants 4 (mux)",
                 "var3 DSP + original mux scan",
                 "Split of current var3: same two-allpass tail, but Scan is a multiplexer (does not stretch time) like the original scheme." };
    }

    int numModels() const override { return kCombModelCount; }
    const char* modelName (int modelIndex) const override
    {
        return var4::CombScannerVariantsDSP::getVariantName (kCombModelMap[clampCombModel (modelIndex)]);
    }

    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }
    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        Variants3Engine::apply (index, value, p);
        dsp.setParameters (p);
    }

    void setModel (int modelIndex) override
    {
        p.variant = kCombModelMap[clampCombModel (modelIndex)];
        dsp.setParameters (p);
    }

    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

    static constexpr ParamInfo table[] = {
        { "gain",      "Gain",      0.0f, 2.0f,    0.99f, 0.001f, "",   0, "Bank drive" },
        { "feedback",  "Feedback",  0.0f, 0.999f,  0.99f, 0.001f, "",   0, "Loop gain — tail length" },
        { "damp",      "Damp",      0.0f, 0.999f,  0.0f,  0.001f, "",   0, "0 = bypass, does not touch the tail" },
        { "phase",     "Phase",     0.0f, 1.0f,    0.0f,  0.001f, "",   0, "First all-pass in the loop (|H|=1)" },
        { "diffusion", "Diffusion", 0.0f, 1.0f,    0.35f, 0.001f, "",   0, "Second all-pass — metallic tail. 0 = off" },
        { "delay1",    "Delay 1",   0.0f, 1000.0f, 32.6f, 0.01f,  "ms", 0, "Primary comb time (ms)" },
        { "delay2",    "Delay 2",   0.0f, 1000.0f, 13.1f, 0.01f,  "ms", 0, "Parallel tap (ms)" },
        { "scan",      "Scan",      0.0f, 1.0f,    0.0f,  0.001f, "",   0, "Multiplexer: picks a comb, does not stretch time" },
        { "ratio0", "Ratio 1", 0.05f, 8.0f, 0.33f, 0.01f, "", 1, "Voice 1 delay multiplier" },
        { "ratio1", "Ratio 2", 0.05f, 8.0f, 0.50f, 0.01f, "", 1, "Voice 2 delay multiplier" },
        { "ratio2", "Ratio 3", 0.05f, 8.0f, 0.66f, 0.01f, "", 1, "Voice 3 delay multiplier" },
        { "ratio3", "Ratio 4", 0.05f, 8.0f, 1.00f, 0.01f, "", 1, "Voice 4 delay multiplier" },
        { "ratio4", "Ratio 5", 0.05f, 8.0f, 1.11f, 0.01f, "", 1, "Voice 5 delay multiplier" },
        { "ratio5", "Ratio 6", 0.05f, 8.0f, 1.45f, 0.01f, "", 1, "Voice 6 delay multiplier" },
        { "ratio6", "Ratio 7", 0.05f, 8.0f, 2.22f, 0.01f, "", 1, "Voice 7 delay multiplier" },
        { "ratio7", "Ratio 8", 0.05f, 8.0f, 3.33f, 0.01f, "", 1, "Voice 8 delay multiplier" },
        { "manual",   "Manual Ratios", 0.0f, 1.0f, 0.0f, 1.0f, "", 1, "1 = use Ratio knobs" },
        { "scancurve","Scan Curve", 0.2f, 3.0f, 1.0f, 0.01f, "", 1, "Power curve on Scan" },
        { "modx",     "Mod X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "LFO depth scale" },
        { "apbasex",  "AP Base X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "All-pass base scale" },
        { "apphasex", "AP Phase X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Phase depth scale" },
        { "fbfloorx", "FB Floor X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Min feedback scale" },
        { "fbrangex", "FB Range X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Feedback range scale" },
        { "wetx",     "Wet X", 0.0f, 3.0f, 1.0f, 0.01f, "", 1, "Wet level scale" },
        { "dry",      "Dry", 0.0f, 1.0f, 0.12f, 0.001f, "", 1, "Dry mix after engine" },
        { "outgain",  "Out Gain", 0.0f, 8.0f, 4.5f, 0.01f, "", 1, "Output tanh gain" },
        { "spread",   "Spread", 0.0f, 0.6f, 0.12f, 0.001f, "", 1, "Per-comb character spread" },
        { "scanfade", "Scan Fade", 0.002f, 0.25f, 0.02f, 0.001f, "s", 1, "Mux crossfade time" },
        { "scanmode", "Scan Mode", 0.0f, 1.0f, 1.0f, 1.0f, "", 1, "0 = sweep, 1 = mux (this page)" },
        P_END
    };

    var4::CombScannerVariantsDSP dsp;
    var4::CombScannerVariantsDSP::Parameters p;
};

class CsMmEngine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "cs_mm", "cs_mm (20 models)",
                 "other/zigcomb/cs_mm",
                 "20 algorithms and a separate chorus layer. Unchanged from the pre-fix lab." };
    }

    int numModels() const override { return csmm::CsMmVariantsDSP::variantCount; }
    const char* modelName (int modelIndex) const override { return csmm::CsMmVariantsDSP::getVariantName (modelIndex); }
    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }
    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        switch (index)
        {
            case 0:  p.feedback = value; break;
            case 1:  p.damp = value; break;
            case 2:  p.phase = value; break;
            case 3:  p.diffusion = value; break;
            case 4:  p.crossMix = value; break;
            case 5:  p.motion = value; break;
            case 6:  p.chorusMix = value; break;
            case 7:  p.chorusDepth = value; break;
            case 8:  p.chorusRate = value; break;
            case 9:  p.delay1Ms = value; break;
            case 10: p.delay2Ms = value; break;
            case 11: p.scan = value; break;
            default: break;
        }
        dsp.setParameters (p);
    }

    void setModel (int modelIndex) override { p.variant = modelIndex; dsp.setParameters (p); }
    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

    static constexpr ParamInfo table[] = {
        { "feedback",  "Feedback",     0.0f, 0.999f,   0.62f,  0.001f, "",   0, nullptr },
        { "damp",      "Damp",         0.0f, 0.999f,   0.62f,  0.001f, "",   0, nullptr },
        { "phase",     "Phase",        0.0f, 1.0f,     0.35f,  0.001f, "",   0, nullptr },
        { "diffusion", "Diffusion",    0.0f, 1.0f,     0.45f,  0.001f, "",   0, nullptr },
        { "crossmix",  "Cross Mix",    0.0f, 1.0f,     0.50f,  0.001f, "",   0, nullptr },
        { "motion",    "Motion",       0.0f, 1.0f,     0.50f,  0.001f, "",   0, nullptr },
        { "chmix",     "Chorus Mix",   0.0f, 1.0f,     0.58f,  0.001f, "",   0, nullptr },
        { "chdepth",   "Chorus Depth", 0.0f, 1.0f,     0.60f,  0.001f, "",   0, nullptr },
        { "chrate",    "Chorus Rate",  0.0f, 1.0f,     0.35f,  0.001f, "",   0, nullptr },
        { "delay1",    "Delay 1",      0.0f, 2000.0f,  20.0f,  0.1f,   "ms", 0, nullptr },
        { "delay2",    "Delay 2",      0.0f, 2000.0f,  45.0f,  0.1f,   "ms", 0, nullptr },
        { "scan",      "Scan",         0.0f, 1.0f,     0.0f,   0.001f, "",   0, nullptr },
        P_END
    };

    csmm::CsMmVariantsDSP dsp;
    csmm::CsMmVariantsDSP::Parameters p;
};

class ZigZagEngine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "zigzag", "zigzag",
                 "other/zigcomb/zigzag_juce_package (both tries, identical files)",
                 "Separate branch: 4 delay lines + Hadamard matrix. Unchanged from the pre-fix lab." };
    }

    int numModels() const override { return 0; }
    const char* modelName (int) const override { return ""; }
    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }
    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        switch (index)
        {
            case 0: p.decay = value; break;
            case 1: p.damping = value; break;
            case 2: p.rotate = value; break;
            case 3: p.fluctuate = value; break;
            default: break;
        }
        dsp.setParameters (p);
    }

    void setModel (int) override {}
    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

    static constexpr ParamInfo table[] = {
        { "decay",     "Decay",     0.0f, 200.0f,  29.74f, 0.01f,   "", 0, nullptr },
        { "damping",   "Damping",   0.0f, 0.9839f, 0.10f,  0.0001f, "", 0, nullptr },
        { "rotate",    "Rotate",    0.0f, 1.0f,    0.25f,  0.001f,  "", 0, nullptr },
        { "fluctuate", "Fluctuate", 0.0f, 0.016f,  0.0f,   0.0001f, "", 0, nullptr },
        P_END
    };

    zzg::ZigZagDSP dsp;
    zzg::ZigZagDSP::Parameters p;
};

Engine* makeEngine (int index)
{
    switch (index)
    {
        case 0: return new Variants1FavEngine();
        case 1: return new Variants2Engine();
        case 2: return new Variants3Engine();
        case 3: return new Variants4Engine();
        case 4: return new CsMmEngine();
        case 5: return new ZigZagEngine();
        default: return nullptr;
    }
}

} // namespace

int numEngines() { return kNumSlots; }

Engine* createEngine (int index)
{
    if (index < 0 || index >= kNumSlots)
        return nullptr;
    return makeEngine (index);
}

namespace
{
int clampSlot (int slot) noexcept
{
    return slot < 0 ? 0 : (slot >= kNumSlots ? kNumSlots - 1 : slot);
}

const std::array<std::unique_ptr<Engine>, kNumSlots>& descriptorEngines()
{
    static const auto instances = []
    {
        std::array<std::unique_ptr<Engine>, kNumSlots> result;
        for (int i = 0; i < kNumSlots; ++i)
            result[(size_t) i].reset (makeEngine (i));
        return result;
    }();
    return instances;
}
} // namespace

int engineParamCount (int slot)
{
    auto* engine = descriptorEngines()[(size_t) clampSlot (slot)].get();
    return engine != nullptr ? engine->numParams() : 0;
}

const ParamInfo* engineParams (int slot)
{
    auto* engine = descriptorEngines()[(size_t) clampSlot (slot)].get();
    return engine != nullptr ? engine->params() : nullptr;
}

EngineInfo engineInfo (int slot)
{
    auto* engine = descriptorEngines()[(size_t) clampSlot (slot)].get();
    if (engine != nullptr)
        return engine->info();
    return { "none", "no page", "", "" };
}

int engineModelCount (int slot)
{
    auto* engine = descriptorEngines()[(size_t) clampSlot (slot)].get();
    return engine != nullptr ? engine->numModels() : 0;
}

const char* engineModelName (int slot, int modelIndex)
{
    auto* engine = descriptorEngines()[(size_t) clampSlot (slot)].get();
    return engine != nullptr ? engine->modelName (modelIndex) : "";
}

const char* slotParamId (int slot, const char* key)
{
    static std::string storage[kNumSlots][40];
    static std::string overflow[kNumSlots];

    if (slot < 0 || slot >= kNumSlots)
        slot = 0;

    const std::string wanted = "s" + std::to_string (slot) + "_" + key;

    for (int i = 0; i < 40; ++i)
    {
        if (storage[slot][i] == wanted)
            return storage[slot][i].c_str();
        if (storage[slot][i].empty())
        {
            storage[slot][i] = wanted;
            return storage[slot][i].c_str();
        }
    }

    overflow[slot] = wanted + "_" + std::to_string (storage[slot][0].size());
    return overflow[slot].c_str();
}

const char* slotModelId (int slot) { return slotParamId (slot, "model"); }
const char* slotTrimId  (int slot) { return slotParamId (slot, "trim"); }
}
