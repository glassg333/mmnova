# Аудит маршрутизации по Package-8 (`MnmVoiceFrame`) — 2026-09-29

## Почему этот документ появился

Марштизация проверена по указанному исходному пути, а не по прежнему
описанию, догадке или по названиям ручек:

<https://github.com/glassg333/mmnova/tree/main/decompiled%20data/juce/fm/%21fm%20fix%20patch/5%20fix%20pitch%2Benv%20fm%20full>

Проверен файл `dsp/mnm/MnmVoiceFrame.hpp` вместе с
`mnm_voice_frame_code.h`, `mnm_dsp_kernel.hpp`, таблицами и векторами.
Локальная сверка checkout источника дала commit
`353dc02ae2111aa4fed734da6320bf3c9de7938d`.

У upstream-пакета выполнен штатный регрессионный прогон:

```text
g++ -O2 -std=c++17 -I dsp/mnm test_voice_frame.cpp -o test_voice_frame
./test_voice_frame vectors/voice_frame_vectors.txt
checks=392824 mismatches=0 sets_with_mism=0
[100% VOICE-FRAME BIT-EXACT]
```

Это векторно подтверждённый **M8 / FM-STAT** полный кадр. Сам класс
`VoiceFrame` содержит интерпретатор 2636 DSP-инструкций и нативный hook только
для `MnmFmStat`. Он нужен как точный оракул для аудита и будущего прямого
порта, а не как вызываемый из audio callback интерпретатор: прогон 155 сетов
в данной среде занял примерно 410 секунд. Подстановка такого интерпретатора в
note-on или в посемпловый рендер создала бы ровно те CPU-пики, которые нужно
устранять.

## Проверка нового Package-9 block-routing upstream

После первоначального аудита в том же GitHub-пути появился commit
`8a666e72d2721341125108658e388a096a1dd107` (parent `353dc02…`). Он добавляет
`MnmBlockMixer.hpp`, `mnm_block_loop_code.h`, `routing_vectors.txt` (22 сета),
`test_block_routing.cpp`, `exp67_routing.py` и документацию block-routing.
Это ценный следующий уровень: три sub-block/track на DSP-блок, bus routing,
MIX, codec input и echo bus.

Однако поставленный upstream-тест сейчас не воспроизводится в опубликованном
состоянии: его первая инструкция `P:$0087` — `movep`, а приложенный
`mnm_dsp_kernel.hpp` не реализует mnemonic `movep`. Фактический запуск
`test_block_routing vectors/routing_vectors.txt` завершается на
`mnm kernel: unimplemented movep at 000087`. При этом `PROOF_ROUTING` заявляет
поддержку `+movep`, но сам kernel в commit не изменён относительно parent.
Поэтому Package-9 не импортирован в runtime и не объявлен в этой поставке
исполняемым/бит-точным портом. Сначала должен проходить его собственный
`360756/0` тест на исправленном, самосогласованном upstream-коде.

## Подтверждённая карта слов кадра

`MnmVoiceFrame.hpp` явно фиксирует `r6 = $400` в хвосте и различает следующие
группы:

| Ячейки DSP | Роль по Package-8 | Соответствие текущей P1-структуре |
|---|---|---|
| `$404`, `x$40B` | EQF, EQG | EFFX EQF/EQG (`p24`, `p25`) |
| `$405`, `$406` | VOL, PAN | AMP VOL/PAN (`p13`, `p14`) |
| `$408`, `$409`, `$40E`, `$413` | BASE, HPQ, WDTH, LPQ | FILT BASE/HPQ/WDTH/LPQ (`p16`, `p18`, `p17`, `p19`) |
| `$40C`, `$40D`, `$410`, `$411` | cascade-1 FILT ENV: ATK, DEC, BOFS, WOFS | FILT `p20..p23` |
| `$414..$417` | **самостоятельная stage-2 группа**: ATK, DEC, BOFS, WOFS | в обычных P1 `p0..p31` отдельной ручки нет |
| `$438..$43F` | delay/echo state cells, проверявшиеся в векторах | это не P1 EFFX `DSND/DFB/DBAS/DWID` |

Особенно важны реальные чтения из транслитерированного кода:

- `P:$0A22` читает `y:(r6+$14)` — это **`Y:$414`**, а не `$40C`;
- `P:$0AB7` читает `y:(r6+$14)` и `y:(r6+$15)` — это **`Y:$414/$415`**;
- README пакета отдельно называет `$414..$417` «stage-2
  ATK/DEC/BOFS/WOFS» и `$40C/$40D` — обычными FILT ENV ATK/DEC.

Следовательно, прежняя подмена `P1 FILT ATK/DEC` (`p20/p21`, `$40C/$40D`)
как выбора банка/глубины stage-2 была неверна. Совпадение слова «ATK» не
является доказательством одинаковой ячейки.

## Что исправлено в Fix-5 worktree

1. Удалён alias `p20/p21 -> stage-2 return-bank / return-depth` из
   `TrackDelayRouting.hpp`, `TrackDelay.inl`, `MnmDelay.hpp` и
   `MnmTrackDelayNew.hpp`.
2. Ручки P1 `FILT ATK/DEC/BOFS/WOFS` снова отвечают только за свою filter-env
   группу и не могут переставить L/R-возвраты, заглушить echo на значении 64
   или превратить delay в ping-pong.
3. DSND сохранён как ColdFire host-side mono-send: `raw / 128`, поэтому
   `0 -> 0`, `64 -> 1/2`, `127 -> 127/128`. LFO на DSND меняет лишь уровень
   посыла. Он не выбирает сторону, фазу, bank или повторный attack.
4. Возвраты host-delay остаются в собственных L/R-банках. Отдельный native
   stage-2 появится только вместе с прямым, векторно проверенным портом
   `$414..$417`; он не будет угадываться по P1 filter controls.
5. `DBAS/DWID` и их Q оставлены как явно помеченный **host candidate bridge**
   для OLD/MNM-model/NEW delay. Package-8 доказывает наличие сложного DSP-tail
   фильтра, но не доказывает, что его DSP читает P1 `DBAS/DWID`; поэтому код и
   документация больше не называют эту host-связку нативной маршрутизацией.
6. RMB-панель DSND переписана: она больше не сообщает ложные «RETURN SWAP:
   FILT ATK >= 64» и «FILT ATK = 64: RETURN DEPTH 0».

## Граница ответственности

- **Импортированный DSP-tail Package-8**: env2, cascade-1 filter, EQ, AMP,
  VOL²/PAN, resonance, delay, stage-2 и выход кадра. Это доказанный источник
  семантики ячеек.
- **Текущий host `TrackChain`**: совместимая practical-модель вокруг P1/P2,
  включая host DSND/DFB/DBAS/DWID. Она не вправе выдавать себя за полный
  побитовый `MnmVoiceFrame`.
- **TRY4**: самостоятельный импорт FM + kernel pitch + AMP/VOL²/PAN для
  STAT/PAR/DYN; он не использует retained или m6 FM-код. Его прежнее
  словосочетание «full-voice-frame» означало только собственный frame/page
  bridge и было слишком широким: полный Package-8 tail пока не запускается
  внутри TRY4.

## Безопасный следующий этап для полного native tail

Для полного direct-runtime порта необходимы одновременно:

1. скомпилированный прямой C++ tail, а не строковый DSP-интерпретатор;
2. отдельные state/tables для TRY4 без зависимости от retained/m6/Fix-5;
3. покрытие не только M8-векторами, но и проверка contract для M9/M10;
4. аудио-профилирование note-on и блока до включения режима пользователю;
5. отсутствие повторного применения generic EQ/FILT/ENV/VOLPAN/DELAY к уже
   обработанному native frame.

До этого этапа корректнее держать границу явной, чем включать в рендер
доказательный, но непригодный для real-time интерпретатор.
