// Nova v6: рабочий кит BBOX = СТАРЫЙ (до реворка 1.4) + снейр и два тома из нового,
//          старый закон RTIM с инверсией, сглаживание повторов по СЭМПЛАМ.
// -----------------------------------------------------------------------------
// ЧТО ИЗМЕНИЛОСЬ В v6 (мономашина-нова, раздел BBOX)
//   1. RTIM: возвращена СТАРАЯ формула (1.0-1.3.1) 5...255 мс -- но ИНВЕРТИРОВАННАЯ,
//      как просили: rtim=0 -> 255 мс (реже всего), rtim=127 -> 5 мс (чаще всего).
//      Интервал повтора теперь НИКОГДА не 0 (было 255*((127-rtim)/126)^2 -> 0 мс),
//      поэтому звук не пропадает и повторы не сливаются в тишину на 100...127.
//   2. Затухание повторов оставлено из новой версии: gain = 10^(-0.75*counter/total).
//   3. Клик на перезапуске сэмпла убран сглаживанием ПО СЭМПЛАМ (kDeclickSamples=8,
//      ~0.18 мс при 44.1 кГц): короткий фейд в начале удара + спад перед повтором.
//      Миллисекундных огибающих в тракте повтора больше нет.
//   4. STRT (start position): смещение мягче (1/8 активной длины вместо 1/4),
//      короткие удары больше не срезаются так сильно; предел 30 мс сохранён.
//   5. Кит по умолчанию: ОСНОВНОЙ=старый кит (кик, клэп, колокол, Blipp/лазер,
//      остальное) + три голоса нового кита: Snare Drum 1 (слот 2), Tom 1 (слот 3),
//      Tom 2 (слот 4). Полностью старый кит остался в generateLegacyKit().
#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstdint>
#include <utility>

namespace monomachine {

/**
 * @brief Elektron Monomachine DPRO-BBOX (Machine 7) & Extended Sampler Engine.
 * 
 * Hardware Architecture:
 * - DPRO-BBOX is the drum synthesizer/beatbox machine of the Monomachine.
 * - Hardware parameters:
 *   - PTCH (Pitch): Sample playback pitch / rate (-64 .. +63 semitone/fine ratio)
 *   - STRT (Start): Sample playback start offset (0 .. 127 -> bounded quadratic offset, up to 30 ms)
 *   - RTRG (Retrig Count): Number of rapid retrigger repeats (0 = off, 1 .. 127)
 *   - RTIM (Retrig Time): Time between retriggers (tempo synchronized / milliseconds)
 * 
 * Extension for Modern DAW / VST3:
 * - 24 Sample Slots with dynamic loading.
 * - Sample randomization mode (random slot selection and/or random start position).
 * - Linear / Hermite sample interpolation with pitch tracking.
 */
class MonomachineBBox {
public:
    static constexpr size_t kMaxSlots = 24;
    // Длина сглаживания при перезапуске удара -- в СЭМПЛАХ, а не в миллисекундах:
    // 16 сэмплов = 0.36 мс при 44.1 кГц. Спад ДОХОДИТ ровно до нуля к моменту
    // перезапуска, поэтому перезапуск не даёт щелчка (раньше позиция прыгала скачком).
    static constexpr size_t kDeclickSamples = 16;

    struct SampleSlot {
        std::string name;
        std::vector<float> data;
        double originalSampleRate = 44100.0;
        bool loaded = false;
        size_t activeFrames=0;
    };

    static void analyseSlot(SampleSlot& slot){float peak=0;for(float v:slot.data)peak=std::max(peak,std::abs(v));slot.activeFrames=slot.data.size();while(slot.activeFrames>1&&std::abs(slot.data[slot.activeFrames-1])<peak*0.0001f)--slot.activeFrames;}
    int playingSlot()const noexcept{return m_isPlaying?static_cast<int>(m_currentPlayingSlot):-1;}
    size_t retriggerInterval()const noexcept{return m_retrigIntervalSamples;}
    // 1.6.12: STRT мягче (жалоба: на 50 уже срезал слишком много): кубическая
    // кривая и шапка 25 мс вместо квадратичной на 30 мс; STRT=0 всегда точный старт.
    double startPosition(const SampleSlot& slot)const{const double x=static_cast<double>(m_paramStrt)/127;return x*x*x*std::min(static_cast<double>(slot.activeFrames?slot.activeFrames:slot.data.size())*0.25,slot.originalSampleRate*0.025);}
    // 1.6.12: адаптивный деклик вместо фиксированных 16 сэмплов: длина фейда
    // зависит от величины скачка в точке старта. Тихое начало (транзиент) --
    // 2 сэмпла (атака не убивается), старт с синусоиды на полной амплитуде --
    // до ~4 мс (сильный клик убран).
    MonomachineBBox(bool modern=true) {
        m_slots.resize(kMaxSlots);
        m_rng.seed(1337);
        if(modern)generateDefaultDrumKit();else generateLegacyKit();
        for(auto& slot:m_slots)analyseSlot(slot);
    }

    void reset(double sampleRate = 44100.0) {
        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        m_playbackPos = 0.0;
        m_isPlaying = false;
        m_retrigCounter = 0;
        m_retrigSamplesRemaining = 0;
    }

    /**
     * @brief Load a raw mono sample into one of the 24 slots (0..23).
     */
    bool loadSample(size_t slotIndex, const std::string& name, const float* samples, size_t numSamples, double sampleRate = 44100.0) {
        if (slotIndex >= kMaxSlots || samples == nullptr || numSamples == 0) return false;
        m_slots[slotIndex].name = name;
        m_slots[slotIndex].data.assign(samples, samples + numSamples);
        m_slots[slotIndex].originalSampleRate = sampleRate;
        m_slots[slotIndex].loaded = true;analyseSlot(m_slots[slotIndex]);
        return true;
    }

    /**
     * @brief Monomachine BBox hardware parameter setter.
     */
    void setParameters(uint8_t ptch, uint8_t strt, uint8_t rtrg, uint8_t rtim,
                       uint8_t slotSelect = 0, bool randomSlot = false, bool randomStrt = false) {
        m_paramPtch = ptch;
        m_paramStrt = strt;
        m_paramRtrg = rtrg;
        m_paramRtim = rtim;
        m_activeSlot = slotSelect % kMaxSlots;
        m_randomSlot = randomSlot;
        m_randomStart = randomStrt;

        // Pitch multiplier: 0 -> -2 octaves (0.25x), 64 -> 1.0x (normal), 127 -> +2 octaves (4.0x)
        float semitones = (static_cast<float>(ptch) - 64.0f) / 64.0f * 24.0f;
        m_pitchRatio = std::pow(2.0f, semitones / 12.0f);

        // Retrig interval in samples.
        // v6: СТАРАЯ формула 1.0-1.3.1 (5.0 + 250.0*rtim/127 = 5...255 мс), но по
        // просьбе ИНВЕРТИРОВАННАЯ: rtim=0 -> 255 мс, rtim=127 -> 5 мс.
        // Главное: интервал никогда не равен нулю, поэтому на 100...127 повторы
        // не превращаются в вечную перезагрузку сэмпла (это и съедало звук).
        float retrigMs = 5.0f + ((127.0f - static_cast<float>(rtim)) / 127.0f) * 250.0f;
        m_retrigIntervalSamples = std::max(size_t(1),static_cast<size_t>((retrigMs / 1000.0f) * m_sampleRate));
    }

    /**
     * @brief Trigger note on / drum hit.
     */
    void setChromatic(bool enabled) noexcept { m_chromatic=enabled; }
    static int chromaticSlot(int note) noexcept { const int n=note-48;return (n%24+24)%24; }
    static int chromaticOctave(int note) noexcept {const int n=note-48;return (n-chromaticSlot(note))/24;}
    void noteOn(uint8_t midiNote = 60, uint8_t velocity = 127) {
        (void)velocity;
        size_t targetSlot = m_chromatic?static_cast<size_t>(chromaticSlot(midiNote)):m_activeSlot;

        if (m_randomSlot) {
            std::uniform_int_distribution<size_t> dist(0, kMaxSlots - 1);
            targetSlot = dist(m_rng);
        }

        m_currentPlayingSlot = targetSlot;
        auto& slot = m_slots[m_currentPlayingSlot];
        if (!slot.loaded || slot.data.empty()) {
            m_isPlaying = false;
            return;
        }

        float startFrac = static_cast<float>(m_paramStrt) / 127.0f;
        if (m_randomStart) {
            std::uniform_real_distribution<float> dist(0.0f, 0.85f);
            startFrac = dist(m_rng);
        }

        m_playbackPos = m_randomStart?startFrac*static_cast<double>(slot.data.size()):startPosition(slot);
        m_hitAge=0;updateHitFade(slot);
        m_isPlaying = true;

        // Retrigger setup
        m_retrigTotal = m_paramRtrg;
        m_retrigCounter = 0;
        m_retrigSamplesRemaining = m_retrigIntervalSamples;

        // Key tracking pitch offset (Middle C / 60 is neutral)
        float noteSemitones = m_chromatic?12.0f*static_cast<float>(chromaticOctave(midiNote)):static_cast<float>(midiNote)-60.0f;
        m_keyTrackRatio = std::pow(2.0f, noteSemitones / 12.0f);
    }

    /**
     * @brief Process a block of stereo samples.
     */
    void processStereo(float* outL, float* outR, size_t numFrames) {
        if (!m_isPlaying || m_currentPlayingSlot >= kMaxSlots) {
            for (size_t i = 0; i < numFrames; ++i) {
                outL[i] = 0.0f;
                outR[i] = 0.0f;
            }
            return;
        }

        const auto& slot = m_slots[m_currentPlayingSlot];
        const auto& data = slot.data;
        const size_t dataSize = data.size();
        const double rateFactor = (slot.originalSampleRate / m_sampleRate) * m_pitchRatio * m_keyTrackRatio * m_bendRatio;


        for (size_t i = 0; i < numFrames; ++i) {
            if (!m_isPlaying) {
                outL[i] = 0.0f;
                outR[i] = 0.0f;
                continue;
            }

            // Check Retrigger
            size_t samplesToRestart = 0;   // сколько сэмплов осталось ДО перезапуска
            if (m_retrigTotal > 0 && m_retrigCounter < m_retrigTotal) {
                samplesToRestart = m_retrigSamplesRemaining;
                if (m_retrigSamplesRemaining == 0) {
                    m_playbackPos = startPosition(slot);m_hitAge=0;updateHitFade(slot);
                    m_retrigSamplesRemaining = m_retrigIntervalSamples;
                    ++m_retrigCounter;
                }
                if(m_retrigSamplesRemaining>0)--m_retrigSamplesRemaining;
            }

            // Interpolated sample read
            size_t idx0 = static_cast<size_t>(m_playbackPos);
            if (idx0 >= dataSize) {
                if (m_retrigCounter >= m_retrigTotal) m_isPlaying = false;
                outL[i] = 0.0f;
                outR[i] = 0.0f;
                continue;
            }

            size_t idx1 = std::min(idx0 + 1, dataSize - 1);
            float frac = static_cast<float>(m_playbackPos - idx0);
            float sample = data[idx0] + frac * (data[idx1] - data[idx0]);

            // 1.6.12: адаптивный деклик (см. startPosition): длина фейда задаётся
            // амплитудой скачка при старте/перезапуске, 2 сэмпла...4 мс.
            const float fadeIn=static_cast<float>(std::min(1.0,static_cast<double>(m_hitAge++)/static_cast<double>(m_hitFade)));
            float fadeOut=1.0f;
            if(m_retrigTotal>0&&m_retrigCounter<m_retrigTotal&&samplesToRestart<m_hitFade)
                fadeOut=static_cast<float>(samplesToRestart)/static_cast<float>(m_hitFade);
            const float fade=std::min(fadeIn,fadeOut);
            // Затухание серии повторов -- из новой версии (10^(-0.75*counter/total)).
            const float gain=m_retrigTotal?std::pow(10.0f,-0.75f*static_cast<float>(m_retrigCounter)/m_retrigTotal):1.0f;
            outL[i] = sample*gain*fade;
            outR[i] = outL[i];

            m_playbackPos += rateFactor;
        }
    }

    void setPitchBend(float semitones) { m_bendRatio = std::pow(2.0f, semitones / 12.0f); }
    // Caller supplies an already decoded slot; allocation/destruction happen off the audio thread.
    void swapSample(size_t slot, SampleSlot& prepared) { std::swap(m_slots.at(slot), prepared); m_isPlaying = false; }
    const std::vector<SampleSlot>& getSlots() const noexcept { return m_slots; }

private:
    // РАБОЧИЙ кит v6: основа -- СТАРЫЙ кит (до реворка 1.4), он же звучал как надо:
    // кик, клэп, колокол (Cow Bell), лазер/zap (Blipp) и остальное. Из нового кита
    // взяты только три голоса, которые понравились: Snare Drum 1 (слот 2),
    // Tom 1 (слот 3), Tom 2 (слот 4) -- см. таблицу modernVoice ниже.
    // Индексы слотов, которые берутся из нового кита (0-based).
    void generateDefaultDrumKit() {
        static const char* names[24]={"Bass Drum 1","Snare Drum 1","Tom 1","Tom 2","Bongo Congo","Clap",
            "Rim Shot","Cow Bell","Closed HiHat","Open HiHat","Ride Cymbal","Crash Cymbal",
            "Bass Drum 2","Snare Drum 2","Timbale","Aggobell","Timpani","Snap",
            "Wood","Triangle","Shaker","Maracas","Whistle","Blipp"};
        const size_t type[24]={0,1,3,15,18,6,4,8,2,7,19,19,12,13,15,20,3,5,16,20,14,14,20,10};
        // true = голос из НОВОГО кита (снейр -- слот 2, Tom 1 -- слот 3, Tom 2 -- слот 4),
        // false = СТАРЫЙ кит (кик, клэп, колокол Cow Bell, лазер Blipp и всё остальное).
        static const bool modernVoice[24]={false,true,true,true,false,false,false,false,false,false,false,false,
                                           false,false,false,false,false,false,false,false,false,false,false,false};
        constexpr double pi=3.14159265358979323846;
        for(size_t index=0;index<kMaxSlots;++index){const size_t slot=type[index];auto& out=m_slots[index];out.loaded=true;out.originalSampleRate=44100;out.name=names[index];
            out.data.resize(22050);double phase=0,phase2=0;uint32_t seed=0x1234567u+static_cast<uint32_t>(index)*7919u;float previous=0;
            for(size_t i=0;i<out.data.size();++i){const double t=static_cast<double>(i)/44100;
                seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;const float noise=static_cast<float>(seed)/2147483648.0f-1;
                const double decay=(slot==7||slot==19||slot==23)?0.17:0.035+0.009*static_cast<double>(slot%7);
                const double env=std::exp(-t/decay);double v=0;
                if(index==22){phase+=2*pi*2100/44100;v=std::sin(phase)*std::exp(-t/0.12);}
                else if(index==10){phase+=2*pi*1830/44100;phase2+=2*pi*2947/44100;v=(0.22*(std::sin(phase)+std::sin(phase2))+noise*0.20)*std::exp(-t/0.22);}
                else if(index==11){const float high=noise-previous;previous=noise;v=high*0.5*std::exp(-t/0.30);}
                else if(index==20){const float high=noise-previous;previous=noise;v=high*0.6*std::exp(-t/0.045);}
                else if(index==21){v=noise*(0.5+0.5*std::sin(2*pi*95*t))*std::exp(-t/0.095);}
                else if(slot==0||slot==12){phase+=2*pi*(slot==0?45:65)/44100+0.10*std::exp(-t/0.008);v=std::sin(phase)*std::exp(-t/(slot==0?0.13:0.075))+noise*std::exp(-t/0.002)*0.12;}
                else if(slot==1||slot==13||slot==5){
                    if(slot!=5&&modernVoice[index]){phase+=2*pi*(slot==1?185:240)/44100;const double body=std::sin(phase+0.65*std::exp(-t/.016)*std::sin(phase*1.83));const double high=noise-previous;previous=noise;v=0.6*body*std::exp(-t/.024)+high*.35*std::exp(-t/(slot==1?.032:.045));}
                    else {phase+=2*pi*(slot==5?430:190)/44100;v=(0.25*std::sin(phase)+noise*0.75)*env;}}
                else if(slot==6||slot==17){double burst=t<0.03?(std::fmod(t,slot==6?0.009:0.012)<0.004?1.0:0.1):0.35;v=noise*burst*env;}
                else if(slot==3||slot==15||slot==18){
                    phase+=2*pi*((index==16?65:slot==3?85:index==14?290:slot==15?175:230)+90*std::exp(-t/.015))/44100;
                    v=modernVoice[index]?(std::sin(phase+1.8*std::exp(-t/.04)*std::sin(phase*1.414))+.16*std::sin(phase*2.76))*env:std::sin(phase)*env;}
                else if(slot==4||slot==16||slot==22){phase+=2*pi*(slot==4?850:slot==16?1750:3500)/44100;v=(std::sin(phase)+0.4*std::sin(phase*1.67))*std::exp(-t/0.014);}
                else if(slot==8||slot==9||slot==20){phase+=2*pi*(index==19?2200:slot==8?540:slot==9?870:1300)/44100;phase2+=2*pi*(slot==8?800:slot==9?1237:1743)/44100;v=(std::sin(phase)+0.6*std::sin(phase2))*env*0.65;}
                else if(slot==10||slot==21){const double hz=slot==10?90+2200*std::exp(-t/0.025):100+2300*(1-std::exp(-t/0.07));phase+=2*pi*hz/44100;v=std::sin(phase+0.7*std::sin(phase*2.31))*env;}
                else {const float high=noise-previous;previous=noise;v=(slot==11||slot==23?noise:high*0.55)*env;}
                const double fade=std::min(1.0,t/0.0005)*std::min(1.0,(0.5-t)/0.01);
                out.data[i]=static_cast<float>(std::clamp(v*fade*0.7,-1.0,1.0));
            }
        }
    }
    void generateLegacyKit() {
        static const char* names[24]={"Bass Drum 1","Snare Drum 1","Tom 1","Tom 2","Bongo Congo","Clap",
            "Rim Shot","Cow Bell","Closed HiHat","Open HiHat","Ride Cymbal","Crash Cymbal",
            "Bass Drum 2","Snare Drum 2","Timbale","Aggobell","Timpani","Snap",
            "Wood","Triangle","Shaker","Maracas","Whistle","Blipp"};
        const size_t type[24]={0,1,3,15,18,6,4,8,2,7,19,19,12,13,15,20,3,5,16,20,14,14,20,10};
        constexpr double pi=3.14159265358979323846;
        for(size_t index=0;index<kMaxSlots;++index){const size_t slot=type[index];auto& out=m_slots[index];out.loaded=true;out.originalSampleRate=44100;out.name=names[index];
            out.data.resize(22050);double phase=0,phase2=0;uint32_t seed=0x1234567u+static_cast<uint32_t>(index)*7919u;float previous=0;
            for(size_t i=0;i<out.data.size();++i){const double t=static_cast<double>(i)/44100;
                seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;const float noise=static_cast<float>(seed)/2147483648.0f-1;
                const double decay=(slot==7||slot==19||slot==23)?0.17:0.035+0.009*static_cast<double>(slot%7);
                const double env=std::exp(-t/decay);double v=0;
                if(index==22){phase+=2*pi*2100/44100;v=std::sin(phase)*std::exp(-t/0.12);}
                else if(index==10){phase+=2*pi*1830/44100;phase2+=2*pi*2947/44100;v=(0.22*(std::sin(phase)+std::sin(phase2))+noise*0.20)*std::exp(-t/0.22);}
                else if(index==11){const float high=noise-previous;previous=noise;v=high*0.5*std::exp(-t/0.30);}
                else if(index==20){const float high=noise-previous;previous=noise;v=high*0.6*std::exp(-t/0.045);}
                else if(index==21){v=noise*(0.5+0.5*std::sin(2*pi*95*t))*std::exp(-t/0.095);}
                else if(slot==0||slot==12){phase+=2*pi*(slot==0?45:65)/44100+0.10*std::exp(-t/0.008);v=std::sin(phase)*std::exp(-t/(slot==0?0.13:0.075))+noise*std::exp(-t/0.002)*0.12;}
                else if(slot==1||slot==13||slot==5){phase+=2*pi*(slot==5?430:190)/44100;v=(0.25*std::sin(phase)+noise*0.75)*env;}
                else if(slot==6||slot==17){double burst=t<0.03?(std::fmod(t,slot==6?0.009:0.012)<0.004?1.0:0.1):0.35;v=noise*burst*env;}
                else if(slot==3||slot==15||slot==18){phase+=2*pi*((index==16?65:slot==3?85:index==14?290:slot==15?175:230)+90*std::exp(-t/0.015))/44100;v=std::sin(phase)*env;}
                else if(slot==4||slot==16||slot==22){phase+=2*pi*(slot==4?850:slot==16?1750:3500)/44100;v=(std::sin(phase)+0.4*std::sin(phase*1.67))*std::exp(-t/0.014);}
                else if(slot==8||slot==9||slot==20){phase+=2*pi*(index==19?2200:slot==8?540:slot==9?870:1300)/44100;phase2+=2*pi*(slot==8?800:slot==9?1237:1743)/44100;v=(std::sin(phase)+0.6*std::sin(phase2))*env*0.65;}
                else if(slot==10||slot==21){const double hz=slot==10?90+2200*std::exp(-t/0.025):100+2300*(1-std::exp(-t/0.07));phase+=2*pi*hz/44100;v=std::sin(phase+0.7*std::sin(phase*2.31))*env;}
                else {const float high=noise-previous;previous=noise;v=(slot==11||slot==23?noise:high*0.55)*env;}
                const double fade=std::min(1.0,t/0.0005)*std::min(1.0,(0.5-t)/0.01);
                out.data[i]=static_cast<float>(std::clamp(v*fade*0.7,-1.0,1.0));
            }
        }
    }
    bool m_chromatic=true;
    double m_sampleRate = 44100.0;
    std::vector<SampleSlot> m_slots;
    size_t m_activeSlot = 0;
    size_t m_currentPlayingSlot = 0,m_hitAge=0;
    // 1.6.12: адаптивная длина деклик-фейда от амплитуды скачка в точке старта
    // (2 сэмпла...~4 мс): транзиент не убивается, сильный клик убран.
    void updateHitFade(const SampleSlot& slot){const auto& d=slot.data;const size_t i=d.empty()?0:static_cast<size_t>(std::clamp(m_playbackPos,0.0,static_cast<double>(d.size()-1)));const float v0=d.empty()?0.0f:d[i];m_hitFade=static_cast<int>(std::clamp(std::lround(2.0+std::abs(v0)*88.0),2L,176L));}
    int m_hitFade=16;
    bool m_isPlaying = false;
    double m_playbackPos = 0.0;
    float m_bendRatio = 1.0f;
    float m_pitchRatio = 1.0f;
    float m_keyTrackRatio = 1.0f;

    uint8_t m_paramPtch = 64;
    uint8_t m_paramStrt = 0;
    uint8_t m_paramRtrg = 0;
    uint8_t m_paramRtim = 32;
    bool m_randomSlot = false;
    bool m_randomStart = false;

    uint8_t m_retrigTotal = 0;
    uint8_t m_retrigCounter = 0;
    size_t m_retrigIntervalSamples = 2000;
    size_t m_retrigSamplesRemaining = 0;

    std::mt19937 m_rng;
};

} // namespace monomachine
