#include "AudioAdapter.h"
namespace nova {
void AudioAdapter::makeKernel(Kernel& table,double cutoff) noexcept {
 constexpr double pi=3.1415926535897932384626433832795;
 for(int phase=0;phase<phases;++phase){
  const double fraction=double(phase)/phases;double total=0;
  for(int tap=0;tap<taps;++tap){
   const double distance=(tap-(taps/2-1))-fraction;
   const double z=pi*distance*cutoff;
   const double sinc=std::abs(z)<1.0e-12?1.0:std::sin(z)/z;
   const double window=0.42+0.5*std::cos(pi*distance/(taps/2))+0.08*std::cos(2*pi*distance/(taps/2));
   const double c=cutoff*sinc*window;table[std::size_t(phase)][std::size_t(tap)]=float(c);total+=c;
  }
  for(float& c:table[std::size_t(phase)])c=float(c/total);
 }
}
AudioAdapter::Stereo AudioAdapter::interpolate(const Ring& ring,double position,const Kernel& kernel) noexcept {
 const auto base=std::int64_t(std::floor(position));
 const int phase=std::clamp(int((position-double(base))*phases),0,phases-1);
 Stereo result{};
 for(int tap=0;tap<taps;++tap){
  const auto s=ring.get(base+tap-(taps/2-1));const auto c=kernel[std::size_t(phase)][std::size_t(tap)];
  result.l+=s.l*c;result.r+=s.r*c;
 }
 return result;
}
int AudioAdapter::prepare(double rate) noexcept {
 hostRate=(std::isfinite(rate)&&rate>=8000&&rate<=768000)?rate:44100.0;
 ratio=hostRate/44100.0;direct=std::abs(hostRate-44100.0)<0.001;
 latencyFrames=direct?16:int(std::ceil(taps/2+(16+taps/2)*ratio))+2;
 if(!direct){
  makeKernel(inputKernel,0.94*std::min(1.0,1.0/ratio));
  makeKernel(outputKernel,0.94*std::min(1.0,ratio));
 }
 reset(NativeChorusCore::defaults); // 1.8.3: дефолт из нативного ядра (бит-идентичен эталону)
 return latencyFrames;
}
void AudioAdapter::reset(const std::array<int,8>& parameters) noexcept {
 core.reset(parameters);hostInput.clear();nativeOutput.clear();
 inputBlock.fill(0);outputBlock.fill(0);inputFill=0;nextNative=0;
 for(int k=0;k<16;++k){wetOutL[k]=0;wetOutR[k]=0;dryInL[k]=0;dryInR[k]=0;}wetPos=0;dryPos=0; // 1.8.3c
}
void AudioAdapter::pushNative(Stereo sample) noexcept {
 inputBlock[std::size_t(inputFill*2)]=toFixed(sample.l);
 inputBlock[std::size_t(inputFill*2+1)]=toFixed(sample.r);
 if(++inputFill==16){
  core.process16(inputBlock.data(),outputBlock.data());inputFill=0;
  for(int i=0;i<16;++i){
   // *2^-23 бит-идентично /8388608 (int<=2^24 в float точно; степень двойки -- сдвиг порядка)
   const Stereo out{float(outputBlock[std::size_t(i*2)])*1.1920928955078125e-07f,float(outputBlock[std::size_t(i*2+1)])*1.1920928955078125e-07f};
   nativeOutput.push(out);
   if(direct){const std::int64_t k=nativeOutput.written-1;wetOutL[std::size_t(k&15)]=out.l;wetOutR[std::size_t(k&15)]=out.r;} // 1.8.3c: выход #k -> слот k&15 (читается на сэмпле k+16)
  }
 }
}
void AudioAdapter::processBlockWetDry(const float* inL,const float* inR,float* wl,float* wr,float* dl,float* dr,int n) noexcept {
 if(!direct){for(int i=0;i<n;++i){processFrame(inL[i],inR[i],wl[i],wr[i],false);delayedInput(dl[i],dr[i]);}return;} // нереземплинг -- прежний путь дословно
 for(int i=0;i<n;++i){
  wl[i]=wetOutL[wetPos];wr[i]=wetOutR[wetPos]; // выход ядра 16 сэмплов назад (== nativeOutput.get(index-16))
  dl[i]=dryInL[dryPos];dr[i]=dryInR[dryPos];   // сухой 16 сэмплов назад (== hostInput.get(written-1-latencyFrames))
  dryInL[dryPos]=inL[i];dryInR[dryPos]=inR[i];dryPos=(dryPos+1)&15;
  pushNative({inL[i],inR[i]}); // при закрытии 16-блока пишет и в FIFO (зеркало)
  wetPos=(wetPos+1)&15;
 }
}
void AudioAdapter::processFrame(float inL,float inR,float& outL,float& outR,bool bypass) noexcept {
 const auto index=hostInput.written;
 hostInput.push({std::isfinite(inL)?inL:0.0f,std::isfinite(inR)?inR:0.0f});
 Stereo result{};
 if(direct){
  result=nativeOutput.get(index-16);pushNative({inL,inR});
 }else{
  while(double(nextNative)*ratio+taps/2<=double(index)){
   pushNative(interpolate(hostInput,double(nextNative)*ratio,inputKernel));++nextNative;
  }
  result=interpolate(nativeOutput,(double(index)-latencyFrames)/ratio,outputKernel);
 }
 if(bypass)result=hostInput.get(index-latencyFrames);
 outL=result.l;outR=result.r;
}
}
