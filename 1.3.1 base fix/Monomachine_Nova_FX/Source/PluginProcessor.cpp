#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout MonomachineNovaAudioProcessor::createLayout(){
    juce::AudioProcessorValueTreeState::ParameterLayout result;
    for(const auto& s:nova::specs()){
        if(s.choices.isNotEmpty())result.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(s.id,1),s.name,juce::StringArray::fromTokens(s.choices,"|",""),static_cast<int>(s.def)));
        else result.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(s.id,1),s.name,juce::NormalisableRange<float>(s.lo,s.hi,s.step),s.def));
    }
    return result;
}
MonomachineNovaAudioProcessor::MonomachineNovaAudioProcessor():AudioProcessor(isSynthVersion?
    BusesProperties().withOutput("Output",juce::AudioChannelSet::stereo(),true):
    BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),
    parameters(*this,nullptr,isSynthVersion?"NovaUnifiedSynth":"NovaUnifiedFX",createLayout()){
    machineRaw=parameters.getRawParameterValue("machine");
    for(size_t m=0;m<nova::machines().size();++m)for(int i=0;i<8;++i)
        if(nova::machines()[m].synthParams[static_cast<size_t>(i)].maxVal>0)synthRaw[m][static_cast<size_t>(i)]=parameters.getRawParameterValue(nova::machineParam(nova::machines()[m].id,i));
    for(int p=0;p<6;++p)for(int i=0;i<8;++i)pageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]=parameters.getRawParameterValue(nova::pageParam(p,i));
    const char* g[]={"level","bpm","host_sync","fx_mix","gate","arp_mode","arp_play","arp_speed","arp_range","arp_length","macro_x","macro_y"};
    for(size_t i=0;i<GlobalCount;++i)globalRaw[i]=(isSynthVersion&&(i==Mix||i==Gate))?nullptr:parameters.getRawParameterValue(g[i]);
    const char* fields[]={"on","src","dest","depth"};
    for(int r=0;r<8;++r)for(int i=0;i<4;++i)routeRaw[static_cast<size_t>(r)][static_cast<size_t>(i)]=parameters.getRawParameterValue("r"+juce::String(r)+"_"+fields[i]);
    for(int i=0;i<10;++i)sampleNames[static_cast<size_t>(i)]=machine.bbox.getSlots()[static_cast<size_t>(i)].name;
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
    machine.prepare(sr);chain.prepare(sr);envelope.reset(sr);arp.reset(sr);matrix.reset();
    for(auto& l:lfos)l.reset();notes.fill({});pedal.fill(false);wheels.fill(8192);arpHeld.fill(false);
    currentKey=-1;serial=0;activeMachine=-1;lastArpMode=-1;ccX=ccY=-1;
    setLatencySamples(isSynthVersion?0:machine.fxLatency()); // FX-CHORUS native core runs through a resampling adapter
    level.reset(sr,0.02);mix.reset(sr,0.02);velocity.reset(sr,0.005);velocity.setCurrentAndTargetValue(1);
    pitch.reset(sr,0);pitch.setCurrentAndTargetValue(60);prepared=true;snapshot();
    level.setCurrentAndTargetValue(global[Level]/127.0f);mix.setCurrentAndTargetValue(isSynthVersion?1:global[Mix]/127);
    panicRequested.store(false);peak.store(0);
}
void MonomachineNovaAudioProcessor::snapshot(){
    for(size_t i=0;i<GlobalCount;++i)global[i]=globalRaw[i]?globalRaw[i]->load(std::memory_order_relaxed):0;
    int selected=juce::jlimit(0,static_cast<int>(nova::machines().size())-1,juce::roundToInt(machineRaw->load()));
    for(int i=0;i<8;++i){auto* ptr=synthRaw[static_cast<size_t>(selected)][static_cast<size_t>(i)];base[static_cast<size_t>(i)]=ptr?ptr->load():0;}
    for(int p=0;p<3;++p)for(int i=0;i<8;++i)base[static_cast<size_t>(8+p*8+i)]=pageRaw[static_cast<size_t>(p)][static_cast<size_t>(i)]->load();
    for(int p=0;p<3;++p)for(int i=0;i<8;++i)lfoParams[static_cast<size_t>(p)][static_cast<size_t>(i)]=pageRaw[static_cast<size_t>(p+3)][static_cast<size_t>(i)]->load();
    bpm=global[Bpm];if(global[HostSync]>0.5f)if(auto* play=getPlayHead())if(auto position=play->getPosition())if(auto tempo=position->getBpm())if(std::isfinite(*tempo))bpm=std::clamp(*tempo,30.0,300.0);
    tempoDisplay.store(bpm);
    if(global[MacroX]!=previousX){ccX=-1;previousX=global[MacroX];}if(global[MacroY]!=previousY){ccY=-1;previousY=global[MacroY];}
    matrix.setParamX(static_cast<uint8_t>(ccX>=0?ccX:global[MacroX]));matrix.setParamY(static_cast<uint8_t>(ccY>=0?ccY:global[MacroY]));
    for(size_t r=0;r<8;++r)matrix.configureRouting(r,routeRaw[r][0]->load()>0.5f,static_cast<monomachine::ModSource>(juce::roundToInt(routeRaw[r][1]->load())),static_cast<uint8_t>(routeRaw[r][2]->load()),static_cast<int8_t>(routeRaw[r][3]->load()));
    level.setTargetValue(global[Level]/127.0f);mix.setTargetValue(isSynthVersion?1:global[Mix]/127);
    auto mode=static_cast<int>(global[ArpMode]);
    monomachine::MonomachineArpeggiator::ArpSettings settings;
    settings.mode=static_cast<monomachine::MonomachineArpeggiator::Mode>(mode);settings.play=static_cast<monomachine::MonomachineArpeggiator::Play>(static_cast<int>(global[ArpPlay]));
    settings.speed=static_cast<uint8_t>(global[ArpSpeed]);settings.range=static_cast<uint8_t>(global[ArpRange]);settings.noteLength=static_cast<uint8_t>(global[ArpLength]);
    if(mode!=lastArpMode){arp.reset(sr,bpm);arpHeld.fill(false);release();}arp.setSettings(settings);arp.setTempo(bpm);
    if(mode!=lastArpMode){lastArpMode=mode;syncArpNotes();if(mode==0&&currentKey>=0)trigger(currentKey%128,notes[static_cast<size_t>(currentKey)].velocity,currentKey/128);}
    if(selected!=activeMachine){activeMachine=selected;setLatencySamples(isSynthVersion?0:machine.fxLatency());machine.clear();chain.clear();std::array<float,8> p;std::copy_n(base.data(),8,p.data());machine.set(nova::machines()[static_cast<size_t>(selected)].id,p);
        if(currentKey>=0&&mode==0)trigger(currentKey%128,notes[static_cast<size_t>(currentKey)].velocity,currentKey/128);}
}
void MonomachineNovaAudioProcessor::panic(){
    notes.fill({});pedal.fill(false);wheels.fill(8192);arpHeld.fill(false);currentKey=-1;serial=0;ccX=ccY=-1;
    envelope.reset(sr);machine.clear();chain.clear();arp.reset(sr,bpm);matrix.reset();for(auto& l:lfos)l.reset();
}
void MonomachineNovaAudioProcessor::trigger(int note,float vel,int channel){
    soundingNote=note;soundingChannel=channel;
    const float glide=base[15]<=0?0:nova::norm(base[15])*nova::norm(base[15])*2;
    pitch.reset(sr,glide);if(envelope.stage==0||glide<=0)pitch.setCurrentAndTargetValue(static_cast<float>(note));else pitch.setTargetValue(static_cast<float>(note));
    machine.on(note);envelope.on();chain.trigger();velocity.setTargetValue(vel);
    for(size_t i=0;i<3;++i)lfos[i].trigger(lfoParams[i]);
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
    else if(m.isPitchWheel())wheels[ch]=m.getPitchWheelValue();
    else if(m.isController()){
        if(m.getControllerNumber()==64){pedal[ch]=m.getControllerValue()>=64;if(!pedal[ch]){for(size_t i=0;i<128;++i)if(!notes[ch*128+i].down)notes[ch*128+i].order=0;selectNotes(false);}}
        if(m.getControllerNumber()==1){ccX=static_cast<float>(m.getControllerValue());matrix.setParamX(static_cast<uint8_t>(ccX));}
        if(m.getControllerNumber()==11){ccY=static_cast<float>(m.getControllerValue());matrix.setParamY(static_cast<uint8_t>(ccY));}
    }
}
void MonomachineNovaAudioProcessor::render(juce::AudioBuffer<float>& buffer,int start,int length){
    std::array<float,32> l{},r{},dryL{},dryR{},amp{};bool stereo=buffer.getNumChannels()>1;
    while(length>0){
        if(global[ArpMode]>0.5f)for(int k=0;k<3;++k){auto event=arp.poll();if(!event.triggered)break;if(event.isNoteOff)release();else trigger(event.note,event.velocity/127.0f,currentKey>=0?currentKey/128:soundingChannel);}
        int n=std::min(32,length);if(global[ArpMode]>0.5f)n=std::min(n,std::max(1,arp.samplesUntilEvent()));
        std::array<float,3> lfoValues{};std::array<float,32> modulated=base;float pitchMod=0;
        for(size_t j=0;j<3;++j){auto& p=lfoParams[j];float v=lfos[j].process(p,sr,bpm,n);lfoValues[j]=v;float depth=(p[7]-64)/64;
            int page=static_cast<int>(p[0]),dest=juce::jlimit(0,7,static_cast<int>(p[1]));
            if(page==0){const float ranges[]={1,2,3,5,7,12,24,36};pitchMod+=v*depth*ranges[dest];}
            else modulated[static_cast<size_t>((page-1)*8+dest)]+=v*depth*64;}
        matrix.setLfoOutputs(lfoValues[0],lfoValues[1],lfoValues[2]);
        std::array<uint8_t,32> before{},after{};
        for(size_t i=0;i<32;++i)before[i]=static_cast<uint8_t>(juce::jlimit(0,127,juce::roundToInt(modulated[i])));
        matrix.evaluate(before,after);for(size_t i=0;i<32;++i)modulated[i]=after[i];
        std::array<float,8> synthParams{};const auto& def=nova::machines()[static_cast<size_t>(activeMachine)];
        for(size_t i=0;i<8;++i)synthParams[i]=std::clamp(modulated[i],static_cast<float>(def.synthParams[i].minVal),static_cast<float>(def.synthParams[i].maxVal));
        float notePitch=pitch.getNextValue();if(n>1)pitch.skip(n-1);
        pitchMod+=(wheels[static_cast<size_t>(soundingChannel)]-8192)/8192.0f*2;
        machine.setPitch(notePitch,pitchMod);machine.set(def.id,synthParams);chain.set(modulated);
        for(size_t i=0;i<4;++i)envelope.p[i]=modulated[8+i];
        if(isSynthVersion){std::fill_n(l.data(),n,0);std::fill_n(r.data(),n,0);if(envelope.stage!=0)machine.render(l.data(),r.data(),n,true);}
        else{std::copy_n(buffer.getReadPointer(0,start),n,l.data());std::copy_n(buffer.getReadPointer(stereo?1:0,start),n,r.data());std::copy_n(l.data(),n,dryL.data());std::copy_n(r.data(),n,dryR.data());machine.render(l.data(),r.data(),n,false);}
        for(int i=0;i<n;++i){float a=envelope.tick(),vel=velocity.getNextValue();amp[static_cast<size_t>(i)]=(isSynthVersion||global[Gate]>0.5f)?a*vel:1;}
        chain.process(l.data(),r.data(),amp.data(),n);
        for(int i=0;i<n;++i){size_t x=static_cast<size_t>(i);float gain=level.getNextValue(),wet=mix.getNextValue();float a=gain*(l[x]*wet+dryL[x]*(1-wet)),b=gain*(r[x]*wet+dryR[x]*(1-wet));
            if(!std::isfinite(a))a=0;if(!std::isfinite(b))b=0;
            buffer.setSample(0,start+i,stereo?a:(a+b)*0.5f);if(stereo)buffer.setSample(1,start+i,b);blockPeak=std::max(blockPeak,std::max(std::abs(a),std::abs(b)));}
        if(global[ArpMode]>0.5f)arp.advance(n);start+=n;length-=n;
    }
}
void MonomachineNovaAudioProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer& events){
    juce::ScopedNoDenormals denormals;
    const juce::ScopedTryLock lock(sampleLock);
    if(!lock.isLocked()||!prepared||b.getNumChannels()==0){b.clear();events.clear();return;}
    if(panicRequested.exchange(false))panic();snapshot();blockPeak=0;int cursor=0;
    for(auto metadata:events){int pos=juce::jlimit(cursor,b.getNumSamples(),metadata.samplePosition);render(b,cursor,pos-cursor);
        if(metadata.numBytes>0&&metadata.numBytes<=3)midi(juce::MidiMessage(metadata.data,metadata.numBytes));cursor=pos;}
    render(b,cursor,b.getNumSamples()-cursor);events.clear();peak.store(blockPeak);
}

juce::String MonomachineNovaAudioProcessor::loadSample(int slot,const juce::File& file){
    if(slot<0||slot>=10)return "Invalid slot";
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
    juce::MemoryBlock blob(ready.data.data(),ready.data.size()*sizeof(float));juce::String name(ready.name);
    {const juce::ScopedLock lock(sampleLock);machine.bbox.swapSample(static_cast<size_t>(slot),ready);sampleData[static_cast<size_t>(slot)].swapWith(blob);sampleNames[static_cast<size_t>(slot)]=name;}
    return {}; // Retired buffers are destroyed here, outside the audio callback and lock.
}
juce::String MonomachineNovaAudioProcessor::sampleName(int slot){const juce::ScopedLock lock(sampleLock);return sampleNames[static_cast<size_t>(juce::jlimit(0,9,slot))];}
void MonomachineNovaAudioProcessor::resetSamples(){
    monomachine::MonomachineBBox defaults;
    for(size_t i=0;i<10;++i){auto slot=defaults.getSlots()[i];juce::MemoryBlock old;{const juce::ScopedLock lock(sampleLock);sampleNames[i]=slot.name;machine.bbox.swapSample(i,slot);sampleData[i].swapWith(old);}}
    requestPanic();
}
void MonomachineNovaAudioProcessor::getStateInformation(juce::MemoryBlock& out){
    auto state=parameters.copyState();state.setProperty("schema",2,nullptr);auto samples=juce::ValueTree("Samples");
    for(size_t i=0;i<10;++i){juce::MemoryBlock data;juce::String name;{const juce::ScopedLock lock(sampleLock);data=sampleData[i];name=sampleNames[i];}
        if(data.getSize()){auto node=juce::ValueTree("Sample");node.setProperty("slot",static_cast<int>(i),nullptr);node.setProperty("name",name,nullptr);node.setProperty("pcm44100",data.toBase64Encoding(),nullptr);samples.addChild(node,-1,nullptr);}}
    state.addChild(samples,-1,nullptr);if(auto xml=state.createXml())copyXmlToBinary(*xml,out);
}
void MonomachineNovaAudioProcessor::setStateInformation(const void* data,int size){
    if(data==nullptr||size<=0||size>32*1024*1024)return;
    if(auto xml=getXmlFromBinary(data,size))if(xml->hasTagName(parameters.state.getType().toString())){
        auto state=juce::ValueTree::fromXml(*xml);auto samples=state.getChildWithName("Samples");state.removeChild(samples,nullptr);
        parameters.replaceState(state);{const juce::ScopedLock lock(stateLock);pendingSamples=samples;}triggerAsyncUpdate();requestPanic();
    }
}
void MonomachineNovaAudioProcessor::handleAsyncUpdate(){
    juce::ValueTree samples;{const juce::ScopedLock lock(stateLock);samples=pendingSamples;pendingSamples={};}
    resetSamples();
    for(int i=0;i<samples.getNumChildren();++i){auto node=samples.getChild(i);int index=node["slot"];if(index<0||index>=10)continue;
        juce::MemoryBlock blob;if(!blob.fromBase64Encoding(node["pcm44100"].toString())||blob.getSize()<sizeof(float)||blob.getSize()>220500*sizeof(float)||blob.getSize()%sizeof(float)!=0)continue;
        monomachine::MonomachineBBox::SampleSlot ready;ready.loaded=true;ready.originalSampleRate=44100;ready.name=node["name"].toString().toStdString();ready.data.resize(blob.getSize()/sizeof(float));std::memcpy(ready.data.data(),blob.getData(),blob.getSize());
        for(auto& v:ready.data)v=std::isfinite(v)?std::clamp(v,-4.0f,4.0f):0;
        const juce::String name(ready.name);{const juce::ScopedLock lock(sampleLock);machine.bbox.swapSample(static_cast<size_t>(index),ready);sampleData[static_cast<size_t>(index)].swapWith(blob);sampleNames[static_cast<size_t>(index)]=name;}}
}
juce::AudioProcessorEditor* MonomachineNovaAudioProcessor::createEditor(){return new MonomachineNovaAudioProcessorEditor(*this);}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new MonomachineNovaAudioProcessor();}
