#!/usr/bin/env python3
from pathlib import Path
import hashlib,re
R=Path(__file__).resolve().parents[1]
H=(R/'include/MnmFilterEvidenceTables.h').read_text()
def bin_words(p):
 b=p.read_bytes(); assert len(b)%3==0
 return [int.from_bytes(b[i:i+3],'big') for i in range(0,len(b),3)]
def head_words(name):
 m=re.search(r'%s\[\d+\] = \{(.*?)\};'%name,H,re.S); assert m,name
 return [int(x,16) for x in re.findall(r'0x([0-9A-Fa-f]+)u',m.group(1))]
for p in sorted((R/'data').glob('*.bin')):
 name=p.stem
 n=len(bin_words(p)); assert head_words(name)==bin_words(p),(name,n)
 print(name,n,hashlib.sha256(p.read_bytes()).hexdigest())
# retained fragments must remain byte-identical to the clean proof set
pairs=[('kCutoffP143546','/home/user/MNM_BLOCKS_FOR_JUCE_CLEAN/data/kernel/filt_cutoff_table.bin'),
       ('kCutoffP1435C6','/home/user/MNM_BLOCKS_FOR_JUCE_CLEAN/data/kernel/filt_cutoff_table2.bin'),
       ('kQTableP141A98','/home/user/MNM_BLOCKS_FOR_JUCE_CLEAN/data/kernel/filter_coeff_a.bin'),
       ('kQTableP142158','/home/user/MNM_BLOCKS_FOR_JUCE_CLEAN/data/kernel/filter_coeff_b.bin'),
       ('kQTableP142F06','/home/user/MNM_BLOCKS_FOR_JUCE_CLEAN/data/kernel/filter_coeff_c.bin')]
for name,p in pairs:
 # Header exact first n words should equal retained raw
 w=bin_words(Path(p)); assert head_words(name)[:len(w)]==w,(name,'retained mismatch')
print('OK: evidence arrays, binary files, and clean fragments agree')
