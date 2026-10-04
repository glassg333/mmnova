# Аудит импортов Fix-5 и TRY4 — 29.09.2026

Исходный репозиторий для аудита: `glassg333/mmnova`, commit
`353dc02ae2111aa4fed734da6320bf3c9de7938d`.

## Fix-5

Путь, заданный пользователем:

```text
decompiled data/juce/fm/!fm fix patch/5 fix pitch+env fm full
```

Импорт расположен только в `Source/dsp/fm_fix5/`. Он имеет отдельный namespace,
таблицы, AMP-огибающую, gain-ring, pitch-цепочку и состояние. ID 8 добавлен
отдельно и не заменяет Fix 4, OLD FIX или сохранённые MNM/OLD/NEW-пути. Host
переводит BPM в `TickRecip=floor(0x800000/(24*BPM))` для нативного HOLD.

## TRY4

TRY4 больше не использует прежний изолированный клон `dsp/fm_try4/`. Активный
путь находится только в `Source/dsp/fm_try4_voice/` и происходит из Package-5:
у него собственные определения STAT/PAR/DYN, ROM, AMP/pitch/pan, очередь
16-семпловых кадров, lifecycle и `VoicePageMap`. Это самостоятельный FM/AMP
frame runtime, а не заявление, что Package-8 полный DSP-tail уже исполняется
в TRY4. Runtime не включает m6, Fix-5 или сохранённые FM-ядра через этот путь.

## Граница посыла задержки

DSND является host-side моно-посылом. DSP-ядро не интерпретирует знак DSND как
stereo-side, фазу или ping-pong. Повторная сверка полного Package-8
`MnmVoiceFrame` показала также, что P1 FILT ENV `$40C..$411` не является
native stage-2 `$414..$417`: host-delay больше не подменяет `FILT ATK/DEC`
выбором банка/глубины возврата. Это закреплено `TrackDelayRouting.hpp`,
`DelayFeedbackDspTests` и `ROUTING_AUDIT_PACKAGE8_2026-09-29.md`.

## Честные ограничения

Выполненные автономные регрессии и синтаксический аудит не равны сборке или
прослушиванию готового VST3. Для выпуска остаются обязательными сборка
Windows/VS2022, CTest и проверка в DAW на нескольких sample rate.
