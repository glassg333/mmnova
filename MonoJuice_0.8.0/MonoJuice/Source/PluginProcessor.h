#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "FactoryPresets.h"
#include "dsp/Rack.h"
#include <atomic>
class MonoJuiceAudioProcessor final:public juce::AudioProcessor,private juce::AudioProcessorValueTreeState::Listener,private juce::AsyncUpdater {
public:
 MonoJuiceAudioProcessor();~MonoJuiceAudioProcessor()override;
 void prepareToPlay(double,int)override;void releaseResources()override{}
 void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&)override;void processBlockBypassed(juce::AudioBuffer<float>&,juce::MidiBuffer&)override;
 bool isBusesLayoutSupported(const BusesLayout&)const override;juce::AudioProcessorEditor* createEditor()override;bool hasEditor()const override{return true;}
 const juce::String getName()const override{return "MonoJuice";}
 bool acceptsMidi()const override{return false;}bool producesMidi()const override{return false;}bool isMidiEffect()const override{return false;}
 double getTailLengthSeconds()const override{return 600.;}int getNumPrograms()override{return 1;}int getCurrentProgram()override{return 0;}void setCurrentProgram(int)override{}
 const juce::String getProgramName(int)override{return "MonoJuice";}void changeProgramName(int,const juce::String&)override{}
 void getStateInformation(juce::MemoryBlock&)override;void setStateInformation(const void*,int)override;
 void panicAll();void initialiseSlot(int);void clearSlot(int i){clearMask.fetch_or(1u<<unsigned(juce::jlimit(0,4,i)));}void metalCloudPreset(int);void panicComb(int);void swapSlots(int,int);
 bool applyFactoryPreset(int,int);void randomiseSlot(int,uint32_t);int matchingFactoryPreset(int)const;
 void setValue(const juce::String&,float);nova::rack::Settings settings()const;
 juce::AudioProcessorValueTreeState parameters;
 std::atomic<float> outputPeak{0};std::atomic<int> grainCount{0},selectedSlot{1},reportedLatency{16};
private:
 void applyPatch(int,const factory::Patch&);
 void parameterChanged(const juce::String&,float)override;void handleAsyncUpdate()override;std::atomic<double> currentRate{44100.};
 static juce::AudioProcessorValueTreeState::ParameterLayout layout();void render(juce::AudioBuffer<float>&,bool);
 struct Raw {std::array<std::atomic<float>*,model::count> values{};std::atomic<float> *type=nullptr,*on=nullptr,*freeze=nullptr,*midSide=nullptr,*oversample=nullptr,*spectralFreeze=nullptr,*window=nullptr,*combModel=nullptr,*spectralMode=nullptr,*routing=nullptr,*negA=nullptr,*negB=nullptr,*chorusMode=nullptr;};
 std::array<Raw,5> raw;std::atomic<float>* bypass=nullptr,*masterFeedback=nullptr,*outputClip=nullptr,*outputDC=nullptr;double freePpq=0;
 std::unique_ptr<nova::rack::Rack> engine;nova::rack::Settings activeSettings;
 std::atomic<unsigned> generation{0},clearMask{0};std::atomic<bool> resetPending{false};
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonoJuiceAudioProcessor)
};
