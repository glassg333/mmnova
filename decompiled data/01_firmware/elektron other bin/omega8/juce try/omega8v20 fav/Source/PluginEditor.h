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
//==============================================================================
// r18.1: LcdMatrix — extend-окно, 2-колоночная раскладка как в оригинале
// OMEGACODE (скриншот-референс): левый блок PARAMETERS (сетка lvl/freq +
// иконки волн + pwm), LFO/ENV3, MOD MATRIX; центр ENVELOPES + per-voice pan;
// правый GLIDE/PAN.
//   enum-ячейки (▼): левый клик — выпадающий список;
//   bit-ячейки (glide/mode): переключают бит байта 1;
//   int-ячейки: драг (dx+dy, ~1 px = 1 шаг), правый клик — ввод числа;
//   wave-ячейки: три кликабельные иконки (WAVE1 битмаска / WAVE2 / SUB).
struct LcdMatrix : juce::Component, public juce::SettableTooltipClient
{
    std::function<omega8::Patch()> getPatch;
    std::function<void (int ofs, uint8_t v)> setByte;

    struct Cell
    {
        juce::String label;
        juce::String tip;
        int ofs = -1;                       // -1 = только заголовок
        bool isEnum = false;
        const juce::StringArray* choices = nullptr;
        std::function<void (int)> enumAction = nullptr;   // кастомная запись
        const int* enumVals = nullptr;      // произвольные значения (voice {0,1,16})
        int enumN = 0;
        std::function<int (uint8_t)> tick = nullptr;       // индекс тика в меню
        uint8_t bitMask = 0;             // bit-ячейка: choices = 0/1 бита
        bool threeDigit = false;         // XMOD dpth: 000..127
        bool signedDisp = false;         // byte = display + 64 (C2 amt, pos)
        bool wave = false;               // волна: 3 иконки
        int waveSrc = 0;                 // 0=WAVE_BITS битмаска, 1=WAVE2, 2=SUB
        std::function<juce::String (int)> fmt = nullptr;
        juce::Rectangle<int> lab;
        juce::Rectangle<int> box;
    };
    std::vector<Cell> cells;
    std::vector<std::pair<juce::String, juce::Rectangle<int>>> headers;

    uint8_t cur[omega8::kPatchSize] = {};
    int dragIdx = -1;
    juce::Point<int> dragStart;
    int dragVal0 = 0;

    juce::TextEditor numEdit;               // inline-ввод числа (правый клик)
    int numEditIdx = -1;

    LcdMatrix()
    {
        addAndMakeVisible (numEdit);
        numEdit.setVisible (false);
        numEdit.setMultiLine (false);
        // JUCE 8: колбэки TextEditor — коммит по Enter и по потере фокуса
        numEdit.onReturnKey = [this] { commitNumEdit (); };
        numEdit.onFocusLost = [this] { commitNumEdit (); };
        setMouseCursor (juce::MouseCursor::PointingHandCursor);
    }

    void commitNumEdit()
    {
        if (numEditIdx < 0) return;
        Cell& c = cells[(size_t) numEditIdx];
        int v = juce::jlimit (c.signedDisp ? -64 : 0, 127, numEdit.getText().getIntValue());
        setByte (c.ofs, (uint8_t) (c.signedDisp ? v + 64 : v));
        numEditIdx = -1;
        numEdit.setVisible (false);
    }

    void buildCells()
    {
        cells.clear();
        headers.clear();
        static const char* destN[] = { "OFF","FRE1","FRE2","1&2F","LEV1","LEV2","PW1","PW2","1&2P",
                                       "FILT","RESO","LEVN","XMOD","EA1","EA3","EXT",
                                       "LF1R","LF2R","LF1D","LF2D","1&2D","1&2R","PAND","PANR","PAN" };
        static juce::StringArray destA;
        static bool destInit = false;
        if (! destInit) { for (auto* t : destN) destA.add (t); destInit = true; }
        static juce::StringArray filtA { "OBLP","OBBP","OBHP","OBBR","MINI","AUX1","AUX2" };
        static juce::StringArray xmodA { "OSC1","PW","VCF" };
        static juce::StringArray lfoW { "TRI","SQR" };
        static juce::StringArray uniA { "OFF","ON" };
        static juce::StringArray voiceA { "OFF","ON","HIGH" };
        static const int voiceV[] = { 0, 1, 16 };
        static juce::StringArray priorA { "LOW","MED","HIGH","LAST" };
        static juce::StringArray vmodA  { "FIRST","NEXT","CYCL","LAST" };
        static juce::StringArray syncA  { "SELF","KEY","OFF" };
        static juce::StringArray subA   { "OFF","PULSE","SINE","TRI","P2","SAW" };
        static juce::StringArray mtrgA  { "OFF","ENV1","ENV2","ENV3" };
        static juce::StringArray modeA  { "HALF","OCT","SUB","TRK","NORM" };
        static juce::StringArray panWA  { "TRI","SAW","SIN","SQR" };
        static juce::StringArray panKA  { "OFF","ON" };
        static juce::StringArray glideA { "OFF","ON" };
        static juce::StringArray gmodeA { "REG","LEGAT" };
        static juce::StringArray octHiA { "L-3","L-2","L-1","L","MID","H","H+","H++" };
        using namespace omega8::off;
        auto octFmt = [] (int v) -> juce::String
        {
            int hi = (v >> 4) & 0xF, lo = v & 0xF;
            double dv = (hi - 4) + (lo > 8 ? lo - 16 : lo) * 0.1;
            juce::String t (dv, 1);
            if (dv > 0) t = juce::String ("+") + t;
            return t;
        };
        auto pct128 = [] (int v) -> juce::String
        {
            return juce::String ((int) (v * 100.0 / 128.0 + 0.5)) + "%";   // 63 -> 49%, 67 -> 52%
        };
        auto tuneFmt = [] (int v) -> juce::String
        {
            return juce::String (100 + (v - 64)) + "%";                     // 64 -> 100%
        };

        auto hdr  = [this] (const juce::String& t, int x, int y, int w)
        {
            headers.push_back (std::make_pair (t, juce::Rectangle<int> (x, y, w, 14)));
        };
        auto colh = [this] (const juce::String& t, int x, int y, int w)
        {
            Cell c; c.label = t; c.tip = t; c.lab = { x, y + 2, w, 14 }; c.box = {};
            cells.push_back (c);
        };
        auto pair = [this] (const char* lab, int x, int y, int ofs,
                            bool enu = false, const juce::StringArray* ch = nullptr,
                            bool d3 = false, bool sgn = false, const char* tip = "",
                            std::function<juce::String (int)> fmt = nullptr,
                            int boxW = 36, int labW = 40)
        {
            Cell c;
            c.label = lab; c.ofs = ofs; c.isEnum = enu; c.choices = ch;
            c.threeDigit = d3; c.signedDisp = sgn; c.tip = juce::String (tip); c.fmt = std::move (fmt);
            c.lab = { x, y + 2, labW, 14 };
            c.box = { x + labW, y, boxW, 16 };
            cells.push_back (c);
        };
        auto boxOnly = [this] (int x, int y, int w, int ofs, bool enu = false,
                               const juce::StringArray* ch = nullptr, bool sgn = false,
                               const char* tip = "", bool d3 = false)
        {
            Cell c;
            c.ofs = ofs; c.isEnum = enu; c.choices = ch; c.signedDisp = sgn;
            c.threeDigit = d3; c.tip = juce::String (tip);
            c.lab = {}; c.box = { x, y, w, 16 };
            cells.push_back (c);
        };
        auto waveCell = [this] (int x, int y, int ofs, int src, const char* tip)
        {
            Cell c;
            c.ofs = ofs; c.wave = true; c.waveSrc = src; c.tip = juce::String (tip);
            c.lab = {}; c.box = { x, y, 84, 16 };
            cells.push_back (c);
        };

        // ================= ЛЕВЫЙ БЛОК: PARAMETERS =================
        hdr ("PARAMETERS", 8, 4, 120);
        // r1
        pair ("uni",    8,   22, UNISON, true, &uniA, false, false, "UNISON on/off");
        pair ("octav",  106, 22, OCTAVE, false, nullptr, false, false,
              "OCTAVE: hi nibble (4=MID), lo = 0.1-octave fine", octFmt);
        cells.back().isEnum = true;
        cells.back().choices = &octHiA;
        cells.back().tick = [] (uint8_t v) { return (v >> 4) & 0x07; };
        cells.back().enumAction = [this] (int hi)
        {
            const int lo = cur[OCTAVE] & 0x0F;
            setByte (OCTAVE, (uint8_t) (lo | (hi << 4)));
        };
        pair ("lvl",    204, 22, OSC1_LEVEL, false, nullptr, false, false, "OSC1 level", nullptr, 36, 30);
        pair ("osc1",   276, 22, OSC1_FREQ, false, nullptr, false, false, "OSC1 freq (0..63)", nullptr, 36, 30);
        waveCell (346, 22, WAVE_BITS, 0, "OSC1 wave: tri/saw/pulse bitmask (click icons)");
        pair ("pwm",    434, 22, PWM1, false, nullptr, false, false, "OSC1 pulse width %", pct128, 40, 24);
        boxOnly (496, 22, 40, XMOD_DPTH, false, nullptr, false, "XMOD depth 000..127", true);
        // r2
        pair ("voice",  8,   40, VOICE_QTY, true, &voiceA, false, false, "VOICE qty {0=OFF,1=ON,16=HIGH}");
        cells.back().enumVals = voiceV;
        cells.back().enumN = 3;
        pair ("tune",   106, 40, MASTER_TUNE, false, nullptr, false, false, "MASTER TUNE %", tuneFmt);
        pair ("",       204, 40, OSC2_LEVEL, false, nullptr, false, false, "OSC2 level", nullptr, 36, 30);
        pair ("osc2",   276, 40, OSC2_FREQ, false, nullptr, false, false, "OSC2 freq (0..63)", nullptr, 36, 30);
        waveCell (346, 40, WAVE2, 1, "OSC2 wave: 0=pulse, 1=tri, 2=saw");
        pair ("",       434, 40, PWM2, false, nullptr, false, false, "OSC2 pulse width %", pct128, 40, 0);
        pair ("fine",   496, 40, OSC2_FINE, false, nullptr, false, false, "OSC2 fine (-64..+63)", nullptr, 40, 24);
        // r3
        pair ("prior",  8,   58, PRIOR, true, &priorA, false, false, "VOICE PRIOR");
        pair ("sync",   106, 58, PAN_SYNC, true, &syncA, false, false, "PAN SYNC");
        pair ("",       204, 58, NOISE_LEVEL, false, nullptr, false, false, "Noise level", nullptr, 36, 30);
        pair ("",       276, 58, UNK36, false, nullptr, false, false, "Byte 36 (undocumented)", nullptr, 36, 30);
        waveCell (346, 58, SUB_WAVE, 2, "SUB wave: pulse/sine/tri icons (5=saw)");
        pair ("in",     434, 58, EXT_IN, false, nullptr, false, false, "EXT IN", nullptr, 40, 24);
        pair ("win",    496, 58, WIN, false, nullptr, false, false, "Window %", pct128, 40, 24);
        // r4
        pair ("vmod",   8,   76, VMODE, true, &vmodA, false, false, "VOICE MODE");
        pair ("sub",    106, 76, SUB_WAVE, true, &subA, false, false, "SUB waveform");
        pair ("envlamt",204, 76, ENV1_AMT, false, nullptr, false, false, "ENV1 -> filter amount", nullptr, 36, 34);
        pair ("invt",   276, 76, INVT, false, nullptr, false, false, "INVERT byte 72", nullptr, 36, 30);
        pair ("mode",   434, 76, OSC2_MODE, true, &modeA, false, false, "OSC2 MODE (4=NORM)");
        // r5
        pair ("mtrg",   8,   94, MTRG, true, &mtrgA, false, false, "MATRIX trigger env");
        pair ("filter", 106, 94, FILT_TYPE, true, &filtA, false, false,
              "Filter TYPE (OBLP/OBBP/OBHP/OBBR/MINI/AUX1/AUX2)");
        pair ("cutoff", 204, 94, CUTOFF, false, nullptr, false, false, "CUTOFF", nullptr, 36, 34);
        pair ("hpf",    276, 94, HPF, false, nullptr, false, false, "HP FREQ (AUX2/CS80)", nullptr, 36, 30);
        pair ("track",  434, 94, TRACKING, false, nullptr, false, false, "CUTOFF TRACK", nullptr, 40, 26);
        // r6
        colh ("xmod | dpth", 8, 112, 96);
        pair ("reso",   204, 112, RESO, false, nullptr, false, false, "RESONANCE", nullptr, 36, 34);
        pair ("hpr",    276, 112, HPR, false, nullptr, false, false, "HP RES (AUX2/CS80)", nullptr, 36, 30);
        boxOnly (434, 112, 56, XMOD_DEST, true, &xmodA, false, "XMOD dest {OSC1,PW,VCF}");
        boxOnly (496, 112, 44, XMOD_DPTH, false, nullptr, false, "XMOD depth 000..127", true);

        // ---- LFO / ENV3 ----
        colh ("", 8, 148, 38);
        colh ("dest1", 46, 148, 54);  colh ("amt", 104, 148, 28);
        colh ("dest2", 136, 148, 54); colh ("amt", 194, 148, 28);
        colh ("dest3", 226, 148, 54); colh ("amt", 284, 148, 28);
        colh ("rate",  316, 148, 30); colh ("wave", 350, 148, 42);
        struct LfoRow { const char* lab; int d1,a1,d2,a2,d3,a3,r,w; };
        static const LfoRow lfos[] = {
            { "lfo1", LFO1_DEST1, LFO1_DEPTH1, LFO1_DEST2, LFO1_DEPTH2, LFO1_DEST3, LFO1_DEPTH3, LFO1_RATE, LFO1_WAVSYNC },
            { "lfo2", LFO2_DEST1, LFO2_DEPTH1, LFO2_DEST2, LFO2_DEPTH2, LFO2_DEST3, LFO2_DEPTH3, LFO2_RATE, LFO2_WAVSYNC },
        };
        for (int i = 0; i < 2; ++i)
        {
            const LfoRow& L = lfos[(size_t) i];
            int y = 166 + i * 18;
            colh (L.lab, 8, y, 38);
            boxOnly (46, y, 54, L.d1, true, &destA);  boxOnly (104, y, 28, L.a1);
            boxOnly (136, y, 54, L.d2, true, &destA); boxOnly (194, y, 28, L.a2);
            boxOnly (226, y, 54, L.d3, true, &destA); boxOnly (284, y, 28, L.a3);
            boxOnly (316, y, 30, L.r);                boxOnly (350, y, 42, L.w, true, &lfoW);
        }
        colh ("env3", 8, 202, 38);
        boxOnly (46, 202, 54, ENV3_DEST1, true, &destA); boxOnly (104, 202, 28, ENV3_AMT1);
        boxOnly (136, 202, 54, ENV3_DEST2, true, &destA); boxOnly (194, 202, 28, ENV3_AMT2);
        boxOnly (226, 202, 54, ENV3_DEST3, true, &destA); boxOnly (284, 202, 28, ENV3_AMT3);

        // ---- MOD MATRIX ----
        colh ("dest1", 46, 220, 54);  colh ("amt", 104, 220, 28);
        colh ("dest2", 136, 220, 54); colh ("amt", 194, 220, 28);
        struct ModRow { const char* lab; int d1, a1, d2, a2; bool sgn; };
        static const ModRow mods[] = {
            { "modwhl",   MODW_D1, MODW_A1, MODW_D2, MODW_A2, false },
            { "dynamics", DYN_D1,  DYN_A1,  DYN_D2,  DYN_A2,  false },
            { "bender",   BEND_D1, BEND_A1, BEND_D2, BEND_A2, false },
            { "pressure", PRES_D1, PRES_A1, PRES_D2, PRES_A2, false },
            { "cont1",    C1_D1,   C1_A1,   C1_D2,   C1_A2,   false },
            { "cont2",    C2_D1,   C2_A1,   C2_D2,   C2_A2,   true  },
        };
        for (int i = 0; i < 6; ++i)
        {
            const ModRow& m = mods[(size_t) i];
            int y = 238 + i * 18;
            colh (m.lab, 8, y, 38);
            boxOnly (46, y, 54, m.d1, true, &destA);
            boxOnly (104, y, 28, m.a1, false, nullptr, m.sgn);
            boxOnly (136, y, 54, m.d2, true, &destA);
            boxOnly (194, y, 28, m.a2, false, nullptr, m.sgn);
        }

        // ================= ЦЕНТР: ENVELOPES + PER-VOICE PAN =================
        hdr ("ENVELOPES", 560, 4, 140);
        colh ("", 560, 22, 40);
        colh ("env1", 616, 22, 34); colh ("env2", 664, 22, 34); colh ("env3", 712, 22, 34);
        struct EnvRow { const char* lab; int o1, o2, o3; };
        static const EnvRow envs[] = {
            { "dyn",  DYN1,    DYN2,    DYN3    },
            { "dly",  DLY1,    DLY2,    DLY3    },
            { "Atk",  ENV1_ATK, ENV2_ATK, ENV3_ATK },
            { "Dec",  ENV1_DEC, ENV2_DEC, ENV3_DEC },
            { "Dk2",  ENV1_DK2, ENV2_DK2, ENV3_DK2 },
            { "Sus",  ENV1_SUS, ENV2_SUS, ENV3_SUS },
            { "Rel",  ENV1_REL, ENV2_REL, ENV3_REL },
        };
        for (int i = 0; i < 7; ++i)
        {
            const EnvRow& E = envs[(size_t) i];
            int y = 40 + i * 18;
            colh (E.lab, 560, y, 40);
            boxOnly (616, y, 34, E.o1); boxOnly (664, y, 34, E.o2); boxOnly (712, y, 34, E.o3);
        }

        hdr ("PER-VOICE PAN (8)", 560, 168, 160);
        colh ("", 560, 186, 40);
        colh ("pos", 600, 186, 40); colh ("rate", 664, 186, 34); colh ("dpth", 712, 186, 34);
        for (int v = 0; v < 8; ++v)
        {
            int y = 204 + v * 18;
            colh (juce::String (v + 1), 560, y, 40);
            boxOnly (600, y, 40, omega8::kArrPos + v, false, nullptr, true, "voice pan position (c64)");
            boxOnly (664, y, 34, omega8::kArrRate + v, false, nullptr, false, "voice pan rate");
            boxOnly (712, y, 34, omega8::kArrDepth + v, false, nullptr, false, "voice pan depth");
        }

        // ================= ПРАВО: GLIDE / PAN =================
        hdr ("GLIDE / PAN", 892, 4, 160);
        pair ("glide", 892, 22, GLIDE_FLAGS, true, &glideA, false, false,
              "GLIDE: bit 0x40 = on/off");
        cells.back().bitMask = 0x40;
        pair ("mode", 892, 40, GLIDE_FLAGS, true, &gmodeA, false, false,
              "GLIDE mode: bit 0x04 = REG/LEGAT");
        cells.back().bitMask = 0x04;
        pair ("time", 892, 58, GLIDE_TIME, false, nullptr, false, false, "GLIDE time");
        pair ("wave", 892, 94, PAN_WAVE, true, &panWA, false, false, "PAN wave");
        pair ("sync", 892, 112, PAN_SYNC, true, &syncA, false, false, "PAN sync");
        pair ("key",  892, 130, PAN_KEY, true, &panKA, false, false, "PAN key");
    }

    juce::String displayText (const Cell& c, int raw) const
    {
        if (c.fmt != nullptr) return c.fmt (raw);
        if (c.signedDisp)     return juce::String (raw - 64);
        if (c.threeDigit)     return juce::String (raw).paddedLeft ('0', 3);
        if (c.isEnum && c.choices != nullptr)
        {
            int idx = juce::jlimit (0, c.choices->size() - 1, raw);
            return c.choices->getReference (idx);
        }
        return juce::String (raw);
    }

    void paint (juce::Graphics& g) override
    {
        auto b = getLocalBounds().toFloat().reduced (1.5f);
        g.setColour (juce::Colour (0xff101d3e));
        g.fillRoundedRectangle (b, 4.0f);
        g.setColour (juce::Colour (0xff33509c));
        g.drawRoundedRectangle (b, 4.0f, 1.5f);

        g.setFont (juce::Font (11.0f, juce::Font::bold));
        for (auto& h : headers)
        {
            g.setColour (juce::Colour (0xff9db8e8));
            g.drawText (h.first, h.second, juce::Justification::centredLeft);
        }

        g.setFont (juce::Font (10.0f));
        for (auto& c : cells)
        {
            g.setColour (juce::Colour (0xffcfe0ff));
            g.drawText (c.label, c.lab, juce::Justification::centredLeft);
            if (c.box.isEmpty()) continue;
            g.setColour (juce::Colour (0xff1c3163));
            g.fillRoundedRectangle (c.box.toFloat(), 3.0f);
            g.setColour (juce::Colour (0xff4a66ae));
            g.drawRoundedRectangle (c.box.toFloat(), 3.0f, 1.0f);
            if (c.ofs < 0) continue;
            const int raw = c.bitMask != 0 ? ((cur[(size_t) c.ofs] & c.bitMask) ? 1 : 0)
                                           : cur[(size_t) c.ofs];
            if (c.wave) { paintWave (g, c, raw); continue; }
            g.setColour (juce::Colour (0xffe0333c));                       // red marker, как в ориге
            g.fillRect (c.box.getX() + 4, c.box.getCentreY() - 3, 5, 6);
            g.setColour (juce::Colours::white);
            const int tw = c.box.getWidth() - 12 - (c.isEnum ? 10 : 0);
            g.drawText (displayText (c, raw), c.box.getX() + 12, c.box.getY() + 1,
                        tw, c.box.getHeight() - 2, juce::Justification::centredLeft);
            if (c.isEnum)                                                  // ▼
            {
                juce::Path tri;
                tri.addTriangle ((float) c.box.getRight() - 11.0f, (float) c.box.getCentreY() - 2.0f,
                                 (float) c.box.getRight() - 3.0f,  (float) c.box.getCentreY() - 2.0f,
                                 (float) c.box.getRight() - 7.0f,  (float) c.box.getCentreY() + 3.0f);
                g.setColour (juce::Colour (0xff9db8e8));
                g.fillPath (tri);
            }
        }
    }

    void paintWave (juce::Graphics& g, const Cell& c, int raw) const
    {
        struct Ic { int kind; bool on; };   // kind: 0 tri, 1 saw, 2 pulse, 3 sine
        Ic ics[3];
        if (c.waveSrc == 0)
        {
            const int eff = raw == 0 ? 2 : raw;      // 0 -> saw (конвенция движка)
            ics[0] = { 0, (eff & 1) != 0 };
            ics[1] = { 1, (eff & 2) != 0 };
            ics[2] = { 2, (eff & 4) != 0 };
        }
        else if (c.waveSrc == 1)
        {
            ics[0] = { 0, raw == 1 };                // WAVE2: 1=tri, 2=saw, 0=pulse
            ics[1] = { 1, raw == 2 };
            ics[2] = { 2, raw == 0 };
        }
        else
        {
            ics[0] = { 2, raw == 1 || raw == 4 };    // SUB: pulse 1/4, sine 2, tri 3, saw 5
            ics[1] = { 3, raw == 2 };
            ics[2] = { 0, raw == 3 };
        }
        for (int i = 0; i < 3; ++i)
        {
            auto ir = juce::Rectangle<float> (c.box.getX() + 7.0f + i * 27.0f,
                                              c.box.getY() + 2.0f, 22.0f, c.box.getHeight() - 4.0f);
            g.setColour (ics[i].on ? juce::Colours::white
                                   : juce::Colours::white.withAlpha (0.22f));
            juce::Path p;
            switch (ics[i].kind)
            {
                case 0:
                    p.addTriangle (ir.getX(), ir.getBottom(), ir.getCentreX(), ir.getY(),
                                   ir.getRight(), ir.getBottom());
                    break;
                case 1:
                    p.startNewSubPath (ir.getX(), ir.getBottom());
                    p.lineTo (ir.getX(), ir.getY());
                    p.lineTo (ir.getRight(), ir.getBottom());
                    p.lineTo (ir.getX(), ir.getBottom());
                    break;
                case 3:
                    p.startNewSubPath (ir.getX(), ir.getCentreY());
                    // JUCE 8: addQuadCurveFrom удалён -> quadraticTo (контрольная точка, конец)
                    p.quadraticTo (ir.getX() + ir.getWidth() * 0.25f, ir.getY(),
                                    ir.getCentreX(), ir.getCentreY());
                    p.quadraticTo (ir.getRight() - ir.getWidth() * 0.25f, ir.getBottom(),
                                    ir.getRight(), ir.getCentreY());
                    break;
                default:
                    p.addRectangle (ir.getX(), ir.getY(), ir.getWidth() * 0.45f, ir.getHeight());
                    p.addRectangle (ir.getRight() - ir.getWidth() * 0.45f, ir.getY(),
                                    ir.getWidth() * 0.45f, ir.getHeight());
                    break;
            }
            g.strokePath (p, juce::PathStrokeType (1.4f));
        }
    }

    void sync (const omega8::Patch& p)
    {
        std::memcpy (cur, p.raw.data(), omega8::kPatchSize);
        repaint ();
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        const juce::Point<int> pos ((int) e.position.x, (int) e.position.y);
        if (e.mods.isRightButtonDown()) { openNumEdit (pos); return; }
        for (auto& c : cells)
        {
            if (c.wave && c.box.contains (pos))
            {
                for (int i = 0; i < 3; ++i)
                {
                    auto ir = juce::Rectangle<int> (c.box.getX() + 7 + i * 27, c.box.getY(), 26, c.box.getHeight());
                    if (ir.contains (pos))
                    {
                        const int raw = cur[(size_t) c.ofs];
                        if (c.waveSrc == 0)
                            setByte (c.ofs, (uint8_t) ((raw == 0 ? 2 : raw) ^ (1 << i)));
                        else
                        {
                            const int v1[3] = { 1, 2, 0 };   // WAVE2: tri/saw/pulse
                            const int v2[3] = { 1, 2, 3 };   // SUB: pulse/sine/tri
                            const int v = c.waveSrc == 1 ? v1[(size_t) i] : v2[(size_t) i];
                            setByte (c.ofs, (uint8_t) v);
                        }
                        return;
                    }
                }
                return;
            }
            if (c.ofs < 0 || ! c.box.contains (pos)) continue;
            if (c.isEnum)
            {
                if (c.choices == nullptr) return;
                juce::PopupMenu m;
                const int raw = c.bitMask != 0
                        ? ((cur[(size_t) c.ofs] & c.bitMask) ? 1 : 0)
                        : ((c.tick != nullptr) ? c.tick (cur[(size_t) c.ofs])
                                               : juce::jlimit (0, c.choices->size() - 1,
                                                               (int) cur[(size_t) c.ofs]));
                for (int j = 0; j < c.choices->size(); ++j)
                    m.addItem (j + 1, c.choices->getReference (j), true, j == raw);  // тик на текущем
                // JUCE 8, плагин: JUCE_MODAL_LOOPS_PERMITTED=0 -> синхронного show() НЕТ
                // (в JUCE 8 он под #if JUCE_MODAL_LOOPS_PERMITTED); для плагинов меню
                // показывает только showMenuAsync (Options, callback)
                m.showMenuAsync (juce::PopupMenu::Options ().withDeletionCheck (*this),
                                 [this, cc = c] (int id)
                {
                    if (id <= 0) return;
                    const int sel = id - 1;
                    if (cc.bitMask != 0)
                    {
                        const int v0 = (int) cur[(size_t) cc.ofs];
                        const int nv = sel ? (v0 | cc.bitMask) : (v0 & ~cc.bitMask);
                        if (nv != v0) setByte (cc.ofs, (uint8_t) nv);
                    }
                    else if (cc.enumAction != nullptr)
                        cc.enumAction (sel);
                    else if (cc.enumVals != nullptr && sel < cc.enumN)
                        setByte (cc.ofs, (uint8_t) cc.enumVals[(size_t) sel]);
                    else
                        setByte (cc.ofs, (uint8_t) sel);
                });
            }
            else
            {
                dragIdx = indexOf (c);
                dragStart = e.getPosition();
                dragVal0 = cur[(size_t) c.ofs];
            }
            return;
        }
    }

    int indexOf (const Cell& c) const
    {
        for (int i = 0; i < (int) cells.size(); ++i)
            if (&cells[(size_t) i] == &c) return i;
        return -1;
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        if (dragIdx < 0) return;
        Cell& c = cells[(size_t) dragIdx];
        int disp = dragVal0 + (e.x - dragStart.x) + (e.y - dragStart.y);
        disp = c.signedDisp ? juce::jlimit (-64, 63, disp) : juce::jlimit (0, 127, disp);
        const int raw = c.signedDisp ? disp + 64 : disp;
        if (raw != cur[(size_t) c.ofs]) setByte (c.ofs, (uint8_t) raw);
    }

    void mouseUp (const juce::MouseEvent&) override { dragIdx = -1; }

    void openNumEdit (const juce::Point<int>& pos)
    {
        for (int i = 0; i < (int) cells.size(); ++i)
        {
            const Cell& c = cells[(size_t) i];
            if (c.ofs < 0 || c.wave || c.isEnum || ! c.box.contains (pos)) continue;
            numEditIdx = i;
            numEdit.setBounds (c.box.expanded (6, 2));
            numEdit.setText (displayText (c, cur[(size_t) c.ofs]), false);  // JUCE 8: setText(String, bool)
            numEdit.setVisible (true);
            numEdit.grabKeyboardFocus ();
            numEdit.selectAll ();
            return;
        }
    }

    void mouseMove (const juce::MouseEvent& e) override
    {
        const juce::Point<int> pos ((int) e.position.x, (int) e.position.y);
        for (auto& c : cells)
            if ((c.box.contains (pos) || (c.lab.getWidth() > 0 && c.lab.contains (pos))))
            {
                setTooltip (c.tip.isNotEmpty() ? c.tip : c.label);
                return;
            }
        setTooltip ("");
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
        auxCardBox.addItemList (juce::StringArray { "OB-Xa 24", "OB-X 12", "TB-303" }, 1);
        auxCardBox.setTooltip ("Implemented AUX1 models (r16): OB-Xa 24 dB (TPT, AudioFilterOBXa reference), OB-X 12 dB (diode pair), TB-303 (Wurtz reference). These are implemented models of the optional card slot; the manual also lists 303, 2600, Juno/Jupiter, CS-80 (CS-80 is fixed in AUX2).");
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
        // ---- r18: extend-окно = LCD-матрица (текстовая карта, как в OMEGACODE) ----
        matrix = std::make_unique<LcdMatrix>();
        matrix->setByte = [this] (int ofs, uint8_t v) { proc.setPatchByte (proc.editSlot(), ofs, v); };
        matrix->buildCells ();
        for (auto& c : matrix->cells)
            if (c.ofs == omega8::off::FILT_TYPE)
                c.enumAction = [this] (int t) { setFilterTypeFromUI (t); };
        addAndMakeVisible (*matrix);
        matrix->setVisible (matrixOpen);

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

        addKnob ("CUTOFF",    omega8::off::CUTOFF);
        addKnob ("RESO",      omega8::off::RESO);
        addKnob ("TRACKING",  omega8::off::TRACKING);
        addKnob ("ENV1 AMT",  omega8::off::ENV1_AMT);

        addKnob ("LFO1 RATE", omega8::off::LFO1_RATE);
        addKnob ("LFO1 D1",   omega8::off::LFO1_DEPTH1);
        addKnob ("LFO2 RATE", omega8::off::LFO2_RATE);
        addKnob ("LFO2 D1",   omega8::off::LFO2_DEPTH1);
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
        // r11: явное значение без процентов/иероглифов; r17: в попапе видно
        // НАЗВАНИЕ параметра (жалоба «вместо названия параметры»).
        k->slider.textFromValueFunction = [t = title] (double v) { return t + "  " + juce::String ((int) v); };
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
        if (t == "OSC1 LEV") return "Oscillator 1 mix level.";
        if (t == "OSC2 LEV") return "Oscillator 2 mix level. Factory patches store level 0 and wave byte 64 (OSC2 off) - this is the original design; raise this knob to hear OSC2 (sub-osc stays on the SUB toggle).";
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
        auto name = proc.patchNameAt (proc.editSlot());
        int a = proc.presetA(), b = proc.presetB();
        float m = proc.morphAmount();
        // r11: строка1 — PATCH/BLEND как на железе; строка2 — имя, % BLEND
        // или ЗНАЧЕНИЕ последней ручки (1.5 с, как строка курсора на панели).
        juce::String line1, line2;
        // r17: статус загрузки банка на 4 c (раньше отказ молчаливый).
        if (proc.loadStatus != lcdStatusStr)
        {
            lcdStatusStr = proc.loadStatus;
            lcdStatusTime = juce::Time::getMillisecondCounter();
        }
        if ((juce::Time::getMillisecondCounter() - lcdStatusTime) < 4000)
        {
            line1 = "BANK STATUS";
            line2 = lcdStatusStr.length() > 24 ? lcdStatusStr.substring (0, 24) : lcdStatusStr;
        }
        else if (m > 0.001f && a != b)
        {
            line1 = juce::String::formatted ("BLEND: A%03d+B%03d", a + 1, b + 1);
        }
        else
        {
            line1 = juce::String::formatted ("PATCH A%03d  B%03d", a + 1, b + 1);
        }
        // во время показа статуса банка строку значения не трогаем
        if (line1 != "BANK STATUS")
        {
            if (juce::Time::getMillisecondCounter() < lastEditUntil)
                line2 = lastEdit + " " + juce::String (lastEditVal);      // число, без %/иероглифов
            else if (m > 0.001f && a != b)
                line2 = juce::String::formatted ("%s %3.0f%%", name.toRawUTF8(), m * 100.0f);
            else
                line2 = name;
        }
        lcdText = line1 + "\n" + line2;
        lcd.text = lcdText;
        lcd.repaint();
        // r18: LCD-матрица — живое обновление из патча
        if (matrixOpen && matrix != nullptr) matrix->sync (p);

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

        // r14: байт FILT_TYPE -> параметр хоста (панель/Ableton всегда в согласии)
        if (auto* fpp = proc.apvts.getParameter ("filtType"))
            if (auto* fpv = proc.apvts.getRawParameterValue ("filtType"))
            {
                const int want = p.at (omega8::off::FILT_TYPE);
                if ((int) fpv->load() != want)
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

    }

    void resized() override
    {

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
            // r17: LFO1 D2/D3 — в GLOBAL (в оригинале Depth-ручка = dest1,
            // dest2/3 редактируются Q-диалом в меню).

            auto l2 = c.removeFromTop (14);
            lfo2Dest.setText (destText (2), juce::dontSendNotification);
            lfo2Dest.setFont (juce::Font (8.5f));
            lfo2Dest.setColour (juce::Label::textColourId, juce::Colour (0xff9fb2c4));
            lfo2Dest.setTooltip ("LFO 2 destinations 1, 2 and 3. Full destination names are listed in docs/ENGLISH_LABELS_R15.md.");
            addAndMakeVisible (lfo2Dest);
            lfo2Dest.setBounds (l2);
            placeKnobs ({ "LFO2 RATE", "LFO2 D1" }, c.removeFromTop (66));
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
            // r17: FINE/TUNE/EXT IN/SUB — в GLOBAL: в оригинале SUB = тумблер
            // (subLed остался на панели), FINE/TUNE/EXT IN — параметры меню.
        }

        // ---- Filter: legible knobs, full English filter names, wide click targets ----
        {
            auto c = area.removeFromLeft (100).reduced (4, 4);
            placeKnobs ({ "CUTOFF", "RESO" }, c.removeFromTop (62));
            placeKnobs ({ "TRACKING" }, c.removeFromTop (62));
            // r17: HPF/HPR (CS-80 HP) — в GLOBAL: панель оригинала FILTER =
            // только Frequency/Resonate/Track.
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

        // ---- r18: extend-окно = оригинальная LCD-матрица (текстовая карта параметров) ----
        if (matrixOpen)
            matrix->setBounds (getLocalBounds().removeFromBottom (kExtH).reduced (8));
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
    juce::Label    typeLab { "", "TYPE" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> encAtt;
    // r11: строка значений на LCD после кручения ручки
    juce::String lastEdit;
    int          lastEditVal = 0;
    juce::int64  lastEditUntil = 0;
    juce::String lcdStatusStr;          // r17: строка статуса банка на LCD
    juce::int64  lcdStatusTime = 0;
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
    static constexpr int kExtH    = 380;       // r18: extend-окно = LCD-матрица
    juce::ComboBox arpPat2;   // reserved
    juce::Label lfo1Dest, lfo2Dest, env3Dest, morphLab { "", "BLEND  A - B" }, octLabel, tuneLabel;
    std::unique_ptr<LcdMatrix> matrix;   // r18: extend-окно = LCD-матрица
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
            // r18.1: setVisible отсутствовал -> окно расширялось, а матрица не
            // отображалась (конструктор вызывал setVisible(false) один раз)
            if (matrix != nullptr) matrix->setVisible (matrixOpen);
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
