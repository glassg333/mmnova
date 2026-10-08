# 03_RESEARCH_LINKS.md — Карта репозиториев, статей и лицензий (v0.1)

> Верификация: GitHub REST API + raw README (2026-10-08). Пометки: ★ = MUST-SEE, 💀 = заброшен (push > 3–5 лет), ⚖️ = лицензия влияет на использование. Группы: [A] физика ядра · [B] эксайтеры и DSP · [C] фреймворки и open-синты · [D] UI/рисование/жесты · [E] исторические. Приоритет пользователя: физика ядра + UI.

---

## Блок 1 — Ссылки пользователя (7, проверены)

| # | Ссылка | Вердикт |
|---|---|---|
| 1 | ★⚖️💀 [mi-creative/miPhysics_Processing](https://github.com/mi-creative/miPhysics_Processing) | Java/Processing, **GPL-3.0**, 52★, push 2020. Лучшая открытая таксономия mass-interaction модулей (Mass3D/SpringDamper3D/Driver3D, динамическая топология, аудио в callback). Код GPL — **только концепты**. |
| 2 | ★⚖️ [SilvinWillemsen/ModularVST](https://github.com/SilvinWillemsen/ModularVST) | C++/JUCE, **MIT**, 10★. Ближайший прототип нашей задачи: модульный физмод-VST (статья SMC 2021). Исходники на ветке `SMCconf`; код можно брать с атрибуцией. |
| 3 | [pooya-shams/massspring](https://github.com/pooya-shams/massspring) | Python/pygame, **MIT**, 19★, 💀. 3D-симулятор с классами сил (Hooke, упругие столкновения, drag) — образец «сила как объект»; референс-качество. |
| 4 | [davrempe/2d-mass-spring-sim](https://github.com/davrempe/2d-mass-spring-sim) | C/GLUT, **лицензии нет**, 4★, 💀. Ценный UX-референс: LMB-долгое нажатие = спавн массы (дольше = тяжелее), RMB = якорь, drag между массами = пружина; явный Эйлер, нестабилен — антипример численно. |
| 5 | [arasgungore/mass-spring-damper-system](https://github.com/arasgungore/mass-spring-damper-system) | MATLAB/Simulink, **MIT**, 16★. Учебные 1–2 масс MSD; ценность только как sanity-check уравнений. |
| 6 | ★⚖️💀 [lucasw/tao_synth](https://github.com/lucasw/tao_synth) | C++, **GPL-2.0** (форк MindBuffer/tao, upstream taopm), 11★. Классика: материалы из масс-пружин + устройства **Bow/Hammer/Connector/Output**, live 3D-визуализация волн. Концепты bow/connector — оттуда; код GPL — не копируем. |
| 7 | [HAL: VCC15 Conf NIME (GENESIS)](https://hal.science/hal-01262144v1/file/VCC15_Conf_NIME.pdf) | Villeneuve, Cadoz, Castagné — *Visual Representation in GENESIS…* (NIME 2015). PDF **защищён антиботом Anubis** — скачать вручную в браузере (для робота 200+HTML). Статья про визуальные представления масс-интеракционных моделей — прямой источник нашей «рисуемой» концепции. |

**Бонус из тех же авторов (важнее самих ссылок 3–5):** ★ [mi-creative/mi-gen](https://github.com/mi-creative/mi-gen) (Max/gen~ тулбокс, 149★, активен, GPL-3 — таксономия + туториалы bow/mesh) · ★ [mi-creative/MIMS](https://github.com/mi-creative/MIMS) (PyQt визуальный редактор модели → компиляция в Faust/gen~ — прямой прецедент «рисуешь → генерится DSP», GPL-3) · [mi-creative/FaustPM_2021_examples](https://github.com/mi-creative/FaustPM_2021_examples) · SilvinWillemsen: [BowedStringJUCE](https://github.com/SilvinWillemsen/BowedStringJUCE) (минимальный JUCE-смычок), [FastBowedString](https://github.com/SilvinWillemsen/FastBowedString) (**MIT**, модальный быстрый смычок), [NonlinearMassSpring_DAFx23](https://github.com/SilvinWillemsen/NonlinearMassSpring_DAFx23) (статья DAFx-23 «Nonlinear Strings Based on Masses and Springs» — буквально наша физика), [SimpleStringApp](https://github.com/SilvinWillemsen/SimpleStringApp), [RealTimeFDTD](https://github.com/SilvinWillemsen/RealTimeFDTD), [Dynamic_Grid_JAES](https://github.com/SilvinWillemsen/Dynamic_Grid_JAES).

---

## Блок 2 — Найденные недостающие репозитории

### A) Физика ядра (приоритет ★)

| Репо | Язык | Лицензия | Роль в проекте |
|---|---|---|---|
| ★ [mi-creative/mi-gen](https://github.com/mi-creative/mi-gen) | genexpr/Max | GPL-3.0 | Эталонный цикл интегрирования и таксономия модулей (массы, пружины, демпферы, условные контакты, **bow-модули**) |
| ★ [mi-creative/MIMS](https://github.com/mi-creative/MIMS) | Python | GPL-3.0 | Пайплайн «описание модели → сгенерированный DSP» — каркас нашего «рисуешь → звучит» |
| [rmichon/mi_faust](https://github.com/rmichon/mi_faust) | Faust | нет | Mass-interaction библиотека `mi.lib` — по-модульная математика связей в готовом виде |
| [khiner/mesh2audio](https://github.com/khiner/mesh2audio) | C++ | GPL-3.0 | «Меши → играбельный физмод» (FEM/моды, Eigen/Spectra) — ближайший OSS-аналог «нарисованная фигура = резонатор» |
| [Chowdhury-DSP/chowdsp_wdf](https://github.com/Chowdhury-DSP/chowdsp_wdf) | C++ | **BSD-3** | Wave Digital Filters — пассивное согласование эксайтеров с сетью (качество «нобелевского» уровня) |
| [aliallaoui/miPhysics](https://github.com/aliallaoui/miPhysics) | C++ | GPL-2.0 | Ранняя standalone версия ядра miPhysics |

### B) Эксайтеры и DSP (bow, waveguide, KS, FM, коллизии)

| Репо | Язык | Лицензия | Роль |
|---|---|---|---|
| ★ [wasilakis/the_bowed_string](https://github.com/wasilakis/the_bowed_string) | MATLAB | GPL-3.0 | Bow с **гарантированной пассивностью** (elasto-plastic, Front. Signal Process. 2025) — схема для портирования |
| ★ [mvanwalstijn/String-Collisions](https://github.com/mvanwalstijn/String-Collisions) | MATLAB | нет | Энергостабильные схемы столкновений: hammer-string, string-barrier, sitar-bridge (JSV 2023) — математика наших пересечений |
| [Nemus-Project/EfficientBowedString](https://github.com/Nemus-Project/EfficientBowedString) | C++/MATLAB | нет | Дешёвый модальный bow + `RealTimeImpl` JUCE-реализация |
| [SilvinWillemsen/FastBowedString](https://github.com/SilvinWillemsen/FastBowedString) | C++ | **MIT** | Копируемый real-time bow (модальная форма) |
| [Synthesis/FTMSynth](https://github.com/Synthesis/FTMSynth) | C++ | нет | Functional Transformation Method JUCE-плагин с визуализацией мод |
| [hatchjaw/physical-education](https://github.com/hatchjaw/physical-education) | C++ | GPL-3.0 | FDTD-модели в JUCE-плагине — учебный пример интеграции |
| [grame-cncm/faustlibraries](https://github.com/grame-cncm/faustlibraries) | Faust | — | `pm.lib`: bow, waveguide, mesh, modal — исполнимая спецификация примитивов; Faust→C++ для прототипов эксайтеров |
| [olilarkin/Tambura](https://github.com/olilarkin/Tambura) | Faust | нет | Симпатическое резонирование струн + приёмы excite-параметров |
| [mrletourneau/strong_kar](https://github.com/mrletourneau/strong_kar) | C++ | **MIT** | KS как модуль (VCV) — паттерн модульного эксайтера |
| [SMCFY/DrumMachine](https://github.com/SMCFY/DrumMachine) + [chadmckell/DWGMesh](https://github.com/chadmckell/DWGMesh) | Max/MATLAB | нет | 2D banded waveguide mesh — альтернатива сеткам для мембран |
| [dhilowitz/GayageumSynth](https://github.com/dhilowitz/GayageumSynth) | C++ | **MIT** | Аккуратный маленький JUCE-волноводный инструмент |
| [mrahtz/javascript-karplus-strong](https://github.com/mrahtz/javascript-karplus-strong) | JS | — | Наглядный KS-референс для доки/тестов |

### C) Фреймворки и open-source синты (архитектурные референсы)

| Репо | Лицензия | Роль |
|---|---|---|
| [juce-framework/JUCE](https://github.com/juce-framework/JUCE) | AGPL-3.0 / коммерч. | Наш стек |
| ★ [gabrielsoule/resonarium](https://github.com/gabrielsoule/resonarium) | GPL-3.0 | Полный open JUCE-физмод-синт (MPE, voice-overflow защита, kill-switch пресетов) — учим архитектуру |
| ★ [michele-perrone/OpenPiano](https://github.com/michele-perrone/OpenPiano) | AGPL-3.0 | FD-струны+молоточки, многопоток — честная карта производительности |
| ★ [tiagolr/ripplerx](https://github.com/tiagolr/ripplerx) | GPL-3.0 | Модальный резонатор (класс Chromaphone): материал-параметры, инхармоничность |
| [odoare/Mechanodd](https://github.com/odoare/Mechanodd) | нет | Полифонический физмод: excite слоты + feedback-матрица |
| [sudara/pamplejuce](https://github.com/sudara/pamplejuce) | **MIT** | Каркас репозитория: CMake, Catch2, pluginval, CI |
| [iPlug2/iPlug2](https://github.com/iPlug2/iPlug2) / [DISTRHO/DPF](https://github.com/DISTRHO/DPF) | перм./ISC | Альтернативные фреймворки (для сравнения, не для миграции) |
| [Chowdhury-DSP/chowdsp_utils](https://github.com/Chowdhury-DSP/chowdsp_utils) | **BSD-3** | Проверенные JUCE/DSP утилиты, lock-free |
| [jatinchowdhury18/AnalogTapeModel](https://github.com/jatinchowdhury18/AnalogTapeModel) | GPL-3.0 | Золотой стандарт оформления физмод-DSP в плагине (тесты, графики, docs) |
| [Surge-Synthesizer/surge](https://github.com/surge-synthesizer/surge) (Surge XT) | GPL-3.0 | Модуляционная архитектура крупного open-синта |

### D) UI: node-редакторы, рисование, жесты (приоритет ★)

| Репо | Лицензия | Роль |
|---|---|---|
| ★ [thedmd/imgui-node-editor](https://github.com/thedmd/imgui-node-editor) | **MIT** | Самый богатый node-UX (пины, группы, зум) — переносим паттерны в JUCE Graphics |
| [Nelarius/imnodes](https://github.com/Nelarius/imnodes) | **MIT** | Минималистичная альтернатива |
| [Fattorino/ImNodeFlow](https://github.com/Fattorino/ImNodeFlow) | **MIT** | Типизированные сокеты (параметры vs сигналы vs триггеры) |
| ★ [BespokeSynth/BespokeSynth](https://github.com/bespokesynth/bespokesynth) | GPL-3.0 | Node-модуляр на максималках: объекты-канвасы, макросы, автоматизация — UX-референс |
| [mtytel/vital](https://github.com/mtytel/vital) | GPL-3.0 | **Drawable LFO/огибающих** — эталон «рисуй модулятор мышью» |
| [ffAudio/foleys_gui_magic](https://github.com/ffAudio/foleys_gui_magic) | спец. | Быстрый скиннинг сложного JUCE-UI, drag&drop-редактор |
| [TobiasKozel/GuitarD](https://github.com/TobiasKozel/GuitarD) | **MIT** | Node-граф внутри плагина (iPlug2+Skia+Faust) |
| [bcaramiaux/ofxGVF](https://github.com/bcaramiaux/ofxGVF) | LGPL-3.0 | Gesture Variation Follower: запись жестов → темп-следящий реплей с вариациями (наш gesture-движок v2; линковать отдельно) |
| [greguslab/KnobLooper](https://github.com/greguslab/KnobLooper) | **MIT** | Простая UX-схема «лупы ручек» (16 каналов, JSON-пресеты) |
| [anukarimusic/anukari-preset-api](https://github.com/anukarimusic/anukari-preset-api) | **MIT** | Официальный API пресетов Anukari — схема параметров коммерческого эталона (сам Anukari закрыт) |
| [Seneral/Node_Editor_Framework](https://github.com/Seneral/Node_Editor_Framework) | MIT | Доп. UX-паттерны (reroute, группы) — Unity, но идеи переносимы |

### E) Исторические / прочие

[thestk/stk](https://github.com/thestk/stk) (STK: BowTabl, BandedWG, Mesh2D, Modal — канонические референс-классы, перм. лицензия) · [topher6345/ScanPvoc](https://github.com/topher6345/ScanPvoc) (scanned synthesis = «нарисованная форма как колеблющаяся цепочка») · [radarsat1/stk-piano](https://github.com/radarsat1/stk-piano) (наследие SynthBuilder). **Не на GitHub:** CORDIS-ANIMA, GENESIS (ACROE), Modalys (IRCAM) — только статьи/доки.

---

## Блок 3 — Библиография

### В архиве (`papers/`, скачаны и проверены)

| Файл | Что это |
|---|---|
| `2009_Bilbao_numerical-sound-synthesis_ch1.pdf` | Bilbao, *Numerical Sound Synthesis* (Wiley) — глава 1, фрейминг всего FDS/физмод подхода |
| `2009_Bilbao_numerical-sound-synthesis_contents.pdf` | Оглавление книги — карта глав для ручной докачки |
| `2021_jasa_collision-schemes-musical-acoustics.pdf` | **Ducceschi, Bilbao, Willemsen, Serafin** — «Linearly-implicit schemes for collisions in musical acoustics based on energy quadratisation» (JASA) — наша схема коллизий v2 |
| `2024_differentiable-modal-synthesis-strings.pdf` | Jin Woo Lee — «Differentiable Modal Synthesis for Planar String Sound and Motion» (arXiv:2407.05516) — современные модальные методы струн |

### Скачать вручную (HAL закрыт антиботом Anubis для роботов; в браузере открывается нормально)

| Статья | URL |
|---|---|
| **GENESIS / визуальные представления** (ссылка пользователя) | `hal.science/hal-01262144v1/file/VCC15_Conf_NIME.pdf` |
| Leonard и др. — *Formalizing Mass-Interaction Physical Modeling in Faust* (2023) | `hal.science/hal-04869101/document` |
| Leonard & Villeneuve — *mi-gen~: Mass-Interaction Sound Synthesis Toolbox* (DAFx-19) | `hal.science/hal-02270792/document` |
| Kontogeorgakopoulos & Cadoz — *Cordis Anima… System Analysis* (2007) | `hal.science/hal-00439313/document` |
| Taché & Cadoz — *CORDIS-ANIMA Instrumentarium* (2009) | `hal.science/hal-00436294/document` |
| Castagné & Cadoz — *GENESIS: Friendly Musician-Oriented Environment* (2002) | `hal.science/hal-00481717/document` |
| Vigué и др. — *Bowed string toy model, periodic solutions* | `hal.science/hal-03446625/document` |
| Falaize & Roze — *Passive-guaranteed nonlinear interaction…* (2025, коллизии) | `hal.science/hal-04727388/document` |

### Классика (платно / по DOI)

Cadoz, Luciani, Florens — *CORDIS-ANIMA: A Modeling and Simulation System…* (CMJ 17(1), 1993, `doi:10.1162/comj.1993.17.1.19`) · McIntyre, Schumacher, Woodhouse — *On the oscillations of musical instruments* (JASA 1983, `doi:10.1121/1.389367`) · Adrien — *The Missing Link: Modal Synthesis* (CMJ 1991, `doi:10.1162/comj.1991.15.2.41`) · Bilbao — *Numerical Sound Synthesis* (Wiley 2009, `doi:10.1002/9780470510467`).

### Открытые онлайн-источники (не PDF, но обязательные)

Julius O. Smith — *Physical Audio Signal Processing* (бесплатная онлайн-книга): `ccrma.stanford.edu/~jos/pasp/` · NESS project (EDN): `ness.music.ed.ac.uk` · Anukari dev-blog «Anukari on the CPU»: `anukari.com` · SMC 2019 Villeneuve — *Mass-Interaction Physical Models for Sound and Image Synthesis*: `smc2019.uma.es` · DAFx-23 Willemsen — *Nonlinear Strings Based on Masses and Springs*: `dafx.de` (архив proceedings).

---

## Блок 4 — Матрица лицензий (что можно копировать)

| Режим | Репо | Правило |
|---|---|---|
| ✅ Копируем код (с атрибуцией) | ModularVST, FastBowedString, GayageumSynth, strong_kar, pamplejuce, chowdsp_wdf/utils, imgui-node-editor, imnodes, ImNodeFlow, GuitarD, KnobLooper, anukari-preset-api, pooya-shams/massspring, arasgungore (MIT/ BSD) | Вендорим в `third_party/`, фиксируем копирайты |
| ⚖️ Только концепты/UX | miPhysics_Processing, mi-gen, MIMS, tao, mesh2audio, the_bowed_string, physical-education, resonarium, OpenPiano, ripplerx, Surge, BespokeSynth, Vital, Rack, AnalogTapeModel, ofxGVF (LGPL — линковать отдельно), STK (перм., но лучше концепты) | Читаем, конспектируем идеи, пишем своё; если {{HOLE-05}} = GPL — статус меняется на «можно код» |
| ❌ Идеи без кода (нет лицензии) | davrempe/2d-mass-spring-sim, String-Collisions, mi_faust, FTMSynth, EfficientBowedString, Tambura, Mechanodd | Только описание подхода; «clean-room» при необходимости |
