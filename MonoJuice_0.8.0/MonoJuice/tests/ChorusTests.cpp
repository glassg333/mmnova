#include "dsp/ChorusCore.h"
#include "dsp/AudioAdapter.h"
#include <iostream>
#include <random>
#include <chrono>
#include <memory>
#include <limits>
#include <stdexcept>
#include <vector>

static void require(bool v,const char* reason){if(!v)throw std::runtime_error(reason);}
int main(){try{
 std::mt19937 rng(0x147661);nova::ChorusCore core;auto params=nova::ChorusCore::defaults;
 std::int32_t in[32]{},out[32]{};std::uint64_t hash=14695981039346656037ull;
 const auto started=std::chrono::steady_clock::now();
 for(unsigned block=0;block<24576;block++){
  if(block>0 && block<4096 && block%128==0){for(int& p:params)p=int(rng()%128);core.setParameters(params);}
  if(block>=4096 && block<20480 && (block-4096)%16==0){params=nova::ChorusCore::defaults;params[(block-4096)/2048]=int(((block-4096)/16)%128);core.setParameters(params);}
  if(block>=20480 && block%128==0){params.fill((block/128)%2?127:0);core.setParameters(params);}
  for(unsigned i=0;i<32;i++){
   in[i]=std::int32_t(rng()%0x200000)-0x100000;
   if(block>=20480)in[i]=nova::ChorusCore::signed24(rng());
   if(block<4)in[i]=(block==0&&i<2)?0x100000:0;
  }
  core.process16(in,out);
  for(auto sample:out)for(int byte=0;byte<3;byte++){hash^=(std::uint32_t(sample)>>(byte*8))&255;hash*=1099511628211ull;}
 }
 const auto elapsed=std::chrono::duration<double>(std::chrono::steady_clock::now()-started).count();
 std::cout<<"Native golden hash: "<<std::hex<<hash<<std::dec<<"; 393216 frames / "<<elapsed<<" seconds; real-time load "<<elapsed/(393216.0/44100)<<"\n";
 require(hash==0x7007ca4d75708b0full,"independent original-instruction golden hash mismatch");

 const char* ids[]={"DEL","DEP","SPD","MIX","FB","WID","LP","INP"};
 for(int parameter=0;parameter<8;++parameter){
  std::vector<std::int32_t> first;double difference=0;
  for(int pass=0;pass<2;++pass){
   auto p=nova::ChorusCore::defaults;p[std::size_t(parameter)]=pass?127:0;core.reset(p);
   for(int block=0;block<5512;++block){
    for(int frame=0;frame<16;++frame){const double t=double(block*16+frame)/44100;in[frame*2]=std::int32_t(0.06*8388608*std::sin(6.283185307179586*220*t));in[frame*2+1]=std::int32_t(0.06*8388608*std::sin(6.283185307179586*331*t));}
    core.process16(in,out);
    for(int i=0;i<32;++i){if(pass==0)first.push_back(out[i]);else{const double d=double(out[i]-first[std::size_t(block*32+i)])/8388608.0;difference+=d*d;}}
   }
  }
  const auto delta=std::sqrt(difference/(5512*32));require(delta>1.0e-6,"control does not affect DSP output");std::cout<<"CONTROL "<<ids[parameter]<<" difference RMS="<<delta<<" PASS\n";
 }
 auto adapter=std::make_unique<nova::AudioAdapter>();
 for(double rate:{22050.0,32000.0,44100.0,48000.0,88200.0,96000.0,192000.0}){
  const int latency=adapter->prepare(rate);auto p=nova::ChorusCore::defaults;p[3]=0;adapter->reset(p);
  double error=0,energy=0;int count=0;constexpr double pi=3.14159265358979323846;
  for(int i=0;i<int(rate);++i){
   const float sample=float(0.125*std::sin(2*pi*1000*i/rate));float l,r;adapter->processFrame(sample,-sample,l,r,false);
   require(std::isfinite(l)&&std::isfinite(r),"non-finite output");
   if(i>int(rate/4)){const double reference=0.125*std::sin(2*pi*1000*(i-latency)/rate);error+=(l-reference)*(l-reference);energy+=reference*reference;++count;}
  }
  require(std::sqrt(error/energy)<0.003,"dry SRC accuracy / time alignment");
  std::cout<<"Adapter "<<rate<<" Hz: latency="<<latency<<" relative RMS error="<<std::sqrt(error/energy)<<" PASS\n";
  adapter->reset(p);
  for(int i=0;i<latency+200;++i){float l,r;adapter->processFrame(i==0?0.5f:0.0f,0,l,r,true);require(l==(i==latency?0.5f:0.0f)&&r==0,"bypass latency");}
 }
 // At native rate the adapter must preserve every fixed-point result, including the block delay.
 adapter->prepare(44100);adapter->reset(nova::ChorusCore::defaults);core.reset(nova::ChorusCore::defaults);
 std::vector<std::int32_t> expected(32,0);
 for(int b=0;b<512;++b){
  for(int i=0;i<32;++i)in[i]=std::int32_t(rng()%0x200000)-0x100000;
  core.process16(in,out);expected.insert(expected.end(),out,out+32);
  for(int i=0;i<16;++i){float l,r;adapter->processFrame(float(in[i*2])/8388608.0f,float(in[i*2+1])/8388608.0f,l,r,false);require(l==float(expected[std::size_t(b*32+i*2)])/8388608.0f && r==float(expected[std::size_t(b*32+i*2+1)])/8388608.0f,"native adapter changed PCM");}
 }
 float l,r;adapter->processFrame(std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::infinity(),l,r,false);require(std::isfinite(l)&&std::isfinite(r),"input sanitization");
 std::cout<<"ALL NATIVE TESTS PASSED\n";return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
