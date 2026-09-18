#pragma once
#include "MnMFixed.h"
#include <cmath>
#include <algorithm>

// ============================================================================
//  MnMVoiceChain_Canonical.h — КАНОНИЧЕСКИЙ РОУТИНГ ГОЛОСА OS 1.32B
//  Подтверждено: слуховой тест + CHANGELOG monomachine-lab 0.1.4 + ColdFire
//
//  Цепочка:
//    1. SYNTH OSC (16 кадров @ 44.1 кГц)
//    2. DIST      (AMP page, ручка 5: headroom scale + сатурация SR.SM bset #$14,sr)
//    3. SRR       (EFFX page, ручка 3: Sample Rate Reduction, дециматор)
//    4. FILT      (FILT page: 24 dB SVF HP + LP, независимые HPQ/LPQ + модулятор)
//    5. EQ        (EFFX page: 1-полосный parametric EQF/EQG)
//    6. AMP ENV   (AMP page: AHDR огибающая + VOL + PAN)
//    7. DELAY     (EFFX page: DSND посыл в кольцевой буфер X:$114000)
// ============================================================================

namespace mmnova {

class CanonicalVoiceChain
{
public:
    struct Params
    {
        // AMP page (CC 56..63)
        int atk  = 0;    // CC 56
        int hold = 16;   // CC 57
        int dec  = 64;   // CC 58
        int rel  = 32;   // CC 59
        int dist = 0;    // CC 60: -64..+63 (0 = 0 dB headroom)
        int vol  = 100;  // CC 61: 0..127
        int pan  = 0;    // CC 62: -64..+63

        // FILT page (CC 72..79)
        int base = 64;   // CC 72: HP cutoff
        int wdth = 64;   // CC 73: LP cutoff offset
        int hpq  = 32;   // CC 74: HP resonance
        int lpq  = 32;   // CC 75: LP resonance
        int fAtk = 0;    // CC 76: filter env attack
        int fDec = 64;   // CC 77: filter env decay
        int bofs = 0;    // CC 78: band offset (-64..+63)
        int wofs = 0;    // CC 79: width offset (-64..+63)

        // EFFX page (CC 80..87)
        int eqf  = 64;   // CC 80: EQ frequency
        int eqg  = 0;    // CC 81: EQ gain (-64..+63)
        int srr  = 0;    // CC 82: Sample Rate Reduction (0..127)
        int dsnd = 0;    // CC 84: Delay send (0..127)
    };

    void reset() noexcept
    {
        srrAcc = 0.0; srrHold = 0;
        hpLo = hpBp = lpLo = lpBp = 0.0;
        eqZ1 = eqZ2 = 0.0;
        ampLvl = 0; heldBlocks = 0; ampState = 0;
        filtEnv = 0.0; filtAttacking = true;
    }

    void noteOn() noexcept
    {
        ampState = 1; // Attack
        ampLvl = 0;
        heldBlocks = 0;
        filtEnv = 0.0;
        filtAttacking = true;
    }

    void noteOff() noexcept
    {
        ampState = 2; // Release
    }

    // Обработка ровно 16 кадров голоса (do #<$10)
    void processBlock16 (s24* synthAudio, s24* outL, s24* outR, s24* sendToDelay,
                         const Params& p, double sr = 44100.0) noexcept
    {
        // 1. DIST (headroom multiplier + жесткий клип SR.SM bset #$14,sr)
        const double distGain = (p.dist == 0) ? 1.0 : std::pow (2.0, (double) p.dist / 16.0);
        const s24 distScale = fromFloat (std::clamp (distGain, 0.0, 7.999));

        // 2. SRR step (шаг децимации)
        const double srrStep = (p.srr <= 0) ? 1.0 : std::pow (2.0, -7.0 * (p.srr / 127.0));

        // 3. FILT ENV tick (AD огибающая фильтра на блок из 16 сэмплов)
        if (filtAttacking) {
            filtEnv += 1.0 / (1.0 + p.fAtk * 8.0);
            if (filtEnv >= 1.0) { filtEnv = 1.0; filtAttacking = false; }
        } else {
            filtEnv -= 1.0 / (1.0 + p.fDec * 8.0);
            if (filtEnv < 0.0) filtEnv = 0.0;
        }

        // Модуляция BASE и WIDTH
        const double bofsNorm = (double) p.bofs / 64.0;
        const double wofsNorm = (double) p.wofs / 64.0;
        const int baseMod = std::clamp ((int) std::lround (p.base + filtEnv * bofsNorm * 64.0), 0, 127);
        const int wdthMod = std::clamp ((int) std::lround (p.wdth + filtEnv * wofsNorm * 64.0), 0, 127);

        // Расчёт частот среза HP и LP
        const double fHp = cutoffCoeff (baseMod, sr);
        const double fLp = cutoffCoeff (std::min (127, baseMod + wdthMod), sr);
        // Зашитые константы SVF P:$05D3 ($F528BD = -0.084694564)
        const double kA = -0.084694564;
        const double qHp = std::clamp (2.0 + kA - (p.hpq / 127.0) * 1.85, 0.05, 2.0);
        const double qLp = std::clamp (2.0 + kA - (p.lpq / 127.0) * 1.85, 0.05, 2.0);

        // 4. EQ коэфф (1-полосный peaking, EFFX page)
        double eqB0 = 1, eqB1 = 0, eqB2 = 0, eqA1 = 0, eqA2 = 0;
        calc1BandEq (p.eqf, p.eqg, sr, eqB0, eqB1, eqB2, eqA1, eqA2);

        // 5. AMP ENV тик на блок
        const s24 curAmpGain = tickAmpEnv (p);

        // Панорама и громкость
        const double pn = std::clamp ((double)(p.pan + 64) / 128.0, 0.0, 1.0);
        const s24 gL = fromFloat (std::cos (pn * 1.5707963267948966));
        const s24 gR = fromFloat (std::sin (pn * 1.5707963267948966));
        const s24 volGain = fmul (paramWord (p.vol), curAmpGain);
        const s24 dsndGain = paramWord (p.dsnd);

        for (int i = 0; i < 16; ++i)
        {
            s24 s = synthAudio[i];

            // --- ШАГ 1: DIST (headroom + SR.SM сатурация) ---
            s = sat24 (((s56) s * (s56) distScale) >> 23);

            // --- ШАГ 2: SRR (Sample Rate Reduction децимация) ---
            if (p.srr > 0) {
                srrAcc += srrStep;
                if (srrAcc >= 1.0) { srrAcc -= 1.0; srrHold = s; }
                s = srrHold;
            }

            // --- ШАГ 3: FILT (24dB multi-mode: HP секция + LP секция) ---
            double x = toFloat (s);
            // HP SVF (P:$05D3)
            hpLo += fHp * hpBp;
            const double hpHi = x - hpLo - qHp * hpBp;
            hpBp += fHp * hpHi;
            hpBp  = std::clamp (hpBp, -1.6, 1.6);
            x = hpHi; // HP выход
            // LP SVF (P:$05EB)
            lpLo += fLp * lpBp;
            const double lpHi = x - lpLo - qLp * lpBp;
            lpBp += fLp * lpHi;
            lpBp  = std::clamp (lpBp, -1.6, 1.6);
            x = lpLo; // LP выход

            // --- ШАГ 4: EQ (после фильтра!) ---
            const double eqOut = eqB0 * x + eqB1 * eqZ1 + eqB2 * eqZ2 - eqA1 * eqZ1 - eqA2 * eqZ2;
            eqZ2 = eqZ1; eqZ1 = x;
            s = fromFloat (std::clamp (eqOut, -1.0, 1.0));

            // --- ШАГ 5: AMP ENV (огибающая глушит сигнал) ---
            s = fmul (s, volGain);

            outL[i] = sat24 ((s56) outL[i] + fmul (s, gL));
            outR[i] = sat24 ((s56) outR[i] + fmul (s, gR));

            // --- ШАГ 6: DELAY SEND (посыл уходит после огибающей) ---
            if (sendToDelay)
                sendToDelay[i] = fmul (s, dsndGain);
        }
    }

private:
    static double cutoffCoeff (int knob7, double sr) noexcept {
        const double n = std::clamp (knob7, 0, 127) / 127.0;
        const double hz = 20.0 * std::pow (1000.0, n);
        return std::clamp (2.0 * std::sin (3.14159265358979 * std::min (hz, sr * 0.45) / sr), 0.002, 0.98);
    }

    static void calc1BandEq (int eqf7, int eqgBi, double sr,
                            double& b0, double& b1, double& b2, double& a1, double& a2) noexcept
    {
        if (eqgBi == 0) { b0 = 1; b1 = b2 = a1 = a2 = 0; return; }
        const double f0 = 40.0 * std::pow (300.0, (double) eqf7 / 127.0);
        const double gDb = ((double) eqgBi / 64.0) * 12.0; // +-12 дБ
        const double A = std::pow (10.0, gDb / 40.0);
        const double w0 = 6.28318530718 * std::min (f0, sr * 0.45) / sr;
        const double alpha = std::sin (w0) * 0.5; // Q ~ 1.0
        const double cosw0 = std::cos (w0);
        const double a0 = 1.0 + alpha / A;
        b0 = (1.0 + alpha * A) / a0;
        b1 = (-2.0 * cosw0) / a0;
        b2 = (1.0 - alpha * A) / a0;
        a1 = (-2.0 * cosw0) / a0;
        a2 = (1.0 - alpha / A) / a0;
    }

    s24 tickAmpEnv (const Params& p) noexcept
    {
        switch (ampState) {
            case 1: // Attack
                ampLvl += std::max (1, (int)(kOne24 / (1 + p.atk * 16)));
                if (ampLvl >= kOne24) { ampLvl = kOne24; ampState = 3; heldBlocks = 0; }
                break;
            case 3: // Hold
                if (++heldBlocks >= (p.hold * 8)) ampState = 4;
                break;
            case 4: // Decay
                ampLvl -= std::max (1, (int)(ampLvl / (1 + p.dec * 12)));
                if (ampLvl <= 1024) { ampLvl = 0; ampState = 0; }
                break;
            case 2: // Release
                ampLvl -= std::max (1, (int)(ampLvl / (1 + p.rel * 8)));
                if (ampLvl <= 1024) { ampLvl = 0; ampState = 0; }
                break;
            default:
                ampLvl = 0;
                break;
        }
        return ampLvl;
    }

    double srrAcc { 0.0 }; s24 srrHold { 0 };
    double hpLo { 0 }, hpBp { 0 }, lpLo { 0 }, lpBp { 0 };
    double eqZ1 { 0 }, eqZ2 { 0 };
    s24 ampLvl { 0 }; int heldBlocks { 0 }, ampState { 0 };
    double filtEnv { 0 }; bool filtAttacking { true };
};

} // namespace mmnova
