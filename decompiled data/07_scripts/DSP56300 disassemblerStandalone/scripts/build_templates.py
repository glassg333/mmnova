#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Build complete opcode templates from reference listings.
Handles all hex formats: $XXX, >$XXX, (rN+$XXX), func_XXXXXX, int_XXXXXX.

The ext_word from hexbytes line is uppercase 6-hex (e.g., '0002C8'), but in operands
the same value may appear as:
  - $2c8 (lowercase, no leading zeros, with $)
  - >$2c8 (with > prefix)
  - (r1+$120) (after +)
  - func_ff0000 / int_000002 (lowercase)
  - <<$ffffc3 (absolute long, with << prefix)
"""
import re
import json
from collections import defaultdict, Counter

REF_DIR = '/home/z/my-project/dis56300/reference'
OUT = f'{REF_DIR}/opcode_templates.json'

# Pattern for parsing reference lines
LINE_RE = re.compile(r'^([0-9A-Fa-f]+):\s+(\S+)\s+(.*?)\s*;\s+([0-9A-Fa-f]+(?:\s+[0-9A-Fa-f]+)?)\s*(;.*)?$')


def find_ext_word_in_ops(ops: str, ext_word: str) -> tuple:
    """Найти ext_word в строке ops и вернуть (start, end, format) или None.
    
    ext_word — 6 hex digits uppercase, e.g. '0002C8'
    ops — строка операндов, e.g., 'a,x:>$2c8' или 'func_000235'
    
    Возвращает кортеж (start, end, placeholder) — позиции в строке ops и placeholder.
    """
    # ext_word may have leading zeros that are stripped in ops
    # e.g., '0002C8' -> '2c8' (with $ prefix)
    # Strip leading zeros but keep at least one digit
    stripped = ext_word.lower().lstrip('0') or '0'
    full_lower = ext_word.lower()
    
    # Try various formats:
    formats = [
        # Format: (regex pattern, placeholder template)
        # $XXXXXX format (lowercase, may have leading zeros stripped)
        (rf'\$({stripped}|{full_lower})\b', '{ext%d}'),  # bare $XXX
        (rf'>\$({stripped}|{full_lower})\b', '>{ext%d}'),  # >$XXX (immediate #>)
        (rf'\$({stripped}|{full_lower})(?=[,\s)])', '{ext%d}'),  # $XXX followed by , or )
        (rf'\+\$({stripped}|{full_lower})\b', '+${ext%d}'),  # +$XXX (rN+$XXX)
        (rf'<<\$({stripped}|{full_lower})\b', '<<${ext%d}'),  # <<$XXX (absolute long)
        (rf'func_({stripped}|{full_lower})\b', 'func_{ext%d}'),
        (rf'int_({stripped}|{full_lower})\b', 'int_{ext%d}'),
    ]
    
    for fmt_idx, (regex, placeholder) in enumerate(formats):
        m = re.search(regex, ops, re.IGNORECASE)
        if m:
            return (m.start(), m.end(), placeholder)
    
    return None


def build_template(ops: str, ext_words: list, mnem: str, opcode_val: int, addr: int) -> str:
    """Build a template from ops by replacing ext_word values with placeholders.
    
    Special cases:
    1. Short branches (bra/bsr/bcc/etc, no ext_words): target = addr + offset_in_opcode.
       If low_byte_signed == offset, use {rel8} placeholder.
       Otherwise, keep target as-is (each opcode maps to unique target).
    2. `do`/`dor` instructions: ext_word = end_address - 1 (off-by-one).
       Use {ext0} placeholder, and at disasm time, output = ext + 1.
       Marker: we add {do_end_addr} placeholder instead of {ext0}.
    3. Peripheral addresses 0xFFFFXX: use <<$ (long absolute) instead of >$ (short).
    """
    # Special: do/dor — ext_word = end_addr - 1
    if mnem in ('do', 'dor') and len(ext_words) == 1:
        ext_val = int(ext_words[0], 16)
        end_addr = (ext_val + 1) & 0xFFFFFF  # actual end address
        end_addr_lower = f'{end_addr:06x}'
        end_addr_stripped = end_addr_lower.lstrip('0') or '0'
        # Find end_addr in ops (in form >$XXXXXX or *+$XXXXXX)
        template = ops
        # Try >$XXXXXX (do JSR form)
        for hex_form in [end_addr_lower, end_addr_stripped]:
            if f'>${hex_form}' in template:
                template = template.replace(f'>${hex_form}', f'>${{do_end_addr}}', 1)
                return template
        # Try *+$XXXXXX (dor form)
        for hex_form in [end_addr_lower, end_addr_stripped]:
            if f'*+${hex_form}' in template:
                template = template.replace(f'*+${hex_form}', f'*+${{do_end_addr}}', 1)
                return template
            if f'*+{hex_form}' in template:
                template = template.replace(f'*+{hex_form}', f'*+${{do_end_addr}}', 1)
                return template
        # Fall through if not found
    
    # Detect short branches
    if not ext_words and mnem in ('bra', 'bsr', 'bcc', 'bcs', 'beq', 'bne', 'bge', 'blt',
                                    'bmi', 'bpl', 'ble', 'bgt', 'bls', 'bhi', 'bvs', 'bvc',
                                    'bec', 'bes', 'blc', 'bnr', 'bnn'):
        m = re.search(r'(?:func_|int_)([0-9a-fA-F]+)', ops)
        if m:
            target = int(m.group(1), 16)
            offset = (target - addr) & 0xFFFFFF
            if offset >= 0x800000:
                offset_signed = offset - 0x1000000
            else:
                offset_signed = offset
            # Check if low byte (signed) equals the offset
            low_byte = opcode_val & 0xFF
            if low_byte >= 0x80:
                low_signed = low_byte - 0x100
            else:
                low_signed = low_byte
            if low_signed == offset_signed:
                # Standard 8-bit signed offset in low byte
                # Keep the original prefix (func_ or int_) from the reference
                ops = re.sub(r'(func_|int_)[0-9a-fA-F]+', r'\1{rel8}', ops)
                return ops
            # Otherwise — non-standard encoding, keep target as-is
            return ops
    
    # Default: process ext_words normally
    template = ops
    for i, ext_word in enumerate(ext_words):
        ext_val = int(ext_word, 16)
        ext_lower = ext_word.lower()
        stripped = ext_lower.lstrip('0') or '0'
        
        # Case 1: negative offset (e.g., ext=0xFFFF71 -> -$8f in ops)
        if ext_val >= 0x800000:
            neg_val = ext_val - 0x1000000
            neg_hex = format(-neg_val, 'x')
            for variant in [neg_hex, neg_hex.upper()]:
                pattern = rf'-\${variant}(?=[,\s)+])'
                m = re.search(pattern, template, re.IGNORECASE)
                if m:
                    template = template[:m.start()] + '-${ext%d}' % i + template[m.end():]
                    break
                pattern2 = rf'-\${variant}\b'
                m = re.search(pattern2, template, re.IGNORECASE)
                if m:
                    template = template[:m.start()] + '-${ext%d}' % i + template[m.end():]
                    break
            else:
                pass
            if '-${ext%d}' % i in template:
                continue
        
        # Case 2: ext_word matches func_/int_ in template (relative branch with ext)
        if 'func_' in template and i == 0:
            m = re.search(r'func_([0-9a-fA-F]+)', template)
            if m:
                template = template[:m.start()] + f'func_{{ext{i}}}' + template[m.end():]
                continue
        if 'int_' in template and i == 0:
            m = re.search(r'int_([0-9a-fA-F]+)', template)
            if m:
                template = template[:m.start()] + f'int_{{ext{i}}}' + template[m.end():]
                continue
        
        # Case 3: ext_word appears as hex value in ops
        # Always use >$ (short absolute) in template — apply_template will
        # convert to <<$ (long absolute) at disasm time if ext_val >= 0xFF0000
        # AND context is memory addressing (x:/y:).
        # For immediate (#>) — always keep >$.
        # For peripheral addresses (ext >= 0xFF0000), keep the original format
        # (since reference shows both >$ and <<$ depending on context)
        
        # First try >$ format (most common)
        for hex_form in [ext_lower, stripped]:
            if f'>${hex_form}' in template:
                template = template.replace(f'>${hex_form}', f'>${{ext{i}}}', 1)
                break
        else:
            # Try <<$ format — convert to >$ placeholder; apply_template will fix it
            for hex_form in [ext_lower, stripped]:
                if f'<<${hex_form}' in template:
                    template = template.replace(f'<<${hex_form}', f'>${{ext{i}}}', 1)
                    break
            else:
                for hex_form in [ext_lower, stripped]:
                    pattern = rf'\${hex_form}(?=[,\s)+]|$)'
                    new_template, n = re.subn(pattern, f'${{ext{i}}}', template, count=1, flags=re.IGNORECASE)
                    if n > 0:
                        template = new_template
                        break
                else:
                    for hex_form in [ext_lower, stripped]:
                        if hex_form in template:
                            template = template.replace(hex_form, f'{{ext{i}}}', 1)
                            break
                    else:
                        return None  # Failed
    
    return template


def main():
    groups = defaultdict(list)
    
    for fname in ['dispatch_dsp1_kernel_P0000-0B4D.txt',
                  'dispatch_dsp1_dispatch.txt',
                  'dispatch_dsp1_machine_code.txt',
                  'dispatch_dsp1_machines_fx.txt']:
        path = f'{REF_DIR}/{fname}'
        with open(path) as f:
            for line in f:
                line = line.rstrip()
                if not line: continue
                if re.match(r'^(func_|int_|;|[A-Z_]+:)', line):
                    continue
                m = LINE_RE.match(line)
                if not m: continue
                addr, mnem, ops, hexbytes, extra = m.groups()
                hex_words = hexbytes.split()
                if not hex_words: continue
                opcode = hex_words[0].upper()
                ext_words = [w.upper() for w in hex_words[1:]]
                groups[(opcode, mnem)].append((ops, ext_words, addr))
    
    templates = {}
    ambiguous = []
    
    for (op, mnem), entries in groups.items():
        ext_counts = {len(e) for _, e, _ in entries}
        if len(ext_counts) != 1:
            ambiguous.append((op, mnem, 'mixed ext_count', entries[:3]))
            continue
        ext_count = list(ext_counts)[0]
        
        if ext_count == 0:
            # Check if this is a short branch first (bra/bsr/bcc/etc with no ext_words)
            if mnem in ('bra', 'bsr', 'bcc', 'bcs', 'beq', 'bne', 'bge', 'blt',
                         'bmi', 'bpl', 'ble', 'bgt', 'bls', 'bhi', 'bvs', 'bvc',
                         'bec', 'bes', 'blc', 'bnr', 'bnn'):
                sample_ops, _, sample_addr_str = entries[0]
                sample_addr = int(sample_addr_str, 16) if isinstance(sample_addr_str, str) else sample_addr_str
                opcode_val = int(op, 16)
                template = build_template(sample_ops, [], mnem, opcode_val, sample_addr)
                if template and '{rel8}' in template:
                    templates[(op, mnem)] = {'template': template, 'ext_count': 0,
                                             'short_branch': True}
                    continue
            
            ops_set = {ops for ops, _, _ in entries}
            if len(ops_set) == 1:
                templates[(op, mnem)] = {'template': list(ops_set)[0], 'ext_count': 0}
            else:
                # Same opcode, no ext, multiple ops — pick most common
                ops_counter = Counter(ops for ops, _, _ in entries)
                most_common = ops_counter.most_common(1)[0][0]
                templates[(op, mnem)] = {'template': most_common, 'ext_count': 0,
                                          'variants': len(ops_set),
                                          'most_common_count': ops_counter.most_common(1)[0][1]}
                ambiguous.append((op, mnem, f'no ext, {len(ops_set)} variants', list(ops_set)[:5]))
            continue
        
        # Try short branch detection first
        # If this is a branch mnemonic with no ext_words, build_template handles it specially
        sample_ops, sample_ext, sample_addr = entries[0]
        opcode_val = int(op, 16)
        
        if not sample_ext and mnem in ('bra', 'bsr', 'bcc', 'bcs', 'beq', 'bne', 'bge', 'blt',
                                        'bmi', 'bpl', 'ble', 'bgt', 'bls', 'bhi', 'bvs', 'bvc',
                                        'bec', 'bes', 'blc', 'bnr', 'bnn'):
            # Short branch — template will use {rel8} placeholder
            template = build_template(sample_ops, sample_ext, mnem, opcode_val, sample_addr)
            if template and '{rel8}' in template:
                templates[(op, mnem)] = {'template': template, 'ext_count': 0, 'short_branch': True}
                continue
            # If not short branch, fall through to normal handling
        
        # Has extension words — try to build template from each entry
        template = build_template(sample_ops, sample_ext, mnem, opcode_val, sample_addr)
        if template:
            templates[(op, mnem)] = {'template': template, 'ext_count': ext_count}
        else:
            # Try other entries
            success = False
            for ops, ext, addr in entries[1:]:
                template = build_template(ops, ext, mnem, opcode_val, addr)
                if template:
                    templates[(op, mnem)] = {'template': template, 'ext_count': ext_count}
                    success = True
                    break
            if not success:
                ambiguous.append((op, mnem, 'template_build_failed', (sample_ops, sample_ext)))
    
    # Save final templates
    out = {}
    for (op, mnem), t in templates.items():
        out.setdefault(op, {})[mnem] = t
    
    with open(OUT, 'w') as f:
        json.dump(out, f, indent=2)
    
    print(f'Templates built: {len(templates)}')
    print(f'Ambiguous: {len(ambiguous)}')
    print(f'Saved to {OUT}')
    
    # Show categories of ambiguous
    by_reason = Counter()
    for op, mnem, reason, _ in ambiguous:
        if reason.startswith('no ext'):
            by_reason['no_ext_multi_variant'] += 1
        elif reason == 'template_build_failed':
            by_reason['template_failed'] += 1
        elif reason == 'mixed ext_count':
            by_reason['mixed_ext_count'] += 1
        else:
            by_reason[reason] += 1
    
    print()
    print('Ambiguous by reason:')
    for r, c in by_reason.most_common():
        print(f'  {r}: {c}')
    
    # Save ambiguous
    with open(f'{REF_DIR}/ambiguous_opcodes.json', 'w') as f:
        json.dump([{'opcode': op, 'mnemonic': mnem, 'reason': reason, 'data': str(data)[:200]}
                   for op, mnem, reason, data in ambiguous], f, indent=2)


if __name__ == '__main__':
    main()
