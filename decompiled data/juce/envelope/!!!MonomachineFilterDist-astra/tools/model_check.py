"""Interpreted verification model; NOT an independent hardware/emulator oracle."""
import json,pathlib,re,math,hashlib,random
from generate_port import prog,regions,OUT,ROOT
MASK=0xffffff;M56=(1<<56)-1
def signed(x,n):
 x&=(1<<n)-1
 return x-(1<<n) if x&(1<<(n-1)) else x
def s24(x):return signed(x,24)
def val(t):return int(t.lstrip('#<>$'),16)
MEM=[{int(k):v for k,v in d.items()} for d in json.loads((ROOT/'memory.json').read_text())]
class Model:
 def __init__(self):
  self.x={};self.y={};self.reg={r:0 for r in ['a','b','x0','x1','y0','y1']+[t+str(i) for t in 'rnm' for i in range(8)]}
  for i in range(8):self.reg['m'+str(i)]=MASK
  self.sr=0x080300;self.f=dict(c=False,v=False,z=False,n=False,e=False);self.reads=set();self.writes=set();self.visited=set()
 def flags(self,x):
  x=signed(x,56);self.f['n']=x<0;self.f['z']=x==0
  lo=46 if self.sr&0x800 else 48 if self.sr&0x400 else 47
  h=(x&M56)>>lo;self.f['e']=h not in (0,(1<<(56-lo))-1)
 def limited(self,r):
  a=self.reg[r]
  if self.sr&0x800:a*=2
  elif self.sr&0x400:a>>=1
  return max(-0x800000,min(0x7fffff,a>>24))&MASK
 def word(self,r):
  if r.startswith('#'):return val(r)
  if r in ('a','b'):return self.limited(r)
  if r in ('a0','a1','a2','b0','b1','b2'):return(self.reg[r[0]]>>(24*int(r[1])))&(255 if r[1]=='2' else MASK)
  if r=='sr':return self.sr
  return self.reg[r]
 def setword(self,r,w):
  w&=MASK
  if r in ('a','b'):self.reg[r]=s24(w)<<24
  elif r in ('a0','a1','a2','b0','b1','b2'):
   shift=24*int(r[1]);mask=(255 if r[1]=='2' else MASK)<<shift
   self.reg[r[0]]=signed((self.reg[r[0]]&~mask)|((w<<(shift))&mask),56)
  else:self.reg[r]=w
 def longreg(self,r):
  if r in ('a','b'):return self.reg[r]&((1<<48)-1)
  if r in ('x','y'):return(self.reg[r+'1']<<24)|self.reg[r+'0']
  if r=='ab':return(self.limited('a')<<24)|self.limited('b')
  if r=='ba':return(self.limited('b')<<24)|self.limited('a')
  raise ValueError(r)
 def setlong(self,r,w):
  if r in ('a','b'):self.reg[r]=signed(w,48)
  elif r in ('x','y'):self.reg[r+'1']=(w>>24)&MASK;self.reg[r+'0']=w&MASK
  else:
   self.setword(r[0],w>>24);self.setword(r[1],w)
 def read(self,space,a):
  self.reads.add((space,a))
  if space=='l':return(self.read('x',a)<<24)|self.read('y',a)
  if a>=0x100000:
   assert 0x141800<=a<0x1448c6,hex(a)
   return MEM[0][a]
  assert 0<=a<2048,hex(a)
  return(self.x if space=='x' else self.y).get(a,0)
 def write(self,space,a,w):
  self.writes.add((space,a))
  assert 0<=a<2048,hex(a)
  if space=='l':self.write('x',a,w>>24);self.write('y',a,w);return
  (self.x if space=='x' else self.y)[a]=w&MASK
 def addr(self,t):
  t=t.split(':')[-1].lstrip('?<>')
  if t.startswith('$'):return val(t)
  m=re.fullmatch(r'\((r\d)([+-])\$([0-9a-f]+)\)',t)
  if m:return(self.reg[m[1]]+(1 if m[2]=='+' else -1)*int(m[3],16))&MASK
  m=re.fullmatch(r'(-?)\((r\d)\)([+-]?)(n\d)?',t);assert m,t
  before,r,op,idx=m.groups();old=self.reg[r]
  assert self.reg['m'+r[1]]==MASK
  delta=s24(self.reg[idx]) if idx else 1
  if before:self.reg[r]=(old-1)&MASK;return self.reg[r]
  if op:self.reg[r]=(old+(delta if op=='+' else -delta))&MASK
  return old
 def av(self,r):return self.reg[r] if r in ('a','b') else s24(self.word(r))<<24
 def cond(self,c):
  f=self.f
  return {'eq':f['z'],'ne':not f['z'],'cc':not f['c'],'cs':f['c'],'ec':not f['e'],'es':f['e'],'mi':f['n'],'pl':not f['n'],'ge':f['n']==f['v'],'gt':f['n']==f['v'] and not f['z'],'lt':f['n']!=f['v'],'le':f['n']!=f['v'] or f['z']}[c]
 def latch(self,ts):
  actions=[]
  for t in ts:
   src,dst=t.split(',');long=src.startswith('l:')or dst.startswith('l:')
   if ':'in src:w=self.read(src[0],self.addr(src))
   elif long:w=self.longreg(src)
   elif src.startswith('#'):
    w=val(src)
    if '>' not in src and '<'not in src and dst in ('a','b','x0','x1','y0','y1'):w<<=16
   else:w=self.word(src)
   if ':'in dst:actions.append((dst[0],self.addr(dst),w))
   else:actions.append(('long' if long else 'reg',dst,w))
  return actions
 def alu(self,op,t):
  aa=t.split(',');d=aa[-1];f=self.f;old=self.reg.get(d,0)
  if op=='tfr':self.reg[d]=self.av(aa[0]);return
  if op=='tgt':
   if self.cond('gt'):self.reg[d]=self.av(aa[0])
   return
  if op in ('mpy','mac','mpyi','maci','mpysu','macsu','dmac'):
   a=aa[0];neg=a.startswith('-');a=a.lstrip('-');x=s24(self.word(a));y=self.word(aa[1]);y=y if op.endswith('su') else s24(y)
   res=x*y*2*(-1 if neg else 1)
   if op in ('mac','maci','macsu'):res+=old
   elif op=='dmac':res+=old>>24
   f['v']=res!=signed(res,56)
  elif op in ('add','sub','cmp'):
   a=self.av(aa[0]);res=old+a if op=='add' else old-a
   f['c']=((old&M56)+(a&M56)>M56) if op=='add' else(old&M56)<(a&M56)
   f['v']=False
  elif op=='subr':res=(old>>1)-self.av(aa[0]);f['c']=(old<0)!=(signed(res,56)<0)
  elif op in ('asr','asl'):
   n=val(aa[0]) if len(aa)==3 else 1;old=self.reg[aa[1]] if len(aa)==3 else old
   if op=='asr':res=old>>n;f['c']=bool((old>>(n-1))&1);f['v']=False
   else:
    res=old<<n;f['c']=bool((old>>(56-n))&1);h=(old&M56)>>(55-n);f['v']=h not in(0,(1<<(n+1))-1)
  elif op=='abs':res=abs(old)
  elif op=='clr':res=0;f['v']=False
  elif op=='tst':res=old;f['v']=False
  elif op=='rnd':
   q=0x400000 if self.sr&0x800 else 0x1000000 if self.sr&0x400 else 0x800000
   res=old+q;mask=2*q-1
   if not(self.sr&0x200000) and not(res&mask):res&=~(2*q)
   res&=~mask;f['v']=False
  elif op=='div':
   src=s24(self.word(aa[0]));different=(old<0)!=(src<0)
   shifted=((old&M56)<<1)|int(f['c']);res=shifted+(src<<24) if different else shifted-(src<<24)
   self.reg[d]=signed(res,56);f['c']=self.reg[d]>=0;f['v']=bool((shifted>>55)&1)!=(old<0);return
  else:raise ValueError(op)
  res=signed(res,56);self.flags(res)
  if op!='cmp':self.reg[d]=res
 def run(self,start,end):
  pc=start;loops=[];stack=[];budget=0
  while pc!=end:
   budget+=1;assert budget<20000
   self.visited.add(pc);text,ws,_=prog[pc];p=text.split();op=p[0];ts=p[1:];nextpc=pc+len(ws)
   if op=='do':cnt,target=ts[0].split(',');loops.append([val(cnt),nextpc,val(target)])
   elif op=='rts':nextpc=stack.pop()
   elif op=='jsr':stack.append(nextpc);nextpc=int(ts[0].split('_')[-1],16)
   elif op=='bra':nextpc=int(ts[0].split('_')[-1],16)
   elif op.startswith('b') and op[1:] in ('ne','eq','ec','es','cc','cs','mi','pl','ge','gt','lt','le'):
    if self.cond(op[1:]):nextpc=int(ts[0].split('_')[-1],16)
   elif op in ('jset','jclr','brset','brclr'):
    bit,r,target=ts[0].split(',')
    if bool((self.word(r)>>val(bit))&1)==op.endswith('set'):nextpc=int(target.split('_')[-1],16)
   elif op in ('btst','bset','bclr'):
    bit,r=ts[0].split(',');r=r+'1'if r in('a','b')else r;bit=val(bit);w=self.word(r);self.f['c']=bool((w>>bit)&1)
    if op!='btst':
     w=w|(1<<bit) if op=='bset'else w&~(1<<bit)
     if r=='sr':self.sr=w
     else:self.setword(r,w)
   elif op=='andi':self.f['c']=False
   elif op=='lua':
    src,dst=ts[0].split(',');m=re.fullmatch(r'\((r\d)\)\+(n\d)',src)
    w=self.reg[m[1]]+self.reg[m[2]] if m else self.addr(src);self.setword(dst,w)
   else:
    condition=None
    if ts[-1].startswith('if'):condition=ts.pop()[2:]
    if op=='move':actions=self.latch(ts)
    else:
     if op=='dmac':assert ts.pop(0)=='ss'
     args=ts.pop(0);actions=self.latch(ts)
     if condition is None or self.cond(condition):self.alu(op,args)
    for typ,dst,w in actions:
     if typ=='reg':self.setword(dst,w)
     elif typ=='long':self.setlong(dst,w)
     else:self.write(typ,dst,w)
   if loops and pc+len(ws)==loops[-1][2]:
    loops[-1][0]-=1
    if loops[-1][0]:nextpc=loops[-1][1]
    else:loops.pop()
   pc=nextpc
  assert not loops and not stack
  return budget
 def block(self,left,right,raw=None,dist=64,trig=False,pitch=10240,key=0,headroom=0x200000):
  raw=raw or [0,127,0,0,0,32,64,64]
  for i,w in enumerate(raw):self.y[0x508+i]=w<<16
  self.y[0x504]=dist<<16;self.y[0x525]=key;self.x[0x501]=pitch;self.x[0x50b]=headroom;self.x[0x50f]=0;self.y[0x520]=int(trig)
  self.reg['r6']=0x500;self.reg['r7']=0x5dc;self.sr=0x080300
  self.run(0x4ff,0x56d)
  for i in range(16):self.y[0x94+i]=left[i]&MASK;self.y[0xd4+i]=right[i]&MASK
  self.reg['r7']=0x5e0;self.run(0x5bf,0x88e)
  return [[signed(self.read('l',a+i),48) for i in range(16)] for a in (0x6d,0xad)]
def tests():
 # Arithmetic hazards that caused incorrect earlier research.
 m=Model();m.setword('a',0xffffff);assert m.reg['a']==-(1<<24)
 m.reg['a']=(1<<47)+10;assert m.word('a')==0x7fffff
 m.reg['a']=-(1<<47)-1;assert m.word('a')==0x800000
 for n,expected in [(0,0),(1,2),(2,2),(-1,0),(-2,-2)]:
  m.reg['a']=(n<<24)+0x800000;m.alu('rnd','a');assert m.reg['a']==expected<<24
 m.reg['a']=6<<24;m.reg['b']=10<<24;m.alu('subr','a,b');assert m.reg['b']==-1<<24
 # The short fractional MOVE #$10,x0 is 0x100000, NOT 0x10.
 actions=m.latch(['#$10,x0']);assert actions[0][2]==0x100000
 for dividend,divisor,expected in [(1,0x400000,2),(0x800,0x400000,0x1000),(0x80000,0x200000,0x200000)]:
  m.reg['a']=dividend<<24;m.reg['x0']=divisor;m.f['c']=False
  for _ in range(24):m.alu('div','x0,a')
  assert m.word('a0')==expected
 q=Model();silence=[0]*16
 for _ in range(10):assert all((v>>24)==0 for ch in q.block(silence,silence) for v in ch)
 out=[];allread=set();allpc=set();traces=[]
 for raw,dist in [([0,127,0,0,0,32,64,64],64),([24,48,80,90,16,64,96,32],80),([0,127,0,0,127,127,127,0],0)]:
  model=Model();blocks=[]
  for b in range(18):
   l=[int(0x100000*math.sin(2*math.pi*220*(b*16+i)/44100)) for i in range(16)];r=[0]*16
   y=model.block(l,r,raw,dist,b==0);blocks.append(y)
  assert all((v>>24)==0 for yy in blocks for v in yy[1]),'crosstalk in signal words'
  allread|=model.reads;allpc|=model.visited
  traces.append({'parameters':raw,'dist':dist,'peak_raw':max(abs(v) for yy in blocks for v in yy[0]),'envelope':model.x.get(0x5db),'phase':model.y.get(0x5db),'hp_index':s24(model.x.get(0x5d9,0)),'lp_index':s24(model.y.get(0x5da,0)),'last_output':blocks[-1]})
 # DIST changes audio, not envelope trajectory. BOFS/WOFS change indices.
 env=[]
 for dist in (0,32,64,96,127):
  m=Model();trajectory=[]
  for b in range(12):
   m.block(silence,silence,[24,48,40,40,64,32,96,96],dist,b==0)
   trajectory.append((m.x.get(0x5db),m.y.get(0x5db),m.x.get(0x5d9),m.y.get(0x5da)))
  env.append(trajectory)
 assert all(t==env[0]for t in env)
 # Both resonance knobs and both envelope offsets must affect the selected slice.
 def response(raw):
  m=Model();result=[]
  for b in range(6):
   signal=[0x100000 if b==0 and i==0 else 0 for i in range(16)]
   result.append(m.block(signal,signal,raw,64,b==0))
  return result,(m.x.get(0x5d9),m.y.get(0x5da))
 controls=[24,48,16,16,0,64,64,64]
 reference,indices=response(controls)
 for index in (2,3):
  altered=controls.copy();altered[index]=96
  assert response(altered)[0]!=reference,'resonance control does not affect audio'
 for index in (6,7):
  altered=controls.copy();altered[index]=96
  assert response(altered)[1]!=indices,'filter envelope offset does not affect indices'
 # The pre-ALU accumulator stores at the FIR entry must not leak EQ accumulator state.
 altered=Model();normal=Model();run=altered.run
 def entry_scramble(start,end):
  if start==0x5bf:
   altered.reg['a']=123456<<24;altered.reg['b']=-123456<<24
  return run(start,end)
 altered.run=entry_scramble
 for _ in range(3):
  assert altered.block([12345]*16,[6789]*16)==normal.block([12345]*16,[6789]*16)
 excluded=set(range(0x56d,0x5bf))|set(range(0x88e,0xb4e))
 assert not(allpc&excluded)
 # Fixed seed, legal control extremes, pitch/key-track routes, and retriggers.
 rng=random.Random(132)
 cases=[]
 for i in range(72):
  raw=[rng.choice([0,1,63,64,126,127]) for _ in range(8)]
  dist=rng.choice([0,1,63,64,65,126,127]);key=rng.choice([0,1<<9,1<<11,(1<<9)|(1<<11)])
  pitch=rng.choice([0,2048,10240,21674,0x5800]);mm=Model()
  for b in range(3):
   sig=[rng.randint(-0x100000,0x100000) for _ in range(16)]
   mm.block(sig,sig,raw,dist,True,pitch,key)
  cases.append({'raw':raw,'dist':dist,'pitch':pitch,'key_bits':key})
  allpc|=mm.visited;allread|=mm.reads
 report={'cpp_compiled':False,'independent_oracle':False,'model':'Python instruction model, using official DSP arithmetic rules; not a sample-for-sample validation of C++','passed':['24-bit sign extension','positive and negative limited transfers','convergent rounding ties','SUBR half-destination semantics','8-bit fractional immediate expansion','zero input zero output in X signal words (Y low-word scratch retained)','left input does not leak into right channel X signal words','DIST independent of filter envelope and envelope-derived indices','no execution of EQ/AMP/SRR/delay regions'],'visited_instructions':len(allpc),'table_read_min':hex(min(a for s,a in allread if a>=0x100000)),'table_read_max':hex(max(a for s,a in allread if a>=0x100000)),'scenarios':traces}
 (OUT/'evidence/model_checks.json').write_text(json.dumps(report,indent=2)+'\n')
 report['passed']+=['24-step DIV quotient vectors','HPQ and LPQ each affect audio','BOFS and WOFS each affect filter indices','audio result independent of incoming EQ A/B registers in three-block scenario','72 extreme-control/key-tracking scenarios with bounded table accesses and retriggers']
 report['extreme_scenarios']=cases
 (OUT/'evidence/model_checks.json').write_text(json.dumps(report,indent=2)+'\n')
 print('PASS: Python model; '+str(len(allpc))+' instructions visited; 72 extreme scenarios. C++/independent oracle not run.')
if __name__=='__main__':tests()
