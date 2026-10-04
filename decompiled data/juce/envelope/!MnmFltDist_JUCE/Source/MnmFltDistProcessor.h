// ============================================================================
// MnmFltDistProcessor.h — JUCE-обвязка тракта FLT+DIST (Monomachine DSP1)
// mnm_19. ОБВЯЗКА переписана начисто после сбоя воркспейса (06.10.2026):
// DSP-блоки — оригинальные бит-точные (см. шапки файлов Source/), здесь
// только маппинг страниц ячеек и потоковый ввод. Параметры — СЫРЫЕ СЛОВА
// страниц (knob<<16), как в ядре. mnm_19: (1) добавлен параметр DIST
// (слово Y:$404, линейно; folding ступени активен при слове ≥ $400000
// = v ≥ 64, см. mnm_dist_stage.h); (2) стерео-шина идёт через
// pushStereo() — один счётчик кадра на пару L/R (фикс двойного инкремента
// cnt_, найден при разборе импорта в Nova, 2026-10-04).
// ============================================================================
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <array>
#include "MnmFltDistVoice.h"

class MnmFltDistProcessor : public juce::AudioProcessor
{
public:
    MnmFltDistProcessor()
        : AudioProcessor(BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    {
        // страницы ячеек (CONTROL_MODS_MAP_RU §3, пак 14):
        // FLT CC72..79, AMP CC56..59, EFFX CC80..83, DIST — CC54 (слово $404)
        for (int i = 0; i < kNumParams; ++i)
            addParameter(words_[i] = new juce::AudioParameterFloat(
                juce::ParameterID{"raw" + juce::String(i), 1},
                names_[i], 0.0f, 1.0f, defaults_[i]));
    }

    void prepareToPlay(double, int) override
    {
        for (auto& v : voices_) v.reset();
    }
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& l) const override
    {
        return l.getMainInputChannelSet() == l.getMainOutputChannelSet()
            && (l.getMainInputChannelSet() == juce::AudioChannelSet::mono()
             || l.getMainInputChannelSet() == juce::AudioChannelSet::stereo());
    }

    void processBlock(juce::AudioBuffer<float>& buf, juce::MidiBuffer& midi) override
    {
        // 1) страницы → слова (knob<<16 — линейный маппинг; кривая = DSP-ступень, mnm_18)
        auto w = [](juce::AudioParameterFloat* p) -> uint32_t
        {
            return (uint32_t)(juce::jlimit(0.0f, 1.0f, p->get()) * 8388607.0f) & 0xFFFFFFu;
        };
        voice_.setFiltWords(w(words_[0]), w(words_[1]), w(words_[2]), w(words_[3]),
                            w(words_[4]), w(words_[5]), w(words_[6]), w(words_[7]));
        voice_.setAmpWords(w(words_[8]), w(words_[9]), w(words_[10]), w(words_[11]));
        voice_.setEnv2Words(w(words_[12]), w(words_[13]), w(words_[14]), w(words_[15]));
        voice_.setDistWord(w(words_[kDistParam]));   // Y:$404 (mnm_19)

        // 2) события $420/$421/$428
        for (const auto md : midi) {
            const auto m = md.getMessage();
            if (m.isNoteOn())  voice_.trigger();   // $420:=1, $428:=1, $421:=1
            if (m.isNoteOff()) voice_.release();   // $428:=2
        }

        // 3) тракт, сэмпл-в-сэмпл (латентность 1 кадр = 16 сэмплов — свойство прошивки)
        auto* L = buf.getWritePointer(0);
        auto* R = buf.getNumChannels() > 1 ? buf.getWritePointer(1) : nullptr;
        if (R != nullptr)   // СТЕРЕО: один счётчик кадра на пару L/R (mnm_19)
            for (int i = 0; i < buf.getNumSamples(); ++i)
                voice_.pushStereo(L[i], R[i], &L[i], &R[i]);
        else                // МОНО: заполняется только L-буфер кадра
            for (int i = 0; i < buf.getNumSamples(); ++i)
                L[i] = voice_.processL(L[i]);
    }

    // ===== служебное =====
    juce::AudioProcessorEditor* createEditor() override { return new MnmFltDistEditor(*this); }
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "MnmFltDist"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.5; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock& dest) override
    {
        juce::ValueTree t("MnmFltDist");
        for (int i = 0; i < kNumParams; ++i)
            t.setProperty("w" + juce::String(i), words_[i]->get(), nullptr);
        juce::MemoryOutputStream mo(dest, false);
        t.writeToStream(mo);
    }
    void setStateInformation(const void* data, int size) override
    {
        const auto t = juce::ValueTree::readFromData(data, (size_t)size);
        if (!t.isValid()) return;
        for (int i = 0; i < kNumParams; ++i)
            *words_[i] = (float)t.getProperty("w" + juce::String(i), 0.0);
    }

private:
    static constexpr int kNumParams = 17;
    static constexpr int kDistParam = 16;   // Y:$404 — слово ручки DIST (mnm_19)
    static constexpr const char* names_[kNumParams] = {
        "BASE", "WDTH", "HPQ", "LPQ", "FATK", "FDEC", "BOFS", "WOFS",
        "AATK", "HOLD", "ADEC", "REL", "E2ATK", "E2DEC", "E2SUS", "E2REL",
        "DIST" };
    static constexpr float defaults_[kNumParams] = {
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 0.0f,
        0.5f };   // DIST: слово $400000 = v 64 — нижняя граница folding (x0 = 0)
    juce::AudioParameterFloat* words_[kNumParams]{};
    MnmFltDistVoice voice_;
    std::array<MnmFltDistVoice, 0> voices_;   // один голос (монотракт)

    class MnmFltDistEditor : public juce::AudioProcessorEditor
    {
    public:
        explicit MnmFltDistEditor(MnmFltDistProcessor& p) : juce::AudioProcessorEditor(p), proc_(p)
        {
            for (int i = 0; i < kNumParams; ++i) {
                auto* s = new juce::Slider(juce::Slider::LinearHorizontal,
                                           juce::Slider::TextBoxRight);
                s->setRange(0.0, 1.0);
                s->onValueChange = [this, i] { *proc_.words_[i] = (float)sliders_[i]->getValue(); };
                addAndMakeVisible(sliders_[i] = s);
                labels_[i].setText(proc_.names_[i], juce::dontSendNotification);
                addAndMakeVisible(labels_[i]);
            }
            setSize(420, 26 * kNumParams + 10);
        }
        void resized() override
        {
            for (int i = 0; i < kNumParams; ++i) {
                labels_[i].setBounds(4, 4 + i * 26, 60, 22);
                sliders_[i]->setBounds(68, 4 + i * 26, 340, 22);
            }
        }
    private:
        MnmFltDistProcessor& proc_;
        juce::Slider* sliders_[kNumParams]{};
        juce::Label labels_[kNumParams];
    };
};
