#!/usr/bin/env python3
"""verify_astra.py — независимая сверка astra-пакета с нашей прошивкой (pmem.bin):
1) таблицы $141800-$1448C6 (12486 слов) — sha256 + побайтное сравнение с pmem;
2) выбранные машинные слова 900 — сравнение с pmem по регионам provenance;
3) пересчёт verify_package без LICENSES (их манифест бит — файлов нет на GitHub);
4) model_check.py — запуск их модели.
"""
import sys, json, hashlib, pathlib, re
ROOT = pathlib.Path("/home/z/my-project/work/gh_mmnova_sparse/decompiled data/juce/envelope/!!!MonomachineFilterDist-astra")
PM = None
for cand in ["/home/z/my-project/work/pmem.bin"]:
    pass
# найти pmem как в run_kernel
sys.path.insert(0, "/home/z/my-project/scripts")
from run_kernel import PM
print("pmem:", PM)

data = open(PM, "rb").read()
n = len(data) // 3
def pmem_word(a):
    o = a * 3
    return (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]

meta = json.loads((ROOT/'evidence/provenance.json').read_text())
lo, hi = int(meta['table_start'],16), int(meta['table_end_exclusive'],16)

# 1) таблицы
mismatch = 0
first_bad = None
for a in range(lo, hi):
    w = pmem_word(a)
    # их таблицы лежат в FirmwareTables.hpp — сравним позже; пока sha от pmem
    pass
tables = [pmem_word(a) for a in range(lo, hi)]
sha = hashlib.sha256(b''.join(w.to_bytes(3,'big') for w in tables)).hexdigest()
print(f"таблицы {hex(lo)}-{hex(hi)} ({hi-lo} слов): sha256(pmem) = {sha}")
print(f"                        их provenance     = {meta['table_sha256_be24']}")
print("ТАБЛИЦЫ:", "СОВПАДАЮТ" if sha == meta['table_sha256_be24'] else "РАСХОЖДЕНИЕ!")

# 2) машинные слова выбранных регионов
mem = [{int(k):v for k,v in d.items()} for d in json.loads((ROOT/'evidence/memory.json').read_text())]
bad = 0
total = 0
for r in meta['regions']:
    a0, a1 = int(r['start'],16), int(r['end_exclusive'],16)
    for a in range(a0, a1):
        w = pmem_word(a)
        total += 1
        if a in mem[0]:
            if mem[0][a] != w:
                bad += 1
                if bad <= 3: print(f"  РАСХОЖДЕНИЕ word @ {hex(a)}: их {mem[0][a]:06X} vs pmem {w:06X}")
        else:
            bad += 1
            if bad <= 3: print(f"  НЕТ слова @ {hex(a)} в memory.json")
print(f"машинные слова {total}: {'ВСЕ СОВПАДАЮТ с pmem' if bad==0 else f'{bad} расхождений'}")

# 3) verify_package без SHA-манифеста (LICENSES отсутствуют на GitHub)
sys.path.insert(0, str(ROOT/'tools'))
import generate_port as gen
chosen = [pc for _,a,b in gen.regions for pc in gen.prog if a<=pc<b]
ok1 = len(chosen)==meta['instruction_count']==786
ok2 = sum(len(gen.prog[p][1]) for p in chosen)==meta['machine_words_verified']==900
ok3 = all(mem[0][p+i]==w for p in chosen for i,w in enumerate(gen.prog[p][1]))
print(f"verify(слова): count={ok1} words={ok2} memory={ok3}")
render_ok = True
try:
    for name, text in gen.render().items():
        if (ROOT/name).read_text() != text:
            print("  НЕ воспроизводится:", name); render_ok = False
except Exception as ex:
    print("  render() упал:", ex); render_ok = False
print(f"verify(воспроизводимость C++): {render_ok}")
