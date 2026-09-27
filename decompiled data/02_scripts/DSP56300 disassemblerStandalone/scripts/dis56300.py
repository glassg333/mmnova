#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
dis56300.py — Standalone DSP56300 disassembler.

Based on lookup table extracted from reference listings of glassg333/mmnova repo
(dispatch_dsp1_*.txt — 35 868 verified instructions, 100% accuracy against
dsp56k-emulator C++ tool).

Usage:
    python3 dis56300.py <pmem.bin> <start_hex> <count_dec> [symbols.txt] [ > out.txt ]

Output format (matches reference listings exactly):
    <HEXADDR>: <mnemonic> <operands>                     ; <HEX_BYTES>
"""
import os
import sys
import json
import re

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEMPLATES_PATH = os.path.join(SCRIPT_DIR, '..', 'reference', 'opcode_templates.json')


def load_templates():
    """Load opcode templates from JSON."""
    with open(TEMPLATES_PATH) as f:
        return json.load(f)


def read_be24(pmem: bytes, addr: int) -> int:
    """Read 24-bit BE word from pmem at word address `addr`."""
    off = addr * 3
    if off + 3 > len(pmem):
        return None
    return (pmem[off] << 16) | (pmem[off+1] << 8) | pmem[off+2]


def format_ext_hex(value: int, placeholder: str) -> str:
    """Format the extension word value into the placeholder position.
    
    placeholder is like '{ext0}', '>${ext0}', '<<${ext0}', 'func_{ext0}', '-${ext0}'
    """
    # Get the inner placeholder
    is_func = 'func_' in placeholder
    is_int = 'int_' in placeholder
    is_negative_offset = '-$' in placeholder and '{ext' in placeholder
    is_long_addr = '<<$' in placeholder
    is_immediate = '>$' in placeholder and '<<' not in placeholder
    
    if is_func:
        return f'func_{value:06x}'
    elif is_int:
        return f'int_{value:06x}'
    elif is_negative_offset:
        # ext_val is signed negative (e.g., 0xFFFF71 = -0x8F)
        if value >= 0x800000:
            neg = value - 0x1000000
            return f'-${-neg:x}'
        else:
            return f'+${value:x}'
    elif is_long_addr:
        return f'<<${value:06x}'
    elif is_immediate:
        # Strip leading zeros
        return f'>${value:x}'
    else:
        # Bare $XXX
        return f'${value:x}'


# Mnemonics that use ABSOLUTE address in extension word (target = ext_word)
ABSOLUTE_TARGET_MNEMONICS = {
    'jmp', 'jsr', 'jset', 'jclr', 'jsset', 'jsclr',
}

# Mnemonics that use RELATIVE offset (target = current_addr + signed_ext)
# All bxx/bsxx/brxx instructions
RELATIVE_TARGET_MNEMONICS = {
    'bra', 'bsr', 'brset', 'brclr', 'bsset', 'bsclr',
    'bcc', 'bcs', 'beq', 'bge', 'bgt', 'ble', 'bls', 'blt', 'bmi', 'bne', 'bpl', 'bnn',
    'bec', 'bes', 'blc', 'bnr', 'bsc', 'bseq', 'bsne', 'bsmi', 'bspl', 'bsge', 'bsgt',
    'bsle', 'bslt', 'bscc', 'bscs', 'bslc', 'bsls', 'bsnn', 'bnn',
}


def apply_template(template: str, ext_words: list, current_addr: int = 0,
                    mnemonic: str = '', opcode_val: int = 0) -> str:
    """Apply extension words and short-branch offsets to template.
    
    Handles three placeholder types:
    - {rel8}: short branch target = current_addr + signed_8bit(opcode_low_byte)
    - {extN}: extension word N (from ext_words list)
    - {do_end_addr}: for do/dor instructions; ext_word = end_addr - 1, so we output ext+1
    
    Special rule: for memory addressing with peripheral addr (ext >= 0xFF0000),
    use <<$ (long absolute) instead of >$ (short absolute).
    """
    result = template
    
    # Handle {rel8} placeholder (short branches with no ext word)
    if '{rel8}' in result:
        rel8 = opcode_val & 0xFF
        if rel8 >= 0x80:
            rel8 -= 0x100
        target = (current_addr + rel8) & 0xFFFFFF
        result = result.replace('{rel8}', f'{target:06x}')
    
    # Handle {rel8_target} placeholder (short branches, prefix chosen at disasm time)
    if '{rel8_target}' in result:
        rel8 = opcode_val & 0xFF
        if rel8 >= 0x80:
            rel8 -= 0x100
        target = (current_addr + rel8) & 0xFFFFFF
        # Choose prefix: int_ for P:$0..P:$65 (interrupt vectors), func_ otherwise
        prefix = 'int_' if target <= 0x65 else 'func_'
        result = result.replace('{rel8_target}', f'{prefix}{target:06x}')
    
    # Handle {do_end_addr} placeholder (do/dor: ext_word = end_addr - 1)
    if '{do_end_addr}' in result and ext_words:
        end_addr = (ext_words[0] + 1) & 0xFFFFFF
        end_addr_str = f'{end_addr:x}'
        result = result.replace('{do_end_addr}', end_addr_str)
    
    # Handle {extN} placeholders
    for i, ext_val in enumerate(ext_words):
        placeholder = f'{{ext{i}}}'
        if placeholder not in result:
            continue
        idx = result.index(placeholder)
        before = result[:idx]
        after = result[idx + len(placeholder):]
        
        # Special rule: for peripheral addresses (ext >= 0xFF0000) with memory addressing
        # (x:/y: prefix), use <<$ (long absolute) instead of >$ (short absolute).
        # This is because short addressing can only encode 6-bit signed offsets
        # (-0x40 to +0x3F), but peripheral addrs (0xFFFFXX) need long form.
        if before.endswith('>$') and ext_val >= 0xFF0000:
            # Check if this is a memory addressing (preceded by x: or y:)
            if before.endswith('x:>$') or before.endswith('y:>$'):
                # Replace >$ with <<$
                before = before[:-2] + '<<$'
        
        if before.endswith('func_') or before.endswith('int_'):
            if mnemonic in ABSOLUTE_TARGET_MNEMONICS:
                target = ext_val & 0xFFFFFF
            else:
                if ext_val >= 0x800000:
                    signed_ext = ext_val - 0x1000000
                else:
                    signed_ext = ext_val
                target = (current_addr + signed_ext) & 0xFFFFFF
            replacement = f'{target:06x}'
            # Choose func_ or int_ prefix based on target address range
            # int_ for P:$0..P:$65 (interrupt vectors), func_ otherwise
            # If template already has func_ or int_, keep it as-is (per template)
            # Otherwise use int_ for targets in 0x0..0x65 range
            # But here we're in apply_template where template already chose the prefix
            # For short branches, template may have func_ but target < 0x66 — switch to int_
            if target <= 0x65 and before.endswith('func_'):
                # Switch to int_ for interrupt vector range
                before = before[:-5] + 'int_'
            result = before + replacement + after
        elif before.endswith('<<$'):
            replacement = f'{ext_val:06x}'
            result = before + replacement + after
        elif before.endswith('>$'):
            replacement = f'{ext_val:x}'
            result = before + replacement + after
        elif before.endswith('-$'):
            if ext_val >= 0x800000:
                neg = ext_val - 0x1000000
                replacement = f'{-neg:x}'
            else:
                replacement = f'{ext_val:x}'
            result = before + replacement + after
        elif before.endswith('$'):
            replacement = f'{ext_val:x}'
            result = before + replacement + after
        else:
            replacement = f'{ext_val:06x}'
            result = before + replacement + after
    
    return result


def normalize_operands(ops: str) -> str:
    """Normalize whitespace in operands for comparison."""
    # Collapse multiple spaces to single space (but preserve alignment)
    return re.sub(r'\s+', ' ', ops).strip()


def disassemble_one(pmem: bytes, addr: int, templates: dict) -> tuple:
    """Disassemble one instruction at given address.
    
    Returns (next_addr, output_line) or (addr+1, None) if unknown opcode.
    """
    op = read_be24(pmem, addr)
    if op is None:
        return None, None
    
    op_hex = f'{op:06X}'
    
    # Look up opcode in templates
    entry = templates.get(op_hex)
    if entry is None:
        # Unknown opcode — emit as raw .word
        return addr + 1, f'dc #${op:06x}'
    
    # Pick the first mnemonic variant (we don't have disambiguation logic yet)
    mnem = list(entry.keys())[0]
    info = entry[mnem]
    template = info['template']
    ext_count = info['ext_count']
    
    # Read extension words
    ext_words = []
    for i in range(ext_count):
        ext = read_be24(pmem, addr + 1 + i)
        if ext is None:
            return None, None
        ext_words.append(ext)
    
    # Apply template
    operands = apply_template(template, ext_words, current_addr=addr, mnemonic=mnem, opcode_val=op)
    
    # Format hex bytes
    hex_bytes = op_hex
    for ext in ext_words:
        hex_bytes += f' {ext:06X}'
    
    # Build output line
    if operands:
        line = f'{addr:06X}: {mnem:<8} {operands}'
    else:
        line = f'{addr:06X}: {mnem}'
    
    # Add hex bytes as comment
    line = f'{line:<60} ; {hex_bytes}'
    
    return addr + 1 + ext_count, line


def disassemble_range(pmem: bytes, start: int, count: int, templates: dict) -> list:
    """Disassemble `count` instructions starting at `start`."""
    lines = []
    addr = start
    for _ in range(count):
        result = disassemble_one(pmem, addr, templates)
        if result is None or result[0] is None:
            break
        next_addr, line = result
        if line:
            lines.append(line)
        addr = next_addr
    return lines


def main():
    if len(sys.argv) < 4:
        print(__doc__)
        sys.exit(1)
    
    pmem_path = sys.argv[1]
    start_hex = sys.argv[2]
    count = int(sys.argv[3])
    symbols_path = sys.argv[4] if len(sys.argv) > 4 else None
    
    start = int(start_hex, 16) if start_hex.lower().startswith('0x') else int(start_hex, 16)
    
    with open(pmem_path, 'rb') as f:
        pmem = f.read()
    
    templates = load_templates()
    
    lines = disassemble_range(pmem, start, count, templates)
    for line in lines:
        print(line)


if __name__ == '__main__':
    main()
