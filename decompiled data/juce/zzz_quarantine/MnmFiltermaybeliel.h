#pragma once
#include <algorithm>
#include <cstdint>
#include "MnmFilterTables.h"

// MnmFilterTruthful.h
//
// ЧЕСТНЫЙ КОР: exact raw cutoff tables + fixed-point two-integrator model.
//
// This is deliberately NOT advertised as a bit-exact replacement for the full
// Monomachine FILT page. The original listing proves the table reads, the
// block cadence and the presence of two filter loops. It does not, in the
// available material, prove the complete ColdFire UI mapping, the exact HPQ /
// LPQ curve, or whether the two loops are serial sections or channel paths.
// Those values must be supplied to this class as raw kernel values.
//
// Exact inputs accepted by this class:
//   cutoffIndex: an index already produced by the original kernel
//   resonance:   a raw Q1.23 kernel coefficient, not a UI value 0..127
//
// The recurrence below is a fixed-point reconstruction of the documented
// two-integrator SVF shape. It is useful and deterministic, but the recurrence
// ordering is marked MODEL because a complete accepted instruction-level
// translation of P:$07E7-$07FB is not present in the source folder.
namespace mmnova_truthful {

using Word = std::int32_t;
using Acc  = std::int64_t;

constexpr Word kMax = 0x7FFFFF;
constexpr Word kMin = -0x800000;

inline Word sign24(std::uint32_t x) noexcept
{
    x &= 0xFFFFFFu;
    return (x & 0x800000u) ? static_cast<Word>(x | 0xFF000000u)
                           : static_cast<Word>(x);
}

inline Word sat24(Acc x) noexcept
{
    if (x > kMax) return kMax;
    if (x < kMin) return kMin;
    return static_cast<Word>(x);
}

// Q1.23 word multiplication. The DSP keeps a wider accumulator; this helper
// rounds only when a state/output word is materialised.
inline Word mulQ(Word a, Word b) noexcept
{
    return sat24((static_cast<Acc>(a) * static_cast<Acc>(b)) >> 23);
}

// P:$056D-$0572 shape: delta is shifted left by 8, rounded, then shifted back.
// This gives one coefficient update per 16-sample block.
inline Word roundDelta256(Word delta) noexcept
{
    if (delta >= 0)
        return static_cast<Word>((static_cast<Acc>(delta) + 0x80) & ~Acc(0xFF));
    const Acc a = -static_cast<Acc>(delta);
    return static_cast<Word>(-((a + 0x80) & ~Acc(0xFF)));
}

enum class Table : std::uint8_t { Primary, Secondary };
enum class Output : std::uint8_t { LowPass, BandPass, HighPass };

inline Word cutoffWord(Table table, int index) noexcept
{
    if (table == Table::Primary)
    {
        index = std::clamp(index, 0, kCutoffP143546Count - 1);
        return sign24(kCutoffP143546[index]);
    }
    index = std::clamp(index, 0, kCutoffP1435C6Count - 1);
    return sign24(kCutoffP1435C6[index]);
}

struct RawKernelFrame
{
    Table  table = Table::Primary;
    int    cutoffIndex = 0; // already in the original table's index domain
    Word   resonance = 0;  // already prepared by the original kernel, Q1.23
    Output output = Output::LowPass;
};

class MnmFilterTruthfulCore
{
public:
    void reset() noexcept
    {
        m_g = 0;
        m_targetG = 0;
        m_k = 0;
        m_low = 0;
        m_band = 0;
        m_mode = Output::LowPass;
    }

    // No UI curve is hidden here. The caller supplies a kernel table index.
    void setRawFrame(RawKernelFrame frame) noexcept
    {
        m_targetG = cutoffWord(frame.table, frame.cutoffIndex);
        m_k = frame.resonance;
        m_mode = frame.output;
    }

    // Alternative for a caller that already captured the kernel's g word.
    void setRawCoefficient(Word g, Word resonance, Output output = Output::LowPass) noexcept
    {
        m_targetG = g;
        m_k = resonance;
        m_mode = output;
    }

    // One coefficient update per DSP block, matching the proven cadence.
    void beginBlock() noexcept
    {
        m_g = sat24(static_cast<Acc>(m_g) + roundDelta256(m_targetG - m_g));
    }

    Word processSample(Word input) noexcept
    {
        // MODEL: transparent fixed-point two-integrator SVF recurrence.
        const Word high = sat24(static_cast<Acc>(input)
                              - m_low
                              - mulQ(m_k, m_band));
        m_band = sat24(static_cast<Acc>(m_band) + mulQ(m_g, high));
        m_low  = sat24(static_cast<Acc>(m_low) + mulQ(m_g, m_band));

        switch (m_mode)
        {
            case Output::HighPass: return high;
            case Output::BandPass: return m_band;
            case Output::LowPass:  return m_low;
        }
        return m_low;
    }

    void processBlock16(Word* samples) noexcept
    {
        beginBlock();
        for (int i = 0; i < 16; ++i)
            samples[i] = processSample(samples[i]);
    }

    Word currentCoefficient() const noexcept { return m_g; }
    Word targetCoefficient() const noexcept { return m_targetG; }
    Word resonance() const noexcept { return m_k; }

private:
    Word m_g = 0;
    Word m_targetG = 0;
    Word m_k = 0;
    Word m_low = 0;
    Word m_band = 0;
    Output m_mode = Output::LowPass;
};

} // namespace mmnova_truthful
