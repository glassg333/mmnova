// =============================================================================
// TRY4 — частный импорт Package-5 FM/AMP frame для FM+STAT/PAR/DYN.
//
// Этот файл и все его зависимости находятся только в fm_try4_voice. Они не
// включают m6/fm_mnm_frq_env_fix, retained MNM/OLD/NEW/FIX или Fix-5. Package-5
// даёт собственные ядра, ROM, AMP-огибающую, gain/pan-кольцо, kernel pitch и
// 16-семпловую каденцию. Try4VoicePageMap.hpp фиксирует используемые слова
// FM/AMP-части страницы. Полный Package-8 MnmVoiceFrame tail (FILT/EQ/delay/
// stage-2) намеренно не исполняется здесь строковым DSP-интерпретатором:
// это был бы realtime CPU-risk и ложное обещание полного voice-frame порта.
//
// Особенности оригинального кадра:
//   * PROC выдаёт 32 слова = 16 L/R-пар; наружу выпускаются ровно 16 кадров.
//   * AMP — P:$088E-$08FA: ATK/HOLD/DEC/REL/KILL, VOL² и sin/cos PAN.
//   * Pitch — 48-битное слово ядра с TUNE и модуляцией, включая A=0.
//
// Имена внутренних классов сохранены от исходного Package-5 только внутри
// private namespace try4voicefm; это отдельные определения и отдельное
// состояние TRY4, а не ссылка на активный путь другой FM-кандидатуры.
// =============================================================================
#pragma once

#include "Try4VoiceKernel.hpp"

#include "Try4VoiceFmDsp.hpp"
#include "Try4VoiceFmPar.hpp"
#include "Try4VoiceFmDyn.hpp"
#include "Try4VoiceFmStat.hpp"
#include "Try4VoiceAmpEnv.hpp"
#include "Try4VoicePageMap.hpp"

#include <array>
#include <algorithm>
#include <cmath>

namespace monomachine {
namespace fm_try4_voice {

enum class FmKind { Stat, Par, Dyn };

// Parameter order is the hardware order from the ColdFire descriptors:
//  STAT: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE   (defaults 3C 40 50 1E 50 40 62 40)
//  PAR:  1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE  (defaults 3C 40 50 40 66 50 62 40)
//  DYN:  1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB  TUNE   (defaults 40 40 40 40 4A 50 1E 40)

// Ratio table Y:$141A80, 24 entries (raw/0x80000), dumped from dsp1_pmem.bin.
inline constexpr std::array<float, 24> kFmRatioExact = {
    0.03125f, 0.0625f, 0.125f, 0.1875f, 0.25f, 0.3125f,
    0.375f, 0.5f, 0.625f, 0.75f, 0.875f, 1.0f,
    1.25f, 1.5f, 1.75f, 2.0f, 2.5f, 3.0f,
    3.5f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
};

class FmCore {
public:
    void reset(double sampleRate) {
        sr = sampleRate > 0.0 ? sampleRate : kDspRate;
        parCore.init();
        dynCore.init();
        statCore.init();
        fifoLen = 0;
        fifoPos = 0;
        env.reset();
        gain.reset();
        frameQueueLen = 0;
        frameQueuePos = 0;
        lastOutL = lastOutR = 0.0f;
        pendingTrig = 0;
        pendingInit = false;
        voicePageState = {};
    }

    void noteOn(float midiNote, float velocity = 1.0f) {
        note = midiNote;
        vel = velocity;
        pendingTrig = 1;                      // TRIG word 1 (consumed per frame)
        voicePageState.triggerWord = 1;
        pendingInit = true;                   // machine INIT on the note-on frame
        fifoLen = 0;
        fifoPos = 0;
        frameQueueLen = 0;
        frameQueuePos = 0;
    }

    void noteOff() { pendingTrig = 2; voicePageState.triggerWord = 2; } // TRIG word 2 -> REL
    void kill()    { pendingTrig = 3; voicePageState.triggerWord = 3; } // TRIG word 3 -> KILL

    void setEnvelopeBypass(bool bypass) { envelopeBypass = bypass; }
    bool envelopeBypassed() const { return envelopeBypass; }

    // FM machine page: the 8 raw knob values 0..127.
    void setParameters(FmKind k, const std::array<float, 8>& p) {
        kind = k;
        params = p;
        for (int i = 0; i < 8; ++i)
            knobWords[(size_t)i] = (uint32_t)std::clamp(p[(size_t)i], 0.0f, 127.0f);
        voicePageState.setMachine(p);
    }

    // AMP page (raw 0..127): ATK HOLD DEC REL VOL PAN. DIST/PORT are part of
    // the full upstream voice frame (DIST drive, PORT glide) and are not used here.
    void setAmpParams(int atk, int hold, int dec, int rel, int vol, int pan) {
        ampAtk = std::clamp(atk, 0, 127);
        ampHold = std::clamp(hold, 0, 127);
        ampDec = std::clamp(dec, 0, 127);
        ampRel = std::clamp(rel, 0, 127);
        ampVol = std::clamp(vol, 0, 127);
        ampPan = std::clamp(pan, 0, 127);
        voicePageState.setAmp(ampAtk, ampHold, ampDec, ampRel, ampVol, ampPan);
    }

    // Y[P+$23] word. The OS writes TickRecip = 0x800000/tick (tick = 24*BPM);
    // the HOLD phase scales by this word exactly as the firmware does.
    void setTempoWord(uint32_t w) { tempoWord = w; voicePageState.tempoWord = w; }

    // Pitch-mod source: semitones (-64..+64). Converted to the w30 page word
    // (one w30 step = 33/16 semitones; rest = 64).
    void setPitchMod(float semitones) { pitchMod = semitones; }

    // Direct pitch-word override for the exact cores (the kernel pitch word,
    // the FULL 48-bit accumulator A on PROC entry — a1:a0; the fractional low
    // word participates in the machine's $0BE37C*A inc multiply). The original
    // firmware accepts A=0 = frozen carrier — use pitchWordValid to enable it.
    void setPitchWordOverride(uint64_t w, bool valid = true) {
        pitchWordOverride = w; pitchWordValid = valid;
    }

    // Renders frames; mono convenience output = (L+R)/2 like a collapsed pair.
    void processBlock(float* out, int frames) {
        float tmpL[kMaxFrames];
        float tmpR[kMaxFrames];
        for (int i = 0; i < frames; i += kMaxFrames) {
            const int n = std::min(frames - i, kMaxFrames);
            processBlockStereo(tmpL, tmpR, n);
            for (int j = 0; j < n; ++j)
                out[i + j] = 0.5f * (tmpL[j] + tmpR[j]);
        }
    }

    // Stereo render: LEFT = cos channel, RIGHT = sin channel (PAN 0 = hard
    // left, as on the hardware: cos[1] = full, sin[0] = 0 at PAN word 0).
    void processBlockStereo(float* outL, float* outR, int frames) {
        int done = 0;
        while (done < frames) {
            if (frameQueueLen == 0)
                renderFrame();                    // one 16-sample track frame
            const int n = std::min(frames - done, frameQueueLen);
            for (int i = 0; i < n; ++i) {
                outL[done + i] = queueL[frameQueuePos + i];
                outR[done + i] = queueR[frameQueuePos + i];
            }
            frameQueuePos += n;
            frameQueueLen -= n;
            if (frameQueueLen == 0) frameQueuePos = 0;
            done += n;
        }
    }

    float lastValue() const { return lastOutL; }
    float lastValueR() const { return lastOutR; }
    const VoicePageMap& voicePageMap() const noexcept { return voicePageState; }

    // Exposed for verification/tests.
    const try4voicefm::MnmAmpEnv& envState() const { return env; }
    int32_t envLevel() const { return env.level; }
    int envPhase() const { return env.phase; }

private:
    // ------------------------------------------------------- one 16-sample
    // track frame: CONF+PROC on the bit-exact core (32 words = 16 L/R pairs),
    // envelope tick, gain ring, and the *16 unique samples* queued out.
    void renderFrame() {
        // TRIG is consumed at the TOP of the firmware state machine, then the
        // phase step runs within the same call — so the note-on frame already
        // applies the first attack increment (verified on the emulator:
        // frame 0 of a gate shows level = tblA[ATK]).
        const int trig = pendingTrig;
        pendingTrig = 0;
        if (trig != 0) env.trig(trig);
        env.tick(ampAtk, ampHold, ampDec, ampRel, int(tempoWord));

        // machine INIT on the note-on frame (the kernel calls INIT from the
        // dispatch table when a track triggers)
        if (pendingInit) {
            parCore.init();
            dynCore.init();
            statCore.init();
            pendingInit = false;
        }

        // pitch word A (kernel law), unless overridden
        uint64_t A;
        if (pitchWordValid) {
            A = pitchWordOverride;
        } else {
            const int glide = (int(note) << 11) / 12;
            const int tuneRaw = int(knobWords[7]) - 64;   // TUNE = knob 8, bipolar
            const int w30 = 64 + int(pitchMod * 16.0f / 33.0f);
            A = (uint64_t)try4voicefm::MnmPitchChain::computeA48(glide, tuneRaw, w30)
                & 0xFFFFFFFFFFFFull;
        }

        voicePageState.pitchWord = A;
        uint32_t fifoBuf[32];
        if (kind == FmKind::Par) {
            parCore.conf(knobWords.data());
            parCore.proc(A, fifoBuf);
        } else if (kind == FmKind::Dyn) {
            dynCore.conf(knobWords.data());
            dynCore.proc(A, fifoBuf);
        } else {
            statCore.conf(knobWords.data());
            statCore.proc(A, fifoBuf);
        }

        // envelope gain ring (or unity when bypassed: raw core output)
        int32_t ring[32];
        if (!envelopeBypass) {
            gain.render(env.level, ampVol, ampPan,
                        try4voicefm::MnmPanTables::sinTable(),
                        try4voicefm::MnmPanTables::cosTable(), ring);
        } else {
            for (int i = 0; i < 16; ++i) { ring[2*i] = ring[2*i+1] = 0x7FFFFF; }
        }

        // 16 unique samples (de-interleave the duplicated L/R pairs),
        // Q23 -> float normalised to the measured default-knob peak
        // (2.14 Q23 for all 3 machines), then the per-frame gain.
        for (int i = 0; i < 16; ++i) {
            const int32_t sw = (int32_t(fifoBuf[2 * i] << 8) >> 8);
            const float s = float(sw) * (1.0f / 17949485.0f);
            queueL[i] = s * float(ring[2*i]) * (1.0f / 8388608.0f);
            queueR[i] = s * float(ring[2*i+1]) * (1.0f / 8388608.0f);
        }
        lastOutL = queueL[15];
        lastOutR = queueR[15];
        frameQueueLen = 16;
        frameQueuePos = 0;
    }

public:
    static constexpr int kFrameSamples = 16;
    static constexpr int kMaxFrames = 64;

private:
    FmKind kind = FmKind::Stat;
    std::array<float, 8> params{};
    std::array<uint32_t, 8> knobWords{};

    try4voicefm::MnmFmPar parCore;
    try4voicefm::MnmFmDyn dynCore;
    try4voicefm::MnmFmStat statCore;

    try4voicefm::MnmAmpEnv env;
    try4voicefm::MnmGainPath gain;
    bool pendingInit = false;
    int pendingTrig = 0;

    int ampAtk = 0, ampHold = 0, ampDec = 127, ampRel = 0;
    int ampVol = 127, ampPan = 64;
    uint32_t tempoWord = 120;

    float note = 60.0f, vel = 1.0f, pitchMod = 0.0f;
    uint64_t pitchWordOverride = 0;
    bool pitchWordValid = false;
    bool envelopeBypass = false;
    // Separate Package-5 page state: the runtime route writes raw words via
    // the imported FM/AMP page map rather than borrowing m6 state.
    VoicePageMap voicePageState{};

    uint32_t fifoBuf[32]{};
    int fifoLen = 0, fifoPos = 0;

    float queueL[16]{}, queueR[16]{};
    int frameQueueLen = 0, frameQueuePos = 0;
    float lastOutL = 0.0f, lastOutR = 0.0f;
    double sr = kDspRate;
};

}  // namespace fm_try4_voice
}  // namespace monomachine
