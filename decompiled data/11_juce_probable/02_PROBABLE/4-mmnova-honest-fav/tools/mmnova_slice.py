#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
mmnova_slice.py — локальный тулкит для добивания порта Monomachine OS 1.32B.

Зачем: у ассистента web_fetch обрезает большие файлы на ~100-150 строках,
поэтому длинные машины (REVERB 1257 слов, VO-6 2267) он видит на 3-8%.
Локально обрезки нет. Этот скрипт готовит куски, которые можно отдавать
на порт по одному.

Команды:
  slice   — нарезать dsp1_machines_fx.txt на файлы по машинам
  tables  — дампить таблицы из образа БЕЗ бага (addr - 0x100000)
  show    — напечатать листинг в диапазоне адресов
  stats   — сколько слов в каждом слайсе

Образы никуда не копируются, читаются по месту.
"""
import argparse, os, re, sys

# Адреса из таблицы диспетчеризации X:$10016B (подтверждены отчётом)
MACHINES = [
    ("01_GND-SIN",    0x144CC9, 0x144D4D),
    ("02_GND-NOIS",   0x144D9A, 0x144E80),
    ("03_SID",        0x14636F, 0x1464FB),
    ("04_SWAVE-SAW",  0x145932, 0x145AB4),
    ("05_SWAVE-PULS", 0x147214, 0x1473A1),
    ("06_DPRO-WAVE",  0x1473A2, 0x1474E8),
    ("07_DPRO-BBOX",  0x1474E9, 0x14764A),
    ("08_FM-STAT",    0x145D12, 0x145EC8),
    ("09_FM-PAR",     0x145EC9, 0x14619C),
    ("10_FM-DYN",     0x14619D, 0x1464FB),
    ("11_VO-6",       0x1464FC, 0x147213),
    ("13_FX-REV",     0x14538E, 0x145931),
    ("14_SWAVE-ENS",  0x145AB5, 0x145D11),
    ("15_FX-CHORUS",  0x147661, 0x1477DD),
    ("16_FX-DYN",     0x1477DE, 0x14789D),
    ("17_FX-RING",    0x14789E, 0x147B37),
    ("18_FX-PHA",     0x145036, 0x14538D),
    ("19_FX-FLA",     0x144E81, 0x145035),
    ("22_DPRO-DDRW",  0x147B38, 0x147D00),
    ("23_DPRO-DENS",  0x147D01, 0x148063),
]

# Таблицы. ВАЖНО: адрес отображается в образ НАПРЯМУЮ (off = addr * 3).
# Оригинальный extract_tables.py делал off = addr - 0x100000, из-за чего
# 26 из 43 дампов вышли нулевыми (Часть 5.1 аудита).
TABLES = [
    ("kLp144AC7",      0x144AC7,  258, "LP/TONE, общая для chorus и FM"),
    ("kRate141800",    0x141800,  128, "скорость AMP env + LFO SPD"),
    ("kRate141A00",    0x141A00,  128, "LFO MULT"),
    ("kDynWave141880", 0x141880, 1024, "вейвтейбл FM-DYN"),
    ("kFmRatio141A80", 0x141A80,   24, "24 отношения FM (эталон самопроверки)"),
    ("kPitch101AFB",   0x101AFB,  128, "питч SWAVE-ENS / DPRO-DENS / FX-EXT"),
    ("kSwave101A7B",   0x101A7B,  128, "зеркальная пара к $101AFB"),
    ("kPhaser141900",  0x141900,  128, "ступень 0 фазера"),
    ("kPhaser141980",  0x141980,  128, "ступень 1 фазера"),
    ("kDynAtk1444C6",  0x1444C6,  128, "attack FX-DYNAMIX"),
    ("kDynRel1446C6",  0x1446C6,  128, "release FX-DYNAMIX"),
    ("kSidFilt143D06", 0x143D06,  128, "фильтр SID"),
]

FM_RATIO_REF = [
    0x004000,0x008000,0x010000,0x018000,0x020000,0x028000,0x030000,0x040000,
    0x050000,0x060000,0x070000,0x080000,0x0A0000,0x0C0000,0x0E0000,0x100000,
    0x140000,0x180000,0x1C0000,0x200000,0x280000,0x300000,0x380000,0x400000,
]

ADDR_RE = re.compile(r"^([0-9a-fA-F]{6}):")


def parse_addr(line):
    m = ADDR_RE.match(line.strip())
    return int(m.group(1), 16) if m else None


def cmd_slice(a):
    if not os.path.exists(a.listing):
        sys.exit("нет файла: %s" % a.listing)
    os.makedirs(a.out, exist_ok=True)
    buckets = {name: [] for name, _, _ in MACHINES}
    with open(a.listing, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            ad = parse_addr(line)
            if ad is None:
                continue
            for name, lo, hi in MACHINES:
                if lo <= ad <= hi:
                    buckets[name].append(line.rstrip("\n"))
                    break
    total = 0
    for name, lo, hi in MACHINES:
        rows = buckets[name]
        path = os.path.join(a.out, name + "_full.txt")
        with open(path, "w", encoding="utf-8") as g:
            g.write("# %s\n# range $%06x..$%06x (%d words)\n# lines: %d\n\n"
                    % (name, lo, hi, hi - lo + 1, len(rows)))
            g.write("\n".join(rows) + "\n")
        total += len(rows)
        flag = "" if rows else "   <-- ПУСТО, проверь листинг"
        print("%-16s $%06x..$%06x  %5d строк%s" % (name, lo, hi, len(rows), flag))
    print("\nвсего строк: %d -> %s" % (total, a.out))


def read_words(path, addr, count, ws=3):
    off = addr * ws            # ПРЯМАЯ адресация. Никаких -0x100000.
    size = os.path.getsize(path)
    if off + count * ws > size:
        return None
    out = []
    with open(path, "rb") as f:
        f.seek(off)
        raw = f.read(count * ws)
    for i in range(count):
        b = raw[i * ws:(i + 1) * ws]
        v = (b[0] << 16) | (b[1] << 8) | b[2]     # 24-bit big-endian
        if v & 0x800000:
            v -= 0x1000000
        out.append(v)
    return out


def cmd_tables(a):
    if not os.path.exists(a.pmem):
        sys.exit("нет образа: %s" % a.pmem)

    # самопроверка адресации по эталонной ratio-таблице
    ref = read_words(a.pmem, 0x141A80, 24)
    ok = (ref == FM_RATIO_REF)
    print("самопроверка P:$141A80: %s" % ("OK, адресация прямая" if ok else "НЕ СОШЛАСЬ!"))
    if not ok:
        print("  ожидалось:", [hex(x) for x in FM_RATIO_REF[:6]], "...")
        print("  получено: ", [hex(x) for x in (ref or [])[:6]], "...")
        if not a.force:
            sys.exit("останов. Проверь образ или используй --force")

    dumped, zero = [], []
    for name, addr, cnt, note in TABLES:
        w = read_words(a.pmem, addr, cnt)
        if w is None:
            print("%-18s $%06x  ВНЕ ОБРАЗА" % (name, addr)); continue
        nz = sum(1 for v in w if v != 0)
        pct = 100.0 * nz / len(w)
        print("%-18s $%06x  %4d слов  ненулевых %5.1f%%  %s" % (name, addr, cnt, pct, note))
        if nz == 0:
            zero.append(name)
        dumped.append((name, addr, w, note))

    with open(a.out, "w", encoding="utf-8") as f:
        f.write("// Сгенерировано mmnova_slice.py tables — НЕ РЕДАКТИРОВАТЬ\n")
        f.write("// Источник: %s, прямая адресация off = addr*3\n" % os.path.basename(a.pmem))
        f.write("#pragma once\n#include <cstdint>\n\n")
        f.write("namespace mmnova { namespace tables { namespace fw {\n\n")
        for name, addr, w, note in dumped:
            f.write("// $%06x — %s\n" % (addr, note))
            f.write("static constexpr int %sSize = %d;\n" % (name, len(w)))
            f.write("static constexpr int32_t %s[%d] = {\n" % (name, len(w)))
            for i in range(0, len(w), 8):
                f.write("  " + ", ".join("%d" % v for v in w[i:i+8]) + ",\n")
            f.write("};\n\n")
        f.write("}}} // namespace\n")

    print("\nзаписано: %s (%d таблиц)" % (a.out, len(dumped)))
    if zero:
        print("нулевые (возможно runtime-область, инициализируются на живом железе):")
        for z in zero:
            print("  !", z)
        return 2
    return 0


def cmd_show(a):
    lo = int(a.frm, 16)
    hi = int(a.to, 16)
    n = 0
    with open(a.listing, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            ad = parse_addr(line)
            if ad is not None and lo <= ad <= hi:
                sys.stdout.write(line)
                n += 1
    print("\n# строк: %d  (диапазон $%06x..$%06x)" % (n, lo, hi), file=sys.stderr)


def cmd_stats(a):
    print("%-16s %-10s %6s" % ("машина", "адрес", "слов"))
    for name, lo, hi in MACHINES:
        print("%-16s $%06x %6d" % (name, lo, hi - lo + 1))


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    s = sub.add_parser("slice", help="нарезать листинг по машинам")
    s.add_argument("--listing", required=True)
    s.add_argument("--out", default="slices")
    s.set_defaults(fn=cmd_slice)

    t = sub.add_parser("tables", help="дампить таблицы из образа")
    t.add_argument("--pmem", required=True)
    t.add_argument("--out", default="MnMFirmwareTables.h")
    t.add_argument("--force", action="store_true")
    t.set_defaults(fn=cmd_tables)

    w = sub.add_parser("show", help="напечатать диапазон адресов")
    w.add_argument("--listing", required=True)
    w.add_argument("--from", dest="frm", required=True)
    w.add_argument("--to", required=True)
    w.set_defaults(fn=cmd_show)

    st = sub.add_parser("stats", help="размеры машин")
    st.set_defaults(fn=cmd_stats)

    a = ap.parse_args()
    rc = a.fn(a)
    sys.exit(rc if isinstance(rc, int) else 0)


if __name__ == "__main__":
    main()
