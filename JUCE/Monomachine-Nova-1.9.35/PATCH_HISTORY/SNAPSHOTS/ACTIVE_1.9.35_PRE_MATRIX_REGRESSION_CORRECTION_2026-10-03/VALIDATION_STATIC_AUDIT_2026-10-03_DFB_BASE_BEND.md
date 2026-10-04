# Статический аудит — schema 44 DFB BASE BEND

**Дата:** 03.10.2026  
**Пакет:** Monomachine Nova 1.9.35, source-only  
**Граница проверки:** только статический анализ исходников и manifests. Ни полная
сборка, ни CTest, ни executable DSP tests, ни VST3/DAW-проверка, ни render не
запускались.

## Выполненные команды

```text
python3 DFB_GUARD_CORE_STATIC_CHECK.py
STATIC AUDIT: PASS (148 checks; source-only, no build/test/render run)

python3 FM_MODE_CLEANUP_STATIC_CHECK.py
FM MODE CLEANUP STATIC CHECK: OK
```

Обе проверки подтвердили byte-identical общий DFB/Track Delay source и retained
mode tests между `Monomachine_Nova_Synth` и `Monomachine_Nova_FX`; target-specific
`NovaConfig.h` остался разным только по правильному `NOVA_SYNTH` identity.

## Проверенные изменения DFB

- Schema поднята с 43 до **44**.
- Добавлены persisted P1/P2 APVTS controls `BASE CURVE` (`0.10..2.00`, fresh
  default `.55`) и `HOLD @64` (`63/64..1.0000`, fresh default `.9920`).
- Runtime law статически подтверждён: RAW 63 является reference `63/64`; BASE
  CURVE формирует RAW 0..63; HOLD задаёт RAW 64; RAW 65+ сохраняет `RAW/64`.
  Low side, GUARD, DTIM, delay buffer, send и return gain не смешаны.
- Для state schema `<44` migration добавляет нейтральные `BASE CURVE=1.00` и
  `HOLD @64=1.0000`. Эта пара математически повторяет прежний strict `RAW/64`,
  поэтому существующий проект не переводится на новый fresh curve молча.
- Свежие GUARD defaults совпадают с принятой настройкой panel: START@64 `0`,
  PLT@64 `.05`, START@127 `0`, PLT@127 `3.46`, OFFSET `.18`, CURVE `1.08`,
  AMOUNT `2`, ATTACK/RELEASE `1 ms`.
- `setGuardAndGovernor()` теперь сравнивает sanitized snapshot и не rebuild
  guard-drive LUT `65×257` без реального edit. BASE использует отдельный LUT,
  который также rebuild только при edit. Это source-level исправление причины
  повторной тяжёлой работы; фактическое idle-CPU измерение требует будущего
  запуска в DAW.
- DFB panel остаётся embedded VST/editor child overlay: `FloatingPanel`, white
  text, pin, без `X` и без `DocumentWindow`; размер `400×386`, repaint `10 Hz`,
  bounded screen-coordinate drag. Static audit подтвердил отсутствие detached
  DFB-window route и obsolete hidden controls в panel.
- `DLY CORE` остаётся снятым: menu/runtime state допускает только
  `mnm|old|new`, а saved value `3` schema 40/41 мигрирует в `new=2`.
- `FxSlotEngine` по-прежнему не получил speculative DFB route
  (`DFB_ROUTE_UNRESOLVED`). DBAS/DWID 12 dB contract, CHOR Native/Core, FM и
  Phaser contracts не менялись.

## Дополнение ARP UI

- Статически проверено отсутствие рисуемой строки `PAGE 1..64` в ARP canvas.
- Первый пункт COPY/PASTE menu теперь ASCII `WHOLE PAGE`.
- SONG toggle сохраняет control/tooltip, но его перекрывающийся text label
  очищен.
- Static check сканирует все active `setTooltip` calls `PluginEditor.cpp` и
  запрещает кириллицу в tooltip payload; function descriptions остаются
  English/ASCII для текущего pixel-font tooltip window.
- После этого ARP follow-up оба source-only audit повторно прошли: DFB static
  audit — `PASS (148 checks)`, FM/metadata/static audit — `OK`.

## Manifests и выпуск

Перед упаковкой обновлены оба `SOURCE_BUILD.json` (по 167 payload hashes),
актуальные README/source map и root SHA-256 package manifest. Предыдущий
`VALIDATION_STATIC_AUDIT_2026-10-03_DFB_RAW_WINDOW.md` сохранён как historical
schema-43 record и не выдаётся за проверку BASE BEND.

## Дополнение MOD ENV main-surface UI

- `MOD ENV → AMP/FIL ENV` при удерживаемой ЛКМ статически прослежен как
  atomic dock handoff: новый selected state устанавливается до destruction
  outgoing mini dock, а `dismissDock()` получает явный режим сохранения этого
  state во время replacement.
- Для `PixelButton` с `heldDrag` generic white drag-pressed fill отключён;
  отображается только фактическая toggle selection target. Это снимает ложное
  белое состояние исходной `MOD ENV` до отпускания ЛКМ.
- `ModEnvelopeMiniPage` статически зафиксирован на AMP/FIL compact geometry:
  `387×236`, header `28 px`, graph `8,32,371,114`, slider grid
  `x=8+i×95`, `y=152`. В mini отсутствуют page/ROUTE widgets и paint strings
  `MATRIX SOURCE` / `RMB LARGE`; выбор source и route остаются на full MOD ENV
  page по RMB.
- `FM_MODE_CLEANUP_STATIC_CHECK.py` расширен этими invariants: compact geometry,
  отсутствие лишнего MINI UI и порядок held-LMB replacement. В проверке также
  сохранён Synth/FX byte-identical requirement для `PluginEditor.cpp`.

Это по-прежнему source-only evidence: никаких compile, CTest, executable DSP,
VST3/DAW или render/audio tests не запускалось.
