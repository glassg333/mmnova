# Monomachine Nova 1.9.30 — актуальная навигация Synth

Это Synth часть source-only пакета. Текущий FM+ MODE SYNT для STAT/PAR/DYN
содержит шесть selectable значений: `mnm frq`, `old frq`, `new frq`, `mnm bpm`,
`new bpm`, `old bpm` (renderer IDs `0..5`). Experimental raw IDs `6..8`
недоступны и при загрузке state нормализуются в `mnm frq`.

## Читать в таком порядке

1. [`../README_FIRST.md`](../README_FIRST.md) — текущая точка входа.
2. [`../README_1.9.30_SOURCE.md`](../README_1.9.30_SOURCE.md) — актуальный FM MODE contract, schema 42 DFB BASE/GUARD LVL и снятый неработающий DLY CORE, ARP SONG UI, CHOR Native/Core и MSVC C2397 fix.
3. [`../TRACK_DELAY_CORRECTION_2026-09-30.md`](../TRACK_DELAY_CORRECTION_2026-09-30.md) — действующий статус Track Delay и граница `DFB_ROUTE_UNRESOLVED` для custom FX slots.
4. [`../PATCH_HISTORY/README_RU.md`](../PATCH_HISTORY/README_RU.md) — навигация по истории.
5. [`../PATCH_HISTORY/FIXES_APPLIED_1.9.21.md`](../PATCH_HISTORY/FIXES_APPLIED_1.9.21.md) — полные старые patch/readme/validation записи.

Исторические документы не являются текущей DSP/UI спецификацией. Полная сборка,
VST3/DAW, render и test execution этой source-only поставкой не заявляются.
