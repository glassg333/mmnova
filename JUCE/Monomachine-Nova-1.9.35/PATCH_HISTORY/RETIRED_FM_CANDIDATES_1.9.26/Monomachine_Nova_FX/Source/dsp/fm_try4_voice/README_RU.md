# TRY4 — прямой импорт Package-5: FM + AMP frame contract

Источник: `decompiled data/juce/fm/!fm fix patch/5 fix pitch+env fm full`
Git-аудит: `353dc02ae2111aa4fed734da6320bf3c9de7938d`.

В эту папку перенесены собственные FM-ядра STAT/PAR/DYN, таблицы синуса, AMP/pitch/pan-таблицы и отдельное состояние. Единственные интеграционные изменения — имена заголовков и private namespace `fm_try4_voice` / `try4voicefm`; числовые данные и тела импортированы из Package-5.

`Try4VoicePageMap.hpp` использует слова машинной и AMP-страницы и отдельно
фиксирует независимые native stage-2 ячейки `$414..$417`. TRY4 **не**
подменяет их P1 `FILT ATK/DEC` и не вызывает полный Package-8
`MnmVoiceFrame` через строковый DSP-интерпретатор: такой вызов не годится для
real-time и не является честным полным voice-frame портом. Результат точного
аудита маршрутизации описан в верхнеуровневом
`ROUTING_AUDIT_PACKAGE8_2026-09-29.md`.

## Локальная C++17 ODR-правка

`Try4VoiceAmpEnv.hpp` подключается более чем одной единицей трансляции VST3.
Три определения storage `MnmPanTables::s_sin`, `s_cos`, `s_built` поэтому
помечены `inline`. Это устраняет MSVC `LNK2005/LNK1169` при линковке
`PluginProcessor.obj` и `PluginEditor.obj`, не меняя таблицы, их адресный
контракт или закон AMP/PAN.

## Файлы и исходные SHA-256
- `Try4VoiceKernel.hpp` ← `MnmKernel.hpp`: `4d14dd287996b8c11ae8844c59cc846610e680e48c791c35c9fba3b1e4acdf53`
- `Try4VoiceFmDsp.hpp` ← `MnmFmDsp.hpp`: `0ab1251a3053eb81f67da370cd7c318e035134a1fc41cd059c4937295f938df0`
- `Try4VoiceFmPar.hpp` ← `MnmFmPar.hpp`: `30183d4ab60f7b91337d9f0519f825468f8b454f87a065906ac1697c274c203b`
- `Try4VoiceFmDyn.hpp` ← `MnmFmDyn.hpp`: `39c2cecb37f5e19af8a588a7008f789791e56d5e130aad5a9c9d48d0bbe5b995`
- `Try4VoiceFmStat.hpp` ← `MnmFmStat.hpp`: `1b60ec7ec670e043a4e35e994cb3ab59ad531ac807862308a0541c009080d787`
- `Try4VoiceAmpEnv.hpp` ← `MnmAmpEnv.hpp`: `9fe9c950c7c0691cfaf33dbb372207d695420e40d03f163a931ac3484873d9a3`
- `Try4VoiceAmpEnvTables.h` ← `MnmAmpEnvTables.h`: `ecd9dccd49fe5a93ed6b1c5ce9f2339d72364d980a53f91f858529e185e93848`
- `Try4VoiceFmSineTable.h` ← `MnmFmSineTable.h`: `e43b0dabae626d8a6c1ac34d2dcaea997bea4df3fd352dbac7ca6f7a9bee4445`
- `Try4VoiceFm.hpp` ← `MnmFm.hpp`: `7c9cc7f1ac19fe5845ae366a94a299322673f2cf5aa6d9194eb433eeb3f9048c`
