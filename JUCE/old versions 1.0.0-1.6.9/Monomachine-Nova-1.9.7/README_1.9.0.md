# Monomachine Nova 1.9.0 — native DSP rollback

Дата выпуска: **26 сентября 2026**.

Этот исходный выпуск содержит два продукта:

- [`Monomachine_Nova_Synth/`](Monomachine_Nova_Synth/)
- [`Monomachine_Nova_FX/`](Monomachine_Nova_FX/)

## Решение выпуска

Экспериментальные source-derived DSP-ветви полностью удалены из обоих
проектов. Пользовательский FMA-пункт также удалён. Выпуск возвращает и сохраняет
штатные native/default пути без guessed substitute DSP, автоматического limiter,
дополнительного filter-darkening или fabricated envelope/DIST hybrid.

Остаются:

- `mnm|old` для SYNT, FILT, DIST и DLY;
- `old|mnm|vital` для AMP;
- оригинальный P2-список и native `MachineEngine` для P2 FX-слота;
- порядок обработки `EQ → FILT → DIST → ENV → VOL/PAN → SRR → DELAY`.

Устаревшие значения состояния, прежде выбирающие withdrawn ветви, при загрузке
приводятся к `mnm`; устаревший `p2_fx_mode` отбрасывается. Подробная совместимость
описана в документации каждого продукта.

## Документация

- [Synth: rollback, совместимость и состав](Monomachine_Nova_Synth/README_1.9.0.md)
- [FX: rollback, совместимость и состав](Monomachine_Nova_FX/README_1.9.0.md)
- [Текущий статус DIST](DIST_NEW_MODE.md)
- [Решение по evidence-аудиту DSP](DSP_EVIDENCE_AUDIT_2026-09-26.md)
- [P2 de-click и P2 VOL](P2_DECLICK_FIX.md)
- [Результаты валидации](VALIDATION_1.9.0_NATIVE_ROLLBACK.md)
- [Инструкция по сборке](BUILD_1.9.0_NATIVE_ROLLBACK.md)

Каждый продукт содержит `SOURCE_BUILD.json` с SHA-256-инвентарём файлов,
входящих в поставку.
