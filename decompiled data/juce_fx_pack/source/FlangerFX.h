#pragma once
//==============================================================================
//  FlangerFX.h — ФЛЭНЖЕР.
//
//  ФАКТ ИССЛЕДОВАНИЯ: отдельного класса dsp::Flanger в JUCE НЕ существует
//  (grep по всему juce_dsp даёт 0 совпадений). Официальная документация
//  dsp::Chorus прямо говорит, как получить флэнжер [C.h:46-49]:
//
//      "This effect can also be used as a flanger with a lower centre delay
//       time and a lot of feedback, and as a vibrato effect if the mix value
//       is 1."                          — juce_Chorus.h, строки 46-49
//
//  Поэтому FlangerFX = ChorusFX + пресет «классический флэнжер»:
//      меньшая центральная задержка (3 мс против 7 мс у хоруса [C.h:167])
//      и большой feedback («a lot of feedback»).
//
//  Источник пресета: [C.h:46-49]; дефолт хоруса для сравнения [C.h:166-167].
//==============================================================================

#include "ChorusFX.h"

template <typename SampleType>
class FlangerFX : public ChorusFX<SampleType>
{
public:
    FlangerFX()
    {
        // Пресет «флэнжер» по официальному рецепту [C.h:46-49]:
        // «lower centre delay» + «a lot of feedback».
        this->setRate        ((SampleType) 0.25); // медленный LFO — типичный флэнжер
        this->setDepth       ((SampleType) 0.35);
        this->setCentreDelay ((SampleType) 3.0);  // «lower centre delay»: 3 мс (у хоруса 7 мс [C.h:167])
        this->setFeedback    ((SampleType) 0.7);  // «a lot of feedback»
        this->setMix         ((SampleType) 0.5);
    }

    // Быстрые пресеты поверх той же формулы [C.h:46-49]
    void presetJetFlanger()   // «самолётный» эффект
    {
        this->setRate ((SampleType) 0.15);
        this->setDepth ((SampleType) 0.6);
        this->setCentreDelay ((SampleType) 2.0);
        this->setFeedback ((SampleType) 0.85);
        this->setMix ((SampleType) 0.5);
    }

    void presetSubtleThickening() // лёгкое утолщение звука
    {
        this->setRate ((SampleType) 0.4);
        this->setDepth ((SampleType) 0.2);
        this->setCentreDelay ((SampleType) 5.0);
        this->setFeedback ((SampleType) 0.3);
        this->setMix ((SampleType) 0.35);
    }
};
