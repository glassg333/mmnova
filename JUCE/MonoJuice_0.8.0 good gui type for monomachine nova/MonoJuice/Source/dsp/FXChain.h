#pragma once
#include "AudioAdapter.h"
#include "NewEffects.h"
namespace nova::fx {
struct Settings {PitchSettings pitch;GrainSettings grain;std::array<int,8> chorus=ChorusCore::defaults;bool pitchOn=false,chorusOn=true,grainOn=false,bypass=false;};
class Chain {
public:
 int prepare(double rate,Settings initial){sr=rate;latencyFrames=chorus.prepare(rate);dryChorus.prepare(latencyFrames+32);dryMaster.prepare(latencyFrames+32);pitch.prepare(rate);grain.prepare(rate);smoothStep=float(1/(rate*.02));reset(initial);return latencyFrames;}
 void reset(Settings s){settings=s;pitch.reset();grain.reset();chorus.reset(s.chorus);dryChorus.clear();dryMaster.clear();pAmount=s.pitchOn?1.f:0.f;cAmount=s.chorusOn?1.f:0.f;gAmount=s.grainOn?1.f:0.f;bAmount=s.bypass?1.f:0.f;set(s);}
 void clearStage(int stage){if(stage==0)pitch.reset();else if(stage==1)chorus.reset(settings.chorus);else grain.reset();}
 void set(Settings s){settings=s;pitch.set(s.pitch);grain.set(s.grain);chorus.setParameters(s.chorus);}
 Frame process(Frame in,bool forcedBypass=false){
  in={clean(in.l),clean(in.r)};const auto index=dryMaster.sizeWritten();dryMaster.push(in);
  approach(pAmount,settings.pitchOn);approach(cAmount,settings.chorusOn);approach(gAmount,settings.grainOn);approach(bAmount,settings.bypass||forcedBypass);
  Frame afterPitch=in;if(pAmount>0)afterPitch=fx::mix(in,pitch.process(in),pAmount);else pitch.recordBypassed(in);
  const auto chIndex=dryChorus.sizeWritten();dryChorus.push(afterPitch);
  Frame chOut;chorus.processFrame(afterPitch.l,afterPitch.r,chOut.l,chOut.r,false);
  const Frame afterChorus=fx::mix(dryChorus.at(chIndex-latencyFrames),chOut,cAmount);
  Frame afterGrains=afterChorus;if(gAmount>0)afterGrains=fx::mix(afterChorus,grain.process(afterChorus),gAmount);else grain.recordBypassed(afterChorus);
  return fx::mix(afterGrains,dryMaster.at(index-latencyFrames),bAmount);
 }
 int latency()const{return latencyFrames;}
 int activeGrains()const{return grain.grainsActive();}
private:
 void approach(float& value,bool on){value=on?std::min(1.f,value+smoothStep):std::max(0.f,value-smoothStep);}
 double sr=44100;int latencyFrames=16;float smoothStep=.001f,pAmount=0,cAmount=1,gAmount=0,bAmount=0;
 Settings settings;Pitch pitch;Granular grain;AudioAdapter chorus;History dryChorus,dryMaster;
};
}
