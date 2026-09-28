# Патч экрана: цифры под ручками 1FRQ / 2FRQ / 3FRQ

## Зачем

После установки нового кода ЗВУК FM-машин верный, но ЭКРАН продолжает
показывать соотношения из третьей таблицы (`getFmListedRatio(raw/4)`,
32 ступени 1/64..12) — она не совпадает ни со звуком, ни с железом.
Из-за этого экран врёт: например на ручке 127 покажет не 8.00.

Чтобы экран показывал ровно то, что звучит и как в железе, замени
один if-блок в `PluginEditor.cpp`.

## Где

Файл: `<репо>/JUCE/Monomachine-Nova-1.9.7/Monomachine_Nova_Synth/Source/PluginEditor.cpp`
Строка ~915 (ищи по `getFmListedRatio`).

## Было

```cpp
if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))
    return juce::String(monomachine::getFmListedRatio(static_cast<uint8_t>(raw/4)),2);
```

## Стало

```cpp
if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))
{
    // Та же таблица и тот же закон, что в звуке и в железе:
    // n = floor(((K<<16)+$8000)*48/2^24), таблица Y:$141A80, 24 ступени.
    static constexpr float kR[24] = {0.03125f,0.0625f,0.125f,0.1875f,0.25f,0.3125f,
        0.375f,0.5f,0.625f,0.75f,0.875f,1.0f,1.25f,1.5f,1.75f,2.0f,2.5f,3.0f,
        3.5f,4.0f,5.0f,6.0f,7.0f,8.0f};
    const int K = juce::jlimit(0, (int)raw, 127);
    const int n = juce::jlimit(0, 23, (((K<<16)+0x8000)*48)>>24);
    return juce::String(kR[n], 2);
}
```

## Проверка после патча

- 1FRQ на 0    -> экран `0.03` (это 1/32);
- 1FRQ на 64   -> экран `1.25`;
- 1FRQ на 127  -> экран `8.00` (прежний порт показывал 1.25 — вот и весь
  ответ, почему «ручки не совпадают»).
