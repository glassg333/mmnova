#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout MonomachineNovaAudioProcessor::createLayout(){
    juce::AudioProcessorValueTreeState::ParameterLayout result;
    for(const auto& s:nova::specs()){
        if(s.choices.isNotEmpty())result.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(s.id,1),s.name,juce::StringArray::fromTokens(s.choices,"|",""),static_cast<int>(s.def)));
        else if(s.id.endsWith("_depth"))result.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID(s.id,1),s.name,juce::NormalisableRange<float>(s.lo,s.hi,s.step),s.def,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction([](float v,int){return juce::String(v < 0.0f ? "-" : "+") + juce::String(std::round(std::abs(v) * 100.0f / (v < 0.0f ? 64.0f : 63.0f)), 0) + "%";})
                .withValueFromStringFunction([](const juce::String& text){const float percent=text.retainCharacters("-0123456789").getFloatValue();return juce::jlimit(-64.0f,63.0f,percent * (percent < 0.0f ? 64.0f : 63.0f) / 100.0f);}))) ;
        else if(s.id=="p0_4"||s.id=="p2_4"||s.id=="m2_3")result.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID(s.id,1),s.name,juce::NormalisableRange<float>(s.lo,s.hi,s.step),s.def,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction([](float v,int){return juce::String(juce::roundToInt(v)-64);})
                .withValueFromStringFunction([](const juce::String& text){return juce::jlimit(0.0f,127.0f,text.getFloatValue()+64.0f);}))) ;
        else result.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(s.id,1),s.name,juce::NormalisableRange<float>(s.lo,s.hi,s.step),s.def));
    }
    return result;
}
MonomachineNovaAudioProcessor::MonomachineNovaAudioProcessor():AudioProcessor(isSynthVersion?
    BusesProperties().withOutput("Output",juce::AudioChannelSet::stereo(),true):
    BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),
    parameters(*this,nullptr,isSynthVersion?"NovaUnifiedSynth":"NovaUnifiedFX",createLayout()){
    machineRaw=parameters.getRawParameterValue("machine");
    for(int s=0;s<kPlockSlots;++s){plockTargetI[static_cast<size_t>(s)].store(-1);plockPolI[static_cast<size_t>(s)].store(0);} // 1.7.1: P-LOCK
    for(size_t i=0;i<plockVal.size();++i){plockVal[i].store(0.0f);plockSlideV[i].store(0.0f);}
    const char* ampIds[]{"amp_mode","amp_curve_a","amp_curve_d","amp_curve_r"};for(size_t i=0;i<4;++i)ampRaw[i]=parameters.getRawParameterValue(ampIds[i]);
    portaSpeedRaw=parameters.getRawParameterValue("porta_speed"); // 1.6.14
    repitchRaw=parameters.getRawParameterValue("dly_repitch");
    repitchSmoothRaw=parameters.getRawParameterValue("dly_repitch_smooth");
    ppModeRaw=parameters.getRawParameterValue("dly_ppmode");
    {const char* p1Q[]{"dly_dbas_q","dly_dwid_q"};const char* p2Q[]{"p2_dly_dbas_q","p2_dly_dwid_q"};
     for(size_t i=0;i<2;++i){delayFeedbackQP1Raw[i]=parameters.getRawParameterValue(p1Q[i]);delayFeedbackQP2Raw[i]=parameters.getRawParameterValue(p2Q[i]);}}
    // 1.8.0: P2 -- полноценный FX-движок (p2_ прописка)
    p2MachineRaw=parameters.getRawParameterValue("p2_machine");
    for(int p2m=0;p2m<int(p2FxMachines().size());++p2m)for(int i=0;i<8;++i)
        if(p2FxMachines()[static_cast<size_t>(p2m)].synthParams[static_cast<size_t>(i)].maxVal>0)
            p2HandRaw[static_cast<size_t>(p2m)][static_cast<size_t>(i)]=parameters.getRawParameterValue("p2m"+juce::String(p2FxMachines()[static_cast<size_t>(p2m)].id)+"_"+juce::String(i));
    for(int p=0;p<2;++p)for(int i=0;i<8;++i)p2PageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]=parameters.getRawParameterValue("p2_"+juce::String(p+1)+"_"+juce::String(i));
    {static const char* sfx[3]={"hold","transpose","velocity"}; // 1.8.1b: кэш arp-степов -- snapshot без аллокаций
    for(int pg=0;pg<16;++pg)for(int st=0;st<8;++st)for(int k=0;k<3;++k)stepRaw[static_cast<size_t>(pg)][static_cast<size_t>(st)][static_cast<size_t>(k)]=parameters.getRawParameterValue("arp_s"+juce::String(pg)+"_"+juce::String(st)+"_"+sfx[k]);}
    {const char* ids[]{"p2_amp_atk","p2_amp_hold","p2_amp_dec","p2_amp_rel","p2_amp_mode","p2_amp_curve_a","p2_amp_curve_d","p2_amp_curve_r"};
     for(size_t i=0;i<8;++i)p2AmpRaw[i]=parameters.getRawParameterValue(ids[i]);}
    p2MixRaw=parameters.getRawParameterValue("p2_mix");
    {const char* ids[]{"p2_0_4","p2_0_5","p2_0_6"};for(size_t i=0;i<3;++i)p2GainRaw[i]=parameters.getRawParameterValue(ids[i]);} // 1.8.0: P2 DIST/VOL/PAN (PORT/LEV/MIX-ручки убраны -- P2 = вставка в цепь)
    {const char* mids[]{"p2_mode_filt","p2_mode_dist","p2_mode_dly"};for(size_t i=0;i<3;++i)p2ModeRaw[i]=parameters.getRawParameterValue(mids[i]);}
    {const char* p1ids[]{"hybrid_p1_mode_l","hybrid_p1_mode_h","hybrid_p1_mode_s"};const char* p2ids[]{"hybrid_p2_mode_l","hybrid_p2_mode_h","hybrid_p2_mode_s"};
     for(size_t i=0;i<3;++i){hybridP1Raw[i]=parameters.getRawParameterValue(p1ids[i]);hybridP2Raw[i]=parameters.getRawParameterValue(p2ids[i]);}}
    {const char* ids[]{"vel_l","vel_h","kt_l","kt_h","sat","env_atk","env_hold","env_dec","env_rel","env_mix","env_base_depth","env_width_depth"};
     for(size_t i=0;i<12;++i){filterExtrasP1Raw[i]=parameters.getRawParameterValue("filt_"+juce::String(ids[i]));filterExtrasP2Raw[i]=parameters.getRawParameterValue("p2_filt_"+juce::String(ids[i]));}}
    {const char* ids[]{"track_hpf","track_lpf"};
     for(size_t i=0;i<2;++i){filterTrackingP1Raw[i]=parameters.getRawParameterValue("filt_"+juce::String(ids[i]));filterTrackingP2Raw[i]=parameters.getRawParameterValue("p2_filt_"+juce::String(ids[i]));}}
    {const char* fields[]{"atk","hold","dec","rel"};for(int me=0;me<4;++me)for(int f=0;f<4;++f)modEnvRaw[static_cast<size_t>(me)][static_cast<size_t>(f)]=parameters.getRawParameterValue("modenv"+juce::String(me+1)+"_"+fields[f]);}
    for(int l=0;l<12;++l){const juce::String pfx=l<6?"lfo"+juce::String(l+1)+"_":"p2lfo"+juce::String(l-5)+"_";const char* suf[]{"page_locks","dest_locks","page_solo","dest_solo"};for(int k=0;k<4;++k)lfoLockRaw[static_cast<size_t>(l*4+k)]=parameters.getRawParameterValue(pfx+suf[k]);} // 1.8.0e: 12 LFO
    // 1.6.8: mode_synt is per machine now (mode_synt_m<id>), the shared parameter is gone.
    for(int i=0;i<monomachine::DspSectionCount;++i)modeRaw[static_cast<size_t>(i)]=(i==monomachine::DspSynt)?nullptr:parameters.getRawParameterValue(monomachine::dspModeParamId(i));
    for(size_t m=0;m<nova::machines().size();++m)syntModeRaw[m]=parameters.getRawParameterValue(syntModeParamIdFor(static_cast<int>(m)));
    for(size_t m=0;m<nova::machines().size();++m)for(int i=0;i<8;++i)
        if(nova::machines()[m].synthParams[static_cast<size_t>(i)].maxVal>0)synthRaw[m][static_cast<size_t>(i)]=parameters.getRawParameterValue(nova::machineParam(nova::machines()[m].id,i));
    for(int p=0;p<15;++p)for(int i=0;i<8;++i)pageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]=parameters.getRawParameterValue(nova::pageParam(p,i)); // 1.8.0e: +страницы 9..14-6
    arpStepStartRaw=parameters.getRawParameterValue("arp_step_start");
    arpStepEndRaw=parameters.getRawParameterValue("arp_step_end");
    plockSyncRaw=parameters.getRawParameterValue("plock_sync");
    plockStepStartRaw=parameters.getRawParameterValue("plock_step_start");
    plockStepEndRaw=parameters.getRawParameterValue("plock_step_end");
    const char* g[]={"level","bpm","host_sync","gate","arp_mode","arp_play","arp_speed","arp_range","arp_length","macro_x","macro_y","arp_on","arp_hold","arp_sync","arp_time","arp_grid","arp_wrap","arp_velocity_mode","arp_step","arp_step_velocity","arp_step_transpose","arp_step_hold","arp_step_page","arp_step_page_limit","arp_step_random","arp_step_rnd","mseg_rate","mseg_sync","mseg_loop","mseg2_rate","mseg2_sync","mseg2_loop","mseg3_rate","mseg3_sync","mseg3_loop","mseg4_rate","mseg4_sync","mseg4_loop","mseg5_rate","mseg5_sync","mseg5_loop","mseg6_rate","mseg6_sync","mseg6_loop","mseg7_rate","mseg7_sync","mseg7_loop","mseg8_rate","mseg8_sync","mseg8_loop","mseg_retrig","mseg_rmode","mseg_key","mseg_lpoint","mseg2_retrig","mseg2_rmode","mseg2_key","mseg2_lpoint","mseg3_retrig","mseg3_rmode","mseg3_key","mseg3_lpoint","mseg4_retrig","mseg4_rmode","mseg4_key","mseg4_lpoint","mseg5_retrig","mseg5_rmode","mseg5_key","mseg5_lpoint","mseg6_retrig","mseg6_rmode","mseg6_key","mseg6_lpoint","mseg7_retrig","mseg7_rmode","mseg7_key","mseg7_lpoint","mseg8_retrig","mseg8_rmode","mseg8_key","mseg8_lpoint"};
    for(size_t i=0;i<GlobalCount;++i)globalRaw[i]=(isSynthVersion&&i==Gate)?nullptr:parameters.getRawParameterValue(g[i]);
    const char* fields[]={"on","src","dest","depth","mode"};
    for(int r=0;r<64;++r)for(int i=0;i<5;++i)routeRaw[static_cast<size_t>(r)][static_cast<size_t>(i)]=parameters.getRawParameterValue("r"+juce::String(r)+"_"+fields[i]); // 1.7.7: 64 слота
    for(int r=0;r<64;++r)auxRaw[static_cast<size_t>(r)]=parameters.getRawParameterValue("r"+juce::String(r)+"_aux"); // 1.6.24/1.7.7: AUX SOURCE (64)
    for(int r=0;r<64;++r)auxDepthRaw[static_cast<size_t>(r)]=parameters.getRawParameterValue("r"+juce::String(r)+"_aux_depth"); // 1.6.32: AUX DEPTH // 1.7.7: 64
    for(int i=0;i<24;++i)sampleNames[static_cast<size_t>(i)]=machine.bbox.getSlots()[static_cast<size_t>(i)].name;
    wheels.fill(8192);
}
MonomachineNovaAudioProcessor::~MonomachineNovaAudioProcessor(){cancelPendingUpdate();}
bool MonomachineNovaAudioProcessor::isBusesLayoutSupported(const BusesLayout& l)const{
    const auto out=l.getMainOutputChannelSet();if(out!=juce::AudioChannelSet::mono()&&out!=juce::AudioChannelSet::stereo())return false;
    return isSynthVersion?l.getMainInputChannelSet().isDisabled():l.getMainInputChannelSet()==out;
}
void MonomachineNovaAudioProcessor::prepareToPlay(double rate,int block){
    juce::ignoreUnused(block);sr=rate>0?rate:44100;
    const juce::ScopedLock lock(sampleLock);
    machine.prepare(sr);machine2.prepare(sr);fxSlotsPrepare();classicRouteAudioSerial=~uint64_t{0};feedbackNetwork.prepare(sr);feedbackAudioSerial=~uint64_t{0}; // 1.8.4: slot cores + classic route + FB delay state
    chain.volumeReference=isSynthVersion?127.0f:64.0f;
    // P2 is an insert, so its centred VOL (64) is unity in both targets.
    // Copying the Synth P1 reference (127) made P2 MIX=127 lose ~6 dB.
    chain2.volumeReference=64.0f;chain2.setNeutralFilterThru(true);
    chain.prepare(sr);chain2.prepare(sr);midiGateBlend.reset(sr,0.005);envelope.reset(sr);envelope2.reset(sr);for(auto& env:modEnvelopes)env.reset(sr);modEnvValue.fill(0.0f);arp.reset(sr);matrix.reset();for(auto& m:mseg)m.reset();msegPhase.fill(0.0);msegValue.fill(0.0f);for(auto& p:msegPhaseUi)p.store(0.0);liveStepVelocity=0.0f;liveStepTranspose=0.0f;liveArpGate=0.0f;selectedStepVelocity=1.0f;selectedStepTranspose=0.0f;selectedStepHold=0.0f;
    for(auto& l:lfos)l.reset();for(auto& l:lfos2)l.reset();notes.fill({});pedal.fill(false);wheels.fill(8192);arpHeld.fill(false);
    for(int j=0;j<12;++j){lfoLastPage[static_cast<size_t>(j)]=juce::jlimit(0,nova::kLfoPageChoiceCount-1,juce::roundToInt(pageRaw[static_cast<size_t>(3+j)][0]->load()));lfoLastDest[static_cast<size_t>(j)]=juce::jlimit(0,nova::kLfoDestinationCount-1,juce::roundToInt(pageRaw[static_cast<size_t>(3+j)][1]->load()));} // all 21 PAGE values are sticky-safe
    arpPageRndWasEnabled.store(parameters.getRawParameterValue("arp_step_random")&&parameters.getRawParameterValue("arp_step_random")->load()>0.5f,std::memory_order_relaxed);
    currentKey=-1;serial=0;activeMachine=-1;lastArpMode=-1;ccX=ccY=-1;plockStepCursor=0;
    dryDelay.clear();dryDelay2.clear();previousLfo.fill(0);smoothedParams2.fill(0.0f);p2SmoothingPrimed=false;p2WetMix.reset(sr,0.012);p2WetMix.setCurrentAndTargetValue(1.0f);p1ChorusSafeActive=false;p2ChorusSafeActive=false;
    level.reset(sr,0.02);mix.reset(sr,0.02);velocity.reset(sr,0.005);velocity.setCurrentAndTargetValue(1);
    pitch.reset(sr,0);pitch.setCurrentAndTargetValue(60);prepared=true;snapshot();
    midiGateBlend.setCurrentAndTargetValue(global[Gate]>0.5f?1.0f:0.0f);
    level.setCurrentAndTargetValue(global[Level]/127.0f);mix.setCurrentAndTargetValue(1.0f); // 1.8.0: fx_mix убран -- FX = вставка, полный wet (сухое = машина THRU; глобальный dry/wet есть в DAW)
    panicRequested.store(false);peak.store(0);
}
const std::vector<monomachine::MachineDef>& MonomachineNovaAudioProcessor::p2FxMachines(){ // 1.8.0: FX-слоты для страницы P2 (в обеих версиях плагина)
    static const auto list=[]{
        auto all=monomachine::getAllMachineDefinitions();
        std::vector<monomachine::MachineDef> fx;
        for(const auto& m:all) if(m.isEffect) fx.push_back(m);
        return fx;
    }();
    return list;
}
void MonomachineNovaAudioProcessor::snapshot(){
    // 1.6.8: SYNT-режим берётся из параметра ТЕКУЩЕЙ машины (mode_synt_m<id>),
    // поэтому при листании машин у каждой остаётся свой mode.  NEW and the
    // appended FIX candidates remain opt-in only for FM+ STAT/PAR/DYN.
    const int selectedMachine=machineIndex();
    const int selectedMachineId=nova::machines()[static_cast<size_t>(selectedMachine)].id;
    for(int i=0;i<monomachine::DspSectionCount;++i){
        auto* ptr=(i==monomachine::DspSynt)?syntModeRaw[static_cast<size_t>(selectedMachine)]:modeRaw[static_cast<size_t>(i)];
        int idx = ptr ? juce::roundToInt(ptr->load()) : monomachine::dspModeMnm;
        idx = juce::jlimit(0, monomachine::dspModeCount-1, idx);
        // SYNT's selector is stored per machine.  NEW is legal only for the
        // explicit FM+ m8/m9/m10 allow-list; every other section retains its
        // own stable list and all malformed/out-of-era values fall to MNM.
        const bool allowed=(i==monomachine::DspSynt)
            ?monomachine::dspSyntModeAllowedForMachine(selectedMachineId,idx)
            :monomachine::dspModeAllowedForSection(i,idx);
        if(!allowed) idx=monomachine::dspModeMnm;
        dspModes[static_cast<size_t>(i)] = idx;
    }
    (void)monomachine::dspModeFilter;
    // 1.6.19: выбор огибающей вернулся (amp_mode: old = legacy 1.4, mnm = kernel),
    // БЕЗ привязки к режиму DSP-секции AMP.
    {const int am2=juce::jlimit(0,2,p2AmpRaw[4]?juce::roundToInt(p2AmpRaw[4]->load()):1);
     envelope2.configure(am2==1?nova::AmpEnvelope::kKernelAlgorithm:(am2==2?3:0),{p2AmpRaw[5]->load(),p2AmpRaw[6]->load(),p2AmpRaw[7]->load()});}
    {const int am=juce::jlimit(0,2,juce::roundToInt(ampRaw[0]->load()));envelope.configure(am==1?nova::AmpEnvelope::kKernelAlgorithm:(am==2?3:0),{ampRaw[1]->load(),ampRaw[2]->load(),ampRaw[3]->load()});} // old|mnm|vital
    machine.setModes(dspModes[monomachine::DspSynt],dspModes[monomachine::DspDist]);
    chain.setModes(dspModes[monomachine::DspFilter],dspModes[monomachine::DspDist],dspModes[monomachine::DspDelay],dspModes[monomachine::DspRouting]);
    chain.setHybridTestModes(hybridP1Raw[0]?juce::roundToInt(hybridP1Raw[0]->load()):0,hybridP1Raw[1]?juce::roundToInt(hybridP1Raw[1]->load()):0,hybridP1Raw[2]?juce::roundToInt(hybridP1Raw[2]->load()):0);
    auto setFilterExtras=[&](const std::array<std::atomic<float>*,12>& raw,nova::TrackChain& destination){
        const auto value=[&raw](size_t i,float fallback){return raw[i]?raw[i]->load(std::memory_order_relaxed):fallback;};
        destination.setFilterExtras(value(0,0),value(1,0),value(2,0),value(3,0),value(4,0),
                                    value(5,0),value(6,0),value(7,127),value(8,127),
                                    value(9,0),value(10,0),value(11,0));
    };
    setFilterExtras(filterExtrasP1Raw,chain);setFilterExtras(filterExtrasP2Raw,chain2);
    // Old projects did not serialize these new switches: preserve documented
    // normal tracking by treating an absent value as ON.
    const auto trackingOn=[](const std::atomic<float>* value){return value==nullptr||value->load(std::memory_order_relaxed)>0.5f;};
    chain.setFilterKeyTracking(trackingOn(filterTrackingP1Raw[0]),trackingOn(filterTrackingP1Raw[1]));
    chain2.setFilterKeyTracking(trackingOn(filterTrackingP2Raw[0]),trackingOn(filterTrackingP2Raw[1]));
    const auto delayQValue=[](const std::atomic<float>* value){return value?value->load(std::memory_order_relaxed):0.0f;};
    chain.setDelayFeedbackQ(delayQValue(delayFeedbackQP1Raw[0]),delayQValue(delayFeedbackQP1Raw[1]));
    chain2.setDelayFeedbackQ(delayQValue(delayFeedbackQP2Raw[0]),delayQValue(delayFeedbackQP2Raw[1]));
    for(int me=0;me<4;++me){
        auto& env=modEnvelopes[static_cast<size_t>(me)];
        env.p={modEnvRaw[static_cast<size_t>(me)][0]?modEnvRaw[static_cast<size_t>(me)][0]->load():0.0f,
               modEnvRaw[static_cast<size_t>(me)][1]?modEnvRaw[static_cast<size_t>(me)][1]->load():0.0f,
               modEnvRaw[static_cast<size_t>(me)][2]?modEnvRaw[static_cast<size_t>(me)][2]->load():127.0f,
               modEnvRaw[static_cast<size_t>(me)][3]?modEnvRaw[static_cast<size_t>(me)][3]->load():127.0f};
        env.configure(nova::AmpEnvelope::kKernelAlgorithm,{0.0f,0.0f,0.0f});
    }
    chain.setRepitch(repitchRaw?repitchRaw->load():2.0f,repitchSmoothRaw?repitchSmoothRaw->load():0.0f);
    chain.setPpMode(ppModeRaw?juce::roundToInt(ppModeRaw->load()):0);
    for(int j=0;j<12;++j){ // P1 LFO1..6 and P2 LFO1..6 must honour the same lock/solo state.
        lfoPageLocks[static_cast<size_t>(j)]=lfoLockRaw[static_cast<size_t>(j*4)]?juce::roundToInt(lfoLockRaw[static_cast<size_t>(j*4)]->load()):0;
        lfoDestLocks[static_cast<size_t>(j)]=lfoLockRaw[static_cast<size_t>(j*4+1)]?juce::roundToInt(lfoLockRaw[static_cast<size_t>(j*4+1)]->load()):0;
        lfoPageSolo[static_cast<size_t>(j)]=lfoLockRaw[static_cast<size_t>(j*4+2)]?juce::roundToInt(lfoLockRaw[static_cast<size_t>(j*4+2)]->load()):0;
        lfoDestSolo[static_cast<size_t>(j)]=lfoLockRaw[static_cast<size_t>(j*4+3)]?juce::roundToInt(lfoLockRaw[static_cast<size_t>(j*4+3)]->load()):0;
    }
    for(size_t i=0;i<GlobalCount;++i)global[i]=globalRaw[i]?globalRaw[i]->load(std::memory_order_relaxed):0;
    int selected=juce::jlimit(0,static_cast<int>(nova::machines().size())-1,juce::roundToInt(machineRaw->load()));
    for(int i=0;i<8;++i){auto* ptr=synthRaw[static_cast<size_t>(selected)][static_cast<size_t>(i)];base[static_cast<size_t>(i)]=ptr?ptr->load():0;}
    for(int p=0;p<3;++p)for(int i=0;i<8;++i)base[static_cast<size_t>(8+p*8+i)]=pageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]->load();
    for(size_t p=0;p<12;++p)for(size_t i=0;i<8;++i){ // 1.8.0e: 12 LFO (страницы 3..14)
        const float value=pageRaw[p+3][i]->load();const float maximum=i==0?20.0f:i==1?7.0f:i==2?3.0f:i==3?10.0f:i==4?6.0f:127.0f;effectiveLfoParams[p][i]=std::clamp(effectiveLfoParams[p][i]+value-lfoParams[p][i],0.0f,maximum);lfoParams[p][i]=value;} // 1.8.1 FIX: PAGE -- до 20 (чужая папка 11..20), было 7
    bpm=global[Bpm];if(global[HostSync]>0.5f)if(auto* play=getPlayHead())if(auto position=play->getPosition())if(auto tempo=position->getBpm())if(std::isfinite(*tempo))bpm=std::clamp(*tempo,30.0,300.0);
    envelope.setTempo(static_cast<float>(bpm));envelope2.setTempo(static_cast<float>(bpm));
    chain.setHostTempo(static_cast<float>(bpm));chain2.setHostTempo(static_cast<float>(bpm));
    tempoDisplay.store(bpm);
    if(global[MacroX]!=previousX){ccX=-1;previousX=global[MacroX];}if(global[MacroY]!=previousY){ccY=-1;previousY=global[MacroY];}
    matrix.setParamX(static_cast<uint8_t>(ccX>=0?ccX:global[MacroX]));matrix.setParamY(static_cast<uint8_t>(ccY>=0?ccY:global[MacroY]));
    for(size_t r=0;r<64;++r){const int dvv=juce::jlimit(0,248,juce::roundToInt(routeRaw[r][2]->load())); // raw 0 = OFF; raw 1..248 = stable targets 0..247
        matrix.configureRouting(r,routeRaw[r][0]->load()>0.5f&&dvv>0,static_cast<monomachine::ModSource>(juce::jlimit(0,35,juce::roundToInt(routeRaw[r][1]->load()))),static_cast<uint8_t>(dvv>0?dvv-1:0),static_cast<int8_t>(juce::jlimit(-64,63,juce::roundToInt(routeRaw[r][3]->load()))),(juce::roundToInt(routeRaw[r][4]->load())==1?monomachine::ModPolarity::Bipolar:monomachine::ModPolarity::Unipolar),static_cast<uint8_t>(auxRaw[static_cast<size_t>(r)]?juce::jlimit(0,36,juce::roundToInt(auxRaw[static_cast<size_t>(r)]->load())):0),static_cast<int8_t>(juce::jlimit(-64,63,auxDepthRaw[static_cast<size_t>(r)]?juce::roundToInt(auxDepthRaw[static_cast<size_t>(r)]->load()):0)));} // source IDs are 0..35 (MOD ENV1..4 appended); AUX uses source+one, 0..36.
    matrix.setPitchWheel((wheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]-8192.0f)/8192.0f);
    matrix.setModWheel(modWheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
    matrix.setAftertouch(aftertouch[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
    matrix.setMsegOutputs(msegValue[0],msegValue[1],msegValue[2]); // 1.6.21
    matrix.setModEnvOutputs(modEnvValue.data(),4);
    matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:global[ArpStepVelocity]/127.0f,liveStepTranspose!=0.0f?liveStepTranspose:global[ArpStepTranspose]/24.0f,liveArpGate>0.0f?liveArpGate:global[ArpStepHold]);
    matrix.setArpRate(global[ArpGrid]/15.0f);
    midiGateBlend.setTargetValue(global[Gate]>0.5f?1.0f:0.0f);
    level.setTargetValue(global[Level]/127.0f);mix.setTargetValue(1.0f);
    global[ArpMode]=global[ArpOn]>0.5f?(global[ArpHold]>0.5f?2.0f:1.0f):0.0f;
    auto mode=static_cast<int>(global[ArpMode]);
    monomachine::MonomachineArpeggiator::ArpSettings settings;
    settings.mode=static_cast<monomachine::MonomachineArpeggiator::Mode>(mode);settings.play=[&]{static const monomachine::MonomachineArpeggiator::Play playMap[]={monomachine::MonomachineArpeggiator::Play::Step,monomachine::MonomachineArpeggiator::Play::StepChord,monomachine::MonomachineArpeggiator::Play::True,monomachine::MonomachineArpeggiator::Play::Up,monomachine::MonomachineArpeggiator::Play::Down,monomachine::MonomachineArpeggiator::Play::Cycl,monomachine::MonomachineArpeggiator::Play::Rnd};return playMap[juce::jlimit(0,6,juce::roundToInt(global[ArpPlay]))];}(); // 1.6.22: порядок STEP|STEP CHORD|TRUE|UP|DOWN|CYCL|RND
    const double ticks[]{96,72,64,48,36,32,24,18,16,12,9,8,6,4.5,4,3,2.25,2,1.5};settings.speed=ticks[juce::jlimit(0,18,static_cast<int>(global[ArpGrid]))]; // 1.7.0: 19 рейтов
    settings.sync=global[ArpSync]>0.5f;settings.milliseconds=global[ArpTime];settings.range=static_cast<uint8_t>(global[ArpRange]);settings.noteLength=static_cast<uint8_t>(global[ArpLength]);settings.wrap=static_cast<uint8_t>(juce::jlimit(1,16,juce::roundToInt(global[ArpWrap])));settings.velocityMode=[&]{static const monomachine::MonomachineArpeggiator::VelocityMode velMap[]={monomachine::MonomachineArpeggiator::VelocityMode::Step,monomachine::MonomachineArpeggiator::VelocityMode::StepKey,monomachine::MonomachineArpeggiator::VelocityMode::StepHold,monomachine::MonomachineArpeggiator::VelocityMode::Key,monomachine::MonomachineArpeggiator::VelocityMode::Hold};return velMap[juce::jlimit(0,4,juce::roundToInt(global[ArpVelocityMode]))];}(); // 1.6.24: порядок STEP|STEP+KEY|STEP+HOLD|KEY|HOLD
    settings.stepPage=static_cast<uint8_t>(juce::jlimit(0,15,juce::roundToInt(global[ArpStepPage]))); // retained automation/state mirror
    settings.stepPageLimit=static_cast<uint8_t>(juce::jlimit(1,16,juce::roundToInt(global[ArpStepPageLimit])));
    const int windowStart=juce::jlimit(0,63,juce::roundToInt(arpStepStartRaw?arpStepStartRaw->load()-1.0f:0.0f));
    const int requestedEnd=juce::jlimit(0,63,juce::roundToInt(arpStepEndRaw?arpStepEndRaw->load()-1.0f:15.0f));
    const int windowEnd=juce::jlimit(windowStart,63,requestedEnd);
    arpStepWindowStartBase=windowStart;arpStepWindowEndBase=windowEnd;
    // Initialise the independent P-LOCK window from its own automatable
    // parameters. Its final runtime bounds are refreshed after Matrix outputs.
    const int plockStart=juce::jlimit(0,63,juce::roundToInt(plockStepStartRaw?plockStepStartRaw->load()-1.0f:0.0f));
    const int plockRequestedEnd=juce::jlimit(0,63,juce::roundToInt(plockStepEndRaw?plockStepEndRaw->load()-1.0f:15.0f));
    const int plockEnd=juce::jlimit(plockStart,63,plockRequestedEnd);
    const bool previousPlockSync=plockSyncRuntime;
    plockSyncRuntime=plockSyncRaw==nullptr||plockSyncRaw->load()>0.5f;
    if(plockSyncRuntime){plockStepWindowStartRuntime=windowStart;plockStepWindowEndRuntime=windowEnd;if(previousPlockSync!=plockSyncRuntime)plockStepCursor=windowStart;}
    else {plockStepWindowStartRuntime=plockStart;plockStepWindowEndRuntime=plockEnd;if(previousPlockSync!=plockSyncRuntime||plockStepCursor<plockStart||plockStepCursor>plockEnd)plockStepCursor=plockStart;}
    // PAGE RND is committed once when it becomes enabled.  The audio path
    // only queues the action; APVTS/host writes are made on the message thread.
    const bool pageRandomOn=global[ArpStepRandom]>0.5f;
    if(pageRandomOn){if(!arpPageRndWasEnabled.load(std::memory_order_relaxed))queueArpPageRandomWrite(windowStart,windowEnd);arpPageRndWasEnabled.store(true,std::memory_order_relaxed);}
    else if(!arpPageRndWritePending.load(std::memory_order_acquire))arpPageRndWasEnabled.store(false,std::memory_order_relaxed);
    settings.stepWindowStart=static_cast<uint8_t>(windowStart);settings.stepWindowEnd=static_cast<uint8_t>(windowEnd);
    arpStepWindowStartRuntime=windowStart;arpStepWindowEndRuntime=windowEnd;
    settings.stepRandom=0.0f; // consumed above as a one-shot write action, never as a transient DSP source.
    settings.stepRnd=global[ArpStepRnd]>0.5f;settings.autoSwap=arpAutoSwap.load();
    for(int absolute=0;absolute<64;++absolute){const int pg=absolute/8,st=absolute%8;auto& stepValue=settings.steps[static_cast<size_t>(absolute)];
        if(auto* h=stepRaw[static_cast<size_t>(pg)][static_cast<size_t>(st)][0])stepValue.hold=h->load()>0.5f;
        if(auto* t=stepRaw[static_cast<size_t>(pg)][static_cast<size_t>(st)][1])stepValue.transpose=static_cast<int8_t>(juce::jlimit(-24,24,juce::roundToInt(t->load())));
        if(auto* v=stepRaw[static_cast<size_t>(pg)][static_cast<size_t>(st)][2])stepValue.velocity=static_cast<uint8_t>(juce::jlimit(0,127,juce::roundToInt(v->load())));}
    const auto& selectedStep=settings.steps[static_cast<size_t>(windowStart)];selectedStepVelocity=selectedStep.velocity/127.0f;selectedStepTranspose=selectedStep.transpose/24.0f;selectedStepHold=selectedStep.hold?1.0f:0.0f;matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:selectedStepVelocity,liveStepTranspose!=0.0f?liveStepTranspose:selectedStepTranspose,liveArpGate>0.0f?liveArpGate:selectedStepHold);
    if(mode!=lastArpMode){arp.reset(sr,bpm);arpHeld.fill(false);release();plockClearRuntime();plockStepCursor=plockStepWindowStartRuntime;plockStepEcho.store(plockStepCursor,std::memory_order_relaxed);} // reset independent P-LOCK transport too
    arp.setSettings(settings);arp.setTempo(bpm);
    if(mode!=lastArpMode){lastArpMode=mode;syncArpNotes();if(mode==0&&currentKey>=0)trigger(currentKey%128,notes[static_cast<size_t>(currentKey)].velocity,currentKey/128);}
    if(selected!=activeMachine){activeMachine=selected;effectiveLfoParams=lfoParams;smoothedParams=base;dryDelay.clear();machine.clear();chain.clear();std::array<float,8> p;std::copy_n(base.data(),8,p.data());machine.set(nova::machines()[static_cast<size_t>(selected)].id,p);
        if(currentKey>=0&&mode==0)trigger(currentKey%128,notes[static_cast<size_t>(currentKey)].velocity,currentKey/128);}
    // Both serial native CHORUS positions delay their dry and wet paths by the
    // adapter latency.  Reporting only P1 left P2 16 samples late against a
    // DAW's parallel path, which sounds like a phase shift/uneven stereo gain.
    // Use the selected IDs rather than the last rendered engine IDs so PDC is
    // correct on the very first block after a machine change.
    const int p1Latency=machine.latencyForMachine(nova::machines()[static_cast<size_t>(selected)].id);
    const int p2Selected=juce::jlimit(0,static_cast<int>(p2FxMachines().size())-1,juce::roundToInt(p2MachineRaw?p2MachineRaw->load():0));
    const int p2Latency=machine2.latencyForMachine(p2FxMachines()[static_cast<size_t>(p2Selected)].id);
    const int latency=isSynthVersion?0:p1Latency+p2Latency+fxSlotsLatency; // P2 stays latency-aligned even when its group MIX is 0.
    if(getLatencySamples()!=latency)setLatencySamples(latency);
}
void MonomachineNovaAudioProcessor::queueArpPageRandomWrite(int firstStep,int lastStep){
    arpPageRndWasEnabled.store(true,std::memory_order_relaxed);
    const int first=juce::jlimit(0,63,firstStep),last=juce::jlimit(first,63,lastStep);
    arpPageRndPendingStart.store(first,std::memory_order_relaxed);arpPageRndPendingEnd.store(last,std::memory_order_relaxed);
    arpPageRndWritePending.store(true,std::memory_order_release);triggerAsyncUpdate();
}
int MonomachineNovaAudioProcessor::plockStepForArpEvent(int arpAbsoluteStep) noexcept {
    if(plockSyncRuntime)return juce::jlimit(plockStepWindowStartRuntime,plockStepWindowEndRuntime,arpAbsoluteStep);
    const int out=juce::jlimit(plockStepWindowStartRuntime,plockStepWindowEndRuntime,plockStepCursor);
    ++plockStepCursor;
    if(plockStepCursor>plockStepWindowEndRuntime)plockStepCursor=plockStepWindowStartRuntime;
    return out;
}
void MonomachineNovaAudioProcessor::applyArpAndPlockWindowModulation(const std::array<float,2>& arpWindowMod,const std::array<float,2>& plockWindowMod) noexcept {
    const int arpStart=juce::jlimit(0,63,juce::roundToInt(static_cast<float>(arpStepWindowStartBase)+arpWindowMod[0]));
    const int arpRequestedEnd=juce::jlimit(0,63,juce::roundToInt(static_cast<float>(arpStepWindowEndBase)+arpWindowMod[1]));
    const int arpEnd=juce::jlimit(arpStart,63,arpRequestedEnd);
    arpStepWindowStartRuntime=arpStart;arpStepWindowEndRuntime=arpEnd;
    arp.setStepWindow(static_cast<uint8_t>(arpStart),static_cast<uint8_t>(arpEnd));
    const bool sync=plockSyncRaw==nullptr||plockSyncRaw->load(std::memory_order_relaxed)>0.5f;
    const int rawStart=juce::jlimit(0,63,juce::roundToInt(plockStepStartRaw?plockStepStartRaw->load(std::memory_order_relaxed)-1.0f:0.0f));
    const int rawRequestedEnd=juce::jlimit(0,63,juce::roundToInt(plockStepEndRaw?plockStepEndRaw->load(std::memory_order_relaxed)-1.0f:15.0f));
    const int rawEnd=juce::jlimit(rawStart,63,rawRequestedEnd);
    const int nextStart=sync?arpStart:juce::jlimit(0,63,juce::roundToInt(static_cast<float>(rawStart)+plockWindowMod[0]));
    const int nextRequestedEnd=sync?arpEnd:juce::jlimit(0,63,juce::roundToInt(static_cast<float>(rawEnd)+plockWindowMod[1]));
    const int nextEnd=juce::jlimit(nextStart,63,nextRequestedEnd);
    const bool oldSync=plockSyncRuntime;
    const bool changed=sync!=oldSync||nextStart!=plockStepWindowStartRuntime||nextEnd!=plockStepWindowEndRuntime;
    plockSyncRuntime=sync;plockStepWindowStartRuntime=nextStart;plockStepWindowEndRuntime=nextEnd;
    if(changed&&(oldSync!=sync||plockStepCursor<nextStart||plockStepCursor>nextEnd))plockStepCursor=nextStart;
}
void MonomachineNovaAudioProcessor::panic(){
    notes.fill({});pedal.fill(false);wheels.fill(8192);arpHeld.fill(false);currentKey=-1;serial=0;ccX=ccY=-1;plockStepCursor=plockStepWindowStartRuntime;plockStepEcho.store(plockStepCursor,std::memory_order_relaxed);
    plockClearRuntime(); // 1.7.1
    envelope.reset(sr);dryDelay.clear();machine.clear();chain.clear();envelope2.reset(sr);for(auto& env:modEnvelopes)env.reset(sr);modEnvValue.fill(0.0f);dryDelay2.clear();machine2.clear();chain2.clear();
    for(auto& engine:fxEngines)engine.clear(); // active user FX-slots own native tail state too
    feedbackNetwork.clear();arp.reset(sr,bpm);matrix.reset();msegPhase.fill(0.0);msegValue.fill(0.0f);for(auto& p:msegPhaseUi)p.store(0.0);liveStepVelocity=0.0f;liveStepTranspose=0.0f;liveArpGate=0.0f;selectedStepVelocity=1.0f;selectedStepTranspose=0.0f;selectedStepHold=0.0f;for(auto& l:lfos)l.reset(); // PANIC clears P1/P2, native classic cores, FX-slots and FB state
}
void MonomachineNovaAudioProcessor::trigger(int note,float vel,int channel){
    soundingNote=note;soundingChannel=channel;
    for(int mi=0;mi<8;++mi){auto* rt=globalRaw[static_cast<size_t>(MsegRetrig+4*mi)]; // 1.7.8: 0 RETRIG / 1 VITAL / 2 FREE / 3 SUSTAIN / 4 LOOP POINT / 5 LOOP HOLD (семантика как в Vital synth_lfo)
        const int mode=rt==nullptr?0:juce::jlimit(0,5,juce::roundToInt(rt->load()));
        if(mode==0||mode==1||mode==3){msegPhase[static_cast<size_t>(mi)]=0.0;msegValue[static_cast<size_t>(mi)]=mseg[static_cast<size_t>(mi)].value(0.0f);}}
    liveArpGate=1.0f;
    // 1.6.14: портаменто быстрее и явное: время = (PORT/127)^1.5 * 0.8 с, масштабируется SPEED (ПКМ на PORT).
    const float portaSpeed=portaSpeedRaw?juce::jlimit(0.1f,3.0f,0.2f+2.8f*nova::norm(portaSpeedRaw->load())):1.0f;
    const float glide=base[15]<=0?0:std::pow(nova::norm(base[15]),1.5f)*0.8f/portaSpeed;
    pitch.reset(sr,glide);if(envelope.stage==0||glide<=0)pitch.setCurrentAndTargetValue(static_cast<float>(note));else pitch.setTargetValue(static_cast<float>(note));
    if(nova::machines()[static_cast<size_t>(activeMachine)].id==7){std::array<float,8> values{};std::copy_n(base.begin(),8,values.begin());machine.set(7,values);}
    machine.on(note);machine2.on(note);if(nova::machines()[static_cast<size_t>(activeMachine)].id==7){lastBboxSample.store(machine.bbox.playingSlot());++bboxHitCounter;}envelope.on();envelope2.on();chain.trigger(note,vel);chain2.trigger(note,vel);for(auto& env:modEnvelopes)env.on();velocity.setTargetValue(vel); // P1/P2 filter extras and MOD ENV share the actual trigger velocity
    for(size_t i=0;i<3;++i){lfos[i].trigger(effectiveLfoParams[i]);lfos3[i].trigger(effectiveLfoParams[3+i]); // 1.8.0e: P1 LFO1-6
        lfos2[i].trigger(effectiveLfoParams[6+i]);lfos4[i].trigger(effectiveLfoParams[9+i]);} // P2 LFO1-6
    matrix.setNoteAndVelocity(static_cast<uint8_t>(note),static_cast<uint8_t>(std::round(vel*127)));
}
void MonomachineNovaAudioProcessor::release(){envelope.off();chain.release();envelope2.off();chain2.release();for(auto& env:modEnvelopes)env.off();}
int MonomachineNovaAudioProcessor::fxVisualZoneAt(int visualIndex) noexcept {
    static constexpr int order[fxZonesCount]={
        fxG_P1pre,fxG_EQ_FILT,fxG_FILT_DIST,fxG_DIST_ENV,fxG_ENV_VOLPAN,
        fxG_VOLPAN_SRR,fxG_P1_SRR_DELAY,fxG_P1post,
        fxG_P2pre,fxG_P2MACH_CHAIN,fxG_P2_EQ_FILT,fxG_P2_FILT_DIST,
        fxG_P2_DIST_ENV,fxG_P2_ENV_VOLPAN,fxG_P2_VOLPAN_SRR,
        fxG_P2_SRR_DELAY,fxG_P2post};
    return order[juce::jlimit(0,fxZonesCount-1,visualIndex)];
}
int MonomachineNovaAudioProcessor::fxVisualIndexOfZone(int zone) noexcept {
    for(int i=0;i<fxZonesCount;++i)if(fxVisualZoneAt(i)==zone)return i;
    return 0;
}
bool MonomachineNovaAudioProcessor::isFxSlotMachineId(int machineId) noexcept {
    if(machineId==0)return true;
    for(const auto& m:p2FxMachines())if(m.id==machineId)return true;
    return false;
}
std::array<float,8> MonomachineNovaAudioProcessor::fxSlotDefaultsForMachine(int machineId){
    std::array<float,8> result{{64,64,64,64,64,64,64,64}};
    for(const auto& m:p2FxMachines())if(m.id==machineId){
        for(int i=0;i<8;++i){const auto& def=m.synthParams[static_cast<size_t>(i)];result[static_cast<size_t>(i)]=def.maxVal>0?static_cast<float>(def.defaultVal):0.0f;}
        break;
    }
    // Та же стартовая панель CHORUS, что у P2 (NovaData::kChorusDef).
    if(machineId==15)result={{70,95,41,0,127,127,127,64}};
    return result;
}
void MonomachineNovaAudioProcessor::fxSlotsBeginEdit() noexcept {
    fxSlotEditSerial.fetch_add(1,std::memory_order_acq_rel); // even -> odd: аудио не берёт половину пачки
}
void MonomachineNovaAudioProcessor::fxSlotsEndEdit() noexcept {
    fxSlotEditSerial.fetch_add(1,std::memory_order_release); // odd -> even: снимок готов
}
MonomachineNovaAudioProcessor::FxSlotState MonomachineNovaAudioProcessor::fxSlotState(int slot) const noexcept {
    FxSlotState result;
    if(slot<0||slot>=fxSlotsMax)return result;
    const auto& source=fxSlotControls[static_cast<size_t>(slot)];
    for(int attempt=0;attempt<3;++attempt){
        const uint64_t before=fxSlotEditSerial.load(std::memory_order_acquire);
        if((before&1u)!=0u)continue;
        result.used=source.used.load(std::memory_order_relaxed);
        result.zone=source.zone.load(std::memory_order_relaxed);
        result.machineId=source.machineId.load(std::memory_order_relaxed);
        result.on=source.on.load(std::memory_order_relaxed);
        result.order=source.order.load(std::memory_order_relaxed);
        for(int i=0;i<8;++i)result.p[static_cast<size_t>(i)]=source.p[static_cast<size_t>(i)].load(std::memory_order_relaxed);
        if(fxSlotEditSerial.load(std::memory_order_acquire)==before)return result;
    }
    return result; // UI перерисует на следующем таймере, если попала в чужую запись состояния
}
int MonomachineNovaAudioProcessor::fxSlotUsedCount() const noexcept {
    int count=0;for(const auto& s:fxSlotControls)if(s.used.load(std::memory_order_relaxed))++count;return count;
}
int MonomachineNovaAudioProcessor::fxSlotAdd(int zone){
    zone=juce::jlimit(0,fxZonesCount-1,zone);
    fxSlotsBeginEdit();
    int freeSlot=-1,maxOrder=-1;
    for(int i=0;i<fxSlotsMax;++i){const auto& c=fxSlotControls[static_cast<size_t>(i)];
        if(!c.used.load(std::memory_order_relaxed)){if(freeSlot<0)freeSlot=i;continue;}
        if(c.zone.load(std::memory_order_relaxed)==zone)maxOrder=std::max(maxOrder,c.order.load(std::memory_order_relaxed));}
    if(freeSlot>=0){auto& c=fxSlotControls[static_cast<size_t>(freeSlot)];
        const auto defaults=fxSlotDefaultsForMachine(0);
        c.zone.store(zone,std::memory_order_relaxed);c.machineId.store(0,std::memory_order_relaxed);c.on.store(false,std::memory_order_relaxed);c.order.store(maxOrder+10,std::memory_order_relaxed);
        for(int i=0;i<8;++i)c.p[static_cast<size_t>(i)].store(defaults[static_cast<size_t>(i)],std::memory_order_relaxed);
        c.used.store(true,std::memory_order_relaxed);
    }
    fxSlotsEndEdit();
    return freeSlot;
}
void MonomachineNovaAudioProcessor::fxSlotRemove(int slot){
    if(slot<0||slot>=fxSlotsMax)return;
    fxSlotsBeginEdit();
    auto& c=fxSlotControls[static_cast<size_t>(slot)];
    c.on.store(false,std::memory_order_relaxed);c.machineId.store(0,std::memory_order_relaxed);c.used.store(false,std::memory_order_relaxed);
    fxSlotsEndEdit();
}
void MonomachineNovaAudioProcessor::fxSlotsClearAll(){
    fxSlotsFromTree(juce::ValueTree());
}
void MonomachineNovaAudioProcessor::fxSlotSetMachine(int slot,int machineId){
    if(slot<0||slot>=fxSlotsMax)return;
    machineId=isFxSlotMachineId(machineId)?machineId:0;
    auto& c=fxSlotControls[static_cast<size_t>(slot)];
    if(!c.used.load(std::memory_order_relaxed))return;
    fxSlotsBeginEdit();
    const int old=c.machineId.load(std::memory_order_relaxed);
    if(old!=machineId){const auto defaults=fxSlotDefaultsForMachine(machineId);for(int i=0;i<8;++i)c.p[static_cast<size_t>(i)].store(defaults[static_cast<size_t>(i)],std::memory_order_relaxed);}
    c.machineId.store(machineId,std::memory_order_relaxed);
    c.on.store(machineId!=0,std::memory_order_relaxed); // выбрать машину = включить её, как обычную FX-слот
    fxSlotsEndEdit();
}
void MonomachineNovaAudioProcessor::fxSlotSetOn(int slot,bool on){
    if(slot<0||slot>=fxSlotsMax)return;
    auto& c=fxSlotControls[static_cast<size_t>(slot)];
    if(!c.used.load(std::memory_order_relaxed)||c.machineId.load(std::memory_order_relaxed)==0)return;
    fxSlotsBeginEdit();c.on.store(on,std::memory_order_relaxed);fxSlotsEndEdit();
}
void MonomachineNovaAudioProcessor::fxSlotSetParameter(int slot,int parameter,float value){
    if(slot<0||slot>=fxSlotsMax||parameter<0||parameter>=8)return;
    auto& c=fxSlotControls[static_cast<size_t>(slot)];
    if(!c.used.load(std::memory_order_relaxed))return;
    const int machineId=c.machineId.load(std::memory_order_relaxed);
    float lo=0.0f,hi=127.0f;
    for(const auto& m:p2FxMachines())if(m.id==machineId){const auto& def=m.synthParams[static_cast<size_t>(parameter)];lo=static_cast<float>(def.minVal);hi=static_cast<float>(def.maxVal);break;}
    fxSlotsBeginEdit();c.p[static_cast<size_t>(parameter)].store(juce::jlimit(lo,hi,value),std::memory_order_relaxed);fxSlotsEndEdit();
}
void MonomachineNovaAudioProcessor::fxSlotMoveToGap(int slot,int targetZone,int targetPosition){
    if(slot<0||slot>=fxSlotsMax)return;
    targetZone=juce::jlimit(0,fxZonesCount-1,targetZone);
    auto& source=fxSlotControls[static_cast<size_t>(slot)];
    if(!source.used.load(std::memory_order_relaxed))return;
    fxSlotsBeginEdit();
    const int oldZone=source.zone.load(std::memory_order_relaxed);
    std::array<int,fxSlotsMax> target{};int targetCount=0;
    for(int i=0;i<fxSlotsMax;++i){if(i==slot)continue;const auto& c=fxSlotControls[static_cast<size_t>(i)];if(c.used.load(std::memory_order_relaxed)&&c.zone.load(std::memory_order_relaxed)==targetZone)target[static_cast<size_t>(targetCount++)]=i;}
    for(int i=1;i<targetCount;++i){int key=target[static_cast<size_t>(i)],j=i;while(j>0&&fxSlotControls[static_cast<size_t>(target[static_cast<size_t>(j-1)])].order.load(std::memory_order_relaxed)>fxSlotControls[static_cast<size_t>(key)].order.load(std::memory_order_relaxed)){target[static_cast<size_t>(j)]=target[static_cast<size_t>(j-1)];--j;}target[static_cast<size_t>(j)]=key;}
    targetPosition=juce::jlimit(0,targetCount,targetPosition);
    for(int i=targetCount;i>targetPosition;--i)target[static_cast<size_t>(i)]=target[static_cast<size_t>(i-1)];
    target[static_cast<size_t>(targetPosition)]=slot;++targetCount;
    source.zone.store(targetZone,std::memory_order_relaxed);
    for(int i=0;i<targetCount;++i)fxSlotControls[static_cast<size_t>(target[static_cast<size_t>(i)])].order.store(i*10,std::memory_order_relaxed);
    if(oldZone!=targetZone){ // сжать порядки покинутой перемычки
        std::array<int,fxSlotsMax> old{};int oldCount=0;
        for(int i=0;i<fxSlotsMax;++i){const auto& c=fxSlotControls[static_cast<size_t>(i)];if(c.used.load(std::memory_order_relaxed)&&c.zone.load(std::memory_order_relaxed)==oldZone)old[static_cast<size_t>(oldCount++)]=i;}
        for(int i=1;i<oldCount;++i){int key=old[static_cast<size_t>(i)],j=i;while(j>0&&fxSlotControls[static_cast<size_t>(old[static_cast<size_t>(j-1)])].order.load(std::memory_order_relaxed)>fxSlotControls[static_cast<size_t>(key)].order.load(std::memory_order_relaxed)){old[static_cast<size_t>(j)]=old[static_cast<size_t>(j-1)];--j;}old[static_cast<size_t>(j)]=key;}
        for(int i=0;i<oldCount;++i)fxSlotControls[static_cast<size_t>(old[static_cast<size_t>(i)])].order.store(i*10,std::memory_order_relaxed);
    }
    fxSlotsEndEdit();
}
void MonomachineNovaAudioProcessor::fxSlotsSyncToAudio() noexcept {
    const uint64_t before=fxSlotEditSerial.load(std::memory_order_acquire);
    if(before==fxSlotAudioSerial||(before&1u)!=0u)return;
    std::array<FxSlotState,fxSlotsMax> next{};
    for(int s=0;s<fxSlotsMax;++s){const auto& c=fxSlotControls[static_cast<size_t>(s)];auto& d=next[static_cast<size_t>(s)];
        d.used=c.used.load(std::memory_order_relaxed);d.zone=juce::jlimit(0,fxZonesCount-1,c.zone.load(std::memory_order_relaxed));
        d.machineId=c.machineId.load(std::memory_order_relaxed);d.machineId=isFxSlotMachineId(d.machineId)?d.machineId:0;
        d.on=c.on.load(std::memory_order_relaxed)&&d.machineId!=0;d.order=c.order.load(std::memory_order_relaxed);
        for(int i=0;i<8;++i)d.p[static_cast<size_t>(i)]=c.p[static_cast<size_t>(i)].load(std::memory_order_relaxed);
    }
    if(fxSlotEditSerial.load(std::memory_order_acquire)!=before)return;

    for(int s=0;s<fxSlotsMax;++s){const auto& old=fxSlots[static_cast<size_t>(s)];const auto& now=next[static_cast<size_t>(s)];
        if(old.used!=now.used||old.machineId!=now.machineId||old.on!=now.on||old.p!=now.p)
            fxEngines[static_cast<size_t>(s)].setState(now.used?now.machineId:0,now.used&&now.on,now.p);
        fxSlots[static_cast<size_t>(s)]=now;
    }
    fxSlotRouteCount.fill(0);fxSlotsUsed=false;fxSlotsLatency=0;
    for(int s=0;s<fxSlotsMax;++s){const auto& state=fxSlots[static_cast<size_t>(s)];if(!state.used||!state.on||state.machineId==0)continue;
        const int zone=state.zone;int& count=fxSlotRouteCount[static_cast<size_t>(zone)];
        int pos=count++;fxSlotRoute[static_cast<size_t>(zone)][static_cast<size_t>(pos)]=s;
        while(pos>0){const int prev=fxSlotRoute[static_cast<size_t>(zone)][static_cast<size_t>(pos-1)];
            if(fxSlots[static_cast<size_t>(prev)].order<=state.order)break;
            fxSlotRoute[static_cast<size_t>(zone)][static_cast<size_t>(pos)]=prev;--pos;}
        fxSlotRoute[static_cast<size_t>(zone)][static_cast<size_t>(pos)]=s;
        fxSlotsUsed=true;fxSlotsLatency+=fxEngines[static_cast<size_t>(s)].latencyFrames();
    }
    fxSlotAudioSerial=before;
}
void MonomachineNovaAudioProcessor::fxSlotsPrepare(){
    for(auto& engine:fxEngines)engine.prepare(sr);
    fxSlotAudioSerial=~uint64_t{0}; // после смены SR каждый занятый слот снова получает state/prepare
}
void MonomachineNovaAudioProcessor::fxZone(int zone,float* l,float* r,int n) noexcept {
    if(zone<0||zone>=fxZonesCount||n<=0)return;
    // A RETURN enters before the user FX-slots in this bridge; a SEND is taken
    // after them. When FB is off these branches are bit-transparent no-ops.
    if(feedbackNetwork.isActive())feedbackNetwork.returnAt(zone,l,r,n);
    if(fxSlotsUsed){
        const int count=fxSlotRouteCount[static_cast<size_t>(zone)];
        for(int i=0;i<count;++i)fxEngines[static_cast<size_t>(fxSlotRoute[static_cast<size_t>(zone)][static_cast<size_t>(i)])].process(l,r,n);
    }
    if(feedbackNetwork.isActive())feedbackNetwork.sendAt(zone,l,r,n);
}
juce::ValueTree MonomachineNovaAudioProcessor::fxSlotsToTree() const {
    juce::ValueTree tree("FXSLOTS");
    for(int s=0;s<fxSlotsMax;++s){const auto slot=fxSlotState(s);if(!slot.used)continue;
        juce::ValueTree node("SLOT");node.setProperty("slot",s,nullptr);node.setProperty("zone",slot.zone,nullptr);node.setProperty("machine",slot.machineId,nullptr);node.setProperty("on",slot.on?1:0,nullptr);node.setProperty("order",slot.order,nullptr);
        for(int i=0;i<8;++i)node.setProperty("p"+juce::String(i),slot.p[static_cast<size_t>(i)],nullptr);
        tree.addChild(node,-1,nullptr);
    }
    return tree;
}
void MonomachineNovaAudioProcessor::fxSlotsFromTree(const juce::ValueTree& tree){
    fxSlotsBeginEdit();
    for(auto& c:fxSlotControls){c.used.store(false,std::memory_order_relaxed);c.on.store(false,std::memory_order_relaxed);c.machineId.store(0,std::memory_order_relaxed);c.zone.store(fxG_P1pre,std::memory_order_relaxed);c.order.store(0,std::memory_order_relaxed);for(auto& value:c.p)value.store(64.0f,std::memory_order_relaxed);}
    if(tree.isValid())for(int ni=0;ni<tree.getNumChildren();++ni){const auto node=tree.getChild(ni);if(!node.hasType("SLOT"))continue;
        const int s=static_cast<int>(node.getProperty("slot",-1));if(s<0||s>=fxSlotsMax)continue;
        auto& c=fxSlotControls[static_cast<size_t>(s)];const int machineId=static_cast<int>(node.getProperty("machine",0));
        const int fixedMachine=isFxSlotMachineId(machineId)?machineId:0;const auto defaults=fxSlotDefaultsForMachine(fixedMachine);
        c.zone.store(juce::jlimit(0,fxZonesCount-1,static_cast<int>(node.getProperty("zone",fxG_P1pre))),std::memory_order_relaxed);
        c.machineId.store(fixedMachine,std::memory_order_relaxed);c.on.store(static_cast<int>(node.getProperty("on",0))!=0&&fixedMachine!=0,std::memory_order_relaxed);c.order.store(static_cast<int>(node.getProperty("order",s*10)),std::memory_order_relaxed);
        for(int i=0;i<8;++i)c.p[static_cast<size_t>(i)].store(juce::jlimit(0.0f,127.0f,static_cast<float>(node.getProperty("p"+juce::String(i),defaults[static_cast<size_t>(i)]))),std::memory_order_relaxed);
        c.used.store(true,std::memory_order_relaxed);
    }
    fxSlotsEndEdit();
}
// ===== 1.8.5: movable classic P1/P2 DSP order ===============================
namespace {
using ClassicOrder=std::array<int,MonomachineNovaAudioProcessor::classicStagesCount>;
static constexpr int kLegacyClassicStagesCount=6;
static constexpr int kClassicRouteFormat=4;
using LegacyClassicOrder=std::array<int,kLegacyClassicStagesCount>;
ClassicOrder canonicalClassicOrder(){return {{MonomachineNovaAudioProcessor::classicEq,MonomachineNovaAudioProcessor::classicFilt,MonomachineNovaAudioProcessor::classicDist,MonomachineNovaAudioProcessor::classicEnv,MonomachineNovaAudioProcessor::classicVolPan,MonomachineNovaAudioProcessor::classicSrr,MonomachineNovaAudioProcessor::classicDelay}};}
// format=2 briefly serialized the previous wrong default with DIST before the
// filter. Preserve any user permutation, but remap that exact default only.
ClassicOrder prePostFilterDistDefault(){return {{MonomachineNovaAudioProcessor::classicDist,MonomachineNovaAudioProcessor::classicEq,MonomachineNovaAudioProcessor::classicFilt,MonomachineNovaAudioProcessor::classicEnv,MonomachineNovaAudioProcessor::classicVolPan,MonomachineNovaAudioProcessor::classicSrr,MonomachineNovaAudioProcessor::classicDelay}};}
bool validClassicOrder(const ClassicOrder& order){
    std::array<bool,MonomachineNovaAudioProcessor::classicStagesCount> seen{};
    for(const int stage:order){if(stage<0||stage>=MonomachineNovaAudioProcessor::classicStagesCount||seen[static_cast<size_t>(stage)])return false;seen[static_cast<size_t>(stage)]=true;}
    return true;
}
bool validLegacyClassicOrder(const LegacyClassicOrder& order){
    std::array<bool,kLegacyClassicStagesCount> seen{};
    for(const int stage:order){if(stage<0||stage>=kLegacyClassicStagesCount||seen[static_cast<size_t>(stage)])return false;seen[static_cast<size_t>(stage)]=true;}
    return true;
}
ClassicOrder migratePrePostFilterDistOrder(int serializedFormat,const ClassicOrder& stored){
    if(!validClassicOrder(stored))return canonicalClassicOrder();
    return serializedFormat<kClassicRouteFormat&&stored==prePostFilterDistDefault()?canonicalClassicOrder():stored;
}
// The exact reset sequence written by the six-stage release was
// DIST -> SRR -> FILT -> EQ -> ENV -> DELAY. It was a product default, not a
// user decision: upgrade it directly to the documented reset topology. For a
// genuinely customised legacy order there is no equivalent intent marker, so
// preserve its six existing relative positions and insert explicit VOL/PAN just
// before SRR (never after rate reduction).
ClassicOrder migrateLegacyClassicOrder(const LegacyClassicOrder& oldOrder){
    if(!validLegacyClassicOrder(oldOrder))return canonicalClassicOrder();
    static constexpr LegacyClassicOrder legacyReleaseDefault{{
        MonomachineNovaAudioProcessor::classicDist,
        MonomachineNovaAudioProcessor::classicSrr,
        MonomachineNovaAudioProcessor::classicFilt,
        MonomachineNovaAudioProcessor::classicEq,
        MonomachineNovaAudioProcessor::classicEnv,
        MonomachineNovaAudioProcessor::classicDelay
    }};
    if(oldOrder==legacyReleaseDefault)return canonicalClassicOrder();
    ClassicOrder expanded{};int write=0;
    for(const int stage:oldOrder){
        if(stage==MonomachineNovaAudioProcessor::classicSrr)expanded[static_cast<size_t>(write++)]=MonomachineNovaAudioProcessor::classicVolPan;
        expanded[static_cast<size_t>(write++)]=stage;
    }
    return write==MonomachineNovaAudioProcessor::classicStagesCount&&validClassicOrder(expanded)?expanded:canonicalClassicOrder();
}
}
void MonomachineNovaAudioProcessor::classicRouteBeginEdit() noexcept {classicRouteEditSerial.fetch_add(1,std::memory_order_acq_rel);}
void MonomachineNovaAudioProcessor::classicRouteEndEdit() noexcept {classicRouteEditSerial.fetch_add(1,std::memory_order_release);}
MonomachineNovaAudioProcessor::ClassicRouteState MonomachineNovaAudioProcessor::classicRouteState() const noexcept {
    ClassicRouteState result;
    for(int attempt=0;attempt<3;++attempt){
        const uint64_t before=classicRouteEditSerial.load(std::memory_order_acquire);if((before&1u)!=0u)continue;
        for(int i=0;i<classicStagesCount;++i){
            const auto at=static_cast<size_t>(i);
            result.p1[at]=classicRouteControl.p1[at].load(std::memory_order_relaxed);
            result.p2[at]=classicRouteControl.p2[at].load(std::memory_order_relaxed);
            result.p1Enabled[at]=classicRouteControl.p1Enabled[at].load(std::memory_order_relaxed);
            result.p2Enabled[at]=classicRouteControl.p2Enabled[at].load(std::memory_order_relaxed);
        }
        if(classicRouteEditSerial.load(std::memory_order_acquire)==before)return result;
    }
    return result;
}
void MonomachineNovaAudioProcessor::classicRouteMove(int chain,int stage,int targetPosition){
    if(chain<0||chain>1||stage<0||stage>=classicStagesCount)return;
    auto state=classicRouteState();auto& order=chain==0?state.p1:state.p2;if(!validClassicOrder(order))order=canonicalClassicOrder();
    int from=-1;for(int i=0;i<classicStagesCount;++i)if(order[static_cast<size_t>(i)]==stage){from=i;break;}if(from<0)return;
    targetPosition=juce::jlimit(0,classicStagesCount-1,targetPosition);const int moved=order[static_cast<size_t>(from)];
    if(from<targetPosition)for(int i=from;i<targetPosition;++i)order[static_cast<size_t>(i)]=order[static_cast<size_t>(i+1)];
    else if(from>targetPosition)for(int i=from;i>targetPosition;--i)order[static_cast<size_t>(i)]=order[static_cast<size_t>(i-1)];
    order[static_cast<size_t>(targetPosition)]=moved;
    classicRouteBeginEdit();auto& control=chain==0?classicRouteControl.p1:classicRouteControl.p2;for(int i=0;i<classicStagesCount;++i)control[static_cast<size_t>(i)].store(order[static_cast<size_t>(i)],std::memory_order_relaxed);classicRouteEndEdit();
}
void MonomachineNovaAudioProcessor::classicRouteSetEnabled(int chain,int stage,bool enabled){
    if(chain<0||chain>1||stage<0||stage>=classicStagesCount)return;
    classicRouteBeginEdit();auto& control=chain==0?classicRouteControl.p1Enabled:classicRouteControl.p2Enabled;
    control[static_cast<size_t>(stage)].store(enabled,std::memory_order_relaxed);classicRouteEndEdit();
}
void MonomachineNovaAudioProcessor::classicRouteReset(int chain){
    if(chain<0||chain>1)return;const auto canonical=canonicalClassicOrder();classicRouteBeginEdit();
    auto& control=chain==0?classicRouteControl.p1:classicRouteControl.p2;
    auto& enabled=chain==0?classicRouteControl.p1Enabled:classicRouteControl.p2Enabled;
    for(int i=0;i<classicStagesCount;++i){control[static_cast<size_t>(i)].store(canonical[static_cast<size_t>(i)],std::memory_order_relaxed);enabled[static_cast<size_t>(i)].store(true,std::memory_order_relaxed);}classicRouteEndEdit();
}
void MonomachineNovaAudioProcessor::classicRoutesResetAll(){
    const auto canonical=canonicalClassicOrder();classicRouteBeginEdit();for(int i=0;i<classicStagesCount;++i){
        const auto at=static_cast<size_t>(i);classicRouteControl.p1[at].store(canonical[at],std::memory_order_relaxed);classicRouteControl.p2[at].store(canonical[at],std::memory_order_relaxed);
        classicRouteControl.p1Enabled[at].store(true,std::memory_order_relaxed);classicRouteControl.p2Enabled[at].store(true,std::memory_order_relaxed);
    }classicRouteEndEdit();
}
void MonomachineNovaAudioProcessor::classicRouteSyncToAudio() noexcept {
    const uint64_t before=classicRouteEditSerial.load(std::memory_order_acquire);if(before==classicRouteAudioSerial||(before&1u)!=0u)return;
    ClassicRouteState next;for(int i=0;i<classicStagesCount;++i){const auto at=static_cast<size_t>(i);next.p1[at]=classicRouteControl.p1[at].load(std::memory_order_relaxed);next.p2[at]=classicRouteControl.p2[at].load(std::memory_order_relaxed);next.p1Enabled[at]=classicRouteControl.p1Enabled[at].load(std::memory_order_relaxed);next.p2Enabled[at]=classicRouteControl.p2Enabled[at].load(std::memory_order_relaxed);}
    if(classicRouteEditSerial.load(std::memory_order_acquire)!=before)return;
    if(!validClassicOrder(next.p1))next.p1=canonicalClassicOrder();if(!validClassicOrder(next.p2))next.p2=canonicalClassicOrder();
    classicRouteAudio=next;classicRouteAudioSerial=before;
}
juce::ValueTree MonomachineNovaAudioProcessor::classicRouteToTree() const {
    const auto route=classicRouteState();juce::ValueTree tree("CLASSICROUTE");tree.setProperty("format",kClassicRouteFormat,nullptr);
    for(int i=0;i<classicStagesCount;++i){const auto at=static_cast<size_t>(i);tree.setProperty("p1_"+juce::String(i),route.p1[at],nullptr);tree.setProperty("p2_"+juce::String(i),route.p2[at],nullptr);tree.setProperty("p1_on_"+juce::String(i),route.p1Enabled[at],nullptr);tree.setProperty("p2_on_"+juce::String(i),route.p2Enabled[at],nullptr);}return tree;
}
void MonomachineNovaAudioProcessor::classicRouteFromTree(const juce::ValueTree& tree){
    ClassicRouteState next;
    const int serializedFormat=tree.isValid()?static_cast<int>(tree.getProperty("format",0)):0;
    const auto load=[&tree,serializedFormat](const juce::String& prefix,ClassicOrder& destination){
        if(!tree.isValid()||!tree.hasProperty(prefix+"0"))return;
        if(!tree.hasProperty(prefix+juce::String(kLegacyClassicStagesCount))){
            LegacyClassicOrder legacy{};
            for(int i=0;i<kLegacyClassicStagesCount;++i){const auto key=prefix+juce::String(i);if(!tree.hasProperty(key))return;legacy[static_cast<size_t>(i)]=static_cast<int>(tree.getProperty(key,0));}
            destination=migrateLegacyClassicOrder(legacy);return;
        }
        ClassicOrder current{};
        for(int i=0;i<classicStagesCount;++i){const auto key=prefix+juce::String(i);if(!tree.hasProperty(key)){destination=canonicalClassicOrder();return;}current[static_cast<size_t>(i)]=static_cast<int>(tree.getProperty(key,0));}
        destination=migratePrePostFilterDistOrder(serializedFormat,current);
    };
    const auto loadEnabled=[&tree](const juce::String& prefix,std::array<bool,classicStagesCount>& destination){
        if(!tree.isValid())return;
        for(int stage=0;stage<classicStagesCount;++stage){const auto key=prefix+juce::String(stage);if(tree.hasProperty(key))destination[static_cast<size_t>(stage)]=static_cast<bool>(tree.getProperty(key,true));}
    };
    load("p1_",next.p1);load("p2_",next.p2);loadEnabled("p1_on_",next.p1Enabled);loadEnabled("p2_on_",next.p2Enabled);
    if(!validClassicOrder(next.p1))next.p1=canonicalClassicOrder();if(!validClassicOrder(next.p2))next.p2=canonicalClassicOrder();
    classicRouteBeginEdit();for(int i=0;i<classicStagesCount;++i){const auto at=static_cast<size_t>(i);classicRouteControl.p1[at].store(next.p1[at],std::memory_order_relaxed);classicRouteControl.p2[at].store(next.p2[at],std::memory_order_relaxed);classicRouteControl.p1Enabled[at].store(next.p1Enabled[at],std::memory_order_relaxed);classicRouteControl.p2Enabled[at].store(next.p2Enabled[at],std::memory_order_relaxed);}classicRouteEndEdit();
}

// ===== 1.8.4: single FB route / future multi-line graph ====================
void MonomachineNovaAudioProcessor::feedbackBeginEdit() noexcept { 
    feedbackEditSerial.fetch_add(1,std::memory_order_acq_rel);
}
void MonomachineNovaAudioProcessor::feedbackEndEdit() noexcept {
    feedbackEditSerial.fetch_add(1,std::memory_order_release);
}
MonomachineNovaAudioProcessor::FeedbackState MonomachineNovaAudioProcessor::feedbackState() const noexcept {
    FeedbackState result;
    for(int attempt=0;attempt<3;++attempt){
        const uint64_t before=feedbackEditSerial.load(std::memory_order_acquire);
        if((before&1u)!=0u)continue;
        result.on=feedbackControl.on.load(std::memory_order_relaxed);
        result.sendZone=feedbackControl.sendZone.load(std::memory_order_relaxed);
        result.returnZone=feedbackControl.returnZone.load(std::memory_order_relaxed);
        result.sendGain=feedbackControl.sendGain.load(std::memory_order_relaxed);
        result.feedbackGain=feedbackControl.feedbackGain.load(std::memory_order_relaxed);
        result.returnGain=feedbackControl.returnGain.load(std::memory_order_relaxed);
        result.dcControl=feedbackControl.dcControl.load(std::memory_order_relaxed);
        result.clipEnabled=feedbackControl.clipEnabled.load(std::memory_order_relaxed);
        if(feedbackEditSerial.load(std::memory_order_acquire)==before)return result;
    }
    return result;
}
void MonomachineNovaAudioProcessor::feedbackSetOn(bool on){
    feedbackBeginEdit();feedbackControl.on.store(on,std::memory_order_relaxed);feedbackEndEdit();
}
void MonomachineNovaAudioProcessor::feedbackSetSendZone(int zone){
    feedbackBeginEdit();feedbackControl.sendZone.store(juce::jlimit(0,fxZonesCount-1,zone),std::memory_order_relaxed);feedbackEndEdit();
}
void MonomachineNovaAudioProcessor::feedbackSetReturnZone(int zone){
    feedbackBeginEdit();feedbackControl.returnZone.store(juce::jlimit(0,fxZonesCount-1,zone),std::memory_order_relaxed);feedbackEndEdit();
}
void MonomachineNovaAudioProcessor::feedbackSetSendGain(float value){
    feedbackBeginEdit();feedbackControl.sendGain.store(juce::jlimit(0.0f,127.0f,value),std::memory_order_relaxed);feedbackEndEdit();
}
void MonomachineNovaAudioProcessor::feedbackSetFeedbackGain(float value){
    feedbackBeginEdit();feedbackControl.feedbackGain.store(juce::jlimit(0.0f,127.0f,value),std::memory_order_relaxed);feedbackEndEdit();
}
void MonomachineNovaAudioProcessor::feedbackSetReturnGain(float value){
    feedbackBeginEdit();feedbackControl.returnGain.store(juce::jlimit(0.0f,127.0f,value),std::memory_order_relaxed);feedbackEndEdit();
}
void MonomachineNovaAudioProcessor::feedbackSetDcControl(float value){
    feedbackBeginEdit();feedbackControl.dcControl.store(juce::jlimit(0.0f,127.0f,value),std::memory_order_relaxed);feedbackEndEdit();
}
void MonomachineNovaAudioProcessor::feedbackSetClipEnabled(bool enabled){
    feedbackBeginEdit();feedbackControl.clipEnabled.store(enabled,std::memory_order_relaxed);feedbackEndEdit();
}
void MonomachineNovaAudioProcessor::feedbackReset(){
    feedbackBeginEdit();
    feedbackControl.on.store(false,std::memory_order_relaxed);
    feedbackControl.sendZone.store(fxG_P1post,std::memory_order_relaxed);
    feedbackControl.returnZone.store(fxG_P1pre,std::memory_order_relaxed);
    feedbackControl.sendGain.store(64.0f,std::memory_order_relaxed);
    feedbackControl.feedbackGain.store(0.0f,std::memory_order_relaxed);
    feedbackControl.returnGain.store(64.0f,std::memory_order_relaxed);
    feedbackControl.dcControl.store(64.0f,std::memory_order_relaxed);
    feedbackControl.clipEnabled.store(true,std::memory_order_relaxed);
    feedbackEndEdit();
}
float MonomachineNovaAudioProcessor::feedbackDcHz(float raw) noexcept {
    // Preserve the established raw-64 ~= 1 Hz working point while extending
    // the upper half of the control to a genuinely useful 100 Hz high-pass.
    const float t=juce::jlimit(0.0f,127.0f,raw)/127.0f;
    constexpr float pivot=64.0f/127.0f;
    if(t<=pivot)return 0.1f*std::pow(10.0f,t/pivot); // 0.1 .. 1 Hz
    return std::pow(100.0f,(t-pivot)/(1.0f-pivot));  // 1 .. 100 Hz
}
void MonomachineNovaAudioProcessor::feedbackSyncToAudio() noexcept {
    const uint64_t before=feedbackEditSerial.load(std::memory_order_acquire);
    if(before==feedbackAudioSerial||(before&1u)!=0u)return;
    FeedbackState next;
    next.on=feedbackControl.on.load(std::memory_order_relaxed);
    next.sendZone=juce::jlimit(0,fxZonesCount-1,feedbackControl.sendZone.load(std::memory_order_relaxed));
    next.returnZone=juce::jlimit(0,fxZonesCount-1,feedbackControl.returnZone.load(std::memory_order_relaxed));
    next.sendGain=juce::jlimit(0.0f,127.0f,feedbackControl.sendGain.load(std::memory_order_relaxed));
    next.feedbackGain=juce::jlimit(0.0f,127.0f,feedbackControl.feedbackGain.load(std::memory_order_relaxed));
    next.returnGain=juce::jlimit(0.0f,127.0f,feedbackControl.returnGain.load(std::memory_order_relaxed));
    next.dcControl=juce::jlimit(0.0f,127.0f,feedbackControl.dcControl.load(std::memory_order_relaxed));
    next.clipEnabled=feedbackControl.clipEnabled.load(std::memory_order_relaxed);
    if(feedbackEditSerial.load(std::memory_order_acquire)!=before)return;

    const bool topologyChanged=feedbackAudio.on!=next.on||feedbackAudio.sendZone!=next.sendZone||feedbackAudio.returnZone!=next.returnZone;
    feedbackAudio=next;
    feedbackRoutes.fill({});
    if(next.on){
        auto& route=feedbackRoutes[0];route.on=true;route.sendZone=next.sendZone;route.returnZone=next.returnZone;
        route.sendGain=next.sendGain/64.0f;route.feedbackGain=next.feedbackGain/64.0f;route.returnGain=next.returnGain/64.0f;
        route.clipEnabled=next.clipEnabled;
        const float fc=feedbackDcHz(next.dcControl);
        route.hpPole=std::exp(-2.0f*nova::pi*fc/static_cast<float>(std::max(1.0,sr)));
    }
    if(topologyChanged)feedbackNetwork.clear();
    feedbackAudioSerial=before;
}
juce::ValueTree MonomachineNovaAudioProcessor::feedbackToTree() const {
    const auto state=feedbackState();juce::ValueTree tree("FEEDBACK");
    tree.setProperty("on",state.on?1:0,nullptr);tree.setProperty("send",state.sendZone,nullptr);tree.setProperty("return",state.returnZone,nullptr);
    tree.setProperty("sendGain",state.sendGain,nullptr);tree.setProperty("feedbackGain",state.feedbackGain,nullptr);tree.setProperty("returnGain",state.returnGain,nullptr);tree.setProperty("dc",state.dcControl,nullptr);tree.setProperty("clip",state.clipEnabled?1:0,nullptr);
    return tree;
}
void MonomachineNovaAudioProcessor::feedbackFromTree(const juce::ValueTree& tree){
    feedbackReset();
    if(!tree.isValid())return;
    feedbackBeginEdit();
    feedbackControl.on.store(static_cast<int>(tree.getProperty("on",0))!=0,std::memory_order_relaxed);
    feedbackControl.sendZone.store(juce::jlimit(0,fxZonesCount-1,static_cast<int>(tree.getProperty("send",fxG_P1post))),std::memory_order_relaxed);
    feedbackControl.returnZone.store(juce::jlimit(0,fxZonesCount-1,static_cast<int>(tree.getProperty("return",fxG_P1pre))),std::memory_order_relaxed);
    feedbackControl.sendGain.store(juce::jlimit(0.0f,127.0f,static_cast<float>(tree.getProperty("sendGain",64.0f))),std::memory_order_relaxed);
    feedbackControl.feedbackGain.store(juce::jlimit(0.0f,127.0f,static_cast<float>(tree.getProperty("feedbackGain",0.0f))),std::memory_order_relaxed);
    feedbackControl.returnGain.store(juce::jlimit(0.0f,127.0f,static_cast<float>(tree.getProperty("returnGain",64.0f))),std::memory_order_relaxed);
    feedbackControl.dcControl.store(juce::jlimit(0.0f,127.0f,static_cast<float>(tree.getProperty("dc",64.0f))),std::memory_order_relaxed);
    feedbackControl.clipEnabled.store(static_cast<int>(tree.getProperty("clip",1))!=0,std::memory_order_relaxed);
    feedbackEndEdit();
}

void MonomachineNovaAudioProcessor::syncArpNotes(){
    for(size_t note=0;note<128;++note){uint64_t latest=0;float vel=1;
        for(size_t c=0;c<16;++c)if(notes[c*128+note].order>latest){latest=notes[c*128+note].order;vel=notes[c*128+note].velocity;}
        bool held=latest>0;if(held&&!arpHeld[note]){bool hadHeld=false;for(bool h:arpHeld)hadHeld|=h;if(!hadHeld)plockStepCursor=plockStepWindowStartRuntime;arp.noteOn(static_cast<uint8_t>(note),static_cast<uint8_t>(std::round(vel*127)));}
        if(!held&&arpHeld[note])arp.noteOff(static_cast<uint8_t>(note));arpHeld[note]=held;}
}
void MonomachineNovaAudioProcessor::selectNotes(bool retrigger){
    uint64_t latest=0;int key=-1;for(size_t i=0;i<notes.size();++i)if(notes[i].order>latest){latest=notes[i].order;key=static_cast<int>(i);}
    syncArpNotes();bool changed=key!=currentKey;currentKey=key;
    if(global[ArpMode]>0.5f)return;if(!changed&&!retrigger)return;
    if(key<0)release();else trigger(key%128,notes[static_cast<size_t>(key)].velocity,key/128);
}
void MonomachineNovaAudioProcessor::midi(const juce::MidiMessage& m){
    size_t ch=static_cast<size_t>(juce::jlimit(0,15,m.getChannel()-1));
    if(m.isAllSoundOff()){panic();return;}
    if(m.isNoteOn()){auto& n=notes[ch*128+static_cast<size_t>(m.getNoteNumber())];n={true,m.getFloatVelocity(),++serial};selectNotes(true);}
    else if(m.isNoteOff()){auto& n=notes[ch*128+static_cast<size_t>(m.getNoteNumber())];n.down=false;if(!pedal[ch])n.order=0;selectNotes(false);}
    else if(m.isAllNotesOff()){for(size_t i=0;i<128;++i){auto& n=notes[ch*128+i];n.down=false;if(!pedal[ch])n.order=0;}selectNotes(false);if(global[ArpMode]==2){arp.reset(sr,bpm);arpHeld.fill(false);syncArpNotes();release();}}
    else if(m.isPitchWheel()){wheels[ch]=m.getPitchWheelValue();matrix.setPitchWheel((wheels[ch]-8192.0f)/8192.0f);}
    else if(m.isAftertouch()){aftertouch[ch]=m.getAfterTouchValue();matrix.setAftertouch(aftertouch[ch]/127.0f);}
    else if(m.isChannelPressure()){aftertouch[ch]=m.getChannelPressureValue();matrix.setAftertouch(aftertouch[ch]/127.0f);}
    else if(m.isController()){
        if(m.getControllerNumber()==64){pedal[ch]=m.getControllerValue()>=64;if(!pedal[ch]){for(size_t i=0;i<128;++i)if(!notes[ch*128+i].down)notes[ch*128+i].order=0;selectNotes(false);}}
        if(m.getControllerNumber()==1){modWheels[ch]=m.getControllerValue();ccX=static_cast<float>(m.getControllerValue());matrix.setModWheel(modWheels[ch]/127.0f);matrix.setParamX(static_cast<uint8_t>(ccX));}
        if(m.getControllerNumber()==11){ccY=static_cast<float>(m.getControllerValue());matrix.setParamY(static_cast<uint8_t>(ccY));}
    }
}
void MonomachineNovaAudioProcessor::render(juce::AudioBuffer<float>& buffer,int start,int length){
    if(length<=0)return;
    if(feedbackAudio.on){renderFeedback(buffer,start,length);return;}
    renderStandard(buffer,start,length);
}

void MonomachineNovaAudioProcessor::renderStandard(juce::AudioBuffer<float>& buffer,int start,int length){
    std::array<float,32> l{},r{},dryL{},dryR{},amp{};bool stereo=buffer.getNumChannels()>1;
    while(length>0){
        if(global[ArpMode]>0.5f){for(int k=0;k<3;++k){auto event=arp.poll();if(!event.triggered)break;if(event.isNoteOff){liveArpGate=0.0f;release();}else{liveStepVelocity=event.velocity/127.0f;liveStepTranspose=0.0f;liveArpGate=1.0f;trigger(event.note,event.velocity/127.0f,currentKey>=0?currentKey/128:soundingChannel);plockStep(plockStepForArpEvent(arp.stepEcho));}arpStepEcho.store(arp.stepEcho);arpPageEcho.store(arp.playingPage);}} // 1.6.42: страница для FOLLOW PLAY
        // A one-sample FB loop cannot be evaluated as a block delay. Its active
        // path intentionally renders samplewise; disabled FB keeps the original
        // 8-sample mini-block cadence bit-for-bit.
        int n=feedbackAudio.on?1:std::min(8,length);if(global[ArpMode]>0.5f)n=std::min(n,std::max(1,arp.samplesUntilEvent()));
        const int msegN=msegPageCount(); // 1.6.29: считаем только видимые страницы
        for(int mi=0;mi<msegN;++mi)msegValue[static_cast<size_t>(mi)]=mseg[mi].value(static_cast<float>(msegPhase[static_cast<size_t>(mi)]));
        for(int mi=msegN;mi<8;++mi)msegValue[static_cast<size_t>(mi)]=0.0f;
        matrix.setMsegOutputs(msegValue.data(),8);matrix.setModEnvOutputs(modEnvValue.data(),4);
        matrix.setPitchWheel((wheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]-8192.0f)/8192.0f);
        matrix.setModWheel(modWheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
        matrix.setAftertouch(aftertouch[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
        matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:selectedStepVelocity,liveStepTranspose!=0.0f?liveStepTranspose:selectedStepTranspose,liveArpGate>0.0f?liveArpGate:selectedStepHold);
        matrix.setArpRate(global[ArpGrid]/15.0f);
        std::array<float,56> before{},after{};std::copy(base.begin(),base.end(),before.begin());
        std::array<float,24> lfo2{};  // 1.8.0e: строки P2 LFO1-3 (164..187)
        std::array<float,24> lfo1b{}; // 1.8.0e: строки P1 LFO4-6 (188..211)
        std::array<float,24> lfo2b{}; // 1.8.0e: строки P2 LFO4-6 (212..235)
        for(size_t j=0;j<3;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),before.begin()+32+j*8);
        for(size_t j=3;j<6;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),lfo1b.begin()+(j-3)*8); // 1.8.0e: P1 LFO4-6
        for(size_t j=6;j<9;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),lfo2.begin()+(j-6)*8);  // P2 LFO1-3
        for(size_t j=9;j<12;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),lfo2b.begin()+(j-9)*8); // P2 LFO4-6
        matrix.setLfoOutputs(previousLfo[0],previousLfo[1],previousLfo[2]);matrix.setLfoOutputs6(previousLfo.data(),12); // 1.8.0e: 12 LFO
        std::array<float,8> msegMod{};std::array<float,3> arpMod{};std::array<float,8> msegRateMod{};std::array<float,2> arpWindowMod{},plockWindowMod{}; // appended Matrix targets 244..247
        std::array<float,64> routeExtra{}; // 1.7.7: +прибавки к глубинам 64 слотов (цели ROUTE)
        std::array<float,1> lfoFm{}; // 1.7.8: цель 131 LFO FM (октавы)
        {rndCount+=n;if(rndCount>=static_cast<int>(sr*0.08)){rndCount=0;rndTarget=rndSrc.nextFloat()*2.0f-1.0f;} // 1.7.10: RANDOM-источник -- новая цель каждые ~80 мс
            const float rk=1.0f-static_cast<float>(std::exp(-static_cast<double>(n)/(sr*0.03)));rndNow+=rk*(rndTarget-rndNow);matrix.setRandom(rndNow);} // сглаживание ~30 мс (раньше источник был всегда 0)
        std::array<float,32> before2{},after2{}; // 1.8.0: P2 (SYNT/AMP/FILT/EFFX по 8)
        {const int p2sel=juce::jlimit(0,static_cast<int>(p2FxMachines().size())-1,juce::roundToInt(p2MachineRaw?p2MachineRaw->load():0));
         const auto& def2=p2FxMachines()[static_cast<size_t>(p2sel)];
         for(int i=0;i<8;++i){auto* ptr=p2HandRaw[static_cast<size_t>(p2sel)][static_cast<size_t>(i)];before2[static_cast<size_t>(i)]=ptr?ptr->load():64.f;}
         for(int i=0;i<4;++i)before2[static_cast<size_t>(8+i)]=p2AmpRaw[static_cast<size_t>(i)]->load();
         for(size_t i=0;i<3;++i)before2[static_cast<size_t>(12+i)]=p2GainRaw[i]?p2GainRaw[i]->load():64.0f;
         before2[15]=p2MixRaw?p2MixRaw->load():127.0f; // P2 MIX is a real matrix/P-LOCK target (147), not a direct unsmoothed side path
         for(int p=0;p<2;++p)for(int i=0;i<8;++i)before2[static_cast<size_t>(16+p*8+i)]=p2PageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]->load();}
        matrix.evaluate(before,after,msegMod.data(),arpMod.data(),routeExtra.data(),lfoFm.data(),before2.data(),after2.data(),lfo2.data(),lfo1b.data(),lfo2b.data(),msegRateMod.data(),arpWindowMod.data(),plockWindowMod.data()); // appended ARP/P-LOCK window destinations 244..247
        applyArpAndPlockWindowModulation(arpWindowMod,plockWindowMod);
        std::copy(msegRateMod.begin(),msegRateMod.end(),msegRateSum.begin()); // 1.8.1: суммы MSEG RATE -- цикл фаз ниже читает прямо
        if(plockOn[131])lfoFm[0]=plockAbs[131]?plockNow[131]:lfoFm[0]+plockNow[131];
        lfoFmValue.store(lfoFm[0],std::memory_order_relaxed); // 1.7.8: страница-движок прочитает в следующем мини-блоке
        for(int mi=0;mi<8;++mi)msegValue[static_cast<size_t>(mi)]=std::clamp(msegValue[static_cast<size_t>(mi)]+msegMod[static_cast<size_t>(mi)]/127.0f,0.0f,1.0f);
        for(int t=0;t<56;++t)if(plockOn[static_cast<size_t>(t)])after[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(t)]?plockNow[static_cast<size_t>(t)]:after[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(t)],0.0f,127.0f); // 1.7.1: P-LOCK поверх маршрутов (UNI абсолют / BIP дельта)
        for(int t=0;t<32;++t)if(plockOn[static_cast<size_t>(132+t)])after2[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(132+t)]?plockNow[static_cast<size_t>(132+t)]:after2[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(132+t)],0.0f,127.0f); // 1.8.0b: P-LOCK на P2 (машины/AMP/FILT/EFFX/MIX)
        for(int t=0;t<24;++t)if(plockOn[static_cast<size_t>(164+t)])lfo2[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(164+t)]?plockNow[static_cast<size_t>(164+t)]:lfo2[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(164+t)],0.0f,127.0f); // 1.8.0e: P-LOCK на строки P2 LFO1-3
        for(int t=0;t<24;++t)if(plockOn[static_cast<size_t>(188+t)])lfo1b[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(188+t)]?plockNow[static_cast<size_t>(188+t)]:lfo1b[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(188+t)],0.0f,127.0f); // 1.8.0e: P-LOCK на строки P1 LFO4-6
        for(int t=0;t<24;++t)if(plockOn[static_cast<size_t>(212+t)])lfo2b[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(212+t)]?plockNow[static_cast<size_t>(212+t)]:lfo2b[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(212+t)],0.0f,127.0f); // 1.8.0e: P-LOCK на строки P2 LFO4-6
        for(int mi=0;mi<8;++mi)if(plockOn[static_cast<size_t>(56+mi)])msegValue[static_cast<size_t>(mi)]=std::clamp(plockAbs[static_cast<size_t>(56+mi)]?plockNow[static_cast<size_t>(56+mi)]:msegValue[static_cast<size_t>(mi)]+plockNow[static_cast<size_t>(56+mi)],0.0f,1.0f); // 1.7.1: P-LOCK на MSEG OUT
        arp.setMod(plockOn[64]?static_cast<double>(plockNow[64]):static_cast<double>(arpMod[static_cast<size_t>(0)])/63.0*2.0,plockOn[65]?plockNow[65]:arpMod[static_cast<size_t>(1)]); // 1.7.1: P-LOCK/маршруты на ARP RATE/GATE
        matrix.setMsegOutputs(msegValue.data(),8);matrix.setModEnvOutputs(modEnvValue.data(),4);
        float pitchMod=0;
        for(size_t j=0;j<12;++j){const auto& p=effectiveLfoParams[j];const float delta=previousLfo[j]*(p[7]-64);
            int page=juce::jlimit(0,nova::kLfoPageChoiceCount-1,static_cast<int>(p[0]));
            int dest=juce::jlimit(0,nova::kLfoDestinationCount-1,static_cast<int>(p[1]));
            if(lfoPageSolo[j]>0)page=juce::jlimit(0,nova::kLfoPageChoiceCount-1,lfoPageSolo[j]-1);
            else if(((lfoPageLocks[j]>>page)&1)!=0)page=lfoLastPage[j];else lfoLastPage[j]=page;
            if(lfoDestSolo[j]>0)dest=juce::jlimit(0,nova::kLfoDestinationCount-1,lfoDestSolo[j]-1);
            else if(((lfoDestLocks[j]>>dest)&1)!=0)dest=lfoLastDest[j];else lfoLastDest[j]=dest;
            const int target=nova::lfoDirectTarget(j>=6,page,dest);
            if(target==nova::kPitchMatrixTarget){const float ranges[]{1,2,3,5,7,12,24,36};pitchMod+=delta/64.0f*ranges[dest];continue;}
            // BBOX SLOT/RAND/CHRM are categorical sound-management controls.
            // They retain stable matrix IDs but are never driven by direct LFOs.
            const int activeId=activeMachine>=0?nova::machines()[static_cast<size_t>(activeMachine)].id:nova::machines()[static_cast<size_t>(machineIndex())].id;
            if(!nova::lfoDirectTargetIsContinuousForCurrentP1Machine(activeId,target))continue;
            if(target>=0&&target<56)after[static_cast<size_t>(target)]+=delta;
            else if(target>=132&&target<164)after2[static_cast<size_t>(target-132)]+=delta;
            else if(target>=164&&target<188)lfo2[static_cast<size_t>(target-164)]+=delta;
            else if(target>=188&&target<212)lfo1b[static_cast<size_t>(target-188)]+=delta;
            else if(target>=212&&target<236)lfo2b[static_cast<size_t>(target-212)]+=delta;
        }
        for(size_t j=0;j<12;++j){std::array<float,8> effective{}; // 1.8.0e: 12 LFO
            for(size_t k=0;k<8;++k){
                const float maximum=k==0?20.0f:k==1?7.0f:k==2?3.0f:k==3?10.0f:k==4?6.0f:127.0f; // 1.8.1 FIX: PAGE -- до 20 (чужая папка 11..20), было 7
                const float srcv=j<3?after[32+j*8+k]:j<6?lfo1b[(j-3)*8+k]:j<9?lfo2[(j-6)*8+k]:lfo2b[(j-9)*8+k];
                effective[k]=std::clamp(srcv,0.0f,maximum);}
            previousLfo[j]=(j<3?lfos[j]:j<6?lfos3[j-3]:j<9?lfos2[j-6]:lfos4[j-9]).process(effective,sr,bpm,n);effectiveLfoParams[j]=effective;
        }
                std::array<float,32> modulated{};
        const float smooth=static_cast<float>(1-std::exp(-static_cast<double>(n)/(sr*0.012)));
        for(size_t i=0;i<32;++i){smoothedParams[i]+=smooth*(std::clamp(after[i],0.0f,127.0f)-smoothedParams[i]);if(std::abs(after[i]-smoothedParams[i])<0.0001f)smoothedParams[i]=std::clamp(after[i],0.0f,127.0f);if(i<8&&nova::machines()[static_cast<size_t>(activeMachine)].id==7&&(i==2||i==3||i==6))smoothedParams[i]=after[i];modulated[i]=smoothedParams[i];}
        std::array<float,8> synthParams{};const auto& def=nova::machines()[static_cast<size_t>(activeMachine)];
        for(size_t i=0;i<8;++i)synthParams[i]=std::clamp(modulated[i],static_cast<float>(def.synthParams[i].minVal),static_cast<float>(def.synthParams[i].maxVal));
        float notePitch=pitch.getNextValue();if(n>1)pitch.skip(n-1);
        pitchMod+=(wheels[static_cast<size_t>(soundingChannel)]-8192)/8192.0f*2;
        machine.noiseDecaySeconds=nova::AmpEnvelope::tau(modulated[10],envelope.mode);
        if(plockOn[66])pitchMod+=plockNow[66];else pitchMod+=arpMod[static_cast<size_t>(2)]/63.0f*24.0f; // 1.7.5: цель PITCH -- полутона, +-24 на полной глубине; 1.7.10: P-LOCK на PITCH
        machine.setPitch(notePitch,pitchMod);machine.set(def.id,synthParams);chain.set(modulated);
        for(size_t i=0;i<4;++i)envelope.p[i]=modulated[8+i];
        if(isSynthVersion){std::fill_n(l.data(),n,0.0f);std::fill_n(r.data(),n,0.0f);if(envelope.stage!=0)machine.render(l.data(),r.data(),n,true);}
        else{std::copy_n(buffer.getReadPointer(0,start),n,l.data());std::copy_n(buffer.getReadPointer(stereo?1:0,start),n,r.data());for(int i=0;i<n;++i)dryDelay.process(l[static_cast<size_t>(i)],r[static_cast<size_t>(i)],machine.fxLatency(),dryL[static_cast<size_t>(i)],dryR[static_cast<size_t>(i)]);
            const bool chorusBypass=chorusSafe.load(std::memory_order_relaxed)&&machine.idOf()==15&&synthParams[3]<=0.0001f;
            if(chorusBypass){
                // Explicit DSP SAFE only: preserve the native core by default.
                // The delayed dry copy retains the chorus latency alignment.
                if(!p1ChorusSafeActive){machine.clear();p1ChorusSafeActive=true;}
                for(int i=0;i<n;++i){l[static_cast<size_t>(i)]=dryL[static_cast<size_t>(i)];r[static_cast<size_t>(i)]=dryR[static_cast<size_t>(i)];}
            }else{p1ChorusSafeActive=false;machine.render(l.data(),r.data(),n,false);}}
        std::array<float,32> velNow{},gbNow{}; // 1.8.0: P2-гейт разделяет velocity и FX MIDI/LATCH-бленд
        for(int i=0;i<n;++i){float a=envelope.tick(),vel=velocity.getNextValue();const float gb=midiGateBlend.getNextValue();for(size_t me=0;me<modEnvelopes.size();++me)modEnvValue[me]=modEnvelopes[me].tick();velNow[static_cast<size_t>(i)]=vel;gbNow[static_cast<size_t>(i)]=gb;amp[static_cast<size_t>(i)]=isSynthVersion?a*vel:1+(a*vel-1)*gb;}
        if(feedbackAudio.on)feedbackNetwork.beginSample(feedbackRoutes);
        // P1: physical g0..g6 points never move. The classic stage permutation
        // moves through them, so FX-slots stay at their chosen physical point.
        static constexpr int p1Gaps[classicStagesCount+1]={fxG_P1pre,fxG_EQ_FILT,fxG_FILT_DIST,fxG_DIST_ENV,fxG_ENV_VOLPAN,fxG_VOLPAN_SRR,fxG_P1_SRR_DELAY,fxG_P1post};
        auto runP1Classic=[&](int stage){switch(stage){
            case classicDist:chain.stageDIST(l.data(),r.data(),n);break;
            case classicSrr:chain.stageSRR(l.data(),r.data(),n);break;
            case classicFilt:chain.stageFILT(l.data(),r.data(),n);break;
            case classicEq:chain.stageEQ(l.data(),r.data(),n);break;
            case classicEnv:chain.stageENV(l.data(),r.data(),amp.data(),n);break;
            case classicVolPan:chain.stageVOLPAN(l.data(),r.data(),n);break;
            case classicDelay:chain.stageDELAY(l.data(),r.data(),n);break;
            default:break;}};
        fxZone(p1Gaps[0],l.data(),r.data(),n);
        for(int position=0;position<classicStagesCount;++position){const int stage=classicRouteAudio.p1[static_cast<size_t>(position)];if(classicRouteAudio.p1Enabled[static_cast<size_t>(stage)])runP1Classic(stage);fxZone(p1Gaps[position+1],l.data(),r.data(),n);}
        { // ===== 1.8.0: P2 -- полноценный FX-движок (вход = выход P1) =====
            const int p2sel=juce::jlimit(0,static_cast<int>(p2FxMachines().size())-1,juce::roundToInt(p2MachineRaw?p2MachineRaw->load():0));
            const auto& def2=p2FxMachines()[static_cast<size_t>(p2sel)];
            std::array<float,32> v2{}; // P2 gets the same anti-zipper policy as P1.
            const float smooth2=static_cast<float>(1-std::exp(-static_cast<double>(n)/(sr*0.012)));
            for(int i=0;i<32;++i){const float target=std::clamp(after2[static_cast<size_t>(i)],0.0f,127.0f);
                if(!p2SmoothingPrimed)smoothedParams2[static_cast<size_t>(i)]=target;
                else {smoothedParams2[static_cast<size_t>(i)]+=smooth2*(target-smoothedParams2[static_cast<size_t>(i)]);if(std::abs(target-smoothedParams2[static_cast<size_t>(i)])<0.0001f)smoothedParams2[static_cast<size_t>(i)]=target;}
                v2[static_cast<size_t>(i)]=smoothedParams2[static_cast<size_t>(i)];}
            if(!p2SmoothingPrimed)p2WetMix.setCurrentAndTargetValue(v2[15]/127.0f);
            else p2WetMix.setTargetValue(v2[15]/127.0f);
            p2SmoothingPrimed=true;
            for(int i=0;i<4;++i)envelope2.p[static_cast<size_t>(i)]=v2[static_cast<size_t>(8+i)];
            std::array<float,8> h2{};
            for(int i=0;i<8;++i)h2[static_cast<size_t>(i)]=std::clamp(v2[static_cast<size_t>(i)],static_cast<float>(def2.synthParams[static_cast<size_t>(i)].minVal),static_cast<float>(def2.synthParams[static_cast<size_t>(i)].maxVal));
            machine2.set(def2.id,h2);machine2.setModes(monomachine::dspModeMnm,monomachine::dspModeMnm);machine2.setPitch(60.0f,0.0f); // 1.8.0: THRU = сухой режим (id12), работает как в pre-FX
            machine2.noiseDecaySeconds=nova::AmpEnvelope::tau(v2[10],envelope2.mode);
            std::array<float,32> c2{}; // для chain2: [12]=DIST, [13]=VOL, [14]=PAN (зеркало P1), [16..23]=FILT, [24..31]=FX
            for(int i=0;i<16;++i)c2[static_cast<size_t>(i)]=64.0f;
            for(int i=0;i<4;++i)c2[static_cast<size_t>(12+i)]=std::clamp(v2[static_cast<size_t>(12+i)],0.0f,127.0f); // 1.8.0 FIX: DIST/VOL/PAN с P2-AMP-страницы (было намертво 64/127/64)
            for(int i=0;i<8;++i){c2[static_cast<size_t>(16+i)]=std::clamp(v2[static_cast<size_t>(16+i)],0.0f,127.0f);c2[static_cast<size_t>(24+i)]=std::clamp(v2[static_cast<size_t>(24+i)],0.0f,127.0f);}
            chain2.set(c2);
            chain2.setModes(p2ModeRaw[0]?juce::jlimit(0,monomachine::dspSectionModeCount(monomachine::DspFilter)-1,juce::roundToInt(p2ModeRaw[0]->load())):0,p2ModeRaw[1]?juce::jlimit(0,monomachine::dspSectionModeCount(monomachine::DspDist)-1,juce::roundToInt(p2ModeRaw[1]->load())):0,p2ModeRaw[2]?juce::jlimit(0,monomachine::dspSectionModeCount(monomachine::DspDelay)-1,juce::roundToInt(p2ModeRaw[2]->load())):0,0);
            chain2.setHybridTestModes(hybridP2Raw[0]?juce::roundToInt(hybridP2Raw[0]->load()):0,hybridP2Raw[1]?juce::roundToInt(hybridP2Raw[1]->load()):0,hybridP2Raw[2]?juce::roundToInt(hybridP2Raw[2]->load()):0);
            chain2.setRepitch(repitchRaw?repitchRaw->load():2.0f,repitchSmoothRaw?repitchSmoothRaw->load():0.0f);
            chain2.setPpMode(ppModeRaw?juce::roundToInt(ppModeRaw->load()):0);
            // P2 is an insert + group DRY/WET. DSP SAFE never hides the
            // physical g7 bridge or the classic P2 chain; it only substitutes
            // the latency-aligned dry signal for a CHORUS machine at MIX=0.
            if(machine2.idOf()==0){p2ChorusSafeActive=false;/* GND = P2 выключен -- прозрачный проход */}
            else{
                fxZone(fxG_P2pre,l.data(),r.data(),n); // g7 is shared: it affects both dry and wet branches
                std::array<float,32> d2L{},d2R{};for(int i=0;i<n;++i)dryDelay2.process(l[static_cast<size_t>(i)],r[static_cast<size_t>(i)],machine2.fxLatency(),d2L[static_cast<size_t>(i)],d2R[static_cast<size_t>(i)]); // dry = post-g7 P1, delay-aligned to a chorus machine
                const bool chorusBypass=chorusSafe.load(std::memory_order_relaxed)&&machine2.idOf()==15&&h2[3]<=0.0001f;
                if(chorusBypass){
                    if(!p2ChorusSafeActive){machine2.clear();p2ChorusSafeActive=true;}
                    for(int i=0;i<n;++i){l[static_cast<size_t>(i)]=d2L[static_cast<size_t>(i)];r[static_cast<size_t>(i)]=d2R[static_cast<size_t>(i)];}
                }else{p2ChorusSafeActive=false;machine2.render(l.data(),r.data(),n,false);} // native FX slot P2, in-place like FX version
                // P2 uses its own independent permutation. Its g8/g10..g14/g9
                // points stay in place, including the wet-only boundary at g8.
                fxZone(fxG_P2MACH_CHAIN,l.data(),r.data(),n);
              for(int i=0;i<n;++i){const float a2=envelope2.stage!=0?envelope2.tick():0.0f;
                  // P2 is serial after P1 in Synth: P1 ENV has already applied
                  // MIDI velocity. Applying vel a second time made P2 MIX=127
                  // quieter by another velocity factor and encouraged a hot,
                  // clipped compensation gain. P2 AMP still supplies its own
                  // envelope; only the duplicate velocity multiplication is gone.
                  amp2[static_cast<size_t>(i)]=isSynthVersion?a2:1+(a2*velNow[static_cast<size_t>(i)]-1)*gbNow[static_cast<size_t>(i)];} // In FX LATCH, P2 remains unity.
              static constexpr int p2Gaps[classicStagesCount]={fxG_P2MACH_CHAIN,fxG_P2_EQ_FILT,fxG_P2_FILT_DIST,fxG_P2_DIST_ENV,fxG_P2_ENV_VOLPAN,fxG_P2_VOLPAN_SRR,fxG_P2_SRR_DELAY};
              auto runP2Classic=[&](int stage){switch(stage){
                  case classicDist:chain2.stageDIST(l.data(),r.data(),n);break;
                  case classicSrr:chain2.stageSRR(l.data(),r.data(),n);break;
                  case classicFilt:chain2.stageFILT(l.data(),r.data(),n);break;
                  case classicEq:chain2.stageEQ(l.data(),r.data(),n);break;
                  case classicEnv:chain2.stageENV(l.data(),r.data(),amp2.data(),n);break;
                  case classicVolPan:chain2.stageVOLPAN(l.data(),r.data(),n);break;
                  case classicDelay:chain2.stageDELAY(l.data(),r.data(),n);break;
                  default:break;}};
              for(int position=0;position<classicStagesCount;++position){const int stage=classicRouteAudio.p2[static_cast<size_t>(position)];if(classicRouteAudio.p2Enabled[static_cast<size_t>(stage)])runP2Classic(stage);if(position+1<classicStagesCount)fxZone(p2Gaps[position+1],l.data(),r.data(),n);}
              for(int i=0;i<n;++i){const size_t x=static_cast<size_t>(i);const float mix2=p2WetMix.getNextValue();l[x]=d2L[x]*(1.0f-mix2)+l[x]*mix2;r[x]=d2R[x]*(1.0f-mix2)+r[x]*mix2;} // DRY/WET: 0 = clean P1, 127 = full P2; audio-rate ramp avoids zipper clicks
            }
        }
        fxZone(fxG_P2post,l.data(),r.data(),n); // 1.8.4: g9 -- после P2, до OUT
        if(feedbackAudio.on)feedbackNetwork.endSample();
        for(int i=0;i<n;++i){size_t x=static_cast<size_t>(i);float gain=level.getNextValue()/(isSynthVersion?1.0f:100.0f/127.0f),wet=mix.getNextValue();
            const float dryGate=isSynthVersion?1.0f:amp[x];
            float a=gain*(l[x]*wet+dryL[x]*dryGate*(1-wet)),b=gain*(r[x]*wet+dryR[x]*dryGate*(1-wet));
            if(!std::isfinite(a))a=0;if(!std::isfinite(b))b=0;
            buffer.setSample(0,start+i,stereo?a:(a+b)*0.5f);if(stereo)buffer.setSample(1,start+i,b);blockPeak=std::max(blockPeak,std::max(std::abs(a),std::abs(b)));}
        if(global[ArpMode]>0.5f)arp.advance(n); // 1.6.25: RETRIG SYNC убран (делал не то)
        const int msegBase[8]={MsegRate,Mseg2Rate,Mseg3Rate,Mseg4Rate,Mseg5Rate,Mseg6Rate,Mseg7Rate,Mseg8Rate}; // 1.6.29: страницы идут пачками по 3 параметра
        { bool hostPlaying=false; if(auto* ph=getPlayHead())if(auto pos=ph->getPosition())hostPlaying=pos->getIsPlaying(); // 1.7.7: HOST -- старт плей хоста перезапускает фазы MSEG (клок совпадает)
        if(hostPlaying&&!hostWasPlaying)for(int mi=0;mi<8;++mi)msegPhase[static_cast<size_t>(mi)]=0.0;
        hostWasPlaying=hostPlaying; }
        for(int mi=0;mi<msegN;++mi){const int b=msegBase[static_cast<size_t>(mi)]; // 1.6.29: rate/sync/loop каждой страницы лежат подряд
            // 1.7.8: RATE -- семья шкалы (как в Vital TempoSelector): TEMPO/TRIPLET/DOTTED -- тактовые доли 32bar..1/256+FAST, TIME -- 1ms..60s, HZ -- 0.001..20kHz
            static const double cpb[15]={1.0/128,1.0/64,1.0/32,1.0/16,1.0/8,1.0/4,1.0/2,1.0,2.0,4.0,8.0,16.0,32.0,64.0,128.0}; // циклов на бит (FAST = 128)
            const float rv=global[static_cast<size_t>(b)];
            auto* rmRaw=globalRaw[static_cast<size_t>(MsegRmode+4*mi)];auto* kyRaw=globalRaw[static_cast<size_t>(MsegKey+4*mi)];auto* lpRaw=globalRaw[static_cast<size_t>(MsegLpoint+4*mi)];
            const int rmode=rmRaw==nullptr?0:juce::jlimit(0,4,juce::roundToInt(rmRaw->load()));
            double rate=0.001;
            if(rmode<=2){const int idx=juce::jlimit(0,14,juce::roundToInt(rv*14.0f));const double mult=rmode==1?1.5:(rmode==2?1.0/1.5:1.0);rate=bpm/60.0*cpb[static_cast<size_t>(idx)]*mult;}
            else if(rmode==3)rate=1.0/std::max(0.001,0.001*std::pow(60000.0,rv)); // 1.7.9 FIX TIME: период действительно 1 ms..60 s (было 1 s..60 s)
            else rate=0.001*std::pow(10.0,4.301*rv); // HZ: 0.001 -> 0.01 -> 0.1 -> ... -> 20kHz
            if(kyRaw!=nullptr&&kyRaw->load()>0.5f&&currentKey>=0)rate*=std::pow(2.0,(static_cast<double>(currentKey%128)-60.0)/12.0); // 1.7.8: KEYTRACK -- октава вверх = x2
            rate*=std::pow(2.0,juce::jlimit(-1.0,1.0,static_cast<double>(lfoFmValue.load(std::memory_order_relaxed)))*13.3); // 1.7.8: LFO FM из матрицы (цель 131, до ~20 kHz)
            if(plockOn[static_cast<size_t>(236+mi)])rate*=std::pow(2.0,juce::jlimit(-2.0,2.0,static_cast<double>(plockAbs[static_cast<size_t>(236+mi)]?plockNow[static_cast<size_t>(236+mi)]-64.0f:plockNow[static_cast<size_t>(236+mi)])/32.0)); // 1.8.1: P-LOCK MSEG RATE (шаг 64 = +-2 октавы; ABS: центр 64)
            rate=std::max(0.0005,rate);
            msegPhase[static_cast<size_t>(mi)]+=rate*static_cast<double>(n)/sr;
            if(msegPhase[static_cast<size_t>(mi)]>=1.0){const int mmode=[&]{auto* rt=globalRaw[static_cast<size_t>(MsegRetrig+4*mi)];return rt==nullptr?0:juce::jlimit(0,5,juce::roundToInt(rt->load()));}(); // 1.7.8
                const double lp=lpRaw==nullptr?0.5:juce::jlimit(0.0,1.0,static_cast<double>(lpRaw->load()));const bool held=currentKey>=0;
                if(mmode==1)msegPhase[static_cast<size_t>(mi)]=1.0; // VITAL (= старый ENVELOPE): один проход и держит последнюю точку
                else if(mmode==3)msegPhase[static_cast<size_t>(mi)]=held?std::min(msegPhase[static_cast<size_t>(mi)],lp):1.0; // SUSTAIN: держит на LOOP POINT, после note-off доигрывает до конца
                else if(mmode==4)msegPhase[static_cast<size_t>(mi)]=msegPhase[static_cast<size_t>(mi)]-1.0+lp; // LOOP POINT: после первого прохода крутит lp..1
                else if(mmode==5)msegPhase[static_cast<size_t>(mi)]=held?std::fmod(msegPhase[static_cast<size_t>(mi)],lp):(msegPhase[static_cast<size_t>(mi)]>=1.0?1.0:msegPhase[static_cast<size_t>(mi)]); // LOOP HOLD: держит ноту -- крутит 0..lp, отпустил -- доигрывает хвост
                else if(global[static_cast<size_t>(b+2)]>0.5f)msegPhase[static_cast<size_t>(mi)]=std::fmod(msegPhase[static_cast<size_t>(mi)],1.0);else msegPhase[static_cast<size_t>(mi)]=1.0;}}
        for(int mi=0;mi<8;++mi)msegPhaseUi[static_cast<size_t>(mi)].store(msegPhase[static_cast<size_t>(mi)],std::memory_order_relaxed); // 1.7.6: PLAY FOLLOWER на странице MSEG
        start+=n;length-=n;
    }
}

// FB-only renderer. The audio graph remains one sample at a time so RETURN->SEND
// timing is exactly one sample. Only the control/modulation refresh uses the
// established 8-sample cadence; rendering the entire control plane for every
// feedback sample was the source of the large CPU spike.
void MonomachineNovaAudioProcessor::renderFeedback(juce::AudioBuffer<float>& buffer,int start,int length){
    std::array<float,32> l{},r{},dryL{},dryR{},amp{},amp2{};
    std::array<float,8> synthParams{},h2{};
    std::array<float,32> v2{};
    const bool stereo=buffer.getNumChannels()>1;
    const int msegN=msegPageCount();
    int controlRemaining=0,controlSpan=0;

    while(length>0){
        if(controlRemaining==0){
            if(global[ArpMode]>0.5f){for(int k=0;k<3;++k){auto event=arp.poll();if(!event.triggered)break;if(event.isNoteOff){liveArpGate=0.0f;release();}else{liveStepVelocity=event.velocity/127.0f;liveStepTranspose=0.0f;liveArpGate=1.0f;trigger(event.note,event.velocity/127.0f,currentKey>=0?currentKey/128:soundingChannel);plockStep(plockStepForArpEvent(arp.stepEcho));}arpStepEcho.store(arp.stepEcho);arpPageEcho.store(arp.playingPage);}}
            controlSpan=std::min(8,length);
            if(global[ArpMode]>0.5f)controlSpan=std::min(controlSpan,std::max(1,arp.samplesUntilEvent()));
            controlRemaining=controlSpan;

            for(int mi=0;mi<msegN;++mi)msegValue[static_cast<size_t>(mi)]=mseg[mi].value(static_cast<float>(msegPhase[static_cast<size_t>(mi)]));
            for(int mi=msegN;mi<8;++mi)msegValue[static_cast<size_t>(mi)]=0.0f;
            matrix.setMsegOutputs(msegValue.data(),8);matrix.setModEnvOutputs(modEnvValue.data(),4);
            matrix.setPitchWheel((wheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]-8192.0f)/8192.0f);
            matrix.setModWheel(modWheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
            matrix.setAftertouch(aftertouch[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
            matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:selectedStepVelocity,liveStepTranspose!=0.0f?liveStepTranspose:selectedStepTranspose,liveArpGate>0.0f?liveArpGate:selectedStepHold);
            matrix.setArpRate(global[ArpGrid]/15.0f);
            std::array<float,56> before{},after{};std::copy(base.begin(),base.end(),before.begin());
            std::array<float,24> lfo2{},lfo1b{},lfo2b{};
            for(size_t j=0;j<3;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),before.begin()+32+j*8);
            for(size_t j=3;j<6;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),lfo1b.begin()+(j-3)*8);
            for(size_t j=6;j<9;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),lfo2.begin()+(j-6)*8);
            for(size_t j=9;j<12;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),lfo2b.begin()+(j-9)*8);
            matrix.setLfoOutputs(previousLfo[0],previousLfo[1],previousLfo[2]);matrix.setLfoOutputs6(previousLfo.data(),12);
            std::array<float,8> msegMod{},msegRateMod{};std::array<float,3> arpMod{};std::array<float,2> arpWindowMod{},plockWindowMod{};std::array<float,64> routeExtra{};std::array<float,1> lfoFm{};
            {rndCount+=controlSpan;if(rndCount>=static_cast<int>(sr*0.08)){rndCount=0;rndTarget=rndSrc.nextFloat()*2.0f-1.0f;}
             const float rk=1.0f-static_cast<float>(std::exp(-static_cast<double>(controlSpan)/(sr*0.03)));rndNow+=rk*(rndTarget-rndNow);matrix.setRandom(rndNow);}
            std::array<float,32> before2{},after2{};
            {const int p2sel=juce::jlimit(0,static_cast<int>(p2FxMachines().size())-1,juce::roundToInt(p2MachineRaw?p2MachineRaw->load():0));
             for(int i=0;i<8;++i){auto* ptr=p2HandRaw[static_cast<size_t>(p2sel)][static_cast<size_t>(i)];before2[static_cast<size_t>(i)]=ptr?ptr->load():64.f;}
             for(int i=0;i<4;++i)before2[static_cast<size_t>(8+i)]=p2AmpRaw[static_cast<size_t>(i)]->load();
             for(size_t i=0;i<3;++i)before2[static_cast<size_t>(12+i)]=p2GainRaw[i]?p2GainRaw[i]->load():64.0f;
             before2[15]=p2MixRaw?p2MixRaw->load():127.0f;
             for(int p=0;p<2;++p)for(int i=0;i<8;++i)before2[static_cast<size_t>(16+p*8+i)]=p2PageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]->load();}
            matrix.evaluate(before,after,msegMod.data(),arpMod.data(),routeExtra.data(),lfoFm.data(),before2.data(),after2.data(),lfo2.data(),lfo1b.data(),lfo2b.data(),msegRateMod.data(),arpWindowMod.data(),plockWindowMod.data());
            applyArpAndPlockWindowModulation(arpWindowMod,plockWindowMod);
            std::copy(msegRateMod.begin(),msegRateMod.end(),msegRateSum.begin());if(plockOn[131])lfoFm[0]=plockAbs[131]?plockNow[131]:lfoFm[0]+plockNow[131];
        lfoFmValue.store(lfoFm[0],std::memory_order_relaxed);
            for(int mi=0;mi<8;++mi)msegValue[static_cast<size_t>(mi)]=std::clamp(msegValue[static_cast<size_t>(mi)]+msegMod[static_cast<size_t>(mi)]/127.0f,0.0f,1.0f);
            for(int t=0;t<56;++t)if(plockOn[static_cast<size_t>(t)])after[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(t)]?plockNow[static_cast<size_t>(t)]:after[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(t)],0.0f,127.0f);
            for(int t=0;t<32;++t)if(plockOn[static_cast<size_t>(132+t)])after2[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(132+t)]?plockNow[static_cast<size_t>(132+t)]:after2[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(132+t)],0.0f,127.0f);
            for(int t=0;t<24;++t)if(plockOn[static_cast<size_t>(164+t)])lfo2[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(164+t)]?plockNow[static_cast<size_t>(164+t)]:lfo2[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(164+t)],0.0f,127.0f);
            for(int t=0;t<24;++t)if(plockOn[static_cast<size_t>(188+t)])lfo1b[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(188+t)]?plockNow[static_cast<size_t>(188+t)]:lfo1b[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(188+t)],0.0f,127.0f);
            for(int t=0;t<24;++t)if(plockOn[static_cast<size_t>(212+t)])lfo2b[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(212+t)]?plockNow[static_cast<size_t>(212+t)]:lfo2b[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(212+t)],0.0f,127.0f);
            for(int mi=0;mi<8;++mi)if(plockOn[static_cast<size_t>(56+mi)])msegValue[static_cast<size_t>(mi)]=std::clamp(plockAbs[static_cast<size_t>(56+mi)]?plockNow[static_cast<size_t>(56+mi)]:msegValue[static_cast<size_t>(mi)]+plockNow[static_cast<size_t>(56+mi)],0.0f,1.0f);
            arp.setMod(plockOn[64]?static_cast<double>(plockNow[64]):static_cast<double>(arpMod[static_cast<size_t>(0)])/63.0*2.0,plockOn[65]?plockNow[65]:arpMod[static_cast<size_t>(1)]);
            matrix.setMsegOutputs(msegValue.data(),8);matrix.setModEnvOutputs(modEnvValue.data(),4);
            float pitchMod=0.0f;
            for(size_t j=0;j<12;++j){const auto& p=effectiveLfoParams[j];const float delta=previousLfo[j]*(p[7]-64);
            int page=juce::jlimit(0,nova::kLfoPageChoiceCount-1,static_cast<int>(p[0]));
            int dest=juce::jlimit(0,nova::kLfoDestinationCount-1,static_cast<int>(p[1]));
            if(lfoPageSolo[j]>0)page=juce::jlimit(0,nova::kLfoPageChoiceCount-1,lfoPageSolo[j]-1);
            else if(((lfoPageLocks[j]>>page)&1)!=0)page=lfoLastPage[j];else lfoLastPage[j]=page;
            if(lfoDestSolo[j]>0)dest=juce::jlimit(0,nova::kLfoDestinationCount-1,lfoDestSolo[j]-1);
            else if(((lfoDestLocks[j]>>dest)&1)!=0)dest=lfoLastDest[j];else lfoLastDest[j]=dest;
            const int target=nova::lfoDirectTarget(j>=6,page,dest);
            if(target==nova::kPitchMatrixTarget){const float ranges[]{1,2,3,5,7,12,24,36};pitchMod+=delta/64.0f*ranges[dest];continue;}
            // BBOX SLOT/RAND/CHRM are categorical sound-management controls.
            // They retain stable matrix IDs but are never driven by direct LFOs.
            const int activeId=activeMachine>=0?nova::machines()[static_cast<size_t>(activeMachine)].id:nova::machines()[static_cast<size_t>(machineIndex())].id;
            if(!nova::lfoDirectTargetIsContinuousForCurrentP1Machine(activeId,target))continue;
            if(target>=0&&target<56)after[static_cast<size_t>(target)]+=delta;
            else if(target>=132&&target<164)after2[static_cast<size_t>(target-132)]+=delta;
            else if(target>=164&&target<188)lfo2[static_cast<size_t>(target-164)]+=delta;
            else if(target>=188&&target<212)lfo1b[static_cast<size_t>(target-188)]+=delta;
            else if(target>=212&&target<236)lfo2b[static_cast<size_t>(target-212)]+=delta;
        }
        for(size_t j=0;j<12;++j){std::array<float,8> effective{};for(size_t k=0;k<8;++k){const float maximum=k==0?20.0f:k==1?7.0f:k==2?3.0f:k==3?10.0f:k==4?6.0f:127.0f;const float srcv=j<3?after[32+j*8+k]:j<6?lfo1b[(j-3)*8+k]:j<9?lfo2[(j-6)*8+k]:lfo2b[(j-9)*8+k];effective[k]=std::clamp(srcv,0.0f,maximum);}previousLfo[j]=(j<3?lfos[j]:j<6?lfos3[j-3]:j<9?lfos2[j-6]:lfos4[j-9]).process(effective,sr,bpm,controlSpan);effectiveLfoParams[j]=effective;}
            std::array<float,32> modulated{};
            const float smooth=static_cast<float>(1-std::exp(-static_cast<double>(controlSpan)/(sr*0.012)));
            for(size_t i=0;i<32;++i){smoothedParams[i]+=smooth*(std::clamp(after[i],0.0f,127.0f)-smoothedParams[i]);if(std::abs(after[i]-smoothedParams[i])<0.0001f)smoothedParams[i]=std::clamp(after[i],0.0f,127.0f);if(i<8&&nova::machines()[static_cast<size_t>(activeMachine)].id==7&&(i==2||i==3||i==6))smoothedParams[i]=after[i];modulated[i]=smoothedParams[i];}
            const auto& def=nova::machines()[static_cast<size_t>(activeMachine)];
            for(size_t i=0;i<8;++i)synthParams[i]=std::clamp(modulated[i],static_cast<float>(def.synthParams[i].minVal),static_cast<float>(def.synthParams[i].maxVal));
            float notePitch=pitch.getNextValue();if(controlSpan>1)pitch.skip(controlSpan-1);
            pitchMod+=(wheels[static_cast<size_t>(soundingChannel)]-8192)/8192.0f*2;
            machine.noiseDecaySeconds=nova::AmpEnvelope::tau(modulated[10],envelope.mode);
            if(plockOn[66])pitchMod+=plockNow[66];else pitchMod+=arpMod[static_cast<size_t>(2)]/63.0f*24.0f;
            machine.setPitch(notePitch,pitchMod);machine.set(def.id,synthParams);chain.set(modulated);
            for(size_t i=0;i<4;++i)envelope.p[i]=modulated[8+i];

            const int p2sel=juce::jlimit(0,static_cast<int>(p2FxMachines().size())-1,juce::roundToInt(p2MachineRaw?p2MachineRaw->load():0));
            const auto& def2=p2FxMachines()[static_cast<size_t>(p2sel)];
            const float smooth2=static_cast<float>(1-std::exp(-static_cast<double>(controlSpan)/(sr*0.012)));
            for(int i=0;i<32;++i){const float target=std::clamp(after2[static_cast<size_t>(i)],0.0f,127.0f);if(!p2SmoothingPrimed)smoothedParams2[static_cast<size_t>(i)]=target;else {smoothedParams2[static_cast<size_t>(i)]+=smooth2*(target-smoothedParams2[static_cast<size_t>(i)]);if(std::abs(target-smoothedParams2[static_cast<size_t>(i)])<0.0001f)smoothedParams2[static_cast<size_t>(i)]=target;}v2[static_cast<size_t>(i)]=smoothedParams2[static_cast<size_t>(i)];}
            if(!p2SmoothingPrimed)p2WetMix.setCurrentAndTargetValue(v2[15]/127.0f);else p2WetMix.setTargetValue(v2[15]/127.0f);p2SmoothingPrimed=true;
            for(int i=0;i<4;++i)envelope2.p[static_cast<size_t>(i)]=v2[static_cast<size_t>(8+i)];
            for(int i=0;i<8;++i)h2[static_cast<size_t>(i)]=std::clamp(v2[static_cast<size_t>(i)],static_cast<float>(def2.synthParams[static_cast<size_t>(i)].minVal),static_cast<float>(def2.synthParams[static_cast<size_t>(i)].maxVal));
            machine2.set(def2.id,h2);machine2.setModes(monomachine::dspModeMnm,monomachine::dspModeMnm);machine2.setPitch(60.0f,0.0f);machine2.noiseDecaySeconds=nova::AmpEnvelope::tau(v2[10],envelope2.mode);
            std::array<float,32> c2{};for(int i=0;i<16;++i)c2[static_cast<size_t>(i)]=64.0f;
            for(int i=0;i<4;++i)c2[static_cast<size_t>(12+i)]=std::clamp(v2[static_cast<size_t>(12+i)],0.0f,127.0f);
            for(int i=0;i<8;++i){c2[static_cast<size_t>(16+i)]=std::clamp(v2[static_cast<size_t>(16+i)],0.0f,127.0f);c2[static_cast<size_t>(24+i)]=std::clamp(v2[static_cast<size_t>(24+i)],0.0f,127.0f);}
            chain2.set(c2);chain2.setModes(p2ModeRaw[0]?juce::jlimit(0,monomachine::dspSectionModeCount(monomachine::DspFilter)-1,juce::roundToInt(p2ModeRaw[0]->load())):0,p2ModeRaw[1]?juce::jlimit(0,monomachine::dspSectionModeCount(monomachine::DspDist)-1,juce::roundToInt(p2ModeRaw[1]->load())):0,p2ModeRaw[2]?juce::jlimit(0,monomachine::dspSectionModeCount(monomachine::DspDelay)-1,juce::roundToInt(p2ModeRaw[2]->load())):0,0);chain2.setHybridTestModes(hybridP2Raw[0]?juce::roundToInt(hybridP2Raw[0]->load()):0,hybridP2Raw[1]?juce::roundToInt(hybridP2Raw[1]->load()):0,hybridP2Raw[2]?juce::roundToInt(hybridP2Raw[2]->load()):0);chain2.setRepitch(repitchRaw?repitchRaw->load():2.0f,repitchSmoothRaw?repitchSmoothRaw->load():0.0f);chain2.setPpMode(ppModeRaw?juce::roundToInt(ppModeRaw->load()):0);
        }

        constexpr int n=1;
        if(isSynthVersion){l[0]=r[0]=0.0f;if(envelope.stage!=0)machine.render(l.data(),r.data(),n,true);}
        else{
            l[0]=buffer.getReadPointer(0,start)[0];r[0]=buffer.getReadPointer(stereo?1:0,start)[0];
            dryDelay.process(l[0],r[0],machine.fxLatency(),dryL[0],dryR[0]);
            const bool chorusBypass=chorusSafe.load(std::memory_order_relaxed)&&machine.idOf()==15&&synthParams[3]<=0.0001f;
            if(chorusBypass){if(!p1ChorusSafeActive){machine.clear();p1ChorusSafeActive=true;}l[0]=dryL[0];r[0]=dryR[0];}
            else{p1ChorusSafeActive=false;machine.render(l.data(),r.data(),n,false);}
        }
        const float a=envelope.tick(),vel=velocity.getNextValue(),gb=midiGateBlend.getNextValue();for(size_t me=0;me<modEnvelopes.size();++me)modEnvValue[me]=modEnvelopes[me].tick();
        const float dryGate=isSynthVersion?1.0f:(1+(a*vel-1)*gb);amp[0]=dryGate;
        feedbackNetwork.beginSample(feedbackRoutes);
        static constexpr int p1Gaps[classicStagesCount+1]={fxG_P1pre,fxG_EQ_FILT,fxG_FILT_DIST,fxG_DIST_ENV,fxG_ENV_VOLPAN,fxG_VOLPAN_SRR,fxG_P1_SRR_DELAY,fxG_P1post};
        auto runP1Classic=[&](int stage){switch(stage){case classicDist:chain.stageDIST(l.data(),r.data(),n);break;case classicSrr:chain.stageSRR(l.data(),r.data(),n);break;case classicFilt:chain.stageFILT(l.data(),r.data(),n);break;case classicEq:chain.stageEQ(l.data(),r.data(),n);break;case classicEnv:chain.stageENV(l.data(),r.data(),amp.data(),n);break;case classicVolPan:chain.stageVOLPAN(l.data(),r.data(),n);break;case classicDelay:chain.stageDELAY(l.data(),r.data(),n);break;default:break;}};
        fxZone(p1Gaps[0],l.data(),r.data(),n);for(int position=0;position<classicStagesCount;++position){const int stage=classicRouteAudio.p1[static_cast<size_t>(position)];if(classicRouteAudio.p1Enabled[static_cast<size_t>(stage)])runP1Classic(stage);fxZone(p1Gaps[position+1],l.data(),r.data(),n);}
        if(machine2.idOf()==0){p2ChorusSafeActive=false;}
        else{
            fxZone(fxG_P2pre,l.data(),r.data(),n);
            std::array<float,32> d2L{},d2R{};dryDelay2.process(l[0],r[0],machine2.fxLatency(),d2L[0],d2R[0]);
            const bool chorusBypass=chorusSafe.load(std::memory_order_relaxed)&&machine2.idOf()==15&&h2[3]<=0.0001f;
            if(chorusBypass){if(!p2ChorusSafeActive){machine2.clear();p2ChorusSafeActive=true;}l[0]=d2L[0];r[0]=d2R[0];}
            else{p2ChorusSafeActive=false;machine2.render(l.data(),r.data(),n,false);}
            fxZone(fxG_P2MACH_CHAIN,l.data(),r.data(),n);
            const float a2=envelope2.stage!=0?envelope2.tick():0.0f;
            // Match renderStandard: the serial P2 AMP must not multiply the
            // Synth voice velocity a second time.
            amp2[0]=isSynthVersion?a2:1+(a2*vel-1)*gb;
            static constexpr int p2Gaps[classicStagesCount]={fxG_P2MACH_CHAIN,fxG_P2_EQ_FILT,fxG_P2_FILT_DIST,fxG_P2_DIST_ENV,fxG_P2_ENV_VOLPAN,fxG_P2_VOLPAN_SRR,fxG_P2_SRR_DELAY};
            auto runP2Classic=[&](int stage){switch(stage){case classicDist:chain2.stageDIST(l.data(),r.data(),n);break;case classicSrr:chain2.stageSRR(l.data(),r.data(),n);break;case classicFilt:chain2.stageFILT(l.data(),r.data(),n);break;case classicEq:chain2.stageEQ(l.data(),r.data(),n);break;case classicEnv:chain2.stageENV(l.data(),r.data(),amp2.data(),n);break;case classicVolPan:chain2.stageVOLPAN(l.data(),r.data(),n);break;case classicDelay:chain2.stageDELAY(l.data(),r.data(),n);break;default:break;}};
            for(int position=0;position<classicStagesCount;++position){const int stage=classicRouteAudio.p2[static_cast<size_t>(position)];if(classicRouteAudio.p2Enabled[static_cast<size_t>(stage)])runP2Classic(stage);if(position+1<classicStagesCount)fxZone(p2Gaps[position+1],l.data(),r.data(),n);}
            const float mix2=p2WetMix.getNextValue();l[0]=d2L[0]*(1.0f-mix2)+l[0]*mix2;r[0]=d2R[0]*(1.0f-mix2)+r[0]*mix2;
        }
        fxZone(fxG_P2post,l.data(),r.data(),n);feedbackNetwork.endSample();
        const float gain=level.getNextValue()/(isSynthVersion?1.0f:100.0f/127.0f),wet=mix.getNextValue();
        float outL=gain*(l[0]*wet+dryL[0]*dryGate*(1-wet)),outR=gain*(r[0]*wet+dryR[0]*dryGate*(1-wet));
        if(!std::isfinite(outL))outL=0;if(!std::isfinite(outR))outR=0;
        buffer.setSample(0,start,stereo?outL:(outL+outR)*0.5f);if(stereo)buffer.setSample(1,start,outR);blockPeak=std::max(blockPeak,std::max(std::abs(outL),std::abs(outR)));

        --controlRemaining;
        if(controlRemaining==0){
            if(global[ArpMode]>0.5f)arp.advance(controlSpan);
            const int msegBase[8]={MsegRate,Mseg2Rate,Mseg3Rate,Mseg4Rate,Mseg5Rate,Mseg6Rate,Mseg7Rate,Mseg8Rate};
            {bool hostPlaying=false;if(auto* ph=getPlayHead())if(auto pos=ph->getPosition())hostPlaying=pos->getIsPlaying();if(hostPlaying&&!hostWasPlaying)for(int mi=0;mi<8;++mi)msegPhase[static_cast<size_t>(mi)]=0.0;hostWasPlaying=hostPlaying;}
            for(int mi=0;mi<msegN;++mi){const int b=msegBase[static_cast<size_t>(mi)];const float rv=global[static_cast<size_t>(b)];auto* rmRaw=globalRaw[static_cast<size_t>(MsegRmode+4*mi)];auto* kyRaw=globalRaw[static_cast<size_t>(MsegKey+4*mi)];auto* lpRaw=globalRaw[static_cast<size_t>(MsegLpoint+4*mi)];const int rmode=rmRaw==nullptr?0:juce::jlimit(0,4,juce::roundToInt(rmRaw->load()));double rate=0.001;static const double cpb[15]={1.0/128,1.0/64,1.0/32,1.0/16,1.0/8,1.0/4,1.0/2,1.0,2.0,4.0,8.0,16.0,32.0,64.0,128.0};if(rmode<=2){const int idx=juce::jlimit(0,14,juce::roundToInt(rv*14.0f));const double mult=rmode==1?1.5:(rmode==2?1.0/1.5:1.0);rate=bpm/60.0*cpb[static_cast<size_t>(idx)]*mult;}else if(rmode==3)rate=1.0/std::max(0.001,0.001*std::pow(60000.0,rv));else rate=0.001*std::pow(10.0,4.301*rv);if(kyRaw!=nullptr&&kyRaw->load()>0.5f&&currentKey>=0)rate*=std::pow(2.0,(static_cast<double>(currentKey%128)-60.0)/12.0);rate*=std::pow(2.0,juce::jlimit(-1.0,1.0,static_cast<double>(lfoFmValue.load(std::memory_order_relaxed)))*13.3);if(plockOn[static_cast<size_t>(236+mi)])rate*=std::pow(2.0,juce::jlimit(-2.0,2.0,static_cast<double>(plockAbs[static_cast<size_t>(236+mi)]?plockNow[static_cast<size_t>(236+mi)]-64.0f:plockNow[static_cast<size_t>(236+mi)])/32.0));rate=std::max(0.0005,rate);msegPhase[static_cast<size_t>(mi)]+=rate*static_cast<double>(controlSpan)/sr;if(msegPhase[static_cast<size_t>(mi)]>=1.0){const int mmode=[&]{auto* rt=globalRaw[static_cast<size_t>(MsegRetrig+4*mi)];return rt==nullptr?0:juce::jlimit(0,5,juce::roundToInt(rt->load()));}();const double lp=lpRaw==nullptr?0.5:juce::jlimit(0.0,1.0,static_cast<double>(lpRaw->load()));const bool held=currentKey>=0;if(mmode==1)msegPhase[static_cast<size_t>(mi)]=1.0;else if(mmode==3)msegPhase[static_cast<size_t>(mi)]=held?std::min(msegPhase[static_cast<size_t>(mi)],lp):1.0;else if(mmode==4)msegPhase[static_cast<size_t>(mi)]=msegPhase[static_cast<size_t>(mi)]-1.0+lp;else if(mmode==5)msegPhase[static_cast<size_t>(mi)]=held?std::fmod(msegPhase[static_cast<size_t>(mi)],lp):(msegPhase[static_cast<size_t>(mi)]>=1.0?1.0:msegPhase[static_cast<size_t>(mi)]);else if(global[static_cast<size_t>(b+2)]>0.5f)msegPhase[static_cast<size_t>(mi)]=std::fmod(msegPhase[static_cast<size_t>(mi)],1.0);else msegPhase[static_cast<size_t>(mi)]=1.0;}}
            for(int mi=0;mi<8;++mi)msegPhaseUi[static_cast<size_t>(mi)].store(msegPhase[static_cast<size_t>(mi)],std::memory_order_relaxed);
        }
        ++start;--length;
    }
}
void MonomachineNovaAudioProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer& events){
    juce::ScopedNoDenormals denormals;
    const juce::ScopedTryLock lock(sampleLock);
    if(!lock.isLocked()||!prepared||b.getNumChannels()==0){b.clear();if(isSynthVersion)events.clear();return;}
    fxSlotsSyncToAudio();classicRouteSyncToAudio();feedbackSyncToAudio(); // 1.8.4: UI/state -> цельные аудиоснимки только между блоками
    if(panicRequested.exchange(false))panic();snapshot();blockPeak=0;int cursor=0;
    for(auto metadata:events){int pos=juce::jlimit(cursor,b.getNumSamples(),metadata.samplePosition);render(b,cursor,pos-cursor);
        if(metadata.numBytes>0&&metadata.numBytes<=3)midi(juce::MidiMessage(metadata.data,metadata.numBytes));cursor=pos;}
    render(b,cursor,b.getNumSamples()-cursor);if(isSynthVersion)events.clear();peak.store(blockPeak);
    { // 1.6.13: предпрослушивание семпла BBOX (не зависит от выбранной машины)
        const int slot=previewSlot.load();
        if(slot>=0){const juce::ScopedLock pl(previewLock);
            const int total=static_cast<int>(previewBuffer.size());int pos=previewPos;
            const int n=std::min(b.getNumSamples(),std::max(0,total-pos));
            if(n>0)for(int ch=0;ch<b.getNumChannels();++ch){auto* dst=b.getWritePointer(ch);for(int i=0;i<n;++i)dst[i]+=previewBuffer[static_cast<size_t>(pos+i)]*0.5f;}
            previewPos=pos+n;
            if(previewPos>=total)previewSlot.store(-1);}
    }
    playingSampleSlot.store(previewSlot.load()>=0?previewSlot.load():(activeMachine>=0&&envelope.stage!=0&&nova::machines()[static_cast<size_t>(activeMachine)].id==7?machine.bbox.playingSlot():-1));
}

// 1.6.13: PLAY в списке BBOX: копируем слот под локом и крутим отдельный голос.
void MonomachineNovaAudioProcessor::previewSample(int slot){
    if(slot<0||slot>23)return;
    juce::MemoryBlock blob;{const juce::ScopedLock lock(sampleLock);blob=sampleData[static_cast<size_t>(slot)];}
    const juce::ScopedLock pl(previewLock);
    previewBuffer.resize(static_cast<size_t>(blob.getSize())/sizeof(float));
    if(!previewBuffer.empty()&&blob.getSize()>0)std::memcpy(previewBuffer.data(),blob.getData(),previewBuffer.size()*sizeof(float));
    previewPos=0;previewSlot.store(slot);
}

juce::String MonomachineNovaAudioProcessor::loadSample(int slot,const juce::File& file){
    if(slot<0||slot>=24)return "Invalid slot";
    juce::AudioFormatManager formats;formats.registerBasicFormats();std::unique_ptr<juce::AudioFormatReader> reader(formats.createReaderFor(file));
    if(!reader||reader->sampleRate<=0||reader->lengthInSamples<1||reader->numChannels<1)return "Cannot read WAV/AIFF sample";
    int inputCount=static_cast<int>(std::min<juce::int64>(reader->lengthInSamples,static_cast<juce::int64>(reader->sampleRate*5)));
    if(inputCount<1||reader->sampleRate>384000)return "Unsupported sample rate";
    juce::AudioBuffer<float> input(std::min(2,static_cast<int>(reader->numChannels)),inputCount);
    if(!reader->read(&input,0,inputCount,0,true,true))return "Sample read failed";
    monomachine::MonomachineBBox::SampleSlot ready;ready.loaded=true;ready.name=file.getFileName().toStdString();ready.originalSampleRate=44100;
    const int count=std::max(1,static_cast<int>(inputCount*44100/reader->sampleRate));ready.data.resize(static_cast<size_t>(count));
    for(int i=0;i<count;++i){double pos=i*reader->sampleRate/44100;int a=std::min(inputCount-1,static_cast<int>(pos)),b=std::min(a+1,inputCount-1);float frac=static_cast<float>(pos-a),v=0;
        for(int ch=0;ch<input.getNumChannels();++ch)v+=input.getSample(ch,a)+frac*(input.getSample(ch,b)-input.getSample(ch,a));
        v/=static_cast<float>(input.getNumChannels());ready.data[static_cast<size_t>(i)]=std::isfinite(v)?std::clamp(v,-4.0f,4.0f):0;}
    monomachine::MonomachineBBox::analyseSlot(ready);
    juce::MemoryBlock blob(ready.data.data(),ready.data.size()*sizeof(float));juce::String name(ready.name);
    {const juce::ScopedLock lock(sampleLock);machine.bbox.swapSample(static_cast<size_t>(slot),ready);sampleData[static_cast<size_t>(slot)].swapWith(blob);sampleNames[static_cast<size_t>(slot)]=name;}
    return {}; // Retired buffers are destroyed here, outside the audio callback and lock.
}
juce::String MonomachineNovaAudioProcessor::sampleName(int slot){const juce::ScopedLock lock(sampleLock);return sampleNames[static_cast<size_t>(juce::jlimit(0,23,slot))];}
void MonomachineNovaAudioProcessor::resetAllMsegs(){for(auto& m:mseg)m.reset();} // 1.6.32: RESET ALL -- восемь страниц к дефолтной форме
void MonomachineNovaAudioProcessor::resetSamples(bool modern){
    monomachine::MonomachineBBox defaults(modern);
    for(size_t i=0;i<24;++i){auto slot=defaults.getSlots()[i];juce::MemoryBlock old;if(!modern)old=juce::MemoryBlock(slot.data.data(),slot.data.size()*sizeof(float));{const juce::ScopedLock lock(sampleLock);sampleNames[i]=slot.name;machine.bbox.swapSample(i,slot);sampleData[i].swapWith(old);}}
    requestPanic();
}
void MonomachineNovaAudioProcessor::getStateInformation(juce::MemoryBlock& out){
    auto state=parameters.copyState();state.setProperty("schema",monomachine::kDspModeSchemaVersion,nullptr); // 1.6.32x: БЕЗ перезаписей -- copyState() сам флашит атомы в дерево (в т.ч. ARP/рандом без notify)
    auto samples=juce::ValueTree("Samples");
    for(size_t i=0;i<24;++i){juce::MemoryBlock data;juce::String name;{const juce::ScopedLock lock(sampleLock);data=sampleData[i];name=sampleNames[i];}
        if(data.getSize()){auto node=juce::ValueTree("Sample");node.setProperty("slot",static_cast<int>(i),nullptr);node.setProperty("name",name,nullptr);node.setProperty("pcm44100",data.toBase64Encoding(),nullptr);samples.addChild(node,-1,nullptr);}}
    state.addChild(samples,-1,nullptr);
    const char* msegTags[]{"MSEG","MSEG2","MSEG3","MSEG4","MSEG5","MSEG6","MSEG7","MSEG8"}; // 1.6.29: восемь страниц (первая -- ещё и как старый "MSEG")
    state.setProperty("msegpages",msegPageCount(),nullptr);
    for(int mi=0;mi<8;++mi){auto msegState=juce::ValueTree(msegTags[mi]);msegState.setProperty("count",msegPointCount(mi),nullptr);
        msegState.setProperty("steps",msegSteps(mi)?1:0,nullptr);
        for(int i=0;i<msegPointCount(mi);++i){auto point=juce::ValueTree("POINT");point.setProperty("x",msegPointX(mi,i),nullptr);point.setProperty("y",msegPointY(mi,i),nullptr);point.setProperty("k",msegSegK(mi,i),nullptr);msegState.addChild(point,-1,nullptr);}
        state.addChild(msegState,-1,nullptr);}
    state.removeChild(state.getChildWithName("PLOCK"),nullptr);state.addChild(plockToTree(),-1,nullptr); // 1.7.4: сериализация в helper, без дублей после undo
    state.removeChild(state.getChildWithName("FXSLOTS"),nullptr);state.addChild(fxSlotsToTree(),-1,nullptr); // 1.8.4: 0..32 пользовательских слотов и их 8 ручек
    state.removeChild(state.getChildWithName("CLASSICROUTE"),nullptr);state.addChild(classicRouteToTree(),-1,nullptr); // 1.8.4: independent P1/P2 stage order
    state.removeChild(state.getChildWithName("FEEDBACK"),nullptr);state.addChild(feedbackToTree(),-1,nullptr); // 1.8.4: one FB route, manual gains/DC control
    if(auto xml=state.createXml())copyXmlToBinary(*xml,out);
}
void MonomachineNovaAudioProcessor::setStateInformation(const void* data,int size){
    if(data==nullptr||size<=0||size>64*1024*1024)return;
    if(auto xml=getXmlFromBinary(data,size))if(xml->hasTagName(parameters.state.getType().toString())){
        auto state=juce::ValueTree::fromXml(*xml);auto samples=state.getChildWithName("Samples");state.removeChild(samples,nullptr);
        auto savedFxSlots=state.getChildWithName("FXSLOTS");state.removeChild(savedFxSlots,nullptr); // 1.8.4: отдельное состояние, не APVTS-параметры
        auto savedClassicRoute=state.getChildWithName("CLASSICROUTE");state.removeChild(savedClassicRoute,nullptr); // absent old state = canonical route
        auto savedFeedback=state.getChildWithName("FEEDBACK");state.removeChild(savedFeedback,nullptr); // 1.8.4: separate FB state, absent means safely OFF
        const int schema=static_cast<int>(state.getProperty("schema",2)); // 1.6.29: ФИКС -- MSEG-детей больше НЕ удаляем до loadMseg (MSEG1 теряла точки)
        if(schema<4){
            float oldArp=0,oldSpeed=6;
            for(int i=0;i<state.getNumChildren();++i){auto node=state.getChild(i);const auto id=node["id"].toString();
                if(id=="level"&&schema==3)node.setProperty("value",static_cast<float>(node["value"])*127.0f,nullptr);
                if(id=="arp_mode")oldArp=static_cast<float>(node["value"]);
                if(id=="arp_speed")oldSpeed=static_cast<float>(node["value"]);
                for(int page=3;page<6;++page)if(id==nova::pageParam(page,3)){
                    const int old=juce::jlimit(0,10,static_cast<int>(node["value"]));
                    const int map3[]{0,2,4,6,10,8,1,3,5,7,9};const int map2[]{0,2,4,4,10};node.setProperty("value",schema<=2?map2[std::min(old,4)]:map3[old],nullptr);
                }
            }
            auto add=[&](const char* id,float value){auto node=state.getChildWithProperty("id",id);if(!node.isValid()){node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);state.addChild(node,-1,nullptr);}node.setProperty("value",value,nullptr);};
            const float ticks[]{96,48,24,12,6,3,1.5f,16,8,4,2,1,36,18,9,4.5f};int closest=0;for(int i=1;i<16;++i)if(std::abs(oldSpeed-ticks[i])<std::abs(oldSpeed-ticks[closest]))closest=i;add("arp_grid",static_cast<float>(closest));
            add("arp_on",oldArp>0?1.0f:0.0f);add("arp_hold",oldArp==2?1.0f:0.0f);
            add("m7_6",1.0f);state.setProperty("schema",4,nullptr);
        }
        if(schema<5&&!state.getChildWithProperty("id","amp_mode").isValid()){
            const char* ids[]{"amp_mode","amp_curve_a","amp_curve_d","amp_curve_r"};for(const char* id:ids){auto node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",0.0f,nullptr);state.addChild(node,-1,nullptr);}
        }
        if(schema<6&&isSynthVersion){for(auto pair:{std::pair<const char*,float>{"m4_3",0.0f},{"m2_3",64.0f}}){if(!state.getChildWithProperty("id",pair.first).isValid()){auto node=juce::ValueTree("PARAM");node.setProperty("id",pair.first,nullptr);node.setProperty("value",pair.second,nullptr);state.addChild(node,-1,nullptr);}}auto old=state.getChildWithProperty("id","m2_1");if(old.isValid()&&!state.getChildWithProperty("id","m2_4").isValid()){auto bit=juce::ValueTree("PARAM");bit.setProperty("id","m2_4",nullptr);bit.setProperty("value",old["value"],nullptr);state.addChild(bit,-1,nullptr);old.setProperty("value",0.0f,nullptr);}}
        if(schema<8){for(int section=0;section<monomachine::DspSectionCount;++section){const char* id=monomachine::dspModeParamId(section);if(!state.getChildWithProperty("id",id).isValid()){auto node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",static_cast<float>(monomachine::dspModeOld),nullptr);state.addChild(node,-1,nullptr);}}}
        if(schema<13){ // 1.7.8: RATE 0.05..20 Hz/beat -> 0..1 в семье RMODE; семья по старой галке SYNC (TEMPO/HZ); 1.7.9: +чистые дефолты всех узлов 64 строк (лечит "случайные" сорсы из старых пресетов/RESET ALL)
            for(int r=0;r<64;++r){const juce::String pr="r"+juce::String(r)+"_";const std::pair<const char*,float> defs[]={{"on",1.0f},{"src",0.0f},{"dest",0.0f},{"depth",0.0f},{"mode",0.0f},{"lock",0.0f},{"aux",0.0f},{"aux_depth",0.0f}};
                for(auto& df:defs){auto n2=state.getChildWithProperty("id",pr+df.first);if(!n2.isValid()){auto nn=juce::ValueTree("PARAM");nn.setProperty("id",pr+df.first,nullptr);nn.setProperty("value",df.second,nullptr);state.addChild(nn,-1,nullptr);}}} // 1.7.9
            static const double cpb13[15]={1.0/128,1.0/64,1.0/32,1.0/16,1.0/8,1.0/4,1.0/2,1.0,2.0,4.0,8.0,16.0,32.0,64.0,128.0};
            for(int mi=1;mi<=8;++mi){const juce::String n=mi==1?"mseg":"mseg"+juce::String(mi);
                auto rateN=state.getChildWithProperty("id",n+"_rate");auto syncN=state.getChildWithProperty("id",n+"_sync");
                auto modeN=state.getChildWithProperty("id",n+"_rmode");
                if(!modeN.isValid()){modeN=juce::ValueTree("PARAM");modeN.setProperty("id",n+"_rmode",nullptr);state.addChild(modeN,-1,nullptr);}
                const bool syncOn=syncN.isValid()&&static_cast<float>(syncN["value"])>0.5f;
                if(syncOn){const double old=rateN.isValid()?std::clamp(static_cast<double>(static_cast<float>(rateN["value"])),1.0/128.0,128.0):1.0;
                    int idx=7;double best=1e9;for(int k=0;k<15;++k){const double d=std::abs(std::log(cpb13[k]/old));if(d<best){best=d;idx=k;}}
                    if(rateN.isValid())rateN.setProperty("value",static_cast<float>(idx)/14.0f,nullptr);
                    modeN.setProperty("value",0.0f,nullptr);}
                else{const double oldHz=rateN.isValid()?std::clamp(static_cast<double>(static_cast<float>(rateN["value"])),0.001,20.0):1.0;
                    if(rateN.isValid())rateN.setProperty("value",juce::jlimit(0.0f,1.0f,static_cast<float>(std::log10(oldHz/0.001)/4.301)),nullptr);
                    modeN.setProperty("value",4.0f,nullptr);} }}
        if(schema<12){ // 1.7.7: MSEG retrig-галка -> режим (RETRIG|ENVELOPE|FREE); матрица -- все 64 слота ON
            const char* rtags[8]={"mseg_retrig","mseg2_retrig","mseg3_retrig","mseg4_retrig","mseg5_retrig","mseg6_retrig","mseg7_retrig","mseg8_retrig"};
            for(auto* tag:rtags){auto node=state.getChildWithProperty("id",tag);
                if(node.isValid()){const float v=static_cast<float>(node["value"]);node.setProperty("value",v>0.5f?0.0f:2.0f,nullptr);} // 1=RETRIG -> 0; 0=FREE -> 2
                else{auto n2=juce::ValueTree("PARAM");n2.setProperty("id",tag,nullptr);n2.setProperty("value",0.0f,nullptr);state.addChild(n2,-1,nullptr);}}
            for(int r=0;r<64;++r){auto on=state.getChildWithProperty("id",juce::String("r")+juce::String(r)+"_on");
                if(!on.isValid()){auto n3=juce::ValueTree("PARAM");n3.setProperty("id",juce::String("r")+juce::String(r)+"_on",nullptr);n3.setProperty("value",1.0f,nullptr);state.addChild(n3,-1,nullptr);}
                else on.setProperty("value",1.0f,nullptr);} // все слоты включены
            state.setProperty("schema",12,nullptr); }
        if(schema<10){ // v6: альтернативные режимы dist2/fm2/bbox2 удалены, список -- mnm|old
            for(int section=0;section<monomachine::DspSectionCount;++section){
                const char* id=monomachine::dspModeParamId(section);
                auto node=state.getChildWithProperty("id",id);
                if(!node.isValid())continue;
                const int mapped=monomachine::dspModeLegacyIndexToCurrent(juce::roundToInt(static_cast<float>(node["value"])));
                const int fixed=monomachine::dspModeAllowedForSection(section,mapped)?mapped:monomachine::dspModeMnm;
                node.setProperty("value",static_cast<float>(fixed),nullptr);
            }
        }
        if(schema<11){ // 1.6.8: режим SYNT теперь хранится по каждой машине -- наследуем общий mode_synt
            auto shared=state.getChildWithProperty("id","mode_synt");
            const float inherited=shared.isValid()?static_cast<float>(shared["value"]):static_cast<float>(monomachine::dspModeMnm);
            for(const auto& m:nova::machines()){
                const auto id=juce::String(monomachine::dspMachineModeParamId(m.id));
                if(!state.getChildWithProperty("id",id).isValid()){
                    auto node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",inherited,nullptr);state.addChild(node,-1,nullptr);
                }
            }
        }
        // Schema 18 ordered MODE L/H by physical side and made MODE S the
        // one DIST selector. Schema 19 inserts a separately selectable
        // experimental MNM FIX without changing the existing MNM/OLD/imported
        // sounds. Both migrations are one-shot and preserve prior projects.
        if(schema<19){
            auto ensureParam=[&state](const juce::String& id,float fallback){
                auto node=state.getChildWithProperty("id",id);
                if(!node.isValid()){
                    node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);
                    node.setProperty("value",fallback,nullptr);state.addChild(node,-1,nullptr);
                }
                return node;
            };
            if(schema<18){
                auto remapSide=[&](const juce::String& id,bool lower){
                    auto node=ensureParam(id,0.0f);
                    const int old=juce::jlimit(0,8,juce::roundToInt(static_cast<float>(node["value"])));
                    static constexpr int lowerMap[]{0,6,1,7,8,4,5,2,3};
                    static constexpr int upperMap[]{0,1,6,2,3,4,5,7,8};
                    node.setProperty("value",static_cast<float>((lower?lowerMap:upperMap)[old]),nullptr);
                };
                remapSide("hybrid_p1_mode_l",true); remapSide("hybrid_p1_mode_h",false);
                remapSide("hybrid_p2_mode_l",true); remapSide("hybrid_p2_mode_h",false);
            }
            const auto remapSaturation=[&](const juce::String& selectorId,const juce::String& legacyDistId){
                auto selector=state.getChildWithProperty("id",selectorId);
                int mapped=nova::hybrid_private::hybridDistMnm;
                if(schema<18){
                    const auto legacy=state.getChildWithProperty("id",legacyDistId);
                    const bool legacyOld=legacy.isValid()&&juce::roundToInt(static_cast<float>(legacy["value"]))==monomachine::dspModeOld;
                    const int old=selector.isValid()?juce::jlimit(0,3,juce::roundToInt(static_cast<float>(selector["value"]))):0;
                    // Old MODE S was NATIVE|FOLD|ZERO|CLAMP. legacy OLD won
                    // over every private selection at render time.
                    mapped=legacyOld?nova::hybrid_private::hybridDistOld:(old==0?nova::hybrid_private::hybridDistMnm:old+2);
                } else {
                    // Schema 18 was MNM|OLD|FOLD|ZERO|CLAMP. Insert MNM FIX
                    // at index 2 while retaining every existing sound value.
                    selector=ensureParam(selectorId,0.0f);
                    const int old=juce::jlimit(0,4,juce::roundToInt(static_cast<float>(selector["value"])));
                    static constexpr int schema18Map[]{nova::hybrid_private::hybridDistMnm,nova::hybrid_private::hybridDistOld,nova::hybrid_private::hybridDistFold,nova::hybrid_private::hybridDistZero,nova::hybrid_private::hybridDistClamp};
                    mapped=schema18Map[old];
                }
                selector=ensureParam(selectorId,static_cast<float>(mapped));
                selector.setProperty("value",static_cast<float>(mapped),nullptr);
            };
            remapSaturation("hybrid_p1_mode_s","mode_dist");
            remapSaturation("hybrid_p2_mode_s","p2_mode_dist");
        }
        // Schema 20 introduces only opt-in filter-extra and MOD ENV controls.
        // Insert explicit neutral/default values into older state trees so a
        // project saved before these controls remains audibly identical.
        if(schema<20){
            auto ensureNeutral=[&state](const juce::String& id,float value){
                if(state.getChildWithProperty("id",id).isValid())return;
                auto node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",value,nullptr);state.addChild(node,-1,nullptr);
            };
            const char* filterFields[]{"vel_l","vel_h","kt_l","kt_h","sat","env_atk","env_hold","env_dec","env_rel","env_mix","env_base_depth","env_width_depth"};
            const float filterDefaults[]{0,0,0,0,0,0,0,127,127,0,0,0};
            for(const char* prefix:{"filt_","p2_filt_"})for(size_t i=0;i<12;++i)ensureNeutral(juce::String(prefix)+filterFields[i],filterDefaults[i]);
            for(int me=1;me<=4;++me){const auto prefix="modenv"+juce::String(me)+"_";ensureNeutral(prefix+"atk",0);ensureNeutral(prefix+"hold",0);ensureNeutral(prefix+"dec",127);ensureNeutral(prefix+"rel",127);}
        }
        // Schema 21 replaces the fixed 16x8 playback page model with a live
        // 1..64 contiguous window.  Retain all old parameter IDs, and migrate
        // the old current page/limit into the first 64 stable step slots so an
        // existing pattern keeps playing rather than being silently discarded.
        if(schema<21){
            auto ensureArpParam=[&state](const juce::String& id,float fallback){
                auto node=state.getChildWithProperty("id",id);
                if(!node.isValid()){node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",fallback,nullptr);state.addChild(node,-1,nullptr);}
                return node;
            };
            const auto oldValue=[&state](const char* id,float fallback){auto n=state.getChildWithProperty("id",id);return n.isValid()?static_cast<float>(n["value"]):fallback;};
            const int oldPage=juce::jlimit(0,15,juce::roundToInt(oldValue("arp_step_page",0.0f)));
            const int oldLimit=juce::jlimit(1,16,juce::roundToInt(oldValue("arp_step_page_limit",1.0f)));
            const int oldStart=oldPage*8;
            const int copied=juce::jlimit(1,64,juce::jmin(128-oldStart,oldLimit*8));
            std::array<std::array<float,3>,64> staged{};
            const char* fields[]{"hold","transpose","velocity"};
            const float fallback[]{0.0f,0.0f,127.0f};
            for(int i=0;i<copied;++i)for(int f=0;f<3;++f){
                const int absolute=oldStart+i;
                const auto id="arp_s"+juce::String(absolute/8)+"_"+juce::String(absolute%8)+"_"+fields[f];
                auto n=state.getChildWithProperty("id",id);staged[static_cast<size_t>(i)][static_cast<size_t>(f)]=n.isValid()?static_cast<float>(n["value"]):fallback[f];
            }
            for(int i=0;i<copied;++i)for(int f=0;f<3;++f){
                const auto id="arp_s"+juce::String(i/8)+"_"+juce::String(i%8)+"_"+fields[f];
                ensureArpParam(id,fallback[f]).setProperty("value",staged[static_cast<size_t>(i)][static_cast<size_t>(f)],nullptr);
            }
            ensureArpParam("arp_step_start",1.0f).setProperty("value",1.0f,nullptr);
            ensureArpParam("arp_step_end",16.0f).setProperty("value",static_cast<float>(copied),nullptr);
            // Preserve the old IDs but make their migrated compatibility view
            // begin at the now-active first page.
            ensureArpParam("arp_step_page",0.0f).setProperty("value",0.0f,nullptr);
            ensureArpParam("arp_step_page_limit",1.0f).setProperty("value",static_cast<float>((copied+7)/8),nullptr);
            // P-lock CSV arrays are legacy 128-step storage.  Copy the same
            // active region to 0..63 without throwing away the rest of a saved
            // project, then the new engine consumes its first 64 slots.
            if(auto pl=state.getChildWithName("PLOCK");pl.isValid())for(int ni=0;ni<pl.getNumChildren();++ni){
                auto n=pl.getChild(ni);
                auto remap=[oldStart,copied](const juce::String& csv){
                    auto tok=juce::StringArray::fromTokens(csv,",","");std::array<juce::String,128> all{};
                    for(int i=0;i<128;++i)all[static_cast<size_t>(i)]=i<tok.size()?tok[i]:"0";
                    auto source=all;for(int i=0;i<copied;++i)all[static_cast<size_t>(i)]=source[static_cast<size_t>(oldStart+i)];
                    juce::StringArray out;for(const auto& v:all)out.add(v);return out.joinIntoString(",");
                };
                n.setProperty("v",remap(n.getProperty("v").toString()),nullptr);
                n.setProperty("sl",remap(n.getProperty("sl").toString()),nullptr);
            }
        }
        // Schema 22 adds an opt-in independent P-LOCK transport. Existing
        // patches must keep the exact historical ARP-step-indexed behaviour,
        // hence SYNC defaults to ON and the new local window is 1..16.
        if(schema<22){
            auto ensurePlockWindow=[&state](const char* id,float value){
                if(state.getChildWithProperty("id",id).isValid())return;
                auto node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",value,nullptr);state.addChild(node,-1,nullptr);
            };
            ensurePlockWindow("plock_sync",1.0f);
            ensurePlockWindow("plock_step_start",1.0f);
            ensurePlockWindow("plock_step_end",16.0f);
        }
        // Schema 23 appends the NEW delay selector at index two plus two
        // neutral feedback-Q values per P1/P2 chain.  A schema-22 value of two
        // was a withdrawn branch, so it is explicitly collapsed below rather
        // than being reinterpreted as NEW.
        if(schema<23){
            auto ensureDelayQ=[&state](const char* id){
                if(state.getChildWithProperty("id",id).isValid())return;
                auto node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",0.0f,nullptr);state.addChild(node,-1,nullptr);
            };
            for(const char* id:{"dly_dbas_q","dly_dwid_q","p2_dly_dbas_q","p2_dly_dwid_q"})ensureDelayQ(id);
        }
        // Schema 24 appends twelve R-classic responses after stable 0..8;
        // schema 25 in turn appends R Import 2 after the complete 0..20 range;
        // schema 28 appends only the six derived HP complements after 0..51.
        // An older tree can never legitimately select an ID introduced later:
        // normalize it to NATIVE rather than reinterpreting a malformed value as
        // a new sound. Schema-24 projects retain 0..20; schema-25..27 projects
        // retain 0..51; only schema 28 admits the appended 52..57 values.
        const auto migrateHybridFilterMode=[&state,schema](const char* id){
            auto node=state.getChildWithProperty("id",id);
            if(!node.isValid())return;
            int value=juce::roundToInt(static_cast<float>(node["value"]));
            if(schema<24) value=(value>=0&&value<=8)?value:0;
            else if(schema<25) value=(value>=0&&value<=20)?value:0;
            else if(schema<28) value=(value>=0&&value<=51)?value:0;
            else value=juce::jlimit(0,nova::kHybridFilterChoiceCount-1,value);
            node.setProperty("value",static_cast<float>(value),nullptr);
        };
        for(const char* id:{"hybrid_p1_mode_l","hybrid_p1_mode_h","hybrid_p2_mode_l","hybrid_p2_mode_h"})
            migrateHybridFilterMode(id);
        // Schema 26 adds independent original-style HPF/LPF key tracking.
        // Absent old-state controls mean normal tracking is ON.
        if(schema<26){
            for(const char* id:{"filt_track_hpf","filt_track_lpf","p2_filt_track_hpf","p2_filt_track_lpf"}){
                if(state.getChildWithProperty("id",id).isValid())continue;
                auto node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",1.0f,nullptr);state.addChild(node,-1,nullptr);
            }
        }
        // Schema 27 appends MODE S values 6..8 only. Existing 0..5 keep
        // their exact meanings; no old project is remapped into a candidate.
        if(schema<27){
            for(const char* id:{"hybrid_p1_mode_s","hybrid_p2_mode_s"}){
                auto node=state.getChildWithProperty("id",id);
                if(!node.isValid())continue;
                const int value=juce::roundToInt(static_cast<float>(node["value"]));
                // A pre-27 tree has no legal candidate IDs. Treat any
                // malformed/out-of-era value as MNM, not as a nearby old mode.
                node.setProperty("value",static_cast<float>((value>=0&&value<=5)?value:0),nullptr);
            }
        }
        // 1.9.0 rollback: values formerly selecting FMA or source-derived
        // experimental branches must never activate another reserve by index.
        // Map all withdrawn values to the native/default path before APVTS sees
        // the reduced choice lists. AMP's withdrawn fourth choice maps to mnm.
        auto nativeMode=[&](const juce::String& id){
            auto node=state.getChildWithProperty("id",id);
            if(node.isValid() && static_cast<float>(node["value"])>1.0f)
                node.setProperty("value",0.0f,nullptr);
        };
        nativeMode("mode_synt");
        nativeMode("mode_filt"); nativeMode("mode_dist");
        nativeMode("p2_mode_filt"); nativeMode("p2_mode_dist");
        // Only DLY gains NEW at index 2.  State written before schema 23 may
        // contain a withdrawn index 2, which must remain MNM; schema 23+ may
        // intentionally retain NEW while malformed values still fall to MNM.
        const auto migrateDelayMode=[&](const juce::String& id){
            auto node=state.getChildWithProperty("id",id);
            if(!node.isValid())return;
            const float value=static_cast<float>(node["value"]);
            if(value>static_cast<float>(monomachine::dspModeNew)
               || (schema<23 && value>static_cast<float>(monomachine::dspModeOld)))
                node.setProperty("value",static_cast<float>(monomachine::dspModeMnm),nullptr);
        };
        migrateDelayMode("mode_dly");migrateDelayMode("p2_mode_dly");
        // Schema 29 introduces SYNT NEW only on FM+ STAT/PAR/DYN.  Schema 30
        // appends MNM FIX=3 and NEW FIX=4 only for those same machine-local
        // parameters.  Earlier projects must never reinterpret arbitrary raw
        // values as either exact or FIX candidates; non-FM machines remain the
        // stable mnm|old pair.
        const auto migrateSyntMode=[&state,schema](int machineId){
            auto node=state.getChildWithProperty("id",juce::String(monomachine::dspMachineModeParamId(machineId)));
            if(!node.isValid())return;
            const int value=juce::roundToInt(static_cast<float>(node["value"]));
            const bool legal=value==monomachine::dspModeMnm || value==monomachine::dspModeOld
                || (schema>=29 && value==monomachine::dspModeNew
                    && monomachine::dspSyntSupportsNewForMachine(machineId))
                || (schema>=30 && (value==monomachine::dspModeMnmFix
                                    || value==monomachine::dspModeNewFix)
                    && monomachine::dspSyntSupportsFixForMachine(machineId));
            if(!legal) node.setProperty("value",static_cast<float>(monomachine::dspModeMnm),nullptr);
        };
        for(const auto& machineDef:nova::machines()) migrateSyntMode(machineDef.id);
        for(const char* ampId:{"amp_mode","p2_amp_mode"}){
            auto node=state.getChildWithProperty("id",ampId);
            if(node.isValid() && static_cast<float>(node["value"])>2.0f)
                node.setProperty("value",1.0f,nullptr);
        }
        auto p2FxMode=state.getChildWithProperty("id","p2_fx_mode");
        if(p2FxMode.isValid()) state.removeChild(p2FxMode,nullptr);
        state.setProperty("schema",monomachine::kDspModeSchemaVersion,nullptr);
        parameters.replaceState(state);
        // Loading a saved ON state restores its already-generated normal steps;
        // it is not a new enable edge and must not randomise them again.
        arpPageRndWasEnabled.store(parameters.getRawParameterValue("arp_step_random")&&parameters.getRawParameterValue("arp_step_random")->load()>0.5f,std::memory_order_relaxed);
        arpPageRndWritePending.store(false,std::memory_order_relaxed);
        fxSlotsFromTree(savedFxSlots); // 1.8.4: отсутствует в старом пресете -> чистые 0 слотов
        classicRouteFromTree(savedClassicRoute); // 1.8.4: absent old state = canonical mnm order
        feedbackFromTree(savedFeedback); // 1.8.4: старый пресет -> FB OFF / normal manual defaults
        const char* msegTags[]{"MSEG","MSEG2","MSEG3","MSEG4","MSEG5","MSEG6","MSEG7","MSEG8"};
        auto loadMseg=[this,&state](const char* tag,int mi){auto saved=state.getChildWithName(tag);if(!saved.isValid())return;
            mseg[mi].reset();const int n=juce::jlimit(2,monomachine::MSEG::kMaxPoints,static_cast<int>(saved["count"]));
            mseg[mi].setSteps(static_cast<int>(saved.getProperty("steps",0))>0); // 1.6.21
            for(int i=0;i<n&&i<saved.getNumChildren();++i){auto point=saved.getChild(i);mseg[mi].setPoint(i,static_cast<float>(point["x"]),static_cast<float>(point["y"]));mseg[mi].setK(i,static_cast<float>(point.getProperty("k",0.0f)));}
            mseg[mi].setPointCount(n);};
        for(int mi=0;mi<8;++mi)loadMseg(msegTags[mi],mi); // 1.6.29: восемь страниц
        { auto pl=state.getChildWithName("PLOCK");if(pl.isValid())plockFromTree(pl);else plockClearRuntime(); } // 1.7.3: helper (слот без записи = пустой)
        setMsegPageCount(static_cast<int>(state.getProperty("msegpages",3)));
        {const juce::ScopedLock lock(stateLock);pendingSamples=samples;}triggerAsyncUpdate();requestPanic();
    }
}
int MonomachineNovaAudioProcessor::addMsegRoute(int msegIndex,uint8_t target){if(msegIndex>=msegPageCount())setMsegPageCount(msegIndex+1); // 1.6.30: бинд активирует страницу; 1.6.34: возвращает слот
    const int destination=juce::jlimit(0,247,static_cast<int>(target)); // 1.8.0e: цели 0..235 (+LFO-строки); 1.8.1: +MSEG RATE (236..243) -- было +P2 (132..163)
    int slot=-1;
    for(int r=0;r<64;++r){auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");if(on&&on->load()<0.5f){slot=r;break;}} // 1.7.7: 64 слота
    if(slot<0)slot=63;
    auto set=[this](const juce::String& id,float value){if(auto* p=parameters.getParameter(id)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(value));p->endChangeGesture();}};
    set("r"+juce::String(slot)+"_src",static_cast<float>(msegIndex<3?10+msegIndex:15+msegIndex)); // 1.6.21/29: страница 0..2 -> MSEG=10..12, 3..7 -> MSEG4=18..MSEG8=22
    set("r"+juce::String(slot)+"_dest",static_cast<float>(destination+1)); // 1.6.32: 0 зарезервирован под OFF
    set("r"+juce::String(slot)+"_depth",32.0f);
    set("r"+juce::String(slot)+"_mode",0.0f);
    set("r"+juce::String(slot)+"_on",1.0f);
    if(slot>0)moveModRoute(slot,0); // 1.6.34: новый маршрут MSEG всегда всплывает наверх
    return slot;
}
void MonomachineNovaAudioProcessor::addMsegPage(){ // 1.6.29
    const int n=msegPageCount();if(n>=8)return;
    mseg[static_cast<size_t>(n)].reset();
    const juce::String ids[]{"mseg"+juce::String(n+1)+"_rate","mseg"+juce::String(n+1)+"_sync","mseg"+juce::String(n+1)+"_loop"};
    const float defs[]{1.0f,1.0f,1.0f};
    for(int k=0;k<3;++k)if(auto* raw=parameters.getRawParameterValue(ids[k]))raw->store(defs[k]); // батч-запись без уведомления хоста
    setMsegPageCount(n+1);
}
void MonomachineNovaAudioProcessor::removeMsegPage(int page){ // 1.6.29: сдвигаем страницы и ПЕРЕИМЕНОВЫВАЕМ назначения
    const int n=msegPageCount();if(n<=1||page<0||page>=n)return;
    for(int mi=page;mi<n-1;++mi){ // MSEG некопируем (атомики) -- переносим точки поэлементно
        auto& dcur=mseg[static_cast<size_t>(mi)];const auto& snext=mseg[static_cast<size_t>(mi+1)];
        const int cnt=snext.size();for(int i=0;i<cnt;++i){dcur.setPoint(i,snext.x(i),snext.y(i));dcur.setK(i,snext.k(i));}
        dcur.setSteps(snext.isSteps());dcur.setPointCount(cnt);
        for(int k=0;k<3;++k){auto* dst=globalRaw[static_cast<size_t>(MsegRate+mi*3+k)];auto* src=globalRaw[static_cast<size_t>(MsegRate+(mi+1)*3+k)];
            if(dst&&src)dst->store(src->load());}
    }
    mseg[static_cast<size_t>(n-1)].reset();
    for(int k=0;k<3;++k){const juce::String id="mseg"+juce::String(n)+"_"+(k==0?"rate":k==1?"sync":"loop"); // сброс освободившейся страницы
        if(auto* raw=parameters.getRawParameterValue(id))raw->store(1.0f);}
    for(int r=0;r<64;++r){ // атомики -- блокировка не нужна // 1.7.7: 64 слота
        auto* srcRaw=routeRaw[static_cast<size_t>(r)][1];auto* dstRaw=routeRaw[static_cast<size_t>(r)][2];auto* onRaw=routeRaw[static_cast<size_t>(r)][0];
        if(srcRaw==nullptr||dstRaw==nullptr||onRaw==nullptr)continue;
        const int s=juce::roundToInt(srcRaw->load()),d=juce::roundToInt(dstRaw->load());
        if((s>=10&&s<=12)||(s>=18&&s<=22)){const int pg=s<13?s-10:s-15; // 1.6.29: источник-страница MSEG
            if(pg==page){onRaw->store(0.0f);}else if(pg>page){const int npg=pg-1;srcRaw->store(static_cast<float>(npg<3?10+npg:15+npg));}}
        if(d>=57&&d<=64){const int pg=d-57; // 1.6.32: MSEG OUT = значение-1
            if(pg==page){onRaw->store(0.0f);dstRaw->store(0.0f);}else if(pg>page){dstRaw->store(static_cast<float>(d-1));}}
    }
    setMsegPageCount(n-1);
}
// ---- 1.7.1: P-LOCK ---------------------------------------------------------
void MonomachineNovaAudioProcessor::plockSetTarget(int slot,int target){if(slot<0||slot>=kPlockSlots)return;plockTargetI[static_cast<size_t>(slot)].store(juce::jlimit(-1,243,target));} // 1.8.0e: цели до 235; 1.8.1: +MSEG RATE (236..243)
void MonomachineNovaAudioProcessor::plockResetAll(){ // 1.7.5: RESET ALL -- все слоты P-LOCK в ноль
    for(int s=0;s<kPlockSlots;++s){plockTargetI[static_cast<size_t>(s)].store(-1);
        for(int i=0;i<128;++i){plockVal[static_cast<size_t>(s*128+i)].store(0.0f);plockSlideV[static_cast<size_t>(s*128+i)].store(0.0f);}}
    plockClearRuntime();}
int MonomachineNovaAudioProcessor::plockAcquire(int slot,int target){ // 1.7.2: та же цель = тот же слот; иначе этот если пуст; иначе первый с той же целью; иначе первый свободный
    if(target<0||target>243)return juce::jlimit(0,kPlockSlots-1,slot); // every currently routable P1/P2/LFO/MSEG target
    if(slot>=0&&slot<kPlockSlots){const int cur=plockTargetI[static_cast<size_t>(slot)].load();if(cur<0||cur==target)return slot;}
    for(int s=0;s<kPlockSlots;++s)if(plockTargetI[static_cast<size_t>(s)].load()==target)return s;
    for(int s=0;s<kPlockSlots;++s)if(plockTargetI[static_cast<size_t>(s)].load()<0)return s;
    return juce::jlimit(0,kPlockSlots-1,slot);}
void MonomachineNovaAudioProcessor::plockSetPolarity(int slot,int pol){if(slot<0||slot>=kPlockSlots)return;plockPolI[static_cast<size_t>(slot)].store(pol==0?0:1);}
void MonomachineNovaAudioProcessor::plockSetValue(int slot,int page,int step,float v){if(slot<0||slot>=kPlockSlots||page<0||page>15||step<0||step>7)return;plockVal[static_cast<size_t>(slot*128+page*8+step)].store(juce::jlimit(0.0f,127.0f,v));}
void MonomachineNovaAudioProcessor::plockSetSlide(int slot,int page,int step,bool on){if(slot<0||slot>=kPlockSlots||page<0||page>15||step<0||step>7)return;plockSlideV[static_cast<size_t>(slot*128+page*8+step)].store(on?1.0f:0.0f);}
float MonomachineNovaAudioProcessor::plockValue(int slot,int page,int step) const {if(slot<0||slot>=kPlockSlots||page<0||page>15||step<0||step>7)return 0.0f;return plockVal[static_cast<size_t>(slot*128+page*8+step)].load();}
float MonomachineNovaAudioProcessor::plockSlideValue(int slot,int page,int step) const {if(slot<0||slot>=kPlockSlots||page<0||page>15||step<0||step>7)return 0.0f;return plockSlideV[static_cast<size_t>(slot*128+page*8+step)].load();}
int MonomachineNovaAudioProcessor::plockTarget(int slot) const {if(slot<0||slot>=kPlockSlots)return -1;return plockTargetI[static_cast<size_t>(slot)].load();}
int MonomachineNovaAudioProcessor::plockPolarity(int slot) const {if(slot<0||slot>=kPlockSlots)return 0;return plockPolI[static_cast<size_t>(slot)].load();}
float MonomachineNovaAudioProcessor::plockMaxFor(int t) const {
    if(t>=0&&t<8){const auto& sp=nova::machines()[static_cast<size_t>(machineIndex())].synthParams[static_cast<size_t>(t)];return sp.maxVal>0?static_cast<float>(sp.maxVal):127.0f;}
    return 127.0f;} // 1.7.1: UNI-абсолют масштабируется под максимум цели (SYNT), остальное клампует DSP
float MonomachineNovaAudioProcessor::plockScaled(int t,int slot,float v) const {
    const int pol=plockPolI[static_cast<size_t>(slot)].load();
    if(t>=236)return v; // MSEG RATE uses raw centre-64 units below.
    if(t==131)return pol==0?(v-1.0f)/126.0f*2.0f-1.0f:(v-64.0f)/63.0f; // retained LFO FM in octaves
    if(t==66)return pol==0?(v-1.0f)/126.0f*48.0f-24.0f:(v-64.0f)/63.0f*24.0f;
    if(t>=64){if(t==64)return pol==0?(v-1.0f)/126.0f*4.0f:(v-64.0f)/63.0f*2.0f;return pol==0?(v-1.0f)/126.0f*126.0f:(v-64.0f)/63.0f*63.0f;}
    if(t>=56)return pol==0?v/127.0f:(v-64.0f)/63.0f*0.5f;
    const float mx=plockMaxFor(t);
    return pol==0?v/127.0f*mx:(v-64.0f)/63.0f*(mx*0.5f);}
void MonomachineNovaAudioProcessor::plockStep(int absoluteStep){
    const int idx=juce::jlimit(0,63,absoluteStep);plockStepEcho.store(idx,std::memory_order_relaxed);
    plockOn.fill(false); // removed/expired locks release their target immediately
    for(int s=0;s<kPlockSlots;++s){
        auto& R=plockRun[static_cast<size_t>(s)];
        const int t=plockTargetI[static_cast<size_t>(s)].load();
        if(t<0||(t>=67&&t<=130)){R.active=false;R.sliding=false;continue;}
        const float v=plockVal[static_cast<size_t>(s*128+idx)].load();
        if(v>0.5f){
            R.active=true;R.sliding=false;R.now=plockScaled(t,s,v);
            plockOn[static_cast<size_t>(t)]=true;plockAbs[static_cast<size_t>(t)]=plockPolI[static_cast<size_t>(s)].load()==0;plockNow[static_cast<size_t>(t)]=R.now;
            if(plockSlideV[static_cast<size_t>(s*128+idx)].load()>0.5f){int next=-1;
                for(int i=idx+1;i<=plockStepWindowEndRuntime;++i)if(plockVal[static_cast<size_t>(s*128+i)].load()>0.5f){next=i;break;}
                if(next>idx){R.sliding=true;R.from=R.now;R.to=plockScaled(t,s,plockVal[static_cast<size_t>(s*128+next)].load());R.span=next-idx;R.prog=0;}}
        }else if(R.active){
            if(R.sliding){R.prog=std::min(R.prog+1,R.span);R.now=R.from+(R.to-R.from)*(static_cast<float>(R.prog)/static_cast<float>(std::max(1,R.span)));
                plockNow[static_cast<size_t>(t)]=R.now;if(R.prog>=R.span)R.sliding=false;}
            else{R.active=false;plockOn[static_cast<size_t>(t)]=false;}
        }
    }
}
void MonomachineNovaAudioProcessor::plockClearRuntime(){plockOn.fill(false);for(auto& R:plockRun){R.active=false;R.sliding=false;}} // 1.7.1
void MonomachineNovaAudioProcessor::plockFillPage(int slot,int page){ // 1.7.1: бинд сразу назначает степы страницы (текущее значение цели)
    if(slot<0||slot>=kPlockSlots||page<0||page>15)return;const int t=plockTargetI[static_cast<size_t>(slot)].load();if(t<0)return;
    const int pol=plockPolI[static_cast<size_t>(slot)].load();float v=64.0f;
    if(t<56)v=pol==0?juce::jlimit(1.0f,127.0f,base[static_cast<size_t>(t)]/plockMaxFor(t)*127.0f):64.0f;
    else if(t<64)v=pol==0?juce::jlimit(1.0f,127.0f,msegValue[static_cast<size_t>(t-56)]*127.0f):64.0f;
    else if(t==66||t==131)v=64.0f; // PITCH/LFO FM neutral centre
    else if(t>=236)v=pol==0?64.0f:1.0f; // 1.8.1c: MSEG RATE -- ABS нейтраль = центр 64
    else v=1.0f; // RATE/GATE: 1 = нейтраль (0 октав / 0 прибавки)
    for(int st=0;st<8;++st)plockVal[static_cast<size_t>(slot*128+page*8+st)].store(v);}
juce::ValueTree MonomachineNovaAudioProcessor::plockToTree() const { // 1.7.3: слоты P-LOCK -> дерево (хост-состояние и undo-снимки, формат один)
    auto pl=juce::ValueTree("PLOCK");
    for(int s=0;s<kPlockSlots;++s){
        if(plockTarget(s)<0)continue;
        auto n=juce::ValueTree("S");n.setProperty("slot",s,nullptr);n.setProperty("target",plockTarget(s),nullptr);n.setProperty("pol",plockPolarity(s),nullptr);
        juce::String vs,sl;for(int i=0;i<128;++i){vs<<juce::String(juce::roundToInt(plockValue(s,i/8,i%8)))<<(i<127?",":"");sl<<juce::String(plockSlideValue(s,i/8,i%8)>0.5f?1:0)<<(i<127?",":"");}
        n.setProperty("v",vs,nullptr);n.setProperty("sl",sl,nullptr);pl.addChild(n,-1,nullptr);}
    return pl;}
void MonomachineNovaAudioProcessor::plockFromTree(const juce::ValueTree& pl){ // 1.7.3: восстановление из дерева; слот без записи = пустой (undo снимает слоты честно)
    for(int s=0;s<kPlockSlots;++s){
        auto n=pl.getChildWithProperty("slot",s);
        if(!n.isValid()){plockTargetI[static_cast<size_t>(s)].store(-1);continue;}
        plockTargetI[static_cast<size_t>(s)].store(juce::jlimit(-1,243,static_cast<int>(n.getProperty("target",-1)))); // 1.7.10: +PITCH (66)
        plockPolI[static_cast<size_t>(s)].store(static_cast<int>(n.getProperty("pol",0))==1?1:0);
        auto fill=[this,s](const juce::String& csv,bool slide){int i=0;
            for(const auto& tok:juce::StringArray::fromTokens(csv,",","")){if(i>=128)break;
                (slide?plockSlideV:plockVal)[static_cast<size_t>(s*128+i)].store(juce::jlimit(0.0f,127.0f,tok.getFloatValue()));++i;}};
        fill(n.getProperty("v").toString(),false);fill(n.getProperty("sl").toString(),true);}
    plockClearRuntime();}
int MonomachineNovaAudioProcessor::addRouteFromSource(int src, uint8_t target){
    const int source=juce::jlimit(0,35,src); // Existing IDs 0..31 stay stable; MOD ENV1..4 append at 32..35.
    const int destination=juce::jlimit(0,247,static_cast<int>(target)); // 1.8.0e: цели 0..235 (+LFO-строки); 1.8.1: +MSEG RATE (236..243) -- было +P2 (132..163)
    // 1.6.5: если у этого источника уже есть маршрут на эту цель -- перезаписать
    // его (повторное перетаскивание не плодит маршруты), иначе занять первый
    // свободный слот, иначе слот 16.
    int slot=-1;
    for(int r=0;r<64;++r){ // 1.7.7: 64 слота
        auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");
        auto* dest=parameters.getRawParameterValue("r"+juce::String(r)+"_dest");
        auto* srcPar=parameters.getRawParameterValue("r"+juce::String(r)+"_src");
        if(on&&dest&&srcPar&&on->load()>0.5f&&juce::roundToInt(srcPar->load())==source&&juce::roundToInt(dest->load())==destination+1){slot=r;break;} // 1.6.32: dest хранится как цель+1
    }
    // 1.6.12: замок маршрута (LOCK/SOLO) защищает слот от перезаписи прицелом.
    if(slot<0)for(int r=0;r<64;++r){auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");auto* lk=parameters.getRawParameterValue("r"+juce::String(r)+"_lock");if(on&&on->load()<0.5f&&!(lk&&juce::roundToInt(lk->load())>0)){slot=r;break;}} // 1.7.7: 64 слота
    if(slot<0)for(int r=0;r<64;++r){auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");if(on&&on->load()<0.5f){slot=r;break;}} // 1.7.7
    if(slot<0)slot=63;
    auto set=[this](const juce::String& id,float value){if(auto* p=parameters.getParameter(id)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(value));p->endChangeGesture();}};
    set("r"+juce::String(slot)+"_src",static_cast<float>(source));
    set("r"+juce::String(slot)+"_dest",static_cast<float>(destination+1)); // 1.6.32: 0 зарезервирован под OFF
    set("r"+juce::String(slot)+"_depth",32.0f);
    set("r"+juce::String(slot)+"_mode",0.0f);
    set("r"+juce::String(slot)+"_on",1.0f);
    // 1.6.8: свежий маршрут всплывает на первую строку матрицы, чтобы его
    // было легко найти сразу после перетаскивания кнопки LFO.
    if(slot>0){
        moveModRoute(slot,0);
        // The route has been promoted, so the caller must focus row zero rather
        // than the former free row at the bottom of the matrix.
        return 0;
    }
    return slot;
}
void MonomachineNovaAudioProcessor::moveModRoute(int from,int to){
    if(from<0||from>=64||to<0||to>=64||from==to)return; // 1.7.7: 64 слота
    const char* fields[]={"on","src","dest","depth","mode","aux","aux_depth","lock"}; // 1.6.34: + aux_depth; 1.8.1 FIX: +lock -- замок следует за маршрутом (драг его не отрывает)
    for(const char* field:fields){
        auto* a=parameters.getParameter("r"+juce::String(from)+"_"+field);auto* b=parameters.getParameter("r"+juce::String(to)+"_"+field);
        if(!a||!b)continue;const float av=a->getValue(),bv=b->getValue();a->setValueNotifyingHost(bv);b->setValueNotifyingHost(av);
    }
}
void MonomachineNovaAudioProcessor::sortModRoutes(int column, bool descending){
    if(column<1||column>7)return; // 1.6.8/41: ON..AUX DEPTH (6 = AUX, 7 = AUX DEPTH)
    const char* fields[]{"on","src","dest","depth","mode","aux","aux_depth","lock"}; // 1.6.34: + aux_depth; 1.8.1 FIX: +lock (сортировка не отрывает замок от строки)
    struct RouteValue{std::array<float,8> values{};float key=0,tie=0;}; // 1.8.1: +lock
    std::array<RouteValue,64> rows{}; // 1.7.7: 64 слота
    for(int r=0;r<64;++r){
        for(int c=0;c<7;++c){if(auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[c]))rows[static_cast<size_t>(r)].values[static_cast<size_t>(c)]=p->getValue();}
        auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[column-1]);
        const float raw=p?p->convertFrom0to1(p->getValue()):0.0f; rows[static_cast<size_t>(r)].key=column==4?std::abs(raw):raw;
        if(auto* depth=parameters.getParameter("r"+juce::String(r)+"_depth"))rows[static_cast<size_t>(r)].tie=std::abs(depth->convertFrom0to1(depth->getValue()));
    }
    std::stable_sort(rows.begin(),rows.end(),[descending,column](const RouteValue& a,const RouteValue& b){if(a.key!=b.key)return descending?a.key>b.key:a.key<b.key;if(column!=4&&a.tie!=b.tie)return a.tie>b.tie;return false;});
    // 1.6.13: защита от краша хоста ( Ableton падал без ошибки при клике по шапке ):
    // пишем только ИЗМЕН ИВШИЕСЯ значения, аудио-поток на время сортировки подвешен.
    suspendProcessing(true);
    for(int r=0;r<64;++r)for(int c=0;c<8;++c)if(auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[c])){const float v=rows[static_cast<size_t>(r)].values[static_cast<size_t>(c)];if(std::abs(p->getValue()-v)>1e-6f)p->setValueNotifyingHost(v);} // 1.6.34: пишем ВСЕ 7 полей (aux/aux_depth больше не теряются)
    suspendProcessing(false);
}
void MonomachineNovaAudioProcessor::handleAsyncUpdate(){
    // PAGE RND may queue this updater from the audio thread.  Keep sample
    // restoration strictly conditional: an empty pendingSamples tree is not a
    // request to erase the BBOX bank.
    if(arpPageRndWritePending.exchange(false,std::memory_order_acq_rel)){
        const int first=juce::jlimit(0,63,arpPageRndPendingStart.load(std::memory_order_relaxed));
        const int last=juce::jlimit(first,63,arpPageRndPendingEnd.load(std::memory_order_relaxed));
        auto nextRandom=[this](){arpPageRndSeed^=arpPageRndSeed<<13;arpPageRndSeed^=arpPageRndSeed>>17;arpPageRndSeed^=arpPageRndSeed<<5;return arpPageRndSeed;};
        for(int absolute=first;absolute<=last;++absolute){const int pg=absolute/8,st=absolute%8;
            auto write=[this,pg,st](const char* field,float value){const auto id="arp_s"+juce::String(pg)+"_"+juce::String(st)+"_"+field;if(auto* par=parameters.getParameter(id)){par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(value));par->endChangeGesture();}};
            write("hold",(nextRandom()%100u)<25u?1.0f:0.0f);
            write("transpose",static_cast<float>(static_cast<int>(nextRandom()%49u)-24));
            write("velocity",static_cast<float>(1u+nextRandom()%127u));
        }
    }
    juce::ValueTree samples;{const juce::ScopedLock lock(stateLock);samples=pendingSamples;pendingSamples={};}
    if(samples.getNumChildren()<=0)return;
    resetSamples();
    for(int i=0;i<samples.getNumChildren();++i){auto node=samples.getChild(i);int index=node["slot"];if(index<0||index>=24)continue;
        juce::MemoryBlock blob;if(!blob.fromBase64Encoding(node["pcm44100"].toString())||blob.getSize()<sizeof(float)||blob.getSize()>220500*sizeof(float)||blob.getSize()%sizeof(float)!=0)continue;
        monomachine::MonomachineBBox::SampleSlot ready;ready.loaded=true;ready.originalSampleRate=44100;ready.name=node["name"].toString().toStdString();ready.data.resize(blob.getSize()/sizeof(float));std::memcpy(ready.data.data(),blob.getData(),blob.getSize());
        for(auto& v:ready.data)v=std::isfinite(v)?std::clamp(v,-4.0f,4.0f):0;
        const juce::String name(ready.name);monomachine::MonomachineBBox::analyseSlot(ready);
        {const juce::ScopedLock lock(sampleLock);machine.bbox.swapSample(static_cast<size_t>(index),ready);sampleData[static_cast<size_t>(index)].swapWith(blob);sampleNames[static_cast<size_t>(index)]=name;}}
}
juce::AudioProcessorEditor* MonomachineNovaAudioProcessor::createEditor(){return new MonomachineNovaAudioProcessorEditor(*this);}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new MonomachineNovaAudioProcessor();}
