import pathlib,re,json,hashlib
ROOT=pathlib.Path(__file__).resolve().parent.parent/'evidence'
OUT=ROOT.parent
LIST=ROOT/'selected_dsp.asm'
prog={}
for line in LIST.read_text().splitlines():
 m=re.match(r'([0-9A-F]{6}):\s*(.*?)\s*; ([0-9A-F ]+)$',line)
 if m:prog[int(m[1],16)]=(m[2].lower(),[int(x,16) for x in m[3].split()],line)
regions=[('filterEnvelope',0x4ff,0x56d),('filterAudio',0x5bf,0x88e),('f350',0x350,0x365),('f365',0x365,0x37c),('f37c',0x37c,0x38a),('f38a',0x38a,0x397)]
def num(v):
 return int(v.lstrip('#<>$'),16)
def rr(r):return 'R::'+r
def word(r):
 if r.startswith('#'):return hex(num(r))+'u'
 return 's.word('+rr(r)+')'
def accval(r):
 if r in ('a','b'):return 's.acc['+str(r=='b').lower()+']'
 return 'State::extendWord('+word(r)+')'
def accu(r):return str(int(r=='b'))
def ea(v):
 v=v.split(':')[-1].lstrip('?<>')
 if v.startswith('$'):return hex(num(v))+'u'
 m=re.fullmatch(r'\(r([0-7])([+-])\$([a-f0-9]+)\)',v)
 if m:return f'((s.r[{m[1]}] {m[2]} 0x{m[3]}u) & 0xffffffu)'
 m=re.fullmatch(r'\(r([0-7])\)\+n([0-7])',v)
 if m:return f's.post({m[1]}, State::sign24(s.n[{m[2]}]))'
 m=re.fullmatch(r'\(r([0-7])\)([+-]?)',v)
 if m:return f's.post({m[1]}, {1 if m[2]=="+" else -1 if m[2]=="-" else 0})'
 m=re.fullmatch(r'-\(r([0-7])\)',v)
 if m:return f's.pre({m[1]}, -1)'
 raise ValueError(v)
def mem(v):return len(v)>1 and v[1]==':'
def moves(ts):
 before=[];after=[]
 for i,t in enumerate(ts):
  src,dst=t.split(',');long=(src.startswith('l:') or dst.startswith('l:'))
  if mem(src):
   before.append(f'const auto e{i} = {ea(src)};')
   read=f's.read{"Long" if long else "X" if src.startswith("x:") else "Y"}(e{i})'
  elif long:read='s.longReg('+rr(src)+')'
  elif src.startswith('#'):
   value=num(src)
   # MOVE #xx uses fractional expansion for data registers and accumulators.
   short=('>' not in src and '<' not in src and dst in ('a','b','x0','x1','y0','y1'))
   if short:value<<=16
   read=hex(value)+'u'
  else:read=word(src)
  before.append(f'const auto v{i} = {read};')
  if mem(dst):
   before.append(f'const auto d{i} = {ea(dst)};')
   after.append(f's.write{"Long" if long else "X" if dst.startswith("x:") else "Y"}(d{i}, v{i});')
  else:after.append(f's.{'setLongReg' if long else 'setWord'}({rr(dst)}, v{i});')
 return before,after
def alu(op,args,opcode):
 aa=args.split(',');dst=accu(aa[-1])
 if op in ('mpy','mac','mpyi','maci','mpysu','macsu','dmac'):
  neg=aa[0].startswith('-');aa[0]=aa[0].lstrip('-')
  # Mixed signed/unsigned operand order is encoded by QQQQ, not textual convention.
  kind={'mpy':0,'mpyi':0,'mac':1,'maci':1,'mpysu':2,'macsu':3,'dmac':4}[op]
  return f's.multiply({dst}, {word(aa[0])}, {word(aa[1])}, {kind}, {str(neg).lower()});'
 if op in ('tfr','tgt'):
  code=f's.acc[{dst}] = {accval(aa[0])};'
  return ('if (s.condition(C::gt)) '+code) if op=='tgt' else code
 if op in ('add','sub','cmp','subr'):
  return f's.{op}({dst}, {accval(aa[0])});'
 if op in ('asl','asr'):
  shift=num(aa[0]) if len(aa)==3 else 1
  src=accu(aa[1]) if len(aa)==3 else dst
  return f's.shift({dst}, {src}, {shift}, {str(op=="asl").lower()});'
 if op in ('abs','clr','tst','rnd'):
  return f's.{op}({dst});'
 if op=='div':return f's.divide({dst}, {word(aa[0])});'
 raise ValueError((op,args,hex(opcode)))
def generate(name,start,end):
 lines=[f'inline void {name}(State& s) noexcept', '{']
 used={}
 for pc in sorted(prog):
  if not start<=pc<end:continue
  text,ws,_=prog[pc];parts=text.split();op=parts[0];toks=parts[1:]
  for tok in toks:
   if 'func_' in tok:
    target=int(tok.split('func_')[-1],16)
    if start<=target<end:used[target]=1
 loops={}
 for pc in sorted(prog):
  if not start<=pc<end:continue
  text,ws,raw=prog[pc];parts=text.split();op=parts[0];toks=parts[1:]
  if pc in loops:lines.extend(['    }']*loops[pc])
  if pc in used:lines.append(f'L{pc:06x}:;')
  lines.append(f'    // P:{pc:06X}  {text}  [{" ".join(f"{w:06X}" for w in ws)}]')
  if op=='do':
   cnt,last=toks[0].split(',');last=num(last)
   lines.append(f'    for (unsigned loop_{pc:x}=0; loop_{pc:x}<{num(cnt)}u; ++loop_{pc:x}) {{')
   loops[last]=loops.get(last,0)+1;continue
  if op=='rts':lines.append('    return;');continue
  if op=='jsr':
   target=int(toks[0].split('_')[-1],16)
   lines.append(f'    f{target:x}(s);');continue
  if op=='bra' or (op.startswith('b') and op[1:] in ['ne','eq','ec','es','cc','cs','mi','pl','ge','gt','lt','le']):
   target=int(toks[0].split('_')[-1],16)
   lines.append(f'    {"" if op=="bra" else "if (s.condition(C::"+op[1:]+")) "}goto L{target:06x};');continue
  if op in ('jset','jclr','brset','brclr'):
   bit,r,target=toks[0].split(',');target=int(target.split('_')[-1],16)
   lines.append(f'    if ((({word(r)} >> {num(bit)}) & 1u) == {int(op.endswith("set"))}u) goto L{target:06x};');continue
  if op in ('btst','bset','bclr'):
   bit,r=toks[0].split(',')
   lines.append(f'    s.bit({rr(r)}, {num(bit)}, {0 if op=="btst" else 1 if op=="bset" else -1});');continue
  if op=='andi':
   assert toks[0]=='#$fe,ccr';lines.append('    s.c = false;');continue
  if op=='lua':
   src,dst=toks[0].split(',')
   m=re.fullmatch(r'\(r([0-7])\)\+n([0-7])',src)
   val=f'((s.r[{m[1]}]+s.n[{m[2]}])&0xffffffu)' if m else ea('x:'+src)
   lines.append(f'    s.setWord({rr(dst)}, {val});');continue
  condition=None
  if toks and toks[-1].startswith('if'):condition=toks.pop()[2:]
  if op=='move':pre,post=moves(toks);operation=[]
  else:
   if op=='dmac':assert toks.pop(0)=='ss'
   arg=toks.pop(0);pre,post=moves(toks);operation=[alu(op,arg,ws[0])]
  lines.append('    {')
  lines.extend('        '+l for l in pre)
  if condition:
   assert not pre and not post
   operation=['if (s.condition(C::'+condition+')) { '+' '.join(operation)+' }']
  lines.extend('        '+l for l in operation+post)
  lines.append('    }')
 if end in loops:lines.extend(['    }']*loops[end])
 lines.append('}')
 return '\n'.join(lines)
def render():
 files={}
 mems=[{int(k):v for k,v in m.items()} for m in json.loads((ROOT/'memory.json').read_text())]
 chosen=[pc for _,a,b in regions for pc in prog if a<=pc<b]
 for pc in chosen:
  for i,w in enumerate(prog[pc][1]):assert mems[0][pc+i]==w
 tableLo,tableHi=0x141800,0x1448c6
 tables=[mems[0][a] for a in range(tableLo,tableHi)]
 text='// Extracted verbatim from OS 1.32B. See evidence/provenance.json.\n#pragma once\n#include <array>\n#include <cstdint>\nnamespace mnm132::detail {\ninline constexpr uint32_t tableBase=0x141800u;\ninline constexpr std::array<uint32_t, '+str(len(tables))+'> tables{{\n'
 text+='\n'.join('    '+', '.join(f'0x{x:06x}u' for x in tables[i:i+10])+',' for i in range(0,len(tables),10))
 text+='\n}};\n}\n';files['include/FirmwareTables.hpp']=text
 text='// Instruction-level native C++ translation; do not edit generated code.\n#pragma once\n#include "DspArithmetic.hpp"\nnamespace mnm132::detail {\n'
 text+='\n'.join('inline void '+name+'(State&) noexcept;' for name,a,b in regions)+'\n'
 text+='\n\n'.join(generate(*region) for region in regions)+'\n}\n'
 files['include/FirmwareFilterDist.hpp']=text
 files['evidence/selected_dsp.asm']='\n'.join(prog[pc][2] for pc in sorted(chosen))+'\n'
 meta={'status':'instruction translation; C++ not compiled or hardware/null tested','regions':[{'function':n,'start':hex(a),'end_exclusive':hex(b)} for n,a,b in regions],'instruction_count':len(chosen),'machine_words_verified':sum(len(prog[p][1]) for p in chosen),'table_start':hex(tableLo),'table_end_exclusive':hex(tableHi),'table_words':len(tables),'table_sha256_be24':hashlib.sha256(b''.join(w.to_bytes(3,'big') for w in tables)).hexdigest(),'extraction':json.loads((ROOT/'extraction_report.json').read_text())}
 files['evidence/provenance.json']=json.dumps(meta,indent=2)+'\n'
 return files
def main():
 for name,text in render().items():
  p=OUT/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(text,encoding='utf-8')
 print('Generated 786 instructions; 12486 table words')
if __name__=='__main__':main()
