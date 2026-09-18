// ============================================================================
// MnMVoiceChain.h — СКВОЗНАЯ post-voice цепочка (то, чего не хватало)
//                    Monomachine SFX-60 MKII OS 1.32B
// ============================================================================
// Порядок стадии подтверждён кодом ядра func_0004A8 (P:$04A8-$0B4C):
//
//   [машина, 16 кадров] -> AMP env (вычисляется блочно) -> DIST-каскад
//     -> путь коэффициента фильтра (с щвязкой c(DIST) -> срез)
//     -> HP-SVF -> LP-SVF -> пан/объём (таблицы P:$143C06/$143D06)
//     -> пер-сэмпловые рампы усиления (4-tap FIR интерполятор состояний)
//     -> буфер трека (добавление/перезапись по флагам X:$2C4)
//
// Огибающая AMP и фильтра — блочные (тик раз в 16 кадров). DIST и фильтр
// работают по-сэмплно в 24-битной Q1.23 семантике с насыщением.
//
// Финальное умножение на огибающую идёт ПОСЛЕ фильтра (глушит хвост резонанса).
// ============================================================================
#pragma once
#include "MnmFixed.h"
#include "MnMAmpDist.h"
#include "MnMFilter24.h"

namespace mnmsys {

struct MnmVoiceParams
{
    // AMP-страница (V-$28..V-$21): значения страниц Q1.23 как float 0..1
    float ampAtk = 0, ampHold = 0, ampDec = 0, ampRel = 0;
    float dist   = 0.5f;            // V-$24: рабочая зона прошивки 0.5..1.0
    float vol    = 1.0f;            // V-$23
    float pan    = 0.5f;            // V-$22: 0.5 = центр
    // FILT-страница (V-$20..V-$19)
    float fBase = 0, fWdth = 0, fHpq = 0, fLpq = 0;
    float fAtk = 0, fDec = 0, fBofs = 0.5f, fWofs = 0.5f;
    bool  lpEnabled = true;         // бит 0 из Y:(V-$19)
};

class MnMVoiceChain
{
public:
    void prepare() noexcept { reset(); }
    void reset() noexcept
    {
        dist_.reset(); filt_.reset();
        envState_ = 0; envLevel_ = 0; holdCnt_ = 0;
    }

    void setParams(const MnmVoiceParams& p) noexcept
    {
        p_ = p;
        dist_.setDist(p.dist);
        filt_.setPages(p.fBase, p.fWdth, p.fHpq, p.fLpq, p.fAtk, p.fDec, p.fBofs, p.fWofs);
        amp_.setParams(int(p.ampAtk * 127.0f), int(p.ampHold * 127.0f),
                       int(p.ampDec * 127.0f), int(p.ampRel * 127.0f));
    }

    void noteOn()  noexcept { envState_ = 1; envLevel_ = 0; holdCnt_ = 0; filt_.trigger(); }
    void noteOff() noexcept { if (envState_) envState_ = 2; }

    // Один блок 16 кадров. in — выход машины (или вход трека для FX-вставок).
    void processBlock16(const float* in, float* outL, float* outR, int n) noexcept
    {
        // 1) AMP env тик (блочно, P:$04A8-$04F5)
        tickAmpEnv();

        // 2) DIST-каскад по-сэмплно (P:$07A6-$07D1) + захват кривой привода
        float tmp[16];
        const float c = dist_.processBlock16(in, tmp, n);
        filt_.setDriveCurve(c);

        // 3) Фильтр (P:$0537-$05FA): env фильтра тикнет внутри
        float flt[16];
        filt_.processBlock16(tmp, flt, n, p_.lpEnabled);

        // 4) Финальное умножение на огибающую AMP (после фильтра, P:$0B33-...)
        //    + пан по таблицам прошивки (P:$143C06/$143D06, idx = pan*$FF)
        const float env = q23ToF(uint32_t(envLevel_) & 0xFFFFFF);
        // Пан-закон: в ядре пан применяется в финальном миксе через ROM-таблицы;
        // точный закон — OPEN (см. VERIFICATION_REPORT §5). Здесь равномощный
        // закон (как у FX WID), чтобы центр = −3дБ на канал.
        const float ph = p_.pan * 1.5707963f;
        const float gl = std::cos(ph);
        const float gr = std::sin(ph);
        const float v   = p_.vol;

        for (int i = 0; i < n; ++i)
        {
            const s24 x = fToQ23(flt[i]);
            // пер-сэмпл: произведение с env в 24-битном домене (насыщение АЛУ)
            const s24 e = fmpy(x, fToQ23(env));
            const s24 g = fmpy(e, fToQ23(v));
            outL[i] = q23ToF(uint32_t(fmpy(g, fToQ23(gl))) & 0xFFFFFF);
            outR[i] = q23ToF(uint32_t(fmpy(g, fToQ23(gr))) & 0xFFFFFF);
        }
    }

    float distC() const noexcept { return dist_.c(); }
    float envLevel() const noexcept { return q23ToF(uint32_t(envLevel_) & 0xFFFFFF); }

private:
    // Точная машина огибающей (см. MnMAmpDist.h / VERIFICATION_REPORT §1.2)
    void tickAmpEnv() noexcept
    {
        const int atk = int(p_.ampAtk * 127.0f) & 0x7F;
        const int dec = int(p_.ampDec * 127.0f) & 0x7F;
        const int rel = int(p_.ampRel * 127.0f) & 0x7F;
        const int hold = int(p_.ampHold * 127.0f) & 0x7F;

        switch (envState_)
        {
        case 1: {
            const s24 inc = s24(kAmpRateTable128[atk] & 0xFFFFFF);
            const int64_t sum = int64_t(envLevel_) + inc;
            if (sum >= Q23_ONE) { envLevel_ = Q23_ONE; envState_ = 3; holdCnt_ = 0; }
            else envLevel_ = s24(sum);
            break; }
        case 3:
            if (++holdCnt_ >= hold) envState_ = 4;
            break;
        case 4: {
            const s24 m = s24(kAmpDecayTable128[dec] & 0xFFFFFF);
            const s24 next = sat24((int64_t(envLevel_) * int64_t(m < 0 ? -m : m)) >> 22);
            // сустейн = (V-$0E)^2: страница V-$0E хранит sqrt(sustain);
            // в этой модели sustain передаётся через vol-подобный параметр не нужен —
            // прошивка держит уровень до noteOff, пол = 0 при DEC->REL переходе.
            if (next <= 0 || envLevel_ <= sustain_) { envLevel_ = sustain_; envState_ = 5; }
            else envLevel_ = next;
            break; }
        case 5:
            envLevel_ = sustain_;
            break;
        case 2: {
            const s24 m = s24(kAmpDecayTable128[rel] & 0xFFFFFF);
            envLevel_ = sat24((int64_t(envLevel_) * int64_t(m < 0 ? -m : m)) >> 22);
            break; }
        default: break;
        }
    }

public:
    void setSustain(float sus) noexcept   // (V-$0E)^2, подайте sqrt(sus)
    {
        const s24 s = fToQ23(std::sqrt(std::clamp(sus, 0.0f, 1.0f)));
        sustain_ = fmpy(s, s);
    }

private:
    MnmVoiceParams p_;
    MnMDist      dist_;
    MnMFilter24  filt_;
    MnMAmpEnv    amp_;      // справочный EXACT-класс (используется в тестах)
    int  envState_ = 0;     // 0 idle, 1 ATK, 2 REL, 3 HOLD, 4 DEC, 5 SUS
    s24  envLevel_ = 0, sustain_ = 0;
    int  holdCnt_ = 0;
};

} // namespace mnmsys
