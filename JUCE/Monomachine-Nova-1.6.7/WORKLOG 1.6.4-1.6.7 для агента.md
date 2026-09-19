# Monomachine Nova — журнал работ 1.6.4 → 1.6.7 (для следующего агента)

Дата: 2026-09-19. Рабочее дерево агента: `JUCE/Monomachine-Nova-1.6.4` (правка на месте).
Дерево пользователя: `E:\mm\Monomachine-Nova-1.6.7` == содержимое 1.6.6 + патч C4244 (см. раздел 4).

---

## 1. Что сделано в раунде 1.6.5 (DSP-режимы)

- Новые параметры `mode_synt / mode_amp / mode_filt / mode_dist / mode_dly` (+ route, chorus):
  `0 = mnm` (новый DSP, default), `1 = old` (прежний алгоритм). Старые состояния НЕ переключаются молча:
  схема состояния v10, при загрузке старого пресета режимы берутся из версии схемы.
- Ядро mnm: `Source/dsp/mnm/` — MnmKernel.hpp (AHDSR-огибающая как в фирмваре), MnmFm.hpp (FM-движок),
  MnmDelay.hpp, MnmChorus.hpp и др. ВАЖНО: `models/monomachine_fm_dynamic.hpp` — НЕ живой путь mnm,
  правки FM только в `Source/dsp/mnm/MnmFm.hpp`.
- Фирмварные референсы: AMP Y:$141800/$141880; фильтр P:$144AC7; FM ratio P:$141A80;
  delay X:$114000, таблица времён P:$144C49; FM+DYN feedback P:$1461AB–$1461BF.
- Тесты: `tests/MnmCoreTests.cpp` в обоих проектах (standalone, g++-совместимы).
- Скрипт проверки: `verify_dsp_mode_patch.py` (статические «иголки» по исходникам, ответ PASS).

## 2. Что сделано в раунде 1.6.6 (по списку пользователя)

1. **REFERENCE FIT (главная огибающая)**: ядро = фирмварный AHDSR когда AMP mode==mnm.
   Исправлено: sustain-аргумент принудительно 0.0f (у фирмварного AHDSR нет сустейна),
   добавлены holdPeak/resumeAfterHold — HOLD больше не обрывает звук, DECAY не «фоном тянет сустейн».
2. **Арп**: шаги переведены из вертикального списка фейдеров в ГОРИЗОНТАЛЬНЫЙ секвенсор
   (класс StepLane: верх — transpose ±24, низ — velocity 0..127, рисование всех шагов за раз),
   RndButton: ЛКМ = мгновенный рандом без входа в список, ПКМ = прежнее меню.
   Параметры: `arp_s{1..16}_{0..7}_{hold|transpose|velocity}` + `arp_step_page` (1-based),
   `arp_step_page_limit`, `arp_step_random`; lane bounds (20,318,1115,360).
3. **FM DYN — машина по умолчанию при открытии** (machine id 10; в specs() `if(m.id==10) defaultMachine=index;`).
4. **FM DYN feedback**: исправлена маппинг-формула —
   `(clamp(params[6],0,127)/127) * 4 * (params[2]/64)` — униполярно, 0 = нет обратной связи.
5. **DSP MODE — в заголовки страниц** (как REFERENCE FIT у огибающей), НЕ ручками среди кнобов:
   ComboBox-ы `syntModeCombo/filtModeCombo/effxDistCombo/effxDlyCombo` в шапках SYNT/FILT/EFFX,
   attachment-ы `syntModeLink/filtModeLink/effxDistLink/effxDlyLink`.
   SWAVE (machine id 4): комбо SYNT переключается на m4_3 «ALG A/ALG B», MODE-ячейка сетки гасится.
6. **Delay mnm, ручка DSND (send)**: в mnm-режим записана СТАРАЯ топология записи в линию:
   `left[write]=(pingPong?0.5f*(inL+inR):inL)*send+fbL*feedback;`
   `right[write]=(pingPong?0:inR)*send+fbR*feedback;`
7. **Ретюн delay обнаруживаем**: ПКМ по DTIM (pageIndex==2 && knob==3) открывает список
   скорости ретюна OFF/FAST/MED/SLOW → `dly_repitch` slew {0, 0.006, 0.030, 0.090} с.

## 3. Сборочные фиксы (после логов MSVC 14.40 / VS18)

- **FX/Source/NovaData.h**: комментарий `// 1.6.6: FM DYN default on open` был вставлен в середину
  строки и проглотил хвост `++index;}` → лямбда specs() не закрылась → каскад C2059/C2440/C2143,
  `nova::std::_Base128`, C3861 `_Is_constant_evaluated` (std-хедеры парсились внутри namespace nova).
  Исправлено: комментарий в конец строки.
- **Оба PluginEditor.cpp**: тот же баг в строке деклараций Surface — `// 1.6.6` проглотил
  `std::unique_ptr<...> ampModeLink; std::unique_ptr<EnvelopePage> envelopePage;` → C2065.
  Исправлено переносом на новую строку.
- ПРАВИЛО ДЛЯ АГЕНТА: **никогда не вставлять `//`-комментарий в середину строки с кодом**;
  после правок обязательно гонять балансировщик скобок (он в истории сессии) + verify + MnmCoreTests.
  g++-чисто ≠ MSVC-чисто, но незакрытые скобки/проглоченные хвосты ловятся обоими.

## 4. Патч C4244 (последняя правка, НЕ входит в дерево 1.6.7 на GitHub)

Файл: `mmnova-c4244.patch` (проверен `git apply --check` против реального дерева 1.6.7 с GitHub).
Содержание: явные `float()`-касты во всех вызовах JUCE с float-сигнатурой
(`drawLine/drawHorizontalLine/drawVerticalLine/drawEllipse`) в обоих PluginEditor.cpp —
13 замен на проект, поведение не меняется, все C4244 из лога исчезают.
`drawRect/fillRect` с int-аргументами не правили: JUCE выводит `Rectangle<int>`, конверсии нет.
Применение изнутри папки Monomachine-Nova-1.6.7: `git apply -p1 mmnova-c4244.patch`.

## 5. Постоянные ограничения (не нарушать)

- В workspace плагин НЕ компилировать; лимит ~128 МБ / 10 000 файлов.
- Кнопки/контролы не менять, если не просят. ПКМ НИКОГДА не крутит параметр — только альт-действия.
- bbox остаётся вариантом 1.5.1; не добавлять loop/кнопки без запроса.
- Новые алгоритмы — новыми записями в список, default `mnm`, старый сохранить как `old`;
  сохранённые состояния не переключать молча (схема v10).
- Одобренный chorus не трогать; альтернативы — только отдельными режимами.
- Не глушить варнинги компилятора; максимум DSP-правок за компиляцию.
- Все не-ASCII строковые литералы — эскейпами `\xHH` (скрипт escape_strings.py, идемпотентен);
  кракозябры в логе MSBuild — отдельная тема (кодировка консоли, лечится `VSLANG=1033` + `chcp 65001`).
- Деревья Synth и FX РАЗНЫЕ: никогда не копировать PluginEditor/NovaDSP/NovaData между ними;
  идентичны только заголовки dsp/mnm. Правки — питоном с assert на old-строку.
- `git diff` только как `git diff -- JUCE/Monomachine-Nova-1.6.4`.

## 6. Команды проверки (после любых правок)

```
python3 verify_dsp_mode_patch.py Monomachine_Nova_Synth/Source   # → PASS
python3 verify_dsp_mode_patch.py Monomachine_Nova_FX/Source      # → PASS
cd Monomachine_Nova_Synth && g++ -std=c++17 -I Source -O1 tests/MnmCoreTests.cpp -o /tmp/m && /tmp/m   # → all OK
cd Monomachine_Nova_FX    && g++ -std=c++17 -I Source -O1 tests/MnmCoreTests.cpp -o /tmp/m && /tmp/m   # → all OK
```

BUILD-маркер в UI: «BUILD 1.6.6». Машины: 8=FM STAT, 9=FM PAR, 10=FM DYN (default).
