# Monomachine Nova 1.9.27 — актуальные исходники (source-only)

Дата: 30.09.2026.

Это добавочная исходная поставка для двух веток:

- `Monomachine_Nova_Synth`;
- `Monomachine_Nova_FX`.

Предыдущие source archives 1.9.21 сохранены без удаления. Номерные
исторические patch/readme/validation записи собраны в `PATCH_HISTORY/`; текущий
source archive намеренно не перезаписывался этой документной организацией.

## Что изменено в MODE SYNT

У FM+ `STAT` (m8), `PAR` (m9) и `DYN` (m10) осталось ровно шесть selectable
режимов в этом порядке:

| ID renderer | Новая подпись |
|---:|---|
| 0 | `mnm frq` |
| 1 | `old frq` |
| 2 | `new frq` |
| 3 | `mnm bpm` |
| 4 | `new bpm` |
| 5 | `old bpm` |

Числовые renderer IDs и DSP mappings этих шести путей не переставлялись. В
частности, `new bpm` остаётся ID 4, а `old bpm` — ID 5.

Из пользовательского MODE SYNT удалены experimental slots:

- `mnm frq env fix` (бывший raw ID 6);
- `try4` (бывший raw ID 7);
- `fix5 pitch+env full` (бывший raw ID 8).

Полные исходники импортированных экспериментов, candidate-only tests и fixture
сохранены с SHA-256 в `PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`, а из
shipping Source удалены. Они не попадают в choices, UI/state allow-list,
processor-owned runtime route или compilation graph.

## Состояние и защита от неверного выбора renderer

- Schema поднята до **38**: прежние FM-normalization rules и CHOR A/B
  сохранены; добавлен serializable DFB dynamics state с default `ON`.
- При загрузке старого state raw ID 6, 7 или 8 нормализуется в `mnm frq` (ID 0).
  Он не ограничивается до ID 5, поэтому не происходит скрытого перехода на
  `old bpm`.
- Та же защита есть в processor snapshot и в границе `MachineEngine::setModes`.
- Обычный mode parameter FM+ теперь имеет диапазон `0..5` и шесть labels.

## Изменённые рабочие исходники

В обеих ветках синхронно обновлены:

- `Source/models/DspModes.hpp`, `Source/NovaData.h`;
- `Source/PluginProcessor.{h,cpp}`;
- `Source/PluginEditor.cpp`;
- `Source/NovaDSP.h`;
- `Source/dsp/TrackVOLPAN.inl`, `Source/dsp/TrackDelay.inl`;
- `Source/dsp/DelayFeedbackDynamics.hpp`, `Source/dsp/mnm/MnmDelay.hpp` и
  `Source/dsp/mnm/MnmTrackDelayNew.hpp`;
- `Source/dsp/AudioAdapter.{h,cpp}` и `Source/FxSlotEngine.h`;
- `CMakeLists.txt`, `tests/FmModeRetirementTests.cpp` и обновлённые source-only
  проверки `tests/DelayFeedbackDspTests.cpp` / `tests/FmFixModeTests.cpp`.

Сняты candidate-only imports, пустой FM STAT tombstone, profile header, CMake
registrations, tests и fixture; точные версии до снятия лежат в
`PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`. Active m9 PAR unity readout
сохранён без candidate profile header. Указанные общие файлы и retained-mode
тесты Synth/FX проверяются на byte-identical mirror. Их ранее сохранённые
registry test sources также остаются в
`PATCH_HISTORY/SNAPSHOTS/FM_MODE_TESTS_PRE_CLEANUP_2026-09-30/`.

## Phaser

Для FX machine ID 18 в `Source/dsp/monomachine_effects.hpp` full-wet output
заменён с unity-gain all-pass image на bounded phase-difference:
`0.5f * (allpassCascade - input)`. Старая формула сохранена закомментированной
сразу рядом с новой. Это целевая практическая коррекция без заявления о
доказанном hardware-эквиваленте; machine ID, шесть передаваемых ручек и routing
не менялись.

## Chorus: реальный A/B runtime и INP

В DSP-меню `CHOR` теперь есть два **реальных** значения параметра `mode_cho`:

| index | подпись | вызываемое ядро |
|---:|---|---|
| 0 | `native` — default | `NativeChorusCore` через `AudioAdapter` |
| 1 | `core` — A/B | эталонный `ChorusCore` через тот же `AudioAdapter` |

Переключатель доходит до обычного P1, P2 и уже созданных пользовательских
FX-slots. Он не меняет host `INP`, dry/wet/MIX, gain, latency или routing.
При смене `native`/`core` `AudioAdapter` сознательно сбрасывает audio/tail state
обоих ядер: совместимый перенос X/Y/delay-memory не выдумывается. Поэтому A/B
начинается с чистого состояния, а Native остаётся и factory/default, и путём
для всех старых state (schema `<37` принудительно мигрирует в `native`).

`CHORUS SAFE` не выбирает ядро: при default OFF работает выбранное ядро и
сохраняет tail при machine `MIX=0`; SAFE=ON только обходит/сбрасывает это же
выбранное runtime-ядро при `MIX=0`.

По пользовательскому сравнению только host-участок `INP −64..−60` получил
квадратичный ease-in: −64 остаётся нулём, −60 и все значения выше сохраняют
прежний коэффициент `raw/64`. Меняются только −63/−62/−61; это не изменение
native 24-bit core и не заявление о hardware-формуле.

## Track Delay: DFB dynamics и экспериментальная polarity

Новая временная law DFB действует во всех трёх путях Track Delay (`OLD`, `MNM`,
`NEW`) и **включена по умолчанию**, включая migration старых state:

| DFB raw | Поведение DFB DYNAMICS=ON |
|---:|---|
| `64` | точный feedback `1.0`: unity hold без намеренного роста или затухания |
| `63` | `63/64`: мягко прекращает удержание без CLEAR/PANIC |
| `65..127` | bounded over-unity часть плавно меняется при значении выше 64; верхняя граница коэффициента — `1.125` |
| `0` | не чистит buffer: feedback линейно отпускается по `ZERO TAIL` |

`RISE` (default `1.20 s`) определяет плавность bounded over-unity части при
`DFB>64`; `ZERO TAIL` (default `0.65 s`) — время мягкого ухода коэффициента к
нулю. Наблюдение о примерно секундной задержке изменения громкости при быстрых
жестах DFB **не** объявлено отдельным законом исходника: неизвестно, относится
ли оно к send, feedback, return или иной части маршрута. Поэтому controller не
навязывает искусственный reset для `0→127→0`, а сохраняет своё состояние
непрерывным. Реальное время слышимого последнего повторения всё ещё зависит от
`DTIM`: уже записанное содержимое линии физически приходит к write точке только
через длину delay.

ПКМ по ручке `DFB` открывает одно мини-окно: `DFB DYNAMICS`, `RISE`, `ZERO
TAIL`, прежние `RETURN COMP`/`FB CLIP` и два default-OFF experimental switches:
`DSND + FB INVERT` и `DSND - FB INVERT`. Они меняют только знак recurrence,
не DSND mono/split send route. `DYNAMICS=OFF` оставляет отдельный A/B путь с
прежней прямой law `raw / 63`.

`DBAS`/`DWID` и выбранная текущая feedback-filter response **не менялись**:
остается каскад двух двухполюсных Korg-35 edge stages (nominally 12 dB на
каждую границу); 12/24 selector не добавлен. Новый controller не является
panic-clear, automatic RMS normaliser или обязательным clipper: `FB CLIP`
по-прежнему отдельная явная защита в том же RMB-окне.

## ARP SONG: активное окно и рисование

`S START`/`S END` теперь задают не только playback range: в поле SONG
показываются **только** активные позиции этого диапазона. Они динамически
занимают ширину строки тем же образом, что и обычные ARP step rows; скрытые
SONG positions не удаляются и не переписываются.

ЛКМ-перетаскивание по строке `S-PART` или `REPEAT` работает кистью: значение рисуется
через все активные позиции, которые пересекает курсор, включая пропущенные при
быстром горизонтальном движении. Для каждой затронутой позиции открывается и
закрывается собственный жест хоста. ПКМ по одной ячейке, как прежде, возвращает
только эту позицию к её заданному default.

## Исправление MSVC C2397

В `Source/NovaData.h` для SONG part page default добавлен явный
`static_cast<float>(part+1)`. Это исправляет MSVC C2397: переменный `int`
в list-initializer `Spec::def` нельзя неявно сужать до `float`.

## Самостоятельная сборка

Нужен отдельный checkout JUCE 8. Пример для каждой ветки (подставьте абсолютный
путь к своему JUCE):

```bash
cmake -S Monomachine_Nova_Synth -B build-synth -DJUCE_DIR=/абсолютный/путь/к/JUCE
cmake --build build-synth --config Release

cmake -S Monomachine_Nova_FX -B build-fx -DJUCE_DIR=/абсолютный/путь/к/JUCE
cmake --build build-fx --config Release
```

`NOVA_BUILD_TESTS` по умолчанию выключен. В этой поставке не заявляется
результат полной сборки, VST3/DAW-проверки, render или выполнения тестов:
пользовательская сборка остаётся следующим gate.

## Выполненная статическая проверка

Запускался только новый локальный статический аудит:

```bash
python FM_MODE_CLEANUP_STATIC_CHECK.py
```

Он проверяет шесть FM labels, IDs 0..5, schema 38, отсутствие candidate
source/hooks/CMake targets из shipping tree, SHA-256 архивной истории, защиту
raw state, Phaser replacement с прежней формулой рядом, реальный `mode_cho`
route `NativeChorusCore|ChorusCore` в P1/P2/FX slots, DFB dynamics
route/legacy A/B/polarity switches, активное поле/кисть ARP SONG и
синхронность ключевых Synth/FX файлов. Он не компилирует и не исполняет DSP.
