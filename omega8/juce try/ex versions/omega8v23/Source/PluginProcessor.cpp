#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "OmegaBankData.h"   // вшитый фабричный банк A (generated)

namespace
{
constexpr const char* kAutoBankPaths[] = {
    "banks/Omega8FactoryA-CS.syx",
    "../banks/Omega8FactoryA-CS.syx",
    "../../banks/Omega8FactoryA-CS.syx",
    "../../../banks/Omega8FactoryA-CS.syx"
};
}

Omega8AudioProcessor::Omega8AudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withInput  ("Ext In", juce::AudioChannelSet::stereo(), true)
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "OMEGA8", createLayout())
{
    for (auto& p : bank) p = omega8::Patch::init();
    bankCount = 128;

    // embedded factory bank (generated header — имя символа не зависит от версии Projucer)
    {
        std::vector<uint8_t> v (omega8_bank::data, omega8_bank::data + omega8_bank::size);
        if (loadBankBytes (v)) return;
    }

    // best-effort auto-load of factory bank next to the binary / project
    const auto exeDir = juce::File::getSpecialLocation (juce::File::currentExecutableFile).getParentDirectory();
    const auto projDir = juce::File::getCurrentWorkingDirectory();
    for (const char* rel : kAutoBankPaths)
    {
        for (const auto& base : { exeDir, projDir })
        {
            auto f = base.getChildFile (juce::String (rel));
            if (f.existsAsFile()) { loadBankFile (f); return; }
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout Omega8AudioProcessor::createLayout()
{
    using P = juce::ParameterID;
    juce::AudioProcessorValueTreeState::ParameterLayout l;

    l.add (std::make_unique<juce::AudioParameterInt> (P { "presetA", 1 }, "Preset A (ROM/RAM)", 0, 127, 0));
    l.add (std::make_unique<juce::AudioParameterInt> (P { "presetB", 1 }, "Preset B (ROM/RAM)", 0, 127, 1));
    l.add (std::make_unique<juce::AudioParameterFloat> (P { "morph", 1 }, "Patch Morph A->B",
             juce::NormalisableRange<float> (0.0f, 1.0f), 0.0f));
    l.add (std::make_unique<juce::AudioParameterFloat> (P { "volume", 1 }, "Master Volume",
             juce::NormalisableRange<float> (0.0f, 1.0f), 0.8f));
    l.add (std::make_unique<juce::AudioParameterBool> (P { "arpOn", 1 }, "Arpeggiator", false));
    l.add (std::make_unique<juce::AudioParameterFloat> (P { "arpRate", 1 }, "Arp Rate",
             juce::NormalisableRange<float> (0.5f, 20.0f, 0.01f, 0.5f), 8.0f));
    l.add (std::make_unique<juce::AudioParameterChoice> (P { "arpPat", 1 }, "Arp Pattern",
             juce::StringArray { "Up", "Down", "Up-Down", "Random" }, 0));
    l.add (std::make_unique<juce::AudioParameterInt> (P { "arpOct", 1 }, "Arp Octaves", 1, 4, 1));
    // r9: карта в слоте AUX1 (конфиг железа оригинала — не байт патча)
    l.add (std::make_unique<juce::AudioParameterChoice> (P { "auxCard", 1 }, "AUX1 Card (implemented models)",
             juce::StringArray { "OB-Xa 24", "OB-X 12", "TB-303", "ARP2600", "CS-80", "WAH", "TS-808" }, 0));
    // r14: host-automatable TYPE; IDs 0..6 match the original filter table.
    l.add (std::make_unique<juce::AudioParameterChoice> (P { "filtType", 1 }, "Filter Type",
             juce::StringArray { "SEM LP", "SEM BP", "SEM HP", "SEM BR", "MINI LP", "AUX1", "AUX2" }, 0));
    return l;
}

float Omega8AudioProcessor::getParamValue (const juce::String& id) const
{
    if (auto* p = apvts.getRawParameterValue (id)) return p->load();
    return 0.0f;
}

void Omega8AudioProcessor::setCurrentProgram (int i)
{
    if (auto* p = apvts.getParameter ("presetA"))
    {
        p->beginChangeGesture();
        p->setValueNotifyingHost (p->convertTo0to1 ((float) i));
        p->endChangeGesture();
    }
}

const juce::String Omega8AudioProcessor::getProgramName (int i)
{
    return juce::String (i + 1) + " - " + patchNameAt (i);
}

bool Omega8AudioProcessor::loadBankFile (const juce::File& f)
{
    juce::MemoryBlock data;
    if (! f.loadFileAsData (data) || data.isEmpty()) return false;
    const auto* p = static_cast<const uint8_t*> (data.getData());
    std::vector<uint8_t> v (p, p + data.getSize());
    lastBankFile = f;
    return loadBankBytes (v);
}

bool Omega8AudioProcessor::loadBankBytes (const std::vector<uint8_t>& v)
{
    auto b = omega8::loadBank (v);
    if (! b.ok)
    {
        // r17: не «тихо молчим» — UI видит причину (LCD-строка).
        loadStatus = "LOAD FAILED: not an Omega 8 bank";
        loadStatusTime = juce::Time::getMillisecondCounter();
        return false;
    }

    juce::SpinLock::ScopedLockType guard (bankLock);   // r5: аудио читает bank под TryLock

    if (b.isMulti)
    {
        // r17: мульти-банк (128 x 80 байт): каждый слот = ПЕРВЫЙ активный part
        // (0x7F = пустой part) из ТЕКУЩЕГО банка — best effort: другие ROM-банки
        // в VST не загружаются. Имя/громкость/панорама — из part-записи.
        // Без активных parts — молчаливый плейсхолдер (уровни 0).
        for (int i = 0; i < 128; ++i)
        {
            omega8::Patch p;
            p.raw.fill (0);
            const auto& parts = b.multiParts[(size_t) i];
            int active = -1;
            for (int q = 0; q < 8; ++q)
                if (parts[(size_t) q].patch != 0x7F && parts[(size_t) q].patch < 128)
                { active = q; break; }

            if (active >= 0)
            {
                const omega8::Patch& src = bank[parts[(size_t) active].patch];  // до замены
                for (int k = 0; k < omega8::kParamBytes; ++k) p.raw[(size_t) k] = src.raw[(size_t) k];
                p.raw[omega8::off::VOLUME] = parts[(size_t) active].vol;
                for (int qv = 0; qv < 8; ++qv) p.raw[omega8::kArrPos + qv] = parts[(size_t) active].pan;
            }
            // иначе: p.raw остаётся нулями — плейсхолдер молчит (VOLUME = 0).
            // Имя — прямым memcpy: setName() делает raw.fill(' ') и зальёт
            // только что скопированные 160 байт параметров (quirk r5, см. init()).
            std::memset (&p.raw[omega8::kNameOffs], ' ', 16);
            const auto nm = b.multiNames[(size_t) i];
            const size_t nl = nm.size() < 16 ? nm.size() : 16;
            if (nl > 0) std::memcpy (&p.raw[omega8::kNameOffs], nm.data(), nl);
            bank[i] = p;
        }
        bankCount = 128;
        loadStatus = "MULTI: first part per slot (8-part ref)";
        loadStatusTime = juce::Time::getMillisecondCounter();
        return true;
    }

    for (auto& p : bank) p = omega8::Patch::init();
    bankCount = juce::jmin (128, (int) b.patches.size());
    for (int i = 0; i < bankCount; ++i) bank[i] = b.patches[(size_t) i];
    loadStatus = "BANK LOADED: " + std::to_string (bankCount) + " patches";
    loadStatusTime = juce::Time::getMillisecondCounter();
    return true;   // engine подхватит на следующем блоке (refresh в processBlock)
}

juce::String Omega8AudioProcessor::patchNameAt (int idx) const
{
    if (idx < 0 || idx >= 128) return {};
    juce::SpinLock::ScopedLockType guard (bankLock);
    return juce::String (bank[idx].name());
}

int Omega8AudioProcessor::editSlot() const
{
    return juce::jlimit (0, 127, morphAmount() <= 0.5f ? presetA() : presetB());
}

void Omega8AudioProcessor::setPatchByte (int patchIdx, int ofs, uint8_t v)
{
    if (ofs < 0 || ofs >= omega8::kPatchSize) return;
    juce::SpinLock::ScopedLockType guard (bankLock);
    bank[juce::jlimit (0, 127, patchIdx)].raw[(size_t) ofs] = v;
    // звук обновится в processBlock через refreshPatchFromParams
}

omega8::Patch Omega8AudioProcessor::getEditPatch()
{
    juce::SpinLock::ScopedLockType guard (bankLock);
    return bank[editSlot()];
}

// r18.2: RESET ALL
void Omega8AudioProcessor::resetAll()
{
    if (auto* mp = apvts.getParameter ("morph"))
        mp->setValueNotifyingHost (0.0f);
    arp = Arp();                    // arp-состояние в ноль
    curBend = curPressure = curCont1 = curCont2 = 0.0f;
    omega8::Patch a;
    {
        juce::SpinLock::ScopedLockType guard (bankLock);
        a = bank[(size_t) juce::jlimit (0, 127, presetA())];
    }
    {
        juce::SpinLock::ScopedLockType guard (bankLock);
        engine.clearVoices();
        engine.setPatch (a);        // свежий push; аудио-поток подхватит morph=0 сам
    }
}

void Omega8AudioProcessor::refreshPatchFromParams()
{
    juce::SpinLock::ScopedTryLockType guard (bankLock);
    if (! guard.isLocked()) return;   // аудиопоток занят записью из UI — возьмём следующий блок

    int a = (int) getParamValue ("presetA");
    int b = (int) getParamValue ("presetB");
    float t = getParamValue ("morph");
    // r14: параметр Filter Type -> байт текущего редактируемого патча
    // (первое чтение лишь калибрует lastFiltParam — не затираем байты банка!)
    int ft = (int) getParamValue ("filtType");
    if (lastFiltParam < 0)
        lastFiltParam = ft;
    else if (ft != lastFiltParam)
    {
        auto& slot = bank[juce::jlimit (0, 127, editSlot())];
        if (ft != slot.raw[omega8::off::FILT_TYPE])
            slot.raw[omega8::off::FILT_TYPE] = (uint8_t) ft;
    }
    lastFiltParam = ft;
    // r16: BLEND живой — лерп в Patch::lerp (enum-байты снап на 50%), движок
    // сам ставит кроссфейд 30 мс + сброс фильтров на enum-скачки. Смена A/B
    // без морфа = «прыжок» — тот же путь, сглаженный кроссфейдом (на железе
    // при смене пресета тоже «незначительно пукает»).
    auto pa = bank[juce::jlimit (0, 127, a)];
    auto pb = bank[juce::jlimit (0, 127, b)];
    engine.setPatch (omega8::Patch::lerp (pa, pb, t));
}

void Omega8AudioProcessor::prepareToPlay (double sampleRate, int)
{
    engine.prepare (sampleRate);
    refreshPatchFromParams();
}

bool Omega8AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto out = layouts.getMainOutputChannelSet();
    if (out != juce::AudioChannelSet::stereo() && out != juce::AudioChannelSet::mono())
        return false;
    const auto in = layouts.getMainInputChannelSet();
    return in == juce::AudioChannelSet::stereo() || in == juce::AudioChannelSet::mono()
        || in == juce::AudioChannelSet::disabled();
}

void Omega8AudioProcessor::handleArp (double dt, juce::MidiBuffer* /*midiOut*/)
{
    if (! arp.on)
    {
        // r11: held НЕ чистим — клавиши продолжают отслеживаться, чтобы
        // START/STOP запускал арп сразу по уже зажатым нотам.
        if (arp.lastNote >= 0) { engine.noteOff (arp.lastNote); arp.lastNote = -1; }
        return;
    }
    if (arp.held.empty())
    {
        if (arp.lastNote >= 0) { engine.noteOff (arp.lastNote); arp.lastNote = -1; }
        return;
    }

    auto sorted = arp.held;
    std::sort (sorted.begin(), sorted.end());

    arp.phase += dt;
    if (arp.phase < arp.interval) return;
    arp.phase -= arp.interval;

    if (arp.lastNote >= 0) engine.noteOff (arp.lastNote);

    int n = (int) sorted.size() * arp.octaves;
    int k = juce::jlimit (0, n - 1, arp.idx);
    int noteBase = sorted[(size_t) (k % (int) sorted.size())];
    int oct = k / (int) sorted.size();
    int note = juce::jlimit (0, 127, noteBase + 12 * oct);

    // pattern movement
    switch (arp.pattern)
    {
        case 0: arp.idx = (k + 1) % n; break;                       // up
        case 1: arp.idx = (k - 1 + n) % n; break;                   // down
        case 2:                                                      // up-down
            if (arp.dir > 0 && k + 1 >= n) { arp.dir = -1; arp.idx = k - 1; }
            else if (arp.dir < 0 && k - 1 < 0) { arp.dir = 1; arp.idx = k + 1; }
            else arp.idx = k + arp.dir;
            break;
        default: arp.idx = arp.rnd.nextInt (n); break;              // random
    }

    arp.lastNote = note;
    engine.noteOn (note, 0.85f);
}

void Omega8AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;

    refreshPatchFromParams();

    const int n = buffer.getNumSamples();

    // --- MIDI events (applied at block start; arp notes generated per block) ---
    for (const auto meta : midi)
    {
        const auto msg = meta.getMessage();
        if (msg.isNoteOn())
        {
            // r11: зажатые клавиши отслеживаем ВСЕГДА (для мгновенного старта арп)
            if (std::find (arp.held.begin(), arp.held.end(), msg.getNoteNumber()) == arp.held.end())
                arp.held.push_back (msg.getNoteNumber());
            if (arp.on)
            {
                arp.idx = 0;
            }
            else if (multiActive())
            {
                // r18.7: MULTI — активные части звучат вместе (layer)
                juce::SpinLock::ScopedTryLockType g (bankLock);
                for (int qi = 0; qi < 8; ++qi)
                    if (multi[(size_t) qi].on)
                        engine.noteOnPart (msg.getNoteNumber(), msg.getFloatVelocity(),
                                           bank[(size_t) (multi[(size_t) qi].patch & 0x7F)],
                                           multi[(size_t) qi].vol, multi[(size_t) qi].pan, qi);
            }
            else
            {
                engine.noteOn (msg.getNoteNumber(), msg.getFloatVelocity());
            }
        }
        else if (msg.isNoteOff())
        {
            arp.held.erase (std::remove (arp.held.begin(), arp.held.end(), msg.getNoteNumber()), arp.held.end());  // r11: всегда
            if (! arp.on) engine.noteOff (msg.getNoteNumber());
        }
        else if (msg.isPitchWheel())
        {
            // MIDI pitch wheel is unsigned 0..16383 with centre 8192; subtract centre first.
            // The old value/8192 mapping treated centre as +1 and detuned every note by +2 semitones.
            const int wheel = msg.getPitchWheelValue();
            curBend = juce::jlimit (-1.0f, 1.0f, (wheel - 8192) / 8192.0f);
        }
        else if (msg.isAftertouch() || msg.isChannelPressure())
            curPressure = (msg.getRawDataSize() > 1 ? msg.getRawData()[1] : 0) / 127.0f;
        else if (msg.isController())
        {
            switch (msg.getControllerNumber())
            {
                case 1:  curCont1 = msg.getControllerValue() / 127.0f; break; // modwheel
                case 16: curCont1 = msg.getControllerValue() / 127.0f; break;
                case 17: curCont2 = msg.getControllerValue() / 127.0f; break;
                // r17: vendor-маппинг (в мануале не описан — best effort,
                // см. docs/CHANGELOG_R17.md): CC98 = BLEND-глубина, CC99 = выбор
                // пресета A (0..127). raw-параметры — atomic store, безопасно из
                // аудио-потока.
                case 98: apvts.getRawParameterValue ("morph")->store (msg.getControllerValue() / 127.0f); break;
                case 99: apvts.getRawParameterValue ("presetA")->store (msg.getControllerValue()); break;
                default: break;
            }
        }
    }

    engine.modWheel = curCont1;
    engine.bend     = curBend;
    engine.pressure = curPressure;
    engine.cont1    = curCont1;
    engine.cont2    = curCont2;

    // r18.6: MW=BLEND (ячейка матрицы "mw=bl", UNK36 bit0, галка из орига):
    // modwheel управляет BLEND (morph) вместо mod-dest.
    if (engine.patch.raw[omega8::off::UNK36] & 0x01)
        apvts.getRawParameterValue ("morph")->store (curCont1);

    // --- arpeggiator parameters ---
    engine.auxCard.store ((int) rawParam ("auxCard"));   // r9: карта AUX1
    {
        const bool onNow = rawParam ("arpOn") > 0.5f;
        if (onNow && ! arp.on && ! arp.held.empty())
            arp.phase = arp.interval;      // r11: START со зжатыми клавишами — первый шаг сразу
        arp.on = onNow;
    }
    arp.pattern  = (int) rawParam ("arpPat");
    arp.octaves  = juce::jlimit (1, 4, (int) rawParam ("arpOct"));
    double rate  = (double) rawParam ("arpRate");       // steps per second
    arp.interval = 1.0 / juce::jmax (0.05, rate);

    handleArp (n / engine.sr, nullptr);

    // --- synth render ---
    const float vol = getParamValue ("volume");
    engine.masterGain = vol * vol * 2.0f;

    const bool hasInput = getBusCount (true) > 0 && getBus (true, 0)->getNumberOfChannels() > 0;
    const float* inL = hasInput ? buffer.getReadPointer (0) : nullptr;
    const float* inR = (hasInput && buffer.getNumChannels() > 1 && getBus (true, 0)->getNumberOfChannels() > 1)
                         ? buffer.getReadPointer (1) : nullptr;

    if (buffer.getNumChannels() >= 2)
        engine.process (buffer.getWritePointer (0), buffer.getWritePointer (1), n, inL, inR);
    else if (buffer.getNumChannels() == 1)
    {
        scratchL.resize ((size_t) n);
        scratchR.resize ((size_t) n);
        engine.process (scratchL.data(), scratchR.data(), n, inL, inL);
        auto* dst = buffer.getWritePointer (0);
        for (int i = 0; i < n; ++i)
            dst[i] = 0.5f * (scratchL[(size_t) i] + scratchR[(size_t) i]);
    }
}

juce::AudioProcessorEditor* Omega8AudioProcessor::createEditor()
{
    return new Omega8AudioProcessorEditor (*this);
}

void Omega8AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto xml = apvts.copyState().createXml();
    xml->setAttribute ("bankPath", lastBankFile.getFullPathName());
    copyXmlToBinary (*xml, destData);
}

void Omega8AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
        auto path = xml->getStringAttribute ("bankPath");
        if (path.isNotEmpty() && juce::File (path).existsAsFile())
            loadBankFile (juce::File (path));
        // engine подхватит bank/параметры в processBlock (без вызова setPatch из UI-потока)
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Omega8AudioProcessor();
}
