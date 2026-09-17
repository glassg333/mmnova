#pragma once
#include "FirmwarePrimitives.h"
#include <array>
namespace nova::fx {
struct PitchSettings {float semitones=0,cents=0,windowMs=80,mix=100;};
class Pitch {
public:
 void prepare(double sampleRate){sr=sampleRate;history.prepare(int(sr*0.3)+32);smooth=1-std::exp(-1/(sr*.02));reset();}
 void reset(){history.clear();phase=0;ratio=1;windowSamples=sr*.08;wetMix=1;filtered={};}
 void set(PitchSettings p){targetRatio=std::pow(2.0,std::clamp(double(p.semitones)+p.cents*.01,-25.0,25.0)/12.0);targetWindow=sr*std::clamp(double(p.windowMs),20.0,160.0)/1000;targetMix=std::clamp(p.mix*.01f,0.f,1.f);cut=1-std::exp(-6.283185307179586*.43/std::max(1.0,targetRatio));}
 void recordBypassed(Frame in){history.push(in);filtered=in;ratio=targetRatio;windowSamples=targetWindow;wetMix=targetMix;}
 Frame process(Frame in){
  ratio+=(targetRatio-ratio)*smooth;windowSamples+=(targetWindow-windowSamples)*smooth;wetMix+=float((targetMix-wetMix)*smooth);
  filtered=filtered+(in+filtered*(-1))*float(cut);
  const auto now=history.sizeWritten();history.push(ratio>1.001?filtered:in);
  if(std::abs(ratio-1)<1.e-7&&std::abs(targetRatio-1)<1.e-7)return in;
  phase+=(1-ratio)/windowSamples;phase-=std::floor(phase);double other=phase+.5;other-=std::floor(other);
  const float w=window(phase),v=window(other);const float total=std::max(.0001f,w+v);
  const Frame out=(history.read(double(now)-4-phase*windowSamples)*w+history.read(double(now)-4-other*windowSamples)*v)*(1.f/total);
  return fx::mix(in,out,wetMix);
 }
private:
 History history;double sr=44100,smooth=.001,phase=0,ratio=1,targetRatio=1,windowSamples=3528,targetWindow=3528,cut=.9;float wetMix=1,targetMix=1;Frame filtered{};
};
struct GrainSettings {float sizeMs=100,density=20,positionMs=250,sprayMs=120,pitch=0,jitter=0,width=80,mix=60,feedback=65,metal=40,tone=7000,reverse=0;bool freeze=false;};
class Granular {
public:
 static constexpr int maxGrains=48;
 void prepare(double sampleRate){sr=sampleRate;history.prepare(int(sr*8)+16);for(int i=0;i<3;i++)comb[std::size_t(i)].prepare(int(sr*.006)+8);smooth=1-std::exp(-1/(sr*.025));dcPole=std::exp(-6.283185307179586*20/sr);reset();}
 void reset(){history.clear();for(auto& c:comb)c.clear();for(auto& g:grains)g={};random=0x563003;countdown=0;lastWet={};low={};dcIn={};dcOut={};fb=0;metal=0;wetMix=.6f;activeCount=0;}
 void set(GrainSettings settings){p=settings;p.sizeMs=std::clamp(p.sizeMs,10.f,500.f);p.density=std::clamp(p.density,1.f,80.f);p.feedback=std::clamp(p.feedback,0.f,99.5f);p.metal=std::clamp(p.metal,0.f,100.f);p.tone=std::clamp(p.tone,300.f,float(std::min(16000.,sr*.45)));tonePole=1-std::exp(-6.283185307179586*p.tone/sr);}
 void recordBypassed(Frame in){
  if(!asleep){for(auto& g:grains)g.active=false;for(auto& c:comb)c.clear();lastWet={};low={};dcIn={};dcOut={};countdown=0;activeCount=0;fb=0;asleep=true;}
  if(!p.freeze)history.push(in);
 }
 Frame process(Frame in){
  asleep=false;
  fb+=float((p.feedback*.01-fb)*smooth);metal+=float((p.metal*.01-metal)*smooth);wetMix+=float((std::clamp(p.mix*.01f,0.f,1.f)-wetMix)*smooth);
  const Frame dc=lastWet+dcIn*(-1)+dcOut*float(dcPole);dcIn=lastWet;dcOut=dc;
  low=low+(dc+low*(-1))*float(tonePole);
  Frame colored=low;
  constexpr double delayMs[3]={1.13,2.71,4.37};
  for(int i=0;i<3;i++){
   auto& c=comb[std::size_t(i)];const Frame tap=c.read(double(c.sizeWritten())-delayMs[i]*.001*sr);
   const Frame value=tap+colored*(-.72f);c.push(colored+value*.72f);colored=value;
  }
  Frame ret=fx::mix(low,colored,metal)*fb;
  ret={2.f*std::tanh(ret.l*.5f),2.f*std::tanh(ret.r*.5f)};
  if(!p.freeze)history.push({std::clamp(clean(in.l)+ret.l,-4.f,4.f),std::clamp(clean(in.r)+ret.r,-4.f,4.f)});
  if(--countdown<=0){spawn();countdown+=sr/p.density;}
  Frame sum{},feedbackSum{};float weights=0;activeCount=0;
  for(auto& g:grains){if(!g.active)continue;++activeCount;const float w=window(double(g.age)/g.length);
   const Frame sample=p.freeze?history.frozen(g.position):history.read(g.position);feedbackSum=feedbackSum+sample*w;
   // A bounded stereo-width/pan law; no > unity feedback gain from panning.
   const float mid=(sample.l+sample.r)*.5f,side=(sample.l-sample.r)*.5f;
   const Frame spread={mid+side*g.width,mid-side*g.width};
   sum=sum+Frame{spread.l*g.panL,spread.r*g.panR}*w;weights+=w;
   g.position+=g.rate;if(++g.age>=g.length)g.active=false;
  }
  const float normalise=1.f/std::max(1.f,weights);lastWet=feedbackSum*normalise;
  return fx::mix(in,sum*normalise,wetMix);
 }
 int grainsActive()const{return activeCount;}
private:
 struct Grain {double position=0,rate=1;int age=0,length=1;float panL=1,panR=1,width=1;bool active=false;};
 float rand01(){random^=random<<13;random^=random>>17;random^=random<<5;return float(random>>8)/16777216.f;}
 void spawn(){
  if(history.sizeWritten()<4)return;
  for(auto& g:grains)if(!g.active){
   g.length=std::max(16,int(p.sizeMs*.001*sr));g.age=0;
   const double semi=std::clamp(double(p.pitch)+(rand01()*2-1)*p.jitter*.01,-36.,36.);
   g.rate=std::pow(2.0,semi/12);if(rand01()*100<p.reverse)g.rate=-g.rate;
   const double advance=std::max(0.,g.rate-1)*g.length;
   const double offset=std::clamp(double(p.positionMs)+(rand01()*2-1)*p.sprayMs,0.,3000.)*.001*sr;
   g.position=double(history.sizeWritten()-2)-std::max(4.,offset+advance);
   g.width=std::clamp(p.width*.01f,0.f,1.f);
   const float pan=(rand01()*2-1)*g.width*.35f;g.panL=std::min(1.f,1-pan);g.panR=std::min(1.f,1+pan);
   g.active=true;break;
  }
 }
 double sr=44100,smooth=.001,countdown=0,dcPole=.99,tonePole=.5;
 GrainSettings p{};History history;std::array<History,3> comb;std::array<Grain,maxGrains> grains{};
 Frame lastWet{},low{},dcIn{},dcOut{};float fb=0,metal=0,wetMix=.6f;std::uint32_t random=0x563003;int activeCount=0;bool asleep=true;
};
}
