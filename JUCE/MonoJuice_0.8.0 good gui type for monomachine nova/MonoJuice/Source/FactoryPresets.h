/* MonoJuice 0.8.0 factory control patches, GPL-3.0-only. */
#pragma once
#include <array>
#include <vector>
#include <utility>
#include <random>
#include <cmath>
#include <cstdint>
#include <cstring>
namespace factory {
using Value=std::pair<const char*,float>;using Patch=std::vector<Value>;
inline constexpr const char* combKeys[]={"K_T1","K_T2","K_T3","K_T4","K_MULT","K_FB","K_HP","K_LIM","K_RATE","K_SMOOTH","K_LEAN","K_CROSS","K_AM","K_GATE","K_GATE_REL","K_DRY_DB","K_WET_DB","K_TUNE"};
inline constexpr const char* spectreKeys[]={"S_PROM","S_MIX","S_LOW","S_HIGH","S_PEAKS","S_WIDTH","S_FLOOR","S_ATTACK","S_RELEASE","S_SHIFT"};
inline constexpr const char* colourKeys[]={"K_DRIVE","K_COLOR_MIX","K_OUT","K_DEPTH","K_SPEED","K_ENV","K_DECAY"};
inline Patch preset(int type,int index){
 Patch p;if(index<0||index>=10)return p;auto add=[&](const char* k,float v){p.emplace_back(k,v);};
 if(type==4){
  // Full-wet patches: source material is driven into strongly resonant networks.
  const float colour[10][7]={
   {9,100,-15,0,.5f,0,180},{12,100,-18,.12f,.31f,0,140},{15,100,-21,.3f,.23f,0,280},{18,100,-21,.5f,.17f,0,350},{12,100,-18,1.2f,.7f,2,150},
   {12,100,-3,1.5f,.8f,-24,130},{18,100,-6,3,.36f,30,320},{9,100,-3,6,2.4f,-18,65},{15,100,-6,2.5f,.5f,24,210},{18,100,-6,7,.2f,-30,450}};
  for(int i=0;i<7;i++)add(colourKeys[i],colour[index][i]);add("K_MODEL",index<5?0:1);
  if(index<5){const float odin[5][9]={
    {337,96,541,94,50,100,1,0,1},{877,98,1409,96,45,100,2,1,0},{137,98.5f,223,96,50,100,1,0,1},{463,97,739,98,60,100,2,1,1},{211,96,419,97,50,100,1,1,0}};
   const char* keys[]={"K_FREQ_A","K_RES_A","K_FREQ_B","K_RES_B","K_BALANCE","K_MIX","K_ROUTING","K_NEG_A","K_NEG_B"};for(int i=0;i<9;i++)add(keys[i],odin[index][i]);
  }else{const float times[5][4]={{.9f,1.3f,2.1f,3.1f},{2.3f,3.7f,5.3f,7.9f},{.43f,.67f,1.07f,1.73f},{1.7f,2.9f,4.3f,6.7f},{3.1f,4.7f,7.1f,10.7f}};
   const float common[]={0,0,0,0,1,24,65,150,.5f,2,-9,38,0,-110,160,-60,-3,0};
   for(int i=0;i<18;i++)add(combKeys[i],i<4?times[index-5][i]:common[i]);add("K_MS",index==7?1:0);add("K_OS",0);
  }
 }
 if(type==5){const float prom[]={10,28,42,58,72,18,35,50,65,80};const int windows[]={3,0,1,2,4,0,1,2,3,5};
  const float v[]={prom[index],100,0,0,0,0,-120,0,0,0};for(int i=0;i<10;i++)add(spectreKeys[i],v[i]);add("S_WINDOW",float(windows[index]));add("S_FREEZE",0);add("S_MODE",0);
 }
 return p;
}
inline const char* description(int type,int index){if(index<0||index>=10)return "Custom parameters";if(type==5)return index==0?"Exact 0.5 default":"CLASSIC 0.5 / isolated spectrum";return index<5?"ODIN2 / driven metallic resonance":"ParallelComb / envelope-pitched rubber";}
inline Patch randomize(int type,uint32_t seed,int variant=0){
 std::mt19937 rng(seed);auto unit=[&]{return double(rng())/4294967296.0;};auto f=[&](float lo,float hi){return float(lo+(hi-lo)*unit());};auto log=[&](float lo,float hi){return float(lo*std::pow(double(hi/lo),unit()));};auto pick=[&](int lo,int hi){return lo+int(unit()*(hi-lo+1));};Patch p;auto add=[&](const char* k,float v){p.emplace_back(k,v);};
 if(type==1){const float shifts[]={-24,-19,-12,-7,-5,0,5,7,12,19,24};add("P_SHIFT",shifts[pick(0,10)]);add("P_FINE",float(pick(-35,35)));add("P_WINDOW",float(pick(24,145)));add("P_MIX",float(pick(40,100)));}
 if(type==2){add("DEL",float(pick(4,115)));add("DEP",float(pick(8,110)));add("SPD",float(pick(3,108)));add("MIX",float(pick(50,127)));add("FB",float(pick(0,80)));add("WID",float(pick(0,110)));add("LP",float(pick(45,127)));add("INP",64);}
 if(type==3){add("G_SIZE",log(15,300));add("G_DENSITY",float(pick(12,55)));add("G_POSITION",log(15,850));add("G_SPRAY",f(0,180));add("G_PITCH",float(pick(-12,12)));add("G_JITTER",float(pick(0,120)));add("G_WIDTH",float(pick(30,100)));add("G_MIX",float(pick(40,85)));add("G_FEEDBACK",f(40,94));add("G_METAL",float(pick(30,100)));add("G_TONE",log(1400,12000));add("G_REVERSE",float(pick(0,65)));add("G_FREEZE",0);}
 if(type==4){p=preset(4,pick(0,4)+(variant?5:0));for(auto& x:p){
  if(std::strcmp(x.first,"K_DRIVE")==0)x.second=f(6,20);
  if(std::strcmp(x.first,"K_DEPTH")==0)x.second=f(.2f,7);
  if(std::strcmp(x.first,"K_SPEED")==0)x.second=log(.1f,3);
  if(std::strcmp(x.first,"K_ENV")==0)x.second=f(-30,30);
  if(std::strcmp(x.first,"K_DECAY")==0)x.second=f(50,450);
  if(std::strcmp(x.first,"K_FREQ_A")==0||std::strcmp(x.first,"K_FREQ_B")==0)x.second=log(100,1800);
  if(std::strcmp(x.first,"K_RES_A")==0||std::strcmp(x.first,"K_RES_B")==0)x.second=f(88,98.5f);
  if(std::strncmp(x.first,"K_T",3)==0&&std::strlen(x.first)==4)x.second=log(.4f,8);
 }}
 if(type==5){p=preset(5,pick(0,9));for(auto& x:p){if(std::strcmp(x.first,"S_PROM")==0)x.second=f(15,75);if(std::strcmp(x.first,"S_WINDOW")==0)x.second=float(pick(0,5));if(std::strcmp(x.first,"S_MODE")==0)x.second=float(variant);}
  if(variant){for(auto& x:p){if(std::strcmp(x.first,"S_SHIFT")==0)x.second=f(-12,12);if(std::strcmp(x.first,"S_WIDTH")==0)x.second=float(pick(0,3));if(std::strcmp(x.first,"S_RELEASE")==0)x.second=f(20,180);}}
 }
 return p;
}
}
