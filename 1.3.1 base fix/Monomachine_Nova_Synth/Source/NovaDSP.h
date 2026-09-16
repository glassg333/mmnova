#pragma once
#include <JuceHeader.h>
#include "dsp/monomachine_fm_dynamic.hpp"
#include "dsp/monomachine_fm_stat_par.hpp"
#include "dsp/monomachine_bbox.hpp"
#include "dsp/AudioAdapter.h"
#include "dsp/monomachine_effects.hpp"
#include "dsp/monomachine_filter.hpp"
#include <array>
#include <cmath>

namespace nova {
constexpr float pi=3.14159265358979323846f;
inline float norm(float v) {return std::clamp(v/127.0f,0.0f,1.0f);}
inline float hz(float note) {return 440.0f*std::pow(2.0f,(note-69.0f)/12.0f);}
inline double wrap(double x) {return x-std::floor(x);}
inline float noise(uint32_t& state) {state^=state<<13;state^=state>>17;state^=state<<5;return (state/4294967295.0f)*2-1;}
inline float blep(float t,float dt) {if(t<dt){t/=dt;return t+t-t*t-1;} if(t>1-dt){t=(t-1)/dt;return t*t+t+t+1;}return 0;}
// Monomachine LFO table: 0 TRI 1 SAW 2 SQR 3 PUL 4 S&H (handled by the LFO state machine).
inline float lfoShape(int kind,double t0) {
    double t=wrap(t0);
    switch(kind) {
        case 0:return t<0.5?static_cast<float>(4*t-1):static_cast<float>(3-4*t);
        case 1:return static_cast<float>(2*t-1);
        case 2:return t<0.5?1.0f:-1.0f;
        case 3:return t<0.25f?1.0f:-1.0f;
        default:return 0.0f;
    }
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
    void peak(double sr,float frequency,float db,double q=1.414) {
        double w=2*pi*std::clamp(frequency,20.0f,static_cast<float>(sr*0.45))/sr;
        double a=std::pow(10.0,db/40.0),alpha=std::sin(w)/(2*std::max(0.3,q)),den=1+alpha/a;
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
    void reset(double rate){sr=rate;stage=0;level=0;gate=false;}
    void on(){gate=true;stage=1;}
    void off(){gate=false;if(stage)stage=4;}
    float tick() {
        if(stage==1){level+=static_cast<float>(1/(sr*(0.001+norm(p[0])*0.5)));if(level>=1){level=1;stage=2;holdRemaining=static_cast<int>(sr*norm(p[1])*2);}}
        else if(stage==2){if(p[1]>=127 && gate){}else if(--holdRemaining<=0)stage=3;}
        else if(stage==3 || stage==4){const float t=stage==3?p[2]:p[3];level*=static_cast<float>(std::exp(-1/(sr*(0.005+norm(t)*1.5))));if(level<0.0001f){level=0;stage=0;}}
        return level;
    }
};
struct LFO {
    double phase=0;float held=0,random=0;bool oneActive=false;uint32_t rng=0xabcde;
    float shape(int kind,float offset) const {
        if(kind==4)return random;
        return nova::lfoShape(kind,phase+static_cast<double>(offset));
    }
    void reset(){phase=0;held=0;oneActive=false;random=noise(rng);}
    void trigger(const std::array<float,8>& p){int mode=static_cast<int>(p[2]);if(mode==1||mode==3){phase=0;oneActive=true;random=noise(rng);}if(mode==2)held=shape(static_cast<int>(p[3]),norm(p[6]));}
    float process(const std::array<float,8>& p,double sr,double bpm,int n){
        int mode=static_cast<int>(p[2]);float result=mode==2?held:(mode==3&&!oneActive?0:shape(static_cast<int>(p[3]),norm(p[6])));
        if(mode!=3||oneActive){double step=(bpm/60.0)*std::pow(2.0,(p[5]-64)/24.0)*std::pow(2.0,p[4])*n/sr;
            phase+=step;if(phase>=1){phase=wrap(phase);random=noise(rng);if(mode==3)oneActive=false;}}
        return result;
    }
};

// Missing hardware engines are explicitly native approximations, not ROM/firmware emulation.
class MachineEngine {
public:
    monomachine::MonomachineBBox bbox;
    // FX-CHORUS runs the native 24-bit core from OS 1.32B (see dsp/ChorusCore.*), 44.1 kHz schedule with resampling.
    AudioAdapter chorus;std::array<int,8> chorusParams{64,64,64,127,0,0,127,64};int chorusLatency=0;
    int fxLatency() const noexcept {return id==15?chorusLatency:0;}
    void prepare(double rate) {
        sr=rate;dyn.reset(sr);stat.reset(sr);par.reset(sr);phaser.reset(sr);flanger.reset(sr);ring.reset(sr);bbox.reset(sr);
        chorusLatency=chorus.prepare(rate);chorus.reset(chorusParams);
        reverb.setSampleRate(sr);clear();
    }
    void clear() {
        phases.fill(0);age=0;toneState=0;colourL=colourR=0;compressorEnv=compressorPower=0;gateCounter=0;noiseCounter=0;
        dyn.reset(sr);stat.reset(sr);par.reset(sr);bbox.reset(sr);chorus.reset(chorusParams);phaser.reset(sr);flanger.clearBuffers();ring.reset(sr);reverb.reset();
        rvHpInL=rvHpOutL=rvHpInR=rvHpOutR=rvLpL=rvLpR=0;
        for(auto& f:formant)f.clear();
    }
    void set(int machine,const std::array<float,8>& values) {
        id=machine;p=values;
        auto b=[&](int i){return static_cast<uint8_t>(std::clamp(juce::roundToInt(p[static_cast<size_t>(i)]),0,127));};
        if(id==10) dyn.setParameters(b(0),b(1),b(2),b(3),b(4),b(5),b(6),b(7));
        if(id==8) stat.setParameters(b(0),b(1),b(2),b(3),b(4),b(5),b(6),b(7));
        if(id==9) par.setParameters(b(0),b(1),b(2),b(3),b(4),b(5),b(6),b(7));
        if(id==7) bbox.setParameters(b(0),b(1),b(4),b(5),b(3),false,b(6)>0,b(2)>0);
        if(id==15) {for(int i=0;i<8;++i)chorusParams[static_cast<size_t>(i)]=b(i);chorus.setParameters(chorusParams);}
        if(id==18) phaser.setParameters(b(0),b(1),b(2),b(3),b(4),b(5));
        if(id==19) flanger.setParameters(b(0),b(1),b(2),b(3),b(4),b(5));
        if(id==17) ring.setParameters(b(0),b(1),b(3),p[2]+pitchMod+(pitch-60));
        if(id==13) {
            juce::Reverb::Parameters r;r.roomSize=norm(p[0])*0.95f;r.damping=norm(p[1]);r.wetLevel=norm(p[3]);r.dryLevel=1-r.wetLevel;r.width=1;r.freezeMode=0;
            reverb.setParameters(r);
        }
        if(id==11){float sweep=norm(p[2])*300*std::sin(static_cast<float>(age/sr)*5);formant[0].band(sr,200+norm(p[0])*1200+sweep,4);formant[1].band(sr,700+norm(p[1])*2700,6);formant[2].band(sr,2400+norm(p[3])*1800,8);}
    }
    void on(int midi) {
        baseNote=midi;age=0;dyn.noteOn(static_cast<uint8_t>(midi));stat.noteOn(static_cast<uint8_t>(midi));par.noteOn(static_cast<uint8_t>(midi));bbox.noteOn(static_cast<uint8_t>(midi));
        if((id==3 && p[2]>63)||(id==5 && p[6]>63)||(id==6 && p[3]>63))phases.fill(0);
    }
    void setPitch(float absoluteNote,float modulation) {
        pitch=absoluteNote;pitchMod=modulation;
        float bend=absoluteNote-baseNote+modulation;dyn.setPitchBend(bend);stat.setPitchBend(bend);par.setPitchBend(bend);bbox.setPitchBend(bend);
    }
    void render(float* l,float* r,int n,bool synth) {
        if(!synth){renderFX(l,r,n);return;}
        if(id==10){dyn.processStereo(l,r,static_cast<size_t>(n));return;}
        if(id==8||id==9){if(id==8)stat.processStereo(l,r,static_cast<size_t>(n));else par.processStereo(l,r,static_cast<size_t>(n));
            float a=1-std::exp(-2*pi*(100+19000*norm(p[6])*norm(p[6]))/static_cast<float>(sr));for(int i=0;i<n;++i){toneState+=a*(l[i]-toneState);l[i]=r[i]=toneState;}return;}
        if(id==7){bbox.processStereo(l,r,static_cast<size_t>(n));for(int i=0;i<n;++i)l[i]=r[i]=l[i]*norm(p[7]);return;}
        const float frequency=std::min(hz(pitch+pitchMod+(p[7]-64)/64*12),static_cast<float>(sr*0.45));
        const float dt=frequency/static_cast<float>(sr);
        for(int i=0;i<n;++i){
            float a=0,b=0;double t=phases[0];float env=std::exp(-static_cast<float>(age/sr)*4);
            if(id==0){a=b=0;}
            else if(id==1){a=b=wave(0,t,dt);}
            else if(id==2){
                int interval=1+static_cast<int>(norm(p[1])*63*sr/44100);
                if(noiseCounter==0){heldNoiseL=noise(rng);heldNoiseR=noise(rng);}noiseCounter=(noiseCounter+1)%interval;
                float cutoff=100+norm(p[7])*norm(p[7])*17000;float smooth=1-std::exp(-2*pi*cutoff/static_cast<float>(sr));
                colourL+=smooth*(heldNoiseL-colourL);colourR+=smooth*(heldNoiseR-colourR);
                a=colourL;b=colourL+(colourR-colourL)*norm(p[0])*norm(p[2]);
            } else if(id==3){
                float pw=std::clamp(norm(p[0])+norm(p[1])*env*0.4f,0.05f,0.95f);int w=static_cast<int>(p[3])/32;
                float ratio=std::pow(2.0f,(p[6]-64)/32);double old=phases[1];phases[1]=wrap(phases[1]+dt*ratio);
                if(p[4]>42&&p[4]<85&&phases[1]<old)phases[0]=t=0;
                a=w==3?noise(rng):wave(w==0?3:w==1?1:2,t,dt,pw);
                if(p[4]>=85)a*=wave(static_cast<int>(p[5])/32,phases[1],dt*ratio);b=a;
            } else if(id==4||id==5){
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
        // INP is the track's FX input gain (64 = unity, 0 = silence, 127 ~= +6 dB). CHORUS feeds its own INP word to the native core.
        if(id!=15){const float g=p[7]/64.0f;for(int i=0;i<n;++i){l[i]*=g;r[i]*=g;}}
        if(id==15){for(int i=0;i<n;++i){float a=l[i],b=r[i],oL=0,oR=0;chorus.processFrame(a,b,oL,oR,false);l[i]=oL;r[i]=oR;}}
        else if(id==18)phaser.processStereo(l,r,l,r,static_cast<size_t>(n));
        else if(id==19)flanger.processStereo(l,r,l,r,static_cast<size_t>(n));
        else if(id==17){ring.setParameters(static_cast<uint8_t>(p[0]),static_cast<uint8_t>(p[1]),static_cast<uint8_t>(p[3]),p[2]+pitchMod+(pitch-60));ring.processStereo(l,r,l,r,static_cast<size_t>(n));}
        else if(id==13){
            std::array<float,32> dryL{},dryR{};std::copy_n(l,n,dryL.data());std::copy_n(r,n,dryR.data());
            const float hpF=30.0f+std::pow(norm(p[4]),2.0f)*970.0f,ah=hpF>31.0f?1.0f-std::exp(-2*pi*hpF/static_cast<float>(sr)):0.0f;
            const float lpF=400.0f*std::pow(50.0f,norm(p[5])),al=1.0f-std::exp(-2*pi*lpF/static_cast<float>(sr));
            for(int i=0;i<n;++i){float x=l[i],y=r[i];
                float hl=ah*(rvHpOutL+x-rvHpInL);rvHpInL=x;rvHpOutL=hl;
                float hr=ah*(rvHpOutR+y-rvHpInR);rvHpInR=y;rvHpOutR=hr;
                l[i]=hl;r[i]=hr;}
            reverb.processStereo(l,r,n);
            for(int i=0;i<n;++i){rvLpL+=al*(l[i]-rvLpL);rvLpR+=al*(r[i]-rvLpR);l[i]=rvLpL;r[i]=rvLpR;}
            for(int i=0;i<n;++i){if(std::max(std::abs(dryL[static_cast<size_t>(i)]),std::abs(dryR[static_cast<size_t>(i)]))>0.002f)gateCounter=static_cast<int>(sr*(0.02+norm(p[2])*2));
                if(p[2]>0&&--gateCounter<=0){float dry=1-norm(p[3]);l[i]=dryL[static_cast<size_t>(i)]*dry;r[i]=dryR[static_cast<size_t>(i)]*dry;}}
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
    monomachine::MonomachineFmDynamic dyn;monomachine::MonomachineFmStatic stat;monomachine::MonomachineFmParallel par;
    monomachine::MonomachinePhaser phaser;monomachine::MonomachineFlanger flanger;monomachine::MonomachineRingMod ring;
    float rvHpInL=0,rvHpOutL=0,rvHpInR=0,rvHpOutR=0,rvLpL=0,rvLpR=0;
    juce::Reverb reverb;
};

class TrackChain {
public:
    void prepare(double rate){sr=rate;delayL.assign(static_cast<size_t>(sr*0.75)+8,0);delayR=delayL;filter.reset(sr);delayFilter.reset(sr);clear();}
    void clear(){filter.reset(sr);delayFilter.reset(sr);eqL.clear();eqR.clear();std::fill(delayL.begin(),delayL.end(),0);std::fill(delayR.begin(),delayR.end(),0);write=0;srrCounter=0;heldL=heldR=0;}
    void trigger(){filter.triggerEnvelope();}
    void release(){filter.releaseEnvelope();}
    void set(const std::array<float,32>& p){params=p;
        filter.setParameters(u(16),u(17),u(18),u(19),u(20),u(21),static_cast<int8_t>(p[22]-64),static_cast<int8_t>(p[23]-64));
        delayFilter.setParameters(u(30),u(31),0,0,0,0,0,0);
        float frequency=20*std::pow(1000.0f,norm(p[24])),db=(p[25]-64)/64*18;eqL.peak(sr,frequency,db);eqR.peak(sr,frequency,db);
        targetDelay=static_cast<float>(sr*(0.001+norm(p[27])*0.749));
    }
    void process(float* l,float* r,const float* amp,int n){
        // SRR: sample-rate hold plus bit depth collapse, both scaled by the same knob (hardware behaviour).
        const int hold=1+static_cast<int>(std::pow(norm(params[26]),1.5f)*63*sr/44100);
        const float steps=std::pow(2.0f,16.0f-std::pow(norm(params[26]),1.5f)*12.0f);
        for(int i=0;i<n;++i){if(srrCounter==0){heldL=std::round(l[i]*steps)/steps;heldR=std::round(r[i]*steps)/steps;}srrCounter=(srrCounter+1)%hold;l[i]=heldL;r[i]=heldR;
            // DIST is bipolar like the hardware: -64..-1 stays clean (level trim only), +1..+63 drives the soft clipper.
            const float d=params[12]-64;
            if(d>0){float drive=1+(d/63)*23,k=1/std::tanh(drive);l[i]=std::tanh(l[i]*drive)*k;r[i]=std::tanh(r[i]*drive)*k;}
            else if(d<0){float trim=1+d/64*0.4f;l[i]*=trim;r[i]*=trim;}}
        filter.processStereo(l,r,l,r,static_cast<size_t>(n));
        // DSND is bipolar: negative = ping-pong (send crosses channels), positive = stereo send.
        const float dsend=(params[28]-64)/64,send=std::abs(dsend);const bool cross=dsend<0;
        float pan=(params[14]-64)/64,volume=norm(params[13]),feedback=norm(params[29])*0.95f;
        for(int i=0;i<n;++i){
            float dryL=eqL.tick(l[i])*amp[i]*volume*(1-std::max(0.0f,pan));
            float dryR=eqR.tick(r[i])*amp[i]*volume*(1+std::min(0.0f,pan));
            time+=static_cast<float>(1-std::exp(-1/(sr*0.03)))*(targetDelay-time);
            float pos=static_cast<float>(write)-std::clamp(time,1.0f,static_cast<float>(delayL.size()-2));if(pos<0)pos+=static_cast<float>(delayL.size());
            size_t i0=static_cast<size_t>(pos)%delayL.size(),i1=(i0+1)%delayL.size();float frac=pos-std::floor(pos);
            float a=delayL[i0]+frac*(delayL[i1]-delayL[i0]),b=delayR[i0]+frac*(delayR[i1]-delayR[i0]);
            delayFilter.processStereo(&a,&b,&a,&b,1);
            delayL[write]=(cross?dryR:dryL)*send+a*feedback;delayR[write]=(cross?dryL:dryR)*send+b*feedback;
            l[i]=dryL+a;r[i]=dryR+b;write=(write+1)%delayL.size();
        }
    }
private:
    uint8_t u(int i)const{return static_cast<uint8_t>(std::clamp(juce::roundToInt(params[static_cast<size_t>(i)]),0,127));}
    double sr=44100;std::array<float,32> params{};monomachine::MonomachineFilter filter,delayFilter;Biquad eqL,eqR;
    std::vector<float> delayL,delayR;size_t write=0;int srrCounter=0;float heldL=0,heldR=0,time=1,targetDelay=1;
};
}
