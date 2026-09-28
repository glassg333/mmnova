#!/usr/bin/env python3
"""fenv_extract_curves.py — извлечение кривых фильтра из образа P-памяти DSP1.

Кривая $144AC7 (258 слов) — «тон/срез» (та же, что TONE у FM-машин и HP/LP
реверба). Адрес $144B48, используемый блоком L3 (стадия 2 фильтра) — это
ТА ЖЕ область со смещением +$81 (129 слов): X:$144B48[i] = X:$144AC7[129+i].
Следующая известная таблица начинается на $144BC9 (LPQ), поэтому кривая AC7
= ровно 258 слов ($144AC7..$144BC8).

Для безопасности индексов L3 (idx до 255) экстракция расширяется до $144FFF.
Выход: work/fm_fenv/MnmFilterCurves.h (C++-заголовок, hex-слова).
"""
import os

PM = "/home/z/my-project/work/mmnova/decompiled data/02_memory_images/dsp1_pmem.bin"
OUT_DIR = "/home/z/my-project/work/fm_fenv"
OUT_H = os.path.join(OUT_DIR, "MnmFilterCurves.h")

BASE = 0x144AC7
END = 0x160000          # окно $144AC7..$15FFFF — весь диапазон возможных
                        # чтений L3 (индекс = биты [40..17] состояния делеЯ,
                        # до 24 бит); за $160000 эмулятор читает 0


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    raw = open(PM, "rb").read()
    n = len(raw) // 3
    words = []
    for i in range(BASE, min(END, n)):
        o = i * 3
        words.append((raw[o] << 16) | (raw[o + 1] << 8) | raw[o + 2])
    lines = []
    lines.append("// MnmFilterCurves.h — Elektron Monomachine SFX-60/MkII OS 1.32B")
    lines.append("// Кривая фильтра X:$144AC7 (258 слов, 0.0..1.0 Q23) и окно до $144FFF")
    lines.append("// для чтений L3 через базу $144B48 (= AC7 + 129).")
    lines.append("// Источник: decompiled data/02_memory_images/dsp1_pmem.bin (P-ROM).")
    lines.append("// $144BC9-$144C48 = таблицы T1/T2 (LPQ-резонанс) — включены, чтобы")
    lines.append("// выходы за 258-е слово читали реальную прошивку, как эмулятор.")
    lines.append("#ifndef MNM_FILTER_CURVES_H")
    lines.append("#define MNM_FILTER_CURVES_H")
    lines.append("")
    lines.append("#include <cstdint>")
    lines.append("")
    lines.append("namespace mnmfm {")
    lines.append("")
    lines.append("// words[i] = X:$144AC7 + i, i = 0..%d" % (len(words) - 1))
    lines.append("static const int kFilterCurveLen = %d;" % len(words))
    lines.append("static const uint32_t kFilterCurve[%d] = {" % len(words))
    for i in range(0, len(words), 8):
        row = ", ".join("0x%06X" % w for w in words[i:i + 8])
        lines.append("    " + row + ",")
    lines.append("};")
    lines.append("")
    lines.append("} // namespace mnmfm")
    lines.append("")
    lines.append("#endif // MNM_FILTER_CURVES_H")
    open(OUT_H, "w").write("\n".join(lines) + "\n")
    print("words: %d ($%X..$%X) -> %s" % (len(words), BASE, BASE + len(words) - 1, OUT_H))
    # первые/последние значения для контроля
    print("AC7[0..4] =", ["%06X" % w for w in words[:5]])
    print("AC7[254..257] =", ["%06X" % w for w in words[254:258]])
    print("B48 offset check: AC7[129] =", "%06X" % words[129])


if __name__ == "__main__":
    main()
