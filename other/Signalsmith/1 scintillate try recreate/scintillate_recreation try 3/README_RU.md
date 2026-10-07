# Scintillate Recreation

Поведенческий прототип спектрального feedback-движка Scintillate на C++/JUCE и отдельное HTML-превью. Это не побитная копия закрытого DSP Signalsmith Compose: исходников Scintillate нет, а приложенный VST3 собран под Windows и не запускается в текущем Linux-окружении.

## Что подтверждено у оригинала

- Плагин: `Scintillate`, Sweet Audio / Signalsmith Audio, VST3/CLAP, версия вложения `1.0.0`.
- DSP by Signalsmith; UI и plugin framework by Sweet Audio.
- Встроенная метаинформация VST3 даёт параметрный порядок и ID: `Mix 0`, `Length 1`, `Tone 2`, `Rate 3`, `Decay 4`, `Shimmer 5`, `Density 6`, `Width 7`, `Low Cut 8`, `High Cut 9`.
- `Density`: “Play only the loudest sparkles”. Это подтверждает режим спектрального gate/resynthesis, а не обычную громкостную плотность реверберации.
- `Rate`: частота появления новых sparkles; `Decay`: скорость затухания; `Shimmer`: количество pitch-up sparkles.
- По ручным тестам: `Length` идёт от 5 ms до 45 s, затем на максимуме переходит в `Infinity`; короткие значения дают быстрые, короткие detected sparkles.
- По слуху `Tone` действует слабо и заметнее на длинных хвостах; `Decay=-100%` даёт reverse/subtractive характер, а для плотного spectral trash полезен высокий `Rate`.
- Наиболее показательная комбинация для большого spectral feedback: низкий `Rate` и высокий `Density`, при `Mix=100%`.
- В публичном описании указано: обычный reverb tail анализируется в спектре, внутри него создаются sparkles.
- `signalsmith-fft` и открытые репозитории Signalsmith дают FFT/DSP/STFT-инструменты, но низколатентный `Signalsmith Compose` обозначен автором как proprietary.

## Запуск HTML-превью

Откройте `preview/scintillate_preview.html` обычным браузером. Нажмите `Start audio`; браузер разрешит звук только после жеста пользователя. Можно загрузить WAV/AIFF/MP3 через `Load audio`.

Рекомендуемый тест:

1. `Mix = 100%`, `Density = 0%`, `Shimmer = 0%`.
2. Постепенно увеличивать `Density`: от одного пика к более плотному набору.
3. Увеличивать `Shimmer`: выбранный индекс пика сдвигается к следующему, затем применяется pitch-up.
4. Крутить `Rate`: маска появления/исчезновения синусоид становится быстрее.
5. Проверять `Decay` на отрицательных значениях для reverse-like attack и на положительных для длинного хвоста.

## Сборка JUCE

Требуются CMake 3.24+, C++17 и интернет для первого скачивания JUCE и Signalsmith FFT:

```bash
cmake --preset default
cmake --build --preset default
```

Собираются VST3 и Standalone. DSP находится в `include/scintillate/ScintillateDSP.h` и `src/ScintillateDSP.cpp`; JUCE-обвязка находится в `plugin/`.

## Архитектура модели

- Четыре коротких feedback comb-линии здесь являются только прозрачным приближением спектрального feedback/scaffold, а не утверждением, что оригинал использует обычный room reverb.
- `signalsmith::fft::RealFFT<float>` анализирует 512-сэмпловый кадр с hop 128.
- Локальные спектральные максимумы сортируются по громкости; `Density=0` оставляет один, `Density=1` допускает до 24.
- `Shimmer` интерполирует индекс выбранного пика между соседними максимумами, поднимает частоту до одной октавы и добавляет лёгкий phase-twist как гипотезу для слышимого all-pass-like smear.
- Sparkles ресинтезируются отдельными синусоидами с `Rate`-spawn cadence, `Decay`-огибающей и stereo pan от `Width`; lifetime события связан с `Length`.
- Host latency объявлена нулевой: dry/reverb-путь идёт в текущем блоке, FFT используется как анализирующий side-chain. Это не означает, что спектральная реакция физически мгновенна.

## Ограничения

Для точного клона нужны рендеры оригинала на одинаковом входе, sample rate и всех состояниях ручек. В этой среде невозможно загрузить Windows VST3 в хост, поэтому здесь реализована проверяемая гипотеза, а не утверждение о внутреннем коде оригинала.

Измерения реальных Git LFS-рендеров и выводы для следующего DSP-приближения находятся в `AUDIO_ANALYSIS_RU.md`.
