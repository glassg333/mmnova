# Происхождение и права — MonoJuice 0.8.0

Неофициальный исследовательский проект, ранее MonomachineNova FX5. Не является продуктом Elektron, TheWaveWarden, Uhhyou или Michael Nuzzo и не заявляет их одобрения.

## ODIN2 вновь включён

Источник: https://github.com/TheWaveWarden/odin2
Commit: `265e9e227581357efb87ed7217381f8a40c5e2bf`.

Copyright (C) 2020–2021 TheWaveWarden. **GPL-3.0-or-later**; `Licenses/ODIN2-GPL-3.0.txt`. Сохранённые исходные CombFilter/DCBlockingFilter и части manual — `Reference/odin2/`.

`Source/dsp/OdinComb.h` возвращён из FX5 0.5.0, с единственным техническим изменением имени `CombSettings` → `OdinSettings`, чтобы две модели не конфликтовали. Алгоритм самого native-порта 0.5 не изменён: адаптация от synth modulation pointers, стерео A/B, три маршрута, предвыделение буферов, ограничение resonance/истории. Upstream вычисляет DC-blocked значение, но возвращает другое выражение; старый порт сохраняет возвращаемую формулу. Новый общий DC BLOCK находится отдельно в выходной секции.

Сравнение выполнено с контрольной копией **порта 0.5**, не запущенным ODIN VST или полным синтезатором. `Reference/Odin_0_5_0.h` отличается только namespace/include path.

## Uhhyou ParallelComb сохранён

Источник: https://github.com/ryukau/VSTPlugins
Релиз **UhhyouPlugins0.70.0**, commit `cd1f6404d91d41c3270ed9862cbd77e987470883`.

Copyright Takamitsu Endo, `ryukau@gmail.com`, **GPL-3.0-only**. Текст: `Licenses/Uhhyou-GPL-3.0.txt`.

Модель из предоставленной папки Drive была идентифицирована по moduleinfo: ParallelComb, audio class **0.2.14.14**, CID `885129EAFB1F40189877B912E7F740AF`. Бинарник не встраивается и не запускается.

`Source/dsp/ParallelCombModel.h` и семь runtime helper headers сохранены из 0.7 без изменения. `Reference/Uhhyou/` — 17 закреплённых исходных/служебных файлов для аудита. Исторические metadata, release comparison и SHA-256 лежат в `TestsResults/FX5_0.6.0/`. Там все 17 файлов совпали с релизом.

Перенесены четыре отрицательно суммируемых отвода, feedback HPF/limiter, gate, cross/lean, M/S и 16×. В порте сглаживающие коэффициенты локальны на экземпляр вместо общего статического состояния. MIDI note-stack заменён ручным TUNE; preset импорт из исходного VST не реализован. Латентность исходной модели сообщается как 0; физические задержки её компонентов не объявляются нулевыми.

`Reference/Uhhyou/lib/juce_ScopedNoDenormal.hpp` — test-only helper под ISC, с собственным сохранённым notice. Test-only upstream oracle требует C++20; плагин не линкуется с VST parameter facade или runtime-эмулятором.

## Новый слой COMB и master-секция

`CombColour.h`, новые параметры и фабричные патчи, LFO/envelope tuning, drive/mix/output, общая обратная связь, mono-aware return, DC block и clipper — **новые самостоятельные функции MonoJuice**, не восстановленные функции ODIN/ParallelComb/Monomachine. Они лицензируются GPL v3 only, см. LICENSE_NEW_CODE.txt.

При нейтральном слое выполняется прежнее ядро. При активном DRIVE/MOTION/ENV TUNE это намеренно изменённая обработка; нельзя выдавать её за побитовую эквивалентность исходному стороннему плагину.

## SPECTRE

Основа: https://github.com/michaelnuzzo/spectrumPixelator
Commit `a74435fae817ed22c5c9b34f5fa38df06050de19`.
Copyright (c) 2019 Michael Nuzzo, **BSD-3-Clause**; `Licenses/SpectrumPixelator-BSD-3-Clause.txt`.

CLASSIC: `Source/dsp/SpectreClassic.h` использует processor/mask/overlap-add native-порта 0.5. Namespace изменён, идентичные KBD/FFT-планы разделяются с расширенным процессором через type aliases. `Reference/ClassicOracle.h` — отдельно сохранённый полный код 0.5 с изменёнными namespace/include; используется для сравнения.

EXTENDED: прежний `Source/dsp/Spectre.h` из 0.7 сохранён. Частотные границы, число/ширина пиков, reject level, mask attack/release, shift и spectral Freeze — собственные расширения MonoJuice, не функции MATLAB-исходника.

В обоих режимах сохраняется адаптация к восьми power-of-two окнам 512…65536 samples вместо непрерывного TIME RESOLUTION исходного MATLAB. KBD beta=5, MDCT/IMDCT, dB prominence и 50% overlap-add. MATLAB и оригинальный VST не запускались. Проверка CLASSIC означает совпадение с **нашим native-портом 0.5**, а не независимую сертификацию MATLAB-результата.

## CHORUS / PITCH / GRANULAR

`ChorusCore.h/.cpp`, `ChorusTables.h`, `AudioAdapter.h/.cpp` побайтно сохранены. PITCH/GRANULAR и FirmwarePrimitives также не изменены. Проверки: `TestsResults/source-audit.json` и `unchanged-chorus.json`.

Прошивка: https://github.com/glassg333/mmmm
Commit `e84501f512dd3d7d03a60dbf8a640ce70fb4f6f6`.
BIN SHA-256 `369849175602e20a9dd2b6e0ad8ac404b76f82718b14afbf1cbc01b7acabec7e`.
SYX SHA-256 `fe13c09f661cdba5fe57813240fa1574de1ee0a3ddd7047fc39d0318cba1a710`.

CHORUS: OS 1.32B, P:$147661 init, P:$14767B–$1477DD process; WID 4096 слов с $14A000, LP 258 слов с $144AC7. `Analysis/` — исторические материалы CHORUS 0.3.0, не описание новых функций 0.8. Runtime-эмулятор отсутствует.

PITCH и GRANULAR — новые структуры с исследованными firmware-примитивами, **не восстановленные аппаратные машины**. Прежние самостоятельные вклады сохраняют ограниченный MIT notice `Licenses/FX3-MIT-CONTRIBUTIONS.txt`, с firmware-derived исключениями.

## Ограничения объединённого продукта

Публичный доступ к BIN/SYX не устанавливает свободную лицензию. Исходная прошивка, восстановленные инструкции, коэффициенты и производные материалы **не перелицензированы в MIT/GPL**. Полные BIN/SYX в архив не включены, но производные материалы включены.

GPL-части нельзя объявлять MIT; сохранение BSD/MIT notices не отменяет copyleft-обязательства для производного объединённого продукта. **Перед публичным/коммерческим распространением объединённого бинарника нужно отдельно разрешить права на firmware-derived части и совместимость лицензий.** Этот архив таких отсутствующих разрешений не выдаёт.

JUCE SDK не включён и сохраняет условия используемой установки. GUI/шрифты оригинальных сторонних VST не включены. Исследовательские GPL-патчи в Analysis не являются runtime и не перелицензируют прошивку.
