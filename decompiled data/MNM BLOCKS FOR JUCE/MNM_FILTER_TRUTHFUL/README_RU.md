подсказка по нахождению и пруфу фильтра но не сам фильтр

# MnmFilterTruthfulCore

Правдивый фильтр без выдуманного UI-маппинга.

## Что делает

`include/MnmFilterTruthful.h` — JUCE-independent C++ header. Он:

1. использует полные raw-таблицы cutoff из OS 1.32B;
2. обновляет коэффициент один раз на блок из 16 сэмплов;
3. принимает `cutoffIndex` и `resonance` уже в том виде, в каком их подготовило
   оригинальное kernel, а не притворяется, что знает кривую ручки 0..127;
4. выполняет детерминированный Q1.23 двухинтеграторный SVF.

## Важное ограничение

Это **не заявленный бит-в-бит порт всей страницы FILT**. В исходных обрывках
доказаны таблицы, адреса, блоковый темп и наличие двух циклов, но не закрыты
ColdFire-кривые HPQ/LPQ, полная раскладка параметров и точная последовательность
всех DSP MAC. Поэтому рекурсия в заголовке помечена `MODEL`, а класс называется
`TruthfulCore`, не `ExactFilter`.

Пример:

```cpp
#include "MnmFilterTruthful.h"
using namespace mmnova_truthful;

MnmFilterTruthfulCore filter;
filter.setRawFrame({Table::Primary, 900, /*raw k*/ 0x200000, Output::LowPass});
filter.processBlock16(samples16);
```

`900` и `0x200000` в примере — не утверждение о значении ручки; реальные raw
значения нужно получить из ядра/ColdFire или измерением.

Проверка таблиц без компилятора:

```sh
python3 tests/selfcheck.py
```

В архив не включены компиляторы, JUCE, эмуляторы и toolchain.
