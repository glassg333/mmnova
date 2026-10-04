#pragma once
#include <JuceHeader.h>
#include "dsp/monomachine_fm_dynamic.hpp"
#include "dsp/monomachine_fm_par.hpp"
#include "dsp/monomachine_bbox.hpp"
#include "dsp/AudioAdapter.h"
#include "dsp/NoiseContour.h"
#include "dsp/monomachine_effects.hpp"
#include "dsp/mnm/MnmTables.hpp"
#include "dsp/mnm/MnmKernel.hpp"
#include "dsp/mnm/MnmRealFilter.hpp"
#include "dsp/mnm/MnmFm.hpp"
#include "dsp/fm_fix/MnmFmFix.hpp"
#include "dsp/fm_fix/OldFmFix.hpp"
#include "dsp/fm_new/FmExactNew.hpp"
#include "dsp/fm_new/FmNewLevelBridge.hpp"
#include "dsp/fm_new_fix/FmExactNewFix.hpp"
#include "dsp/fm_fix/FmFixTables.hpp"
#include "dsp/mnm/MnmDelay.hpp"
#include "dsp/mnm/MnmTrackDelayNew.hpp"
#include "dsp/TrackDelayRouting.hpp"
#include "dsp/DelayFeedbackDynamics.hpp"
#include "models/DspModes.hpp"
#include "dsp/monomachine_filter.hpp"
#include <array>
#include <cmath>
#include <memory>

namespace nova {
constexpr float pi=3.14159265358979323846f;
inline float norm(float v) {return std::clamp(v/127.0f,0.0f,1.0f);}
inline float hz(float note) {return 440.0f*std::pow(2.0f,(note-69.0f)/12.0f);}
inline double wrap(double x) {return x-std::floor(x);}
inline float noise(uint32_t& state) {state^=state<<13;state^=state>>17;state^=state<<5;return (state/4294967295.0f)*2-1;}
inline float blep(float t,float dt) {if(t<dt){t/=dt;return t+t-t*t-1;} if(t>1-dt){t=(t-1)/dt;return t*t+t+t+1;}return 0;}
inline float lfoShape(int kind,double phase) {
    const float t=static_cast<float>(wrap(phase));
    const int shape=kind/2;float v=0;
    switch(shape){case 0:v=1-4*std::abs(t-0.5f);break;case 1:v=2*t-1;break;
        case 2:v=t<0.5f?1.0f:-1.0f;break;case 3:v=2*std::exp(-5*t)-1;break;
        case 4:v=1-2*t;break;default:return 0;}
    return kind%2?-v:v;
}
inline float bipolarDist(float input,float raw) {
    return monomachine::mnm::oldBipolarSaturator(input, raw);
}
inline void delayWrite(float dryL,float dryR,float a,float b,float send,float feedback,
                       bool negativeComb,bool invertFeedback,float& nextL,float& nextR) {
    // DSND keeps its observed send topology: positive is one mono comb and
    // negative is split L/R. The two explicit experimental switches can flip
    // feedback polarity only; with both OFF this is the retained exact route.
    const float mono=0.5f*(dryL+dryR);
    const bool negativeFeedback=negativeComb!=invertFeedback;
    nextL=(negativeComb?dryL:mono)*send+(negativeFeedback?-a:a)*feedback;
    nextR=(negativeComb?dryR:mono)*send+(negativeFeedback?-b:b)*feedback;
}
inline float wave(int type,double phase,float dt,float pw=0.5f) {
    float t=static_cast<float>(wrap(phase));dt=std::clamp(dt,0.000001f,0.49f);
    switch(type%4) {
        case 0:return std::sin(2*pi*t);
        case 1:return 2*t-1-blep(t,dt);
        case 2:return (t<pw?1.0f:-1.0f)+blep(t,dt)-blep(static_cast<float>(wrap(t-pw)),dt);
        default:return 1-4*std::abs(t-0.5f);
    }
}
inline float digitalWave(float slot,double phase,float bright) {
    int family=static_cast<int>(slot)/16;float result=0,weight=0;
    for(int h=1;h<=10;++h) {
        float a=std::pow(std::max(0.03f,bright),static_cast<float>(h-1))/h;
        if(family%2 && h%2==0) a*=0.15f;
        if(family>=4) a*=0.5f+0.5f*std::sin(static_cast<float>(h)*(1+slot/18));
        result+=a*std::sin(2*pi*static_cast<float>(wrap(phase*h))+(family%3)*0.3f*h);
        weight+=a;
    }
    return result/std::max(0.1f,weight);
}
struct Biquad {
    double b0=1,b1=0,b2=0,a1=0,a2=0,z1=0,z2=0;
    void clear(){z1=z2=0;}
    float tick(float x){double y=b0*x+z1;z1=b1*x-a1*y+z2;z2=b2*x-a2*y;return static_cast<float>(y);}
    void peak(double sr,float frequency,float db) {
        double w=2*pi*std::clamp(frequency,20.0f,static_cast<float>(sr*0.45))/sr;
        double a=std::pow(10.0,db/40.0),alpha=std::sin(w)/2,den=1+alpha/a;
        b0=(1+alpha*a)/den;b1=-2*std::cos(w)/den;b2=(1-alpha*a)/den;a1=b1;a2=(1-alpha/a)/den;
    }
    void band(double sr,float frequency,float q) {
        double w=2*pi*std::clamp(frequency,20.0f,static_cast<float>(sr*0.45))/sr;
        double alpha=std::sin(w)/(2*q),den=1+alpha;
        b0=alpha/den;b1=0;b2=-b0;a1=-2*std::cos(w)/den;a2=(1-alpha)/den;
    }
};
struct AmpEnvelope {
    double sr=44100;int stage=0;float level=0;int holdRemaining=0;bool gate=false;
    std::array<float,4> p{0,0,64,64};
    static constexpr int kKernelAlgorithm=2;
    monomachine::mnm::AmpEnvelope kernelEnv{};bool kernelHold=false;int kernelHoldRemaining=0;
    int mode=0;std::array<float,3> curves{},currentCurves{};float curveSlew=1;
    void setTempo(float){}
    static double tau(float raw,int algorithm){return 0.005+norm(raw)*(algorithm==0?1.5:(0.2489085587-0.005)/(64.0/127.0));}
    void configure(int algorithm,const std::array<float,3>& shape){mode=std::clamp(algorithm,0,3); // old|mnm|vital
        for(size_t i=0;i<3;++i)curves[i]=std::clamp(shape[i],i==1?-150.0f:-100.0f,i==1?150.0f:100.0f);}
    void reset(double rate){sr=rate;stage=0;level=0;gate=false;kernelEnv.reset();kernelHold=false;kernelHoldRemaining=0;currentCurves=curves;curveSlew=static_cast<float>(1-std::exp(-1/(sr*0.012)));}
    void on(){gate=true;stage=1;if(mode==kKernelAlgorithm){kernelEnv.trigger();kernelHold=p[1]>0.0f;kernelHoldRemaining=static_cast<int>(sr*norm(p[1])*2);}}
    void off(){gate=false;kernelHold=false;if(mode==kKernelAlgorithm)kernelEnv.release();if(stage)stage=4;}
    float tick() {
        if(mode==kKernelAlgorithm){
            // 1.6.6: sustain floor must stay 0 (firmware AHDSR has no sustain); the old
            // norm(p[2]) made a "sustain bed" follow the DEC knob. Hold now lets the attack
            // run and freezes at peak instead of returning level 0 (which killed the note).
            kernelEnv.setParameters(p[0],p[2],p[3],0.0f);
            if(kernelHold){
                if(kernelHoldRemaining>0){
                    --kernelHoldRemaining;
                    if(kernelEnv.currentState()==monomachine::mnm::AmpEnvelope::State::Attack)kernelEnv.tick();
                    else kernelEnv.holdPeak();
                    return kernelEnv.value();
                }
                kernelHold=false;kernelEnv.resumeAfterHold();
            }
            kernelEnv.tick();
            stage=(kernelEnv.currentState()==monomachine::mnm::AmpEnvelope::State::Other)?0:1;
            return kernelEnv.value();
        }
        for(size_t i=0;i<3;++i)currentCurves[i]+=curveSlew*(curves[i]-currentCurves[i]);
        if(mode==1||mode==3){ // 1.7.9: 3 = VITAL -- витальные изгибы там, где кривая юзера не задана
            const float ca=currentCurves[0]!=0.0f?currentCurves[0]:(mode==3?70.0f:0.0f), cd=(currentCurves[1]!=0.0f?currentCurves[1]:(mode==3?55.0f:0.0f)), cr=(currentCurves[2]!=0.0f?currentCurves[2]:(mode==3?55.0f:0.0f)); // 1.7.9
            if(stage==1){const float bend=std::exp2(ca*0.04f*(level-0.5f));level+=bend*static_cast<float>(1/(sr*(0.001+norm(p[0])*0.5)));if(level>=1){level=1;stage=2;holdRemaining=static_cast<int>(sr*norm(p[1])*2);}}
            else if(stage==2){if(p[1]>=127&&gate){}else if(--holdRemaining<=0)stage=3;}
            else if(stage==3||stage==4){const size_t i=stage==3?1u:2u;const float bend=std::exp2((i==1?cd:cr)*0.04f*(1-level));level*=static_cast<float>(std::exp(-bend/(sr*tau(stage==3?p[2]:p[3],1))));if(level<0.0001f){level=0;stage=0;}} // 1.7.9: vital-дефолты кривых
            return level;
        }
        if(stage==1){level+=static_cast<float>(1/(sr*(0.001+norm(p[0])*0.5)));if(level>=1){level=1;stage=2;holdRemaining=static_cast<int>(sr*norm(p[1])*2);}}
        else if(stage==2){if(p[1]>=127 && gate){}else if(--holdRemaining<=0)stage=3;}
        else if(stage==3 || stage==4){const float t=stage==3?p[2]:p[3];level*=static_cast<float>(std::exp(-1/(sr*(0.005+norm(t)*1.5))));if(level<0.0001f){level=0;stage=0;}}
        return level;
    }
};

struct LFO {
    // Elektron trigger modes. HALF deliberately remains the final raw choice
    // so all existing FREE/TRIG/HOLD/ONE state values retain their meanings.
    static constexpr int kFree=0,kTrig=1,kHold=2,kOne=3,kHalf=4;
    static constexpr double kFullCycle=1.0,kHalfCycle=0.5;
    // Monomachine's tempo-relative timing uses 2048 sixteenth-note steps for
    // SPD=1/MULT=1. Four 1/16 notes form a quarter note, hence:
    // cycles/second = BPM * SPD * MULT / (2048 * 15) = /30720.
    static constexpr double kElektronLfoBaseSteps=2048.0;
    static constexpr double kElektronLfoRateDenominator=kElektronLfoBaseSteps*15.0;

    double phase=0;float held=0,halfHeld=0,random=0;bool oneActive=false,halfActive=false;uint32_t rng=0xabcde;
    int triggerMode(const std::array<float,8>& p) const noexcept {
        return std::clamp(static_cast<int>(p[2]),kFree,kHalf);
    }
    float shape(int kind,float offset) const {
        if(kind==10)return random;
        return nova::lfoShape(kind,phase+static_cast<double>(offset));
    }
    void reset(){phase=0;held=0;halfHeld=0;oneActive=false;halfActive=false;random=noise(rng);}
    void trigger(const std::array<float,8>& p){
        const int mode=triggerMode(p),kind=std::clamp(static_cast<int>(p[3]),0,10);
        oneActive=false;halfActive=false;
        if(mode==kTrig||mode==kOne||mode==kHalf){
            phase=0;random=noise(rng);oneActive=mode==kOne;halfActive=mode==kHalf;
            if(mode==kHalf)halfHeld=interlace(shape(kind,0.0f),norm(p[6]));
        }
        if(mode==kHold)held=shape(kind,0.0f);
    }
    // 1.6.12: INTL = INTERLACE, по мануалу Monomachine ("a function taken straight
    // from the Sidstation"): волна LFO чередуется с нулём, INTL задаёт скорость
    // этих нулевых циклов, 0 = interlace выключен. Раньше ручку ошибочно
    // трактовали как фазовый сдвиг -- поэтому она "не работала".
    float interlace(float value,float intl) const {
        if(intl<=0.0f)return value;
        const double divisions=1.0+std::floor(static_cast<double>(intl)*15.999); // 1..16 пар волна/ноль за цикл
        const double t=std::fmod(phase*divisions,1.0);
        return t<0.5?value:0.0f;
    }
    double rateCyclesPerSecond(const std::array<float,8>& p,double bpm) const noexcept {
        const double speed=std::clamp(static_cast<double>(p[5]),0.0,127.0);
        const int multIndex=std::clamp(static_cast<int>(std::lround(p[4])),0,6);
        const double multiplier=static_cast<double>(1<<multIndex); // 1X, 2X ... 64X
        return std::max(0.0,bpm)*speed*multiplier/kElektronLfoRateDenominator;
    }
    float process(const std::array<float,8>& p,double sr,double bpm,int n){
        const int mode=triggerMode(p),kind=std::clamp(static_cast<int>(p[3]),0,10);
        const float intl=norm(p[6]);
        float result=mode==kHold?held:(mode==kOne&&!oneActive?0.0f:(mode==kHalf&&!halfActive?halfHeld:shape(kind,0.0f)));
        const bool running=(mode!=kOne&&mode!=kHalf)||(mode==kOne&&oneActive)||(mode==kHalf&&halfActive);
        if(running&&sr>0.0&&n>0){
            const double step=rateCyclesPerSecond(p,bpm)*static_cast<double>(n)/sr;
            if(mode==kHalf){
                if(phase+step>=kHalfCycle){
                    phase=kHalfCycle;
                    // Capture the post-INTL output at the exact half-cycle
                    // endpoint. Subsequent calls must retain this final level.
                    halfHeld=interlace(shape(kind,0.0f),intl);halfActive=false;
                }else phase+=step;
            }else{
                phase+=step;
                if(phase>=kFullCycle){
                    phase=wrap(phase);random=noise(rng);if(mode==kOne)oneActive=false;
                }
            }
        }
        return mode==kHalf&&!halfActive?halfHeld:interlace(result,intl);
    }
};

class MachineEngine {
public:
    monomachine::MonomachineBBox bbox;
    NoiseContour noiseContour;double noiseDecaySeconds=0.2489085587;
    std::unique_ptr<AudioAdapter> chorus=std::make_unique<AudioAdapter>();std::array<int,8> chorusParams{64,64,64,127,0,0,127,64};int chorusLatency=0;int chorusCoreMode=0;
    int fxLatency() const noexcept {return id==15?chorusLatency:0;}
    // The adapter latency belongs to the CHORUS machine type, not to whichever
    // machine happened to be rendered most recently.  P2 must be able to
    // report it before its first audio block is processed.
    int latencyForMachine(int machineId) const noexcept {return machineId==15?chorusLatency:0;}
    int idOf() const noexcept {return id;}
    void prepare(double rate) {
        sr=rate;noiseContour.prepare(sr);dyn.reset(sr);par.reset(sr);phaser.reset(sr);flanger.reset(sr);ring.reset(sr);bbox.reset(sr);
        chorusLatency=chorus->prepare(rate);chorus->setCoreMode(chorusCoreMode);chorus->reset(chorusParams);
        mnmFm.reset(sr);mnmFmFix.reset(sr);oldFixStat.reset(sr);oldFixPar.reset(sr);oldFixDyn.reset(sr);fmNew.reset(sr);fmNewFix.reset(sr);
        reverb.setSampleRate(sr);clear();
    }
    void setModes(int syntModeIn,int distModeIn){
        // MODE SYNT's public registry ends at OLD BPM=5. This second boundary
        // is deliberate: even a non-APVTS caller cannot revive retired raw
        // IDs 6..8; they normalize to the retained MNM FRQ renderer.
        const int nextSynt=(syntModeIn>=monomachine::dspModeMnm&&syntModeIn<monomachine::dspModeCount)
            ?syntModeIn:monomachine::dspModeMnm;
        if(syntMode!=nextSynt){
            syntMode=nextSynt;mnmFm.reset(sr);mnmFmFix.reset(sr);oldFixStat.reset(sr);oldFixPar.reset(sr);oldFixDyn.reset(sr);oldFixToneState=0;fmNew.reset(sr);fmNewFix.reset(sr);
        }
        distMode=distModeIn;
    }
    // DSP CHOR: 0 NativeChorusCore (default), 1 ChorusCore reference. The
    // adapter owns the clean reset boundary so no incompatible core state is copied.
    void setChorusCoreMode(int mode) noexcept {
        const int next=mode==monomachine::dspModeOld?monomachine::dspModeOld:monomachine::dspModeMnm;
        if(next==chorusCoreMode)return;
        chorusCoreMode=next;chorus->setCoreMode(chorusCoreMode);
    }
    void clear() {
        phases.fill(0);age=0;noiseContour.clear();toneState=0;colourL=colourR=0;compressorEnv=compressorPower=0;gateCounter=0;noiseCounter=0;
        dyn.reset(sr);par.reset(sr);bbox.reset(sr);chorus->reset(chorusParams);phaser.reset(sr);flanger.clearBuffers();ring.reset(sr);reverb.reset();
        mnmFm.reset(sr);mnmFmFix.reset(sr);oldFixStat.reset(sr);oldFixPar.reset(sr);oldFixDyn.reset(sr);fmNew.reset(sr);fmNewFix.reset(sr);
        rvHpInL=rvHpOutL=rvHpInR=rvHpOutR=rvLpL=rvLpR=0;oldFixToneState=0;
        for(auto& f:formant)f.clear();
    }
    void set(int machine,const std::array<float,8>& values) {
        id=machine;p=values;
        auto b=[&](int i){return static_cast<uint8_t>(std::clamp(juce::roundToInt(p[static_cast<size_t>(i)]),0,127));};
        if(id==10) dyn.setParameters(b(0),b(1),b(2),b(3),b(4),b(5),b(6),b(7));
        // 1.6.5: old-движок FM+STAT удалён -- машина 8 всегда идёт через mnm-ядро.
        if(id==9) par.setParameters(b(0),b(1),b(2),b(3),b(4),b(5),b(6),b(7));
        if(id==7){
            bbox.setFirmwareStart(false);bbox.setFirmwarePitch(false);
            bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false);bbox.setChromatic(b(6)>0);
        }
        if(id==15) {for(int i=0;i<8;++i)chorusParams[static_cast<size_t>(i)]=b(i);auto coreParams=chorusParams;coreParams[3]=127;coreParams[7]=64;chorus->setParameters(coreParams);}
        if(id==18) phaser.setParameters(p[0],p[1],p[2],p[3],p[4],p[5]);
        if(id==19) flanger.setParameters(p[0],p[1],p[2],p[3],p[4],p[5]);
        if(id==17) ring.setParameters(p[0],p[1],p[3],p[2]+pitchMod+(pitch-60));
        if(id==13) {
            juce::Reverb::Parameters r;r.roomSize=norm(p[0])*0.95f;r.damping=norm(p[1]);r.wetLevel=1.0f/3.0f;r.dryLevel=0;r.width=1;r.freezeMode=0;
            reverb.setParameters(r);
        }
        if(id==11){float sweep=norm(p[2])*300*std::sin(static_cast<float>(age/sr)*5);formant[0].band(sr,200+norm(p[0])*1200+sweep,4);formant[1].band(sr,700+norm(p[1])*2700,6);formant[2].band(sr,2400+norm(p[3])*1800,8);}
    }
    void on(int midi) {
        baseNote=midi;age=0;
        // Only retained public FM renderers are primed here. Retired imports
        // are archived outside shipping Source and cannot allocate or receive notes.
        const bool isFm=id==8||id==9||id==10;
        if(isFm){
            const float note=static_cast<float>(midi);
            const auto primeMnm=[&]{mnmFm.setPitchMod(0.0f);mnmFm.noteOn(note,1.0f);};
            const auto primeMnmFix=[&]{mnmFmFix.setPitchMod(0.0f);mnmFmFix.noteOn(note,1.0f);};
            const auto primeNew=[&]{fmNew.setPitchMod(0.0f);fmNew.noteOn(note);};
            const auto primeNewFix=[&]{fmNewFix.setPitchMod(0.0f);fmNewFix.noteOn(note);};
            switch(syntMode){
                case monomachine::dspModeMnmFix:       primeMnmFix(); break;
                case monomachine::dspModeNew:          primeNew(); break;
                case monomachine::dspModeNewFix:       primeNewFix(); break;
                case monomachine::dspModeOldFix:
                    if(id==8)oldFixStat.noteOn(static_cast<uint8_t>(midi));
                    else if(id==9)oldFixPar.noteOn(static_cast<uint8_t>(midi));
                    else oldFixDyn.noteOn(static_cast<uint8_t>(midi));
                    break;
                case monomachine::dspModeMnm:
                    primeMnm();
                    break;
                case monomachine::dspModeOld:
                default:
                    // m8's historical OLD choice remains its retained MNM
                    // renderer; m9/m10 retain their OLD engines unchanged.
                    if(id==8)primeMnm();
                    else if(id==9)par.noteOn(static_cast<uint8_t>(midi));
                    else dyn.noteOn(static_cast<uint8_t>(midi));
                    break;
            }
        }else if(id==7){
            bbox.noteOn(static_cast<uint8_t>(midi));
        }
        if((id==3 && p[2]>63)||(id==5 && p[6]>63)||(id==6 && p[3]>63))phases.fill(0);
    }
    void setPitch(float absoluteNote,float modulation) {
        pitch=absoluteNote;pitchMod=modulation;
        float bend=absoluteNote-baseNote+modulation;dyn.setPitchBend(bend);par.setPitchBend(bend);bbox.setPitchBend(bend);
        mnmFm.setPitchMod(bend);mnmFmFix.setPitchMod(bend);oldFixStat.setPitchBend(bend);oldFixPar.setPitchBend(bend);oldFixDyn.setPitchBend(bend);fmNew.setPitchMod(bend);fmNewFix.setPitchMod(bend);
    }
    void render(float* l,float* r,int n,bool synth) {
        if(!synth){renderFX(l,r,n);return;}
        // Each retained FM variant owns separate state. In particular, NEW and
        // MNM never receive a FIX table pointer/flag, even transiently.
        const bool isFm=id==8||id==9||id==10;
        const bool newExact=syntMode==monomachine::dspModeNew&&isFm;
        const bool newFix=syntMode==monomachine::dspModeNewFix&&isFm;
        if(newExact){
            const auto kind=id==8?monomachine::fm_new::FmExactKind::Stat:(id==9?monomachine::fm_new::FmExactKind::Par:monomachine::fm_new::FmExactKind::Dyn);
            fmNew.setParameters(kind,p);
            // Retained NEW's historical bridge is deliberately untouched.
            const float fineTune=(std::clamp(p[7],0.0f,127.0f)-64.0f)/64.0f;
            fmNew.setPitchMod(pitch-baseNote+pitchMod+fineTune);
            constexpr int kExactHostChunk=16;
            int done=0;while(done<n){const int block=std::min(kExactHostChunk,n-done);float blockBuf[kExactHostChunk]{};
                fmNew.processBlock(blockBuf,block);
                // User-measured +10 dB bridge only; the byte-stable raw exact
                // core remains unmodified and retained MNM/OLD are untouched.
                for(int i=0;i<block;++i){const float sample=blockBuf[i]*monomachine::fm_new::kOutputBridgeGain;l[static_cast<size_t>(done+i)]+=sample;r[static_cast<size_t>(done+i)]+=sample;}
                done+=block;}
            return;
        }
        if(newFix){
            const auto kind=id==8?monomachine::fm_new::FmExactKind::Stat:(id==9?monomachine::fm_new::FmExactKind::Par:monomachine::fm_new::FmExactKind::Dyn);
            fmNewFix.setParameters(kind,p);
            const float fineTune=monomachine::fm_fix::tuneSemitonesForRaw(juce::roundToInt(p[7]));
            fmNewFix.setPitchMod(pitch-baseNote+pitchMod+fineTune);
            constexpr int kExactHostChunk=16;
            int done=0;while(done<n){const int block=std::min(kExactHostChunk,n-done);float blockBuf[kExactHostChunk]{};
                fmNewFix.processBlock(blockBuf,block);
                // NEW FIX uses the identical host bridge so switching NEW/FIX
                // cannot create a 10 dB gain jump.
                for(int i=0;i<block;++i){const float sample=blockBuf[i]*monomachine::fm_new::kOutputBridgeGain;l[static_cast<size_t>(done+i)]+=sample;r[static_cast<size_t>(done+i)]+=sample;}
                done+=block;}
            return;
        }
        const bool mnmFix=syntMode==monomachine::dspModeMnmFix&&isFm;
        const bool oldFix=syntMode==monomachine::dspModeOldFix&&isFm;
        if(mnmFix){
            const auto kind=id==8?monomachine::fm_fix::MnmFixKind::Stat:(id==9?monomachine::fm_fix::MnmFixKind::Par:monomachine::fm_fix::MnmFixKind::Dyn);
            std::array<float,8> values{};for(int i=0;i<8;++i)values[static_cast<size_t>(i)]=p[static_cast<size_t>(i)];
            mnmFmFix.setParameters(kind,values);
            int done=0;while(done<n){const int block=std::min(monomachine::mnm::kBlock,n-done);float blockBuf[monomachine::mnm::kBlock]{};
                mnmFmFix.processBlock(blockBuf,block);
                for(int i=0;i<block;++i){l[static_cast<size_t>(done+i)]+=blockBuf[i];r[static_cast<size_t>(done+i)]+=blockBuf[i];}
                done+=block;}
            return;
        }
        if(oldFix){
            auto raw=[&](int i){return static_cast<uint8_t>(std::clamp(juce::roundToInt(p[static_cast<size_t>(i)]),0,127));};
            // OLD FIX m8 is a dedicated archived OLD STATIC renderer. It never
            // delegates to MNM; only its measured FREQ/TUNE control laws differ.
            if(id==8){
                oldFixStat.setParameters(raw(0),raw(1),raw(2),raw(3),raw(4),raw(5),raw(6),raw(7));
                oldFixStat.processStereo(l,r,static_cast<size_t>(n));
                return;
            }
            if(id==9){
                oldFixPar.setParameters(raw(0),raw(1),raw(2),raw(3),raw(4),raw(5),raw(6),raw(7));
                oldFixPar.processStereo(l,r,static_cast<size_t>(n));
                const float a=1-std::exp(-2*pi*(100+19000*norm(p[6])*norm(p[6]))/static_cast<float>(sr));
                for(int i=0;i<n;++i){oldFixToneState+=a*(l[i]-oldFixToneState);l[i]=r[i]=oldFixToneState;}
                return;
            }
            oldFixDyn.setParameters(raw(0),raw(1),raw(2),raw(3),raw(4),raw(5),raw(6),raw(7));
            oldFixDyn.processStereo(l,r,static_cast<size_t>(n));
            return;
        }
        // Baseline MNM and m8 OLD alias: this remains the original core source
        // and original state object, with no FIX branch or FIX table reference.
        if((syntMode==monomachine::dspModeMnm||id==8)&&isFm){
            const auto kind=id==8?monomachine::mnm::FmKind::Stat:(id==9?monomachine::mnm::FmKind::Par:monomachine::mnm::FmKind::Dyn);
            std::array<float,8> values{};for(int i=0;i<8;++i)values[static_cast<size_t>(i)]=p[static_cast<size_t>(i)];
            mnmFm.setParameters(kind,values);
            int done=0;while(done<n){const int block=std::min(monomachine::mnm::kBlock,n-done);float blockBuf[monomachine::mnm::kBlock]{};
                mnmFm.processBlock(blockBuf,block);
                for(int i=0;i<block;++i){l[static_cast<size_t>(done+i)]+=blockBuf[i];r[static_cast<size_t>(done+i)]+=blockBuf[i];}
                done+=block;}
            return;
        }
        if(id==10){dyn.processStereo(l,r,static_cast<size_t>(n));return;}
        if(id==9){par.processStereo(l,r,static_cast<size_t>(n));
            float a=1-std::exp(-2*pi*(100+19000*norm(p[6])*norm(p[6]))/static_cast<float>(sr));for(int i=0;i<n;++i){toneState+=a*(l[i]-toneState);l[i]=r[i]=toneState;}return;}
        if(id==7){bbox.processStereo(l,r,static_cast<size_t>(n));for(int i=0;i<n;++i)l[i]=r[i]=l[i]*norm(p[7]);return;}
        const float frequency=std::min(hz(pitch+pitchMod+(p[7]-64)/64*12),static_cast<float>(sr*0.45));
        const float dt=frequency/static_cast<float>(sr);
        for(int i=0;i<n;++i){
            float a=0,b=0;double t=phases[0];float env=std::exp(-static_cast<float>(age/sr)*4);
            if(id==0){a=b=0;}
            else if(id==1){a=b=wave(0,t,dt);}
            else if(id==2){
                int interval=1+static_cast<int>(norm(p[4])*63*sr/44100);
                if(noiseCounter==0){heldNoiseL=noise(rng);heldNoiseR=noise(rng);}noiseCounter=(noiseCounter+1)%interval;
                float cutoff=8000+norm(p[7])*8000,sm=1-std::exp(-2*pi*cutoff/static_cast<float>(sr));
                colourL+=sm*(heldNoiseL-colourL);colourR+=sm*(heldNoiseR-colourR);
                float left=noiseContour.process(heldNoiseL*0.9f+colourL*0.1f,0,pitch+pitchMod,p[1]);
                float right=noiseContour.process(heldNoiseR*0.9f+colourR*0.1f,1,pitch+pitchMod,p[1]);
                float duration=static_cast<float>(noiseDecaySeconds)*std::exp2((p[3]-64)/16);
                const float envelopeGain=std::exp(-static_cast<float>(age/sr)/std::max(0.001f,duration));
                a=left*envelopeGain;b=(left+(right-left)*norm(p[0])*std::clamp(p[2],0.0f,1.0f))*envelopeGain;
            } else if(id==3){
                float pw=std::clamp(norm(p[0])+norm(p[1])*env*0.4f,0.05f,0.95f);int w=static_cast<int>(p[3])/32;
                float ratio=std::pow(2.0f,(p[6]-64)/32);double old=phases[1];phases[1]=wrap(phases[1]+dt*ratio);
                if(p[4]>42&&p[4]<85&&phases[1]<old)phases[0]=t=0;
                a=w==3?noise(rng):wave(w==0?3:w==1?1:2,t,dt,pw);
                if(p[4]>=85)a*=wave(static_cast<int>(p[5])/32,phases[1],dt*ratio);
                b=a;
            } else if(id==4){
                if(p[3]<0.5f){
                const float detune=norm(p[1])*0.48f;
                float sum=wave(1,t,dt);const float nearGain=norm(p[0])*0.5f,farGain=norm(p[2])*0.5f;
                for(int v=0;v<4;++v){const float sign=(v%2)?1.0f:-1.0f;
                    const float inc=dt*std::pow(2.0f,sign*detune*(v<2?1.0f:2.0f)/12.0f);
                    const auto ix=static_cast<size_t>(v+1);phases[ix]=wrap(phases[ix]+inc);
                    sum+=wave(1,phases[ix],inc)*(v<2?nearGain:farGain);}
                phases[5]=wrap(phases[5]+dt*0.5);phases[6]=wrap(phases[6]+dt*0.25);
                const float sub=(wave(static_cast<int>(p[5])/32,phases[5],dt*0.5f)+wave(static_cast<int>(p[6])/32,phases[6],dt*0.25f))*norm(p[4])*0.5f;
                a=b=(sum+sub)/(1+2*nearGain+2*farGain+norm(p[4]));
                }else{
                    const float width=norm(p[1])*0.48f,enable=norm(p[0])*std::min(1.0f,p[1]/4.0f);
                    auto retro=[](double phase,float step){return 0.78f*wave(1,phase,step)+(step*2<0.45f?0.14f*wave(1,wrap(phase*2),step*2):0)+(step*3<0.45f?0.08f*wave(1,wrap(phase*3),step*3):0);};
                    float sum=retro(t,dt);
                    for(int v=0;v<4;++v){float inc=dt*std::pow(2.0f,((v%2)?1.0f:-1.0f)*width*(v<2?1.0f:2.0f)/12);size_t ix=static_cast<size_t>(v+1);phases[ix]=wrap(phases[ix]+inc);sum+=retro(phases[ix],inc)*0.8f*enable*(v<2?1.0f:norm(p[2]));}
                    phases[5]=wrap(phases[5]+dt*0.5);phases[6]=wrap(phases[6]+dt*0.25);
                    const float dirty=0.65f*wave(2,phases[5],dt*0.5f)+0.35f*wave(1,phases[5],dt*0.5f);
                    sum+=norm(p[4])*dirty+norm(p[5])*wave(0,phases[5],dt*0.5f)+norm(p[6])*wave(0,phases[6],dt*0.25f);
                    a=b=sum*0.1f;
                }
            } else if(id==5){
                float detune=std::pow(2.0f,norm(p[1])*0.04f);phases[1]=wrap(phases[1]+dt/detune);phases[2]=wrap(phases[2]+dt*detune);
                float pw=id==5?std::clamp(norm(p[4])+norm(p[5])*env*0.4f,0.05f,0.95f):0.5f;
                int kind=id==4?1:2;float main=wave(kind,t,dt,pw),u1=wave(kind,phases[1],dt/detune,pw),u2=wave(kind,phases[2],dt*detune,pw);
                float unison=norm(p[0]);float mix=id==4?norm(p[2]):0;
                a=((1-mix)*main+mix*(u1+u2)*0.5f+unison*u1)/(1+unison);
                b=((1-mix)*main+mix*(u1+u2)*0.5f+unison*u2)/(1+unison);
                phases[3]=wrap(phases[3]+dt*0.5);phases[4]=wrap(phases[4]+dt*0.25);
                float sub1=wave(static_cast<int>(id==4?p[5]:p[2])/32,phases[3],dt*0.5f);
                float sub2=wave(static_cast<int>(id==4?p[6]:p[3])/32,phases[4],dt*0.25f);
                float subMix=id==4?norm(p[4]):0.25f;a=(a+(sub1+sub2)*subMix*0.5f)/(1+subMix);b=(b+(sub1+sub2)*subMix*0.5f)/(1+subMix);
            } else if(id==14||id==33){
                a=b=0;for(int v=0;v<4;++v){float offset=v==0?0:(p[static_cast<size_t>(v-1)]-64)/64*12;
                    float wobble=norm(p[5])*0.12f*std::sin(static_cast<float>(age/sr)*2*pi*0.7f+v);
                    float inc=dt*std::pow(2.0f,(offset+wobble)/12);phases[static_cast<size_t>(v+1)]=wrap(phases[static_cast<size_t>(v+1)]+inc);
                    float s=id==14?wave(static_cast<int>(p[3])/32,phases[static_cast<size_t>(v+1)],inc,std::clamp(norm(p[4]),0.05f,0.95f)):digitalWave(p[3],phases[static_cast<size_t>(v+1)],0.9f);
                    float side=(v%2==0?-1.0f:1.0f)*norm(p[6]);a+=s*(1-side)*0.25f;b+=s*(1+side)*0.25f;}
            } else if(id==6){
                float ratio=std::pow(2.0f,(p[5]-64)/32);double old=phases[1];phases[1]=wrap(phases[1]+dt*ratio);
                if(p[4]>63&&phases[1]<old)phases[0]=t=0;
                float position=std::clamp(norm(p[1])+norm(p[2])*std::sin(static_cast<float>(age/sr)*4)*0.5f,0.0f,1.0f);
                a=b=(1-position)*digitalWave(p[0],t,0.85f)+position*digitalWave(std::fmod(p[0]+17,128.0f),t,1.0f);
            } else if(id==32){
                float coefficient=1-std::exp(-1/(static_cast<float>(sr)*(0.001f+norm(p[3])*2)));
                waveMix+=coefficient*(norm(p[1])-waveMix);
                float x=digitalWave(p[0],t,norm(p[4])),y=digitalWave(p[2],t+0.02*norm(p[5]),norm(p[6]));
                float mid=x*(1-waveMix)+y*waveMix,side=(x-y)*norm(p[5])*0.25f;a=mid+side;b=mid-side;
            } else if(id==11){
                float source=wave(1,t,dt),vowel=0;for(auto& f:formant)vowel+=f.tick(source);
                float decay=std::exp(-static_cast<float>(age/sr)/(0.01f+norm(p[5])*0.4f));
                float consonant=noise(rng);if(p[4]>42)consonant*=wave(p[4]>84?2:0,phases[1],dt*3);phases[1]=wrap(phases[1]+dt*3);
                a=b=vowel*2+consonant*decay*norm(p[6])*0.5f;
            }
            l[i]=a;r[i]=b;phases[0]=wrap(phases[0]+dt);++age;
        }
    }
private:
    void renderFX(float* l,float* r,int n) {
        if(id!=15){const float g=p[7]/64.0f;for(int i=0;i<n;++i){l[i]*=g;r[i]*=g;}}
        if(id==15){ // 1.8.3c: блоковый direct-путь хоруса -- бит-в-бит тот же обмен, кольца/норм/деление вынесены из цикла
            // INP lives in the host wrapper: the native 24-bit core receives
            // unity input while AudioAdapter aligns its wet and dry frames.
            // Preserve raw 0 (-64), raw 4 (-60), and every higher value exactly.
            // Only the user-confirmed bottom four-step fade uses a quadratic
            // ease-in so -64..-60 rises more gently in both host paths.
            const float inputRaw=std::clamp(p[7],0.0f,127.0f);
            constexpr float kChorusInpKneeRaw=4.0f;
            const float kneeT=inputRaw/kChorusInpKneeRaw;
            const float lowKneeGain=kChorusInpKneeRaw*0.015625f
                *(kneeT*kneeT);
            const float inGain=inputRaw>=kChorusInpKneeRaw
                ?inputRaw*0.015625f:lowKneeGain;
            const float wet=norm(p[3]),dryw=1.0f-wet;
            if(sInL.size()<static_cast<size_t>(n)){sInL.resize(static_cast<size_t>(n));sInR.resize(static_cast<size_t>(n));sWL.resize(static_cast<size_t>(n));sWR.resize(static_cast<size_t>(n));sDL.resize(static_cast<size_t>(n));sDR.resize(static_cast<size_t>(n));}
            for(int i=0;i<n;++i){sInL[static_cast<size_t>(i)]=l[i]*inGain;sInR[static_cast<size_t>(i)]=r[i]*inGain;}
            chorus->processBlockWetDry(sInL.data(),sInR.data(),sWL.data(),sWR.data(),sDL.data(),sDR.data(),n);
            for(int i=0;i<n;++i){l[i]=sDL[static_cast<size_t>(i)]*dryw+sWL[static_cast<size_t>(i)]*wet;r[i]=sDR[static_cast<size_t>(i)]*dryw+sWR[static_cast<size_t>(i)]*wet;}}
        else if(id==18)phaser.processStereo(l,r,l,r,static_cast<size_t>(n));
        else if(id==19)flanger.processStereo(l,r,l,r,static_cast<size_t>(n));
        else if(id==17){ring.setParameters(p[0],p[1],p[3],p[2]+pitchMod+(pitch-60));ring.processStereo(l,r,l,r,static_cast<size_t>(n));}
        else if(id==13){
            // v6: буферы сухого сигнала под РАЗМЕР БЛОКА хоста (было std::array<float,32>;
            // при блоке больше 32 сэмплов это переполняло стек и роняло плагин на ревере).
            if(rvDryL.size()<static_cast<size_t>(n)){rvDryL.resize(static_cast<size_t>(n));rvDryR.resize(static_cast<size_t>(n));}
            std::copy_n(l,n,rvDryL.data());std::copy_n(r,n,rvDryR.data());
            const float hpF=30.0f+norm(p[4])*norm(p[4])*970.0f;
            const float hpPole=std::exp(-2*pi*hpF/static_cast<float>(sr));
            const float lpF=std::min(400.0f*std::pow(50.0f,norm(p[5])),static_cast<float>(sr*0.45));
            const float lpAmount=1.0f-std::exp(-2*pi*lpF/static_cast<float>(sr));
            for(int i=0;i<n;++i){
                const float x=l[i],y=r[i];
                const float hl=hpPole*(rvHpOutL+x-rvHpInL),hr=hpPole*(rvHpOutR+y-rvHpInR);
                rvHpInL=x;rvHpInR=y;rvHpOutL=hl;rvHpOutR=hr;
                if(p[4]>0){l[i]=hl;r[i]=hr;}
            }
            reverb.processStereo(l,r,n);
            const float wet=norm(p[3]);
            for(int i=0;i<n;++i){const auto k=static_cast<size_t>(i);
                rvLpL+=lpAmount*(l[i]-rvLpL);rvLpR+=lpAmount*(r[i]-rvLpR);
                float wl=p[5]>=127?l[i]:rvLpL,wr=p[5]>=127?r[i]:rvLpR;
                if(std::max(std::abs(rvDryL[k]),std::abs(rvDryR[k]))>0.002f)
                    gateCounter=static_cast<int>(sr*(0.02+norm(p[2])*2));
                else if(gateCounter>0)--gateCounter;
                if(p[2]>0&&gateCounter==0){wl=0;wr=0;}
                l[i]=rvDryL[k]*(1-wet)+wl*wet;r[i]=rvDryR[k]*(1-wet)+wr*wet;
            }
        } else if(id==16){
            float atk=std::exp(-1/(static_cast<float>(sr)*(0.0001f+norm(p[0])*0.1f))),rel=std::exp(-1/(static_cast<float>(sr)*(0.005f+norm(p[1])*1.5f)));
            float threshold=-60+norm(p[2])*60,ratio=1+norm(p[4])*19,makeup=std::pow(10.0f,((p[5]-64)/64*18)/20),mix=norm(p[3]);
            for(int i=0;i<n;++i){float peak=std::max(std::abs(l[i]),std::abs(r[i]));compressorPower+=0.002f*(peak*peak-compressorPower);
                float detector=(1-norm(p[6]))*peak+norm(p[6])*std::sqrt(compressorPower);
                float coeff=detector>compressorEnv?atk:rel;compressorEnv=detector+coeff*(compressorEnv-detector);
                float over=std::max(0.0f,20*std::log10(std::max(compressorEnv,1.0e-8f))-threshold);
                float gain=(1-mix)+mix*makeup*std::pow(10.0f,-over*(1-1/ratio)/20);l[i]*=gain;r[i]*=gain;}
        }
    }
    int id=4,baseNote=60;double sr=44100,age=0;float pitch=60,pitchMod=0,toneState=0,waveMix=0.5f;
    std::array<float,8> p{};std::array<double,8> phases{};std::array<Biquad,3> formant{};
    uint32_t rng=0x12345678;int noiseCounter=0,gateCounter=0;float heldNoiseL=0,heldNoiseR=0,colourL=0,colourR=0,compressorEnv=0,compressorPower=0;
    monomachine::MonomachineFmDynamic dyn;monomachine::MonomachineFmParallel par; // 1.6.5: MonomachineFmStatic удалён
    monomachine::MonomachinePhaser phaser;monomachine::MonomachineFlanger flanger;monomachine::MonomachineRingMod ring;
    int syntMode=monomachine::dspModeMnm,distMode=monomachine::dspModeMnm;
    monomachine::mnm::FmCore mnmFm; // exact retained pre-FIX MNM / m8 OLD alias path
    monomachine::fm_fix::MnmFixCore mnmFmFix; // separate opt-in MNM FIX state/core
    // OLD FIX owns isolated OLD STATIC/PAR/DYN renderer topologies. Only their
    // requested FREQ/TUNE control laws are measured FIX variants; neither OLD
    // FIX state nor audio dispatch delegates to an MNM renderer.
    monomachine::fm_fix::OldFixStaticCore oldFixStat;
    monomachine::fm_fix::OldFixParallelCore oldFixPar;
    monomachine::fm_fix::OldFixDynamicCore oldFixDyn;
    float oldFixToneState=0;
    monomachine::fm_new::FmExactCore fmNew; // byte-stable retained NEW state/core
    monomachine::fm_new::FmExactFixCore fmNewFix; // separate opt-in NEW FIX state/core
    float rvHpInL=0,rvHpOutL=0,rvHpInR=0,rvHpOutR=0,rvLpL=0,rvLpR=0;
    std::vector<float> rvDryL,rvDryR;
    std::vector<float> sInL,sInR,sWL,sWR,sDL,sDR; // 1.8.3c: скрэтч блокового пути хоруса
    juce::Reverb reverb;
};

class TrackChain {
public:
    // BPM may be as low as 30: DTIM=127 then resolves to exactly four seconds.
    static constexpr float kDelayMaximumSeconds=4.0f;
    float volumeReference=127.0f;
    void prepare(double rate){
        sr=rate;
        distSlew=static_cast<float>(1.0-std::exp(-1.0/(sr*0.003)));
        delayL.assign(static_cast<size_t>(sr*kDelayMaximumSeconds)+8,0);
        delayR=delayL;
        filter.reset(sr);
        delayFilter.reset(sr);
        delayFeedbackDynamics.prepare(sr);
        delayFeedbackDynamics.setLoopGuard(delayFeedbackLoopClip);
        // RAW is not known at prepare time. stageDELAY enables each core's
        // final clip only for the RAW>=64 guard zone.
        mnmDelay.prepare(sr);mnmDelay.setLoopClip(false);
        newDelay.prepare(sr);newDelay.setLoopClip(false);
        mnmFilter.setSampleRate(sr);
        independentPhysicalFilter.setSampleRate(sr);
        hybridSaturation.reset();
        filterSaturation.reset();
        mnmFixNotch.prepare(sr);
        filterModEnv.reset(sr);
        clear();
    }
    void setModes(int filterModeIn,int distModeIn,int delayModeIn,int routingModeIn){
        filterMode=filterModeIn;distMode=distModeIn;delayMode=delayModeIn;
        if(routingMode!=routingModeIn){routingMode=routingModeIn;}
        // Sample-rate setup belongs to prepare().  snapshot()/setModes() runs
        // once per host callback; repeating setup here used to clear imported
        // Korg/Odin/R-classic filter state at every callback.
    }
    // Private hybrid closed-test selectors. MODE L/H are side-specific UI
    // choices translated to FilterCore's canonical algorithm IDs. MODE S is
    // the sole selector for this already-existing, post-filter DIST block.
    void setHybridTestModes(int lowerModeIn,int upperModeIn,int saturationModeIn){
        hybridLowerMode=hybrid_private::modeLChoiceToAlgorithm(lowerModeIn);
        hybridUpperMode=hybrid_private::modeHChoiceToAlgorithm(upperModeIn);
        const int nextSaturation=std::clamp(saturationModeIn,
                                            static_cast<int>(hybrid_private::hybridDistMnm),
                                            static_cast<int>(hybrid_private::hybridDistOldV2));
        if(hybridSaturationMode!=nextSaturation){
            hybridSaturationMode=nextSaturation;
            hybridSaturation.reset();
            mnmFixNotch.reset();
        }
        mnmFilter.setHybridTestModes(hybridLowerMode,hybridUpperMode);
        independentPhysicalFilter.setHybridTestModes(hybridLowerMode,hybridUpperMode);
        if(hybridSaturationMode>=hybrid_private::hybridDistFold
           && hybridSaturationMode<=hybrid_private::hybridDistClamp)
            hybridSaturation.setMode(hybridSaturationMode-hybrid_private::hybridDistFold+1);
    }
    void setHostTempo(float tempo) noexcept {delayTempo=std::clamp(std::isfinite(tempo)?tempo:120.0f,30.0f,300.0f);}
    // BPM=ON uses the DTIM musical law supplied for the manual. BPM=OFF uses
    // the user-visible endpoint as the value at raw DTIM=127.
    void setDelayTiming(bool bpmSync,float maximumSeconds) noexcept {
        delayBpmSync=bpmSync;
        delayMaximumSeconds=std::clamp(std::isfinite(maximumSeconds)?maximumSeconds:0.75f,0.01f,kDelayMaximumSeconds);
    }
    // Extra filter controls live outside the historic 32 page parameters so
    // saved P1/P2 page layout and firmware controls stay intact. All defaults
    // are neutral: VEL/KT=0, SAT=0 and FIL ENV MIX=0.
    void setFilterExtras(float velLower, float velUpper, float keyLower, float keyUpper, float sat,
                         float filEnvAtk, float filEnvHold, float filEnvDec, float filEnvRel,
                         float filEnvMix, float filEnvBase, float filEnvWidth) noexcept {
        filterVelLower = std::clamp(velLower, 0.0f, 127.0f);
        filterVelUpper = std::clamp(velUpper, 0.0f, 127.0f);
        filterKeyLower = std::clamp(keyLower, 0.0f, 127.0f);
        filterKeyUpper = std::clamp(keyUpper, 0.0f, 127.0f);
        filterEnvMix = std::clamp(filEnvMix, 0.0f, 127.0f);
        filterEnvBase = std::clamp(filEnvBase, -64.0f, 63.0f);
        filterEnvWidth = std::clamp(filEnvWidth, -64.0f, 63.0f);
        filterModEnv.p = {std::clamp(filEnvAtk, 0.0f, 127.0f),
                           std::clamp(filEnvHold, 0.0f, 127.0f),
                           std::clamp(filEnvDec, 0.0f, 127.0f),
                           std::clamp(filEnvRel, 0.0f, 127.0f)};
        filterModEnv.configure(AmpEnvelope::kKernelAlgorithm, {0.0f, 0.0f, 0.0f});
        filterSaturation.setAmount(sat);
    }
    // Normal Monomachine key tracking is binary and independent for the two
    // physical edges. It remains separate from the older continuous KT-L/KT-H
    // modulation amounts, which are still available to old saved states.
    void setFilterKeyTracking(bool hpfEnabled,bool lpfEnabled) noexcept {
        filterTrackHpf=hpfEnabled;filterTrackLpf=lpfEnabled;
    }
    // P2 is an insert: an untouched P2 filter page must be an exact THRU.
    // P1 keeps its historic native filter behaviour.  This switch does not
    // replace the filter core; it merely avoids running it while every control
    // that can make it audible is at the documented neutral value.
    void setNeutralFilterThru(bool enabled) noexcept {neutralFilterThru=enabled;neutralFilterThruActive=false;}
    // 1.6.5/1.6.8: настройка «DLY REPITCH»: скорость, с которой длина линии
    // mnm-дилея следует за ручкой DTIM. Непрерывная шкала 0..3:
    //   0 = OFF (мгновенно, прежнее поведение), 1 = FAST (0.006 с),
    //   2 = MED (0.030 с, дефолт как в 1.6.5), 3 = «лента» ~2.5 с (максимум
    //   в разумных 2-3 с). Промежуточные значения интерполируются линейно.
    // smoothRaw (0..127) -- отдельная доводка-антиклик: даже в OFF длина
    // дотягивается не резче, чем за 10 мс * smooth/127. 0 = как было.
    void setRepitch(float raw,float smoothRaw){
        const float v=juce::jlimit(0.0f,3.0f,raw);
        const float anchors[4]={0.0f,0.006f,0.030f,2.5f};
        if(v<=0.0005f)repitchSlew=0.0f;
        else{const int i=static_cast<int>(v);repitchSlew=i>=3?anchors[3]:anchors[i]+(anchors[i+1]-anchors[i])*(v-static_cast<float>(i));}
        declickSlew=juce::jlimit(0.0f,127.0f,smoothRaw)/127.0f*0.010f;
    }
    // Optional feedback-Q settings are deliberately outside the historic
    // eight EFFX parameters.  Q=0 preserves the retained OLD response and
    // leaves MNM/NEW at their exact feedback-filter bypass default.
    void setDelayFeedbackQ(float baseQ,float widthQ) noexcept {
        delayBaseQ=std::clamp(baseQ,0.0f,127.0f);
        delayWidthQ=std::clamp(widthQ,0.0f,127.0f);
    }
    // DYNAMICS=ON keeps RAW=63 at exact unity, then feeds RAW>=64 through
    // BASE CURVE/HOLD and the optional GUARD. DYNAMICS=OFF preserves raw/63
    // for direct legacy A/B. RISE/ZERO TAIL are retained state IDs only.
    void setDelayFeedbackDynamics(bool enabled,float riseSeconds,float zeroTailSeconds,
                                  bool invertPositive,bool invertNegative) noexcept {
        delayFeedbackDynamics.setControls(enabled,riseSeconds,zeroTailSeconds);
        delayFeedbackInvertPositive=invertPositive;delayFeedbackInvertNegative=invertNegative;
    }
    // Schema 44 BASE controls are intentionally independent of the safety
    // follower and may be changed only through the DFB panel/APVTS state.
    void setDelayFeedbackBase(const DelayFeedbackDynamics::Base& base) noexcept {
        delayFeedbackDynamics.setBase(base);
    }
    // Schema 44's visible DFB controls retain the raw-gated 64..127 GUARD
    // window with four explicit anchors, common offset and follower timing.
    // The safety law is a user governor design, not an asserted firmware curve.
    void setDelayFeedbackGuard(const DelayFeedbackDynamics::Guard& guard,
                               const DelayFeedbackDynamics::Governor& governor) noexcept {
        // Snapshot runs each audio block. The controller ignores unchanged
        // values and rebuilds its raw/level LUT once per actual parameter edit.
        delayFeedbackDynamics.setGuardAndGovernor(guard,governor);
    }
    // RETURN COMP is a separate post-return option and does not alter the
    // recurrence. FB CLIP enables the level governor and, only at RAW>=64,
    // every core's final write guard. RAW<=63 remains a true unity/bypass path.
    void setDelayFeedbackSafety(bool returnCompensation,bool loopClip) noexcept {
        delayFeedbackReturnComp=returnCompensation;
        delayFeedbackLoopClip=loopClip;
        delayFeedbackDynamics.setLoopGuard(loopClip);
        // stageDELAY owns the raw-aware value for this block; never leave a
        // previous RAW>=64 clip state active after returning to RAW<=63.
        mnmDelay.setLoopClip(false);
        newDelay.setLoopClip(false);
    }
    float delayFeedbackLoopLevel() const noexcept { return delayFeedbackDynamics.loopLevel(); }
    float delayFeedbackEffective() const noexcept { return delayFeedbackDynamics.effectiveFeedback(); }
    float delayFeedbackRaw() const noexcept { return lastDelayFeedbackRaw; }
    float delayReturnGainForFeedback(float rawFeedback) const noexcept {
        if(!delayFeedbackReturnComp)return 1.0f;
        const float hot=std::clamp((rawFeedback-63.0f)/64.0f,0.0f,1.0f);
        return std::pow(10.0f,-hot); // -20 dB * hot / 20
    }
    void clear(){
        // PANIC / CLEAR TAILS must reset every stateful branch, not only the
        // legacy buffers.  The native filter and native delay own independent
        // resonator/line memory; leaving them alive made a cleared P2 chain
        // able to replay a low, feedback-like residue on the next CHORUS pass.
        filter.reset(sr);
        delayFilter.reset(sr);
        delayFeedbackDynamics.reset();
        mnmFilter.reset();
        independentPhysicalFilter.reset();
        mnmDelay.reset();
        newDelay.reset();
        hybridSaturation.reset();
        filterSaturation.reset();
        mnmFixNotch.reset();
        filterModEnv.reset(sr);
        eqL.clear();eqR.clear();
        std::fill(delayL.begin(),delayL.end(),0.0f);
        std::fill(delayR.begin(),delayR.end(),0.0f);
        write=0;srrCounter=0;heldL=heldR=0;negativeDelayComb=false;time=1;lastDelayFeedbackRaw=0.0f;panSmoothingReady=false;smoothedPan=0.0f;smoothedVolume=1.0f;distSmoothingReady=false;smoothedDist=64.0f;neutralFilterThruActive=false;
    }
    void trigger(int midiNote = 60, float velocity = 1.0f){
        filterMidiNote = std::clamp(midiNote, 0, 127);
        filterMidiVelocity = std::clamp(velocity, 0.0f, 1.0f) * 127.0f;
        // Snapshot normally refreshes modifier targets once per block. Refresh
        // again at the note edge so key tracking never starts a new note at the
        // preceding note's cutoff for one audio block.
        const float lower=lowerFilterSemitones(),upper=upperFilterSemitones();
        filter.setExternalFilterModifiers(lower,upper,0.0f,0.0f,filterTrackHpf,filterTrackLpf,filterMidiNote);
        mnmFilter.setExternalFilterModifiers(lower,upper,0.0f,0.0f,filterTrackHpf,filterTrackLpf,filterMidiNote);
        independentPhysicalFilter.setExternalFilterModifiers(lower,upper,0.0f,0.0f,filterTrackHpf,filterTrackLpf,filterMidiNote);
        filter.triggerEnvelope();mnmFilter.trigger();independentPhysicalFilter.trigger();filterModEnv.on();
    }
    void release(){
        filter.releaseEnvelope();mnmFilter.release();independentPhysicalFilter.release();filterModEnv.off();
    }
    void set(const std::array<float,32>& p){params=p;
        // тот же порядок страницы FILT: ATK, DEC, BOFS(64=0), WOFS(64=0)
        filter.setParameters(p[16],p[17],p[18],p[19],p[20],p[21],p[22]-64,p[23]-64);
        // OLD retains its own historic biquad topology; MNM, NEW and CORE
        // own separate feedback-filter state but receive the same documented
        // host-side DBAS/DWID control mapping.
        delayFilter.setParameters(p[track_delay_routing::kDelayFilterBase],p[track_delay_routing::kDelayFilterWidth],delayBaseQ,delayWidthQ,0,0,0,0);
        mnmDelay.setFeedbackFilterParameters(p[track_delay_routing::kDelayFilterBase],p[track_delay_routing::kDelayFilterWidth],delayBaseQ,delayWidthQ);
        newDelay.setFeedbackFilterParameters(p[track_delay_routing::kDelayFilterBase],p[track_delay_routing::kDelayFilterWidth],delayBaseQ,delayWidthQ);
        float frequency=20*std::pow(1000.0f,norm(p[24])),db=(p[25]-64)/64*18;eqL.peak(sr,frequency,db);eqR.peak(sr,frequency,db);
        targetDelay=static_cast<float>(sr*delaySecondsForRaw(p[track_delay_routing::kDelayTime]));
    }
    void process(float* l,float* r,const float* amp,int n){
        // Documented track path: EQ -> FILT -> DIST -> AMP ENV -> VOL/PAN
        // -> SRR -> DELAY. Track LEV is applied by the processor only after
        // the complete P1/P2 path.
        //
        // There is one audible TrackChain DIST block and it is post-filter.
        // Its exact firmware headroom/threshold law remains open: do not add a
        // guessed pre-gain, cutoff multiplier, or second distortion stage here.
        stageEQ(l,r,n);
        stageFILT(l,r,n);
        stageDIST(l,r,n);
        stageENV(l,r,amp,n);
        stageVOLPAN(l,r,n);
        stageSRR(l,r,n);
        stageDELAY(l,r,n);
    }
    void stageDIST(float* l,float* r,int n);

    void stageEQ(float* l,float* r,int n);

    void stageFILT(float* l,float* r,int n);

    void stageENV(float* l,float* r,const float* amp,int n);

    void stageVOLPAN(float* l,float* r,int n);

    void stageSRR(float* l,float* r,int n);

    void stageDELAY(float* l,float* r,int n);
private:
    float delaySecondsForRaw(float rawDtim) const noexcept {
        const float dtim=std::clamp(std::isfinite(rawDtim)?rawDtim:0.0f,0.0f,127.0f);
        if(delayBpmSync)
            return (60.0f/delayTempo)*((dtim+1.0f)/64.0f);
        return delayMaximumSeconds*((dtim+1.0f)/128.0f);
    }
    uint8_t u(int i)const{return static_cast<uint8_t>(std::clamp(juce::roundToInt(params[static_cast<size_t>(i)]),0,127));}
    bool hasActiveFilterExtras() const noexcept {
        return filterVelLower>0.5f || filterVelUpper>0.5f || filterKeyLower>0.5f || filterKeyUpper>0.5f
            || filterSaturation.active()
            || (filterEnvMix>0.5f && (std::abs(filterEnvBase)>0.5f || std::abs(filterEnvWidth)>0.5f));
    }
    float lowerFilterSemitones() const noexcept {
        // Legacy opt-in VEL/KT modulation remains additive. The documented
        // binary HPF/LPF tracking law itself is applied in each filter core so
        // BASE=0 can be anchored to note / 4 rather than an arbitrary C4 delta.
        return (filterVelLower / 127.0f) * (filterMidiVelocity / 127.0f) * 64.0f
             + (filterKeyLower / 127.0f) * static_cast<float>(filterMidiNote - 60);
    }
    float upperFilterSemitones() const noexcept {
        return (filterVelUpper / 127.0f) * (filterMidiVelocity / 127.0f) * 64.0f
             + (filterKeyUpper / 127.0f) * static_cast<float>(filterMidiNote - 60);
    }
    bool hasNeutralMnmFilter() const noexcept {
        return params[16]<=0.5f&&params[17]>=126.5f&&params[18]<=0.5f&&params[19]<=0.5f
            &&std::abs(params[22]-64.0f)<0.5f&&std::abs(params[23]-64.0f)<0.5f
            // P2 may be bypassed only while both documented keytrack
            // switches retain their factory ON position. A user disabling
            // either physical edge must enter the filter path rather than
            // being hidden by the neutral-insert optimisation.
            &&filterTrackHpf&&filterTrackLpf&&!hasActiveFilterExtras();
    }
    int filterMode=monomachine::dspModeMnm,distMode=monomachine::dspModeMnm,delayMode=monomachine::dspModeMnm,routingMode=monomachine::dspModeMnm;
    int hybridLowerMode=hybrid_private::hybridNative,hybridUpperMode=hybrid_private::hybridNative;
    int hybridSaturationMode=hybrid_private::hybridDistMnm;
    monomachine::mnm::FilterCore mnmFilter;
    monomachine::mnm::IndependentPhysicalFilterCore independentPhysicalFilter;
    monomachine::mnm::DelayCore mnmDelay;
    monomachine::mnm::TrackDelayNewCore newDelay;
    hybrid_private::OversamplingDistortionStereo hybridSaturation;
    hybrid_private::OdinKorgFilterSaturationStereo filterSaturation;
    hybrid_private::MnmFixNotchCompensatorStereo mnmFixNotch;
    double sr=44100;std::array<float,32> params{};monomachine::MonomachineFilter filter,delayFilter;Biquad eqL,eqR;
    AmpEnvelope filterModEnv;
    float filterVelLower=0.0f,filterVelUpper=0.0f,filterKeyLower=0.0f,filterKeyUpper=0.0f;
    float filterEnvMix=0.0f,filterEnvBase=0.0f,filterEnvWidth=0.0f,filterMidiVelocity=127.0f;
    int filterMidiNote=60;
    // Normal Monomachine filter tracking is connected unless its particular
    // HPF/LPF checkbox is explicitly bypassed.
    bool filterTrackHpf=true,filterTrackLpf=true;
    bool negativeDelayComb=false;std::vector<float> delayL,delayR;size_t write=0;int srrCounter=0;float heldL=0,heldR=0,time=1,targetDelay=1;float repitchSlew=0.030f;float declickSlew=0.0f; // 1.6.8: гладкость при резкой смене длины
    float delayTempo=120.0f,delayMaximumSeconds=0.75f;
    bool delayBpmSync=true;
    bool delayFeedbackReturnComp=false,delayFeedbackLoopClip=true;
    bool delayFeedbackInvertPositive=false,delayFeedbackInvertNegative=false;
    float lastDelayFeedbackRaw=0.0f;
    DelayFeedbackDynamics delayFeedbackDynamics;
    float delayBaseQ=0.0f,delayWidthQ=0.0f; // retained 12 dB DBAS/DWID feedback Q
    bool neutralFilterThru=false,neutralFilterThruActive=false;
    // Pan/volume are manual continuous controls. Their target may arrive on the
    // 8-sample control cadence, so finish the ramp at audio rate to avoid the
    // zipper/"bitcrush" heard while moving PAN quickly.
    bool panSmoothingReady=false;
    float smoothedPan=0.0f,smoothedVolume=1.0f;
    // The DIST target is refreshed on the 8-sample control cadence.  The
    // current compatibility saturation needs an audio-rate de-zipper so the
    // cadence itself cannot create a waveform step.
    bool distSmoothingReady=false;
    float smoothedDist=64.0f,distSlew=1.0f;

};

// 1.8.4: DSP stages are separate files; TrackChain above remains the compatibility compositor.
#include "dsp/TrackDIST.inl"
#include "dsp/TrackSRR.inl"
#include "dsp/TrackFILT.inl"
#include "dsp/TrackEQ.inl"
#include "dsp/TrackENV.inl"
#include "dsp/TrackVOLPAN.inl"
#include "dsp/TrackDelay.inl"
} 
