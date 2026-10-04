# Статический аудит — schema 45 с retained schema-44 DFB BASE BEND

**Дата:** 03.10.2026  
**Пакет:** Monomachine Nova 1.9.35, source-only  
**Граница проверки:** только статический анализ исходников и manifests. Ни полная
сборка, ни CTest, ни executable DSP tests, ни VST3/DAW-проверка, ни render не
запускались.

## Актуальная schema

Текущая `kDspModeSchemaVersion` равна **45**. Schema 45 добавляет independent
P1/P2 direct-LFO/MOD MATRIX state; она сохраняет ранее введённый schema-44 DFB
BASE BEND contract. `NovaConfig.h` обеих targets прямо маркирует это сочетание,
не выдавая schema 44 за текущую state schema.

Проверочные C++ sources `DelayFeedbackDspTests.cpp` и `FmFixModeTests.cpp`
теперь требуют schema 45 и формулируют schema-44 DFB как retained historical
contract. Это source correction тестовых ожиданий; исполняемые tests не
запускались.

## Final source-only запуск

```text
python3 -m py_compile *_STATIC_CHECK.py

python3 DFB_GUARD_CORE_STATIC_CHECK.py
STATIC AUDIT: PASS (148 checks; source-only, no build/test/render run)

python3 FM_MODE_CLEANUP_STATIC_CHECK.py
FM MODE CLEANUP STATIC CHECK: OK
```

DFB audit подтвердил byte-identical общий DFB/Track Delay source и retained
mode tests между `Monomachine_Nova_Synth` и `Monomachine_Nova_FX`; target-specific
`NovaConfig.h` различается только правильными `NOVA_SYNTH` identity и product
wording. FM audit подтвердил оба regenerated `SOURCE_BUILD.json`, metadata
`1.9.35`, mirrored shipping source и historical retained candidates.

## Проверенные DFB invariants

- Persisted P1/P2 APVTS controls `BASE CURVE` (`0.10..2.00`, fresh `.55`) и
  `HOLD @64` (`63/64..1.0000`, fresh `.9920`) сохранены.
- Runtime law статически подтверждён: RAW 63 — reference `63/64`; BASE CURVE
  формирует RAW 0..63; HOLD задаёт RAW 64; RAW 65+ сохраняет `RAW/64` growth.
- Для historical state schema `<44` migration добавляет neutral
  `BASE CURVE=1.00` и `HOLD @64=1.0000`, математически повторяя прежний strict
  `RAW/64` state.
- Fresh GUARD defaults: START@64 `0`, PLT@64 `.05`, START@127 `0`, PLT@127
  `3.46`, OFFSET `.18`, CURVE `1.08`, AMOUNT `2`, ATTACK/RELEASE `1 ms`.
  BASE LUT и GUARD `65×257` LUT rebuild только при реальном sanitized edit.
- DFB panel остаётся embedded VST/editor child overlay (`FloatingPanel`),
  с white text/pin, без `X` и без `DocumentWindow`; `400×386`, repaint `10 Hz`
  и bounded screen-coordinate drag сохранены.
- `DLY CORE` отсутствует из active menu/runtime/shipping Source: selector
  только `mnm|old|new`; saved value `3` schema 40/41 migrates в `new=2`.
  `FxSlotEngine` не получил speculative DFB route (`DFB_ROUTE_UNRESOLVED`).

## Связь с Matrix corrective pass

Schema-45 Matrix/LFO corrective work не меняет DFB DSP law. Оно обновляет только
current schema metadata/retained test expectations и добавляет source-only
Matrix/LFO fixes, записанные в
`VALIDATION_STATIC_AUDIT_2026-10-03_MATRIX_UI_FEEDBACK.md`.

Это по-прежнему source-only evidence: compilation, CTest, executable DSP,
VST3/DAW и render/audio tests намеренно не заявляются.