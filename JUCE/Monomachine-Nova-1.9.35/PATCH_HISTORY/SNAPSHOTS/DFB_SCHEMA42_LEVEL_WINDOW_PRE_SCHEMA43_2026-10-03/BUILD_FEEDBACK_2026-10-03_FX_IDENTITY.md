# Внешняя сборка Windows — исправление FX project identity

Дата сообщения: 03.10.2026.

## Полученный результат

Пользовательская фоновая VST3-сборка **до снятия DLY CORE** показала:

- `Monomachine_Nova_FX`: `C1189` в `Source/ProjectIdentity.h(8)` —
  `JucePlugin_IsSynth differs from NovaConfig.h`;
- `Monomachine_Nova_Synth`: VST3 собран и заменён в указанной пользователем
  тестовой папке.

Сообщение FX означает именно несовпадение project identity, а не ошибку
ManifestHelper: Projucer генерирует для FX `JucePlugin_IsSynth=0`, тогда как
в доставленном FX `NovaConfig.h` ошибочно стояло `NOVA_SYNTH=1`.

## Внесённое исправление

В active FX source восстановлено:

```cpp
#define NOVA_SYNTH 0
```

Также в оба current static audit добавлена обязательная target-specific проверка:

| Цель | Обязательное значение | Build marker |
|---|---:|---|
| Synth | `NOVA_SYNTH=1` | `Monomachine Nova Synth source BUILD` |
| FX | `NOVA_SYNTH=0` | `Monomachine Nova FX source BUILD` |

Это предотвращает ошибочное byte-copy `NovaConfig.h` между variant-целями.

## Граница проверки

Локально не запускались Projucer, MSBuild, CTest, VST3, render или DAW. После
этого source исправления потребуется повторить Windows FX build из обновлённого
архива; предыдущий лог не подтверждает сборку текущей schema-42 поставки.
