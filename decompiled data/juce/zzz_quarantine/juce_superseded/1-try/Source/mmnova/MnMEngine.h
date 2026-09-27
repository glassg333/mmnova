// ============================================================================
//  MnMEngine.h — 6 треков Monomachine + роутинг FX (биты X:$2C4 = INP).
//  Единственный класс, который нужен PluginProcessor'у.
// ============================================================================
#pragma once
#include "MnMVoice.h"
#include "MnMMachineFactory.h"
#include <array>

namespace mmnova {

constexpr int kNumTracks = 6;

class Engine
{
public:
    Engine();

    void prepare (double hostSampleRate);
    void reset();

    // --- управление ---------------------------------------------------------
    void setTrackMachine (int track, MachineId id);
    MachineId trackMachine (int track) const { return trackId[(size_t) track]; }
    void setTrackParam (int track, int paramIdx, int value7);
    int  getTrackParam (int track, int paramIdx) const;
    Voice& voice (int track) { return voices[(size_t) track]; }

    // Источник входа FX-машины трека (по умолчанию — предыдущий трек, как на железе)
    void setFxSource (int track, int sourceTrack) { fxSrc[(size_t) track] = sourceTrack; }

    void noteOn  (int track, int note, int velocity);
    void noteOff (int track, int note);
    void allNotesOff();

    // --- рендер -------------------------------------------------------------
    // Универсальный float-вход/выход. Внутри — 44 100 Гц, блоки по 16 кадров.
    void render (float* left, float* right, int numFrames);

    // JUCE-обёртка (без зависимости от JUCE в остальном модуле)
   #if defined (JUCE_VERSION) || defined (JUCE_APP_VERSION)
    template <typename BufferT, typename MidiT>
    void process (BufferT& buffer, MidiT& midi)
    {
        int pos = 0;
        for (const auto meta : midi)
        {
            const int t = meta.samplePosition;
            if (t > pos) { renderInto (buffer, pos, t - pos); pos = t; }
            const auto msg = meta.getMessage();
            const int ch = std::min (msg.getChannel() - 1, kNumTracks - 1);
            if (msg.isNoteOn())       noteOn  (ch, msg.getNoteNumber(), msg.getVelocity());
            else if (msg.isNoteOff()) noteOff (ch, msg.getNoteNumber());
            else if (msg.isController())
                setTrackParam (ch, (msg.getControllerNumber() - 16) & 7, msg.getControllerValue());
        }
        renderInto (buffer, pos, buffer.getNumSamples() - pos);
    }
    template <typename BufferT>
    void renderInto (BufferT& b, int start, int n)
    {
        if (n <= 0) return;
        float* L = b.getWritePointer (0) + start;
        float* R = b.getNumChannels() > 1 ? b.getWritePointer (1) + start : L;
        render (L, R, n);
    }
   #endif

private:
    void renderEngineBlock();       // ровно kBlockFrames кадров на 44.1 кГц

    std::array<Voice, kNumTracks>       voices;
    std::array<MachineId, kNumTracks>   trackId;
    std::array<int, kNumTracks>         fxSrc;
    std::array<std::array<s24, kBlockFrames>, kNumTracks> trackOut;  // для роутинга FX

    s24 blkL[kBlockFrames] {}, blkR[kBlockFrames] {};
    int blkPos { kBlockFrames };

    double hostRate { kEngineRate };
    double resampPos { 0.0 };
    float  lastL { 0 }, lastR { 0 }, curL { 0 }, curR { 0 };
};

} // namespace mmnova
