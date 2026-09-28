# Текущие видимые меню MODE L / MODE H — 1.9.17 source worktree

Источник: `JUCE/Monomachine-Nova-1.9.17-work/.../Source/NovaData.h` и response-карты `HybridDSP.hpp`.

MODE L показывает HP/low-cut, MODE H — LP/high-cut. Все BP находятся в одной папке. Hyperion намеренно стоит последним внутри каждой response-папки. Шесть LP-derived HP с прямым входом вынесены в отдельную последнюю папку **DRY** только в MODE L.

## MODE L

`NATIVE`

### LOW CUT / HP
- K35 HP
- MOOG HP24
- MOOG HP12
- R 303 HP
- R MS20 HP
- R MOOG HP24
- R MOOG HP12
- R ANALOG HP24
- R ANALOG HP12
- R LINEAR HP24
- R LINEAR HP12
- R RBJ HP
- R TPT HP
- R HYPER HP4
- R HYPER HP2

### NOTCH
- R HYPER NOTCH

### BAND PASS
- MOOG BP24
- MOOG BP12
- R 303 BP
- R MS20 BP
- R MOOG BP24
- R MOOG BP12
- R ANALOG BP24
- R ANALOG BP12
- R LINEAR BP24
- R LINEAR BP12
- R RBJ BP
- R TPT BP
- R HYPER BP4
- R HYPER BP2

### DRY — последняя папка (direct-input HP tests)
Первые пять используют `dry - LP`; `R DVAL HP4` — `dry + LP`.
- R HUV HP4
- R KRAJ HP4
- R MICRO HP4
- R MUSIC HP4
- R OBERHEIM HP4
- R DVAL HP4

## MODE H

`NATIVE`

### HIGH CUT / LP
- K35 LP
- MOOG LP24
- MOOG LP12
- R 303 LP
- R MS20 LP
- R MOOG LP24
- R MOOG LP12
- R ANALOG LP24
- R ANALOG LP12
- R LINEAR LP24
- R LINEAR LP12
- R RBJ LP
- R TPT LP
- R HUV LP4
- R KRAJ LP4
- R MICRO LP4
- R MUSIC LP4
- R OBERHEIM LP4
- R DVAL LP4
- R HYPER LP4
- R HYPER LP2

### NOTCH
- R HYPER NOTCH

### BAND PASS — последняя папка
- MOOG BP24
- MOOG BP12
- R 303 BP
- R MS20 BP
- R MOOG BP24
- R MOOG BP12
- R ANALOG BP24
- R ANALOG BP12
- R LINEAR BP24
- R LINEAR BP12
- R RBJ BP
- R TPT BP
- R HYPER BP4
- R HYPER BP2

Противоположные response-типы не показываются. DRY намеренно существует только в MODE L, поэтому `R OBERHEIM HP4` не дублируется в MODE H.
