# SORTING_REPORT_2026-09-24 — v2 (пересборка из ПОЛНОГО клона)

## Зачем v2, если была v1
После выдачи v1 пользователь обнаружил пропажи: `digitone2.txt` нет, «файлов в оригинале было больше»,
не находится `ram program show hex on emulation.png`. Проверка по sha256 против полного клона
`github.com/glassg333/mmnova@main` (codeload-тарбол) показала: **в v1 отсутствовали 34 файла репозитория**.

Причина: рабочая копия, из которой собиралась v1, была снята с **устаревшего клона** — репозиторий
обновлялся (новые итерации добычи 19–24, заметки digitone2/routing, док MNM BLOCKS FOR JUCE),
а копия агента — нет. Это НЕ потеря при сортировке, а дефект источника. v1 честно сохранила всё,
что имела (1414 файлов), но её источник был неполон.

## 34 восстановленных файла (полный список)
- `digitone2.txt` (1924 B) — корень; `routing.txt` (1871 B) — корень.
- `zzz JUCE .../MNM BLOCKS FOR JUCE/synth → dist → srr → filt → eq → amplitude synth env (may be used by filter too) → dsnd.txt` (2746 B).
- `1 filter_phaser_pack/mnm_fm_exact.hpp` (8996 B, свежая версия).
- `19_coldfire_transport/` — 7 файлов (README, coldfire_excerpts.asm, dsp_dma_handlers.txt, exp26_*).
- `20_coldfire_clock/` — 5 файлов (README, coldfire_clock_excerpts.asm, exp27_*).
- `21_echo_kernel_routing/` — 6 файлов (README, exp28_*, kernel_echo_flow.asm, table_140000_mantissa.txt, table_1449C6_glide_rates.bin/.txt).
- `22_phaser_lfo/` — 5 файлов (README, exp29_lfo_rate.*, X_14A000_sine8k_full.txt, X_14A800_cos8k_full.txt, exp30_quad_verify.json).
- `23_dsnd_dtim_routing/` — README_dsnd_dtim.md.
- `24_dfb_knob_spec_map/` — 5 файлов (README, exp30_*).

## Что сделано в v2
1. Источник = полный клон (2043 файла `decompiled data`). Канон 01–08 скопирован как есть.
2. Хаос `09_not_sorted/missing_data` разобран: архивы → `12_mining_archive/`, PLAIN-таблицы
   final_recovery → `04_tables/`, гипотезы → `13_quarantine/hypotheses_unverified/`,
   анализы → `12_mining_archive/final_recovery_analysis/`.
3. `zzz JUCE` разобран по уровням доверия: 01_CORE_VERIFIED → `10_juce_import_ready/`,
   clean (MNM_BLOCKS_FOR_JUCE_CLEAN) → `10_juce_import_ready/clean_proof_audit/`,
   02_PROBABLE/05_LAB_PAGES/honest_fav/2deep → `11_juce_probable/`,
   03_ARCHIVE_SUPERSEDED и старые попытки → `13_quarantine/`.
4. Whitelist-пак `09_dsp_models_verified/` (09_juce_port + 22_fm_machines/mnm_fm_exact.hpp).
5. Скриншоты эмулятора — на постоянное место: `06_docs/reference_screenshots/ram program show hex on emulation.png`.
6. Дедуп по sha256 (первый по приоритету владелец сохраняет файл, остальные — в tsv).
7. Доказанные дубли удалены с проверкой КАЖДОГО хэша: `mmnova_firmware_mining_v6` (≡v7),
   `recovered`, `runtime`, `zero_in_image`. `zero_in_image_recovered` сохранён (обнаружены уникальные хэши).
8. Удалены ровно 24 документированных фейка `04_QUARANTINE/bbox_wav` (wav-подделки, не из прошивки).

## Верификация
- Скрипт-ассерт: **каждый уникальный sha256 репозитория присутствует в v2-дереве** — PASS
  (исключение: 12 хэшей bbox_wav-фейков, все владельцы — фейки).
- `sha256sum -c SHA256SUMS.txt` — PASS (см. финальный прогон в упаковке).
- Контрольные файлы: digitone2.txt [OK], routing.txt [OK], png [OK], mnm_fm_exact.hpp [OK], 01_CORE_VERIFIED [OK].

## Цифры
- В дереве: 1400 файлов (~47 МБ + доки), уникальных хэшей: 1385.
- Дублей удалено при сортировке: 634 (полный список: `06_docs/REMOVED_AS_DUPLICATE.tsv`).
- Фейков отброшено: 24 (bbox_wav).

## Отличия от v1 (для тех, у кого v1 на руках)
- Все пути v1, основанные на моём старом дереве, актуальны; v2 = v1-логика + полный источник.
- НЕ вошли в v2 (в отличие от v1): дополнительные аудиты пака iter28 (261 файл) — воркспейс-откат
  утерял их, gofile был недоступен для обратного скачивания. Исходники всего верифицированного
  содержимого в v2 ЕСТЬ (`09_juce_port`, `22_fm_machines`, `01_CORE_VERIFIED`). Если нужен пак iter28
  целиком — взять из архива v1 или попросить агента пересобрать.
- Новые места, о которых нужно знать: `06_docs/reference_screenshots/`, `12_mining_archive/zero_in_image_recovered/`,
  `12_mining_archive/_docs/` (старые манифесты и старый SHA256SUMS корня сохранены байт-в-байт).
