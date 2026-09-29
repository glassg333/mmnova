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
        auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (2.0f);
        auto size   = juce::jmin (bounds.getWidth(), bounds.getHeight());
        auto centre = bounds.getCentre();
        auto radius = size * 0.5f - 2.0f;

        // knob body
        juce::ColourGradient grad (juce::Colour (0xff5a646e), centre.getX(), centre.getY(),
                                   juce::Colour (0xff22272d), centre.getX(), centre.getY() + radius, false);
        g.setGradientFill (grad);
        g.fillEllipse (juce::Rectangle<float> (radius * 2, radius * 2).withCentre (centre));

        g.setColour (juce::Colour (0xff14171a));
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
                          : (on ? juce::Colour (0xffc05a1e) : juce::Colour (0xff3a4048)));
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

        addAndMakeVisible (lcd);
        addAndMakeVisible (labA);
        addAndMakeVisible (labB);
        addAndMakeVisible (morphLab);
        addAndMakeVisible (octLabel);
        addAndMakeVisible (tuneLabel);
        addAndMakeVisible (glideLed);
        addAndMakeVisible (subLed);
        for (auto* w : { &wv1t, &wv1s, &wv1p, &wv2t, &wv2s, &wv2p }) addAndMakeVisible (*w);
        labA.setFont (juce::Font (10.0f));
        labB.setFont (juce::Font (10.0f));
        octLabel.setFont (juce::Font (10.0f));
        tuneLabel.setFont (juce::Font (10.0f));
        octLabel.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
        tuneLabel.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));

        // ---- display-only patch knobs ----
        addKnob ("GLIDE",     [] (const omega8::Patch& q) { return (double) q.at (omega8::off::GLIDE_TIME); });
        addKnob ("OSC1 FREQ", [] (const omega8::Patch& q) { return (double) q.at (omega8::off::OSC1_FREQ); });
        addKnob ("OSC2 FREQ", [] (const omega8::Patch& q) { return (double) q.at (omega8::off::OSC2_FREQ); });
        addKnob ("PWM 1",     [] (const omega8::Patch& q) { return (double) q.at (omega8::off::PWM1); });
        addKnob ("PWM 2",     [] (const omega8::Patch& q) { return (double) q.at (omega8::off::PWM2); });
        addKnob ("OSC1 LEV",  [] (const omega8::Patch& q) { return (double) q.at (omega8::off::OSC1_LEVEL); });
        addKnob ("OSC2 LEV",  [] (const omega8::Patch& q) { return (double) q.at (omega8::off::OSC2_LEVEL); });
        addKnob ("NOISE",     [] (const omega8::Patch& q) { return (double) q.at (omega8::off::NOISE_LEVEL); });
        addKnob ("SUB",       [] (const omega8::Patch& q) { return (double) q.at (omega8::off::SUB_WAVE); });
        addKnob ("FINE",      [] (const omega8::Patch& q) { return (double) q.at (omega8::off::OSC2_FINE); });
        addKnob ("TUNE",      [] (const omega8::Patch& q) { return (double) q.at (omega8::off::MASTER_TUNE); });
        addKnob ("EXT IN",    [] (const omega8::Patch& q) { return (double) q.at (omega8::off::EXT_IN); });

        addKnob ("CUTOFF",    [] (const omega8::Patch& q) { return (double) q.at (omega8::off::CUTOFF); });
        addKnob ("RESO",      [] (const omega8::Patch& q) { return (double) q.at (omega8::off::RESO); });
        addKnob ("TRACKING",  [] (const omega8::Patch& q) { return (double) q.at (omega8::off::TRACKING); });
        addKnob ("ENV1 AMT",  [] (const omega8::Patch& q) { return (double) q.at (omega8::off::ENV1_AMT); });
        addKnob ("HPF",       [] (const omega8::Patch& q) { return (double) q.at (omega8::off::HPF); });
        addKnob ("HPR",       [] (const omega8::Patch& q) { return (double) q.at (omega8::off::HPR); });

        addKnob ("LFO1 RATE", [] (const omega8::Patch& q) { return (double) q.at (omega8::off::LFO1_RATE); });
        addKnob ("LFO1 D1",   [] (const omega8::Patch& q) { return (double) q.at (omega8::off::LFO1_DEPTH1); });
        addKnob ("LFO1 D2",   [] (const omega8::Patch& q) { return (double) q.at (omega8::off::LFO1_DEPTH2); });
        addKnob ("LFO1 D3",   [] (const omega8::Patch& q) { return (double) q.at (omega8::off::LFO1_DEPTH3); });
        addKnob ("LFO2 RATE", [] (const omega8::Patch& q) { return (double) q.at (omega8::off::LFO2_RATE); });
        addKnob ("LFO2 D1",   [] (const omega8::Patch& q) { return (double) q.at (omega8::off::LFO2_DEPTH1); });
        addKnob ("LFO2 D2",   [] (const omega8::Patch& q) { return (double) q.at (omega8::off::LFO2_DEPTH2); });
        addKnob ("LFO2 D3",   [] (const omega8::Patch& q) { return (double) q.at (omega8::off::LFO2_DEPTH3); });
        addKnob ("XMOD",      [] (const omega8::Patch& q) { return (double) q.at (omega8::off::XMOD_DPTH); });
        addKnob ("ENV3 AMT",  [] (const omega8::Patch& q) { return (double) q.at (omega8::off::ENV3_AMT); });

        const char* envNames[3][5] = {
            { "A", "D", "Dk2", "S", "R" }, { "A", "D", "Dk2", "S", "R" }, { "A", "D", "Dk2", "S", "R" } };
        const int envBase[3] = { omega8::off::ENV1_ATK, omega8::off::ENV2_ATK, omega8::off::ENV3_ATK };
        for (int e = 0; e < 3; ++e)
            for (int k = 0; k < 5; ++k)
            {
                int ofs = envBase[e] + k;
                juce::String title = juce::String ("E") + juce::String (e + 1) + " " + envNames[e][k];
                addKnob (title, [ofs] (const omega8::Patch& q) { return (double) q.at (ofs); });
            }

        rebuildPatchList();
        setSize (1180, 470);
        startTimerHz (20);
    }

    ~Omega8AudioProcessorEditor() override
    {
        setLookAndFeel (nullptr);
    }

    //==========================================================================
    struct DisplayKnob
    {
        juce::Slider slider;
        juce::Label  label;
        std::function<double(const omega8::Patch&)> read;
        juce::String title;
    };

    void addKnob (juce::String title, std::function<double(const omega8::Patch&)> read)
    {
        auto k = std::make_unique<DisplayKnob>();
        k->read = std::move (read);
        k->title = title;
        k->slider.setSliderStyle (juce::Slider::Rotary);
        k->slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        k->label.setText (title, juce::NotificationType::dontSendNotification);
        k->label.setJustificationType (juce::Justification::centred);
        k->label.setFont (juce::Font (10.0f));
        k->label.setColour (juce::Label::textColourId, juce::Colour (0xffcfd8e0));
        addAndMakeVisible (k->slider);
        addAndMakeVisible (k->label);
        knobs.push_back (std::move (k));
    }

    void timerCallback() override
    {
        const auto& p = proc.currentPatch();
        for (auto& k : knobs)
        {
            auto v = k->read (p);
            if (std::abs (k->slider.getValue() - v) > 0.001)
                k->slider.setValue (v, juce::dontSendNotification);
        }
        auto name = proc.patchNameAt (proc.presetA());
        int a = proc.presetA(), b = proc.presetB();
        float m = proc.morphAmount();
        lcdText = juce::String::formatted ("%s\nA:%03d  B:%03d  MORPH:%3.0f%%",
                                           name.toRawUTF8(), a + 1, b + 1, m * 100.0f);
        lcd.text = lcdText;
        lcd.repaint();

        // filter type LEDs
        int ft = p.at (omega8::off::FILT_TYPE);
        for (int i = 0; i < 7 && i < (int) filtTypes.size(); ++i)
            filtTypes[(size_t) i]->setToggleState (i == ft, juce::dontSendNotification);

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
        octLabel.setText ("OCT: " + juce::String (((ob >> 4) & 0x0F) - 4), juce::dontSendNotification);
        tuneLabel.setText ("TUNE: " + juce::String (p.at (omega8::off::MASTER_TUNE)) + "%",
                           juce::dontSendNotification);
    }

    //==========================================================================
    void paint (juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();

        // rack panel
        juce::ColourGradient grad (juce::Colour (0xff3b4652), 0, 0,
                                   juce::Colour (0xff242c35), 0, getHeight(), false);
        g.setGradientFill (grad);
        g.fillAll();

        g.setColour (juce::Colour (0xff1b2127));
        g.fillRect (0, 0, getWidth(), 6);
        g.fillRect (0, getHeight() - 6, getWidth(), 6);

        // brand strip
        g.setColour (juce::Colour (0xff2e3742));
        g.fillRect (4, 8, 26, getHeight() - 16);

        // vertical brand text
        g.saveState();
        g.setColour (juce::Colours::white.withAlpha (0.85f));
        g.setFont (juce::Font (11.0f, juce::Font::bold));
        g.addTransform (juce::AffineTransform::rotation ((float) -juce::MathConstants<double>::halfPi,
                                                         17.0f, (float) (getHeight() - 14)));
        g.drawText ("STUDIO ELECTRONICS  OMEGA 8 / CODE",
                    0.0f, 0.0f, (float) (getHeight() - 28), 14.0f, juce::Justification::centred);
        g.restoreState();

        // section separators + titles
        auto line = [&] (float x, float top = 8.0f, float bot = -8.0f)
        {
            g.setColour (juce::Colours::white.withAlpha (0.25f));
            g.drawVerticalLine ((int) x, top, bot < 0 ? (float) getHeight() + bot : bot);
        };
        for (float x : { 150.0f, 306.0f, 470.0f, 756.0f, 892.0f }) line (x);

        g.setColour (juce::Colours::white);
        g.setFont (juce::Font (12.0f, juce::Font::bold));
        g.drawText ("Multi/Midi",   36, 10, 110, 14, juce::Justification::centred);
        g.drawText ("Modulation",  154, 10, 148, 14, juce::Justification::centred);
        g.drawText ("Programmer",  310, 10, 156, 14, juce::Justification::centred);
        g.drawText ("Oscillators", 474, 10, 278, 14, juce::Justification::centred);
        g.drawText ("Filter",      760, 10, 128, 14, juce::Justification::centred);
        g.drawText ("Envelopes",   896, 10, 276, 14, juce::Justification::centred);

        // env row titles
        g.setFont (juce::Font (10.0f));
        g.drawText ("VCF (1)",   896, 44, 44, 12, juce::Justification::centredLeft);
        g.drawText ("VCA (2)",   896, 150, 44, 12, juce::Justification::centredLeft);
        g.drawText ("ASSIGN (3)",896, 256, 52, 12, juce::Justification::centredLeft);

        // filter type LEDs
        static const char* ftNames[7] = { "SEM", "BP", "HP", "BR", "MINI", "OBER", "CS80" };
        (void) ftNames;

        // lcd frame
        g.setColour (juce::Colour (0xff10151c));
        g.fillRoundedRectangle (314, 34, 148, 46, 5);
        g.setColour (juce::Colour (0xff7f9db3));
        g.drawRoundedRectangle (314, 34, 148, 46, 5, 1.0f);

        // omega badge
        g.setColour (juce::Colour (0xffd8dee6));
        g.setFont (juce::Font (16.0f, juce::Font::bold));
        g.drawText ("omega 8", 900, getHeight() - 44, 260, 30, juce::Justification::centredRight);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced (10);
        area.removeFromLeft (28);   // brand strip

        // ---- Multi/Midi column ----
        {
            auto c = area.removeFromLeft (112).reduced (4, 4);
            auto r2 = c.removeFromTop (70);
            // volume (automatable) + glide display
            vol.setBounds (r2.removeFromLeft (56).reduced (4));
            auto gk = findKnob ("GLIDE");
            if (gk != nullptr)
            {
                gk->slider.setBounds (r2.removeFromLeft (52).reduced (4));
                gk->label.setBounds (gk->slider.getX(), r2.getY() - 12, 52, 12);
            }
            glideLed.setBounds (c.removeFromTop (20).reduced (8, 2));
            octLabel.setBounds (c.removeFromTop (18));
            tuneLabel.setBounds (c.removeFromTop (18));
        }

        // ---- Modulation column ----
        {
            auto c = area.removeFromLeft (150).reduced (4, 4);
            auto l1 = c.removeFromTop (14);
            lfo1Dest.setText (destText (1), juce::dontSendNotification);
            addAndMakeVisible (lfo1Dest);
            lfo1Dest.setBounds (l1);
            placeKnobs ({ "LFO1 RATE", "LFO1 D1", "LFO1 D2", "LFO1 D3" }, c.removeFromTop (62));
            auto l2 = c.removeFromTop (14);
            lfo2Dest.setText (destText (2), juce::dontSendNotification);
            lfo2Dest.setFont (juce::Font (10.0f));
            lfo2Dest.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
            addAndMakeVisible (lfo2Dest);
            lfo2Dest.setBounds (l2);
            placeKnobs ({ "LFO2 RATE", "LFO2 D1", "LFO2 D2", "LFO2 D3" }, c.removeFromTop (62));
            placeKnobs ({ "XMOD", "ENV3 AMT" }, c.removeFromTop (62));
            env3Dest.setText (env3Text(), juce::dontSendNotification);
            env3Dest.setFont (juce::Font (10.0f));
            env3Dest.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
            addAndMakeVisible (env3Dest);
            env3Dest.setBounds (c.removeFromTop (16));
        }

        // ---- Programmer column ----
        {
            auto c = area.removeFromLeft (158).reduced (4, 4);
            lcd.setBounds (c.removeFromTop (46).reduced (2));
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
            loadBtn.setBounds (c.removeFromTop (22));
            c.removeFromTop (6);
            arpPanel.setBounds (c);
            addAndMakeVisible (arpPanel);
            // arp internals (children of the editor — offset by panel origin)
            auto ap = arpPanel.getBounds();
            arpOn.setBounds (ap.getX() + 4, ap.getY() + 4, ap.getWidth() - 8, 22);
            arpRate.setBounds (ap.getX() + 4, ap.getY() + 30, 56, 56);
            arpPat.setBounds (ap.getX() + 64, ap.getY() + 32, ap.getWidth() - 70, 24);
            arpOct.setBounds (ap.getX() + 64, ap.getY() + 58, ap.getWidth() - 70, 24);
        }

        // ---- Oscillators ----
        {
            auto c = area.removeFromLeft (278).reduced (4, 4);
            auto waveRow1 = c.removeFromTop (20);
            wv1t.setBounds (waveRow1.removeFromLeft (44));
            wv1s.setBounds (waveRow1.removeFromLeft (44));
            wv1p.setBounds (waveRow1.removeFromLeft (44));
            subLed.setBounds (waveRow1.removeFromLeft (44));
            placeKnobs ({ "OSC1 FREQ", "PWM 1", "OSC1 LEV", "NOISE" }, c.removeFromTop (74));
            auto waveRow2 = c.removeFromTop (20);
            wv2t.setBounds (waveRow2.removeFromLeft (44));
            wv2s.setBounds (waveRow2.removeFromLeft (44));
            wv2p.setBounds (waveRow2.removeFromLeft (44));
            placeKnobs ({ "OSC2 FREQ", "PWM 2", "OSC2 LEV", "FINE" }, c.removeFromTop (74));
            placeKnobs ({ "SUB", "TUNE", "EXT IN" }, c.removeFromTop (74));
        }

        // ---- Filter ----
        {
            auto c = area.removeFromLeft (130).reduced (4, 4);
            placeKnobs ({ "CUTOFF", "RESO" }, c.removeFromTop (74));
            placeKnobs ({ "TRACKING", "ENV1 AMT" }, c.removeFromTop (74));
            placeKnobs ({ "HPF", "HPR" }, c.removeFromTop (74));
            auto ft = c.removeFromTop (90);
            static const char* ftNames[7] = { "SEM", "BP", "HP", "BR", "MIN", "OB", "CS" };
            if (filtTypes.empty())
            {
                for (int i = 0; i < 7; ++i)
                {
                    auto b = std::make_unique<juce::ToggleButton> (ftNames[i]);
                    b->setClickingTogglesState (false);
                    b->setInterceptsMouseClicks (false, false);
                    addAndMakeVisible (*b);
                    filtTypes.push_back (std::move (b));
                }
            }
            for (int i = 0; i < 7; ++i)
                filtTypes[(size_t) i]->setBounds (ft.removeFromTop (12));
        }

        // ---- Envelopes: 3 rows x 5 knobs ----
        {
            auto c = area.reduced (4, 4);
            c.removeFromTop (26);   // section title
            for (int e = 0; e < 3; ++e)
            {
                c.removeFromTop (14);   // row label
                juce::Array<juce::String> titles { "A", "D", "Dk2", "S", "R" };
                juce::Array<juce::String> names;
                for (auto& t : titles) names.add (juce::String ("E") + juce::String (e + 1) + " " + t);
                placeKnobs (names, c.removeFromTop (74));
                c.removeFromTop (32);
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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> morphAtt, volAtt, arpRateAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> arpOnAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> arpPatAtt;

    // programmer
    struct Lcd : juce::Component
    {
        juce::String text;
        void paint (juce::Graphics& g) override
        {
            g.setColour (juce::Colour (0xff0e2f66));
            g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (2.0f), 4.0f);
            g.setColour (juce::Colour (0xffd8e8ff));
            g.setFont (juce::Font (13.0f, juce::Font::bold));
            g.drawFittedText (text, getLocalBounds().reduced (6), juce::Justification::centred, 3);
        }
    } lcd;
    juce::String lcdText;

    juce::TextButton prevA { "" }, nextA { "" }, prevB { "" }, nextB { "" };
    juce::Label labA { "", "PATCH A" }, labB { "", "PATCH B" };
    juce::ComboBox patchList;
    juce::TextButton loadBtn;
    juce::Component arpPanel;
    juce::ComboBox arpPat2;   // reserved
    juce::Label lfo1Dest, lfo2Dest, env3Dest, morphLab { "", "MORPH A > B" }, octLabel, tuneLabel;
    juce::ToggleButton glideLed { "GLIDE" }, subLed { "SUB" },
                       wv1t { "tri" }, wv1s { "saw" }, wv1p { "pwm" },
                       wv2t { "tri" }, wv2s { "saw" }, wv2p { "pwm" };

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
        const auto& p = proc.currentPatch();
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
        const auto& p = proc.currentPatch();
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
        int w = r.getWidth() / titles.size();
        for (int i = 0; i < titles.size(); ++i)
        {
            auto* k = findKnob (titles[(size_t) i]);
            if (k == nullptr) continue;
            auto cell = r.removeFromLeft (w);
            k->slider.setBounds (cell.getX(), cell.getY() + 2, cell.getWidth(), cell.getWidth());
            int sh = juce::jmin (cell.getWidth(), 52);
            k->label.setBounds (cell.getX(), cell.getY() + cell.getWidth() - 2, cell.getWidth(), 14);
            juce::ignoreUnused (sh);
        }
    }

    void rebuildPatchList()
    {
        patchList.clear (juce::dontSendNotification);
        for (int i = 0; i < 128; ++i)
            patchList.addItem (juce::String (i + 1).paddedLeft ('0', 3) + " " + proc.patchNameAt (i), i + 1);
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
        if (b == &prevA) setPreset (0, -1);
        else if (b == &nextA) setPreset (0, +1);
        else if (b == &prevB) setPreset (1, -1);
        else if (b == &nextB) setPreset (1, +1);
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
