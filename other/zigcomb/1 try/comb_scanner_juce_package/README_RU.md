# Comb Scanner JUCE

Перенос схемы `comb_scanner` по приложенным скриншотам Max/RNBO в отдельный DSP-блок и JUCE-плагин.

## Что внутри

- `include/combscanner/CombScannerDSP.h` и `src/CombScannerDSP.cpp` — JUCE-независимый блок для подключения к другим проектам.
- `plugin/` — VST3/Standalone-плагин на JUCE.
- `tools/render_demo.cpp` и `tools/render_demo.js` — офлайн-рендер тестового сигнала.
- `comb_scanner_demo.wav` — 12-секундное стерео-превью с автоматическим проходом Scan.
- `reference/` — три исходных изображения.

## Управление

| Параметр | Диапазон | Значение на скриншоте | Назначение |
| --- | ---: | ---: | --- |
| Gain | 0..0.999 | 0.99 | Обратная связь comb-линий |
| Damp | 0..0.999 | 0.90 | Сглаживание/затухание в петле |
| Phase | 0..1 | 0.75 | All-pass коэффициент и лёгкая модуляция |
| Delay 1 | 5..1000 ms | 115 ms | Первая задержка каждой линии |
| Delay 2 | 5..1500 ms | 500 ms | Вторая задержка каждой линии |
| Scan | 0..1 | 0.00 | Кроссфейд между восемью comb-голосами |

Архитектура перенесена по видимым связям: восемь `GEN.NESTEDCOMB`, две задержки на голос, разные коэффициенты отношений задержек, DC-block, all-pass этап и scanning multiplexer. Это стабильная C++-реализация по схеме, а не буквальный экспорт Max/RNBO runtime, поэтому внутренние численные детали могут немного отличаться.

## Прослушивание

Откройте `comb_scanner_demo.wav` обычным аудиоплеером. В нём Scan плавно проходит от первого к последнему голосу, чтобы были слышны разные задержки и стереопозиции.

## Visual Studio 2026

Нужны Visual Studio 2026 с workload `Desktop development with C++`, CMake и интернет для загрузки JUCE:

```powershell
cmake --preset windows-vs2026
cmake --build --preset windows-vs2026-release
```

Результаты:

- `build/vs2026/CombScanner_artefacts/Release/VST3/Comb Scanner.vst3`
- `build/vs2026/CombScanner_artefacts/Release/Standalone/Comb Scanner.exe`

Если JUCE уже установлен:

```powershell
cmake --preset windows-vs2026 -DJUCE_DIR=C:/SDK/JUCE
```

## Подключение DSP

Добавьте два файла из `include/combscanner` и `src` в свой проект:

```cpp
#include "combscanner/CombScannerDSP.h"

combscanner::CombScannerDSP effect;
effect.prepare(sampleRate, maximumBlockSize);
effect.setParameters({ 0.99f, 0.90f, 0.75f, 115.0f, 500.0f, 0.0f });
effect.processBlock(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
```

`processBlock()` не выделяет память; память резервируется только в `prepare()`.
