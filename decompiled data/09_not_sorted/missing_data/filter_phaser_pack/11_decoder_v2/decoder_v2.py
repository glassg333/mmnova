#!/usr/bin/env python3
"""
decoder_v2.py — Iteration 18 decoding-system upgrade for mmnova firmware mining.

  PASS 1  light decode of EVERY word in dsp1_pmem.bin (valid, mnemonic id,
          ext count) -> numpy arrays
  PASS 2  recursive code discovery with HARD REGION CAPS (kernel listing bound,
          machine dispatch entries sequential caps) + exact rendering (dis56300)
  PASS 3  xref map: call/jump/branch/do_end/mem_x/mem_y/imm_rN/imm
  PASS 4  data scan: pointer tables, numeric classification, coverage report

Outputs to /home/z/my-project/mining/decoder_v2/
"""
import os, sys, json, re, gzip, time
import numpy as np

DIS_DIR = "/home/z/my-project/work/mmnova/decompiled data/07_scripts/DSP56300 disassemblerStandalone/scripts"
REF_DIR = "/home/z/my-project/work/mmnova/decompiled data/07_scripts/DSP56300 disassemblerStandalone/reference"
PMEM = "/home/z/my-project/work/mmnova/decompiled data/02_memory_images/dsp1_pmem.bin"
LIST = "/home/z/my-project/mining/mmnova/decompiled data/03_listings/dispatch"
OUT = "/home/z/my-project/mining/decoder_v2"
os.makedirs(OUT, exist_ok=True)

sys.path.insert(0, DIS_DIR)
import dis56300

t0 = time.time()
def log(msg):
    print("[%7.1fs] %s" % (time.time() - t0, msg), flush=True)

raw = open(PMEM, "rb").read()
N = len(raw) // 3
bb = np.frombuffer(raw[: N * 3], dtype=np.uint8).reshape(-1, 3).astype(np.uint32)
WORDS = (bb[:, 0] << 16) | (bb[:, 1] << 8) | bb[:, 2]
log("pmem loaded: %d words ($0-$%x)" % (N, N - 1))

templates = json.load(open(os.path.join(REF_DIR, "opcode_templates.json")))
tpl_int = {int(k, 16): v for k, v in templates.items()}
MNEMS = {}
log("templates: %d" % len(templates))

# ---------------------------------------------------------------- PASS 1
valid = np.zeros(N, dtype=np.uint8)
extcnt = np.zeros(N, dtype=np.uint8)
mnemid = np.zeros(N, dtype=np.uint16)
for i, w in enumerate(WORDS.tolist()):
    e = tpl_int.get(w)
    if e is None:
        continue
    mnem = next(iter(e))
    mid = MNEMS.setdefault(mnem, len(MNEMS))
    valid[i] = 1
    mnemid[i] = mid
    extcnt[i] = e[mnem]["ext_count"]
log("PASS1: %d valid starts, %d mnemonics" % (int(valid.sum()), len(MNEMS)))

# ---------------------------------------------------------------- PASS 2
cflag = np.zeros(N, dtype=np.uint8)

def listing_extent(path):
    """[min_addr, max_addr] of addresses in a reference listing."""
    lo, hi = None, None
    for line in open(path, errors="ignore"):
        m = re.match(r"^\s*([0-9A-Fa-f]{5,6}):", line)
        if m:
            a = int(m.group(1), 16)
            lo = a if lo is None else min(lo, a)
            hi = a if hi is None else max(hi, a)
    return [lo, hi]

# hard caps from verified listings
kernel_lo, kernel_hi = listing_extent(os.path.join(LIST, "dsp1_kernel_P0000-0B4D.txt"))
monitor_lo, monitor_hi = listing_extent(os.path.join(LIST, "dsp1_monitor_P100000.txt"))
log("kernel extent $%x-$%x, monitor extent $%x-$%x" % (kernel_lo, kernel_hi, monitor_lo, monitor_hi))

# dispatch entries (machines only; tables live at $10016B/$10018D/$1001AF)
MACH_LO = 0x144CC9
disp_entries = set()
for base in (0x10016B, 0x10018D, 0x1001AF):
    for i in range(96):
        v = int(WORDS[base + i])
        if MACH_LO <= v < N:
            disp_entries.add(v)
disp_sorted = sorted(disp_entries)
log("dispatch entries: %d" % len(disp_sorted))

# code can ONLY live in three ranges (kernel internal PRAM, monitor, machines);
# everything else in the image = tables / SRAM data
ALLOWED = [(0x0000, kernel_hi + 1), (0x100000, 0x10016B), (MACH_LO, N)]

def in_allowed(a):
    return any(lo <= a < hi for lo, hi in ALLOWED)

# entry -> cap: machines sequential; kernel/monitor capped by map
caps = {}
for i, a in enumerate(disp_sorted):
    nxt = disp_sorted[i + 1] if i + 1 < len(disp_sorted) else min(a + 24000, N)
    caps[a] = min(nxt, a + 24000)
caps[0x0000] = kernel_hi + 1
caps[0x100000] = 0x10016B
caps[0x100169] = 0x10016B

entries = set([0x0000, 0x100000]) | disp_entries
if int(WORDS[0x100169]) != 0 or True:
    entries.add(0x100169)                      # no-op machine stub
log("entries: %d" % len(entries))

FUNC_RE = re.compile(r"\bfunc_([0-9a-f]{6})")
INT_RE = re.compile(r"\bint_([0-9a-f]{6})")
HEX_RE = re.compile(r"\$([0-9a-f]{1,6})\b")
DO_RE = re.compile(r">\$(?:[0-9a-f]{1,6})\b")
XY_RE = re.compile(r"\b([xy]):>\$([0-9a-f]{1,6})\b")
IMM_RE = re.compile(r"#>\$([0-9a-f]{1,6})\b")
SHORTXY_RE = re.compile(r"\b([xy]):\$([0-9a-fA-F]{1,2})\b(?![0-9a-fA-F])")
DISP_RE = re.compile(r"\((?:r|n)([0-7])\+\$([0-9a-f]{4,6})\)")   # (rN+$XXXX) register-displacement TABLE indexing

xref = {}
code_text = {}
indirect_sites = []
callgraph = []
regions = []
suspicious = []

def add_xref(src, target, kind):
    xref.setdefault("%06x" % target, []).append(["%06x" % src, kind])

def resolve_tgt(ops_s, regexes):
    for r in regexes:
        m = r.search(ops_s)
        if m:
            return int(m.group(1), 16)
    return None

work = []
cur = [None, None, 0]  # region start, region last, capped?
end_sites = []         # flow-terminator instruction addresses (rts/jmp)

def close_region():
    if cur[0] is not None:
        if cur[1] + 1 <= cur[0]:
            import traceback
            print("!!! bad region", hex(cur[0]), hex(cur[1]), flush=True)
            traceback.print_stack()
            sys.exit(2)
        regions.append([cur[0], cur[1] + 1, cur[2]])
    cur[0] = None

def walk_from(a):
    """One recursive-descent linear walk from address a. Returns True if walked."""
    if a >= N or cflag[a] != 0 or not in_allowed(a):
        return False
    import bisect
    i = bisect.bisect_right(disp_sorted, a)
    def_cap = disp_sorted[i] if i < len(disp_sorted) else min(a + 24000, N)
    if a < kernel_hi + 1:
        def_cap = kernel_hi + 1
    elif 0x100000 <= a < 0x10016B:
        def_cap = 0x10016B
    cap = caps.get(a, def_cap)
    walked = False
    while a < N:
        if cflag[a] != 0:
            close_region()
            break
        if a >= cap:
            suspicious.append(a)
            close_region()
            break
        if not valid[a]:
            close_region()
            break
        ec = int(extcnt[a])
        nxt, line = dis56300.disassemble_one(raw, a, templates)
        if line is None:
            close_region()
            break
        cflag[a] = 1
        for k in range(1, ec + 1):
            if a + k < N:
                cflag[a + k] = 2
        code_text[a] = line
        walked = True
        if cur[0] is None:
            cur[0] = a
        cur[1] = a
        parts = line.split(":", 1)[1].split(None, 1)
        mnem_s = parts[0] if parts else ""
        ops_s = parts[1].strip() if len(parts) > 1 else ""
        stop = False
        # --- flow classification
        is_call = mnem_s.startswith("js") or mnem_s.startswith("bsr")
        is_jump = mnem_s.startswith("jmp") or (mnem_s.startswith("j") and not (mnem_s.startswith("js") or mnem_s.startswith("jclr") or mnem_s.startswith("jset")))
        is_br = (mnem_s.startswith("b") and mnem_s not in ("bset", "bclr", "btst", "brk")) or mnem_s in ("jclr", "jset")
        is_do = mnem_s.startswith("do")
        is_ret = mnem_s in ("rts", "rti", "end", "stop", "wait", "illegal", "debug", "swi")
        if is_call:
            tgt = resolve_tgt(ops_s, [FUNC_RE, INT_RE])
            if tgt is not None:
                add_xref(a, tgt, "call")
                callgraph.append((a, tgt))
                push(tgt)
            elif "(" in ops_s:
                indirect_sites.append(a)
        elif is_jump:
            tgt = resolve_tgt(ops_s, [FUNC_RE, INT_RE, HEX_RE])
            if tgt is not None and "(" not in ops_s:
                add_xref(a, tgt, "jump")
                push(tgt)
                stop = True
            else:
                indirect_sites.append(a)
                stop = True
        elif is_br:
            tgt = resolve_tgt(ops_s, [FUNC_RE, INT_RE, HEX_RE])
            if tgt is not None:
                add_xref(a, tgt, "branch")
                push(tgt)
        elif is_do:
            m = DO_RE.search(ops_s)
            if m:
                end_body = int(m.group(0)[2:], 16)          # printed = last body addr
                after = end_body + 1
                add_xref(a, after, "do_end")
                push(after)
        if is_ret:
            stop = True
        for mm in XY_RE.finditer(line):
            add_xref(a, int(mm.group(2), 16), "mem_" + mm.group(1))
        for mm in SHORTXY_RE.finditer(line):
            add_xref(a, int(mm.group(2), 16), "memS_" + mm.group(1))
        for mm in DISP_RE.finditer(line):
            v = int(mm.group(2), 16)
            if v >= 0x1000:
                add_xref(a, v, "disp_r" + mm.group(1))   # table base in displacement
        m_imm = IMM_RE.search(line)
        if m_imm:
            v = int(m_imm.group(1), 16)
            if v >= 0x1000:
                dest = ops_s.split(",")[-1].strip() if "," in ops_s else ""
                kind = "imm_rN" if re.fullmatch(r"r[0-7]", dest) else "imm"
                add_xref(a, v, kind)
        if stop:
            end_sites.append(a)
            close_region()
            break
        a = a + 1 + ec
    return walked

def push(tgt):
    if tgt is not None and tgt < N and cflag[tgt] == 0 and in_allowed(tgt) and tgt not in work:
        work.append(tgt)

work = list(entries)
while work:
    walk_from(work.pop())
    while work:
        walk_from(work.pop())

# multi-round seeding: the word after every flow terminator may start a block
# reachable only via indirect jumps (machine-level dispatch)
for rnd in range(8):
    seeds = []
    for ea in end_sites:
        s = ea + 1 + int(extcnt[ea])
        if s < N and cflag[s] == 0 and in_allowed(s) and valid[s]:
            seeds.append(s)
    added = 0
    for s in sorted(set(seeds)):
        while work:
            walk_from(work.pop())
        if walk_from(s):
            added += 1
        while work:
            walk_from(work.pop())
    log("seed round %d: %d seed sites, +%d new blocks" % (rnd, len(set(seeds)), added))
    if added == 0:
        break
log("PASS2: %d code starts, %d regions, %d xref targets, %d indirect, %d cap-hits"
    % (int((cflag == 1).sum()), len(regions), len(xref), len(indirect_sites), len(suspicious)))
# sanity: phaser machine = init+conf+proc regions must total 437 words
ph = sum(e - s for s, e, c in regions if 0x144E81 <= s < 0x145036)
log("sanity phaser total %d words %s" % (ph, "OK" if ph == 437 else "MISMATCH"))

# QUALITY METRIC (вычисляется после PASS 4, см. ниже — нужны таблицы)
listing_files = {
    "kernel": (os.path.join(LIST, "dsp1_kernel_P0000-0B4D.txt"), 0, kernel_hi + 1),
    "machines_fx": (os.path.join(LIST, "dsp1_machines_fx.txt"), 0x144CC9, N),
}
ref_sets = {}
for lname, (lpath, lo, hi) in listing_files.items():
    if not os.path.exists(lpath):
        continue
    ref = set()
    for line in open(lpath, errors="ignore"):
        m = re.match(r"^\s*([0-9A-Fa-f]{5,6}):", line)
        if m:
            ref.add(int(m.group(1), 16))
    ref_sets[lname] = (ref, lo, hi)
miss_other_report = {}

# ---------------------------------------------------------------- functions
func_in = {}
for s, d in callgraph:
    func_in[d] = func_in.get(d, 0) + 1
fn_regions = {}
for d in func_in:
    for s, e, c in regions:
        if s <= d < e:
            fn_regions[d] = [s, e]
            break
functions = {
    "%06x" % d: {
        "calls_in": func_in[d],
        "region": ["%06x" % fn_regions[d][0], "%06x" % fn_regions[d][1]] if d in fn_regions else None,
    }
    for d in sorted(func_in)
}

# ---------------------------------------------------------------- PASS 4
code_mask = cflag == 1
cont_mask = cflag == 2
data_mask = ~(code_mask | cont_mask)
W = WORDS

ptr_ok = data_mask & (W >= 0x1000) & (W <= N - 8)
pidx = np.flatnonzero(ptr_ok)
ptr_tables = []
if len(pidx):
    gaps = np.flatnonzero(np.diff(pidx) != 1)
    st = np.r_[0, gaps + 1]
    en = np.r_[gaps, len(pidx) - 1]
    for s, e in zip(st, en):
        run_a = pidx[s:e + 1]
        if len(run_a) >= 6:
            ptr_tables.append((int(run_a[0]), int(run_a[-1]) + 1))
log("pointer-run candidates: %d" % len(ptr_tables))

data_idx = np.flatnonzero(data_mask)
gaps = np.flatnonzero(np.diff(data_idx) != 1)
st = np.r_[0, gaps + 1]
en = np.r_[gaps, len(data_idx) - 1]
tables = []
for s, e in zip(st, en):
    run_a = data_idx[s:e + 1]
    L = len(run_a)
    if L < 16:
        continue
    vals = W[run_a]
    if bool((vals == 0).all()):
        typ, note = "zero_pad", ""
    else:
        d = np.diff(vals.astype(np.int64))
        lo, hi = int(vals.min()), int(vals.max())
        p2 = int(((vals & (vals - 1)) == 0).sum())
        if bool((d == 0).all()):
            typ, note = "constant", "v=$%06x" % lo
        elif bool((d > 0).all()):
            typ, note = "ramp_up", "d=[%d..%d]" % (int(d.min()), int(d.max()))
        elif bool((d < 0).all()):
            typ, note = "ramp_down", "d=[%d..%d]" % (int(d.min()), int(d.max()))
        elif bool((d >= 0).all() and (d > 0).any()):
            typ, note = "ramp_up_ns", "d=[%d..%d]" % (int(d.min()), int(d.max()))
        elif bool((d <= 0).all() and (d < 0).any()):
            typ, note = "ramp_down_ns", "d=[%d..%d]" % (int(d.min()), int(d.max()))
        else:
            typ = "curve"
            note = "min=$%06x max=$%06x p2=%d/%d" % (lo, hi, p2, L)
    tables.append({"addr": int(run_a[0]), "len": L, "type": typ, "note": note,
                   "head": ["%06x" % int(v) for v in vals[:8]]})
log("numeric runs >=16: %d" % len(tables))

def refs_to(tgt_addr):
    out = []
    for k in ("imm_rN", "imm", "mem_x", "mem_y", "disp_r0", "disp_r1",
              "disp_r2", "disp_r3", "disp_r4", "disp_r5", "disp_r6", "disp_r7"):
        for src, _ in xref.get("%06x" % tgt_addr, []):
            if not out or out[-1] != src:
                out.append(src)
    return out[:8]

ptr_tables_json = [{
    "addr": a0, "len": a1 - a0,
    "targets": ["%06x" % int(v) for v in W[a0:a1]],
    "refs": refs_to(a0),
    "code_targets": int(code_mask[W[a0:a1]].sum()),
} for a0, a1 in ptr_tables]

tables_json = []
for t in tables:
    r = refs_to(t["addr"])
    if t["type"] != "zero_pad" or r:
        t["refs"] = r
        tables_json.append(t)

# ---- QUALITY METRIC (после построения таблиц)
miss_other_report = {}
tbl_ranges = [(t["addr"], t["addr"] + t["len"]) for t in tables_json]
for lname, (ref, lo, hi) in ref_sets.items():
    ref_r = {a for a in ref if lo <= a < hi}
    mine_r = {a for a in code_text if lo <= a < hi}
    miss = sorted(ref_r - mine_r)
    miss_data = sum(1 for a in miss if any(l <= a < h for l, h in tbl_ranges))
    miss_other = [a for a in miss if not any(l <= a < h for l, h in tbl_ranges)]
    miss_other_report[lname] = ["%06x" % a for a in miss_other[:64]]
    log("quality %s: ref=%d mine=%d miss=%d (data=%d, other=%d) recall-code=%.2f%%"
        % (lname, len(ref_r), len(mine_r), len(miss), miss_data, len(miss_other),
           100.0 * len(ref_r & mine_r) / max(1, len(ref_r) - miss_data)))

# ---- TABLE BASE INDEX: every referenced address in the data area, with referencers
data_lo = 0x10016B
table_bases = []
for tgt_s, refs in sorted(xref.items()):
    tgt = int(tgt_s, 16)
    if not (data_lo <= tgt < N):
        continue
    kinds = {}
    srcs = []
    for src, k in refs:
        kinds[k] = kinds.get(k, 0) + 1
        if src not in srcs:
            srcs.append(src)
    if not srcs:
        continue
    # classify local shape (value run at tgt)
    v0 = int(WORDS[tgt])
    shape = ""
    run = WORDS[tgt:tgt + 32].astype(np.int64)
    if len(run) > 8:
        d = np.diff(run)
        if (d > 0).all():
            shape = "ramp_up"
        elif (d < 0).all():
            shape = "ramp_down"
        elif (d == 0).all():
            shape = "const"
    table_bases.append({
        "addr": tgt_s, "v0": "%06x" % v0, "shape32": shape,
        "kinds": kinds, "refs": srcs[:12],
    })
json.dump(table_bases, open(os.path.join(OUT, "table_bases.json"), "w"), indent=1)
log("table bases referenced from code: %d" % len(table_bases))

# ---------------------------------------------------------------- save
np.savez_compressed(os.path.join(OUT, "arrays.npz"),
                    valid=valid, extcnt=extcnt, mnemid=mnemid, cflag=cflag, words=WORDS)
json.dump(sorted(MNEMS.keys()), open(os.path.join(OUT, "mnemonics.json"), "w"), indent=0)
json.dump(functions, open(os.path.join(OUT, "functions.json"), "w"), indent=1)
with gzip.open(os.path.join(OUT, "xref.json.gz"), "wt") as f:
    json.dump(xref, f)
json.dump({"ptr_tables": ptr_tables_json, "tables": tables_json},
          open(os.path.join(OUT, "tables_v2.json"), "w"), indent=1)
with open(os.path.join(OUT, "code_full.asm"), "w") as f:
    for a in sorted(code_text):
        f.write(code_text[a] + "\n")
json.dump({"regions": [["%06x" % s, "%06x" % e, e - s, bool(c)] for s, e, c in regions],
           "indirect_sites": ["%06x" % a for a in indirect_sites],
           "suspicious_cap_hits": ["%06x" % a for a in suspicious[:64]],
           "miss_other": miss_other_report},
          open(os.path.join(OUT, "regions.json"), "w"), indent=1)

code_words = int(code_mask.sum()) + int(cont_mask.sum())
top_fn = sorted(func_in.items(), key=lambda kv: -kv[1])[:40]
with open(os.path.join(OUT, "coverage.md"), "w") as f:
    f.write("# decoder_v2 coverage (dsp1_pmem, %d words = $0-$%x)\n\n" % (N, N - 1))
    f.write("- code words (starts+ext): %d (%.2f%%)\n" % (code_words, 100.0 * code_words / N))
    f.write("- code regions: %d (capped: %d)\n" % (len(regions), sum(1 for _, _, c in regions if c)))
    f.write("- functions (jsr targets): %d; indirect sites: %d\n" % (len(functions), len(indirect_sites)))
    f.write("- xref targets: %d\n" % len(xref))
    f.write("- pointer-run tables: %d; numeric runs >=16: %d\n\n" % (len(ptr_tables_json), len(tables_json)))
    f.write("## top referenced functions\n\n| addr | calls_in | region |\n|---|---|---|\n")
    for d, c in top_fn:
        rg = fn_regions.get(d)
        f.write("| $%06x | %d | %s |\n" % (d, c, rg and ["$%06x-$%06x" % (rg[0], rg[1])] or "-"))
    f.write("\n## biggest referenced data runs\n\n")
    big = [t for t in tables_json if t["len"] >= 48][:48]
    for t in big:
        f.write("- $%06x len=%d %s %s refs=%s\n" % (t["addr"], t["len"], t["type"], t["note"], t.get("refs", [])[:4]))
log("saved outputs to %s" % OUT)
