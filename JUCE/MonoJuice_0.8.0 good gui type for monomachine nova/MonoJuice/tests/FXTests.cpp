#include "dsp/FXChain.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <chrono>
using namespace nova::fx;
void require(bool x,const char* s){if(!x)throw std::runtime_error(s);}
Frame signal(int n,double sr){const double t=n/sr;return {float(.07*std::sin(6.283185307179586*220*t)),float(.05*std::sin(6.283185307179586*331*t))};}
double tail(float feedback,float metal){
 Granular g;g.prepare(44100);GrainSettings p;p.sizeMs=200;p.density=40;p.positionMs=130;p.sprayMs=0;p.mix=100;p.feedback=feedback;p.metal=metal;p.width=0;g.set(p);
 double energy=0;for(int i=0;i<44100*7;i++){const auto out=g.process(i<44100?signal(i,44100):Frame{});require(std::isfinite(out.l)&&std::abs(out.l)<2,"feedback stability");if(i>44100*3)energy+=out.l*out.l+out.r*out.r;}return energy;
}
int main(int argc,char** argv){try{
 for(double sr:{44100.,48000.,96000.}){
  auto chain=std::make_unique<Chain>();auto original=std::make_unique<nova::AudioAdapter>();Settings s;chain->prepare(sr,s);original->prepare(sr);
  for(int i=0;i<int(sr*.4);i++){auto in=signal(i,sr);auto a=chain->process(in);float l,r;original->processFrame(in.l,in.r,l,r,false);require(a.l==l&&a.r==r,"CHORUS changed");}
  std::cout<<"CHORUS-only bit-exact "<<sr<<" PASS\n";
  for(int mask=0;mask<8;mask++){
   s.pitchOn=(mask&1)!=0;s.chorusOn=(mask&2)!=0;s.grainOn=(mask&4)!=0;s.pitch.semitones=7;s.grain.feedback=97;s.grain.metal=90;s.grain.positionMs=50;s.grain.sprayMs=20;
   chain->prepare(sr,s);double energy=0;
   for(int i=0;i<int(sr*.5);i++){auto out=chain->process(signal(i,sr));require(std::isfinite(out.l)&&std::abs(out.l)<2,"combination stability");energy+=out.l*out.l;if(mask==0&&i>=chain->latency())require(out.l==signal(i-chain->latency(),sr).l,"all-off not dry");}
   require(energy>1.e-5,"silent combination");std::cout<<"COMBO "<<mask<<" @ "<<sr<<" energy="<<energy<<" PASS\n";
  }
 }
 for(float st:{-24.f,-12.f,12.f,24.f}){
  Pitch pitch;pitch.prepare(44100);PitchSettings s;s.semitones=st;s.windowMs=100;pitch.set(s);std::vector<float> audio;
  for(int i=0;i<88200;i++){auto out=pitch.process(signal(i,44100));if(i>44100)audio.push_back(out.l);}
  auto amplitude=[&](double hz){double re=0,im=0;for(std::size_t i=0;i<audio.size();i++){const double a=6.283185307179586*hz*double(i)/44100;re+=audio[i]*std::cos(a);im+=audio[i]*std::sin(a);}return std::hypot(re,im)/audio.size();};
  const double target=220*std::pow(2.,st/12.);require(amplitude(target)>.01&&amplitude(target)>amplitude(220)*4,"pitch frequency");std::cout<<"PITCH "<<st<<" -> "<<target<<"Hz PASS\n";
 }
 const auto shortTail=tail(15,0),longTail=tail(99,0),metalTail=tail(99,100);require(longTail>shortTail*100+1.e-6,"feedback not extending tail");require(std::abs(longTail-metalTail)>1.e-5,"metal ineffective");std::cout<<"TAIL energy 3-7s: FB15="<<shortTail<<" FB99="<<longTail<<" FB99/METAL100="<<metalTail<<" PASS\n";
 Granular frozen;frozen.prepare(44100);GrainSettings p;p.mix=100;frozen.set(p);for(int i=0;i<44100;i++)frozen.process(signal(i,44100));p.freeze=true;frozen.set(p);double frozenEnergy=0;for(int i=0;i<88200;i++){auto o=frozen.process({});frozenEnergy+=o.l*o.l;}require(frozenEnergy>.1,"freeze empty");std::cout<<"FREEZE holds captured audio PASS\n";
 auto chain=std::make_unique<Chain>();Settings s;s.pitchOn=s.chorusOn=s.grainOn=true;s.pitch.semitones=24;s.grain.sizeMs=500;s.grain.density=80;s.grain.feedback=99.5f;s.grain.metal=100;s.grain.pitch=24;s.grain.jitter=1200;s.grain.reverse=100;chain->prepare(48000,s);
 for(int i=0;i<48000*3;i++){if(i%137==0){s.pitchOn=!s.pitchOn;s.chorusOn=!s.chorusOn;s.grainOn=!s.grainOn;chain->set(s);}auto o=chain->process(signal(i,48000));require(std::isfinite(o.l)&&std::isfinite(o.r)&&std::abs(o.l)<2,"stress switching");require(chain->activeGrains()<=48,"voice cap");}
 if(argc>1){ // float32 stereo render, converted to WAV by the packaging script
  std::ofstream file(argv[1],std::ios::binary);s={};s.pitchOn=true;s.pitch.semitones=7;s.grainOn=true;s.grain.feedback=97.5f;s.grain.metal=85;s.grain.density=36;s.grain.sizeMs=180;s.grain.sprayMs=8;s.grain.mix=75;chain->prepare(44100,s);
  for(int i=0;i<44100*8;i++){auto in=signal(i,44100);const float gate=i<44100*3?float(std::min(1.,(i%22050)/220.0))*float(std::exp(-double(i%22050)/6000)):0.f;auto o=chain->process(in*gate);file.write(reinterpret_cast<const char*>(&o),sizeof(o));}
 }
 std::cout<<"ALL FX TESTS PASS\n";return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
