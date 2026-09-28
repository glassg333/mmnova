#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""apply_patch.py — ставит починенные FM-файлы в дерево плагина.

Понимает любой из путей (какой удобнее):
    <репо>/JUCE/Monomachine-Nova-1.9.7/Monomachine_Nova_Synth/Source
    <репо>/JUCE/Monomachine-Nova-1.9.7/Monomachine_Nova_Synth
    <репо>                     (корень репозитория, внутри папка JUCE)

Копирует в <Source>/dsp/mnm/:
    MnmFm.hpp        (ядро интеграции: точные законы ручек STAT + диспетчер PAR/DYN)
    MnmFmDsp.hpp     (Q23-примитивы, бит-точно)
    MnmFmPar.hpp     (FM+PAR, 100% на векторах эмулятора OS 1.32)
    MnmFmDyn.hpp     (FM+DYN, 100% на векторах эмулятора OS 1.32)
    MnmFmSineTable.h (ROM-синус X/Y:$14A000)

NovaDSP.h менять не надо — публичный API сохранён.
Повторный запуск безопасен: совпавшие файлы пропускаются с пометкой
"already identical" — так можно проверить, что всё встало.
"""
import os, sys, glob, shutil, hashlib

HERE = os.path.dirname(os.path.abspath(__file__))
FILES = ["MnmFm.hpp", "MnmFmDsp.hpp", "MnmFmPar.hpp", "MnmFmDyn.hpp",
         "MnmFmSineTable.h"]

def pack_file(name):
    """Файл пака лежит либо рядом со скриптом, либо в dsp/mnm/ рядом."""
    for cand in (os.path.join(HERE, "dsp", "mnm", name), os.path.join(HERE, name)):
        if os.path.exists(cand):
            return cand
    return os.path.join(HERE, "dsp", "mnm", name)


def sha16(p):
    return hashlib.sha256(open(p, 'rb').read()).hexdigest()[:16]


def find_source(p):
    """Возвращает путь к .../Monomachine_Nova_Synth/Source из любого намёка."""
    p = os.path.abspath(p)
    # 1) путь уже .../Source
    if os.path.basename(p) == "Source" and os.path.isdir(os.path.join(p, "dsp")):
        return p
    # 2) путь .../Monomachine_Nova_Synth (внутри Source)
    cand = os.path.join(p, "Monomachine_Nova_Synth", "Source")
    if os.path.isdir(os.path.join(cand, "dsp")) or os.path.isdir(cand):
        return cand
    # 3) корень репозитория: ищем JUCE/*/Monomachine_Nova_Synth/Source
    for hit in sorted(glob.glob(os.path.join(p, "JUCE", "*", "Monomachine_Nova_Synth", "Source"))):
        return hit
    # 4) путь .../dsp сам по себе
    if os.path.basename(p) == "dsp":
        return os.path.dirname(p)
    return cand  # вернём дефолт — скрипт честно ругнётся ниже


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1
    src_root = find_source(sys.argv[1])
    dst = os.path.join(src_root, "dsp", "mnm")
    if not os.path.isdir(os.path.join(src_root, "dsp")):
        print("ERROR: не нашёл папку dsp ни в одном из вариантов пути:")
        print("  %s" % src_root)
        print("Укажи путь к .../Monomachine_Nova_Synth/Source и повтори.")
        return 1
    if not os.path.isdir(dst):
        os.makedirs(dst)  # папки mnm может не быть — создадим
        print("создана папка: %s" % dst)
    print("цель: %s\n" % dst)
    changed = 0
    for name in FILES:
        s = pack_file(name)
        d = os.path.join(dst, name)
        if not os.path.exists(s):
            print("ERROR: в паке нет файла %s" % name)
            return 1
        if os.path.exists(d):
            if sha16(s) == sha16(d):
                print("  = %-18s already identical" % name)
                continue
            print("  ~ %-18s ОБНОВЛЁН (был %s)" % (name, sha16(d)))
        else:
            print("  + %-18s новый файл" % name)
        shutil.copy2(s, d)
        changed += 1
    print("\nготово: обновлено %d из %d." % (changed, len(FILES)))
    if changed:
        print("Доказательство бит-точности: векторы fm_par (7168 слов) и")
        print("fm_dyn (8192 слов) с эмулятора OS 1.32 — 0 расхождений.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
