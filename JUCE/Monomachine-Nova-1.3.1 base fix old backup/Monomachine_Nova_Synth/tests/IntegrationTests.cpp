#include "PluginProcessor.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
using P=MonomachineNovaAudioProcessor;
static int checks=0;
void require(bool yes,const char* message){++checks;if(!yes)throw std::runtime_error(message);}
void set(P& p,const juce::String& id,float v){auto* x=p.parameters.getParameter(id);require(x!=nullptr,"Parameter missing");x->setValueNotifyingHost(x->convertTo0to1(v));}
void defaults(P& p){for(auto* a:p.getParameters())a->setValueNotifyingHost(a->getDefaultValue());}
float peak(const juce::AudioBuffer<float>& b,int first=0,int end=-1){if(end<0)end=b.getNumSamples();float x=0;for(int ch=0;ch<b.getNumChannels();++ch)for(int i=first;i<end;++i){require(std::isfinite(b.getSample(ch,i)),"NaN audio");x=std::max(x,std::abs(b.getSample(ch,i)));}return x;}
void input(juce::AudioBuffer<float>& b,double& phase,double rate){for(int i=0;i<b.getNumSamples();++i){float x=0.2f*std::sin(static_cast<float>(phase));phase+=2*nova::pi*220/rate;for(int ch=0;ch<b.getNumChannels();++ch)b.setSample(ch,i,x);}}
void run(P& p,juce::AudioBuffer<float>& b,juce::MidiBuffer& m){p.processBlock(b,m);require(m.isEmpty(),"MIDI output not cleared");}
int main(int argc,char** argv){juce::ScopedJuceInitialiser_GUI juceInit;
    try{
        require(nova::machines().size()==(P::isSynthVersion?15u:7u),"Mode count");
        P p;juce::MidiBuffer midi;juce::AudioBuffer<float> b(2,1024);double phase=0;
        std::cout<<"Parameters: "<<p.getParameters().size()<<"\n";
        for(size_t index=0;index<nova::machines().size();++index){
            defaults(p);set(p,"machine",static_cast<float>(index));p.prepareToPlay(48000,64);b.clear();
            if(!P::isSynthVersion)input(b,phase,48000);
            midi.addEvent(juce::MidiMessage::noteOn(1,60,static_cast<juce::uint8>(127)),64);run(p,b,midi);
            float max=peak(b);const auto& def=nova::machines()[index];
            if(P::isSynthVersion){require(peak(b,0,64)==0,"Early MIDI onset");if(def.id==0)require(max==0,"GND-GND must be silent");else require(max>1e-5f,"Silent synthesis mode");}
            else require(max>1e-5f,"Silent FX mode");
            std::cout<<def.name<<" peak="<<max<<"\n";
        }
        defaults(p);p.prepareToPlay(48000,64);b.clear();run(p,b,midi);require(peak(b)==0,"Startup silence");
        if(P::isSynthVersion){
            set(p,"p0_1",127);set(p,"p0_3",0);
            midi.addEvent(juce::MidiMessage::noteOn(1,60,static_cast<juce::uint8>(127)),32);run(p,b,midi);require(peak(b,0,32)==0&&peak(b,33)>0.001f,"MIDI offset");
            midi.addEvent(juce::MidiMessage::noteOff(1,60),0);for(int k=0;k<8;++k)run(p,b,midi);require(peak(b)==0,"Release did not end");
            midi.addEvent(juce::MidiMessage::noteOn(1,60,static_cast<juce::uint8>(127)),0);midi.addEvent(juce::MidiMessage::noteOn(1,67,static_cast<juce::uint8>(127)),10);midi.addEvent(juce::MidiMessage::noteOff(1,60),20);run(p,b,midi);require(peak(b)>0.001f,"Last-note priority");
            midi.addEvent(juce::MidiMessage::controllerEvent(1,64,127),0);midi.addEvent(juce::MidiMessage::noteOff(1,67),1);for(int k=0;k<4;++k)run(p,b,midi);require(peak(b)>0.001f,"Sustain pedal");
            midi.addEvent(juce::MidiMessage::controllerEvent(1,64,0),0);for(int k=0;k<8;++k)run(p,b,midi);require(peak(b)==0,"Sustain pedal release");
        }else{
            set(p,"gate",1);p.prepareToPlay(48000,64);input(b,phase,48000);run(p,b,midi);require(peak(b)==0,"FX gated without MIDI");
            input(b,phase,48000);midi.addEvent(juce::MidiMessage::noteOn(1,60,static_cast<juce::uint8>(127)),64);run(p,b,midi);require(peak(b,0,64)==0&&peak(b,65)>1e-5f,"FX gate onset");
            set(p,"fx_mix",0);p.prepareToPlay(48000,64);input(b,phase,48000);juce::AudioBuffer<float> copy;copy.makeCopyOf(b);run(p,b,midi);
            for(int ch=0;ch<2;++ch)for(int i=0;i<1024;++i)require(std::abs(copy.getSample(ch,i)-b.getSample(ch,i))<1e-6f,"Dry FX changed audio");
        }
        p.requestPanic();b.clear();run(p,b,midi);require(peak(b)==0,"Panic leaves audio");
        set(p,"bpm",143.5f);set(p,"p1_2",41);juce::MemoryBlock state;p.getStateInformation(state);require(state.getSize()>0,"State empty");set(p,"p1_2",0);p.setStateInformation(state.getData(),static_cast<int>(state.getSize()));p.flushSampleRestore();require(p.parameters.getRawParameterValue("p1_2")->load()==41,"Restore parameters");p.setStateInformation(nullptr,0);
        // Fixed-size arp scheduler: no note loss in large blocks, all modes produce events.
        for(int mode=1;mode<=3;++mode){monomachine::MonomachineArpeggiator arp;arp.reset(48000,120);monomachine::MonomachineArpeggiator::ArpSettings s;s.mode=static_cast<monomachine::MonomachineArpeggiator::Mode>(mode);s.speed=6;s.noteLength=32;arp.setSettings(s);arp.noteOn(60);arp.noteOn(64);arp.noteOn(67);int on=0,off=0;
            for(int i=0;i<48000;++i){for(int k=0;k<2;++k){auto event=arp.poll();if(!event.triggered)break;if(event.isNoteOff)++off;else ++on;}arp.advance(1);}
            require(on==8&&off==8,"Arp timing at 120 BPM");}
        // LFO modes/waves and matrix target mapping.
        for(int wave=0;wave<5;++wave){nova::LFO lfo;lfo.reset();std::array<float,8> x{0,6,0,static_cast<float>(wave),0,64,0,127};float min=2,max=-2;for(int i=0;i<1000;++i){float v=lfo.process(x,48000,120,64);min=std::min(min,v);max=std::max(max,v);}require(max>min,"LFO constant output");}
        monomachine::ModulationMatrix mat;mat.reset();for(size_t i=0;i<8;++i)mat.configureRouting(i,false,monomachine::ModSource::LFO1,16,0);mat.configureRouting(0,true,monomachine::ModSource::LFO1,16,32);mat.setLfoOutputs(1,0,0);std::array<uint8_t,32> before{},after{};before.fill(64);mat.evaluate(before,after);require(after[16]==96,"Matrix output");
        // Verify an enabled matrix route reaches the audio path, not just the standalone matrix.
        {P clean,mod;set(clean,"p1_0",20);set(mod,"p1_0",20);set(clean,"p1_1",80);set(mod,"p1_1",80);
            set(mod,"r0_on",1);set(mod,"r0_src",1);set(mod,"r0_dest",16);set(mod,"r0_depth",32);
            clean.prepareToPlay(48000,512);mod.prepareToPlay(48000,512);juce::AudioBuffer<float> a(2,512),bmod(2,512);a.clear();bmod.clear();
            if(!P::isSynthVersion){phase=0;input(a,phase,48000);phase=0;input(bmod,phase,48000);}juce::MidiBuffer ma,mbb;
            ma.addEvent(juce::MidiMessage::noteOn(1,60,static_cast<juce::uint8>(127)),0);mbb=ma;run(clean,a,ma);run(mod,bmod,mbb);
            double difference=0;for(int i=0;i<512;++i)difference+=std::abs(a.getSample(0,i)-bmod.getSample(0,i));require(difference>0.01,"Matrix route not connected to DSP");}
        // Stress every mode, non-standard block sizes, high notes, all min/max synthesis knobs.
        for(double sr:{44100.0,48000.0,96000.0,192000.0})for(size_t index=0;index<nova::machines().size();++index){defaults(p);set(p,"machine",static_cast<float>(index));p.prepareToPlay(sr,64);
            for(int size:{0,1,17,32,257,4096})for(int maximum=0;maximum<2;++maximum){for(int i=0;i<8;++i){const auto& def=nova::machines()[index].synthParams[static_cast<size_t>(i)];if(def.maxVal)set(p,nova::machineParam(nova::machines()[index].id,i),maximum?def.maxVal:def.minVal);}
                juce::AudioBuffer<float> audio(2,size);audio.clear();if(!P::isSynthVersion)input(audio,phase,sr);if(size)midi.addEvent(juce::MidiMessage::noteOn(1,127,static_cast<juce::uint8>(127)),0);run(p,audio,midi);peak(audio);}}
        // Mono/stereo output must match at the centre, with a deterministic oscillator/input.
        P mono,stereo;auto layout=mono.getBusesLayout();layout.outputBuses.set(0,juce::AudioChannelSet::mono());if(!P::isSynthVersion)layout.inputBuses.set(0,juce::AudioChannelSet::mono());require(mono.setBusesLayout(layout),"Mono rejected");mono.prepareToPlay(48000,64);stereo.prepareToPlay(48000,64);
        juce::AudioBuffer<float> mb(1,1024),sb(2,1024);mb.clear();sb.clear();if(!P::isSynthVersion){phase=0;input(mb,phase,48000);phase=0;input(sb,phase,48000);}juce::MidiBuffer mm,sm;mm.addEvent(juce::MidiMessage::noteOn(1,60,static_cast<juce::uint8>(127)),0);sm=mm;run(mono,mb,mm);run(stereo,sb,sm);for(int i=0;i<1024;++i)require(std::abs(mb.getSample(0,i)-sb.getSample(0,i))<1e-6f,"Mono gain mismatch");
        if(P::isSynthVersion){
            auto file=juce::File::getSpecialLocation(juce::File::tempDirectory).getNonexistentChildFile("nova-sample",".wav");
            {auto stream=file.createOutputStream();juce::WavAudioFormat format;std::unique_ptr<juce::AudioFormatWriter> writer(format.createWriterFor(stream.release(),44100,1,16,{},0));require(writer!=nullptr,"WAV writer");juce::AudioBuffer<float> sample(1,4410);phase=0;input(sample,phase,44100);require(writer->writeFromAudioSampleBuffer(sample,0,4410),"WAV write");}
            P source;require(source.loadSample(4,file).isEmpty(),"Sample load");source.getStateInformation(state);file.deleteFile();P restored;restored.setStateInformation(state.getData(),static_cast<int>(state.getSize()));restored.flushSampleRestore();require(restored.sampleName(4)==source.sampleName(4),"Embedded sample metadata");juce::MemoryBlock restoredState;restored.getStateInformation(restoredState);require(restoredState==state,"Embedded PCM/state round trip");
            int bboxIndex=0;for(size_t i=0;i<nova::machines().size();++i)if(nova::machines()[i].id==7)bboxIndex=static_cast<int>(i);set(restored,"machine",static_cast<float>(bboxIndex));set(restored,"m7_2",4);restored.prepareToPlay(44100,512);b.clear();midi.addEvent(juce::MidiMessage::noteOn(1,60,static_cast<juce::uint8>(127)),0);run(restored,b,midi);require(peak(b)>0.001f,"Embedded sample playback");
        }
        if(argc>1){P gui;std::unique_ptr<juce::AudioProcessorEditor> editor(gui.createEditor());require(editor!=nullptr,"No editor");auto image=editor->createComponentSnapshot(editor->getLocalBounds());auto file=juce::File::getCurrentWorkingDirectory().getChildFile(argv[1]);file.deleteFile();auto stream=file.createOutputStream();juce::PNGImageFormat png;require(stream&&png.writeImageToStream(image,*stream),"GUI snapshot");
            for(int width:{1008,1890,1260})editor->setSize(width,juce::roundToInt(width*584.0/1260));}
        std::cout<<(P::isSynthVersion?"Synth":"FX")<<": PASS ("<<checks<<" assertions including per-sample finite checks)\n";return 0;
    }catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<"\n";return 1;}
}
