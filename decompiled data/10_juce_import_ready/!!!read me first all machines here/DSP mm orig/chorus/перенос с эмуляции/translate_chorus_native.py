#!/usr/bin/env python3
# 1.8.3: транслирует nova::ChorusCore -> nova::NativeChorusCore.
# Тела процедур (setParameters/reset/initialise/process) переносятся ДОСЛОВНО,
# заменяются только вызовы доступа к памяти: readX/...->RX/... (инлайн в хедере).
# Арифметика (q/word/mac/multiply/offset/post) копируется как есть -> бит-точность.
import io, re, sys

SRC='Monomachine_Nova_Synth/Source/dsp/ChorusCore.cpp'
H='Monomachine_Nova_Synth/Source/dsp/ChorusCore.h'
OUT_CPP='Monomachine_Nova_Synth/Source/dsp/ChorusNative.cpp'
OUT_H='Monomachine_Nova_Synth/Source/dsp/ChorusNative.h'

src=io.open(SRC,encoding='utf-8').read()
def span(start,end=None):
    i=src.index(start)
    j=src.index(end) if end else len(src)
    return src[i:j]

def translate(body):
    body=body.replace('ChorusCore::','NativeChorusCore::')
    for a,b in [('readX(','RX('),('readY(','RY('),('writeX(','WX('),('writeY(','WY('),
                ('readLong(','RL('),('writeLong(','WL(')]:
        body=body.replace(a,b)
    # защита: пост-условие -- не осталось вызовов старых имён вне комментариев
    for bad in ['readX(','readY(','writeX(','writeY(','readLong(','writeLong(']:
        for line in body.split('\n'):
            code=line.split('//')[0]
            assert bad not in code,(bad,line[:100])
    return body

part_ab=translate(span('void ChorusCore::setParameters','void ChorusCore::initialiseProcedure'))
part_b=translate(span('void ChorusCore::initialiseProcedure','void ChorusCore::processProcedure'))
part_c=translate(span('void ChorusCore::processProcedure'))

header=io.open(H,encoding='utf-8').read()
# --- собираем ChorusNative.h из ChorusCore.h
nh=header.replace('ChorusCore','NativeChorusCore')
nh=nh.replace('// Native, statically translated CHORUS procedure from OS 1.32B.',
 '// 1.8.3: NATIVE CHORUS -- та же расшифровка OS 1.32B, доступ к памяти инлайн\n// (без out-of-line вызовов и assert в релизе). Тела процедур -- дословно из\n// ChorusCore.cpp, арифметика идентична -> бит-точность; стенд ChorusAudit.\n// Native, statically translated CHORUS procedure from OS 1.32B.')
# объявление четырёх out-of-line аксессоров -> инлайн-определения в классе
old_decl=''' // Test access uses the original data addresses, not executable P-memory.
 Word readX(Word address) const noexcept;
 Word readY(Word address) const noexcept;
 void writeX(Word address, Word value) noexcept;
 void writeY(Word address, Word value) noexcept;'''
assert nh.count(old_decl)==1
new_decl=''' friend struct ChorusAudit; // 1.8.3: белый ящик для стенда бит-точности
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
 void writeY(Word a,Word v) noexcept { WY(a,v); }'''
nh=nh.replace(old_decl,new_decl,1)
# readLong/writeLong -> через RX/WX (имена readLong/writeLong сохранены)
old_long=''' std::int64_t readLong(Word address) const noexcept { return std::int64_t(signed24(readX(address)))*one+readY(address); }
 void writeLong(Word address,std::int64_t value) noexcept { value=clip48(value);writeX(address,high(value));writeY(address,low(value)); }'''
assert nh.count(old_long)==1
nh=nh.replace(old_long,''' std::int64_t RL(Word address) const noexcept { return std::int64_t(signed24(RX(address)))*one+RY(address); }
 void WL(Word address,std::int64_t value) noexcept { value=clip48(value);WX(address,high(value));WY(address,low(value)); }''',1)
# x/y member-массивы: заполняются нулями, layout тот же
io.open(OUT_H,'w',encoding='utf-8').write(nh)

cpp='''// 1.8.3: NativeChorusCore -- тела процедур перенесены дословно из ChorusCore.cpp
// (транслировано из OS 1.32B); заменены только обращения к памяти (RX/RY/WX/WY/RL/WL).
// Бит-точность доказывается стендом ChorusAudit (старое ядро против нового).
#include "ChorusNative.h"
#include "ChorusTables.h"
namespace nova {
'''+part_ab+part_b+part_c
io.open(OUT_CPP,'w',encoding='utf-8').write(cpp)
print('written',OUT_H,OUT_CPP)
print('cpp lines:',cpp.count('\n'))
