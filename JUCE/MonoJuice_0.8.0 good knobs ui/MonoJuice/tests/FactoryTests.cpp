#include "PatchSettings.h"
#include "../Reference/ClassicOracle.h"
#include "../Reference/Odin_0_5_0.h"
#include "../Reference/Factory_0_7.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <set>
using nova::fx::Frame;
void check(bool b,const char* message){if(!b)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
 nova::fx::MDCTPlans plans;plans.prepare();nova::classicOracle::MDCTPlans oraclePlans;oraclePlans.prepare();auto slot=std::make_unique<nova::rack::Slot>();
 for(int w=0;w<8;w++){
  nova::rack::SlotSettings s;s.type=5;s.on=true;s.spectre={23,100,w,2000,4000,1,8,-6,100,500,12,true};s.spectralExtended=false;slot->prepare(48000,plans,s);
  nova::classicOracle::Spectre old;old.prepare(oraclePlans);old.set({23,100,w});old.reset();
  for(int i=0;i<(512<<w)*3+100;i++){auto x=testInput(i,48000),a=slot->process(x),b=old.process(x);check(a.l==b.l&&a.r==b.r,"CLASSIC not exact 0.5 or leaking extension controls");}
  std::cout<<"CLASSIC exact 0.5, ignores all extension controls / window "<<(512<<w)<<" PASS\n";
 }
 for(int sr:{44100,48000,96000})for(int route=0;route<3;route++)for(int sign=0;sign<4;sign++){
  nova::fx::OdinSettings o{337,93,541,91,38,100,route,bool(sign&1),bool(sign&2)};nova::fx::CombColour current;current.prepare(sr);nova::fx::ColourSettings c;c.model=0;current.set({},o,c);current.reset();nova::odinOracle::OdinComb old;old.prepare(sr);old.set({337,93,541,91,38,100,route,bool(sign&1),bool(sign&2)});old.reset();
  for(int i=0;i<5000;i++){auto x=testInput(i,sr),a=current.process(x),b=old.process(x);check(a.l==b.l&&a.r==b.r,"ODIN original neutral wrapper");}
 }
 std::cout<<"36 ODIN legacy route/polarity/rate comparisons exact PASS\n";
 for(int os=0;os<2;os++){nova::fx::CombSettings p;p.delayMs={1.3f,2.1f,3.3f,5.3f};p.feedback=23;p.oversample=os!=0;nova::fx::ColourSettings c;c.model=1;nova::fx::CombColour a;a.prepare(48000);a.set(p,{},c);a.reset();nova::fx::ParallelCombModel b;b.prepare(48000);b.set(p);b.reset();for(int i=0;i<12000;i++){auto x=testInput(i,48000),y=a.process(x),z=b.process(x);check(y.l==z.l&&y.r==z.r,"Parallel neutral wrapper");}}
 std::cout<<"Parallel kernel neutral creative layer exact PASS\n";
 for(int sr:{44100,48000,96000}){
  slot->prepare(sr,plans,{});std::set<uint64_t> hashes;
  for(int type:{4,5})for(int n=0;n<10;n++){
   const auto s=patchSlot(type,factory::preset(type,n));slot->set(s);slot->clear();double energy=0,peak=0,difference=0;uint64_t h=0;
   for(int i=0;i<sr;i++){auto x=testInput(i,sr),y=slot->process(x);check(std::isfinite(y.l)&&std::isfinite(y.r),"preset finite");energy+=y.l*y.l+y.r*y.r;difference+=(x.l-y.l)*(x.l-y.l);peak=std::max(peak,double(std::max(std::abs(y.l),std::abs(y.r))));uint32_t b;std::memcpy(&b,&y.l,4);h=h*1099511628211ull+b;}
   check(energy>1e-8&&peak<4&&difference>1e-6,"silent / excessive / unchanged preset");check(hashes.insert(h).second,"duplicate preset audio");std::cout<<"Preset "<<type<<" / "<<n+1<<" @ "<<sr<<" peak="<<peak<<" energy="<<energy<<" difference="<<difference<<" PASS\n";
  }
  for(int type=1;type<=5;type++)for(int variant=0;variant<((type>=4)?2:1);variant++)for(uint32_t seed=0;seed<4;seed++){
   const auto p=factory::randomize(type,seed,variant);check(p==factory::randomize(type,seed,variant),"random deterministic seed");auto s=patchSlot(type,p);slot->set(s);slot->clear();double energy=0;for(int i=0;i<sr/2;i++){auto y=slot->process(testInput(i,sr));check(std::isfinite(y.l)&&std::isfinite(y.r)&&std::abs(y.l)<8,"random audio");energy+=y.l*y.l;}check(energy>1e-8,"random silence");
  }
  std::cout<<"RANDOM all types and both model variants @ "<<sr<<" PASS\n";
 }
 // Envelope tuning must produce movement, not merely relabel old delay patches.
 for(int n=5;n<10;n++){
  auto s=patchSlot(4,factory::preset(4,n));auto flat=s.colour;flat.envelope=0;flat.depth=0;nova::fx::CombColour a,b;a.prepare(48000);b.prepare(48000);a.set(s.comb,s.odin,s.colour);b.set(s.comb,s.odin,flat);a.reset();b.reset();double e=0;
  for(int i=0;i<48000;i++){auto x=testInput(i,48000),y=a.process(x),z=b.process(x);e+=(y.l-z.l)*(y.l-z.l)+(y.r-z.r)*(y.r-z.r);}check(e>1e-5,"inactive comb motion");std::cout<<"Rubber preset "<<n+1<<" motion difference="<<e<<" PASS\n";
 }
 auto rack=std::make_unique<nova::rack::Rack>();nova::rack::Settings empty;for(auto& s:empty.slots)s.type=0;
 for(float feedback:{50.f,-50.f}){auto s=empty;s.feedback=feedback;rack->prepare(48000,s);for(int i=0;i<32;i++){auto y=rack->process(i==0?Frame{.25f,-.125f}:Frame{});check(std::abs(y.l-.25f*std::pow(feedback*.01f,i))<1e-8f,"one-sample feedback recurrence");check(std::abs(y.r+.125f*std::pow(feedback*.01f,i))<1e-8f,"stereo feedback isolation");}}
 {auto s=empty;s.feedback=50;rack->reset(s);auto a=rack->process({.5f,.25f},false,true),b=rack->process({},false,true);check(a.l==.375f&&a.r==.375f&&b.l==.1875f,"mono return after downmix");}
 {auto s=empty;s.feedback=50;s.clipper=true;rack->reset(s);auto a=rack->process({2,-2}),b=rack->process({});check(a.l==1&&a.r==-1&&b.l==.5f&&b.r==-.5f,"feedback return must follow output clipper");}
 {auto s=empty;s.dcBlock=true;rack->reset(s);Frame y;for(int i=0;i<48000;i++)y=rack->process({.25f,-.4f});check(std::abs(y.l)<1e-8&&std::abs(y.r)<1e-8,"DC rejection");s.dcBlock=false;rack->set(s);y=rack->process({.25f,-.4f});check(y.l==.25f&&y.r==-.4f,"DC bypass exact");}
 {auto s=empty;s.clipper=true;rack->reset(s);for(float v:{-8.f,-1.f,-.2f,0.f,.2f,1.f,8.f}){auto y=rack->process({v,-v});check(y.l==std::clamp(v,-1.f,1.f)&&y.r==std::clamp(-v,-1.f,1.f),"hard clip bound");}s.clipper=false;rack->set(s);check(rack->process({3,-3}).l==3,"clip OFF is not hidden limiter");}
 {auto s=empty;s.feedback=90;s.clipper=true;s.dcBlock=true;for(auto& v:s.slots)v=patchSlot(4,factory::preset(4,3));rack->reset(s);for(int i=0;i<24000;i++){auto y=rack->process(testInput(i,48000));check(std::isfinite(y.l)&&std::abs(y.l)<=1&&std::abs(y.r)<=1,"high feedback clipper safety");}s.bypass=true;rack->set(s);for(int i=0;i<48000;i++)rack->process({});auto y=rack->process({});check(std::abs(y.l)<1e-5,"bypass cuts output loop");rack->reset(empty);check(rack->process({}).l==0,"reset clears loop");}
 // Repeat automation snapshots at different block boundaries; same audio.
 {auto s=empty;s.slots[0]=patchSlot(4,factory::preset(4,7));s.feedback=35;s.dcBlock=true;s.clipper=true;std::vector<Frame> a(18000);rack->reset(s);for(int i=0;i<int(a.size());i++)a[i]=rack->process(testInput(i,48000));rack->reset(s);for(int i=0;i<int(a.size());i++){if(i%127==0)rack->set(s);auto b=rack->process(testInput(i,48000));check(a[i].l==b.l&&a[i].r==b.r,"block independent modulation / feedback");}}
 std::cout<<"MASTER bipolar one-sample loop / post-clip return / DC / clip OFF+ON / high feedback / bypass / reset / block boundaries PASS\n";
 if(argc>1){slot->prepare(44100,plans,patchSlot(4,factory07::preset(4,0)));for(int type:{4,5}){std::ofstream out(std::string(argv[1])+(type==4?"_COMB_AB.raw":"_CLASSIC.raw"),std::ios::binary);for(int n=0;n<10;n++)for(int version=type==4?0:1;version<=1;version++){auto patch=version?factory::preset(type,n):factory07::preset(type,n);auto s=patchSlot(type,patch);slot->set(s);slot->clear();for(int i=0;i<4*44100;i++){auto y=slot->process(i<2*44100?testInput(i,44100):Frame{});out.write(reinterpret_cast<const char*>(&y),sizeof y);}}}}
 std::cout<<"ALL MONOJUICE 0.8 NATIVE TESTS PASS\n";return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
