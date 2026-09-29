#!/usr/bin/env python3
"""exp66_tail_frames.py — THE one batched experiment (rule 18) for the full
voice-frame tail $02EC-$0B4C of OS 1.32B.

Live FM machine (m8) in the track frame (pack-7 harness). For every set:
  * fresh emulator, machine INIT + CONF (pack-7 contract)
  * SEED capture: full X/Y $000-$0FF and $400-$4FF state before frame 0
  * N frames; per frame capture the same X/Y windows + machine output

Sweeps (absolute cells, r6 = $400 in the tail):
  BASE $408, HPQ? $409, EQF $404, EQG x$40B, FILT-ENV ATK/DEC $40C/$40D,
  WDTH $40E, BOFS? $410, WOFS? $411, LPQ $413, stage-2 ATK/DEC/BOFS/WOFS
  $414-$417 (pack-5 knobs), env2 $418-$41B, VOL $405, PAN $406,
  delay/echo state cells $438-$43F.

Output: /home/z/my-project/work/exp66_tail_frames.json.gz
"""
import sys, json, gzip
sys.path.insert(0, "/home/z/my-project/scripts")
sys.path.insert(0, "/home/z/my-project/work/pack7/tools")
import exp64_fm_pluginlevel as exp64
from exp64_fm_pluginlevel import sgn, P

# cache the disassembly across sets (the subprocess is the per-set bottleneck)
_disasm_cache = {}
_orig_disasm = exp64.disasm

def cached_disasm(start, count):
    key = (start, count)
    if key not in _disasm_cache:
        _disasm_cache[key] = _orig_disasm(start, count)
    return _disasm_cache[key]

exp64.disasm = cached_disasm
build_fm_track = exp64.build_fm_track
setup_page = exp64.setup_page

WIN_LO = [(0x000, 0x700)]     # full low X/Y: scratch + SVF states + page + stage2 work area


def snap(e):
    d = {}
    for side in ("X", "Y"):
        mem = getattr(e, side)
        for lo, hi in WIN_LO:
            for a in range(lo, hi):
                d["%s%03X" % (side, a)] = sgn(mem.get(a, 0))
    return d


def run_set(knobs=None, xknobs=None, seq=None, **page_kw):
    """knobs: {abs_cell: raw0..127} written as raw<<16 (Y side).
    xknobs: same for X side. seq: per-frame trig list."""
    knobs = knobs or {}
    xknobs = xknobs or {}
    seq = seq or [1] + [0] * 3
    e = build_fm_track()
    setup_page(e, **page_kw)
    for a, v in knobs.items():
        e.Y[a] = (v & 0xFFFF) << 16
    for a, v in xknobs.items():
        e.X[a] = (v & 0xFFFF) << 16
    # machine INIT (page wipe erases machine state)
    e.R[6] = P + 0x28
    e.R[7] = 0x100
    for r in range(8):
        e.M[r] = 0xFFFFFF
    e.ret_stack.append(0xDEAD)
    e.run(0x145D12, end=0x145D1C, max_steps=100000)
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()
    # CONF once before seed (pack-7: idempotent, harness contract)
    e.R[6] = P + 0x28
    e.R[7] = 0x100
    for r in range(8):
        e.M[r] = 0xFFFFFF
    e.ret_stack.append(0xDEAD)
    e.run(0x145D1D, end=0x145D20, max_steps=100000)
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()

    seed = snap(e)
    seed["Y120"] = e.Y[0x120]
    seed["Y121"] = e.Y[0x121]
    seed["Y122"] = e.Y[0x122]
    seed["Y123"] = e.Y[0x123]
    seed["Y124"] = e.Y[0x124]
    frames = []
    for f, trig in enumerate(seq):
        e.Y[P + 0x28] = trig
        e.run(0x0100, end=0x02EC, max_steps=500000)
        e.X[0x2C9] = 0x300
        e.run(0x02EC, end=0x0B4C, max_steps=500000)
        fr = {"f": f, "trig": trig}
        fr.update(snap(e))
        fr["mach"] = [sgn(e.Y[0x100 + i]) for i in range(0x22)]
        frames.append(fr)
    return seed, frames


def save(out):
    with gzip.open("/home/z/my-project/work/exp66_tail_frames.json.gz", "wb") as f:
        f.write(json.dumps(out).encode())



SECTIONS = {}

def section(num, name):
    def deco(fn):
        SECTIONS[num] = (name, fn)
        return fn
    return deco


@section(1, "base_wdth")
def sec1(emit):
    for base in (0, 16, 32, 64, 96, 112, 127):
        for wdth in (0, 32, 64, 127):
            s, fr = run_set(knobs={0x408: base, 0x40E: wdth},
                            seq=[1] + [0] * 3)
            emit("base_wdth", s, fr, dict(base=base, wdth=wdth))


@section(2, "hpq_lpq")
def sec2(emit):
    for hpq in (0, 32, 64, 96, 127):
        for lpq in (0, 32, 64, 96, 127):
            s, fr = run_set(knobs={0x409: hpq, 0x413: lpq},
                            seq=[1] + [0] * 3)
            emit("hpq_lpq", s, fr, dict(hpq=hpq, lpq=lpq))


@section(3, "eq")
def sec3(emit):
    for eqf in (0, 32, 64, 96, 127):
        for eqg in (1, 32, 64, 127):
            s, fr = run_set(knobs={0x404: eqf}, xknobs={0x40B: eqg},
                            seq=[1] + [0] * 3)
            emit("eq", s, fr, dict(eqf=eqf, eqg=eqg))


@section(4, "filtenv")
def sec4(emit):
    for atk in (0, 32, 64, 127):
        for dec in (0, 64, 127):
            s, fr = run_set(knobs={0x40C: atk, 0x40D: dec,
                                   0x408: 64, 0x40E: 64},
                            seq=[1] + [0] * 7)
            emit("filtenv", s, fr, dict(atk=atk, dec=dec))
    for bofs in (0, 64, 127):
        for wofs in (0, 64, 127):
            s, fr = run_set(knobs={0x410: bofs, 0x411: wofs,
                                   0x408: 64, 0x40E: 64, 0x40C: 64,
                                   0x40D: 64},
                            seq=[1] + [0] * 7)
            emit("filtbofs", s, fr, dict(bofs=bofs, wofs=wofs))


@section(5, "stage2")
def sec5(emit):
    for atk in (0, 64, 127):
        for dec in (0, 64, 127):
            s, fr = run_set(knobs={0x414: atk, 0x415: dec, 0x416: 64,
                                   0x417: 64},
                            seq=[1] + [0] * 7)
            emit("stage2_ad", s, fr, dict(atk=atk, dec=dec))
    for bofs in (0, 64, 127):
        for wofs in (0, 64, 127):
            s, fr = run_set(knobs={0x416: bofs, 0x417: wofs, 0x414: 64,
                                   0x415: 64},
                            seq=[1] + [0] * 7)
            emit("stage2_bw", s, fr, dict(bofs=bofs, wofs=wofs))


@section(6, "env2")
def sec6(emit):
    for a2 in (0, 64, 127):
        for d2 in (0, 64, 127):
            s, fr = run_set(knobs={0x418: a2, 0x419: d2, 0x41A: 64,
                                   0x41B: 64},
                            seq=[1] + [0] * 7)
            emit("env2_ad", s, fr, dict(atk=a2, dec=d2))
    for c in (0, 64, 127):
        s, fr = run_set(knobs={0x41A: c, 0x41B: c, 0x418: 64, 0x419: 64},
                        seq=[1] + [0] * 7)
        emit("env2_cd", s, fr, dict(c=c))
    s, fr = run_set(knobs={0x418: 64, 0x419: 64, 0x420: 1},
                    seq=[1] + [0] * 7)
    emit("env2_kill420", s, fr, dict(k420=1))
    s, fr = run_set(knobs={0x418: 64, 0x419: 64, 0x421: 4},
                    seq=[1] + [0] * 7)
    emit("env2_phase421", s, fr, dict(ph421=4))


@section(7, "delay")
def sec7(emit):
    for a in range(0x438, 0x440):
        for v in (32, 96):
            s, fr = run_set(knobs={a: v}, seq=[1] + [0] * 7)
            emit("delaycell", s, fr, dict(cell=a, val=v))
    s, fr = run_set(knobs={0x438: 64, 0x439: 64, 0x43A: 64, 0x43B: 64},
                    seq=[1] + [0] * 15)
    emit("delay_long", s, fr, dict(mid=True))


@section(8, "volpan")
def sec8(emit):
    for vol in (0, 64, 127):
        for pan in (0, 64, 127):
            s, fr = run_set(knobs={0x405: vol, 0x406: pan, 0x408: 64,
                                   0x40E: 64},
                            seq=[1] + [0] * 3)
            emit("volpan", s, fr, dict(vol=vol, pan=pan))


@section(9, "gates")
def sec9(emit):
    seq_gate = [1] + [0] * 7 + [1] + [0] * 7
    seq_kill = [1] + [0] * 5 + [2] + [0] * 9
    kw = {0x408: 96, 0x40E: 64, 0x40C: 32, 0x40D: 96,
          0x414: 32, 0x415: 96, 0x416: 32, 0x417: 96,
          0x409: 64, 0x413: 64}
    s, fr = run_set(knobs=kw, seq=seq_gate)
    emit("gate_full", s, fr, dict())
    s, fr = run_set(knobs=kw, seq=seq_kill)
    emit("gate_kill", s, fr, dict())


@section(10, "defaults")
def sec10(emit):
    s, fr = run_set(seq=[1] + [0] * 7)
    emit("defaults", s, fr, dict())


def main():
    nums = [int(x) for x in sys.argv[1:]] or sorted(SECTIONS)
    for n in nums:
        name, fn = SECTIONS[n]
        path = "/home/z/my-project/work/exp66_sec%02d.json.gz" % n
        out = {"sets": []}
        fn(lambda tag, s, fr, p: out["sets"].append(
            dict(tag=tag, params=p, seed=s, frames=fr)))
        with gzip.open(path, "wb") as f:
            f.write(json.dumps(out).encode())
        nf = sum(len(x["sets"]) for x in []) or sum(
            len(s["frames"]) for s in out["sets"])
        print("sec %02d (%s): %d sets, %d frames -> %s" % (
            n, name, len(out["sets"]), nf, path), flush=True)


if __name__ == "__main__":
    main()
