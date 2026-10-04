# Статический аудит — schema 43 DFB RAW window

Дата: 03.10.2026.  
Тип: **только source/static audit**. Этот документ не утверждает результат
сборки, CTest, executable DSP tests, VST3, DAW или render.

## Исправленный пользовательский контракт

- `DYNAMICS=ON` сохраняет strict BASE `RAW / 64`; raw `64` остаётся `1.000`.
- RAW `0..63` — настоящий GUARD bypass: low RAW не несёт предыдущий GUARD
  level/pressure.
- Выбранный пользователем короткий soft transition — только на `RAW 63..64`.
- RAW `64..127` имеет четыре независимых persistent endpoint controls:
  `START @64`, `PLT @64`, `START @127`, `PLT @127`.
- `GUARD OFFSET` сдвигает весь protection window; `CURVE`, `AMOUNT`, `ATTACK`
  и `RELEASE` сохранены как отдельные state controls.
- RMB DFB panel больше не `DocumentWindow`: это white movable child overlay
  внутри VST/Surface. Она не создаёт native desktop window и не должна
  перехватывать DAW focus.
- Старый неработающий DLY CORE не возвращён: active selector — `mnm|old|new`;
  saved schema-40/41 value `3` мигрирует в `NEW=2`.

## State compatibility

Schema поднята с 42 до **43**. Для state `<43` migration создаёт новые IDs
`dfb_guard_level_start_127`, `dfb_guard_plateau_127`, `dfb_guard_offset` для
P1 и P2. Существующие saved START/PLATEAU копируются в оба endpoint, OFFSET
получает `0`; CURVE/AMOUNT/ATTACK/RELEASE не перезаписываются. Schema-39/40
legacy IDs сохранены для загрузки старых projects/presets, но не показаны в
active panel.

## Фактически выполненные проверки

- `python3 DFB_GUARD_CORE_STATIC_CHECK.py`:
  **`STATIC AUDIT: PASS (118 checks; source-only, no build/test/render run)`**.
- `python3 FM_MODE_CLEANUP_STATIC_CHECK.py`: **`OK`**.
- Проверены byte-identical common Synth/FX source и retained tests.
- Проверены target-specific config identities: Synth `NOVA_SYNTH=1`, FX
  `NOVA_SYNTH=0`.
- `SOURCE_BUILD.json` для каждого продукта содержит 167 актуальных SHA-256
  entries после изменения source.
- Лексический delimiter scan изменённых C++ source/test mirrors: **PASS**
  (20 файлов; это не компиляция).

## Не выполнено намеренно

Не запускались Projucer, MSBuild, CMake build, CTest, executable DSP tests,
VST3, DAW или render. Внешний Windows FX pairing failure до schema-43 сохранён
как historical material в
`PATCH_HISTORY/SNAPSHOTS/DFB_SCHEMA42_LEVEL_WINDOW_PRE_SCHEMA43_2026-10-03/`;
он не является подтверждением сборки текущей поставки.
