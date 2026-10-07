# Исследование

## Приложенный VST3

Файл `scintillate.vst3` распознан как PE32+ x86-64 Windows DLL. В бинарнике присутствуют CLAP-метаданные и встроенный WebView UI. Найдены строки `Scintillate`, `Sweet Audio`, `Sparkling Reverb`, `net.sweet-audio.scintillate` и `clap-wrapper`.

Параметры из встроенного UI-кода:

| ID | Имя | Диапазон | Дефолт | Единица |
| --- | --- | --- | --- | --- |
| 0 | Mix | 0..1 | 0.5 | % |
| 1 | Length | 0..1 | 0.5 | специальная шкала |
| 2 | Tone | 0..1 | 0.8 | % |
| 3 | Rate | 0.1..100 | 5 | Hz |
| 4 | Decay | -1..1 | 0 | % |
| 5 | Shimmer | 0..1 | 0 | % |
| 6 | Density | 0..1 | 1 | % |
| 7 | Width | 0..1 | 1 | % |
| 8 | Low Cut | 0..1 | 0 | Hz |
| 9 | High Cut | 0..1 | 1 | Hz |

Встроенные подсказки UI:

- Length: время, после которого reverb tail достигает -60 dB.
- Tone: яркость reverb tail.
- Rate: как часто создаются новые sparkles.
- Decay: как быстро уменьшается громкость sparkle.
- Density: проигрывать только самые громкие sparkles.
- Shimmer: количество sparkles, поднятых по высоте.
- Low/High Cut: нижняя и верхняя граница sparkle-спектра.

По последнему ручному тесту пользователя `Length` отображается как 5 ms..45 s, после 45 s верхняя точка переходит в `Infinity`. В прототипе эта шкала теперь использует 45 s и бесконечный feedback на верхнем значении. Cut сохраняет отдельную частотную шкалу до 50 kHz. Значения `5 ms`, `0 Hz` и `0.10 Hz` на пользовательском скриншоте являются состоянием конкретного пресета/сессии, а не встроенными дефолтами UI.

## Signalsmith

- [`fft`](https://github.com/Signalsmith-Audio/fft): небольшая C++11 FFT-библиотека, поддерживает быстрые размеры и `RealFFT`. Это обычный FFT, не «фейковый FFT».
- [`dsp`](https://github.com/Signalsmith-Audio/dsp): header-only DSP с FFT, spectral/STFT, окнами, задержками и WOLA-инструментами.
- [`linear`](https://github.com/Signalsmith-Audio/linear): более внутренние FFT/STFT-обёртки, chunked computation и асимметричные окна для сниженной задержки.
- [`signalsmith-stretch`](https://github.com/Signalsmith-Audio/signalsmith-stretch): phase-vocoder pitch/time stretching, peak/band analysis, предсказание фаз и измеряемые input/output latency.
- [`basics`](https://github.com/Signalsmith-Audio/basics): открытые MIT-классы, среди них analyser и reverb.
- [`reverb-example-code`](https://github.com/Signalsmith-Audio/reverb-example-code): открытый учебный reverb из ADC 2021 с feedback/diffusion архитектурой.
- [`pitch-time-example-code`](https://github.com/Signalsmith-Audio/pitch-time-example-code): более старые четыре метода pitch-shifting, включая Modified Real FFT с half-bin offset.

На странице Signalsmith про лицензирование прямо сказано, что `Signalsmith Compose` является proprietary low-latency spectral processing framework. Поэтому точная реализация Scintillate, скорее всего, находится там или в лицензированном внутреннем коде, а не в открытом `fft`. По пользовательским рендерам это разумнее описывать как spectral feedback/resynthesis engine, а не как стандартный room reverb.

## Публичное описание Scintillate

KVR описывает продукт как “Spectral Reverb Sparkle Engine”: sparkles генерируются внутри спектра conventional reverb, а уменьшение Density пропускает только самые громкие sparkles и даёт spectral-gate эффект. Там же указано “DSP by Signalsmith | Everything else by Sweet Audio”.

Источники:

- https://www.kvraudio.com/product/scintillate-by-sweet-audio
- https://signalsmith-audio.co.uk/code/
- https://github.com/Signalsmith-Audio/fft
