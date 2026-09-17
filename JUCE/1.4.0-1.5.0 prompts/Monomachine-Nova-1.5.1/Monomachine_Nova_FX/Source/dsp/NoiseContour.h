#pragma once
#include <array>
#include <cmath>
#include <complex>
#include <algorithm>
namespace nova {
// Fitted to the user's four NOISE spectral contours, not extracted firmware coefficients.
class NoiseContour {
    struct Section {double b0=1,b1=0,b2=0,a1=0,a2=0;std::array<double,2> z1{},z2{};
        void set(int type,double hz,double q,double db,double sr){hz=std::clamp(hz,5.0,sr*0.45);double w=6.283185307179586*hz/sr,c=std::cos(w),a=std::sin(w)/(2*q),den=1+a;
            if(type==0){b0=(1+c)/2;b1=-1-c;b2=b0;a1=-2*c;a2=1-a;}
            else if(type==1){b0=(1-c)/2;b1=1-c;b2=b0;a1=-2*c;a2=1-a;}
            else {double A=std::pow(10.0,db/40);den=1+a/A;b0=1+a*A;b1=-2*c;b2=1-a*A;a1=-2*c;a2=1-a/A;}
            b0/=den;b1/=den;b2/=den;a1/=den;a2/=den;z1.fill(0);z2.fill(0);}
        double tick(double x,size_t ch){double y=b0*x+z1[ch];z1[ch]=b1*x-a1*y+z2[ch];z2[ch]=b2*x-a2*y;return y;}
        std::complex<double> response(std::complex<double> z)const{return (b0+b1*z+b2*z*z)/(1.0+a1*z+a2*z*z);}
    };
public:
    void prepare(double sr){rate=sr;for(size_t b=0;b<4;++b){auto& bank=banks[b];const auto& p=parameters[b];bank[0].set(0,p[0],0.707,0,sr);const double qs[]{.5098,.6013,.9,2.5629};for(size_t j=0;j<4;++j)bank[j+1].set(1,p[1],qs[j],0,sr);for(size_t j=0;j<7;++j)bank[j+5].set(2,centres[j],p[j+10],p[j+3],sr);gains[b]=std::pow(10.0,p[2]/20);}}
    void clear(){for(auto& bank:banks)for(auto& s:bank){s.z1.fill(0);s.z2.fill(0);}}
    float process(float x,size_t ch,float note,float red){std::array<double,4> y{};for(size_t b=0;b<4;++b){double a=x*gains[b];for(auto& section:banks[b])a=section.tick(a,ch);y[b]=a;}
        double k0=std::clamp((note-24.0)/96.0,0.0,1.0),k1=std::clamp(note/120.0,0.0,1.0),r=std::clamp(red/127.0,0.0,1.0);
        return static_cast<float>(((1-k0)*y[0]+k0*y[1])*(1-r)+((1-k1)*y[2]+k1*y[3])*r);}
    double magnitudeDb(size_t profile,double hz)const{auto z=std::exp(std::complex<double>(0,-6.283185307179586*hz/rate));std::complex<double> h=gains[profile];for(const auto& s:banks[profile])h*=s.response(z);return 20*std::log10(std::max(1e-20,std::abs(h)));}
private:
    double rate=44100;std::array<std::array<Section,12>,4> banks{};std::array<double,4> gains{};
    inline static constexpr double centres[]{22,100,400,1200,3500,8000,13500};
    inline static constexpr double parameters[4][17]={{18.699416932,13564.6983105,14.9999967882,-47.5964380226,-23.8997678325,-6.79102753466,-7.25822057673,-17.0588766313,-1.47966248151,-4.68050587695,0.362612695145,0.190837769568,0.489514300256,0.431168314937,0.221060021598,1.1074454354,0.475881373859},
{82.7468116894,18891.2761316,-86.6659391207,33.2028949361,-23.6699008977,-24.117648193,31.1306780724,36.3916320262,23.6425170811,41.2581123813,0.12,0.12,0.12,0.12,0.12,0.315157196269,0.79867087},
{40.2239027907,12947.8268889,10.0335439401,3.23799181744,-26.2740813523,-4.69522614996,-11.0822866386,-5.93500251384,-17.1823872854,-36.5612883354,19.9999995895,0.120020895561,0.287681376408,0.260707261775,0.508412688034,0.157587691403,0.917322377294},
{94.3395848084,19500,-89.9999634105,32.2271716328,-27.4422834923,9.60961330538,-12.8336485894,47.9998857434,22.6269078772,27.1065241624,0.12,0.12,0.463478470677,0.389597089229,0.453469724877,0.251029641347,0.472134190992}};
};
}
