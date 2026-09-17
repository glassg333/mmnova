#include "MnmFilterFromClues.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

static void put16(std::ofstream& f, std::uint16_t x) { f.put(char(x & 255)); f.put(char(x >> 8)); }
static void put32(std::ofstream& f, std::uint32_t x) { for (int i=0;i<4;++i) f.put(char((x>>(8*i))&255)); }
static void wav(const char* path, const std::vector<float>& v, int sr) {
    std::ofstream f(path,std::ios::binary);
    const std::uint32_t data = static_cast<std::uint32_t>(v.size()*2);
    f.write("RIFF",4); put32(f,36+data); f.write("WAVEfmt ",8); put32(f,16);
    put16(f,1); put16(f,1); put32(f,sr); put32(f,sr*2); put16(f,2); put16(f,16);
    f.write("data",4); put32(f,data);
    for(float x:v){ x=std::clamp(x,-1.0f,1.0f); auto q=static_cast<std::int16_t>(std::lrint(x*32767.0f)); put16(f,static_cast<std::uint16_t>(q)); }
}
int main(int argc,char**argv){
    const char* out=argc>1?argv[1]:"filter_demo.wav"; constexpr int sr=44100; constexpr int n=sr*3;
    std::vector<float> audio(n); mmnova_practical::MnmFilterFromClues f; f.prepare(sr);
    mmnova_practical::Params p; p.base=20; p.width=54; p.hpq=18; p.lpq=112; p.atk=6; p.dec=48; p.bofs=92; p.wofs=55; p.mode=mmnova_practical::Mode::SeriesLowPass;
    f.setParams(p); f.setNoteHz(261.625565); f.trigger();
    constexpr double pi=3.14159265358979323846;
    for(int pos=0;pos<n;pos+=16){
       if(pos==sr){p.base=55; p.width=34; p.hpq=70; p.lpq=96; f.setParams(p); f.setNoteHz(329.627557); f.trigger();}
       if(pos==2*sr){p.base=8; p.width=86; p.hpq=40; p.lpq=80; f.setParams(p); f.setNoteHz(196.0); f.trigger();}
       float b[16]; for(int i=0;i<16;++i){int k=pos+i; double t=double(k)/sr; double phase=std::fmod((k*(pos<sr?261.625565:pos<2*sr?329.627557:196.0))/sr,1.0); b[i]=static_cast<float>(0.42*(2.0*phase-1.0)+0.10*std::sin(2*pi*880.0*t));}
       f.processBlock16(b); for(int i=0;i<16 && pos+i<n;++i) audio[pos+i]=b[i];
    }
    wav(out,audio,sr); const auto& d=f.debug(); std::cerr<<"wrote "<<out<<" final hpIndex="<<d.hpTableIndex<<" lpIndex="<<d.lpTableIndex<<" hpQ="<<d.hpQ<<" lpQ="<<d.lpQ<<"\n";
}
