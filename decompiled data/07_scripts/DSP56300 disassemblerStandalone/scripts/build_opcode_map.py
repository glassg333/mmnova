#!/usr/bin/env python3
"""Build opcode lookup table from reference listings."""
import re, os, sys, json
from collections import defaultdict, Counter

REF_DIR = '/home/z/my-project/dis56300/reference'
REF_FILES = [
    'dispatch_dsp1_kernel_P0000-0B4D.txt',
    'dispatch_dsp1_dispatch.txt',
    'dispatch_dsp1_machine_code.txt',
    'dispatch_dsp1_machines_fx.txt',
]

LINE_RE = re.compile(r'^([0-9a-fA-F]{4,6}):\s+(\S.*?)(?:\s{2,}|;)\s*;\s*(.+?)\s*$')
HEX_RE = re.compile(r'\b([0-9a-fA-F]{6})\b')
BITS_PREFIX_RE = re.compile(r'^\(.*?\)\s*', re.DOTALL)

def parse_line(raw):
    line = raw.rstrip()
    if not line:
        return None
    # must look like an address line (hex addr, colon)
    mm = re.match(r'^([0-9a-fA-F]{4,6}):\s+(\S.*)$', line)
    if not mm:
        return None
    main = mm.group(2)
    # main has "mnemonic operands ... ; comment-with-hexbytes"
    if ';' not in main:
        return None
    addr = int(mm.group(1), 16)
    ops, comment = main.split(';', 1)
    ops = ops.rstrip()
    comment = comment.strip()
    # strip "(bits: ...)" prefix
    comment = BITS_PREFIX_RE.sub('', comment)
    # find hex words
    hexes = HEX_RE.findall(comment)
    if not hexes:
        return None
    # Split mnemonic vs operands
    mm2 = re.match(r'^(\S+)\s*(.*)$', ops)
    if not mm2:
        return None
    mnemonic = mm2.group(1)
    operands = mm2.group(2).strip()
    opcodes = [int(h, 16) for h in hexes]
    return (addr, mnemonic, operands, opcodes)

def hex_lc(v):
    """Lowercase hex, no leading zeros."""
    return f"{v:x}"

def analyze_ext_role(samples):
    """Given a list of (mnemonic, operands, ext, addr), figure out how ext maps to operands."""
    # All samples should have same mnemonic
    # Try: does the operands string contain the hex form of ext (lowercase, no leading zeros)?
    # Try various forms:
    candidates = [
        # format name, lambda ext->string
        ('imm_hex',   lambda e: hex_lc(e)),
        ('imm_padded6', lambda e: f"{e:06x}"),
        ('imm_padded4', lambda e: f"{e & 0xFFFF:04x}"),
        ('imm_short', lambda e: f"{e & 0xFFFF:x}"),
    ]
    # Forms how ext might appear in operand string
    # #>$value, >$value, <<$value, x:>$value, y:>$value, p:>$value,
    # #$value, <#$value, #>$value, func_xxxxxx, int_xxxxxx
    # Let's just see if hex_lc(ext) is a substring
    return None

def main():
    opcode_to_samples = defaultdict(list)  # opcode1 -> [(addr, mnem, operands, opcodes)]
    addr_to_line = {}  # for vector/target labeling
    file_stats = Counter()
    total = 0
    bad = 0
    for fname in REF_FILES:
        path = os.path.join(REF_DIR, fname)
        with open(path, encoding='utf-8', errors='replace') as f:
            for ln in f:
                p = parse_line(ln)
                if p is None:
                    bad += 1
                    continue
                addr, mnem, operands, opcodes = p
                opcode1 = opcodes[0]
                ext = opcodes[1] if len(opcodes) > 1 else None
                opcode_to_samples[opcode1].append((addr, mnem, operands, ext, fname))
                file_stats[fname] += 1
                total += 1
    print(f"parsed {total} lines, {bad} skipped", file=sys.stderr)
    print(f"unique opcode1 values: {len(opcode_to_samples)}", file=sys.stderr)
    for k,v in file_stats.items():
        print(f"  {k}: {v}", file=sys.stderr)
    # For each opcode1, check consistency
    inconsistent = 0
    no_ext_roles = {}
    out_records = {}
    for opcode1, samples in opcode_to_samples.items():
        mnemonics = set(s[1] for s in samples)
        if len(mnemonics) != 1:
            inconsistent += 1
            print(f"  INCONSISTENT opcode1={opcode1:06x}: mnemonics={mnemonics}", file=sys.stderr)
            continue
        mnemonic = next(iter(mnemonics))
        # All samples should have same operands modulo ext value.
        # Try to compute template by replacing ext value (in various forms) with placeholder.
        # Group by has_ext
        has_ext = samples[0][3] is not None
        consistent_ext = all((s[3] is not None) == has_ext for s in samples)
        if not consistent_ext:
            inconsistent += 1
            print(f"  INCONSISTENT ext opcode1={opcode1:06x}", file=sys.stderr)
            continue
        # Try to derive template
        if not has_ext:
            # No ext word; operands should be identical across samples (or depend on opcode1 only).
            # Check if operands identical (modulo jump targets that are encoded in opcode1)
            operands_set = set(s[2] for s in samples)
            # In some instructions the target address is encoded in lower bits of opcode1
            # We'll try to detect that by looking at common patterns.
            out_records[opcode1] = {
                'mnemonic': mnemonic,
                'has_ext': False,
                'operands_templates': list(operands_set),
                'sample': (samples[0][0], samples[0][2], None),
            }
        else:
            # ext exists. Try: for each sample, replace hex_lc(ext) in operands with {EXT}
            templates = set()
            for s in samples:
                addr, mn, ops, ext, _ = s
                # Try common formats
                t = ops
                # func_XXXXXX where XXXXXX = hex_lc(ext) padded 6
                t = t.replace(f"func_{ext:06x}", "{EXT_FUNC}")
                t = t.replace(f"int_{ext:06x}", "{EXT_INT}")
                # >$value where value = hex_lc(ext)
                t = t.replace(f">${hex_lc(ext)}", "{EXT_G}")
                # <<$value where value = hex_lc(ext)
                t = t.replace(f"<<${hex_lc(ext)}", "{EXT_LL}")
                # #$value where value = hex_lc(ext) (e.g. #$1, #$fa0)
                t = t.replace(f"#{hex_lc(ext)}", "{EXT_HASH}")
                # #<$value
                t = t.replace(f"#<${hex_lc(ext)}", "{EXT_HL}")
                # >$value padded6 form (already covered by hex_lc when leading zeros)
                # x:>$value same as >$value (already covered)
                templates.add(t)
            out_records[opcode1] = {
                'mnemonic': mnemonic,
                'has_ext': True,
                'operands_templates': sorted(templates),
                'sample': (samples[0][0], samples[0][2], samples[0][3]),
            }
    print(f"inconsistent: {inconsistent}", file=sys.stderr)
    # Save
    out = {}
    for op1, rec in out_records.items():
        out[f"{op1:06x}"] = rec
    with open('/home/z/my-project/dis56300/build/opcode_map.json', 'w') as f:
        json.dump(out, f, indent=1)
    # Report some statistics
    no_ext_count = sum(1 for r in out_records.values() if not r['has_ext'])
    with_ext_count = sum(1 for r in out_records.values() if r['has_ext'])
    print(f"no_ext opcodes: {no_ext_count}", file=sys.stderr)
    print(f"with_ext opcodes: {with_ext_count}", file=sys.stderr)
    # Print sample of multi-template opcodes (incomplete)
    multi = {op1: r for op1, r in out_records.items() if len(r['operands_templates']) > 1}
    print(f"multi-template opcodes: {len(multi)}", file=sys.stderr)
    for op1, r in list(multi.items())[:30]:
        print(f"  {op1:06x} {r['mnemonic']} {len(r['operands_templates'])} templates: {r['operands_templates'][:5]}", file=sys.stderr)

if __name__ == '__main__':
    main()
