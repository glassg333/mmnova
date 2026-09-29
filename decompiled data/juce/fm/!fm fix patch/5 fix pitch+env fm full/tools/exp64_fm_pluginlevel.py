#!/usr/bin/env python3
"""exp64_fm_pluginlevel.py — ONE batch experiment (rule 18) closing the
plugin-level FM: pitch law (A at PROC entry), AMP envelope state machine,
gain ring (VOL^2 * pan sin/cos, 16-step ramp), machine output words.

Runs the REAL track frame of OS 1.32 with machine 8 (FM STAT) dispatched,
captures per frame:
  A     register A at PROC entry $145D21 (kernel pitch chain result)
  conf  CONF invocation count at $145D1D
  lvl   Y[P+$D7] envelope level (signed 24-bit)
  ph    X[P+$D8] envelope phase (0..4)
  cnt   Y[P+$D8] hold counter
  ring  Y:$20-$3F gain ring (32 words, interleaved sin/cos ramped pairs)
  prev  X[P+$FA], Y[P+$FA] previous frame targets (sin, cos)
  mach  Y:$100-$121 machine output (34 words)

Sets:
  A-law grid:   w41 x TUNE x w30 (pitch mod)            -> 36 sets x 4 frames
  ENV grid:     ATK/HOLD/DEC/REL x gate sequences       -> 24 sets x 16 frames
  VOL/PAN grid: VOL x PAN                               -> 12 sets x 8 frames
"""
import sys, json, gzip
sys.path.insert(0, "/home/z/my-project/scripts")
from dsp_emu import DSP56300
from track_harness import disasm, PM, R6
from amp_env_measure import inject_sine_tables, load_xy_memory, P

PROC_ENTRY = 0x145D21   # FM STAT PROC
CONF_ENTRY = 0x145D1D   # FM STAT CONF
# kernel dispatch reads pointers at x:(r0 + table_base), r0 = y:$120 =
# 0x1001AF + machine  =>  mirrored addresses beyond the dump:
MIR_INIT = 0x1001AF + 0x10016B   # 0x20031A
MIR_CONF = 0x1001AF + 0x10018D   # 0x20033C
MIR_PROC = 0x1001AF + 0x1001AF   # 0x20035E


def build_fm_track():
    lines = disasm(0x0000, 0x0B4E)
    lines += disasm(0x144000, 0x2000)
    e = DSP56300(lines)
    data = open(PM, "rb").read()
    n = len(data) // 3
    for i in range(0x100000, min(n, 0x160000)):
        o = i * 3
        w = (data[o] << 16) | (data[o + 1] << 8) | data[o + 2]
        e.X[i] = w
        e.Y[i] = w
    inject_sine_tables(e)
    load_xy_memory(e)
    for a in range(0x2C0, 0x2CA):
        e.X[a] = 0
    e.X[0xFF] = 0
    # dispatch tracks 0/1/2 -> machine 8 (FM STAT); the kernel reads the
    # INIT/CONF/PROC pointers at the mirrored addresses (beyond the dump)
    slot = 0x1001AF + 8
    e.Y[0x120] = slot       # track 0 -> FM STAT
    e.Y[0x121] = 0x1001AF   # tracks 1/2 -> null machine (otherwise the same
    e.Y[0x122] = 0x1001AF   # PROC runs 3x per frame and the captured output
                            # belongs to the last continuation)
    e.X[MIR_PROC + 8] = PROC_ENTRY
    e.X[MIR_CONF + 8] = CONF_ENTRY
    e.X[MIR_INIT + 8] = 0x145D12   # FM STAT INIT
    # null-machine pointers -> an rts ($145D1C, parsed from the FM INIT block)
    e.X[MIR_PROC + 0] = 0x145D1C
    e.X[MIR_CONF + 0] = 0x145D1C
    e.X[MIR_INIT + 0] = 0x145D1C
    return e


def setup_page(e, atk=0, hold=0, dec=127, rel=0, vol=127, pan=64,
               tempo=120, w41=11776, knobs=(64,) * 8, tune_raw=None,
               w30=64, tick=2880):
    Y, X = e.Y, e.X
    for i in range(0x100):
        Y[P + i] = 0
        X[P + i] = 0
    Y[P + 0x00] = (atk & 0xFFFF) << 16
    Y[P + 0x01] = (hold & 0xFFFF) << 16
    Y[P + 0x02] = (dec & 0xFFFF) << 16
    Y[P + 0x03] = (rel & 0xFFFF) << 16
    Y[P + 0x05] = (vol & 0xFFFF) << 16
    Y[P + 0x06] = (pan & 0xFFFF) << 16
    Y[P + 0x1E] = (w30 & 0xFFFF) << 16      # pitch mod word (rest = 64)
    Y[P + 0x23] = tempo                     # TEMPO word (harness writes BPM raw)
    Y[P + 0x29] = w41                       # pitch word (note<<11)/12
    Y[P + 0x2A] = tick                      # tick word (24*BPM)
    # glide state = target (PORT=0, settled; as OS after note-on)
    X[P + 0x06] = w41
    X[P + 0x07] = w41
    Y[P + 0x07] = w41
    X[P + 0x08] = w41
    X[P + 0x09] = 0
    X[P + 0x02] = w41
    X[P + 0x03] = 0
    # FM STAT knobs at P+$2C..$33 (raw<<16); TUNE = knob 8 (P+$33)
    for i, v in enumerate(knobs):
        Y[P + 0x2C + i] = (v & 0xFFFF) << 16
    if tune_raw is not None:
        Y[P + 0x33] = (tune_raw & 0xFFFF) << 16
    Y[P + 0x28] = 0                         # trig
    Y[0x120] = 0x1001AF + 8
    Y[0x121] = 0x1001AF + 8
    Y[0x122] = 0x1001AF + 8
    Y[0x123] = P + 0x28                     # frame prologue $0108: r6 = y:$123
    Y[0x124] = 0                            # $0100: r1 = y:$124


def sgn(v):
    return v - 0x1000000 if v >= 0x800000 else v


def run_set(seq, **kw):
    """seq: list of per-frame trig values. Returns list of frame records.

    NOTE: the machine is executed by the jsr (r1) at $02EB — the first run
    must END at $02EC (exclusive) so $02EB (and thus PROC) executes."""
    e = build_fm_track()
    setup_page(e, **kw)
    # machine INIT: the kernel runs it on machine change/trigger; the page
    # wipe erases the machine state cells, so run the firmware INIT once to
    # start every set from a fresh machine (same as the fm_harness contract).
    e.R[6] = P + 0x28
    e.R[7] = 0x100
    for r in range(8):
        e.M[r] = 0xFFFFFF
    e.A = 0
    e.ret_stack.append(0xDEAD)
    e.run(0x145D12, end=0x145D1C, max_steps=100000)
    if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
        e.ret_stack.pop()
    rows = []
    for f, trig in enumerate(seq):
        # CONF per frame: in the live OS the CONF dispatch fires when params
        # change; with static knobs it is idempotent ($2A := $7FFFFF seed +
        # knob latch), so CONF-per-frame == the live behavior after the first
        # knob event. The kernel in this harness never flags it (X:$2C0 flags
        # are wiped), so we run the 4-word CONF explicitly, fm_harness-style.
        e.R[6] = P + 0x28
        e.R[7] = 0x100
        for r in range(8):
            e.M[r] = 0xFFFFFF
        e.ret_stack.append(0xDEAD)
        e.run(0x145D1D, end=0x145D20, max_steps=100000)
        if e.ret_stack and e.ret_stack[-1] == 0xDEAD:
            e.ret_stack.pop()
        e.Y[P + 0x28] = trig
        hits = {"a": None, "conf": 0}

        def hook(pc, emu, hits=hits):
            if pc == PROC_ENTRY:
                # the kernel's frame code leaves modulator addressing in the
                # M regs; the machine contract requires linear (fm_harness
                # resets them before every PROC). Also pin r6/r7.
                for r in range(8):
                    emu.M[r] = 0xFFFFFF
                emu.R[6] = P + 0x28
                emu.R[7] = 0x100
                if hits["a"] is None:
                    hits["a"] = emu.A & 0xFFFFFFFFFFFF   # full a1:a0
            elif pc == CONF_ENTRY:
                hits["conf"] += 1
        e.run(0x0100, end=0x02EC, max_steps=500000, hook=hook)
        e.X[0x2C9] = 0x300
        e.run(0x02EC, end=0x0B4C, max_steps=500000, hook=hook)
        rows.append(dict(
            f=f, trig=trig, A=hits["a"], conf=hits["conf"],
            lvl=sgn(e.Y[P + 0xD7]), ph=e.X[P + 0xD8], cnt=e.Y[P + 0xD8],
            ring=[sgn(e.Y[i]) for i in range(0x20)],
            prevX=sgn(e.X[P + 0xFA]), prevY=sgn(e.Y[P + 0xFA]),
            mach=[sgn(e.Y[0x100 + i]) for i in range(0x22)],
        ))
    return rows


def main():
    out = {"a_law": [], "env": [], "volpan": []}

    # ---- 1. pitch law grid (A at PROC entry) -------------------------------
    print("=== A-law grid ===", flush=True)
    for w41 in (0, 10240, 11776, 21674):
        for tune in (0, 1, 32, 64, 96, 127):
            for w30 in (64, 0, 127):
                kw = dict(w41=w41, tune_raw=tune, w30=w30)
                rows = run_set([1, 0, 0], **kw)
                out["a_law"].append(dict(w41=w41, tune=tune, w30=w30,
                                         A=[r["A"] >> 24 for r in rows],
                                         A48=[r["A"] for r in rows]))
                print("  w41=%5d tune=%3d w30=%3d -> A1=%s" % (
                    w41, tune, w30, [r["A"] >> 24 for r in rows]), flush=True)

    # ---- 2. envelope grid ----------------------------------------------------
    print("=== ENV grid ===", flush=True)
    seq_gate = [1] + [0] * 7 + [2] + [0] * 3 + [1] + [0] * 3     # 16f
    seq_kill = [1] + [0] * 5 + [3] + [0] * 9                     # 16f
    seq_hold = [1] + [0] * 71                                    # 72f
    n = 0
    for atk, hold, dec, rel, seq, tag in (
            (0, 0, 90, 40, seq_gate, "gate"),
            (32, 0, 90, 40, seq_gate, "gate"),
            (64, 0, 90, 40, seq_gate, "gate"),
            (0, 0, 0, 40, seq_gate, "gate"),
            (0, 0, 127, 40, seq_gate, "gate"),
            (0, 0, 90, 40, seq_kill, "kill"),
            (0, 0, 0, 0, seq_kill, "kill"),
            (0, 32, 90, 40, seq_hold, "hold")):
        rows = run_set(seq, atk=atk, hold=hold, dec=dec, rel=rel)
        out["env"].append(dict(atk=atk, hold=hold, dec=dec, rel=rel,
                               tag=tag, frames=rows))
        n += 1
        print("  set %d (atk=%d hold=%d dec=%d rel=%d %s) done" % (
            n, atk, hold, dec, rel, tag), flush=True)

    # ---- 3. VOL/PAN grid (steady gate, ATK=0) --------------------------------
    print("=== VOL/PAN grid ===", flush=True)
    for vol in (0, 64, 127):
        for pan in (0, 32, 64, 127):
            rows = run_set([1] + [0] * 3, atk=0, dec=127, rel=0,
                           vol=vol, pan=pan)
            out["volpan"].append(dict(vol=vol, pan=pan, frames=rows))
            print("  vol=%3d pan=%3d ring@f3=%s" % (
                vol, pan, rows[3]["ring"][:6]), flush=True)

    with gzip.open("/home/z/my-project/work/exp64_fm_pluginlevel.json.gz", "wb") as f:
        f.write(json.dumps(out).encode())
    print("saved work/exp64_fm_pluginlevel.json.gz", flush=True)


if __name__ == "__main__":
    main()
