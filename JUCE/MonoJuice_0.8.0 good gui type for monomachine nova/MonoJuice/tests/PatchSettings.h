#pragma once
#include "FactoryPresets.h"
#include "dsp/Rack.h"
#include <cstring>
inline nova::rack::SlotSettings patchSlot(int type,const factory::Patch& p){
 auto v=[&](const char* key,float fallback=0.f){for(auto x:p)if(std::strcmp(x.first,key)==0)return x.second;return fallback;};
 nova::rack::SlotSettings s;s.type=type;s.on=true;
 s.pitch={v("P_SHIFT"),v("P_FINE"),v("P_WINDOW",80),v("P_MIX",100)};
 const char* ch[]={"DEL","DEP","SPD","MIX","FB","WID","LP","INP"};for(int i=0;i<8;i++)s.chorus[i]=int(v(ch[i],float(nova::ChorusCore::defaults[i])));
 s.grain={v("G_SIZE",100),v("G_DENSITY",20),v("G_POSITION",250),v("G_SPRAY",120),v("G_PITCH"),v("G_JITTER"),v("G_WIDTH",80),v("G_MIX",60),v("G_FEEDBACK",65),v("G_METAL",40),v("G_TONE",7000),v("G_REVERSE"),v("G_FREEZE")!=0};
 s.comb={{v("K_T1",100),v("K_T2",100),v("K_T3",100),v("K_T4",100)},v("K_MULT",1),v("K_FB",25),v("K_HP",5),v("K_LIM",200),v("K_RATE",.5f),v("K_SMOOTH"),v("K_LEAN"),v("K_CROSS"),v("K_AM"),v("K_GATE",-140),v("K_GATE_REL",10),v("K_DRY_DB"),v("K_WET_DB"),v("K_TUNE"),v("K_MS")!=0,v("K_OS")!=0};
 s.odin={v("K_FREQ_A",220),v("K_RES_A",75),v("K_FREQ_B",331),v("K_RES_B",75),v("K_BALANCE",50),v("K_MIX",100),int(v("K_ROUTING")),v("K_NEG_A")!=0,v("K_NEG_B")!=0};
 s.colour={int(v("K_MODEL",1)),v("K_DRIVE"),v("K_COLOR_MIX",100),v("K_OUT"),v("K_DEPTH"),v("K_SPEED",.5f),v("K_ENV"),v("K_DECAY",180)};
 s.spectre={v("S_PROM",10),v("S_MIX",100),int(v("S_WINDOW",3)),v("S_LOW"),v("S_HIGH"),int(v("S_PEAKS")),int(v("S_WIDTH")),v("S_FLOOR",-120),v("S_ATTACK"),v("S_RELEASE"),v("S_SHIFT"),v("S_FREEZE")!=0};s.spectralExtended=v("S_MODE")!=0;
 return s;
}
inline nova::fx::Frame testInput(int i,int sr){uint32_t n=uint32_t(i)*747796405u+2891336453u;n=((n>>((n>>28)+4))^n)*277803737u;n=(n>>22)^n;float noise=float(double(n)/4294967296.0*2-1);double t=double(i)/sr;float e=float(std::exp(-7*std::fmod(t,.5)));return {e*(.09f*noise+.12f*float(std::sin(6.283185307179586*147*t))),e*(-.07f*noise+.11f*float(std::sin(6.283185307179586*233*t)))};}
