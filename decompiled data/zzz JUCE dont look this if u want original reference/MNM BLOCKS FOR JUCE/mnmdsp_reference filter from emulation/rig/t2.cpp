#include "MnmFilter.h"
#include "cutoff_table_1700.h"
using namespace mnmdsp;
#include <cstdio>
#include <cmath>
int main()
{
    mnmdsp::MnmSvf svf;
    svf.setG(mnmdsp::MnmCutoffTable::wordToFloat(mnmdsp::kMnmCutoffTable[1699]));
    svf.setK(0.3f);
    double e=0, ei=0;
    for (int b=0;b<64;++b)
        for (int i=0;i<16;++i){
            float x = 0.25f*float(sin(2.0*M_PI*440.0*(b*16+i)/44100.0));
            float y = svf.lp(x);
            if (b>=48){ e+=double(y)*y; ei+=double(x)*x; }
        }
    printf("svf lp gain (g=%.4f k=%.2f) = %.4f\n",
        mnmdsp::MnmCutoffTable::wordToFloat(mnmdsp::kMnmCutoffTable[1699]), 0.3f, sqrt(e/ei));
    printf("tbl[1699]=%06X tbl[849]=%06X tbl[0]=%06X\n", mnmdsp::kMnmCutoffTable[1699], mnmdsp::kMnmCutoffTable[849], mnmdsp::kMnmCutoffTable[0]);
    return 0;
}
