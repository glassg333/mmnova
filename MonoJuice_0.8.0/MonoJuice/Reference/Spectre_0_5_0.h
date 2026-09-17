/* Spectrum Pixelator algorithm port: Michael Nuzzo, Copyright (c) 2019.
 * BSD-3-Clause: Licenses/SpectrumPixelator-BSD-3-Clause.txt.
 * Native C++ implementation/modifications 2026 MonomachineNova contributors.
 * KBD(beta=5), MDCT/IMDCT, dB peak prominence and 50% overlap-add.
 * New block-independent streaming/FFT implementation; power-of-two window menu
 * replaces MATLAB's arbitrary even window length. No MATLAB runtime or codegen.
 */
#pragma once
#include "../Source/dsp/FirmwarePrimitives.h"
#include <complex>
#include <memory>
#include <array>
namespace nova::oldSpectre {
using namespace nova::fx;
inline constexpr double pi=3.1415926535897932384626433832795;
inline constexpr int spectreMax=65536;
struct SpectreSettings {float prominence=10,mix=100;int window=3;}; // 512 << index
struct MDCTPlan {
 int length=0,n=0;std::vector<float> window;std::vector<std::complex<float>> roots,pre,post;std::vector<int> reverse;
 static double bessel(double x){double term=1,sum=1;for(int k=1;k<80;k++){term*=x*x/(4.*k*k);sum+=term;if(term<sum*1e-16)break;}return sum;}
 void prepare(int l){length=l;n=l/2;window.resize(l);roots.resize(l/2);reverse.resize(l);pre.resize(n);post.resize(n);
  double total=0;for(int i=0;i<=n;i++){const double x=2.*i/n-1;total+=bessel(5*pi*std::sqrt(std::max(0.,1-x*x)));}
  double accum=0;for(int i=0;i<n;i++){const double x=2.*i/n-1;accum+=bessel(5*pi*std::sqrt(std::max(0.,1-x*x)));window[i]=window[l-1-i]=float(std::sqrt(accum/total));}
  int bits=0;while((1<<bits)<l)++bits;
  for(int i=0;i<l;i++){int b=i,r=0;for(int k=0;k<bits;k++){r=(r<<1)|(b&1);b>>=1;}reverse[i]=r;}
  for(int i=0;i<l/2;i++)roots[i]=std::polar(1.f,float(-2*pi*i/l));
  for(int i=0;i<n;i++){pre[i]=std::polar(1.f,float(-pi*i/(2*n)));post[i]=std::polar(1.f,float(-pi*(i+.5)/(2*n)));}
 }
 void fft(std::complex<float>* x)const{
  for(int i=0;i<length;i++)if(i<reverse[i])std::swap(x[i],x[reverse[i]]);
  for(int len=2;len<=length;len*=2){const int half=len/2,step=length/len;for(int pos=0;pos<length;pos+=len)for(int j=0;j<half;j++){const auto u=x[pos+j],v=x[pos+j+half]*roots[j*step];x[pos+j]=u+v;x[pos+j+half]=u-v;}}
 }
 void dct4(const float* input,float* output,std::complex<float>* work)const{
  for(int i=0;i<n;i++)work[i]=input[i]*pre[i];std::fill(work+n,work+length,std::complex<float>{});fft(work);
  for(int i=0;i<n;i++)output[i]=(work[i]*post[i]).real();
 }
};
struct MDCTPlans {std::array<MDCTPlan,8> plans;void prepare(){for(int i=0;i<8;i++)plans[i].prepare(512<<i);}const MDCTPlan& get(int i)const{return plans[std::clamp(i,0,7)];}};
class Spectre {
public:
 void prepare(const MDCTPlans& plans){bank=&plans;input.resize(spectreMax);overlap.resize(spectreMax/2);queue.resize(spectreMax/2);work.resize(spectreMax);folded.resize(spectreMax/2);coeff.resize(spectreMax/2);db.resize(spectreMax/2);left.resize(spectreMax/2);right.resize(spectreMax/2);stack.resize(spectreMax/2);valley.resize(spectreMax/2);dry.prepare(spectreMax+8);set(p);reset();}
 bool set(SpectreSettings s){s.window=std::clamp(s.window,0,7);const bool changed=!plan||p.window!=s.window;p=s;plan=&bank->get(s.window);if(changed)reset();return changed;}
 void reset(){if(!plan)return;std::fill(input.begin(),input.begin()+plan->length,Frame{});std::fill(overlap.begin(),overlap.begin()+plan->n,Frame{});std::fill(queue.begin(),queue.begin()+plan->n,Frame{});dry.clear();position=0;mix=p.mix*.01f;}
 int latency()const{return plan?plan->length:(512<<std::clamp(p.window,0,7));}
 Frame process(Frame in,bool preserveAll=false){
  const auto at=dry.sizeWritten();dry.push(in);Frame wet=queue[position];input[plan->n+position]=in;
  if(++position==plan->n){render(preserveAll);position=0;}
  mix+=(std::clamp(p.mix*.01f,0.f,1.f)-mix)*.002f;
  return fx::mix(dry.at(at-plan->length),wet,mix);
 }
 // Exact prominence bases: minimum between this bin and the nearest higher
 // value (or boundary). Equal-height peaks do not stop the search. O(N).
 static void bases(const float* values,float* result,int* indices,float* minima,int n,bool backwards){int top=0;
  for(int t=0;t<n;t++){const int i=backwards?n-1-t:t;float v=values[i];while(top>0&&values[indices[top-1]]<=values[i]){v=std::min(v,minima[--top]);}result[i]=v;indices[top]=i;minima[top++]=v;}
 }
 static void prominenceMask(float* values,int n,float threshold,float* magnitudes,float* leftBase,float* rightBase,int* indices,float* minima){
  for(int i=0;i<n;i++)magnitudes[i]=20*std::log10(std::max(1.e-15f,std::abs(values[i])));
  bases(magnitudes,leftBase,indices,minima,n,false);bases(magnitudes,rightBase,indices,minima,n,true);
  // No endpoint peaks; preserve the first bin of a flat local maximum.
  int i=1;values[0]=0;while(i<n-1){int end=i;while(end+1<n&&magnitudes[end+1]==magnitudes[i])++end;
   const bool keep=end<n-1&&magnitudes[i]>magnitudes[i-1]&&magnitudes[end]>magnitudes[end+1]&&magnitudes[i]-std::max(leftBase[i],rightBase[i])>=threshold;
   if(!keep)values[i]=0;for(int j=i+1;j<=end;j++)values[j]=0;i=end+1;
  }values[n-1]=0;
 }
private:
 void render(bool preserveAll){const int n=plan->n;
  for(int channel=0;channel<2;channel++){
   auto sample=[&](int i){return (channel?input[i].r:input[i].l)*plan->window[i];};
   for(int m=0;m<n/2;m++)folded[m]=-sample(3*n/2-1-m)-sample(3*n/2+m);
   for(int m=n/2;m<n;m++)folded[m]=sample(m-n/2)-sample(3*n/2-1-m);
   plan->dct4(folded.data(),coeff.data(),work.data());
   if(!preserveAll)prominenceMask(coeff.data(),n,std::clamp(p.prominence,1.f,100.f),db.data(),left.data(),right.data(),stack.data(),valley.data());
   plan->dct4(coeff.data(),folded.data(),work.data());
   for(int i=0;i<2*n;i++){float v;if(i<n/2)v=folded[n/2+i];else if(i<3*n/2)v=-folded[3*n/2-1-i];else v=-folded[i-3*n/2];v*=2.f/n*plan->window[i];
    if(i<n){if(channel)queue[i].r=v+overlap[i].r;else queue[i].l=v+overlap[i].l;}
    else {if(channel)overlap[i-n].r=v;else overlap[i-n].l=v;}
   }
  }
  std::copy(input.begin()+n,input.begin()+2*n,input.begin());
 }
 const MDCTPlans* bank=nullptr;const MDCTPlan* plan=nullptr;SpectreSettings p;int position=0;float mix=1;
 std::vector<Frame> input,overlap,queue;std::vector<std::complex<float>> work;std::vector<float> folded,coeff,db,left,right,valley;std::vector<int> stack;History dry;
};
}
