#include "PluginProcessor.h"
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cmath>

static void require(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
using P=MonomachineNovaAudioProcessor;
static void set(P& p,const juce::String& id,float value){auto* a=p.parameters.getParameter(id);require(a!=nullptr,"missing parameter");a->setValueNotifyingHost(a->convertTo0to1(value));}
static std::vector<float> renderMachine(int id,std::array<float,8> params,double rate=44100){
    auto engine=std::make_unique<nova::MachineEngine>();engine->prepare(rate);engine->set(id,params);
    std::vector<float> result;result.reserve(44100*2);
    for(int n=0;n<44100;n+=32){float l[32],r[32];const int count=std::min(32,44100-n);
        for(int i=0;i<count;++i){const float t=static_cast<float>(n+i)/static_cast<float>(rate);
            l[i]=r[i]=n+i<11025?0.08f*std::sin(2*nova::pi*220*t)+0.04f*std::sin(2*nova::pi*1733*t):0.0f;}
        engine->render(l,r,count,false);for(int i=0;i<count;++i){require(std::isfinite(l[i])&&std::isfinite(r[i]),"nonfinite FX output");result.push_back(l[i]);result.push_back(r[i]);}}
    return result;
}
static double difference(const std::vector<float>& a,const std::vector<float>& b){double sum=0;for(size_t i=0;i<a.size();++i)sum+=std::abs(a[i]-b[i]);return sum/a.size();}
int main(){juce::ScopedJuceInitialiser_GUI juce;
try{
    // Test the actual math used by TrackChain, including mono ping-pong.
    float a=0,b=0;
    for(int echo=0;echo<5;++echo){float l=0,r=0;nova::delayWrite(echo==0?1.0f:0.0f,echo==0?1.0f:0.0f,a,b,1,0.5f,true,l,r);
        require(echo%2==0?(l>0&&r==0):(r>0&&l==0),"ping-pong fails to alternate mono input");a=l;b=r;}
    nova::delayWrite(1,0,0,0,1,0.5f,false,a,b);require(a==1&&b==0,"stereo send swapped channels");
    nova::delayWrite(1,1,0,0,0,0.5f,true,a,b);require(a==0&&b==0,"zero DSND sends audio");
    nova::delayWrite(0,0,1,0,0,0.5f,true,a,b);require(a==0&&b==0.5f,"zero send kills/changes tail routing");
    for(float x:{-0.3f,-0.05f,0.0f,0.05f,0.3f}){
        require(nova::bipolarDist(x,64)==x,"DIST centre not unity");
        for(int d=0;d<64;++d)require(std::abs(nova::bipolarDist(x,static_cast<float>(d)))<=std::abs(x),"negative DIST boosts");
        if(x!=0)require(std::abs(nova::bipolarDist(x,127))>std::abs(x),"positive DIST does not boost");}
    for(int i=0;i<100;++i){const double t=i/100.0;for(auto pair:{std::pair<int,int>{0,1},{2,3},{4,5},{6,7},{8,9}})
        require(std::abs(nova::lfoShape(pair.first,t)+nova::lfoShape(pair.second,t))<1e-6f,"LFO inverse mismatch");}
    for(int id:{18,19,13,16,17}){
        std::array<float,8> p{64,100,80,64,80,90,64,64};if(id==17)p[1]=0;if(id==16){p[0]=12;p[2]=32;}
        const auto baseline=renderMachine(id,p);
        for(int knob:(id==18||id==19?std::vector<int>{3,4,5,7}:id==13?std::vector<int>{0,1,2,3,4,5,7}:id==16?std::vector<int>{2,3,4,5,7}:std::vector<int>{0,1,2,3,7})){
            auto low=p,high=p;low[static_cast<size_t>(knob)]=0;high[static_cast<size_t>(knob)]=(id==13&&knob==2)?1:127;
            const double delta=difference(renderMachine(id,low),renderMachine(id,high));
            std::cout<<"FX "<<id<<" knob "<<knob<<" delta="<<delta<<"\n";
            require(delta>1e-7,"FX control is disconnected");}
        p[3]=0;const auto dry=renderMachine(id,p);
        for(size_t i=0;i<dry.size()/2;++i){const float t=static_cast<float>(i)/44100.0f;
            const float reference=i<11025?0.08f*std::sin(2*nova::pi*220*t)+0.04f*std::sin(2*nova::pi*1733*t):0.0f;
            require(std::abs(dry[2*i]-reference)<1e-6f&&std::abs(dry[2*i+1]-reference)<1e-6f,"FX MIX=0 changes dry");}
        std::cout<<"FX "<<id<<": controls + MIX=0 PASS\n";
    }
    // HP=0 must not mute a full-wet reverb. WID on modulated effects must spread mono.
    auto reverb=renderMachine(13,{80,32,0,127,0,127,0,64});double energy=0;for(float x:reverb)energy+=x*x;require(energy>0.01,"HP=0 mutes reverb");
    for(int id:{18,19}){auto mono=renderMachine(id,{64,100,80,127,0,0,0,64});auto wide=renderMachine(id,{64,100,80,127,0,127,0,64});double side=0;
        for(size_t i=0;i<mono.size();i+=2){require(mono[i]==mono[i+1],"WID=0 is not mono for mono input");side+=std::abs(wide[i]-wide[i+1]);}require(side>0.1,"WID has no stereo effect");}
    if(!P::isSynthVersion){
        auto p=std::make_unique<P>();int chorus=-1,thru=-1;for(size_t i=0;i<nova::machines().size();++i){if(nova::machines()[i].id==15)chorus=static_cast<int>(i);if(nova::machines()[i].id==12)thru=static_cast<int>(i);}
        set(*p,"level",100);set(*p,"fx_mix",0);set(*p,"machine",static_cast<float>(chorus));
        for(auto pair:{std::pair<double,int>{44100,16},{48000,87},{96000,139}}){
            p->prepareToPlay(pair.first,256);require(p->getLatencySamples()==pair.second,"initial chorus latency incorrect");
            juce::AudioBuffer<float> buffer(2,256);buffer.clear();buffer.setSample(0,0,0.25f);juce::MidiBuffer events;p->processBlock(buffer,events);
            for(int i=0;i<256;++i)require(buffer.getSample(0,i)==(i==pair.second?0.25f:0.0f),"global dry is not latency aligned");
            set(*p,"machine",static_cast<float>(thru));buffer.clear();p->processBlock(buffer,events);require(p->getLatencySamples()==0,"leaving chorus retains latency");
            set(*p,"machine",static_cast<float>(chorus));buffer.clear();p->processBlock(buffer,events);require(p->getLatencySamples()==pair.second,"entering chorus loses latency");
        }
    }
    std::cout<<"DIST, DSND, LFO, reverb HP, stereo width, latency/dry PASS\n";
    return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
