// Instruction-level native C++ translation; do not edit generated code.
#pragma once
#include "DspArithmetic.hpp"
namespace mnm132::detail {
inline void filterEnvelope(State&) noexcept;
inline void filterAudio(State&) noexcept;
inline void f350(State&) noexcept;
inline void f365(State&) noexcept;
inline void f37c(State&) noexcept;
inline void f38a(State&) noexcept;
inline void filterEnvelope(State& s) noexcept
{
    // P:0004FF  move y:(r6+$20),a  [0286BE]
    {
        const auto e0 = ((s.r[6] + 0x20u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:000500  cmp #<$1,a  [014185]
    {
        s.cmp(0, State::extendWord(0x1u));
    }
    // P:000501  bne func_000506  [052405]
    if (s.condition(C::ne)) goto L000506;
    // P:000502  move #$0,x0  [240000]
    {
        const auto v0 = 0x0u;
        s.setWord(R::x0, v0);
    }
    // P:000503  move x0,y:(r7-$1)  [03FFE4]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = ((s.r[7] - 0x1u) & 0xffffffu);
        s.writeY(d0, v0);
    }
    // P:000504  move x0,y:(r6+$20)  [0286A4]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = ((s.r[6] + 0x20u) & 0xffffffu);
        s.writeY(d0, v0);
    }
    // P:000505  move x0,x:(r7-$1)  [03FFC4]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = ((s.r[7] - 0x1u) & 0xffffffu);
        s.writeX(d0, v0);
    }
L000506:;
    // P:000506  move y:(r7-$1),a  [03FFFE]
    {
        const auto e0 = ((s.r[7] - 0x1u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:000507  tst a  [200003]
    {
        s.tst(0);
    }
    // P:000508  bne func_000517  [05240F]
    if (s.condition(C::ne)) goto L000517;
    // P:000509  move y:(r6+$c),a  [0236BE]
    {
        const auto e0 = ((s.r[6] + 0xcu) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:00050A  asr #$10,a,a  [0C1C20]
    {
        s.shift(0, 0, 16, false);
    }
    // P:00050B  move x:(r7-$1),b  [03FFDF]
    {
        const auto e0 = ((s.r[7] - 0x1u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::b, v0);
    }
    // P:00050C  move a,r4  [21D400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::r4, v0);
    }
    // P:00050D  move y:(r4+$141800),y0  [0B74C6 141800]
    {
        const auto e0 = ((s.r[4] + 0x141800u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::y0, v0);
    }
    // P:00050F  add y0,b  [200058]
    {
        s.add(1, State::extendWord(s.word(R::y0)));
    }
    // P:000510  bec func_000515  [055405]
    if (s.condition(C::ec)) goto L000515;
    // P:000511  move #>$1,x0  [44F400 000001]
    {
        const auto v0 = 0x1u;
        s.setWord(R::x0, v0);
    }
    // P:000513  move x0,y:(r7-$1)  [03FFE4]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = ((s.r[7] - 0x1u) & 0xffffffu);
        s.writeY(d0, v0);
    }
    // P:000514  move x0,x:(r7-$2)  [03FF84]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = ((s.r[7] - 0x2u) & 0xffffffu);
        s.writeX(d0, v0);
    }
L000515:;
    // P:000515  move b,x:(r7-$1)  [03FFCF]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = ((s.r[7] - 0x1u) & 0xffffffu);
        s.writeX(d0, v0);
    }
    // P:000516  bra func_000537  [050C41]
    goto L000537;
L000517:;
    // P:000517  cmp #<$1,a  [014185]
    {
        s.cmp(0, State::extendWord(0x1u));
    }
    // P:000518  bne func_00052b  [052413]
    if (s.condition(C::ne)) goto L00052b;
    // P:000519  clr a  [200013]
    {
        s.clr(0);
    }
    // P:00051A  move y:(r6+$23),y0  [028EF6]
    {
        const auto e0 = ((s.r[6] + 0x23u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::y0, v0);
    }
    // P:00051B  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:00051C  mpy y0,x0,b  [2000D8]
    {
        s.multiply(1, s.word(R::y0), s.word(R::x0), 0, false);
    }
    // P:00051D  move x:(r7-$2),a  [03FF9E]
    {
        const auto e0 = ((s.r[7] - 0x2u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::a, v0);
    }
    // P:00051E  move b,x1  [21E500]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::x1, v0);
    }
    // P:00051F  mpyi #>$791fd0,x1,b  [0141E8 791FD0]
    {
        s.multiply(1, 0x791fd0u, s.word(R::x1), 0, false);
    }
    // P:000521  asl b  [20003A]
    {
        s.shift(1, 1, 1, true);
    }
    // P:000522  add #<$1,a  [014180]
    {
        s.add(0, State::extendWord(0x1u));
    }
    // P:000523  cmp b,a  [200005]
    {
        s.cmp(0, s.acc[true]);
    }
    // P:000524  blt func_000529  [059405]
    if (s.condition(C::lt)) goto L000529;
    // P:000525  move #>$2,x0  [44F400 000002]
    {
        const auto v0 = 0x2u;
        s.setWord(R::x0, v0);
    }
    // P:000527  move x0,y:(r7-$1)  [03FFE4]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = ((s.r[7] - 0x1u) & 0xffffffu);
        s.writeY(d0, v0);
    }
    // P:000528  bra func_00052b  [050C03]
    goto L00052b;
L000529:;
    // P:000529  move a,x:(r7-$2)  [03FF8E]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = ((s.r[7] - 0x2u) & 0xffffffu);
        s.writeX(d0, v0);
    }
    // P:00052A  bra func_000537  [050C0D]
    goto L000537;
L00052b:;
    // P:00052B  move y:(r6+$d),a  [0236FE]
    {
        const auto e0 = ((s.r[6] + 0xdu) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:00052C  add #>$7fff,a  [0140C0 007FFF]
    {
        s.add(0, State::extendWord(0x7fffu));
    }
    // P:00052E  asr #$10,a,a  [0C1C20]
    {
        s.shift(0, 0, 16, false);
    }
    // P:00052F  rnd a  [200011]
    {
        s.rnd(0);
    }
    // P:000530  move a,r4  [21D400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::r4, v0);
    }
    // P:000531  move x:(r7-$1),b  [03FFDF]
    {
        const auto e0 = ((s.r[7] - 0x1u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::b, v0);
    }
    // P:000532  move y:(r4+$141a00),y0  [0B74C6 141A00]
    {
        const auto e0 = ((s.r[4] + 0x141a00u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::y0, v0);
    }
    // P:000534  sub y0,b  [20005C]
    {
        s.sub(1, State::extendWord(s.word(R::y0)));
    }
    // P:000535  clr b ifmi  [202B1B]
    {
        if (s.condition(C::mi)) { s.clr(1); }
    }
    // P:000536  move b,x:(r7-$1)  [03FFCF]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = ((s.r[7] - 0x1u) & 0xffffffu);
        s.writeX(d0, v0);
    }
L000537:;
    // P:000537  move y:(r6+$8),x0  [0226B4]
    {
        const auto e0 = ((s.r[6] + 0x8u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::x0, v0);
    }
    // P:000538  mpyi #>$800,x0,a  [0141C0 000800]
    {
        s.multiply(0, 0x800u, s.word(R::x0), 0, false);
    }
    // P:00053A  sub #>$80,a  [0140C4 000080]
    {
        s.sub(0, State::extendWord(0x80u));
    }
    // P:00053C  rnd a  [200011]
    {
        s.rnd(0);
    }
    // P:00053D  tfr a,b  [200009]
    {
        s.acc[1] = s.acc[false];
    }
    // P:00053E  move y:(r6+$e),a  [023EBE]
    {
        const auto e0 = ((s.r[6] + 0xeu) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:00053F  add #>$c00000,a  [0140C0 C00000]
    {
        s.add(0, State::extendWord(0xc00000u));
    }
    // P:000541  abs a a,y0  [21C626]
    {
        const auto v0 = s.word(R::a);
        s.abs(0);
        s.setWord(R::y0, v0);
    }
    // P:000542  move #>$700,x1  [45F400 000700]
    {
        const auto v0 = 0x700u;
        s.setWord(R::x1, v0);
    }
    // P:000544  move a,y1  [21C700]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y1, v0);
    }
    // P:000545  mpy y1,y0,a  [2000B0]
    {
        s.multiply(0, s.word(R::y1), s.word(R::y0), 0, false);
    }
    // P:000546  asl #$2,a,a  [0C1D04]
    {
        s.shift(0, 0, 2, true);
    }
    // P:000547  move x:(r7-$1),y1  [03FFD7]
    {
        const auto e0 = ((s.r[7] - 0x1u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:000548  move a,y0  [21C600]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y0, v0);
    }
    // P:000549  mpy y1,y0,a  [2000B0]
    {
        s.multiply(0, s.word(R::y1), s.word(R::y0), 0, false);
    }
    // P:00054A  move a,y1  [21C700]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y1, v0);
    }
    // P:00054B  mpy y1,x1,a  [2000F0]
    {
        s.multiply(0, s.word(R::y1), s.word(R::x1), 0, false);
    }
    // P:00054C  add b,a  [200010]
    {
        s.add(0, s.acc[true]);
    }
    // P:00054D  cmp x1,a  [200065]
    {
        s.cmp(0, State::extendWord(s.word(R::x1)));
    }
    // P:00054E  tfr x1,a ifge  [202161]
    {
        if (s.condition(C::ge)) { s.acc[0] = State::extendWord(s.word(R::x1)); }
    }
    // P:00054F  move #$8,y0  [260800]
    {
        const auto v0 = 0x80000u;
        s.setWord(R::y0, v0);
    }
    // P:000550  move x:(r6+$1),x1  [0206D5]
    {
        const auto e0 = ((s.r[6] + 0x1u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:000551  move y:(r6+$25),b  [0296FF]
    {
        const auto e0 = ((s.r[6] + 0x25u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::b, v0);
    }
    // P:000552  move a1,x:(r7-$3)  [03F7CC]
    {
        const auto v0 = s.word(R::a1);
        const auto d0 = ((s.r[7] - 0x3u) & 0xffffffu);
        s.writeX(d0, v0);
    }
    // P:000553  mac x1,y0,a  [2000E2]
    {
        s.multiply(0, s.word(R::x1), s.word(R::y0), 1, false);
    }
    // P:000554  btst #$b,b  [0BCF6B]
    s.bit(R::b, 11, 0);
    // P:000555  bcc func_000557  [050402]
    if (s.condition(C::cc)) goto L000557;
    // P:000556  move a1,x:(r7-$3)  [03F7CC]
    {
        const auto v0 = s.word(R::a1);
        const auto d0 = ((s.r[7] - 0x3u) & 0xffffffu);
        s.writeX(d0, v0);
    }
L000557:;
    // P:000557  move y:(r6+$9),x0  [0226F4]
    {
        const auto e0 = ((s.r[6] + 0x9u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::x0, v0);
    }
    // P:000558  btst #$9,b  [0BCF69]
    s.bit(R::b, 9, 0);
    // P:000559  mac -x1,y0,a ifcc  [2020E6]
    {
        if (s.condition(C::cc)) { s.multiply(0, s.word(R::x1), s.word(R::y0), 1, true); }
    }
    // P:00055A  maci #>$800,x0,a  [0141C2 000800]
    {
        s.multiply(0, 0x800u, s.word(R::x0), 1, false);
    }
    // P:00055C  move y:(r6+$f),b  [023EFF]
    {
        const auto e0 = ((s.r[6] + 0xfu) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::b, v0);
    }
    // P:00055D  add #>$c00000,b  [0140C8 C00000]
    {
        s.add(1, State::extendWord(0xc00000u));
    }
    // P:00055F  abs b b,y0  [21E62E]
    {
        const auto v0 = s.word(R::b);
        s.abs(1);
        s.setWord(R::y0, v0);
    }
    // P:000560  move #>$700,x1  [45F400 000700]
    {
        const auto v0 = 0x700u;
        s.setWord(R::x1, v0);
    }
    // P:000562  move b,y1  [21E700]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::y1, v0);
    }
    // P:000563  mpy y1,y0,b  [2000B8]
    {
        s.multiply(1, s.word(R::y1), s.word(R::y0), 0, false);
    }
    // P:000564  asl #$2,b,b  [0C1D85]
    {
        s.shift(1, 1, 2, true);
    }
    // P:000565  move x:(r7-$1),y1  [03FFD7]
    {
        const auto e0 = ((s.r[7] - 0x1u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:000566  tfr a,b b,y0  [21E609]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = s.acc[false];
        s.setWord(R::y0, v0);
    }
    // P:000567  mpy y1,y0,a  [2000B0]
    {
        s.multiply(0, s.word(R::y1), s.word(R::y0), 0, false);
    }
    // P:000568  move a,y1  [21C700]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y1, v0);
    }
    // P:000569  mac y1,x1,b  [2000FA]
    {
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, false);
    }
    // P:00056A  clr b ifmi  [202B1B]
    {
        if (s.condition(C::mi)) { s.clr(1); }
    }
    // P:00056B  move y:(r7-$3),a  [03F7FE]
    {
        const auto e0 = ((s.r[7] - 0x3u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:00056C  move b1,y:(r7-$2)  [03FFAD]
    {
        const auto v0 = s.word(R::b1);
        const auto d0 = ((s.r[7] - 0x2u) & 0xffffffu);
        s.writeY(d0, v0);
    }
}

inline void filterAudio(State& s) noexcept
{
    // P:0005BF  lua (r7-$18),r4  [043784]
    s.setWord(R::r4, ((s.r[7] - 0x18u) & 0xffffffu));
    // P:0005C0  move #>$fffffe,n4  [74F400 FFFFFE]
    {
        const auto v0 = 0xfffffeu;
        s.setWord(R::n4, v0);
    }
    // P:0005C2  move r4,r5  [229500]
    {
        const auto v0 = s.word(R::r4);
        s.setWord(R::r5, v0);
    }
    // P:0005C3  move #$91,r0  [309100]
    {
        const auto v0 = 0x91u;
        s.setWord(R::r0, v0);
    }
    // P:0005C4  move #$d1,r2  [32D100]
    {
        const auto v0 = 0xd1u;
        s.setWord(R::r2, v0);
    }
    // P:0005C5  move l:(r4)+,x  [42DC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:0005C6  move x0,y:(r0)+  [4C5800]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 1);
        s.writeY(d0, v0);
    }
    // P:0005C7  move x1,y:(r0)+  [4D5800]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(0, 1);
        s.writeY(d0, v0);
    }
    // P:0005C8  move l:(r4)+,x  [42DC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:0005C9  move x0,y:(r0)+  [4C5800]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 1);
        s.writeY(d0, v0);
    }
    // P:0005CA  move x1,y:(r2)+  [4D5A00]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(2, 1);
        s.writeY(d0, v0);
    }
    // P:0005CB  move l:(r4)+,x  [42DC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:0005CC  move x0,y:(r2)+  [4C5A00]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(2, 1);
        s.writeY(d0, v0);
    }
    // P:0005CD  move x1,y:(r2)+  [4D5A00]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(2, 1);
        s.writeY(d0, v0);
    }
    // P:0005CE  move #$91,r4  [349100]
    {
        const auto v0 = 0x91u;
        s.setWord(R::r4, v0);
    }
    // P:0005CF  move #>$f528bd,x0  [44F400 F528BD]
    {
        const auto v0 = 0xf528bdu;
        s.setWord(R::x0, v0);
    }
    // P:0005D1  move #>$4a4df0,x1  [45F400 4A4DF0]
    {
        const auto v0 = 0x4a4df0u;
        s.setWord(R::x1, v0);
    }
    // P:0005D3  move #$71,r1  [317100]
    {
        const auto v0 = 0x71u;
        s.setWord(R::r1, v0);
    }
    // P:0005D4  move #$73,r2  [327300]
    {
        const auto v0 = 0x73u;
        s.setWord(R::r2, v0);
    }
    // P:0005D5  move y:(r4)+,y0  [4EDC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::y0, v0);
    }
    // P:0005D6  move #$3,n1  [390300]
    {
        const auto v0 = 0x3u;
        s.setWord(R::n1, v0);
    }
    // P:0005D7  move n1,n2  [233A00]
    {
        const auto v0 = s.word(R::n1);
        s.setWord(R::n2, v0);
    }
    // P:0005D8  do #<$8,>$5e2  [060880 0005E1]
    for (unsigned loop_5d8=0; loop_5d8<8u; ++loop_5d8) {
    // P:0005DA  mpy y0,x0,a a,x:(r1)+n1 y:(r4)+,y1  [F909D0]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, State::sign24(s.n[1]));
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 0, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:0005DB  mpy x0,y1,b b,x:(r2)+n2  [574AC8]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(2, State::sign24(s.n[2]));
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
        s.writeX(d0, v0);
    }
    // P:0005DC  mac y1,x1,a y:(r4)+,y0  [4EDCF2]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.multiply(0, s.word(R::y1), s.word(R::x1), 1, false);
        s.setWord(R::y0, v0);
    }
    // P:0005DD  mac x1,y0,b y1,x:(r1)+  [4759EA]
    {
        const auto v0 = s.word(R::y1);
        const auto d0 = s.post(1, 1);
        s.multiply(1, s.word(R::x1), s.word(R::y0), 1, false);
        s.writeX(d0, v0);
    }
    // P:0005DE  mac x1,y0,a y:(r4)+,y1  [4FDCE2]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.multiply(0, s.word(R::x1), s.word(R::y0), 1, false);
        s.setWord(R::y1, v0);
    }
    // P:0005DF  mac y1,x1,b y0,x:(r2)+  [465AFA]
    {
        const auto v0 = s.word(R::y0);
        const auto d0 = s.post(2, 1);
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, false);
        s.writeX(d0, v0);
    }
    // P:0005E0  mac x0,y1,a y:(r4)+n4,y0  [4ECCC2]
    {
        const auto e0 = s.post(4, State::sign24(s.n[4]));
        const auto v0 = s.readY(e0);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::y0, v0);
    }
    // P:0005E1  mac y0,x0,b y:(r4)+,y0  [4EDCDA]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, false);
        s.setWord(R::y0, v0);
    }
    }
    // P:0005E2  move a,x:(r1)+n1  [564900]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, State::sign24(s.n[1]));
        s.writeX(d0, v0);
    }
    // P:0005E3  move b,x:(r2)+n2  [574A00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(2, State::sign24(s.n[2]));
        s.writeX(d0, v0);
    }
    // P:0005E4  move y:(r4)+,y1  [4FDC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::y1, v0);
    }
    // P:0005E5  move y,l:(r5)+  [435D00]
    {
        const auto v0 = s.longReg(R::y);
        const auto d0 = s.post(5, 1);
        s.writeLong(d0, v0);
    }
    // P:0005E6  move y:(r4)-,y1  [4FD400]
    {
        const auto e0 = s.post(4, -1);
        const auto v0 = s.readY(e0);
        s.setWord(R::y1, v0);
    }
    // P:0005E7  move y1,y:(r5)  [4F6500]
    {
        const auto v0 = s.word(R::y1);
        const auto d0 = s.post(5, 0);
        s.writeY(d0, v0);
    }
    // P:0005E8  move x:(r6+$f),y1  [023ED7]
    {
        const auto e0 = ((s.r[6] + 0xfu) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:0005E9  brset #$0,y1,func_0005fb  [0CC7A0 000012]
    if (((s.word(R::y1) >> 0) & 1u) == 1u) goto L0005fb;
    // P:0005EB  move #$d1,r4  [34D100]
    {
        const auto v0 = 0xd1u;
        s.setWord(R::r4, v0);
    }
    // P:0005EC  move #$b1,r1  [31B100]
    {
        const auto v0 = 0xb1u;
        s.setWord(R::r1, v0);
    }
    // P:0005ED  move #$b3,r2  [32B300]
    {
        const auto v0 = 0xb3u;
        s.setWord(R::r2, v0);
    }
    // P:0005EE  move y:(r4)+,y0  [4EDC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::y0, v0);
    }
    // P:0005EF  do #<$8,>$5f9  [060880 0005F8]
    for (unsigned loop_5ef=0; loop_5ef<8u; ++loop_5ef) {
    // P:0005F1  mpy y0,x0,a a,x:(r1)+n1 y:(r4)+,y1  [F909D0]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, State::sign24(s.n[1]));
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 0, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:0005F2  mpy x0,y1,b b,x:(r2)+n2  [574AC8]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(2, State::sign24(s.n[2]));
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
        s.writeX(d0, v0);
    }
    // P:0005F3  mac y1,x1,a y:(r4)+,y0  [4EDCF2]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.multiply(0, s.word(R::y1), s.word(R::x1), 1, false);
        s.setWord(R::y0, v0);
    }
    // P:0005F4  mac x1,y0,b y1,x:(r1)+  [4759EA]
    {
        const auto v0 = s.word(R::y1);
        const auto d0 = s.post(1, 1);
        s.multiply(1, s.word(R::x1), s.word(R::y0), 1, false);
        s.writeX(d0, v0);
    }
    // P:0005F5  mac x1,y0,a y:(r4)+,y1  [4FDCE2]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.multiply(0, s.word(R::x1), s.word(R::y0), 1, false);
        s.setWord(R::y1, v0);
    }
    // P:0005F6  mac y1,x1,b y0,x:(r2)+  [465AFA]
    {
        const auto v0 = s.word(R::y0);
        const auto d0 = s.post(2, 1);
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, false);
        s.writeX(d0, v0);
    }
    // P:0005F7  mac x0,y1,a y:(r4)+n4,y0  [4ECCC2]
    {
        const auto e0 = s.post(4, State::sign24(s.n[4]));
        const auto v0 = s.readY(e0);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::y0, v0);
    }
    // P:0005F8  mac y0,x0,b y:(r4)+,y0  [4EDCDA]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, false);
        s.setWord(R::y0, v0);
    }
    }
    // P:0005F9  move a,x:(r1)+n1  [564900]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, State::sign24(s.n[1]));
        s.writeX(d0, v0);
    }
    // P:0005FA  move b,x:(r2)+n2  [574A00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(2, State::sign24(s.n[2]));
        s.writeX(d0, v0);
    }
L0005fb:;
    // P:0005FB  move y0,x:(r5)+  [465D00]
    {
        const auto v0 = s.word(R::y0);
        const auto d0 = s.post(5, 1);
        s.writeX(d0, v0);
    }
    // P:0005FC  move y:(r4)+,y0  [4EDC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::y0, v0);
    }
    // P:0005FD  move y:(r4)+,y1  [4FDC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::y1, v0);
    }
    // P:0005FE  move y,l:(r5)  [436500]
    {
        const auto v0 = s.longReg(R::y);
        const auto d0 = s.post(5, 0);
        s.writeLong(d0, v0);
    }
    // P:0005FF  move y:(r7-$6),x1  [03EFB5]
    {
        const auto e0 = ((s.r[7] - 0x6u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::x1, v0);
    }
    // P:000600  tfr x1,a  [200061]
    {
        s.acc[0] = State::extendWord(s.word(R::x1));
    }
    // P:000601  sub #>$5bf,a  [0140C4 0005BF]
    {
        s.sub(0, State::extendWord(0x5bfu));
    }
    // P:000603  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:000604  move a,y0  [21C600]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y0, v0);
    }
    // P:000605  sub #>$80,a  [0140C4 000080]
    {
        s.sub(0, State::extendWord(0x80u));
    }
    // P:000607  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:000608  move a,y1  [21C700]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y1, v0);
    }
    // P:000609  move #>$63f,x0  [44F400 00063F]
    {
        const auto v0 = 0x63fu;
        s.setWord(R::x0, v0);
    }
    // P:00060B  tfr x1,a  [200061]
    {
        s.acc[0] = State::extendWord(s.word(R::x1));
    }
    // P:00060C  cmp x0,a  [200045]
    {
        s.cmp(0, State::extendWord(s.word(R::x0)));
    }
    // P:00060D  tfr x0,a ifgt  [202741]
    {
        if (s.condition(C::gt)) { s.acc[0] = State::extendWord(s.word(R::x0)); }
    }
    // P:00060E  move a,x1  [21C500]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x1, v0);
    }
    // P:00060F  move x1,r0  [20B000]
    {
        const auto v0 = s.word(R::x1);
        s.setWord(R::r0, v0);
    }
    // P:000610  move y:(r6+$b),a  [022EFE]
    {
        const auto e0 = ((s.r[6] + 0xbu) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:000611  mpyi #>$fffdf3,x1,b  [0141E8 FFFDF3]
    {
        s.multiply(1, 0xfffdf3u, s.word(R::x1), 0, false);
    }
    // P:000613  maci #>$ffe666,y0,b  [0141DA FFE666]
    {
        s.multiply(1, 0xffe666u, s.word(R::y0), 1, false);
    }
    // P:000615  maci #>$ffa666,y1,b  [0141FA FFA666]
    {
        s.multiply(1, 0xffa666u, s.word(R::y1), 1, false);
    }
    // P:000617  asl #$18,b,b  [0C1DB1]
    {
        s.shift(1, 1, 24, true);
    }
    // P:000618  tfr x1,b b,y0  [21E669]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y0, v0);
    }
    // P:000619  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:00061A  mac y0,x0,a  [2000D2]
    {
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
    }
    // P:00061B  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:00061C  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:00061D  maci #>$fff912,x0,b  [0141CA FFF912]
    {
        s.multiply(1, 0xfff912u, s.word(R::x0), 1, false);
    }
    // P:00061F  move x:(r7-$1e),x0  [038F94]
    {
        const auto e0 = ((s.r[7] - 0x1eu) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:000620  move y:(r7-$1e),a  [038FBE]
    {
        const auto e0 = ((s.r[7] - 0x1eu) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:000621  move b1,x:(r7-$1e)  [038F8D]
    {
        const auto v0 = s.word(R::b1);
        const auto d0 = ((s.r[7] - 0x1eu) & 0xffffffu);
        s.writeX(d0, v0);
    }
    // P:000622  move x1,y:(r7-$1e)  [038FA5]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = ((s.r[7] - 0x1eu) & 0xffffffu);
        s.writeY(d0, v0);
    }
    // P:000623  add x1,a b1,r2  [21B260]
    {
        const auto v0 = s.word(R::b1);
        s.add(0, State::extendWord(s.word(R::x1)));
        s.setWord(R::r2, v0);
    }
    // P:000624  asr a  [200022]
    {
        s.shift(0, 0, 1, false);
    }
    // P:000625  add x0,b  [200048]
    {
        s.add(1, State::extendWord(s.word(R::x0)));
    }
    // P:000626  asr b a1,r1  [21912A]
    {
        const auto v0 = s.word(R::a1);
        s.shift(1, 1, 1, false);
        s.setWord(R::r1, v0);
    }
    // P:000627  move a0,x0  [210400]
    {
        const auto v0 = s.word(R::a0);
        s.setWord(R::x0, v0);
    }
    // P:000628  move b0,x1  [212500]
    {
        const auto v0 = s.word(R::b0);
        s.setWord(R::x1, v0);
    }
    // P:000629  move x:(r1+$141a98),y0  [0A71C6 141A98]
    {
        const auto e0 = ((s.r[1] + 0x141a98u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:00062B  move x:(r1+$141a99),y1  [0A71C7 141A99]
    {
        const auto e0 = ((s.r[1] + 0x141a99u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:00062D  tfr y0,a b1,r3  [21B351]
    {
        const auto v0 = s.word(R::b1);
        s.acc[0] = State::extendWord(s.word(R::y0));
        s.setWord(R::r3, v0);
    }
    // P:00062E  macsu -y0,x0,a  [012695]
    {
        s.multiply(0, s.word(R::y0), s.word(R::x0), 3, true);
    }
    // P:00062F  macsu y1,x0,a  [01268C]
    {
        s.multiply(0, s.word(R::y1), s.word(R::x0), 3, false);
    }
    // P:000630  move x:(r1+$142158),y0  [0A71C6 142158]
    {
        const auto e0 = ((s.r[1] + 0x142158u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:000632  move x:(r1+$142159),y1  [0A71C7 142159]
    {
        const auto e0 = ((s.r[1] + 0x142159u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:000634  mpysu -y0,x0,b  [0127B5]
    {
        s.multiply(1, s.word(R::y0), s.word(R::x0), 2, true);
    }
    // P:000635  add y0,b a,y0  [21C658]
    {
        const auto v0 = s.word(R::a);
        s.add(1, State::extendWord(s.word(R::y0)));
        s.setWord(R::y0, v0);
    }
    // P:000636  macsu y1,x0,b  [0126AC]
    {
        s.multiply(1, s.word(R::y1), s.word(R::x0), 3, false);
    }
    // P:000637  move x:(r3+$142f06),x0  [0A73C4 142F06]
    {
        const auto e0 = ((s.r[3] + 0x142f06u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:000639  move x:(r3+$142f07),y1  [0A73C7 142F07]
    {
        const auto e0 = ((s.r[3] + 0x142f07u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:00063B  mpysu y1,x1,a  [012787]
    {
        s.multiply(0, s.word(R::y1), s.word(R::x1), 2, false);
    }
    // P:00063C  add x0,a b,y1  [21E740]
    {
        const auto v0 = s.word(R::b);
        s.add(0, State::extendWord(s.word(R::x0)));
        s.setWord(R::y1, v0);
    }
    // P:00063D  macsu -x0,x1,a  [01269A]
    {
        s.multiply(0, s.word(R::x0), s.word(R::x1), 3, true);
    }
    // P:00063E  move #>$7fffa4,x0  [44F400 7FFFA4]
    {
        const auto v0 = 0x7fffa4u;
        s.setWord(R::x0, v0);
    }
    // P:000640  cmp x0,a  [200045]
    {
        s.cmp(0, State::extendWord(s.word(R::x0)));
    }
    // P:000641  tgt x0,a  [027040]
    {
        if (s.condition(C::gt)) s.acc[0] = State::extendWord(s.word(R::x0));
    }
    // P:000642  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:000643  mpy x0,y1,b  [2000C8]
    {
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
    }
    // P:000644  mpy y0,x0,a  [2000D0]
    {
        s.multiply(0, s.word(R::y0), s.word(R::x0), 0, false);
    }
    // P:000645  move b,y0  [21E600]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::y0, v0);
    }
    // P:000646  add #>$800000,a  [0140C0 800000]
    {
        s.add(0, State::extendWord(0x800000u));
    }
    // P:000648  move b,y:$1e  [5F1E00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = 0x1eu;
        s.writeY(d0, v0);
    }
    // P:000649  move a,y:$1d  [5E1D00]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = 0x1du;
        s.writeY(d0, v0);
    }
    // P:00064A  mpy x0,x0,b  [200088]
    {
        s.multiply(1, s.word(R::x0), s.word(R::x0), 0, false);
    }
    // P:00064B  subr a,b  [20000E]
    {
        s.subr(1, s.acc[false]);
    }
    // P:00064C  sub #>$400000,b  [0140CC 400000]
    {
        s.sub(1, State::extendWord(0x400000u));
    }
    // P:00064E  andi #$fe,ccr  [00FEB9]
    s.c = false;
    // P:00064F  move #>$800,a  [56F400 000800]
    {
        const auto v0 = 0x800u;
        s.setWord(R::a, v0);
    }
    // P:000651  do #<$18,>$654  [061880 000653]
    for (unsigned loop_651=0; loop_651<24u; ++loop_651) {
    // P:000653  div y0,a  [018050]
    {
        s.divide(0, s.word(R::y0));
    }
    }
    // P:000654  move b1,y1  [21A700]
    {
        const auto v0 = s.word(R::b1);
        s.setWord(R::y1, v0);
    }
    // P:000655  move b0,y0  [212600]
    {
        const auto v0 = s.word(R::b0);
        s.setWord(R::y0, v0);
    }
    // P:000656  move a0,x1  [210500]
    {
        const auto v0 = s.word(R::a0);
        s.setWord(R::x1, v0);
    }
    // P:000657  mpysu x1,y0,b  [0127A6]
    {
        s.multiply(1, s.word(R::x1), s.word(R::y0), 2, false);
    }
    // P:000658  dmac ss x1,y1,b  [0124AF]
    {
        s.multiply(1, s.word(R::x1), s.word(R::y1), 4, false);
    }
    // P:000659  asl #$a,b,b  [0C1D95]
    {
        s.shift(1, 1, 10, true);
    }
    // P:00065A  move y:(r6+$4),a  [0216BE]
    {
        const auto e0 = ((s.r[6] + 0x4u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:00065B  asl a b,y1  [21E732]
    {
        const auto v0 = s.word(R::b);
        s.shift(0, 0, 1, true);
        s.setWord(R::y1, v0);
    }
    // P:00065C  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:00065D  mpyi #>$100,x0,a  [0141C0 000100]
    {
        s.multiply(0, 0x100u, s.word(R::x0), 0, false);
    }
    // P:00065F  move a,r1  [21D100]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::r1, v0);
    }
    // P:000660  move x:(r1+$143c06),x0  [0A71C4 143C06]
    {
        const auto e0 = ((s.r[1] + 0x143c06u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:000662  mpy x0,y1,b  [2000C8]
    {
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
    }
    // P:000663  move x:(r0+$141a98),y0  [0A70C6 141A98]
    {
        const auto e0 = ((s.r[0] + 0x141a98u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:000665  move b,y:$1c  [5F1C00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = 0x1cu;
        s.writeY(d0, v0);
    }
    // P:000666  move x:(r0+$142158),y1  [0A70C7 142158]
    {
        const auto e0 = ((s.r[0] + 0x142158u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:000668  move x:(r2+$142f06),a  [0A72CE 142F06]
    {
        const auto e0 = ((s.r[2] + 0x142f06u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::a, v0);
    }
    // P:00066A  move #>$7fffa4,x0  [44F400 7FFFA4]
    {
        const auto v0 = 0x7fffa4u;
        s.setWord(R::x0, v0);
    }
    // P:00066C  cmp x0,a  [200045]
    {
        s.cmp(0, State::extendWord(s.word(R::x0)));
    }
    // P:00066D  tfr x0,a ifgt  [202741]
    {
        if (s.condition(C::gt)) { s.acc[0] = State::extendWord(s.word(R::x0)); }
    }
    // P:00066E  move #$5,r4  [340500]
    {
        const auto v0 = 0x5u;
        s.setWord(R::r4, v0);
    }
    // P:00066F  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:000670  mpy y0,x0,a  [2000D0]
    {
        s.multiply(0, s.word(R::y0), s.word(R::x0), 0, false);
    }
    // P:000671  mpy x0,y1,b  [2000C8]
    {
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
    }
    // P:000672  move a,y:(r4)+  [5E5C00]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(4, 1);
        s.writeY(d0, v0);
    }
    // P:000673  move b,y:(r4)+  [5F5C00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(4, 1);
        s.writeY(d0, v0);
    }
    // P:000674  mpy x0,x0,b b,y0  [21E688]
    {
        const auto v0 = s.word(R::b);
        s.multiply(1, s.word(R::x0), s.word(R::x0), 0, false);
        s.setWord(R::y0, v0);
    }
    // P:000675  subr a,b  [20000E]
    {
        s.subr(1, s.acc[false]);
    }
    // P:000676  add #>$400000,b  [0140C8 400000]
    {
        s.add(1, State::extendWord(0x400000u));
    }
    // P:000678  andi #$fe,ccr  [00FEB9]
    s.c = false;
    // P:000679  move #>$800,a  [56F400 000800]
    {
        const auto v0 = 0x800u;
        s.setWord(R::a, v0);
    }
    // P:00067B  do #<$18,>$67e  [061880 00067D]
    for (unsigned loop_67b=0; loop_67b<24u; ++loop_67b) {
    // P:00067D  div y0,a  [018050]
    {
        s.divide(0, s.word(R::y0));
    }
    }
    // P:00067E  move b1,y1  [21A700]
    {
        const auto v0 = s.word(R::b1);
        s.setWord(R::y1, v0);
    }
    // P:00067F  move b0,y0  [212600]
    {
        const auto v0 = s.word(R::b0);
        s.setWord(R::y0, v0);
    }
    // P:000680  move a0,x1  [210500]
    {
        const auto v0 = s.word(R::a0);
        s.setWord(R::x1, v0);
    }
    // P:000681  mpysu x1,y0,b  [0127A6]
    {
        s.multiply(1, s.word(R::x1), s.word(R::y0), 2, false);
    }
    // P:000682  dmac ss x1,y1,b  [0124AF]
    {
        s.multiply(1, s.word(R::x1), s.word(R::y1), 4, false);
    }
    // P:000683  move #$2,n0  [380200]
    {
        const auto v0 = 0x2u;
        s.setWord(R::n0, v0);
    }
    // P:000684  asl #$a,b,b  [0C1D95]
    {
        s.shift(1, 1, 10, true);
    }
    // P:000685  move y:(r6+$4),a  [0216BE]
    {
        const auto e0 = ((s.r[6] + 0x4u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:000686  asl a b,y1  [21E732]
    {
        const auto v0 = s.word(R::b);
        s.shift(0, 0, 1, true);
        s.setWord(R::y1, v0);
    }
    // P:000687  move #$5,r0  [300500]
    {
        const auto v0 = 0x5u;
        s.setWord(R::r0, v0);
    }
    // P:000688  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:000689  mpyi #>$100,x0,a  [0141C0 000100]
    {
        s.multiply(0, 0x100u, s.word(R::x0), 0, false);
    }
    // P:00068B  move a,r1  [21D100]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::r1, v0);
    }
    // P:00068C  move x:(r1+$143c06),x0  [0A71C4 143C06]
    {
        const auto e0 = ((s.r[1] + 0x143c06u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:00068E  move #$4,r1  [310400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r1, v0);
    }
    // P:00068F  mpy x0,y1,b y:(r0)+,a  [5ED8C8]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readY(e0);
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
        s.setWord(R::a, v0);
    }
    // P:000690  add #>$800000,a  [0140C0 800000]
    {
        s.add(0, State::extendWord(0x800000u));
    }
    // P:000692  move b,y:$4  [5F0400]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = 0x4u;
        s.writeY(d0, v0);
    }
    // P:000693  move l:(r7),x  [42E700]
    {
        const auto e0 = s.post(7, 0);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:000694  move a,y:(r7)  [5E6700]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(7, 0);
        s.writeY(d0, v0);
    }
    // P:000695  move y:$1d,a  [5E9D00]
    {
        const auto e0 = 0x1du;
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:000696  sub x0,a y:(r0)-,y1  [4FD044]
    {
        const auto e0 = s.post(0, -1);
        const auto v0 = s.readY(e0);
        s.sub(0, State::extendWord(s.word(R::x0)));
        s.setWord(R::y1, v0);
    }
    // P:000697  move y1,x:(r7)  [476700]
    {
        const auto v0 = s.word(R::y1);
        const auto d0 = s.post(7, 0);
        s.writeX(d0, v0);
    }
    // P:000698  move y:$1e,b  [5F9E00]
    {
        const auto e0 = 0x1eu;
        const auto v0 = s.readY(e0);
        s.setWord(R::b, v0);
    }
    // P:000699  sub x1,b a,y0  [21C66C]
    {
        const auto v0 = s.word(R::a);
        s.sub(1, State::extendWord(s.word(R::x1)));
        s.setWord(R::y0, v0);
    }
    // P:00069A  tfr x0,a #$10,x0  [241041]
    {
        const auto v0 = 0x100000u;
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::x0, v0);
    }
    // P:00069B  tfr x1,b b,y1  [21E769]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y1, v0);
    }
    // P:00069C  do #<$8,>$6a0  [060880 00069F]
    for (unsigned loop_69c=0; loop_69c<8u; ++loop_69c) {
    // P:00069E  mac y0,x0,a a,y:(r0)+  [5E58D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(0, 1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeY(d0, v0);
    }
    // P:00069F  mac x0,y1,b b,y:(r0)+n0  [5F48CA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(0, State::sign24(s.n[0]));
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.writeY(d0, v0);
    }
    }
    // P:0006A0  move y:(r0)+,x0  [4CD800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::x0, v0);
    }
    // P:0006A1  move l:(r7)+,ba  [4BDF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::ba, v0);
    }
    // P:0006A2  sub x0,a y:(r0)-,x1  [4DD044]
    {
        const auto e0 = s.post(0, -1);
        const auto v0 = s.readY(e0);
        s.sub(0, State::extendWord(s.word(R::x0)));
        s.setWord(R::x1, v0);
    }
    // P:0006A3  sub x1,b  [20006C]
    {
        s.sub(1, State::extendWord(s.word(R::x1)));
    }
    // P:0006A4  tfr x0,a a,y0  [21C641]
    {
        const auto v0 = s.word(R::a);
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::y0, v0);
    }
    // P:0006A5  tfr x1,b b,y1  [21E769]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y1, v0);
    }
    // P:0006A6  move #$10,x0  [241000]
    {
        const auto v0 = 0x100000u;
        s.setWord(R::x0, v0);
    }
    // P:0006A7  do #<$8,>$6ab  [060880 0006AA]
    for (unsigned loop_6a7=0; loop_6a7<8u; ++loop_6a7) {
    // P:0006A9  mac y0,x0,a a,y:(r0)+  [5E58D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(0, 1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeY(d0, v0);
    }
    // P:0006AA  mac x0,y1,b b,y:(r0)+n0  [5F48CA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(0, State::sign24(s.n[0]));
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.writeY(d0, v0);
    }
    }
    // P:0006AB  move #$3,n0  [380300]
    {
        const auto v0 = 0x3u;
        s.setWord(R::n0, v0);
    }
    // P:0006AC  move n0,n1  [231900]
    {
        const auto v0 = s.word(R::n0);
        s.setWord(R::n1, v0);
    }
    // P:0006AD  move #$1c,r0  [301C00]
    {
        const auto v0 = 0x1cu;
        s.setWord(R::r0, v0);
    }
    // P:0006AE  move y:$1c,x1  [4D9C00]
    {
        const auto e0 = 0x1cu;
        const auto v0 = s.readY(e0);
        s.setWord(R::x1, v0);
    }
    // P:0006AF  tfr x1,a y:(r1),b  [5FE161]
    {
        const auto e0 = s.post(1, 0);
        const auto v0 = s.readY(e0);
        s.acc[0] = State::extendWord(s.word(R::x1));
        s.setWord(R::b, v0);
    }
    // P:0006B0  move y:(r7),x0  [4CE700]
    {
        const auto e0 = s.post(7, 0);
        const auto v0 = s.readY(e0);
        s.setWord(R::x0, v0);
    }
    // P:0006B1  sub x0,a b,y:(r7)+  [5F5F44]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(7, 1);
        s.sub(0, State::extendWord(s.word(R::x0)));
        s.writeY(d0, v0);
    }
    // P:0006B2  sub x1,b  [20006C]
    {
        s.sub(1, State::extendWord(s.word(R::x1)));
    }
    // P:0006B3  tfr x0,a a,y0  [21C641]
    {
        const auto v0 = s.word(R::a);
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::y0, v0);
    }
    // P:0006B4  tfr x1,b b,y1  [21E769]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y1, v0);
    }
    // P:0006B5  move #$10,x0  [241000]
    {
        const auto v0 = 0x100000u;
        s.setWord(R::x0, v0);
    }
    // P:0006B6  do #<$8,>$6ba  [060880 0006B9]
    for (unsigned loop_6b6=0; loop_6b6<8u; ++loop_6b6) {
    // P:0006B8  mac y0,x0,a a,y:(r1)+n1  [5E49D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, State::sign24(s.n[1]));
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeY(d0, v0);
    }
    // P:0006B9  mac x0,y1,b b,y:(r0)+n0  [5F48CA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(0, State::sign24(s.n[0]));
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.writeY(d0, v0);
    }
    }
    // P:0006BA  move #$74,r0  [307400]
    {
        const auto v0 = 0x74u;
        s.setWord(R::r0, v0);
    }
    // P:0006BB  move r0,r1  [221100]
    {
        const auto v0 = s.word(R::r0);
        s.setWord(R::r1, v0);
    }
    // P:0006BC  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:0006BD  move l:(r7)+,a  [48DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:0006BE  move l:(r7)-,b  [49D700]
    {
        const auto e0 = s.post(7, -1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::b, v0);
    }
    // P:0006BF  jsr func_000350  [0D0350]
    f350(s);
    // P:0006C0  move a,l:(r7)+  [485F00]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:0006C1  move b,l:(r7)+  [495F00]
    {
        const auto v0 = s.longReg(R::b);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:0006C2  move x:(r6+$f),x0  [023ED4]
    {
        const auto e0 = ((s.r[6] + 0xfu) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:0006C3  jset #$0,x0,func_0006cb  [0AC420 0006CB]
    if (((s.word(R::x0) >> 0) & 1u) == 1u) goto L0006cb;
    // P:0006C5  move #$b4,r0  [30B400]
    {
        const auto v0 = 0xb4u;
        s.setWord(R::r0, v0);
    }
    // P:0006C6  move r0,r1  [221100]
    {
        const auto v0 = s.word(R::r0);
        s.setWord(R::r1, v0);
    }
    // P:0006C7  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:0006C8  move l:(r7)+,a  [48DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:0006C9  move l:(r7)-,b  [49D700]
    {
        const auto e0 = s.post(7, -1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::b, v0);
    }
    // P:0006CA  jsr func_000350  [0D0350]
    f350(s);
L0006cb:;
    // P:0006CB  move a,l:(r7)+  [485F00]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:0006CC  move b,l:(r7)+  [495F00]
    {
        const auto v0 = s.longReg(R::b);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:0006CD  move x:(r7-$d),x1  [03CFD5]
    {
        const auto e0 = ((s.r[7] - 0xdu) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:0006CE  tfr x1,a  [200061]
    {
        s.acc[0] = State::extendWord(s.word(R::x1));
    }
    // P:0006CF  sub #>$5bf,a  [0140C4 0005BF]
    {
        s.sub(0, State::extendWord(0x5bfu));
    }
    // P:0006D1  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:0006D2  move a,y0  [21C600]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y0, v0);
    }
    // P:0006D3  sub #>$80,a  [0140C4 000080]
    {
        s.sub(0, State::extendWord(0x80u));
    }
    // P:0006D5  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:0006D6  move a,y1  [21C700]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y1, v0);
    }
    // P:0006D7  move #>$63f,x0  [44F400 00063F]
    {
        const auto v0 = 0x63fu;
        s.setWord(R::x0, v0);
    }
    // P:0006D9  tfr x1,a  [200061]
    {
        s.acc[0] = State::extendWord(s.word(R::x1));
    }
    // P:0006DA  cmp x0,a  [200045]
    {
        s.cmp(0, State::extendWord(s.word(R::x0)));
    }
    // P:0006DB  tfr x0,a ifgt  [202741]
    {
        if (s.condition(C::gt)) { s.acc[0] = State::extendWord(s.word(R::x0)); }
    }
    // P:0006DC  tst a  [200003]
    {
        s.tst(0);
    }
    // P:0006DD  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:0006DE  move a,x1  [21C500]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x1, v0);
    }
    // P:0006DF  move x1,r0  [20B000]
    {
        const auto v0 = s.word(R::x1);
        s.setWord(R::r0, v0);
    }
    // P:0006E0  move y:(r6+$a),a  [022EBE]
    {
        const auto e0 = ((s.r[6] + 0xau) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:0006E1  mpyi #>$fffdf3,x1,b  [0141E8 FFFDF3]
    {
        s.multiply(1, 0xfffdf3u, s.word(R::x1), 0, false);
    }
    // P:0006E3  maci #>$ffe666,y0,b  [0141DA FFE666]
    {
        s.multiply(1, 0xffe666u, s.word(R::y0), 1, false);
    }
    // P:0006E5  maci #>$ffd99a,y1,b  [0141FA FFD99A]
    {
        s.multiply(1, 0xffd99au, s.word(R::y1), 1, false);
    }
    // P:0006E7  asl #$18,b,b  [0C1DB1]
    {
        s.shift(1, 1, 24, true);
    }
    // P:0006E8  tfr x1,b b,y0  [21E669]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y0, v0);
    }
    // P:0006E9  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:0006EA  mac y0,x0,a  [2000D2]
    {
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
    }
    // P:0006EB  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:0006EC  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:0006ED  maci #>$fff912,x0,b  [0141CA FFF912]
    {
        s.multiply(1, 0xfff912u, s.word(R::x0), 1, false);
    }
    // P:0006EF  move x:(r7-$25),x0  [036FD4]
    {
        const auto e0 = ((s.r[7] - 0x25u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:0006F0  move y:(r7-$25),a  [036FFE]
    {
        const auto e0 = ((s.r[7] - 0x25u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:0006F1  move b1,x:(r7-$25)  [036FCD]
    {
        const auto v0 = s.word(R::b1);
        const auto d0 = ((s.r[7] - 0x25u) & 0xffffffu);
        s.writeX(d0, v0);
    }
    // P:0006F2  move x1,y:(r7-$25)  [036FE5]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = ((s.r[7] - 0x25u) & 0xffffffu);
        s.writeY(d0, v0);
    }
    // P:0006F3  add x1,a b1,r2  [21B260]
    {
        const auto v0 = s.word(R::b1);
        s.add(0, State::extendWord(s.word(R::x1)));
        s.setWord(R::r2, v0);
    }
    // P:0006F4  asr a  [200022]
    {
        s.shift(0, 0, 1, false);
    }
    // P:0006F5  add x0,b  [200048]
    {
        s.add(1, State::extendWord(s.word(R::x0)));
    }
    // P:0006F6  asr b a,r1  [21D12A]
    {
        const auto v0 = s.word(R::a);
        s.shift(1, 1, 1, false);
        s.setWord(R::r1, v0);
    }
    // P:0006F7  move a0,x0  [210400]
    {
        const auto v0 = s.word(R::a0);
        s.setWord(R::x0, v0);
    }
    // P:0006F8  move b0,x1  [212500]
    {
        const auto v0 = s.word(R::b0);
        s.setWord(R::x1, v0);
    }
    // P:0006F9  move x:(r1+$141a98),y0  [0A71C6 141A98]
    {
        const auto e0 = ((s.r[1] + 0x141a98u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:0006FB  move x:(r1+$141a99),y1  [0A71C7 141A99]
    {
        const auto e0 = ((s.r[1] + 0x141a99u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:0006FD  tfr y0,a b1,r3  [21B351]
    {
        const auto v0 = s.word(R::b1);
        s.acc[0] = State::extendWord(s.word(R::y0));
        s.setWord(R::r3, v0);
    }
    // P:0006FE  macsu -y0,x0,a  [012695]
    {
        s.multiply(0, s.word(R::y0), s.word(R::x0), 3, true);
    }
    // P:0006FF  macsu y1,x0,a  [01268C]
    {
        s.multiply(0, s.word(R::y1), s.word(R::x0), 3, false);
    }
    // P:000700  move x:(r1+$142158),y0  [0A71C6 142158]
    {
        const auto e0 = ((s.r[1] + 0x142158u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:000702  move x:(r1+$142159),y1  [0A71C7 142159]
    {
        const auto e0 = ((s.r[1] + 0x142159u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:000704  mpysu -y0,x0,b  [0127B5]
    {
        s.multiply(1, s.word(R::y0), s.word(R::x0), 2, true);
    }
    // P:000705  add y0,b a,y0  [21C658]
    {
        const auto v0 = s.word(R::a);
        s.add(1, State::extendWord(s.word(R::y0)));
        s.setWord(R::y0, v0);
    }
    // P:000706  macsu y1,x0,b  [0126AC]
    {
        s.multiply(1, s.word(R::y1), s.word(R::x0), 3, false);
    }
    // P:000707  move x:(r3+$142f07),y1  [0A73C7 142F07]
    {
        const auto e0 = ((s.r[3] + 0x142f07u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:000709  move x:(r3+$142f06),x0  [0A73C4 142F06]
    {
        const auto e0 = ((s.r[3] + 0x142f06u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:00070B  mpysu y1,x1,a  [012787]
    {
        s.multiply(0, s.word(R::y1), s.word(R::x1), 2, false);
    }
    // P:00070C  add x0,a b,y1  [21E740]
    {
        const auto v0 = s.word(R::b);
        s.add(0, State::extendWord(s.word(R::x0)));
        s.setWord(R::y1, v0);
    }
    // P:00070D  macsu -x0,x1,a  [01269A]
    {
        s.multiply(0, s.word(R::x0), s.word(R::x1), 3, true);
    }
    // P:00070E  move #>$7fffa4,x0  [44F400 7FFFA4]
    {
        const auto v0 = 0x7fffa4u;
        s.setWord(R::x0, v0);
    }
    // P:000710  cmp x0,a  [200045]
    {
        s.cmp(0, State::extendWord(s.word(R::x0)));
    }
    // P:000711  tgt x0,a  [027040]
    {
        if (s.condition(C::gt)) s.acc[0] = State::extendWord(s.word(R::x0));
    }
    // P:000712  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:000713  mpy x0,y1,b  [2000C8]
    {
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
    }
    // P:000714  mpy y0,x0,a  [2000D0]
    {
        s.multiply(0, s.word(R::y0), s.word(R::x0), 0, false);
    }
    // P:000715  move b,y0  [21E600]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::y0, v0);
    }
    // P:000716  add #>$800000,a  [0140C0 800000]
    {
        s.add(0, State::extendWord(0x800000u));
    }
    // P:000718  move b,y:$1e  [5F1E00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = 0x1eu;
        s.writeY(d0, v0);
    }
    // P:000719  move a,y:$1d  [5E1D00]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = 0x1du;
        s.writeY(d0, v0);
    }
    // P:00071A  mpy x0,x0,b  [200088]
    {
        s.multiply(1, s.word(R::x0), s.word(R::x0), 0, false);
    }
    // P:00071B  subr a,b  [20000E]
    {
        s.subr(1, s.acc[false]);
    }
    // P:00071C  sub #>$400000,b  [0140CC 400000]
    {
        s.sub(1, State::extendWord(0x400000u));
    }
    // P:00071E  andi #$fe,ccr  [00FEB9]
    s.c = false;
    // P:00071F  move #>$800,a  [56F400 000800]
    {
        const auto v0 = 0x800u;
        s.setWord(R::a, v0);
    }
    // P:000721  do #<$18,>$724  [061880 000723]
    for (unsigned loop_721=0; loop_721<24u; ++loop_721) {
    // P:000723  div y0,a  [018050]
    {
        s.divide(0, s.word(R::y0));
    }
    }
    // P:000724  move b1,y1  [21A700]
    {
        const auto v0 = s.word(R::b1);
        s.setWord(R::y1, v0);
    }
    // P:000725  move b0,y0  [212600]
    {
        const auto v0 = s.word(R::b0);
        s.setWord(R::y0, v0);
    }
    // P:000726  move a0,x1  [210500]
    {
        const auto v0 = s.word(R::a0);
        s.setWord(R::x1, v0);
    }
    // P:000727  mpysu x1,y0,b  [0127A6]
    {
        s.multiply(1, s.word(R::x1), s.word(R::y0), 2, false);
    }
    // P:000728  dmac ss x1,y1,b  [0124AF]
    {
        s.multiply(1, s.word(R::x1), s.word(R::y1), 4, false);
    }
    // P:000729  asl #$a,b,b  [0C1D95]
    {
        s.shift(1, 1, 10, true);
    }
    // P:00072A  move x:(r0+$141a98),y0  [0A70C6 141A98]
    {
        const auto e0 = ((s.r[0] + 0x141a98u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:00072C  move b,y:$1c  [5F1C00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = 0x1cu;
        s.writeY(d0, v0);
    }
    // P:00072D  move x:(r0+$142158),y1  [0A70C7 142158]
    {
        const auto e0 = ((s.r[0] + 0x142158u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y1, v0);
    }
    // P:00072F  move x:(r2+$142f06),a  [0A72CE 142F06]
    {
        const auto e0 = ((s.r[2] + 0x142f06u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::a, v0);
    }
    // P:000731  move #>$7fffa4,x0  [44F400 7FFFA4]
    {
        const auto v0 = 0x7fffa4u;
        s.setWord(R::x0, v0);
    }
    // P:000733  cmp x0,a  [200045]
    {
        s.cmp(0, State::extendWord(s.word(R::x0)));
    }
    // P:000734  tgt x0,a  [027040]
    {
        if (s.condition(C::gt)) s.acc[0] = State::extendWord(s.word(R::x0));
    }
    // P:000735  move #$5,r4  [340500]
    {
        const auto v0 = 0x5u;
        s.setWord(R::r4, v0);
    }
    // P:000736  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:000737  mpy y0,x0,a  [2000D0]
    {
        s.multiply(0, s.word(R::y0), s.word(R::x0), 0, false);
    }
    // P:000738  mpy x0,y1,b  [2000C8]
    {
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
    }
    // P:000739  move a,y:(r4)+  [5E5C00]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(4, 1);
        s.writeY(d0, v0);
    }
    // P:00073A  move b,y0  [21E600]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::y0, v0);
    }
    // P:00073B  mpy x0,x0,b b,y:(r4)+  [5F5C88]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(4, 1);
        s.multiply(1, s.word(R::x0), s.word(R::x0), 0, false);
        s.writeY(d0, v0);
    }
    // P:00073C  subr a,b  [20000E]
    {
        s.subr(1, s.acc[false]);
    }
    // P:00073D  add #>$400000,b  [0140C8 400000]
    {
        s.add(1, State::extendWord(0x400000u));
    }
    // P:00073F  andi #$fe,ccr  [00FEB9]
    s.c = false;
    // P:000740  move #>$800,a  [56F400 000800]
    {
        const auto v0 = 0x800u;
        s.setWord(R::a, v0);
    }
    // P:000742  do #<$18,>$745  [061880 000744]
    for (unsigned loop_742=0; loop_742<24u; ++loop_742) {
    // P:000744  div y0,a  [018050]
    {
        s.divide(0, s.word(R::y0));
    }
    }
    // P:000745  move b1,y1  [21A700]
    {
        const auto v0 = s.word(R::b1);
        s.setWord(R::y1, v0);
    }
    // P:000746  move b0,y0  [212600]
    {
        const auto v0 = s.word(R::b0);
        s.setWord(R::y0, v0);
    }
    // P:000747  move a0,x1  [210500]
    {
        const auto v0 = s.word(R::a0);
        s.setWord(R::x1, v0);
    }
    // P:000748  mpysu x1,y0,b  [0127A6]
    {
        s.multiply(1, s.word(R::x1), s.word(R::y0), 2, false);
    }
    // P:000749  dmac ss x1,y1,b  [0124AF]
    {
        s.multiply(1, s.word(R::x1), s.word(R::y1), 4, false);
    }
    // P:00074A  move #$2,n0  [380200]
    {
        const auto v0 = 0x2u;
        s.setWord(R::n0, v0);
    }
    // P:00074B  asl #$a,b,b  [0C1D95]
    {
        s.shift(1, 1, 10, true);
    }
    // P:00074C  move b,y1  [21E700]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::y1, v0);
    }
    // P:00074D  move #$5,r0  [300500]
    {
        const auto v0 = 0x5u;
        s.setWord(R::r0, v0);
    }
    // P:00074E  move #$4,r1  [310400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r1, v0);
    }
    // P:00074F  move y:(r0)+,a  [5ED800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:000750  add #>$800000,a  [0140C0 800000]
    {
        s.add(0, State::extendWord(0x800000u));
    }
    // P:000752  move b,y:$4  [5F0400]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = 0x4u;
        s.writeY(d0, v0);
    }
    // P:000753  move l:(r7),x  [42E700]
    {
        const auto e0 = s.post(7, 0);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:000754  move a,y:(r7)  [5E6700]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(7, 0);
        s.writeY(d0, v0);
    }
    // P:000755  move y:$1d,a  [5E9D00]
    {
        const auto e0 = 0x1du;
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:000756  sub x0,a y:(r0)-,y1  [4FD044]
    {
        const auto e0 = s.post(0, -1);
        const auto v0 = s.readY(e0);
        s.sub(0, State::extendWord(s.word(R::x0)));
        s.setWord(R::y1, v0);
    }
    // P:000757  move y1,x:(r7)  [476700]
    {
        const auto v0 = s.word(R::y1);
        const auto d0 = s.post(7, 0);
        s.writeX(d0, v0);
    }
    // P:000758  move y:$1e,b  [5F9E00]
    {
        const auto e0 = 0x1eu;
        const auto v0 = s.readY(e0);
        s.setWord(R::b, v0);
    }
    // P:000759  sub x1,b a,y0  [21C66C]
    {
        const auto v0 = s.word(R::a);
        s.sub(1, State::extendWord(s.word(R::x1)));
        s.setWord(R::y0, v0);
    }
    // P:00075A  tfr x0,a #$10,x0  [241041]
    {
        const auto v0 = 0x100000u;
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::x0, v0);
    }
    // P:00075B  tfr x1,b b,y1  [21E769]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y1, v0);
    }
    // P:00075C  do #<$8,>$760  [060880 00075F]
    for (unsigned loop_75c=0; loop_75c<8u; ++loop_75c) {
    // P:00075E  mac y0,x0,a a,y:(r0)+  [5E58D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(0, 1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeY(d0, v0);
    }
    // P:00075F  mac x0,y1,b b,y:(r0)+n0  [5F48CA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(0, State::sign24(s.n[0]));
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.writeY(d0, v0);
    }
    }
    // P:000760  move y:(r0)+,x0  [4CD800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::x0, v0);
    }
    // P:000761  move l:(r7)+,ba  [4BDF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::ba, v0);
    }
    // P:000762  sub x0,a y:(r0)-,x1  [4DD044]
    {
        const auto e0 = s.post(0, -1);
        const auto v0 = s.readY(e0);
        s.sub(0, State::extendWord(s.word(R::x0)));
        s.setWord(R::x1, v0);
    }
    // P:000763  sub x1,b  [20006C]
    {
        s.sub(1, State::extendWord(s.word(R::x1)));
    }
    // P:000764  tfr x0,a a,y0  [21C641]
    {
        const auto v0 = s.word(R::a);
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::y0, v0);
    }
    // P:000765  tfr x1,b b,y1  [21E769]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y1, v0);
    }
    // P:000766  move #$10,x0  [241000]
    {
        const auto v0 = 0x100000u;
        s.setWord(R::x0, v0);
    }
    // P:000767  do #<$8,>$76b  [060880 00076A]
    for (unsigned loop_767=0; loop_767<8u; ++loop_767) {
    // P:000769  mac y0,x0,a a,y:(r0)+  [5E58D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(0, 1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeY(d0, v0);
    }
    // P:00076A  mac x0,y1,b b,y:(r0)+n0  [5F48CA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(0, State::sign24(s.n[0]));
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.writeY(d0, v0);
    }
    }
    // P:00076B  move #$3,n0  [380300]
    {
        const auto v0 = 0x3u;
        s.setWord(R::n0, v0);
    }
    // P:00076C  move n0,n1  [231900]
    {
        const auto v0 = s.word(R::n0);
        s.setWord(R::n1, v0);
    }
    // P:00076D  move #$1c,r0  [301C00]
    {
        const auto v0 = 0x1cu;
        s.setWord(R::r0, v0);
    }
    // P:00076E  move y:$1c,x1  [4D9C00]
    {
        const auto e0 = 0x1cu;
        const auto v0 = s.readY(e0);
        s.setWord(R::x1, v0);
    }
    // P:00076F  tfr x1,a y:(r1),b  [5FE161]
    {
        const auto e0 = s.post(1, 0);
        const auto v0 = s.readY(e0);
        s.acc[0] = State::extendWord(s.word(R::x1));
        s.setWord(R::b, v0);
    }
    // P:000770  move y:(r7),x0  [4CE700]
    {
        const auto e0 = s.post(7, 0);
        const auto v0 = s.readY(e0);
        s.setWord(R::x0, v0);
    }
    // P:000771  sub x0,a b,y:(r7)+  [5F5F44]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(7, 1);
        s.sub(0, State::extendWord(s.word(R::x0)));
        s.writeY(d0, v0);
    }
    // P:000772  sub x1,b  [20006C]
    {
        s.sub(1, State::extendWord(s.word(R::x1)));
    }
    // P:000773  tfr x0,a a,y0  [21C641]
    {
        const auto v0 = s.word(R::a);
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::y0, v0);
    }
    // P:000774  tfr x1,b b,y1  [21E769]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y1, v0);
    }
    // P:000775  move #$10,x0  [241000]
    {
        const auto v0 = 0x100000u;
        s.setWord(R::x0, v0);
    }
    // P:000776  do #<$8,>$77a  [060880 000779]
    for (unsigned loop_776=0; loop_776<8u; ++loop_776) {
    // P:000778  mac y0,x0,a a,y:(r1)+n1  [5E49D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, State::sign24(s.n[1]));
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeY(d0, v0);
    }
    // P:000779  mac x0,y1,b b,y:(r0)+n0  [5F48CA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(0, State::sign24(s.n[0]));
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.writeY(d0, v0);
    }
    }
    // P:00077A  move #$0,r5  [350000]
    {
        const auto v0 = 0x0u;
        s.setWord(R::r5, v0);
    }
    // P:00077B  move #$1,r2  [320100]
    {
        const auto v0 = 0x1u;
        s.setWord(R::r2, v0);
    }
    // P:00077C  move #>$fffffe,n7  [77F400 FFFFFE]
    {
        const auto v0 = 0xfffffeu;
        s.setWord(R::n7, v0);
    }
    // P:00077E  move #$74,r0  [307400]
    {
        const auto v0 = 0x74u;
        s.setWord(R::r0, v0);
    }
    // P:00077F  move #$72,r1  [317200]
    {
        const auto v0 = 0x72u;
        s.setWord(R::r1, v0);
    }
    // P:000780  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:000781  move #$10,x0  [241000]
    {
        const auto v0 = 0x100000u;
        s.setWord(R::x0, v0);
    }
    // P:000782  move x0,y:(r5)  [4C6500]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(5, 0);
        s.writeY(d0, v0);
    }
    // P:000783  move l:(r7)+,y  [43DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::y, v0);
    }
    // P:000784  move l:(r7)+,a  [48DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:000785  move l:(r7)+n7,b  [49CF00]
    {
        const auto e0 = s.post(7, State::sign24(s.n[7]));
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::b, v0);
    }
    // P:000786  move y0,x:(r1)+  [465900]
    {
        const auto v0 = s.word(R::y0);
        const auto d0 = s.post(1, 1);
        s.writeX(d0, v0);
    }
    // P:000787  move y1,x:(r1)-  [475100]
    {
        const auto v0 = s.word(R::y1);
        const auto d0 = s.post(1, -1);
        s.writeX(d0, v0);
    }
    // P:000788  jsr func_000365  [0D0365]
    f365(s);
    // P:000789  move x:(r1)+,x0  [44D900]
    {
        const auto e0 = s.post(1, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:00078A  move x:(r1),x1  [45E100]
    {
        const auto e0 = s.post(1, 0);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:00078B  move x,l:(r7)+  [425F00]
    {
        const auto v0 = s.longReg(R::x);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:00078C  move a,l:(r7)+  [485F00]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:00078D  move b,l:(r7)+  [495F00]
    {
        const auto v0 = s.longReg(R::b);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:00078E  move x:(r6+$f),x0  [023ED4]
    {
        const auto e0 = ((s.r[6] + 0xfu) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:00078F  jset #$0,x0,func_00079c  [0AC420 00079C]
    if (((s.word(R::x0) >> 0) & 1u) == 1u) goto L00079c;
    // P:000791  move #$b4,r0  [30B400]
    {
        const auto v0 = 0xb4u;
        s.setWord(R::r0, v0);
    }
    // P:000792  move #$b2,r1  [31B200]
    {
        const auto v0 = 0xb2u;
        s.setWord(R::r1, v0);
    }
    // P:000793  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:000794  move l:(r7)+,y  [43DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::y, v0);
    }
    // P:000795  move l:(r7)+,a  [48DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:000796  move l:(r7)+n7,b  [49CF00]
    {
        const auto e0 = s.post(7, State::sign24(s.n[7]));
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::b, v0);
    }
    // P:000797  move y0,x:(r1)+  [465900]
    {
        const auto v0 = s.word(R::y0);
        const auto d0 = s.post(1, 1);
        s.writeX(d0, v0);
    }
    // P:000798  move y1,x:(r1)-  [475100]
    {
        const auto v0 = s.word(R::y1);
        const auto d0 = s.post(1, -1);
        s.writeX(d0, v0);
    }
    // P:000799  jsr func_000365  [0D0365]
    f365(s);
    // P:00079A  move x:(r1)+,x0  [44D900]
    {
        const auto e0 = s.post(1, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:00079B  move x:(r1),x1  [45E100]
    {
        const auto e0 = s.post(1, 0);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
L00079c:;
    // P:00079C  move x,l:(r7)+  [425F00]
    {
        const auto v0 = s.longReg(R::x);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:00079D  move a,l:(r7)+  [485F00]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:00079E  move b,l:(r7)+  [495F00]
    {
        const auto v0 = s.longReg(R::b);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:00079F  move x:(r6+$b),y0  [022ED6]
    {
        const auto e0 = ((s.r[6] + 0xbu) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:0007A0  move #$8,a  [2E0800]
    {
        const auto v0 = 0x80000u;
        s.setWord(R::a, v0);
    }
    // P:0007A1  andi #$fe,ccr  [00FEB9]
    s.c = false;
    // P:0007A2  do #<$18,>$7a5  [061880 0007A4]
    for (unsigned loop_7a2=0; loop_7a2<24u; ++loop_7a2) {
    // P:0007A4  div y0,a  [018050]
    {
        s.divide(0, s.word(R::y0));
    }
    }
    // P:0007A5  move a0,y1  [210700]
    {
        const auto v0 = s.word(R::a0);
        s.setWord(R::y1, v0);
    }
    // P:0007A6  move y:(r6+$4),a  [0216BE]
    {
        const auto e0 = ((s.r[6] + 0x4u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:0007A7  asl a #$b2,r0  [30B232]
    {
        const auto v0 = 0xb2u;
        s.shift(0, 0, 1, true);
        s.setWord(R::r0, v0);
    }
    // P:0007A8  add #>$800000,a  [0140C0 800000]
    {
        s.add(0, State::extendWord(0x800000u));
    }
    // P:0007AA  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:0007AB  move #$72,r1  [317200]
    {
        const auto v0 = 0x72u;
        s.setWord(R::r1, v0);
    }
    // P:0007AC  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:0007AD  mpyi #>$100,x0,b  [0141C8 000100]
    {
        s.multiply(1, 0x100u, s.word(R::x0), 0, false);
    }
    // P:0007AF  mpy x0,x0,a #$70,r5  [357080]
    {
        const auto v0 = 0x70u;
        s.multiply(0, s.word(R::x0), s.word(R::x0), 0, false);
        s.setWord(R::r5, v0);
    }
    // P:0007B0  move #$b0,r4  [34B000]
    {
        const auto v0 = 0xb0u;
        s.setWord(R::r4, v0);
    }
    // P:0007B1  move b,r2  [21F200]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::r2, v0);
    }
    // P:0007B2  move a,x1  [21C500]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x1, v0);
    }
    // P:0007B3  mpyi #>$7deccd,x1,a  [0141E0 7DECCD]
    {
        s.multiply(0, 0x7deccdu, s.word(R::x1), 0, false);
    }
    // P:0007B5  add #>$21333,a  [0140C0 021333]
    {
        s.add(0, State::extendWord(0x21333u));
    }
    // P:0007B7  move x:(r2+$1447c6),x1  [0A72C5 1447C6]
    {
        const auto e0 = ((s.r[2] + 0x1447c6u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:0007B9  mpy x1,y0,b a,y0  [21C6E8]
    {
        const auto v0 = s.word(R::a);
        s.multiply(1, s.word(R::x1), s.word(R::y0), 0, false);
        s.setWord(R::y0, v0);
    }
    // P:0007BA  mpy y1,y0,a  [2000B0]
    {
        s.multiply(0, s.word(R::y1), s.word(R::y0), 0, false);
    }
    // P:0007BB  move b,x1  [21E500]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::x1, v0);
    }
    // P:0007BC  move x:(r0)+,x0 a,y0  [109800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto v1 = s.word(R::a);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:0007BD  do #<$22,>$7c3  [062280 0007C2]
    for (unsigned loop_7bd=0; loop_7bd<34u; ++loop_7bd) {
    // P:0007BF  mpy y0,x0,a x:(r1)+,x0 a,y:(r4)+  [B299D0]
    {
        const auto e0 = s.post(1, 1);
        const auto v0 = s.readX(e0);
        const auto v1 = s.word(R::a);
        const auto d1 = s.post(4, 1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 0, false);
        s.setWord(R::x0, v0);
        s.writeY(d1, v1);
    }
    // P:0007C0  asl #$8,a,a  [0C1D10]
    {
        s.shift(0, 0, 8, true);
    }
    // P:0007C1  mpy y0,x0,b x:(r0)+,x0 b,y:(r5)+  [B3B8D8]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto v1 = s.word(R::b);
        const auto d1 = s.post(5, 1);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 0, false);
        s.setWord(R::x0, v0);
        s.writeY(d1, v1);
    }
    // P:0007C2  asl #$8,b,b  [0C1D91]
    {
        s.shift(1, 1, 8, true);
    }
    }
    // P:0007C3  bset #$b,sr  [0AF96B]
    s.bit(R::sr, 11, 1);
    // P:0007C4  move #$70,r5  [357000]
    {
        const auto v0 = 0x70u;
        s.setWord(R::r5, v0);
    }
    // P:0007C5  move #$b0,r4  [34B000]
    {
        const auto v0 = 0xb0u;
        s.setWord(R::r4, v0);
    }
    // P:0007C6  move r5,r1  [22B100]
    {
        const auto v0 = s.word(R::r5);
        s.setWord(R::r1, v0);
    }
    // P:0007C7  move r4,r0  [229000]
    {
        const auto v0 = s.word(R::r4);
        s.setWord(R::r0, v0);
    }
    // P:0007C8  move #$80,x0  [248000]
    {
        const auto v0 = 0x800000u;
        s.setWord(R::x0, v0);
    }
    // P:0007C9  move y:(r4)+,y0  [4EDC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::y0, v0);
    }
    // P:0007CA  move y:(r5)+,y1  [4FDD00]
    {
        const auto e0 = s.post(5, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::y1, v0);
    }
    // P:0007CB  do #<$22,>$7cf  [062280 0007CE]
    for (unsigned loop_7cb=0; loop_7cb<34u; ++loop_7cb) {
    // P:0007CD  mpy y0,x0,a a,x:(r0)+ y:(r4)+,y0  [F818D0]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(0, 1);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 0, false);
        s.writeX(d0, v0);
        s.setWord(R::y0, v1);
    }
    // P:0007CE  mpy x0,y1,b b,x:(r1)+ y:(r5)+,y1  [FD39C8]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(1, 1);
        const auto e1 = s.post(5, 1);
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::x0), s.word(R::y1), 0, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    }
    // P:0007CF  move a,x:(r0)+  [565800]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:0007D0  move b,x:(r1)+  [575900]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(1, 1);
        s.writeX(d0, v0);
    }
    // P:0007D1  bclr #$b,sr  [0AF94B]
    s.bit(R::sr, 11, -1);
    // P:0007D2  move #>$6a3,x0  [44F400 0006A3]
    {
        const auto v0 = 0x6a3u;
        s.setWord(R::x0, v0);
    }
    // P:0007D4  move y:(r7-$14),a  [03B7BE]
    {
        const auto e0 = ((s.r[7] - 0x14u) & 0xffffffu);
        const auto v0 = s.readY(e0);
        s.setWord(R::a, v0);
    }
    // P:0007D5  cmp x0,a x1,y1  [20A745]
    {
        const auto v0 = s.word(R::x1);
        s.cmp(0, State::extendWord(s.word(R::x0)));
        s.setWord(R::y1, v0);
    }
    // P:0007D6  tfr x0,a ifge  [202141]
    {
        if (s.condition(C::ge)) { s.acc[0] = State::extendWord(s.word(R::x0)); }
    }
    // P:0007D7  move l:(r7),x  [42E700]
    {
        const auto e0 = s.post(7, 0);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:0007D8  move a,r0  [21D000]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::r0, v0);
    }
    // P:0007D9  move x:(r0+$143546),y0  [0A70C6 143546]
    {
        const auto e0 = ((s.r[0] + 0x143546u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:0007DB  mpy y1,y0,b y0,a  [20CEB8]
    {
        const auto v0 = s.word(R::y0);
        s.multiply(1, s.word(R::y1), s.word(R::y0), 0, false);
        s.setWord(R::a, v0);
    }
    // P:0007DC  move #$72,r0  [307200]
    {
        const auto v0 = 0x72u;
        s.setWord(R::r0, v0);
    }
    // P:0007DD  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:0007DE  sub x0,a ba,l:(r7)+  [4B5F44]
    {
        const auto v0 = s.longReg(R::ba);
        const auto d0 = s.post(7, 1);
        s.sub(0, State::extendWord(s.word(R::x0)));
        s.writeLong(d0, v0);
    }
    // P:0007DF  sub x1,b  [20006C]
    {
        s.sub(1, State::extendWord(s.word(R::x1)));
    }
    // P:0007E0  move a,y0  [21C600]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::y0, v0);
    }
    // P:0007E1  tfr x1,b b,y1  [21E769]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y1, v0);
    }
    // P:0007E2  tfr x0,a #$8,x0  [240841]
    {
        const auto v0 = 0x80000u;
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::x0, v0);
    }
    // P:0007E3  do #<$10,>$7e7  [061080 0007E6]
    for (unsigned loop_7e3=0; loop_7e3<16u; ++loop_7e3) {
    // P:0007E5  mac y0,x0,a a,y:(r4)+  [5E5CD2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(4, 1);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeY(d0, v0);
    }
    // P:0007E6  mac x0,y1,b b,y:(r4)+  [5F5CCA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(4, 1);
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.writeY(d0, v0);
    }
    }
    // P:0007E7  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:0007E8  move r0,r1  [221100]
    {
        const auto v0 = s.word(R::r0);
        s.setWord(R::r1, v0);
    }
    // P:0007E9  move #$b2,r2  [32B200]
    {
        const auto v0 = 0xb2u;
        s.setWord(R::r2, v0);
    }
    // P:0007EA  move r2,r3  [225300]
    {
        const auto v0 = s.word(R::r2);
        s.setWord(R::r3, v0);
    }
    // P:0007EB  move l:(r7)+,a  [48DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:0007EC  move l:(r7)-,b  [49D700]
    {
        const auto e0 = s.post(7, -1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::b, v0);
    }
    // P:0007ED  move x:(r0)+,x0 y:(r4)+,y0  [F09800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:0007EE  move y:(r4)+,x1  [4DDC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::x1, v0);
    }
    // P:0007EF  do #<$10,>$7fa  [061080 0007F9]
    for (unsigned loop_7ef=0; loop_7ef<16u; ++loop_7ef) {
    // P:0007F1  mac x1,x0,a a,x:(r1)+ a,y1  [1919A2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, 1);
        const auto v1 = s.word(R::a);
        s.multiply(0, s.word(R::x1), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:0007F2  mac -y1,y0,a x:(r2)+,x0  [44DAB6]
    {
        const auto e0 = s.post(2, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y1), s.word(R::y0), 1, true);
        s.setWord(R::x0, v0);
    }
    // P:0007F3  mac x1,x0,b b,x:(r3)+ b,y1  [1F1BAA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(3, 1);
        const auto v1 = s.word(R::b);
        s.multiply(1, s.word(R::x1), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:0007F4  mac -y1,y0,b x:(r0)+,x0  [44D8BE]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(1, s.word(R::y1), s.word(R::y0), 1, true);
        s.setWord(R::x0, v0);
    }
    // P:0007F5  mac x1,x0,a a,x:(r1)+ a,y1  [1919A2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, 1);
        const auto v1 = s.word(R::a);
        s.multiply(0, s.word(R::x1), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:0007F6  mac -y1,y0,a x:(r2)+,x0  [44DAB6]
    {
        const auto e0 = s.post(2, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y1), s.word(R::y0), 1, true);
        s.setWord(R::x0, v0);
    }
    // P:0007F7  mac x1,x0,b b,x:(r3)+ b,y1  [1F1BAA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(3, 1);
        const auto v1 = s.word(R::b);
        s.multiply(1, s.word(R::x1), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:0007F8  mac -y1,y0,b x:(r0)+,x0 y:(r4)+,y0  [F098BE]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::y1), s.word(R::y0), 1, true);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:0007F9  move y:(r4)+,x1  [4DDC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::x1, v0);
    }
    }
    // P:0007FA  move a,x:(r1)+  [565900]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, 1);
        s.writeX(d0, v0);
    }
    // P:0007FB  move b,x:(r3)+  [575B00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(3, 1);
        s.writeX(d0, v0);
    }
    // P:0007FC  move a,l:(r7)+  [485F00]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:0007FD  move b,l:(r7)+  [495F00]
    {
        const auto v0 = s.longReg(R::b);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:0007FE  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:0007FF  move #$2,n4  [3C0200]
    {
        const auto v0 = 0x2u;
        s.setWord(R::n4, v0);
    }
    // P:000800  move #$72,r0  [307200]
    {
        const auto v0 = 0x72u;
        s.setWord(R::r0, v0);
    }
    // P:000801  move r0,r1  [221100]
    {
        const auto v0 = s.word(R::r0);
        s.setWord(R::r1, v0);
    }
    // P:000802  move #$b2,r2  [32B200]
    {
        const auto v0 = 0xb2u;
        s.setWord(R::r2, v0);
    }
    // P:000803  move r2,r3  [225300]
    {
        const auto v0 = s.word(R::r2);
        s.setWord(R::r3, v0);
    }
    // P:000804  move l:(r7)+,a  [48DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:000805  move l:(r7)-,b  [49D700]
    {
        const auto e0 = s.post(7, -1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::b, v0);
    }
    // P:000806  jsr func_00037c  [0D037C]
    f37c(s);
    // P:000807  move a,l:(r7)+  [485F00]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:000808  move b,l:(r7)+  [495F00]
    {
        const auto v0 = s.longReg(R::b);
        const auto d0 = s.post(7, 1);
        s.writeLong(d0, v0);
    }
    // P:000809  move #>$eeb88,y0  [46F400 0EEB88]
    {
        const auto v0 = 0xeeb88u;
        s.setWord(R::y0, v0);
    }
    // P:00080B  move #>$39c201,y1  [47F400 39C201]
    {
        const auto v0 = 0x39c201u;
        s.setWord(R::y1, v0);
    }
    // P:00080D  lua (r7-$30),r3  [042F03]
    s.setWord(R::r3, ((s.r[7] - 0x30u) & 0xffffffu));
    // P:00080E  move r3,r4  [227400]
    {
        const auto v0 = s.word(R::r3);
        s.setWord(R::r4, v0);
    }
    // P:00080F  move #$6d,r0  [306D00]
    {
        const auto v0 = 0x6du;
        s.setWord(R::r0, v0);
    }
    // P:000810  move #$6c,r2  [326C00]
    {
        const auto v0 = 0x6cu;
        s.setWord(R::r2, v0);
    }
    // P:000811  move l:(r3)+,x  [42DB00]
    {
        const auto e0 = s.post(3, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:000812  move x0,x:(r0)+  [445800]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:000813  move x1,x:(r0)+  [455800]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:000814  move l:(r3)+,x  [42DB00]
    {
        const auto e0 = s.post(3, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:000815  move x0,x:(r0)+  [445800]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:000816  move x1,x:(r0)+  [455800]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:000817  move x:(r3),x1  [45E300]
    {
        const auto e0 = s.post(3, 0);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:000818  move x1,x:(r0)+  [455800]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:000819  move #$6d,r0  [306D00]
    {
        const auto v0 = 0x6du;
        s.setWord(R::r0, v0);
    }
    // P:00081A  move #>$f75277,x1  [45F400 F75277]
    {
        const auto v0 = 0xf75277u;
        s.setWord(R::x1, v0);
    }
    // P:00081C  move #>$fffffd,n0  [70F400 FFFFFD]
    {
        const auto v0 = 0xfffffdu;
        s.setWord(R::n0, v0);
    }
    // P:00081E  move x:(r0)+,x0  [44D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:00081F  do #<$10,>$828  [061080 000827]
    for (unsigned loop_81f=0; loop_81f<16u; ++loop_81f) {
    // P:000821  mpy x1,x0,a x:(r0)+,x0  [44D8A0]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::x1), s.word(R::x0), 0, false);
        s.setWord(R::x0, v0);
    }
    // P:000822  mac y0,x0,a x:(r0)+,x0  [44D8D2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.setWord(R::x0, v0);
    }
    // P:000823  mac x0,y1,a x:(r0)+,x0  [44D8C2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::x0, v0);
    }
    // P:000824  mac x0,y1,a x:(r0)+,x0  [44D8C2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::x0, v0);
    }
    // P:000825  mac y0,x0,a x:(r0)+n0,x0  [44C8D2]
    {
        const auto e0 = s.post(0, State::sign24(s.n[0]));
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.setWord(R::x0, v0);
    }
    // P:000826  mac x1,x0,a b,x:(r2)+  [575AA2]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(2, 1);
        s.multiply(0, s.word(R::x1), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
    }
    // P:000827  tfr a,b x:(r0)+,x0  [44D809]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.acc[1] = s.acc[false];
        s.setWord(R::x0, v0);
    }
    }
    // P:000828  move a,x:(r2)+  [565A00]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(2, 1);
        s.writeX(d0, v0);
    }
    // P:000829  tfr x0,a r0,r1  [221141]
    {
        const auto v0 = s.word(R::r0);
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::r1, v0);
    }
    // P:00082A  move x:(r0)+,x1  [45D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:00082B  move x,l:(r4)+  [425C00]
    {
        const auto v0 = s.longReg(R::x);
        const auto d0 = s.post(4, 1);
        s.writeLong(d0, v0);
    }
    // P:00082C  move x:(r0)+,x0  [44D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:00082D  move x:(r0)+,x1  [45D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:00082E  move x,l:(r4)+  [425C00]
    {
        const auto v0 = s.longReg(R::x);
        const auto d0 = s.post(4, 1);
        s.writeLong(d0, v0);
    }
    // P:00082F  move x:(r0)+,x1  [45D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:000830  move x1,x:(r4)  [456400]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(4, 0);
        s.writeX(d0, v0);
    }
    // P:000831  move a,x0  [21C400]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::x0, v0);
    }
    // P:000832  move r1,r0  [223000]
    {
        const auto v0 = s.word(R::r1);
        s.setWord(R::r0, v0);
    }
    // P:000833  move x:(r6+$f),x1  [023ED5]
    {
        const auto e0 = ((s.r[6] + 0xfu) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:000834  jset #$0,x1,func_00084e  [0AC520 00084E]
    if (((s.word(R::x1) >> 0) & 1u) == 1u) goto L00084e;
    // P:000836  move #$ad,r0  [30AD00]
    {
        const auto v0 = 0xadu;
        s.setWord(R::r0, v0);
    }
    // P:000837  move #$ac,r2  [32AC00]
    {
        const auto v0 = 0xacu;
        s.setWord(R::r2, v0);
    }
    // P:000838  move y:(r3)+,x0  [4CDB00]
    {
        const auto e0 = s.post(3, 1);
        const auto v0 = s.readY(e0);
        s.setWord(R::x0, v0);
    }
    // P:000839  move x0,x:(r0)+  [445800]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:00083A  move l:(r3)+,x  [42DB00]
    {
        const auto e0 = s.post(3, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:00083B  move x0,x:(r0)+  [445800]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:00083C  move x1,x:(r0)+  [455800]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:00083D  move l:(r3)+,x  [42DB00]
    {
        const auto e0 = s.post(3, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:00083E  move x0,x:(r0)+  [445800]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:00083F  move x1,x:(r0)+  [455800]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(0, 1);
        s.writeX(d0, v0);
    }
    // P:000840  move #$ad,r0  [30AD00]
    {
        const auto v0 = 0xadu;
        s.setWord(R::r0, v0);
    }
    // P:000841  move #>$f75277,x1  [45F400 F75277]
    {
        const auto v0 = 0xf75277u;
        s.setWord(R::x1, v0);
    }
    // P:000843  move x:(r0)+,x0  [44D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:000844  do #<$10,>$84d  [061080 00084C]
    for (unsigned loop_844=0; loop_844<16u; ++loop_844) {
    // P:000846  mpy x1,x0,a x:(r0)+,x0  [44D8A0]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::x1), s.word(R::x0), 0, false);
        s.setWord(R::x0, v0);
    }
    // P:000847  mac y0,x0,a x:(r0)+,x0  [44D8D2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.setWord(R::x0, v0);
    }
    // P:000848  mac x0,y1,a x:(r0)+,x0  [44D8C2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::x0, v0);
    }
    // P:000849  mac x0,y1,a x:(r0)+,x0  [44D8C2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::x0, v0);
    }
    // P:00084A  mac y0,x0,a x:(r0)+n0,x0  [44C8D2]
    {
        const auto e0 = s.post(0, State::sign24(s.n[0]));
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.setWord(R::x0, v0);
    }
    // P:00084B  mac x1,x0,a b,x:(r2)+  [575AA2]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(2, 1);
        s.multiply(0, s.word(R::x1), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
    }
    // P:00084C  tfr a,b x:(r0)+,x0  [44D809]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.acc[1] = s.acc[false];
        s.setWord(R::x0, v0);
    }
    }
    // P:00084D  move a,x:(r2)+  [565A00]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(2, 1);
        s.writeX(d0, v0);
    }
L00084e:;
    // P:00084E  move x0,y:(r4)+  [4C5C00]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(4, 1);
        s.writeY(d0, v0);
    }
    // P:00084F  move x:(r0)+,x0  [44D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:000850  move x:(r0)+,x1  [45D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:000851  move x,l:(r4)+  [425C00]
    {
        const auto v0 = s.longReg(R::x);
        const auto d0 = s.post(4, 1);
        s.writeLong(d0, v0);
    }
    // P:000852  move x:(r0)+,x0  [44D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:000853  move x:(r0)+,x1  [45D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:000854  move x,l:(r4)+  [425C00]
    {
        const auto v0 = s.longReg(R::x);
        const auto d0 = s.post(4, 1);
        s.writeLong(d0, v0);
    }
    // P:000855  move #>$63f,x0  [44F400 00063F]
    {
        const auto v0 = 0x63fu;
        s.setWord(R::x0, v0);
    }
    // P:000857  move x:(r7-$1a),a  [039F9E]
    {
        const auto e0 = ((s.r[7] - 0x1au) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::a, v0);
    }
    // P:000858  tst a  [200003]
    {
        s.tst(0);
    }
    // P:000859  clr a ifmi  [202B13]
    {
        if (s.condition(C::mi)) { s.clr(0); }
    }
    // P:00085A  cmp x0,a  [200045]
    {
        s.cmp(0, State::extendWord(s.word(R::x0)));
    }
    // P:00085B  tfr x0,a ifgt  [202741]
    {
        if (s.condition(C::gt)) { s.acc[0] = State::extendWord(s.word(R::x0)); }
    }
    // P:00085C  move a,r0  [21D000]
    {
        const auto v0 = s.word(R::a);
        s.setWord(R::r0, v0);
    }
    // P:00085D  move #>$7fffff,a  [56F400 7FFFFF]
    {
        const auto v0 = 0x7fffffu;
        s.setWord(R::a, v0);
    }
    // P:00085F  move l:(r7),x  [42E700]
    {
        const auto e0 = s.post(7, 0);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:000860  move x:(r0+$1435c6),b  [0A70CF 1435C6]
    {
        const auto e0 = ((s.r[0] + 0x1435c6u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::b, v0);
    }
    // P:000862  sub b,a a,y0  [21C614]
    {
        const auto v0 = s.word(R::a);
        s.sub(0, s.acc[true]);
        s.setWord(R::y0, v0);
    }
    // P:000863  add y0,a #$6c,r0  [306C50]
    {
        const auto v0 = 0x6cu;
        s.add(0, State::extendWord(s.word(R::y0)));
        s.setWord(R::r0, v0);
    }
    // P:000864  asr a #$4,r4  [340422]
    {
        const auto v0 = 0x4u;
        s.shift(0, 0, 1, false);
        s.setWord(R::r4, v0);
    }
    // P:000865  sub x0,a ba,l:(r7)+  [4B5F44]
    {
        const auto v0 = s.longReg(R::ba);
        const auto d0 = s.post(7, 1);
        s.sub(0, State::extendWord(s.word(R::x0)));
        s.writeLong(d0, v0);
    }
    // P:000866  sub x1,b a,y0  [21C66C]
    {
        const auto v0 = s.word(R::a);
        s.sub(1, State::extendWord(s.word(R::x1)));
        s.setWord(R::y0, v0);
    }
    // P:000867  tfr x0,a #$8,x0  [240841]
    {
        const auto v0 = 0x80000u;
        s.acc[0] = State::extendWord(s.word(R::x0));
        s.setWord(R::x0, v0);
    }
    // P:000868  tfr x1,b b,y1  [21E769]
    {
        const auto v0 = s.word(R::b);
        s.acc[1] = State::extendWord(s.word(R::x1));
        s.setWord(R::y1, v0);
    }
    // P:000869  do #<$10,>$86d  [061080 00086C]
    for (unsigned loop_869=0; loop_869<16u; ++loop_869) {
    // P:00086B  mac y0,x0,a a,y:(r4)  [5E64D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(4, 0);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeY(d0, v0);
    }
    // P:00086C  mac x0,y1,b b,x:(r4)+  [575CCA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(4, 1);
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.writeX(d0, v0);
    }
    }
    // P:00086D  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:00086E  move r0,r1  [221100]
    {
        const auto v0 = s.word(R::r0);
        s.setWord(R::r1, v0);
    }
    // P:00086F  move #$ac,r2  [32AC00]
    {
        const auto v0 = 0xacu;
        s.setWord(R::r2, v0);
    }
    // P:000870  move r2,r3  [225300]
    {
        const auto v0 = s.word(R::r2);
        s.setWord(R::r3, v0);
    }
    // P:000871  move l:(r7)+,a  [48DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:000872  move l:(r7)+,b  [49DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::b, v0);
    }
    // P:000873  move l:(r7),x  [42E700]
    {
        const auto e0 = s.post(7, 0);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:000874  move x0,x:(r0)  [446000]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 0);
        s.writeX(d0, v0);
    }
    // P:000875  move x1,x:(r2)  [456200]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(2, 0);
        s.writeX(d0, v0);
    }
    // P:000876  move x:(r0+$10),x0  [024094]
    {
        const auto e0 = ((s.r[0] + 0x10u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:000877  move x:(r2+$10),x1  [024295]
    {
        const auto e0 = ((s.r[2] + 0x10u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:000878  move x,l:(r7)-  [425700]
    {
        const auto v0 = s.longReg(R::x);
        const auto d0 = s.post(7, -1);
        s.writeLong(d0, v0);
    }
    // P:000879  jsr func_00038a  [0D038A]
    f38a(s);
    // P:00087A  move b,l:(r7)-  [495700]
    {
        const auto v0 = s.longReg(R::b);
        const auto d0 = s.post(7, -1);
        s.writeLong(d0, v0);
    }
    // P:00087B  move a,l:(r7)  [486700]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(7, 0);
        s.writeLong(d0, v0);
    }
    // P:00087C  lua (r7+$3),r7  [040737]
    s.setWord(R::r7, ((s.r[7] + 0x3u) & 0xffffffu));
    // P:00087D  move #$4,r4  [340400]
    {
        const auto v0 = 0x4u;
        s.setWord(R::r4, v0);
    }
    // P:00087E  move #$6c,r0  [306C00]
    {
        const auto v0 = 0x6cu;
        s.setWord(R::r0, v0);
    }
    // P:00087F  move r0,r1  [221100]
    {
        const auto v0 = s.word(R::r0);
        s.setWord(R::r1, v0);
    }
    // P:000880  move #$ac,r2  [32AC00]
    {
        const auto v0 = 0xacu;
        s.setWord(R::r2, v0);
    }
    // P:000881  move r2,r3  [225300]
    {
        const auto v0 = s.word(R::r2);
        s.setWord(R::r3, v0);
    }
    // P:000882  move l:(r7)+,a  [48DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:000883  move l:(r7)+,b  [49DF00]
    {
        const auto e0 = s.post(7, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::b, v0);
    }
    // P:000884  move l:(r7),x  [42E700]
    {
        const auto e0 = s.post(7, 0);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:000885  move x0,x:(r0)  [446000]
    {
        const auto v0 = s.word(R::x0);
        const auto d0 = s.post(0, 0);
        s.writeX(d0, v0);
    }
    // P:000886  move x1,x:(r2)  [456200]
    {
        const auto v0 = s.word(R::x1);
        const auto d0 = s.post(2, 0);
        s.writeX(d0, v0);
    }
    // P:000887  move x:(r0+$10),x0  [024094]
    {
        const auto e0 = ((s.r[0] + 0x10u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x0, v0);
    }
    // P:000888  move x:(r2+$10),x1  [024295]
    {
        const auto e0 = ((s.r[2] + 0x10u) & 0xffffffu);
        const auto v0 = s.readX(e0);
        s.setWord(R::x1, v0);
    }
    // P:000889  move x,l:(r7)-  [425700]
    {
        const auto v0 = s.longReg(R::x);
        const auto d0 = s.post(7, -1);
        s.writeLong(d0, v0);
    }
    // P:00088A  jsr func_00038a  [0D038A]
    f38a(s);
    // P:00088B  move b,l:(r7)-  [495700]
    {
        const auto v0 = s.longReg(R::b);
        const auto d0 = s.post(7, -1);
        s.writeLong(d0, v0);
    }
    // P:00088C  move a,l:(r7)  [486700]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(7, 0);
        s.writeLong(d0, v0);
    }
    // P:00088D  lua (r7+$3),r7  [040737]
    s.setWord(R::r7, ((s.r[7] + 0x3u) & 0xffffffu));
}

inline void f350(State& s) noexcept
{
    // P:000350  move x:(r0)+,x0 y:(r4)+,y1  [F19800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.setWord(R::x0, v0);
        s.setWord(R::y1, v1);
    }
    // P:000351  move #>$fffffe,n4  [74F400 FFFFFE]
    {
        const auto v0 = 0xfffffeu;
        s.setWord(R::n4, v0);
    }
    // P:000353  move b,x1  [21E500]
    {
        const auto v0 = s.word(R::b);
        s.setWord(R::x1, v0);
    }
    // P:000354  do #<$10,>$364  [061080 000363]
    for (unsigned loop_354=0; loop_354<16u; ++loop_354) {
    // P:000356  asr #$4,b,b  [0C1C89]
    {
        s.shift(1, 1, 4, false);
    }
    // P:000357  mac x0,y1,b y:(r4)+,y1  [4FDCCA]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::y1, v0);
    }
    // P:000358  asl #$4,b,b  [0C1D89]
    {
        s.shift(1, 1, 4, true);
    }
    // P:000359  mac y1,x1,b a,x0 y:(r4)+n4,y0  [10CCFA]
    {
        const auto v0 = s.word(R::a);
        const auto e1 = s.post(4, State::sign24(s.n[4]));
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, false);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:00035A  mac -y0,x0,b a,l:(r1)+  [4859DE]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(1, 1);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, true);
        s.writeLong(d0, v0);
    }
    // P:00035B  mac x0,y1,a x:(r0)+,x0 y:(r4)+,y1  [F198C2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::x0, v0);
        s.setWord(R::y1, v1);
    }
    // P:00035C  mac x1,y0,a b,x1  [21E5E2]
    {
        const auto v0 = s.word(R::b);
        s.multiply(0, s.word(R::x1), s.word(R::y0), 1, false);
        s.setWord(R::x1, v0);
    }
    // P:00035D  asr #$4,b,b  [0C1C89]
    {
        s.shift(1, 1, 4, false);
    }
    // P:00035E  mac x0,y1,b y:(r4)+,y1  [4FDCCA]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readY(e0);
        s.multiply(1, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::y1, v0);
    }
    // P:00035F  asl #$4,b,b  [0C1D89]
    {
        s.shift(1, 1, 4, true);
    }
    // P:000360  mac y1,x1,b a,x0 y:(r4)+,y0  [10DCFA]
    {
        const auto v0 = s.word(R::a);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, false);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:000361  mac -y0,x0,b a,l:(r1)+  [4859DE]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(1, 1);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, true);
        s.writeLong(d0, v0);
    }
    // P:000362  mac x0,y1,a x:(r0)+,x0 y:(r4)+,y1  [F198C2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::x0, v0);
        s.setWord(R::y1, v1);
    }
    // P:000363  mac x1,y0,a b,x1  [21E5E2]
    {
        const auto v0 = s.word(R::b);
        s.multiply(0, s.word(R::x1), s.word(R::y0), 1, false);
        s.setWord(R::x1, v0);
    }
    }
    // P:000364  rts  [00000C]
    return;
}

inline void f365(State& s) noexcept
{
    // P:000365  move x:(r0)+,x1 y:(r4)+,y1  [F59800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.setWord(R::x1, v0);
        s.setWord(R::y1, v1);
    }
    // P:000366  move x:(r1),x0 y:(r5),y0  [C0A100]
    {
        const auto e0 = s.post(1, 0);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(5, 0);
        const auto v1 = s.readY(e1);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:000367  move #>$fffffe,n4  [74F400 FFFFFE]
    {
        const auto v0 = 0xfffffeu;
        s.setWord(R::n4, v0);
    }
    // P:000369  do #<$10,>$37b  [061080 00037A]
    for (unsigned loop_369=0; loop_369<16u; ++loop_369) {
    // P:00036B  mac -y1,x1,b b,x1 y:(r4)+,y1  [1DDCFE]
    {
        const auto v0 = s.word(R::b);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, true);
        s.setWord(R::x1, v0);
        s.setWord(R::y1, v1);
    }
    // P:00036C  mac y0,x0,a a,l:(r2)  [4862D2]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(2, 0);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeLong(d0, v0);
    }
    // P:00036D  asl #$7,a,a  [0C1D0E]
    {
        s.shift(0, 0, 7, true);
    }
    // P:00036E  mac y1,x1,b x:(r2),x0 y:(r4)+n4,y0  [D082FA]
    {
        const auto e0 = s.post(2, 0);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, State::sign24(s.n[4]));
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, false);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:00036F  mac -y0,x0,b a,l:(r1)+  [4859DE]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(1, 1);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, true);
        s.writeLong(d0, v0);
    }
    // P:000370  move l:(r2),a  [48E200]
    {
        const auto e0 = s.post(2, 0);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:000371  mac x0,y1,a x:(r1),x0 y:(r4)+,y1  [F181C2]
    {
        const auto e0 = s.post(1, 0);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::x0, v0);
        s.setWord(R::y1, v1);
    }
    // P:000372  mac x1,y0,a x:(r0)+,x1 y:(r5),y0  [C4B8E2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(5, 0);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::x1), s.word(R::y0), 1, false);
        s.setWord(R::x1, v0);
        s.setWord(R::y0, v1);
    }
    // P:000373  mac -y1,x1,b b,x1 y:(r4)+,y1  [1DDCFE]
    {
        const auto v0 = s.word(R::b);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, true);
        s.setWord(R::x1, v0);
        s.setWord(R::y1, v1);
    }
    // P:000374  mac y0,x0,a a,l:(r2)  [4862D2]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(2, 0);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeLong(d0, v0);
    }
    // P:000375  asl #$7,a,a  [0C1D0E]
    {
        s.shift(0, 0, 7, true);
    }
    // P:000376  mac y1,x1,b x:(r2),x0 y:(r4)+,y0  [F082FA]
    {
        const auto e0 = s.post(2, 0);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::y1), s.word(R::x1), 1, false);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:000377  mac -y0,x0,b a,l:(r1)+  [4859DE]
    {
        const auto v0 = s.longReg(R::a);
        const auto d0 = s.post(1, 1);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, true);
        s.writeLong(d0, v0);
    }
    // P:000378  move l:(r2),a  [48E200]
    {
        const auto e0 = s.post(2, 0);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::a, v0);
    }
    // P:000379  mac x0,y1,a x:(r1),x0 y:(r4)+,y1  [F181C2]
    {
        const auto e0 = s.post(1, 0);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, 1);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::x0), s.word(R::y1), 1, false);
        s.setWord(R::x0, v0);
        s.setWord(R::y1, v1);
    }
    // P:00037A  mac x1,y0,a x:(r0)+,x1 y:(r5),y0  [C4B8E2]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(5, 0);
        const auto v1 = s.readY(e1);
        s.multiply(0, s.word(R::x1), s.word(R::y0), 1, false);
        s.setWord(R::x1, v0);
        s.setWord(R::y0, v1);
    }
    }
    // P:00037B  rts  [00000C]
    return;
}

inline void f37c(State& s) noexcept
{
    // P:00037C  move x:(r0)+,x0 y:(r4)+n4,y0  [D09800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, State::sign24(s.n[4]));
        const auto v1 = s.readY(e1);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    // P:00037D  do #<$10,>$387  [061080 000386]
    for (unsigned loop_37d=0; loop_37d<16u; ++loop_37d) {
    // P:00037F  mac y0,x0,a a,x:(r1)+ a,y1  [1919D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, 1);
        const auto v1 = s.word(R::a);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:000380  mac -y1,y0,a x:(r2)+,x0  [44DAB6]
    {
        const auto e0 = s.post(2, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y1), s.word(R::y0), 1, true);
        s.setWord(R::x0, v0);
    }
    // P:000381  mac y0,x0,b b,x:(r3)+ b,y1  [1F1BDA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(3, 1);
        const auto v1 = s.word(R::b);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:000382  mac -y1,y0,b x:(r0)+,x0  [44D8BE]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(1, s.word(R::y1), s.word(R::y0), 1, true);
        s.setWord(R::x0, v0);
    }
    // P:000383  mac y0,x0,a a,x:(r1)+ a,y1  [1919D2]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, 1);
        const auto v1 = s.word(R::a);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:000384  mac -y1,y0,a x:(r2)+,x0  [44DAB6]
    {
        const auto e0 = s.post(2, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y1), s.word(R::y0), 1, true);
        s.setWord(R::x0, v0);
    }
    // P:000385  mac y0,x0,b b,x:(r3)+ b,y1  [1F1BDA]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(3, 1);
        const auto v1 = s.word(R::b);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, false);
        s.writeX(d0, v0);
        s.setWord(R::y1, v1);
    }
    // P:000386  mac -y1,y0,b x:(r0)+,x0 y:(r4)+n4,y0  [D098BE]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        const auto e1 = s.post(4, State::sign24(s.n[4]));
        const auto v1 = s.readY(e1);
        s.multiply(1, s.word(R::y1), s.word(R::y0), 1, true);
        s.setWord(R::x0, v0);
        s.setWord(R::y0, v1);
    }
    }
    // P:000387  move a,x:(r1)+  [565900]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, 1);
        s.writeX(d0, v0);
    }
    // P:000388  move b,x:(r3)+  [575B00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(3, 1);
        s.writeX(d0, v0);
    }
    // P:000389  rts  [00000C]
    return;
}

inline void f38a(State& s) noexcept
{
    // P:00038A  move x:(r0)+,y0  [46D800]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.setWord(R::y0, v0);
    }
    // P:00038B  do #<$10,>$394  [061080 000393]
    for (unsigned loop_38b=0; loop_38b<16u; ++loop_38b) {
    // P:00038D  move l:(r4)+,x  [42DC00]
    {
        const auto e0 = s.post(4, 1);
        const auto v0 = s.readLong(e0);
        s.setLongReg(R::x, v0);
    }
    // P:00038E  mac -y0,x0,a a,x:(r1)+ a,y0  [1819D6]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, 1);
        const auto v1 = s.word(R::a);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, true);
        s.writeX(d0, v0);
        s.setWord(R::y0, v1);
    }
    // P:00038F  mac -x1,y0,a x:(r0),y0  [46E0E6]
    {
        const auto e0 = s.post(0, 0);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::x1), s.word(R::y0), 1, true);
        s.setWord(R::y0, v0);
    }
    // P:000390  mac y0,x0,a x:(r2)+,y0  [46DAD2]
    {
        const auto e0 = s.post(2, 1);
        const auto v0 = s.readX(e0);
        s.multiply(0, s.word(R::y0), s.word(R::x0), 1, false);
        s.setWord(R::y0, v0);
    }
    // P:000391  mac -y0,x0,b b,x:(r3)+ b,y0  [1E1BDE]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(3, 1);
        const auto v1 = s.word(R::b);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, true);
        s.writeX(d0, v0);
        s.setWord(R::y0, v1);
    }
    // P:000392  mac -x1,y0,b x:(r2),y0  [46E2EE]
    {
        const auto e0 = s.post(2, 0);
        const auto v0 = s.readX(e0);
        s.multiply(1, s.word(R::x1), s.word(R::y0), 1, true);
        s.setWord(R::y0, v0);
    }
    // P:000393  mac y0,x0,b x:(r0)+,y0  [46D8DA]
    {
        const auto e0 = s.post(0, 1);
        const auto v0 = s.readX(e0);
        s.multiply(1, s.word(R::y0), s.word(R::x0), 1, false);
        s.setWord(R::y0, v0);
    }
    }
    // P:000394  move a,x:(r1)+  [565900]
    {
        const auto v0 = s.word(R::a);
        const auto d0 = s.post(1, 1);
        s.writeX(d0, v0);
    }
    // P:000395  move b,x:(r3)+  [575B00]
    {
        const auto v0 = s.word(R::b);
        const auto d0 = s.post(3, 1);
        s.writeX(d0, v0);
    }
    // P:000396  rts  [00000C]
    return;
}
}
