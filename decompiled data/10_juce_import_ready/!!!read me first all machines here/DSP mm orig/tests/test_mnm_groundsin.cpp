#include <cstdio>
#include <cstdlib>
#include "MnmGroundSin.hpp"
using namespace mnm::gs;
int main(int argc, char** argv) {
    FILE* f = fopen(argc>1?argv[1]:"m1_vectors.txt","r");
    if (!f) { printf("FAIL no vectors\n"); return 1; }
    char line[64]; long total=0, pass=0; int knob=-1;
    GroundSin* m = nullptr;
    int32_t buf[32];
    int expect = -1;
    while (fgets(line,sizeof line,f)) {
        if (line[0]=='#') {
            if (line[2]=='k') { int k; sscanf(line,"# knob %d",&k); delete m; m=new GroundSin(); m->setPitchQ23(k<<16); expect=32; }
            else if (line[2]=='f') expect=32;
            continue;
        }
        if (expect != 32) continue;
        uint32_t want[32]; want[0]=strtoul(line,0,16);
        for (int i=1;i<32;++i) { if(!fgets(line,sizeof line,f)) return 1; want[i]=strtoul(line,0,16); }
        m->processFrame(buf);
        for (int i=0;i<32;++i) { ++total; if (((uint32_t)buf[i]&0xFFFFFFu)==want[i]) ++pass; }
    }
    delete m; fclose(f);
    printf("m1 GND-SIN frame vectors: %ld/%ld bit-exact (%.2f%%)\n", pass,total,100.0*pass/(total?total:1));
    return pass==total?0:1;
}
