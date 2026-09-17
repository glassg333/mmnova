// ============================================================================
//  MnMEngine.cpp
// ============================================================================
#include "MnMEngine.h"

namespace mmnova {

Engine::Engine()
{
    tables::init();
    for (int t = 0; t < kNumTracks; ++t)
    {
        trackId[(size_t) t] = MachineId::GND_SIN;
        fxSrc[(size_t) t]   = (t + kNumTracks - 1) % kNumTracks;   // вход от соседа
        voices[(size_t) t].setMachine (createMachineWithDefaults (MachineId::GND_SIN));
    }
}

void Engine::prepare (double hostSampleRate)
{
    hostRate = hostSampleRate > 0.0 ? hostSampleRate : kEngineRate;
    for (int t = 0; t < kNumTracks; ++t)
        voices[(size_t) t].prepare (kEngineRate, t);   // движок всегда 44.1 кГц
    blkPos = kBlockFrames; resampPos = 0.0;
}

void Engine::reset()
{
    for (auto& v : voices) v.noteOff();
    blkPos = kBlockFrames;
}

void Engine::setTrackMachine (int track, MachineId id)
{
    if ((unsigned) track >= kNumTracks) return;
    trackId[(size_t) track] = id;
    voices[(size_t) track].setMachine (createMachineWithDefaults (id));
    voices[(size_t) track].prepare (kEngineRate, track);
}

void Engine::setTrackParam (int track, int paramIdx, int value7)
{
    if ((unsigned) track >= kNumTracks) return;
    if (auto* m = voices[(size_t) track].machine()) m->setParam (paramIdx, value7);
}

int Engine::getTrackParam (int track, int paramIdx) const
{
    if ((unsigned) track >= kNumTracks) return 0;
    const auto* m = const_cast<Engine*> (this)->voices[(size_t) track].machine();
    return m ? m->getParam (paramIdx) : 0;
}

void Engine::noteOn (int track, int note, int velocity)
{
    if ((unsigned) track >= kNumTracks) return;
    voices[(size_t) track].noteOn (note, velocity);
}

void Engine::noteOff (int track, int note)
{
    if ((unsigned) track >= kNumTracks) return;
    if (voices[(size_t) track].currentNote() == note) voices[(size_t) track].noteOff();
}

void Engine::allNotesOff() { for (auto& v : voices) v.noteOff(); }

void Engine::renderEngineBlock()
{
    for (int i = 0; i < kBlockFrames; ++i) { blkL[i] = 0; blkR[i] = 0; }

    // Порядок как на железе: сначала источники, затем FX-треки читают буфер соседа.
    for (int pass = 0; pass < 2; ++pass)
        for (int t = 0; t < kNumTracks; ++t)
        {
            const auto* d = descriptorFor (trackId[(size_t) t]);
            const bool isFx = d && d->isFx;
            if (isFx != (pass == 1)) continue;

            s24 l[kBlockFrames] {}, r[kBlockFrames] {};
            const s24* in = isFx ? trackOut[(size_t) fxSrc[(size_t) t]].data() : nullptr;
            voices[(size_t) t].renderBlock (l, r, in);

            for (int i = 0; i < kBlockFrames; ++i)
            {
                trackOut[(size_t) t][(size_t) i] = sat24 (((s56) l[i] + r[i]) >> 1);
                blkL[i] = sat24 ((s56) blkL[i] + (l[i] >> 1));
                blkR[i] = sat24 ((s56) blkR[i] + (r[i] >> 1));
            }
        }
    blkPos = 0;
}

void Engine::render (float* left, float* right, int numFrames)
{
    const double ratio = kEngineRate / hostRate;      // шаг движка на кадр хоста

    for (int n = 0; n < numFrames; ++n)
    {
       #if defined (MMNOVA_NATIVE_RATE_ONLY)
        if (blkPos >= kBlockFrames) renderEngineBlock();
        left[n]  = toFloat (blkL[blkPos]);
        right[n] = toFloat (blkR[blkPos]);
        ++blkPos;
       #else
        resampPos += ratio;
        while (resampPos >= 1.0)
        {
            resampPos -= 1.0;
            if (blkPos >= kBlockFrames) renderEngineBlock();
            lastL = curL; lastR = curR;
            curL = toFloat (blkL[blkPos]); curR = toFloat (blkR[blkPos]);
            ++blkPos;
        }
        const float f = (float) resampPos;
        left[n]  = lastL + (curL - lastL) * f;
        right[n] = lastR + (curR - lastR) * f;
       #endif
    }
}

} // namespace mmnova
