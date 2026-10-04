#pragma once
#include <array>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include "ChorusTables.h"

namespace nova {
// 1.8.3: NATIVE CHORUS -- та же расшифровка OS 1.32B, доступ к памяти инлайн
// (без out-of-line вызовов и assert в релизе). Тела процедур -- дословно из
// ChorusCore.cpp, арифметика идентична -> бит-точность; стенд ChorusAudit.
// Native, statically translated CHORUS procedure from OS 1.32B.
// No instruction decoder, program counter, JIT, firmware loader or emulator.
// Original 16-frame schedule and 24/48/56-bit arithmetic are retained.
class alignas(32) NativeChorusCore {
public:
 using Word=std::uint32_t;
 static constexpr std::array<int,8> defaults{{64,64,64,127,0,0,127,64}};
 NativeChorusCore() { reset(defaults); }
 void reset(const std::array<int,8>& parameters) noexcept;
 void setParameters(const std::array<int,8>& parameters) noexcept;
 bool auditNoSkip=false; // 1.8.3: стенд -- бит-точность самого процессинга без тихого пути
 // 1.8.3: тихий путь (АВТОМАТИЧЕСКИ, без галки). Пока хвост не затух -- полный
 // процессинг. Когда вход и выход длинно нулевые, делаем пробу: снапшот
 // звучащей памяти, полный процессинг, сравнение. Если процедура на тишине не
 // изменила НИЧЕГО вне свободно бегущих мод-зон (осциллятор, рамп, счётчики)
 // -- неподвижная точка: дальше достаточно гонять мод-секцию дословно
 // (quietAdvance), выдавая нули; хвосты к этому моменту честно затухли,
 // состояние при возврате из тишины бит-в-байт. Любой ненулевой вход или
 // реальное изменение параметров мгновенно возвращает полный процессинг.
 // Совпадение доказывается стендом ChorusAudit (уход в тишину и возврат).
 // 1.8.3c: порог 64 (~46 мс после бит-тишины выхода); проба каждые 16 блоков.
 bool latched=false;
 int quietRun=0; // подряд тихие блоки с нулевым выходом (дешёвый гейт пробы)
 long quietCount=0; // диагностика: блоков прошло тихим путём
 Word snapX[0x580]{}; Word snapY[0x580]{}; Word snapD[4096]{}; // снапшот пробного блока
 void quietAdvance() noexcept; // мод-секция процедуры -- дословно
 void process16(const std::int32_t* interleavedInput,std::int32_t* interleavedOutput) noexcept {
  bool inz=true;for(int i=0;i<32;++i)if(interleavedInput[i]!=0){inz=false;break;}
  if(!auditNoSkip){
   if(!inz){quietRun=0;latched=false;}
   else if(latched){quietAdvance();++quietCount;for(unsigned i=0;i<32;++i)interleavedOutput[i]=0;return;}
  }
  const bool probe=!auditNoSkip&&inz&&!latched&&quietRun>=64&&((quietRun&15)==0); // 1.8.3c: 64 блоков (~46 мс) вместо 192 -- защёлка ловит короткие паузы; точность гарантирует проба, не порог
  if(probe){for(unsigned i=0;i<0x580;++i){snapX[i]=x[i];snapY[i]=y[i];}for(unsigned i=0;i<4096;++i)snapD[i]=delay[i];}
  for(unsigned i=0;i<32;++i)x[0x220+i]=Word(interleavedInput[i])&0xffffffu;
  processProcedure();
  for(unsigned i=0;i<32;++i)interleavedOutput[i]=signed24(y[0x100+i]);
  if(!auditNoSkip){
   bool outz=true;for(unsigned i=0x100;i<0x120;++i)if(y[i]!=0){outz=false;break;}
   if(inz&&outz)++quietRun; // 1.8.3c: без капа -- после порога проба каждые 16 блоков (кап делал пробу КАЖДЫЙ блок: снапшот зря)
   else quietRun=0;
   if(probe){
    bool fixedpt=true;
    for(unsigned i=0x000;i<0x020;++i)if(x[i]!=snapX[i]||y[i]!=snapY[i]){fixedpt=false;break;} // скрэтч
    if(fixedpt)for(unsigned i=0x090;i<0x520;++i)if(x[i]!=snapX[i]){fixedpt=false;break;}      // x: мод-зоны [0x20..0x90) и осц.[0x520..0x580) свободны
    if(fixedpt)for(unsigned i=0x0a0;i<0x520;++i)if(y[i]!=snapY[i]){fixedpt=false;break;}      // y: мод-зоны [0x20..0xa0) и осц.[0x520..0x580) свободны
    if(fixedpt)for(unsigned i=0;i<4096;++i)if(delay[i]!=snapD[i]){fixedpt=false;break;}       // линия задержки
    if(fixedpt)latched=true;
   }
  }
 }
 friend struct ChorusAudit; // 1.8.3: белый ящик для стенда бит-точности
 static std::int32_t signed24(Word v) noexcept { v&=0xffffffu; return v&0x800000u ? std::int32_t(v)-0x1000000 : std::int32_t(v); }
 friend struct ChorusAudit; // 1.8.3: белый ящик для стенда бит-точности
 // 1.8.3: доступ к памяти -- ИНЛАЙН (та же диспетчеризация, без вызова функции;
 // assert остаётся только в аудите/debug: релиз JUCE собран с NDEBUG).
 Word RX(Word a) const noexcept {
  if(a<0x580u)return x[a];
  if(a>=0x114000u&&a<0x115000u)return delay[a-0x114000u];
  if(a>=0x14a000u&&a<0x14b000u)return widthTable[a-0x14a000u];
  if(a>=0x144ac7u&&a<0x144bc9u)return lowpassTable[a-0x144ac7u];
  assert(false && "Unexpected CHORUS X address");return 0;}
 Word RY(Word a) const noexcept { return a<0x580u?y[a]:RX(a); }
 void WX(Word a,Word v) noexcept {
  v&=0xffffffu;
  if(a<0x580u){x[a]=v;return;}
  if(a>=0x114000u&&a<0x115000u){delay[a-0x114000u]=v;return;}
  assert(false && "Unexpected CHORUS X write");}
 void WY(Word a,Word v) noexcept { if(a<0x580u){y[a]=v&0xffffffu;return;}WX(a,v); }
 Word readX(Word a) const noexcept { return RX(a); } // стенд: доступ по адресам
 Word readY(Word a) const noexcept { return RY(a); }
 void writeX(Word a,Word v) noexcept { WX(a,v); }
 void writeY(Word a,Word v) noexcept { WY(a,v); }
private:
 std::array<Word,0x580> x{};
 std::array<Word,0x580> y{};
 std::array<Word,4096> delay{};
 void initialiseProcedure() noexcept;
 void processProcedure() noexcept;
 static constexpr std::int64_t one=0x1000000ll;
 static std::int64_t q(Word v) noexcept { return std::int64_t(signed24(v))*one; }
 static std::int64_t asr(std::int64_t v,unsigned n) noexcept {
  const auto divisor=std::int64_t(1)<<n;
  return v>=0?v/divisor:-((-v+divisor-1)/divisor);
 }
 // floor-деление == арифметический сдвиг (эквивалентность доказана стендом ChorusAudit)
 static std::int64_t asrN(std::int64_t v,unsigned n) noexcept { return v>>n; }
 static std::int64_t wrap56(std::int64_t v) noexcept { // 1.8.3: branchless (XOR-знак), семантика та же
  return std::int64_t((std::uint64_t(v)&0xffffffffffffffull)^0x80000000000000ull)-0x80000000000000ll;
 }
 static std::int64_t asl(std::int64_t v,unsigned n) noexcept { return wrap56(std::int64_t((std::uint64_t(v)<<n)&0xffffffffffffffull)); }
 static std::int64_t clip48(std::int64_t v) noexcept { return std::clamp<std::int64_t>(v,-0x800000000000ll,0x7fffffffffffll); }
 static Word word(std::int64_t v) noexcept { return Word(asr(clip48(v),24))&0xffffffu; }
 static Word high(std::int64_t v) noexcept { return Word(std::uint64_t(v)>>24)&0xffffffu; }
 static Word low(std::int64_t v) noexcept { return Word(v)&0xffffffu; }
 static std::int64_t withLow(std::int64_t a,Word v) noexcept { return wrap56(std::int64_t((std::uint64_t(a)&0xffffffff000000ull)|(v&0xffffffu))); }
 static std::int64_t multiply(Word a,Word b,bool unsignedB=false) noexcept { return std::int64_t(signed24(a))*(unsignedB?std::int64_t(b&0xffffffu):std::int64_t(signed24(b)))*2; }
 static std::int64_t mac(std::int64_t a,Word b,Word c,bool negative,bool saturate,bool unsignedC=false) noexcept {
  const auto v=a+(negative?-1:1)*multiply(b,c,unsignedC);
  return saturate?clip48(v):wrap56(v);
 }
 static Word offset(Word address,Word displacement,Word modulo) noexcept {
  const auto d=signed24(displacement);
  if(modulo==0xffffffu) return Word(std::int64_t(address)+d)&0xffffffu;
  Word mask=1;while(mask<=modulo)mask<<=1;--mask;
  const auto base=address&~mask;
  auto pos=std::int64_t(address&mask)+d;
  const auto size=std::int64_t(modulo)+1;
  pos%=size;if(pos<0)pos+=size;
  return base+Word(pos);
 }
 static Word post(Word& address,Word displacement,Word modulo) noexcept { auto old=address;address=offset(address,displacement,modulo);return old; }
 std::int64_t RL(Word address) const noexcept { return std::int64_t(signed24(RX(address)))*one+RY(address); }
 void WL(Word address,std::int64_t value) noexcept { value=clip48(value);WX(address,high(value));WY(address,low(value)); }
};
}
