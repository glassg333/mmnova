==========================================================================
 juce_fx_pack — ТВОИ рабочие FX-классы для JUCE DSP
 (JUCE 8.0.8, коммит d6181bde38d858c283c3b7bf699ce6340c050b5d)
==========================================================================

ВАЖНОЕ ПОЯСНЕНИЕ (про «файлы МОИ, а не из репозитория»)
--------------------------------------------------------------------------
Файлы из репозитория JUCE НИКОГДА не включались в этот пак и не отдавались.
Они использовались ТОЛЬКО как СПРАВОЧНИК: чтобы в комментариях кода указать
ТОЧНЫЙ номер строки, откуда взята каждая формула/константа/диапазон.
Весь код в source/ — написан с нуля для тебя: собственные кольцевые буферы,
интерполяция, сглаживание параметров, структура классов. Ни один файл JUCE
не скопирован.

СОСТАВ ПАКА
--------------------------------------------------------------------------
source/
  FXCommon.h    — общие помощники (линейный сглаживатель = аналог
                  SmoothedValue Linear; синус-LFO). Ссылки на строки JUCE,
                  откуда взяты константы сглаживания.
  ChorusFX.h    — ХОРУС. Алгоритм dsp::Chorus: формулы d_ms = max(1,
                  20*lfoGain + centre), вычитающая feedback, linear mix.
                  Каждая строка-источник помечена [C.h:NN]/[C.cpp:NN].
  FlangerFX.h   — ФЛЭНЖЕР. Факт: dsp::Flanger в JUCE НЕ существует.
                  Официальный рецепт [C.h:46-49]: меньший centreDelay +
                  большой feedback поверх Chorus. Пресет: 3 мс, fb 0.7.
  PhaserFX.h    — ФАЗЕР. 6 каскадов TPT all-pass [P.h:39-40], обновление
                  среза раз в 4 сэмпла [P.h:205], log-карта частоты
                  20..min(20000,0.49*sr) [P.h:140-141], ядро Zavalishin
                  v=G(x-s); y=v+s; s=y+v [FT.cpp:93-95], allpass=2y-x
                  [FT.cpp:101], G=tan(pi*fc/sr)/(1+tan) [FT.cpp:120-121].
  ReverbFX.h    — РЕВЕРБ (Freeverb). 8 comb параллельно + 4 all-pass
                  последовательно на канал [R.h:158-168]; тюнинги
                  [R.h:104-105]; уравнения [R.h:255-266]/[R.h:299-307];
                  гейны wet*3/dry*2 [R.h:83-94]; room->fb=0.28*room+0.7
                  [R.h:214-222]; freeze [R.h:69-70,210,218-219].
  OtherFX.h     — WaveShaperFX (механизм [WS.h:43-58]; tanh-формула НЕ
                  из JUCE — там функция задаётся пользователем) и
                  EchoFX (фидбек-эхо, стандартная топология).

docs/
  raw_data.md   — ВСЕ исходные данные: таблицы параметров (диапазоны,
                  дефолты), формулы, номера строк-источников для
                  chorus/flanger/phaser/reverb + карта остальных FX.
  fm_pending.md — fm+dyn / fm+stat / fm+par: статус ОТЛОЖЕНО. В JUCE таких
                  режимов нет, исходная ссылка мертва. Что нужно от тебя,
                  чтобы закончить — список в этом файле.

scripts/
  unpack_juce_fx_pack.py — распаковщик base64 (тот же принцип, что твой
                  monomachine-скрипт).

КАК ПОДКЛЮЧИТЬ К ПРОЕКТУ
--------------------------------------------------------------------------
1. Положи *.h из source/ в папку Source твоего JUCE-проекта.
2. В PluginProcessor.cpp:

     #include "ChorusFX.h"
     #include "PhaserFX.h"
     #include "ReverbFX.h"

     juce::dsp::ProcessorChain<ChorusFX<float>, PhaserFX<float>, ReverbFX<float>> fxChain;

   В prepareToPlay:
     fxChain.prepare ({ sampleRate, (juce::uint32) samplesPerBlock,
                        (juce::uint32) getTotalNumOutputChannels() });

   В processBlock:
     fxChain.process (juce::dsp::ProcessContextReplacing<float> (block));

3. Все классы шаблонные (float/double); ReverbFX — float (как в JUCE).

ИСТОЧНИКИ-СПРАВОЧНИКИ (только чтение, не копирование)
--------------------------------------------------------------------------
- modules/juce_dsp/widgets/juce_Chorus.h / .cpp
- modules/juce_dsp/widgets/juce_Phaser.h / .cpp
- modules/juce_dsp/processors/juce_FirstOrderTPTFilter.cpp
- modules/juce_audio_basics/utilities/juce_Reverb.h
- modules/juce_dsp/widgets/juce_WaveShaper.h
- modules/juce_core/maths/juce_MathsFunctions.h
Версия: JUCE 8.0.8, коммит d6181bde38d858c283c3b7bf699ce6340c050b5d

ЛИЦЕНЗИОННАЯ ЗАМЕТКА
--------------------------------------------------------------------------
Код написан с нуля, но алгоритмы повторяют документированное поведение
эффектов JUCE. Для коммерческого проекта проверь условия лицензии JUCE
(AGPLv3 / коммерческая): https://juce.com/legal/juce-8-licence/
