#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Тест: сравнение вывода dis56300.py с эталонными листингами.
"""
import os
import re
import sys
import json
from collections import Counter

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'scripts'))
from dis56300 import load_templates, disassemble_one, read_be24, normalize_operands

PMEM_PATH = '/home/z/my-project/re_data/source_images/dsp1_pmem.bin'
REF_DIR = '/home/z/my-project/dis56300/reference'

# Test ranges — для каждого файла указываем стартовый адрес и ожидаемое число инструкций
TESTS = [
    # (reference_file, start_addr, expected_lines)
    ('dispatch_dsp1_kernel_P0000-0B4D.txt', 0x000000, 2243),  # kernel: 2243 instructions
    ('dispatch_dsp1_dispatch.txt', 0x100000, 800),  # dispatch/monitor: just first 800 for speed
    ('dispatch_dsp1_machine_code.txt', 0x1001D1, 500),
    # machines_fx очень большой (22 836 инструкций), тестировать полностью долго
    ('dispatch_dsp1_machines_fx.txt', 0x141A98, 500),
]

# Parse reference lines
REF_RE = re.compile(r'^([0-9A-Fa-f]+):\s+(\S+)\s*(.*?)\s*;\s+([0-9A-Fa-f]+(?:\s+[0-9A-Fa-f]+)?)')


def parse_reference(path: str, max_lines: int = None):
    """Parse reference listing into list of (addr, mnemonic, operands, hex_bytes)."""
    entries = []
    with open(path) as f:
        for line in f:
            line = line.rstrip()
            if not line: continue
            if re.match(r'^(func_|int_|;|[A-Z_]+:)', line):
                continue
            m = REF_RE.match(line)
            if not m: continue
            addr_str, mnem, ops, hexbytes = m.groups()
            addr = int(addr_str, 16)
            entries.append({
                'addr': addr,
                'mnemonic': mnem,
                'operands': ops.strip(),
                'hex_bytes': hexbytes.replace(' ', ''),
            })
            if max_lines and len(entries) >= max_lines:
                break
    return entries


def normalize(s: str) -> str:
    """Normalize whitespace for comparison."""
    return re.sub(r'\s+', ' ', s).strip()


def run_test(ref_file: str, start_addr: int, expected_count: int):
    """Run a single test: disassemble from start_addr, compare with reference."""
    ref_path = os.path.join(REF_DIR, ref_file)
    ref_entries = parse_reference(ref_path, max_lines=expected_count)
    
    print(f'\n=== Test: {ref_file} ===')
    print(f'  Reference: {len(ref_entries)} instructions')
    
    with open(PMEM_PATH, 'rb') as f:
        pmem = f.read()
    templates = load_templates()
    
    # Disassemble each reference entry
    matches = 0
    mismatches = []
    unknown_opcodes = 0
    multi_variant_skipped = 0
    
    for i, ref in enumerate(ref_entries):
        addr = ref['addr']
        op = read_be24(pmem, addr)
        if op is None:
            mismatches.append((addr, ref, 'OOB'))
            continue
        
        op_hex = f'{op:06X}'
        entry = templates.get(op_hex)
        if entry is None:
            unknown_opcodes += 1
            mismatches.append((addr, ref, f'unknown_opcode 0x{op_hex}'))
            continue
        
        # Pick the first mnemonic
        mnem = list(entry.keys())[0]
        info = entry[mnem]
        template = info['template']
        ext_count = info['ext_count']
        
        # Read extension words
        ext_words = []
        ok = True
        for j in range(ext_count):
            ext = read_be24(pmem, addr + 1 + j)
            if ext is None:
                ok = False
                break
            ext_words.append(ext)
        if not ok:
            mismatches.append((addr, ref, 'ext_word_OOB'))
            continue
        
        # Apply template
        from dis56300 import apply_template
        actual_ops = apply_template(template, ext_words, current_addr=addr, mnemonic=mnem, opcode_val=op)
        
        # Compare
        ref_mnem = ref['mnemonic']
        ref_ops = ref['operands']
        
        if mnem == ref_mnem and normalize(actual_ops) == normalize(ref_ops):
            matches += 1
        else:
            mismatches.append((addr, ref, f'GOT: {mnem} {actual_ops}'))
    
    total = len(ref_entries)
    pct = (matches / total * 100) if total else 0
    print(f'  Matches: {matches}/{total} ({pct:.1f}%)')
    print(f'  Unknown opcodes: {unknown_opcodes}')
    print(f'  Mismatches: {len(mismatches)}')
    
    if mismatches:
        print(f'\n  First 20 mismatches:')
        for addr, ref, reason in mismatches[:20]:
            print(f'    ${addr:06X}: REF: {ref["mnemonic"]} {ref["operands"][:50]}  |  {reason[:80]}')
    
    return matches, total, mismatches


def main():
    print('=' * 70)
    print('TEST: dis56300.py against reference listings')
    print('=' * 70)
    
    total_matches = 0
    total_instructions = 0
    all_mismatches = []
    
    for ref_file, start, count in TESTS:
        m, t, mm = run_test(ref_file, start, count)
        total_matches += m
        total_instructions += t
        all_mismatches.extend([(ref_file, addr, ref, reason) for addr, ref, reason in mm])
    
    print('\n' + '=' * 70)
    print(f'OVERALL: {total_matches}/{total_instructions} ({total_matches/total_instructions*100:.1f}%)')
    print('=' * 70)
    
    # Save full mismatch list for inspection
    with open('/home/z/my-project/dis56300/tests/mismatches.json', 'w') as f:
        json.dump([{
            'file': fname, 'addr': f'0x{addr:06X}',
            'ref_mnemonic': ref['mnemonic'],
            'ref_operands': ref['operands'],
            'reason': reason
        } for fname, addr, ref, reason in all_mismatches[:200]], f, indent=2)
    
    # Group mismatches by opcode
    by_mnemonic = Counter()
    for fname, addr, ref, reason in all_mismatches:
        if 'GOT:' in reason:
            # Extract our mnemonic
            got_part = reason.split('GOT:')[1].strip().split()[0]
            by_mnemonic[f'{ref["mnemonic"]} -> {got_part}'] += 1
        elif 'unknown_opcode' in reason:
            by_mnemonic[f'{ref["mnemonic"]} -> UNKNOWN'] += 1
    
    print('\nTop mismatch patterns:')
    for pattern, count in by_mnemonic.most_common(15):
        print(f'  {count:5d}  {pattern}')


if __name__ == '__main__':
    main()
