# Снять кривые Q и DIST измерением, без реверса

## Почему это работает
Jick238/monomachine-lab — НЕ порт. Это dsp56kEmu, который исполняет
настоящий бинарь прошивки OS 1.32B. Его выход = выход железа.

В API уже есть всё нужное (core/monomachine_engine.h):

  bool     setParameter(Parameter p, uint8_t value);  // 0..127
  bool     renderBlock(Block& dst);                   // 16 кадров
  uint32_t debugXWord(uint32_t address);
  uint32_t debugYWord(uint32_t address);              // <-- ключ

debugYWord позволяет подсмотреть, что прошивка САМА положила
в память после пересчёта ручки. Дизассемблер не нужен.

## ШАГ 1 — найти адрес каждой ручки в Y:$500..$527

  uint32_t before[0x28];
  for (int a = 0; a < 0x28; ++a) before[a] = eng.debugYWord(0x500 + a);

  eng.setParameter(Parameter::FilterHighPassQ, 127);
  eng.renderBlock(blk);        // дать прошивке применить

  for (int a = 0; a < 0x28; ++a) {
      uint32_t now = eng.debugYWord(0x500 + a);
      if (now != before[a])
          printf("HPQ -> Y:$%03x  %06x => %06x\n", 0x500+a, before[a], now);
  }

Повторить для: FilterLowPassQ, FilterBaseOffset, FilterWidthOffset,
FilterAttack, FilterDecay, AmpDistortion.

Результат закрывает открытый вопрос №1 доки репо.

## ШАГ 2 — снять кривую целиком

  for (int v = 0; v <= 127; ++v) {
      eng.setParameter(Parameter::FilterHighPassQ, v);
      eng.renderBlock(blk);
      printf("%3d %06x\n", v, eng.debugYWord(ADDR_НАЙДЕННЫЙ_НА_ШАГЕ_1));
  }

На выходе готовая таблица 128 значений. Вставляется в порт как есть.

## ШАГ 3 — снять готовые f и q, которые читает петля SVF

Док §4.6: "коэффициенты читаются из Y:$91+ (рабочая область)".

  for (int a = 0x91; a < 0xB0; ++a)
      printf("Y:$%02x = %06x\n", a, eng.debugYWord(a));

Снять при разных BASE/WIDTH/HPQ/LPQ — станет видно, какой слот чей.
Это заменяет мою заглушку cutoffCoeff() реальными числами.

## ШАГ 4 — кривая DIST

Та же процедура для AmpDistortion. Иначе её пришлось бы доставать
из ColdFire (coldfire_main.bin, 425858 байт, MCF5206e, capstone M68K) —
это отдельная тяжёлая сессия. Измерением получается за минуты.

## АЛЬТЕРНАТИВА без сборки эмулятора

В core/monomachine_engine.cpp (44 КБ) есть готовая функция:

  static std::size_t parameterPageOffset(Parameter parameter) noexcept;

  grep -n -A60 "parameterPageOffset" core/monomachine_engine.cpp

Она сразу даёт смещение каждого параметра в странице.
Файл 44 КБ — мой загрузчик режет, локально прочитается целиком.

## ЧТО ПРИСЛАТЬ МНЕ
Любое из:
  1. вывод grep parameterPageOffset
  2. таблицу "ручка -> адрес" из шага 1
  3. дамп 128 значений из шага 2
Соберу точный C++ без заглушек.
