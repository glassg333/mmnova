# Проверка v1.2.0 / Synth

## Выполнено

- JUCE 8.0.9, Linux x86_64, GCC 14.2.0, CMake/Ninja.
- Release `-O1 -DNDEBUG`, последовательная сборка. Успешны обе цели VST3 и создание moduleinfo.json.
- Начальная тишина / MIDI sample offsets / Note Off / sustain / last-note priority у Synth.
- Все 15 Synth-режимов (GND намеренно тихий) и все 7 FX-режимов проверены на выходной сигнал.
- FX gate и прозрачный global dry path, Panic.
- Каждая машина на 44.1/48/96/192 кГц, блоки 0/1/17/32/257/4096, минимумы/максимумы SYNTHESIS-параметров.
- Проверка конечности выходных сэмплов; mono/stereo на детерминированном сигнале.
- Арпеджиатор: события/gate на 120 BPM в KEY/SID/ADD; LFO: все пять форм.
- Матрица: unit-проверка и проверка изменения аудио включённым маршрутом.
- Сохранение/восстановление параметров; у Synth загрузка WAV, round trip встроенного PCM/state и playback после удаления файла.
- Создание/рендеринг/масштабирование GUI в Xvfb: Preview.png.

Результат:

```text
Parameters: 189
GND-GND peak=0
GND-SIN peak=0.271717
GND-NOIS peak=0.206062
SID-6581 peak=0.248161
SWAVE-SAW peak=0.316995
SWAVE-PULS peak=0.287245
SWAVE-ENS peak=0.271723
DPRO-WAVE peak=0.151031
DPRO-BBOX peak=0.25754
DPRO-DDRW peak=0.197692
DPRO-DENS peak=0.150149
FM+ STAT peak=0.209142
FM+ PAR peak=0.242513
FM+ DYN peak=0.207517
VO-6 peak=0.453062
Synth: PASS (1112383 assertions including per-sample finite checks)
```

Счётчик — количество assertions, включая проверку каждого сэмпла. Это НЕ число независимых сценариев/музыкальных тестов.
В тестовом WAV writer JUCE выдаёт предупреждение о deprecated overload; это не ошибка сборки плагина.

## Не выполнено

- Windows/MSVC и macOS/Xcode сборка; реальное сохранение exporter через GUI Projucer.
- Загрузка в конкретную DAW, pluginval/Steinberg validator, signing/notarization.
- Длительный realtime soak test, тест всех сочетаний модуляций/автоматизации.
- Сравнение с аппаратной Monomachine и подтверждение точности эмуляции.
- GitHub Actions workflow вложен, но не запускался.

Защита от NaN на выходе не является лимитером и не доказывает устойчивость всех комбинаций DSP.
В архив входят исходники; Linux-бинарники тестовой сборки не включены и всё равно не подошли бы для Windows/macOS.
