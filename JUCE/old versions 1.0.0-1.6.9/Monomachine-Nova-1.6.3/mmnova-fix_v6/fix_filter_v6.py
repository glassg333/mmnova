#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# =============================================================================
# Monomachine Nova 1.6.3 -- правка v6 (чистка режимов + старый BBOX + 1.6.0 для хоруса)
# =============================================================================
# Требуется дерево, в котором уже применена правка v5 (в репозитории это 1.6.3).
# Что делает (оба проекта: Synth и FX):
#   1. dsp/monomachine_bbox.hpp -- РАБОЧИЙ кит = старый кит (крепкий кик, клэп,
#      колокол, лазер/Blipp) + новые Snare Drum 1, Tom 1, Tom 2 (слоты 2/3/4).
#      RTIM: старая формула 5...255 мс, но инвертированная -> rtim 127 = самый частый
#      повтор; интервал НИКОГДА не 0, поэтому на 100...127 звук больше не пропадает.
#      Клик на перезапуске убран сглаживанием по СЭМПЛАМ (16 сэмплов, спад ровно до нуля).
#   2. models/DspModes.hpp -- режимов всего два (mnm основной, old резерв), свой список
#      у каждого раздела; новые режимы остались только у фильтра.
#   3. NovaDSP.h -- удалены альтернативные движки dist2 / fm2 / bbox2 (ветки, объекты,
#      вызовы); хорус и FX возвращены к 1.6.0: линейный INP, без домешиваний x1.5 / x3.0.
#   4. PluginProcessor.cpp -- схема состояния 10, старые индексы режимов сводятся к mnm.
#   5. MnmDist2.hpp / MnmFm2.hpp / MnmBbox2.hpp -- выводятся из сборки (*.bak-v6).
#   6. tests/DeliveryTests.cpp -- ожидания под новый закон RTIM и новый кит.
#
# Запуск:  python fix_filter_v6.py "E:\\mm\\Monomachine-Nova-1.6.3"
#          python fix_filter_v6.py "E:\\mm\\Monomachine-Nova-1.6.3" --check
# Оригиналы сохраняются рядом как *.bak-v6 (один раз). Повторный запуск безвреден.
# =============================================================================

BBOX_V6 = "// Nova v6: рабочий кит BBOX = СТАРЫЙ (до реворка 1.4) + снейр и два тома из нового,\n//          старый закон RTIM с инверсией, сглаживание повторов по СЭМПЛАМ.\n// -----------------------------------------------------------------------------\n// ЧТО ИЗМЕНИЛОСЬ В v6 (мономашина-нова, раздел BBOX)\n//   1. RTIM: возвращена СТАРАЯ формула (1.0-1.3.1) 5...255 мс -- но ИНВЕРТИРОВАННАЯ,\n//      как просили: rtim=0 -> 255 мс (реже всего), rtim=127 -> 5 мс (чаще всего).\n//      Интервал повтора теперь НИКОГДА не 0 (было 255*((127-rtim)/126)^2 -> 0 мс),\n//      поэтому звук не пропадает и повторы не сливаются в тишину на 100...127.\n//   2. Затухание повторов оставлено из новой версии: gain = 10^(-0.75*counter/total).\n//   3. Клик на перезапуске сэмпла убран сглаживанием ПО СЭМПЛАМ (kDeclickSamples=8,\n//      ~0.18 мс при 44.1 кГц): короткий фейд в начале удара + спад перед повтором.\n//      Миллисекундных огибающих в тракте повтора больше нет.\n//   4. STRT (start position): смещение мягче (1/8 активной длины вместо 1/4),\n//      короткие удары больше не срезаются так сильно; предел 30 мс сохранён.\n//   5. Кит по умолчанию: ОСНОВНОЙ=старый кит (кик, клэп, колокол, Blipp/лазер,\n//      остальное) + три голоса нового кита: Snare Drum 1 (слот 2), Tom 1 (слот 3),\n//      Tom 2 (слот 4). Полностью старый кит остался в generateLegacyKit().\n#pragma once\n\n#include <vector>\n#include <string>\n#include <cmath>\n#include <algorithm>\n#include <random>\n#include <cstdint>\n#include <utility>\n\nnamespace monomachine {\n\n/**\n * @brief Elektron Monomachine DPRO-BBOX (Machine 7) & Extended Sampler Engine.\n * \n * Hardware Architecture:\n * - DPRO-BBOX is the drum synthesizer/beatbox machine of the Monomachine.\n * - Hardware parameters:\n *   - PTCH (Pitch): Sample playback pitch / rate (-64 .. +63 semitone/fine ratio)\n *   - STRT (Start): Sample playback start offset (0 .. 127 -> bounded quadratic offset, up to 30 ms)\n *   - RTRG (Retrig Count): Number of rapid retrigger repeats (0 = off, 1 .. 127)\n *   - RTIM (Retrig Time): Time between retriggers (tempo synchronized / milliseconds)\n * \n * Extension for Modern DAW / VST3:\n * - 24 Sample Slots with dynamic loading.\n * - Sample randomization mode (random slot selection and/or random start position).\n * - Linear / Hermite sample interpolation with pitch tracking.\n */\nclass MonomachineBBox {\npublic:\n    static constexpr size_t kMaxSlots = 24;\n    // Длина сглаживания при перезапуске удара -- в СЭМПЛАХ, а не в миллисекундах:\n    // 16 сэмплов = 0.36 мс при 44.1 кГц. Спад ДОХОДИТ ровно до нуля к моменту\n    // перезапуска, поэтому перезапуск не даёт щелчка (раньше позиция прыгала скачком).\n    static constexpr size_t kDeclickSamples = 16;\n\n    struct SampleSlot {\n        std::string name;\n        std::vector<float> data;\n        double originalSampleRate = 44100.0;\n        bool loaded = false;\n        size_t activeFrames=0;\n    };\n\n    static void analyseSlot(SampleSlot& slot){float peak=0;for(float v:slot.data)peak=std::max(peak,std::abs(v));slot.activeFrames=slot.data.size();while(slot.activeFrames>1&&std::abs(slot.data[slot.activeFrames-1])<peak*0.0001f)--slot.activeFrames;}\n    int playingSlot()const noexcept{return m_isPlaying?static_cast<int>(m_currentPlayingSlot):-1;}\n    size_t retriggerInterval()const noexcept{return m_retrigIntervalSamples;}\n    double startPosition(const SampleSlot& slot)const{const double x=static_cast<double>(m_paramStrt)/127;return x*x*std::min(static_cast<double>(slot.activeFrames?slot.activeFrames:slot.data.size())*0.125,slot.originalSampleRate*0.030);}\n    MonomachineBBox(bool modern=true) {\n        m_slots.resize(kMaxSlots);\n        m_rng.seed(1337);\n        if(modern)generateDefaultDrumKit();else generateLegacyKit();\n        for(auto& slot:m_slots)analyseSlot(slot);\n    }\n\n    void reset(double sampleRate = 44100.0) {\n        m_sampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;\n        m_playbackPos = 0.0;\n        m_isPlaying = false;\n        m_retrigCounter = 0;\n        m_retrigSamplesRemaining = 0;\n    }\n\n    /**\n     * @brief Load a raw mono sample into one of the 24 slots (0..23).\n     */\n    bool loadSample(size_t slotIndex, const std::string& name, const float* samples, size_t numSamples, double sampleRate = 44100.0) {\n        if (slotIndex >= kMaxSlots || samples == nullptr || numSamples == 0) return false;\n        m_slots[slotIndex].name = name;\n        m_slots[slotIndex].data.assign(samples, samples + numSamples);\n        m_slots[slotIndex].originalSampleRate = sampleRate;\n        m_slots[slotIndex].loaded = true;analyseSlot(m_slots[slotIndex]);\n        return true;\n    }\n\n    /**\n     * @brief Monomachine BBox hardware parameter setter.\n     */\n    void setParameters(uint8_t ptch, uint8_t strt, uint8_t rtrg, uint8_t rtim,\n                       uint8_t slotSelect = 0, bool randomSlot = false, bool randomStrt = false) {\n        m_paramPtch = ptch;\n        m_paramStrt = strt;\n        m_paramRtrg = rtrg;\n        m_paramRtim = rtim;\n        m_activeSlot = slotSelect % kMaxSlots;\n        m_randomSlot = randomSlot;\n        m_randomStart = randomStrt;\n\n        // Pitch multiplier: 0 -> -2 octaves (0.25x), 64 -> 1.0x (normal), 127 -> +2 octaves (4.0x)\n        float semitones = (static_cast<float>(ptch) - 64.0f) / 64.0f * 24.0f;\n        m_pitchRatio = std::pow(2.0f, semitones / 12.0f);\n\n        // Retrig interval in samples.\n        // v6: СТАРАЯ формула 1.0-1.3.1 (5.0 + 250.0*rtim/127 = 5...255 мс), но по\n        // просьбе ИНВЕРТИРОВАННАЯ: rtim=0 -> 255 мс, rtim=127 -> 5 мс.\n        // Главное: интервал никогда не равен нулю, поэтому на 100...127 повторы\n        // не превращаются в вечную перезагрузку сэмпла (это и съедало звук).\n        float retrigMs = 5.0f + ((127.0f - static_cast<float>(rtim)) / 127.0f) * 250.0f;\n        m_retrigIntervalSamples = std::max(size_t(1),static_cast<size_t>((retrigMs / 1000.0f) * m_sampleRate));\n    }\n\n    /**\n     * @brief Trigger note on / drum hit.\n     */\n    void setChromatic(bool enabled) noexcept { m_chromatic=enabled; }\n    static int chromaticSlot(int note) noexcept { const int n=note-48;return (n%24+24)%24; }\n    static int chromaticOctave(int note) noexcept {const int n=note-48;return (n-chromaticSlot(note))/24;}\n    void noteOn(uint8_t midiNote = 60, uint8_t velocity = 127) {\n        (void)velocity;\n        size_t targetSlot = m_chromatic?static_cast<size_t>(chromaticSlot(midiNote)):m_activeSlot;\n\n        if (m_randomSlot) {\n            std::uniform_int_distribution<size_t> dist(0, kMaxSlots - 1);\n            targetSlot = dist(m_rng);\n        }\n\n        m_currentPlayingSlot = targetSlot;\n        auto& slot = m_slots[m_currentPlayingSlot];\n        if (!slot.loaded || slot.data.empty()) {\n            m_isPlaying = false;\n            return;\n        }\n\n        float startFrac = static_cast<float>(m_paramStrt) / 127.0f;\n        if (m_randomStart) {\n            std::uniform_real_distribution<float> dist(0.0f, 0.85f);\n            startFrac = dist(m_rng);\n        }\n\n        m_playbackPos = m_randomStart?startFrac*static_cast<double>(slot.data.size()):startPosition(slot);\n        m_hitAge=0;\n        m_isPlaying = true;\n\n        // Retrigger setup\n        m_retrigTotal = m_paramRtrg;\n        m_retrigCounter = 0;\n        m_retrigSamplesRemaining = m_retrigIntervalSamples;\n\n        // Key tracking pitch offset (Middle C / 60 is neutral)\n        float noteSemitones = m_chromatic?12.0f*static_cast<float>(chromaticOctave(midiNote)):static_cast<float>(midiNote)-60.0f;\n        m_keyTrackRatio = std::pow(2.0f, noteSemitones / 12.0f);\n    }\n\n    /**\n     * @brief Process a block of stereo samples.\n     */\n    void processStereo(float* outL, float* outR, size_t numFrames) {\n        if (!m_isPlaying || m_currentPlayingSlot >= kMaxSlots) {\n            for (size_t i = 0; i < numFrames; ++i) {\n                outL[i] = 0.0f;\n                outR[i] = 0.0f;\n            }\n            return;\n        }\n\n        const auto& slot = m_slots[m_currentPlayingSlot];\n        const auto& data = slot.data;\n        const size_t dataSize = data.size();\n        const double rateFactor = (slot.originalSampleRate / m_sampleRate) * m_pitchRatio * m_keyTrackRatio * m_bendRatio;\n\n        const size_t declick = std::max<size_t>(1, std::min(kDeclickSamples, m_retrigIntervalSamples / 2));\n\n        for (size_t i = 0; i < numFrames; ++i) {\n            if (!m_isPlaying) {\n                outL[i] = 0.0f;\n                outR[i] = 0.0f;\n                continue;\n            }\n\n            // Check Retrigger\n            size_t samplesToRestart = 0;   // сколько сэмплов осталось ДО перезапуска\n            if (m_retrigTotal > 0 && m_retrigCounter < m_retrigTotal) {\n                samplesToRestart = m_retrigSamplesRemaining;\n                if (m_retrigSamplesRemaining == 0) {\n                    m_playbackPos = startPosition(slot);m_hitAge=0;\n                    m_retrigSamplesRemaining = m_retrigIntervalSamples;\n                    ++m_retrigCounter;\n                }\n                if(m_retrigSamplesRemaining>0)--m_retrigSamplesRemaining;\n            }\n\n            // Interpolated sample read\n            size_t idx0 = static_cast<size_t>(m_playbackPos);\n            if (idx0 >= dataSize) {\n                if (m_retrigCounter >= m_retrigTotal) m_isPlaying = false;\n                outL[i] = 0.0f;\n                outR[i] = 0.0f;\n                continue;\n            }\n\n            size_t idx1 = std::min(idx0 + 1, dataSize - 1);\n            float frac = static_cast<float>(m_playbackPos - idx0);\n            float sample = data[idx0] + frac * (data[idx1] - data[idx0]);\n\n            // Сглаживание в СЭМПЛАХ: фейд в начале удара + спад перед перезапуском.\n            // Раньше здесь был миллисекундный fadeFrames (~22 сэмпла), который\n            // заметно съедал атаку и не спасал от щелчка в момент перезапуска.\n            const float fadeIn=static_cast<float>(std::min(1.0,static_cast<double>(m_hitAge++)/static_cast<double>(declick)));\n            float fadeOut=1.0f;\n            if(m_retrigTotal>0&&m_retrigCounter<m_retrigTotal&&samplesToRestart<declick)\n                fadeOut=static_cast<float>(samplesToRestart)/static_cast<float>(declick);\n            const float fade=std::min(fadeIn,fadeOut);\n            // Затухание серии повторов -- из новой версии (10^(-0.75*counter/total)).\n            const float gain=m_retrigTotal?std::pow(10.0f,-0.75f*static_cast<float>(m_retrigCounter)/m_retrigTotal):1.0f;\n            outL[i] = sample*gain*fade;\n            outR[i] = outL[i];\n\n            m_playbackPos += rateFactor;\n        }\n    }\n\n    void setPitchBend(float semitones) { m_bendRatio = std::pow(2.0f, semitones / 12.0f); }\n    // Caller supplies an already decoded slot; allocation/destruction happen off the audio thread.\n    void swapSample(size_t slot, SampleSlot& prepared) { std::swap(m_slots.at(slot), prepared); m_isPlaying = false; }\n    const std::vector<SampleSlot>& getSlots() const noexcept { return m_slots; }\n\nprivate:\n    // РАБОЧИЙ кит v6: основа -- СТАРЫЙ кит (до реворка 1.4), он же звучал как надо:\n    // кик, клэп, колокол (Cow Bell), лазер/zap (Blipp) и остальное. Из нового кита\n    // взяты только три голоса, которые понравились: Snare Drum 1 (слот 2),\n    // Tom 1 (слот 3), Tom 2 (слот 4) -- см. таблицу modernVoice ниже.\n    // Индексы слотов, которые берутся из нового кита (0-based).\n    void generateDefaultDrumKit() {\n        static const char* names[24]={\"Bass Drum 1\",\"Snare Drum 1\",\"Tom 1\",\"Tom 2\",\"Bongo Congo\",\"Clap\",\n            \"Rim Shot\",\"Cow Bell\",\"Closed HiHat\",\"Open HiHat\",\"Ride Cymbal\",\"Crash Cymbal\",\n            \"Bass Drum 2\",\"Snare Drum 2\",\"Timbale\",\"Aggobell\",\"Timpani\",\"Snap\",\n            \"Wood\",\"Triangle\",\"Shaker\",\"Maracas\",\"Whistle\",\"Blipp\"};\n        const size_t type[24]={0,1,3,15,18,6,4,8,2,7,19,19,12,13,15,20,3,5,16,20,14,14,20,10};\n        // true = голос из НОВОГО кита (снейр -- слот 2, Tom 1 -- слот 3, Tom 2 -- слот 4),\n        // false = СТАРЫЙ кит (кик, клэп, колокол Cow Bell, лазер Blipp и всё остальное).\n        static const bool modernVoice[24]={false,true,true,true,false,false,false,false,false,false,false,false,\n                                           false,false,false,false,false,false,false,false,false,false,false,false};\n        constexpr double pi=3.14159265358979323846;\n        for(size_t index=0;index<kMaxSlots;++index){const size_t slot=type[index];auto& out=m_slots[index];out.loaded=true;out.originalSampleRate=44100;out.name=names[index];\n            out.data.resize(22050);double phase=0,phase2=0;uint32_t seed=0x1234567u+static_cast<uint32_t>(index)*7919u;float previous=0;\n            for(size_t i=0;i<out.data.size();++i){const double t=static_cast<double>(i)/44100;\n                seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;const float noise=static_cast<float>(seed)/2147483648.0f-1;\n                const double decay=(slot==7||slot==19||slot==23)?0.17:0.035+0.009*static_cast<double>(slot%7);\n                const double env=std::exp(-t/decay);double v=0;\n                if(index==22){phase+=2*pi*2100/44100;v=std::sin(phase)*std::exp(-t/0.12);}\n                else if(index==10){phase+=2*pi*1830/44100;phase2+=2*pi*2947/44100;v=(0.22*(std::sin(phase)+std::sin(phase2))+noise*0.20)*std::exp(-t/0.22);}\n                else if(index==11){const float high=noise-previous;previous=noise;v=high*0.5*std::exp(-t/0.30);}\n                else if(index==20){const float high=noise-previous;previous=noise;v=high*0.6*std::exp(-t/0.045);}\n                else if(index==21){v=noise*(0.5+0.5*std::sin(2*pi*95*t))*std::exp(-t/0.095);}\n                else if(slot==0||slot==12){phase+=2*pi*(slot==0?45:65)/44100+0.10*std::exp(-t/0.008);v=std::sin(phase)*std::exp(-t/(slot==0?0.13:0.075))+noise*std::exp(-t/0.002)*0.12;}\n                else if(slot==1||slot==13||slot==5){\n                    if(slot!=5&&modernVoice[index]){phase+=2*pi*(slot==1?185:240)/44100;const double body=std::sin(phase+0.65*std::exp(-t/.016)*std::sin(phase*1.83));const double high=noise-previous;previous=noise;v=0.6*body*std::exp(-t/.024)+high*.35*std::exp(-t/(slot==1?.032:.045));}\n                    else {phase+=2*pi*(slot==5?430:190)/44100;v=(0.25*std::sin(phase)+noise*0.75)*env;}}\n                else if(slot==6||slot==17){double burst=t<0.03?(std::fmod(t,slot==6?0.009:0.012)<0.004?1.0:0.1):0.35;v=noise*burst*env;}\n                else if(slot==3||slot==15||slot==18){\n                    phase+=2*pi*((index==16?65:slot==3?85:index==14?290:slot==15?175:230)+90*std::exp(-t/.015))/44100;\n                    v=modernVoice[index]?(std::sin(phase+1.8*std::exp(-t/.04)*std::sin(phase*1.414))+.16*std::sin(phase*2.76))*env:std::sin(phase)*env;}\n                else if(slot==4||slot==16||slot==22){phase+=2*pi*(slot==4?850:slot==16?1750:3500)/44100;v=(std::sin(phase)+0.4*std::sin(phase*1.67))*std::exp(-t/0.014);}\n                else if(slot==8||slot==9||slot==20){phase+=2*pi*(index==19?2200:slot==8?540:slot==9?870:1300)/44100;phase2+=2*pi*(slot==8?800:slot==9?1237:1743)/44100;v=(std::sin(phase)+0.6*std::sin(phase2))*env*0.65;}\n                else if(slot==10||slot==21){const double hz=slot==10?90+2200*std::exp(-t/0.025):100+2300*(1-std::exp(-t/0.07));phase+=2*pi*hz/44100;v=std::sin(phase+0.7*std::sin(phase*2.31))*env;}\n                else {const float high=noise-previous;previous=noise;v=(slot==11||slot==23?noise:high*0.55)*env;}\n                const double fade=std::min(1.0,t/0.0005)*std::min(1.0,(0.5-t)/0.01);\n                out.data[i]=static_cast<float>(std::clamp(v*fade*0.7,-1.0,1.0));\n            }\n        }\n    }\n    void generateLegacyKit() {\n        static const char* names[24]={\"Bass Drum 1\",\"Snare Drum 1\",\"Tom 1\",\"Tom 2\",\"Bongo Congo\",\"Clap\",\n            \"Rim Shot\",\"Cow Bell\",\"Closed HiHat\",\"Open HiHat\",\"Ride Cymbal\",\"Crash Cymbal\",\n            \"Bass Drum 2\",\"Snare Drum 2\",\"Timbale\",\"Aggobell\",\"Timpani\",\"Snap\",\n            \"Wood\",\"Triangle\",\"Shaker\",\"Maracas\",\"Whistle\",\"Blipp\"};\n        const size_t type[24]={0,1,3,15,18,6,4,8,2,7,19,19,12,13,15,20,3,5,16,20,14,14,20,10};\n        constexpr double pi=3.14159265358979323846;\n        for(size_t index=0;index<kMaxSlots;++index){const size_t slot=type[index];auto& out=m_slots[index];out.loaded=true;out.originalSampleRate=44100;out.name=names[index];\n            out.data.resize(22050);double phase=0,phase2=0;uint32_t seed=0x1234567u+static_cast<uint32_t>(index)*7919u;float previous=0;\n            for(size_t i=0;i<out.data.size();++i){const double t=static_cast<double>(i)/44100;\n                seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;const float noise=static_cast<float>(seed)/2147483648.0f-1;\n                const double decay=(slot==7||slot==19||slot==23)?0.17:0.035+0.009*static_cast<double>(slot%7);\n                const double env=std::exp(-t/decay);double v=0;\n                if(index==22){phase+=2*pi*2100/44100;v=std::sin(phase)*std::exp(-t/0.12);}\n                else if(index==10){phase+=2*pi*1830/44100;phase2+=2*pi*2947/44100;v=(0.22*(std::sin(phase)+std::sin(phase2))+noise*0.20)*std::exp(-t/0.22);}\n                else if(index==11){const float high=noise-previous;previous=noise;v=high*0.5*std::exp(-t/0.30);}\n                else if(index==20){const float high=noise-previous;previous=noise;v=high*0.6*std::exp(-t/0.045);}\n                else if(index==21){v=noise*(0.5+0.5*std::sin(2*pi*95*t))*std::exp(-t/0.095);}\n                else if(slot==0||slot==12){phase+=2*pi*(slot==0?45:65)/44100+0.10*std::exp(-t/0.008);v=std::sin(phase)*std::exp(-t/(slot==0?0.13:0.075))+noise*std::exp(-t/0.002)*0.12;}\n                else if(slot==1||slot==13||slot==5){phase+=2*pi*(slot==5?430:190)/44100;v=(0.25*std::sin(phase)+noise*0.75)*env;}\n                else if(slot==6||slot==17){double burst=t<0.03?(std::fmod(t,slot==6?0.009:0.012)<0.004?1.0:0.1):0.35;v=noise*burst*env;}\n                else if(slot==3||slot==15||slot==18){phase+=2*pi*((index==16?65:slot==3?85:index==14?290:slot==15?175:230)+90*std::exp(-t/0.015))/44100;v=std::sin(phase)*env;}\n                else if(slot==4||slot==16||slot==22){phase+=2*pi*(slot==4?850:slot==16?1750:3500)/44100;v=(std::sin(phase)+0.4*std::sin(phase*1.67))*std::exp(-t/0.014);}\n                else if(slot==8||slot==9||slot==20){phase+=2*pi*(index==19?2200:slot==8?540:slot==9?870:1300)/44100;phase2+=2*pi*(slot==8?800:slot==9?1237:1743)/44100;v=(std::sin(phase)+0.6*std::sin(phase2))*env*0.65;}\n                else if(slot==10||slot==21){const double hz=slot==10?90+2200*std::exp(-t/0.025):100+2300*(1-std::exp(-t/0.07));phase+=2*pi*hz/44100;v=std::sin(phase+0.7*std::sin(phase*2.31))*env;}\n                else {const float high=noise-previous;previous=noise;v=(slot==11||slot==23?noise:high*0.55)*env;}\n                const double fade=std::min(1.0,t/0.0005)*std::min(1.0,(0.5-t)/0.01);\n                out.data[i]=static_cast<float>(std::clamp(v*fade*0.7,-1.0,1.0));\n            }\n        }\n    }\n    bool m_chromatic=true;\n    double m_sampleRate = 44100.0;\n    std::vector<SampleSlot> m_slots;\n    size_t m_activeSlot = 0;\n    size_t m_currentPlayingSlot = 0,m_hitAge=0;\n    bool m_isPlaying = false;\n    double m_playbackPos = 0.0;\n    float m_bendRatio = 1.0f;\n    float m_pitchRatio = 1.0f;\n    float m_keyTrackRatio = 1.0f;\n\n    uint8_t m_paramPtch = 64;\n    uint8_t m_paramStrt = 0;\n    uint8_t m_paramRtrg = 0;\n    uint8_t m_paramRtim = 32;\n    bool m_randomSlot = false;\n    bool m_randomStart = false;\n\n    uint8_t m_retrigTotal = 0;\n    uint8_t m_retrigCounter = 0;\n    size_t m_retrigIntervalSamples = 2000;\n    size_t m_retrigSamplesRemaining = 0;\n\n    std::mt19937 m_rng;\n};\n\n} // namespace monomachine\n"

DSP_MODES_V6 = "#pragma once\n// DSP mode registry -- Monomachine Nova 1.6.3 (ревизия v6)\n// -----------------------------------------------------------------------------\n// ГЛАВНОЕ ПРАВИЛО: в каждом разделе свой короткий список режимов,\n// первым всегда основной \"mnm\".\n//\n// ЧТО ИЗМЕНИЛОСЬ В v6\n//   * Альтернативные движки dist2 / fm2 / bbox2 УДАЛЕНЫ вместе с кодом\n//     (MnmDist2.hpp, MnmFm2.hpp, MnmBbox2.hpp выведены из сборки в *.bak-v6).\n//     Осталось ровно два режима: mnm (основной) и old (прежнее приближение Nova).\n//   * Новые режимы теперь есть ТОЛЬКО у фильтра: FILT mnm = реальный фильтр из дампа\n//     прошивки (MnmRealFilter.hpp, таблицы kLP 258 коэффициентов, SVF $F528BD/$4A4DF0).\n//     Все остальные разделы -- как в 1.6.0: их прежние алгоритмы получили режим old.\n//   * Схема состояния 9 -> 10: старые индексы режимов (2/3/4 для новых и 5/6/7 для\n//     прежних нумераций) сводятся к основному mnm, см. dspModeLegacyIndexToCurrent.\n//\n// Числа режимов:\n// 0 mnm -- ОСНОВНОЙ: реальный фильтр из прошивки (FILT), ядра прошивки для AMP/DIST/DLY/SYNT\n// 1 old -- РЕЗЕРВ: прежние приближения Nova (biquad-фильтр, tanh-дист, линейный дилей...)\n\n#include <array>\n#include <cstdint>\n#include <cstring>\n\nnamespace monomachine {\n\nenum DspSection : int {\n    DspSynt = 0,\n    DspAmp,\n    DspFilter,\n    DspDist,\n    DspDelay,\n    DspRouting,\n    DspChorus,\n    DspSectionCount\n};\n\ninline const char* dspModeParamId(int section) {\n    switch (section) {\n        case DspSynt:   return \"mode_synt\";\n        case DspAmp:    return \"mode_amp\";\n        case DspFilter: return \"mode_filt\";\n        case DspDist:   return \"mode_dist\";\n        case DspDelay:  return \"mode_dly\";\n        case DspRouting:return \"mode_route\";\n        case DspChorus: return \"mode_cho\";\n        default:        return \"mode_unknown\";\n    }\n}\n\ninline const char* dspSectionLabel(int section) {\n    switch (section) {\n        case DspSynt:   return \"SYNT\";\n        case DspAmp:    return \"AMP\";\n        case DspFilter: return \"FILT\";\n        case DspDist:   return \"DIST\";\n        case DspDelay:  return \"DLY\";\n        case DspRouting:return \"ROUTE\";\n        case DspChorus: return \"CHOR\";\n        default:        return \"?\";\n    }\n}\n\n// Полный список режимов. Нужен параметру AudioParameterChoice (значение = индекс),\n// поэтому порядок здесь = нумерация выше и меняться не должен.\ninline const char* dspModeChoices() { return \"mnm|old\"; }\ninline constexpr int dspModeMnm = 0;\ninline constexpr int dspModeOld = 1;\ninline constexpr int dspModeCount = 2;\n\n// Версия схемы состояния: 9 -> 10 из-за удаления режимов dist2/fm2/bbox2.\ninline constexpr int kDspModeSchemaVersion = 10;\n\ninline const char* dspModeName(int mode) {\n    switch (mode) {\n        case 0: return \"mnm\";\n        case 1: return \"old\";\n        default: return \"?\";\n    }\n}\ninline const char* dspModeLabel(int mode) { return dspModeName(mode); }\n\ninline int dspModeIndexByName(const char* name) {\n    if (name == nullptr) return -1;\n    for (int m = 0; m < dspModeCount; ++m)\n        if (std::strcmp(dspModeName(m), name) == 0) return m;\n    return -1;\n}\n\n// Перевод индексов режимов из состояний с прежними схемами (8 и меньше):\n//   0 mnm -> mnm, 1 old -> old,\n//   2/3/4 (cascade/dual/raw из старого патча) и 5/6/7 (dist2/fm2/bbox2) -> mnm.\n// Так ни одно сохранённое состояние не остаётся с несуществующим режимом.\ninline int dspModeLegacyIndexToCurrent(int legacy) noexcept {\n    switch (legacy) {\n        case 0: return dspModeMnm;\n        case 1: return dspModeOld;\n        default: return dspModeMnm;\n    }\n}\n\n// false -> в меню остаются только основные (mnm) режимы, резерв полностью скрыт.\ninline constexpr bool kShowBackupModes = true;\n\n// -----------------------------------------------------------------------------\n// Списки режимов ПО РАЗДЕЛАМ. Первым всегда \"mnm\" (основной режим раздела).\n// Только эти списки решают, что можно включить в разделе: и меню, и аудио-поток\n// читают их.\n// -----------------------------------------------------------------------------\ninline const char* dspSectionModeChoices(int section) {\n    switch (section) {\n        case DspSynt:    return \"mnm|old\";   // синт: FmCore прошивки, резерв -- прежний Nova FM\n        case DspAmp:     return \"mnm|old\";   // амплитуда: kernel ADSR-таблицы\n        case DspFilter:  return \"mnm|old\";   // фильтр: РЕАЛЬНЫЙ дамп прошивки, резерв -- прежнее приближение\n        case DspDist:    return \"mnm|old\";   // дист: ALU-сатурация прошивки, резерв -- прежний bipolarDist\n        case DspDelay:   return \"mnm|old\";   // дилей: m32 FX-DLY\n        case DspRouting: return \"mnm\";       // цепочка проверена и она одна\n        case DspChorus:  return \"mnm\";       // хорус: единственная реализация (ядро OS 1.32B)\n        default:         return \"mnm\";\n    }\n}\n\ninline int dspSectionModeCount(int section) {\n    const char* list = dspSectionModeChoices(section);\n    int n = 0;\n    for (const char* p = list; *p != 0; ++p) if (*p == '|') ++n;\n    return n + 1;\n}\n\n// i-й режим раздела (0 = основной mnm). Если индекса нет -- возвращает mnm.\ninline int dspSectionModeAt(int section, int index) {\n    const char* list = dspSectionModeChoices(section);\n    int i = 0;\n    const char* start = list;\n    for (const char* p = list; ; ++p) {\n        if (*p == '|' || *p == 0) {\n            if (i == index) {\n                std::array<char, 16> token{};\n                const std::size_t len = static_cast<std::size_t>(p - start);\n                const std::size_t n = len < token.size() - 1 ? len : token.size() - 1;\n                for (std::size_t k = 0; k < n; ++k) token[k] = start[k];\n                const int mode = dspModeIndexByName(token.data());\n                return mode < 0 ? dspModeMnm : mode;\n            }\n            ++i;\n            if (*p == 0) break;\n            start = p + 1;\n        }\n    }\n    return dspModeMnm;\n}\n\n// Единственное правило допустимости: режим должен быть в списке своего раздела.\ninline bool dspModeAllowedForSection(int section, int mode) {\n    const int n = dspSectionModeCount(section);\n    for (int i = 0; i < n; ++i) if (dspSectionModeAt(section, i) == mode) return true;\n    return false;\n}\n\n// Основной режим раздела -- всегда mnm и всегда доступен.\ninline constexpr int dspModePrimaryForSection(int) { return dspModeMnm; }\n\n// Совместимость с прежним кодом.\ninline constexpr int dspModeFilter = DspFilter;\n\n// -----------------------------------------------------------------------------\n// Тексты (меню/тултипы). Константы резонанса -- реальные из прошивки:\n// $F528BD = -0.084747577, $4A4DF0 = +0.581161499.\n// -----------------------------------------------------------------------------\ninline const char* dspModeProvenance(int section) {\n    switch (section) {\n        case DspSynt:   return \"SYNT: mnm=восстановленное ядро OS1.32B (P:$4A8-$4F5, tone LP P:$144AC7). Резерв: old=прежний Nova FM\";\n        case DspAmp:    return \"AMP: kernel P:$4A8-$4F5 ADSR FSM, таблицы Y:$141800/Y:$141880, LFO $791FD0=0.94628\";\n        case DspFilter: return \"FILT: mnm=РЕАЛЬНЫЙ фильтр из дампа прошивки (kLP 258 коэффициентов P:$144AC7, SVF $F528BD=-0.0847476 $4A4DF0=+0.5811615, AD-огибающая BOFS/WOFS). Резерв: old=прежнее приближение\";\n        case DspDist:   return \"DIST: mnm=ALU-сатурация прошивки bset #$14,sr (hardclip +-1.0). Резерв: old=прежний bipolarDist\";\n        case DspDelay:  return \"DLY: mnm=m32 FX-DLY X:$114000 + P:$144C49. Резерв: old=линейная интерполяция\";\n        case DspRouting:return \"ROUTE: mnm=проверенная цепочка synth->dist->srr->filt+eq->env->dsnd\";\n        case DspChorus: return \"CHOR: mnm=нативное ядро OS1.32B 24-bit (как в 1.6.0), INP линейный, микс dry->wet по MIX\";\n        default:        return \"\";\n    }\n}\n\ninline const char* dspModeProvenanceForMode(int section, int mode) {\n    if (mode == dspModeMnm) {\n        switch (section) {\n            case DspSynt:   return \"SYNT mnm (основной): восстановленное ядро OS1.32B (P:$4A8-$4F5, tone LP P:$144AC7)\";\n            case DspAmp:    return \"AMP mnm (основной): kernel ADSR FSM Y:$141800/Y:$141880\";\n            case DspFilter: return \"FILT mnm (ОСНОВНОЙ): реальный дамп -- kLP 258 коэффициентов + SVF $F528BD=-0.0847476 $4A4DF0=+0.5811615, Q 0.5..16, AD-огибающая BOFS/WOFS, при дефолте unity THRU\";\n            case DspDist:   return \"DIST mnm (основной): ALU-сатурация прошивки bset #$14,sr\";\n            case DspDelay:  return \"DLY mnm (основной): m32 FX-DLY X:$114000 + P:$144C49\";\n            case DspRouting:return \"ROUTE mnm (основной): synth->dist->srr->filt+eq->env->dsnd (проверено)\";\n            case DspChorus: return \"CHOR mnm (основной): нативное ядро OS1.32B 24-bit, линейный INP, микс dry->wet\";\n            default:        return \"mnm (основной): восстановленная прошивка\";\n        }\n    }\n    if (mode == dspModeOld) return \"old (резерв): прежнее приближение Nova (фильтр -- biquad, дист -- tanh/bipolar, дилей -- линейная интерполяция)\";\n    return \"?\";\n}\n\n}  // namespace monomachine\n"

VERIFY_PY_V6 = "#!/usr/bin/env python3\n\"\"\"Static checks for the DSP MODE / filter modes patch (revision v6).\n\nКомпилятор не нужен: скрипт читает исходники и проверяет обещания правки.\n\n  1. dsp/mnm/MnmRealFilter.hpp -- основной фильтр: реальные таблицы, устойчивый\n     TPT-SVF, DC-блокер, порядок аргументов страницы FILT, живое отпускание ноты.\n  2. dsp/mnm/MnmKernel.hpp -- дубль FilterCore/real_detail удалён, std::clamp исправлен.\n  3. dsp/mnm/MnmFilterModes.hpp (cascade/dual/raw) и альтернативные движки\n     MnmDist2.hpp / MnmFm2.hpp / MnmBbox2.hpp -- УДАЛЕНЫ (v5/v6).\n  4. models/DspModes.hpp -- 2 режима (mnm=0 основной, old=1 резерв), свой список\n     режимов у каждого раздела, схема состояния 10.\n  5. NovaDSP.h -- основной фильтр с правильными ручками, резерв old на месте,\n     никаких веток dist2/fm2/bbox2, хорус и FX как в 1.6.0 (линейный INP, без домешиваний).\n  6. dsp/monomachine_bbox.hpp -- BBOX: старый закон RTIM (5...255 мс) с инверсией,\n     интервал никогда не 0, сглаживание по сэмплам, рабочий кит = старый + снейр/томы.\n  7. PluginProcessor.cpp -- одно правило допустимости режима + миграция на схему 10.\n  8. PluginEditor.cpp -- меню: основной режим первым, резерв отдельной группой.\n\nRun:  python3 verify_dsp_mode_patch.py <path-to-Monomachine_Nova_FX/Source>\n\"\"\"\n\nfrom pathlib import Path\nimport re\nimport sys\n\n\ndef read(path: Path) -> str:\n    return path.read_text(encoding=\"utf-8\", errors=\"replace\")\n\n\ndef balanced(text: str) -> bool:\n    return text.count(\"{\") == text.count(\"}\")\n\n\ndef without_comments(text: str) -> str:\n    return \"\\n\".join(line for line in text.splitlines() if not line.lstrip().startswith(\"//\"))\n\n\ndef main() -> int:\n    root = Path(sys.argv[1] if len(sys.argv) > 1 else \".\").resolve()\n    real = root / \"dsp\" / \"mnm\" / \"MnmRealFilter.hpp\"\n    kernel = root / \"dsp\" / \"mnm\" / \"MnmKernel.hpp\"\n    removed = root / \"dsp\" / \"mnm\" / \"MnmFilterModes.hpp\"\n    bbox = root / \"dsp\" / \"monomachine_bbox.hpp\"\n    modes = root / \"models\" / \"DspModes.hpp\"\n    dsp = root / \"NovaDSP.h\"\n    editor = root / \"PluginEditor.cpp\"\n    processor = root / \"PluginProcessor.cpp\"\n\n    for path in (real, kernel, bbox, modes, dsp, editor, processor):\n        if not path.is_file():\n            raise AssertionError(f\"missing file: {path}\")\n\n    tests = root.parent / \"tests\" / \"DeliveryTests.cpp\"\n\n    failures = []\n\n    def check(ok: bool, message: str) -> None:\n        print((\"OK   \" if ok else \"FAIL \") + message)\n        if not ok:\n            failures.append(message)\n\n    def present(text: str, needle: str, message: str) -> None:\n        check(needle in text, message)\n\n    def absent(text: str, needle: str, message: str) -> None:\n        check(needle not in text, message)\n\n    # 1. Основной фильтр\n    text = read(real)\n    check(balanced(text), \"MnmRealFilter.hpp: скобки сбалансированы\")\n    present(text, \"class RealFilterCore\", \"MnmRealFilter.hpp: определён RealFilterCore\")\n    present(text, \"using FilterCore = RealFilterCore\", \"MnmRealFilter.hpp: FilterCore = реальный фильтр\")\n    present(text, \"class MnmResoSVF\", \"MnmRealFilter.hpp: резонанс -- устойчивый TPT-SVF\")\n    present(text, \"DcBlocker\", \"MnmRealFilter.hpp: DC-блокер на выходе активного фильтра\")\n    present(text, \"kMaxQ\", \"MnmRealFilter.hpp: закон Q вынесен в константу kMaxQ\")\n    present(text, \"float envAtk, float envDec, float bofs, float wofs\",\n            \"MnmRealFilter.hpp: порядок аргументов = страница FILT (ATK, DEC, BOFS, WOFS)\")\n    present(text, \"release() noexcept { released_ = true; }\",\n            \"MnmRealFilter.hpp: отпускание ноты возвращает огибающую к нулю\")\n    absent(text, \"bp1 += g1\", \"MnmRealFilter.hpp: старого Chamberlin больше нет\")\n    absent(text, \"class FilterCore {\", \"MnmRealFilter.hpp: нет второго класса FilterCore\")\n\n    # 2. Ядро без дубля\n    text = read(kernel)\n    absent(text, \"class FilterCore {\", \"MnmKernel.hpp: дубль класса FilterCore удалён\")\n    absent(text, \"kRealSVF_x0_F528BD\", \"MnmKernel.hpp: дубль констант SVF удалён\")\n    absent(text, \"namespace real_detail\", \"MnmKernel.hpp: дубль real_detail удалён\")\n    present(text, \"class FilterCoreLegacy\", \"MnmKernel.hpp: резервный FilterCoreLegacy сохранён\")\n    absent(text, \"std::clamp(attack,0,127)\", \"MnmKernel.hpp: std::clamp(float,0,127) исправлен\")\n\n    # 3. Мусорные режимы удалены\n    check(not removed.is_file(), \"dsp/mnm/MnmFilterModes.hpp удалён из сборки (*.bak-v5)\")\n    for name in (\"MnmDist2.hpp\", \"MnmFm2.hpp\", \"MnmBbox2.hpp\"):\n        check(not (root / \"dsp\" / \"mnm\" / name).is_file(),\n              f\"dsp/mnm/{name} удалён из сборки (*.bak-v6)\")\n\n    live = [p for p in (dsp, modes, processor, editor, real, kernel, bbox) ]\n    for word in (\"MnmFilterModes.hpp\", \"SvfCascadeFilter\", \"DualSectionFilter\", \"TruthfulRawFilter\",\n                 \"cascadeFilter\", \"dualFilter\", \"rawFilter\", \"rawBlockCounter\",\n                 \"dspModeCascade\", \"dspModeDual\", \"dspModeRaw\", \"filt_detail\",\n                 \"MnmDist2.hpp\", \"MnmFm2.hpp\", \"MnmBbox2.hpp\",\n                 \"Dist2Core\", \"FMPlusBlock2\", \"Bbox2Core\",\n                 \"dspModeDist2\", \"dspModeFm2\", \"dspModeBbox2\",\n                 \"fm2Core\", \"bbox2Core\", \"dist2Core\",\n                 \"dspModeIsDistOnly\", \"dspModeIsSyntOnly\"):\n        hits = [p.name for p in live if word in without_comments(read(p))]\n        if hits:\n            check(False, f\"{word} остался в коде: \" + \", \".join(hits))\n        else:\n            check(True, f\"{word} не встречается в коде DSP-части\")\n\n    # 4. Реестр режимов\n    text = read(modes)\n    present(text, '\"mnm|old\"', \"DspModes.hpp: список режимов = mnm|old (всего два)\")\n    check(re.search(r\"dspModeMnm\\s*=\\s*0\\b\", text) is not None, \"dspModeMnm = 0 (основной)\")\n    check(re.search(r\"dspModeOld\\s*=\\s*1\\b\", text) is not None, \"dspModeOld = 1 (резерв)\")\n    check(re.search(r\"dspModeCount\\s*=\\s*2\\b\", text) is not None, \"dspModeCount = 2\")\n    present(text, \"dspSectionModeChoices\", \"DspModes.hpp: список режимов на каждый раздел\")\n    present(text, \"dspModeAllowedForSection\", \"DspModes.hpp: правило допустимости режима\")\n    present(text, \"dspModeLegacyIndexToCurrent\", \"DspModes.hpp: перевод старой нумерации режимов\")\n    present(text, \"kDspModeSchemaVersion = 10\", \"DspModes.hpp: версия схемы состояния = 10\")\n    present(text, \"kShowBackupModes\", \"DspModes.hpp: резервные режимы скрываются флагом\")\n    table = re.search(r\"dspSectionModeChoices\\(int section\\)\\s*\\{(.*?)\\n\\}\", text, re.S)\n    check(table is not None, \"DspModes.hpp: найдена таблица режимов по разделам\")\n    body = table.group(1) if table else \"\"\n    for section, expect in ((\"DspSynt\", \"mnm|old\"), (\"DspAmp\", \"mnm|old\"), (\"DspFilter\", \"mnm|old\"),\n                            (\"DspDist\", \"mnm|old\"), (\"DspDelay\", \"mnm|old\"),\n                            (\"DspRouting\", \"mnm\"), (\"DspChorus\", \"mnm\")):\n        m = re.search(r\"case %s:\\s+return \\\"([^\\\"]+)\\\"\" % section, body)\n        check(bool(m) and m.group(1) == expect,\n              f\"DspModes.hpp: {section} -> {expect}\")\n    present(text, \"case DspFilter: return \\\"FILT: mnm=РЕАЛЬНЫЙ фильтр\",\n            \"DspModes.hpp: у FILT основной режим описан как реальный фильтр прошивки\")\n\n    # 5. Диспетчер\n    text = read(dsp)\n    present(text, \"MnmRealFilter.hpp\", \"NovaDSP.h: подключён основной фильтр\")\n    call = re.search(r\"mnmFilter\\.setParameters\\(([^;]*)\\)\", text)\n    check(call is not None, \"NovaDSP.h: вызов основного фильтра найден\")\n    if call:\n        args = re.sub(r\"\\s+\", \"\", call.group(1))\n        check(\"params[20]-64\" not in args and \"params[21]-64\" not in args,\n              \"NovaDSP.h: ATK/DEC не передаются как BOFS/WOFS (баг «по частотам» закрыт)\")\n        check(args == \"fltBase,fltWidth,fltHpq,fltLpq,fltAtk,fltDec,fltBofs,fltWofs\",\n              \"NovaDSP.h: порядок аргументов fltBase..fltWofs\")\n    present(text, \"filter.processStereo(l,r,l,r\", \"NovaDSP.h: резервный режим old (MonomachineFilter) на месте\")\n    present(text, \"bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false)\",\n            \"NovaDSP.h: BBOX получает ручки как в 1.6.0 (PTCH,ST/RT,START-позиция,SLOT,RND)\")\n    present(text, \"bbox.setChromatic(b(6)>0)\", \"NovaDSP.h: хроматический режим BBOX на месте\")\n    present(text, \"if(id!=15){const float g=p[7]/64.0f;\",\n            \"NovaDSP.h: INP для FX -- линейный, как в 1.6.0 (без квадратичного «FIXED»)\")\n    present(text, \"const float wet=norm(p[3]);l[i]=dl*(1-wet)+wl*wet;\",\n            \"NovaDSP.h: хорус микшируется dry->wet по MIX (как в 1.6.0, без домешивания x1.5)\")\n    for word in (\"inpGainQuad\", \"chorusMakeup\", \"reverbMakeup\", \"FIXED\"):\n        absent(without_comments(text), word, f\"NovaDSP.h: нет остатков приёма «{word}»\")\n\n    # 6. BBOX\n    text = read(bbox)\n    check(balanced(text), \"monomachine_bbox.hpp: скобки сбалансированы\")\n    present(text, \"float retrigMs = 5.0f + ((127.0f - static_cast<float>(rtim)) / 127.0f) * 250.0f;\",\n            \"BBOX: старый закон RTIM (5...255 мс) с инверсией\")\n    absent(text, \"255.0f * std::pow((127.0f-static_cast<float>(rtim))/126.0f,2.0f)\",\n           \"BBOX: прежняя формула с интервалом 0 мс удалена\")\n    present(text, \"static constexpr size_t kDeclickSamples = 16;\",\n            \"BBOX: сглаживание задано в сэмплах (kDeclickSamples)\")\n    present(text, \"samplesToRestart\", \"BBOX: спад доходит до нуля к перезапуску (деклик)\")\n    present(text, \"static const bool modernVoice[24]=\", \"BBOX: таблица голосов нового кита\")\n    present(text, \"*0.125\", \"BBOX: STRT ограничен 1/8 активной длины (короткие удары целее)\")\n    present(text, \"void generateLegacyKit()\", \"BBOX: полностью старый кит оставлен рядом\")\n\n    # 7. Аудио-поток\n    text = read(processor)\n    present(text, \"dspModeAllowedForSection(i,idx)\",\n            \"PluginProcessor.cpp: режим проверяется списком своего раздела\")\n    absent(text, \"dspModeIsFilterOnly(idx)\", \"PluginProcessor.cpp: ad-hoc правил больше нет\")\n    present(text, \"dspModeLegacyIndexToCurrent\", \"PluginProcessor.cpp: старая нумерация переводится\")\n    present(text, \"state.setProperty(\\\"schema\\\",monomachine::kDspModeSchemaVersion,nullptr)\",\n            \"PluginProcessor.cpp: схема состояния обновлена\")\n    present(text, \"if(schema<10)\", \"PluginProcessor.cpp: миграция состояний до схемы 10\")\n    present(text, \"static_cast<float>(monomachine::dspModeOld),nullptr\",\n            \"PluginProcessor.cpp: старые состояния без режимов заполняются old (как в DeliveryTests)\")\n\n    # 8. Меню\n    text = read(editor)\n    bounds = re.search(r\"dspModeButton\\.setBounds\\(([^)]*)\\)\", text)\n    check(bounds is not None, \"PluginEditor.cpp: dspModeButton позиционируется\")\n    if bounds:\n        values = [int(v) for v in re.findall(r\"-?\\d+\", bounds.group(1))]\n        x, y = values[0], values[1]\n        check(y != 58 or x >= 1014, \"PluginEditor.cpp: dspModeButton не залезает на строку ARP\")\n    present(text, \"dspModePrimaryForSection\", \"PluginEditor.cpp: в меню основной режим идёт первым\")\n    present(text, \"dspModeAllowedForSection\", \"PluginEditor.cpp: меню предлагает только режимы раздела\")\n    present(text, \"РЕЗЕРВ\", \"PluginEditor.cpp: резервные режимы помечены отдельной группой\")\n\n    # 9. Тесты\n    if tests.is_file():\n        text = read(tests)\n        present(text, \"RTIM 127 must be about 5 ms, not zero\",\n                \"DeliveryTests.cpp: ожидание нового закона RTIM\")\n        present(text, \"new snare/toms missing from the working kit\",\n                \"DeliveryTests.cpp: ожидание рабочего кита (старый + снейр/томы)\")\n    else:\n        print(\"..   tests/DeliveryTests.cpp не найден рядом с проектом -- проверки тестов пропущены\")\n\n    if failures:\n        print(f\"\\nFAIL: {len(failures)} check(s) failed\", file=sys.stderr)\n        return 1\n    print(\"\\nPASS static patch verification\")\n    return 0\n\n\nif __name__ == \"__main__\":\n    raise SystemExit(main())\n"

import io
import os
import re
import shutil


def _status(new_text, old_text):
    return "applied" if new_text != old_text else "already"


def _replace_once(text, old, new, what):
    """Точечная замена. Если старого текста нет, а новый уже стоит -- 'уже было'."""
    if old in text:
        return text.replace(old, new, 1), True
    if new in text:
        return text, False
    raise RuntimeError("не найден фрагмент: " + what)


# ---------------------------------------------------------------------------
# 1. NovaDSP.h: убрать альтернативные движки dist2 / fm2 / bbox2
# ---------------------------------------------------------------------------
def edit_novadsp_remove_alt_dsp(text):
    orig = text
    # Повторный запуск: движков dist2/fm2/bbox2 в файле уже нет.
    if ("MnmDist2.hpp" not in text and "MnmFm2.hpp" not in text and "MnmBbox2.hpp" not in text
            and "fm2Core" not in text and "bbox2Core" not in text and "dist2Core" not in text):
        return text, "already"
    pairs = [
        # --- заголовки
        ('#include "dsp/mnm/MnmDist2.hpp"\n', ""),
        ('#include "dsp/mnm/MnmFm2.hpp"\n', ""),
        ('#include "dsp/mnm/MnmBbox2.hpp"\n', ""),
        # --- prepare / clear
        ("        fm2Core.reset(sr); fm2Core.setKind(monomachine::mnm::FMPlusBlock2::Kind::Dyn);\n"
         "        bbox2Core.reset(sr);\n", ""),
        ("        fm2Core.reset(sr);\n        bbox2Core.reset(sr);\n", ""),
        # --- setModes
        ("    void setModes(int syntModeIn,int distModeIn){\n"
         "        if(syntMode!=syntModeIn){syntMode=syntModeIn;mnmFm.reset(sr);fm2Core.reset(sr);}\n"
         "        distMode=distModeIn;\n    }",
         "    void setModes(int syntModeIn,int distModeIn){if(syntMode!=syntModeIn){syntMode=syntModeIn;mnmFm.reset(sr);}distMode=distModeIn;}"),
        # --- set(): bbox2 и fm2
        ("""        if(id==7) {
            bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false);
            bbox.setChromatic(b(6)>0);
            // bbox2 alternative repeat-sample knob mode
            typename monomachine::mnm::Bbox2Core::Params bp{};
            bp.base=b(0); bp.width=b(1); bp.q1=b(4); bp.q2=b(5); bp.envAtk=b(2); bp.envDec=b(3); bp.sampleSel=b(2); bp.repeat=b(7);
            bbox2Core.setParameters(bp);
        }""",
         "        if(id==7){bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false);bbox.setChromatic(b(6)>0);}"),
        ("""        // fm2 setup
        if((id==8||id==9||id==10) && syntMode==monomachine::dspModeFm2){
            fm2Core.setKind(id==8?monomachine::mnm::FMPlusBlock2::Kind::Stat:(id==9?monomachine::mnm::FMPlusBlock2::Kind::Par:monomachine::mnm::FMPlusBlock2::Kind::Dyn));
        }
""", ""),
        # --- on()
        ("        bbox2Core.noteOn(static_cast<uint8_t>(midi));\n", ""),
        ("        fm2Core.noteOn(static_cast<float>(midi));\n", ""),
        # --- члены MachineEngine
        ("    monomachine::mnm::FMPlusBlock2 fm2Core;\n    monomachine::mnm::Bbox2Core bbox2Core;\n", ""),
        # --- render(): SYNT (полностью как в 1.6.0, без веток fm2/bbox2)
        ("""        // SYNT modes: mnm (0) = recovered firmware FM+, old (1) = previous Nova FM, fm2 (6) = alternative FMPlusBlock2, bbox2 (7) = repeat-sample BBOX
        if((id==8||id==9||id==10)){
            if(syntMode==monomachine::dspModeMnm){
                const auto kind=id==8?monomachine::mnm::FmKind::Stat:(id==9?monomachine::mnm::FmKind::Par:monomachine::mnm::FmKind::Dyn);
                std::array<float,8> values{};for(int i=0;i<8;++i)values[static_cast<size_t>(i)]=p[static_cast<size_t>(i)];
                mnmFm.setParameters(kind,values);
                int done=0;while(done<n){const int block=std::min(monomachine::mnm::kBlock,n-done);float blockBuf[monomachine::mnm::kBlock]{};
                    mnmFm.processBlock(blockBuf,block);
                    for(int i=0;i<block;++i){l[static_cast<size_t>(done+i)]+=blockBuf[i];r[static_cast<size_t>(done+i)]+=blockBuf[i];}
                    done+=block;}
                return;
            } else if(syntMode==monomachine::dspModeFm2){
                typename monomachine::mnm::FMPlusBlock2::Params fp{};
                fp.pitch=static_cast<int>(p[0]); fp.mod=static_cast<int>(p[1]); fp.shape=static_cast<int>(p[2]); fp.decay=static_cast<int>(p[3]);
                fp.portamento=static_cast<int>(p[4]); fp.lfo=static_cast<int>(p[5]); fp.fmAmt=static_cast<int>(p[6]); fp.fmShape=static_cast<int>(p[7]);
                for(int i=0;i<n;++i){float s=fm2Core.tick(fp); l[i]+=s; r[i]+=s;}
                return;
            }
        }""",
         """        // SYNT: mnm (0) = recovered firmware FM+ (FM+STAT / FM+PAR / FM+DYN), old (1) = прежний Nova FM.
        if(syntMode==monomachine::dspModeMnm&&(id==8||id==9||id==10)){
            const auto kind=id==8?monomachine::mnm::FmKind::Stat:(id==9?monomachine::mnm::FmKind::Par:monomachine::mnm::FmKind::Dyn);
            std::array<float,8> values{};for(int i=0;i<8;++i)values[static_cast<size_t>(i)]=p[static_cast<size_t>(i)];
            mnmFm.setParameters(kind,values);
            int done=0;while(done<n){const int block=std::min(monomachine::mnm::kBlock,n-done);float blockBuf[monomachine::mnm::kBlock]{};
                mnmFm.processBlock(blockBuf,block);
                for(int i=0;i<block;++i){l[static_cast<size_t>(done+i)]+=blockBuf[i];r[static_cast<size_t>(done+i)]+=blockBuf[i];}
                done+=block;}
            return;
        }"""),
        # --- render(): BBOX
        ("""        if(id==7){
            if(syntMode==monomachine::dspModeBbox2){
                // bbox2 repeat-sample knob mode (alternative from DproBboxBlock Nova_2KnobFade)
                for(int i=0;i<n;++i){float s=bbox2Core.tick(l[i]); l[i]=r[i]=s*norm(p[7]);}
                return;
            } else {
                bbox.processStereo(l,r,static_cast<size_t>(n));for(int i=0;i<n;++i)l[i]=r[i]=l[i]*norm(p[7]);return;
            }
        }""",
         "        if(id==7){bbox.processStereo(l,r,static_cast<size_t>(n));for(int i=0;i<n;++i)l[i]=r[i]=l[i]*norm(p[7]);return;}"),
        # --- TrackChain: dist2
        ("""            } else if(distMode==monomachine::dspModeDist2){
                l[i]=dist2Core.process(l[i],params[12],0.0f);
                r[i]=dist2Core.process(r[i],params[12],0.0f);
            } else {""",
         """            } else {"""),
        ("        dist2Core.reset();\n        clear();\n", "        clear();\n"),
        ("        write=0;srrCounter=0;heldL=heldR=0;pingPong=false;time=1;\n        dist2Core.reset();\n",
         "        write=0;srrCounter=0;heldL=heldR=0;pingPong=false;time=1;\n"),
        ("    monomachine::mnm::Dist2Core dist2Core;\n", ""),
    ]
    changed = False
    for old, new in pairs:
        text, did = _replace_once(text, old, new, old.split("\n")[0][:70])
        changed = changed or did
    return text, ("applied" if changed else "already")


def edit_novadsp_fx_back_to_160(text):
    """Хорус и FX: полный возврат к 1.6.0 (линейный INP, без домешиваний)."""
    orig = text
    # Повторный запуск: правка уже на месте (квадратичного INP и домешиваний нет).
    if "inpGainQuad" not in text and "chorusMakeup" not in text and "reverbMakeup" not in text:
        return text, "already"
    pairs = [
        ("""        // FIXED: INP quadratic law P:$14768B  gain = INP^2 *4 in 56-bit, 32->-12dB 64->0dB 127->+12dB
        auto inpGainQuad = [](float inpParam)->float {
            float v = std::clamp(inpParam,0.0f,127.0f);
            float lin = v/64.0f;
            return lin*lin; // (INP/64)^2 => 0.25 at 32, 1 at 64, ~3.94 at 127
        };
        // For synth machines, g = p[7]/64 linear; for FX we use quadratic per firmware
        if(id!=15){
            float g = inpGainQuad(p[7]);
            for(int i=0;i<n;++i){l[i]*=g; r[i]*=g;}
        }
        if(id==15){
            for(int i=0;i<n;++i){
                float wl=0,wr=0,dl=0,dr=0;
                float g = inpGainQuad(p[7]);
                chorus->processFrame(l[i]*g,r[i]*g,wl,wr,false);
                chorus->delayedInput(dl,dr);
                const float wet=norm(p[3]);
                // FIXED: chorus makeup - previously 127 sounded quiet vs 64. Now wet at 127 has 0dB same as dry at 0.
                // Wet from native core is lower; apply makeup 1.5x to make 127 loudness = 64 previously, and dry 0 = 0dB difference
                const float chorusMakeup = 1.5f; // makes wet unity, 127 now sounds like 64 before
                wl*=chorusMakeup; wr*=chorusMakeup;
                // dl/dr already unity (delayed input)
                l[i]=dl*(1.0f-wet)+wl*wet;
                r[i]=dr*(1.0f-wet)+wr*wet;
            }
        }""",
         """        // INP is the track's FX input gain (64 = unity, 0 = silence, 127 ~= +6 dB). CHORUS feeds its own INP word to the native core.
        if(id!=15){const float g=p[7]/64.0f;for(int i=0;i<n;++i){l[i]*=g;r[i]*=g;}}
        if(id==15){for(int i=0;i<n;++i){float wl=0,wr=0,dl=0,dr=0;
            chorus->processFrame(l[i]*p[7]/64.0f,r[i]*p[7]/64.0f,wl,wr,false);chorus->delayedInput(dl,dr);
            const float wet=norm(p[3]);l[i]=dl*(1-wet)+wl*wet;r[i]=dr*(1-wet)+wr*wet;}}"""),
        ("""            const float wet=norm(p[3]);
            const float reverbMakeup = 3.0f; // wetLevel 1/3 -> need 3x for 0dB wet
            for(int i=0;i<n;++i){const auto k=static_cast<size_t>(i);
                rvLpL+=lpAmount*(l[i]-rvLpL);rvLpR+=lpAmount*(r[i]-rvLpR);
                float wl=p[5]>=127?l[i]:rvLpL,wr=p[5]>=127?r[i]:rvLpR;
                wl*=reverbMakeup; wr*=reverbMakeup;""",
         """            const float wet=norm(p[3]);
            for(int i=0;i<n;++i){const auto k=static_cast<size_t>(i);
                rvLpL+=lpAmount*(l[i]-rvLpL);rvLpR+=lpAmount*(r[i]-rvLpR);
                float wl=p[5]>=127?l[i]:rvLpL,wr=p[5]>=127?r[i]:rvLpR;"""),
        ("            // FIXED: ensure dry unity when mix=0, wet makeup for 0dB at mix=127\n", ""),
        ("""        }
        // For other FX (phaser/flanger/ring), ensure dry path unity when bypassed: they already process wet only,
        // but outer mix in PluginProcessor will blend dryDelay. Here we keep wet unity.
    }
    int id=4,baseNote=60;""",
         """        }
    }
    int id=4,baseNote=60;"""),
    ]
    changed = False
    for old, new in pairs:
        text, did = _replace_once(text, old, new, old.split("\n")[0][:70])
        changed = changed or did
    return text, ("applied" if changed else "already")


def edit_novadsp_reverb_block_fix(text):
    """Реверб (FX-машина 13): буферы сухого сигнала под размер блока хоста.

    В 1.6.0/1.6.3 там стоял std::array<float,32>, а render() вызывается с полным
    блоком хоста (обычно 512) -- стек переполнялся и плагин падал на ревере.
    """
    orig = text
    # Повторный запуск: буферы уже под размер блока.
    if "rvDryL" in text and "std::array<float,32> dryL" not in text:
        return text, "already"
    pairs = [
        ("            std::array<float,32> dryL{},dryR{};std::copy_n(l,n,dryL.data());std::copy_n(r,n,dryR.data());",
         "            // v6: буферы сухого сигнала под РАЗМЕР БЛОКА хоста (было std::array<float,32>;\n"
         "            // при блоке больше 32 сэмплов это переполняло стек и роняло плагин на ревере).\n"
         "            if(rvDryL.size()<static_cast<size_t>(n)){rvDryL.resize(static_cast<size_t>(n));rvDryR.resize(static_cast<size_t>(n));}\n"
         "            std::copy_n(l,n,rvDryL.data());std::copy_n(r,n,rvDryR.data());"),
        ("                if(std::max(std::abs(dryL[k]),std::abs(dryR[k]))>0.002f)",
         "                if(std::max(std::abs(rvDryL[k]),std::abs(rvDryR[k]))>0.002f)"),
        ("                l[i]=dryL[k]*(1-wet)+wl*wet;r[i]=dryR[k]*(1-wet)+wr*wet;",
         "                l[i]=rvDryL[k]*(1-wet)+wl*wet;r[i]=rvDryR[k]*(1-wet)+wr*wet;"),
        ("    float rvHpInL=0,rvHpOutL=0,rvHpInR=0,rvHpOutR=0,rvLpL=0,rvLpR=0;\n",
         "    float rvHpInL=0,rvHpOutL=0,rvHpInR=0,rvHpOutR=0,rvLpL=0,rvLpR=0;\n"
         "    std::vector<float> rvDryL,rvDryR;\n"),
    ]
    changed = False
    for old, new in pairs:
        text, did = _replace_once(text, old, new, old.strip()[:60])
        changed = changed or did
    return text, ("applied" if changed else "already")


def edit_novadsp_indentation(text):
    """Мелкая гигиена: строка, из-за которой GCC ругался -Wmisleading-indentation."""
    orig = text
    old = "                if(p[4]>=85)a*=wave(static_cast<int>(p[5])/32,phases[1],dt*ratio);b=a;"
    new = ("                if(p[4]>=85)a*=wave(static_cast<int>(p[5])/32,phases[1],dt*ratio);\n"
           "                b=a;")
    if old not in text and new in text:
        return text, "already"
    text, _ = _replace_once(text, old, new, "indentation (b=a)")
    return text, _status(text, orig)


# ---------------------------------------------------------------------------
# 2. PluginProcessor.cpp: схема состояния 10
# ---------------------------------------------------------------------------
PROCESSOR_COMMENT_OLD = """        // Единственное правило: в разделе допустимы только его собственные режимы
        // (списки -- в models/DspModes.hpp, dspSectionModeChoices). Всё остальное
        // сводится к основному режиму mnm.  Раньше здесь были ad-hoc правила,
        // которые разрешали dist2/fm2/bbox2 внутри FILT -- из-за них блоки
        // "расползались" по чужим разделам и меню предлагало выбирать их везде."""

PROCESSOR_COMMENT_NEW = """        // Единственное правило: в разделе допустимы только его собственные режимы
        // (списки -- в models/DspModes.hpp, dspSectionModeChoices). Всё остальное
        // сводится к основному режиму mnm.  В v6 допустимы только mnm (основной)
        // и old (резерв): альтернативные движки dist2/fm2/bbox2 удалены."""


def edit_processor_v6(text):
    orig = text
    # Повторный запуск: схема состояния уже 10.
    if "if(schema<10)" in text:
        return text, "already"
    t, _ = _replace_once(text, PROCESSOR_COMMENT_OLD, PROCESSOR_COMMENT_NEW, "комментарий о правилах режимов")
    old_block = """        if(schema<9){ // v5: режимы cascade/dual/raw удалены, нумерация режимов новая (mnm=0 old=1 dist2=2 fm2=3 bbox2=4)"""
    new_block = """        if(schema<10){ // v6: альтернативные режимы dist2/fm2/bbox2 удалены, список -- mnm|old"""
    t, _ = _replace_once(t, old_block, new_block, "миграция схемы режимов")
    return t, _status(t, orig)


# ---------------------------------------------------------------------------
# 3. tests/DeliveryTests.cpp: ожидания под v6
# ---------------------------------------------------------------------------
def edit_delivery_tests(text):
    orig = text
    # Повторный запуск: ожидания уже новые.
    if "RTIM 127 must be about 5 ms, not zero" in text:
        return text, "already"
    t, d1 = _replace_once(
        text,
        'b.setParameters(64,0,1,127);check(b.retriggerInterval()==1,"RTIM127 not minimum");',
        'b.setParameters(64,0,1,127);check(b.retriggerInterval()>=200&&b.retriggerInterval()<=441,"RTIM 127 must be about 5 ms, not zero");',
        "RTIM127")
    t, d2 = _replace_once(
        t,
        'for(size_t i:{size_t(0),size_t(1),size_t(2),size_t(3),size_t(5)})check(old.getSlots()[i].data!=modern.getSlots()[i].data,"kit family not updated");',
        'for(size_t i:{size_t(1),size_t(2),size_t(3)})check(old.getSlots()[i].data!=modern.getSlots()[i].data,"new snare/toms missing from the working kit");'
        'for(size_t i:{size_t(0),size_t(5),size_t(12),size_t(14),size_t(16)})check(old.getSlots()[i].data==modern.getSlots()[i].data,"working kit must keep the old voice");',
        "проверка кита")
    return t, ("applied" if (d1 or d2) else "already")


def main(argv=None):
    import sys

    args = list(sys.argv[1:] if argv is None else argv)
    check_only = "--check" in args
    args = [a for a in args if not a.startswith("--")]
    root = os.path.abspath(args[0] if args else ".")
    if not os.path.isdir(root):
        print("НЕТ папки: " + root)
        return 2

    projects = [p for p in ("Monomachine_Nova_Synth", "Monomachine_Nova_FX")
                if os.path.isdir(os.path.join(root, p))]
    if not projects:
        print("В папке нет проектов Monomachine_Nova_Synth / Monomachine_Nova_FX:")
        print("  " + root)
        return 2

    # --- проверка, что правка v5 (реальный фильтр) уже на месте
    missing_v5 = []
    for proj in projects:
        src = os.path.join(root, proj, "Source")
        real = os.path.join(src, "dsp", "mnm", "MnmRealFilter.hpp")
        reg = os.path.join(src, "models", "DspModes.hpp")
        if not os.path.isfile(real) or not os.path.isfile(reg):
            missing_v5.append(proj + ": нет dsp/mnm/MnmRealFilter.hpp или models/DspModes.hpp")
            continue
        txt = io.open(real, encoding="utf-8", errors="replace").read()
        reg_txt = io.open(reg, encoding="utf-8", errors="replace").read()
        # Признак правки v5: реестр режимов со списками по разделам (в 1.6.0-1.6.2 такого нет).
        if "dspSectionModeChoices" not in reg_txt:
            missing_v5.append(proj + ": models/DspModes.hpp без списков режимов по разделам (нет правки v5)")
        elif "class RealFilterCore" not in txt or "using FilterCore = RealFilterCore" not in txt:
            missing_v5.append(proj + ": MnmRealFilter.hpp не от правки v5")
    if missing_v5:
        print("=== ЭТО ДЕРЕВО ЕЩЁ НЕ ПРОШЛО ПРАВКУ v5 ===")
        for m in missing_v5:
            print("  - " + m)
        print("\nСначала примените fix_filter_v5.py (реальный фильтр основным режимом),")
        print("потом запускайте fix_filter_v6.py. Если дерево -- чистая 1.6.3 из репозитория,")
        print("то v5 в нём уже есть, и этого сообщения быть не должно.")
        return 2

    print("=== Правка v6: чистка режимов, старый BBOX (кит/RTIM/деклик), хорус и FX как в 1.6.0 ===")
    print("папка: " + root)
    print("проекты: " + ", ".join(projects))
    if check_only:
        print("режим --check: файлы НЕ меняются, только отчёт\n")

    applied = 0
    already = 0
    problems = []

    def backup(path):
        # оригинал сохраняем только если файл существует и копии ещё нет
        if not check_only and os.path.isfile(path):
            bak = path + ".bak-v6"
            if not os.path.exists(bak):
                shutil.copy2(path, bak)

    def write_file(path, content, label):
        nonlocal applied, already
        old = None
        if os.path.isfile(path):
            old = io.open(path, encoding="utf-8", errors="replace").read()
        if old == content:
            print("   [=] " + label + " -- уже так")
            already += 1
            return
        if not check_only:
            backup(path)
            os.makedirs(os.path.dirname(path), exist_ok=True)
            with io.open(path, "w", encoding="utf-8", newline="\n") as fh:
                fh.write(content)
        print("   [+] " + label)
        applied += 1

    def edit_file(path, func, label):
        nonlocal applied, already
        if not os.path.isfile(path):
            problems.append("нет файла: " + path)
            print("   [!] " + label + " -- файла нет")
            return
        text = io.open(path, encoding="utf-8", errors="replace").read()
        try:
            new_text, status = func(text)
        except Exception as exc:                     # noqa: BLE001
            problems.append(label + ": " + str(exc))
            print("   [!] " + label + " -- " + str(exc))
            return
        if status == "already":
            print("   [=] " + label + " -- уже так")
            already += 1
            return
        if not check_only:
            backup(path)
            with io.open(path, "w", encoding="utf-8", newline="\n") as fh:
                fh.write(new_text)
        print("   [+] " + label)
        applied += 1

    def drop_file(path, label):
        nonlocal applied, already
        if not os.path.isfile(path):
            print("   [=] " + label + " -- файла уже нет")
            already += 1
            return
        if check_only:
            print("   [+] " + label + " (будет выведен из сборки)")
            applied += 1
            return
        target = path + ".bak-v6"
        if os.path.exists(target):
            os.remove(path)
        else:
            os.rename(path, target)
        print("   [+] " + label)
        applied += 1

    for proj in projects:
        base = os.path.join(root, proj)
        src = os.path.join(base, "Source")
        print("\n--- " + proj + " ---")
        write_file(os.path.join(src, "dsp", "monomachine_bbox.hpp"), BBOX_V6,
                   "dsp/monomachine_bbox.hpp (старый кит + новые снейр/томы, старый закон RTIM с инверсией, деклик по сэмплам)")
        write_file(os.path.join(src, "models", "DspModes.hpp"), DSP_MODES_V6,
                   "models/DspModes.hpp (2 режима: mnm основной, old резерв; новые режимы -- только у фильтра)")
        edit_file(os.path.join(src, "NovaDSP.h"), edit_novadsp_remove_alt_dsp,
                  "NovaDSP.h (удалены движки dist2/fm2/bbox2 и все их ветки)")
        edit_file(os.path.join(src, "NovaDSP.h"), edit_novadsp_fx_back_to_160,
                  "NovaDSP.h (хорус и FX -- как в 1.6.0: линейный INP, без домешиваний)")
        edit_file(os.path.join(src, "NovaDSP.h"), edit_novadsp_reverb_block_fix,
                  "NovaDSP.h (ревер: буфер сухого сигнала под размер блока -- падение на ревере закрыто)")
        edit_file(os.path.join(src, "NovaDSP.h"), edit_novadsp_indentation,
                  "NovaDSP.h (мелкая гигиена отступа: предупреждение компилятора снято)")
        edit_file(os.path.join(src, "PluginProcessor.cpp"), edit_processor_v6,
                  "PluginProcessor.cpp (схема состояния 10, перевод старых режимов в mnm)")
        edit_file(os.path.join(base, "tests", "DeliveryTests.cpp"), edit_delivery_tests,
                  "tests/DeliveryTests.cpp (ожидания: RTIM 127 ~5 мс, рабочий кит = старый + снейр/томы)")
        drop_file(os.path.join(src, "dsp", "mnm", "MnmDist2.hpp"),
                  "Source/dsp/mnm/MnmDist2.hpp (режим dist2 -- вне сборки)")
        drop_file(os.path.join(src, "dsp", "mnm", "MnmFm2.hpp"),
                  "Source/dsp/mnm/MnmFm2.hpp (режим fm2 -- вне сборки)")
        drop_file(os.path.join(src, "dsp", "mnm", "MnmBbox2.hpp"),
                  "Source/dsp/mnm/MnmBbox2.hpp (режим bbox2 -- вне сборки)")

    write_file(os.path.join(root, "verify_dsp_mode_patch.py"), VERIFY_PY_V6,
               "verify_dsp_mode_patch.py (ожидания под v6)")
    for proj in projects:
        copy_path = os.path.join(root, proj, "verify_dsp_mode_patch.py")
        if os.path.isfile(copy_path):
            write_file(copy_path, VERIFY_PY_V6, proj + "/verify_dsp_mode_patch.py (ожидания под v6)")

    print("\n=== итог ===")
    print(("  требуют правки: " if check_only else "  внесено правок: ") + str(applied)
          + ", уже было: " + str(already))
    if problems:
        print("  ПРОБЛЕМЫ:")
        for p in problems:
            print("    - " + p)
        print("  Пришли этот вывод целиком -- разберём.")
        return 1
    if check_only:
        if applied == 0:
            print("  Состояние: всё уже на месте (правки v6 применены).")
        else:
            print("  Состояние: правки v6 ЕЩЁ НЕ применены (запустите без --check).")
    else:
        print("  Состояние: дерево приведено к v6.")
        print("  Дальше: Visual Studio -> Build -> Rebuild Solution (Release x64).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
