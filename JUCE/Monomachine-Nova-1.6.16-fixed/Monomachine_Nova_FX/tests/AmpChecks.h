struct LegacyAmpOracle {
    double sr=44100;int stage=0;float level=0;int holdRemaining=0;bool gate=false;
    std::array<float,4> p{0,0,64,64};
    void reset(double rate){sr=rate;stage=0;level=0;gate=false;}
    void on(){gate=true;stage=1;}
    void off(){gate=false;if(stage)stage=4;}
    float tick() {
        if(stage==1){level+=static_cast<float>(1/(sr*(0.001+nova::norm(p[0])*0.5)));if(level>=1){level=1;stage=2;holdRemaining=static_cast<int>(sr*nova::norm(p[1])*2);}}
        else if(stage==2){if(p[1]>=127 && gate){}else if(--holdRemaining<=0)stage=3;}
        else if(stage==3 || stage==4){const float t=stage==3?p[2]:p[3];level*=static_cast<float>(std::exp(-1/(sr*(0.005+nova::norm(t)*1.5))));if(level<0.0001f){level=0;stage=0;}}
        return level;
    }
};

static void checkAmp(P& p){
 check(nova::AmpEnvelope::tau(0,0)==nova::AmpEnvelope::tau(0,1),"zero-time endpoint preserved");
 check(p.parameters.getRawParameterValue("amp_mode")->load()==1,"new instance reference AMP default");
 for(double sr:{44100.0,48000.0,96000.0}){
  nova::AmpEnvelope e;LegacyAmpOracle old;e.reset(sr);old.reset(sr);e.on();old.on();
  for(int i=0;i<200000;++i){if(i==4000){e.off();old.off();}if(i==20000){e.p={5,127,100,20};old.p=e.p;e.on();old.on();}if(i==30000){e.off();old.off();}check(e.tick()==old.tick(),"LEGACY AMP changed");}
  e.configure(1,{0,0,0});e.p={0,0,64,64};e.reset(sr);e.on();float first=0,last=0;
  for(int i=0;i<static_cast<int>(sr*1.21);++i){auto v=e.tick();if(i==static_cast<int>(sr*0.2))first=v;if(i==static_cast<int>(sr*1.2))last=v;}
  const float slope=20*std::log10(last/first);check(std::abs(slope+34.8959f)<0.06f,"reference 64 decay slope");
  std::cout<<"AMP reference slope @"<<sr<<": "<<slope<<" dB/s\n";
  for(float curve:{-100.0f,0.0f,100.0f}){e.configure(1,{curve,curve,curve});e.reset(sr);e.on();float previous=0;for(int i=0;i<static_cast<int>(sr*3);++i){if(i==1000)e.off();float v=e.tick();check(std::isfinite(v)&&v>=0&&v<=1,"AMP curve bounds");if(i>1000)check(v<=previous,"release not monotone");previous=v;}}
 }
 auto state=p.parameters.copyState();state.setProperty("schema",4,nullptr);
 for(auto id:{"amp_mode","amp_curve_a","amp_curve_d","amp_curve_r"})state.removeChild(state.getChildWithProperty("id",id),nullptr);
 juce::MemoryBlock data;if(auto xml=state.createXml())P::copyXmlToBinary(*xml,data);
 p.setStateInformation(data.getData(),static_cast<int>(data.getSize()));p.flushSampleRestore();check(p.parameters.getRawParameterValue("amp_mode")->load()==0,"1.4 state must select Legacy AMP");
 set(p,"amp_mode",1);set(p,"amp_curve_d",37.5f);p.getStateInformation(data);auto other=std::make_unique<P>();other->setStateInformation(data.getData(),static_cast<int>(data.getSize()));other->flushSampleRestore();check(other->parameters.getRawParameterValue("amp_curve_d")->load()==37.5f,"curve roundtrip");check(other->parameters.getRawParameterValue("amp_mode")->load()==1,"AMP mode roundtrip");set(p,"amp_curve_d",0);
 std::cout<<"AMP: legacy exact, reference rate, curves, state PASS\n";
}
