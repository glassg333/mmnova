# Три пути к точным кривым HPQ / LPQ / DIST

## ПУТЬ 1 — перечитать дескрипторы  (дешевле всего, начинать с него)

Формат известен. Нужно просто разобрать 176 байт правильно
и посмотреть на 2 хвостовых байта каждого параметра.

python - <<'EOF'
d = open("decompiled data/05_descriptors/coldfire_main.bin","rb").read()
base, stride = 0x57FC5, 0xB0
for i in range(26):
    b = d[base+i*stride : base+(i+1)*stride]
    if len(b) < stride: break
    print(f"--- блок {i}  @0x{base+i*stride:05x}")
    print("  short   :", b[0:9])
    print("  display :", b[9:0x12])
    for p in range(8):
        q = b[0x12+p*6 : 0x12+(p+1)*6]
        print(f"  p{p}: имя={q[0:4]}  хвост={q[4]:02x} {q[5]:02x}")
    print("  defaults:", " ".join(f"{x:02x}" for x in b[0x42:0x4A]))
    print("  raw_tail:", " ".join(f"{x:02x}" for x in b[0x4A:0xB0]))
EOF

Что проверять:
  у SID параметры PWRS/WAVE/MOD/MSRC должны иметь тип 02/05/04/02
  (это уже видел автор первого порта). Если сойдётся — формат разгадан.

Потом искать такой же блок для страниц AMP/FILT/EFFX/LFO.
Он должен быть рядом с 0x57FC5. Признак: строки
"BASE" "WDTH" "HPQ" "LPQ" "ATK" "DEC" "BOFS" "WOFS" подряд.

  grep -abo "HPQ" coldfire_main.bin
  grep -abo "WOFS" coldfire_main.bin

Найдёшь смещение — от него отсчитываешь блок той же структуры,
и кривые HPQ/LPQ окажутся там же, где у машинных ручек.

## ПУТЬ 2 — измерить на живой прошивке  (точнее всего)

Jick238/monomachine-lab это dsp56kEmu на настоящем бинаре.
В API есть debugYWord() — можно смотреть, что прошивка сама
положила в память после пересчёта ручки.

  // шаг 1: найти адрес ручки
  uint32_t before[0x28];
  for (int a=0; a<0x28; ++a) before[a] = eng.debugYWord(0x500+a);
  eng.setParameter(Parameter::FilterHighPassQ, 127);
  eng.renderBlock(blk);
  for (int a=0; a<0x28; ++a)
      if (eng.debugYWord(0x500+a) != before[a])
          printf("HPQ -> Y:$%03x\n", 0x500+a);

  // шаг 2: снять кривую целиком
  for (int v=0; v<=127; ++v) {
      eng.setParameter(Parameter::FilterHighPassQ, v);
      eng.renderBlock(blk);
      printf("%3d %06x\n", v, eng.debugYWord(ADDR));
  }

База страницы подтверждена: kVoicePageYAddress = 0x000500,
и V = Y:$528 при параметрах в V-$28..V-$01  ($528-$28 = $500).

Так же снимаются: LPQ, BOFS, WOFS, FilterAttack/Decay, AmpDistortion,
и готовые f/q из рабочей области Y:$91+.

## ПУТЬ 3 — опкоды  (если нужна именно структура)

  python tools/mmnova_slice.py show --listing "...dsp1_kernel_P0000-0B4D.txt" --from 0537 --to 05d2
  python tools/mmnova_slice.py show --listing "...dsp1_kernel_P0000-0B4D.txt" --from 05d3 --to 05fb
  grep -n -A60 "parameterPageOffset" core/monomachine_engine.cpp

Первая — блок расчёта Q и записи в Y:$91+.
Вторая — сами две петли SVF.
Третья — готовая таблица смещений из эмулятора.

## ЧТО ПРИСЛАТЬ МНЕ
Любое из:
  - вывод скрипта из Пути 1 (хотя бы для SID и CHORUS)
  - результат grep "HPQ" / "WOFS" по coldfire_main.bin
  - таблицу "ручка -> адрес" из Пути 2
  - вывод любой команды Пути 3
Соберу точный C++ без заглушек.
