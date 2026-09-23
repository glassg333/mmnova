#!/usr/bin/env python3
"""exp28: echo kernel routing — extract tables + structured findings.

Discovers (iteration 28):
  - kernel voice-page flow ($000087-$0002EB): Y:$123 = page+$28 pointer,
    X:$500-$52F per-voice X-shadow, machine dispatch $10016B/$10018D/$1001AF
  - kernel glide engine ($000262-$0002EA): DSND (P+$1C) read at $000284,
    DBAS (P+$1E) read at $0002C8, P$2A, P+$07/$22/$26/$29 involved
  - rate table Y:$1449C6 (indexed by DSND>>16 and P+$07>>16)
  - mantissa curve X:$140000 (2048 words, 0.5..1.0) — float delay length
  - echo bus cells X:$2C9/$2CA/$2CB, DMA4 (ESSI0 RX + host block $2FC)
"""
import json, os, sys

BASE = '/home/z/my-project'
PMEM = f'{BASE}/work/mmnova/decompiled data/02_memory_images/dsp1_pmem.bin'
OUT  = f'{BASE}/mining/pack_work/filter_phaser_pack/21_echo_kernel_routing'

os.makedirs(OUT, exist_ok=True)
d = open(PMEM, 'rb').read()

def word(a):
    return int.from_bytes(d[a*3:a*3+3], 'big')

# ---------------------------------------------------------------- tables
rate_base  = 0x1449C6
mant_base  = 0x140000
N_RATE, N_MANT = 256, 2048

rate  = [word(rate_base + i) for i in range(N_RATE)]
mant  = [word(mant_base + i) for i in range(N_MANT)]

def dump_txt(name, vals, base):
    with open(f'{OUT}/{name}.txt', 'w') as f:
        f.write(f'table at ${base:X} ({len(vals)} words, 24-bit)\n')
        f.write('idx(hex) hex frac(=/2^23)\n')
        for i, v in enumerate(vals):
            s = v - (1 << 24) if v & 0x800000 else v
            f.write(f'{i:3d} ({i:02x}) {v:06X} {s/ (1<<23):+.7f}\n')

dump_txt('table_1449C6_glide_rates', rate, rate_base)
dump_txt('table_140000_mantissa',    mant, mant_base)
open(f'{OUT}/table_1449C6_glide_rates.bin', 'wb').write(
    b''.join(v.to_bytes(3, 'big') for v in rate))
open(f'{OUT}/table_140000_mantissa.bin', 'wb').write(
    b''.join(v.to_bytes(3, 'big') for v in mant))

# numeric sanity
mant_f  = [v / (1 << 23) for v in mant]
rate_f  = [(v - (1 << 24) if v & 0x800000 else v) / (1 << 23) for v in rate]
findings = {
    'iteration': 28,
    'topic': 'echo/delay kernel routing (DSND/DBAS/P$2A) + voice page flow',

    'voice_page_flow': {
        'page_bases_Y': ['0x0500', '0x0600', '0x0700'],
        'current_page_cell': 'Y:$123 = page_base + 0x28 (starts 0x528)',
        'voice_counter': 'Y:$124 (0..2), +0x100 per voice step at 0x0163',
        'X_shadow': 'X:$0500-0x052F mirrors page words $00-$27 (r6-0x28+k = X-shadow)',
        'key_cells': {
            'X:$501 (r6-0x27)': 'delay position/length float (out of glide engine)',
            'X:$502 (r6-0x26)': 'target A shadow (P+$22)',
            'X:$503 (r6-0x25)': 'glide A current',
            'X:$504:X$505 (r6-0x24/23)': 'glide A 48-bit accumulator',
            'X:$506 (r6-0x22)': 'glide B current (P+$29)',
            'X:$507 (r6-0x21)': 'target B shadow (P+$07)',
            'X:$508:X$509 (r6-0x20/1f)': 'glide B 48-bit accumulator',
            'X:$50B (r6-0x1d)': 'set to $20 on machine change',
            'X:$50F (r6-0x19)': 'set to 1 on machine change',
        },
        'machine_dispatch': {
            'init_on_change': 'x:($10016B + machine_id) at 0x00EB-0x00F6',
            'conf_every_frame': 'x:($10018D + machine_id) at 0x00F7-0x00FF',
            'proc': 'x:($1001AF + machine_id) at 0x0100-0x0106, called at 0x02EB',
        },
        'codec_frame_sync': '0x0087 polls, 3 phases 0x140/0x160/0x180 select '
                            'rotating 48-word audio banks (r1/r2/r3/r0 sets), '
                            'pointer cells X:$2C0-0x2C3, phase saved to X:$2C8',
        'frame_timer': '0x0020 jsset #$4,x:$FFFFF4 -> 0x0259 (timer flag test-and-set)',
    },

    'glide_engine_0x0262': {
        'entry': ['0x0124 jmp (command path)', '0x0129/0x012D brset #$C/#$D of X:$2C4'],
        'reads': {
            'P+$1C DSND': 'y:(r6-0x0C) at 0x0284  <- CORRECTS iter24 "zero reads"',
            'P+$1E DBAS': 'y:(r6-0x0A) at 0x02C8  <- CORRECTS iter24 "zero reads"',
            'P+$07':      'y:(r6-0x21) at 0x0285',
            'P+$22':      'y:(r6-0x06) at 0x0271/0x029E (BPM whole word)',
            'P+$26':      'y:(r6-0x02) at 0x02C6',
            'P+$29':      'y:(r6+0x01) at 0x0263/0x02B5',
            'P$2A':       'y:(r6+0x02) at 0x028A',
            'P+$25 mode': 'y:(r6-0x03) at 0x00DD -> X:$2C4',
            'P+$24 mch':  'y:(r6-0x04) at 0x00E5 vs y:$120+voice',
            'P+$28 trig': 'y:(r6) bit7 = INIT, cleared 0x00E2',
        },
        'rates': 'frac(T[idx] * P$2A >> 8); T = Y:$1449C6; '
                 'idxA = DSND>>16, idxB = P+$07>>16 (clamped negative->0x7F?)',
        'glide_A': 'acc(X:504:505) += (X:502 - X:503) * rateA; snap/clamp to P+$22',
        'glide_B': 'acc(X:508:509) += (X:507 - X:506) * rateB; snap/clamp to P+$29',
        'position': 'X:501 = accB + P+$26 + $B000*DBAS - $5800 + accA, '
                    'clamp [0, $5800]; voice<2 also writes next voice X:$627 (r6+0xFF)',
        'float_decode': 'idx = X:501 & 0x7FF (mantissa ptr), exp = X:501 >> 11; '
                        'len = X:140000[idx] >> 10 << exp = mantissa * 2^(exp-10); '
                        'a = len * $1D22A >> 3 -> machine PROC input (delay length)',
        'mantissa_curve': 'X:$140000: 2048 words, linear 0.5..1.0 (3FFFFF..7FF4E8)',
    },

    'rate_table_1449C6': {
        'entries_0_127': 'glide rate: [0]=negative (FEDCBB), [1]=0x010F79 max, '
                         'monotonic down to [127]=0x00000A',
        'entries_128_255': 'second rising exponential curve 0x000000..0x7FFFFF '
                           '(0 at 128, ~2^(k/16) shape, peak 7FFFFF at 256)',
        'meaning': 'index = knob high byte (0..127); value = per-frame glide '
                   'fraction; DSND high byte -> glide A rate, P+$07 high byte -> B',
    },

    'echo_bus': {
        'X_2C9': 'master-mix write ptr; init $32C (phase-dependent: $2CC/$32C/$2FC); '
                 'kernel writes 16 words/track at 0x0B48, saves ptr 0x0B49',
        'X_2CA': 'init $2FC; DMA4 host-block destination (47 words) via 0x0259',
        'X_2CB': 'init $32C; echo-bus start, kernel copies into banks 0x01C2-0x01E1',
        'master_mix_0x0B33': 'reads x:(r2)=X:$00-$1F mod bank, l:(r5)=L:$10 pairs, '
                             'y:(r4)=Y:$20+; writes X:(r1)=X:$FF+ state, Y:(r6)=Y:$00 '
                             'output, b->X:(r3)+ echo bus',
        'ring_write_0x0B15': 'r4 = X:$C5 (ring ptr); 16 words X:(r0)+ -> Y:(r4)+',
    },

    'dma4': {
        'normal': 'DSR4=$FFFFB8 (ESSI0_RX), DDR4=X-shadow bank ptr (X:$2C0+... '
                  'cycles +$20, cmp $1A0 wrap -$60), DCO4=$301F (32 words), '
                  'DCR4=$CA5140 (Drs=11, word, X-dest) at 0x0197-0x01A0',
        'host_block_0x0259': 'DCR4=$4A5A20 (prep), DDR4=X:$2CA value=$2FC, '
                             'DCO4=$2F (47 words), DCR4=$8A52C0 (Drs=10)',
        'coldfire_readback': '$24828E (DSP2 $600001) / $248204 (DSP1 $500001): '
                             'CVR $06/$86, addr = *(0x259114)+1 = X:$701, '
                             'result -> staging word $27 ($1000AF0 / $1000D60)',
    },

    'coldfire_spec_routing_0x242058': {
        'pair_structure': '6 iterations x 2 calls; per track t (stride 12): '
                          'group byte $7001FA+t*12+j, offset byte $700242+t*12+j, '
                          'third byte $70028A+t*12+j (j=0..11)',
        'dispatcher_0x2403D4': 'd2 = $14(a7) = group (jump table 0x240400, 8 entries); '
                               'd3 = $10(a7) = track; a1 = $18(a7) = offset',
        'groups': {
            '0': '0x240422 -> $1000ACC + track*0xD0 (staging fade-in cell)',
            '1': '0x240472 -> $1000B04 + track*4 + track2*0xD0 (flags)',
            '2,3,4': '0x240450 -> $1000A14 + (group*8 + track*52 + offset)*4 '
                     '= staging page word (offset = word index, group 2: words 0-7, '
                     '3: 8-15, 4: 16-23)',
            '5,6,7': '0x240438 -> $1000E94 + ((3*offset + group_offset)*8 + track)*4',
            '>=8': 'default $10013E4',
        },
        'arg_decode_fix': 'README iter27 named args (spec1, spec2, track) — actual: '
                          'd3=track (x52), d2=group (0x7001FA byte), a1=offset '
                          '(0x700242 byte); one-pointer-per-page-word',
        'getter_0x242BD0': 'index = (argA*7 + argB)*2 + bit; outputs ext.b of '
                           '$7001FA/$700242/$70028A at that index',
        'glide_ptrs': '$1001508 + t*$30 (12 longs = 2 banks of 6), rates $10013E8+t*$30',
        'mirror_image': '$2C01D2 / $2C021A / $2C0262 (+t*12) = DSP2 image',
    },

    'tables_dumped': {
        'Y_1449C6_glide_rates': f'{N_RATE} words',
        'X_140000_mantissa': f'{N_MANT} words',
    },
    'numerics': {
        'mant_first': mant[0], 'mant_last': mant[-1],
        'mant_first_f': mant_f[0], 'mant_last_f': mant_f[-1],
        'rate_1_f': rate_f[1], 'rate_127_f': rate_f[127],
        'rate_0_raw': rate[0],
        'mant_slope_per_entry': (mant[-1] - mant[0]) / (N_MANT - 1),
    },
}

with open(f'{OUT}/exp28_findings.json', 'w') as f:
    json.dump(findings, f, indent=2, ensure_ascii=False)

print('mantissa:', hex(mant[0]), '->', hex(mant[-1]),
      f'({mant_f[0]:.4f}..{mant_f[-1]:.4f}), slope/entry =', hex(int(findings['numerics']['mant_slope_per_entry'])))
print('rate[1] =', rate_f[1], ' rate[127] =', rate_f[127], ' rate[0] =', rate_f[0])
print('OK ->', OUT)
