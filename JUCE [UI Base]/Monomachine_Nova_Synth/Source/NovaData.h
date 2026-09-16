#pragma once
#include "NovaConfig.h"
#include <JuceHeader.h>
#include "models/machine_definitions.hpp"
#include "models/track_pages.hpp"
#include "models/parameter_conversions.hpp"

namespace nova {
inline const std::vector<monomachine::MachineDef>& machines() {
    static const auto list=[] {
        auto all=monomachine::getAllMachineDefinitions();
        for(auto& m:all) {
            if(m.id==10) m.synthParams[1].defaultVal=64;
            if(m.id==7) {
                m.synthParams[2]={"SLOT","Sample slot",0,9,0};
                m.synthParams[3]={"RAND","Random slot on note",0,1,0};
                m.synthParams[6]={"RSTR","Random start on note",0,1,0};
                m.synthParams[7]={"LEV","Sample level",0,127,127};
            }
            if(m.id==17) m.synthParams[2]={"TUNE","Ring carrier MIDI pitch",0,127,60};
            if(m.isEffect) m.synthParams[7]={"INP","Input: stereo / left / right / mono sum",0,3,0};
        }
        all.erase(std::remove_if(all.begin(),all.end(),[](const auto& m){return m.isEffect==(NOVA_SYNTH!=0);}),all.end());
        return all;
    }();
    return list;
}
inline juce::String machineParam(int id,int index) {return "m"+juce::String(id)+"_"+juce::String(index);}
inline juce::String pageParam(int page,int index) {return "p"+juce::String(page)+"_"+juce::String(index);}
inline const char* pageLabel(int page,int index) {
    static const char* labels[6][8]={
        {"ATK","HOLD","DEC","REL","DIST","VOL","PAN","PORT"},
        {"BASE","WDTH","HPQ","LPQ","ATK","DEC","BOFS","WOFS"},
        {"EQF","EQG","SRR","DTIM","DSND","DFB","DBAS","DWID"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"}};
    return labels[page][index];
}
struct Spec {juce::String id,name; float lo=0,hi=127,def=0,step=1; juce::String choices;};
inline const std::vector<Spec>& specs() {
    static const std::vector<Spec> all=[] {
        std::vector<Spec> s;
        juce::String names;
        int defaultMachine=0,index=0;
        for(const auto& m:machines()) {if(names.isNotEmpty()) names+="|";names+=juce::String(m.name);if(m.id==4) defaultMachine=index;++index;}
        s.push_back({"machine","Machine",0,static_cast<float>(machines().size()-1),static_cast<float>(defaultMachine),1,names});
        for(const auto& m:machines()) for(int i=0;i<8;++i) {
            const auto& p=m.synthParams[static_cast<size_t>(i)];
            if(p.maxVal==0) continue;
            juce::String choices;
            if(m.isEffect && i==7) choices="STEREO|LEFT|RIGHT|MONO";
            if(m.id==7 && (i==3 || i==6)) choices="OFF|ON";
            if(m.id==7 && i==2) choices="1|2|3|4|5|6|7|8|9|10";
            s.push_back({machineParam(m.id,i),juce::String(m.name)+" "+juce::String(p.name),static_cast<float>(p.minVal),static_cast<float>(p.maxVal),static_cast<float>(p.defaultVal),1,choices});
        }
        const int defaults[6][8]={{0,0,64,64,0,NOVA_SYNTH?64:127,64,0},{0,127,0,0,0,93,64,64},{64,64,0,64,0,28,0,127},
                                 {0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64}};
        const char* sections[]={"AMP","FILTER","FX","LFO1","LFO2","LFO3"};
        for(int page=0;page<6;++page) for(int i=0;i<8;++i) {
            juce::String choices;
            if(page>=3) {
                if(i==0) choices="PTCH|SYN|AMP|FLT|FX";
                if(i==2) choices="FREE|TRIG|HOLD|ONE";
                if(i==3) choices="TRI|SAW|SQR|EXP|RND";
                if(i==4) choices="1X|2X|4X|8X|16X|32X|64X";
            }
            const float high=choices.isEmpty()?(page>=3 && i==1?7.0f:127.0f):static_cast<float>(juce::StringArray::fromTokens(choices,"|","").size()-1);
            s.push_back({pageParam(page,i),juce::String(sections[page])+" "+pageLabel(page,i),0,high,static_cast<float>(defaults[page][i]),1,choices});
        }
        s.push_back({"level","Master level",0,1,NOVA_SYNTH?0.5f:1.0f,0.001f,{}});
        s.push_back({"bpm","Internal tempo",30,300,120,0.1f,{}});
        s.push_back({"host_sync","Use host tempo",0,1,1,1,"OFF|ON"});
        if(!NOVA_SYNTH) s.push_back({"fx_mix","Global dry wet",0,127,127,1,{}});
        if(!NOVA_SYNTH) s.push_back({"gate","FX MIDI gate",0,1,0,1,"OFF|ON"});
        s.push_back({"arp_mode","Arp mode",0,3,0,1,"OFF|KEY|SID|ADD"});
        s.push_back({"arp_play","Arp direction",0,4,1,1,"TRUE|UP|DOWN|CYCL|RND"});
        s.push_back({"arp_speed","Arp ticks (24 PPQN)",1,96,6,1,{}});
        s.push_back({"arp_range","Arp octaves",1,4,1,1,{}});
        s.push_back({"arp_length","Arp gate length",1,127,64,1,{}});
        s.push_back({"macro_x","Macro X / MIDI CC1",0,127,64,1,{}});
        s.push_back({"macro_y","Macro Y / MIDI CC11",0,127,0,1,{}});
        juce::String targets;
        for(int i=0;i<32;++i) {if(i) targets+="|";targets+=i<8?"SYN "+juce::String(i+1):juce::String(sections[(i-8)/8])+" "+pageLabel((i-8)/8,(i-8)%8);}
        for(int row=0;row<8;++row) {
            const auto prefix="r"+juce::String(row)+"_";
            s.push_back({prefix+"on","Route "+juce::String(row+1)+" enable",0,1,0,1,"OFF|ON"});
            s.push_back({prefix+"src","Route source",0,6,static_cast<float>(row%7),1,"KEY|VEL|MACRO X|MACRO Y|LFO1|LFO2|LFO3"});
            s.push_back({prefix+"dest","Route target",0,31,16,1,targets});
            s.push_back({prefix+"depth","Route depth",-64,63,0,1,{}});
        }
        return s;
    }();
    return all;
}
}
