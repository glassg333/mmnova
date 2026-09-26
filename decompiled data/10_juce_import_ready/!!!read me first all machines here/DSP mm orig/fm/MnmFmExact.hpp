// MnmFmExact.hpp — режим "ex" для FM-машин m8 FM+STAT / m9 FM+PAR / m10 FM+DYN.
// Точные законы ручек OS 1.32B (итерации 27–28, верифицированы на бит-точном
// эмуляторе — см. mnm_fm_exact.hpp и README_fm_machines.md), поверх
// восстановленной топологии движка Monomachine Nova (та же структура, что у
// mnm-ядра MnmFm.hpp: синус-LUT $14A000 с интерполяцией, серийная/параллельная
// маршрутизация, квантизатор фазы mix>>12).
//
// ЧЕСТНЫЕ ГРАНИЦЫ (как в DSP_New/distortion):
//   БИТ-В-БИТ:  таблица отношений P:$141A80 (24 значения), индекс n=((K+$8000)*24)>>24,
//               скрытый ×2 в канале модулятора-1 STAT ($145DB6 `asl b`),
//               1FIN = 1+(K-64)/256, уровни (K/64)^2 c wrap-через-знак mod 2^24,
//               друп 1-((K-64)/128)^4, гейн шейпера G = (K-64)/16 (гейт 64),
//               FB = K*32 табличных единиц (8192 = цикл), квантизатор фазы mix>>12,
//               TUNE униполярный +683*K/128 шагов таблицы $140000 (2048/октава)
//               => 0..+397 центов, DYN: 1FRQ K/2^19*2 (кламп $7FFFFF), 2FRQ (K/128)^2*4.
//   [РЕКОНСТР.]: размещение first-difference/шейпера внутри выборки, TONE как
//               выходная стадия (в прошивке LP стоит в мод-пути) и квадратурные
//               ротаторы стерео (в моноварианте плагина нет) — тот же уровень
//               допущений, что у текущего mnm-ядра 1.7.11.
//   DYN: по трём независимым доказательствам итерации 28 кросс-FM структурно
//               неслышим => слышимый выход = чистая синусоида несущей.
//
// Сознательное задокументированное отклонение: прошивочный выход FM = ±16 слов
// (гейн добирает AMP-стадией ядра $04A8); в плагине уровень нормирован к линии,
// множитель прошивки не применяется. На тембр законов не влияет.
#pragma once

#include "MnmFm.hpp"          // FmKind (m8/m9/m10), SineTable, AmpEnvelope, ToneLowpass
#include "mnm_fm_exact.hpp"   // точные законы итераций 27-28 (namespace mnm_fm)

#include <array>
#include <cmath>
#include <cstdint>

namespace monomachine {
namespace mnm {

class FmCoreExact {
public:
    void reset(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : kDspRate;
        tone.setSampleRate(sr);
        phase.fill(0.0f);
        fbState1 = 0.0f;
        mod1Prev = mod2Prev = mod3Prev = 0.0f;
        d2Prev = d3Prev = 0.0f;
        env.reset();
        lastOut = 0.0f;
    }
    void noteOn(float midiNote, float velocity = 1.0f) {
        note = midiNote; vel = velocity;
        phase.fill(0.0f); fbState1 = 0.0f;
        mod1Prev = mod2Prev = mod3Prev = 0.0f;
        d2Prev = d3Prev = 0.0f; lastOut = 0.0f;
        env.trigger();
    }
    void noteOff() { env.release(); }
    void setEnvelope(float atk, float dec, float rel, float sustainLevel) {
        env.setParameters(atk, dec, rel, sustainLevel);
    }
    void setPitchMod(float semitones) { pitchMod = semitones; }
    void setEnvelopeBypass(bool bypass) { envelopeBypass = bypass; }

    void setParameters(FmKind k, const std::array<float, 8>& p) {
        kind = k; params = p;
        // TONE: общий однополюсник P:$144AC7 — STAT/PAR (слот 6); у DYN слот 6 = 2FB,
        // пост-стадия открыта.
        tone.setTone(kind == FmKind::Dyn ? 127.0f : params[6]);
        // Предвычисление законов на блок (как в прошивке: CONF считает коэффициенты
        // один раз на блок, P:$145D36-$145E17).
        std::array<int, 8> kp{};
        for (int i = 0; i < 8; ++i)
            kp[static_cast<size_t>(i)] = std::clamp(int(std::lround(params[static_cast<size_t>(i)])), 0, 127);
        if (kind == FmKind::Stat) {
            // 1FRQ: ratio = TBL[idx] * 2 * finmult (скрытый x2 мод-1)
            exRatio1 = mnm_fm::ratioOf(mnm_fm::ratioIndex(kp[0])) * 2.0f * mnm_fm::finMultiplier(kp[1]);
            exRatio2 = mnm_fm::ratioOf(mnm_fm::ratioIndex(kp[4]));
            // уровни: (K/64)^2 mod 2^24 со знаком; друп на 1ENV
            exLevel1 = mnm_fm::wrapLevelSq(kp[2]) * mnm_fm::envDroop(kp[2]);
            exLevel2 = mnm_fm::wrapLevelSq(kp[5]);
            // 1FB: фидбек = sin_prev * K*32 единиц таблицы
            exFbFrac = mnm_fm::fbPhaseUnits(kp[3]) / 8192.0f;
            // шейпер канала оп-2 (гейт от 2VOL)
            exG2 = mnm_fm::shaperGain(kp[5]);
        } else if (kind == FmKind::Par) {
            exRatio1 = mnm_fm::ratioOf(mnm_fm::ratioIndex(kp[0]));   // без x2 и без FIN
            exRatio2 = mnm_fm::ratioOf(mnm_fm::ratioIndex(kp[2]));
            exRatio3 = mnm_fm::ratioOf(mnm_fm::ratioIndex(kp[4]));
            exLevel1 = mnm_fm::wrapLevelSq(kp[1]) * mnm_fm::envDroop(kp[1]);
            exLevel2 = mnm_fm::wrapLevelSq(kp[3]) * mnm_fm::envDroop(kp[3]);
            exLevel3 = mnm_fm::wrapLevelSq(kp[5]) * mnm_fm::envDroop(kp[5]);
            exG1 = mnm_fm::shaperGain(kp[1]);
            exG2 = mnm_fm::shaperGain(kp[3]);
            exG3 = mnm_fm::shaperGain(kp[5]);
        } else { // Dyn
            exRatio1 = mnm_fm::dynRatio1(kp[0]);                     // непрерывный, кламп $7FFFFF
            exRatio2 = mnm_fm::dynRatio2(kp[4]);                     // (K/128)^2*4
            // слышимая часть — чистая несущая (iter-28: 3 доказательства неслышимости)
        }
        // TUNE: униполярный, pitch += TUNE*683/128 шагов таблицы $140000
        exTuneSemi = mnm_fm::tuneCents(kp[7]) / 100.0f;
    }

    // Один 16-сэмпловый блок, моно — тот же контракт, что у MnmFm::FmCore::processBlock.
    void processBlock(float* out, int frames) {
        const float f0 = noteToHz(note + pitchMod + exTuneSemi);
        const float dt = static_cast<float>(f0 / sr);

        for (int i = 0; i < frames; ++i) {
            const float e = envelopeBypass ? 1.0f : env.tick();
            float carrierPhase = phase[2];

            if (kind == FmKind::Stat) {
                // оп-1: синус с собственным фидбеком FB (в единицах таблицы)
                const float mod1 = SineTable::instance().read(wrapf(phase[0] + fbState1 * exFbFrac)) * e;
                // оп-2: синус -> first difference -> шейпер d[i]+G*d[i-1]
                const float mod2 = SineTable::instance().read(phase[1]) * e;
                const float d2 = mod2 - mod2Prev; mod2Prev = mod2;
                const float sh2 = d2 + exG2 * d2Prev; d2Prev = d2;
                // взвешенный микс: (mod1 + sh2*level2) * level1  [2VOL модулирует оп-1]
                const float mix = (mod1 + sh2 * exLevel2) * exLevel1;
                carrierPhase = phase[2] + quantDev(mix);
                phase[1] = wrapf(phase[1] + dt * exRatio2);
                fbState1 = mod1;
            } else if (kind == FmKind::Par) {
                const float mod1 = SineTable::instance().read(phase[0]) * e;
                const float mod2 = SineTable::instance().read(phase[1]) * e;
                const float mod3 = SineTable::instance().read(phase[3]) * e;
                const float d1 = mod1 - mod1Prev; mod1Prev = mod1;
                const float d2 = mod2 - mod2Prev; mod2Prev = mod2;
                const float d3 = mod3 - mod3Prev; mod3Prev = mod3;
                const float sh1 = d1 + exG1 * d1Prev; d1Prev = d1;
                const float sh2 = d2 + exG2 * d2Prev; d2Prev = d2;
                const float sh3 = d3 + exG3 * d3Prev; d3Prev = d3;
                const float mix = sh1 * exLevel1 + sh2 * exLevel2 + sh3 * exLevel3;
                carrierPhase = phase[2] + quantDev(mix);
                phase[1] = wrapf(phase[1] + dt * exRatio2);
                phase[3] = wrapf(phase[3] + dt * exRatio3);
            } else { // Dyn — слышимый выход = чистая несущая (iter-28)
                carrierPhase = phase[2];
            }

            const float carrier = SineTable::instance().read(wrapf(carrierPhase));
            // TONE — общий однополюсник P:$144AC7 (пост-стадия, как в mnm-ядре)
            lastOut = tone.process(0, carrier * vel);
            out[i] = lastOut;

            phase[0] = wrapf(phase[0] + dt * exRatio1);
            phase[2] = wrapf(phase[2] + dt);
        }
    }

    float lastValue() const { return lastOut; }

private:
    static float wrapf(float v) { return v - std::floor(v); }

    // Прошивочный квантизатор фазы: raw = signed24(mix * 2^23) с wrap mod 2^24
    // (то же семейство wrap-законов, что у уровней); dev = raw >> 12 единиц
    // таблицы; 8192 единиц = цикл.
    static float quantDev(float mix) {
        int64_t raw = static_cast<int64_t>(std::lround(mix * 8388608.0f));
        raw &= 0xFFFFFFLL;
        if (raw & 0x800000LL) raw -= 0x1000000LL;
        const int64_t dev = raw >> 12;
        return static_cast<float>(dev) / 8192.0f;
    }

    FmKind kind = FmKind::Stat;
    std::array<float, 8> params{};
    std::array<float, 5> phase{};
    AmpEnvelope env;
    ToneLowpass tone;
    // законы "ex" (предвычислены в setParameters)
    float exRatio1 = 1.0f, exRatio2 = 1.0f, exRatio3 = 1.0f;
    float exLevel1 = 0.0f, exLevel2 = 0.0f, exLevel3 = 0.0f;
    float exFbFrac = 0.0f, exG1 = 0.0f, exG2 = 0.0f, exG3 = 0.0f, exTuneSemi = 0.0f;
    float fbState1 = 0.0f, mod1Prev = 0.0f, mod2Prev = 0.0f, mod3Prev = 0.0f;
    float d1Prev = 0.0f, d2Prev = 0.0f, d3Prev = 0.0f;
    float lastOut = 0.0f, vel = 1.0f, note = 60.0f, pitchMod = 0.0f;
    bool envelopeBypass = true;
    double sr = kDspRate;
};

}  // namespace mnm
}  // namespace monomachine
