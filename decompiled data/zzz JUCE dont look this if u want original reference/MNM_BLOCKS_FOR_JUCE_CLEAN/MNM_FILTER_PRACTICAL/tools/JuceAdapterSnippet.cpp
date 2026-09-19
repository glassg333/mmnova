// Example adapter for a JUCE AudioProcessor/SynthesiserVoice.
// This file is intentionally not compiled here because JUCE is not part of the
// evidence/practical package.
#include "MnmFilterFromClues.h"

class FilterAdapter
{
public:
    void prepareToPlay(double sampleRate, int /*maxBlockSize*/) noexcept
    {
        filter.prepare(sampleRate);
        filter.setNoteHz(261.625565); // replace from the current MIDI note
        filter.setParams(params);
    }

    void noteOn(int midiNote) noexcept
    {
        filter.setNoteHz(440.0 * std::exp2((midiNote - 69) / 12.0));
        filter.trigger();
    }

    void processBlock(float* mono, int numSamples) noexcept
    {
        // The implementation updates controls on each native 16-sample block.
        // It also accepts a non-multiple-of-16 host buffer.
        filter.processBlock(mono, numSamples);
    }

    mmnova_practical::Params params{};
    mmnova_practical::MnmFilterFromClues filter;
};
