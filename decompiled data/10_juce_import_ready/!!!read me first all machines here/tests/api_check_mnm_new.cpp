// api_check_mnm_new.cpp — проверка, что ВСЕ вызовы API из 00_PATCH_NOTES.md
// существуют с точными сигнатурами (симуляция правок NovaDSP.h без JUCE).
#include <cstdint>
#include "dsp/mnm_new/distortion/MnmDistExact.hpp"
#include "dsp/mnm_new/filter/MnmFilterExact.hpp"
#include "dsp/mnm_new/envelope/MnmEnvExact.hpp"

using namespace monomachine;

int main()
{
    // --- TrackChain поля (шаги 3.2 / 4.2) ---
    mnm::DistExactCore distEx;
    mnm::FilterExactCore filtEx;

    // --- prepare (4.3) ---
    distEx.reset();
    filtEx.reset();
    filtEx.setSampleRate(44100.0);

    // --- trigger (3.3) ---
    distEx.retrig();

    // --- TrackChain::set (4.4) ---
    std::array<float,32> p{};
    filtEx.setParameters(p[16],p[17],p[18],p[19]);            // 4 аргумента
    filtEx.setParameters(p[16],p[17],p[18],p[19],p[20],p[21],p[22]-64,p[23]-64); // 8 аргументов

    // --- DIST ветка process (3.4) ---
    float l=0.1f, r=-0.1f;
    distEx.setKnob(p[12]); distEx.setVol(127.0f);
    distEx.processStereo(l,r);

    // --- FILT ветка process (4.5) ---
    float l2 = filtEx.process(0,l);
    float r2 = filtEx.process(1,r);
    (void)l2; (void)r2;

    // --- мод-матрица словами каждый сэмпл (4.5 комментарий) ---
    filtEx.setWords(mnm::exactWordFromKnob(64), mnm::exactWordFromKnob(32),
                    mnm::exactWordFromKnob(10), mnm::exactWordFromKnob(10));

    // --- AmpEnvelope (5.2) ---
    mnm::EnvExactCore exactEnv;
    exactEnv.reset();
    exactEnv.setParameters(p[0],p[1],p[2],p[3],120.0f);       // + tempo
    exactEnv.noteOn(); exactEnv.noteOff(); exactEnv.kill();
    float g = exactEnv.process(); (void)g;
    int phase = exactEnv.phase(); (void)phase;

    // --- диагностика (шапки/тултипы) ---
    (void)distEx.drivePhase(); (void)distEx.driveRaw(); (void)distEx.timbreIndex();
    (void)filtEx.bypassed(); (void)filtEx.ringWords();
    (void)exactEnv.levelSigned();

    // --- пространства имён таблиц разведены (один TU без конфликтов) ---
    (void)mnm::exact_tables::filt::kDiv1[0];
    (void)mnm::exact_tables::env::kEnvAtk[0];
    (void)mnm::exact_tables::dist::kDistAtk[0];
    return 0;
}
