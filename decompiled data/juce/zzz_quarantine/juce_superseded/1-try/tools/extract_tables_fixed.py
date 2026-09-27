#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
extract_tables_fixed.py — корректный экстрактор таблиц прошивки Monomachine
OS 1.32B в C++-заголовок для порта (Source/mmnova/MnMFirmwareTables.h).

ЧТО ИСПРАВЛЕНО (аудит 00_ОТЧЁТ_аудит_данных.md, Часть 5.1):
    Оригинальный extract_tables.py для всех адресов >= $100000 делал
        off = addr - 0x100000
    хотя докстринг обещал прямое отображение. Внешняя SRAM DSP56303 алиасится
    между P:/X:/Y: (доказано в 06_docs/KERNEL_VOICE_CHAIN.md §1), а образы
    dsp1_pmem.bin и т.п. индексируются АДРЕСОМ НАПРЯМУЮ. Из-за вычитания
    26 из 43 дампов получились нулевыми (LP-коэффициенты, SID-фильтр,
    attack/release/RMS компрессора, все 6 таблиц фазера, вейвтейблы SWAVE,
    4 таблицы DPRO-BBOX, GND-NOIS).
    Здесь: off = addr * WORDSIZE, плюс проверка «дамп не нулевой»
    (ненулевой код возврата, если таблица пустая).

Использование:
    python3 extract_tables_fixed.py \
        --pmem "decompiled data/02_memory_images/dsp1_pmem.bin" \
        --ymem "decompiled data/02_memory_images/dsp1_ymem.bin" \
        --out  Source/mmnova/MnMFirmwareTables.h

Потом собирать плагин с -DMMNOVA_HAVE_FW_TABLES=1.
Образы НИКУДА не копируются — читаются по месту, потоково.
"""

import argparse, os, struct, sys

WORDSIZE_DEFAULT = 3          # 24-битное слово = 3 байта (big-endian)

# --- таблицы, нужные порту -------------------------------------------------
#  имя в C++            адрес      слов   память  статус в репо
TABLES = [
    ("kLp144AC7",        0x144AC7,  258,   "P",  "FIXED  (был нулевым)"),
    ("kRate141800",      0x141800,  128,   "Y",  "FIXED  (AMP env + LFO SPD)"),
    ("kDynWave141880",   0x141880, 1024,   "P",  "FIXED  (FM-DYN wavetable)"),
    ("kFmRatio141A80",   0x141A80,   24,   "P",  "VERIFIED"),
    ("kRate141A00",      0x141A00,  128,   "Y",  "FIXED  (LFO MULT)"),
    ("kNoiseTable",      0x11B451, 1024,   "P",  "ZERO-IN-IMAGE (проверьте)"),
    ("kPitch101AFB",     0x101AFB,  256,   "P",  "FX-EXT pitch table"),
    ("kPitch101BFB",     0x101BFB,  256,   "P",  "FX-EXT pitch table"),
    ("kPitch101CFB",     0x101CFB,  256,   "P",  "FX-EXT pitch table"),
]

# эталон для самопроверки: P:$141A80 = 24 музыкальных отношения 1.23 fixed-point
FM_RATIO_REF = [
    0x004000, 0x008000, 0x010000, 0x018000, 0x020000, 0x028000, 0x030000, 0x040000,
    0x050000, 0x060000, 0x070000, 0x080000, 0x0A0000, 0x0C0000, 0x0E0000, 0x100000,
    0x140000, 0x180000, 0x1C0000, 0x200000, 0x280000, 0x300000, 0x380000, 0x400000,
]


def read_words(path, addr, count, wordsize):
    """Прямое отображение: offset = addr * wordsize. Никаких -0x100000."""
    off = addr * wordsize
    size = os.path.getsize(path)
    if off + count * wordsize > size:
        return None, "адрес за пределами образа (off=0x%X, size=0x%X)" % (off, size)
    out = []
    with open(path, "rb") as f:
        f.seek(off)
        raw = f.read(count * wordsize)
    for i in range(count):
        b = raw[i * wordsize:(i + 1) * wordsize]
        if wordsize == 3:
            v = (b[0] << 16) | (b[1] << 8) | b[2]
        elif wordsize == 4:
            v = struct.unpack(">I", b)[0] & 0xFFFFFF
        else:
            raise SystemExit("wordsize must be 3 or 4")
        if v & 0x800000:
            v -= 0x1000000
        out.append(v)
    return out, None


def autodetect_wordsize(path, hint):
    """Проверяем по эталонной ratio-таблице, 3 или 4 байта на слово."""
    for ws in (hint, 3, 4):
        w, err = read_words(path, 0x141A80, 24, ws)
        if w and w == FM_RATIO_REF:
            return ws, True
    return hint, False


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--pmem", required=True, help="dsp1_pmem.bin")
    ap.add_argument("--ymem", help="dsp1_ymem.bin (если Y-таблицы в отдельном образе)")
    ap.add_argument("--out", required=True, help="выходной MnMFirmwareTables.h")
    ap.add_argument("--wordsize", type=int, default=WORDSIZE_DEFAULT)
    ap.add_argument("--allow-zero", action="store_true",
                    help="не считать нулевую таблицу ошибкой")
    a = ap.parse_args()

    ws, verified = autodetect_wordsize(a.pmem, a.wordsize)
    print("word size: %d bytes%s" % (ws, "  (подтверждён по P:$141A80)" if verified
                                     else "  (эталон НЕ совпал - проверьте образ!)"))

    dumped, problems = {}, []
    for name, addr, count, mem, note in TABLES:
        # внешняя SRAM алиасится: X:/Y:/P: >= $100000 -> один и тот же образ.
        src = a.pmem
        if mem == "Y" and a.ymem and addr < 0x100000:
            src = a.ymem
        words, err = read_words(src, addr, count, ws)
        if err:
            problems.append("%-16s $%06X : %s" % (name, addr, err))
            continue
        if all(v == 0 for v in words):
            problems.append("%-16s $%06X : ВСЕ НУЛИ (%s)" % (name, addr, note))
            if not a.allow_zero:
                continue
        dumped[name] = (addr, words, note)
        print("  ok  %-16s $%06X  %4d слов  %s" % (name, addr, count, note))

    if "kFmRatio141A80" in dumped and dumped["kFmRatio141A80"][1] != FM_RATIO_REF:
        problems.append("kFmRatio141A80 не совпал с эталоном -> образ/адресация неверны")

    os.makedirs(os.path.dirname(os.path.abspath(a.out)) or ".", exist_ok=True)
    with open(a.out, "w", encoding="utf-8") as f:
        f.write("// Сгенерировано tools/extract_tables_fixed.py — НЕ РЕДАКТИРОВАТЬ\n")
        f.write("// Источник: %s (word=%d B, прямая адресация)\n" % (
            os.path.basename(a.pmem), ws))
        f.write("#pragma once\n#include <cstdint>\n\n")
        f.write("namespace mmnova { namespace tables { namespace fw {\n\n")
        for name, (addr, words, note) in dumped.items():
            f.write("// $%06X — %s\n" % (addr, note))
            f.write("static constexpr int %sSize = %d;\n" % (name, len(words)))
            f.write("static constexpr int32_t %s[%d] = {\n" % (name, len(words)))
            for i in range(0, len(words), 8):
                f.write("    " + ", ".join("%d" % v for v in words[i:i + 8]) + ",\n")
            f.write("};\n\n")
        # алиасы под имена, которые ждёт MnMTables.cpp
        f.write("}}} // namespace\n")
    print("\nзаписано: %s (%d таблиц)" % (a.out, len(dumped)))

    if problems:
        print("\nПРОБЛЕМЫ:")
        for p in problems:
            print("  ! " + p)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
