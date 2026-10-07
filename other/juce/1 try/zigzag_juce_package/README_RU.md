# ZigZag JUCE

Перенос аудио-алгоритма из `zigzag_1.maxpat` в переносимый DSP-блок и JUCE-плагин.

## Что внутри

- `include/zigzag/ZigZagDSP.h` и `src/ZigZagDSP.cpp` — JUCE-независимый DSP-блок для подключения к другим проектам.
- `plugin/` — VST3/Standalone-плагин на JUCE с четырьмя автоматизируемыми ручками.
- `tools/render_demo.cpp` — маленький офлайн-рендерер тестового сигнала.
- `zigzag_demo.wav` — готовое стерео-превью для прослушивания без DAW.
- `reference/` — исходный Max-патч и приложенные изображения для сверки.

## Ручки

| Ручка | Диапазон | Значение из патча | Назначение |
| --- | ---: | ---: | --- |
| Decay | 0..127 | 29.74 | Время затухания FDN |
| Damping | 0.1..0.9839 | 0.10 | Частотное затухание в линиях |
| Rotate | 0..1 | 0.25 | Скорость/угол вращения матрицы обратной связи |
| Fluctuate | 0..1 | 0.0 | Случайное плавное изменение длин задержек |

Внутри сохранена идея исходника: четыре линии задержки до 3 секунд, Hadamard-перемешивание, вращение пар, переменные веса, DC-safe damping и четыре all-pass диффузора.

Это стабильная ручная реализация по структуре RNBO-патча, а не буквальный экспорт закрытого Max/RNBO runtime. Поэтому отдельные численные детали интерполяции `delay 3000 2` и случайных коэффициентов могут отличаться, но четыре параметра и звуковая архитектура перенесены.

## Быстрое прослушивание

Откройте файл `zigzag_demo.wav` любым аудиоплеером. Это 12-секундный стерео-пример с короткими аккордами и импульсами, чтобы были слышны хвосты и модуляция.

## Сборка в Visual Studio 2026

Нужны Visual Studio 2026 с workload `Desktop development with C++`, CMake и интернет для загрузки JUCE.

```powershell
cmake --preset windows-vs2026
cmake --build --preset windows-vs2026-release
```

После сборки ищите:

- `build/vs2026/ZigZag_artefacts/Release/VST3/ZigZag.vst3`
- `build/vs2026/ZigZag_artefacts/Release/Standalone/ZigZag.exe`

Если JUCE уже скачан, можно указать его путь:

```powershell
cmake --preset windows-vs2026 -DJUCE_DIR=C:/SDK/JUCE
```

Если установленный CMake еще не знает генератор Visual Studio 2026, откройте Developer PowerShell и используйте установленный генератор явно, либо обновите CMake. Исходники не завязаны на конкретный minor-релиз Visual Studio.

## Подключение DSP к своему проекту

Добавьте `include/zigzag/ZigZagDSP.h` и `src/ZigZagDSP.cpp` в проект:

```cpp
#include "zigzag/ZigZagDSP.h"

zigzag::ZigZagDSP effect;
effect.prepare(sampleRate, maximumBlockSize);
effect.setParameters({ 29.74f, 0.10f, 0.25f, 0.0f });
effect.processBlock(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
```

`prepare()` и `reset()` вызываются из жизненного цикла аудиопотока, а `processBlock()` не выделяет память.

## Рендер WAV заново

После конфигурации проекта:

```powershell
cmake --build --preset windows-vs2026-release --target zigzag_demo
build/vs2026/Release/zigzag_demo.exe zigzag_demo.wav
```
