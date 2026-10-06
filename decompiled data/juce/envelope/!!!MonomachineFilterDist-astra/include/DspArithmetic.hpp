// DSP56300 arithmetic subset used by the selected Monomachine routines.
// Semantics checked against dsp56300 c051afad, dsp_ops_alu.inl,
// dsp_decode.inl, types.h and dsp.h. No instruction decoder or DSP dependency.
// See THIRD_PARTY_NOTICES.md for source provenance and license boundaries.
#pragma once
#include <array>
#include <algorithm>
#include <cstdint>
#include "FirmwareTables.hpp"

namespace mnm132::detail {
enum class R { a,b,a0,a1,a2,b0,b1,b2,x0,x1,y0,y1,x,y,ab,ba,
    r0,r1,r2,r3,r4,r5,r6,r7,n0,n1,n2,n3,n4,n5,n6,n7,
    m0,m1,m2,m3,m4,m5,m6,m7,sr };
enum class C { eq,ne,cc,cs,ec,es,mi,pl,ge,gt,lt,le };
struct State {
    static constexpr uint64_t mask56=0x00ffffffffffffffULL;
    static constexpr uint64_t mask48=0x0000ffffffffffffULL;
    static constexpr uint32_t mask24=0x00ffffffu;
    std::array<uint32_t,2048> X{},Y{};
    std::array<uint32_t,8> r{},n{},m{};
    std::array<int64_t,2> acc{};
    uint32_t x0=0,x1=0,y0=0,y1=0,sr=0x080300;
    bool c=false,v=false,z=false,negative=false,e=false,fault=false;
    State() noexcept { m.fill(mask24); }
    static int32_t sign24(uint32_t w) noexcept {
        w &= mask24; return w<0x800000u ? int32_t(w) : int32_t(w)-0x1000000;
    }
    static int64_t sign56(uint64_t w) noexcept {
        w &= mask56; return w<(uint64_t(1)<<55) ? int64_t(w) : int64_t(w)-int64_t(uint64_t(1)<<56);
    }
    static int64_t sign48(uint64_t w) noexcept {
        w &= mask48; return w<(uint64_t(1)<<47) ? int64_t(w) : int64_t(w)-int64_t(uint64_t(1)<<48);
    }
    static int64_t extendWord(uint32_t w) noexcept { return int64_t(sign24(w))*0x1000000LL; }
    static int64_t asr64(int64_t x,unsigned n) noexcept {
        if (!n) return x;
        return x>=0 ? int64_t(uint64_t(x)>>n) : -1-int64_t(uint64_t(-(x+1))>>n);
    }
    uint32_t limited(unsigned a) noexcept {
        int64_t x=acc[a];
        if (sr&0x800u) x*=2; else if(sr&0x400u) x=asr64(x,1);
        if(x>0x7fffff000000LL) return 0x7fffffu;
        if(x< -0x800000000000LL) return 0x800000u;
        return uint32_t(uint64_t(x)>>24)&mask24;
    }
    uint32_t word(R reg) noexcept {
        const auto k=unsigned(reg);
        if(k>=unsigned(R::r0)&&k<=unsigned(R::r7))return r[k-unsigned(R::r0)];
        if(k>=unsigned(R::n0)&&k<=unsigned(R::n7))return n[k-unsigned(R::n0)];
        if(k>=unsigned(R::m0)&&k<=unsigned(R::m7))return m[k-unsigned(R::m0)];
        switch(reg){
        case R::a:return limited(0);case R::b:return limited(1);
        case R::a0:return uint32_t(acc[0])&mask24;case R::b0:return uint32_t(acc[1])&mask24;
        case R::a1:return uint32_t(uint64_t(acc[0])>>24)&mask24;case R::b1:return uint32_t(uint64_t(acc[1])>>24)&mask24;
        case R::a2:return uint32_t(uint64_t(acc[0])>>48)&255u;case R::b2:return uint32_t(uint64_t(acc[1])>>48)&255u;
        case R::x0:return x0;case R::x1:return x1;case R::y0:return y0;case R::y1:return y1;
        case R::sr:return (sr&~0x3fu)|uint32_t(c)|(uint32_t(v)<<1)|(uint32_t(z)<<2)|(uint32_t(negative)<<3)|(uint32_t(e)<<5);
        default:fault=true;return 0;
        }
    }
    void setWord(R reg,uint32_t w) noexcept {
        w&=mask24;const auto k=unsigned(reg);
        if(k>=unsigned(R::r0)&&k<=unsigned(R::r7)){r[k-unsigned(R::r0)]=w;return;}
        if(k>=unsigned(R::n0)&&k<=unsigned(R::n7)){n[k-unsigned(R::n0)]=w;return;}
        if(k>=unsigned(R::m0)&&k<=unsigned(R::m7)){m[k-unsigned(R::m0)]=w;return;}
        switch(reg){
        case R::a:acc[0]=extendWord(w);break;case R::b:acc[1]=extendWord(w);break;
        case R::a0:case R::b0:{auto a=reg==R::b0;acc[a]=sign56((uint64_t(acc[a])&~uint64_t(mask24))|w);break;}
        case R::a1:case R::b1:{auto a=reg==R::b1;acc[a]=sign56((uint64_t(acc[a])&~(uint64_t(mask24)<<24))|(uint64_t(w)<<24));break;}
        case R::a2:case R::b2:{auto a=reg==R::b2;acc[a]=sign56((uint64_t(acc[a])&mask48)|(uint64_t(w&255u)<<48));break;}
        case R::x0:x0=w;break;case R::x1:x1=w;break;case R::y0:y0=w;break;case R::y1:y1=w;break;
        case R::sr:sr=w;c=w&1;v=w&2;z=w&4;negative=w&8;e=w&32;break;
        default:fault=true;break;
        }
    }
    uint64_t longReg(R reg) noexcept {
        switch(reg){
        case R::a:return uint64_t(acc[0])&mask48;case R::b:return uint64_t(acc[1])&mask48;
        case R::x:return (uint64_t(x1)<<24)|x0;case R::y:return (uint64_t(y1)<<24)|y0;
        case R::ab:return (uint64_t(limited(0))<<24)|limited(1);
        case R::ba:return (uint64_t(limited(1))<<24)|limited(0);
        default:fault=true;return 0;
        }
    }
    void setLongReg(R reg,uint64_t w) noexcept {
        const uint32_t h=uint32_t(w>>24)&mask24,l=uint32_t(w)&mask24;
        switch(reg){
        case R::a:acc[0]=sign48(w);break;case R::b:acc[1]=sign48(w);break;
        case R::x:x1=h;x0=l;break;case R::y:y1=h;y0=l;break;
        case R::ab:acc[0]=extendWord(h);acc[1]=extendWord(l);break;
        case R::ba:acc[1]=extendWord(h);acc[0]=extendWord(l);break;
        default:fault=true;break;
        }
    }
    uint32_t readX(uint32_t a) noexcept {
        if(a<X.size())return X[a];
        if(a>=tableBase && a-tableBase<tables.size())return tables[a-tableBase];
        fault=true;return 0;
    }
    uint32_t readY(uint32_t a) noexcept {
        if(a<Y.size())return Y[a];
        if(a>=tableBase && a-tableBase<tables.size())return tables[a-tableBase];
        fault=true;return 0;
    }
    uint64_t readLong(uint32_t a) noexcept {return (uint64_t(readX(a))<<24)|readY(a);}
    void writeX(uint32_t a,uint32_t w) noexcept {if(a<X.size())X[a]=w&mask24;else fault=true;}
    void writeY(uint32_t a,uint32_t w) noexcept {if(a<Y.size())Y[a]=w&mask24;else fault=true;}
    void writeLong(uint32_t a,uint64_t w) noexcept {writeX(a,uint32_t(w>>24));writeY(a,uint32_t(w));}
    uint32_t post(unsigned i,int32_t delta) noexcept {
        uint32_t old=r[i];
        // Selected functions use linear addressing only. Fail closed if changed.
        if(m[i]!=mask24){fault=true;return old;}
        r[i]=(r[i]+uint32_t(delta))&mask24;return old;
    }
    uint32_t pre(unsigned i,int32_t delta) noexcept {post(i,delta);return r[i];}
    void flags(int64_t x) noexcept {
        x=sign56(uint64_t(x));negative=x<0;z=x==0;
        const unsigned lo=(sr&0x800u)?46u:(sr&0x400u)?48u:47u;
        const uint64_t bits=(uint64_t(x)&mask56)>>lo;
        e=bits!=0 && bits!=((uint64_t(1)<<(56-lo))-1);
    }
    bool condition(C q) const noexcept {
        switch(q){case C::eq:return z;case C::ne:return !z;case C::cc:return !c;case C::cs:return c;
        case C::ec:return !e;case C::es:return e;case C::mi:return negative;case C::pl:return !negative;
        case C::ge:return negative==v;case C::gt:return negative==v&&!z;
        case C::lt:return negative!=v;case C::le:return negative!=v||z;}return false;
    }
    void add(unsigned d,int64_t val) noexcept {
        uint64_t res=(uint64_t(acc[d])&mask56)+(uint64_t(val)&mask56);
        c=res>mask56;v=false;acc[d]=sign56(res);flags(acc[d]);
    }
    void sub(unsigned d,int64_t val) noexcept {
        const auto lhs=uint64_t(acc[d])&mask56,rhs=uint64_t(val)&mask56;
        c=lhs<rhs;v=false;acc[d]=sign56(lhs-rhs);flags(acc[d]);
    }
    void cmp(unsigned d,int64_t val) noexcept {const auto old=acc[d];sub(d,val);acc[d]=old;}
    void subr(unsigned d,int64_t val) noexcept {
        const auto old=acc[d];acc[d]=sign56(uint64_t(asr64(old,1)-val));
        c=(old<0)!=(acc[d]<0);flags(acc[d]);
    }
    void shift(unsigned d,unsigned src,unsigned amount,bool left) noexcept {
        const auto old=acc[src];
        if(left){
            c=amount && ((uint64_t(old)>>(56-amount))&1u);
            const auto top=(uint64_t(old)&mask56)>>(55-amount);
            v=top!=0 && top!=((uint64_t(1)<<(amount+1))-1);
            acc[d]=sign56(uint64_t(old)<<amount);
        }else{c=amount&&((uint64_t(old)>>(amount-1))&1u);v=false;acc[d]=asr64(old,amount);}
        flags(acc[d]);
    }
    void abs(unsigned d) noexcept {if(acc[d]<0)acc[d]=sign56(uint64_t(-acc[d]));flags(acc[d]);}
    void clr(unsigned d) noexcept {acc[d]=0;v=false;flags(0);}
    void tst(unsigned d) noexcept {v=false;flags(acc[d]);}
    void rnd(unsigned d) noexcept {
        uint64_t rounder=0x800000;
        if(sr&0x800u)rounder>>=1;else if(sr&0x400u)rounder<<=1;
        uint64_t val=uint64_t(acc[d])+rounder;
        const uint64_t mask=(rounder<<1)-1;
        if(!(sr&0x200000u) && !(val&mask)) val&=~(rounder<<1);
        acc[d]=sign56(val&~mask);v=false;flags(acc[d]);
    }
    void multiply(unsigned d,uint32_t a,uint32_t b,unsigned kind,bool negate) noexcept {
        const int64_t lhs=sign24(a),rhs=(kind==2||kind==3)?int64_t(b&mask24):sign24(b);
        int64_t res=lhs*rhs*2;
        if(negate)res=-res;
        if(kind==1||kind==3)res+=acc[d];else if(kind==4)res+=asr64(acc[d],24);
        acc[d]=sign56(uint64_t(res));v=res!=acc[d];flags(acc[d]);
    }
    void divide(unsigned d,uint32_t src) noexcept {
        // One DIV step, matching DSP56300 op_Div: old carry is shifted in.
        const auto old=acc[d];const bool unlike=(old<0)!=(sign24(src)<0);
        const uint64_t shifted=((uint64_t(old)&mask56)<<1)|uint64_t(c);
        const bool shiftedSign=((shifted>>55)&1u)!=0;
        const int64_t term=extendWord(src);
        const uint64_t res=unlike?shifted+uint64_t(term):shifted-uint64_t(term);
        acc[d]=sign56(res);c=acc[d]>=0;v=shiftedSign!=(old<0);
        // DIV changes C/V/L only. In particular it does not refresh N/Z/E.
    }
    void bit(R reg,unsigned bitNumber,int change) noexcept {
        // BTST on a/b addresses A1/B1, not the limited accumulator transfer.
        const auto actual=reg==R::a?R::a1:reg==R::b?R::b1:reg;
        uint32_t val=word(actual);c=((val>>bitNumber)&1u)!=0;
        if(change){val=change>0?(val|(1u<<bitNumber)):(val&~(1u<<bitNumber));
            if(actual==R::sr)sr=val;else setWord(actual,val);}
    }
};
}
