// ============================================================================
//  MnMMachine.h — база машины Monomachine.
//  Соответствует записи диспетчера: init / config / process
//  (таблицы X:$10016B / X:$10018D / X:$1001AF, по 34 входа).
//  Контракт железа:
//    * 8 параметров патча лежат в Y:(R6+$4)..Y:(R6+$B) как (value7 << 16);
//    * process вызывается блоками по 16 кадров (do #<$10) на 44100 Гц;
//    * на входе A48 = экспоненциальный фазовый инкремент от pre-voice (P:$2EA);
//    * FX-машины читают вход из буфера соседнего трека (биты X:$2C4 = INP).
// ============================================================================
#pragma once
#include "MnMTables.h"

namespace mmnova {

constexpr int kBlockFrames = 16;    // do #<$10
constexpr int kNumParams   = 8;
constexpr double kEngineRate = 44100.0;

// Идентификаторы слотов = порядок в таблице диспетчеризации (m1..m33)
enum class MachineId : int
{
    GND_GND = 0, GND_SIN, GND_NOIS, SID_6581, SWAVE_SAW, SWAVE_PULS,
    DPRO_WAVE, DPRO_BBOX, FM_STAT, FM_PAR, FM_DYN, VO_6,
    FX_THRU, FX_REVERB, SWAVE_ENS, FX_CHORUS, FX_DYNAMIX, FX_RINGMOD,
    FX_PHASER, FX_FLANGER, DPRO_DDRW, DPRO_DENS, FX_DLY, FX_EXT,
    kCount
};

// Контекст блока: то, что kernel кладёт в регистры перед jsr (r1)
struct BlockContext
{
    s56      pitchInc   { 0 };   // A48 из P:$2EA (экспоненциальный инкремент фазы)
    const s24* input    { nullptr }; // r0 — буфер соседнего трека (для FX)
    int      voiceIndex { 0 };   // выбор области delay-буфера X:$114000 + v*$2000
    bool     trigger    { false };  // Y:(V-$08) == 1 — note-on в этом блоке
    double   rate       { kEngineRate };
};

class Machine
{
public:
    virtual ~Machine() = default;

    // --- диспетчер -----------------------------------------------------------
    virtual void init   (int voiceIndex) = 0;               // запись init-таблицы
    virtual void config () {}                               // запись config-таблицы
    virtual void process (s24* out, const BlockContext& c) = 0;  // 16 кадров

    virtual MachineId  id()   const = 0;
    virtual const char* name() const = 0;

    // --- параметры Y:(R6+$4..$B) ---------------------------------------------
    void setParam (int idx, int value7) noexcept
    {
        if ((unsigned) idx < kNumParams)
        {
            p[idx] = paramWord (value7);
            dirty = true;
        }
    }
    int  getParam (int idx) const noexcept { return paramIndex (p[idx]); }
    void setAllParams (const int* v7) noexcept
    {
        for (int i = 0; i < kNumParams; ++i) p[i] = paramWord (v7[i]);
        dirty = true;
    }

    // Общая для всех машин обработка «конфиг изменился»
    void tickConfig() { if (dirty) { config(); dirty = false; } }

protected:
    s24  p[kNumParams] { 0,0,0,0,0,0,0,0 };   // R6+$4 .. R6+$B
    bool dirty { true };

    // --- сервис, повторяющий паттерны прошивки --------------------------------

    // Вход FX-машины: input * INP^2 * 4   ($14768B-$147690, asl #$2).
    // INP = $40 -> ровно x1.0. Линейный INP ломает насыщение — не упрощать.
    static inline s24 fxInput (s24 in, s24 inpWord) noexcept
    {
        const s24 inp = (s24) (inpWord >> 0);           // param<<16, макс $7F0000
        const s24 sq  = fmul (inp, inp);                // INP^2
        return sat24 (((s56) fmul (in, sq)) << 2);      // *4
    }

    // Финальный микс любой машины: out = dry*($7FFFFF - MIX) + wet*MIX
    // При MIX=127 остаётся 0.78 % сухого — оригинальное поведение прошивки.
    static inline s24 fxMix (s24 dry, s24 wet, s24 mixWord) noexcept
    {
        const s24 mix = mixWord;
        const s24 inv = sat24 ((s56) kOne24 - mix);
        s56 a = mpy (dry, inv);
        mac (a, wet, mix);
        return sat24 (a >> 24);
    }

    // inline-LFO FX-машины: аккумулятор фазы + синус $14A000, SPD = y:(r6+$6)
    struct InlineLfo
    {
        uint64_t phase { 0 };
        inline s24 tick (s24 spdWord) noexcept
        {
            phase += (uint64_t) (uint32_t) tables::rateFromParam (spdWord) << 8;
            return lerpTable<tables::kSineBits> (tables::sine(), phase);
        }
        inline void reset() noexcept { phase = 0; }
    };
};

} // namespace mmnova
