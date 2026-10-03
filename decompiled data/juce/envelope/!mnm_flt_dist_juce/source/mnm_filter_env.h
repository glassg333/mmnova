// ============================================================================
// mnm_filter_env.h — ОГИБАЮЩАЯ ФИЛЬТРА (FLT ATK/DEC), Monomachine OS 1.32B
// ----------------------------------------------------------------------------
// Декомпиляция P:$0506-$0536 ядра DSP1 (func_0004A8, per-voice, тик раз в блок
// 16 сэмплов). Выписка листинга: reference/kernel_P04A8-0536_amp_env_filterenv.txt
//
// Роли регистров голоса (r6 = V-$28, r7 = V+$B4):
//   V-$1C (r6+$C) — FILT ATTACK  : индекс таблицы kEnvShapeA (P:$141800)   [P:$0509]
//   V-$1B (r6+$D) — FILT DECAY   : индекс таблицы kEnvShapeE (P:$141A00)   [P:$052B]
//   X:(V+$B3) (r7-$1) — фаза огибающей 0..1 (X-сторона)
//   Y:(V+$B3) (r7-$1) — состояние машины (0=ATK, 1=промежуточное, 2=DEC)
//   X:(V+$B2) (r7-$2) — счётчик/флаг перехода
//   триггер: Y:(V-$08)==1 → сброс состояния и фазы [P:$04FF-$0505]
//
// Машина состояний (транскрипция инструкций):
//   state 0 (ATK)   [P:$0509-$0516]:
//       phase += kEnvShapeA[(V-$1C)>>16]        ; P:$050D y:(r4+$141800)
//       при переносе (переполнение до 1.0):     ; P:$0510 bec
//           state = 1, flag(X:(V+$B2)) = 1      ; P:$0511-$0514
//   state 1        [P:$0517-$052A]: одноблочный переход (транскрибирован дословно;
//       декодированное поведение: один блок, затем state=2)
//   state 2 (DEC)  [P:$052B-$0536]:
//       idx = (V-$1B + $7FFF) >> 16, округление [P:$052C-$052F rnd]
//       phase -= kEnvShapeE[idx]                ; P:$0532 y:(r4+$141a00)
//       phase = max(0, phase)                   ; P:$0535 clr b ifmi
//
// ПРИМЕЧАНИЕ ПРО ИСТОРИЮ: в итерациях ≤14 пакета filter_phaser_pack эту машину
// атрибутировали как "LFO", а итерация 15 вообще объявила "у фильтра нет
// огибающей" (ошибочно переатрибутировав P+$14-$17 delay-модуляции). ERRATA
// (decompiled data/juce/distortion/CORE_VERIFIED docs) возвращает: V-$1C/V-$1B =
// FilterAttack/FilterDecay, V-$1A/V-$19 = BOFS/WOFS (биполярная центровка
// |param + $C00000|·$700, P:$053E-$054B) — что совпадает с мануалом OS 1.32
// (FILTER ENVELOPE: ATK/DEC/BOFS/WOFS) и с описанием на реальной машине.
// ============================================================================
#pragma once
#include "mnm_fixed.h"
#include "mnm_tables_data.h"

namespace mnm {

class MnmFilterEnv
{
public:
    // atk/dec: значения страниц 0..127 (будут записаны как word = knob<<16)
    void setParams(uint32_t atkWord, uint32_t decWord) noexcept
    {
        atkWord_ = atkWord & 0xFFFFFFu;
        decWord_ = decWord & 0xFFFFFFu;
    }

    // P:$04FF-$0505: по триггеру ноты state и фаза сбрасываются в 0
    void trigger() noexcept
    {
        state_ = 0;
        phase_ = 0;       // x:(r7-$1)
        flag_  = 0;       // x:(r7-$2)
    }

    void release() noexcept
    {
        // Ядро не имеет отдельной REL-ветки у фильтровой огибающей:
        // DEC-ветка (state 2) исполняется всегда после атаки. При выключенной
        // ноте фаза продолжает спад по kEnvShapeE — оставляем как есть.
    }

    // фаза 0..1 (float) — вход в модуляцию среза (P:$0547/P:$0565)
    float phase() const noexcept { return q23ToF(phase_ & 0xFFFFFFu); }
    bool  running() const noexcept { return state_ != 2 || phase_ != 0; }

    // тик раз в блок 16 сэмплов (сетка ядра)
    void tickBlock() noexcept
    {
        switch (state_)
        {
        case 0: { // ATTACK: phase += kEnvShapeA[(atk)>>16]  [P:$0509-$0516]
            const uint32_t idx = (atkWord_ >> 16) & 0x7Fu;
            const int64_t sum = (int64_t)(phase_ & 0xFFFFFFu)
                              + (int64_t)(kEnvShapeA[idx] & 0xFFFFFFu);
            if (sum >= (1ll << 24))            // bec: перенос из 24-бит дроби
            {
                state_ = 1;                    // P:$0513 y:(r7-$1)=1
                flag_  = 1;                    // P:$0514 x:(r7-$2)=1
                phase_ = 0xFFFFFFu;            // насыщение до 1.0-eps
            }
            else phase_ = (uint32_t)sum & 0xFFFFFFu;
            break; }
        case 1: { // одноблочный переход [P:$0517-$052A, транскрипция дословно]
            // $0519 clr a; $051A y0=V-$05; $051B x0=0; $051C b=y0*x0=0
            // $051D a = flag; $051F b = 0*0.9469 = 0; $0521 asl b = 0
            // $0522 a = flag+1; $0523 cmp b(0),a; $0524 blt — не берётся (a>=0)
            // => $0525-527: state = 2.  Счётчик не сохраняется.
            state_ = 2;
            break; }
        case 2: { // DEC: phase = max(0, phase - kEnvShapeE[(dec+$7FFF)>>16])
            const uint32_t dec = (decWord_ + 0x7FFFu) & 0xFFFFFFu;
            const uint32_t idx = (dec >> 16) & 0x7Fu;   // asr #$10 + rnd [P:$052E-52F]
            const int64_t d = (int64_t)(phase_ & 0xFFFFFFu)
                            - (int64_t)(kEnvShapeE[idx] & 0xFFFFFFu);
            phase_ = (d <= 0) ? 0u : (uint32_t)d & 0xFFFFFFu;  // clr b ifmi
            break; }
        default: break;
        }
    }

    void reset() noexcept { state_ = 0; phase_ = 0; flag_ = 0; }

private:
    uint32_t atkWord_ = 0, decWord_ = (64u << 16);
    uint32_t phase_ = 0;
    int      state_ = 0;
    uint32_t flag_  = 0;
};

} // namespace mnm
