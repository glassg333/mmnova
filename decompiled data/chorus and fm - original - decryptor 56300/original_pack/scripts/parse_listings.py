#!/usr/bin/env python3
# Парсер листингов DSP56300 -> эталонные пары (адрес, слова, текст)
import re, os, json

REPO = "/home/z/my-project/repo-mmnova/decompiled data"
FILES = ["chorus_disasm.txt", "dsp1_vectors_core.txt", "dsp1_dispatch.txt",
         "dsp1_machines_fx.txt", "dsp2_vectors_core.txt", "dsp2_dispatch.txt",
         "dsp2_machines_fx.txt"]

LINE_RE = re.compile(r"^([0-9a-f]{6}): (.*?)(?:\s*;\s*(.+))?$")

def parse_all():
    pairs = {}
    for fn in FILES:
        p = os.path.join(REPO, fn)
        for line in open(p, encoding="utf-8", errors="replace"):
            m = LINE_RE.match(line.rstrip("\n"))
            if not m:
                continue
            addr = int(m.group(1), 16)
            text = m.group(2).rstrip()
            words_hex = m.group(3)
            if not words_hex:
                continue
            # отбрасываем аннотации вида "(bits: ...)" / "(callers: ...)"
            words_hex = re.sub(r"\([^)]*\)", "", words_hex)
            try:
                words = [int(x, 16) for x in words_hex.split()]
            except ValueError:
                continue
            if not words:
                continue
            pairs[addr] = (words, text)
    return pairs

if __name__ == "__main__":
    pairs = parse_all()
    print(f"эталонных инструкций: {len(pairs)}")
    # статистика мнемоник
    from collections import Counter
    c = Counter(t.split()[0] for _, t in pairs.values())
    print("мнемоник:", len(c))
    for m, n in c.most_common(50):
        print(f"  {m}: {n}")
    import pickle
    pickle.dump(pairs, open("/home/z/my-project/mm_work/gt_pairs.pkl", "wb"))
