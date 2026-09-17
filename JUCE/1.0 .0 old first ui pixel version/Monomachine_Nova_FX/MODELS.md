# Реализации режимов

Это таблица происхождения/метода, а не обещание идентичности оригинальному устройству.

| Dropdown | Реализация в поставке |
|---|---|
| GND / GND | Нулевая генерация (намеренная тишина) |
| GND / SIN | Синусоидальный осциллятор |
| GND / NOIS | Шум, sample hold, цвет и стереосмешивание |
| SID / 6581 | Новая приближённая tri/saw/pulse/noise-схема, PW envelope, sync/ring; НЕ эмулятор SID-чипа |
| SWAVE / SAW | PolyBLEP saw, detuned unison и два sub-осциллятора |
| SWAVE / PULS | PolyBLEP pulse, PW envelope, unison и sub-осцилляторы |
| SWAVE / ENS | Четыре осциллятора с независимыми offsets, шириной и медленным pitch wobble |
| DPRO / WAVE | Процедурные гармонические формы, crossfade и hard sync; не оригинальные таблицы |
| DPRO / BBOX | Модуль репозитория с исправлениями, 10 процедурных слотов / загрузка PCM |
| DPRO / DDRW | Процедурные формы и сглаженный crossfade; НЕ исходные DigiDraw-таблицы |
| DPRO / DENS | Четыре голоса процедурных форм с шириной и wobble |
| FM+ / STAT | FM Static из репозитория + ограничение фаз, pitch control и tone low-pass |
| FM+ / PAR | FM Parallel из репозитория + ограничение фаз, pitch control и tone low-pass |
| FM+ / DYN | FM Dynamic из репозитория + исправления фаз/pitch; внешняя AHD |
| VO / VO-6 | Новая приближённая формантная фильтрация и шумные согласные; не фонемный движок оригинала |
| FX / THRU | Основной аудиовход с INP-селектором, далее общая track-chain |
| FX / REVERB | JUCE Reverb (Freeverb-style) и нативный signal-triggered gate |
| FX / CHORUS | Модуль репозитория с sample-rate-sized кольцом и ограниченными taps |
| FX / DYNAMIX | Новый stereo-linked compressor, peak/RMS blend, threshold/ratio/makeup и mix |
| FX / RINGMOD | Модуль репозитория: внутренний носитель или cross-channel ring; добавлен TUNE |
| FX / PHASER | 6-stage allpass-модуль репозитория |
| FX / FLANGER | Модуль репозитория с кольцом по sample rate и ограниченными taps |

## Остальные файлы

- `NovaData.h`: каталог, параметры APVTS и диапазоны; базируется на `machine_definitions.hpp` и `track_pages.hpp`.
- `NovaDSP.h`: новые native engines, AHD, LFO и общая chain с EQ/SRR/delay.
- `models/arpeggiator.hpp`: переписанный fixed-storage scheduler с poll/advance и отдельными Note Off.
- `models/modulation_matrix.hpp`: исходная матрица с суммированием до clamp и проверкой индексов.
- `ui/MonomachineGuiLayout.hpp`, `parameter_conversions.hpp`, `exact_firmware_fm_dispatch.hpp` сохранены как описания/утилиты.
  Само присутствие этих файлов не означает выполнение DSP-прошивки. Адреса/комментарии firmware не проверялись.
- Исходный `monomachine_voice_chain.hpp` сохранён для справки/совместимости; в новой обработке используется `nova::TrackChain`.

Неиспользуемые `---` не получают фиктивных ручек/параметров. Для современных дополнительных BBOX SLOT/RAND/RSTR/LEV
и Ring TUNE задействованы ранее пустые позиции. Поэтому эта схема расширяет, а не точно повторяет аппаратную страницу.
