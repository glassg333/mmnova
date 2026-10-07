# Comb Scanner Pro — JUCE VST3

JUCE audio-plugin project with eight comb stages, scanner, insert effects, modulation matrix and 16-step sequencers. The GUI layout follows `Resources/combscanner_upgrade_reference.jpg`.

## Сборка в Visual Studio 2026 — без CMake и ручного Resave

Точка входа — `CombScannerPro.jucer`. Настройки сверены только по JUCE/Visual Studio `.jucer`-проектам Monomachine-Nova FX и Synth: C++17, VST3, Visual Studio 2022 exporter, x64 в Debug/Release, `/utf-8`, общие модули JUCE. В список включён `juce_audio_processors_headless`; его путь явно задан как `C:/JUCE/modules`.

Запусти из папки проекта:

```cmd
tools\BuildVST3.bat
```

При первом запуске (или после изменения `.jucer`) скрипт автоматически пересохраняет JUCE-проект с `--fix-missing-dependencies`, затем собирает `CombScannerPro - VST3` в `Release | x64`. При обычной пересборке только изменённого C++-кода Projucer повторно не запускается. Вручную нажимать Resave не нужно. Если менялась установка JUCE или нужно принудительно пересоздать solution, передай `-ForceResave`. Для Visual Studio/Projucer в других местах передай пути параметрами:

```cmd
tools\BuildVST3.bat -ProjucerPath "C:\JUCE\Projucer.exe" -MSBuildPath "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe"
```

Проект использует JUCE modules из `C:/JUCE/modules`; JUCE framework не дублируется в архиве. Собранный VST3 для теста копируй целиком как каталог `CombScannerPro.vst3`, не отдельный файл внутри него.

## Состав

- `Source/` — JUCE-процессор, APVTS-параметры, редактор и DSP-ядро.
- `ThirdParty/SignalsmithDSP/` — используемые FFT-заголовки и лицензия Signalsmith.
- `Resources/combscanner_upgrade_reference.jpg` — визуальный референс.
- `tests/dsp_smoke_test.cpp` — изолированный smoke test DSP-ядра.

## Проверки и ограничения

DSP smoke test выполнялся успешно. Полная Windows-сборка в Visual Studio 2026 и проверка редактора в Ableton/SnappySnap пока не подтверждены.

FFT Stretch — экспериментальный короткооконный spectral warp/freeze, не законченный формант-сохраняющий phase-vocoder. Это новая реализация по предоставленным ориентирам, а не побитовая реконструкция внутренней схемы Max/RNBO.
