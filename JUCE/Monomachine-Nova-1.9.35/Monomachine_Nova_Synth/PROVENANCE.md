# Происхождение — Monomachine Nova Synth 1.9.35

## Основа и версия

- Базовая линия исходников: предоставленная стабильная ветвь 1.9.17/Package-4.
- Версия текущей source-only поставки и проекта: **1.9.35**.
- Проверенный commit исходного репозитория: `353dc02ae2111aa4fed734da6320bf3c9de7938d`.
- Исторический пользовательский путь Fix-5: `decompiled data/juce/fm/!fm fix patch/5 fix pitch+env fm full`.

## Архивированные экспериментальные импорты

Бывшие FM slots raw IDs 6, 7 и 8 (`mnm frq env fix`, TRY4 и Fix-5 full) сняты
из shipping Source вместе с их integration hooks, candidate-only tests и
fixture. Их полные относительные пути и SHA-256 сохранены в
`../PATCH_HISTORY/RETIRED_FM_CANDIDATES_1.9.26/`.

Текущий runtime не включает эти каталоги и не может выбрать их через MODE
SYNT. Для FM+ m8/m9/m10 действуют только six retained renderer IDs `0..5`;
сохранённые raw IDs `6..8` мигрируют в `mnm frq`.

`monomachine_chorus.hpp` и `monomachine_voice_chain.hpp` в данное снятие не
входили и оставлены без изменений. Исторический
`verify_dsp_mode_patch.py` сохранён в
`../PATCH_HISTORY/SNAPSHOTS/RETIRED_VERIFY_DSP_MODE_PATCH/`; он описывает
pre-cleanup registry и не является текущей проверкой. Для текущего контракта
используются `../FM_MODE_CLEANUP_STATIC_CHECK.py` и
`../DFB_GUARD_CORE_STATIC_CHECK.py`; полный список хэшей поставленного дерева
находится в `SOURCE_BUILD.json`.

## Границы проверки

Выполнен только source-only статический аудит, включая SHA-256 manifests и
архив retired material. Сборка VST3, CTest, executable DSP tests и проверка в
DAW в этой поставке не запускались; их должна выполнить штатная Windows-цепочка
перед бинарным релизом.
