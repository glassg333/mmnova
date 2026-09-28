// =============================================================================
// MnmFm.hpp — FM+STAT / FM+PAR / FM+DYN integration core (Monomachine Nova)
// ВЕРСИЯ 2 (2026-09-28, итерация «фикс ручек и тюна по реальным данным»).
//
// ЧТО ИСПРАВЛЕНО В ЭТОЙ ВЕРСИИ (все пункты ИЗМЕРЕНЫ, не угаданы):
//
// 1. ТАБЛИЦА СООТНОШЕНИЙ STAT/PAR БЫЛА НА ОКТАВУ ВЫШЕ.
//    Прежний код делил слова таблицы Y:$141A80 на 2^19 (получалось 1/32..8).
//    Реальная машина (сверено с живой Monomachine и эмулятором, данные
//    пользователя от 28.09) даёт 24 ступени 1/64..4 = слово/2^20:
//        1/64 1/32 1/16 3/32 1/8 5/32 3/16 1/4 5/16 3/8 7/16 1/2
//        5/8 3/4 7/8 1 1.25 1.5 1.75 2 2.5 3 3.5 4
//    Проверка по дефолтам дескриптора: STAT 1FRQ=$3C(60) -> 1/2, 2FRQ=$50(80)
//    -> 1.0 — ровно как на машине («по дефолту 1frq - 1/2, 2frq - 1»).
//    Закон индекса НЕ менялся (он был верный, регистровые измерения $145D38/
//    $145DAD): n = floor(((K<<16)+$8000)*48/2^24).
//
// 2. ПИТЧ-СЛОВО БЫЛО НА ОКТАВУ НИЖЕ (PAR/DYN).
//    Машина тикает на ПОЛОВИННОЙ частоте: 32 слова блока = 16 L/R пар
//    (проверено: L==R во всех 16 парах), ядро ОС кладёт их на шину с
//    удвоением (полифазные ротаторы). При потреблении «32 слова = 32 кадра»
//    (наш FIFO) слово A даёт частоту A/2. Измерение на бит-точных ядрах:
//    A=440 -> 220 Гц, A=880 -> 440 Гц. Поэтому ядро ОС подаёт A = 2*Гц.
//    Фикс: pitchA = 2 * noteToHz(нота + бенд + TUNE) * 44100/sr.
//    (множитель 44100/sr — компенсация хостовой частоты, машина —
//    устройство 44.1 кГц).
//
// 3. TUNE ВООБЩЕ НЕ ДЕЙСТВОВАЛА (закон прошивки добавлен).
//    Кернел ($2C1-$2C9) прибавляет к питч-слову X:$140000 (2048 шагов/октава)
//    (K-64)*683/128 шага. В центах: (K-64)*3.12652587890625, т.е.
//    K=0 -> -200.10 цента (C -> A#, ровно ~2 полутона вниз),
//    K=127 -> +196.97 цента («не хватает до D» — как на машине).
//    Прежний порт игнорировал params[7]; старые движки плагина считали
//    (K-64)/64*12 полутонов (±12 СЕМИТОНОВ вместо ±2) — исправлено и тут,
//    и в патче старых движков.
//
// 4. FM+STAT ТЕПЕРЬ ТОЖЕ ЧЕРЕЗ БИТ-ТОЧНОЕ ЯДРО.
//    MnmFmStat.hpp — пословная транскрипция P:$145D12-$145EC8, сверена с
//    эмулятором OS 1.32 (400 блоков / 52800 слов, 0 расхождений; свип всех
//    8 ручек x 7 позиций: 1344 блока / 177408 слов, 0 расхождений).
//    Прежний float-путь STAT (с самодельной 1ENV/2VOL/1FB) оставлен ниже как
//    processStatFloatLegacy() и НЕ вызывается.
//
// 5. DYN: законы ручек частоты подтверждены списком с живой машины:
//        1FRQ = K/64 (K=127 -> 2.0, кламп $7FFFFF),
//        2FRQ = (K/64)^2 (K=127 -> 4.0) — совпали 128/128 позиций.
//    Это уже было внутри бит-точного ядра; здесь только фикс питча (п.2).
//
// ЭКРАН (цифры под ручками): патч PluginEditor.cpp шлётся отдельно
// (FM_KNOB_TABLES.md / display_patch_snippet.cpp) — таблица та же, что тут.
// =============================================================================
#pragma once

#include "MnmKernel.hpp"

#include "MnmFmDsp.hpp"
#include "MnmFmStat.hpp"
#include "MnmFmPar.hpp"
#include "MnmFmDyn.hpp"

#include <array>
#include <cmath>
#include <cstdint>

namespace monomachine {
namespace mnm {

enum class FmKind { Stat, Par, Dyn };

// Parameter order is the hardware order from the ColdFire descriptors:
//  STAT: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE   (defaults 3C 40 50 1E 50 40 62 40)
//  PAR:  1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE  (defaults 3C 40 50 40 66 50 62 40)
//  DYN:  1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB  TUNE   (defaults 40 40 40 40 4A 50 1E 40)

// Ratio table Y:$141A80, 24 entries, scale word/2^20 -> effective 1/64 .. 4.0
// (сверено с живой машиной; дефолты дескриптора дают 1/2 и 1 — как в оригинале).
inline constexpr std::array<float, 24> kFmRatioExact = {
    0.015625f, 0.03125f, 0.0625f, 0.09375f, 0.125f, 0.15625f,
    0.1875f, 0.25f, 0.3125f, 0.375f, 0.4375f, 0.5f,
    0.625f, 0.75f, 0.875f, 1.0f, 1.25f, 1.5f,
    1.75f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f,
};

// Как то же самое показывает дисплей машины (дроби).
inline constexpr std::array<const char*, 24> kFmRatioText = {
    "1/64", "1/32", "1/16", "3/32", "1/8", "5/32",
    "3/16", "1/4", "5/16", "3/8", "7/16", "1/2",
    "5/8", "3/4", "7/8", "1", "5/4", "3/2",
    "7/4", "2", "5/2", "3", "7/2", "4",
};

class FmCore {
public:
    void reset(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : kDspRate;
        tone.setSampleRate(sr);
        phase.fill(0.0f);
        fbState = 0.0f;
        env.reset();
        lastOut = 0.0f;
        statCore.init();
        parCore.init();
        dynCore.init();
        fifoLen = 0;
    }

    void noteOn(float midiNote, float velocity = 1.0f) {
        note = midiNote;
        vel = velocity;
        phase.fill(0.0f);
        fbState = 0.0f;
        lastOut = 0.0f;
        env.trigger();
        // init() на каждой ноте — как в прошивке: CONF переинициирует FM+
        // огибающие, INIT чистит страницу голоса (включая мусор $29/$31).
        statCore.init();
        parCore.init();
        dynCore.init();
        fifoLen = 0;
    }
    void noteOff() { env.release(); }
    // Плагиновая AMP-огибающая работает ПОСЛЕ голоса (как в прошивке — кернел),
    // поэтому для бит-точного пути это сознательный no-op (совместимость API).
    void setEnvelope(float, float, float, float) {}

    void setParameters(FmKind k, const std::array<float, 8>& p) {
        kind = k;
        params = p;
        for (int i = 0; i < 8; ++i)
            knobWords[(size_t)i] = (uint32_t)std::clamp(p[(size_t)i], 0.0f, 127.0f);
    }
    void setPitchMod(float semitones) { pitchMod = semitones; }
    void setEnvelopeBypass(bool bypass) { envelopeBypass = bypass; }
    // Direct pitch-word override for the exact cores (the kernel pitch word,
    // register A on PROC entry). The original firmware accepts pitch word 0 =
    // frozen carrier (frequency truly 0) — use pitchWordValid to enable it.
    // ВНИМАНИЕ: слово задаётся в конвенции ядра ОС (A = 2*Гц при sr=44100).
    void setPitchWordOverride(uint32_t w, bool valid = true) {
        pitchWordOverride = w; pitchWordValid = valid;
    }

    // Renders frames, mono.
    void processBlock(float* out, int frames) {
        processExact(out, frames);
    }

    float lastValue() const { return lastOut; }

private:
    // ------------------------------------------------------------- knob laws
    static float wrapf(float v) { return v - std::floor(v); }

    // REGISTER-MEASURED on the emulator at both fetch sites ($145D38/$145DAD):
    // n = floor(((K<<16) + $8000) * 48 / 2^24) = floor((K + 0.5) * 3 / 16).
    static int ratioIndexExact(float k0to127) {
        const int K = static_cast<int>(std::clamp(k0to127, 0.0f, 127.0f));
        const int n = (((K << 16) + 0x8000) * 48) >> 24;
        return std::clamp(n, 0, 23);
    }

    // TUNE: кернел прошивки ($2C1-$2C9) прибавляет (K-64)*683/128 шага к
    // питч-слову X:$140000 (2048 шагов/октава) -> в полутонах:
    // (K-64) * 683/128 * 12/2048 = (K-64) * 0.03125762939453125.
    // K=0 -> -2.0008 полутона (-200.10 цента), K=127 -> +1.9747 (+196.97 ц).
    static float tuneSemitones(float k0to127) {
        const float K = std::clamp(k0to127, 0.0f, 127.0f);
        return (K - 64.0f) * (683.0f / 128.0f) * (12.0f / 2048.0f);
    }

    // 1FIN word-exact: w = ((K<<16) - $400000) >> 2 (arithmetic), w += $400000;
    // multiplier = w / $400000 -> 0.75 .. 1.2461, centre 1.0 at K=64.
    static float fineWord(float k0to127) {
        const int K = static_cast<int>(std::clamp(k0to127, 0.0f, 127.0f));
        int w = (K << 16) - 0x400000;
        w >>= 2;                     // asr #$2  ($145DB1)
        w += 0x400000;               // ($145DB2)
        return static_cast<float>(w) * (1.0f / 4194304.0f);
    }

    static float noteToHz(float midi) {
        return 440.0f * std::pow(2.0f, (midi - 69.0f) / 12.0f);
    }

    // --------------------------------------------------- exact path (ALL 3)
    // Бит-точные ядра рендерят 32-словные блоки (16 L/R пар = 16 машинных
    // тиков на ПОЛОВИННОЙ частоте). FIFO кладёт все 32 слова как 32 кадра —
    // это ровно поток машины до её полифазных ротаторов. Питч-слово — в
    // конвенции ядра ОС: A = 2*Гц * (44100/sr)  (см. шапку, пункт 2).
    void processExact(float* out, int frames) {
        int done = 0;
        while (done < frames) {
            if (fifoLen == 0) {
                const uint32_t pitchA = pitchWordValid
                    ? pitchWordOverride
                    : kernelPitchWord(note + pitchMod + tuneSemitones(params[7]));
                switch (kind) {
                    case FmKind::Stat:
                        statCore.conf(knobWords.data());
                        statCore.proc(pitchA, fifoBuf);
                        break;
                    case FmKind::Par:
                        parCore.conf(knobWords.data());
                        parCore.proc(pitchA, fifoBuf);
                        break;
                    case FmKind::Dyn:
                        dynCore.conf(knobWords.data());
                        dynCore.proc(pitchA, fifoBuf);
                        break;
                }
                fifoLen = 32;
                fifoPos = 0;
            }
            const int n = std::min(frames - done, fifoLen);
            for (int i = 0; i < n; ++i) {
                // Q23 word -> float: sign-extend 24-bit first, then normalise
                // to the measured default-knob peak (2.14 Q23 at descriptor
                // defaults for all 3 machines).
                const int32_t sw = (int32_t)(fifoBuf[fifoPos + i] << 8) >> 8;
                const float v = static_cast<float>(sw) * (1.0f / 17949485.0f);
                out[done + i] = v;
                lastOut = v;
            }
            fifoPos += n;
            fifoLen -= n;
            done += n;
        }
    }

    // Слово PROC в конвенции ядра ОС: машина тикает на sr/2, поэтому
    // A = 2 * f(Гц) * 44100/sr  (при sr=44100 — ровно 2*Гц; измерено:
    // A=880 в этой конвенции даёт 440 Гц на выходе).
    uint32_t kernelPitchWord(float midi) const {
        const double f = 2.0 * (double)noteToHz(midi) * (44100.0 / sr);
        if (f < 1.0) return 1;                      // ниже не слышно, 0 = стоп кэрриера
        if (f > 8388607.0) return 0x7FFFFF;
        return (uint32_t)std::llround(f);
    }

public:
    // ------------------------------------------------------------------
    // LEGACY float STAT path — НЕ вызывается, оставлен для истории/сравнения.
    // Здесь была таблица 1/32..8 (октава вверх) и самодельные 1ENV/2VOL/1FB.
    // ------------------------------------------------------------------
    void processStatFloatLegacy(float* out, int frames) {
        const float f0 = noteToHz(note + pitchMod + tuneSemitones(params[7]));
        const float dt = static_cast<float>(f0 / sr);

        const float ratio1 = kFmRatioExact[(size_t)ratioIndexExact(params[0])] *
                             fineWord(params[1]);                       // 1FRQ, 1FIN
        const float ratio2 = kFmRatioExact[(size_t)ratioIndexExact(params[4])];  // 2FRQ

        const float depth1 = squaredDepth(params[2]);
        const bool op2On = params[5] >= 64.0f;
        const float depth2 = op2On ? 1.0f : 0.0f;
        const float fb = fbDepth(params[3]);

        for (int i = 0; i < frames; ++i) {
            const float e = envelopeBypass ? 1.0f : env.tick();

            const float mod1Phase = phase[0] + fbState * fb;
            const float mod1 = SineTable::instance().read(mod1Phase) * e;
            const float mod2 = SineTable::instance().read(phase[1]) * e;

            phase[1] = wrapf(phase[1] + dt * ratio2);
            const float carrierPhase = phase[2] + (mod1 + mod2 * depth2) * depth1 * 8.0f;

            const float carrier = SineTable::instance().read(carrierPhase);
            lastOut = carrier * vel;
            out[i] = tone.process(0, lastOut);

            phase[0] = wrapf(phase[0] + dt * ratio1);
            phase[2] = wrapf(phase[2] + dt);
            fbState = mod1;
        }
    }

private:
    // 1ENV depth: (K/128)^2 — from `mpy x0,x0,a; asl #$2` ($145DFB/$145E14).
    static float squaredDepth(float k0to127) {
        const float x = std::clamp(k0to127, 0.0f, 127.0f) / 128.0f;
        return x * x;
    }

    // 1FB: K^2 word law ($145DFB), normalised to ~0.5 cycle at the top.
    static float fbDepth(float k0to127) {
        const float x = std::clamp(k0to127, 0.0f, 127.0f) / 127.0f;
        return x * x * 0.5f;
    }

    FmKind kind = FmKind::Stat;
    std::array<float, 8> params{};
    std::array<uint32_t, 8> knobWords{};
    std::array<float, 5> phase{};
    AmpEnvelope env;
    ToneLowpass tone;
    mnmfm::MnmFmStat statCore;
    mnmfm::MnmFmPar parCore;
    mnmfm::MnmFmDyn dynCore;
    uint32_t fifoBuf[32]{};
    int fifoLen = 0, fifoPos = 0;
    float fbState = 0.0f, lastOut = 0.0f, vel = 1.0f, note = 60.0f, pitchMod = 0.0f;
    uint32_t pitchWordOverride = 0;
    bool pitchWordValid = false;
    bool envelopeBypass = true;
    double sr = kDspRate;
};

}  // namespace mnm
}  // namespace monomachine
