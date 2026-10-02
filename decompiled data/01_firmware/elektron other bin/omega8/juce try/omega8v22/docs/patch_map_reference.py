# -*- coding: utf-8 -*-
"""
Omega 8 / Omega CODE patch byte map (176 bytes, 7-bit values).
Verified against: factory banks stats + Omega/CODE editor screenshot (REVELATION edit buffer).
confidence: 3=confirmed by editor screenshot, 2=strong statistical/structural, 1=weak, 0=unknown/reserved
"""

# offset: (name, kind, confidence)
# kind: 'i7' 0..127 integer
#       'c64' 0..127 centered (64=0)
#       's64' signed bipolar displayed (stored = display+64)
#       'enum' named enum (values as in ENUMS)
#       'flag' bitfield
#       'sel' selector into dest list (DESTn)
#       'arr8' 8-voice array (voice count stored at 1-8 — see ARR_BASE)
PATCH_MAP = {}

def _m(off, name, kind, conf, note=''):
    PATCH_MAP[off] = dict(name=name, kind=kind, conf=conf, note=note)

# ---- Global / keyboard ----
_m(0,  'GLIDE_TIME', 'i7', 3, 'editor: time=42 (REVELATION)')
_m(1,  'GLIDE_FLAGS','flag',2, 'bit6=on(64), bit2(4)=legato/reg variant')
_m(2,  'OCTAVE',     'oct', 2, 'nibble: hi=octave (4=MID, step1=16raw), lo=fine')
_m(3,  'ENV3_AMT',   'i7',  2, 'pad>>perc; panel env3 amt')
_m(4,  'SUB_WAVE',   'enum',2, '{0=OFF,1..6}')
_m(5,  'RESERVED0',  'i7',  0, 'always 0')
_m(6,  'UNK6',       'enum',0, '{0,1,2} REVELATION=2')
_m(7,  'PRIOR',      'enum',2, '0=LOW(default,845),1,2,3=LAST?,4,5')
_m(8,  'LFO1_RATE',  'i7',  3, '=21 editor')
_m(9,  'LFO1_WAVSYNC','enum',1,'{0:tri,2:sqr?}; editor wave=tri/self')
_m(10, 'LFO1_DEPTH1','i7',  3, '=31 editor')
_m(11, 'LFO1_DEPTH2','i7',  3, '=3')
_m(12, 'LFO1_DEPTH3','i7',  3, '=4')
_m(13, 'LFO1_DEST1', 'sel', 3, '=9 FILT editor')
_m(14, 'LFO1_DEST2', 'sel', 3, '=10 RESO')
_m(15, 'LFO1_DEST3', 'sel', 3, '=6 PW1')
_m(16, 'UNISON',     'flag',1, 'const0 factory (uni OFF)')
_m(17, 'UNK17',      'enum',0, '{0:946,1:58,16:4} candidate voice qty')
_m(18, 'UNK18',      'flag',0, '{0:1004,57:4}')
_m(19, 'VMODE',      'enum',2, '{0:first,1,2=CYCL(56)} editor vmode=CYCL')
_m(20, 'MTRG',       'enum',1, '{0,1,2,3,5,16} editor mtrg=OFF')
_m(21, 'LFO2_RATE',  'i7',  3, '=94 editor')
_m(22, 'LFO2_WAVSYNC','enum',1,'{0:tri,8:sqr?}')
_m(23, 'LFO2_DEPTH1','i7',  3, '=1')
_m(24, 'LFO2_DEPTH2','i7',  3, '=8')
_m(25, 'LFO2_DEPTH3','i7',  3, '=5')
_m(26, 'LFO2_DEST1', 'sel', 3, '=1 FRE1')
_m(27, 'LFO2_DEST2', 'sel', 3, '=7 PW2')
_m(28, 'LFO2_DEST3', 'sel', 3, '=10 RESO')
_m(29, 'RESERVED1',  'i7',  0, 'const0')
# ---- Oscillators ----
_m(30, 'OSC1_FREQ',  'i7',  3, 'panel scale 0..63; =24 editor')
_m(31, 'OSC2_FREQ',  'i7',  3, '=36 editor')
_m(32, 'VOLUME',     'i7',  2, 'hist 49..88 mode60')
_m(33, 'UNK33',      'flag',0, '{0,16,17}')
_m(34, 'PWM1',       'i7',  3, '52% = 67 editor')
_m(35, 'PWM2',       'i7',  3, '46% ~ 57')
_m(36, 'UNK36',      'i7',  0, 'wide 16..106')
_m(37, 'OSC1_LEVEL', 'i7',  3, '=127')
_m(38, 'OSC2_LEVEL', 'i7',  3, '=88')
_m(39, 'NOISE_LEVEL','i7',  3, '=0')
# ---- Filter ----
_m(40, 'HPF',        'i7',  2, 'CS80 HP freq; 4/6 type6 patches >0')
_m(41, 'XMOD_DPTH',  'i7',  3, 'perc>>pad; =000 editor')
_m(42, 'XMOD_DEST',  'enum',3,'{0,1,2}=Osc1F,Osc1PW,Vcf(REVELATION=2=Vcf ✓)')
_m(43, 'FILT_TYPE',  'enum',3,'0=SEM LP,1=BP,2=HP,3=BR,4=MINI,5=AUX1(Ob),6=AUX2(Cs); MAGIC HP=2 ✓')
_m(44, 'CUTOFF',     'i7',  3, '=23')
_m(45, 'RESO',       'i7',  3, '=62')
_m(46, 'TRACKING',   'i7',  3, '=91')
_m(47, 'ENV1_AMT',   'i7',  3, '=117; perc>pad')
# ---- Envelopes (A, D, Dk2, S, R) ----
_m(48, 'DLY1',       'i7',  2, 'delay before env1 (rare >0)')
_m(49, 'ENV1_ATK',   'i7',  3, '=9')
_m(50, 'ENV1_DEC',   'i7',  3, '=99')
_m(51, 'ENV1_DK2',   'i7',  3, '=0')
_m(52, 'ENV1_SUS',   'i7',  3, '=0; pads>>percs')
_m(53, 'ENV1_REL',   'i7',  3, '=97')
_m(54, 'ENV2_ATK',   'i7',  3, '=17')
_m(55, 'ENV2_DEC',   'i7',  3, '=0')
_m(56, 'ENV2_DK2',   'i7',  3, '=127')
_m(57, 'ENV2_SUS',   'i7',  3, '=127; pads>>percs')
_m(58, 'ENV2_REL',   'i7',  3, '=117')
_m(59, 'ENV3_ATK',   'i7',  3, '=0')
_m(60, 'ENV3_DEC',   'i7',  3, '=92')
_m(61, 'ENV3_DK2',   'i7',  3, '=0')
_m(62, 'ENV3_SUS',   'i7',  3, '=0')
_m(63, 'ENV3_REL',   'i7',  3, '=127')
# ---- Env3 mod matrix (3 slots) ----
_m(64, 'ENV3_DEST1', 'sel', 3, '=16 LF1R (1-based with 0=OFF)')
_m(65, 'ENV3_DEST2', 'sel', 3, '=0')
_m(66, 'ENV3_DEST3', 'sel', 3, '=0')
_m(67, 'ENV3_AMT1',  'i7',  3, '=100')
_m(68, 'ENV3_AMT2',  'i7',  3, '=0')
_m(69, 'ENV3_AMT3',  'i7',  3, '=0')
# ---- Misc block 70-75 ----
_m(70, 'UNK70',      'enum',0, '{0,1,2,3}')
_m(71, 'UNK71',      'i7',  0, 'wide sparse; USEMODWHEEL=21')
_m(72, 'FLAGBITS72', 'flag',1, '{0,1,2,4,6,7} filter/env flags?')
_m(73, 'UNK73',      'flag',0, '{0,1,64}')
_m(74, 'WAVE_BITS',  'flag',2, 'bit0=tri,bit1=saw,bit2=pulse,bit5(32)=sub? perc>>pad')
_m(75, 'UNK75',      'enum',1, '{0,1,2}')
# ---- Extended / CODE ----
_m(76, 'EXT_IN',     'i7',  2, '=64 for ~all; editor in=64')
_m(77, 'MASTER_TUNE','c64', 2, '=64 almost always')
_m(78, 'OSC2_FINE',  'c64', 2, '64:center; 40=fat detune, 65=+1')
_m(79, 'HPR',        'i7',  3, 'CS80 HP reso; 911 patches=31, editor=31')
_m(80, 'WIN',        'i7',  3, '63→display 49% ✓')
_m(81, 'UNK81',      'i7',  0, 'wide 10..60')
_m(82, 'UNK82',      'enum',0, '{0,5}')
_m(83, 'UNK83',      'enum',0, '{0,1,2,3}')
_m(84, 'UNK84',      'enum',0, '{0..4}')
_m(85, 'FLAG85',     'flag',0, '{0,64}')
# ---- Controllers: (dest1, dest2, amt1, amt2) sel=1-based into CONTDEST ----
_m(86, 'MODWHEEL_DEST1','sel',3,'=9 FILT')
_m(87, 'MODWHEEL_DEST2','sel',3,'=17 LF2R')
_m(88, 'MODWHEEL_AMT1', 'i7', 3,'=74')
_m(89, 'MODWHEEL_AMT2', 'i7', 3,'=4')
_m(90, 'DYN_DEST1',   'sel',3, '=7 PW2')
_m(91, 'DYN_DEST2',   'sel',3, '=4 LEV1')
_m(92, 'DYN_AMT1',    'i7', 3, '=11')
_m(93, 'DYN_AMT2',    'i7', 3, '=5')
_m(94, 'BENDER_DEST1','sel',3, '=3 1&2F')
_m(95, 'BENDER_DEST2','sel',3, '=9 FILT')
_m(96, 'BENDER_AMT1', 'i7', 3, '=2')
_m(97, 'BENDER_AMT2', 'i7', 3, '=0')
_m(98, 'PRESS_DEST1', 'sel',2, '=0/17/18 (LF2D/LF1D)')
_m(99, 'PRESS_DEST2', 'sel',2, 'mostly0')
_m(100,'PRESS_AMT1',  'i7', 2, 'bipolar? REVELATION=0; hist {63,12,15,0}')
_m(101,'PRESS_AMT2',  'i7', 2, '=0')
_m(102,'CONT1_DEST1', 'sel',3, '=12 XMOD')
_m(103,'CONT1_DEST2', 'sel',3, '=9 FILT')
_m(104,'CONT1_AMT1',  'i7', 3, '=14')
_m(105,'CONT1_AMT2',  'i7', 3, '=3')
_m(106,'CONT2_DEST1', 'sel',3, '=6 PW1')
_m(107,'CONT2_DEST2', 'sel',3, '=5 LEV2')
_m(108,'CONT2_AMT1',  's64',3,'=67→display +3')
_m(109,'CONT2_AMT2',  's64',3,'=57→display -7')
_m(110,'UNK110',      'enum',0,'{1:910,64}')
_m(111,'UNK111',      'flag',0,'{1:719,64:257,0:32}')
# ---- Env dynamics (patch) ----
_m(112,'DYN1',        'i7',  3, '=35 editor')
_m(113,'DYN2',        'i7',  3, '=107')
_m(114,'DYN3',        'i7',  3, '=0')
_m(115,'OSC2_MODE',   'enum',2,'{0..4}; 4=NORM(816)')
_m(116,'PAN_SYNC',    'enum',1,'const0=SELF (editor sync=SELF)')
_m(117,'DLY2',        'i7',  1, 'rare >0 (20 patches =1)')
_m(118,'DLY3',        'i7',  1, 'rare >0')
_m(119,'PAN_KEY',     'enum',1,'const0=OFF (editor key=OFF)')
_m(120,'PAN_WAVE',    'enum',1,'{0:tri,1,2,...}; editor wave=tri')
# ---- 121..135 reserved ----
for o in range(121,136):
    _m(o, f'RESV{o}', 'i7', 0, 'const/unknown')
# ---- 8-voice arrays (voice count at byte-1 — ARR_BASE) ----
_m(136,'PAN_RATE_ARR','arr8',3, '8× rate')
_m(144,'PAN_POS_ARR', 'arr8',3, '8× pos (64=center)')
_m(152,'PAN_DEPTH_ARR','arr8',3,'8× depth')
# name 160..175

# destination lists (1-based, 0=OFF). Verified: env3 dest 16=LF1R (editor).
CONT_DEST = [None, 'FRE1','FRE2','1&2F','LEV1','LEV2','PW1','PW2','1&2P',
             'FILT','RESO','LEVN','XMOD','EA1','EA3','EXT',
             'LF1R','LF2R','LF1D','LF2D','1&2D','1&2R','PAND','PANR','PAN']  # index0=OFF
LFO_DEST = CONT_DEST  # subset (1..14/15)
ENV3_DEST = CONT_DEST

FILT_TYPES = ['SEM_LP','SEM_BP','SEM_HP','SEM_BR','MINI','AUX1','AUX2']  # 0..6

def dump_json(path):
    import json
    with open(path,'w') as f:
        json.dump({'map':{str(k):v for k,v in PATCH_MAP.items()},
                   'CONT_DEST':CONT_DEST, 'FILT_TYPES':FILT_TYPES}, f, indent=1)

if __name__ == '__main__':
    for off in sorted(PATCH_MAP):
        e = PATCH_MAP[off]
        print(f"{off:3d} {e['name']:16s} {e['kind']:5s} c={e['conf']} {e['note']}")
    print('confirmed(c=3):', sum(1 for e in PATCH_MAP.values() if e['conf']==3))
    print('strong (c=2):  ', sum(1 for e in PATCH_MAP.values() if e['conf']==2))
    print('weak   (c=1):  ', sum(1 for e in PATCH_MAP.values() if e['conf']==1))
    print('unknown(c=0):  ', sum(1 for e in PATCH_MAP.values() if e['conf']==0))
