#pragma once
#include "NovaData.h"
#include "NovaDSP.h"
#include "models/arpeggiator.hpp"
#include "models/modulation_matrix.hpp"
#include "models/mseg.hpp"

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
    bool producesMidi() const override {return !isSynthVersion;}
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
    int playingSample() const {return playingSampleSlot.load();}
    std::atomic<int> playingSampleSlot{-1},lastBboxSample{-1};
    std::atomic<unsigned> bboxHitCounter{0};
    float outputPeak() const {return peak.load();}
    double currentBpm() const {return tempoDisplay.load();}
    juce::String loadSample(int slot,const juce::File&);
    juce::String sampleName(int slot);
    void resetSamples(bool modern=true);
    void flushSampleRestore() {handleUpdateNowIfNeeded();}

    // MSEG editor/DSP bridge. Point writes are atomic and safe while audio runs.
    int msegPointCount() const noexcept { return mseg.size(); }
    float msegPointX(int index) const noexcept { return mseg.x(index); }
    float msegPointY(int index) const noexcept { return mseg.y(index); }
    void setMsegPoint(int index, float x, float y) noexcept { mseg.setPoint(index, x, y); }
    bool addMsegPoint(float x, float y) noexcept { return mseg.addPoint(x, y); }
    bool removeMsegPoint(int index) noexcept { return mseg.removePoint(index); }
    void resetMseg() noexcept { mseg.reset(); }
    void addMsegRoute(uint8_t target);
    void moveModRoute(int from, int to);
    void sortModRoutes(int column, bool descending);
    int dspMode(int section) const {return (section>=0&&section<monomachine::DspSectionCount)?dspModes[static_cast<size_t>(section)]:monomachine::dspModeMnm;}
    int machineIndex() const {return juce::jlimit(0,static_cast<int>(nova::machines().size())-1,juce::roundToInt(parameters.getRawParameterValue("machine")->load()));}
private:
    enum Global {Level,Bpm,HostSync,Mix,Gate,ArpMode,ArpPlay,ArpSpeed,ArpRange,ArpLength,MacroX,MacroY,ArpOn,ArpHold,ArpSync,ArpTime,ArpGrid,ArpWrap,ArpVelocityMode,ArpStep,ArpStepVelocity,ArpStepTranspose,ArpStepHold,ArpStepPage,ArpStepPageLimit,ArpStepRandom,MsegRate,MsegSync,MsegLoop,GlobalCount};
    std::array<std::atomic<float>*,4> ampRaw{};
    // DSP mode lists ("mnm" default, "old" selectable). See models/DspModes.hpp.
    std::array<std::atomic<float>*,monomachine::DspSectionCount> modeRaw{};
    std::array<int,monomachine::DspSectionCount> dspModes{};
    std::atomic<float>* machineRaw=nullptr;
    std::array<std::array<std::atomic<float>*,8>,22> synthRaw{};
    std::array<std::array<std::atomic<float>*,8>,6> pageRaw{};
    std::array<std::array<std::atomic<float>*,5>,16> routeRaw{};
    std::array<std::atomic<float>*,GlobalCount> globalRaw{};
    std::array<float,GlobalCount> global{};
    std::array<float,32> base{};
    std::array<float,3> previousLfo{};
    std::array<float,32> smoothedParams{};
    float liveStepVelocity=0.0f, liveStepTranspose=0.0f, liveArpGate=0.0f;
    float selectedStepVelocity=1.0f, selectedStepTranspose=0.0f, selectedStepHold=0.0f;
    double msegPhase=0.0;
    float msegValue=0.0f;
    std::array<std::array<float,8>,3> lfoParams{},effectiveLfoParams{};
    struct Note {bool down=false;float velocity=1;uint64_t order=0;};
    std::array<Note,2048> notes{};
    std::array<bool,16> pedal{};
    std::array<int,16> wheels{};
    std::array<int,16> modWheels{};
    std::array<int,16> aftertouch{};
    std::array<bool,128> arpHeld{};
    uint64_t serial=0;int currentKey=-1,soundingNote=60,soundingChannel=0,activeMachine=-1,lastArpMode=-1;
    float ccX=-1,ccY=-1,previousX=64,previousY=0;
    double sr=44100,bpm=120;bool prepared=false;float blockPeak=0;
    nova::MachineEngine machine;
    nova::TrackChain chain;
    juce::SmoothedValue<float> midiGateBlend;
    // Fixed capacity covers the chorus adapter's latency at its maximum supported rate.
    struct DryDelay {
        std::array<float,2048> left{},right{};size_t write=0;
        void clear(){left.fill(0);right.fill(0);write=0;}
        void process(float l,float r,int latency,float& outL,float& outR){
            const auto delay=static_cast<size_t>(std::clamp(latency,0,2047));
            left[write]=l;right[write]=r;const auto read=(write+2048-delay)%2048;
            outL=left[read];outR=right[read];write=(write+1)%2048;
        }
    } dryDelay;
    nova::AmpEnvelope envelope;
    std::array<nova::LFO,3> lfos;
    monomachine::MonomachineArpeggiator arp;
    monomachine::ModulationMatrix matrix;
    monomachine::MSEG mseg;
    juce::SmoothedValue<float> pitch,level,mix,velocity;
    juce::CriticalSection sampleLock,stateLock;
    std::array<juce::MemoryBlock,24> sampleData;
    std::array<juce::String,24> sampleNames;
    juce::ValueTree pendingSamples;
    std::atomic<bool> panicRequested{false};
    std::atomic<float> peak{0};std::atomic<double> tempoDisplay{120};
    void snapshot();void panic();void midi(const juce::MidiMessage&);void selectNotes(bool);
    void syncArpNotes();void trigger(int,float,int);void release();void render(juce::AudioBuffer<float>&,int,int);
    void handleAsyncUpdate() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonomachineNovaAudioProcessor)
};
