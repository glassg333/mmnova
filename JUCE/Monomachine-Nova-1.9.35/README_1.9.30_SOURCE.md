# Monomachine Nova 1.9.30 — актуальные исходники (source-only)

Дата актуальной source revision: 03.10.2026.

Это добавочная исходная поставка для двух веток:

- `Monomachine_Nova_Synth`;
- `Monomachine_Nova_FX`.

Предыдущие source archives 1.9.21 и 1.9.27 сохранены без удаления.
Полные активные README и validation log релиза 1.9.27 также сохранены без
сокращения в `PATCH_HISTORY/SNAPSHOTS/RELEASE_1.9.27_SOURCE_ONLY/`; текущий
source archive не перезаписывает исторические материалы.

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

- Schema поднята до **43**: прежние FM-normalization rules и CHOR A/B
  сохранены; неработающий DLY CORE снят из поставляемых исходников, а DFB
  использует RAW-gated окно anchors @64/@127 внутри VST.
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
- `Source/dsp/DelayFeedbackDynamics.hpp`, `Source/dsp/mnm/MnmDelay.hpp`
  и `Source/dsp/mnm/MnmTrackDelayNew.hpp`;
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

## Track Delay: DFB BASE + RAW GUARD

Старый capture/HOLD вариант и прежнее schema-42 level-window описание сохранены
без сокращений в
`PATCH_HISTORY/SNAPSHOTS/DFB_SCHEMA42_LEVEL_WINDOW_PRE_SCHEMA43_2026-10-03/`.
Они не являются текущей спецификацией.

Schema **43** оставляет BASE при включённом `DYNAMICS` строго `RAW / 64`, с
unity ровно на raw `64`, и задаёт новый пользовательский контракт GUARD:

- RAW `0..63` — полный GUARD bypass: низкие DFB значения не несут pressure
  защиты и не используют накопленный GUARD level;
- RAW `63..64` — только короткий мягкий arm; на raw `64` BASE остаётся ровно
  `1.000`;
- участок RAW `64..127` редактируется четырьмя persistent anchors:
  `START @64`, `PLT @64`, `START @127`, `PLT @127`;
- `GUARD OFFSET` сдвигает весь этот диапазон; `GUARD CURVE`, `GUARD AMOUNT`,
  `ATTACK`, `RELEASE` остаются отдельными ручными controls;
- reference defaults: START@64 `.55`, PLT@64 `.73`, START@127 `.55`,
  PLT@127 `.73`, OFFSET `0`, CURVE `1`, AMOUNT `1`, ATTACK `37 ms`,
  RELEASE `184 ms`;
- DFB открывается по RMB как белый compact child overlay **внутри VST**. Это
  не native OS-window: панель не перехватывает фокус DAW, закрывается крестом
  или повторным RMB по DFB и перетаскивается за заголовок в пределах редактора;
- оба live graph/marker, все toggles и nine controls сохранены. `DYNAMICS`,
  оба `DSND ± FB INVERT`, `RETURN COMP` и `FB CLIP / GUARD` стартуют ON.

Schema-39 anchors и schema-40 `dfb_guard_raw` остаются compatibility-only
APVTS/state IDs. При migration schema `<43` сохранённые START/PLATEAU становятся
одинаковыми anchors @64 и @127, а новый OFFSET получает `0`: старые проекты не
теряют state и получают нейтральный исходный отрезок до ручной настройки.

`DLY` в DSP menu содержит только `mnm | old | new`. Неработающий `core` снят
из UI, runtime, state owner, tests и shipping Source. При загрузке state schema
40/41 сохранённое бывшее значение `core=3` явно мигрирует в `new=2`; прочие
ошибочные DLY values нормализуются в `mnm`.

DBAS/DWID сохраняют текущий 12 dB feedback-filter response; selector slope не
добавлялся. `FxSlotEngine` по-прежнему не получил speculative DFB routing:
`DFB_ROUTE_UNRESOLVED` остаётся границей текущей реализации.

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

## Статическая проверка этой поставки

Для актуальной source-only ревизии используется только локальный static audit:

```bash
python DFB_GUARD_CORE_STATIC_CHECK.py
python FM_MODE_CLEANUP_STATIC_CHECK.py
```

Первый проверяет schema 43, DFB RAW-gate/anchors/OFFSET parameter/runtime/live-
graph связку, embedded VST panel и отсутствие удалённого DLY CORE. Второй
сохраняет расширенный audit retained FM/CHOR/Phaser и source metadata. Оба
проверяют reference defaults, byte-identical Synth/FX mirrors и отсутствие
speculative `FxSlotEngine` DFB route; также они проверяют target pairing
`Synth:NOVA_SYNTH=1` / `FX:NOVA_SYNTH=0`. Ни один не компилирует и не исполняет
DSP. Результат актуального запуска записан в
`VALIDATION_STATIC_AUDIT_2026-10-03_DFB_RAW_WINDOW.md`; schema-42 audit сохранён
в historical snapshot.
