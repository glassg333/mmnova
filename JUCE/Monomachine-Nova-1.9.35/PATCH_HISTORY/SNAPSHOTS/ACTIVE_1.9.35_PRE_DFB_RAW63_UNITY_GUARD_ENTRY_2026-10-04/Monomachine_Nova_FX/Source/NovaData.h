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

// ARP pattern pages are deliberately independent from song positions.  A page
// has sixteen note/P-LOCK steps; the SONG lane has up to sixty-four positions
// which select one of these sixty-four pages.
inline constexpr int kArpPageCount = 64;
inline constexpr int kArpStepsPerPage = 16;
inline constexpr int kArpTotalSteps = kArpPageCount * kArpStepsPerPage;
inline constexpr int kSongPartCount = 64;
inline juce::String arpStepParam(int page,int step,const char* field) {
    return "arp_s"+juce::String(page)+"_"+juce::String(step)+"_"+field;
}
inline juce::String arpPageParam(int page,const char* field) {
    return "arp_pg"+juce::String(page)+"_"+field;
}
inline juce::String songPartParam(int part,const char* field) {
    return "arp_song_s"+juce::String(part)+"_"+field;
}
inline const char* pageLabel(int page,int index) {
    static const char* labels[15][8]={ // 1.8.0e: +P2 LFO1-6 (страницы 9..14)
        {"ATK","HOLD","DEC","REL","DIST","VOL","PAN","PORT"},
        {"BASE","WDTH","HPQ","LPQ","ATK","DEC","BOFS","WOFS"},
        {"EQF","EQG","SRR","DTIM","DSND","DFB","DBAS","DWID"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"},
        {"PAGE","DEST","TRIG","WAVE","MULT","SPD","INTL","DPTH"}};
    return labels[page][index];
}

// One canonical translation table for every direct LFO PAGE/DEST selector.
// The parameter values deliberately keep the 1.8.x numbering: existing host
// automation and saved PAGE values therefore retain their destination.  The UI
// is free to present P2's legacy value zero under P1 PARAM > PITCH instead of
// exposing it as a misleading top-level P2 item.
inline constexpr int kLfoCountPerSide = 6;
inline constexpr int kLfoDestinationCount = 8;
inline constexpr int kLfoPageChoiceCount = 21;
inline constexpr int kP1LfoControlPageFirst = 3;
inline constexpr int kP2LfoControlPageFirst = 9;
inline constexpr int kPitchMatrixTarget = 66;

inline bool lfoSourceIsP2FromControlPage(int controlPage) noexcept { return controlPage >= kP2LfoControlPageFirst; }
inline int lfoControlPage(bool sourceP2,int lfoIndex) noexcept {
    return (sourceP2 ? kP2LfoControlPageFirst : kP1LfoControlPageFirst)
        + juce::jlimit(0,kLfoCountPerSide-1,lfoIndex);
}

// PAGE text is intentionally explicit for the neighbouring side.  It is used
// by the lock panel as well as popup menus, so no display path can silently
// drift from the DSP mapping below.
inline juce::String lfoPageName(bool sourceP2,int page) {
    page=juce::jlimit(0,kLfoPageChoiceCount-1,page);
    if(!sourceP2){
        if(page==0)return "PITCH";
        if(page<=4){static const char* n[]{"SYNT","AMP","FILT","EFFX"};return n[page-1];}
        if(page<=10)return "P1 LFO"+juce::String(page-4);
        if(page<=14){static const char* n[]{"P2 SYNT","P2 AMP","P2 FILT","P2 EFFX"};return n[page-11];}
        return "P2 LFO"+juce::String(page-14);
    }
    // Value zero has always been pitch for P2 LFOs.  Keep that stable value,
    // but name it where it really belongs: P1 PARAM > PITCH.
    if(page==0)return "P1 PITCH";
    if(page<=4){static const char* n[]{"P2 SYNT","P2 AMP","P2 FILT","P2 EFFX"};return n[page-1];}
    if(page<=10)return "P2 LFO"+juce::String(page-4);
    if(page<=14){static const char* n[]{"P1 SYNT","P1 AMP","P1 FILT","P1 EFFX"};return n[page-11];}
    return "P1 LFO"+juce::String(page-14);
}

// Returns the stable matrix target represented by a direct LFO PAGE/DEST
// pair.  Target 66 is the physical pitch destination; the DEST field selects
// one of its eight pitch ranges.  -1 means an invalid page only.
inline int lfoDirectTarget(bool sourceP2,int page,int dest) noexcept {
    page=juce::jlimit(0,kLfoPageChoiceCount-1,page);
    dest=juce::jlimit(0,kLfoDestinationCount-1,dest);
    juce::ignoreUnused(dest);
    if(!sourceP2){
        if(page==0)return kPitchMatrixTarget;
        if(page<=4)return (page-1)*8+dest;              // P1 SYNT/AMP/FILT/EFFX
        if(page<=7)return 32+(page-5)*8+dest;           // P1 LFO1..3
        if(page<=10)return 188+(page-8)*8+dest;         // P1 LFO4..6
        if(page<=14)return 132+(page-11)*8+dest;        // P2 SYNT/AMP/FILT/EFFX
        if(page<=17)return 164+(page-15)*8+dest;        // P2 LFO1..3
        return 212+(page-18)*8+dest;                    // P2 LFO4..6
    }
    if(page==0)return kPitchMatrixTarget;               // P1 PITCH (legacy P2 value zero)
    if(page==1)return 132+dest;                         // P2 SYNT
    if(page<=4)return 140+(page-2)*8+dest;              // P2 AMP/FILT/EFFX
    if(page<=7)return 164+(page-5)*8+dest;              // P2 LFO1..3
    if(page<=10)return 212+(page-8)*8+dest;             // P2 LFO4..6
    if(page<=14)return (page-11)*8+dest;                // P1 SYNT/AMP/FILT/EFFX
    if(page<=17)return 32+(page-15)*8+dest;             // P1 LFO1..3
    return 188+(page-18)*8+dest;                        // P1 LFO4..6
}

// Inverse for cable/matrix navigation.  It accepts only destinations which
// have a direct LFO PAGE/DEST representation; ARP, MSEG, route-depth and the
// retained legacy LFO-FM target correctly return false.
inline bool lfoPageForDirectTarget(bool sourceP2,int target,int& page) noexcept {
    if(target==kPitchMatrixTarget){page=0;return true;}
    if(!sourceP2){
        if(target>=0&&target<32){page=target/8+1;return true;}
        if(target>=32&&target<56){page=5+(target-32)/8;return true;}
        if(target>=188&&target<212){page=8+(target-188)/8;return true;}
        if(target>=132&&target<164){page=11+(target-132)/8;return true;}
        if(target>=164&&target<188){page=15+(target-164)/8;return true;}
        if(target>=212&&target<236){page=18+(target-212)/8;return true;}
    }else{
        if(target>=132&&target<164){page=1+(target-132)/8;return true;}
        if(target>=164&&target<188){page=5+(target-164)/8;return true;}
        if(target>=212&&target<236){page=8+(target-212)/8;return true;}
        if(target>=0&&target<32){page=11+target/8;return true;}
        if(target>=32&&target<56){page=15+(target-32)/8;return true;}
        if(target>=188&&target<212){page=18+(target-188)/8;return true;}
    }
    page=-1;return false;
}

// Direct-LFO DEST is a page-local slot, not target % 8. P1 target banks
// happen to begin on an eight-boundary; P2 begins at 132, so target % 8
// rotates P2's physical top/bottom rows by four (DSND<->EQF, etc.). This
// covers direct selectors on either side, including a cross-side target.
inline int lfoDirectDestinationForTarget(bool sourceP2,int target) noexcept {
    juce::ignoreUnused(sourceP2);
    if(target==kPitchMatrixTarget)return 0;
    if(target>=0&&target<56)return target%8;
    if(target>=132&&target<164)return (target-132)%8;
    if(target>=164&&target<188)return (target-164)%8;
    if(target>=188&&target<212)return (target-188)%8;
    if(target>=212&&target<236)return (target-212)%8;
    return 0;
}

inline bool targetLivesOnP2Surface(int target) noexcept {
    return (target>=132&&target<164)||(target>=164&&target<188)||(target>=212&&target<236);
}
inline bool targetLivesOnP1Surface(int target) noexcept {
    return (target>=0&&target<56)||(target>=188&&target<212)||target==kPitchMatrixTarget;
}
inline int targetLfoIndexOnSurface(bool p2,int target) noexcept {
    if(p2){
        if(target>=164&&target<188)return (target-164)/8;
        if(target>=212&&target<236)return 3+(target-212)/8;
    }else{
        if(target>=32&&target<56)return (target-32)/8;
        if(target>=188&&target<212)return 3+(target-188)/8;
    }
    return -1;
}

// BBOX slot selectors are categorical sound-management controls, not smooth
// DSP destinations.  Retain their target IDs for old matrix/state data, but
// do not offer or apply them through a direct LFO PAGE/DEST assignment.
inline bool lfoDirectTargetIsContinuousForCurrentP1Machine(int machineId,int target) noexcept {
    return !(machineId==7 && target>=0 && target<8 && (target==2||target==3||target==6));
}

struct Spec {juce::String id,name; float lo=0,hi=127,def=0,step=1; juce::String choices;};

// The hybrid IDs are physical-side choice indices. Values 0..8 are frozen
// for 1.9.8 compatibility; 9..20 append the R-classic choices in 1.9.9;
// 21..51 append R Import 2; and 52..57 append requested HP complements for
// the six R cores that originally exposed LP output only.  No saved ID moves.
// Keep these strings in the same order as HybridDSP.hpp's
// kModeLChoiceToAlgorithm/kModeHChoiceToAlgorithm tables.
inline constexpr int kHybridFilterChoiceCount = 58;
inline const juce::String& hybridModeLChoices() {
    static const juce::String choices{
        "NATIVE|K35 HP|MOOG HP24|MOOG HP12|MOOG BP24|MOOG BP12|K35 LP|MOOG LP24|MOOG LP12|"
        "R 303 HP|R MS20 HP|R MOOG HP24|R MOOG HP12|R 303 BP|R MS20 BP|R MOOG BP24|R MOOG BP12|"
        "R 303 LP|R MS20 LP|R MOOG LP24|R MOOG LP12|"
        "R ANALOG HP24|R ANALOG HP12|R ANALOG BP24|R ANALOG BP12|R ANALOG LP24|R ANALOG LP12|"
        "R LINEAR HP24|R LINEAR HP12|R LINEAR BP24|R LINEAR BP12|R LINEAR LP24|R LINEAR LP12|"
        "R RBJ HP|R RBJ BP|R RBJ LP|R TPT HP|R TPT BP|R TPT LP|R HUV LP4|"
        "R HYPER HP4|R HYPER HP2|R HYPER BP4|R HYPER BP2|R HYPER NOTCH|R HYPER LP4|R HYPER LP2|"
        "R KRAJ LP4|R MICRO LP4|R MUSIC LP4|R OBERHEIM LP4|R DVAL LP4|"
        "R HUV HP4|R KRAJ HP4|R MICRO HP4|R MUSIC HP4|R OBERHEIM HP4|R DVAL HP4"};
    return choices;
}
inline const juce::String& hybridModeHChoices() {
    static const juce::String choices{
        "NATIVE|K35 LP|MOOG LP24|MOOG LP12|MOOG BP24|MOOG BP12|K35 HP|MOOG HP24|MOOG HP12|"
        "R 303 LP|R MS20 LP|R MOOG LP24|R MOOG LP12|R 303 BP|R MS20 BP|R MOOG BP24|R MOOG BP12|"
        "R 303 HP|R MS20 HP|R MOOG HP24|R MOOG HP12|"
        "R ANALOG LP24|R ANALOG LP12|R ANALOG BP24|R ANALOG BP12|R ANALOG HP24|R ANALOG HP12|"
        "R LINEAR LP24|R LINEAR LP12|R LINEAR BP24|R LINEAR BP12|R LINEAR HP24|R LINEAR HP12|"
        "R RBJ LP|R RBJ BP|R RBJ HP|R TPT LP|R TPT BP|R TPT HP|R HUV LP4|"
        "R HYPER LP4|R HYPER LP2|R HYPER BP4|R HYPER BP2|R HYPER NOTCH|R HYPER HP4|R HYPER HP2|"
        "R KRAJ LP4|R MICRO LP4|R MUSIC LP4|R OBERHEIM LP4|R DVAL LP4|"
        "R HUV HP4|R KRAJ HP4|R MICRO HP4|R MUSIC HP4|R OBERHEIM HP4|R DVAL HP4"};
    return choices;
}

inline const std::vector<Spec>& specs() {
    static const std::vector<Spec> all=[] {
        std::vector<Spec> s;
        juce::String names;
        int defaultMachine=0,index=0;
        for(const auto& m:machines()) {if(names.isNotEmpty()) names+="|";names+=juce::String(m.name);if(m.id==(NOVA_SYNTH?9:15)) defaultMachine=index;++index;} // 1.7.5: FM+ PAR -- машина по умолчанию; 1.8.1: в FX-версии P1 = FX-CHORUS (первая страница -- рабочая, P2 = запас)
        s.push_back({"machine","Machine",0,static_cast<float>(machines().size()-1),static_cast<float>(defaultMachine),1,names});
        static const float kChorusDef[8]={70,95,41,0,127,127,127,64}; // 1.8.0b: панель FX-CHORUS юзера: DEL DEP SPD MIX FB WID LP INP // 1.8.1b: MIX=0 (подмешивает юзер), INP=64 (центр)
        for(const auto& m:machines()) for(int i=0;i<8;++i) {
            const auto& p=m.synthParams[static_cast<size_t>(i)];
            if(p.maxVal==0) continue;
            const float hdv=(!NOVA_SYNTH&&m.id==15)?kChorusDef[i]:static_cast<float>(p.defaultVal); // 1.8.1: в FX-версии панель хоруса юзера -- на P1 (открывается первой)
            juce::String choices;
            if(m.id==4 && i==3)choices="old|mnm";
            if(m.id==2 && i==2)choices="OFF|ON";
            if(m.id==7 && i==2) choices="1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24";
            if(m.id==7 && (i==3 || i==6)) choices="OFF|ON";
            s.push_back({machineParam(m.id,i),juce::String(m.name)+" "+juce::String(p.name),static_cast<float>(p.minVal),static_cast<float>(p.maxVal),hdv,1,choices});
        }
        // DIST and DSND are centred bipolar: raw 64 reads 0; DSND<0 selects split L/R negative comb.
        const int defaults[15][8]={{0,0,64,64,64,64,64,0},{0,127,0,0,0,32,64,64},{64,64,0,64,64,28,0,127},
                                 {0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},
                                 {0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},
                                 {0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},
                                 {0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64},{0,6,0,0,0,64,0,64}};
        const char* sections[]{"AMP","FILTER","FX","LFO1","LFO2","LFO3","LFO4","LFO5","LFO6","P2 LFO1","P2 LFO2","P2 LFO3","P2 LFO4","P2 LFO5","P2 LFO6"}; // 1.8.0e: страницы 6..8 = LFO4-6 (P1!), 9..14 = P2 LFO1-6
        for(int page=0;page<15;++page) for(int i=0;i<8;++i) { // 1.8.0e: +страницы 9..14
            juce::String choices;
            if(page>=3) {
                if(i==0){ // Keep the established 0..20 values, but make every label describe its true side.
                    const bool sourceP2=page>=kP2LfoControlPageFirst;
                    for(int v=0;v<kLfoPageChoiceCount;++v){if(v)choices+="|";choices+=lfoPageName(sourceP2,v);}
                }
                if(i==2) choices="FREE|TRIG|HOLD|ONE|HALF";
                if(i==3) choices="TRI|ITRI|SAW|ISAW|SQR|ISQR|EXP|IEXP|RMP|IRMP|RND";
                if(i==4) choices="1X|2X|4X|8X|16X|32X|64X";
            }
            // DPTH (index 7) is an amount, no longer a legacy centre-coded
            // bias: 0 is silent/default, 127 is full. ALT DUAL may use -127.
            const bool directDepth=page>=3 && i==7;
            const float high=choices.isEmpty()?(page>=3 && i==1?7.0f:127.0f):static_cast<float>(juce::StringArray::fromTokens(choices,"|","").size()-1);
            s.push_back({pageParam(page,i),juce::String(sections[page])+" "+pageLabel(page,i),directDepth?-127.0f:0.0f,high,directDepth?0.0f:static_cast<float>(defaults[page][i]),1,choices});
        }
        // 1.6.14: old AMP DSP удалён (выбор бесполезен) -- параметр оставлен для
        // совместимости старых состояний, UI и DSP его больше не читают.
        s.push_back({"amp_mode","AMP DSP",0,2,1,1,"old|mnm|vital"});
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
        // Retained IDs keep old host automation readable. The current page
        // selector now spans 64 page banks; the live bounds are page-local.
        s.push_back({"arp_step_page","ARP edit page",0,static_cast<float>(kArpPageCount-1),0,1,{}});
        s.push_back({"arp_step_page_limit","Legacy step page limit",1,static_cast<float>(kArpPageCount),1,{}});
        s.push_back({"arp_step_random","Legacy page random",0,1,0,1,"OFF|ON"});
        s.push_back({"arp_step_rnd","Legacy step random order",0,2,0,1,"OFF|ALL SYNC|ALL UNSYNC"});
        s.push_back({"arp_step_start","ARP legacy pattern start",1,64,1,1,{}});
        s.push_back({"arp_step_end","ARP legacy pattern end",1,64,16,1,{}});
        s.push_back({"plock_sync","P-LOCK sync",0,1,1,1,"OFF|ON"});
        s.push_back({"plock_step_start","P-LOCK legacy pattern start",1,64,1,1,{}});
        s.push_back({"plock_step_end","P-LOCK legacy pattern end",1,64,16,1,{}});

        // Every page owns a 1..16 note/P-LOCK pass and its own random flags.
        // PAGE RND and STEP RND therefore remain with a page for song fills.
        for(int pg=0;pg<kArpPageCount;++pg){
            const auto pageName="ARP PAGE "+juce::String(pg+1)+" ";
            s.push_back({arpPageParam(pg,"start"),pageName+"start",1,kArpStepsPerPage,1,1,{}});
            s.push_back({arpPageParam(pg,"end"),pageName+"end",1,kArpStepsPerPage,kArpStepsPerPage,1,{}});
            s.push_back({arpPageParam(pg,"plock_start"),pageName+"P-LOCK start",1,kArpStepsPerPage,1,1,{}});
            s.push_back({arpPageParam(pg,"plock_end"),pageName+"P-LOCK end",1,kArpStepsPerPage,kArpStepsPerPage,1,{}});
            s.push_back({arpPageParam(pg,"page_rnd"),pageName+"page random",0,1,0,1,"OFF|ON"});
            s.push_back({arpPageParam(pg,"step_rnd"),pageName+"step random",0,2,0,1,"OFF|ALL SYNC|ALL UNSYNC"});
        }
        // SONG chooses ARP and P-LOCK page together. Positions 1 and 2 are
        // active by default; all 64 positions start at their one-based number.
        s.push_back({"arp_song_mode","ARP song mode",0,1,0,1,"OFF|ON"});
        s.push_back({"arp_song_part_start","ARP song part start",1,kSongPartCount,1,1,{}});
        s.push_back({"arp_song_part_end","ARP song part end",1,kSongPartCount,2,1,{}});
        for(int part=0;part<kSongPartCount;++part){
            s.push_back({songPartParam(part,"page"),"ARP SONG part "+juce::String(part+1)+" page",1,kArpPageCount,static_cast<float>(part+1),1,{}});
            s.push_back({songPartParam(part,"repeat"),"ARP SONG part "+juce::String(part+1)+" repeat",1,127,1,1,{}});
        }
        for(int row=0;row<64;++row)s.push_back({"r"+juce::String(row)+"_aux","Route "+juce::String(row+1)+" aux source",0,36,0,1, // OFF plus all 36 matrix sources, including MOD ENV1..4.
            "OFF|KEY|VEL|MACRO X|MACRO Y|LFO1|LFO2|LFO3|PITCH WHL|MOD WHL|AFTERTOUCH|MSEG1|MSEG2|MSEG3|STEP VEL|STEP TRANS|ARP GATE|ARP RATE|RANDOM|MSEG4|MSEG5|MSEG6|MSEG7|MSEG8|LFO4|LFO5|LFO6|P2 LFO1|P2 LFO2|P2 LFO3|P2 LFO4|P2 LFO5|P2 LFO6|MOD ENV1|MOD ENV2|MOD ENV3|MOD ENV4"}); // 1.6.24/25: AUX SOURCE = OFF + порядок ModSource; 1.6.29: MSEG4..8 = 19..23
        for(int row=0;row<64;++row)s.push_back({"r"+juce::String(row)+"_aux_depth","Route "+juce::String(row+1)+" aux depth",-64,63,0,1,{}}); // 1.6.32: вклад AUX, 0 = не влияет; 1.7.10: 64 строки
        for(int pg=0;pg<kArpPageCount;++pg) for(int st=0;st<kArpStepsPerPage;++st){
            const auto prefix="arp_s"+juce::String(pg)+"_"+juce::String(st)+"_";
            s.push_back({prefix+"hold","Step "+juce::String(pg+1)+"/"+juce::String(st+1)+" hold",0,1,0,1,"OFF|ON"});
            s.push_back({prefix+"transpose","Step "+juce::String(pg+1)+"/"+juce::String(st+1)+" transpose",-24,24,0,1,{}});
            s.push_back({prefix+"velocity","Step "+juce::String(pg+1)+"/"+juce::String(st+1)+" velocity",0,127,127,1,{}});
        }
        // DSP mode lists keep stable mnm|old values for automation.  DLY keeps
        // NEW at index 2; only FM+ m8/m9/m10 SYNT parameters additionally own
        // NEW=2, MNM FIX=3, NEW FIX=4 and appended OLD FIX=5. Existing IDs remain intact.
        // New instances default FILT and DLY to the retained OLD paths while
        // existing saved mode values are left untouched by state loading.
        // 1.6.8: SYNT больше не общий параметр -- режим хранится ПО КАЖДОЙ МАШИНЕ
        // (mode_synt_m<id> ниже), чтобы выбранный режим запоминался при листании машин.
        for(int section=0;section<monomachine::DspSectionCount;++section){
            if(section==monomachine::DspSynt)continue;
            const int defaultMode=(section==monomachine::DspFilter||section==monomachine::DspDelay)
                ?monomachine::dspModeOld:monomachine::dspModeMnm;
            s.push_back({monomachine::dspModeParamId(section),juce::String("DSP ")+monomachine::dspSectionLabel(section)+" mode",0,
                         static_cast<float>(monomachine::dspSectionModeCount(section)-1),static_cast<float>(defaultMode),1,
                         monomachine::dspSectionParameterChoices(section)});
        }
        for(const auto& m:machines()) {
            // SYNT is per machine: NEW=2 is deliberately appended only to the
            // three FM+ parameters.  All other machines retain their exact
            // old mnm|old choice list, range, default, and parameter ID.
            const int syntModeCount=monomachine::dspSyntModeCountForMachine(m.id);
            s.push_back({juce::String(monomachine::dspMachineModeParamId(m.id)),juce::String("DSP SYNT mode ")+juce::String(m.name),0,
                         static_cast<float>(syntModeCount-1),static_cast<float>(monomachine::dspModeMnm),1,
                         monomachine::dspSyntModeChoicesForMachine(m.id)});
        }
        // ===== 1.8.0: страница P2 -- полноценный FX-движок (p2_ прописка). Пресеты до 1.8 не мигрируем. =====
        {
            const auto& all=monomachine::getAllMachineDefinitions(); // P2 доступен в ОБОИХ версиях: FX-слоты и в synth-версии
            std::vector<const monomachine::MachineDef*> fxm;
            for(const auto& m:all) if(m.isEffect) fxm.push_back(&m);
            juce::String p2names;
            int chorusIdx=0;
            for(size_t i=0;i<fxm.size();++i){if(i)p2names+="|";p2names+=juce::String(fxm[i]->name);if(fxm[i]->id==15)chorusIdx=static_cast<int>(i);}
            s.push_back({"p2_machine","P2 FX slot",0,static_cast<float>(fxm.size()-1),static_cast<float>(chorusIdx),1,p2names}); // native FX list stays unchanged
            for(const auto* m:fxm) for(int i=0;i<8;++i){
                const auto& p=m->synthParams[static_cast<size_t>(i)];
                if(p.maxVal==0) continue;
                const float dv=m->id==15?kChorusDef[i]:p.defaultVal;
                s.push_back({"p2m"+juce::String(m->id)+"_"+juce::String(i),juce::String("P2 ")+m->name+" "+p.name,static_cast<float>(p.minVal),static_cast<float>(p.maxVal),dv,1,{}});
                (void)dv;
            }
        }
        const int p2d[2][8]={{0,127,0,0,0,93,64,64},{64,64,0,64,64,28,0,127}}; // дефолты страниц FILT/EFFX как у P1
        for(int page=0;page<2;++page) for(int i=0;i<8;++i)
            s.push_back({juce::String("p2_")+juce::String(page+1)+"_"+juce::String(i),juce::String("P2 ")+(page==0?"FILTER ":"FX ")+pageLabel(page+1,i),0,127,static_cast<float>(p2d[page][i]),1,{}});
        s.push_back({"p2_amp_atk","P2 AMP ATK",0,127,0,1,{}});
        s.push_back({"p2_amp_hold","P2 AMP HOLD",0,127,0,1,{}}); // 1.8.0: как у P1
        s.push_back({"p2_amp_dec","P2 AMP DEC",0,127,127,1,{}}); // 1.8.0: DEC 127 = оригинальный mnm: ничего не гейтит, уровень держится
        s.push_back({"p2_amp_rel","P2 AMP REL",0,127,127,1,{}}); // 1.8.0: REL 127 = хвосты проходят (как в оригинале на FX-треке)
        s.push_back({"p2_amp_mode","P2 AMP DSP",0,2,1,1,"old|mnm|vital"});
        s.push_back({"p2_amp_curve_a","P2 AMP Attack curve",-100,100,0,0.1f,{}});
        s.push_back({"p2_amp_curve_d","P2 AMP Decay curve",-150,150,0,0.1f,{}});
        s.push_back({"p2_amp_curve_r","P2 AMP Release curve",-100,100,0,0.1f,{}});
        s.push_back({"p2_0_4","P2 DIST",0,127,64,1,{}}); // 1.8.0: зеркало AMP-страницы P1 (DIST/VOL/PAN); PORT не возвращаем -- P2 только для эффектов
        s.push_back({"p2_0_5","P2 VOL",0,127,64,1,{}});
        s.push_back({"p2_0_6","P2 PAN",0,127,64,1,{}});
        s.push_back({"p2_mix","P2 MIX",0,127,127,1,{}}); // 1.8.0: DRY/WET второй страницы (слот PORT): 127 = весь P2, 0 = чистый P1
        s.push_back({"p2_mode_filt","P2 DSP FILT mode",0,static_cast<float>(monomachine::dspSectionModeCount(monomachine::DspFilter)-1),static_cast<float>(monomachine::dspModeOld),1,monomachine::dspSectionParameterChoices(monomachine::DspFilter)});
        s.push_back({"p2_mode_dist","P2 DSP DIST mode",0,static_cast<float>(monomachine::dspSectionModeCount(monomachine::DspDist)-1),0,1,monomachine::dspSectionParameterChoices(monomachine::DspDist)});
        s.push_back({"p2_mode_dly","P2 DSP DLY mode",0,static_cast<float>(monomachine::dspSectionModeCount(monomachine::DspDelay)-1),static_cast<float>(monomachine::dspModeOld),1,monomachine::dspSectionParameterChoices(monomachine::DspDelay)});
        // 1.9.8: DBAS/DWID remain the two visible delay feedback-filter
        // controls.  Their optional Q values are appended state parameters and
        // appear only in the right-click panel, keeping the eight-knob page and
        // older project values intact.  Q=0 is neutral.
        s.push_back({"dly_dbas_q","DLY DBAS feedback Q",0,127,0,1,{}});
        s.push_back({"dly_dwid_q","DLY DWID feedback Q",0,127,0,1,{}});
        s.push_back({"p2_dly_dbas_q","P2 DLY DBAS feedback Q",0,127,0,1,{}});
        s.push_back({"p2_dly_dwid_q","P2 DLY DWID feedback Q",0,127,0,1,{}});
        // DFB controls live only in the in-editor DFB RMB panel. DYNAMICS is
        // ON by default; BASE CURVE keeps RAW=63 as reference, lifts RAW 60..62
        // smoothly toward it, and HOLD @64 provides a tiny separate trim.
        // FB CLIP enables the separately adjustable raw-gated GUARD: RAW 0..63
        // is bypassed, while 64..127 interpolates explicit start/plateau
        // anchors. This is a user safety design rather than a
        // claimed recovered hardware formula. Existing eight EFFX knobs and
        // DBAS/DWID's retained 12 dB response remain untouched.
        // DYNAMICS, both DSND feedback-invert toggles, return compensation and
        // FB CLIP start ON by the user-selected reference state.
        for(const char* prefix:{"dly_","p2_dly_"}){
            const juce::String title=juce::String(prefix).startsWith("p2_")?"P2 DLY DFB ":"DLY DFB ";
            s.push_back({juce::String(prefix)+"dfb_dynamics",title+"DYNAMICS",0,1,1,1,"ВЫКЛ|ВКЛ"});
            s.push_back({juce::String(prefix)+"dfb_rise",title+"RISE",0.20f,2.20f,1.20f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_zero_tail",title+"ZERO TAIL",0.12f,1.20f,0.65f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dsnd_pos_invert",title+"DSND + FB INVERT",0,1,1,1,"ВЫКЛ|ВКЛ"});
            s.push_back({juce::String(prefix)+"dsnd_neg_invert",title+"DSND - FB INVERT",0,1,1,1,"ВЫКЛ|ВКЛ"});
            s.push_back({juce::String(prefix)+"dfb_return_comp",title+"КОМП. ВОЗВРАТА",0,1,1,1,"ВЫКЛ|ВКЛ"});
            s.push_back({juce::String(prefix)+"dfb_clip",title+"КЛИПЕР ПЕТЛИ",0,1,1,1,"ВЫКЛ|ВКЛ"});
        }
        // PRIVATE HYBRID DSP TEST: controls are isolated in dsp/hybrid_private
        // MODE L and MODE H select the physical lower/upper filter sides.
        // Their host values follow each side's physical direction, not FilterCore
        // canonical IDs. MODE S is the sole user-facing selection for the one
        // existing physical DIST block; OLD remains a separate reference.
        for(const char* prefix:{"hybrid_p1_","hybrid_p2_"}){
            const bool p2=juce::String(prefix).startsWith("hybrid_p2_");
            const juce::String sidePrefix=p2?"P2 ":"";
            s.push_back({juce::String(prefix)+"mode_l",sidePrefix+"MODE L",0,static_cast<float>(kHybridFilterChoiceCount-1),0,1,hybridModeLChoices()});
            s.push_back({juce::String(prefix)+"mode_h",sidePrefix+"MODE H",0,static_cast<float>(kHybridFilterChoiceCount-1),0,1,hybridModeHChoices()});
            s.push_back({juce::String(prefix)+"mode_s",sidePrefix+"MODE S",0,8,0,1,"MNM|OLD|MNM FIX|FOLD|ZERO|CLAMP|MNM+OLD|MNM V2|OLD V2"});
        }
        // Explicit filter extras. They sit outside the historic eight FILT
        // knobs, so old project values remain native/neutral. VEL/KT are
        // direct physical-side cutoff controls; SAT is one opt-in post-FILT
        // Korg/Odin overdrive stage. FIL ENV is a second, additive envelope:
        // ENV FIL=0 is an exact no-op.
        for(const char* prefix:{"filt_","p2_filt_"}){
            const bool p2=juce::String(prefix).startsWith("p2_");
            const juce::String title=p2?"P2 FILTER ":"FILTER ";
            s.push_back({juce::String(prefix)+"vel_l",title+"VEL-L",0,127,0,1,{}});
            s.push_back({juce::String(prefix)+"vel_h",title+"VEL-H",0,127,0,1,{}});
            s.push_back({juce::String(prefix)+"kt_l",title+"KT-L",0,127,0,1,{}});
            s.push_back({juce::String(prefix)+"kt_h",title+"KT-H",0,127,0,1,{}});
            s.push_back({juce::String(prefix)+"sat",title+"SAT",0,127,0,1,{}});
            s.push_back({juce::String(prefix)+"env_atk",title+"FIL ENV ATK",0,127,0,1,{}});
            s.push_back({juce::String(prefix)+"env_hold",title+"FIL ENV HOLD",0,127,0,1,{}});
            s.push_back({juce::String(prefix)+"env_dec",title+"FIL ENV DEC",0,127,127,1,{}});
            s.push_back({juce::String(prefix)+"env_rel",title+"FIL ENV REL",0,127,127,1,{}});
            s.push_back({juce::String(prefix)+"env_mix",title+"ENV FIL",0,127,0,1,{}});
            s.push_back({juce::String(prefix)+"env_base_depth",title+"FIL ENV BASE depth",-64,63,0,1,{}});
            s.push_back({juce::String(prefix)+"env_width_depth",title+"FIL ENV WDTH depth",-64,63,0,1,{}});
            // Original-style independent keyboard tracking. These are switches,
            // not the older optional continuous KT modulation amounts above.
            s.push_back({juce::String(prefix)+"track_hpf",title+"HPF KEYTRACK",0,1,1,1,"OFF|ON"});
            s.push_back({juce::String(prefix)+"track_lpf",title+"LPF KEYTRACK",0,1,1,1,"OFF|ON"});
        }
        // 1.6.12: замки PAGE/DEST каждого LFO. *_locks -- битовая маска 0..255
        // (запрещённые значения, модуляция их перескакивает), *_solo 0..8 --
        // единственное разрешённое значение (0 = выкл; модуляция бессильна,
        // руками менять можно, замок не снимается).
        for(int l=0;l<12;++l){ // 1.8.0e: 12 LFO -- замки P1 (lfo1..6) и P2 (p2lfo1..6)
            const juce::String pfx=l<6?"lfo"+juce::String(l+1)+"_":"p2lfo"+juce::String(l-5)+"_";
            const juce::String nm=l<6?"LFO"+juce::String(l+1):"P2 LFO"+juce::String(l-5);
            s.push_back({pfx+"page_locks",nm+" PAGE locks",0,2097151,0,1,{}}); // 1.8.1: маска 21 бит (PAGE 0..20)
            s.push_back({pfx+"dest_locks",nm+" DEST locks",0,255,0,1,{}});
            s.push_back({pfx+"page_solo",nm+" PAGE solo",0,21,0,1,{}}); // 1.8.1: solo 0..21
            s.push_back({pfx+"dest_solo",nm+" DEST solo",0,8,0,1,{}});
            // Persistent direct-LFO polarity controls. ALT DUAL only exposes
            // the signed half of DPTH; the parameter itself retains it safely.
            s.push_back({pfx+"mod_mode",nm+" direct modulation mode",0,1,0,1,"UNIPOLAR|BIPOLAR"});
            s.push_back({pfx+"mod_inv",nm+" direct modulation invert",0,1,0,1,"OFF|INV"});
            s.push_back({pfx+"mod_alt_dual",nm+" direct modulation ALT DUAL",0,1,0,1,"OFF|ALT DUAL"});
        }
        s.push_back({"macro_x","Macro X / MIDI CC1",0,127,64,1,{}});
        s.push_back({"macro_y","Macro Y / MIDI CC11",0,127,0,1,{}});
        // Four independent global MOD ENV sources. They have AMP-style
        // ADHR controls and route through the normal matrix to P1 or P2.
        for(int me=1;me<=4;++me){
            const juce::String tag="modenv"+juce::String(me)+"_";
            const juce::String name="MOD ENV"+juce::String(me)+" ";
            s.push_back({tag+"atk",name+"ATK",0,127,0,1,{}});
            s.push_back({tag+"hold",name+"HOLD",0,127,0,1,{}});
            s.push_back({tag+"dec",name+"DEC",0,127,127,1,{}});
            s.push_back({tag+"rel",name+"REL",0,127,127,1,{}});
        }
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
        // DTIM: по умолчанию длительность синхронизирована с темпом. При BPM=OFF
        // второй контрол задаёт длительность именно на raw DTIM=127 (0..127 --
        // это 128 шагов, поэтому DTIM=0 остаётся ненулевым первым шагом).
        s.push_back({"dly_bpm_sync","DLY BPM СИНХР.",0,1,1,1,"ВЫКЛ|ВКЛ"});
        s.push_back({"dly_max_time","DLY МАКС. ВРЕМЯ @127 (С)",0.01f,4.0f,0.75f,0.01f,{}});
        // Retained serialized ID only: old sessions can still load this dormant
        // extra selector. DSND itself owns the observed comb-route sign; no
        // separate CLASSIC/MID SAFE chooser is exposed.
        s.push_back({"dly_ppmode","DLY LEGACY PP (UNUSED)",0,1,0,1,"LEGACY A|LEGACY B"});
        juce::String targets="OFF"; // value 0 means no destination; never use a blank selectable row as a separator.
        for(int i=0;i<64;++i) {targets+="|";if(i<8)targets+="P1 SYNT "+juce::String(i+1);else if(i<56){const char* group[]{"P1 AMP","P1 FILT","P1 EFFX","P1 LFO1","P1 LFO2","P1 LFO3"};const int sec=(i-8)/8;targets+=juce::String(group[sec])+" "+pageLabel(sec,(i-8)%8);}else targets+="MSEG"+juce::String(i-55)+" OUT";}targets+="|ARP RATE|ARP GATE";
        targets+="|P1 PITCH";for(int rn=0;rn<64;++rn){targets+="|ROUTE "+juce::String(rn+1);} // PITCH (66) + ROUTE 1..64 (67..130 -- depth of any route)
        // ID 131 is a retained, real LFO-FM destination for legacy projects.
        // It stays named instead of masquerading as an empty visual separator.
        targets+="|LEGACY LFO FM";
        for(int p2t=0;p2t<32;++p2t){
            const int sec=(p2t-8)/8;targets+=juce::String("|P2 ")+(p2t==15?"AMP MIX":p2t<8?"SYNT "+juce::String(p2t+1):juce::String(sec==0?"AMP ":sec==1?"FILT ":"EFFX ")+pageLabel(sec,(p2t-8)%8));}
        for(int l2t=0;l2t<24;++l2t)targets+="|P2 LFO"+juce::String(1+l2t/8)+" "+pageLabel(9+l2t/8,l2t%8); // 1.8.0e: 164..187 = строки P2 LFO1-3
        for(int l3t=0;l3t<24;++l3t)targets+="|P1 LFO"+juce::String(4+l3t/8)+" "+pageLabel(6+l3t/8,l3t%8); // 1.8.0e: 188..211 = строки LFO4-6 (P1)
        for(int l4t=0;l4t<24;++l4t)targets+="|P2 LFO"+juce::String(4+l4t/8)+" "+pageLabel(12+l4t/8,l4t%8);
        for(int mt=0;mt<8;++mt)targets+="|MSEG"+juce::String(mt+1)+" RATE"; // target IDs 236..243
        // Window boundaries are real, automatable matrix destinations. ARP
        // start/end can move the note pattern; the P-LOCK pair moves its
        // independent step pattern when P-LOCK SYNC is OFF.
        targets+="|ARP START|ARP END|P-LOCK START|P-LOCK END"; // 244..247
        for(int row=0;row<64;++row) { // 1.7.7: 64 slots
            const auto prefix="r"+juce::String(row)+"_";
            // ON defaults to enabled: depth 0 / destination OFF is inert, and
            // a reset row is immediately ready for a Matrix target assignment.
            s.push_back({prefix+"on","Route "+juce::String(row+1)+" enable",0,1,1,1,"OFF|ON"});
            // 1.6.12: замок маршрута: FREE = обычный, LOCK = прицел/перезапись не
            // трогают dest, SOLO = приколочен к этому параметру (только руками).
            s.push_back({prefix+"lock","Route "+juce::String(row+1)+" lock",0,2,0,1,"FREE|LOCK|SOLO"});
            s.push_back({prefix+"src","Route source",0,35,0,1,"KEY|VEL|MACRO X|MACRO Y|LFO1|LFO2|LFO3|PITCH WHL|MOD WHL|AFTERTOUCH|MSEG1|MSEG2|MSEG3|STEP VEL|STEP TRANS|ARP GATE|ARP RATE|RANDOM|MSEG4|MSEG5|MSEG6|MSEG7|MSEG8|LFO4|LFO5|LFO6|P2 LFO1|P2 LFO2|P2 LFO3|P2 LFO4|P2 LFO5|P2 LFO6|MOD ENV1|MOD ENV2|MOD ENV3|MOD ENV4"}); // 36 appended-stable sources: existing IDs 0..31 plus MOD ENV1..4 = 32..35
            s.push_back({prefix+"dest","Route target",0,248,0,1,targets}); // raw 0 = OFF; raw 1..248 = stable target IDs 0..247
            s.push_back({prefix+"depth","Route depth",-64,63,0,1,{}});
            s.push_back({prefix+"mode","Route polarity",0,1,0,1,"UNIPOLAR|BIPOLAR"});
            s.push_back({prefix+"inv","Route invert",0,1,0,1,"OFF|INV"});
        }
        // Schema-39 anchor IDs remain in the layout so saved projects keep
        // their complete state. Schema 40 added retained GUARD IDs; schema 44
        // adds active BASE BEND/HOLD @64 to the schema-43 raw window. No old
        // host parameter is removed or repurposed.
        for(const char* prefix:{"dly_","p2_dly_"}){
            const juce::String title=juce::String(prefix).startsWith("p2_")?"P2 DLY DFB ":"DLY DFB ";
            // Retained serialized schema-39 IDs stay compatibility-only. New
            // BASE CURVE/HOLD controls below are explicit live controls, not
            // a revival of the old raw-anchor/TUNE parameter set.
            s.push_back({juce::String(prefix)+"dfb_low_div",title+"LOW DIV",8.0f,256.0f,63.0f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_soft_raw",title+"SOFT RAW",1.0f,125.0f,63.0f,1.0f,{}});
            s.push_back({juce::String(prefix)+"dfb_soft_level",title+"SOFT FB",0.0f,2.0f,62.0f/63.0f,0.0001f,{}});
            s.push_back({juce::String(prefix)+"dfb_unity_raw",title+"UNITY RAW",2.0f,126.0f,64.0f,1.0f,{}});
            s.push_back({juce::String(prefix)+"dfb_hot_raw",title+"HOT RAW",3.0f,127.0f,65.0f,1.0f,{}});
            s.push_back({juce::String(prefix)+"dfb_hot_level",title+"HOT FB",0.0f,2.0f,64.0f/63.0f,0.0001f,{}});
            s.push_back({juce::String(prefix)+"dfb_ceiling",title+"CEILING",0.0f,2.0f,1.0625f,0.0001f,{}});
            // Schema 44 BASE BEND lifts RAW 60..62 smoothly toward the fixed
            // RAW-63 reference; HOLD @64 is a tiny independently editable
            // trim. Defaults below intentionally reproduce the user's posted
            // DFB panel setting for newly created projects.
            s.push_back({juce::String(prefix)+"dfb_base_curve",title+"BASE CURVE",0.10f,2.00f,0.55f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_base_hold_64",title+"BASE HOLD @64",63.0f/64.0f,1.0f,0.992f,0.0001f,{}});
            s.push_back({juce::String(prefix)+"dfb_plateau",title+"PLATEAU",0.05f,4.0f,0.05f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_attack_ms",title+"ATTACK MS",1.0f,1000.0f,1.0f,1.0f,{}});
            s.push_back({juce::String(prefix)+"dfb_release_ms",title+"RELEASE MS",1.0f,5000.0f,1.0f,1.0f,{}});
            // Schema-40 RAW guard ID remains serialized compatibility data.
            // Schema 44 keeps the active raw-gated 64..127 window, adds BASE
            // BEND/HOLD @64, and leaves the legacy RAW threshold hidden.
            s.push_back({juce::String(prefix)+"dfb_guard_raw",title+"GUARD RAW (LEGACY)",65.0f,127.0f,65.0f,1.0f,{}});
            s.push_back({juce::String(prefix)+"dfb_guard_level_start",title+"GUARD LVL START @64",0.0f,4.0f,0.0f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_guard_level_start_127",title+"GUARD LVL START @127",0.0f,4.0f,0.0f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_guard_plateau_127",title+"GUARD PLATEAU @127",0.05f,4.0f,3.46f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_guard_offset",title+"GUARD LVL OFFSET",-2.0f,2.0f,0.18f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_guard_curve",title+"GUARD CURVE",0.10f,4.0f,1.08f,0.01f,{}});
            s.push_back({juce::String(prefix)+"dfb_guard_amount",title+"GUARD AMOUNT",0.0f,2.0f,2.0f,0.01f,{}});
        }
        return s;
    }();
    return all;
}
}
