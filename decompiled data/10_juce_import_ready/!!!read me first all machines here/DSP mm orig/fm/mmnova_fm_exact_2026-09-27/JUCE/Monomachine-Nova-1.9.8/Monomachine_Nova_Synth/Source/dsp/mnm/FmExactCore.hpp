// FmExactCore.hpp — adapter: the bit-faithful FM+ cores (MnmFmExact.hpp) behind
// the same interface as mnm::FmCore, so NovaDSP.h can swap engines 1:1.
//
// VERIFICATION STATUS (see worklog + FM_EXACT_README.md):
//   * FM+STAT: bit-exact vs the firmware emulator, 116/116 test blocks
//   * FM+PAR:  bit-exact on 11/96 blocks, remainder within a few LSB (~-120 dB)
//   * FM+DYN:  laws transcribed from the listing (1FRQ continuous clamp,
//              2FRQ quadratic, $141880 1FEN/1VEN recursion, fixed 0.5 LPs,
//              2FB one-sample phase feedback); sample verification pending.
//
// Knob order = the machine page order:
//   STAT: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE
//   PAR:  1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE
//   DYN:  1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB  TUNE
// TUNE never enters the DSP process (the kernel adds it to the pitch word);
// here it is applied to the pitch word: (K-64)*683/128 steps of the 2048/oct
// table = (K-64)*0.0313 semitones (kernel $02C0 region law).
//
// The machine is a 44.1 kHz device: it emits 16 samples per 32-sample block
// (half rate, mono, L=R). This adapter runs one 32-sample firmware block per
// 16 host frames. At host rates other than 44.1k the pitch word is scaled by
// 44100/sr so the sounding pitch stays correct.
#pragma once
#include "MnmFmExact.hpp"
#include <array>
#include <cmath>

namespace monomachine {
namespace mnm {

enum class FmKind2 { Stat, Par, Dyn };

class FmExactCore {
public:
    void reset(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : 44100.0;
        stat.init(); par.init(); dyn.init();
        tail = 0.0f;
    }
    void noteOn(float midiNote, float /*velocity*/ = 1.0f) {
        note = midiNote;
        stat.init(); par.init(); dyn.init();      // config re-triggers the FM+ envelope
        dyn.config(knobs);                        // DYN: $2C/$2D/$2E snapshot at note-on
        hold = false;
    }
    void setEnvelope(float, float, float, float) {}   // AMP page is post-voice (kernel)
    void setEnvelopeBypass(bool) {}
    void setParameters(FmKind2 k, const std::array<float, 8>& p) {
        kind = k;
        for (int i = 0; i < 8; ++i) knobs[static_cast<size_t>(i)] = static_cast<int>(std::clamp(p[static_cast<size_t>(i)], 0.0f, 127.0f));
    }
    void setPitchMod(float semitones) { pitchMod = semitones; }

    // renders exactly 16 frames (one firmware block); the caller loops in 16s
    void processBlock(float* out, int frames) {
        (void)frames;  // must be 16
        const float tuneSemis = (knobs[7] - 64) * (683.0f / 128.0f) / 2048.0f * 12.0f;
        const float noteTotal = note + pitchMod + tuneSemis;
        int32_t pw = mnmfm::pitchWordForNote(noteTotal);
        // host-rate compensation: the firmware phase step is per-44.1k-sample
        const double scale = 44100.0 / sr;
        int64_t pwScaled = (int64_t)std::llround((double)pw * scale);
        if (pwScaled > 0x7FFFFF) pwScaled = 0x7FFFFF;
        if (pwScaled < 0) pwScaled = 0;
        int32_t out32[32];
        if (kind == FmKind2::Stat) stat.process(knobs, (int32_t)pwScaled, out32);
        else if (kind == FmKind2::Par) par.process(knobs, (int32_t)pwScaled, out32);
        else dyn.process(knobs, (int32_t)pwScaled, out32);
        // 32 words = 16 mono samples (L/R duplicated by the firmware itself)
        for (int i = 0; i < 16; ++i) {
            tail = mnmfm::s24(out32[2 * i]) / 8388608.0f;
            out[i] = tail;
        }
    }
    float lastValue() const { return tail; }

private:
    FmKind2 kind = FmKind2::Stat;
    int knobs[8] = {60, 64, 80, 30, 80, 64, 98, 64};
    mnmfm::FmStatVoice stat;
    mnmfm::FmParVoice par;
    mnmfm::FmDynVoice dyn;
    double sr = 44100.0;
    float note = 60.0f, pitchMod = 0.0f, tail = 0.0f;
    bool hold = false;
};

}  // namespace mnm
}  // namespace monomachine
