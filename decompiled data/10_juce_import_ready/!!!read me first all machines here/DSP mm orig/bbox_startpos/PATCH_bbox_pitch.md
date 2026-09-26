# BBOX PITCH (m7) — прошивочные питч-правила, режим "ex"

## Диагноз (что в порту неверно)

В `Source/dsp/monomachine_bbox.hpp` (~строка 112 и ~161) питч собран из двух
законов:

```cpp
// строка ~112: КРИВАЯ PTCH
float semitones = (static_cast<float>(ptch) - 64.0f) / 64.0f * 24.0f;
m_pitchRatio = std::pow(2.0f, semitones / 12.0f);
// строка ~161: ВЫДУМАННЫЙ КЕЙТРЕК
float noteSemitones = m_chromatic ? 12.0f*chromaticOctave(midiNote)
                                  : static_cast<float>(midiNote)-60.0f;
m_keyTrackRatio = std::pow(2.0f, noteSemitones / 12.0f);
```

Сверка с прошивкой (листинг `07_DPRO-BBOX_full.txt`, proc P:$14752B-$147561,
эмулятор `scripts/exp_m7_bbox.py`, свипы note/PTCH/slot/mod):

1. **Кривая PTCH В ПОРТУ ВЕРНАЯ** — совпадает с прошивкой бит-в-бит по форме:
   ratio = 2^((k−64)/32), т.е. 0.375 полутона на шаг, ±2 октавы. Прошивка:
   терм = k×64 единиц (по 2048 на октаву, P:$147544-$147547: mac x1,x0 c
   x1=$2000) → подтверждено эмулятором: rate удваивается ровно за 32 шага.
2. **КЕЙТРЕКА В ПРОШИВКЕ НЕТ.** Внутри слота rate от ноты НЕ зависит вообще
   (эмулятор: note 0..127 при фиксированном слоте → rate байт-в-байт тот же).
   Нота входит только в **выбор сэмпла/слота** (config P:$1474ED: s = note;
   slot = floor(s/24) — 6 ROM-слотов по 24 хроматических ступени; старт/конец
   ROM-региона берутся из таблиц Y:$147632/$147633 по ноте-в-слоте).
   Порт же добавляет `2^((note−60)/12)` (и `2^(12×octave/12)` в хроматике) —
   это выдумка, её слышно как «неправильный» питч по клавишам.
3. **СЛОТ-ОФСЕТ**: прошивка добавляет slot×1024 единиц = **+0.5 октавы на
   слот** (P:$14753B-$14753E: mpyi #$200 c последующим переносом младшего
   слова в старшее; эмулятор: rate(slot→slot+1) = ×2^0.5 ровно). В порту
   этого закона нет.
4. **Питч-модуляция** (вход r6−$a, «добавка» ядра): вклад =
   floor(mod×11/2048) единиц аккамулятора (P:$14753F-$147540: maci #$B000).
5. Квантование: rate = таблица X:$140000 (пол-октавный ROI, 2048 ступеней,
   мантисса 13 бит после asr #$a) со знаковым динамическим сдвигом на октаву
   (P:$14754A-$147552) — отсюда «ступеньки» мантиссы ~0.4% — это подлинное
   поведение ROM, не баг.

## Прошивочная цепочка (для сверки)

```
acc = slot*1024 + floor(mod*11/2048) + PTCH_k*64 − 22528 + 3328   (единицы a1)
idx = acc & 2047;  oct = acc >> 11 (арифм., знак)
rate = X:$140000[idx] >> 10  ASL(знаковый oct)                    (мантисса 13 бит)
rate48 = rate × $1D22A (>>3) × $2F8E  (48-бит)                    (P:$147553-$147560)
```

## Патч (режим "ex", поведение mnm не трогается)

1. `Source/dsp/monomachine_bbox.hpp` — рядом с `setFirmwareStart` (добавлен
   поставкой bbox_startpos):

```cpp
    void setFirmwarePitch(bool on) noexcept { m_firmwarePitch = on; } // 1.7.11 ex: прошивочные питч-правила (P:$14753B/$14754F)
```

2. Туда же, в `private:` рядом с `m_firmwareStart`:

```cpp
    bool m_firmwarePitch = false; // 1.7.11 ex
```

3. Блок установки ратио в `setParameters` (строка ~112) — ЗАМЕНИТЬ целиком:

```cpp
        // Pitch multiplier: 0 -> -2 octaves (0.25x), 64 -> 1.0x (normal), 127 -> +2 octaves (4.0x)
        float semitones = (static_cast<float>(ptch) - 64.0f) / 64.0f * 24.0f;
        m_pitchRatio = std::pow(2.0f, semitones / 12.0f);
        // 1.7.11 ex: прошивка добавляет +0.5 октавы на слот кита (P:$14753B-14753E,
        // slot*1024 единиц по 2048/октаву); в режиме mnm слот-офсета нет.
        m_slotPitchRatio = m_firmwarePitch ? std::pow(2.0f, static_cast<float>(m_activeSlot) * 0.5f) : 1.0f;
```

4. Блок кейтрека в конце `setParameters` (строка ~161) — ЗАМЕНИТЬ целиком:

```cpp
    // Key tracking pitch offset (Middle C / 60 is neutral)
    if (m_firmwarePitch) {
        // ex: OS 1.32B — кейтрека НЕТ (эмулятор: rate внутри слота от ноты не
        // зависит; нота = выбор сэмпла, P:$1474ED). Нота смещает питч только
        // через выбор слота (см. m_slotPitchRatio).
        m_keyTrackRatio = 1.0f;
    } else {
        float noteSemitones = m_chromatic?12.0f*static_cast<float>(chromaticOctave(midiNote)):static_cast<float>(midiNote)-60.0f;
        m_keyTrackRatio = std::pow(2.0f, noteSemitones / 12.0f);
    }
```

5. В `processStereo`/место расчёта `rateFactor` (строка ~182) — добавить
   слот-множитель:

```cpp
    // БЫЛО:
    const double rateFactor = (slot.originalSampleRate / m_sampleRate) * m_pitchRatio * m_keyTrackRatio * m_bendRatio;
    // СТАЛО:
    const double rateFactor = (slot.originalSampleRate / m_sampleRate) * m_pitchRatio * m_keyTrackRatio * m_bendRatio * m_slotPitchRatio; // 1.7.11 ex
```

6. В `private:` рядом с `m_pitchRatio` (строка ~326):

```cpp
    float m_slotPitchRatio = 1.0f; // 1.7.11 ex
```

7. `Source/NovaDSP.h`, `set()`, строка ~189 (строка с id==7, уже правленая
   поставкой bbox_startpos) — СТАЛО:

```cpp
    if(id==7){bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false);bbox.setChromatic(b(6)>0);const bool ex=syntMode==monomachine::dspModeEx;bbox.setFirmwareStart(ex);bbox.setFirmwarePitch(ex);} // 1.7.11 ex
```

## Что НЕ трогается

- Намеренные bbox-моды (random-start/random-slot, импорт семплов, адаптивный
  деклик) — остаются.
- Хроматический режим — остаётся как режим `mnm` (в `ex` кейтрека гасится,
  нота работает только как выбор сэмпла/слота — как в прошивке).
- PTCH-формула не меняется (она и была верной).

## A/B

Кнопка DSP MODE → SYNT → режим `ex` на машине 7:
- mnm: кейтрек 2^((note−60)/12) (или октавы в хроматике), без слот-офсета;
- ex: rate от ноты не зависит (нота = выбор сэмпла), слот даёт +6 полутонов.
На миди-клавиатуре разница слышна сразу: в mnm питч ползёт с клавишами, в ex
каждая клавиша = свой кусок кита на одной высоте (поведение ROM-машины).
