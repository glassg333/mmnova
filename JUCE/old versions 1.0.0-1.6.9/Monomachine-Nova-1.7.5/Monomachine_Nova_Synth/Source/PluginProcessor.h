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
    std::atomic<int> playingSampleSlot{-1},lastBboxSample{-1},previewSlot{-1}; // 1.6.13: previewSlot -- прослушивание из списка
    std::atomic<int> arpStepEcho{0}; // 1.6.14: шаг арпеджиатора для STEP RND (только UI)
    std::atomic<int> arpPageEcho{0}; std::atomic<bool> arpAutoSwap{false}; // 1.6.42: FOLLOW PLAY / AUTO SWAP PAGE
    std::atomic<unsigned> bboxHitCounter{0};
    std::atomic<float>* portaSpeedRaw=nullptr; // 1.6.14: скорость портаменто
    float outputPeak() const {return peak.load();}
    double currentBpm() const {return tempoDisplay.load();}
    juce::String loadSample(int slot,const juce::File&);
    void previewSample(int slot); // 1.6.13: прослушивание слота BBOX из списка (PLAY)
    juce::String sampleName(int slot);
    void resetSamples(bool modern=true);
    void resetAllMsegs(); // 1.6.32: RESET ALL PARAMETERS -- тоже и MSEG-страницы
    void flushSampleRestore() {handleUpdateNowIfNeeded();}

    // MSEG editor/DSP bridge. Point writes are atomic and safe while audio runs.
    // 1.6.21: три кривых MSEG (индекс 0..2), каждая со своей фазой/скоростью.
    int msegPointCount(int m) const noexcept { return mseg[static_cast<size_t>(m)].size(); }
    float msegPointX(int m, int index) const noexcept { return mseg[static_cast<size_t>(m)].x(index); }
    float msegPointY(int m, int index) const noexcept { return mseg[static_cast<size_t>(m)].y(index); }
    void setMsegPoint(int m, int index, float x, float y) noexcept { mseg[static_cast<size_t>(m)].setPoint(index, x, y); }
    bool addMsegPoint(int m, float x, float y) noexcept { return mseg[static_cast<size_t>(m)].addPoint(x, y); }
    bool removeMsegPoint(int m, int index) noexcept { return mseg[static_cast<size_t>(m)].removePoint(index); }
    void resetMseg(int m) noexcept { mseg[static_cast<size_t>(m)].reset(); }
    void setMsegPointCount(int m, int n) noexcept { mseg[static_cast<size_t>(m)].setPointCount(n); }
    float msegSegK(int m, int segment) const noexcept { return mseg[static_cast<size_t>(m)].k(segment); } // 1.6.21: изгиб сегмента
    void setMsegSegK(int m, int segment, float k) noexcept { mseg[static_cast<size_t>(m)].setK(segment, k); }
    bool msegSteps(int m) const noexcept { return mseg[static_cast<size_t>(m)].isSteps(); }
    void setMsegSteps(int m, bool on) noexcept { mseg[static_cast<size_t>(m)].setSteps(on); }
    int addMsegRoute(int msegIndex, uint8_t target); // 1.6.21/34: какая из кривых; возвращает слот (для мигания ячейки)
    // 1.6.5: назначение модуляции «прицелом» перетаскиванием с кнопки источника
    // (LFO1/2/3): создать/перезаписать маршрут src->target и включить его.
    // 1.7.1: P-LOCK -- значение параметра на степ (Elektron-style), 4 слота x 16 страниц x 8 степов
    static constexpr int kPlockSlots=32; // 1.7.2: практический предел "безгранично" -- играют все, UI показывает по одному
    void plockSetTarget(int slot,int target); // -1 = слот пуст
    int plockAcquire(int slot,int target); // 1.7.2: та же цель = тот же слот, иначе первый свободный
    void plockResetAll(); // 1.7.5: RESET ALL PARAMETERS -- очистить все слоты P-LOCK
    juce::ValueTree plockToTree() const; void plockFromTree(const juce::ValueTree&); // 1.7.3: сериализация для хост-состояния и undo/redo
    void plockSetPolarity(int slot,int pol);  // 0 = UNI (абсолют), 1 = BIP (вокруг текущего)
    void plockSetValue(int slot,int page,int step,float v); // 0 = снять лок
    void plockSetSlide(int slot,int page,int step,bool on);
    void plockFillPage(int slot,int page);    // бинд сразу назначает степы страницы
    float plockValue(int slot,int page,int step) const;
    float plockSlideValue(int slot,int page,int step) const;
    int plockTarget(int slot) const;
    int plockPolarity(int slot) const;
    int addRouteFromSource(int src, uint8_t target); // 1.6.33: возвращает слот (для мигания DEST-ячейки)
    void moveModRoute(int from, int to);
    void sortModRoutes(int column, bool descending);
    int dspMode(int section) const {return (section>=0&&section<monomachine::DspSectionCount)?dspModes[static_cast<size_t>(section)]:monomachine::dspModeMnm;}
    // 1.6.8: SYNT mode is stored per machine (mode_synt_m<id>), so the selected
    // machine keeps its own mnm/old choice while cycling through the machines.
    juce::String syntModeParamIdFor(int machineListIndex) const {
        const int m=juce::jlimit(0,static_cast<int>(nova::machines().size())-1,machineListIndex);
        return juce::String(monomachine::dspMachineModeParamId(nova::machines()[static_cast<size_t>(m)].id));
    }
    juce::String dspModeParamIdFor(int section) const {
        return (section==monomachine::DspSynt)?syntModeParamIdFor(machineIndex()):juce::String(monomachine::dspModeParamId(section));
    }
    int machineIndex() const {return juce::jlimit(0,static_cast<int>(nova::machines().size())-1,juce::roundToInt(parameters.getRawParameterValue("machine")->load()));}
private:
    enum Global {Level,Bpm,HostSync,Mix,Gate,ArpMode,ArpPlay,ArpSpeed,ArpRange,ArpLength,MacroX,MacroY,ArpOn,ArpHold,ArpSync,ArpTime,ArpGrid,ArpWrap,ArpVelocityMode,ArpStep,ArpStepVelocity,ArpStepTranspose,ArpStepHold,ArpStepPage,ArpStepPageLimit,ArpStepRandom,ArpStepRnd,MsegRate,MsegSync,MsegLoop,Mseg2Rate,Mseg2Sync,Mseg2Loop,Mseg3Rate,Mseg3Sync,Mseg3Loop,
        Mseg4Rate,Mseg4Sync,Mseg4Loop,Mseg5Rate,Mseg5Sync,Mseg5Loop,Mseg6Rate,Mseg6Sync,Mseg6Loop,
        Mseg7Rate,Mseg7Sync,Mseg7Loop,Mseg8Rate,Mseg8Sync,Mseg8Loop,MsegRetrig,Mseg2Retrig,Mseg3Retrig,Mseg4Retrig,Mseg5Retrig,Mseg6Retrig,Mseg7Retrig,Mseg8Retrig,GlobalCount}; // 1.7.2: RETRIG постранично // 1.7.1: +ARP STEP RND, MSEG RETRIG // 1.6.21: три MSEG; 1.6.29: до восьми
    std::array<std::atomic<float>*,16> auxRaw{}; // 1.6.24: AUX SOURCE маршрутов
    std::array<std::atomic<float>*,16> auxDepthRaw{}; // 1.6.32: AUX DEPTH маршрутов
    std::array<std::atomic<float>*,4> ampRaw{};
    std::atomic<float>* repitchRaw=nullptr; // 1.6.5/1.6.8: dly_repitch (непрерывный 0..3)
    std::atomic<float>* repitchSmoothRaw=nullptr; // 1.6.8: dly_repitch_smooth (антиклик)
    std::atomic<float>* ppModeRaw=nullptr; // 1.6.12: dly_ppmode (CLASSIC/MID SAFE)
    std::array<std::atomic<float>*,12> lfoLockRaw{}; // 1.6.12: page/dest locks+solo каждого LFO
    // 1.6.12: копии замков для аудио-потока + последние разрешённые PAGE/DEST (sticky)
    std::array<int,3> lfoPageLocks{},lfoDestLocks{},lfoPageSolo{},lfoDestSolo{},lfoLastPage{},lfoLastDest{};
    // DSP mode lists ("mnm" default, "old" selectable). See models/DspModes.hpp.
    std::array<std::atomic<float>*,monomachine::DspSectionCount> modeRaw{};
    std::array<std::atomic<float>*,24> syntModeRaw{}; // 1.6.8: per-machine SYNT DSP mode
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
    // 1.7.1: P-LOCK -- данные слотов (атомики: UI пишет, аудио читает)
    std::array<std::atomic<float>,kPlockSlots*128> plockVal{},plockSlideV{};
    std::array<std::atomic<int>,kPlockSlots> plockTargetI{},plockPolI{};
    float plockMaxFor(int t) const; float plockScaled(int t,int slot,float v) const;
    void plockStep(int page,int step); void plockClearRuntime();
    std::array<bool,66> plockOn{}; std::array<bool,66> plockAbs{}; std::array<float,66> plockNow{};
    struct PlockRun { bool active=false,sliding=false; float from=0,to=0,now=0; int span=0,prog=0; };
    std::array<PlockRun,kPlockSlots> plockRun{};
    std::array<double,8> msegPhase{}; // 1.6.21; 1.6.29: восемь страниц
    std::array<float,8> msegValue{};
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
    monomachine::MSEG mseg[8]; // 1.6.21: MSEG1..MSEG3; 1.6.29: MSEG1..MSEG8
public: // 1.6.29: страницы MSEG (сколько видно в списке редактора и матрице)
    int msegPageCount() const noexcept { return 8; } // 1.6.31: страницы фиксированы -- все 8 всегда активны
    void setMsegPageCount(int) noexcept {} // 1.6.31: счётчик страниц фиксирован (совместимость вызовов)
    void addMsegPage(); // 1.6.29: +1 страница (кривая/параметры по умолчанию)
    void removeMsegPage(int page); // 1.6.29: удалить страницу, переименовать назначения
private:
    std::atomic<int> msegPages{3};
    juce::SmoothedValue<float> pitch,level,mix,velocity;
    juce::CriticalSection sampleLock,stateLock;
    std::array<juce::MemoryBlock,24> sampleData;
    std::array<juce::String,24> sampleNames;
    std::vector<float> previewBuffer;int previewPos=0;juce::CriticalSection previewLock; // 1.6.13: голос предпрослушивания
    juce::ValueTree pendingSamples;
    std::atomic<bool> panicRequested{false};
    std::atomic<float> peak{0};std::atomic<double> tempoDisplay{120};
    void snapshot();void panic();void midi(const juce::MidiMessage&);void selectNotes(bool);
    void syncArpNotes();void trigger(int,float,int);void release();void render(juce::AudioBuffer<float>&,int,int);
    void handleAsyncUpdate() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonomachineNovaAudioProcessor)
};
