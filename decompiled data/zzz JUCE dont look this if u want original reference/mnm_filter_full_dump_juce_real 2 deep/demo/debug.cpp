// Debug: сравнить первые 8 сэмплов поодиночке
#include "../src/MnmFilter.h"
#include "../src/MnmFilterFaithful.h"
#include <cstdio>
#include <cmath>

int main() {
    using namespace mnm;
    constexpr float kFS = 44100.0f;
    constexpr int TONE = 127;

    MnmOnePole lp_float;
    lp_float.setCutoffFromTable(TONE);
    float a = lp_float.coef();
    printf("TONE=%d, a=%.6f (q.23 = %d, hex = 0x%06X)\n\n", TONE, a, kLP_filter_coeffs[TONE], kLP_filter_coeffs[TONE] & 0xFFFFFF);

    MnmOnePoleFaithful lp_fxp;
    lp_fxp.setTone(TONE);
    printf("  n  input_f  input_q23     out_f  out_fxp   diff\n");
    printf("  --  -------  --------    -------  --------  -----\n");

    for (int i = 0; i < 1000; ++i) {
        float in_f = 0.8f * std::sin(2.0f * 3.14159265f * 100.0f * i / kFS);
        float outf = lp_float.processLP(in_f);

        fxp::Word in_q = fxp::from_float(in_f);
        fxp::Word outq = lp_fxp.processOne(in_q);
        float out_fxp_f = fxp::to_float(outq);

        float diff = outf - out_fxp_f;
        // Print every 100 samples or where diff is significant
        if (i >= 48 && i <= 80) {
            printf("  %4d  in=%+7.4f  out_f=%+7.4f  out_fxp=%+7.4f  diff=%+9.6f  state=0x%06X\n",
                   i, in_f, outf, out_fxp_f, diff, lp_fxp.stateA());
        }
    }
    return 0;
}
