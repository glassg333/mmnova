#pragma once
#include "NovaData.h"
#include "NovaDSP.h"
#include "models/arpeggiator.hpp"
#include "models/modulation_matrix.hpp"

class MonomachineNovaAudioProcessor final : public juce::AudioProcessor, private juce::AsyncUpdater {
public:
    static constexpr bool isSynthVersion=NOVA_SYNTH!=0;
    MonomachineNovaAudioProcessor();
    ~MonomachineNovaAudioProcessor() override;
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    juce::AudioProcessorValueTreeState parameters;
    void prepareToPlay(double,int) override;
    void releaseResources() override {prepared=false;}
    void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&) override;
    using juce::AudioProcessor::processBlock;
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    const juce::String getName() const override {return JucePlugin_Name;}
    bool acceptsMidi() const override {return true;}
    bool producesMidi() const override {return false;}
    bool isMidiEffect() const override {return false;}
    double getTailLengthSeconds() const override {return 120;}
    bool hasEditor() const override {return true;}
    juce::AudioProcessorEditor* createEditor() override;
    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override {return "Default";}
    void changeProgramName(int,const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*,int) override;
    void requestPanic() {panicRequested.store(true);}
    float outputPeak() const {return peak.load();}
    double currentBpm() const {return tempoDisplay.load();}
    juce::String loadSample(int slot,const juce::File&);
    juce::String sampleName(int slot);
    void resetSamples();
    void flushSampleRestore() {handleUpdateNowIfNeeded();}
    int machineIndex() const {return juce::jlimit(0,static_cast<int>(nova::machines().size())-1,juce::roundToInt(parameters.getRawParameterValue("machine")->load()));}
private:
    enum Global {Level,Bpm,HostSync,Mix,Gate,ArpMode,ArpPlay,ArpSpeed,ArpRange,ArpLength,MacroX,MacroY,GlobalCount};
    std::atomic<float>* machineRaw=nullptr;
    std::array<std::array<std::atomic<float>*,8>,22> synthRaw{};
    std::array<std::array<std::atomic<float>*,8>,6> pageRaw{};
    std::array<std::array<std::atomic<float>*,4>,8> routeRaw{};
    std::array<std::atomic<float>*,GlobalCount> globalRaw{};
    std::array<float,GlobalCount> global{};
    std::array<float,32> base{};
    std::array<std::array<float,8>,3> lfoParams{};
    struct Note {bool down=false;float velocity=1;uint64_t order=0;};
    std::array<Note,2048> notes{};
    std::array<bool,16> pedal{};
    std::array<int,16> wheels{};
    std::array<bool,128> arpHeld{};
    uint64_t serial=0;int currentKey=-1,soundingNote=60,soundingChannel=0,activeMachine=-1,lastArpMode=-1;
    float ccX=-1,ccY=-1,previousX=64,previousY=0;
    double sr=44100,bpm=120;bool prepared=false;float blockPeak=0;
    nova::MachineEngine machine;
    nova::TrackChain chain;
    nova::AmpEnvelope envelope;
    std::array<nova::LFO,3> lfos;
    monomachine::MonomachineArpeggiator arp;
    monomachine::ModulationMatrix matrix;
    juce::SmoothedValue<float> pitch,level,mix,velocity;
    juce::CriticalSection sampleLock,stateLock;
    std::array<juce::MemoryBlock,10> sampleData;
    std::array<juce::String,10> sampleNames;
    juce::ValueTree pendingSamples;
    std::atomic<bool> panicRequested{false};
    std::atomic<float> peak{0};std::atomic<double> tempoDisplay{120};
    void snapshot();void panic();void midi(const juce::MidiMessage&);void selectNotes(bool);
    void syncArpNotes();void trigger(int,float,int);void release();void render(juce::AudioBuffer<float>&,int,int);
    void handleAsyncUpdate() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonomachineNovaAudioProcessor)
};
