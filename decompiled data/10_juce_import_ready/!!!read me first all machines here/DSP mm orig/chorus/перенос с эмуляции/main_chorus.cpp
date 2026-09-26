// 1.8.3: ChorusAudit -- стенд бит-точности ChorusCore (эталон) против
// NativeChorusCore (натив). Одинаковые потоки: случайные параметры (включая
// края MIX 0/127), сигналы шум/синус/тишина/импульсы; сравнение выхода
// побитово КАЖДЫЙ вызов + полное сравнение состояния x/y/delay периодически
// и в конце. Вердикт: BIT-EXACT PASS при нулевых расхождениях.
#include "ChorusCore.h"
#include "ChorusNative.h"
#include <cstdio>
#include <cstring>
#include <chrono>
#include <cmath>
using namespace nova;
struct ChorusAudit {
    template<class C> static bool sameState(const C&a,const C&b){
        if(a.x!=b.x||a.y!=b.y||a.delay!=b.delay)return false;
        return true;
    }
    template<class C> static int diffState(const C&a,const C&b){
        int d=0;for(size_t i=0;i<a.x.size();++i)if(a.x[i]!=b.x[i])++d;
        for(size_t i=0;i<a.y.size();++i)if(a.y[i]!=b.y[i])++d;
        for(size_t i=0;i<a.delay.size();++i)if(a.delay[i]!=b.delay[i])++d;
        return d;
    }
};
struct Rng{ std::uint64_t s; std::uint32_t next(){s^=s<<13;s^=s>>7;s^=s<<17;return std::uint32_t(s>>17);} };
int main(){
    Rng rng{0x9e3779b97f4a7c15ull};
    ChorusCore ref; NativeChorusCore nat; // оба reset(defaults) в ctor
    long calls=0,mismatch=0; int stateDiffs=0;
    double phase=0.0;
    for(int seg=0;seg<400;++seg){
        std::array<int,8> p{};
        for(auto&v:p)v=int(rng.next()%128);
        if(seg%4==0)p[3]=(seg%8==0)?0:127;           // MIX в края
        if(seg%7==0)p[0]=seg%128;                     // DEL качается
        ref.setParameters(p);nat.setParameters(p);
        if(seg==200){ref.reset(p);nat.reset(p);}      // полный reset посередине
        for(int k=0;k<53;++k){
            std::array<std::int32_t,32> in,oa,ob;
            const int kind=(seg+k)%5;
            for(int i=0;i<32;++i){
                switch(kind){
                    case 0: in[i]=std::int32_t(rng.next()&0xffffffu)-8388608; break;           // шум полный
                    case 1: in[i]=std::int32_t(std::sin(phase)*4194304.0); phase+=2.0*3.14159265358979*350.0/44100.0; break; // синус
                    case 2: in[i]=0; break;                                                     // тишина
                    case 3: in[i]=(i%16==0)?((i%32)?-8388608:8388607):0; break;                 // импульсы
                    default: in[i]=std::int32_t(rng.next()%2048)-1024; break;                   // тихий шум
                }
            }
            ref.process16(in.data(),oa.data());
            nat.process16(in.data(),ob.data());
            ++calls;
            for(int i=0;i<32;++i)if(oa[i]!=ob[i]){
                ++mismatch;
                if(mismatch<=5)std::printf("MISMATCH call=%ld sample=%d ref=%d nat=%d\n",calls,i,oa[i],ob[i]);
            }
        }
        if(seg%97==3){const int d=diffState(ref,nat);if(d)stateDiffs+=d;}
    }
    {const int d=diffState(ref,nat);if(d)stateDiffs+=d;
     std::printf("calls=%ld out-mismatches=%ld state-diffs=%d\n",calls,mismatch,stateDiffs);
     std::printf(sameState(ref,nat)&&mismatch==0?"BIT-EXACT PASS\n":"FAIL\n");}
    // === микро-бенч: оба ядра на одном шуме, худший случай (MIX=127) ===
    {
        std::array<std::int32_t,32> in{},oa{},ob{};
        for(auto&v:in)v=std::int32_t(rng.next()&0xffffffu)-8388608;
        std::array<int,8> p{{70,95,41,127,127,127,127,64}}; // панель юзера + MIX=127
        ref.setParameters(p);nat.setParameters(p);
        for(int i=0;i<500;++i){ref.process16(in.data(),oa.data());nat.process16(in.data(),ob.data());}
        const int N=400000; // 400к*32 сэмпла ~= 4.8 мин аудио на ядро
        auto t0=std::chrono::steady_clock::now();
        for(int i=0;i<N;++i)ref.process16(in.data(),oa.data());
        auto t1=std::chrono::steady_clock::now();
        for(int i=0;i<N;++i)nat.process16(in.data(),ob.data());
        auto t2=std::chrono::steady_clock::now();
        const double dr=std::chrono::duration<double>(t1-t0).count();
        const double dn=std::chrono::duration<double>(t2-t1).count();
        std::printf("bench ref: %.1f ns/block | native: %.1f ns/block | speedup x%.2f\n",
                    dr/N*1e9,dn/N*1e9,dr/dn);
    }
    return mismatch==0&&stateDiffs==0?0:1;
}
