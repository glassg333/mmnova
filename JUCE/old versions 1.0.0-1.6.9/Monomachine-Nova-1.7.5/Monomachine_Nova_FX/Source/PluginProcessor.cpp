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
    for(int l=0;l<3;++l){const juce::String pfx="lfo"+juce::String(l+1)+"_";const char* suf[]{"page_locks","dest_locks","page_solo","dest_solo"};for(int k=0;k<4;++k)lfoLockRaw[static_cast<size_t>(l*4+k)]=parameters.getRawParameterValue(pfx+suf[k]);}
    // 1.6.8: mode_synt is per machine now (mode_synt_m<id>), the shared parameter is gone.
    for(int i=0;i<monomachine::DspSectionCount;++i)modeRaw[static_cast<size_t>(i)]=(i==monomachine::DspSynt)?nullptr:parameters.getRawParameterValue(monomachine::dspModeParamId(i));
    for(size_t m=0;m<nova::machines().size();++m)syntModeRaw[m]=parameters.getRawParameterValue(syntModeParamIdFor(static_cast<int>(m)));
    for(size_t m=0;m<nova::machines().size();++m)for(int i=0;i<8;++i)
        if(nova::machines()[m].synthParams[static_cast<size_t>(i)].maxVal>0)synthRaw[m][static_cast<size_t>(i)]=parameters.getRawParameterValue(nova::machineParam(nova::machines()[m].id,i));
    for(int p=0;p<6;++p)for(int i=0;i<8;++i)pageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]=parameters.getRawParameterValue(nova::pageParam(p,i));
    const char* g[]={"level","bpm","host_sync","fx_mix","gate","arp_mode","arp_play","arp_speed","arp_range","arp_length","macro_x","macro_y","arp_on","arp_hold","arp_sync","arp_time","arp_grid","arp_wrap","arp_velocity_mode","arp_step","arp_step_velocity","arp_step_transpose","arp_step_hold","arp_step_page","arp_step_page_limit","arp_step_random","arp_step_rnd","mseg_rate","mseg_sync","mseg_loop","mseg2_rate","mseg2_sync","mseg2_loop","mseg3_rate","mseg3_sync","mseg3_loop","mseg4_rate","mseg4_sync","mseg4_loop","mseg5_rate","mseg5_sync","mseg5_loop","mseg6_rate","mseg6_sync","mseg6_loop","mseg7_rate","mseg7_sync","mseg7_loop","mseg8_rate","mseg8_sync","mseg8_loop","mseg_retrig","mseg2_retrig","mseg3_retrig","mseg4_retrig","mseg5_retrig","mseg6_retrig","mseg7_retrig","mseg8_retrig"};
    for(size_t i=0;i<GlobalCount;++i)globalRaw[i]=(isSynthVersion&&(i==Mix||i==Gate))?nullptr:parameters.getRawParameterValue(g[i]);
    const char* fields[]={"on","src","dest","depth","mode"};
    for(int r=0;r<16;++r)for(int i=0;i<5;++i)routeRaw[static_cast<size_t>(r)][static_cast<size_t>(i)]=parameters.getRawParameterValue("r"+juce::String(r)+"_"+fields[i]);
    for(int r=0;r<16;++r)auxRaw[static_cast<size_t>(r)]=parameters.getRawParameterValue("r"+juce::String(r)+"_aux"); // 1.6.24: AUX SOURCE
    for(int r=0;r<16;++r)auxDepthRaw[static_cast<size_t>(r)]=parameters.getRawParameterValue("r"+juce::String(r)+"_aux_depth"); // 1.6.32: AUX DEPTH
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
    machine.prepare(sr);chain.volumeReference=isSynthVersion?127.0f:64.0f;chain.prepare(sr);midiGateBlend.reset(sr,0.005);envelope.reset(sr);arp.reset(sr);matrix.reset();for(auto& m:mseg)m.reset();msegPhase.fill(0.0);msegValue.fill(0.0f);liveStepVelocity=0.0f;liveStepTranspose=0.0f;liveArpGate=0.0f;selectedStepVelocity=1.0f;selectedStepTranspose=0.0f;selectedStepHold=0.0f;
    for(auto& l:lfos)l.reset();notes.fill({});pedal.fill(false);wheels.fill(8192);arpHeld.fill(false);
    for(int j=0;j<3;++j){lfoLastPage[static_cast<size_t>(j)]=juce::jlimit(0,7,juce::roundToInt(pageRaw[static_cast<size_t>(3+j)][0]->load()));lfoLastDest[static_cast<size_t>(j)]=juce::jlimit(0,7,juce::roundToInt(pageRaw[static_cast<size_t>(3+j)][1]->load()));}
    currentKey=-1;serial=0;activeMachine=-1;lastArpMode=-1;ccX=ccY=-1;
    dryDelay.clear();previousLfo.fill(0);
    level.reset(sr,0.02);mix.reset(sr,0.02);velocity.reset(sr,0.005);velocity.setCurrentAndTargetValue(1);
    pitch.reset(sr,0);pitch.setCurrentAndTargetValue(60);prepared=true;snapshot();
    midiGateBlend.setCurrentAndTargetValue(global[Gate]>0.5f?1.0f:0.0f);
    level.setCurrentAndTargetValue(global[Level]/127.0f);mix.setCurrentAndTargetValue(isSynthVersion?1:global[Mix]/127);
    panicRequested.store(false);peak.store(0);
}
void MonomachineNovaAudioProcessor::snapshot(){
    // 1.6.8: SYNT-режим берётся из параметра ТЕКУЩЕЙ машины (mode_synt_m<id>),
    // поэтому при листании машин у каждой остаётся свой режим old/mnm.
    const int selectedMachine=machineIndex();
    for(int i=0;i<monomachine::DspSectionCount;++i){
        auto* ptr=(i==monomachine::DspSynt)?syntModeRaw[static_cast<size_t>(selectedMachine)]:modeRaw[static_cast<size_t>(i)];
        int idx = ptr ? juce::roundToInt(ptr->load()) : monomachine::dspModeMnm;
        idx = juce::jlimit(0, monomachine::dspModeCount-1, idx);
        // Единственное правило: в разделе допустимы только его собственные режимы
        // (списки -- в models/DspModes.hpp, dspSectionModeChoices). Всё остальное
        // сводится к основному режиму mnm.  В v6 допустимы только mnm (основной)
        // и old (резерв): альтернативные движки dist2/fm2/bbox2 удалены.
        if(!monomachine::dspModeAllowedForSection(i,idx)){
            idx = monomachine::dspModeMnm;
        }
        dspModes[static_cast<size_t>(i)] = idx;
    }
    (void)monomachine::dspModeFilter;
    // 1.6.19: выбор огибающей вернулся (amp_mode: old = legacy 1.4, mnm = kernel),
    // БЕЗ привязки к режиму DSP-секции AMP.
    envelope.configure(ampRaw[0]->load()>0.5f?nova::AmpEnvelope::kKernelAlgorithm:0,{ampRaw[1]->load(),ampRaw[2]->load(),ampRaw[3]->load()});
    machine.setModes(dspModes[monomachine::DspSynt],dspModes[monomachine::DspDist]);
    chain.setModes(dspModes[monomachine::DspFilter],dspModes[monomachine::DspDist],dspModes[monomachine::DspDelay],dspModes[monomachine::DspRouting]);
    chain.setRepitch(repitchRaw?repitchRaw->load():2.0f,repitchSmoothRaw?repitchSmoothRaw->load():0.0f);
    chain.setPpMode(ppModeRaw?juce::roundToInt(ppModeRaw->load()):0);
    for(int j=0;j<3;++j){
        lfoPageLocks[static_cast<size_t>(j)]=lfoLockRaw[static_cast<size_t>(j*4)]?juce::roundToInt(lfoLockRaw[static_cast<size_t>(j*4)]->load()):0;
        lfoDestLocks[static_cast<size_t>(j)]=lfoLockRaw[static_cast<size_t>(j*4+1)]?juce::roundToInt(lfoLockRaw[static_cast<size_t>(j*4+1)]->load()):0;
        lfoPageSolo[static_cast<size_t>(j)]=lfoLockRaw[static_cast<size_t>(j*4+2)]?juce::roundToInt(lfoLockRaw[static_cast<size_t>(j*4+2)]->load()):0;
        lfoDestSolo[static_cast<size_t>(j)]=lfoLockRaw[static_cast<size_t>(j*4+3)]?juce::roundToInt(lfoLockRaw[static_cast<size_t>(j*4+3)]->load()):0;
    }
    for(size_t i=0;i<GlobalCount;++i)global[i]=globalRaw[i]?globalRaw[i]->load(std::memory_order_relaxed):0;
    int selected=juce::jlimit(0,static_cast<int>(nova::machines().size())-1,juce::roundToInt(machineRaw->load()));
    for(int i=0;i<8;++i){auto* ptr=synthRaw[static_cast<size_t>(selected)][static_cast<size_t>(i)];base[static_cast<size_t>(i)]=ptr?ptr->load():0;}
    for(int p=0;p<3;++p)for(int i=0;i<8;++i)base[static_cast<size_t>(8+p*8+i)]=pageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]->load();
    for(size_t p=0;p<3;++p)for(size_t i=0;i<8;++i){const float value=pageRaw[p+3][i]->load();const float maximum=i==0||i==1?7.0f:i==2?3.0f:i==3?10.0f:i==4?6.0f:127.0f;effectiveLfoParams[p][i]=std::clamp(effectiveLfoParams[p][i]+value-lfoParams[p][i],0.0f,maximum);lfoParams[p][i]=value;}
    bpm=global[Bpm];if(global[HostSync]>0.5f)if(auto* play=getPlayHead())if(auto position=play->getPosition())if(auto tempo=position->getBpm())if(std::isfinite(*tempo))bpm=std::clamp(*tempo,30.0,300.0);
    tempoDisplay.store(bpm);
    if(global[MacroX]!=previousX){ccX=-1;previousX=global[MacroX];}if(global[MacroY]!=previousY){ccY=-1;previousY=global[MacroY];}
    matrix.setParamX(static_cast<uint8_t>(ccX>=0?ccX:global[MacroX]));matrix.setParamY(static_cast<uint8_t>(ccY>=0?ccY:global[MacroY]));
    for(size_t r=0;r<16;++r){const int dvv=juce::jlimit(0,66,juce::roundToInt(routeRaw[r][2]->load())); // 1.7.1: +ARP RATE/GATE (64, 65) // 1.6.32: DEST 0 = OFF, цель = значение-1
        matrix.configureRouting(r,routeRaw[r][0]->load()>0.5f&&dvv>0,static_cast<monomachine::ModSource>(juce::jlimit(0,17,juce::roundToInt(routeRaw[r][1]->load()))),static_cast<uint8_t>(dvv>0?dvv-1:0),static_cast<int8_t>(juce::jlimit(-64,63,juce::roundToInt(routeRaw[r][3]->load()))),(juce::roundToInt(routeRaw[r][4]->load())==1?monomachine::ModPolarity::Bipolar:monomachine::ModPolarity::Unipolar),static_cast<uint8_t>(auxRaw[static_cast<size_t>(r)]?juce::jlimit(0,18,juce::roundToInt(auxRaw[static_cast<size_t>(r)]->load())):0),static_cast<int8_t>(juce::jlimit(-64,63,auxDepthRaw[static_cast<size_t>(r)]?juce::roundToInt(auxDepthRaw[static_cast<size_t>(r)]->load()):0)));}
    matrix.setPitchWheel((wheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]-8192.0f)/8192.0f);
    matrix.setModWheel(modWheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
    matrix.setAftertouch(aftertouch[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
    matrix.setMsegOutputs(msegValue[0],msegValue[1],msegValue[2]); // 1.6.21
    matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:global[ArpStepVelocity]/127.0f,liveStepTranspose!=0.0f?liveStepTranspose:global[ArpStepTranspose]/24.0f,liveArpGate>0.0f?liveArpGate:global[ArpStepHold]);
    matrix.setArpRate(global[ArpGrid]/15.0f);
    midiGateBlend.setTargetValue(global[Gate]>0.5f?1.0f:0.0f);
    level.setTargetValue(global[Level]/127.0f);mix.setTargetValue(isSynthVersion?1:global[Mix]/127);
    global[ArpMode]=global[ArpOn]>0.5f?(global[ArpHold]>0.5f?2.0f:1.0f):0.0f;
    auto mode=static_cast<int>(global[ArpMode]);
    monomachine::MonomachineArpeggiator::ArpSettings settings;
    settings.mode=static_cast<monomachine::MonomachineArpeggiator::Mode>(mode);settings.play=[&]{static const monomachine::MonomachineArpeggiator::Play playMap[]={monomachine::MonomachineArpeggiator::Play::Step,monomachine::MonomachineArpeggiator::Play::StepChord,monomachine::MonomachineArpeggiator::Play::True,monomachine::MonomachineArpeggiator::Play::Up,monomachine::MonomachineArpeggiator::Play::Down,monomachine::MonomachineArpeggiator::Play::Cycl,monomachine::MonomachineArpeggiator::Play::Rnd};return playMap[juce::jlimit(0,6,juce::roundToInt(global[ArpPlay]))];}(); // 1.6.22: порядок STEP|STEP CHORD|TRUE|UP|DOWN|CYCL|RND
    const double ticks[]{96,72,64,48,36,32,24,18,16,12,9,8,6,4.5,4,3,2.25,2,1.5};settings.speed=ticks[juce::jlimit(0,18,static_cast<int>(global[ArpGrid]))]; // 1.7.0: 19 рейтов
    settings.sync=global[ArpSync]>0.5f;settings.milliseconds=global[ArpTime];settings.range=static_cast<uint8_t>(global[ArpRange]);settings.noteLength=static_cast<uint8_t>(global[ArpLength]);settings.wrap=static_cast<uint8_t>(juce::jlimit(1,16,juce::roundToInt(global[ArpWrap])));settings.velocityMode=[&]{static const monomachine::MonomachineArpeggiator::VelocityMode velMap[]={monomachine::MonomachineArpeggiator::VelocityMode::Step,monomachine::MonomachineArpeggiator::VelocityMode::StepKey,monomachine::MonomachineArpeggiator::VelocityMode::StepHold,monomachine::MonomachineArpeggiator::VelocityMode::Key,monomachine::MonomachineArpeggiator::VelocityMode::Hold};return velMap[juce::jlimit(0,4,juce::roundToInt(global[ArpVelocityMode]))];}(); // 1.6.24: порядок STEP|STEP+KEY|STEP+HOLD|KEY|HOLD
    settings.stepPage=static_cast<uint8_t>(juce::jlimit(0,15,juce::roundToInt(global[ArpStepPage])));settings.stepPageLimit=static_cast<uint8_t>(juce::jlimit(1,16,juce::roundToInt(global[ArpStepPageLimit])));settings.stepRandom=juce::jlimit(0.0f,1.0f,global[ArpStepRandom]); // 1.7.1 FIX: параметр 0..1 (в 1.7.0 через /100 движок читал галку как OFF)
    settings.stepRnd=global[ArpStepRnd]>0.5f;settings.autoSwap=arpAutoSwap.load(); // 1.7.1: STEP RND (порядок степов); autoSwap -- 1.6.42
    for(int pg=0;pg<16;++pg)for(int st=0;st<8;++st){const auto prefix="arp_s"+juce::String(pg)+"_"+juce::String(st)+"_";auto& stepValue=settings.steps[static_cast<size_t>(pg*8+st)];if(auto* h=parameters.getRawParameterValue(prefix+"hold"))stepValue.hold=h->load()>0.5f;if(auto* t=parameters.getRawParameterValue(prefix+"transpose"))stepValue.transpose=static_cast<int8_t>(juce::jlimit(-24,24,juce::roundToInt(t->load())));if(auto* v=parameters.getRawParameterValue(prefix+"velocity"))stepValue.velocity=static_cast<uint8_t>(juce::jlimit(0,127,juce::roundToInt(v->load())));}
    const auto& selectedStep=settings.steps[static_cast<size_t>(settings.stepPage*8+juce::jlimit(0,7,juce::roundToInt(global[ArpStep])))];selectedStepVelocity=selectedStep.velocity/127.0f;selectedStepTranspose=selectedStep.transpose/24.0f;selectedStepHold=selectedStep.hold?1.0f:0.0f;matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:selectedStepVelocity,liveStepTranspose!=0.0f?liveStepTranspose:selectedStepTranspose,liveArpGate>0.0f?liveArpGate:selectedStepHold);
    if(mode!=lastArpMode){arp.reset(sr,bpm);arpHeld.fill(false);release();plockClearRuntime();} // 1.7.1
    arp.setSettings(settings);arp.setTempo(bpm);
    if(mode!=lastArpMode){lastArpMode=mode;syncArpNotes();if(mode==0&&currentKey>=0)trigger(currentKey%128,notes[static_cast<size_t>(currentKey)].velocity,currentKey/128);}
    if(selected!=activeMachine){activeMachine=selected;effectiveLfoParams=lfoParams;smoothedParams=base;dryDelay.clear();machine.clear();chain.clear();std::array<float,8> p;std::copy_n(base.data(),8,p.data());machine.set(nova::machines()[static_cast<size_t>(selected)].id,p);
        const int latency=isSynthVersion?0:machine.fxLatency();if(getLatencySamples()!=latency)setLatencySamples(latency);
        if(currentKey>=0&&mode==0)trigger(currentKey%128,notes[static_cast<size_t>(currentKey)].velocity,currentKey/128);}
}
void MonomachineNovaAudioProcessor::panic(){
    notes.fill({});pedal.fill(false);wheels.fill(8192);arpHeld.fill(false);currentKey=-1;serial=0;ccX=ccY=-1;
    plockClearRuntime(); // 1.7.1
    envelope.reset(sr);dryDelay.clear();machine.clear();chain.clear();arp.reset(sr,bpm);matrix.reset();msegPhase.fill(0.0);msegValue.fill(0.0f);liveStepVelocity=0.0f;liveStepTranspose=0.0f;liveArpGate=0.0f;selectedStepVelocity=1.0f;selectedStepTranspose=0.0f;selectedStepHold=0.0f;for(auto& l:lfos)l.reset();
}
void MonomachineNovaAudioProcessor::trigger(int note,float vel,int channel){
    soundingNote=note;soundingChannel=channel;
    for(int mi=0;mi<8;++mi){auto* rt=globalRaw[static_cast<size_t>(MsegRetrig+mi)]; // 1.7.2: RETRIG постранично -- нота MIDI/арп перезапускает фазу этой страницы (дефолт ON = как было)
        if(rt==nullptr||rt->load()>0.5f){msegPhase[static_cast<size_t>(mi)]=0.0;msegValue[static_cast<size_t>(mi)]=mseg[static_cast<size_t>(mi)].value(0.0f);}}
    liveArpGate=1.0f;
    // 1.6.14: портаменто быстрее и явное: время = (PORT/127)^1.5 * 0.8 с, масштабируется SPEED (ПКМ на PORT).
    const float portaSpeed=portaSpeedRaw?juce::jlimit(0.1f,3.0f,0.2f+2.8f*nova::norm(portaSpeedRaw->load())):1.0f;
    const float glide=base[15]<=0?0:std::pow(nova::norm(base[15]),1.5f)*0.8f/portaSpeed;
    pitch.reset(sr,glide);if(envelope.stage==0||glide<=0)pitch.setCurrentAndTargetValue(static_cast<float>(note));else pitch.setTargetValue(static_cast<float>(note));
    if(nova::machines()[static_cast<size_t>(activeMachine)].id==7){std::array<float,8> values{};std::copy_n(base.begin(),8,values.begin());machine.set(7,values);}
    machine.on(note);if(nova::machines()[static_cast<size_t>(activeMachine)].id==7){lastBboxSample.store(machine.bbox.playingSlot());++bboxHitCounter;}envelope.on();chain.trigger();velocity.setTargetValue(vel);
    for(size_t i=0;i<3;++i)lfos[i].trigger(effectiveLfoParams[i]);
    matrix.setNoteAndVelocity(static_cast<uint8_t>(note),static_cast<uint8_t>(std::round(vel*127)));
}
void MonomachineNovaAudioProcessor::release(){envelope.off();chain.release();}
void MonomachineNovaAudioProcessor::syncArpNotes(){
    for(size_t note=0;note<128;++note){uint64_t latest=0;float vel=1;
        for(size_t c=0;c<16;++c)if(notes[c*128+note].order>latest){latest=notes[c*128+note].order;vel=notes[c*128+note].velocity;}
        bool held=latest>0;if(held&&!arpHeld[note])arp.noteOn(static_cast<uint8_t>(note),static_cast<uint8_t>(std::round(vel*127)));
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
    std::array<float,32> l{},r{},dryL{},dryR{},amp{};bool stereo=buffer.getNumChannels()>1;
    while(length>0){
        if(global[ArpMode]>0.5f){for(int k=0;k<3;++k){auto event=arp.poll();if(!event.triggered)break;if(event.isNoteOff){liveArpGate=0.0f;release();}else{liveStepVelocity=event.velocity/127.0f;liveStepTranspose=0.0f;liveArpGate=1.0f;trigger(event.note,event.velocity/127.0f,currentKey>=0?currentKey/128:soundingChannel);plockStep(arp.playingPage,arp.stepEcho);}arpStepEcho.store(arp.stepEcho);arpPageEcho.store(arp.playingPage);}} // 1.6.42: страница для FOLLOW PLAY
        int n=std::min(8,length);if(global[ArpMode]>0.5f)n=std::min(n,std::max(1,arp.samplesUntilEvent()));
        const int msegN=msegPageCount(); // 1.6.29: считаем только видимые страницы
        for(int mi=0;mi<msegN;++mi)msegValue[static_cast<size_t>(mi)]=mseg[mi].value(static_cast<float>(msegPhase[static_cast<size_t>(mi)]));
        for(int mi=msegN;mi<8;++mi)msegValue[static_cast<size_t>(mi)]=0.0f;
        matrix.setMsegOutputs(msegValue.data(),8);
        matrix.setPitchWheel((wheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]-8192.0f)/8192.0f);
        matrix.setModWheel(modWheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
        matrix.setAftertouch(aftertouch[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
        matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:selectedStepVelocity,liveStepTranspose!=0.0f?liveStepTranspose:selectedStepTranspose,liveArpGate>0.0f?liveArpGate:selectedStepHold);
        matrix.setArpRate(global[ArpGrid]/15.0f);
        std::array<float,56> before{},after{};std::copy(base.begin(),base.end(),before.begin());
        for(size_t j=0;j<3;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),before.begin()+32+j*8);
        matrix.setLfoOutputs(previousLfo[0],previousLfo[1],previousLfo[2]);
        std::array<float,8> msegMod{};std::array<float,3> arpMod{}; // 1.7.5: [2] = PITCHmatrix.evaluate(before,after,msegMod.data(),arpMod.data()); // 1.7.1: +цели ARP RATE/GATE
        for(int mi=0;mi<8;++mi)msegValue[static_cast<size_t>(mi)]=std::clamp(msegValue[static_cast<size_t>(mi)]+msegMod[static_cast<size_t>(mi)]/127.0f,0.0f,1.0f);
        for(int t=0;t<56;++t)if(plockOn[static_cast<size_t>(t)])after[static_cast<size_t>(t)]=std::clamp(plockAbs[static_cast<size_t>(t)]?plockNow[static_cast<size_t>(t)]:after[static_cast<size_t>(t)]+plockNow[static_cast<size_t>(t)],0.0f,127.0f); // 1.7.1: P-LOCK поверх маршрутов (UNI абсолют / BIP дельта)
        for(int mi=0;mi<8;++mi)if(plockOn[static_cast<size_t>(56+mi)])msegValue[static_cast<size_t>(mi)]=std::clamp(plockAbs[static_cast<size_t>(56+mi)]?plockNow[static_cast<size_t>(56+mi)]:msegValue[static_cast<size_t>(mi)]+plockNow[static_cast<size_t>(56+mi)],0.0f,1.0f); // 1.7.1: P-LOCK на MSEG OUT
        arp.setMod(plockOn[64]?static_cast<double>(plockNow[64]):static_cast<double>(arpMod[static_cast<size_t>(0)])/63.0*2.0,plockOn[65]?plockNow[65]:arpMod[static_cast<size_t>(1)]); // 1.7.1: P-LOCK/маршруты на ARP RATE/GATE
        matrix.setMsegOutputs(msegValue.data(),8);
        float pitchMod=0;
        for(size_t j=0;j<3;++j){const auto& p=effectiveLfoParams[j];const float delta=previousLfo[j]*(p[7]-64);
            // 1.6.12: замки PAGE/DEST. solo -- единственное разрешённое значение
            // (модуляция бессильна, руками менять можно -- замок не снимается);
            // битовую маску locks модуляция перескакивает (sticky: держится
            // последний разрешённый значение, скачок "случайно" мимо замков невозможен).
            int page=juce::jlimit(0,7,static_cast<int>(p[0])),dest=juce::jlimit(0,7,static_cast<int>(p[1]));
            if(lfoPageSolo[j]>0)page=juce::jlimit(0,7,lfoPageSolo[j]-1);
            else if(((lfoPageLocks[j]>>page)&1)!=0)page=lfoLastPage[j];else lfoLastPage[j]=page;
            if(lfoDestSolo[j]>0)dest=juce::jlimit(0,7,lfoDestSolo[j]-1);
            else if(((lfoDestLocks[j]>>dest)&1)!=0)dest=lfoLastDest[j];else lfoLastDest[j]=dest;
            if(page==0){const float ranges[]{1,2,3,5,7,12,24,36};pitchMod+=delta/64*ranges[dest];}
            else after[static_cast<size_t>((page-1)*8+dest)]+=delta;
        }
        for(size_t j=0;j<3;++j){std::array<float,8> effective{};
            for(size_t k=0;k<8;++k){
                const float maximum=k==0||k==1?7.0f:k==2?3.0f:k==3?10.0f:k==4?6.0f:127.0f;
                effective[k]=std::clamp(after[32+j*8+k],0.0f,maximum);}
            previousLfo[j]=lfos[j].process(effective,sr,bpm,n);effectiveLfoParams[j]=effective;
        }
        std::array<float,32> modulated{};
        const float smooth=static_cast<float>(1-std::exp(-static_cast<double>(n)/(sr*0.012)));
        for(size_t i=0;i<32;++i){smoothedParams[i]+=smooth*(std::clamp(after[i],0.0f,127.0f)-smoothedParams[i]);if(std::abs(after[i]-smoothedParams[i])<0.0001f)smoothedParams[i]=std::clamp(after[i],0.0f,127.0f);if(i<8&&nova::machines()[static_cast<size_t>(activeMachine)].id==7&&(i==2||i==3||i==6))smoothedParams[i]=after[i];modulated[i]=smoothedParams[i];}
        std::array<float,8> synthParams{};const auto& def=nova::machines()[static_cast<size_t>(activeMachine)];
        for(size_t i=0;i<8;++i)synthParams[i]=std::clamp(modulated[i],static_cast<float>(def.synthParams[i].minVal),static_cast<float>(def.synthParams[i].maxVal));
        float notePitch=pitch.getNextValue();if(n>1)pitch.skip(n-1);
        pitchMod+=(wheels[static_cast<size_t>(soundingChannel)]-8192)/8192.0f*2;
        machine.noiseDecaySeconds=nova::AmpEnvelope::tau(modulated[10],envelope.mode);
        pitchMod+=arpMod[static_cast<size_t>(2)]/63.0f*24.0f; // 1.7.5: цель PITCH -- полутона, +-24 на полной глубине
        machine.setPitch(notePitch,pitchMod);machine.set(def.id,synthParams);chain.set(modulated);
        for(size_t i=0;i<4;++i)envelope.p[i]=modulated[8+i];
        if(isSynthVersion){std::fill_n(l.data(),n,0.0f);std::fill_n(r.data(),n,0.0f);if(envelope.stage!=0)machine.render(l.data(),r.data(),n,true);}
        else{std::copy_n(buffer.getReadPointer(0,start),n,l.data());std::copy_n(buffer.getReadPointer(stereo?1:0,start),n,r.data());for(int i=0;i<n;++i)dryDelay.process(l[static_cast<size_t>(i)],r[static_cast<size_t>(i)],machine.fxLatency(),dryL[static_cast<size_t>(i)],dryR[static_cast<size_t>(i)]);machine.render(l.data(),r.data(),n,false);}
        for(int i=0;i<n;++i){float a=envelope.tick(),vel=velocity.getNextValue();amp[static_cast<size_t>(i)]=isSynthVersion?a*vel:1+(a*vel-1)*midiGateBlend.getNextValue();}
        chain.process(l.data(),r.data(),amp.data(),n,machine.fmaActive()&&machine.idOf()>=8&&machine.idOf()<=10); // 1.6.14: fma -- мягкий DIST на FM-машинах
        for(int i=0;i<n;++i){size_t x=static_cast<size_t>(i);float gain=level.getNextValue()/(isSynthVersion?1.0f:100.0f/127.0f),wet=mix.getNextValue();
            const float dryGate=isSynthVersion?1.0f:amp[x];
            float a=gain*(l[x]*wet+dryL[x]*dryGate*(1-wet)),b=gain*(r[x]*wet+dryR[x]*dryGate*(1-wet));
            if(!std::isfinite(a))a=0;if(!std::isfinite(b))b=0;
            buffer.setSample(0,start+i,stereo?a:(a+b)*0.5f);if(stereo)buffer.setSample(1,start+i,b);blockPeak=std::max(blockPeak,std::max(std::abs(a),std::abs(b)));}
        if(global[ArpMode]>0.5f)arp.advance(n); // 1.6.25: RETRIG SYNC убран (делал не то)
        const int msegBase[8]={MsegRate,Mseg2Rate,Mseg3Rate,Mseg4Rate,Mseg5Rate,Mseg6Rate,Mseg7Rate,Mseg8Rate}; // 1.6.29: страницы идут пачками по 3 параметра
        for(int mi=0;mi<msegN;++mi){const int b=msegBase[static_cast<size_t>(mi)]; // 1.6.29: rate/sync/loop каждой страницы лежат подряд
            const double rate=std::max(0.001,static_cast<double>(global[static_cast<size_t>(b)])*(global[static_cast<size_t>(b+1)]>0.5f?bpm/60.0:1.0));
            msegPhase[static_cast<size_t>(mi)]+=rate*static_cast<double>(n)/sr;
            if(msegPhase[static_cast<size_t>(mi)]>=1.0){if(global[static_cast<size_t>(b+2)]>0.5f)msegPhase[static_cast<size_t>(mi)]=std::fmod(msegPhase[static_cast<size_t>(mi)],1.0);else msegPhase[static_cast<size_t>(mi)]=1.0;}}
        start+=n;length-=n;
    }
}
void MonomachineNovaAudioProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer& events){
    juce::ScopedNoDenormals denormals;
    const juce::ScopedTryLock lock(sampleLock);
    if(!lock.isLocked()||!prepared||b.getNumChannels()==0){b.clear();if(isSynthVersion)events.clear();return;}
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
    if(auto xml=state.createXml())copyXmlToBinary(*xml,out);
}
void MonomachineNovaAudioProcessor::setStateInformation(const void* data,int size){
    if(data==nullptr||size<=0||size>64*1024*1024)return;
    if(auto xml=getXmlFromBinary(data,size))if(xml->hasTagName(parameters.state.getType().toString())){
        auto state=juce::ValueTree::fromXml(*xml);auto samples=state.getChildWithName("Samples");state.removeChild(samples,nullptr);
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
        state.setProperty("schema",monomachine::kDspModeSchemaVersion,nullptr);
        parameters.replaceState(state);
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
    const int destination=juce::jlimit(0,82,static_cast<int>(target)); // 1.7.5: +PITCH/ROUTE
    int slot=-1;
    for(int r=0;r<16;++r){auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");if(on&&on->load()<0.5f){slot=r;break;}}
    if(slot<0)slot=15;
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
    for(int r=0;r<16;++r){ // атомики -- блокировка не нужна
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
void MonomachineNovaAudioProcessor::plockSetTarget(int slot,int target){if(slot<0||slot>=kPlockSlots)return;plockTargetI[static_cast<size_t>(slot)].store(juce::jlimit(-1,82,target));} // 1.7.5: цели до 82
void MonomachineNovaAudioProcessor::plockResetAll(){ // 1.7.5: RESET ALL -- все слоты P-LOCK в ноль
    for(int s=0;s<kPlockSlots;++s){plockTargetI[static_cast<size_t>(s)].store(-1);
        for(int i=0;i<128;++i){plockVal[static_cast<size_t>(s*128+i)].store(0.0f);plockSlideV[static_cast<size_t>(s*128+i)].store(0.0f);}}
    plockClearRuntime();}
int MonomachineNovaAudioProcessor::plockAcquire(int slot,int target){ // 1.7.2: та же цель = тот же слот; иначе этот если пуст; иначе первый с той же целью; иначе первый свободный
    if(target<0||target>65)return juce::jlimit(0,kPlockSlots-1,slot);
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
    if(t>=64){if(t==64)return pol==0?(v-1.0f)/126.0f*4.0f:(v-64.0f)/63.0f*2.0f;return pol==0?(v-1.0f)/126.0f*126.0f:(v-64.0f)/63.0f*63.0f;} // RATE: октавы; GATE: прибавка к длине
    if(t>=56)return pol==0?v/127.0f:(v-64.0f)/63.0f*0.5f; // MSEG OUT: 0..1 / дельта
    const float mx=plockMaxFor(t);
    return pol==0?v/127.0f*mx:(v-64.0f)/63.0f*(mx*0.5f);} // UNI: абсолют, BIP: дельта от текущего
void MonomachineNovaAudioProcessor::plockStep(int page,int step){
    const int p=juce::jlimit(0,15,page),st=juce::jlimit(0,7,step);
    plockOn.fill(false); // 1.7.3: пересчёт с нуля -- снятый/откатившийся слот отпускает цель сразу (без замороженной модуляции)
    for(int s=0;s<kPlockSlots;++s){
        auto& R=plockRun[static_cast<size_t>(s)];
        const int t=plockTargetI[static_cast<size_t>(s)].load();if(t<0||t>65){R.active=false;R.sliding=false;continue;}
        const int idx=p*8+st;const float v=plockVal[static_cast<size_t>(s*128+idx)].load();
        if(v>0.5f){ // лок: SLIDE на степе -- диагональ до следующего p-lock, тот останавливает и играет обычно
            R.active=true;R.sliding=false;R.now=plockScaled(t,s,v);
            plockOn[static_cast<size_t>(t)]=true;plockAbs[static_cast<size_t>(t)]=plockPolI[static_cast<size_t>(s)].load()==0;plockNow[static_cast<size_t>(t)]=R.now;
            if(plockSlideV[static_cast<size_t>(s*128+idx)].load()>0.5f){int j=-1;
                for(int i=idx+1;i<128;++i)if(plockVal[static_cast<size_t>(s*128+i)].load()>0.5f){j=i;break;}
                if(j>idx){R.sliding=true;R.from=R.now;R.to=plockScaled(t,s,plockVal[static_cast<size_t>(s*128+j)].load());R.span=j-idx;R.prog=0;}}
        }else if(R.active){
            if(R.sliding){R.prog=std::min(R.prog+1,R.span);R.now=R.from+(R.to-R.from)*(static_cast<float>(R.prog)/static_cast<float>(std::max(1,R.span)));
                plockNow[static_cast<size_t>(t)]=R.now;if(R.prog>=R.span)R.sliding=false;}
            else{R.active=false;plockOn[static_cast<size_t>(t)]=false;} // без лока параметр играет обычно
        }}}
void MonomachineNovaAudioProcessor::plockClearRuntime(){plockOn.fill(false);for(auto& R:plockRun){R.active=false;R.sliding=false;}} // 1.7.1
void MonomachineNovaAudioProcessor::plockFillPage(int slot,int page){ // 1.7.1: бинд сразу назначает степы страницы (текущее значение цели)
    if(slot<0||slot>=kPlockSlots||page<0||page>15)return;const int t=plockTargetI[static_cast<size_t>(slot)].load();if(t<0)return;
    const int pol=plockPolI[static_cast<size_t>(slot)].load();float v=64.0f;
    if(t<56)v=pol==0?juce::jlimit(1.0f,127.0f,base[static_cast<size_t>(t)]/plockMaxFor(t)*127.0f):64.0f;
    else if(t<64)v=pol==0?juce::jlimit(1.0f,127.0f,msegValue[static_cast<size_t>(t-56)]*127.0f):64.0f;
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
        plockTargetI[static_cast<size_t>(s)].store(juce::jlimit(-1,65,static_cast<int>(n.getProperty("target",-1))));
        plockPolI[static_cast<size_t>(s)].store(static_cast<int>(n.getProperty("pol",0))==1?1:0);
        auto fill=[this,s](const juce::String& csv,bool slide){int i=0;
            for(const auto& tok:juce::StringArray::fromTokens(csv,",","")){if(i>=128)break;
                (slide?plockSlideV:plockVal)[static_cast<size_t>(s*128+i)].store(juce::jlimit(0.0f,127.0f,tok.getFloatValue()));++i;}};
        fill(n.getProperty("v").toString(),false);fill(n.getProperty("sl").toString(),true);}
    plockClearRuntime();}
int MonomachineNovaAudioProcessor::addRouteFromSource(int src, uint8_t target){
    const int source=juce::jlimit(0,17,src); // 1.6.25: 18 источников (MSEG2/3 больше не зажимаются в 15)
    const int destination=juce::jlimit(0,82,static_cast<int>(target)); // 1.7.5: +PITCH/ROUTE
    // 1.6.5: если у этого источника уже есть маршрут на эту цель -- перезаписать
    // его (повторное перетаскивание не плодит маршруты), иначе занять первый
    // свободный слот, иначе слот 16.
    int slot=-1;
    for(int r=0;r<16;++r){
        auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");
        auto* dest=parameters.getRawParameterValue("r"+juce::String(r)+"_dest");
        auto* srcPar=parameters.getRawParameterValue("r"+juce::String(r)+"_src");
        if(on&&dest&&srcPar&&on->load()>0.5f&&juce::roundToInt(srcPar->load())==source&&juce::roundToInt(dest->load())==destination+1){slot=r;break;} // 1.6.32: dest хранится как цель+1
    }
    // 1.6.12: замок маршрута (LOCK/SOLO) защищает слот от перезаписи прицелом.
    if(slot<0)for(int r=0;r<16;++r){auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");auto* lk=parameters.getRawParameterValue("r"+juce::String(r)+"_lock");if(on&&on->load()<0.5f&&!(lk&&juce::roundToInt(lk->load())>0)){slot=r;break;}}
    if(slot<0)for(int r=0;r<16;++r){auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");if(on&&on->load()<0.5f){slot=r;break;}}
    if(slot<0)slot=15;
    auto set=[this](const juce::String& id,float value){if(auto* p=parameters.getParameter(id)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(value));p->endChangeGesture();}};
    set("r"+juce::String(slot)+"_src",static_cast<float>(source));
    set("r"+juce::String(slot)+"_dest",static_cast<float>(destination+1)); // 1.6.32: 0 зарезервирован под OFF
    set("r"+juce::String(slot)+"_depth",32.0f);
    set("r"+juce::String(slot)+"_mode",0.0f);
    set("r"+juce::String(slot)+"_on",1.0f);
    // 1.6.8: свежий маршрут всплывает на первую строку матрицы, чтобы его
    // было легко найти сразу после перетаскивания кнопки LFO.
    if(slot>0)moveModRoute(slot,0);
    return slot;
}
void MonomachineNovaAudioProcessor::moveModRoute(int from,int to){
    if(from<0||from>=16||to<0||to>=16||from==to)return;
    const char* fields[]={"on","src","dest","depth","mode","aux","aux_depth"}; // 1.6.34: + aux_depth -- перетаскивание/сортировка НЕ отрывает AUX DEPTH от строки
    for(const char* field:fields){
        auto* a=parameters.getParameter("r"+juce::String(from)+"_"+field);auto* b=parameters.getParameter("r"+juce::String(to)+"_"+field);
        if(!a||!b)continue;const float av=a->getValue(),bv=b->getValue();a->setValueNotifyingHost(bv);b->setValueNotifyingHost(av);
    }
}
void MonomachineNovaAudioProcessor::sortModRoutes(int column, bool descending){
    if(column<1||column>7)return; // 1.6.8/41: ON..AUX DEPTH (6 = AUX, 7 = AUX DEPTH)
    const char* fields[]{"on","src","dest","depth","mode","aux","aux_depth"}; // 1.6.34: + aux_depth (сортировка не отрывает колонки)
    struct RouteValue{std::array<float,7> values{};float key=0,tie=0;};
    std::array<RouteValue,16> rows{};
    for(int r=0;r<16;++r){
        for(int c=0;c<7;++c){if(auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[c]))rows[static_cast<size_t>(r)].values[static_cast<size_t>(c)]=p->getValue();}
        auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[column-1]);
        const float raw=p?p->convertFrom0to1(p->getValue()):0.0f; rows[static_cast<size_t>(r)].key=column==4?std::abs(raw):raw;
        if(auto* depth=parameters.getParameter("r"+juce::String(r)+"_depth"))rows[static_cast<size_t>(r)].tie=std::abs(depth->convertFrom0to1(depth->getValue()));
    }
    std::stable_sort(rows.begin(),rows.end(),[descending,column](const RouteValue& a,const RouteValue& b){if(a.key!=b.key)return descending?a.key>b.key:a.key<b.key;if(column!=4&&a.tie!=b.tie)return a.tie>b.tie;return false;});
    // 1.6.13: защита от краша хоста ( Ableton падал без ошибки при клике по шапке ):
    // пишем только ИЗМЕН ИВШИЕСЯ значения, аудио-поток на время сортировки подвешен.
    suspendProcessing(true);
    for(int r=0;r<16;++r)for(int c=0;c<7;++c)if(auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[c])){const float v=rows[static_cast<size_t>(r)].values[static_cast<size_t>(c)];if(std::abs(p->getValue()-v)>1e-6f)p->setValueNotifyingHost(v);} // 1.6.34: пишем ВСЕ 7 полей (aux/aux_depth больше не теряются)
    suspendProcessing(false);
}
void MonomachineNovaAudioProcessor::handleAsyncUpdate(){
    juce::ValueTree samples;{const juce::ScopedLock lock(stateLock);samples=pendingSamples;pendingSamples={};}
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
