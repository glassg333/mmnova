#include "EngineRegistry.h"

#include "../engines/CombScannerVariants1Fav.h"
#include "../engines/CombScannerVariants2.h"
#include "../engines/CombScannerVariants3.h"
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

// --------------------------- variants 1 fav (BASE) -------------------------
// Оригинальный комб-сканер (модель 01), сверен с Max-схемой из
// other/zigcomb/zzz pic ref amxd original code scheme/cs ref
class Variants1FavEngine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "var_1fav", "variants 1 (fav)",
                 "other/zigcomb/comb_scanner_variants 1 fav",
                 "BASE REFERENCE: stock comb_scanner_variants 1 fav DSP (model 01 = original hypothesis). Matches the original Max scheme: 8 combs, ratios 0.33..3.33, base delay 115 ms, gain/damp/phase as in the patch. Scan = multiplexer crossfade between combs (no panning). Delay jumps are INSTANT (no smoothing) but click-free. Per-comb soft limiter = the peaklim~ from the scheme." };
    }

    // Оставлены только осмысленные модели: 01..04 и 06 Ping Pong (5,7..10 звучали одинаково/бесполезно).
    static constexpr int modelMap[] = { 0, 1, 2, 3, 5 };

    int numModels() const override { return (int) (sizeof (modelMap) / sizeof (modelMap[0])); }
    const char* modelName (int modelIndex) const override
    {
        return var1::CombScannerVariantsDSP::getVariantName (modelMap[clampModel (modelIndex)]);
    }

    static int clampModel (int m) noexcept { return m < 0 ? 0 : (m > 4 ? 4 : m); }

    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }

    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        switch (index)
        {
            case 0: p.gain = value; break;
            case 1: p.damp = value; break;
            case 2: p.phase = value; break;
            case 3: p.delay1Ms = value; break;
            case 4: p.delay2Ms = value; break;
            case 5: p.scan = value; break;
            default: break;
        }

        dsp.setParameters (p);
    }

    void setModel (int modelIndex) override
    {
        p.variant = modelMap[clampModel (modelIndex)];
        dsp.setParameters (p);
    }

    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

private:
    static constexpr ParamInfo table[] = {
        { "gain",   "Gain",    0.0f, 0.999f,   0.99f,  0.001f, ""   },
        { "damp",   "Damp",    0.0f, 0.999f,   0.90f,  0.001f, ""   },
        { "phase",  "Phase",   0.0f, 1.0f,     0.75f,  0.001f, ""   },
        { "delay1", "Delay 1", 0.0f, 2000.0f,  115.0f, 0.1f,   "ms" },
        { "delay2", "Delay 2", 0.0f, 2000.0f,  500.0f, 0.1f,   "ms" },
        { "scan",   "Scan",    0.0f, 1.0f,     0.0f,   0.001f, ""   },
        { nullptr,  nullptr,   0.0f, 0.0f,     0.0f,   0.0f,   nullptr }
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
                 "Same as variants 1, but Phase has a real effect (movement depth + slight detune of the 2nd line). Useful models only (01..04 + 06). No Character knob." };
    }

    // Оставлены только осмысленные модели: 01..04 и 06 Ping Pong (5,7..10 звучали одинаково/бесполезно).
    static constexpr int modelMap[] = { 0, 1, 2, 3, 5 };

    int numModels() const override { return (int) (sizeof (modelMap) / sizeof (modelMap[0])); }
    const char* modelName (int modelIndex) const override
    {
        return var2::CombScannerVariantsDSP::getVariantName (modelMap[clampModel (modelIndex)]);
    }

    static int clampModel (int m) noexcept { return m < 0 ? 0 : (m > 4 ? 4 : m); }

    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }

    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        switch (index)
        {
            case 0: p.gain = value; break;
            case 1: p.damp = value; break;
            case 2: p.phase = value; break;
            case 3: p.delay1Ms = value; break;
            case 4: p.delay2Ms = value; break;
            case 5: p.scan = value; break;
            default: break;
        }

        dsp.setParameters (p);
    }

    void setModel (int modelIndex) override
    {
        p.variant = modelMap[clampModel (modelIndex)];
        dsp.setParameters (p);
    }

    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

private:
    static constexpr ParamInfo table[] = {
        { "gain",      "Gain",      0.0f, 0.999f,   0.99f, 0.001f, ""   },
        { "damp",      "Damp",      0.0f, 0.999f,   0.90f, 0.001f, ""   },
        { "phase",     "Phase",     0.0f, 1.0f,     0.75f, 0.001f, ""   },
        { "delay1",    "Delay 1",   0.0f, 2000.0f,  115.0f, 0.1f,  "ms" },
        { "delay2",    "Delay 2",   0.0f, 2000.0f,  500.0f, 0.1f,  "ms" },
        { "scan",      "Scan",      0.0f, 1.0f,     0.0f,  0.001f, ""   },
        { nullptr,     nullptr,     0.0f, 0.0f,     0.0f,  0.0f,   nullptr }
    };

    var2::CombScannerVariantsDSP dsp;
    var2::CombScannerVariantsDSP::Parameters p;
};

// --------------------------- variants 3 more control -----------------------
class Variants3Engine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "var_3ctl", "variants 3 (ctrl)",
                 "other/zigcomb/comb_scanner_variants 3 more control",
                 "Feedback + Diffusion + Cross Mix + Motion. Useful models only (01..04 + 06)." };
    }

    // Оставлены только осмысленные модели: 01..04 и 06 Ping Pong (5,7..10 звучали одинаково/бесполезно).
    static constexpr int modelMap[] = { 0, 1, 2, 3, 5 };

    int numModels() const override { return (int) (sizeof (modelMap) / sizeof (modelMap[0])); }
    const char* modelName (int modelIndex) const override
    {
        return var3::CombScannerVariantsDSP::getVariantName (modelMap[clampModel (modelIndex)]);
    }

    static int clampModel (int m) noexcept { return m < 0 ? 0 : (m > 4 ? 4 : m); }

    const ParamInfo* params() const override { return table; }
    int numParams() const override { return countOf (table); }

    void prepare (double sr, int bs) override { dsp.prepare (sr, bs); }
    void reset() override { dsp.reset(); }

    void setParam (int index, float value) override
    {
        switch (index)
        {
            case 0: p.feedback = value; break;
            case 1: p.damp = value; break;
            case 2: p.phase = value; break;
            case 3: p.diffusion = value; break;
            case 4: p.crossMix = value; break;
            case 5: p.motion = value; break;
            case 6: p.delay1Ms = value; break;
            case 7: p.delay2Ms = value; break;
            case 8: p.scan = value; break;
            default: break;
        }

        dsp.setParameters (p);
    }

    void setModel (int modelIndex) override
    {
        p.variant = modelMap[clampModel (modelIndex)];
        dsp.setParameters (p);
    }

    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

private:
    static constexpr ParamInfo table[] = {
        { "feedback",  "Feedback",  0.0f, 0.999f,  0.99f, 0.001f, ""   },
        { "damp",      "Damp",      0.0f, 0.999f,  0.90f, 0.001f, ""   },
        { "phase",     "Phase",     0.0f, 1.0f,    0.75f, 0.001f, ""   },
        { "diffusion", "Diffusion", 0.0f, 1.0f,    0.55f, 0.001f, ""   },
        { "crossmix",  "Cross Mix", 0.0f, 1.0f,    0.50f, 0.001f, ""   },
        { "motion",    "Motion",    0.0f, 1.0f,    0.55f, 0.001f, ""   },
        { "delay1",    "Delay 1",   0.0f, 2000.0f, 115.0f, 0.1f,  "ms" },
        { "delay2",    "Delay 2",   0.0f, 2000.0f, 500.0f, 0.1f,  "ms" },
        { "scan",      "Scan",      0.0f, 1.0f,    0.0f,  0.001f, ""   },
        { nullptr,     nullptr,     0.0f, 0.0f,    0.0f,  0.0f,   nullptr }
    };

    var3::CombScannerVariantsDSP dsp;
    var3::CombScannerVariantsDSP::Parameters p;
};

// --------------------------- cs_mm -----------------------------------------
class CsMmEngine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "cs_mm", "cs_mm (20 models)",
                 "other/zigcomb/cs_mm",
                 "20 algorithms and a separate chorus layer. Defaults are now ~20 ms (fine and metallic at high feedback). Delays smoothed, scan does not pan." };
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

    void setModel (int modelIndex) override
    {
        p.variant = modelIndex;
        dsp.setParameters (p);
    }

    void process (float* const* channels, int numChannels, int numSamples) override
    {
        dsp.processBlock (channels, numChannels, numSamples);
    }

private:
    static constexpr ParamInfo table[] = {
        { "feedback",  "Feedback",     0.0f, 0.999f,   0.62f,  0.001f, ""   },
        { "damp",      "Damp",         0.0f, 0.999f,   0.62f,  0.001f, ""   },
        { "phase",     "Phase",        0.0f, 1.0f,     0.35f,  0.001f, ""   },
        { "diffusion", "Diffusion",    0.0f, 1.0f,     0.45f,  0.001f, ""   },
        { "crossmix",  "Cross Mix",    0.0f, 1.0f,     0.50f,  0.001f, ""   },
        { "motion",    "Motion",       0.0f, 1.0f,     0.50f,  0.001f, ""   },
        { "chmix",     "Chorus Mix",   0.0f, 1.0f,     0.58f,  0.001f, ""   },
        { "chdepth",   "Chorus Depth", 0.0f, 1.0f,     0.60f,  0.001f, ""   },
        { "chrate",    "Chorus Rate",  0.0f, 1.0f,     0.35f,  0.001f, ""   },
        { "delay1",    "Delay 1",      0.0f, 2000.0f,  20.0f,  0.1f,   "ms" },
        { "delay2",    "Delay 2",      0.0f, 2000.0f,  45.0f,  0.1f,   "ms" },
        { "scan",      "Scan",         0.0f, 1.0f,     0.0f,   0.001f, ""   },
        { nullptr,     nullptr,        0.0f, 0.0f,     0.0f,   0.0f,   nullptr }
    };

    csmm::CsMmVariantsDSP dsp;
    csmm::CsMmVariantsDSP::Parameters p;
};

// --------------------------- zigzag ----------------------------------------
class ZigZagEngine final : public Engine
{
public:
    EngineInfo info() const override
    {
        return { "zigzag", "zigzag",
                 "other/zigcomb/zigzag_juce_package (both tries, identical files)",
                 "Separate branch: 4 delay lines + Hadamard matrix from zigzag_1.maxpat. Damping goes to 0, Decay up to 200 (longer tail), Fluctuate range is 0..0.016 like in the reference patch (tiny drift, not chaos)." };
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

private:
    static constexpr ParamInfo table[] = {
        { "decay",     "Decay",     0.0f, 200.0f,  29.74f, 0.01f,   "" },
        { "damping",   "Damping",   0.0f, 0.9839f, 0.10f,  0.0001f, "" },
        { "rotate",    "Rotate",    0.0f, 1.0f,    0.25f,  0.001f,  "" },
        { "fluctuate", "Fluctuate", 0.0f, 0.016f,  0.0f,   0.0001f, "" },
        { nullptr,     nullptr,     0.0f, 0.0f,    0.0f,   0.0f,    nullptr }
    };

    zzg::ZigZagDSP dsp;
    zzg::ZigZagDSP::Parameters p;
};

Engine* makeEngine (int index)
{
    switch (index)
    {
        case 0: return new Variants1FavEngine();   // база: оригинальный комб-сканер (модель 01)
        case 1: return new Variants2Engine();
        case 2: return new Variants3Engine();
        case 3: return new CsMmEngine();
        case 4: return new ZigZagEngine();
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

// Ленивый набор «описательных» экземпляров: нужны только для таблиц параметров и имён моделей,
// DSP-буферы у них не выделяются (аллокации происходят в prepare()).
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
    // ID параметров должны жить столько же, сколько плагин, поэтому строки кэшируются.
    static std::string storage[kNumSlots][24];
    static std::string overflow[kNumSlots]; // на случай переполнения: свой ID, а не дубликат чужого

    if (slot < 0 || slot >= kNumSlots)
        slot = 0;

    const std::string wanted = "s" + std::to_string (slot) + "_" + key;

    for (int i = 0; i < 24; ++i)
    {
        if (storage[slot][i] == wanted)
            return storage[slot][i].c_str();

        if (storage[slot][i].empty())
        {
            storage[slot][i] = wanted;
            return storage[slot][i].c_str();
        }
    }

    // Сюда попасть не должны (максимум 14 параметров на слот), но если попадём —
    // вернём УНИКАЛЬНЫЙ id, чтобы не создать два параметра с одинаковым именем.
    overflow[slot] = wanted + "_" + std::to_string (storage[slot][0].size());
    return overflow[slot].c_str();
}

const char* slotModelId (int slot) { return slotParamId (slot, "model"); }
const char* slotTrimId  (int slot) { return slotParamId (slot, "trim"); }
}
