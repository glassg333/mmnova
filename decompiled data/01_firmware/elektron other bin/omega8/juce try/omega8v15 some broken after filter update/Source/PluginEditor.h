// PluginEditor.h — Omega 8 front panel recreation (rack-mount style)
#pragma once
#include "PluginProcessor.h"

//==============================================================================
struct OmegaLookAndFeel : juce::LookAndFeel_V4
{
    juce::Font getComboBoxFont (juce::ComboBox&) override { return juce::Font (13.0f); }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h, float pos,
                           float /*rotaryStartAngle*/, float /*rotaryEndAngle*/,
                           juce::Slider& s) override
    {
        juce::ignoreUnused (s);
        auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (6.0f);
        auto size   = juce::jmin (bounds.getWidth(), bounds.getHeight());
        auto centre = bounds.getCentre();
        auto radius = size * 0.5f - 2.0f;

        // knob body
        juce::ColourGradient grad (juce::Colour (0xff3c4147), centre.getX(), centre.getY(),
                                   juce::Colour (0xff0f1113), centre.getX(), centre.getY() + radius, false);
        g.setGradientFill (grad);
        g.fillEllipse (juce::Rectangle<float> (radius * 2, radius * 2).withCentre (centre));

        g.setColour (juce::Colour (0xff050607));
        g.drawEllipse (juce::Rectangle<float> (radius * 2, radius * 2).withCentre (centre), 1.2f);

        // ticks
        auto arc = juce::MathConstants<float>::pi * 1.35f;
        g.setColour (juce::Colour (0x88ffffff));
        for (int i = 0; i <= 10; ++i)
        {
            float a = -arc + arc * 2.0f * ((float) i / 10.0f) + juce::MathConstants<float>::halfPi;
            juce::Point<float> p1 (centre.getX() + std::cos (a) * (radius + 3.0f),
                                   centre.getY() - std::sin (a) * (radius + 3.0f));
            juce::Point<float> p2 (centre.getX() + std::cos (a) * (radius + 6.0f),
                                   centre.getY() - std::sin (a) * (radius + 6.0f));
            g.drawLine ({ p1, p2 }, 1.0f);
        }

        // r11: дуга-ход min→текущее + ограничители — видно, где пределы
        {
            auto ptAt = [&] (float t)
            {
                float aa = -arc + arc * 2.0f * t + juce::MathConstants<float>::halfPi;
                return juce::Point<float> (centre.getX() + std::cos (aa) * (radius + 5.0f),
                                           centre.getY() - std::sin (aa) * (radius + 5.0f));
            };
            const int segs = 20;
            g.setColour (juce::Colour (0x5066717d));
            for (int i = 0; i < segs; ++i)
                g.drawLine ({ ptAt ((float) i / segs), ptAt ((float) (i + 1) / segs) }, 2.0f);
            const int filled = juce::jlimit (0, segs, (int) std::lround (pos * segs));
            g.setColour (juce::Colour (0xccffb04a));
            for (int i = 0; i < filled; ++i)
                g.drawLine ({ ptAt ((float) i / segs), ptAt ((float) (i + 1) / segs) }, 2.0f);
            g.setColour (juce::Colour (0xff9aa4ad));
            for (float t : { 0.0f, 1.0f })
            {
                auto q = ptAt (t);
                g.fillEllipse (q.x - 1.5f, q.y - 1.5f, 3.0f, 3.0f);
            }
        }

        // pointer
        float a = -arc + arc * 2.0f * pos + juce::MathConstants<float>::halfPi;
        g.setColour (juce::Colour (0xffffab33));
        g.drawLine (centre.getX(), centre.getY(),
                    centre.getX() + std::cos (a) * (radius - 4.0f),
                    centre.getY() - std::sin (a) * (radius - 4.0f), 2.4f);
        g.setColour (juce::Colour (0xffffab33));
        g.fillEllipse (juce::Rectangle<float> (4, 4).withCentre (
            juce::Point<float> (centre.getX() + std::cos (a) * (radius - 6.0f),
                                centre.getY() - std::sin (a) * (radius - 6.0f))));
    }

    void drawButtonBackground (juce::Graphics& g, juce::Button& b, const juce::Colour& bg,
                               bool, bool down) override
    {
        auto r = b.getLocalBounds().toFloat().reduced (1.0f);
        auto on = b.getToggleState();
        g.setColour (down ? juce::Colour (0xff1a1e22)
                          : (on ? juce::Colour (0xffc05a1e) : juce::Colour (0xff24282c)));
        g.fillRoundedRectangle (r, 4.0f);
        g.setColour (juce::Colour (0xff8899aa));
        g.drawRoundedRectangle (r, 4.0f, 1.0f);
    }
};

//==============================================================================
struct Omega8AudioProcessorEditor : juce::AudioProcessorEditor,
                                    private juce::Timer,
                                    private juce::Button::Listener,
                                    private juce::ComboBox::Listener
{
    Omega8AudioProcessorEditor (Omega8AudioProcessor& p)
        : AudioProcessorEditor (&p), proc (p)
    {
        setLookAndFeel (&lnf);

        auto& ps = proc.apvts;

        // ---- automatable controls ----
        morph.setSliderStyle (juce::Slider::LinearHorizontal);
        morph.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible (morph);
        morphAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (ps, "morph", morph);

        vol.setSliderStyle (juce::Slider::Rotary);
        vol.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible (vol);
        volAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (ps, "volume", vol);
        vol.setTooltip ("Master output level.");
        volLab.setText ("VOLUME", juce::dontSendNotification);
        volLab.setFont (juce::Font (9.0f, juce::Font::bold));
        volLab.setColour (juce::Label::textColourId, juce::Colour (0xffcfd8e0));
        volLab.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (volLab);

        arpOn.setClickingTogglesState (true);
        arpOn.setButtonText ("START/STOP");
        addAndMakeVisible (arpOn);
        arpOnAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (ps, "arpOn", arpOn);

        arpRate.setSliderStyle (juce::Slider::Rotary);
        arpRate.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible (arpRate);
        arpRateAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (ps, "arpRate", arpRate);

        arpPat.addItemList (juce::StringArray { "UP", "DOWN", "UP-DN", "RND" }, 1);
        addAndMakeVisible (arpPat);
        arpPatAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (ps, "arpPat", arpPat);

        arpOct.addItemList (juce::StringArray { "1 OCT", "2 OCT", "3 OCT", "4 OCT" }, 1);
        addAndMakeVisible (arpOct);
        arpOct.addListener (this);

        // r9: карта AUX1 (OB SVF / OB-X / TB303) — конфиг слота, не байт патча
        auxCardLab.setText ("AUX1 CARD", juce::dontSendNotification);
        auxCardLab.setTooltip ("AUX1 is an expansion-filter slot. The three menu entries are the currently implemented models, not an exhaustive official card list.");
        auxCardLab.setFont (juce::Font (10.0f));
        auxCardLab.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
        auxCardLab.setJustificationType (juce::Justification::centredLeft);
        addAndMakeVisible (auxCardLab);
        auxCardBox.addItemList (juce::StringArray { "OB SVF", "OB-X", "TB-303" }, 1);
        auxCardBox.setTooltip ("Implemented AUX1 models: Oberheim SVF, OB-X-style filter, TB-303-style filter. These are DSP approximations; see the manual for the instrument's optional AUX slots.");
        addAndMakeVisible (auxCardBox);
        auxCardAtt = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (ps, "auxCard", auxCardBox);

        // ---- r11: Programmer (скрин OmegaCODEeditor): 8 ламп программ ----
        for (int i = 0; i < 8; ++i)
        {
            lamp[i].setButtonText (juce::String (i + 1));
            lamp[i].setClickingTogglesState (false);      // состояние = индикатор из таймера
            lamp[i].addListener (this);
            addAndMakeVisible (lamp[i]);
        }
        enc.setSliderStyle (juce::Slider::Rotary);
        enc.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        enc.setMouseDragSensitivity (128);
        enc.setPopupDisplayEnabled (true, true, this);
        enc.textFromValueFunction = [] (double v) { return juce::String ((int) v); };
        addAndMakeVisible (enc);
        encAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (ps, "presetA", enc);
        for (auto* a : { &arrL, &arrR, &arrU, &arrD })
        {
            a->addListener (this);
            addAndMakeVisible (*a);
        }
        typeLab.setFont (juce::Font (9.0f));
        typeLab.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
        typeLab.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (typeLab);
        typeBox.addItemList (juce::StringArray { "SEM LP", "SEM BP", "SEM HP", "SEM BR", "MINI LP", "AUX1", "AUX2" }, 1);
        typeBox.setTooltip ("Filter TYPE: SEM low-pass/band-pass/high-pass/band-reject, MINI low-pass, AUX1, AUX2.");
        typeBox.addListener (this);
        addAndMakeVisible (typeBox);

        // ---- preset pickers ----
        for (auto* btn : { &prevA, &nextA, &prevB, &nextB })
        {
            btn->setButtonText (btn == &prevA || btn == &prevB ? "<" : ">");
            btn->addListener (this);
            addAndMakeVisible (btn);
        }
        addAndMakeVisible (patchList);
        patchList.setTextWhenNothingSelected ("-- patch --");
        patchList.addListener (this);

        loadBtn.setButtonText ("LOAD BANK .syx/.mid");
        loadBtn.addListener (this);
        addAndMakeVisible (loadBtn);

        matrixBtn.setClickingTogglesState (true);
        matrixBtn.addListener (this);
        addAndMakeVisible (matrixBtn);

        addAndMakeVisible (lcd);
        addAndMakeVisible (labA);
        addAndMakeVisible (labB);
        for (auto& l : envExtLab) { addAndMakeVisible (l); l.setFont (juce::Font (10.0f));
            l.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
            l.setJustificationType (juce::Justification::centred); }
        for (auto& l : panLab)    { addAndMakeVisible (l); l.setFont (juce::Font (10.0f));
            l.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
            l.setJustificationType (juce::Justification::centred); }
        addAndMakeVisible (morphLab);
        addAndMakeVisible (octLabel);
        addAndMakeVisible (tuneLabel);
        octLabel.setTooltip ("Oscillator octave offset; MID is the neutral octave.");
        tuneLabel.setTooltip ("Patch Tune amount. Raw value 64 corresponds to 100% in the original editor; lower values add intentional tuning variation.");
        addAndMakeVisible (glideLed);
        addAndMakeVisible (subLed);
        for (auto* w : { &wv1t, &wv1s, &wv1p, &wv2t, &wv2s, &wv2p }) addAndMakeVisible (*w);
        labA.setFont (juce::Font (10.0f));
        labB.setFont (juce::Font (10.0f));
        octLabel.setFont (juce::Font (10.0f));
        tuneLabel.setFont (juce::Font (10.0f));
        octLabel.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
        tuneLabel.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));

        // ---- patch knobs (r5: редактируют слот под морфом) ----
        addKnob ("GLIDE",     omega8::off::GLIDE_TIME);
        addKnob ("OSC1 FREQ", omega8::off::OSC1_FREQ);
        addKnob ("OSC2 FREQ", omega8::off::OSC2_FREQ);
        addKnob ("PWM 1",     omega8::off::PWM1);
        addKnob ("PWM 2",     omega8::off::PWM2);
        addKnob ("OSC1 LEV",  omega8::off::OSC1_LEVEL);
        addKnob ("OSC2 LEV",  omega8::off::OSC2_LEVEL);
        addKnob ("NOISE",     omega8::off::NOISE_LEVEL);
        addKnob ("SUB",       omega8::off::SUB_WAVE);
        addKnob ("FINE",      omega8::off::OSC2_FINE);
        addKnob ("TUNE",      omega8::off::MASTER_TUNE);
        addKnob ("EXT IN",    omega8::off::EXT_IN);

        addKnob ("CUTOFF",    omega8::off::CUTOFF);
        addKnob ("RESO",      omega8::off::RESO);
        addKnob ("TRACKING",  omega8::off::TRACKING);
        addKnob ("ENV1 AMT",  omega8::off::ENV1_AMT);
        addKnob ("HPF",       omega8::off::HPF);
        addKnob ("HPR",       omega8::off::HPR);

        addKnob ("LFO1 RATE", omega8::off::LFO1_RATE);
        addKnob ("LFO1 D1",   omega8::off::LFO1_DEPTH1);
        addKnob ("LFO1 D2",   omega8::off::LFO1_DEPTH2);
        addKnob ("LFO1 D3",   omega8::off::LFO1_DEPTH3);
        addKnob ("LFO2 RATE", omega8::off::LFO2_RATE);
        addKnob ("LFO2 D1",   omega8::off::LFO2_DEPTH1);
        addKnob ("LFO2 D2",   omega8::off::LFO2_DEPTH2);
        addKnob ("LFO2 D3",   omega8::off::LFO2_DEPTH3);
        addKnob ("XMOD",      omega8::off::XMOD_DPTH);
        addKnob ("ENV3 AMT",  omega8::off::ENV3_AMT);

        const char* envNames[3][5] = {
            { "A", "D", "DKY2", "S", "R" }, { "A", "D", "DKY2", "S", "R" }, { "A", "D", "DKY2", "S", "R" } };
        const int envBase[3] = { omega8::off::ENV1_ATK, omega8::off::ENV2_ATK, omega8::off::ENV3_ATK };
        for (int e = 0; e < 3; ++e)
            for (int k = 0; k < 5; ++k)
            {
                juce::String title = juce::String ("E") + juce::String (e + 1) + " " + envNames[e][k];
                addKnob (title, envBase[e] + k);
            }

        // ---- r6: extend-window knobs (по скрину OMEGACODE: amounts, dyn/dly, pan) ----
        addKnob ("MW A1", omega8::off::MODW_A1);  addKnob ("MW A2", omega8::off::MODW_A2);
        addKnob ("DY A1", omega8::off::DYN_A1);   addKnob ("DY A2", omega8::off::DYN_A2);
        addKnob ("BN A1", omega8::off::BEND_A1);  addKnob ("BN A2", omega8::off::BEND_A2);
        addKnob ("PR A1", omega8::off::PRES_A1);  addKnob ("PR A2", omega8::off::PRES_A2);
        addKnob ("C1 A1", omega8::off::C1_A1);    addKnob ("C1 A2", omega8::off::C1_A2);
        addKnob ("C2 A1", omega8::off::C2_A1);    addKnob ("C2 A2", omega8::off::C2_A2);
        addKnob ("E3 A1", omega8::off::ENV3_AMT1);
        addKnob ("E3 A2", omega8::off::ENV3_AMT2);
        addKnob ("E3 A3", omega8::off::ENV3_AMT3);
        addKnob ("DYN1", omega8::off::DYN1);
        addKnob ("DYN2", omega8::off::DYN2);
        addKnob ("DYN3", omega8::off::DYN3);
        addKnob ("DLY1", omega8::off::DLY1);
        addKnob ("DLY2", omega8::off::DLY2);
        addKnob ("DLY3", omega8::off::DLY3);
        for (int v = 0; v < 4; ++v)
        {
            addKnob ("P" + juce::String (v + 1) + " POS",  omega8::kArrPos  + v);
            addKnob ("P" + juce::String (v + 1) + " RATE", omega8::kArrRate + v);
            addKnob ("P" + juce::String (v + 1) + " DPTH", omega8::kArrDepth + v);
        }
        createMatrix();

        // r15: every faceplate switch has a listener; previously several toggles
        // changed their lamp state but never wrote the decoded patch byte.
        for (auto* w : { &wv1t, &wv1s, &wv1p, &wv2t, &wv2s, &wv2p })
        {
            w->setClickingTogglesState (true);
            w->addListener (this);
        }
        glideLed.setClickingTogglesState (true);
        glideLed.addListener (this);
        subLed.setClickingTogglesState (true);
        subLed.addListener (this);

        rebuildPatchList();
        setSize (1180, 506);
        startTimerHz (20);
    }

    ~Omega8AudioProcessorEditor() override
    {
        setLookAndFeel (nullptr);
    }

    //==========================================================================
    // r11: круговое вращение ручки (тянешь по дуге = крутишь; у центра — вертикаль)
    // r14: свободный драг — тянется в ЛЮБОМ направлении (вверх/вниз/влево/вправо/
    // по диагонали), 1px ~ 1 байт, полный ход = 120px. Круговой драг r11 заменён:
    // аккумулятор (dx - dy) надёжно работает при любом движении руки.
    struct ArcSlider : juce::Slider
    {
        float lastX = 0.0f, lastY = 0.0f;
        void mouseDown (const juce::MouseEvent& e) override
        {
            lastX = e.position.x; lastY = e.position.y;
            juce::Slider::mouseDown (e);
        }
        void mouseDrag (const juce::MouseEvent& e) override
        {
            const float dx = e.position.x - lastX, dy = e.position.y - lastY;
            lastX = e.position.x; lastY = e.position.y;
            const double range = getMaximum() - getMinimum();
            const double dd = ((double) dx - (double) dy) * range / 120.0;
            if (dd != 0.0)
                // Send a synchronous change so addKnob::onValueChange writes the byte.
                setValue (juce::jlimit (getMinimum(), getMaximum(), getValue() + dd),
                          juce::sendNotificationSync);
        }
        void mouseUp (const juce::MouseEvent& e) override { juce::Slider::mouseUp (e); }
    };

    //==========================================================================
    struct DisplayKnob
    {
        ArcSlider slider;
        juce::Label  label;
        std::function<double(const omega8::Patch&)> read;
        juce::String title;
        int ofs = -1;   // raw-offset патча (для записи из ручки)
    };

    void addKnob (juce::String title, int ofs)
    {
        auto k = std::make_unique<DisplayKnob>();
        k->ofs = ofs;
        k->read = [ofs] (const omega8::Patch& q) { return (double) q.at (ofs); };
        k->title = title;
        k->slider.setSliderStyle (juce::Slider::Rotary);
        k->slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        const double maxValue = (ofs == omega8::off::SUB_WAVE ? 6.0 : 127.0);
        k->slider.setRange (0.0, maxValue, 1.0); // SUB_WAVE is an enum 0..6; other byte values are 0..127
        k->slider.setTooltip (knobTooltip (title, ofs));
        // r15: свободный линейный drag; value changes are sent to onValueChange.
        // r8: число-попап под курсором
        k->slider.setMouseDragSensitivity (128);
        k->slider.setPopupDisplayEnabled (true, true, this);   // (hover, click, parent)
        // r11: явное значение без процентов/иероглифов: только число 0..127
        k->slider.textFromValueFunction = [] (double v) { return juce::String ((int) v); };
        k->label.setText (title, juce::NotificationType::dontSendNotification);
        k->label.setJustificationType (juce::Justification::centred);
        k->label.setFont (juce::Font (10.0f));
        k->label.setColour (juce::Label::textColourId, juce::Colour (0xffcfd8e0));
        k->label.setTooltip (k->slider.getTooltip());
        // r5: ручка ПИШЕТ байт в слот, который правишь (A при morph<=50%, иначе B)
        k->slider.onValueChange = [this, dk = k.get()]
        {
            if (dk->ofs < 0) return;
            const int maxByte = (dk->ofs == omega8::off::SUB_WAVE ? 6 : 127);
            int v = juce::jlimit (0, maxByte, (int) std::lround (dk->slider.getValue()));
            proc.setPatchByte (proc.editSlot(), dk->ofs, (uint8_t) v);
            // r11: значение уходит на LCD (строка курсора как на железе)
            lastEdit = dk->title;
            lastEditVal = v;
            lastEditUntil = juce::Time::getMillisecondCounter() + 1500;
        };
        addAndMakeVisible (k->slider);
        addAndMakeVisible (k->label);
        knobs.push_back (std::move (k));
    }

    juce::String knobTooltip (const juce::String& t, int ofs) const
    {
        if (ofs == omega8::off::SUB_WAVE) return "Oscillator 1 sub-waveform: OFF plus the decoded sub-wave options (0-6).";
        if (t == "GLIDE") return "Portamento / glide time.";
        if (t == "OSC1 FREQ") return "Oscillator 1 pitch / tuning offset. r15 restores the centered raw-32 mapping from r10; confirm absolute tuning with a tuner on a neutral patch.";
        if (t == "OSC2 FREQ") return "Oscillator 2 pitch / tuning offset. r15 restores the centered raw-32 mapping from r10; confirm absolute tuning with a tuner on a neutral patch.";
        if (t == "PWM 1" || t == "PWM 2") return "Pulse width of the selected oscillator's pulse waveform.";
        if (t == "OSC1 LEV" || t == "OSC2 LEV") return "Oscillator mix level.";
        if (t == "NOISE") return "White-noise mix level.";
        if (t == "FINE") return "Oscillator 2 fine tuning.";
        if (t == "TUNE") return "Patch Tune field: the original editor shows raw byte 64 as 100%. This build displays and stores it; the undocumented random-variation depth is not yet modeled in DSP.";
        if (t == "EXT IN") return "External-input level.";
        if (t == "CUTOFF") return "Filter cutoff / centre frequency.";
        if (t == "RESO") return "Filter resonance (Q).";
        if (t == "TRACKING") return "Keyboard tracking amount for the filter.";
        if (t == "HPF") return "AUX2 / CS-80 high-pass frequency control.";
        if (t == "HPR") return "AUX2 / CS-80 high-pass resonance control.";
        if (t == "ENV1 AMT") return "Envelope 1 amount routed to filter frequency.";
        if (t == "ENV3 AMT") return "Envelope 3 amount for destination 1.";
        if (t.endsWith (" A")) return "Envelope attack time.";
        if (t.endsWith (" D")) return "Envelope decay time.";
        if (t.endsWith (" DKY2")) return "Envelope decay 2 time: additional held-note decay stage.";
        if (t.endsWith (" S")) return "Envelope sustain level.";
        if (t.endsWith (" R")) return "Envelope release time.";
        if (t.contains (" POS")) return "Pan position for this voice.";
        if (t.contains (" RATE") && t.startsWith ("P")) return "Pan movement rate for this voice.";
        if (t.contains (" DPTH")) return "Pan movement depth for this voice.";
        if (t.contains (" RATE")) return "Low-frequency oscillator rate.";
        if (t.contains (" D1")) return "Modulation amount for destination 1.";
        if (t.contains (" D2")) return "Modulation amount for destination 2.";
        if (t.contains (" D3")) return "Modulation amount for destination 3.";
        if (t == "XMOD") return "Cross-modulation depth: Oscillator 2 audio-rate modulation.";
        if (t.startsWith ("MW ")) return "Mod wheel amount; A1/A2 means destination amount 1/2.";
        if (t.startsWith ("DY ")) return "Keyboard dynamics / velocity amount; A1/A2 means destination amount 1/2.";
        if (t.startsWith ("BN ")) return "Pitch-bend amount; A1/A2 means destination amount 1/2.";
        if (t.startsWith ("PR ")) return "Aftertouch / pressure amount; A1/A2 means destination amount 1/2.";
        if (t.startsWith ("C1 ")) return "Continuous controller 1 amount; A1/A2 means destination amount 1/2.";
        if (t.startsWith ("C2 ")) return "Continuous controller 2 amount; A1/A2 means destination amount 1/2.";
        if (t.startsWith ("E3 A")) return "Envelope 3 modulation amount for the matching destination slot.";
        if (t.startsWith ("DLY")) return "Delay before the matching envelope starts.";
        if (t.startsWith ("DYN")) return "Dynamic / velocity sensitivity for the matching envelope.";
        return "Decoded Omega 8 patch parameter.";
    }

    //==========================================================================
    // r6: dest-дропдауны (матрица как в оригинале / SE-1X extend window)
    struct DestCombo
    {
        juce::ComboBox box;
        juce::Label    lab;
        int ofs = -1;
    };
    std::vector<std::unique_ptr<DestCombo>> dests;

    void addDest (const juce::String& lbl, int ofs, const juce::StringArray& items)
    {
        auto d = std::make_unique<DestCombo>();
        d->ofs = ofs;
        d->box.addItemList (items, 1);                       // id = byte + 1
        d->lab.setText (lbl, juce::NotificationType::dontSendNotification);
        d->lab.setFont (juce::Font (9.0f));
        d->lab.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
        d->lab.setJustificationType (juce::Justification::centredLeft);
        d->box.onChange = [this, dc = d.get()]
        {
            int id = dc->box.getSelectedId();                // только пользовательский выбор
            if (id >= 1)
                proc.setPatchByte (proc.editSlot(), dc->ofs, (uint8_t) (id - 1));
        };
        addAndMakeVisible (d->box);
        addAndMakeVisible (d->lab);
        dests.push_back (std::move (d));
    }

    void createMatrix()
    {
        static const char* dn[] = { "OFF", "FRE1", "FRE2", "1&2F", "LEV1", "LEV2", "PW1", "PW2",
                                    "1&2P", "FILT", "RESO", "LEVN", "XMOD", "EA1", "EA3", "EXT",
                                    "LF1R", "LF2R", "LF1D", "LF2D", "1&2D", "1&2R", "PAND", "PANR", "PAN" };
        juce::StringArray items;
        for (auto* s : dn) items.add (s);

        // 0..11 — контроллеры (pairs): modwhl, dynamics, bender, pressure, cont1, cont2
        addDest ("modwhl",   omega8::off::MODW_D1, items);
        addDest ("",         omega8::off::MODW_D2, items);
        addDest ("dynamics", omega8::off::DYN_D1,  items);
        addDest ("",         omega8::off::DYN_D2,  items);
        addDest ("bender",   omega8::off::BEND_D1, items);
        addDest ("",         omega8::off::BEND_D2, items);
        addDest ("pressure", omega8::off::PRES_D1, items);
        addDest ("",         omega8::off::PRES_D2, items);
        addDest ("cont1",    omega8::off::C1_D1,   items);
        addDest ("",         omega8::off::C1_D2,   items);
        addDest ("cont2",    omega8::off::C2_D1,   items);
        addDest ("",         omega8::off::C2_D2,   items);
        // 12..20 — LFO1/LFO2/ENV3 по 3 dest
        addDest ("LFO1", omega8::off::LFO1_DEST1, items);
        addDest ("",     omega8::off::LFO1_DEST2, items);
        addDest ("",     omega8::off::LFO1_DEST3, items);
        addDest ("LFO2", omega8::off::LFO2_DEST1, items);
        addDest ("",     omega8::off::LFO2_DEST2, items);
        addDest ("",     omega8::off::LFO2_DEST3, items);
        addDest ("ENV3", omega8::off::ENV3_DEST1, items);
        addDest ("",     omega8::off::ENV3_DEST2, items);
        addDest ("",     omega8::off::ENV3_DEST3, items);
        // 21 — XMOD назначение; 22..23 — волны LFO
        addDest ("XMOD", omega8::off::XMOD_DEST,     juce::StringArray { "FREQ", "PW", "CUTOFF" });
        addDest ("W1",   omega8::off::LFO1_WAVSYNC,  juce::StringArray { "SIN", "SQR" });
        addDest ("W2",   omega8::off::LFO2_WAVSYNC,  juce::StringArray { "SIN", "SQR" });
    }

    void timerCallback() override
    {
        const auto p = proc.getEditPatch();   // слот, который правят ручки (копия под локом)
        for (auto& k : knobs)
        {
            if (k->slider.isMouseButtonDown()) continue;   // r5: не сбрасываем во время вращения
            auto v = k->read (p);
            if (std::abs (k->slider.getValue() - v) > 0.001)
                k->slider.setValue (v, juce::dontSendNotification);
        }
        for (auto& d : dests)
        {
            if (d->box.hasKeyboardFocus (true) || d->box.isMouseButtonDown()) continue;
            int want = juce::jlimit (1, juce::jmax (1, d->box.getNumItems()),
                                     (int) p.at (d->ofs) + 1);
            if (d->box.getSelectedId() != want)
                d->box.setSelectedId (want, juce::dontSendNotification);
        }
        auto name = proc.patchNameAt (proc.editSlot());
        int a = proc.presetA(), b = proc.presetB();
        float m = proc.morphAmount();
        // r11: строка1 — PATCH/BLEND как на железе; строка2 — имя, % BLEND
        // или ЗНАЧЕНИЕ последней ручки (1.5 с, как строка курсора на панели).
        juce::String line1, line2;
        if (m > 0.001f && a != b)
            line1 = juce::String::formatted ("BLEND: A%03d+B%03d", a + 1, b + 1);
        else
            line1 = juce::String::formatted ("PATCH A%03d  B%03d", a + 1, b + 1);
        if (juce::Time::getMillisecondCounter() < lastEditUntil)
            line2 = lastEdit + " " + juce::String (lastEditVal);          // число, без %/иероглифов
        else if (m > 0.001f && a != b)
            line2 = juce::String::formatted ("%s %3.0f%%", name.toRawUTF8(), m * 100.0f);
        else
            line2 = name;
        lcdText = line1 + "\n" + line2;
        lcd.text = lcdText;
        lcd.repaint();

        // r11: лампы программ (группа 8) + подписи A/B + TYPE combo
        int curA = proc.presetA();
        for (int i = 0; i < 8; ++i)
            lamp[i].setToggleState ((curA % 8) == i, juce::dontSendNotification);
        labA.setText ("A " + juce::String (a + 1).paddedLeft ('0', 3) + " " + proc.patchNameAt (a),
                      juce::dontSendNotification);
        labB.setText ("B " + juce::String (b + 1).paddedLeft ('0', 3) + " " + proc.patchNameAt (b),
                      juce::dontSendNotification);

        // filter type LEDs
        int ft = p.at (omega8::off::FILT_TYPE);
        for (int i = 0; i < 7 && i < (int) filtTypes.size(); ++i)
            filtTypes[(size_t) i]->setToggleState (i == ft, juce::dontSendNotification);
        if (! typeBox.isMouseButtonDown() && typeBox.getSelectedId() != ft + 1)
            typeBox.setSelectedId (ft + 1, juce::dontSendNotification);

        // r14: байт FILT_TYPE -> параметр хоста (панель/Ableton всегда в согласии)
        if (auto* fpp = proc.apvts.getParameter ("filtType"))
            if (auto* fpv = proc.apvts.getRawParameterValue ("filtType"))
            {
                const int want = p.at (omega8::off::FILT_TYPE);
                if ((int) fpv->load() != want && ! typeBox.isMouseButtonDown())
                    fpp->setValueNotifyingHost (fpp->convertTo0to1 ((float) want));
            }

        // waveform LEDs: osc1 = bitmask (0 shows SAW), osc2 = enum 75 {0=pulse,1=tri,2=saw}
        int wb = p.at (omega8::off::WAVE_BITS);
        int m1 = (wb & 7) != 0 ? (wb & 7) : 2;
        wv1t.setToggleState ((m1 & 1) != 0, juce::dontSendNotification);
        wv1s.setToggleState ((m1 & 2) != 0, juce::dontSendNotification);
        wv1p.setToggleState ((m1 & 4) != 0, juce::dontSendNotification);
        int w2 = p.at (omega8::off::WAVE2);
        wv2t.setToggleState (w2 == 1, juce::dontSendNotification);
        wv2s.setToggleState (w2 == 2, juce::dontSendNotification);
        wv2p.setToggleState (w2 == 0, juce::dontSendNotification);
        subLed.setToggleState (p.at (omega8::off::SUB_WAVE) > 0, juce::dontSendNotification);
        glideLed.setToggleState ((p.at (omega8::off::GLIDE_FLAGS) & 0x40) != 0, juce::dontSendNotification);

        if (! arpOct.hasKeyboardFocus (true))
        {
            int oct = juce::jlimit (1, 4, (int) proc.apvts.getRawParameterValue ("arpOct")->load());
            if (arpOct.getSelectedId() != oct) arpOct.setSelectedId (oct, juce::dontSendNotification);
        }

        if (! patchList.hasKeyboardFocus (true))
        {
            int sel = proc.presetA();
            if (proc.presetB() != proc.presetA())
                sel = -1;
            if (patchList.getSelectedId() - 1 != sel)
                patchList.setSelectedId (sel + 1, juce::dontSendNotification);
        }

        int ob = p.at (omega8::off::OCTAVE);
        const int oct = ((ob >> 4) & 0x0F) - 4;
        const juce::String octText = (oct == 0 ? juce::String ("MID")
                                              : (oct > 0 ? juce::String ("+") + juce::String (oct)
                                                         : juce::String (oct)));
        octLabel.setText ("OCT: " + octText, juce::dontSendNotification);
        const int tunePct = juce::jlimit (0, 200, (int) std::lround (100.0 * p.at (omega8::off::MASTER_TUNE) / 64.0));
        tuneLabel.setText ("TUNE: " + juce::String (tunePct) + "%", juce::dontSendNotification);
    }

    //==========================================================================
    void paint (juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();

        // rack panel
        juce::ColourGradient grad (juce::Colour (0xff22262b), 0, 0,
                                   juce::Colour (0xff0b0d0f), 0, getHeight(), false);
        g.setGradientFill (grad);
        g.fillAll();

        g.setColour (juce::Colour (0xff1b2127));
        g.fillRect (0, 0, getWidth(), 6);
        g.fillRect (0, getHeight() - 6, getWidth(), 6);

        // r15: clean horizontal faceplate; brand sits on the lower rail as in the reference.

        // section separators + titles
        auto line = [&] (float x, float top = 8.0f, float bot = -8.0f)
        {
            g.setColour (juce::Colours::white.withAlpha (0.25f));
            g.drawVerticalLine ((int) x, top, bot < 0 ? (float) getHeight() + bot : bot);
        };
        for (float x : { 176.0f, 318.0f, 518.0f, 794.0f, 894.0f }) line (x);

        g.setColour (juce::Colours::white);
        g.setFont (juce::Font (12.0f, juce::Font::bold));
        g.drawText ("MULTI / MIDI", 10, 10, 166, 14, juce::Justification::centred);
        g.drawText ("MODULATION",  176, 10, 142, 14, juce::Justification::centred);
        g.drawText ("PROGRAMMER",  318, 10, 200, 14, juce::Justification::centred);
        g.drawText ("OSCILLATORS", 518, 10, 276, 14, juce::Justification::centred);
        g.drawText ("FILTER",      794, 10, 100, 14, juce::Justification::centred);
        g.drawText ("ENVELOPES",   894, 10, 276, 14, juce::Justification::centred);

        // Envelope row identifiers and full terms live on each control's tooltip.
        g.setFont (juce::Font (10.0f, juce::Font::bold));
        g.drawText ("ENV 1  /  FILTER", 902, 40, 262, 12, juce::Justification::centredLeft);
        g.drawText ("ENV 2  /  AMPLIFIER", 902, 124, 262, 12, juce::Justification::centredLeft);
        g.drawText ("ENV 3  /  ASSIGNABLE", 902, 208, 262, 12, juce::Justification::centredLeft);

        // r15 brand plaque on the lower rail.
        g.setColour (juce::Colour (0xffd8dee6));
        g.setFont (juce::Font (10.0f, juce::Font::bold));
        g.drawText ("STUDIO ELECTRONICS  |  OMEGA 8 / CODE", 14, kPanelH - 22, 400, 14,
                    juce::Justification::centredLeft);

        // ---- r6: extend window background (LCD-стиль как на скрине OMEGACODE/SE-1X) ----
        if (matrixOpen)
        {
            auto ext = getLocalBounds().removeFromBottom (kExtH);
            g.setColour (juce::Colour (0xff0a0b0d));
            g.fillRect (ext);
            g.setColour (juce::Colour (0xff3a3f45));
            g.drawRect (ext.reduced (3), 1.0f);

            auto e2 = ext.reduced (12);
            g.setColour (juce::Colour (0xffe9e4d4));
            g.setFont (juce::Font (10.0f, juce::Font::bold));
            g.drawText ("CONTROLLERS   dest1  amt  dest2  amt", e2.getX(), e2.getY() + 2, 396, 14,
                        juce::Justification::centredLeft);
            g.drawText ("LFO1 / LFO2 / ENV3 / XMOD", e2.getX() + 404, e2.getY() + 2, 376, 14,
                        juce::Justification::centredLeft);
            g.drawText ("ENV DLY / DYN / DKY2      PAN 1-4", e2.getX() + 784, e2.getY() + 2, 360, 14,
                        juce::Justification::centredLeft);
        }
    }

    void resized() override
    {
        // Hide GLOBAL-only controls when the extension is closed. Without this they
        // remained at their old coordinates and overlaid the faceplate after resize.
        auto isGlobalKnob = [] (const juce::String& t)
        {
            return t.startsWith ("MW ") || t.startsWith ("DY ") || t.startsWith ("BN ")
                || t.startsWith ("PR ") || t.startsWith ("C1 ") || t.startsWith ("C2 ")
                || t.startsWith ("E3 A") || t.startsWith ("DYN") || t.startsWith ("DLY")
                || t.startsWith ("P1 ") || t.startsWith ("P2 ") || t.startsWith ("P3 ")
                || t.startsWith ("P4 ") || t.contains (" DKY2");
        };
        for (auto& k : knobs)
            if (isGlobalKnob (k->title))
            {
                k->slider.setVisible (matrixOpen);
                k->label.setVisible (matrixOpen);
            }
        for (auto& d : dests) { d->box.setVisible (matrixOpen); d->lab.setVisible (matrixOpen); }
        for (auto& l : envExtLab) l.setVisible (matrixOpen);
        for (auto& l : panLab) l.setVisible (matrixOpen);
        typeBox.setVisible (matrixOpen);

        auto area = getLocalBounds().removeFromTop (kPanelH).reduced (10);
        area.removeFromTop (24); // reserve the original faceplate title strip; no controls over headings

        // ---- Multi / MIDI ----
        {
            auto c = area.removeFromLeft (166).reduced (4, 4);
            auto row = c.removeFromTop (82);
            const int half = row.getWidth() / 2;
            auto vr = row.removeFromLeft (half);
            vol.setBounds (vr.getX() + 4, vr.getY() + 2, vr.getWidth() - 8, 58);
            volLab.setBounds (vr.getX(), vr.getY() + 61, vr.getWidth(), 14);
            auto* gk = findKnob ("GLIDE");
            if (gk != nullptr)
            {
                gk->slider.setBounds (row.getX() + 4, row.getY() + 2, row.getWidth() - 8, 58);
                gk->label.setBounds (row.getX(), row.getY() + 61, row.getWidth(), 14);
            }
            glideLed.setBounds (c.removeFromTop (24).reduced (18, 1));
            octLabel.setBounds (c.removeFromTop (20));
            tuneLabel.setBounds (c.removeFromTop (20));
        }

        // ---- Modulation: 2 x 2 layout like the reference; all six LFO depths stay editable ----
        {
            auto c = area.removeFromLeft (142).reduced (4, 4);
            auto l1 = c.removeFromTop (14);
            lfo1Dest.setText (destText (1), juce::dontSendNotification);
            lfo1Dest.setFont (juce::Font (8.5f));
            lfo1Dest.setTooltip ("LFO 1 destinations 1, 2 and 3. Full destination names are listed in docs/ENGLISH_LABELS_R15.md.");
            addAndMakeVisible (lfo1Dest);
            lfo1Dest.setBounds (l1);
            placeKnobs ({ "LFO1 RATE", "LFO1 D1" }, c.removeFromTop (66));
            placeKnobs ({ "LFO1 D2", "LFO1 D3" }, c.removeFromTop (66));

            auto l2 = c.removeFromTop (14);
            lfo2Dest.setText (destText (2), juce::dontSendNotification);
            lfo2Dest.setFont (juce::Font (8.5f));
            lfo2Dest.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
            lfo2Dest.setTooltip ("LFO 2 destinations 1, 2 and 3. Full destination names are listed in docs/ENGLISH_LABELS_R15.md.");
            addAndMakeVisible (lfo2Dest);
            lfo2Dest.setBounds (l2);
            placeKnobs ({ "LFO2 RATE", "LFO2 D1" }, c.removeFromTop (66));
            placeKnobs ({ "LFO2 D2", "LFO2 D3" }, c.removeFromTop (66));
            placeKnobs ({ "XMOD" }, c.removeFromTop (66));
            env3Dest.setText (env3Text(), juce::dontSendNotification);
            env3Dest.setFont (juce::Font (8.5f));
            env3Dest.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
            env3Dest.setTooltip ("Envelope 3 destination assignments. ENV3 amount is in the envelope section.");
            addAndMakeVisible (env3Dest);
            env3Dest.setBounds (c.removeFromTop (18));
        }

        // ---- Programmer ----
        {
            auto c = area.removeFromLeft (200).reduced (4, 4);
            lcd.setBounds (c.removeFromTop (46).reduced (2));
            c.removeFromTop (2);
            {
                auto lr = c.removeFromTop (16);
                const int lw = lr.getWidth() / 8;
                for (int i = 0; i < 8; ++i)
                    lamp[i].setBounds (lr.getX() + i * lw, lr.getY(), lw - 2, 16);
            }
            c.removeFromTop (4);
            {
                auto er = c.removeFromTop (46);
                enc.setBounds (er.getX(), er.getY(), 46, 46);
                arrL.setBounds (er.getX() + 52, er.getY() + 13, 24, 20);
                arrR.setBounds (er.getX() + 78, er.getY() + 13, 24, 20);
                arrU.setBounds (er.getX() + 108, er.getY() + 2,  28, 20);
                arrD.setBounds (er.getX() + 108, er.getY() + 24, 28, 20);
                arrL.setTooltip ("Previous part / patch.");
                arrR.setTooltip ("Next part / patch.");
                arrU.setTooltip ("Previous bank.");
                arrD.setTooltip ("Next bank.");
            }
            c.removeFromTop (4);
            auto rowA = c.removeFromTop (22);
            prevA.setBounds (rowA.removeFromLeft (24));
            nextA.setBounds (rowA.removeFromRight (24));
            labA.setBounds (rowA);
            auto rowB = c.removeFromTop (22);
            prevB.setBounds (rowB.removeFromLeft (24));
            nextB.setBounds (rowB.removeFromRight (24));
            labB.setBounds (rowB);
            c.removeFromTop (4);
            morph.setBounds (c.removeFromTop (26));
            morphLab.setBounds (c.removeFromTop (14));
            c.removeFromTop (4);
            patchList.setBounds (c.removeFromTop (24));
            c.removeFromTop (4);
            {
                auto rowLoad = c.removeFromTop (22);
                matrixBtn.setBounds (rowLoad.removeFromLeft (64));
                loadBtn.setBounds (rowLoad.reduced (2, 0));
            }
            c.removeFromTop (6);
            // Use a local layout rectangle only. A visible blank Component here
            // would sit above its sibling controls and swallow their mouse clicks.
            auto ap = c;
            arpOn.setBounds (ap.getX() + 4, ap.getY() + 4, ap.getWidth() - 8, 22);
            arpRate.setBounds (ap.getX() + 4, ap.getY() + 30, 56, 56);
            arpPat.setBounds (ap.getX() + 64, ap.getY() + 32, ap.getWidth() - 70, 24);
            arpOct.setBounds (ap.getX() + 64, ap.getY() + 58, ap.getWidth() - 70, 24);
        }

        // ---- Oscillators ----
        {
            auto c = area.removeFromLeft (276).reduced (4, 4);
            auto waveRow1 = c.removeFromTop (22);
            const int w1 = waveRow1.getWidth() / 4;
            wv1t.setBounds (waveRow1.removeFromLeft (w1));
            wv1s.setBounds (waveRow1.removeFromLeft (w1));
            wv1p.setBounds (waveRow1.removeFromLeft (w1));
            subLed.setBounds (waveRow1);
            placeKnobs ({ "OSC1 FREQ", "PWM 1" }, c.removeFromTop (70));
            auto waveRow2 = c.removeFromTop (22);
            const int w2 = waveRow2.getWidth() / 3;
            wv2t.setBounds (waveRow2.removeFromLeft (w2));
            wv2s.setBounds (waveRow2.removeFromLeft (w2));
            wv2p.setBounds (waveRow2);
            placeKnobs ({ "OSC2 FREQ", "PWM 2" }, c.removeFromTop (70));
            placeKnobs ({ "OSC1 LEV", "OSC2 LEV", "NOISE" }, c.removeFromTop (70));
            placeKnobs ({ "FINE", "TUNE", "EXT IN", "SUB" }, c.removeFromTop (70));
        }

        // ---- Filter: legible knobs, full English filter names, wide click targets ----
        {
            auto c = area.removeFromLeft (100).reduced (4, 4);
            placeKnobs ({ "CUTOFF", "RESO" }, c.removeFromTop (62));
            placeKnobs ({ "TRACKING", "HPF" }, c.removeFromTop (62));
            placeKnobs ({ "HPR" }, c.removeFromTop (62));
            typeLab.setText ("FILTER TYPE", juce::dontSendNotification);
            typeLab.setFont (juce::Font (9.0f, juce::Font::bold));
            typeLab.setTooltip ("Click one of the seven filter types below. AUX1/AUX2 are hardware expansion slots.");
            typeLab.setBounds (c.removeFromTop (16));
            static const char* ftNames[7] = { "SEM LP", "SEM BP", "SEM HP", "SEM BR", "MINI LP", "AUX1", "AUX2" };
            static const char* ftTips[7] = {
                "SEM 12 dB low-pass", "SEM 12 dB band-pass", "SEM high-pass", "SEM band-reject / notch",
                "Moog-style MINI 24 dB low-pass", "AUX1 expansion card; selected model is set below",
                "AUX2 expansion slot; CS-80 is the documented fixed card"
            };
            if (filtTypes.empty())
            {
                for (int i = 0; i < 7; ++i)
                {
                    auto b = std::make_unique<juce::ToggleButton> (ftNames[i]);
                    b->setClickingTogglesState (false);
                    b->setTooltip (ftTips[i]);
                    b->onClick = [this, i] { setFilterTypeFromUI (i); };
                    addAndMakeVisible (*b);
                    filtTypes.push_back (std::move (b));
                }
            }
            for (int i = 0; i < 7; ++i)
                filtTypes[(size_t) i]->setBounds (c.removeFromTop (18));
            c.removeFromTop (4);
            auxCardLab.setBounds (c.removeFromTop (14));
            auxCardBox.setBounds (c.removeFromTop (22));
        }

        // ---- Envelopes: match the original four-knob ADSR face; DKY2 remains in GLOBAL ----
        {
            auto c = area.reduced (4, 4);
            c.removeFromTop (16); // row title is painted in the header strip below
            for (int e = 0; e < 3; ++e)
            {
                juce::Array<juce::String> names;
                for (auto suffix : { "A", "D", "S", "R" })
                    names.add (juce::String ("E") + juce::String (e + 1) + " " + suffix);
                placeKnobs (names, c.removeFromTop (68));
                c.removeFromTop (16);
            }
            placeKnobs ({ "ENV1 AMT", "ENV3 AMT" }, c.removeFromTop (68));
        }

        // ---- Global/extension window: all extra decoded controls stay available ----
        if (matrixOpen)
        {
            auto ext = getLocalBounds().removeFromBottom (kExtH).reduced (12);
            ext.removeFromTop (16);
            auto putKnob = [this] (const juce::String& t, juce::Rectangle<int> cell)
            {
                auto* k = findKnob (t);
                if (k == nullptr || cell.isEmpty()) return;
                const int d = juce::jmax (16, juce::jmin (cell.getWidth(), cell.getHeight() - 12));
                juce::Rectangle<int> dial (d, d);
                dial.setCentre (cell.getCentreX(), cell.getY() + d / 2);
                k->slider.setBounds (dial);
                k->label.setBounds (cell.getX(), cell.getY() + d, cell.getWidth(), juce::jmax (10, cell.getHeight() - d));
            };

            // -- column 1: controller destinations and their two amounts --
            {
                auto c = ext.removeFromLeft (400);
                static const char* amtT[6][2] = { { "MW A1", "MW A2" }, { "DY A1", "DY A2" },
                                                  { "BN A1", "BN A2" }, { "PR A1", "PR A2" },
                                                  { "C1 A1", "C1 A2" }, { "C2 A1", "C2 A2" } };
                for (int r = 0; r < 6; ++r)
                {
                    auto row = c.removeFromTop (50);
                    auto lr = row.removeFromLeft (64);
                    dests[(size_t) (r * 2)]->lab.setBounds (lr.getX(), lr.getY() + 4, 64, 14);
                    auto b1 = row.removeFromLeft (104).removeFromTop (40);
                    dests[(size_t) (r * 2)]->box.setBounds (b1.getX(), b1.getY() + 14, b1.getWidth(), 22);
                    putKnob (amtT[r][0], row.removeFromLeft (34).removeFromTop (34));
                    auto b2 = row.removeFromLeft (104).removeFromTop (40);
                    dests[(size_t) (r * 2 + 1)]->box.setBounds (b2.getX(), b2.getY() + 14, b2.getWidth(), 22);
                    putKnob (amtT[r][1], row.removeFromLeft (34).removeFromTop (34));
                }
            }

            // TYPE is selected from the clickable faceplate list; this host dropdown remains here too.
            {
                auto c = ext;
                c.removeFromTop (6 * 46 + 6);
                auto trow = c.removeFromTop (30);
                typeBox.setBounds (trow.removeFromLeft (180).removeFromTop (22));
            }

            // -- column 2: destinations, amounts, XMOD and waveform selectors --
            {
                auto c = ext.removeFromLeft (380);
                auto row = c.removeFromTop (46);
                auto lr = row.removeFromLeft (44); dests[12]->lab.setBounds (lr);
                dests[12]->box.setBounds (row.removeFromLeft (108).removeFromTop (22));
                dests[13]->box.setBounds (row.removeFromLeft (108).removeFromTop (22));
                dests[14]->box.setBounds (row.removeFromLeft (108).removeFromTop (22));

                row = c.removeFromTop (46);
                lr = row.removeFromLeft (44); dests[15]->lab.setBounds (lr);
                dests[15]->box.setBounds (row.removeFromLeft (108).removeFromTop (22));
                dests[16]->box.setBounds (row.removeFromLeft (108).removeFromTop (22));
                dests[17]->box.setBounds (row.removeFromLeft (108).removeFromTop (22));

                row = c.removeFromTop (46);
                lr = row.removeFromLeft (44); dests[18]->lab.setBounds (lr);
                dests[18]->box.setBounds (row.removeFromLeft (112).removeFromTop (22));
                dests[19]->box.setBounds (row.removeFromLeft (112).removeFromTop (22));
                dests[20]->box.setBounds (row.removeFromLeft (112).removeFromTop (22));

                row = c.removeFromTop (46);
                lr = row.removeFromLeft (44); dests[21]->lab.setBounds (lr);
                dests[21]->box.setBounds (row.removeFromLeft (100).removeFromTop (22));
                putKnob ("E3 A1", row.removeFromLeft (36).removeFromTop (34));
                putKnob ("E3 A2", row.removeFromLeft (36).removeFromTop (34));
                putKnob ("E3 A3", row.removeFromLeft (36).removeFromTop (34));

                row = c.removeFromTop (46);
                lr = row.removeFromLeft (30); dests[22]->lab.setBounds (lr);
                dests[22]->box.setBounds (row.removeFromLeft (76).removeFromTop (22));
                lr = row.removeFromLeft (30); dests[23]->lab.setBounds (lr);
                dests[23]->box.setBounds (row.removeFromLeft (76).removeFromTop (22));
            }

            // -- column 3: envelope delay/dynamics/decay-2 + per-voice pan --
            {
                auto c = ext;
                for (int e = 0; e < 3; ++e)
                {
                    auto row = c.removeFromTop (45);
                    auto lr = row.removeFromLeft (38);
                    envExtLab[(size_t) e].setBounds (lr);
                    envExtLab[(size_t) e].setText ("E" + juce::String (e + 1), juce::dontSendNotification);
                    putKnob ("DLY" + juce::String (e + 1), row.removeFromLeft (36).removeFromTop (34));
                    putKnob ("DYN" + juce::String (e + 1), row.removeFromLeft (36).removeFromTop (34));
                    putKnob ("E" + juce::String (e + 1) + " DKY2", row.removeFromLeft (44).removeFromTop (34));
                }
                for (int v = 0; v < 4; ++v)
                {
                    auto row = c.removeFromTop (45);
                    auto lr = row.removeFromLeft (38);
                    panLab[(size_t) v].setBounds (lr);
                    panLab[(size_t) v].setText (juce::String (v + 1), juce::dontSendNotification);
                    putKnob ("P" + juce::String (v + 1) + " POS",  row.removeFromLeft (36).removeFromTop (34));
                    putKnob ("P" + juce::String (v + 1) + " RATE", row.removeFromLeft (36).removeFromTop (34));
                    putKnob ("P" + juce::String (v + 1) + " DPTH", row.removeFromLeft (36).removeFromTop (34));
                }
            }
        }
    }

    //==========================================================================
private:
    Omega8AudioProcessor& proc;
    OmegaLookAndFeel lnf;

    // automatable
    juce::Slider morph, vol, arpRate;
    juce::ToggleButton arpOn { "ARPEGGIATOR" };
    juce::ComboBox arpPat, arpOct { "" };
    juce::ComboBox auxCardBox;          // r9
    juce::Label    auxCardLab;          // r9
    juce::Label    volLab;
    // ---- r11: Programmer по скрину CODE (8 ламп, энкодер, стрелки Bank/Part) ----
    juce::ToggleButton lamp[8];
    ArcSlider           enc { };        // Q-dial: выбор патча (параметр presetA)
    juce::TextButton    arrL { "<" }, arrR { ">" }, arrU { "^" }, arrD { "v" };
    juce::ComboBox typeBox;             // r11: TYPE фильтра в матрице
    juce::Label    typeLab { "", "TYPE" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> encAtt;
    // r11: строка значений на LCD после кручения ручки
    juce::String lastEdit;
    int          lastEditVal = 0;
    juce::int64  lastEditUntil = 0;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> morphAtt, volAtt, arpRateAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> arpOnAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> arpPatAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> auxCardAtt;   // r9

    // programmer
    struct Lcd : juce::Component
    {
        juce::String text;
        void paint (juce::Graphics& g) override
        {
            auto box = getLocalBounds().toFloat().reduced (1.0f);
            g.setColour (juce::Colour (0xff0b0d0e));
            g.fillRoundedRectangle (box, 4.0f);
            g.setColour (juce::Colour (0xff5a6a75));
            g.drawRoundedRectangle (box, 4.0f, 1.0f);
            g.setColour (juce::Colour (0xffe9e4d4));
            g.setFont (juce::Font (13.0f, juce::Font::bold));
            g.drawFittedText (text, getLocalBounds().reduced (6), juce::Justification::centred, 3);
        }
    } lcd;
    juce::String lcdText;

    juce::TextButton prevA { "" }, nextA { "" }, prevB { "" }, nextB { "" };
    juce::Label labA { "", "PATCH A" }, labB { "", "PATCH B" };
    juce::ComboBox patchList;
    juce::TextButton loadBtn;
    juce::TextButton matrixBtn { "GLOBAL" };  // r7: как на железе (Programmer Global)
    bool matrixOpen = false;
    static constexpr int kPanelH  = 506;       // r15: пропорции интерфейса как на референсе (~2.33:1)
    static constexpr int kExtH    = 360;       // extend-окно матрицы
    juce::ComboBox arpPat2;   // reserved
    juce::Label lfo1Dest, lfo2Dest, env3Dest, morphLab { "", "BLEND  A - B" }, octLabel, tuneLabel;
    juce::Label envExtLab[3];   // r6: метки E1..E3 в extend-зоне
    juce::Label panLab[4];      // r6: метки 1..4 (PAN) в extend-зоне
    juce::ToggleButton glideLed { "GLIDE" }, subLed { "SUB" },
                       wv1t { "TRI" }, wv1s { "SAW" }, wv1p { "PULSE" },
                       wv2t { "TRI" }, wv2s { "SAW" }, wv2p { "PULSE" };

    std::vector<std::unique_ptr<DisplayKnob>> knobs;
    std::vector<std::unique_ptr<juce::ToggleButton>> filtTypes;
    std::unique_ptr<juce::FileChooser> chooser;

    DisplayKnob* findKnob (const juce::String& t)
    {
        for (auto& k : knobs) if (k->title == t) return k.get();
        return nullptr;
    }

    juce::String destText (int lfo)
    {
        const auto p = proc.getEditPatch();
        static const char* names[] = { "", "FRE1", "FRE2", "1&2F", "LEV1", "LEV2", "PW1", "PW2",
                                       "1&2P", "FILT", "RESO", "LEVN", "XMOD", "EA1", "EA3", "EXT",
                                       "LF1R", "LF2R", "LF1D", "LF2D", "1&2D", "1&2R", "PAND", "PANR", "PAN" };
        auto d1 = lfo == 1 ? p.at (omega8::off::LFO1_DEST1) : p.at (omega8::off::LFO2_DEST1);
        auto d2 = lfo == 1 ? p.at (omega8::off::LFO1_DEST2) : p.at (omega8::off::LFO2_DEST2);
        auto d3 = lfo == 1 ? p.at (omega8::off::LFO1_DEST3) : p.at (omega8::off::LFO2_DEST3);
        auto nm = [&] (int d) { return d < 25 ? names[d] : "?"; };
        return juce::String::formatted ("D1:%s  D2:%s  D3:%s", nm (d1), nm (d2), nm (d3));
    }

    juce::String env3Text()
    {
        const auto p = proc.getEditPatch();
        static const char* names[] = { "OFF", "FRE1", "FRE2", "1&2F", "LEV1", "LEV2", "PW1", "PW2",
                                       "1&2P", "FILT", "RESO", "LEVN", "XMOD", "EA1", "EA3", "EXT",
                                       "LF1R", "LF2R", "LF1D", "LF2D", "1&2D", "1&2R", "PAND", "PANR", "PAN" };
        auto d = p.at (omega8::off::ENV3_DEST1);
        return juce::String::formatted ("ENV3 > %s x%d", d < 25 ? names[d] : "?", p.at (omega8::off::ENV3_AMT1));
    }

    void placeKnobRow (juce::Array<juce::String> titles, juce::Rectangle<int> r, bool = false)
    {
        placeKnobs (titles, r);
    }

    void placeKnobs (const juce::Array<juce::String>& titles, juce::Rectangle<int> r)
    {
        if (titles.isEmpty() || r.isEmpty()) return;
        const int cellW = r.getWidth() / titles.size();
        for (int i = 0; i < titles.size(); ++i)
        {
            auto* k = findKnob (titles[(size_t) i]);
            auto cell = r.removeFromLeft (cellW);
            if (k == nullptr) continue;
            const int dialSize = juce::jmax (16, juce::jmin (cellW - 4, cell.getHeight() - 18));
            juce::Rectangle<int> dial (dialSize, dialSize);
            dial.setCentre (cell.getCentreX(), cell.getY() + 2 + dialSize / 2);
            k->slider.setBounds (dial);
            const int labelY = cell.getY() + dialSize + 4;
            const int labelH = juce::jmax (10, cell.getBottom() - labelY);
            k->label.setBounds (cell.getX(), labelY, cell.getWidth(), labelH);
        }
    }

    void rebuildPatchList()
    {
        patchList.clear (juce::dontSendNotification);
        for (int i = 0; i < 128; ++i)
            patchList.addItem (juce::String (i + 1).paddedLeft ('0', 3) + " " + proc.patchNameAt (i), i + 1);
    }

    void setFilterTypeFromUI (int type)
    {
        type = juce::jlimit (0, 6, type);
        if (auto* fp = proc.apvts.getParameter ("filtType"))
        {
            fp->beginChangeGesture();
            fp->setValueNotifyingHost (fp->convertTo0to1 ((float) type));
            fp->endChangeGesture();
        }
        proc.setPatchByte (proc.editSlot(), omega8::off::FILT_TYPE, (uint8_t) type);
    }

    //==========================================================================
    void buttonClicked (juce::Button* b) override
    {
        auto setPreset = [this] (int slot, int delta)
        {
            const char* id = slot == 0 ? "presetA" : "presetB";
            float cur = proc.apvts.getRawParameterValue (id)->load();
            int v = juce::jlimit (0, 127, (int) cur + delta);
            if (auto* p = proc.apvts.getParameter (id))
            {
                p->beginChangeGesture();
                p->setValueNotifyingHost (p->convertTo0to1 ((float) v));
                p->endChangeGesture();
            }
        };
        auto setPresetAbs = [this] (int slot, int v)
        {
            const char* id = slot == 0 ? "presetA" : "presetB";
            v = juce::jlimit (0, 127, v);
            if (auto* p = proc.apvts.getParameter (id))
            {
                p->beginChangeGesture();
                p->setValueNotifyingHost (p->convertTo0to1 ((float) v));
                p->endChangeGesture();
            }
        };
        bool lampHit = false;
        for (int i = 0; i < 8; ++i)
            if (b == &lamp[i])
            {
                lampHit = true;
                int cur = (int) proc.apvts.getRawParameterValue ("presetA")->load();
                setPresetAbs (0, (cur / 8) * 8 + i);      // r11: 8 ламп = программы группы
            }
        if (lampHit) return;
        if (b == &arrL) { setPreset (0, -1); return; }
        if (b == &arrR) { setPreset (0, +1); return; }
        if (b == &arrU || b == &arrD)                       // ▲▼ = смена банка (мануал)
        {
            int cur = (int) proc.apvts.getRawParameterValue ("presetA")->load();
            int idx = cur % 32, banky = cur / 32;
            banky = (banky + (b == &arrU ? 3 : 1)) % 4;
            setPresetAbs (0, banky * 32 + idx);
            return;
        }
        if (b == &subLed)
        {
            int sw = proc.getEditPatch().at (omega8::off::SUB_WAVE);
            proc.setPatchByte (proc.editSlot(), omega8::off::SUB_WAVE, (uint8_t) (sw > 0 ? 0 : 1));
            return;
        }
        for (int i = 0; i < (int) filtTypes.size(); ++i)
            if (b == filtTypes[(size_t) i].get())
            {
                setFilterTypeFromUI (i);
                return;
            }
        if (b == &prevA) setPreset (0, -1);
        else if (b == &nextA) setPreset (0, +1);
        else if (b == &prevB) setPreset (1, -1);
        else if (b == &nextB) setPreset (1, +1);
        else if (b == &matrixBtn)
        {
            matrixOpen = matrixBtn.getToggleState();
            setSize (1180, matrixOpen ? kPanelH + kExtH : kPanelH);
        }
        else if (b == &glideLed)
        {
            int fl = proc.getEditPatch().at (omega8::off::GLIDE_FLAGS);
            int nv = glideLed.getToggleState() ? (fl | 0x40) : (fl & ~0x40);
            proc.setPatchByte (proc.editSlot(), omega8::off::GLIDE_FLAGS, (uint8_t) nv);
        }
        else if (b == &wv1t || b == &wv1s || b == &wv1p)
        {
            int wb = proc.getEditPatch().at (omega8::off::WAVE_BITS);
            int bit = (b == &wv1t ? 1 : (b == &wv1s ? 2 : 4));
            auto* t = (b == &wv1t ? &wv1t : (b == &wv1s ? &wv1s : &wv1p));
            int nv = t->getToggleState() ? (wb | bit) : (wb & ~bit);
            proc.setPatchByte (proc.editSlot(), omega8::off::WAVE_BITS, (uint8_t) nv);
        }
        else if (b == &wv2t || b == &wv2s || b == &wv2p)
        {
            int w2 = (b == &wv2t ? 1 : (b == &wv2s ? 2 : 0));
            proc.setPatchByte (proc.editSlot(), omega8::off::WAVE2, (uint8_t) w2);
        }
        else if (b == &loadBtn)
        {
            chooser = std::make_unique<juce::FileChooser> ("Load Omega 8 bank (.syx / .mid)",
                proc.lastBankFile.exists() ? proc.lastBankFile
                    : juce::File::getSpecialLocation (juce::File::userDocumentsDirectory),
                "*.syx;*.mid");
            chooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                [safe = juce::Component::SafePointer<Omega8AudioProcessorEditor> (this)] (const juce::FileChooser& fc)
                {
                    if (safe == nullptr) return;
                    auto f = fc.getResult();
                    if (f.existsAsFile() && safe->proc.loadBankFile (f))
                        safe->rebuildPatchList();
                });
        }
    }

    void comboBoxChanged (juce::ComboBox* cb) override
    {
        if (cb == &typeBox)
        {
            int id = typeBox.getSelectedId();
            if (id >= 1) setFilterTypeFromUI (id - 1);
            return;
        }
        if (cb == &arpOct)
        {
            int id = arpOct.getSelectedId();
            if (auto* p = proc.apvts.getParameter ("arpOct"))
            {
                p->beginChangeGesture();
                p->setValueNotifyingHost (p->convertTo0to1 ((float) juce::jlimit (1, 4, id)));
                p->endChangeGesture();
            }
            return;
        }
        if (cb == &patchList)
        {
            int v = patchList.getSelectedId() - 1;
            if (juce::isPositiveAndBelow (v, 128))
            {
                if (auto* p = proc.apvts.getParameter ("presetA"))
                {
                    p->beginChangeGesture();
                    p->setValueNotifyingHost (p->convertTo0to1 ((float) v));
                    p->endChangeGesture();
                }
            }
        }
    }
};
