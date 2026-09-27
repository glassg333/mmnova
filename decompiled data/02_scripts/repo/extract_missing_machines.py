#!/usr/bin/env python3
"""
Extract all missing Monomachine machine listings from the verified disassembly.

Source: /home/z/my-project/work/mmnova/decompiled data/dsp1_machines_fx.txt
        (already authenticated byte-exact against firmware BIN)

For each machine slot in the dispatch tables at P:$10016B (init), $10018D (config),
$1001AF (process), we slice the disassembly from the machine's init address up to
(but excluding) the next machine's init address.  This captures init+config+process
plus any private helper routines sitting between them.

We skip the slots already shipped in the previous pack (m8/m9/m10/m15) and the
unused placeholder m12.

Output: /home/z/my-project/work/extract/listings/<slot>_<name>_full.txt
"""
import os, re

ROOT = "/home/z/my-project/work/mmnova/decompiled data"
DISASM = os.path.join(ROOT, "dsp1_machines_fx.txt")
DISPATCH = os.path.join(ROOT, "dsp1_dispatch.txt")
OUT = "/home/z/my-project/work/extract/listings"
os.makedirs(OUT, exist_ok=True)

LINE_RE = re.compile(r"^([0-9a-f]{6}):\s+(.*)$")

# Parse the disassembly into an ordered list of (addr_int, raw_line)
def parse_disasm(path):
    rows = []
    for line in open(path, encoding="utf-8", errors="replace"):
        line = line.rstrip("\n")
        m = LINE_RE.match(line)
        if not m:
            continue
        try:
            addr = int(m.group(1), 16)
        except ValueError:
            continue
        rows.append((addr, line))
    rows.sort(key=lambda r: r[0])
    return rows

# Parse the three dispatch tables from dsp1_dispatch.txt
def parse_dispatch(path):
    """Return dict {slot_idx -> (init, config, process)} for slots 1..34."""
    text = open(path, encoding="utf-8", errors="replace").read()
    # The dispatch entries are stored as comments like:
    #   10016b: tfr  x1,b  ...  ; 144cc9
    # The 'real' instruction is `tfr x1,b` (placeholder) and the comment
    # is the machine routine address.  The same line also has y0,y:(r1)-n1
    # pattern but that's just noise.
    # We grab words $10016B..$10018C (init, 34 entries), $10018D..$1001AE
    # (config), $1001AF..$1001D0 (process).
    lines = {}
    for line in text.splitlines():
        m = LINE_RE.match(line)
        if not m:
            continue
        try:
            addr = int(m.group(1), 16)
        except ValueError:
            continue
        lines[addr] = line
    def grab(base, n):
        out = []
        for i in range(n):
            ln = lines.get(base + i)
            if ln is None:
                out.append(None)
                continue
            # comment after last ';'
            if ";" in ln:
                addr_str = ln.rsplit(";", 1)[1].strip()
                # strip "(callers: ...)" / "(bits: ...)" parenthesised notes
                addr_str = re.sub(r"\([^)]*\)", "", addr_str).strip()
                try:
                    out.append(int(addr_str.split()[0], 16))
                    continue
                except (ValueError, IndexError):
                    pass
            out.append(None)
        return out
    # The dispatch tables at P:$10016B/$10018D/$1001AF each have 34 entries,
    # but entry [0] of every table is a "no machine" placeholder ($100169).
    # Real machine slots m1..m21 are at indices [1..21] of each table
    # (i.e. addresses $10016C, $10018E, $1001B0 onwards).
    inits   = grab(0x10016B, 35)
    configs = grab(0x10018D, 35)
    procs   = grab(0x1001AF, 35)
    return inits, configs, procs

# Slot names from REPORT.md (machine map)
NAMES = {
    1:  "GND-SIN",
    2:  "GND-NOIS",
    3:  "SID",
    4:  "SWAVE-SAW",
    5:  "SWAVE-PULS",
    6:  "DPRO-WAVE",
    7:  "DPRO-BBOX",
    8:  "FM-STAT",
    9:  "FM-PAR",
    10: "FM-DYN",
    11: "VO-6",
    12: "UNUSED",
    13: "FX-REV",
    14: "FX-EQ",     # placeholder name, to be confirmed by disasm
    15: "FX-CHO",
    16: "FX-DYN",
    17: "FX-RING",
    18: "FX-PHA",
    19: "FX-FLA",
    20: "FX-DLY",
    21: "FX-EXT",    # placeholder name, to be confirmed by disasm
}

# Already shipped in the previous pack — skip
SHIPPED = {8, 9, 10, 15}

def slice_listing(rows, start_addr, end_addr):
    out = []
    for addr, line in rows:
        if addr < start_addr:
            continue
        if end_addr is not None and addr >= end_addr:
            break
        out.append(line)
    return out

def main():
    rows = parse_disasm(DISASM)
    print(f"parsed {len(rows)} disasm rows, range ${rows[0][0]:06x}..${rows[-1][0]:06x}")
    inits, configs, procs = parse_dispatch(DISPATCH)
    print(f"dispatch: {sum(1 for x in inits if x)} init, {sum(1 for x in configs if x)} config, {sum(1 for x in procs if x)} process entries")

    # Build the ordered list of machine init addresses so we can find the
    # boundary of each slice (start of NEXT machine init).
    machine_starts = sorted({a for a in inits if a and a != 0x100169})
    print(f"machine starts: {['%06x'%a for a in machine_starts]}")

    manifest = []
    for slot in range(1, 35):
        if slot in SHIPPED:
            continue
        # Skip unused slot 12 (placeholder)
        if slot == 12:
            continue
        # inits/configs/procs arrays are now indexed by *table* position 0..34,
        # where position 0 is the "no machine" placeholder.  Real machine
        # slot N lives at table position N.
        ini = inits[slot]
        cfg = configs[slot]
        prc = procs[slot]
        if ini is None or ini == 0x100169:
            continue
        # Find next machine start after this one (boundary = end exclusive)
        next_start = None
        for a in machine_starts:
            if a > ini:
                next_start = a
                break
        # If this is the very last machine, slice to the end of the disasm
        if next_start is None:
            next_start = rows[-1][0] + 1
        body = slice_listing(rows, ini, next_start)
        name = NAMES.get(slot, f"m{slot}")
        fn = f"{slot:02d}_{name.replace('+','p').replace(' ','_')}_full.txt"
        path = os.path.join(OUT, fn)
        with open(path, "w", encoding="utf-8") as f:
            f.write(f"# slot m{slot} {name}\n")
            f.write(f"# init   = ${ini:06x}\n")
            f.write(f"# config = ${cfg:06x}\n" if cfg else "# config = (none)\n")
            f.write(f"# proc   = ${prc:06x}\n" if prc else "# proc   = (none)\n")
            cfg_str = f"${cfg:06x}" if cfg else "none"
            prc_str = f"${prc:06x}" if prc else "none"
            f.write(f"# slice  = ${ini:06x}..${next_start-1:06x}  ({len(body)} words)\n")
            f.write("\n")
            for line in body:
                f.write(line + "\n")
        manifest.append((slot, name, ini, cfg, prc, len(body), fn))
        print(f"  m{slot:02d} {name:10s}  init=${ini:06x} cfg={cfg_str} proc={prc_str}  -> {fn} ({len(body)} lines)")

    # write manifest
    with open(os.path.join(OUT, "MANIFEST.txt"), "w", encoding="utf-8") as f:
        f.write("slot\tname\tinit\tconfig\tprocess\tlines\tfile\n")
        for slot, name, ini, cfg, prc, n, fn in manifest:
            cfg_str = f"${cfg:06x}" if cfg else "none"
            prc_str = f"${prc:06x}" if prc else "none"
            f.write(f"m{slot}\t{name}\t${ini:06x}\t{cfg_str}\t{prc_str}\t{n}\t{fn}\n")
    print(f"\nmanifest written: {os.path.join(OUT, 'MANIFEST.txt')}")

if __name__ == "__main__":
    main()
