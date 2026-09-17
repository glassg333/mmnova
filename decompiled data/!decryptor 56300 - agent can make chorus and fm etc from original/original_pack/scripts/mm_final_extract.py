#!/usr/bin/env python3
# ФИНАЛЬНАЯ ЭКСТРАКЦИЯ: полные P/X/Y образы DSP1/DSP2 из BIN + сверка с репо.
import struct, os, sys
sys.path.insert(0, "/home/z/my-project/scripts")
from mm_aplib_mm import read_sections

OUT = "/home/z/my-project/mm_work"
REPO = "/home/z/my-project/repo-mmnova/decompiled data"

def depack_full(d, ip, hard_end):
    """Декомпрессор: работает до исчерпания входа (жёсткая граница секции)."""
    out = bytearray(); last = 1; tag = 0
    def getbit():
        nonlocal tag, ip
        tag = (tag << 1) & 0xFFFFFFFF
        if (tag & 0xFF) == 0:
            if ip >= hard_end: raise EOFError
            b = d[ip]; ip += 1
            tag = ((b << 1) | 1) & 0xFFFFFFFF
            return (b >> 7) & 1
        return (tag >> 8) & 1
    def getgamma():
        v = 1
        while True:
            v = v * 2 + getbit()
            if getbit(): return v
    try:
        while True:
            if getbit():
                out.append(d[ip]); ip += 1
                continue
            v = getgamma()
            if v == 2: dist = last
            else:
                b = d[ip]
                dist = (v << 8) + b - 768 + 1
                ip += 1
                last = dist
            ba = getbit(); bb = getbit()
            sl = 2 * ba + bb
            L = sl if sl else getgamma() + 2
            if dist > 3328: L += 1
            cp = len(out) - dist
            if cp < 0: raise ValueError("dist>out")
            for i in range(L + 1):
                out.append(out[cp + i])
    except EOFError:
        pass
    return bytes(out), ip

def parse_records(buf):
    """func_7ce: [cmd LE24][addr LE24][count LE24][data LE24]; cmd 0=P 1=X 2=Y; 3=JMP → стоп."""
    w = [int.from_bytes(buf[i:i+3], 'little') for i in range(0, len(buf) - len(buf) % 3, 3)]
    recs = []
    i = 0
    # пропускаем ведущий JMP ([3][addr])
    if w[0] == 3:
        i = 2
    final = None
    while i < len(w):
        cmd = w[i]
        if cmd == 3:
            final = w[i+1]
            i += 2
            break
        if cmd > 3:
            return recs, final, i, ('bad_cmd', cmd)
        addr, cnt = w[i+1], w[i+2]
        data = w[i+3:i+3+cnt]
        if len(data) < cnt:
            return recs, final, i, ('truncated',)
        recs.append((cmd, addr, cnt, data))
        i += 3 + cnt
    return recs, final, i, None

def build_image(recs):
    P, X, Y = {}, {}, {}
    for cmd, addr, cnt, data in recs:
        tgt = {0: P, 1: X, 2: Y}[cmd]
        for k, v in enumerate(data):
            tgt[addr + k] = v
    return P, X, Y

def save_flat(d, path, max_addr):
    b = bytearray()
    for a in range(max_addr + 1):
        v = d.get(a)
        b += (v if v is not None else 0).to_bytes(3, 'big')
    open(path, 'wb').write(b)
    return bytes(b)

def main():
    data = open("/home/z/my-project/repo-mmnova/elektron_sfx6-60_os1.32b.bin", "rb").read()
    secs = read_sections(data)
    bufs = {}
    for k in (1, 2, 3):
        s = secs[k]
        bufs[k], ip = depack_full(data, s["off"] + 8, s["off"] + 8 + s["slen"])
        open(f"{OUT}/sec{k}_full.bin", "wb").write(bufs[k])
    recs = {}
    for k in (1, 2, 3):
        r, final, consumed, err = parse_records(bufs[k])
        recs[k] = r
        banks = {}
        for cmd, addr, cnt, dat in r:
            banks[{0:'P',1:'X',2:'Y'}[cmd]] = banks.get({0:'P',1:'X',2:'Y'}[cmd], 0) + cnt
        jmpstr = f"{final:#x}" if final is not None else "нет"
        print(f"секция {k}: {len(r)} записей, JMP->{jmpstr}, err={err}, банки={banks}")
        with open(f"{OUT}/sec{k}_records.txt", "w") as f:
            for cmd, addr, cnt, dat in r:
                nm = {0: 'P', 1: 'X', 2: 'Y'}[cmd]
                f.write(f"{nm} {addr:#08x} {cnt:>6} слов\n")

    P1, X1, Y1 = build_image(recs[1] + recs[3])
    P2, X2, Y2 = build_image(recs[2] + recs[3])
    print(f"DSP1: P {len(P1)} слов, X {len(X1)}, Y {len(Y1)}")
    print(f"DSP2: P {len(P2)} слов, X {len(X2)}, Y {len(Y2)}")

    # сравнение с репо
    rp1 = open(f"{REPO}/dsp1_pmem.bin", "rb").read()
    rp2 = open(f"{REPO}/dsp2_pmem.bin", "rb").read()
    rx1 = open(f"{REPO}/dsp1_xmem.bin", "rb").read()
    rx2 = open(f"{REPO}/dsp2_xmem.bin", "rb").read()
    ry1 = open(f"{REPO}/dsp1_ymem.bin", "rb").read()
    ry2 = open(f"{REPO}/dsp2_ymem.bin", "rb").read()

    def cmp_bank(mydict, refbytes, name):
        nref = len(refbytes) // 3
        mism = 0; first = None; ref_nonzero_missing = 0
        for a in range(nref):
            refv = int.from_bytes(refbytes[a*3:a*3+3], 'big')
            myv = mydict.get(a)
            if myv is None:
                if refv != 0:
                    ref_nonzero_missing += 1
                    if first is None: first = (a, None, refv)
                continue
            if myv != refv:
                mism += 1
                if first is None: first = (a, myv, refv)
        extra = sum(1 for a in mydict if a >= nref and mydict[a] != 0)
        print(f"{name}: слов эталона {nref}, несовпадений {mism}, эталон≠0 без моего {ref_nonzero_missing}, моих лишних {extra}, first={first}")

    cmp_bank(P1, rp1, "DSP1 P vs dsp1_pmem.bin")
    cmp_bank(P2, rp2, "DSP2 P vs dsp2_pmem.bin")
    cmp_bank(X1, rx1, "DSP1 X vs dsp1_xmem.bin")
    cmp_bank(X2, rx2, "DSP2 X vs dsp2_xmem.bin")
    cmp_bank(Y1, ry1, "DSP1 Y vs dsp1_ymem.bin")
    cmp_bank(Y2, ry2, "DSP2 Y vs dsp2_ymem.bin")

    # сохранение моих образов
    save_flat(P1, f"{OUT}/my_dsp1_pmem.bin", max(P1))
    save_flat(P2, f"{OUT}/my_dsp2_pmem.bin", max(P2))
    save_flat(X1, f"{OUT}/my_dsp1_xmem.bin", max(X1))
    save_flat(X2, f"{OUT}/my_dsp2_xmem.bin", max(X2))
    save_flat(Y1, f"{OUT}/my_dsp1_ymem.bin", max(Y1))
    save_flat(Y2, f"{OUT}/my_dsp2_ymem.bin", max(Y2))
    print("мои образы сохранены в", OUT)

if __name__ == "__main__":
    main()
