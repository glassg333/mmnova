/* MonoJuice 0.7.0 factory patches and constrained randomization.
 * New independent contribution, GPL-3.0-only. UI/message-thread helpers only;
 * these do not change or run any audio algorithm. Physical APVTS values.
 */
#pragma once
#include <array>
#include <vector>
#include <utility>
#include <random>
#include <cmath>
#include <cstdint>
namespace factory07 {
using Value=std::pair<const char*,float>;
using Patch=std::vector<Value>;
struct CombPreset {std::array<float,18> v;bool ms,os;const char* description;};
// TIME1..4, MULT, FB%, HP, LIMms, RATE, SMOOTHms, LEAN%, CROSS%, AM,
// GATEdB, GATEreleaseMs, DRYdB, WETdB, TUNEst. Short resonators, not echo presets.
inline constexpr std::array<CombPreset,10> comb{{
 {{{1.13f,1.79f,2.57f,3.71f,1,21.5f,120,110,.5f,0,0,12,0,-100,70,-9,-15,0}},false,false,"Bright short metal"},
 {{{.43f,.71f,1.09f,1.67f,1,22.8f,450,80,.5f,0,-7,25,0,-100,60,-12,-15,0}},false,false,"Thin steel / wire"},
 {{{3.17f,4.73f,7.09f,10.61f,1,24,65,250,.5f,0,9,38,0,-105,180,-12,-18,0}},false,false,"Low struck plate"},
 {{{.79f,1.41f,2.23f,3.53f,.83f,23.4f,280,140,.5f,0,15,60,0,-100,100,-12,-15,0}},true,false,"Inharmonic stereo alloy"},
 {{{2.11f,2.13f,4.27f,6.43f,1,24.2f,90,320,.5f,0,-3,18,0,-105,220,-12,-18,0}},false,false,"Beating metal strings"},
 {{{2.7f,3.9f,5.3f,7.7f,1,21.8f,55,160,.008f,8,-8,20,.8f,-100,100,-9,-15,0}},false,false,"Taut rubber / latex"},
 {{{5.7f,7.3f,10.1f,13.7f,.9f,23,35,260,.004f,25,11,42,1.8f,-105,200,-12,-18,0}},false,false,"Low elastic membrane"},
 {{{.83f,1.31f,2.17f,3.19f,1,20.5f,240,90,.015f,4,-18,65,2.4f,-100,70,-12,-15,0}},true,false,"Bright rubber squeak"},
 {{{1.9f,3.1f,4.9f,7.1f,.8f,23.8f,110,210,.006f,12,6,32,1.2f,-105,140,-12,-18,0}},false,false,"Metal / latex hybrid"},
 {{{4.3f,6.7f,9.7f,14.3f,.72f,22.6f,70,300,.003f,35,-13,72,3.2f,-105,240,-12,-18,0}},false,false,"Stretched hollow rubber"}
}};
struct SpectrePreset {std::array<float,10> v;int window;const char* description;};
// PROM, MIX, LOW, HIGH, PEAKS, WIDTH, FLOOR, ATTACK, RELEASE, SHIFT.
// First four use exclusively the old SPECTRE controls; all extensions neutral.
inline constexpr std::array<SpectrePreset,10> spectre{{
 {{{10,100,0,0,0,0,-120,0,0,0}},3,"Previous-version default / neutral extensions"},
 {{{20,100,0,0,0,0,-120,0,0,0}},1,"Classic short metallic grains"},
 {{{35,100,0,0,0,0,-120,0,0,0}},4,"Classic ringing spectrum"},
 {{{55,100,0,0,0,0,-120,0,0,0}},5,"Classic sparse metal"},
 {{{18,100,350,12000,24,0,-60,0,100,0}},3,"Bright selected metal peaks"},
 {{{26,100,180,9000,12,1,-72,8,260,7}},4,"Shifted resonant alloy"},
 {{{14,90,900,15000,36,1,-48,2,90,12}},2,"High metallic foil"},
 {{{12,100,90,6000,18,2,-42,12,140,-12}},1,"Low rubber membrane"},
 {{{9,95,180,8500,28,3,-36,22,220,-7}},2,"Soft stretched latex"},
 {{{20,100,350,11000,10,2,-54,4,70,5}},0,"Tight rubber / metal grains"}
}};
inline constexpr const char* combKeys[]={"K_T1","K_T2","K_T3","K_T4","K_MULT","K_FB","K_HP","K_LIM","K_RATE","K_SMOOTH","K_LEAN","K_CROSS","K_AM","K_GATE","K_GATE_REL","K_DRY_DB","K_WET_DB","K_TUNE"};
inline constexpr const char* spectreKeys[]={"S_PROM","S_MIX","S_LOW","S_HIGH","S_PEAKS","S_WIDTH","S_FLOOR","S_ATTACK","S_RELEASE","S_SHIFT"};
inline Patch preset(int type,int index){
 Patch p;if(index<0||index>=10)return p;
 if(type==4){const auto& s=comb[size_t(index)];for(size_t i=0;i<s.v.size();i++)p.emplace_back(combKeys[i],s.v[i]);p.emplace_back("K_MS",s.ms?1.f:0.f);p.emplace_back("K_OS",s.os?1.f:0.f);}
 if(type==5){const auto& s=spectre[size_t(index)];for(size_t i=0;i<s.v.size();i++)p.emplace_back(spectreKeys[i],s.v[i]);p.emplace_back("S_WINDOW",float(s.window));p.emplace_back("S_FREEZE",0.f);}
 return p;
}
inline const char* description(int type,int index){if(index<0||index>=10)return "Custom parameters";return type==4?comb[size_t(index)].description:(type==5?spectre[size_t(index)].description:"No factory bank");}
// Predictable seed for tests; UI provides a fresh seed per press. Not audio-thread RNG.
inline Patch randomize(int type,uint32_t seed){
 std::mt19937 rng(seed);auto unit=[&]{return double(rng())/4294967296.0;};
 auto f=[&](float lo,float hi){return float(lo+(hi-lo)*unit());};
 auto log=[&](float lo,float hi){return float(lo*std::pow(double(hi/lo),unit()));};
 auto pick=[&](int lo,int hi){return lo+int(unit()*(hi-lo+1));};
 Patch p;auto add=[&](const char* key,float v){p.emplace_back(key,v);};
 if(type==1){const float shifts[]={-24,-19,-12,-7,-5,0,5,7,12,19,24};add("P_SHIFT",shifts[pick(0,10)]);add("P_FINE",float(pick(-35,35)));add("P_WINDOW",float(pick(24,145)));add("P_MIX",float(pick(40,100)));}
 if(type==2){add("DEL",float(pick(4,115)));add("DEP",float(pick(8,110)));add("SPD",float(pick(3,108)));add("MIX",float(pick(50,127)));add("FB",float(pick(0,80)));add("WID",float(pick(0,110)));add("LP",float(pick(45,127)));add("INP",64);}
 if(type==3){add("G_SIZE",log(15,300));add("G_DENSITY",float(pick(12,55)));add("G_POSITION",log(15,850));add("G_SPRAY",f(0,180));add("G_PITCH",float(pick(-12,12)));add("G_JITTER",float(pick(0,120)));add("G_WIDTH",float(pick(30,100)));add("G_MIX",float(pick(40,85)));add("G_FEEDBACK",f(40,94));add("G_METAL",float(pick(30,100)));add("G_TONE",log(1400,12000));add("G_REVERSE",float(pick(0,65)));add("G_FREEZE",0);}
 if(type==4){const bool elastic=pick(0,1)!=0;const float base=log(.45f,5.f);for(int i=0;i<4;i++)add(combKeys[i],base*(1.f+i*f(.38f,.8f)));add("K_MULT",f(.65f,1));add("K_FB",f(16,24));add("K_HP",log(35,700));add("K_LIM",f(70,350));add("K_RATE",elastic?log(.003f,.02f):.5f);add("K_SMOOTH",elastic?f(3,35):0);add("K_LEAN",f(-20,20));add("K_CROSS",f(0,75));add("K_AM",elastic?f(.4f,2.8f):0);add("K_GATE",-105);add("K_GATE_REL",f(70,230));add("K_DRY_DB",f(-15,-6));add("K_WET_DB",f(-21,-15));add("K_TUNE",0);add("K_MS",float(pick(0,1)));add("K_OS",0);}
 if(type==5){const bool classic=pick(0,3)==0;add("S_PROM",f(7,42));add("S_MIX",float(pick(70,100)));add("S_LOW",classic?0:log(60,1000));add("S_HIGH",classic?0:log(4000,16000));add("S_PEAKS",classic?0:float(pick(8,48)));add("S_WIDTH",classic?0:float(pick(0,3)));add("S_FLOOR",classic?-120:f(-78,-36));add("S_ATTACK",classic?0:float(pick(0,35)));add("S_RELEASE",classic?0:float(pick(25,300)));const float shifts[]={-12,-7,0,0,5,7,12};add("S_SHIFT",classic?0:shifts[pick(0,6)]);add("S_WINDOW",float(pick(0,5)));add("S_FREEZE",0);}
 return p;
}
}
