#!/usr/bin/env python3
"""trace_param_reads.py — абсолютные адреса чтений параметров в секциях
$0985/$0A22/$0A5D/$0A84/$0AB7/$0ABF/$0B1E (r6-база: P или P+$28?)."""
import sys
sys.path.insert(0, "/home/z/my-project/scripts")
from amp_env_measure import EnvCase

P = 0x400
WATCH_PCS = {0x0985, 0x0A22, 0x0A5D, 0x0A84, 0x0A85, 0x0AB7, 0x0ABF, 0x0B1E,
             0x056D, 0x0578, 0x08FA, 0x07A6, 0x079F}

def main():
    c = EnvCase(bofs=0, wofs=0, filt_atk=64, filt_dec=64)
    e = c.e
    e.Y[P + 0x05] = (127 & 0xFFFF) << 16
    e.Y[P + 0x06] = (64 & 0xFFFF) << 16
    e.Y[P + 0x10] = (64 & 0xFFFF) << 16
    e.Y[P + 0x11] = (64 & 0xFFFF) << 16
    e.Y[P + 0x12] = (64 & 0xFFFF) << 16
    e.Y[P + 0x13] = (64 & 0xFFFF) << 16
    _rd = e.rd
    log = []
    def rd_hook(space, ea, _rd=_rd):
        v = _rd(space, ea)
        if e.pc in WATCH_PCS:
            log.append((e.pc, space, ea, v))
        return v
    e.rd = rd_hook
    for k in range(3):
        c.frame(trig=1 if k == 0 else 0)
    seen = {}
    for pc, sp, ea, v in log:
        seen.setdefault((pc, sp, ea), 0)
        seen[(pc, sp, ea)] += 1
    print("Абсолютные адреса чтений (pc, space, ea, count):")
    for (pc, sp, ea), n in sorted(seen.items()):
        rel = ea - P if P <= ea < P + 0x100 else None
        rels = "  -> P+$%02X" % rel if rel is not None and rel >= 0 else ""
        print("  pc=$%04X %s:$%04X n=%d%s" % (pc, sp.upper(), ea, n, rels))

if __name__ == "__main__":
    main()
