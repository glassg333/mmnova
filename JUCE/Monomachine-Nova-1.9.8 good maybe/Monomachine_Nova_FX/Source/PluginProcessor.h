#pragma once
#include "NovaData.h"
#include "NovaDSP.h"
#include "FxSlotEngine.h" // 1.8.4: ленивое родное ядро FX-слоты для слота
#include "dsp/FeedbackNetwork.h" // 1.8.4: one-sample FB routes, expandable to a small graph
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
    void queueArpPageRandomWrite(int firstStep,int lastStep);
    int playingSample() const {return playingSampleSlot.load();}
    std::atomic<int> playingSampleSlot{-1},lastBboxSample{-1},previewSlot{-1}; // 1.6.13: previewSlot -- прослушивание из списка
    std::atomic<int> arpStepEcho{0}; // absolute live ARP step 0..63 for compressed lanes
    std::atomic<int> plockStepEcho{0}; // current independent/synchronised P-LOCK step for its lane
    std::atomic<int> plockUiSlot{0}; // 1.7.9: выбранный слот P-LOCK общий (синх-страница и детач-окно показывают один)
    float rndNow=0.0f,rndTarget=0.0f; int rndCount=0; juce::Random rndSrc; // 1.7.10: RANDOM-источник матрицы (сглаженный шум)
    std::atomic<int> arpPageEcho{0}; std::atomic<bool> arpAutoSwap{false}; // 1.6.42: FOLLOW PLAY / AUTO SWAP PAGE
    std::atomic<bool> arpFollowPlay{false}; // 1.7.6: FOLLOW PLAY переживает пересоздание ARP-страницы (галки больше не отжимаются)
    std::array<std::atomic<double>,8> msegPhaseUi{}; // 1.7.6: фазы MSEG для маркера PLAY FOLLOWER на странице MSEG
    bool hostWasPlaying=false; // 1.7.7: фронт старта плейхоста
    std::atomic<unsigned> bboxHitCounter{0};
    std::atomic<float>* portaSpeedRaw=nullptr; // 1.6.14: скорость портаменто
    // New live STEP window. Legacy page IDs remain in Global for old sessions,
    // while this pair is the 1..64 active sequence boundary.
    std::atomic<float>* arpStepStartRaw=nullptr;
    std::atomic<float>* arpStepEndRaw=nullptr;
    std::atomic<float>* plockSyncRaw=nullptr;
    std::atomic<float>* plockStepStartRaw=nullptr;
    std::atomic<float>* plockStepEndRaw=nullptr;
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
    enum Global {Level,Bpm,HostSync,Gate,ArpMode,ArpPlay,ArpSpeed,ArpRange,ArpLength,MacroX,MacroY,ArpOn,ArpHold,ArpSync,ArpTime,ArpGrid,ArpWrap,ArpVelocityMode,ArpStep,ArpStepVelocity,ArpStepTranspose,ArpStepHold,ArpStepPage,ArpStepPageLimit,ArpStepRandom,ArpStepRnd,MsegRate,MsegSync,MsegLoop,Mseg2Rate,Mseg2Sync,Mseg2Loop,Mseg3Rate,Mseg3Sync,Mseg3Loop,
        Mseg4Rate,Mseg4Sync,Mseg4Loop,Mseg5Rate,Mseg5Sync,Mseg5Loop,Mseg6Rate,Mseg6Sync,Mseg6Loop,
        Mseg7Rate,Mseg7Sync,Mseg7Loop,Mseg8Rate,Mseg8Sync,Mseg8Loop,MsegRetrig,MsegRmode,MsegKey,MsegLpoint,Mseg2Retrig,Mseg2Rmode,Mseg2Key,Mseg2Lpoint,Mseg3Retrig,Mseg3Rmode,Mseg3Key,Mseg3Lpoint,Mseg4Retrig,Mseg4Rmode,Mseg4Key,Mseg4Lpoint,Mseg5Retrig,Mseg5Rmode,Mseg5Key,Mseg5Lpoint,Mseg6Retrig,Mseg6Rmode,Mseg6Key,Mseg6Lpoint,Mseg7Retrig,Mseg7Rmode,Mseg7Key,Mseg7Lpoint,Mseg8Retrig,Mseg8Rmode,Mseg8Key,Mseg8Lpoint,GlobalCount}; // 1.7.8: +RMODE/KEY/LPOINT постранично (порядок = NovaData: retrig,rmode,key,lpoint); 1.7.2: RETRIG постранично // 1.7.1: +ARP STEP RND, MSEG RETRIG // 1.6.21: три MSEG; 1.6.29: до восьми
    std::array<std::atomic<float>*,64> auxRaw{}; // 1.6.24/1.7.7: AUX SOURCE маршрутов (64 слота)
    std::atomic<float> lfoFmValue{0.0f}; // 1.7.8: LFO FM из матрицы (октавы, +-13.3) -- читается в начале блока, пишется в evaluate
    std::array<std::atomic<float>*,64> auxDepthRaw{}; // 1.6.32/1.7.7: AUX DEPTH маршрутов (64 слота)
    std::array<std::atomic<float>*,4> ampRaw{};
    std::atomic<float>* repitchRaw=nullptr; // 1.6.5/1.6.8: dly_repitch (непрерывный 0..3)
    std::atomic<float>* repitchSmoothRaw=nullptr; // 1.6.8: dly_repitch_smooth (антиклик)
    std::atomic<float>* ppModeRaw=nullptr; // 1.6.12: dly_ppmode (CLASSIC/MID SAFE)
    // 1.9.8: optional Q controls for the two visible DBAS/DWID feedback edges.
    // P1/P2 are intentionally independent, like their own EFFX pages.
    std::array<std::atomic<float>*,2> delayFeedbackQP1Raw{},delayFeedbackQP2Raw{};
    std::array<std::atomic<float>*,48> lfoLockRaw{}; // 1.8.0e: 12 LFO (P1 = lfo1..6, P2 = p2lfo1..6); 1.6.12: page/dest locks+solo
    // 1.6.12: копии замков для аудио-потока + последние разрешённые PAGE/DEST (sticky)
    std::array<int,12> lfoPageLocks{},lfoDestLocks{},lfoPageSolo{},lfoDestSolo{},lfoLastPage{},lfoLastDest{}; // 1.8.0e: 12 LFO
    // DSP mode lists ("mnm" default; section-local opt-in alternatives). See models/DspModes.hpp.
    std::array<std::atomic<float>*,monomachine::DspSectionCount> modeRaw{};
    std::array<std::atomic<float>*,24> syntModeRaw{}; // 1.6.8: per-machine SYNT DSP mode
    std::array<int,monomachine::DspSectionCount> dspModes{};
    std::atomic<float>* machineRaw=nullptr;
    std::array<std::array<std::atomic<float>*,8>,22> synthRaw{};
    std::array<std::array<std::atomic<float>*,8>,15> pageRaw{}; // 1.8.0e: +страницы 9..14 (P2 LFO1-6)
    std::array<std::array<std::atomic<float>*,5>,64> routeRaw{}; // 1.7.7: 64 слота
    std::array<std::atomic<float>*,GlobalCount> globalRaw{};
    std::array<float,GlobalCount> global{};
    std::array<float,32> base{};
    std::array<float,12> previousLfo{}; // 1.8.0e: 12 LFO
    std::array<float,32> smoothedParams{};
    float liveStepVelocity=0.0f, liveStepTranspose=0.0f, liveArpGate=0.0f;
    float selectedStepVelocity=1.0f, selectedStepTranspose=0.0f, selectedStepHold=0.0f;
    // PAGE RND is a write-on-enable action, not a transient render overlay.
    // Audio only queues it; the AsyncUpdater performs APVTS writes safely on
    // the message thread and does not touch sample restoration unless needed.
    // Base boundaries come from automatable APVTS values; runtime boundaries
    // additionally include Matrix modulation and are consumed by the engines.
    int arpStepWindowStartBase=0,arpStepWindowEndBase=15;
    int arpStepWindowStartRuntime=0,arpStepWindowEndRuntime=15;
    int plockStepWindowStartRuntime=0,plockStepWindowEndRuntime=15;
    int plockStepCursor=0;
    bool plockSyncRuntime=true;
    int plockStepForArpEvent(int arpAbsoluteStep) noexcept;
    void applyArpAndPlockWindowModulation(const std::array<float,2>& arpWindowMod,const std::array<float,2>& plockWindowMod) noexcept;
    std::atomic<bool> arpPageRndWasEnabled{false};
    std::atomic<bool> arpPageRndWritePending{false};
    std::atomic<int> arpPageRndPendingStart{0},arpPageRndPendingEnd{15};
    uint32_t arpPageRndSeed=0x6d2b79f5u;
    // 1.7.1: P-LOCK -- данные слотов (атомики: UI пишет, аудио читает)
    std::array<std::atomic<float>,kPlockSlots*128> plockVal{},plockSlideV{};
    std::array<std::atomic<int>,kPlockSlots> plockTargetI{},plockPolI{};
    float plockMaxFor(int t) const; float plockScaled(int t,int slot,float v) const;
    void plockStep(int absoluteStep); void plockClearRuntime();
    std::array<bool,244> plockOn{}; std::array<bool,244> plockAbs{}; std::array<float,244> plockNow{}; // 1.8.1: до 243 (+MSEG RATE 236..243) // 1.8.0e: до 235 (LFO-строки 4-6/P2) // 1.8.0b: 164 -- P-LOCK на P2 (132..163) и LFO FM
    struct PlockRun { bool active=false,sliding=false; float from=0,to=0,now=0; int span=0,prog=0; };
    std::array<PlockRun,kPlockSlots> plockRun{};
    std::array<double,8> msegPhase{}; // 1.6.21; 1.6.29: восемь страниц
    std::array<float,8> msegRateSum{}; // 1.8.1: суммы маршрутов MSEG RATE (цели 236..243) -- читается в этом же аудио-блоке
    std::array<float,8> msegValue{};
    std::array<std::array<float,8>,12> lfoParams{},effectiveLfoParams{}; // 1.8.0e: 12 LFO (0..5 = P1, 6..11 = P2)
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
    } dryDelay, dryDelay2; // 1.8.0: латентность хоруса P2
    nova::AmpEnvelope envelope;
    std::array<nova::LFO,3> lfos;
    std::array<nova::LFO,3> lfos3; // 1.8.0e: P1 LFO4-6 (страницы 6..8)
    // ===== 1.8.0: P2 -- полноценный FX-движок (второй экземпляр, прописка p2_) =====
    std::atomic<float>* p2MachineRaw=nullptr;
    std::array<std::array<std::atomic<float>*,8>,32> p2HandRaw{};
    std::array<std::array<std::atomic<float>*,8>,2> p2PageRaw{};
    std::array<std::array<std::array<std::atomic<float>*,3>,8>,16> stepRaw{}; // 1.8.1b: кэш hold/transpose/velocity -- snapshot без String на каждый блок
    std::array<std::atomic<float>*,8> p2AmpRaw{}; // atk hold dec rel mode curveA curveD curveR
    std::atomic<float>* p2MixRaw=nullptr;std::array<std::atomic<float>*,3> p2GainRaw{}; // 1.8.0: P2 DIST/VOL/PAN (p2_0_4..6); p2_level убран -- LEV фейдер мастер всего плагина
    std::atomic<float>* p2ModeRaw[3]{}; // 1.8.0: FILT/DIST/DLY режимы цепи P2
    // Private hybrid closed-test controls: L/H select physical filter edges;
    // S selects only the existing post-filter DIST block.  They never remap
    // the native mnm or retained old selectors.
    std::array<std::atomic<float>*,3> hybridP1Raw{},hybridP2Raw{};
    // Direct VEL/KT/SAT and additive FIL ENV settings stay separate from the
    // historic eight FILTER page controls. Index order: VEL-L, VEL-H, KT-L,
    // KT-H, SAT, ATK, HOLD, DEC, REL, ENV FIL, BASE depth, WDTH depth.
    std::array<std::atomic<float>*,12> filterExtrasP1Raw{},filterExtrasP2Raw{};
    std::array<std::array<std::atomic<float>*,4>,4> modEnvRaw{};
    std::array<nova::AmpEnvelope,4> modEnvelopes{};
    std::array<float,4> modEnvValue{};
    nova::MachineEngine machine2; nova::TrackChain chain2; nova::AmpEnvelope envelope2;
    // P2 used to bypass P1's anti-zipper path entirely. It now has its own
    // continuous-control state, including the group DRY/WET crossfade.
    std::array<float,32> smoothedParams2{};
    bool p2SmoothingPrimed=false;
    juce::SmoothedValue<float> p2WetMix;
    bool p1ChorusSafeActive=false,p2ChorusSafeActive=false;
    std::array<nova::LFO,3> lfos2; // 1.8.0e: P2 LFO1-3 (страницы 9..11)
    std::array<nova::LFO,3> lfos4; // 1.8.0e: P2 LFO4-6 (страницы 12..14)
    std::array<float,32> amp2{}; // 1.8.0: p2Level-сглаживание убрано вместе с P2 LEV
    monomachine::MonomachineArpeggiator arp;
    monomachine::ModulationMatrix matrix;
    monomachine::MSEG mseg[8]; // 1.6.21: MSEG1..MSEG3; 1.6.29: MSEG1..MSEG8
public: // 1.8.0: список FX-слотов для страницы P2
    // Explicit non-original safety option. Default OFF leaves the native chorus
    // running at MIX=0, preserving its real tail/state behaviour.
    std::atomic<bool> chorusSafe{false};

    // ===== 1.8.5: FX-СЛОТЫ ===================================================
    // Шляпки на FX-странице -- это только обозначения существующих стадий кода.
    // Пользовательские слоты вставляются в физические перемычки; P1/P2 не
    // пересоздаются. Documented track path: EQ -> FILT -> DIST -> AMP ENV ->
    // VOL/PAN -> SRR -> DELAY -> final LEV. Numeric zone IDs stay stable;
    // their captions are corrected to this physical default. g15/g16 are the
    // explicit SRR->DELAY bridges.
    enum { fxG_P1pre=0, fxG_EQ_FILT=1, fxG_FILT_DIST=2, fxG_DIST_ENV=3,
           fxG_ENV_VOLPAN=4, fxG_VOLPAN_SRR=5, fxG_P1post=6, fxG_P2pre=7,
           fxG_P2MACH_CHAIN=8, fxG_P2post=9, fxG_P2_EQ_FILT=10,
           fxG_P2_FILT_DIST=11, fxG_P2_DIST_ENV=12, fxG_P2_ENV_VOLPAN=13,
           fxG_P2_VOLPAN_SRR=14, fxG_P1_SRR_DELAY=15,
           fxG_P2_SRR_DELAY=16, fxZonesCount=17 };
    static constexpr int fxSlotsMax=32;

    // Классические DSP-блоки P1/P2 можно переставлять между физическими
    // перемычками. Номера перемычек остаются на экране и FX-слоты намеренно
    // остаются в своей физической точке, а не следуют за блоком.
    // Stage IDs 0..5 remain stable for serialized routes.  VOL/PAN is a
    // distinct seventh block; legacy six-block routes are migrated on load.
    enum ClassicStage { classicDist=0,classicSrr=1,classicFilt=2,classicEq=3,
                        classicEnv=4,classicDelay=5,classicDsnd=classicDelay,
                        classicVolPan=6,classicStagesCount=7 };
    struct ClassicRouteState {
        std::array<int,classicStagesCount> p1{{classicEq,classicFilt,classicDist,classicEnv,classicVolPan,classicSrr,classicDelay}};
        std::array<int,classicStagesCount> p2{{classicEq,classicFilt,classicDist,classicEnv,classicVolPan,classicSrr,classicDelay}};
        // Indexed by stable stage ID, not its movable route position.
        std::array<bool,classicStagesCount> p1Enabled{{true,true,true,true,true,true,true}};
        std::array<bool,classicStagesCount> p2Enabled{{true,true,true,true,true,true,true}};
    };
    ClassicRouteState classicRouteState() const noexcept;
    void classicRouteMove(int chain,int stage,int targetPosition); // chain: 0=P1, 1=P2
    void classicRouteSetEnabled(int chain,int stage,bool enabled);
    void classicRouteReset(int chain); // documented reset order and ON defaults, modes untouched
    void classicRoutesResetAll();

    // Снимок одного слота для UI/сериализации. machineId==0 означает пустой слот;
    // иначе это РОДНОЙ id из p2FxMachines(), а p -- восемь ручек именно этого слота.
    struct FxSlotState {
        bool used=false;
        int zone=fxG_P1pre;
        int machineId=0;
        bool on=false;
        int order=0;
        std::array<float,8> p{{64,64,64,64,64,64,64,64}};
    };

    // UI-мост. Все записи идут через атомарную control-копию; аудиопоток принимает
    // цельный снимок между блоками. Поэтому UI никогда не пишет прямо в DSP-объекты.
    int fxSlotAdd(int zone);
    void fxSlotRemove(int slot);
    void fxSlotsClearAll(); // RESET ALL: убрать все пользовательские FX-слоты
    void fxSlotSetMachine(int slot,int machineId);
    void fxSlotSetOn(int slot,bool on);
    void fxSlotSetParameter(int slot,int parameter,float value);
    void fxSlotMoveToGap(int slot,int targetZone,int targetPosition);
    FxSlotState fxSlotState(int slot) const noexcept;
    int fxSlotUsedCount() const noexcept;
    static int fxVisualZoneAt(int visualIndex) noexcept;
    static int fxVisualIndexOfZone(int zone) noexcept;

    // One FB route is exposed in v1. The DSP network owns room for multiple
    // independent routes, so cross-feedback/waveguide work adds routes rather
    // than replacing this state model. Values are panel-style raw 0..127.
    struct FeedbackState {
        bool on=false;
        int sendZone=fxG_P1post;
        int returnZone=fxG_P1pre;
        float sendGain=64.0f;
        float feedbackGain=0.0f;
        float returnGain=64.0f;
        float dcControl=64.0f;
        bool clipEnabled=true;
    };
    FeedbackState feedbackState() const noexcept;
    void feedbackSetOn(bool on);
    void feedbackSetSendZone(int zone);
    void feedbackSetReturnZone(int zone);
    void feedbackSetSendGain(float value);
    void feedbackSetFeedbackGain(float value);
    void feedbackSetReturnGain(float value);
    void feedbackSetDcControl(float value);
    void feedbackSetClipEnabled(bool enabled);
    void feedbackReset(); // RESET ALL: route OFF + normal manual defaults
    static float feedbackDcHz(float raw) noexcept;

    void fxZone(int zone,float* l,float* r,int n) noexcept;
    static const std::vector<monomachine::MachineDef>& p2FxMachines();

private:
    struct FxSlotControl {
        std::atomic<bool> used{false};
        std::atomic<int> zone{fxG_P1pre};
        std::atomic<int> machineId{0};
        std::atomic<bool> on{false};
        std::atomic<int> order{0};
        std::array<std::atomic<float>,8> p{};
        FxSlotControl() noexcept { for(auto& v:p)v.store(64.0f,std::memory_order_relaxed); }
    };
    std::array<FxSlotControl,fxSlotsMax> fxSlotControls{};
    std::array<nova::FxSlotEngine,fxSlotsMax> fxEngines{}; // создают MachineEngine только у реально включённого слота
    std::atomic<uint64_t> fxSlotEditSerial{0}; // even=готовый снимок, odd=UI пишет пачку
    uint64_t fxSlotAudioSerial=~uint64_t{0};   // доступ только из аудиопотока
    std::array<FxSlotState,fxSlotsMax> fxSlots{}; // аудиокопия, ею же определяется порядок в зоне
    std::array<std::array<int,fxSlotsMax>,fxZonesCount> fxSlotRoute{};
    std::array<int,fxZonesCount> fxSlotRouteCount{};
    bool fxSlotsUsed=false;
    int fxSlotsLatency=0; // сумма задержек активных slot-CHORUS для репорта хосту; внутри FB не компенсируется
    void fxSlotsBeginEdit() noexcept;
    void fxSlotsEndEdit() noexcept;
    void fxSlotsSyncToAudio() noexcept;
    void fxSlotsPrepare();
    juce::ValueTree fxSlotsToTree() const;
    void fxSlotsFromTree(const juce::ValueTree&);
    static std::array<float,8> fxSlotDefaultsForMachine(int machineId);
    static bool isFxSlotMachineId(int machineId) noexcept;

    struct ClassicRouteControl {
        std::array<std::atomic<int>,classicStagesCount> p1{};
        std::array<std::atomic<int>,classicStagesCount> p2{};
        std::array<std::atomic<bool>,classicStagesCount> p1Enabled{};
        std::array<std::atomic<bool>,classicStagesCount> p2Enabled{};
        ClassicRouteControl() noexcept {
            const int canonical[classicStagesCount]={classicEq,classicFilt,classicDist,classicEnv,classicVolPan,classicSrr,classicDelay};
            for(int i=0;i<classicStagesCount;++i){
                p1[static_cast<size_t>(i)].store(canonical[i],std::memory_order_relaxed);
                p2[static_cast<size_t>(i)].store(canonical[i],std::memory_order_relaxed);
                p1Enabled[static_cast<size_t>(i)].store(true,std::memory_order_relaxed);
                p2Enabled[static_cast<size_t>(i)].store(true,std::memory_order_relaxed);
            }
        }
    };
    ClassicRouteControl classicRouteControl{};
    std::atomic<uint64_t> classicRouteEditSerial{0};
    uint64_t classicRouteAudioSerial=~uint64_t{0};
    ClassicRouteState classicRouteAudio{};
    void classicRouteBeginEdit() noexcept;
    void classicRouteEndEdit() noexcept;
    void classicRouteSyncToAudio() noexcept;
    juce::ValueTree classicRouteToTree() const;
    void classicRouteFromTree(const juce::ValueTree&);

    struct FeedbackControl {
        std::atomic<bool> on{false};
        std::atomic<int> sendZone{fxG_P1post};
        std::atomic<int> returnZone{fxG_P1pre};
        std::atomic<float> sendGain{64.0f};
        std::atomic<float> feedbackGain{0.0f};
        std::atomic<float> returnGain{64.0f};
        std::atomic<float> dcControl{64.0f};
        std::atomic<bool> clipEnabled{true};
    };
    FeedbackControl feedbackControl{};
    std::atomic<uint64_t> feedbackEditSerial{0};
    uint64_t feedbackAudioSerial=~uint64_t{0};
    FeedbackState feedbackAudio{};
    std::array<nova::FeedbackRouteState,nova::FeedbackNetwork::kMaxLines> feedbackRoutes{};
    nova::FeedbackNetwork feedbackNetwork{};
    void feedbackBeginEdit() noexcept;
    void feedbackEndEdit() noexcept;
    void feedbackSyncToAudio() noexcept;
    juce::ValueTree feedbackToTree() const;
    void feedbackFromTree(const juce::ValueTree&);

public:
public: // 1.6.29: страницы MSEG (сколько видно в списке редактора и матрице)
    int msegPageCount() const noexcept { return 8; } // 1.6.31: страницы фиксированы -- все 8 всегда активны
    void setMsegPageCount(int) noexcept {} // 1.6.31: счётчик страниц фиксирован (совместимость вызовов)
    void addMsegPage(); // 1.6.29: +1 страница (кривая/параметры по умолчанию)
    void removeMsegPage(int page); // 1.6.29: удалить страницу, переименовать назначения
private:
    std::atomic<int> msegPages{3};
    juce::SmoothedValue<float> pitch,level,mix,velocity; // 1.8.0: PORT P2 убран -- глайд дело синтовой страницы P1
    juce::CriticalSection sampleLock,stateLock;
    std::array<juce::MemoryBlock,24> sampleData;
    std::array<juce::String,24> sampleNames;
    std::vector<float> previewBuffer;int previewPos=0;juce::CriticalSection previewLock; // 1.6.13: голос предпрослушивания
    juce::ValueTree pendingSamples;
    std::atomic<bool> panicRequested{false};
    std::atomic<float> peak{0};std::atomic<double> tempoDisplay{120};
    void snapshot();void panic();void midi(const juce::MidiMessage&);void selectNotes(bool);
    void syncArpNotes();void trigger(int,float,int);void release();void render(juce::AudioBuffer<float>&,int,int);
    void renderStandard(juce::AudioBuffer<float>&,int,int); // original 8-sample cadence while FB is OFF
    void renderFeedback(juce::AudioBuffer<float>&,int,int); // exact 1-sample audio FB, cached 8-sample control plane
    void handleAsyncUpdate() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonomachineNovaAudioProcessor)
};
