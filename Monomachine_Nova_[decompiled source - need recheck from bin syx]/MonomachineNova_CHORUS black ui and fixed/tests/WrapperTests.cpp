#include "PluginProcessor.h"
#include <iostream>
#include <stdexcept>
#include <memory>
#include <chrono>

static void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
static void set(MonomachineNovaAudioProcessor& p,const char* id,int v){auto* param=p.parameters.getParameter(id);param->setValueNotifyingHost(param->convertTo0to1(float(v)));}
static double render(MonomachineNovaAudioProcessor& p,double rate,int frames,juce::AudioBuffer<float>* capture=nullptr){
 p.setRateAndBufferSizeDetails(rate,512);p.prepareToPlay(rate,512);juce::MidiBuffer midi;double sum=0;
 const int sizes[]={1,7,16,31,127,512,3,251};int step=0;
 for(int pos=0;pos<frames;){const int count=std::min(sizes[(step++)%8],frames-pos);juce::AudioBuffer<float> block(2,count);
  for(int i=0;i<count;i++){const double t=double(pos+i)/rate;block.setSample(0,i,float(0.04*(std::sin(6.283185307179586*220*t)+0.5*std::sin(6.283185307179586*733*t))));block.setSample(1,i,float(0.04*std::sin(6.283185307179586*331*t)));}
  p.processBlock(block,midi);
  for(int ch=0;ch<2;ch++)for(int i=0;i<count;i++){const auto v=block.getSample(ch,i);require(std::isfinite(v),"nonfinite wrapper output");sum+=v*v;if(capture)capture->setSample(ch,pos+i,v);}
  pos+=count;
 }
 return std::sqrt(sum/(frames*2.0));
}
int main(int argc,char** argv){try{
 juce::ScopedJuceInitialiser_GUI init;MonomachineNovaAudioProcessor p;
 require(p.getParameters().size()==9,"parameter registration");require(!p.acceptsMidi(),"FX should not require MIDI");
 for(double rate:{44100.0,48000.0,96000.0}){auto t=std::chrono::steady_clock::now();const auto rms=render(p,rate,int(rate*2));require(rms>0.001,"silent FX");std::cout<<"Wrapper "<<rate<<" RMS="<<rms<<" latency="<<p.getLatencySamples()<<" wall="<<std::chrono::duration<double>(std::chrono::steady_clock::now()-t).count()<<" PASS\n";}
 for(int i=0;i<8;i++)set(p,MonomachineNovaAudioProcessor::parameterIds[i],i*15+4);
 set(p,"BYPASS",1);juce::MemoryBlock state;p.getStateInformation(state);
 MonomachineNovaAudioProcessor restored;restored.setStateInformation(state.getData(),int(state.getSize()));
 require(restored.parameterValues()==p.parameterValues(),"state values");require(restored.parameters.getRawParameterValue("BYPASS")->load()==1,"state bypass");
 restored.prepareToPlay(44100,512);require(restored.parameterValues()==p.parameterValues(),"prepare lost restored state");
 restored.initialisePatch();require(restored.parameterValues()==nova::ChorusCore::defaults,"INIT defaults");
 MonomachineNovaAudioProcessor::BusesLayout mono;mono.inputBuses.add(juce::AudioChannelSet::mono());mono.outputBuses.add(juce::AudioChannelSet::mono());require(restored.setBusesLayout(mono),"mono layout");
 juce::AudioBuffer<float> mb(1,257);mb.clear();juce::MidiBuffer mm;restored.processBlock(mb,mm);
 p.initialisePatch();set(p,"BYPASS",0);set(p,"WID",96);
 const int frames=44100*4;juce::AudioBuffer<float> demonstration(2,frames);render(p,44100,frames,&demonstration);
 if(argc>1){
  juce::File folder=juce::File::getCurrentWorkingDirectory().getChildFile(argv[1]);folder.createDirectory();
  auto demoFile=folder.getChildFile("MonomachineNova_CHORUS_Demo.wav");demoFile.deleteFile();auto stream=demoFile.createOutputStream();juce::WavAudioFormat format;
  if(stream){std::unique_ptr<juce::AudioFormatWriter> writer(format.createWriterFor(stream.release(),44100,2,24,{},0));require(writer!=nullptr,"WAV writer");writer->writeFromAudioSampleBuffer(demonstration,0,frames);}
  std::unique_ptr<juce::AudioProcessorEditor> editor(p.createEditor());require(editor!=nullptr,"editor");editor->setVisible(true);
  int sliders=0;for(auto* child:editor->getChildren())if(auto* slider=dynamic_cast<juce::Slider*>(child)){slider->setValue(17+sliders,juce::sendNotificationSync);require(p.parameterValues()[std::size_t(sliders)]==17+sliders,"GUI attachment did not update parameter");++sliders;}require(sliders==8,"visible controls");p.initialisePatch();set(p,"WID",96);
  auto initial=editor->createComponentSnapshot(editor->getLocalBounds());juce::ignoreUnused(initial);
  juce::Timer::callAfterDelay(4500,[]{juce::MessageManager::getInstance()->stopDispatchLoop();});juce::MessageManager::getInstance()->runDispatchLoop();
  auto image=editor->createComponentSnapshot(editor->getLocalBounds(),true,1.0f);auto pngFile=folder.getChildFile("MonomachineNova_CHORUS_GUI.png");pngFile.deleteFile();auto png=pngFile.createOutputStream();juce::PNGImageFormat pngFormat;require(png&&pngFormat.writeImageToStream(image,*png),"PNG screenshot");
 }
 std::cout<<"STATE / INIT / MONO / GUI / AUDIO PASS\n";return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
