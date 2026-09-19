#!/usr/bin/env python3
from pathlib import Path
import hashlib, re, struct

ROOT = Path(__file__).resolve().parents[1]

def words(path):
    b = path.read_bytes()
    assert len(b) % 3 == 0, path
    return [int.from_bytes(b[i:i+3], "big") for i in range(0,len(b),3)]

def header_words(text, name):
    s = (ROOT / "include" / text).read_text()
    m = re.search(r"%s\[\d+\] = \{(.*?)\};" % name, s, re.S)
    assert m, name
    return [int(x,16) for x in re.findall(r"0x([0-9A-Fa-f]+)u",m.group(1))]

checks = [
    ("filt_cutoff_P143546_1700.bin", "kCutoffP143546", 1700),
    ("filt_cutoff_P1435C6_1600.bin", "kCutoffP1435C6", 1600),
]
for fn,name,n in checks:
    p=ROOT/"data"/fn
    b=p.read_bytes()
    assert len(b)==n*3 and len(words(p))==n
    assert words(p)==header_words("MnmFilterTables.h",name)
    print(fn, n, hashlib.sha256(b).hexdigest())

assert words(ROOT/"data"/"filt_cutoff_P143546_1700.bin")[0] == 0x000D35
assert words(ROOT/"data"/"filt_cutoff_P143546_1700.bin")[-1] == 0x74242A
assert words(ROOT/"data"/"filt_cutoff_P1435C6_1600.bin")[0] == 0x001A68
print("OK: embedded arrays and raw files match")
