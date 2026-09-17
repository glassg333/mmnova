#pragma once
#include "ChorusTables.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>
namespace nova::fx {
struct Frame { float l=0,r=0; };
inline Frame operator+(Frame a,Frame b){return {a.l+b.l,a.r+b.r};}
inline Frame operator*(Frame a,float b){return {a.l*b,a.r*b};}
inline Frame mix(Frame a,Frame b,float t){if(t<=0)return a;if(t>=1)return b;return a*(1-t)+b*t;}
inline float clean(float v){return std::isfinite(v)?v:0.f;}
// Arithmetic of P:$147763-$147768, re-expressed without an ISA interpreter.
// The read-head scheduling and grain envelopes using it are NEW algorithms.
inline std::int32_t q24(float v){return std::int32_t(std::round(std::clamp(double(clean(v))*8388608.0,-8388608.0,8388607.0)));}
inline float interpolate24(float a,float b,double f){
 const auto x=q24(a),y=q24(b);
 const auto fraction=std::int64_t(std::clamp(f,0.0,0.999999999999)*16777216.0);
 const auto value=std::int64_t(x)*16777216+(std::int64_t(y)-x)*fraction;
 const auto result=value>=0?value/16777216:-((-value+16777215)/16777216);
 return float(std::clamp<std::int64_t>(result,-8388608,8388607))/8388608.f;
}
// Hann envelope derived from the original quarter-sine WID coefficients.
// No claim that this envelope or a granular machine existed in the firmware.
inline float window(double phase){
 phase=std::clamp(phase,0.0,1.0);const double p=std::min(phase,1-phase)*4096.0;
 const auto index=std::min(2048,int(p));const float s=index==2048?1.f:float(nova::widthTable[index])/8388608.f;
 return s*s;
}
class History {
public:
 void prepare(int size){data.assign(std::size_t(std::max(8,size)),{});written=0;}
 void clear(){std::fill(data.begin(),data.end(),Frame{});written=0;}
 void push(Frame f){data[std::size_t(written)%data.size()]=f;++written;}
 Frame at(std::int64_t n) const {if(n<0||n>=written||n<written-std::int64_t(data.size()))return {};return data[std::size_t(n)%data.size()];}
 Frame read(double position) const {const auto n=std::int64_t(std::floor(position));const auto a=at(n),b=at(n+1);const double f=position-double(n);return {interpolate24(a.l,b.l,f),interpolate24(a.r,b.r,f)};}
 // Frozen grains wrap exclusively within the captured valid interval.
 Frame frozen(double position) const {if(written<2)return {};const auto count=std::min<std::int64_t>(written,std::int64_t(data.size()));const auto base=written-count;double p=std::fmod(position-double(base),double(count));if(p<0)p+=double(count);const auto n=std::int64_t(p);const auto a=at(base+n),b=at(base+(n+1)%count);return {interpolate24(a.l,b.l,p-n),interpolate24(a.r,b.r,p-n)};}
 std::int64_t sizeWritten() const{return written;}
private:std::vector<Frame> data;std::int64_t written=0;
};
}
