#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout MonomachineNovaAudioProcessor::createParameters(){
 juce::AudioProcessorValueTreeState::ParameterLayout layout;
 for(int i=0;i<8;++i)layout.add(std::make_unique<juce::AudioParameterInt>(parameterIds[i],parameterNames[i],0,127,nova::ChorusCore::defaults[std::size_t(i)]));
 layout.add(std::make_unique<juce::AudioParameterBool>("BYPASS","Bypass",false));
 return layout;
}
MonomachineNovaAudioProcessor::MonomachineNovaAudioProcessor()
 : AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),
 parameters(*this,nullptr,"NOVA_CHORUS_STATE",createParameters()),engine(std::make_unique<nova::AudioAdapter>()) {
 for(int i=0;i<8;++i)raw[std::size_t(i)]=parameters.getRawParameterValue(parameterIds[i]);
 bypassValue=parameters.getRawParameterValue("BYPASS");
}
bool MonomachineNovaAudioProcessor::isBusesLayoutSupported(const BusesLayout& layout) const {
 const auto in=layout.getMainInputChannelSet(),out=layout.getMainOutputChannelSet();
 return (in==juce::AudioChannelSet::mono() || in==juce::AudioChannelSet::stereo()) &&
        (out==juce::AudioChannelSet::stereo() || (in==juce::AudioChannelSet::mono() && out==juce::AudioChannelSet::mono()));
}
std::array<int,8> MonomachineNovaAudioProcessor::parameterValues() const noexcept {
 std::array<int,8> values{};for(int i=0;i<8;++i)values[std::size_t(i)]=juce::jlimit(0,127,juce::roundToInt(raw[std::size_t(i)]->load()));return values;
}
void MonomachineNovaAudioProcessor::prepareToPlay(double sampleRate,int){
 setLatencySamples(engine->prepare(sampleRate));engine->reset(parameterValues());resetPending.store(false);outputPeak.store(0);
}
void MonomachineNovaAudioProcessor::render(juce::AudioBuffer<float>& buffer,bool forcedBypass){
 juce::ScopedNoDenormals noDenormals;
 const auto values=parameterValues();
 if(resetPending.exchange(false))engine->reset(values);else engine->setParameters(values);
 const bool bypass=forcedBypass || bypassValue->load()>=0.5f;
 const int inChannels=getTotalNumInputChannels(),outChannels=getTotalNumOutputChannels();
 float peak=0;
 if(buffer.getNumChannels()==0)return;
 auto* left=buffer.getWritePointer(0);auto* right=buffer.getNumChannels()>1?buffer.getWritePointer(1):nullptr;
 for(int i=0;i<buffer.getNumSamples();++i){
  const float inL=inChannels>0?left[i]:0.0f;const float inR=inChannels>1&&right?right[i]:inL;
  float outL=0,outR=0;engine->processFrame(inL,inR,outL,outR,bypass);
  if(outChannels==1)left[i]=(outL+outR)*0.5f;
  else{left[i]=outL;if(right)right[i]=outR;}
  peak=std::max(peak,std::max(std::abs(outL),std::abs(outR)));
 }
 for(int ch=outChannels;ch<buffer.getNumChannels();++ch)buffer.clear(ch,0,buffer.getNumSamples());
 outputPeak.store(peak);
}
void MonomachineNovaAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,juce::MidiBuffer& midi){midi.clear();render(buffer,false);}
void MonomachineNovaAudioProcessor::processBlockBypassed(juce::AudioBuffer<float>& buffer,juce::MidiBuffer& midi){midi.clear();render(buffer,true);}
void MonomachineNovaAudioProcessor::initialisePatch(){
 for(int i=0;i<8;++i){auto* p=parameters.getParameter(parameterIds[i]);p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(float(nova::ChorusCore::defaults[std::size_t(i)])));p->endChangeGesture();}
 resetPending.store(true);
}
void MonomachineNovaAudioProcessor::getStateInformation(juce::MemoryBlock& destination){
 auto state=parameters.copyState();state.setProperty("formatVersion",1,nullptr);if(auto xml=state.createXml())copyXmlToBinary(*xml,destination);
}
void MonomachineNovaAudioProcessor::setStateInformation(const void* data,int bytes){
 if(data==nullptr || bytes<=0)return;
 if(auto xml=getXmlFromBinary(data,bytes))if(xml->hasTagName(parameters.state.getType())){
  auto state=juce::ValueTree::fromXml(*xml);if(state.isValid()){parameters.replaceState(state);resetPending.store(true);}
 }
}
juce::AudioProcessorEditor* MonomachineNovaAudioProcessor::createEditor(){return new MonomachineNovaAudioProcessorEditor(*this);}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new MonomachineNovaAudioProcessor();}
