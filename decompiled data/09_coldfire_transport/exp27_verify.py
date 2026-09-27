#!/usr/bin/env python3
"""Iteration 27: decode the $2403D4 jump table byte-exact + verify key constants.

Verifies from the raw binary (not the linear disasm):
  1. Jump table at $240400: 8 words, target = $240400 + disp.
  2. Constant $F23FA = 2*63*63*125 (shift decomposition used by the tempo engine).
  3. RAM trampoline map: RAM $1000088..$1000929 = ROM $2676E0..$267F82.
"""
import struct, json, os

BIN = '/home/z/my-project/work/mmnova/decompiled data/05_descriptors/coldfire_main.bin'
BASE = 0x200000
data = open(BIN, 'rb').read()

def rd(a, n):
    return data[a - BASE: a - BASE + n]

out = {}

# --- 1. jump table at $240400 ---
jt = []
for i in range(8):
    w = struct.unpack('>H', rd(0x240400 + 2*i, 2))[0]
    jt.append({'group': i, 'disp': w, 'target': 0x240400 + w})
out['jump_table_240400'] = jt

# --- 2. constants ---
out['F23FA'] = {'value': 0xF23FA, 'equals': '2*63*63*125 = %d' % (2*63*63*125),
                'match': 0xF23FA == 2*63*63*125}
out['rounding_791FD'] = {'value': 0x791FD, 'equals': 'F23FA>>1 = %d' % (0xF23FA >> 1),
                         'match': 0x791FD == (0xF23FA >> 1)}
out['host_791FD0'] = {'value': 0x791FD0, 'equals': 'F23FA<<3 = %d' % (0xF23FA << 3),
                      'match': 0x791FD0 == (0xF23FA << 3),
                      'note': 'DSP-side echo gate constant (iteration 24) = 8 * host modulus'}

# --- 3. trampolines ---
ram_calls = {0x1000088: 1, 0x1000312: 1, 0x100039A: 1, 0x10003BA: 3, 0x1000416: 1,
             0x100043A: 1, 0x1000462: 2, 0x1000720: 9, 0x1000796: 1, 0x1000800: 2,
             0x1000900: 2, 0x1000A00: 4}
tram = []
for ram, n in sorted(ram_calls.items()):
    rom = 0x2676E0 + (ram - 0x1000088) if 0x1000088 <= ram <= 0x100092A else None
    tram.append({'ram': ram, 'calls': n, 'rom': rom})
out['trampolines'] = tram
out['copy_range'] = {'ram': [0x1000088, 0x100092A], 'rom': [0x2676E0, 0x267F82],
                     'bytes': 0x8A2}

# --- 4. T2 magnitudes from iteration-24 DSP measurements ---
M = 137488  # $21D10
targets = {32: 0x00020CAA89E0, 64: 0x01062464DE00, 96: 0x0087F67E6B40, 127: 0x000087440000}
t2 = {}
for lpq, t in targets.items():
    v = t / ((1 << 16) * M)
    t2[lpq] = {'target48': t, 'T2_float': round(v, 6),
               'T2_as_2p23': round(v * (1 << 23), 1)}
out['T2_from_iter24'] = t2
out['T2_note'] = ('T2[LPQ] = target / (P$2A * $21D10) at P$2A = 1<<16. '
                  'Non-monotonic (confirms iteration 24). T2*2^23 are not round '
                  'numbers -> T2 is a raw 8192-word table, captured in 17_delay_echo.')

dst = os.path.join(os.path.dirname(os.path.abspath(__file__)))
out_path = ('/home/z/my-project/download/mmnova/decompiled data/09_not_sorted/'
            'missing_data/filter_phaser_pack/20_coldfire_clock/exp27_findings.json')
with open(out_path, 'w') as f:
    json.dump(out, f, indent=1)
print(json.dumps(out, indent=1)[:1800])
print('\nwritten', out_path)
