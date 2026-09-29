#!/usr/bin/env python3
"""exp67_routing.py — THE one batched experiment (rule 18) for the block-level
ROUTING/MIXER of OS 1.32B: P:$0087-$0218 (the parts around the voice frame).

What the oracle runs, per set (REAL block loop, no harness shortcuts):
  S1 $0087-$00CE  header poll (HRX = $140/$160/$180) -> mode tables:
                  X:$2C0-$2C3 = bus bases, X:$2C9/2CA/2CB = echo pointers,
                  y:$123 = $528, y:$124 = 0
  per sub-block k = 0..2 (page base $500+$100k, y:$123 = $528+$100k):
  S2 $00CF-$00FF  flags y:(page-$3) -> X:$2C4; INIT dispatch (ID cell
                  page-$4 vs latch y:$120+k, table $10016B); CONF ($10018D)
     $0100-$02EB  frame pre: flags re-read, codec-input path (bits 6/7),
                  pitch/prep func_000262, machine PROC (jsr $1001AF+id)
     $02EC-$0B4C  voice frame tail (pack-8 code, page from y:$123)
  S3 $0143-$0163  mixer: bus Y:$0000-$1F into buses 1/2/3 per flags
                  bits 0/1/2 (add), or replace when bit3 + C/D/E
     $01A3-$01E1  end of block (flags of page 2): bit8 MIX sum of all
                  three buses into bus 1; bitA copy X:$2CA/$2CB regions
                  into the buses

Captured (diffs vs seed, windows X/Y $000-$700):
  MODE  S1 results (X:$2C0-$2CB explicit)
  SB k  after the mixer of sub-block k
  END   end of block

Sweeps: 3 headers x (single/combined bus bits, replace mode bit3+C/D/E,
MIX bit8, copy bitA, input paths bits 6/7, FM slot placement, realistic
mode layouts 6*MONO / 3*STEREO).

Output: /home/z/my-project/work/pack9/vectors/routing_vectors.txt
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
sys.path.insert(0, "/home/z/my-project/work/pack7/tools")
import exp64_fm_pluginlevel as exp64
from exp64_fm_pluginlevel import sgn

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

FM_ID = 0x1001AF + 8      # FM STAT slot id (PROC-table offset)
NULL_ID = 0x1001AF + 0    # null machine (rts at $145D1C)

PAGE_BASES = (0x500, 0x600, 0x700)
MPAGE_BASES = (0x528, 0x628, 0x728)
WIN = 0x800               # diff/seed window X/Y $000-$800 (pages $500-$7FF!)

DIFF_LO, DIFF_HI = 0x000, 0x800


def setup_pages(e, ids, flags, knobs=(64,) * 8, vol=127, pan=64, dsnd=0x400000):
    """Wipe + fill the three OS pages ($500/$600/$700) exactly like the
    pack-7 harness fills page $400 (same cell semantics), plus the ID cell
    (page+$24), the routing flag word (page+$25) and the T6 scale cell
    (page+$1F, the delay-return depth input)."""
    for base, mid, fl in zip(PAGE_BASES, ids, flags):
        Y, X = e.Y, e.X
        for i in range(0x100):
            Y[base + i] = 0
            X[base + i] = 0
        # AMP env + gain (exp64.setup_page defaults, P -> base)
        Y[base + 0x00] = 0                       # ATK
        Y[base + 0x01] = 0                       # HOLD
        Y[base + 0x02] = (127 & 0xFFFF) << 16    # DEC
        Y[base + 0x03] = 0                       # REL
        Y[base + 0x05] = (vol & 0xFFFF) << 16    # VOL
        Y[base + 0x06] = (pan & 0xFFFF) << 16    # PAN
        Y[base + 0x1E] = (64 & 0xFFFF) << 16     # w30 pitch mod
        Y[base + 0x23] = 120                     # TEMPO (BPM raw)
        Y[base + 0x29] = 11776                   # pitch word (note 69-ish)
        Y[base + 0x2A] = 2880                    # tick word (24*BPM)
        # glide state settled (PORT = 0)
        X[base + 0x06] = 11776
        X[base + 0x07] = 11776
        Y[base + 0x07] = 11776
        X[base + 0x08] = 11776
        X[base + 0x09] = 0
        X[base + 0x02] = 11776
        X[base + 0x03] = 0
        for i, v in enumerate(knobs):
            Y[base + 0x2C + i] = (v & 0xFFFF) << 16
        # trig cell = page word 0 (y:$123): bit7 = "params changed" one-shot
        # (gates the INIT/CONF dispatch at $00E1-$00FF and is consumed there),
        # bit0 = trig so the AMP env actually fires (non-zero frame bus).
        Y[base + 0x28] = 0x81
        Y[base + 0x1F] = dsnd                  # T6 mod/return scale cell
        Y[base + 0x24] = mid                     # machine ID cell (page-$4 from y:$123)
        Y[base + 0x25] = fl & 0xFFFFFF           # routing flags word (page-$3)
    # sub-block latches = 0 -> INIT fires on the first block (OS behavior)
    for k in range(3):
        e.Y[0x120 + k] = 0


def seed_buses(e, echo=True, work=True, cin=False):
    """Recognizable non-zero seed in the bus/echo/work regions + codec input.
    Y:$4FF = the host-written DSND master scale (Task 45): without it T6
    outputs zeros and the routing law is unobservable."""
    X = e.X
    e.Y[0x4FF] = 0x400000    # DSND master scale (global, host-written)
    pat = lambda a: (0x400000 + 0x011111 * (a % 37)) & 0xFFFFFF   # ~0.5..0.9
    for a in range(0x140, 0x260):        # all three bus bases, all modes
        X[a] = pat(a)
    if work:
        for a in range(0x260, 0x2C0):    # work/input regions — STOP before the
            X[a] = pat(a + 7)            # OS control cells X:$2C0-$2CB!
    if echo:
        for a in range(0x2CC, 0x35C):    # echo regions $2CC/$2FC/$32C
            X[a] = pat(a + 13)
    if cin:
        for a in range(0x100, 0x140):    # codec input window (X:$100/$120)
            X[a] = pat(a + 29)


def snap_seed(e):
    d = {"X": [e.X.get(a, 0) & 0xFFFFFF for a in range(WIN)],
         "Y": [e.Y.get(a, 0) & 0xFFFFFF for a in range(WIN)]}
    return d


def snap_now(e):
    d = {"X": [e.X.get(a, 0) & 0xFFFFFF for a in range(WIN)],
         "Y": [e.Y.get(a, 0) & 0xFFFFFF for a in range(WIN)]}
    return d


def diffs(seed, now):
    out = []
    for side in ("X", "Y"):
        bit = 0x1000000 if side == "Y" else 0
        s, n = seed[side], now[side]
        for a in range(DIFF_LO, DIFF_HI):
            if s[a] != n[a]:
                out.append((bit | a, n[a]))
    return out


def emit_diffs(lines, name, nd, dl):
    lines.append("%s %d" % (name, nd))
    for a, v in dl:
        lines.append("D %d %06X" % (a, v))


def run_set(tag, header, flags, ids, cin=False, knobs=(64,) * 8,
            vol=127, pan=64, seed_extra=None):
    """One full block through the real OS block loop. Returns vector text."""
    e = build_fm_track()
    setup_pages(e, ids, flags, knobs=knobs, vol=vol, pan=pan)
    seed_buses(e, cin=cin)
    if seed_extra:
        seed_extra(e)
    e.X[0xFFFFEB] = header          # HRX: host header word
    seed = snap_seed(e)
    out = []
    out.append("SET %s" % tag)
    out.append("HDR %03X" % header)
    out.append("FLAGS %d %d %d" % (flags[0], flags[1], flags[2]))
    slots = [ (i - 0x1001AF) for i in ids ]
    out.append("SLOTS %d %d %d" % tuple(slots))
    # IN line: 64 words X:$100-$13F (hex, 0 if absent)
    inv = [e.X.get(0x100 + i, 0) & 0xFFFFFF for i in range(0x40)]
    out.append("IN " + " ".join("%06X" % v for v in inv))
    # SEEDX/SEEDY
    out.append("SEEDX " + " ".join("%06X" % v for v in seed["X"]))
    out.append("SEEDY " + " ".join("%06X" % v for v in seed["Y"]))

    # ---- S1: header poll + mode tables ($0087-$00CE) ----------------------
    e.run(0x0087, end=0x00CF, max_steps=10000)
    mode_cells = [(0x2C0 + i, e.X.get(0x2C0 + i, 0) & 0xFFFFFF) for i in range(12)]
    emit_diffs(out, "MODE", len(mode_cells) + 2,
               mode_cells + [(0x1000000 | 0x123, e.Y.get(0x123, 0) & 0xFFFFFF),
                             (0x1000000 | 0x124, e.Y.get(0x124, 0) & 0xFFFFFF)])

    # ---- 3 sub-blocks ------------------------------------------------------
    sb_lines = []
    for k in range(3):
        e.Y[0x123] = MPAGE_BASES[k]     # y:$123 = $528+$100k (OS advance)
        e.Y[0x124] = k
        e.run(0x00CF, end=0x0100, max_steps=200000)    # prologue: flags->X:$2C4, INIT, CONF
        e.run(0x0100, end=0x02EC, max_steps=500000)    # frame pre (+PROC)
        e.run(0x02EC, end=0x0B4C, max_steps=500000)    # frame tail
        e.run(0x0143, end=0x0163, max_steps=100000)    # mixer (add/replace)
        now = snap_now(e)
        dl = diffs(seed, now)
        emit_diffs(sb_lines, "SB %d" % k, len(dl), dl)

    # ---- end of block ($01A3-$01E1): MIX / copy ----------------------------
    # All three paths end with jmp $0087 (the block boundary) -- run to $0087
    # so the section terminates naturally (mailbox reset always runs, the
    # flag dispatch picks MIX / copy / neither, exactly like the real OS).
    e.run(0x01A3, end=0x0087, max_steps=100000)
    now = snap_now(e)
    dl = diffs(seed, now)
    emit_diffs(out, "END", len(dl), dl)
    out += sb_lines
    return "\n".join(out) + "\n"


def main():
    vecs = []

    # ---- 1. mode tables: one set per header --------------------------------
    for h in (0x140, 0x160, 0x180):
        tag = "mode_%03X" % h
        print("== %s" % tag, flush=True)
        vecs.append(run_set(tag, h, [0, 0, 0], [FM_ID, NULL_ID, NULL_ID]))

    # ---- 2. single-bus adds (header $180) ----------------------------------
    for b in (0x001, 0x002, 0x004):
        tag = "add_b%d" % b.bit_length()
        print("== %s" % tag, flush=True)
        vecs.append(run_set(tag, 0x180, [b, b, b], [FM_ID, NULL_ID, NULL_ID]))

    # ---- 3. all-bus add + combos -------------------------------------------
    print("== add_all", flush=True)
    vecs.append(run_set("add_all", 0x180, [0x007, 0x007, 0x007],
                        [FM_ID, NULL_ID, NULL_ID]))
    print("== combo_mixpages", flush=True)
    vecs.append(run_set("combo_mixpages", 0x180, [0x003, 0x005, 0x006],
                        [FM_ID, NULL_ID, NULL_ID]))

    # ---- replace mode (bit3 + C/D/E): EXCLUDED from the sweep --------------
    # The bit3 path ($0149 brset -> $01F3-$0218) reaches func_0001e2/0001ec
    # via bsclr/bsset BRANCHES (no bsr), and both functions end with rts:
    # on real silicon that pops an empty system stack (undefined PC). The
    # host-side routing modes never set bit3, so the path is a firmware
    # vestige that must not be exercised; documented in README_ROUTING.

    # ---- MIX (bit8) at end of block --------------------------------------
    print("== mix_all_buses", flush=True)
    vecs.append(run_set("mix_all_buses", 0x180, [0x007, 0x007, 0x107],
                        [FM_ID, NULL_ID, NULL_ID]))
    print("== mix_bus1_only", flush=True)
    vecs.append(run_set("mix_bus1_only", 0x180, [0x001, 0x001, 0x101],
                        [FM_ID, NULL_ID, NULL_ID]))

    # ---- 6. copy (bitA) at end of block -------------------------------------
    print("== copy_bita", flush=True)
    vecs.append(run_set("copy_bita", 0x180, [0x000, 0x000, 0x400],
                        [FM_ID, NULL_ID, NULL_ID]))
    print("== copy_bita_echowrite", flush=True)
    vecs.append(run_set("copy_bita_echowrite", 0x160, [0x001, 0x001, 0x401],
                        [FM_ID, NULL_ID, NULL_ID]))

    # ---- 7. input paths (bits 6/7) ------------------------------------------
    for fl, nm in ((0x040, "in6"), (0x080, "in7"), (0x0C0, "in67")):
        tag = "%s_h180" % nm
        print("== %s" % tag, flush=True)
        vecs.append(run_set(tag, 0x180, [fl, fl, fl],
                            [FM_ID, NULL_ID, NULL_ID], cin=True))

    # ---- 8. FM on different slots -------------------------------------------
    print("== fm_slot1", flush=True)
    vecs.append(run_set("fm_slot1", 0x180, [0x001, 0x001, 0x001],
                        [NULL_ID, FM_ID, NULL_ID]))
    print("== fm_slot2", flush=True)
    vecs.append(run_set("fm_slot2", 0x180, [0x004, 0x004, 0x004],
                        [NULL_ID, NULL_ID, FM_ID]))

    # ---- 9. realistic layouts ------------------------------------------------
    # 6*MONO (DSP1 side): tracks 1/2/3 -> own buses
    print("== layout_6mono", flush=True)
    vecs.append(run_set("layout_6mono", 0x180, [0x001, 0x002, 0x004],
                        [FM_ID, FM_ID, FM_ID]))
    # 3*STEREO (DSP1 side): pair1 = b0+t1/b1+t2, pair2 L = b2+t3
    print("== layout_3stereo", flush=True)
    vecs.append(run_set("layout_3stereo", 0x160, [0x001, 0x002, 0x004],
                        [FM_ID, FM_ID, FM_ID]))
    # 3*STEREO + AB: pairs + MIX sum
    print("== layout_3stereo_ab", flush=True)
    vecs.append(run_set("layout_3stereo_ab", 0x140, [0x001, 0x002, 0x104],
                        [FM_ID, FM_ID, FM_ID]))
    # MIX only: everything to the main mix
    print("== layout_mix", flush=True)
    vecs.append(run_set("layout_mix", 0x180, [0x007, 0x007, 0x107],
                        [FM_ID, FM_ID, FM_ID]))

    # ---- 10. knob variation for the FM frame (mixer law on live audio) ------
    print("== knobs_live", flush=True)
    vecs.append(run_set("knobs_live", 0x180, [0x001, 0x002, 0x104],
                        [FM_ID, NULL_ID, NULL_ID],
                        knobs=(90, 10, 64, 40, 20, 100, 5, 70), vol=100, pan=90))

    outpath = "/home/z/my-project/work/pack9/vectors/routing_vectors.txt"
    with open(outpath, "w") as f:
        f.write("".join(vecs))
    nsets = sum(1 for v in vecs for l in v.splitlines() if l.startswith("SET "))
    print("wrote %s (%d sets)" % (outpath, nsets))


if __name__ == "__main__":
    main()
