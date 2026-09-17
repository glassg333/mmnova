#pragma once
#include "AudioAdapter.h"
#include "NewEffects.h"
#include "CombColour.h"
#include "SpectreClassic.h"
#include "Spectre.h"
namespace nova::rack {
using fx::Frame;
enum Type {Empty=0,Pitch=1,Chorus=2,Granular=3,Comb=4,Spectre=5};
// Per-slot LFO: one bipolar [-1,1] modulator per slot. Each depth is expressed
// in its target parameter's own units and adds depth*value to the knob value.
// All depths default to 0, so a fresh slot renders exactly as without an LFO.
struct LFOSettings {float rate=1,shape=0,sync=0,div=3;std::array<float,4> pitch{};std::array<float,8> chorus{};std::array<float,12> grain{};std::array<float,18> comb{};std::array<float,6> odin{};std::array<float,7> colour{};std::array<float,10> spectre{};};
struct SlotSettings {int type=Empty;bool on=false;fx::PitchSettings pitch;std::array<int,8> chorus=ChorusCore::defaults;fx::GrainSettings grain;fx::CombSettings comb;fx::SpectreSettings spectre;fx::OdinSettings odin;fx::ColourSettings colour;bool spectralExtended=false;int chorusMode=0;LFOSettings lfo;};
struct Settings {std::array<SlotSettings,5> slots;bool bypass=false;float feedback=0;bool clipper=false,dcBlock=false;Settings(){slots[0].type=Pitch;slots[1].type=Chorus;slots[1].on=true;slots[2].type=Granular;}};
// User CHORUS MIX (index 3) is applied outside the DSP core: the core always
// runs with internal MIX pinned to 127 (wet), and the slot crossfades its
// output with the clean, latency-aligned dry. So MIX=0 is bit-clean input at
// any INP, like every other effect's mix. The core itself is untouched.
inline std::array<int,8> coreChorusParams(const std::array<int,8>& p){auto c=p;c[3]=127;return c;}
inline float chorusMix01(const std::array<int,8>& p){return std::clamp(float(p[3])/127.f,0.f,1.f);}
// Sync divisions in beats: 8, 4, 2, 1, 1/2, 1/4, 1/8, 1/16, 1/32.
inline constexpr float lfoDivBeats[9]={8,4,2,1,.5f,.25f,.125f,.0625f,.03125f};
// Shapes: 0 SINE, 1 TRI, 2 SQUARE, 3 SAW, 4 SAMPLE&HOLD. Bipolar [-1,1].
inline float lfoShape(float shape,float phase,float hold) noexcept {
 const int s=std::clamp(int(shape+.5f),0,4);
 if(s==1)return phase<.5f?4*phase-1:3-4*phase;
 if(s==2)return phase<.5f?1.f:-1.f;
 if(s==3)return 2*phase-1;
 if(s==4)return hold;
 return float(std::sin(6.283185307179586*phase));
}
class Slot {
public:
 void prepare(double rate,const fx::MDCTPlans& bank,SlotSettings s){chorusLatency=chorus.prepare(rate);pitch.prepare(rate);grain.prepare(rate);comb.prepare(rate);spectre.prepare(bank,rate);classicSpectre.prepare(bank);dry.prepare(fx::spectreMax+1024);step=float(1/(rate*.02));slotRate=float(rate);settings=s;set(s);clear();amount=s.on?1:0;}
 void setTempo(double ppq,double ppqPerSample,bool playing) noexcept {if(std::isfinite(ppq))ppqPos=ppq;ppqInc=(std::isfinite(ppqPerSample)&&ppqPerSample>=0)?ppqPerSample:0;tempoPlaying=playing;}
 bool set(SlotSettings s){s.type=std::clamp(s.type,0,5);s.chorusMode=(s.chorusMode==1)?1:0;const bool structural=s.type!=settings.type||(s.type==Spectre&&(s.spectre.window!=settings.spectre.window||s.spectralExtended!=settings.spectralExtended))||(s.type==Comb&&s.colour.model!=settings.colour.model);settings=s;pitch.set(s.pitch);grain.set(s.grain);chorus.setSmoothing(s.chorusMode==1);chorus.setParameters(s.chorusMode==1?coreChorusParams(s.chorus):s.chorus);chorusMixTarget=chorusMix01(s.chorus);comb.set(s.comb,s.odin,s.colour);spectre.set(s.spectre);classicSpectre.set({s.spectre.prominence,s.spectre.mix,s.spectre.window});lfoActive=hasModulation(s);if(structural)clear();return structural;}
 void clear(){dry.clear();lfoPhase=0;lfoValue=0;lfoRand=0x2b6f48;lfoHold=drawLFO();modTick=0;switch(settings.type){case Pitch:pitch.reset();break;case Chorus:chorus.reset(settings.chorusMode==1?coreChorusParams(settings.chorus):settings.chorus);chorusMix=chorusMixTarget;break;case Granular:grain.reset();break;case Comb:comb.reset();break;case Spectre:spectre.reset();classicSpectre.reset();break;default:break;}}
 int latency()const{return settings.type==Chorus?chorusLatency:(settings.type==Spectre?spectre.latency():0);}
 int grains()const{return settings.type==Granular?grain.grainsActive():0;}
 Frame process(Frame in){const auto at=dry.sizeWritten();dry.push(in);const auto cleanDry=dry.at(at-latency());amount=settings.on?std::min(1.f,amount+step):std::max(0.f,amount-step);Frame wet=in;
  const float lfo=lfoActive?stepLFO():0.f;
  if(lfoActive)pushModulation(lfo);
  switch(settings.type){
   case Pitch:if(amount>0)wet=pitch.process(in);else pitch.recordBypassed(in);break;
   case Chorus:chorus.processFrame(in.l,in.r,wet.l,wet.r,false);
    if(settings.chorusMode==1){if(chorusMix!=chorusMixTarget)chorusMix+=std::clamp(chorusMixTarget-chorusMix,-step,step);const float m=lfoActive?std::clamp(chorusMix+settings.lfo.chorus[3]*lfo/127.f,0.f,1.f):chorusMix;wet=fx::mix(cleanDry,wet,m);}
    break;
   case Granular:if(amount>0)wet=grain.process(in);else grain.recordBypassed(in);break;
   case Comb:wet=comb.process(in);break;
   case Spectre:wet=settings.spectralExtended?spectre.process(in):classicSpectre.process(in);break;
   default:return in;
  }return fx::mix(cleanDry,wet,amount);
 }
private:
 static bool hasModulation(const SlotSettings& s) noexcept {auto any=[](const auto& a){for(float d:a)if(d!=0.f)return true;return false;};
  switch(s.type){case Pitch:return any(s.lfo.pitch);case Chorus:return any(s.lfo.chorus);case Granular:return any(s.lfo.grain);case Comb:return any(s.lfo.comb)||any(s.lfo.odin)||any(s.lfo.colour);case Spectre:return any(s.lfo.spectre);default:return false;}}
 float drawLFO() noexcept {lfoRand^=lfoRand<<13;lfoRand^=lfoRand>>17;lfoRand^=lfoRand<<5;return float(lfoRand>>8)/16777216.f*2-1;}
 float stepLFO() noexcept {const auto& L=settings.lfo;
  if(L.sync>=.5f){const int di=std::clamp(int(L.div+.5f),0,8);if(tempoPlaying)ppqPos+=ppqInc;double phase=ppqPos/lfoDivBeats[di];phase-=std::floor(phase);if(float(phase)<lfoPhase)lfoHold=drawLFO();lfoPhase=float(phase);}
  else{const float next=lfoPhase+L.rate/slotRate;if(std::floor(next)!=std::floor(lfoPhase))lfoHold=drawLFO();lfoPhase=next-std::floor(next);}
  lfoValue=lfoShape(L.shape,lfoPhase,lfoHold);return lfoValue;}
 // Chorus adapter target/direct writes are plain stores, so they track the
 // LFO every sample; the heavier effect set() calls run every 16 samples
 // (same grid as the comb colour motion LFO) while their own smoothers iron
 // the steps. All modulation goes through the clamping set() entry points.
 void pushModulation(float lfo) noexcept {const bool tick=(modTick++%16)==0;const auto& L=settings.lfo;
  switch(settings.type){
   case Pitch:if(tick){auto p=settings.pitch;p.semitones+=L.pitch[0]*lfo;p.cents+=L.pitch[1]*lfo;p.windowMs+=L.pitch[2]*lfo;p.mix+=L.pitch[3]*lfo;pitch.set(p);}break;
   case Chorus:{std::array<float,8> f{};for(int i=0;i<8;i++)f[i]=float(settings.chorus[i])+L.chorus[i]*lfo;if(settings.chorusMode==1)f[3]=127.f;chorus.setParametersF(f);}break;
   case Granular:if(tick){auto p=settings.grain;p.sizeMs+=L.grain[0]*lfo;p.density+=L.grain[1]*lfo;p.positionMs+=L.grain[2]*lfo;p.sprayMs+=L.grain[3]*lfo;p.pitch+=L.grain[4]*lfo;p.jitter+=L.grain[5]*lfo;p.width+=L.grain[6]*lfo;p.mix+=L.grain[7]*lfo;p.feedback+=L.grain[8]*lfo;p.metal+=L.grain[9]*lfo;p.tone+=L.grain[10]*lfo;p.reverse+=L.grain[11]*lfo;grain.set(p);}break;
   case Comb:if(tick){auto c=settings.comb;for(int i=0;i<4;i++)c.delayMs[i]+=L.comb[i]*lfo;c.multiplier+=L.comb[4]*lfo;c.feedback+=L.comb[5]*lfo;c.highpass+=L.comb[6]*lfo;c.limiterMs+=L.comb[7]*lfo;c.rate+=L.comb[8]*lfo;c.smoothMs+=L.comb[9]*lfo;c.lean+=L.comb[10]*lfo;c.cross+=L.comb[11]*lfo;c.am+=L.comb[12]*lfo;c.gateDB+=L.comb[13]*lfo;c.gateMs+=L.comb[14]*lfo;c.dryDB+=L.comb[15]*lfo;c.wetDB+=L.comb[16]*lfo;c.tune+=L.comb[17]*lfo;
    auto o=settings.odin;o.freqA+=L.odin[0]*lfo;o.resA+=L.odin[1]*lfo;o.freqB+=L.odin[2]*lfo;o.resB+=L.odin[3]*lfo;o.balance+=L.odin[4]*lfo;o.mix+=L.odin[5]*lfo;
    auto k=settings.colour;k.drive+=L.colour[0]*lfo;k.mix+=L.colour[1]*lfo;k.output+=L.colour[2]*lfo;k.depth+=L.colour[3]*lfo;k.rate+=L.colour[4]*lfo;k.envelope+=L.colour[5]*lfo;k.decay+=L.colour[6]*lfo;comb.set(c,o,k);}break;
   case Spectre:if(tick){auto p=settings.spectre;p.prominence+=L.spectre[0]*lfo;p.mix+=L.spectre[1]*lfo;p.low+=L.spectre[2]*lfo;p.high+=L.spectre[3]*lfo;p.peaks=int(float(p.peaks)+L.spectre[4]*lfo);p.width=int(float(p.width)+L.spectre[5]*lfo);p.reject+=L.spectre[6]*lfo;p.attack+=L.spectre[7]*lfo;p.release+=L.spectre[8]*lfo;p.shift+=L.spectre[9]*lfo;spectre.set(p);classicSpectre.set({settings.spectre.prominence+L.spectre[0]*lfo,settings.spectre.mix+L.spectre[1]*lfo,settings.spectre.window});}break;
   default:break;}}
 SlotSettings settings;fx::Pitch pitch;fx::Granular grain;AudioAdapter chorus;fx::CombColour comb;fx::Spectre spectre;classic::Spectre classicSpectre;fx::History dry;int chorusLatency=0;float amount=0,step=.001f,chorusMix=1,chorusMixTarget=1;
 float slotRate=44100,lfoPhase=0,lfoValue=0,lfoHold=0;double ppqPos=0,ppqInc=0;bool tempoPlaying=true,lfoActive=false;std::uint32_t lfoRand=0x2b6f48;unsigned modTick=0;
};
class Rack {
public:
 int prepare(double rate,Settings s){plans.prepare();for(int i=0;i<5;i++){if(!slots[i])slots[i]=std::make_unique<Slot>();slots[i]->prepare(rate,plans,s.slots[i]);}dry.prepare(5*(fx::spectreMax+1024));step=float(1/(rate*.02));tempoRate=rate;settings=s;bypass=s.bypass?1:0;ramp=1;dcR=std::exp(-6.283185307179586*10/rate);resetOutput();updateLatency();return latencySamples;}
 // DAW tempo for synced slot LFOs. Call once per block; free-running LFOs
 // ignore it. Allocation-free. Falls back to 120 BPM on garbage input.
 void setTempo(double bpm,double ppq,bool playing){const double b=(std::isfinite(bpm)&&bpm>0)?bpm:120;const double inc=b/60/tempoRate;for(auto& slot:slots)slot->setTempo(ppq,inc,playing);}
 void reset(Settings s){set(s);for(auto& slot:slots)slot->clear();dry.clear();bypass=s.bypass?1:0;ramp=1;resetOutput();}
 bool set(Settings s){bool changed=false;for(int i=0;i<5;i++)changed=slots[i]->set(s.slots[i])||changed;settings=s;updateLatency();if(changed){ramp=0;returned={};}if(s.dcBlock!=lastDC){dcX={};dcY={};lastDC=s.dcBlock;}return changed;}
 void clearSlot(int i){if(i>=0&&i<5)slots[i]->clear();}
 Frame process(Frame in,bool forcedBypass=false,bool monoOutput=false){in={fx::clean(in.l),fx::clean(in.r)};const auto at=dry.sizeWritten();dry.push(in);const bool target=settings.bypass||forcedBypass;
  const float wanted=target?0.f:std::clamp(settings.feedback*.01f,-.99f,.99f);fb+=std::clamp(wanted-fb,-step,step);if(target){fb=0;returned={};}
  Frame wet=in+returned*fb;for(auto& slot:slots){wet=slot->process(wet);wet={fx::clean(wet.l),fx::clean(wet.r)};}
  ramp=std::min(1.f,ramp+step);bypass=target?std::min(1.f,bypass+step):std::max(0.f,bypass-step);
  Frame out=fx::mix(wet*ramp,dry.at(at-latencySamples),bypass);
  if(monoOutput){const float m=(out.l+out.r)*.5f;out={m,m};}
  if(settings.dcBlock){double x[]={out.l,out.r};for(int c=0;c<2;c++){const double y=x[c]-dcX[c]+dcR*dcY[c];dcX[c]=x[c];dcY[c]=std::isfinite(y)?y:0;x[c]=dcY[c];}out={float(x[0]),float(x[1])};}
  out={fx::clean(out.l),fx::clean(out.r)};
  if(settings.clipper)out={std::clamp(out.l,-1.f,1.f),std::clamp(out.r,-1.f,1.f)};
  // Return is one sample old, independent of host block size. Guard only the
  // feedback branch at +/-4, not the output when its CLIPPER is disabled.
  returned=target?Frame{}:Frame{std::clamp(out.l,-4.f,4.f),std::clamp(out.r,-4.f,4.f)};
  return out;
 }
 int latency()const{return latencySamples;}int activeGrains()const{int n=0;for(auto& s:slots)n+=s->grains();return n;}
private:void resetOutput(){returned={};fb=settings.feedback*.01f;dcX={};dcY={};lastDC=settings.dcBlock;}
 void updateLatency(){latencySamples=0;for(auto& s:slots)latencySamples+=s->latency();}
 Frame returned;float fb=0;double dcR=0;std::array<double,2> dcX{},dcY{};bool lastDC=false;
 fx::MDCTPlans plans;std::array<std::unique_ptr<Slot>,5> slots;fx::History dry;Settings settings;int latencySamples=0;float bypass=0,step=.001f,ramp=1;double tempoRate=44100;
};
}
