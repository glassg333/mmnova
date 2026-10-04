# Snapshot FX editor до queue-based RESET ALL — 1.9.35

Сохранён 03.10.2026 перед source-only изменением general `RESET ALL PARAMETERS`:
вместо одной длинной серии host `setValueNotifyingHost()` изменений значения
будут передаваться небольшими асинхронными message-thread chunks.

Этот snapshot сохраняет активный FX `Source/PluginEditor.cpp` после Matrix UI
feedback pass и до reset-queue follow-up. SHA-256 исходника указан в
`SHA256SUMS.txt`. Snapshot не является активным shipping source и не проходил
сборку, DSP/DAW/VST3 test или render.
