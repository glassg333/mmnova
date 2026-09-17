#include "PluginProcessor.h"
#include "PluginEditor.h"
juce::AudioProcessorValueTreeState::ParameterLayout MonoJuiceAudioProcessor::layout(){juce::AudioProcessorValueTreeState::ParameterLayout l;
 for(int slot=0;slot<5;slot++){const auto prefix="Slot "+juce::String(slot+1)+" / ";
  for(const auto& p:model::params)l.add(std::make_unique<juce::AudioParameterFloat>(model::id(slot,p.id),prefix+p.name,juce::NormalisableRange<float>(p.min,p.max,p.step),p.initial));
  juce::StringArray types;for(auto* n:model::typeNames)types.add(n);l.add(std::make_unique<juce::AudioParameterChoice>(model::id(slot,"TYPE"),prefix+"Effect",types,slot<3?slot+1:0));
  l.add(std::make_unique<juce::AudioParameterBool>(model::id(slot,"ON"),prefix+"On",slot==1));l.add(std::make_unique<juce::AudioParameterBool>(model::id(slot,"G_FREEZE"),prefix+"Granular / Freeze",false));
  l.add(std::make_unique<juce::AudioParameterBool>(model::id(slot,"K_MS"),prefix+"ParallelComb / Mid-side",false));
  l.add(std::make_unique<juce::AudioParameterBool>(model::id(slot,"K_OS"),prefix+"ParallelComb / 16x oversampling",false));
  l.add(std::make_unique<juce::AudioParameterBool>(model::id(slot,"S_FREEZE"),prefix+"Spectre / Spectral freeze",false));
  l.add(std::make_unique<juce::AudioParameterChoice>(model::id(slot,"K_MODEL"),prefix+"Comb / Model",juce::StringArray{"ODIN2","PARALLELCOMB"},0));
  l.add(std::make_unique<juce::AudioParameterChoice>(model::id(slot,"S_MODE"),prefix+"Spectre / Model",juce::StringArray{"CLASSIC 0.5","EXTENDED"},0));
  l.add(std::make_unique<juce::AudioParameterChoice>(model::id(slot,"K_ROUTING"),prefix+"Odin / Routing",juce::StringArray{"A ONLY","A > B","A + B"},1));
  l.add(std::make_unique<juce::AudioParameterBool>(model::id(slot,"K_NEG_A"),prefix+"Odin / Negative A",false));
  l.add(std::make_unique<juce::AudioParameterBool>(model::id(slot,"K_NEG_B"),prefix+"Odin / Negative B",true));
  juce::StringArray windows;for(auto* n:model::windowNames)windows.add(n);l.add(std::make_unique<juce::AudioParameterChoice>(model::id(slot,"S_WINDOW"),prefix+"Spectre / Window samples",windows,3));
  l.add(std::make_unique<juce::AudioParameterChoice>(model::id(slot,"C_MODE"),prefix+"Chorus / Mode",juce::StringArray{"V1","V2"},0));
 }l.add(std::make_unique<juce::AudioParameterBool>("BYPASS","Bypass all",false));l.add(std::make_unique<juce::AudioParameterFloat>("MASTER_FB","Master / Output to input feedback",juce::NormalisableRange<float>(-99,99,.1f),0));
 l.add(std::make_unique<juce::AudioParameterBool>("OUT_CLIP","Output / Clipper",true));
 l.add(std::make_unique<juce::AudioParameterBool>("OUT_DC","Output / DC block",true));return l;
}
MonoJuiceAudioProcessor::MonoJuiceAudioProcessor():AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),parameters(*this,nullptr,"NOVA_FX5_STATE",layout()),engine(std::make_unique<nova::rack::Rack>()){
 for(int s=0;s<5;s++){auto& r=raw[s];for(int i=0;i<model::count;i++)r.values[i]=parameters.getRawParameterValue(model::id(s,model::params[i].id));
  r.type=parameters.getRawParameterValue(model::id(s,"TYPE"));r.on=parameters.getRawParameterValue(model::id(s,"ON"));r.freeze=parameters.getRawParameterValue(model::id(s,"G_FREEZE"));r.midSide=parameters.getRawParameterValue(model::id(s,"K_MS"));r.oversample=parameters.getRawParameterValue(model::id(s,"K_OS"));r.spectralFreeze=parameters.getRawParameterValue(model::id(s,"S_FREEZE"));r.window=parameters.getRawParameterValue(model::id(s,"S_WINDOW"));r.combModel=parameters.getRawParameterValue(model::id(s,"K_MODEL"));r.spectralMode=parameters.getRawParameterValue(model::id(s,"S_MODE"));r.routing=parameters.getRawParameterValue(model::id(s,"K_ROUTING"));r.negA=parameters.getRawParameterValue(model::id(s,"K_NEG_A"));r.negB=parameters.getRawParameterValue(model::id(s,"K_NEG_B"));r.chorusMode=parameters.getRawParameterValue(model::id(s,"C_MODE"));
 }bypass=parameters.getRawParameterValue("BYPASS");masterFeedback=parameters.getRawParameterValue("MASTER_FB");outputClip=parameters.getRawParameterValue("OUT_CLIP");outputDC=parameters.getRawParameterValue("OUT_DC");
 for(int i=0;i<5;i++){parameters.addParameterListener(model::id(i,"TYPE"),this);parameters.addParameterListener(model::id(i,"S_WINDOW"),this);}
 setLatencySamples(16);
}
MonoJuiceAudioProcessor::~MonoJuiceAudioProcessor(){cancelPendingUpdate();for(int i=0;i<5;i++){parameters.removeParameterListener(model::id(i,"TYPE"),this);parameters.removeParameterListener(model::id(i,"S_WINDOW"),this);}}
void MonoJuiceAudioProcessor::parameterChanged(const juce::String&,float){triggerAsyncUpdate();}
void MonoJuiceAudioProcessor::handleAsyncUpdate(){if(generation.load()&1){triggerAsyncUpdate();return;}const auto s=settings();double sr=currentRate.load();if(!std::isfinite(sr)||sr<8000||sr>768000)sr=44100;
 const int chorus=std::abs(sr-44100)<.001?16:int(std::ceil(32+48*sr/44100))+2;int samples=0;
 for(const auto& slot:s.slots){if(slot.type==nova::rack::Chorus)samples+=chorus;if(slot.type==nova::rack::Spectre)samples+=512<<slot.spectre.window;}
 setLatencySamples(samples);reportedLatency.store(samples);
}
nova::rack::Settings MonoJuiceAudioProcessor::settings()const{nova::rack::Settings result;
 auto integer=[](const std::atomic<float>* p,int lo,int hi,int fallback){const float v=p->load();return std::isfinite(v)?int(std::round(std::clamp(v,float(lo),float(hi)))):fallback;};
 for(int i=0;i<5;i++){const auto& r=raw[i];auto& s=result.slots[i];std::array<float,model::count> v{};for(int j=0;j<model::count;j++){const auto x=r.values[j]->load();const auto& spec=model::params[j];v[j]=std::isfinite(x)?std::clamp(x,spec.min,spec.max):spec.initial;}
  s.type=integer(r.type,0,5,0);s.on=r.on->load()>.5f;s.pitch={v[0],v[1],v[2],v[3]};for(int j=0;j<8;j++)s.chorus[j]=int(std::round(v[j+4]));s.grain={v[12],v[13],v[14],v[15],v[16],v[17],v[18],v[19],v[20],v[21],v[22],v[23],r.freeze->load()>.5f};
  s.comb={{v[24],v[25],v[26],v[27]},v[28],v[29],v[30],v[31],v[32],v[33],v[34],v[35],v[36],v[37],v[38],v[39],v[40],v[41],r.midSide->load()>.5f,r.oversample->load()>.5f};s.spectre={v[42],v[43],integer(r.window,0,7,3),v[44],v[45],int(v[46]),int(v[47]),v[48],v[49],v[50],v[51],r.spectralFreeze->load()>.5f};
  s.odin={v[52],v[53],v[54],v[55],v[56],v[57],integer(r.routing,0,2,0),r.negA->load()>.5f,r.negB->load()>.5f};
  s.colour={integer(r.combModel,0,1,0),v[58],v[59],v[60],v[61],v[62],v[63],v[64]};s.spectralExtended=r.spectralMode->load()>.5f;s.chorusMode=integer(r.chorusMode,0,1,0);s.lfo.rate=v[130];s.lfo.shape=v[131];s.lfo.sync=v[132];s.lfo.div=v[133];for(int j=0;j<4;j++)s.lfo.pitch[j]=v[65+j];for(int j=0;j<8;j++)s.lfo.chorus[j]=v[65+4+j];for(int j=0;j<12;j++)s.lfo.grain[j]=v[65+12+j];for(int j=0;j<18;j++)s.lfo.comb[j]=v[65+24+j];for(int j=0;j<6;j++)s.lfo.odin[j]=v[65+52+j];for(int j=0;j<7;j++)s.lfo.colour[j]=v[65+58+j];for(int j=0;j<10;j++)s.lfo.spectre[j]=v[65+42+j];
 }result.bypass=bypass->load()>.5f;const float master=masterFeedback->load();result.feedback=std::isfinite(master)?std::clamp(master,-99.f,99.f):0.f;result.clipper=outputClip->load()>.5f;result.dcBlock=outputDC->load()>.5f;return result;
}
bool MonoJuiceAudioProcessor::isBusesLayoutSupported(const BusesLayout& l)const{const auto in=l.getMainInputChannelSet(),out=l.getMainOutputChannelSet();return (in==juce::AudioChannelSet::mono()||in==juce::AudioChannelSet::stereo())&&(out==juce::AudioChannelSet::stereo()||out==in);}
void MonoJuiceAudioProcessor::prepareToPlay(double rate,int){currentRate.store(rate);activeSettings=settings();const int samples=engine->prepare(rate,activeSettings);setLatencySamples(samples);reportedLatency.store(samples);resetPending.store(false);clearMask.store(0);outputPeak.store(0);}
void MonoJuiceAudioProcessor::render(juce::AudioBuffer<float>& b,bool forced){juce::ScopedNoDenormals guard;
 const unsigned g=generation.load();if(!(g&1)){auto s=settings();if(g==generation.load()){activeSettings=s;if(resetPending.exchange(false))engine->reset(s);else engine->set(s);auto mask=clearMask.exchange(0);for(int i=0;i<5;i++)if(mask&(1u<<i))engine->clearSlot(i);}}
 // Structural choices change the true algorithmic latency. JUCE's host wrapper
 // receives a latency update; ON/OFF never changes it. No fake fixed maximum PDC.
 if(reportedLatency.load()!=engine->latency())triggerAsyncUpdate();
 if(b.getNumChannels()==0)return;
 {double bpm=120,ppq=freePpq;bool playing=true;if(auto* head=getPlayHead()){if(auto pos=head->getPosition()){const auto tb=pos->getBpm();if(tb.hasValue()&&std::isfinite(*tb)&&*tb>0)bpm=*tb;const auto pq=pos->getPpqPosition();if(pq.hasValue()&&std::isfinite(*pq)){ppq=*pq;freePpq=*pq;}playing=pos->getIsPlaying();}}engine->setTempo(bpm,ppq,playing);const double sr=currentRate.load();freePpq=ppq+bpm/60/((std::isfinite(sr)&&sr>=8000&&sr<=768000)?sr:44100)*b.getNumSamples();}
 const int in=getTotalNumInputChannels(),out=getTotalNumOutputChannels();auto* left=b.getWritePointer(0);auto* right=b.getNumChannels()>1?b.getWritePointer(1):nullptr;float peak=0;
 for(int i=0;i<b.getNumSamples();i++){const float a=in?left[i]:0;const auto v=engine->process({a,in>1&&right?right[i]:a},forced,out==1);left[i]=out==1?(v.l+v.r)*.5f:v.l;if(right&&out>1)right[i]=v.r;peak=std::max(peak,std::max(std::abs(v.l),std::abs(v.r)));}
 for(int ch=out;ch<b.getNumChannels();ch++)b.clear(ch,0,b.getNumSamples());outputPeak.store(peak);grainCount.store(engine->activeGrains());
}
void MonoJuiceAudioProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer& m){m.clear();render(b,false);}void MonoJuiceAudioProcessor::processBlockBypassed(juce::AudioBuffer<float>& b,juce::MidiBuffer& m){m.clear();render(b,true);}
void MonoJuiceAudioProcessor::setValue(const juce::String& id,float value){if(auto* p=parameters.getParameter(id)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(value));p->endChangeGesture();}}
void MonoJuiceAudioProcessor::initialiseSlot(int slot){slot=juce::jlimit(0,4,slot);const int type=settings().slots[slot].type;if(type==4){applyFactoryPreset(slot,settings().slots[slot].colour.model==0?0:5);return;}generation.fetch_add(1);for(const auto& p:model::params)if(p.stage==type-1)setValue(model::id(slot,p.id),p.initial);
 if(type==3)setValue(model::id(slot,"G_FREEZE"),0);if(type==4){setValue(model::id(slot,"K_ROUTING"),0);setValue(model::id(slot,"K_NEG_A"),0);setValue(model::id(slot,"K_NEG_B"),0);setValue(model::id(slot,"K_MS"),0);setValue(model::id(slot,"K_OS"),0);}if(type==5){setValue(model::id(slot,"S_MODE"),0);setValue(model::id(slot,"S_WINDOW"),3);setValue(model::id(slot,"S_FREEZE"),0);}clearSlot(slot);generation.fetch_add(1);
}
void MonoJuiceAudioProcessor::applyPatch(int slot,const factory::Patch& patch){
 if(patch.empty())return;generation.fetch_add(1);
 for(const auto& value:patch)setValue(model::id(slot,value.first),value.second);
 clearSlot(slot);generation.fetch_add(1);
}
bool MonoJuiceAudioProcessor::applyFactoryPreset(int slot,int index){
 if(slot<0||slot>=5)return false;const auto patch=factory::preset(settings().slots[slot].type,index);
 if(patch.empty())return false;applyPatch(slot,patch);return true;
}
void MonoJuiceAudioProcessor::randomiseSlot(int slot,uint32_t seed){
 if(slot<0||slot>=5)return;const auto s=settings().slots[slot];applyPatch(slot,factory::randomize(s.type,seed,s.type==4?s.colour.model:(s.spectralExtended?1:0)));
}
int MonoJuiceAudioProcessor::matchingFactoryPreset(int slot)const{
 if(slot<0||slot>=5)return -1;const int type=settings().slots[slot].type;if(type!=4&&type!=5)return -1;
 for(int index=0;index<10;index++){bool matches=true;for(const auto& v:factory::preset(type,index)){
  const auto id=model::id(slot,v.first);auto* param=parameters.getParameter(id);
  const float target=param->convertFrom0to1(param->convertTo0to1(v.second));
  if(std::abs(parameters.getRawParameterValue(id)->load()-target)>std::max(.0001f,std::abs(target)*.000001f)){matches=false;break;}
 }if(matches)return index;}return -1;
}
void MonoJuiceAudioProcessor::panicAll(){generation.fetch_add(1);setValue("MASTER_FB",0);resetPending.store(true);generation.fetch_add(1);}
void MonoJuiceAudioProcessor::panicComb(int slot){slot=juce::jlimit(0,4,slot);if(settings().slots[slot].type!=nova::rack::Comb)return;generation.fetch_add(1);setValue(model::id(slot,"K_FB"),0);setValue(model::id(slot,"K_RES_A"),0);setValue(model::id(slot,"K_RES_B"),0);clearSlot(slot);generation.fetch_add(1);}
void MonoJuiceAudioProcessor::metalCloudPreset(int slot){slot=juce::jlimit(0,4,slot);generation.fetch_add(1);const std::pair<const char*,float> v[]={{"G_SIZE",180},{"G_DENSITY",36},{"G_POSITION",360},{"G_SPRAY",8},{"G_PITCH",0},{"G_JITTER",0},{"G_WIDTH",85},{"G_MIX",70},{"G_FEEDBACK",97.5f},{"G_METAL",85},{"G_TONE",9000},{"G_REVERSE",0},{"G_FREEZE",0},{"TYPE",3},{"ON",1}};for(auto x:v)setValue(model::id(slot,x.first),x.second);clearSlot(slot);generation.fetch_add(1);}
void MonoJuiceAudioProcessor::swapSlots(int a,int b){if(a<0||a>4||b<0||b>4||a==b)return;generation.fetch_add(1);
 auto swap=[&](const char* key){const auto ia=model::id(a,key),ib=model::id(b,key);const float va=parameters.getRawParameterValue(ia)->load(),vb=parameters.getRawParameterValue(ib)->load();setValue(ia,vb);setValue(ib,va);};
 for(const auto& p:model::params)swap(p.id);for(auto* key:model::slotKeys)swap(key);clearSlot(a);clearSlot(b);selectedSlot.store(b);generation.fetch_add(1);
}
void MonoJuiceAudioProcessor::getStateInformation(juce::MemoryBlock& out){auto state=parameters.copyState();state.setProperty("version",3,nullptr);state.setProperty("slot",selectedSlot.load(),nullptr);copyXmlToBinary(*state.createXml(),out);}
void MonoJuiceAudioProcessor::setStateInformation(const void* data,int bytes){if(!data||bytes<=0||bytes>1024*1024)return;auto xml=getXmlFromBinary(data,bytes);if(!xml)return;
 const bool oldCh=xml->hasTagName("NOVA_CHORUS_STATE"),oldFx=xml->hasTagName("NOVA_FX3_STATE");if(!oldCh&&!oldFx&&!xml->hasTagName("NOVA_FX5_STATE"))return;const auto incoming=juce::ValueTree::fromXml(*xml);auto state=parameters.copyState();
 for(auto node:state){const auto id=node["id"].toString();auto* param=parameters.getParameter(id);if(!param)continue;float value=param->convertFrom0to1(param->getDefaultValue());juce::String lookup=id;
  if(oldCh||oldFx){lookup={};if(id=="BYPASS"&&!oldCh)lookup=id;for(int s=0;s<3;s++){for(const auto& p:model::params)if(p.stage==s&&id==model::id(s,p.id))lookup=p.id;
    if(id==model::id(s,"ON")&&!oldCh)lookup=s==0?"P_ON":(s==1?"C_ON":"G_ON");if(s==2&&id==model::id(s,"G_FREEZE")&&!oldCh)lookup="G_FREEZE";}}
  if(!incoming.getChildWithProperty("id",id).isValid()){
   if(id=="MASTER_FB"||id=="OUT_CLIP"||id=="OUT_DC")value=0;
   for(int slot=0;slot<5;slot++){
    if(id==model::id(slot,"K_MODEL"))value=incoming.getChildWithProperty("id",model::id(slot,"K_T1")).isValid()?1.f:0.f;
    if(id==model::id(slot,"K_DRIVE")||id==model::id(slot,"K_OUT"))value=0;
    if(id==model::id(slot,"S_MODE")){value=0;for(auto pair:std::initializer_list<std::pair<const char*,float>>{{"S_LOW",0},{"S_HIGH",0},{"S_PEAKS",0},{"S_WIDTH",0},{"S_FLOOR",-120},{"S_ATTACK",0},{"S_RELEASE",0},{"S_SHIFT",0},{"S_FREEZE",0}}){auto n=incoming.getChildWithProperty("id",model::id(slot,pair.first));if(n.isValid()&&float(n["value"])!=pair.second)value=1;}}
   }
  }
  if(lookup.isNotEmpty()){auto n=incoming.getChildWithProperty("id",lookup);if(n.isValid()){const double v=double(n["value"]);if(std::isfinite(v))value=float(v);}}
  value=param->convertFrom0to1(juce::jlimit(0.f,1.f,param->convertTo0to1(value)));node.setProperty("value",value,nullptr);
 }generation.fetch_add(1);parameters.replaceState(state);selectedSlot.store(juce::jlimit(0,4,int(incoming.getProperty(oldFx?"tab":"slot",1))));resetPending.store(true);generation.fetch_add(1);
}
juce::AudioProcessorEditor* MonoJuiceAudioProcessor::createEditor(){return new MonoJuiceAudioProcessorEditor(*this);}juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new MonoJuiceAudioProcessor();}
