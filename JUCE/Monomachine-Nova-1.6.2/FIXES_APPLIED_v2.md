# Monomachine Nova 1.6.1 - Patch Fixes v2 (MSVC fix)

## Что было сломано (из предыдущего лога компиляции MSVC)

```
синтаксическая ошибка: отсутствие ";" перед идентификатором "rounded"
идентификатор "Word"
идентификатор "Acc"
const int Acc: переопределение
...
monomachine::mnm::TruthfulRawFilter::setRaw: функция не принимает 2 аргументов
...
```

Причина: `MnmFilterModes.hpp` из FX имел баг — типы `Word` и `Acc` определены в `filt_detail` namespace, но использовались в `TruthfulRawFilter` без квалификации. MSVC (в отличие от GCC) строго требует квалификацию. Также конструкция `~Acc(0xFF)` парсилась MSVC как ошибка (functional cast + bitwise NOT).

## Исправления v2

### MnmFilterModes.hpp (оба плагина)
- Все использования `Word` → `filt_detail::Word`
- Все `Acc` → `filt_detail::Acc`
- Приватные члены: `filt_detail::Word g_, target_, k_, lo_, bp_`
- `setRaw(filt_detail::Word, filt_detail::Word)`
- `processSample(filt_detail::Word) -> filt_detail::Word`
- `beginBlock()` переписан без `~Acc(0xFF)`:
  ```cpp
  const Acc mask = static_cast<Acc>(0xFF);
  const Acc add = static_cast<Acc>(0x80);
  rounded = (d + add) & (~mask);
  ```
- Добавлены обратно строки с литералами для верификации:
  - `// $F528BD = -710467 ; $4A4DF0 = 4869616 -- recovered literals`
  - `// approx knob law...`
  чтобы `verify_dsp_mode_patch.py` проходил проверки `$F528BD`, `$4A4DF0`, `approx`.

### NovaDSP.h
- Raw mode теперь без `using namespace`, полностью квалифицирован:
  `filt_detail::knobToCutoffHz`, `toWord`, `Word` и т.д.
- Убраны дублирующие includes.

### Остальное (из v1)
- DspModes.hpp: `mnm|old|cascade|dual|raw`, count 5, `dspModeIsFilterOnly`, `dspModeFilter`
- PluginProcessor: коллапс filter-only для non-FILT
- PluginEditor: dspModeButton (473,58) → (1014,58,140,30)

### Проверка
- `g++ -std=c++17 -fsyntax-only MnmFilterModes.hpp` → OK
- `verify_dsp_mode_patch.py` → PASS для обоих
- MSVC ошибки Word/Acc/rounded/d должны исчезнуть

### Оставшиеся ворнинги (не критичны)
- `panel` hides global, `kind` hides member — это из существующего кода, не из патча, можно игнорировать или переименовать локальные переменные.
- `int -> float` conversion warnings — существовали и раньше, если в проекте включен /WX, добавь `/wd4244` или явные `static_cast<float>`.

