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
    const char* ampIds[]{"amp_mode","amp_curve_a","amp_curve_d","amp_curve_r"};for(size_t i=0;i<4;++i)ampRaw[i]=parameters.getRawParameterValue(ampIds[i]);
    for(int i=0;i<monomachine::DspSectionCount;++i)modeRaw[static_cast<size_t>(i)]=parameters.getRawParameterValue(monomachine::dspModeParamId(i));
    for(size_t m=0;m<nova::machines().size();++m)for(int i=0;i<8;++i)
        if(nova::machines()[m].synthParams[static_cast<size_t>(i)].maxVal>0)synthRaw[m][static_cast<size_t>(i)]=parameters.getRawParameterValue(nova::machineParam(nova::machines()[m].id,i));
    for(int p=0;p<6;++p)for(int i=0;i<8;++i)pageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]=parameters.getRawParameterValue(nova::pageParam(p,i));
    const char* g[]={"level","bpm","host_sync","fx_mix","gate","arp_mode","arp_play","arp_speed","arp_range","arp_length","macro_x","macro_y","arp_on","arp_hold","arp_sync","arp_time","arp_grid","arp_wrap","arp_velocity_mode","arp_step","arp_step_velocity","arp_step_transpose","arp_step_hold","arp_step_page","arp_step_page_limit","arp_step_random","mseg_rate","mseg_sync","mseg_loop"};
    for(size_t i=0;i<GlobalCount;++i)globalRaw[i]=(isSynthVersion&&(i==Mix||i==Gate))?nullptr:parameters.getRawParameterValue(g[i]);
    const char* fields[]={"on","src","dest","depth","mode"};
    for(int r=0;r<16;++r)for(int i=0;i<5;++i)routeRaw[static_cast<size_t>(r)][static_cast<size_t>(i)]=parameters.getRawParameterValue("r"+juce::String(r)+"_"+fields[i]);
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
    machine.prepare(sr);chain.volumeReference=isSynthVersion?127.0f:64.0f;chain.prepare(sr);midiGateBlend.reset(sr,0.005);envelope.reset(sr);arp.reset(sr);matrix.reset();mseg.reset();msegPhase=0.0;msegValue=0.0f;liveStepVelocity=0.0f;liveStepTranspose=0.0f;liveArpGate=0.0f;selectedStepVelocity=1.0f;selectedStepTranspose=0.0f;selectedStepHold=0.0f;
    for(auto& l:lfos)l.reset();notes.fill({});pedal.fill(false);wheels.fill(8192);arpHeld.fill(false);
    currentKey=-1;serial=0;activeMachine=-1;lastArpMode=-1;ccX=ccY=-1;
    dryDelay.clear();previousLfo.fill(0);
    level.reset(sr,0.02);mix.reset(sr,0.02);velocity.reset(sr,0.005);velocity.setCurrentAndTargetValue(1);
    pitch.reset(sr,0);pitch.setCurrentAndTargetValue(60);prepared=true;snapshot();
    midiGateBlend.setCurrentAndTargetValue(global[Gate]>0.5f?1.0f:0.0f);
    level.setCurrentAndTargetValue(global[Level]/127.0f);mix.setCurrentAndTargetValue(isSynthVersion?1:global[Mix]/127);
    panicRequested.store(false);peak.store(0);
}
void MonomachineNovaAudioProcessor::snapshot(){
    for(int i=0;i<monomachine::DspSectionCount;++i){
        auto* ptr=modeRaw[static_cast<size_t>(i)];
        int idx = ptr ? juce::roundToInt(ptr->load()) : monomachine::dspModeMnm;
        idx = juce::jlimit(0, monomachine::dspModeCount-1, idx);
        // collapse filter-only modes for non-FILT sections
        if(i!=monomachine::DspFilter && monomachine::dspModeIsFilterOnly(idx)){
            idx = monomachine::dspModeMnm;
        }
        dspModes[static_cast<size_t>(i)] = idx;
    }
    // dspModeFilter constant used for collapsing check (kept for verification)
    (void)monomachine::dspModeFilter;
    envelope.configure(dspModes[monomachine::DspAmp]==monomachine::dspModeMnm?nova::AmpEnvelope::kKernelAlgorithm:juce::roundToInt(ampRaw[0]->load()),{ampRaw[1]->load(),ampRaw[2]->load(),ampRaw[3]->load()});
    machine.setModes(dspModes[monomachine::DspSynt],dspModes[monomachine::DspDist]);
    chain.setModes(dspModes[monomachine::DspFilter],dspModes[monomachine::DspDist],dspModes[monomachine::DspDelay],dspModes[monomachine::DspRouting]);
    for(size_t i=0;i<GlobalCount;++i)global[i]=globalRaw[i]?globalRaw[i]->load(std::memory_order_relaxed):0;
    int selected=juce::jlimit(0,static_cast<int>(nova::machines().size())-1,juce::roundToInt(machineRaw->load()));
    for(int i=0;i<8;++i){auto* ptr=synthRaw[static_cast<size_t>(selected)][static_cast<size_t>(i)];base[static_cast<size_t>(i)]=ptr?ptr->load():0;}
    for(int p=0;p<3;++p)for(int i=0;i<8;++i)base[static_cast<size_t>(8+p*8+i)]=pageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]->load();
    for(size_t p=0;p<3;++p)for(size_t i=0;i<8;++i){const float value=pageRaw[p+3][i]->load();const float maximum=i==0||i==1?7.0f:i==2?3.0f:i==3?10.0f:i==4?6.0f:127.0f;effectiveLfoParams[p][i]=std::clamp(effectiveLfoParams[p][i]+value-lfoParams[p][i],0.0f,maximum);lfoParams[p][i]=value;}
    bpm=global[Bpm];if(global[HostSync]>0.5f)if(auto* play=getPlayHead())if(auto position=play->getPosition())if(auto tempo=position->getBpm())if(std::isfinite(*tempo))bpm=std::clamp(*tempo,30.0,300.0);
    tempoDisplay.store(bpm);
    if(global[MacroX]!=previousX){ccX=-1;previousX=global[MacroX];}if(global[MacroY]!=previousY){ccY=-1;previousY=global[MacroY];}
    matrix.setParamX(static_cast<uint8_t>(ccX>=0?ccX:global[MacroX]));matrix.setParamY(static_cast<uint8_t>(ccY>=0?ccY:global[MacroY]));
    for(size_t r=0;r<16;++r)matrix.configureRouting(r,routeRaw[r][0]->load()>0.5f,static_cast<monomachine::ModSource>(juce::jlimit(0,15,juce::roundToInt(routeRaw[r][1]->load()))),static_cast<uint8_t>(juce::jlimit(0,55,juce::roundToInt(routeRaw[r][2]->load()))),static_cast<int8_t>(juce::jlimit(-64,63,juce::roundToInt(routeRaw[r][3]->load()))),(juce::roundToInt(routeRaw[r][4]->load())==1?monomachine::ModPolarity::Bipolar:monomachine::ModPolarity::Unipolar));
    matrix.setPitchWheel((wheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]-8192.0f)/8192.0f);
    matrix.setModWheel(modWheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
    matrix.setAftertouch(aftertouch[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
    matrix.setMsegOutput(msegValue);
    matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:global[ArpStepVelocity]/127.0f,liveStepTranspose!=0.0f?liveStepTranspose:global[ArpStepTranspose]/24.0f,liveArpGate>0.0f?liveArpGate:global[ArpStepHold]);
    matrix.setArpRate(global[ArpGrid]/15.0f);
    midiGateBlend.setTargetValue(global[Gate]>0.5f?1.0f:0.0f);
    level.setTargetValue(global[Level]/127.0f);mix.setTargetValue(isSynthVersion?1:global[Mix]/127);
    global[ArpMode]=global[ArpOn]>0.5f?(global[ArpHold]>0.5f?2.0f:1.0f):0.0f;
    auto mode=static_cast<int>(global[ArpMode]);
    monomachine::MonomachineArpeggiator::ArpSettings settings;
    settings.mode=static_cast<monomachine::MonomachineArpeggiator::Mode>(mode);settings.play=static_cast<monomachine::MonomachineArpeggiator::Play>(static_cast<int>(global[ArpPlay]));
    const double ticks[]{96,48,24,12,6,3,1.5,16,8,4,2,1,36,18,9,4.5};settings.speed=ticks[juce::jlimit(0,15,static_cast<int>(global[ArpGrid]))];
    settings.sync=global[ArpSync]>0.5f;settings.milliseconds=global[ArpTime];settings.range=static_cast<uint8_t>(global[ArpRange]);settings.noteLength=static_cast<uint8_t>(global[ArpLength]);settings.wrap=static_cast<uint8_t>(juce::jlimit(1,16,juce::roundToInt(global[ArpWrap])));settings.velocityMode=static_cast<monomachine::MonomachineArpeggiator::VelocityMode>(juce::jlimit(0,4,juce::roundToInt(global[ArpVelocityMode])));settings.stepPage=static_cast<uint8_t>(juce::jlimit(0,15,juce::roundToInt(global[ArpStepPage])));settings.stepPageLimit=static_cast<uint8_t>(juce::jlimit(1,16,juce::roundToInt(global[ArpStepPageLimit])));settings.stepRandom=juce::jlimit(0.0f,1.0f,global[ArpStepRandom]/100.0f);
    for(int pg=0;pg<16;++pg)for(int st=0;st<8;++st){const auto prefix="arp_s"+juce::String(pg)+"_"+juce::String(st)+"_";auto& stepValue=settings.steps[static_cast<size_t>(pg*8+st)];if(auto* h=parameters.getRawParameterValue(prefix+"hold"))stepValue.hold=h->load()>0.5f;if(auto* t=parameters.getRawParameterValue(prefix+"transpose"))stepValue.transpose=static_cast<int8_t>(juce::jlimit(-24,24,juce::roundToInt(t->load())));if(auto* v=parameters.getRawParameterValue(prefix+"velocity"))stepValue.velocity=static_cast<uint8_t>(juce::jlimit(0,127,juce::roundToInt(v->load())));}
    const auto& selectedStep=settings.steps[static_cast<size_t>(settings.stepPage*8+juce::jlimit(0,7,juce::roundToInt(global[ArpStep])))];selectedStepVelocity=selectedStep.velocity/127.0f;selectedStepTranspose=selectedStep.transpose/24.0f;selectedStepHold=selectedStep.hold?1.0f:0.0f;matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:selectedStepVelocity,liveStepTranspose!=0.0f?liveStepTranspose:selectedStepTranspose,liveArpGate>0.0f?liveArpGate:selectedStepHold);
    if(mode!=lastArpMode){arp.reset(sr,bpm);arpHeld.fill(false);release();}arp.setSettings(settings);arp.setTempo(bpm);
    if(mode!=lastArpMode){lastArpMode=mode;syncArpNotes();if(mode==0&&currentKey>=0)trigger(currentKey%128,notes[static_cast<size_t>(currentKey)].velocity,currentKey/128);}
    if(selected!=activeMachine){activeMachine=selected;effectiveLfoParams=lfoParams;smoothedParams=base;dryDelay.clear();machine.clear();chain.clear();std::array<float,8> p;std::copy_n(base.data(),8,p.data());machine.set(nova::machines()[static_cast<size_t>(selected)].id,p);
        const int latency=isSynthVersion?0:machine.fxLatency();if(getLatencySamples()!=latency)setLatencySamples(latency);
        if(currentKey>=0&&mode==0)trigger(currentKey%128,notes[static_cast<size_t>(currentKey)].velocity,currentKey/128);}
}
void MonomachineNovaAudioProcessor::panic(){
    notes.fill({});pedal.fill(false);wheels.fill(8192);arpHeld.fill(false);currentKey=-1;serial=0;ccX=ccY=-1;
    envelope.reset(sr);dryDelay.clear();machine.clear();chain.clear();arp.reset(sr,bpm);matrix.reset();msegPhase=0.0;msegValue=0.0f;liveStepVelocity=0.0f;liveStepTranspose=0.0f;liveArpGate=0.0f;selectedStepVelocity=1.0f;selectedStepTranspose=0.0f;selectedStepHold=0.0f;for(auto& l:lfos)l.reset();
}
void MonomachineNovaAudioProcessor::trigger(int note,float vel,int channel){
    soundingNote=note;soundingChannel=channel;
    msegPhase=0.0; msegValue=mseg.value(0.0f); liveArpGate=1.0f;
    const float glide=base[15]<=0?0:nova::norm(base[15])*nova::norm(base[15])*2;
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
        if(global[ArpMode]>0.5f)for(int k=0;k<3;++k){auto event=arp.poll();if(!event.triggered)break;if(event.isNoteOff){liveArpGate=0.0f;release();}else{liveStepVelocity=event.velocity/127.0f;liveStepTranspose=0.0f;liveArpGate=1.0f;trigger(event.note,event.velocity/127.0f,currentKey>=0?currentKey/128:soundingChannel);}}
        int n=std::min(8,length);if(global[ArpMode]>0.5f)n=std::min(n,std::max(1,arp.samplesUntilEvent()));
        msegValue=mseg.value(static_cast<float>(msegPhase));
        matrix.setMsegOutput(msegValue);
        matrix.setPitchWheel((wheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]-8192.0f)/8192.0f);
        matrix.setModWheel(modWheels[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
        matrix.setAftertouch(aftertouch[static_cast<size_t>(juce::jlimit(0,15,soundingChannel))]/127.0f);
        matrix.setStepValues(liveStepVelocity>0.0f?liveStepVelocity:selectedStepVelocity,liveStepTranspose!=0.0f?liveStepTranspose:selectedStepTranspose,liveArpGate>0.0f?liveArpGate:selectedStepHold);
        matrix.setArpRate(global[ArpGrid]/15.0f);
        std::array<float,56> before{},after{};std::copy(base.begin(),base.end(),before.begin());
        for(size_t j=0;j<3;++j)std::copy(lfoParams[j].begin(),lfoParams[j].end(),before.begin()+32+j*8);
        matrix.setLfoOutputs(previousLfo[0],previousLfo[1],previousLfo[2]);matrix.evaluate(before,after);
        float pitchMod=0;
        for(size_t j=0;j<3;++j){const auto& p=effectiveLfoParams[j];const float delta=previousLfo[j]*(p[7]-64);
            const int page=juce::jlimit(0,7,static_cast<int>(p[0])),dest=juce::jlimit(0,7,static_cast<int>(p[1]));
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
        machine.setPitch(notePitch,pitchMod);machine.set(def.id,synthParams);chain.set(modulated);
        for(size_t i=0;i<4;++i)envelope.p[i]=modulated[8+i];
        if(isSynthVersion){std::fill_n(l.data(),n,0.0f);std::fill_n(r.data(),n,0.0f);if(envelope.stage!=0)machine.render(l.data(),r.data(),n,true);}
        else{std::copy_n(buffer.getReadPointer(0,start),n,l.data());std::copy_n(buffer.getReadPointer(stereo?1:0,start),n,r.data());for(int i=0;i<n;++i)dryDelay.process(l[static_cast<size_t>(i)],r[static_cast<size_t>(i)],machine.fxLatency(),dryL[static_cast<size_t>(i)],dryR[static_cast<size_t>(i)]);machine.render(l.data(),r.data(),n,false);}
        for(int i=0;i<n;++i){float a=envelope.tick(),vel=velocity.getNextValue();amp[static_cast<size_t>(i)]=isSynthVersion?a*vel:1+(a*vel-1)*midiGateBlend.getNextValue();}
        chain.process(l.data(),r.data(),amp.data(),n);
        for(int i=0;i<n;++i){size_t x=static_cast<size_t>(i);float gain=level.getNextValue()/(isSynthVersion?1.0f:100.0f/127.0f),wet=mix.getNextValue();
            const float dryGate=isSynthVersion?1.0f:amp[x];
            float a=gain*(l[x]*wet+dryL[x]*dryGate*(1-wet)),b=gain*(r[x]*wet+dryR[x]*dryGate*(1-wet));
            if(!std::isfinite(a))a=0;if(!std::isfinite(b))b=0;
            buffer.setSample(0,start+i,stereo?a:(a+b)*0.5f);if(stereo)buffer.setSample(1,start+i,b);blockPeak=std::max(blockPeak,std::max(std::abs(a),std::abs(b)));}
        if(global[ArpMode]>0.5f)arp.advance(n);
        const double msegRate=std::max(0.001,static_cast<double>(global[MsegRate])*(global[MsegSync]>0.5f?bpm/60.0:1.0));
        msegPhase+=msegRate*static_cast<double>(n)/sr;
        if(msegPhase>=1.0){if(global[MsegLoop]>0.5f)msegPhase=std::fmod(msegPhase,1.0);else msegPhase=1.0;}
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
    render(b,cursor,b.getNumSamples()-cursor);if(isSynthVersion)events.clear();peak.store(blockPeak);playingSampleSlot.store(activeMachine>=0&&envelope.stage!=0&&nova::machines()[static_cast<size_t>(activeMachine)].id==7?machine.bbox.playingSlot():-1);
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
void MonomachineNovaAudioProcessor::resetSamples(bool modern){
    monomachine::MonomachineBBox defaults(modern);
    for(size_t i=0;i<24;++i){auto slot=defaults.getSlots()[i];juce::MemoryBlock old;if(!modern)old=juce::MemoryBlock(slot.data.data(),slot.data.size()*sizeof(float));{const juce::ScopedLock lock(sampleLock);sampleNames[i]=slot.name;machine.bbox.swapSample(i,slot);sampleData[i].swapWith(old);}}
    requestPanic();
}
void MonomachineNovaAudioProcessor::getStateInformation(juce::MemoryBlock& out){
    auto state=parameters.copyState();state.setProperty("schema",8,nullptr);auto samples=juce::ValueTree("Samples");
    for(size_t i=0;i<24;++i){juce::MemoryBlock data;juce::String name;{const juce::ScopedLock lock(sampleLock);data=sampleData[i];name=sampleNames[i];}
        if(data.getSize()){auto node=juce::ValueTree("Sample");node.setProperty("slot",static_cast<int>(i),nullptr);node.setProperty("name",name,nullptr);node.setProperty("pcm44100",data.toBase64Encoding(),nullptr);samples.addChild(node,-1,nullptr);}}
    state.addChild(samples,-1,nullptr);
    auto msegState=juce::ValueTree("MSEG");msegState.setProperty("count",msegPointCount(),nullptr);
    for(int i=0;i<msegPointCount();++i){auto point=juce::ValueTree("POINT");point.setProperty("x",msegPointX(i),nullptr);point.setProperty("y",msegPointY(i),nullptr);msegState.addChild(point,-1,nullptr);}
    state.addChild(msegState,-1,nullptr);
    if(auto xml=state.createXml())copyXmlToBinary(*xml,out);
}
void MonomachineNovaAudioProcessor::setStateInformation(const void* data,int size){
    if(data==nullptr||size<=0||size>64*1024*1024)return;
    if(auto xml=getXmlFromBinary(data,size))if(xml->hasTagName(parameters.state.getType().toString())){
        auto state=juce::ValueTree::fromXml(*xml);auto samples=state.getChildWithName("Samples");state.removeChild(samples,nullptr);
        auto savedMseg=state.getChildWithName("MSEG");state.removeChild(savedMseg,nullptr);
        const int schema=static_cast<int>(state.getProperty("schema",2));
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
        if(schema<8){for(int section=0;section<monomachine::DspSectionCount;++section){const char* id=monomachine::dspModeParamId(section);if(!state.getChildWithProperty("id",id).isValid()){auto node=juce::ValueTree("PARAM");node.setProperty("id",id,nullptr);node.setProperty("value",static_cast<float>(monomachine::dspModeOld),nullptr);state.addChild(node,-1,nullptr);}}}state.setProperty("schema",8,nullptr);
        parameters.replaceState(state);
        if(savedMseg.isValid()){mseg.reset();const int n=juce::jlimit(2,monomachine::MSEG::kMaxPoints,static_cast<int>(savedMseg["count"]));for(int i=0;i<n&&i<savedMseg.getNumChildren();++i){auto point=savedMseg.getChild(i);mseg.setPoint(i,static_cast<float>(point["x"]),static_cast<float>(point["y"]));}mseg.setPointCount(n);}
        {const juce::ScopedLock lock(stateLock);pendingSamples=samples;}triggerAsyncUpdate();requestPanic();
    }
}
void MonomachineNovaAudioProcessor::addMsegRoute(uint8_t target){
    const int destination=juce::jlimit(0,55,static_cast<int>(target));
    int slot=-1;
    for(int r=0;r<16;++r){auto* on=parameters.getRawParameterValue("r"+juce::String(r)+"_on");if(on&&on->load()<0.5f){slot=r;break;}}
    if(slot<0)slot=15;
    auto set=[this](const juce::String& id,float value){if(auto* p=parameters.getParameter(id)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(value));p->endChangeGesture();}};
    set("r"+juce::String(slot)+"_src",static_cast<float>(static_cast<int>(monomachine::ModSource::MSEG)));
    set("r"+juce::String(slot)+"_dest",static_cast<float>(destination));
    set("r"+juce::String(slot)+"_depth",32.0f);
    set("r"+juce::String(slot)+"_mode",0.0f);
    set("r"+juce::String(slot)+"_on",1.0f);
}
void MonomachineNovaAudioProcessor::moveModRoute(int from,int to){
    if(from<0||from>=16||to<0||to>=16||from==to)return;
    const char* fields[]={"on","src","dest","depth","mode"};
    for(const char* field:fields){
        auto* a=parameters.getParameter("r"+juce::String(from)+"_"+field);auto* b=parameters.getParameter("r"+juce::String(to)+"_"+field);
        if(!a||!b)continue;const float av=a->getValue(),bv=b->getValue();a->setValueNotifyingHost(bv);b->setValueNotifyingHost(av);
    }
}
void MonomachineNovaAudioProcessor::sortModRoutes(int column, bool descending){
    if(column<2||column>5)return;
    const char* fields[]={"on","src","dest","depth","mode"};
    struct RouteValue{std::array<float,5> values{};float key=0,tie=0;};
    std::array<RouteValue,16> rows{};
    for(int r=0;r<16;++r){
        for(int c=0;c<5;++c){if(auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[c]))rows[static_cast<size_t>(r)].values[static_cast<size_t>(c)]=p->getValue();}
        auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[column-1]);
        const float raw=p?p->convertFrom0to1(p->getValue()):0.0f; rows[static_cast<size_t>(r)].key=column==4?std::abs(raw):raw;
        if(auto* depth=parameters.getParameter("r"+juce::String(r)+"_depth"))rows[static_cast<size_t>(r)].tie=std::abs(depth->convertFrom0to1(depth->getValue()));
    }
    std::stable_sort(rows.begin(),rows.end(),[descending,column](const RouteValue& a,const RouteValue& b){if(a.key!=b.key)return descending?a.key>b.key:a.key<b.key;if(column!=4&&a.tie!=b.tie)return a.tie>b.tie;return false;});
    for(int r=0;r<16;++r)for(int c=0;c<5;++c)if(auto* p=parameters.getParameter("r"+juce::String(r)+"_"+fields[c]))p->setValueNotifyingHost(rows[static_cast<size_t>(r)].values[static_cast<size_t>(c)]);
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
