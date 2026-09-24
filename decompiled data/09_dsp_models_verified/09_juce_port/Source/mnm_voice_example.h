// ============================================================================
// mnm_voice_example.h — готовый «трек» Monomachine: AMP-env -> гейн ->
// dual-фильтр. Порядок узлов соответствует доказанному конвейеру ядра:
//
//   машина (ваш осциллятор/сэмплер) -> [EQ: байпас, Ф3 открыта] ->
//   -> каскад 1 (dual FIR/SVF, точный) -> РЕЗОНАТОР (8 тапов) ->
//   -> гейн |level|*VOL^2*pan (AMP-env, по-сэмплово) -> выход
//
//   AMP-env: trig 1/2/3 (note-on/off/kill); фаза KILL гейтит fb интегратора
//   delay-модуляции (если подключите mnm-делэй — см. 08_env_verdict/).
//
// Кадр = 16 сэмплов (родная сетка машины). processBlock JUCE просто режет
// буфер на кадры (см. Example/MnmFilterPluginExample.h).
// ============================================================================
#pragma once
#include "mnm_dual_filter.h"
#include "mnm_amp_env.h"

namespace mnm {

class MonoMachineTrackVoice
{
public:
    void prepare (double sampleRate) noexcept
    {
        filter.prepare (sampleRate);
        env.reset();
        downsampleCount = 0;
    }

    void reset() noexcept { filter.reset(); env.reset(); }

    // --- события (как TRIG_EVENT Y[P+$28]) ---------------------------------
    void noteOn()  noexcept { env.trig (1); }
    void noteOff() noexcept { env.trig (2); }
    void kill()    noexcept { env.trig (3); }

    // --- ручки машины (0..127, как на железе) ------------------------------
    void setAmp (int atk, int hold, int dec, int rel, int vol, int pan) noexcept
    {
        ep.atk = atk; ep.hold = hold; ep.dec = dec; ep.rel = rel;
        ep.vol = vol; ep.pan = pan;
    }
    void setFilter (int base, int wdth, int hpq, int lpq) noexcept
    {
        // Слова параметров — как CPU оригинала (knob<<16).
        fp.baseWord = wordFromKnob (base);
        fp.wdthWord = wordFromKnob (wdth);
        fp.hpqWord  = wordFromKnob (hpq);
        fp.lpqWord  = wordFromKnob (lpq);
    }
    // Прямая модуляция словами (LFO/мод-матрица плагина — так «двигает»
    // фильтр оригинал): word = 0..0xFFFFFF, Q23.
    void setFilterWords (uint32_t base, uint32_t wdth, uint32_t hpq, uint32_t lpq) noexcept
    {
        fp.baseWord = base; fp.wdthWord = wdth; fp.hpqWord = hpq; fp.lpqWord = lpq;
    }

    FilterParams&       filterParams()       noexcept { return fp; }
    const FilterParams& filterParams() const noexcept { return fp; }
    MonoMachineAmpEnv&  ampEnv()             noexcept { return env; }

    // Блок ANY размера (interleaved stereo). Родная сетка = 44100 Гц;
    // на 48k/96k звук корректен, временнЫе константы фильтра масштабируются
    // естественно, амплитудные таблицы энвелопа остаются в кадрах оригинала.
    template <typename Float>
    void process (Float* io, int numFrames) noexcept   // numFrames = сэмплов/2 (st пар)
    {
        int pos = 0;
        while (pos < numFrames)
        {
            const int n = std::min (kFrame - downsampleCount, numFrames - pos);
            for (int i = 0; i < n; ++i)
            {
                frame[2 * (downsampleCount + i)]     = (float) io[2 * (pos + i)];
                frame[2 * (downsampleCount + i) + 1] = (float) io[2 * (pos + i) + 1];
            }
            downsampleCount += n;
            pos += n;
            if (downsampleCount == kFrame)
            {
                filter.processFrame (frame, filtered);      // каскад 1 + резонанс
                env.gainFrame (ep, filtered);               // AMP-env гейн
                for (int i = 0; i < kFrame; ++i)
                {
                    io[2 * (pos - kFrame + i)]     = (Float) filtered[2 * i];
                    io[2 * (pos - kFrame + i) + 1] = (Float) filtered[2 * i + 1];
                }
                downsampleCount = 0;
            }
        }
    }

private:
    MonoMachineDualFilter filter;
    MonoMachineAmpEnv     env;
    FilterParams          fp;
    MonoMachineAmpEnv::Params ep;
    float frame[kFrame * 2] {};
    float filtered[kFrame * 2] {};
    int   downsampleCount = 0;
};

} // namespace mnm
