// ============================================================================
//  MnMVoice.h — голос kernel'а (256 слов состояния, Y:$528 + voice*$100).
//  Цепочка (доказана дизассемблированием, 06_docs/KERNEL_VOICE_CHAIN.md):
//     pre-voice P:$262  : portamento (2 slew) + pitch-LFO -> A48
//  -> jsr (r1)          : process машины, блок 16 кадров
//  -> post-voice P:$4A8 : AMP env (state machine 1/4/5/2) + LFO1..3 + SVF x2
//                         + панорама + Base/Width
// ============================================================================
#pragma once
#include "MnMMachine.h"
#include <memory>

namespace mmnova {

// ---------------------------------------------------------------- AMP env ---
// P:$4A8-$4F5. Уровень Y:$4FF, скорости из общей таблицы Y:$141800.
class AmpEnvelope
{
public:
    enum State { Idle = 0, Attack = 1, Release = 2, Decay = 4, Sustain = 5 };

    void noteOn ()  { state = Attack; }
    void noteOff()  { state = Release; }
    void reset   () { state = Idle; level = 0; }
    bool active () const { return state != Idle || level > 0; }

    // atk/dec/sus/rel — параметры патча 0..127 (страница AMP)
    s24 tick (int atk, int dec, int sus, int rel);

    s24  level { 0 };          // Y:$4FF
    int  state { Idle };
    bool ahdMode { false };    // AHD вместо ADSR (флаг страницы AMP)
};

// ----------------------------------------------------------------- LFO -----
// P:$506-$536. 3 LFO на голос, окно состояния V+$B4.
class KernelLfo
{
public:
    enum Shape { Tri = 0, Saw, Square, Sine, Exp, Ramp, Random };
    void  reset() { phase = 0; wrapped = false; sh = 0; }
    s24   tick (int speed7, int mult7, int shape, bool freeRun);
    bool  wrapped { false };            // флаг завершения цикла (r7-$2)
private:
    uint32_t phase { 0 };
    s24      sh    { 0 };               // sample&hold для Random
    uint32_t rng   { 0x1234567u };
};

// ----------------------------------------------------------------- SVF -----
// P:$5D3-$5E8 и P:$5EB-$5FA — две одинаковые петли. Коэффициенты прошивки
// $F528BD (= -0.084757) и $4A4DF0 (= +0.580505).
class SVF2
{
public:
    void  reset() { lp1 = bp1 = lp2 = bp2 = 0; }
    // base/width — параметры фильтра голоса (0..127): base = частота, width = полоса
    void  setCutoff (int base7, int width7, double rate);
    s24   process (s24 in);
private:
    s56 lp1 {0}, bp1 {0}, lp2 {0}, bp2 {0};
    s24 f {0x200000}, q {0x400000};
    bool bandpass { false };
};

// ---------------------------------------------------------------- Voice -----
class Voice
{
public:
    void prepare (double sampleRate, int voiceIndex);
    void setMachine (std::unique_ptr<Machine> m);
    Machine* machine() const { return mach.get(); }

    void noteOn  (int midiNote, int velocity);
    void noteOff ();
    bool isActive() const { return env.active(); }
    int  currentNote() const { return note; }

    // Рендер ровно kBlockFrames кадров. fxInput = выход соседнего трека (или nullptr).
    void renderBlock (s24* outL, s24* outR, const s24* fxInput);

    // --- страничные параметры (V-$28..V-$01), см. §9 отчёта ------------------
    struct Page
    {
        int amp[4]   { 0, 64, 100, 32 };    // ATK DEC SUS REL
        int pan      { 64 };
        int volume   { 100 };
        int base     { 127 };               // Base/Width-фильтр
        int width    { 127 };
        int portamento { 0 };
        int lfoSpeed[3]  { 40, 40, 40 };
        int lfoMult[3]   { 64, 64, 64 };
        int lfoShape[3]  { 0, 0, 0 };
        int lfoDepth[3]  { 0, 0, 0 };
        int lfoDestParam[3] { -1, -1, -1 }; // -1 = питч
    } page;

private:
    // pre-voice P:$262: два slew-лимитера + модуляция питча
    s56 preVoice();

    std::unique_ptr<Machine> mach;
    AmpEnvelope env;
    KernelLfo   lfo[3];
    SVF2        svf;

    double rate { kEngineRate };
    int    vIndex { 0 };
    int    note { 60 };
    int    vel  { 100 };
    bool   trig { false };

    // аккумуляторы питча (P:$295-$2EA): accum1/accum2 + цели
    int32_t accum1 { 0 }, accum2 { 0 }, target1 { 0 }, target2 { 0 };

    s24 blockBuf[kBlockFrames] { 0 };
};

} // namespace mmnova
