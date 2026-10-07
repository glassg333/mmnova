# Scintillate test source pack

Все файлы: stereo WAV, PCM 16-bit, 48 kHz.

## Файлы

- `scintillate_test_source_50s.wav` — основной сложный источник, пять характеров по 10 секунд.
- `scintillate_test_impulses_12s.wav` — повторяющиеся гармонические импульсы для измерения хвоста, Density, Decay и Rate.
- `tools/generate_source.py` — детерминированный генератор, если понадобится пересобрать исходники.

## Секции основного файла

| Время | Материал | Что лучше слушать |
| --- | --- | --- |
| 00:00–00:10 | Яркий гармонический drone, 14 движущихся частичных | auto-detect, Density, Tone |
| 00:10–00:20 | Inharmonic metallic cluster, FM и нецелые ratios | Shimmer, Width, High Cut |
| 00:20–00:30 | Vocal-like formant sweep с breath noise | Density, Low Cut, Decay |
| 00:30–00:40 | Kick, snare, hats и chirps | Rate, Decay, Length |
| 00:40–00:50 | Гибридный плотный climax | все ручки и автоматизация |

## Как записать тесты оригинального Scintillate

1. Создай проект с sample rate 48 kHz и отключи остальные плагины.
2. Поставь Scintillate на отдельный канал и сначала установи `Mix=100%`, `Width=100%`, `Low Cut=0`, `High Cut=100%`.
3. Для базового прогона используй дефолты бинарника: `Length=0.5`, `Tone=0.8`, `Rate=5 Hz`, `Decay=0`, `Density=100%`, `Shimmer=0%`.
4. Автоматизируй только одну ручку за раз. Сделай плавный sweep 0→100% за 20 секунд, затем 100→0% за следующие 20 секунд. Остальные ручки не меняй.
5. Для `Rate` используй диапазон UI от `0.1 Hz` до `100 Hz`. Для `Decay` пройди от `-100%` до `+100%` через ноль.
6. Отдельно прогони `scintillate_test_impulses_12s.wav` с `Density=0%`, `50%`, `100%` и с `Shimmer=0%`, `50%`, `100%`.
7. Экспортируй WAV без нормализации и лимитера. Скриншот automation lane должен показывать имя ручки и весь диапазон sweep.

## Что отправить обратно

Достаточно прислать архив с WAV-рендерами и один или несколько скриншотов. Имена лучше сделать такими:

```text
original_density.wav
original_shimmer.wav
original_rate.wav
original_decay.wav
original_length.wav
original_tone.wav
original_width.wav
original_lowcut.wav
original_highcut.wav
original_mix.wav
automation_overview.png
```

В первую очередь нужны `density`, `shimmer`, `rate`, `decay`, `length` и `tone`: по ним я скорректирую модель спектральных пиков, LFO и envelope. После этого сравним Width, Cut и Mix.
