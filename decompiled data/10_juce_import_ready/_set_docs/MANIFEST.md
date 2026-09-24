# MANIFEST — вердикт по каждому элементу исходной папки

Легенда: ✅ верифицировано · 🟡 правдоподобно · 🟠 устарело/ошибки · ❌ фейк/мусор

## Корень папки
| элемент | вердикт | комментарий |
|---|---|---|
| `synth → dist → srr → filt eq → env → dsndEQ....txt` | 🟡 | заметка по роутингу (слух), полезный контекст → 02_PROBABLE |
| `как достать q фильтра.txt` | 🟡 | полезные советы по headless/main.cpp (snapshots, дизасм) → 02_PROBABLE |
| `gpt 6 astra cheker na oriig.html` | 🟡 | GPT-страница Reference Lab, не DSP → 05_LAB_PAGES |

## 01_CORE_VERIFIED (новая сборка)
| элемент | вердикт |
|---|---|
| `dsp/MnmFixed.h` | ✅ Q1.23/56-бит MAC/sat24; исправлен знак в fToQ23 (старый терял минус!) |
| `dsp/MnmKernelTables.h` | ✅ 17 таблиц дословно, якоря 2026-09-18 |
| `dsp/MnMAmpDist.h` | ✅ DIST-закон = листинг+эмулятор; env AHDR |
| `dsp/MnMFilter24.h` | ✅ таблица среза+кламп $6A3+сглаживание 1/256+Q-банки; разводка Y:$91 — OPEN (помечено) |
| `dsp/MnMVoiceChain.h` | ✅ новое: сквозная цепочка |
| `dsp/MnmRouting.h` | ✅ буферы X:$160/1C0/220/280, флаги X:$2C4, fxMix, INP |
| `dsp/MnmParams.h` | ✅ 23 машины × 8 ручек из дескрипторов @0x57FC5 |
| `dsp/machines/ChorusCore.*` | ✅ побитовый порт FX-CHORUS |
| `dsp/machines/MnmBBox.h` | 🟡 каркас+банк ROM; FIR/RTIM приближены (сам README честен) |
| `data/kernel/*` | ✅ выгрузки таблиц (сверены байт-в-байт) |
| `data/bbox_drum_bank.bin` | ✅ 62356/62356 @P:$103E7B |
| `data/pitch_wavetable_2048.bin` | ✅ wt[i]=0.5·2^(i/2048) |
| `rig/*` | ✅ исполняет настоящее ядро OS 1.32B |
| `docs/*` | ✅ ROUTING_RU, BBOX_RU, KERNEL_CHAIN_RU (+ERRATA), VERIFICATION_REPORT_RU |

## 02_PROBABLE
| элемент | вердикт |
|---|---|
| `mnm-routing-100/` (chain README + MnMVoiceChain_Canonical.h) | 🟡 порядок dist→srr→filt→eq→env→dsnd: слух+CHANGELOG; порядок eq/srr относительно filt спорен. Приоритет — код ядра (01_CORE) |
| `mnm-filter-q/FILTER_Q_FINDINGS.md` | 🟡 анализ Q верный по сути; константы $F528BD/$4A4DF0 в 7-м знаке уточнены в 01_CORE |
| `mnm-filter-q/PLAN_MEASURE.md`, `VOICE_PAGE_MAP.md` | 🟡 методика измерений — годная |
| `mnm-filter-q/Source/mmnova/*.h` | 🟡 код до вскрытия делителя среза — уступает 01_CORE |
| `4-mmnova-honest-fav/` | 🟡 Tier-A выгрузки (MnMChorus 382w, MnMGndSin 159w + литерал 2-полюсника P:$144D21 $1E454E/$5B75B8) — полезно; порт-план честный |
| оба корневых txt | 🟡 → сюда |

## 03_ARCHIVE_SUPERSEDED (история, в плагин НЕ брать)
| элемент | вердикт | главные ошибки |
|---|---|---|
| `1-try/` | 🟠 | REVERB с константами Freeverb (в прошивке P:$1453A8: $0A67/$098F/$0713/$0655); SVF tier-B |
| `2-try/` | 🟠 | неверные float-константы $F528BD (−0.08475685); выдуманные fc/q-формулы; «decay bend до 150%» — правка из промптов плагина, в прошивке нет |
| `3-mnmdsp_reference-more-version/` | 🟠 | подмножество более новой версии (→ 01_CORE); дубль |
| `5-mnm-dist-filter/` | 🟠 | DIST сводился к «SR.SM-клипу», скейл 0..0.5 — мёртвая зона; факты вынесены в 6 VERIFIED |
| `mmnova-filter-official-from-manual/` | 🟠 | «8 шагов = 1 октава», «BASE=0 → note/4» — НЕ из прошивки; срез в железе табличный (верх ≈6.6 кГц) |

## 04_QUARANTINE
| элемент | вердикт |
|---|---|
| `bbox_wav_FAKE/` + `_duplicate/` | ❌ файлы по 129 байт, начинаются с "vers http://…" — HTML-обрывки, не аудио; md5 одинаковый у всех |

## 05_LAB_PAGES
| элемент | вердикт |
|---|---|
| `routing-verified.html`, `filter-q*.html`, `try2-preview.html`, `gpt-reference-lab.html` | 🟡 просмотрщики, на DSP не влияют |

## Что удалено из набора полностью (не попало никуда)
- `6 VERIFIED 2026-09-18/` как папка: отчёт перенесён в `01_CORE_VERIFIED/docs/`,
  `MnMDistReal.h` заменён исправленным `MnMAmpDist.h` (у старого был UB-выход
  за kDriveCurve129[129] при idx>128 и мёртвый код `hi`), таблицы расширены
  в `MnmKernelTables.h`.
- дубль `bbox_wav` из «3 mnmdsp_reference more version» — фейк.
