#pragma once
#include "NovaConfig.h"
#include <JuceHeader.h>
#include "ProjectIdentity.h"
#include "models/machine_definitions.hpp"
#include "models/track_pages.hpp"
#include "models/parameter_conversions.hpp"
#include "models/DspModes.hpp"

namespace nova {
inline const std::vector<monomachine::MachineDef>& machines() {
    static const auto list=[] {
        auto all=monomachine::getAllMachineDefinitions();
        for(auto& m:all) {
            if(m.id==10) m.synthParams[1].defaultVal=64;
            if(m.id==7) {
                m.synthParams[2]={"SLOT","Sample slot",0,23,0};
                m.synthParams[3]={"RAND","Random slot on note",0,1,0};
                m.synthParams[6]={"CHRM","Chromatic 24-slot mapping",0,1,1};
                m.synthParams[7]={"LEV","Sample level",0,127,127};
            }
        }
        all.erase(std::remove_if(all.begin(),all.end(),[](const auto& m){return m.isEffect==(NOVA_SYNTH!=0);}),all.end());
        return all;
    }();
    return list;
}
inline juce::String machineParam(int id,int index) {return "m"+juce::String(id)+"_"+juce::String(index);}
inline juce::String pageParam(int page,int index) {return "p"+juce::String(page)+"_"+juce::String(index);}
inline const char* pageLabel(int page,int index) {
    static const char* labels[9][8]={ // 1.8.0: +LFO4-6
        {"ATK","HOLD","DEC","REL","DIST","VOL","PAN","PORT"},
        {"BASE","WDTH","HPQ","LPQ","ATK","DEC","BOFS","WOFS"},
        {"EQF","EQG","SRR","DTIM","DSND","DFB","DBAS","DWID"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
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
        for(const auto& m:machines()) {if(names.isNotEmpty()) names+="|";names+=juce::String(m.name);if(m.id==9) defaultMachine=index;++index;} // 1.7.5: FM+ PAR -- машина по умолчанию
        s.push_back({"machine","Machine",0,static_cast<float>(machines().size()-1),static_cast<float>(defaultMachine),1,names});
        for(const auto& m:machines()) for(int i=0;i<8;++i) {
            const auto& p=m.synthParams[static_cast<size_t>(i)];
            if(p.maxVal==0) continue;
            juce::String choices;
            if(m.id==4 && i==3)choices="old|mnm";
            if(m.id==2 && i==2)choices="OFF|ON";
            if(m.id==7 && i==2) choices="1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24";
            if(m.id==7 && (i==3 || i==6)) choices="OFF|ON";
            s.push_back({machineParam(m.id,i),juce::String(m.name)+" "+juce::String(p.name),static_cast<float>(p.minVal),static_cast<float>(p.maxVal),static_cast<float>(p.defaultVal),1,choices});
        }
        // DIST and DSND are bipolar on hardware: stored centred at 64, shown as -64..+63.
        const int defaults[6][8]={{0,0,64,64,64,64,64,0},{0,127,0,0,0,93,64,64},{64,64,0,64,64,28,0,127},
                                 {0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64}};
        const char* sections[]{"AMP","FILTER","FX","LFO1","LFO2","LFO3","LFO4","LFO5","LFO6"}; // 1.8.0: +LFO4-6 (страницы 6..8)
        for(int page=0;page<9;++page) for(int i=0;i<8;++i) { // 1.8.0: страницы 6..8 = LFO4-6
            juce::String choices;
            if(page>=3) {
                if(i==0) choices=page<6?"PTCH|SYNT|AMP|FILT|EFFX|LFO1|LFO2|LFO3":"P1SY|P1AM|P1FI|P1FX|P2SY|P2AM|P2FI|P2FX"; // 1.8.0: LFO4-6 таргетят и P2
                if(i==2) choices="FREE|TRIG|HOLD|ONE";
                if(i==3) choices="TRI|ITRI|SAW|ISAW|SQR|ISQR|EXP|IEXP|RMP|IRMP|RND";
                if(i==4) choices="1X|2X|4X|8X|16X|32X|64X";
            }
            const float high=choices.isEmpty()?(page>=3 && i==1?7.0f:127.0f):static_cast<float>(juce::StringArray::fromTokens(choices,"|","").size()-1);
            s.push_back({pageParam(page,i),juce::String(sections[page])+" "+pageLabel(page,i),0,high,static_cast<float>(defaults[page][i]),1,choices});
        }
        // 1.6.14: old AMP DSP удалён (выбор бесполезен) -- параметр оставлен для
        // совместимости старых состояний, UI и DSP его больше не читают.
        s.push_back({"amp_mode","AMP DSP",0,2,1,1,"old|mnm|vital"}); // 1.7.9: +VITAL -- витальные кривые огибающей (по запросу; MSEG тут ни при чём)
        // 1.6.14: скорость портаменто (окно по ПКМ на ручке PORT страницы AMP).
        s.push_back({"porta_speed","PORTAMENTO speed",0,127,64,1,{}});
        s.push_back({"amp_curve_a","AMP Attack curve",-100,100,0,0.1f,{}});
        s.push_back({"amp_curve_d","AMP Decay curve",-150,150,0,0.1f,{}});
        s.push_back({"amp_curve_r","AMP Release curve",-100,100,0,0.1f,{}});
        s.push_back({"level","LEV",0,127,100,1,{}});
        s.push_back({"bpm","Internal tempo",30,300,120,0.1f,{}});
        s.push_back({"host_sync","Use host tempo",0,1,1,1,"OFF|ON"});
        // Retained legacy IDs keep old host automation/state readable; the live mode is derived from arp_on/arp_hold.
        s.push_back({"arp_mode","ARP Legacy Mode",0,3,0,1,"OFF|KEY|SID|ADD"});
        s.push_back({"arp_speed","ARP Legacy Rate",0,15,4,1,{}});
        if(!NOVA_SYNTH) s.push_back({"gate","FX MIDI gate",0,1,0,1,"LATCH|MIDI"});
        s.push_back({"arp_on","ARP Enable",0,1,0,1,"OFF|ON"});
        s.push_back({"arp_hold","ARP Hold",0,1,0,1,"OFF|ON"});
        s.push_back({"arp_sync","ARP Clock",0,1,1,1,"TIME|SYNC"});
        s.push_back({"arp_time","ARP Step (ms)",5,2000,125,0.1f,{}});
        s.push_back({"arp_grid","ARP Rate",0,18,12,1,"1/1|1/2D|1/1T|1/2|1/4D|1/2T|1/4|1/8D|1/4T|1/8|1/16D|1/8T|1/16|1/32D|1/16T|1/32|1/64D|1/32T|1/64"}); // 1.7.0: 19 рейтов от меньшего к большему (Sylenth1), дефолт 1/16
        s.push_back({"arp_play","ARP Mode",0,6,0,1,"STEP|STEP CHORD|TRUE|UP|DOWN|CYCL|RND"}); // 1.6.22: STEPS -- дефолт и первые в списке
        s.push_back({"arp_range","Arp octaves",1,4,1,1,{}});
        s.push_back({"arp_length","Arp gate length",1,127,64,1,{}});
        s.push_back({"arp_wrap","Arp wrap",1,16,16,1,{}});
        s.push_back({"arp_velocity_mode","Arp velocity",0,4,0,1,"STEP|STEP+KEY|STEP+HOLD|KEY|HOLD"}); // 1.6.24: STEP -- дефолт и первые в списке
        s.push_back({"arp_step","Arp step",0,7,0,1,{}});
        s.push_back({"arp_step_velocity","Step velocity",0,127,127,1,{}});
        s.push_back({"arp_step_transpose","Step transpose",-24,24,0,1,{}});
        s.push_back({"arp_step_hold","Step hold",0,1,0,1,"OFF|ON"});
        s.push_back({"arp_step_page","Step page",0,15,0,1,{}});
        s.push_back({"arp_step_page_limit","Step page limit",1,16,1,1,{}});
        s.push_back({"arp_step_random","Page random",0,1,0,1,"OFF|ON"});
        s.push_back({"arp_step_rnd","Step random order",0,1,0,1,"OFF|ON"}); // 1.7.1: STEP RND -- случайный порядок степов страницы (мешок) // 1.7.0 FIX: бинарный -- галка больше не слетает после переоткрытия страницы (0..100 + toggle писал 1 из 100)
        for(int row=0;row<64;++row)s.push_back({"r"+juce::String(row)+"_aux","Route "+juce::String(row+1)+" aux source",0,26,0,1, // 1.7.10 FIX: было 16 -- у строк 16..63 AUX молча не работал (модуляция шла как на 100%)
            "OFF|KEY|VEL|MACRO X|MACRO Y|LFO1|LFO2|LFO3|PITCH WHL|MOD WHL|AFTERTOUCH|MSEG1|MSEG2|MSEG3|STEP VEL|STEP TRANS|ARP GATE|ARP RATE|RANDOM|MSEG4|MSEG5|MSEG6|MSEG7|MSEG8|LFO4|LFO5|LFO6"}); // 1.6.24/25: AUX SOURCE = OFF + порядок ModSource; 1.6.29: MSEG4..8 = 19..23
        for(int row=0;row<64;++row)s.push_back({"r"+juce::String(row)+"_aux_depth","Route "+juce::String(row+1)+" aux depth",-64,63,0,1,{}}); // 1.6.32: вклад AUX, 0 = не влияет; 1.7.10: 64 строки
        for(int pg=0;pg<16;++pg) for(int st=0;st<8;++st){
            const auto prefix="arp_s"+juce::String(pg)+"_"+juce::String(st)+"_";
            s.push_back({prefix+"hold","Step "+juce::String(pg+1)+"/"+juce::String(st+1)+" hold",0,1,0,1,"OFF|ON"});
            s.push_back({prefix+"transpose","Step "+juce::String(pg+1)+"/"+juce::String(st+1)+" transpose",-24,24,0,1,{}});
            s.push_back({prefix+"velocity","Step "+juce::String(pg+1)+"/"+juce::String(st+1)+" velocity",0,127,127,1,{}});
        }
        // DSP mode lists. Entry 0 is always "mnm" (recovered OS 1.32B firmware DSP) -- он же основной;
        // and is the default; entry 1 is "old", the previous Nova algorithm, kept for A/B.
        // Legacy states without these parameters are migrated to "old" (совместимость), затем
        // индексы режимов переводятся из старой нумерации -- см. PluginProcessor (schema<9).
        // 1.6.8: SYNT больше не общий параметр -- режим хранится ПО КАЖДОЙ МАШИНЕ
        // (mode_synt_m<id> ниже), чтобы выбранный режим запоминался при листании машин.
        for(int section=0;section<monomachine::DspSectionCount;++section){
            if(section==monomachine::DspSynt)continue;
            s.push_back({monomachine::dspModeParamId(section),juce::String("DSP ")+monomachine::dspSectionLabel(section)+" mode",0,
                         static_cast<float>(monomachine::dspModeCount-1),static_cast<float>(monomachine::dspModeMnm),1,
                         monomachine::dspModeChoices()});
        }
        for(const auto& m:machines())
            s.push_back({juce::String(monomachine::dspMachineModeParamId(m.id)),juce::String("DSP SYNT mode ")+juce::String(m.name),0,
                         static_cast<float>(monomachine::dspModeCount-1),static_cast<float>(monomachine::dspModeMnm),1,
                         monomachine::dspModeChoices()});
        // ===== 1.8.0: страница P2 -- полноценный FX-движок (p2_ прописка). Пресеты до 1.8 не мигрируем. =====
        {
            const auto& all=monomachine::getAllMachineDefinitions(); // P2 доступен в ОБОИХ версиях: FX-машины и в synth-версии
            std::vector<const monomachine::MachineDef*> fxm;
            for(const auto& m:all) if(m.isEffect) fxm.push_back(&m);
            juce::String p2names;
            for(size_t i=0;i<fxm.size();++i){if(i)p2names+="|";p2names+=juce::String(fxm[i]->name);}
            s.push_back({"p2_machine","P2 FX machine",0,static_cast<float>(fxm.size()-1),1,1,p2names}); // 1.8.0: дефолт THRU (сухой режим, как в pre-FX)
            for(const auto* m:fxm) for(int i=0;i<8;++i){
                const auto& p=m->synthParams[static_cast<size_t>(i)];
                if(p.maxVal==0) continue;
                s.push_back({"p2m"+juce::String(m->id)+"_"+juce::String(i),juce::String("P2 ")+m->name+" "+p.name,static_cast<float>(p.minVal),static_cast<float>(p.maxVal),static_cast<float>(p.defaultVal),1,{}});
            }
        }
        const int p2d[2][8]={{0,127,0,0,0,93,64,64},{64,64,0,64,64,28,0,127}}; // дефолты страниц FILT/EFFX как у P1
        for(int page=0;page<2;++page) for(int i=0;i<8;++i)
            s.push_back({juce::String("p2_")+juce::String(page+1)+"_"+juce::String(i),juce::String("P2 ")+(page==0?"FILTER ":"FX ")+pageLabel(page+1,i),0,127,static_cast<float>(p2d[page][i]),1,{}});
        s.push_back({"p2_amp_atk","P2 AMP ATK",0,127,0,1,{}});
        s.push_back({"p2_amp_hold","P2 AMP HOLD",0,127,0,1,{}}); // 1.8.0: как у P1
        s.push_back({"p2_amp_dec","P2 AMP DEC",0,127,127,1,{}}); // 1.8.0: DEC 127 = оригинальный mnm: ничего не гейтит, уровень держится
        s.push_back({"p2_amp_rel","P2 AMP REL",0,127,127,1,{}}); // 1.8.0: REL 127 = хвосты проходят (как в оригинале на FX-треке)
        s.push_back({"p2_amp_mode","P2 AMP DSP",0,2,1,1,"old|mnm|vital"}); // P2: гейт входа FX (гасит хвосты всего что до него)
        s.push_back({"p2_amp_curve_a","P2 AMP Attack curve",-100,100,0,0.1f,{}});
        s.push_back({"p2_amp_curve_d","P2 AMP Decay curve",-150,150,0,0.1f,{}});
        s.push_back({"p2_amp_curve_r","P2 AMP Release curve",-100,100,0,0.1f,{}});
        s.push_back({"p2_0_4","P2 DIST",0,127,64,1,{}}); // 1.8.0: зеркало AMP-страницы P1 (DIST/VOL/PAN); PORT не возвращаем -- P2 только для эффектов
        s.push_back({"p2_0_5","P2 VOL",0,127,64,1,{}});
        s.push_back({"p2_0_6","P2 PAN",0,127,64,1,{}});
        s.push_back({"p2_mix","P2 MIX",0,127,127,1,{}}); // 1.8.0: DRY/WET второй страницы (слот PORT): 127 = весь P2, 0 = чистый P1
        s.push_back({"p2_mode_filt","P2 DSP FILT mode",0,1,0,1,"mnm|old"});
        s.push_back({"p2_mode_dist","P2 DSP DIST mode",0,1,0,1,"mnm|old"});
        s.push_back({"p2_mode_dly","P2 DSP DLY mode",0,1,0,1,"mnm|old"});
        // 1.6.12: замки PAGE/DEST каждого LFO. *_locks -- битовая маска 0..255
        // (запрещённые значения, модуляция их перескакивает), *_solo 0..8 --
        // единственное разрешённое значение (0 = выкл; модуляция бессильна,
        // руками менять можно, замок не снимается).
        for(int l=0;l<6;++l){ // 1.8.0: LFO1-3 = P1, LFO4-6 = P2 (страницы p3..p8)
            const juce::String pfx="lfo"+juce::String(l+1)+"_";
            s.push_back({pfx+"page_locks","LFO"+juce::String(l+1)+" PAGE locks",0,255,0,1,{}});
            s.push_back({pfx+"dest_locks","LFO"+juce::String(l+1)+" DEST locks",0,255,0,1,{}});
            s.push_back({pfx+"page_solo","LFO"+juce::String(l+1)+" PAGE solo",0,8,0,1,{}});
            s.push_back({pfx+"dest_solo","LFO"+juce::String(l+1)+" DEST solo",0,8,0,1,{}});
        }
        s.push_back({"macro_x","Macro X / MIDI CC1",0,127,64,1,{}});
        s.push_back({"macro_y","Macro Y / MIDI CC11",0,127,0,1,{}});
        s.push_back({"mseg_rate","MSEG rate",0,1,0.5f,0.01f,{}}); // 1.7.8: 0..1 -- позиция в режиме RMODE (TEMPO/TRIPLET/DOTTED/TIME/HZ); 0.5 = 1/4 (старый дефолт 1 Hz/beat)
        s.push_back({"mseg_sync","MSEG host sync",0,1,1,1,"OFF|ON"});
        s.push_back({"mseg_loop","MSEG loop",0,1,1,1,"OFF|ON"});
        s.push_back({"mseg2_rate","MSEG2 rate",0,1,0.5f,0.01f,{}}); // 1.7.8: 0..1 в режиме RMODE // 1.6.21: три кривых MSEG
        s.push_back({"mseg2_sync","MSEG2 host sync",0,1,1,1,"OFF|ON"});
        s.push_back({"mseg2_loop","MSEG2 loop",0,1,1,1,"OFF|ON"});
        s.push_back({"mseg3_rate","MSEG3 rate",0,1,0.5f,0.01f,{}}); // 1.6.21: три кривых MSEG; 1.7.8: 0..1 в режиме RMODE
        s.push_back({"mseg3_sync","MSEG3 host sync",0,1,1,1,"OFF|ON"});
        s.push_back({"mseg3_loop","MSEG3 loop",0,1,1,1,"OFF|ON"});
        for(int mi=4;mi<=8;++mi){const juce::String n="mseg"+juce::String(mi); // 1.6.29: страницы 4..8
            s.push_back({n+"_rate","MSEG"+juce::String(mi)+" rate",0,1,0.5f,0.01f,{}}); // 1.7.8: 0..1 в режиме RMODE
            s.push_back({n+"_sync","MSEG"+juce::String(mi)+" host sync",0,1,1,1,"OFF|ON"});
            s.push_back({n+"_loop","MSEG"+juce::String(mi)+" loop",0,1,1,1,"OFF|ON"});}
        for(int mi=1;mi<=8;++mi){const juce::String n=mi==1?"mseg":"mseg"+juce::String(mi); // 1.7.2: RETRIG постранично (как LOOP/SYNC)
            s.push_back({n+"_retrig","MSEG"+(mi>1?juce::String(mi):juce::String())+" mode",0,5,0,1,"RETRIG|ENVELOPE|FREE|SUSTAIN|LOOP POINT|LOOP HOLD"}); // 1.7.8: как в Vital: VITAL = один проход и держит (был ENVELOPE); SUSTAIN = идёт до LOOP POINT и держит, после note-off доигрывает до конца; LOOP POINT = после первого прохода крутит хвост от точки; LOOP HOLD = держит ноту -- крутит 0..точку, отпустил -- доигрывает хвост
            s.push_back({n+"_rmode","MSEG"+(mi>1?juce::String(mi):juce::String())+" rate mode",0,4,0,1,"TEMPO|TRIPLET|DOTTED|TIME|HZ"}); // 1.7.8: семья шкалы RATE (как в Vital TempoSelector)
            s.push_back({n+"_key","MSEG"+(mi>1?juce::String(mi):juce::String())+" keytrack",0,1,0,1,"OFF|ON"}); // 1.7.8: KEYTRACK -- скорость едет с высотой ноты (октава = x2)
            s.push_back({n+"_lpoint","MSEG"+(mi>1?juce::String(mi):juce::String())+" loop point",0,1,0.5f,0.01f,{}}); // 1.7.8: LOOP POINT для SUSTAIN/LOOP POINT/LOOP HOLD
        } // 1.7.8: конец цикла страниц (FIX: закрывала цикл -- потерялась при замене блока)
        // 1.6.5 debug / 1.6.8: скорость, с которой mnm-дилей следует за ручкой
        // DTIM. Теперь НЕПРЕРЫВНАЯ шкала 0..3: 0 = OFF (мгновенно), 1 = FAST,
        // 2 = MED (дефолт, как было), 3 = максимально долгий "ленточный"
        // repitch ~2.5 с. Промежуточные значения интерполируются.
        s.push_back({"dly_repitch","DLY REPITCH",0.0f,3.0f,2.0f,0.01f,{}});
        // 1.6.8: доводка-сглаживание (антиклик) при резкой смене длины:
        // 0 = выключено (как было), 127 = самый мягкий (~10 мс доводка).
        s.push_back({"dly_repitch_smooth","DLY RECLICK SMOOTH",0,127,0,1,{}});
        // 1.6.12: альтернативный ping-pong дилея: CLASSIC = как в прошивке (моно в
        // левую линию, всё в сайде), MID SAFE = тапы смешиваются 65/35 (середина не вычитается).
        s.push_back({"dly_ppmode","DLY PINGPONG MODE",0,1,0,1,"CLASSIC|MID SAFE"});
        juce::String targets="--"; // 1.6.33: индекс 0 = OFF (прочерк, цель = индекс-1); 65 значений -- MSEG8 OUT (64) теперь выбирается
        for(int i=0;i<64;++i) {targets+="|";targets+=i<8?"SYN "+juce::String(i+1):i<56?juce::String(sections[(i-8)/8])+" "+pageLabel((i-8)/8,(i-8)%8):"MSEG"+juce::String(i-55)+" OUT";}targets+="|ARP RATE|ARP GATE";
        targets+="|PITCH";for(int rn=0;rn<64;++rn){targets+="|ROUTE "+juce::String(rn+1);} // 1.7.7: PITCH (66) + ROUTE 1..64 (67..130 -- глубина любого из 64 слотов матрицы)
        targets+="|LFO FM"; // 1.7.8: цель 131 -- частота LFO/MSEG в октавах (до ~20 kHz: 2^13.3)
        for(int p2t=0;p2t<32;++p2t){ // 1.8.0: цели P2 132..163 (SYNT/AMP/FILT/EFFX страницы 2)
            const int sec=(p2t-8)/8;targets+=juce::String("|P2 ")+(p2t==15?"AMP MIX":p2t<8?"SYN "+juce::String(p2t+1):juce::String(sec==0?"AMP ":sec==1?"FILT ":"FX ")+pageLabel(sec,(p2t-8)%8));} // 1.8.0: AMP/FILT/FX как у P1; 15 = PORT не нужен -- прочерк
        for(int row=0;row<64;++row) { // 1.7.7: 64 слота матрицы
            const auto prefix="r"+juce::String(row)+"_";
            s.push_back({prefix+"on","Route "+juce::String(row+1)+" enable",0,1,1,1,"OFF|ON"}); // 1.7.7: все слоты ВКЛЮЧЕНЫ по умолчанию -- сразу можно биндить
            // 1.6.12: замок маршрута: FREE = обычный, LOCK = прицел/перезапись не
            // трогают dest, SOLO = приколочен к этому параметру (только руками).
            s.push_back({prefix+"lock","Route "+juce::String(row+1)+" lock",0,2,0,1,"FREE|LOCK|SOLO"});
            s.push_back({prefix+"src","Route source",0,25,0,1,"KEY|VEL|MACRO X|MACRO Y|LFO1|LFO2|LFO3|PITCH WHL|MOD WHL|AFTERTOUCH|MSEG1|MSEG2|MSEG3|STEP VEL|STEP TRANS|ARP GATE|ARP RATE|RANDOM|MSEG4|MSEG5|MSEG6|MSEG7|MSEG8|LFO4|LFO5|LFO6"}); // 1.6.25: порядок = ModSource; 1.6.29: 23 источника, MSEG4..8 = 18..22
            s.push_back({prefix+"dest","Route target",0,164,0,1,targets}); // 1.7.8: цель = значение-1, 0..132 (0 = OFF); 131 = LFO FM // 1.8.0: 0..164 -- +P2-цели 132..163
            s.push_back({prefix+"depth","Route depth",-64,63,0,1,{}});
            s.push_back({prefix+"mode","Route polarity",0,1,0,1,"UNIPOLAR|BIPOLAR"});
        }
        return s;
    }();
    return all;
}
}
