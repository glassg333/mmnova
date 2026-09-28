#!/usr/bin/env python3
"""fm_knob_sweep.py v2 — FM+STAT knob->frequency laws, measured from WAVEFORMS.

Bullet-proof method (no register-semantics guessing):
  * run the bit-precise emulator, collect 8 blocks (32 samples each) of
      op1 sine  : Y:$C0-$DF (stored in the op1 loop, $145DD3)
      op2 sine  : Y:$80-$9F (stored in the op2 interp block, $145D6E/$6F)
      final out : Y:$100-$11F (machine output)
  * count sign changes -> frequency in Hz
  * ratio(op) = f_op / f_out;  also absolute f_out for the base-pitch law
  * compare with the port float core law (analytic re-implementation)
Answers the user's report: does the original's frequency really reach 0,
and where the port's floor actually sits.
"""
import sys, json
sys.path.insert(0, "/home/z/my-project/scripts")
from fm_harness import build, fm_init, fm_block, R6M, M24  # noqa

SR = 44100.0


def collect(p, A, blocks=8):
    e = build()
    fm_init(e, 8)
    for _ in range(2):
        fm_block(e, 8, p, A)
    op1, op2, out = [], [], []
    for _ in range(blocks):
        fm_block(e, 8, p, A)
        op1 += [e.Y[0xC0 + i] for i in range(32)]
        op2 += [e.Y[0x80 + i] for i in range(32)]
        out += [e.Y[0x100 + i] for i in range(32)]
    return op1, op2, out


def s24(v):
    return v - (1 << 24) if v & 0x800000 else v


def freq_hz(raw):
    w = [v - (1 << 24) if v & 0x800000 else v for v in raw]
    n = len(w)
    if n < 4:
        return 0.0
    zc = sum(1 for i in range(1, n) if (w[i - 1] < 0) != (w[i] < 0))
    # guard against noise flutter: also require some amplitude
    peak = max(abs(x) for x in w)
    if peak < 1000:      # effectively silence/DC
        return 0.0
    return zc / 2.0 / (n / SR)


def measure(knob_index, knob_val, A=2000, base=None):
    p = base if base is not None else [64, 64, 0, 0, 64, 0, 127, 64]
    p = list(p)
    p[knob_index] = knob_val
    op1, op2, out = collect(p, A)
    return freq_hz(op1), freq_hz(op2), freq_hz(out)


# ---------------- port law (MnmFm.hpp, exact float re-implementation) --------
kFmRatio = [0.03125, 0.0625, 0.125, 0.1875, 0.25, 0.3125, 0.375, 0.5,
            0.625, 0.75, 0.875, 1.0, 1.25, 1.5, 1.75, 2.0,
            2.5, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0]


def port_ratioIndex(k):
    word = min(max(k, 0.0), 127.0) * (8388607.0 / 127.0)
    n = int(((word + 32768.0) * 24.0) / 16777216.0)
    return min(max(n, 0), 23)


def port_fine(k):
    word = min(max(k, 0.0), 127.0) * (8388607.0 / 127.0)
    x1 = 4194304.0 + (word - 4194304.0) * 0.25
    return min(max(x1 / 4194304.0, 0.75), 1.25)


def main():
    A = 2000
    print("=== 1FRQ sweep: op1/op2/output frequencies in Hz (emu, A=%d) ===" % A)
    print(f"{'1FRQ':>4} {'f_op1':>9} {'f_op2':>9} {'f_out':>9} "
          f"{'r1=f1/fout':>11} {'port r1':>9} {'port r2':>9}")
    rows = []
    for k in list(range(0, 128, 8)) + [127]:
        p = [k, 64, 0, 0, 64, 0, 127, 64]
        f1, f2, fo = measure(0, k, A, base=p)
        pr1 = kFmRatio[port_ratioIndex(k)] * port_fine(64)
        pr2 = kFmRatio[port_ratioIndex(64)]
        rel1 = f1 / fo if fo else 0.0
        rel2 = f2 / fo if fo else 0.0
        rows.append(dict(knob=k, f_op1=f1, f_op2=f2, f_out=fo, r1=rel1, r2=rel2,
                         port_r1=pr1, port_r2=pr2))
        print(f"{k:>4} {f1:>9.2f} {f2:>9.2f} {fo:>9.2f} "
              f"{rel1:>11.5f} {pr1:>9.5f} {pr2:>9.5f}")

    print()
    print("=== 2FRQ sweep (1FRQ=64): op2 frequency ===")
    for k in list(range(0, 128, 8)) + [127]:
        p = [64, 64, 0, 0, k, 0, 127, 64]
        f1, f2, fo = measure(4, k, A, base=p)
        pr2 = kFmRatio[port_ratioIndex(k)]
        rel2 = f2 / fo if fo else 0.0
        print(f"2FRQ={k:>3} f_op2={f2:>8.2f} f_out={fo:>8.2f} "
              f"r2={rel2:>9.5f}  port_r2={pr2:.5f}")

    print()
    print("=== base pitch law: f_out vs A ===")
    for A2 in [1000, 2000, 4000, 8000]:
        p = [64, 64, 0, 0, 64, 0, 127, 64]
        f1, f2, fo = measure(0, 64, A2, base=p)
        print(f"A={A2:>5}  f_out={fo:>8.2f} Hz   f_out/A={fo / A2:.4f} "
              f" f_op1={f1:>8.2f} r1={f1 / fo if fo else 0:.5f}")

    json.dump(rows, open('/home/z/my-project/scripts/fm_knob_sweep_stat.json', 'w'),
              indent=1)
    print("\nsaved fm_knob_sweep_stat.json")


if __name__ == '__main__':
    main()
