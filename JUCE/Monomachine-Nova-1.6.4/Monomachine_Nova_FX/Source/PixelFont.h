#pragma once
#include <JuceHeader.h>
namespace pixel {
inline std::array<uint8_t,7> glyph(juce::juce_wchar c){
    switch(c){
#define G(C,A,B,D,E,F,H,I) case C:return {{A,B,D,E,F,H,I}};
        G('A',14,17,17,31,17,17,17) G('B',30,17,17,30,17,17,30) G('C',15,16,16,16,16,16,15)
        G('D',30,17,17,17,17,17,30) G('E',31,16,16,30,16,16,31) G('F',31,16,16,30,16,16,16)
        G('G',15,16,16,23,17,17,15) G('H',17,17,17,31,17,17,17) G('I',31,4,4,4,4,4,31)
        G('J',7,2,2,2,18,18,12) G('K',17,18,20,24,20,18,17) G('L',16,16,16,16,16,16,31)
        G('M',17,27,21,21,17,17,17) G('N',17,25,21,19,17,17,17) G('O',14,17,17,17,17,17,14)
        G('P',30,17,17,30,16,16,16) G('Q',14,17,17,17,21,18,13) G('R',30,17,17,30,20,18,17)
        G('S',15,16,16,14,1,1,30) G('T',31,4,4,4,4,4,4) G('U',17,17,17,17,17,17,14)
        G('V',17,17,17,17,17,10,4) G('W',17,17,17,21,21,21,10) G('X',17,17,10,4,10,17,17)
        G('Y',17,17,10,4,4,4,4) G('Z',31,1,2,4,8,16,31)
        G('0',31,17,19,21,25,17,31) G('1',4,12,4,4,4,4,14) G('2',30,1,1,14,16,16,31)
        G('3',30,1,1,14,1,1,30) G('4',18,18,18,31,2,2,2) G('5',31,16,16,30,1,1,30)
        G('6',15,16,16,30,17,17,14) G('7',31,1,2,4,8,8,8) G('8',14,17,17,14,17,17,14) G('9',14,17,17,15,1,1,30)
        G('-',0,0,0,31,0,0,0) G('+',0,4,4,31,4,4,0) G('.',0,0,0,0,0,12,12)
        G(':',0,12,12,0,12,12,0) G('/',1,2,2,4,8,8,16) G('[',14,8,8,8,8,8,14) G(']',14,2,2,2,2,2,14)
        G('(',2,4,8,8,8,4,2) G(')',8,4,2,2,2,4,8) G('%',17,2,4,4,8,16,17) G('?',14,17,1,2,4,0,4)
        G('>',16,8,4,2,4,8,16) G('<',1,2,4,8,4,2,1) G('_',0,0,0,0,0,0,31)
#undef G
        default:return {};}
}
inline void text(juce::Graphics& g,const juce::String& string,juce::Rectangle<int> r,int height=21,bool centre=false){
    auto s=string.toUpperCase();int scale=std::max(1,height/7);
    while(scale>1&&s.length()*6*scale>r.getWidth())--scale;
    const int width=s.length()*6*scale-scale;
    int x=centre?r.getCentreX()-width/2:r.getX();int y=r.getCentreY()-7*scale/2;
    for(auto c:s){auto bits=glyph(c);for(int row=0;row<7;++row)for(int col=0;col<5;++col)if(bits[static_cast<size_t>(row)]&(1<<(4-col)))g.fillRect(x+col*scale,y+row*scale,scale,scale);x+=6*scale;}
}
inline void dotted(juce::Graphics& g,int x1,int y1,int x2,int y2){if(x1==x2){for(int y=y1;y<y2;y+=6)g.fillRect(x1,y,2,3);}else{for(int x=x1;x<x2;x+=6)g.fillRect(x,y1,3,2);}}
}
