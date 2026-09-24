# 12_mining_archive — ИНДЕКС АРХИВА (v2r4, раскладка по темам)

Архив истории добычи данных из прошивки Monomachine SFX-60 OS 1.32B.
Это ЖУРНАЛ ПОИСКА, а не эталон: всё, что доказано бит-в-бит, вынесено в эталонные
папки `04_tables/`, `09_dsp_models_verified/`, `10_juce_import_ready/`. Здесь — как
это добывалось: сырые распечатки, замеры, скрипты, промахи и выводы.

**Принцип раскладки (v2r4):** одна полка = одна тема или один тип данных.
Внутри полки — только файлы, БЕЗ вложенных папок. Версионных имён (v7, iteracy4,
all_tables_v4) больше нет: содержимое разложено по смыслу, а история прогонов
сохранена в таблице «Хронология добычи» внизу и в `_docs/MOVE_MAP_v2r4.tsv`
(карта «старый путь → новый путь» для каждого из 928 файлов).

## Карта полок

### Движки (всё по машине — в одном месте)

| Полка | Что внутри |
|---|---|
| `01_filter/` | ФИЛЬТР: реконструкция трек-фильтра по коду ядра (disasm P:$04A8–0B49), доказательство SVF-рекурсии, подгонка коэффициентов (svf_fit*), сверка с мануалом (exp_f3*), модель stage-2 (exp18 FDN), хвост stage-2 (exp22), доказанные таблицы фильтра (env_shape_tbl A–F, coeff_base, div/width, resonance, kernel_tbl, LP coeffs) |
| `02_phaser/` | ФЕЙЗЕР m18: разбор $145036–$14538D, harness/IR/FFT/notches, хвост PROC, финальная бит-точная модель + C++ порт (mnm_phaser.h, test_vectors.h), законы LFO фейзера (exp29) |
| `03_lfo/` | LFO: формы и траектории (fx_identify_fr, lfo_delaytraj), полные таблицы sine/cos 8K $14A000/$14A800 + quad-верификация + головы таблиц |
| `04_envelopes/` | ОГИБАЮЩИЕ: бит-точная реконструкция AMP-энвелопа (свипы/замеры), вердикт «у фильтра нет своей огибающей» (exp_f2*) |
| `05_echo_delay/` | ЭХО/DELAY: per-voice ECHO (DTIM/DSND/DFB/DBAS/DWID), роутинг ядра эха, глайндвижок, таблица glide rates $1449C6, мантисса $140000 |
| `06_reverb/` | РЕВЕРБ m13: полный структурный разбор + reverb_tables + замеры; таблицы FXREV DEC/DAMP |
| `07_distortion/` | DIST: замеры/бисекция/тест-векторы (probe23*), статус DIST, вердикт $141A00 = kDistRel |
| `08_fm/` | FM-машины m8/m9/m10: бит-точная перепроверка, законы FM, mnm_fm_exact.hpp (+ его копия, совпадающая с whitelist) |
| `09_coldfire_transport/` | ColdFire→DSP: транспорт HI08/DMA5, тактовый модуль, хост-протокол main OS; JSON-аназы поиска coldfire-байтов |
| `10_controls_routing/` | РУЧКИ И РОУТИНГ: карта страницы голоса, данные ручки DSND + роутинг DTIM (свод), вердикт по DFB + карта «ручка→ячейка» |
| `11_sequencer/` | СЕКВЕНСОР: декодер паттернов (patterns), глобальные слоты/локи (locks), ячейки редактора (cells): slot maps, disasm-доказательства, patterns/locks/cells_decoded.json |
| `18_juce_port/` | JUCE-ПОРТ: готовый dual-фильтр (Source/Example/tests — файлы плоско), README_JUCE, список недостающего для полного порта |

### Данные (тип данных = полка)

| Полка | Что внутри |
|---|---|
| `12_factory_data/` | ЗАВОДСКИЕ ДАННЫЕ из прошивки: kits/patterns/songs по слотам (bin-файлы с именами слотов), factory_index.json, names_index.txt, zone_map.json |
| `13_wavetables/` | ВЕЙВТЕЙБЛЫ: 64 таблицы wt00–wt63 (SINE…FEM_, bin+txt), 18 волн dpro (P101D7B…P112D7B), заводская зона SID $140800/$141000 |
| `14_raw_dumps/` | РОУ ДАМПЫ памяти: blk1–blk4_full.bin (заводские блоки), DSP1/DSP2 internal, DSP common external, diff ядер, записи HI08-потока, DSP-образы из прошивки, main OS (строки, первые 1024 байта, сводка), инфо о прошивке и контрольные суммы секций |
| `15_tables_pmem/` | ТАБЛИЦЫ ИЗ P-ПАМЯТИ: полная выгрузка по адресам (P_*/X_*/Y_*), таблицы, снятые с реального pmem (*_FROM_PMEM), манифесты выгрузки, скан ссылок на таблицы, карта диспетчеризации машин, численная верификация, коррекции прошлых добыч |
| `16_hypotheses/` | ГИПОТЕЗЫ (НЕ данные): реконструкция таблиц, которых нет в образе (HYPOTHESIS_sine1024, expdecay и т.п.) — интерпретации, не подтверждены |
| `21_superseded_names/` | ОПРОВЕРГНУТЫЕ ИМЕНА таблиц (эпоха v4): P_141900_PHASER_stage0 и т.п. — имена опровергнуты поздними доказательствами; читай README_REFUTED_NAMES.md перед использованием! |

### Инструменты и история

| Полка | Что внутри |
|---|---|
| `17_tools_emulator/` | ИНСТРУМЕНТЫ: бит-точный эмулятор DSP56300 (dsp_emu.py, harness-ы, замеры), декодер прошивки decoder_v2 (arrays.npz, code_full.asm, table_bases) |
| `19_disasm_listings/` | ПОЛНЫЕ ЛИСТИНГИ дизассма ключевых блоков: SVF-ядро, dual-фильтр, m18, filter envelope, FX DYN, VO6 |
| `20_run_logs/` | ЖУРНАЛЫ ПРОГОНОВ: README и MANIFEST каждого прогона добычи, пайплайн/стейджи дампа DSP-памяти, депак-логи |
| `_docs/` | Старые манифесты архива (история версий), SHA256SUMS__old_root.txt, карта перемещений MOVE_MAP_v2r4.tsv |

## Как пользоваться
1. Нужны ПРОВЕРЕННЫЕ данные → `04_tables/`, `09_dsp_models_verified/`, `10_juce_import_ready/` (корень дерева), НЕ отсюда.
2. Нужна тема («как устроен фейзер») → полка темы (02_phaser) + связанное: таблицы в 15_tables_pmem, листинги в 19_disasm_listings, журнал в 20_run_logs.
3. Наткнулся на файл с подозрительным именем (PHASER_stage0, Q_resonance…) → сначала `21_superseded_names/README_REFUTED_NAMES.md`.
4. Ничего не удаляй: это единственная запись «почему фильтр именно такой».
5. Новая добыча: файлы — в тематическую полку, журнал прогона — в `20_run_logs/<тема>_run_README.md`, строка в таблицу хронологии ниже.

## Хронология добычи (где что лежит теперь)

Сквозная история: итерация 4 → 30. Номера сохранены для ссылок из старых README;
файлы каждой итерации разложены по темам.

| # | Прогон (старое имя) | Что делалось | Файлы теперь |
|---|---|---|---|
| 4 | dsp_mem_dump (iteracy4) | Карта памяти обоих DSP, депак заводских данных, 64 вейвтейбла | `13_wavetables` (wt*), `14_raw_dumps` (blk*, DSP*, records), `20_run_logs/dsp_mem_dump_run_*` |
| 5 | fxrev_waves (iteracy5) | Фантомные адреса: какие «таблицы» НЕ существуют; реальные FXREV, dpro-волны, SID-зона | `06_reverb` (FXREV_*), `13_wavetables` (dpro_wave_*, SID_*), `20_run_logs/fxrev_waves_run_*` |
| 6 | firmware_factory (v7) | HI08-записи, DSP-образы из прошивки, factory data, main OS | `12_factory_data`, `13_wavetables` (SID-зона), `14_raw_dumps` (hi08, dsp images, main OS), `20_run_logs/firmware_factory_run_*` |
| 8 | seq_patterns (v8) | Декодер паттернов: slot map, evidence, patterns_decoded | `11_sequencer/seq_patterns_*`, `20_run_logs/seq_patterns_run_*` |
| 9 | seq_locks (v9) | Глобальные слоты и локи | `11_sequencer/seq_locks_*`, `20_run_logs/seq_locks_run_*` |
| 10 | seq_cells (v10) | Ячейки редактора паттернов | `11_sequencer/seq_cells_*`, `20_run_logs/seq_cells_run_*` |
| 11–30 | filter_phaser_pack, итерации 01–24 | Точечная добыча по машинам (см. ниже) | полки 01–10, 17–19 |
| — | all_tables (v4) | Полная выгрузка найденных таблиц по адресам | `15_tables_pmem` |
| — | hi08_extract | Таблицы, снятые с реального pmem (RUNTIME-ONLY разоблачены) | `15_tables_pmem/*_FROM_PMEM*` |
| — | zero_in_image | Реконструкция нулевых зон образа | `16_hypotheses` |
| — | final_recovery | Поиск пропущенных байтов (coldfire refs, runtime refs) | `09_coldfire_transport/_coldfire_*`, `15_tables_pmem/runtime_refs_analysis.json` |

### filter_phaser_pack: итерации 01–24

| # | Тема | Файлы теперь |
|---|---|---|
| 01 | Фильтр: ядро P:$0859–0B49, env+coeffcalc | `01_filter` |
| 02 | Фейзер: первый разбор m18 | `02_phaser` |
| 03 | Голос. страница, статус DIST | `10_controls_routing/voice_page_map.md`, `07_distortion/DIST_STATUS.md` |
| 04 | Свидетельства: скан ссылок на таблицы, коррекции | `15_tables_pmem` |
| 05 | Эмулятор DSP56300 | `17_tools_emulator` |
| 06 | AMP-энвелоп | `04_envelopes` |
| 07 | SVF-fit | `01_filter` |
| 08 | Вердикт по ENV | `04_envelopes` |
| 09 | JUCE-порт | `18_juce_port` (эталон: `../09_dsp_models_verified/09_juce_port/`) |
| 10 | Сверка с мануалом | `01_filter` |
| 11a | decoder_v2 | `17_tools_emulator` |
| 11b | Stage2-модель (FDN) | `01_filter` |
| 12 | LFO-формы | `03_lfo` |
| 13 | Stage2-tail | `01_filter` |
| 14 | m18: harness/IR/notches | `02_phaser` |
| 15 | DIST | `07_distortion` |
| 16 | m18-хвост | `02_phaser` |
| 17 | Эхо | `05_echo_delay` |
| 18 | m18-финал: бит-точная модель + порт | `02_phaser` (эталон: `../09_dsp_models_verified/`) |
| 19a | ColdFire-транспорт | `09_coldfire_transport` |
| 19b | Реверб m13 | `06_reverb` |
| 20a | ColdFire-клок | `09_coldfire_transport` |
| 20b | ColdFire-хост | `09_coldfire_transport` |
| 21 | Роутинг ядра эха | `05_echo_delay` |
| 22a | FM-машины | `08_fm` (эталон: `../09_dsp_models_verified/22_fm_machines/`) |
| 22b | Phaser LFO, sine/cos 8K | `02_phaser` (законы), `03_lfo` (таблицы) |
| 23 | DSND/DTIM роутинг | `10_controls_routing/README_dsnd_dtim.md` |
| 24 | DFB-ручки, карта «ручка→ячейка» | `10_controls_routing` |

Журнал и полный манифест пака: `20_run_logs/filter_phaser_pack_README.md`,
`20_run_logs/filter_phaser_pack_MANIFEST.json`.
